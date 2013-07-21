//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: BlipNatives.h
// Project: Server.Core
// Author(s): jenksta
//            MaVe
//            Sebihunter
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "../Natives.h"

class CBlipNatives
{
private:
	static int Create(lua_State * pVM);
	static int Delete(lua_State * pVM);
	static int SetCoordinates(lua_State * pVM);
	static int GetCoordinates(lua_State * pVM);
	static int SetColor(lua_State * pVM);
	static int GetColor(lua_State * pVM);
	static int SetSize(lua_State * pVM);
	static int GetSize(lua_State * pVM);
	static int ToggleShortRange(lua_State * pVM);
	static int ToggleRoute(lua_State * pVM);
	static int SetName(lua_State * pVM);
	static int GetName(lua_State * pVM);
	static int SwitchIcon(lua_State * pVM);
	static int SwitchIconPlayer(lua_State * pVM);
	static int CreatePlayerBlip(lua_State * pVM);
	static int DeletePlayerBlip(lua_State * pVM);
	static int TogglePlayerShortRange(lua_State * pVM);
	static int TogglePlayerBlipDisplay(lua_State * pVM);
	static int TogglePlayerShortRangeForPlayer(lua_State * pVM);
	static int TogglePlayerBlipDisplayForPlayer(lua_State * pVM);

public:
	static void      Register(CScriptingManager * pScriptingManager);
};
