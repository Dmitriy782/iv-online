//============== Copyright © 2010 IV:MP Team. All rights reserved. ==============
// File: Functions.cpp
//===============================================================================

#include "Functions.h"

// helloWorld ( fTest )
int lua_helloworld(lua_State * pVM)
{
	// Get float param
	float fTest;
	fTest = lua_tonumber(pVM, -1);

	// Return this float param incremented
	fTest += 0.15f;
	lua_pushnumber(pVM, fTest);

	// No errors.
	return 1;
}

// [Example] function which has several arguments and optional argument.
// createVehicleEx ( iModel, fX,fY,fZ, fRX,fRY,fRZ [, iRespawn_delay] )
int lua_createVehicleEx(lua_State * pVM)
{
	// Check minimum parameters count (we need 7 at least)
	if(lua_gettop(pVM) < 8) // offset is +1 for calculating parameters count
	{
		lua_pushstring(pVM, "Invalid syntax (expected more params) at createVehicleEx");
		lua_error(pVM);
		return -1;	// -1 enables 'raise error' in virtual machine
	}

	int iModel;
	float fX,fY,fZ;
	float fRX,fRY,fRZ;
	int iRespawn_delay = -1;	// default value for optional argument

	iModel = lua_tointeger(pVM, 2);	// offset is +2 for parameter index
	fX = lua_tonumber(pVM, 3);
	fY = lua_tonumber(pVM, 4);
	fZ = lua_tonumber(pVM, 5);
	fRX = lua_tonumber(pVM, 6);
	fRY = lua_tonumber(pVM, 7);
	fRZ = lua_tonumber(pVM, 8);
	if(lua_gettop(pVM) >= 9)
		iRespawn_delay = lua_tointeger(pVM, 9);

	// Create vehicle and return its identifier (ID):
	EntityId veh = IVMP::Vehicles()->Create(iModel, CVector3(fX,fY,fZ), CVector3(fRX,fRY,fRZ), 0,0,0,0, iRespawn_delay);
	lua_pushinteger(pVM, veh);

	// No errors.
	return 1;
}