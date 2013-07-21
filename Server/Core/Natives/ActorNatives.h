//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: ActorNatives.h
// Project: Server.Core
// Author(s): jenksta
//            MaVe
//            Sebihunter
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "../Natives.h"

class CActorNatives
{
private:
	static int Create(lua_State * pVM);
	static int Delete(lua_State * pVM);
	static int IsValid(lua_State * pVM);
	static int SetCoordinates(lua_State * pVM);
	static int GetCoordinates(lua_State * pVM);
	static int SetHeading(lua_State * pVM);
	static int GetHeading(lua_State * pVM);
	static int GetModel(lua_State * pVM);
	static int GetCount(lua_State * pVM);
	static int WalkToCoordinates(lua_State * pVM);
	static int SetName(lua_State * pVM);
	static int GetName(lua_State * pVM);
	static int ToggleNametag(lua_State * pVM);
	static int ToggleBlip(lua_State * pVM);
	static int SetColor(lua_State * pVM);
	static int GetColor(lua_State * pVM);
	static int ToggleFrozen(lua_State * pVM);
	static int ToggleHelmet(lua_State * pVM);
	static int WarpIntoVehicle(lua_State * pVM);
	static int RemoveFromVehicle(lua_State * pVM);
	static int DeleteAll(lua_State * pVM);
	static int DriveToCoordinates(lua_State * pVM);
	static int ForceAnim(lua_State * pVM);
	static int SaySpeech(lua_State * pVM);

public:
	static void      Register(CScriptingManager * pScriptingManager);
};
