//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: 3DLabelNatives.h
// Project: Server.Core
// Author(s): XForce
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "../Natives.h"

class _3DLabelNatives
{
private:
	static int Create(lua_State * pVM);
	static int Delete(lua_State * pVM);
	static int SetPosition(lua_State * pVM);
	static int GetPosition(lua_State * pVM);
	static int SetColor(lua_State * pVM);
	static int GetColor(lua_State * pVM);
	static int SetDimension(lua_State * pVM);
	static int GetDimension(lua_State * pVM);
	static int SetText(lua_State * pVM);
	static int GetText(lua_State * pVM);
	static int SetVisible(lua_State * pVM);
	static int GetVisible(lua_State * pVM);
	static int SetStreamingDistance(lua_State * pVM);
	static int GetStreamingDistance(lua_State * pVM);

public:
	static void      Register(CScriptingManager * pScriptingManager);
};
