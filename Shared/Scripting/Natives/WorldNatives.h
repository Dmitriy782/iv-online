//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: WorldNatives.h
// Project: Shared
// Author(s): mabako
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "Natives.h"

class CWorldNatives
{
private:
	static int SetTime(lua_State * pVM);
	static int GetTime(lua_State * pVM);
	static int SetMinuteDuration(lua_State * pVM);
	static int GetMinuteDuration(lua_State * pVM);
	static int SetDayOfWeek(lua_State * pVM);
	static int GetDayOfWeek(lua_State * pVM);
	static int SetTrafficLightsState(lua_State * pVM);
	static int GetTrafficLightsState(lua_State * pVM);
	static int SetTrafficLightsLocked(lua_State * pVM);
	static int AreTrafficLightsLocked(lua_State * pVM);
	static int SetTrafficLightsPhaseDuration(lua_State * pVM);
	static int GetTrafficLightsPhaseDuration(lua_State * pVM);
	static int ResetTrafficLightsPhaseDuration(lua_State * pVM);

public:
	static void      Register(CScriptingManager * pScriptingManager);
};
