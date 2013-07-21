//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: EventNatives.h
// Project: Client.Core
// Author(s): jenksta
//            Einstein
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "Natives.h"

class CEventNatives
{
private:
	static int Add(lua_State * pVM);
	static int Call(lua_State * pVM);
	static int Remove(lua_State * pVM);

public:
	static void      Register(CScriptingManager * pScriptingManager);
};
