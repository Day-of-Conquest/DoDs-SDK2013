//=============================================================================//
//
// Purpose: Weapon base for Day of Defeat: Source
//
//=============================================================================//

#include "cbase.h"
#include "dods_weaponbase.h"


IMPLEMENT_NETWORKCLASS_ALIASED(WeaponDODSBase, DT_WeaponDODSBase)

BEGIN_NETWORK_TABLE(CWeaponDODSBase, DT_WeaponDODSBase)
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA(CWeaponDODSBase)
END_PREDICTION_DATA()

#ifdef GAME_DLL

BEGIN_DATADESC(CWeaponDODSBase)
END_DATADESC()

#endif


CWeaponDODSBase::CWeaponDODSBase()
{
}