//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: PickupNatives.cpp
// Project: Server.Core
// Author(s): mabako
// License: See LICENSE in root directory
//
//==============================================================================

#include "../Natives.h"
#include "Scripting/CScriptingManager.h"
#include "../CPickupManager.h"

extern CPickupManager * g_pPickupManager;

// Pickup functions

void CPickupNatives::Register(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterFunction("createPickup", Create, 9, "iiiffffff");
	pScriptingManager->RegisterFunction("deletePickup", Delete, 1, "i");
	pScriptingManager->RegisterFunction("getPickupModel", GetModel, 1, "i");
	pScriptingManager->RegisterFunction("getPickupType", GetType, 1, "i");
	pScriptingManager->RegisterFunction("setPickupValue", SetValue, 2, "ii");
	pScriptingManager->RegisterFunction("getPickupValue", GetValue, 1, "i");
	pScriptingManager->RegisterFunction("setPickupCoordinates", SetCoordinates, 4, "ifff");
	pScriptingManager->RegisterFunction("getPickupCoordinates", GetCoordinates, 1, "i");
	pScriptingManager->RegisterFunction("setPickupPosition", SetCoordinates, 4, "ifff");
	pScriptingManager->RegisterFunction("getPickupPosition", GetCoordinates, 1, "i");
	pScriptingManager->RegisterFunction("setPickupRotation", SetRotation, 4, "ifff");
	pScriptingManager->RegisterFunction("getPickupRotation", GetRotation, 1, "i");
}

// createPickup(modelhash, type, value?, x, y, z, rx, ry, rz)
int CPickupNatives::Create(lua_State * pVM)
{
	int modelhash, type, value;
	float x, y, z, rx, ry, rz;
	script_getinteger(pVM, 1, &modelhash);
	script_getinteger(pVM, 2, &type);
	script_getinteger(pVM, 3, &value);
	script_getfloat(pVM, 4, &x);
	script_getfloat(pVM, 5, &y);
	script_getfloat(pVM, 6, &z);
	script_getfloat(pVM, 7, &rx);
	script_getfloat(pVM, 8, &ry);
	script_getfloat(pVM, 9, &rz);
	script_pushinteger(pVM, g_pPickupManager->Create(modelhash, (unsigned char)type, (unsigned int)value, x, y, z, rx, ry, rz));
	return 1;
}

// deletePickup(pickupid)
int CPickupNatives::Delete(lua_State * pVM)
{
	int pickupid;
	script_getinteger(pVM, -1, &pickupid);
	if(g_pPickupManager->DoesExist(pickupid))
	{
		g_pPickupManager->Delete(pickupid);
		script_pushbool(pVM, true);
		return 1;
	}
	script_pushbool(pVM, false);
	return 1;
}

// getPickupModel(pickupid)
int CPickupNatives::GetModel(lua_State * pVM)
{
	int pickupid;
	script_getinteger(pVM, 1, &pickupid);
	if(g_pPickupManager->DoesExist(pickupid))
	{
		script_pushinteger(pVM, g_pPickupManager->GetModel(pickupid));
		return 1;
	}
	script_pushbool(pVM, false);
	return 1;
}

// getPickupType(pickupid)
int CPickupNatives::GetType(lua_State * pVM)
{
	int pickupid;
	script_getinteger(pVM, 1, &pickupid);
	if(g_pPickupManager->DoesExist(pickupid))
	{
		script_pushinteger(pVM, g_pPickupManager->GetType(pickupid));
		return 1;
	}
	script_pushbool(pVM, false);
	return 1;
}

// setPickupValue(pickupid, value)
int CPickupNatives::SetValue(lua_State * pVM)
{
	int pickupid;
	script_getinteger(pVM, 1, &pickupid);
	if(g_pPickupManager->DoesExist(pickupid))
	{
		int pvalue;
		script_getinteger(pVM, 2, &pvalue);
		script_pushbool(pVM, g_pPickupManager->SetValue(pickupid, pvalue));
		return 1;
	}
	script_pushbool(pVM, false);
	return 1;
}

// getPickupValue(pickupid)
int CPickupNatives::GetValue(lua_State * pVM)
{
	int pickupid;
	script_getinteger(pVM, 1, &pickupid);
	if(g_pPickupManager->DoesExist(pickupid))
	{
		script_pushinteger(pVM, g_pPickupManager->GetValue(pickupid));
		return 1;
	}
	script_pushbool(pVM, false);
	return 1;
}

// setPickupCoordinates(Pickupid, x, y, z)
int CPickupNatives::SetCoordinates(lua_State * pVM)
{
	int pickupid;
	script_getinteger(pVM, 1, &pickupid);
	if(g_pPickupManager->DoesExist(pickupid))
	{
		CVector3 vecPosition;
		script_getvector3(pVM, 2, &vecPosition);
		script_pushbool(pVM, g_pPickupManager->SetPosition(pickupid, vecPosition));
		return 1;
	}
	script_pushbool(pVM, false);
	return 1;
}

// getPickupCoordinates(Pickupid)
int CPickupNatives::GetCoordinates(lua_State * pVM)
{
	int pickupid;
	script_getinteger(pVM, 1, &pickupid);
	if(g_pPickupManager->DoesExist(pickupid))
	{
		CVector3 vecPosition;
		if(g_pPickupManager->GetPosition(pickupid, &vecPosition))
		{
			CScriptArray a(pVM);
			//script_newarray(pVM, 0);
			a.push(vecPosition.fX);
			//script_arrayappend(pVM, -2);
			a.push(vecPosition.fY);
			//script_arrayappend(pVM, -2);
			a.push(vecPosition.fZ);
			//script_arrayappend(pVM, -2);
			return 1;
		}
	}
	script_pushbool(pVM, false);
	return 1;
}


// setPickupRotation(Pickupid, x, y, z)
int CPickupNatives::SetRotation(lua_State * pVM)
{
	int pickupid;
	script_getinteger(pVM, 1, &pickupid);
	if(g_pPickupManager->DoesExist(pickupid))
	{
		CVector3 vecPosition;
		script_getvector3(pVM, 2, &vecPosition);
		script_pushbool(pVM, g_pPickupManager->SetRotation(pickupid, vecPosition));
		return 1;
	}
	script_pushbool(pVM, false);
	return 1;
}

// getPickupRotation(Pickupid)
int CPickupNatives::GetRotation(lua_State * pVM)
{
	int pickupid;
	script_getinteger(pVM, 1, &pickupid);
	if(g_pPickupManager->DoesExist(pickupid))
	{
		CVector3 vecPosition;
		if(g_pPickupManager->GetRotation(pickupid, &vecPosition))
		{
			CScriptArray a(pVM);
			//script_newarray(pVM, 0);
			a.push(vecPosition.fX);
			//script_arrayappend(pVM, -2);
			a.push(vecPosition.fY);
			//script_arrayappend(pVM, -2);
			a.push(vecPosition.fZ);
			//script_arrayappend(pVM, -2);
			return 1;
		}
	}
	script_pushbool(pVM, false);
	return 1;
}
