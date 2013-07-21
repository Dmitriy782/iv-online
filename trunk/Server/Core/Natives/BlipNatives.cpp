//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: BlipNatives.cpp
// Project: Server.Core
// Author(s): jenksta
//            Sebihunter
//            Einstein
// License: See LICENSE in root directory
//
//==============================================================================

#include "../Natives.h"
#include "Scripting/CScriptingManager.h"
#include "../CBlipManager.h"
#include "../CNetworkManager.h"
#include "../CPlayerManager.h"

//  Disable bool warning
#pragma warning( disable:4800 )

extern CBlipManager * g_pBlipManager;
extern CNetworkManager * g_pNetworkManager;
extern CPlayerManager * g_pPlayerManager;

// Blip functions

void CBlipNatives::Register(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterFunction("createBlip", Create, 5, "ifffb");
	pScriptingManager->RegisterFunction("deleteBlip", Delete, 1, "i");
	pScriptingManager->RegisterFunction("setBlipCoordinates", SetCoordinates, 4, "ifff");
	pScriptingManager->RegisterFunction("getBlipCoordinates", GetCoordinates, 1, "i");
	pScriptingManager->RegisterFunction("setBlipPosition", SetCoordinates, 4, "ifff");
	pScriptingManager->RegisterFunction("getBlipPosition", GetCoordinates, 1, "i");
	pScriptingManager->RegisterFunction("setBlipColor", SetColor, -1, "ii");
	pScriptingManager->RegisterFunction("getBlipColor", GetColor, 1, "i");
	pScriptingManager->RegisterFunction("setBlipSize", SetSize, -1, "ii");
	pScriptingManager->RegisterFunction("getBlipSize", GetSize, 1, "i");
	pScriptingManager->RegisterFunction("toggleBlipShortRange", ToggleShortRange, -1, "ib");
	pScriptingManager->RegisterFunction("toggleBlipRoute", ToggleRoute, -1, "ib");
	pScriptingManager->RegisterFunction("setBlipName", SetName, -1, "is");
	pScriptingManager->RegisterFunction("getBlipName", GetName, -1, "is");
	pScriptingManager->RegisterFunction("switchBlipIcon", SwitchIcon, 2, "ib");
	pScriptingManager->RegisterFunction("switchBlipIconForPlayer", SwitchIconPlayer, 3, "iib");

	pScriptingManager->RegisterFunction("createPlayerBlip", CreatePlayerBlip, 2, "ii");
	pScriptingManager->RegisterFunction("deletePlayerBlip", DeletePlayerBlip, 1, "i");
	pScriptingManager->RegisterFunction("togglePlayerBlipShortRange", TogglePlayerShortRange, 2, "ib");
	pScriptingManager->RegisterFunction("togglePlayerBlipDisplay", TogglePlayerBlipDisplay, 2, "ib");
	pScriptingManager->RegisterFunction("togglePlayerBlipShortRangeForPlayer", TogglePlayerShortRangeForPlayer, 2, "iib");
	pScriptingManager->RegisterFunction("togglePlayerBlipDisplayForPlayer", TogglePlayerBlipDisplayForPlayer, 2, "iib");
}

// createBlip(spriteID, x, y, z)
int CBlipNatives::Create(lua_State * pVM)
{
	int iSprite;
	CVector3 vecPosition;
	bool bShow;
	script_getinteger(pVM, -5, &iSprite);
	script_getvector3(pVM, -4, &vecPosition);
	script_getbool(pVM, -1, &bShow);
	bool bToggle = (bShow != 0);
	script_pushentity(pVM, g_pBlipManager->Create(iSprite, vecPosition, bToggle));
	return 1;
}

// deleteBlip(blipid)
int CBlipNatives::Delete(lua_State * pVM)
{
	EntityId blipId;
	script_getentity(pVM, -1, &blipId);

	if(g_pBlipManager->DoesExist(blipId))
	{
		g_pBlipManager->Delete(blipId);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// setBlipCoordinates(blipid, x, y, z)
int CBlipNatives::SetCoordinates(lua_State * pVM)
{
	EntityId blipId;
	CVector3 vecPosition;
	script_getentity(pVM, -4, &blipId);
	script_getvector3(pVM, -3, &vecPosition);

	if(g_pBlipManager->DoesExist(blipId))
	{
		g_pBlipManager->SetPosition(blipId, vecPosition);
		script_pushbool(pVM, true);
	}

	script_pushbool(pVM, false);
	return 1;
}

// getBlipCoordinates(blipid)
int CBlipNatives::GetCoordinates(lua_State * pVM)
{
	EntityId blipId;
	script_getentity(pVM, 1, &blipId);

	if(g_pBlipManager->DoesExist(blipId))
	{
		CVector3 vecPosition = g_pBlipManager->GetPosition(blipId);
		CScriptArray a(pVM);
		a.push(vecPosition.fX);
		a.push(vecPosition.fY);
		a.push(vecPosition.fZ);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// setBlipColor(blipid, color[, playerID = INVALID_ENTITY_ID])
int CBlipNatives::SetColor(lua_State * pVM)
{
	CHECK_PARAMS_MIN_MAX("setBlipColor", 2, 3);
	CHECK_TYPE("setBlipColor", 1, 1, SCRIPT_TYPE_INTEGER);
	CHECK_TYPE("setBlipColor", 2, 2, SCRIPT_TYPE_INTEGER);

	EntityId blipId;
	int iColor;
	EntityId playerId = INVALID_ENTITY_ID;

	script_getentity(pVM, 1, &blipId);
	script_getinteger(pVM, 2, &iColor);

	if(script_gettop(pVM) >= 3 && script_gettype(pVM,3) == SCRIPT_TYPE_INTEGER)
		script_getentity(pVM, 3, &playerId);
	
	if(g_pBlipManager->DoesExist(blipId))
	{
		if(playerId != INVALID_ENTITY_ID)
		{
			if(g_pPlayerManager->DoesExist(playerId))
			{
				CBitStream bsSend;
				bsSend.Write(blipId);
				bsSend.Write((int)iColor);
				g_pNetworkManager->RPC(RPC_ScriptingSetBlipColor, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
				script_pushbool(pVM, true);
				return 1;
			}
		}
		else
		{
			g_pBlipManager->SetColor(blipId, iColor);
			script_pushbool(pVM, true);
			return 1;
		}
	}

	script_pushbool(pVM, false);
	return 1;
}

// getBlipColor(blipid)
int CBlipNatives::GetColor(lua_State * pVM)
{
	EntityId blipId;
	//int blipId;
	script_getentity(pVM, 1, &blipId);

	if(g_pBlipManager->DoesExist(blipId))
	{
		script_pushinteger(pVM, g_pBlipManager->GetColor(blipId));
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// setBlipSize(blipid, size[, playerID = -1 ])
int CBlipNatives::SetSize(lua_State * pVM)
{
	CHECK_PARAMS_MIN_MAX("setBlipSize", 2, 3);
	CHECK_TYPE("setBlipSize", 1, 1, SCRIPT_TYPE_INTEGER);
	CHECK_TYPE("setBlipSize", 2, 2, SCRIPT_TYPE_FLOAT);

	EntityId blipId;
	float fSize;
	EntityId playerId = INVALID_ENTITY_ID;

	script_getentity(pVM, 1, &blipId);
	script_getfloat(pVM, 2, &fSize);

	if(script_gettop(pVM) >= 3 && script_gettype(pVM,3) == SCRIPT_TYPE_INTEGER)
		script_getentity(pVM, 3, &playerId);
	
	if(fSize > 0.0f && fSize <= 4.0f && g_pBlipManager->DoesExist(blipId))
	{
		if(playerId != INVALID_ENTITY_ID)
		{
			if(g_pPlayerManager->DoesExist(playerId))
			{
				CBitStream bsSend;
				bsSend.Write(blipId);
				bsSend.Write(fSize);
				g_pNetworkManager->RPC(RPC_ScriptingSetBlipSize, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
				script_pushbool(pVM, true);
				return 1;
			}
		}
		else
		{
			g_pBlipManager->SetSize(blipId, fSize);
			script_pushbool(pVM, true);
			return 1;
		}
	}

	script_pushbool(pVM, false);
	return 1;
}

// getBlipSize(blipid)
int CBlipNatives::GetSize(lua_State * pVM)
{
	EntityId blipId;
	script_getentity(pVM, 1, &blipId);

	if(g_pBlipManager->DoesExist(blipId))
	{
		script_pushfloat(pVM, g_pBlipManager->GetSize(blipId));
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// toggleBlipShortRange(blipid, toggle [, playerid = -1])
int CBlipNatives::ToggleShortRange(lua_State * pVM)
{
	CHECK_PARAMS_MIN_MAX("toggleBlipShortRange", 2, 3);
	CHECK_TYPE("toggleBlipShortRange", 1, 1, SCRIPT_TYPE_INTEGER);
	CHECK_TYPE("toggleBlipShortRange", 2, 2, SCRIPT_TYPE_BOOL);

	EntityId blipId;
	bool bToggle;
	EntityId playerId = INVALID_ENTITY_ID;

	script_getentity(pVM, 1, &blipId);
	script_getbool(pVM, 2, &bToggle);

	if(script_gettop(pVM) >= 3 && script_gettype(pVM,3) == SCRIPT_TYPE_INTEGER)
		script_getentity(pVM, 3, &playerId);

	if(playerId != INVALID_ENTITY_ID)
	{
		if(g_pPlayerManager->DoesExist(playerId))
		{
			CBitStream bsSend;
			bsSend.Write(blipId);
			bsSend.Write((bool)bToggle);
			g_pNetworkManager->RPC(RPC_ScriptingToggleBlipShortRange, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
			script_pushbool(pVM, true);
			return 1;
		}
	}
	else
	{
		g_pBlipManager->ToggleShortRange(blipId, (bToggle != 0));
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// toggleBlipRoute(blipid, toggle [, playerid = -1])
int CBlipNatives::ToggleRoute(lua_State * pVM)
{
	CHECK_PARAMS_MIN_MAX("toggleBlipRoute", 2, 3);
	CHECK_TYPE("toggleBlipRoute", 1, 1, SCRIPT_TYPE_INTEGER);
	CHECK_TYPE("toggleBlipRoute", 2, 2, SCRIPT_TYPE_BOOL);

	EntityId blipId;
	bool bToggle;
	EntityId playerId = INVALID_ENTITY_ID;

	script_getentity(pVM, 1, &blipId);
	script_getbool(pVM, 2, &bToggle);

	if(script_gettop(pVM) >= 3 && script_gettype(pVM,3) == SCRIPT_TYPE_INTEGER)
		script_getentity(pVM, 3, &playerId);

	if(playerId != INVALID_ENTITY_ID)
	{
		if(g_pPlayerManager->DoesExist(playerId))
		{
			CBitStream bsSend;
			bsSend.Write(blipId);
			bsSend.Write((bool)bToggle);
			g_pNetworkManager->RPC(RPC_ScriptingToggleBlipRoute, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
			script_pushbool(pVM, true);
			return 1;
		}
	}
	else
	{
		g_pBlipManager->ToggleRoute(blipId, (bToggle != 0));
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// setBlipName(blipid, name[, playerid = -1])
int CBlipNatives::SetName(lua_State * pVM)
{
	int i = lua_gettop(pVM);

	CHECK_PARAMS_MIN_MAX("setBlipName", 2, 3);
	CHECK_TYPE("setBlipName", 1, 1, SCRIPT_TYPE_INTEGER);
	CHECK_TYPE("setBlipName", 2, 2, SCRIPT_TYPE_STRING);

	EntityId blipId;
	const char * szName;
	EntityId playerId = INVALID_ENTITY_ID;

	script_getentity(pVM, 1, &blipId);
	script_getstring(pVM, 2, &szName);

	if(script_gettop(pVM) >= 3 && script_gettype(pVM,3) == SCRIPT_TYPE_INTEGER)
		script_getentity(pVM, 3, &playerId);

	if(strlen(szName) > 16) {
		CLogFile::Printf("Failed to set blip(%d) name \"%s\"(size: %d/16)",blipId,szName,strlen(szName));
		script_pushbool(pVM,false);
		return 1;
	}

	if(playerId != INVALID_ENTITY_ID)
	{
		if(g_pPlayerManager->DoesExist(playerId))
		{
			CBitStream bsSend;
			bsSend.Write(blipId);
			bsSend.Write(String(szName));
			g_pNetworkManager->RPC(RPC_ScriptingSetBlipName, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
			script_pushbool(pVM, true);
			return 1;
		}
	}
	else
	{
		g_pBlipManager->SetName(blipId, szName);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// getBlipName(blipid)
int CBlipNatives::GetName(lua_State * pVM)
{
	EntityId blipId;
	script_getentity(pVM, 1, &blipId);

	if(g_pBlipManager->DoesExist(blipId))
	{
		script_pushstring(pVM, g_pBlipManager->GetName(blipId).C_String());
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// switchBlipIcon(blipId,switch)
int CBlipNatives::SwitchIcon(lua_State * pVM)
{
	EntityId blipId;
	script_getentity(pVM, -2, &blipId);
	
	bool bShow;
	script_getbool(pVM,-1,&bShow);

	if(g_pBlipManager->DoesExist(blipId))
	{
		bool bToggle = (bShow != 0);
		g_pBlipManager->SwitchIcon(blipId,bToggle,INVALID_ENTITY_ID);
		script_pushbool(pVM,true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// switchBlipIcon(blipId, playerid, switch)
int CBlipNatives::SwitchIconPlayer(lua_State * pVM)
{
	EntityId blipId;
	script_getentity(pVM, -3, &blipId);

	EntityId playerId;
	script_getentity(pVM, -2, &playerId);
	
	bool bShow;
	script_getbool(pVM,-1,&bShow);

	if(g_pBlipManager->DoesExist(blipId) && g_pPlayerManager->DoesExist(playerId))
	{
		bool bToggle = (bShow != 0);
		g_pBlipManager->SwitchIcon(blipId,bToggle,playerId);
		script_pushbool(pVM,true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

int CBlipNatives::CreatePlayerBlip(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -2, &playerId);

	int iSprite;
	script_getinteger(pVM, -1, &iSprite);

	if(g_pPlayerManager->DoesExist(playerId) && !g_pBlipManager->DoesPlayerBlipExist(playerId))
	{
		g_pBlipManager->CreateForPlayer(playerId, (int)iSprite, true);
		script_pushbool(pVM,true);
		return 1;
	}

	script_pushbool(pVM,false);
	return 1;
}

int CBlipNatives::DeletePlayerBlip(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -1, &playerId);

	if(g_pPlayerManager->DoesExist(playerId) && g_pBlipManager->DoesPlayerBlipExist(playerId))
	{
		g_pBlipManager->DeleteForPlayer(playerId);
		script_pushbool(pVM,true);
		return 1;
	}

	script_pushbool(pVM,false);
	return 1;
}

int CBlipNatives::TogglePlayerBlipDisplay(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -2, &playerId);

	bool bToggle;
	script_getbool(pVM, -1, &bToggle);

	bool bShow = (bToggle != 0);
	if(g_pPlayerManager->DoesExist(playerId) && g_pBlipManager->DoesPlayerBlipExist(playerId))
	{
		g_pBlipManager->TogglePlayerDisplay(playerId, bShow);
		script_pushbool(pVM,true);
		return 1;
	}

	script_pushbool(pVM,false);
	return 1;
}

int CBlipNatives::TogglePlayerShortRange(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -2, &playerId);

	bool bToggle;
	script_getbool(pVM, -1, &bToggle);

	bool bShow = (bToggle != 0);
	if(g_pPlayerManager->DoesExist(playerId) && g_pBlipManager->DoesPlayerBlipExist(playerId))
	{
		g_pBlipManager->TogglePlayerShortRange(playerId, bShow);
		script_pushbool(pVM,true);
		return 1;
	}

	script_pushbool(pVM,false);
	return 1;
}

int CBlipNatives::TogglePlayerBlipDisplayForPlayer(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -3, &playerId);

	EntityId toPlayerId;
	script_getentity(pVM, -2, &toPlayerId);

	bool bToggle;
	script_getbool(pVM, -1, &bToggle);

	bool bShow = (bToggle != 0);
	if(g_pPlayerManager->DoesExist(playerId) && g_pPlayerManager->DoesExist(toPlayerId) && g_pBlipManager->DoesPlayerBlipExist(playerId))
	{
		g_pBlipManager->TogglePlayerDisplayForPlayer(playerId, toPlayerId, bShow);
		script_pushbool(pVM,true);
		return 1;
	}

	script_pushbool(pVM,false);
	return 1;
}

int CBlipNatives::TogglePlayerShortRangeForPlayer(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -3, &playerId);

	EntityId toPlayerId;
	script_getentity(pVM, -2, &toPlayerId);

	bool bToggle;
	script_getbool(pVM, -1, &bToggle);

	bool bShow = (bToggle != 0);
	if(g_pPlayerManager->DoesExist(playerId) && g_pPlayerManager->DoesExist(toPlayerId) && g_pBlipManager->DoesPlayerBlipExist(playerId))
	{
		g_pBlipManager->TogglePlayerShortRangeForPlayer(playerId, toPlayerId, bShow);
		script_pushbool(pVM,true);
		return 1;
	}

	script_pushbool(pVM,false);
	return 1;
}