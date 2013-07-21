//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: SystemNatives.h
// Project: Server.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "../Natives.h"

class CSystemNatives
{
private:
	static int Clock(lua_State * pVM);
	static int Time(lua_State * pVM);
	static int Date(lua_State * pVM);
	static int MkDir(lua_State * pVM);
	static int RmDir(lua_State * pVM);
	static int Remove(lua_State * pVM);
	static int Rename(lua_State * pVM);

public:
	static void      Register(CScriptingManager * pScriptingManager);
};
