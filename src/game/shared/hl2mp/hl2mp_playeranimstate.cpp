//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Player animations for Half-Life 2: Deathmatch
//
//=============================================================================//

#include "cbase.h"
#include "datacache/imdlcache.h"
#include "base_playeranimstate.h"
#include "hl2mp_playeranimstate.h"
#include "dods/weapons/dods_weaponbase.h"

#ifdef CLIENT_DLL
#include "c_hl2mp_player.h"
#else
#include "hl2mp_player.h"
#endif

#define HL2MP_RUN_SPEED				320.0f
#define HL2MP_WALK_SPEED			75.0f
#define HL2MP_CROUCHWALK_SPEED		110.0f

extern ConVar anim_showmainactivity;
extern ConVar mp_showgestureslots;

#define DODS_RUN_SPEED			120.0f
#define DODS_WALK_SPEED			60.0f
#define DODS_SPRINT_SPEED		260.0f

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *pPlayer - 
// Output : CMultiPlayerAnimState*
//-----------------------------------------------------------------------------
CHL2MPPlayerAnimState *CreateHL2MPPlayerAnimState( CHL2MP_Player *pPlayer )
{
	MDLCACHE_CRITICAL_SECTION();

	MultiPlayerMovementData_t movementData;

	movementData.m_flBodyYawRate = 720.0f;
	movementData.m_flRunSpeed = DODS_RUN_SPEED;
	movementData.m_flWalkSpeed = DODS_WALK_SPEED;
	movementData.m_flSprintSpeed = DODS_SPRINT_SPEED;

	CHL2MPPlayerAnimState *pRet = new CHL2MPPlayerAnimState( pPlayer, movementData );

	pRet->InitHL2MP( pPlayer );

	return pRet;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CHL2MPPlayerAnimState::CHL2MPPlayerAnimState()
{
	m_pHL2MPPlayer = NULL;
	m_bFirstHoverFrame = false;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *pPlayer - 
//          &movementData - 
//-----------------------------------------------------------------------------
CHL2MPPlayerAnimState::CHL2MPPlayerAnimState( CBasePlayer *pPlayer, MultiPlayerMovementData_t &movementData )
	: CMultiPlayerAnimState( pPlayer, movementData )
{
	m_pHL2MPPlayer = NULL;
	m_bFirstHoverFrame = false;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CHL2MPPlayerAnimState::~CHL2MPPlayerAnimState()
{
}

//-----------------------------------------------------------------------------
// Purpose: Initialize HL2MP specific animation state.
// Input  : *pPlayer - 
//-----------------------------------------------------------------------------
void CHL2MPPlayerAnimState::InitHL2MP( CHL2MP_Player *pPlayer )
{
	m_pHL2MPPlayer = pPlayer;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : eyeYaw - 
//          eyePitch - 
//-----------------------------------------------------------------------------
void CHL2MPPlayerAnimState::Update( float eyeYaw, float eyePitch )
{
	CHL2MP_Player *pPlayer = GetHL2MPPlayer();

	if ( !pPlayer )
		return;

	CStudioHdr *pStudioHdr = pPlayer->GetModelPtr();

	if ( !pStudioHdr )
		return;

	if ( !ShouldUpdateAnimState() )
	{
		ClearAnimationState();

		return;
	}

	m_flEyeYaw = AngleNormalize( eyeYaw );
	m_flEyePitch = AngleNormalize( eyePitch );

	ComputeSequences( pStudioHdr );

	if ( SetupPoseParameters( pStudioHdr ) )
	{
		ComputePoseParam_MoveYaw( pStudioHdr );
		ComputePoseParam_AimPitch( pStudioHdr );
		ComputePoseParam_AimYaw( pStudioHdr );
	}

	ComputePlaybackRate();

	if ( mp_showgestureslots.GetInt() == pPlayer->entindex() )
		DebugGestureInfo();
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : event - 
//          nData - 
//-----------------------------------------------------------------------------
void CHL2MPPlayerAnimState::DoAnimationEvent( PlayerAnimEvent_t event, int nData )
{
	switch ( event )
	{
	case PLAYERANIMEVENT_ATTACK_PRIMARY:
		{
			Activity activity = TranslateActivity( ACT_RANGE_ATTACK1 );
			RestartGesture( GESTURE_SLOT_ATTACK_AND_RELOAD, activity );
			break;
		}

	case PLAYERANIMEVENT_ATTACK_SECONDARY:
		{
			Activity activity = TranslateActivity( ACT_RANGE_ATTACK2 );
			RestartGesture( GESTURE_SLOT_ATTACK_AND_RELOAD, activity );
			break;
		}

	case PLAYERANIMEVENT_RELOAD:
		{
			Activity activity = TranslateActivity( ACT_RELOAD );
			RestartGesture( GESTURE_SLOT_ATTACK_AND_RELOAD, activity );
			break;
		}

	case PLAYERANIMEVENT_CANCEL:
		{
			ResetGestureSlot( GESTURE_SLOT_ATTACK_AND_RELOAD );
			break;
		}

	default:
		{
			BaseClass::DoAnimationEvent( event, nData );
			break;
		}
	}
}
//-----------------------------------------------------------------------------
// Purpose: 
// Output : Activity
//-----------------------------------------------------------------------------
Activity CHL2MPPlayerAnimState::CalcMainActivity( void )
{
	Activity idealActivity = ACT_IDLE;

	if ( HandleJumping( idealActivity ) ||
	     HandleSwimming( idealActivity ) ||
	     HandleProne( idealActivity ) ||
	     HandleDucking( idealActivity ) ||
	     HandleMoving( idealActivity ) )
	{
	}

	ShowDebugInfo();

#ifdef CLIENT_DLL
	if ( anim_showmainactivity.GetBool() )
		DebugShowActivity( idealActivity );
#endif

	return idealActivity;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : actDesired - 
// Output : Activity
//-----------------------------------------------------------------------------
Activity CHL2MPPlayerAnimState::TranslateActivity( Activity actDesired )
{
	CHL2MP_Player *pPlayer = GetHL2MPPlayer();

	if ( !pPlayer )
		return actDesired;

	Activity idealActivity = actDesired;

	CWeaponDODSBase *pWeapon = dynamic_cast<CWeaponDODSBase *>( pPlayer->GetActiveWeapon() );

	if ( pWeapon && pWeapon->IsScoped() )
	{
		switch ( idealActivity )
		{
		case ACT_IDLE:
			idealActivity = ACT_DOD_IDLE_ZOOMED;
			break;

		case ACT_WALK:
		case ACT_RUN:
			idealActivity = ACT_DOD_WALK_ZOOMED;
			break;

		case ACT_CROUCHIDLE:
			idealActivity = ACT_DOD_CROUCH_ZOOMED;
			break;

		case ACT_RUN_CROUCH:
			idealActivity = ACT_DOD_CROUCHWALK_ZOOMED;
			break;

		case ACT_PRONE_IDLE:
			idealActivity = ACT_DOD_PRONE_ZOOMED;
			break;

		case ACT_PRONE_FORWARD:
			idealActivity = ACT_DOD_PRONE_FORWARD_ZOOMED;
			break;

		default:
			break;
		}
	}
	else if ( pPlayer->IsCrawling() )
	{
		switch ( idealActivity )
		{
		case ACT_RANGE_ATTACK1:
			idealActivity = ACT_DOD_PRIMARYATTACK_PRONE;
			break;

		case ACT_RANGE_ATTACK2:
			idealActivity = ACT_DOD_SECONDARYATTACK_PRONE;
			break;

		case ACT_RELOAD:
			idealActivity = ACT_DOD_RELOAD_PRONE;
			break;

		default:
			break;
		}
	}
	else if ( pPlayer->GetFlags() & FL_DUCKING )
	{
		switch ( idealActivity )
		{
		case ACT_RANGE_ATTACK1:
			idealActivity = ACT_DOD_PRIMARYATTACK_CROUCH;
			break;

		case ACT_RANGE_ATTACK2:
			idealActivity = ACT_DOD_SECONDARYATTACK_CROUCH;
			break;

		case ACT_DOD_HS_IDLE:
			idealActivity = ACT_DOD_HS_CROUCH;
			break;

		default:
			break;
		}
	}

	switch ( idealActivity )
	{
	case ACT_IDLE:
		idealActivity = ACT_DOD_STAND_IDLE;
		break;

	case ACT_CROUCHIDLE:
		idealActivity = ACT_DOD_CROUCH_IDLE;
		break;

	case ACT_RUN_CROUCH:
		idealActivity = ACT_DOD_CROUCHWALK_IDLE;
		break;

	case ACT_WALK:
		idealActivity = ACT_DOD_WALK_IDLE;
		break;

	case ACT_RUN:
		idealActivity = ACT_DOD_RUN_IDLE;
		break;

	default:
		break;
	}

	switch (idealActivity)
	{
	case ACT_HOP:
		return idealActivity;

	default:
		break;
	}

	if (pWeapon)
		idealActivity = pWeapon->ActivityOverride(idealActivity, false);

	return idealActivity;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *idealActivity - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CHL2MPPlayerAnimState::HandleJumping( Activity &idealActivity )
{
	CHL2MP_Player *pPlayer = GetHL2MPPlayer();

	if ( !pPlayer )
		return false;

	if ( pPlayer->GetMoveType() != MOVETYPE_WALK )
		return false;

	if ( pPlayer->GetWaterLevel() >= WL_Waist )
		return false;

	bool bOnGround = ( pPlayer->GetFlags() & FL_ONGROUND ) != 0;

	if ( !bOnGround )
	{
		if ( !m_bJumping )
		{
			m_bJumping = true;
			RestartMainSequence();
		}

		idealActivity = ACT_HOP;

		return true;
	}

	if ( m_bJumping )
	{
		m_bJumping = false;
		RestartMainSequence();
	}

	return false;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *idealActivity - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CHL2MPPlayerAnimState::HandleSwimming( Activity &idealActivity )
{
	CHL2MP_Player *pPlayer = GetHL2MPPlayer();

	if ( !pPlayer )
		return false;

	m_bInSwim = pPlayer->GetWaterLevel() >= WL_Waist;

	if ( m_bInSwim )
	{
		if ( m_bFirstSwimFrame )
		{
			RestartMainSequence();

			pPlayer->SetCycle( 1.0f );

			m_bFirstSwimFrame = false;
		}

		idealActivity = ACT_HL2MP_JUMP;
	}
	else
	{
		if ( !m_bFirstSwimFrame )
			m_bFirstSwimFrame = true;
	}

	return m_bInSwim;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *idealActivity - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CHL2MPPlayerAnimState::HandleHovering( Activity &idealActivity )
{
	CHL2MP_Player *pPlayer = GetHL2MPPlayer();

	if ( !pPlayer )
		return false;

	bool bHovering = !(pPlayer->GetFlags() & FL_ONGROUND) ||
	                   pPlayer->GetMoveType() != MOVETYPE_WALK;

	if ( bHovering )
	{
		if ( m_bFirstHoverFrame )
		{
			RestartMainSequence();

			pPlayer->SetCycle( 1.0f );

			m_bFirstHoverFrame = false;
		}

		idealActivity = ACT_HL2MP_JUMP;
	}
	else
	{
		if ( !m_bFirstHoverFrame )
			m_bFirstHoverFrame = true;
	}

	return bHovering;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *idealActivity - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CHL2MPPlayerAnimState::HandleDucking( Activity &idealActivity )
{
	CHL2MP_Player *pPlayer = GetHL2MPPlayer();

	if ( !pPlayer )
		return false;

	bool bDucking = ( pPlayer->GetFlags() & FL_DUCKING ) != 0;

	if ( bDucking )
	{
		if ( GetOuterXYSpeed() > MOVING_MINIMUM_SPEED )
			idealActivity = ACT_RUN_CROUCH;
		else
			idealActivity = ACT_CROUCHIDLE;
	}

	return bDucking;
}
bool CHL2MPPlayerAnimState::HandleProne( Activity &idealActivity )
{
	CHL2MP_Player *pPlayer = GetHL2MPPlayer();

	if ( !pPlayer )
		return false;

	if ( !pPlayer->IsCrawling() )
		return false;

	if ( GetOuterXYSpeed() > MOVING_MINIMUM_SPEED )
		idealActivity = ACT_PRONE_FORWARD;
	else
		idealActivity = ACT_PRONE_IDLE;

	return true;
}
//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *idealActivity - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CHL2MPPlayerAnimState::HandleMoving(Activity& idealActivity)
{
	CHL2MP_Player* pPlayer = GetHL2MPPlayer();

	if (!pPlayer)
		return false;

	float flSpeed = GetOuterXYSpeed();

	if (flSpeed <= MOVING_MINIMUM_SPEED)
		return false;

	if (flSpeed >= 260.0f)
		idealActivity = ACT_SPRINT;
	else if (flSpeed >= 120.0f)
		idealActivity = ACT_RUN;
	else
		idealActivity = ACT_WALK;

	return true;
}

//-----------------------------------------------------------------------------
// Purpose:
// Output : float
//-----------------------------------------------------------------------------
float CHL2MPPlayerAnimState::GetCurrentMaxGroundSpeed( void )
{
	CHL2MP_Player *pPlayer = GetHL2MPPlayer();

	if ( !pPlayer )
		return 1.0f;

	CStudioHdr *pStudioHdr = pPlayer->GetModelPtr();

	if ( !pStudioHdr )
		return 1.0f;

	float flSpeed = pPlayer->GetSequenceGroundSpeed( pPlayer->GetSequence() );

	return flSpeed;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *pStudioHdr - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CHL2MPPlayerAnimState::SetupPoseParameters( CStudioHdr *pStudioHdr )
{
	if ( m_bPoseParameterInit )
		return true;

	if ( !pStudioHdr )
		return false;

	CHL2MP_Player *pPlayer = GetHL2MPPlayer();

	if ( !pPlayer )
		return false;

	m_PoseParameterData.m_iMoveX = pPlayer->LookupPoseParameter( pStudioHdr, "move_x" );
	m_PoseParameterData.m_iMoveY = pPlayer->LookupPoseParameter( pStudioHdr, "move_y" );

	m_PoseParameterData.m_iAimPitch = pPlayer->LookupPoseParameter( pStudioHdr, "body_pitch" );
	m_PoseParameterData.m_iAimYaw = pPlayer->LookupPoseParameter( pStudioHdr, "body_yaw" );

	if ( m_PoseParameterData.m_iMoveX < 0 ||
		 m_PoseParameterData.m_iMoveY < 0 ||
		 m_PoseParameterData.m_iAimPitch < 0 ||
		 m_PoseParameterData.m_iAimYaw < 0 )
	{

		return false;
	}

	m_bPoseParameterInit = true;

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHL2MPPlayerAnimState::EstimateYaw( void )
{
	float flDeltaTime = gpGlobals->frametime;

	if ( flDeltaTime == 0.0f )
		return;

	Vector vecVelocity;

	GetOuterAbsVelocity( vecVelocity );

	QAngle angles = GetBasePlayer()->GetLocalAngles();

	if ( vecVelocity.y == 0 && vecVelocity.x == 0 )
	{
		float flYawDiff = angles[YAW] - m_PoseParameterData.m_flEstimateYaw;

		flYawDiff = flYawDiff - (int)(flYawDiff / 360) * 360;

		if ( flYawDiff > 180 )
			flYawDiff -= 360;
		if ( flYawDiff < -180 )
			flYawDiff += 360;

		if ( flDeltaTime < 0.25 )
			flYawDiff *= flDeltaTime * 4;
		else
			flYawDiff *= flDeltaTime;

		m_PoseParameterData.m_flEstimateYaw += flYawDiff;
		m_PoseParameterData.m_flEstimateYaw = m_PoseParameterData.m_flEstimateYaw - (int)(m_PoseParameterData.m_flEstimateYaw / 360) * 360;
	}
	else
	{
		m_PoseParameterData.m_flEstimateYaw = atan2( vecVelocity.y, vecVelocity.x ) * 180 / M_PI;

		if ( m_PoseParameterData.m_flEstimateYaw > 180 )
			m_PoseParameterData.m_flEstimateYaw = 180;
		else if ( m_PoseParameterData.m_flEstimateYaw < -180 )
			m_PoseParameterData.m_flEstimateYaw = -180;
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *pStudioHdr - 
//-----------------------------------------------------------------------------
void CHL2MPPlayerAnimState::ComputePoseParam_MoveYaw( CStudioHdr *pStudioHdr )
{
	CHL2MP_Player *pPlayer = GetHL2MPPlayer();

	if ( !pPlayer )
		return;

	Vector vecVelocity;
	GetOuterAbsVelocity( vecVelocity );

	vecVelocity.z = 0.0f;

	float flSpeed = vecVelocity.Length2D();

	if ( flSpeed <= MOVING_MINIMUM_SPEED )
	{
		pPlayer->SetPoseParameter( pStudioHdr, m_PoseParameterData.m_iMoveX, 0.0f );
		pPlayer->SetPoseParameter( pStudioHdr, m_PoseParameterData.m_iMoveY, 0.0f );
		return;
	}

	VectorNormalize( vecVelocity );

	QAngle angFacing = GetRenderAngles();

	angFacing[PITCH] = 0.0f;
	angFacing[ROLL] = 0.0f;

	Vector vecForward;
	Vector vecRight;

	AngleVectors( angFacing, &vecForward, &vecRight, NULL );

	float flForward = DotProduct( vecVelocity, vecForward );
	float flSide = -DotProduct( vecVelocity, vecRight );

	flForward = clamp( flForward, -1.0f, 1.0f );
	flSide = clamp( flSide, -1.0f, 1.0f );

	pPlayer->SetPoseParameter( pStudioHdr, m_PoseParameterData.m_iMoveX, flForward );
	pPlayer->SetPoseParameter( pStudioHdr, m_PoseParameterData.m_iMoveY, flSide );
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *pStudioHdr - 
//-----------------------------------------------------------------------------
void CHL2MPPlayerAnimState::ComputePoseParam_AimPitch( CStudioHdr *pStudioHdr )
{
	CHL2MP_Player *pPlayer = GetHL2MPPlayer();

	if ( !pPlayer )
		return;

	float flAimPitch = m_flEyePitch;

	pPlayer->SetPoseParameter( pStudioHdr, m_PoseParameterData.m_iAimPitch, -flAimPitch );

	m_DebugAnimData.m_flAimPitch = flAimPitch;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *pStudioHdr - 
//-----------------------------------------------------------------------------
void CHL2MPPlayerAnimState::ComputePoseParam_AimYaw( CStudioHdr *pStudioHdr )
{
	CHL2MP_Player *pPlayer = GetHL2MPPlayer();

	if ( !pPlayer )
		return;

	Vector vecVelocity;

	GetOuterAbsVelocity( vecVelocity );

	bool bMoving = GetOuterXYSpeed() > MOVING_MINIMUM_SPEED;

	if ( bMoving || m_bForceAimYaw )
	{
		m_flGoalFeetYaw = m_flEyeYaw;
	}
	else
	{
		if ( m_PoseParameterData.m_flLastAimTurnTime <= 0.0f )
		{
			m_flGoalFeetYaw = m_flEyeYaw;
			m_flCurrentFeetYaw = m_flEyeYaw;

			m_PoseParameterData.m_flLastAimTurnTime = gpGlobals->curtime;
		}
		else
		{
			float flYawDelta = AngleNormalize( m_flGoalFeetYaw - m_flEyeYaw );

			if ( fabs( flYawDelta ) > 45.0f )
			{
				float flSide = flYawDelta > 0.0f ? -1.0f : 1.0f;

				m_flGoalFeetYaw += 45.0f * flSide;
			}
		}
	}

	m_flGoalFeetYaw = AngleNormalize( m_flGoalFeetYaw );

	if ( m_flGoalFeetYaw != m_flCurrentFeetYaw )
	{
		if ( m_bForceAimYaw )
		{
			m_flCurrentFeetYaw = m_flGoalFeetYaw;
		}
		else
		{
			ConvergeYawAngles( m_flGoalFeetYaw, 720.0f, gpGlobals->frametime, m_flCurrentFeetYaw );

			m_flLastAimTurnTime = gpGlobals->curtime;
		}
	}

	m_angRender[YAW] = m_flCurrentFeetYaw;

	float flAimYaw = AngleNormalize( m_flEyeYaw - m_flCurrentFeetYaw );

	pPlayer->SetPoseParameter( pStudioHdr, m_PoseParameterData.m_iAimYaw, flAimYaw );

	m_DebugAnimData.m_flAimYaw = flAimYaw;

	m_bForceAimYaw = false;

#ifndef CLIENT_DLL
	QAngle angle = pPlayer->GetAbsAngles();

	angle[YAW] = m_flCurrentFeetYaw;

	pPlayer->SetAbsAngles( angle );
#endif
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHL2MPPlayerAnimState::ComputePlaybackRate( void )
{
	CHL2MP_Player *pPlayer = GetHL2MPPlayer();

	if ( !pPlayer )
		return;

	float flRate = 1.0f;

	if ( pPlayer->GetFlags() & FL_ONGROUND &&
	     pPlayer->GetMoveType() == MOVETYPE_WALK )
	{
		float flSpeed = GetOuterXYSpeed();

		if ( flSpeed > MOVING_MINIMUM_SPEED )
		{
			float flGroundSpeed = GetCurrentMaxGroundSpeed();

			flRate = flGroundSpeed < 0.001f ? 0.01 : clamp( flSpeed / flGroundSpeed, 0.01f, 10.f );
		}
	}

	pPlayer->SetPlaybackRate( flRate );
}
