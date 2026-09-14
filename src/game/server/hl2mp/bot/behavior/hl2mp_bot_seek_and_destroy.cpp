//========= Copyright Valve Corporation, All rights reserved. ============//

#include "cbase.h"
#include "hl2mp_player.h"
#include "hl2mp_gamerules.h"
#include "team_control_point_master.h"
#include "bot/hl2mp_bot.h"
#include "bot/behavior/hl2mp_bot_attack.h"
#include "bot/behavior/hl2mp_bot_seek_and_destroy.h"
#include "bot/behavior/hl2mp_bot_get_prop.h"
#include "nav_mesh.h"

#ifdef DODS_REMAKE
ConVar dods_bot_debug_objectives( "dods_bot_debug_objectives", "0", FCVAR_GAMEDLL, "Log bot objective selections and navigation failures." );
extern int DODSBotCaptureOrder( CBasePlayer *player, CBaseEntity *candidate );
extern bool DODSBotCaptureGoal( CBasePlayer *player, CBaseEntity *candidate, Vector &goal, bool &inside );
#endif
extern ConVar hl2mp_bot_path_lookahead_range;
extern ConVar hl2mp_bot_offense_must_push_time;
extern ConVar hl2mp_bot_defense_must_defend_time;

ConVar hl2mp_bot_debug_seek_and_destroy( "hl2mp_bot_debug_seek_and_destroy", "0", FCVAR_CHEAT );
ConVar hl2mp_bot_disable_seek_and_destroy( "hl2mp_bot_disable_seek_and_destroy", "0", FCVAR_CHEAT );


//---------------------------------------------------------------------------------------------
CHL2MPBotSeekAndDestroy::CHL2MPBotSeekAndDestroy( float duration )
{
	if ( duration > 0.0f )
	{
		m_giveUpTimer.Start( duration );
	}
}


//---------------------------------------------------------------------------------------------
ActionResult< CHL2MPBot >	CHL2MPBotSeekAndDestroy::OnStart( CHL2MPBot *me, Action< CHL2MPBot > *priorAction )
{
	m_path.SetMinLookAheadDistance( me->GetDesiredPathLookAheadRange() );

	RecomputeSeekPath( me );

	// restart the timer if we have one
	if ( m_giveUpTimer.HasStarted() )
	{
		m_giveUpTimer.Reset();
	}

	if ( hl2mp_bot_disable_seek_and_destroy.GetBool() )
	{
		return Done( "Disabled." );
	}

	return Continue();
}


//---------------------------------------------------------------------------------------------
ActionResult< CHL2MPBot >	CHL2MPBotSeekAndDestroy::Update( CHL2MPBot *me, float interval )
{
#ifdef DODS_REMAKE
	if ( HL2MPRules()->IsDODSRoundOver() ) { m_path.Invalidate(); return Continue(); }
	if ( m_dodsRetry.IsElapsed() )
	{
		m_dodsFailedGoals.RemoveAll();
		m_dodsRetry.Start( 10.0f );
	}
	Vector captureGoal;
	bool inside = false;
	bool validGoal = m_dodsGoal && DODSBotCaptureGoal( me, m_dodsGoal, captureGoal, inside );
	if ( !validGoal || ( !inside && m_dodsSelect.IsElapsed() ) )
	{
		EHANDLE previousGoal = m_dodsGoal;
		m_dodsSelect.Start( 1.0f );
		m_dodsGoal = NULL;
		float best = FLT_MAX;
		int bestOrder = INT_MAX;
		for ( CBaseEntity *ent = gEntList.FindEntityByClassname( NULL, "dod_capture_area" ); ent;
			ent = gEntList.FindEntityByClassname( ent, "dod_capture_area" ) )
		{
			if ( m_dodsFailedGoals.Find( EHANDLE( ent ) ) != m_dodsFailedGoals.InvalidIndex() ) continue;
			Vector goal; bool touching;
			if ( !DODSBotCaptureGoal( me, ent, goal, touching ) ) continue;
			float distance = touching ? 0 : ( goal - me->GetAbsOrigin() ).LengthSqr();
			int order = DODSBotCaptureOrder( me, ent );
			if ( order < bestOrder || ( order == bestOrder && distance < best ) )
			{
				bestOrder = order; best = distance;
				m_dodsGoal = ent; captureGoal = goal; inside = touching;
			}
		}
		if ( previousGoal != m_dodsGoal )
		{
			m_path.Invalidate();
			m_dodsRepath.Invalidate();
			if ( dods_bot_debug_objectives.GetBool() ) Msg( "DODS bot %d team %d: target %s\n", me->entindex(), me->GetTeamNumber(), m_dodsGoal ? m_dodsGoal->GetDebugName() : "none reachable (check nav mesh)" );
		}
	}
	if ( m_dodsGoal )
	{
		if ( inside ) { m_path.Invalidate(); return Continue(); }
		if ( !m_path.IsValid() || m_dodsRepath.IsElapsed() )
		{
			m_dodsRepath.Start( 1.0f );
			CNavArea *area = TheNavMesh->GetNearestNavArea( captureGoal );
			Vector destination = captureGoal;
			if ( area ) area->GetClosestPointOnArea( captureGoal, &destination );
			if ( me->IsRangeLessThan( destination, 24.0f ) ) destination = captureGoal;
			CHL2MPBotPathCost cost( me, FASTEST_ROUTE );
			if ( !m_path.Compute( me, destination, cost ) || m_path.GetResult() != Path::COMPLETE_PATH )
			{
				if ( m_dodsFailedGoals.Find( m_dodsGoal ) == m_dodsFailedGoals.InvalidIndex() ) m_dodsFailedGoals.AddToTail( m_dodsGoal );
				if ( dods_bot_debug_objectives.GetBool() ) Msg( "DODS bot %d team %d: cannot reach %s from %.0f %.0f %.0f; trying other objectives.\n", me->entindex(), me->GetTeamNumber(), m_dodsGoal->GetDebugName(), me->GetAbsOrigin().x, me->GetAbsOrigin().y, me->GetAbsOrigin().z );
				m_dodsGoal = NULL; m_path.Invalidate();
				return Continue();
			}
		}
		m_path.Update( me );
		return Continue();
	}
#endif
	if ( m_giveUpTimer.HasStarted() && m_giveUpTimer.IsElapsed() )
	{
		return Done( "Behavior duration elapsed" );
	}

	if ( hl2mp_bot_disable_seek_and_destroy.GetBool() )
	{
		return Done( "Disabled." );
	}


	const CKnownEntity *threat = me->GetVisionInterface()->GetPrimaryKnownThreat();

	bool bShouldAttack = threat != NULL;

	if ( me->IsPropFreak() )
	{
		// Prop freaks should only attack with a prop!
		bShouldAttack &= me->Physcannon_GetHeldProp() != NULL;
	}

	if ( bShouldAttack )
	{
		const float engageRange = 1000.0f;
		if ( me->IsRangeLessThan( threat->GetLastKnownPosition(), engageRange ) )
		{
			return SuspendFor( new CHL2MPBotAttack, "Going after an enemy" );
		}
	}
	else if ( !me->IsPropHater() )
	{
		bool wantsProp = me->Physcannon_GetHeldProp() == NULL;
		if ( wantsProp && CHL2MPBotGetProp::IsPossible( me ) )
		{
			return SuspendFor( new CHL2MPBotGetProp, "Grabbing prop" );
		}
	}

	// move towards our seek goal
	m_path.Update( me );

	m_bTimerElapsed = m_repathTimer.HasStarted() && m_repathTimer.IsElapsed();

	if ( m_bGoingToTargetEntity )
	{
		bool bEntityVisible = false;
		if ( m_hTargetEntity )
		{
			bEntityVisible = true;

			CBaseCombatWeapon* pWeapon = dynamic_cast<CBaseCombatWeapon*>( m_hTargetEntity.Get() );
			if ( pWeapon )
			{
				if ( pWeapon->IsEffectActive( EF_NODRAW ) )
					bEntityVisible = false;

				if ( pWeapon->GetOwner() != NULL )
					bEntityVisible = false;

				// I don't want it anymore.
				if ( me->Weapon_OwnsThisType( pWeapon->GetClassname() ) )
					bEntityVisible = false;
			}
		}

		// If I can see the goal, and the entity isn't visible, then 
		if ( me->IsLineOfSightClear( m_vGoalPos ) && !bEntityVisible )
		{
			// Keep looking for a couple seconds.
			if ( !m_itemStolenTimer.HasStarted() )
				m_itemStolenTimer.Start( 2.0f );

			if ( m_itemStolenTimer.HasStarted() && m_itemStolenTimer.IsElapsed() )
			{
				m_itemStolenTimer.Reset();
				m_path.Invalidate();
			}
		}
	}

	if ( !m_path.IsValid() )
	{
		m_repathTimer.Start( 45.0f );

		RecomputeSeekPath( me );
	}

	return Continue();
}


//---------------------------------------------------------------------------------------------
ActionResult< CHL2MPBot > CHL2MPBotSeekAndDestroy::OnResume( CHL2MPBot *me, Action< CHL2MPBot > *interruptingAction )
{
	RecomputeSeekPath( me );

	return Continue();
}


//---------------------------------------------------------------------------------------------
EventDesiredResult< CHL2MPBot > CHL2MPBotSeekAndDestroy::OnStuck( CHL2MPBot *me )
{
#ifdef DODS_REMAKE
	if ( m_dodsGoal )
	{
		if ( m_dodsFailedGoals.Find( m_dodsGoal ) == m_dodsFailedGoals.InvalidIndex() ) m_dodsFailedGoals.AddToTail( m_dodsGoal );
				if ( dods_bot_debug_objectives.GetBool() ) Msg( "DODS bot %d team %d: cannot reach %s from %.0f %.0f %.0f; trying other objectives.\n", me->entindex(), me->GetTeamNumber(), m_dodsGoal->GetDebugName(), me->GetAbsOrigin().x, me->GetAbsOrigin().y, me->GetAbsOrigin().z );
		m_dodsGoal = NULL; m_path.Invalidate();
		me->GetLocomotionInterface()->ClearStuckStatus();
		return TryContinue();
	}
#endif
	RecomputeSeekPath( me );

	return TryContinue();
}


//---------------------------------------------------------------------------------------------
EventDesiredResult< CHL2MPBot > CHL2MPBotSeekAndDestroy::OnMoveToSuccess( CHL2MPBot *me, const Path *path )
{
	RecomputeSeekPath( me );

	return TryContinue();
}


//---------------------------------------------------------------------------------------------
EventDesiredResult< CHL2MPBot > CHL2MPBotSeekAndDestroy::OnMoveToFailure( CHL2MPBot *me, const Path *path, MoveToFailureType reason )
{
	RecomputeSeekPath( me );

	return TryContinue();
}


//---------------------------------------------------------------------------------------------
QueryResultType	CHL2MPBotSeekAndDestroy::ShouldRetreat( const INextBot *meBot ) const
{
#ifdef DODS_REMAKE
	if ( m_dodsGoal ) return ANSWER_NO;
#endif
	return ANSWER_UNDEFINED;
}


//---------------------------------------------------------------------------------------------
QueryResultType CHL2MPBotSeekAndDestroy::ShouldHurry( const INextBot *me ) const
{
#ifdef DODS_REMAKE
	if ( m_dodsGoal ) return ANSWER_YES;
#endif
	return ANSWER_UNDEFINED;
}

class CNextSpawnFilter : public IEntityFindFilter
{
public:
	CNextSpawnFilter( EHANDLE hPlayer, float flRange )
		: m_hPlayer{ hPlayer }
		, m_flRange{ flRange }
	{
		
	}

	bool ShouldFindEntity( CBaseEntity *pEntity )
	{
		// If we find no truly valid marks, we'll just use the first.
		if ( !m_hEntityFound.Get() )
		{
			m_hEntityFound = pEntity;
		}

		if ( pEntity->GetAbsOrigin().DistToSqr( m_hPlayer->GetAbsOrigin() ) < ( m_flRange * m_flRange ) )
		{
			return false;
		}

		m_hEntityFound = pEntity;
		return true;
	}

	CBaseEntity *GetFilterResult( void )
	{
		return m_hEntityFound;
	}

private:
	EHANDLE		m_hPlayer;

	float		m_flRange;

	// To maintain backwards compatability, store off the first mark
	// we find. If we find no truly valid marks, we'll just use the first.
	EHANDLE		m_hEntityFound;
};

class CNotOwnedWeaponFilter : public IEntityFindFilter
{
public:
	CNotOwnedWeaponFilter( CBasePlayer *pPlayer )
		: m_hPlayer{ pPlayer }
	{
		
	}

	bool ShouldFindEntity( CBaseEntity *pEntity )
	{
		// If we find no truly valid marks, we'll just use the first.
		if ( !m_hEntityFound.Get() )
		{
			m_hEntityFound = pEntity;
		}

		CBaseCombatWeapon *pWeapon = dynamic_cast< CBaseCombatWeapon *>( pEntity );
		if ( !pWeapon )
			return false;

		if ( pWeapon->GetOwner() )
			return false;

		// ignore non-existent ammo to ensure we collect nearby existing ammo
		if ( pWeapon->IsEffectActive( EF_NODRAW ) )
			return false;

		if ( m_hPlayer->Weapon_OwnsThisType( pEntity->GetClassname() ) )
			return false;
		
		m_hEntityFound = pEntity;
		return true;
	}

	CBaseEntity *GetFilterResult( void )
	{
		return m_hEntityFound;
	}

private:
	CHandle<CBasePlayer>	m_hPlayer;

	// To maintain backwards compatability, store off the first mark
	// we find. If we find no truly valid marks, we'll just use the first.
	EHANDLE		m_hEntityFound;
};


//---------------------------------------------------------------------------------------------
void CHL2MPBotSeekAndDestroy::RecomputeSeekPath( CHL2MPBot *me )
{
#ifdef DODS_REMAKE
	if ( m_dodsGoal ) { m_path.Invalidate(); return; }
#endif
	if ( m_bOverrideApproach )
	{
		return;
	}

	m_hTargetEntity = NULL;
	m_bGoingToTargetEntity = false;
	m_vGoalPos = vec3_origin;

	if ( !TheNavAreas.Size() )
	{
		m_path.Invalidate();
		return;
	}

	// Don't try to find weapons if the timer elapsed. Probably went bad?
	if ( !m_bTimerElapsed && !me->IsPropFreak() )
	{
		CUtlVector<CBaseEntity*> pWeapons;

		CNotOwnedWeaponFilter weaponFilter( me );
		CBaseEntity* pSearch = NULL;
		while ( ( pSearch = gEntList.FindEntityByClassname( pSearch, "weapon_*", &weaponFilter ) ) != NULL )
		{
			if ( pSearch )
				pWeapons.AddToTail( pSearch );
		}

		pWeapons.SortPredicate(
			[&]( CBaseEntity* a, CBaseEntity* b )
			{
				float flDistA = me->GetAbsOrigin().DistToSqr( a->GetAbsOrigin() );
				float flDistB = me->GetAbsOrigin().DistToSqr( b->GetAbsOrigin() );

				return flDistA < flDistB;
			}
		);


		// Try and find weapons we don't have above all else on the map.
		for ( int i = 0; i < pWeapons.Size(); i++ )
		{
			CBaseEntity* pClosestWeapon = pWeapons[i];
			if ( pClosestWeapon )
			{
				CHL2MPBotPathCost cost( me, SAFEST_ROUTE );
				m_hTargetEntity = pClosestWeapon;
				m_bGoingToTargetEntity = true;
				m_vGoalPos = pClosestWeapon->WorldSpaceCenter();
				if ( m_path.Compute( me, m_vGoalPos, cost, 0.0f, true, true ) && m_path.IsValid() && m_path.GetResult() == Path::COMPLETE_PATH )
					return;
			}
		}
	}

	// Fallback and roam random spawn points if we have all weapons.
	{
		CNextSpawnFilter spawnFilter( me, 128.0f );

		CUtlVector<CBaseEntity*> pSpawns;

		CBaseEntity* pSearch = NULL;
		while ( ( pSearch = gEntList.FindEntityByClassname( pSearch, "info_player_*", &spawnFilter ) ) != NULL )
		{
			if ( pSearch )
				pSpawns.AddToTail( pSearch );
		}

		// Don't wander between spawns if there aren't that many.
		if ( pSpawns.Size() >= 3 )
		{
			for ( int i = 0; i < 10; i++ )
			{
				CHL2MPBotPathCost cost( me, SAFEST_ROUTE );
				m_hTargetEntity = pSpawns[RandomInt( 0, pSpawns.Size() - 1 )];
				m_bGoingToTargetEntity = true;
				m_vGoalPos = m_hTargetEntity->WorldSpaceCenter();
				if ( m_path.Compute( me, m_vGoalPos, cost, 0.0f, true, true ) && m_path.IsValid() && m_path.GetResult() == Path::COMPLETE_PATH )
					return;
			}
		}
	}

	for ( int i = 0; i < 10; i++ )
	{
		// No spawns we can get to? Just wander... somewhere!

		CHL2MPBotPathCost cost( me, SAFEST_ROUTE );
		Vector vWanderPoint = TheNavAreas[RandomInt( 0, TheNavAreas.Size() - 1 )]->GetCenter();
		m_vGoalPos = vWanderPoint;
		if ( m_path.Compute( me, vWanderPoint, cost ) )
			return;
	}

	m_path.Invalidate();
}


//---------------------------------------------------------------------------------------------
EventDesiredResult< CHL2MPBot > CHL2MPBotSeekAndDestroy::OnTerritoryContested( CHL2MPBot *me, int territoryID )
{
	return TryDone( RESULT_IMPORTANT, "Defending the point" );
}


//---------------------------------------------------------------------------------------------
EventDesiredResult< CHL2MPBot > CHL2MPBotSeekAndDestroy::OnTerritoryCaptured( CHL2MPBot *me, int territoryID )
{
	return TryDone( RESULT_IMPORTANT, "Giving up due to point capture" );
}


//---------------------------------------------------------------------------------------------
EventDesiredResult< CHL2MPBot > CHL2MPBotSeekAndDestroy::OnTerritoryLost( CHL2MPBot *me, int territoryID )
{
	return TryDone( RESULT_IMPORTANT, "Giving up due to point lost" );
}


//---------------------------------------------------------------------------------------------
EventDesiredResult< CHL2MPBot > CHL2MPBotSeekAndDestroy::OnCommandApproach( CHL2MPBot* me, const Vector& pos, float range )
{
	m_bOverrideApproach = true;
	m_vOverrideApproach = pos;

	CHL2MPBotPathCost cost( me, SAFEST_ROUTE );
	m_path.Compute( me, m_vOverrideApproach, cost );

	return TryContinue();
}
