//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CGlobalsManager.cpp
// Project: Server.Core
// Author(s): CrackHD
// License: See LICENSE in root directory
//
//==============================================================================

#include <Scripting/Natives/Natives.h>
#include "CScript.h"
#include "CGlobalsManager.h"

extern CGlobalsManager * g_pGlobalsManager;

void CGlobalsManager::RegisterNatives(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterFunction("isGlobalSet", IsSet, 1, "s");
	pScriptingManager->RegisterFunction("setGlobal", Set, -1, NULL);
	pScriptingManager->RegisterFunction("getGlobal", Get, 1, "s");
	pScriptingManager->RegisterFunction("removeGlobal", Remove, 1, "s");

	pScriptingManager->RegisterFunction("isPVarSet", IsPVarSet, 2, "is");
	pScriptingManager->RegisterFunction("setPVar", SetPVar, -1, NULL);
	pScriptingManager->RegisterFunction("getPVar", GetPVar, 2, "is");
	pScriptingManager->RegisterFunction("removePVar", RemovePVar, 2, "is");
	//pScriptingManager->RegisterFunction("clearPVars", ClearPVars, 1, "i");
}

int CGlobalsManager::IsSet(lua_State* pVM)
{
	const char* szKey;
	script_getstring(pVM, -1, &szKey);

	EntityId playerId = MAX_PLAYERS + 1;
	script_pushbool(pVM, g_pGlobalsManager->IsSet(playerId, String(szKey)));
	return 1;
}
int CGlobalsManager::Set(lua_State* pVM)
{
	const char* szKey;
	CScriptArgument pVar;

	script_getstring(pVM, -2, &szKey);
	pVar.pushFromStack(pVM, -1);//script_getarg(pVM, -1, &pVar);
	
	EntityId playerId = MAX_PLAYERS + 1;
	g_pGlobalsManager->Set(playerId, String(szKey), pVar);
	script_pushbool(pVM, true);
	return 1;
}
int CGlobalsManager::Get(lua_State* pVM)
{
	const char* szKey;
	script_getstring(pVM, -1, &szKey);

	EntityId playerId = MAX_PLAYERS + 1;
	CScriptArgument pVar = g_pGlobalsManager->Get(playerId, String(szKey));
	pVar.push(pVM);
	return 1;
}
int CGlobalsManager::Remove(lua_State* pVM)
{
	const char* szKey;
	script_getstring(pVM, -1, &szKey);

	EntityId playerId = MAX_PLAYERS + 1;
	script_pushbool(pVM, g_pGlobalsManager->Remove(playerId, String(szKey)));
	return 1;
}

int CGlobalsManager::IsPVarSet(lua_State* pVM)
{
	EntityId playerId;
	const char* szKey;
	
	script_getentity(pVM, -2, &playerId);
	script_getstring(pVM, -1, &szKey);

	//if(IvmpNatives::IsPlayerConnected(playerId))
		script_pushbool(pVM, g_pGlobalsManager->IsSet(playerId, String(szKey)));
	//else
	//	script_pushbool(pVM, false);
	return 1;
}
int CGlobalsManager::SetPVar(lua_State* pVM)
{
	EntityId playerId;
	const char* szKey;
	CScriptArgument pVar;

	script_getentity(pVM, -3, &playerId);
	script_getstring(pVM, -2, &szKey);
	pVar.pushFromStack(pVM, -1);//script_getarg(pVM, -1, &pVar);
	
	//if(IvmpNatives::IsPlayerConnected(playerId))
	//{
		g_pGlobalsManager->Set(playerId, String(szKey), pVar);
		script_pushbool(pVM, true);
	//}
	//else
	//	script_pushbool(pVM, false);
	return 1;
}
int CGlobalsManager::GetPVar(lua_State* pVM)
{
	EntityId playerId;
	const char* szKey;

	script_getentity(pVM, -2, &playerId);
	script_getstring(pVM, -1, &szKey);
	
	//if(IvmpNatives::IsPlayerConnected(playerId))
	//{
		CScriptArgument pVar = g_pGlobalsManager->Get(playerId, String(szKey));
		pVar.push(pVM);
	//}
	//else
	//	script_pushbool(pVM, false);
	return 1;
}
int CGlobalsManager::RemovePVar(lua_State* pVM)
{
	EntityId playerId;
	const char* szKey;

	script_getentity(pVM, -2, &playerId);
	script_getstring(pVM, -1, &szKey);
	
	//if(IvmpNatives::IsPlayerConnected(playerId))
		script_pushbool(pVM, g_pGlobalsManager->Remove(playerId, String(szKey)));
	//else
	//	script_pushbool(pVM, false);
	return 1;
}
/*int CGlobalsManager::ClearPVars(lua_State* pVM)
{
	EntityId playerId;
	script_getentity(pVM, -1, &playerId);
	
	if(IvmpNatives::IsPlayerConnected(playerId))
		script_pushbool(pVM, g_pGlobalsManager->Remove(playerId));
	else
		script_pushbool(pVM, false);
	return 1;
}*/