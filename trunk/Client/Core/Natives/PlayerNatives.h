//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: PlayerNatives.h
// Project: Client.Core
// Author(s): jenksta
//            Einstein
//            Sebihunter
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "../Natives.h"

class CPlayerNatives
{
private:
	static int IsConnected(lua_State * pVM);
	static int GetLocal(lua_State * pVM);
	static int GetName(lua_State * pVM);
	static int GetHealth(lua_State * pVM);
	static int GetArmour(lua_State * pVM);
	static int GetCoordinates(lua_State * pVM);
	static int GetVelocity(lua_State * pVM);
	static int IsInAnyVehicle(lua_State * pVM);
	static int IsInVehicle(lua_State * pVM);
	static int GetVehicleId(lua_State * pVM);
	static int GetSeatId(lua_State * pVM);
	static int IsOnFoot(lua_State * pVM);
	static int GetModel(lua_State * pVM);
	static int IsSpawned(lua_State * pVM);
	static int GetHeading(lua_State * pVM);
	static int GetMoney(lua_State * pVM);
	static int GetState(lua_State * pVM);
	static int GetWeapon(lua_State * pVM);
	static int GetAmmoInClip(lua_State * pVM);
	static int GetAmmo(lua_State * pVM);
	static int GetInterior(lua_State * pVM);
	static int GetControlState(lua_State * pVM);
	static int GetPreviousControlState(lua_State * pVM);
	static int GetPing(lua_State * pVM);
	static int GetColor(lua_State * pVM);
	static int IsDucking(lua_State * pVM);
	static int IsJackingAVehicle(lua_State * pVM);
	static int GetWeaponSlot(lua_State * pVM);
	static int SetDoorLockState(lua_State * pVM);
	static int TogglePhysics(lua_State * pVM);
	static int GetWayPointCoords(lua_State * pVM);

public:
	static void      Register(CScriptingManager * pScriptingManager);
};
