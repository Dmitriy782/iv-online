//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CheckpointNatives.cpp
// Project: Server.Core
// Author(s): MaVe
// License: See LICENSE in root directory
//
//==============================================================================

#include "../Natives.h"
#include "Scripting/CScriptingManager.h"
#include "../CCheckpointManager.h"
#include "../CNetworkManager.h"
#include "../CPlayerManager.h"

extern CCheckpointManager * g_pCheckpointManager;
extern CNetworkManager * g_pNetworkManager;
extern CPlayerManager * g_pPlayerManager;

// Checkpoint functions

void CCheckpointNatives::Register(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterFunction("createCheckpoint", Create, 8, "ifffffff");
	pScriptingManager->RegisterFunction("deleteCheckpoint", Delete, 1, "i");
	pScriptingManager->RegisterFunction("showCheckpointForPlayer", ShowForPlayer, 2, "ii");
	pScriptingManager->RegisterFunction("hideCheckpointForPlayer", HideForPlayer, 2, "ii");
	pScriptingManager->RegisterFunction("showCheckpointForAll", ShowForAll, 1, "i");
	pScriptingManager->RegisterFunction("hideCheckpointForAll", HideForAll, 1, "i");
	pScriptingManager->RegisterFunction("setCheckpointType", SetType, 2, "ii");
	pScriptingManager->RegisterFunction("getCheckpointType", GetType, 1, "i");
	pScriptingManager->RegisterFunction("setCheckpointPosition", SetPosition, 4, "ifff");
	pScriptingManager->RegisterFunction("getCheckpointPosition", GetPosition, 1, "i");
	pScriptingManager->RegisterFunction("setCheckpointTargetPosition", SetTargetPosition, 4, "ifff");
	pScriptingManager->RegisterFunction("getCheckpointTargetPosition", GetTargetPosition, 1, "i");
	pScriptingManager->RegisterFunction("setCheckpointRadius", SetRadius, 2, "if");
	pScriptingManager->RegisterFunction("getCheckpointRadius", GetRadius, 1, "i");
	pScriptingManager->RegisterFunction("setCheckpointDimension", SetDimension, 2, "ii");
	pScriptingManager->RegisterFunction("getCheckpointDimension", GetDimension, 1, "i");
}

// createCheckpoint(type, x, y, z, targetx, targety, targetz, radius)
int CCheckpointNatives::Create(lua_State * pVM)
{
	int iType;
	CVector3 vecPosition;
	CVector3 vecTargetPosition;
	float fRadius;
	script_getinteger(pVM, -8, &iType);
	script_getvector3(pVM, -7, &vecPosition);
	script_getvector3(pVM, -4, &vecTargetPosition);
	script_getfloat(pVM, -1, &fRadius);
	script_pushentity(pVM, g_pCheckpointManager->Add(iType, vecPosition, vecTargetPosition, fRadius));
	return 1;
}

// deleteCheckpoint(checkpointid)
int CCheckpointNatives::Delete(lua_State * pVM)
{
	EntityId checkpointId;
	script_getentity(pVM, -1, &checkpointId);
	script_pushbool(pVM, g_pCheckpointManager->Delete(checkpointId));
	return 1;
}

// showCheckpointForPlayer(checkpointid, playerid)
int CCheckpointNatives::ShowForPlayer(lua_State * pVM)
{
	EntityId checkpointId;
	EntityId playerId;
	script_getentity(pVM, -2, &checkpointId);
	script_getentity(pVM, -1, &playerId);
	CCheckpoint * pCheckpoint = g_pCheckpointManager->Get(checkpointId);

	if(pCheckpoint)
	{
		pCheckpoint->ShowForPlayer(playerId);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// hideCheckpointForPlayer(checkpointid, playerid)
int CCheckpointNatives::HideForPlayer(lua_State * pVM)
{
	EntityId checkpointId;
	EntityId playerId;
	script_getentity(pVM, -2, &checkpointId);
	script_getentity(pVM, -1, &playerId);
	CCheckpoint * pCheckpoint = g_pCheckpointManager->Get(checkpointId);

	if(pCheckpoint)
	{
		pCheckpoint->HideForPlayer(playerId);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// showCheckpointForAll(checkpointid)
int CCheckpointNatives::ShowForAll(lua_State * pVM)
{
	EntityId checkpointId;
	script_getentity(pVM, -1, &checkpointId);
	CCheckpoint * pCheckpoint = g_pCheckpointManager->Get(checkpointId);

	if(pCheckpoint)
	{
		pCheckpoint->ShowForWorld();
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// hideCheckpointForAll(checkpointid)
int CCheckpointNatives::HideForAll(lua_State * pVM)
{
	EntityId checkpointId;
	script_getentity(pVM, -1, &checkpointId);
	CCheckpoint * pCheckpoint = g_pCheckpointManager->Get(checkpointId);

	if(pCheckpoint)
	{
		pCheckpoint->HideForWorld();
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// setCheckpointType(checkpointid, type)
int CCheckpointNatives::SetType(lua_State * pVM)
{
	EntityId checkpointId;;
	int iType;
	script_getentity(pVM, -1, &checkpointId);
	script_getinteger(pVM, -1, &iType);
	CCheckpoint * pCheckpoint = g_pCheckpointManager->Get(checkpointId);

	if(pCheckpoint)
	{
		pCheckpoint->SetType(iType);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// getCheckpointType(checkpointid, type)
int CCheckpointNatives::GetType(lua_State * pVM)
{
	EntityId checkpointId;
	script_getentity(pVM, -1, &checkpointId);
	CCheckpoint * pCheckpoint = g_pCheckpointManager->Get(checkpointId);

	if(pCheckpoint)
	{
		script_pushinteger(pVM, pCheckpoint->GetType());
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// setCheckpointPosition(checkpointid, position)
int CCheckpointNatives::SetPosition(lua_State * pVM)
{
	EntityId checkpointId;;
	CVector3 vecPosition;
	script_getentity(pVM, -4, &checkpointId);
	script_getvector3(pVM, -3, &vecPosition);
	CCheckpoint * pCheckpoint = g_pCheckpointManager->Get(checkpointId);

	if(pCheckpoint)
	{
		pCheckpoint->SetPosition(vecPosition);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// getCheckpointPosition(checkpointid)
int CCheckpointNatives::GetPosition(lua_State * pVM)
{
	EntityId checkpointId;;
	script_getentity(pVM, -1, &checkpointId);
	CCheckpoint * pCheckpoint = g_pCheckpointManager->Get(checkpointId);

	if(pCheckpoint)
	{
		CVector3 vecPosition;
		pCheckpoint->GetPosition(vecPosition);
		script_newtable(pVM, 0);
		script_pushinteger(pVM, 0);
		script_pushfloat(pVM, vecPosition.fX);
		script_tableset(pVM, -3);
		script_pushinteger(pVM, 1);
		script_pushfloat(pVM, vecPosition.fY);
		script_tableset(pVM, -3);
		script_pushinteger(pVM, 2);
		script_pushfloat(pVM, vecPosition.fZ);
		script_tableset(pVM, -3);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// setCheckpointTargetPosition(checkpointid, targetposition)
int CCheckpointNatives::SetTargetPosition(lua_State * pVM)
{
	EntityId checkpointId;
	CVector3 vecTargetPosition;
	script_getentity(pVM, -4, &checkpointId);
	script_getvector3(pVM, -3, &vecTargetPosition);
	CCheckpoint * pCheckpoint = g_pCheckpointManager->Get(checkpointId);

	if(pCheckpoint)
	{
		pCheckpoint->SetTargetPosition(vecTargetPosition);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// getCheckpointTargetPosition(checkpointid)
int CCheckpointNatives::GetTargetPosition(lua_State * pVM)
{
	EntityId checkpointId;
	script_getentity(pVM, -1, &checkpointId);
	CCheckpoint * pCheckpoint = g_pCheckpointManager->Get(checkpointId);

	if(pCheckpoint)
	{
		CVector3 vecTargetPosition;
		pCheckpoint->GetTargetPosition(vecTargetPosition);
		script_newtable(pVM, 0);
		script_pushinteger(pVM, 0);
		script_pushfloat(pVM, vecTargetPosition.fX);
		script_tableset(pVM, -3);
		script_pushinteger(pVM, 1);
		script_pushfloat(pVM, vecTargetPosition.fY);
		script_tableset(pVM, -3);
		script_pushinteger(pVM, 2);
		script_pushfloat(pVM, vecTargetPosition.fZ);
		script_tableset(pVM, -3);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// setCheckpointRadius(checkpointid, radius)
int CCheckpointNatives::SetRadius(lua_State * pVM)
{
	EntityId checkpointId;
	float fRadius;
	script_getentity(pVM, -2, &checkpointId);
	script_getfloat(pVM, -1, &fRadius);
	CCheckpoint * pCheckpoint = g_pCheckpointManager->Get(checkpointId);

	if(pCheckpoint)
	{
		pCheckpoint->SetRadius(fRadius);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// getCheckpointRadius(checkpointid)
int CCheckpointNatives::GetRadius(lua_State * pVM)
{
	EntityId checkpointId;
	script_getentity(pVM, -1, &checkpointId);
	CCheckpoint * pCheckpoint = g_pCheckpointManager->Get(checkpointId);

	if(pCheckpoint)
	{
		script_pushfloat(pVM, pCheckpoint->GetRadius());
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

int CCheckpointNatives::SetDimension(lua_State * pVM)
{
	int iDimension;
	EntityId checkpointId;

	script_getinteger(pVM, -1, &iDimension);
	script_getentity(pVM, -2, &checkpointId);
	CCheckpoint* pCheckpoint = g_pCheckpointManager->Get(checkpointId);
	if(pCheckpoint) {
		pCheckpoint->SetDimension(iDimension);
		script_pushbool(pVM, true);
		return 1;
	}
	script_pushbool(pVM, false);

	return 1;
}

int CCheckpointNatives::GetDimension(lua_State * pVM)
{
	EntityId checkpointId;

	script_getentity(pVM, -1, &checkpointId);
	CCheckpoint* pCheckpoint = g_pCheckpointManager->Get(checkpointId);
	if(pCheckpoint) {
		script_pushinteger(pVM, (int)pCheckpoint->GetDimension());
	}
	return 1;
}
