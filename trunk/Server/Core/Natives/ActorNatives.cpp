//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: ActorNatives.cpp
// Project: Server.Core
// Author(s): jenksta
//            Sebihunter
// License: See LICENSE in root directory
//
//==============================================================================

#include "../Natives.h"
#include "Scripting/CScriptingManager.h"
#include "../CActorManager.h"
#include "../CNetworkManager.h"
#include "../CPlayerManager.h"
#include <CLogFile.h>

extern CActorManager * g_pActorManager;
extern CNetworkManager * g_pNetworkManager;
extern CPlayerManager * g_pPlayerManager;

// Actor functions

void CActorNatives::Register(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterFunction("createActor", Create, 5, "iffff");
	pScriptingManager->RegisterFunction("deleteActor", Delete, 1, "i");
	pScriptingManager->RegisterFunction("isActorValid",IsValid,1,"i");
	pScriptingManager->RegisterFunction("getActorModel", GetModel, 1, "i");
	pScriptingManager->RegisterFunction("getActorCount",GetCount,0,NULL);
	pScriptingManager->RegisterFunction("setActorCoordinates", SetCoordinates, 4, "ifff");
	pScriptingManager->RegisterFunction("getActorCoordinates", GetCoordinates, 1, "i");
	pScriptingManager->RegisterFunction("setActorPosition", SetCoordinates, 4, "ifff");
	pScriptingManager->RegisterFunction("setActorHeading", SetHeading, 2, "if");
	pScriptingManager->RegisterFunction("getActorHeading", GetHeading, 1, "i");
	pScriptingManager->RegisterFunction("actorWalkToCoordinatesForPlayer", WalkToCoordinates, 6, "iifffi");
	pScriptingManager->RegisterFunction("setActorName",SetName, 2, "is");
	pScriptingManager->RegisterFunction("getActorName",GetName, 1, "i");
	pScriptingManager->RegisterFunction("toggleActorNametag", ToggleNametag, 2, "ib");
	pScriptingManager->RegisterFunction("toggleActorBlip", ToggleBlip, 2, "ib");
	pScriptingManager->RegisterFunction("setActorNametagColor",SetColor, 2, "ii");
	pScriptingManager->RegisterFunction("getActorNametagColor",GetColor, 1, "i");
	pScriptingManager->RegisterFunction("toggleActorFrozen",ToggleFrozen, 2, "ib");
	pScriptingManager->RegisterFunction("toggleActorHelmet",ToggleHelmet, 2, "ib");
	pScriptingManager->RegisterFunction("warpActorIntoVehicle",WarpIntoVehicle, 3, "iii");
	pScriptingManager->RegisterFunction("removeActorFromVehicle",RemoveFromVehicle, 1, "i");
	pScriptingManager->RegisterFunction("deleteAllActors",DeleteAll, 0, NULL);
	pScriptingManager->RegisterFunction("driveActorToCoordinates", DriveToCoordinates, 4,"ifff");
	pScriptingManager->RegisterFunction("forceAnimationAtActor", ForceAnim, 3, "iss");
	pScriptingManager->RegisterFunction("sayActorSpeech", SaySpeech, 3, "iss");
}

// createActor(modelhash, x, y, z, r)
int CActorNatives::Create(lua_State * pVM)
{
	int iModelId;
	CVector3 vecPosition;
	float fHeading;
	script_getinteger(pVM, -5, &iModelId);
	script_getvector3(pVM, -4, &vecPosition);
	script_getfloat(pVM, -1, &fHeading);
	script_pushentity(pVM, g_pActorManager->Create(iModelId, vecPosition, fHeading));
	return 1;
}

// deleteActor(actorid)
int CActorNatives::Delete(lua_State * pVM)
{
	EntityId actorId;
	script_getentity(pVM, -1, &actorId);

	if(g_pActorManager->DoesExist(actorId))
	{
		g_pActorManager->Delete(actorId);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// isActorValid(actorid)
int CActorNatives::IsValid(lua_State * pVM)
{
	EntityId actorId;
	script_getentity(pVM, -1, &actorId);

	if(g_pActorManager->DoesExist(actorId))
	{
		script_pushbool(pVM, true);
		return 1;
	}
	else
	{
		script_pushbool(pVM, false);
		return 1;
	}
	return 1;
}

// setActorCoordinates(actorid, x, y, z)
int CActorNatives::SetCoordinates(lua_State * pVM)
{
	EntityId actorId;
	CVector3 vecPosition;
	script_getentity(pVM, -4, &actorId);
	script_getvector3(pVM, -3, &vecPosition);

	if(g_pActorManager->DoesExist(actorId))
	{
		g_pActorManager->SetPosition(actorId, vecPosition);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// getActorCoordinates(actorid)
int CActorNatives::GetCoordinates(lua_State * pVM)
{
	EntityId actorId;
    script_getentity(pVM, -1, &actorId);

    if(g_pActorManager->DoesExist(actorId))
    {
		CVector3 vecPosition = g_pActorManager->GetPosition(actorId);
		CScriptArray args(pVM);
		args.push(vecPosition.fX);
		args.push(vecPosition.fY);
		args.push(vecPosition.fZ);
		//CScriptArgument(args, true).push(pVM);//script_pusharg(pVM, CScriptArgument(args, true));
		return 1;
	}
    script_pushbool(pVM, false);
    return 1;
}

// setActorHeading(actorid, r)
int CActorNatives::SetHeading(lua_State * pVM)
{
	EntityId actorId;
	float fHeading;
	script_getentity(pVM, -2, &actorId);
	script_getfloat(pVM, -1, &fHeading);

	if(g_pActorManager->DoesExist(actorId))
	{
		g_pActorManager->SetHeading(actorId, fHeading);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// getActorHeading(actorid)
int CActorNatives::GetHeading(lua_State * pVM)
{
	EntityId actorId;
	script_getentity(pVM, -1, &actorId);

	if(g_pActorManager->DoesExist(actorId))
	{
		float fHeading = g_pActorManager->GetHeading(actorId);
		script_pushfloat(pVM, fHeading);
		return 1;
	}
	script_pushbool(pVM, false);
	return 1;
}

// getActorModel(actorid)
int CActorNatives::GetModel(lua_State * pVM)
{
	EntityId actorId;
	script_getentity(pVM, -1, &actorId);

	if(g_pActorManager->DoesExist(actorId))
	{
		int aModel = g_pActorManager->GetModel(actorId);
		script_pushinteger(pVM, aModel);
		return 1;
	}
	script_pushbool(pVM, false);
	return 1;
}

// getActorCount()
int CActorNatives::GetCount(lua_State * pVM)
{
	if(g_pActorManager)
	{
		int Count = g_pActorManager->GetActorCount();
		script_pushinteger(pVM, Count);
		return 1;
	}
	script_pushbool(pVM, false);
	return 1;
}

// actorWalkToCoordinatesForPlayer(playerid, actorid, x, y, z)
int CActorNatives::WalkToCoordinates(lua_State * pVM)
{
	EntityId playerId;
	EntityId actorId;
	CVector3 vecPosition;
	int iType;
	script_getentity(pVM, -6, &playerId);
	script_getentity(pVM, -5, &actorId);
	script_getvector3(pVM, -4, &vecPosition);
	script_getinteger(pVM, -1, &iType);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write(actorId);
		bsSend.Write(vecPosition);
		bsSend.Write((int)iType);
		g_pNetworkManager->RPC(RPC_ScriptingActorWalkToCoordinates, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// setActorName(actorid, name)
int CActorNatives::SetName(lua_State * pVM)
{
	EntityId actorId;
	const char * name = NULL;
	script_getentity(pVM, -2, &actorId);
	script_getstring(pVM, -1, &name);

	if(g_pActorManager->DoesExist(actorId))
	{
		g_pActorManager->SetActorName(actorId, name);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// getActorName(actorid)
int CActorNatives::GetName(lua_State * pVM)
{
	EntityId actorId;
	script_getentity(pVM, -1, &actorId);

	if(g_pActorManager->DoesExist(actorId))
	{
		script_pushstring(pVM, g_pActorManager->GetActorName(actorId));
		return 1;
	}
	script_pushbool(pVM, false);
	return 1;
}

// toggleActorNametag(actorid, toggle)
int CActorNatives::ToggleNametag(lua_State * pVM)
{
	EntityId actorId;
	bool show;
	script_getentity(pVM, -2, &actorId);
	script_getbool(pVM, -1, &show);
		
	if(g_pActorManager->DoesExist(actorId))
	{
		bool bToggle = (show != 0);
		g_pActorManager->ToggleNametag(actorId, bToggle);
		return 1;
	}
	script_pushbool(pVM, false);
	return 1;
}

// toggleActorBlip(actorid, toggle)
int CActorNatives::ToggleBlip(lua_State * pVM)
{
	EntityId actorId;
	bool show;
	script_getentity(pVM, -2, &actorId);
	script_getbool(pVM, -1, &show);
		
	if(g_pActorManager->DoesExist(actorId))
	{
		bool bToggle = (show != 0);
		g_pActorManager->ToggleBlip(actorId, bToggle);
		return 1;
	}
	script_pushbool(pVM, false);
	return 1;
}
// setActorNametagColor(actorid, color)
int CActorNatives::SetColor(lua_State * pVM)
{
	EntityId actorId;
	int color = 0xFFFFFFFF;
	script_getentity(pVM, -2, &actorId);
	script_getinteger(pVM, -1, &color);

	if(g_pActorManager->DoesExist(actorId))
	{
		g_pActorManager->SetColor(actorId,color);
		return 1;
	}
	script_pushbool(pVM,false);
	return 1;
}

// getActorNametagColor(actorid)
int CActorNatives::GetColor(lua_State * pVM)
{
	EntityId actorId;
	script_getentity(pVM, -1, &actorId);

	if(g_pActorManager->DoesExist(actorId))
	{
		script_pushinteger(pVM, g_pActorManager->GetColor(actorId));
		return 1;
	}
	script_pushbool(pVM,false);
	return 1;
}

// toggleActorFrozen(actorid, toggle)
int CActorNatives::ToggleFrozen(lua_State * pVM)
{
	EntityId actorId;
	bool frozen;
	script_getentity(pVM, -2, &actorId);
	script_getbool(pVM, -1, &frozen);
		
	if(g_pActorManager->DoesExist(actorId))
	{
		bool bToggle = (frozen != 0);
		g_pActorManager->ToggleFrozen(actorId, bToggle);
		return 1;
	}
	script_pushbool(pVM, false);
	return 1;
}

// toggleActorHelmet(actorid, toggle)
int CActorNatives::ToggleHelmet(lua_State * pVM)
{
	EntityId actorId;
	bool helmet;
	script_getentity(pVM, -2, &actorId);
	script_getbool(pVM, -1, &helmet);
		
	if(g_pActorManager->DoesExist(actorId))
	{
		bool bToggle = (helmet != 0);
		g_pActorManager->ToggleHelmet(actorId, bToggle);
		return 1;
	}
	script_pushbool(pVM, false);
	return 1;
}

// warpActorIntoVehicle(actorid, vehicleid, seatid)
int CActorNatives::WarpIntoVehicle(lua_State * pVM)
{
	EntityId actorId;
	EntityId vehicleId;
	int seatid;
	
	script_getentity(pVM, -3, &actorId);
	script_getentity(pVM, -2, &vehicleId);
	script_getinteger(pVM, -1, &seatid);

	if(g_pActorManager->DoesExist(actorId))
	{
		if(seatid >= 0 &&  seatid <= 3)
		{
			g_pActorManager->WarpIntoVehicle(actorId,vehicleId,(int)seatid);
			script_pushbool(pVM, true);
			return 1;
		}
		else
		{
			CLogFile::Printf("Can't warp actor %d on the seat %d from the vehicle %d(Seats are only supported from 0 to 3(driver and passenger))",actorId,seatid,vehicleId);
			script_pushbool(pVM, false);
		}
	}
	script_pushbool(pVM, false);
	return 1;
}

// removeActorFromVehicle(actorid)
int CActorNatives::RemoveFromVehicle(lua_State * pVM)
{
	EntityId actorId;
	script_getentity(pVM, -1, &actorId);

	if(g_pActorManager->DoesExist(actorId))
	{
		g_pActorManager->RemoveFromVehicle(actorId);
		return 1;
	}
	script_pushbool(pVM, false);
	return 1;
}

int CActorNatives::DeleteAll(lua_State * pVM)
{
	for(EntityId x = 0; x < MAX_ACTORS; x++)
	{
		if(g_pActorManager->DoesExist(x))
		{
			g_pActorManager->Delete(x);
		}
	}

	script_pushbool(pVM, true);
	return 1;
}

int CActorNatives::ForceAnim(lua_State * pVM)
{
	EntityId actorId;
	script_getentity(pVM,-3, &actorId);

	const char * szGroup = NULL;
	script_getstring(pVM,-2,&szGroup);

	const char * szAnim = NULL;
	script_getstring(pVM,-1,&szAnim);


	if(g_pActorManager->DoesExist(actorId))
	{
		CBitStream bsSend;
		bsSend.Write(actorId);
		bsSend.Write(String(szGroup));
		bsSend.Write(String(szAnim));
		g_pNetworkManager->RPC(RPC_ScriptingForceActorAnimation, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
	}
	return 1;
}

int CActorNatives::DriveToCoordinates(lua_State * pVM)
{
	EntityId actorId;
	script_getentity(pVM,-4, &actorId);

	CVector3 vecPos;
	script_getfloat(pVM, -3, &vecPos.fX);
	script_getfloat(pVM, -2, &vecPos.fY);
	script_getfloat(pVM, -1, &vecPos.fZ);

	if(g_pActorManager->DoesExist(actorId))
	{
		g_pActorManager->DriveToCoordinates(actorId, vecPos, CVector3());
		if(!g_pActorManager->UpdateDrivePos(actorId, vecPos, CVector3(), false))
			CLogFile::Printf("Failed to set actor's %d state to driving(not in a vehicle!)",actorId);
	}
	return 1;
}

int CActorNatives::SaySpeech(lua_State * pVM)
{
	EntityId actorId;
	script_getentity(pVM, -3, &actorId);
	
	const char * szVoice;
	script_getstring(pVM, -2, &szVoice);

	const char * szText;
	script_getstring(pVM, -1, &szText);

	if(g_pActorManager->DoesExist(actorId))
	{
		CBitStream bsSend;
		bsSend.Write(actorId);
		bsSend.Write(String(szVoice));
		bsSend.Write(String(szText));
		g_pNetworkManager->RPC(RPC_ScriptingActorSaySpeech, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
		script_pushbool(pVM, true);
		return 1;
	}
	script_pushbool(pVM, false);
	return 1;
}
