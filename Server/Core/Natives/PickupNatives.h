//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: PickupNatives.h
// Project: Server.Core
// Author(s): mabako
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "../Natives.h"

class CPickupNatives
{
private:
	static int Create(lua_State * pVM);
	static int Delete(lua_State * pVM);
	static int GetModel(lua_State * pVM);
	static int GetType(lua_State * pVM);
	static int SetValue(lua_State * pVM);
	static int GetValue(lua_State * pVM);
	static int SetCoordinates(lua_State * pVM);
	static int GetCoordinates(lua_State * pVM);
	static int SetRotation(lua_State * pVM);
	static int GetRotation(lua_State * pVM);

public:
	static void      Register(CScriptingManager * pScriptingManager);
};
