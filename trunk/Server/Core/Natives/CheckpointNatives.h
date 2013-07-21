//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CheckpointNatives.h
// Project: Server.Core
// Author(s): MaVe
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "../Natives.h"

class CCheckpointNatives
{
private:
	static int Create(lua_State * pVM);
	static int Delete(lua_State * pVM);
	static int ShowForPlayer(lua_State * pVM);
	static int HideForPlayer(lua_State * pVM);
	static int ShowForAll(lua_State * pVM);
	static int HideForAll(lua_State * pVM);
	static int SetType(lua_State * pVM);
	static int GetType(lua_State * pVM);
	static int SetPosition(lua_State * pVM);
	static int GetPosition(lua_State * pVM);
	static int SetTargetPosition(lua_State * pVM);
	static int GetTargetPosition(lua_State * pVM);
	static int SetRadius(lua_State * pVM);
	static int GetRadius(lua_State * pVM);
	static int SetDimension(lua_State * pVM);
	static int GetDimension(lua_State * pVM);

public:
	static void      Register(CScriptingManager * pScriptingManager);
};
