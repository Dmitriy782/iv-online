//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: VehicleNatives.h
// Project: Server.Core
// Author(s): jenksta
//            MaVe
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "../Natives.h"

class CVehicleNatives
{
private:
	static int GetCoordinates(lua_State * pVM);
	static int GetRotation(lua_State * pVM);
	static int IsValid(lua_State * pVM);
	static int GetColor(lua_State * pVM);
	static int GetModel(lua_State * pVM);
	static int GetHealth(lua_State * pVM);
	static int GetEngineHealth(lua_State * pVM);
	static int GetVelocity(lua_State * pVM);
	static int GetAngularVelocity(lua_State * pVM);
	static int GetLocked(lua_State * pVM);
	static int GetDirtLevel(lua_State * pVM);
	static int GetSirenState(lua_State * pVM);
	static int IsOccupied(lua_State * pVM);
	static int GetOccupants(lua_State * pVM);
	static int GetEngineStatus(lua_State * pVM);
	static int GetGpsState(lua_State *pVM);

public:
	static void      Register(CScriptingManager * pScriptingManager);
};
