//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: ObjectNatives.h
// Project: Server.Core
// Author(s): jenksta
//            MaVe
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "../Natives.h"

class CObjectNatives
{
private:
	static int Create(lua_State * pVM);
	static int Delete(lua_State * pVM);
	static int GetModel(lua_State * pVM);
	static int GetCoordinates(lua_State * pVM);
	static int SetCoordinates(lua_State * pVM);
	static int GetRotation(lua_State * pVM);
	static int SetRotation(lua_State * pVM);
	static int CreateFire(lua_State * pVM);
	static int DeleteFire(lua_State * pVM);
	static int CreateExplosion(lua_State * pVM);
	static int AttachVehicle(lua_State * pVM);
	static int AttachPed(lua_State * pVM);
	static int AttachPedBone(lua_State * pVM);
	static int DetachObject(lua_State * pVM);
	static int MoveObject(lua_State * pVM);
	static int RotateObject(lua_State * pVM);
	static int SetDimension(lua_State * pVM);
	static int GetDimension(lua_State * pVM);
	static int SetInterior(lua_State * pVM);
	static int GetInterior(lua_State * pVM);

public:
	static void      Register(CScriptingManager * pScriptingManager);
};
