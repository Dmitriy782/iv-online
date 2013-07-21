//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: ServerNatives.h
// Project: Server.Core
// Author(s): jenksta
//            MaVe
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "../Natives.h"

class CServerNatives
{
private:
	static int Log(lua_State * pVM);
	static int LogF(lua_State * pVM);
	static int AddRule(lua_State * pVM);
	static int RemoveRule(lua_State * pVM);
	static int SetRule(lua_State * pVM);
	static int GetRule(lua_State * pVM);
	static int Shutdown(lua_State * pVM);
	static int SetPassword(lua_State * pVM);
	static int GetPassword(lua_State * pVM);
	static int GetPlayerCount(lua_State * pVM);
	static int GetPlayers(lua_State * pVM);
	static int GetPlayerSlots(lua_State * pVM);
	static int GetTickCount(lua_State * pVM);
	static int SetHostName(lua_State * pVM);
	static int GetHostName(lua_State * pVM);
	static int TogglePayAndSpray(lua_State * pVM);
	static int ToggleAutoAim(lua_State * pVM);
	static int SendInput(lua_State * pVM);
	static int GetConfig(lua_State * pVM);
	static int GetWeaponName(lua_State * pVM);
	static int GetVehicleName(lua_State * pVM);
	static int ToggleFrequentEvents(lua_State * pVM);
	static int AreFrequentEventsEnabled(lua_State * pVM);
	static int SetWeather(lua_State * pVM);
	static int GetWeather(lua_State * pVM);
	static int ForceWind(lua_State * pvM);

public:
	static void      Register(CScriptingManager * pScriptingManager);
};
