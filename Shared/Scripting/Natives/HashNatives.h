//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: HashNatives.h
// Project: Shared
// Author(s): Einstein
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "Natives.h"

class CHashNatives
{
private:
	static int md5(lua_State * pVM);
	static int md5File(lua_State * pVM);

public:
	static void      Register(CScriptingManager * pScriptingManager);
};
