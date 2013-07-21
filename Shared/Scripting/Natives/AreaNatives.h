//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: AreaNatives.h
// Project: Shared
// Author(s): Einstein
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "Natives.h"

class CAreaNatives
{
private:
	static int Points2D(lua_State * pVM);
	static int Points3D(lua_State * pVM);
	static int PointInCircle(lua_State * pVM);
	static int PointInTube(lua_State * pVM);
	static int PointInBall(lua_State * pVM);
	static int PointInArea(lua_State * pVM);
	static int PointInCuboid(lua_State * pVM);
	static int PointInPolygon(lua_State * pVM);

public:
	static void      Register(CScriptingManager * pScriptingManager);
};
