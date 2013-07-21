//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: VehicleNatives.cpp
// Project: Server.Core
// Author(s): jenksta
//            Einstein
//            Sebihunter
// License: See LICENSE in root directory
//
//==============================================================================

#include "../Natives.h"
#include "Scripting/CScriptingManager.h"
#include "../CClient.h"

extern CClient * g_pClient;

// Vehicle functions

void CVehicleNatives::Register(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterFunction("isVehicleValid", IsValid, 1, "i");
	pScriptingManager->RegisterFunction("getVehicleCoordinates", GetCoordinates, 1, "i");
	//pScriptingManager->RegisterFunction("getVehiclePosition", GetCoordinates, 1, "i"); What is that? Clone? TODO
	pScriptingManager->RegisterFunction("getVehicleRotation", GetRotation, 1, "i");
	pScriptingManager->RegisterFunction("getVehicleColor", GetColor, 1, "i");
	pScriptingManager->RegisterFunction("getVehicleModel", GetModel, 1, "i");
	pScriptingManager->RegisterFunction("getVehicleHealth", GetHealth, 1, "i");
	pScriptingManager->RegisterFunction("getVehicleEngineHealth", GetEngineHealth, 1, "i");
	pScriptingManager->RegisterFunction("getVehicleVelocity", GetVelocity, 1, "i");
	pScriptingManager->RegisterFunction("getVehicleAngularVelocity", GetAngularVelocity, 1, "i");
	pScriptingManager->RegisterFunction("getVehicleLocked", GetLocked, 1, "i");
	pScriptingManager->RegisterFunction("getVehicleDirtLevel", GetDirtLevel, 1, "i");
	pScriptingManager->RegisterFunction("getVehicleSirenState", GetSirenState, 1, "i");
	pScriptingManager->RegisterFunction("isVehicleOccupied", IsOccupied, 1, "i");
	pScriptingManager->RegisterFunction("getVehicleOccupants", GetOccupants, 1, "i");
	pScriptingManager->RegisterFunction("getVehicleEngineState", GetEngineStatus, 1, "i");
	pScriptingManager->RegisterFunction("getVehicleGpsState", GetGpsState, 1, "i");
}

// getVehicleCoordinates(vehicleid)
int CVehicleNatives::GetCoordinates(lua_State * pVM)
{
	EntityId vehicleId;
	script_getentity(pVM, -1, &vehicleId);

	CNetworkVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(vehicleId);

	if(pVehicle)
	{
		CVector3 vecPosition;
		pVehicle->GetPosition(vecPosition);
		CScriptArray args(pVM);
		args.push(vecPosition.fX);
		args.push(vecPosition.fY);
		args.push(vecPosition.fZ);
		//CScriptArgument(args, true).push(pVM);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// getVehicleRotation(vehicleid)
int CVehicleNatives::GetRotation(lua_State * pVM)
{
	EntityId vehicleId;
	script_getentity(pVM, -1, &vehicleId);

	CNetworkVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(vehicleId);

	if(pVehicle)
	{
		CVector3 vecRotation;
		pVehicle->GetRotation(vecRotation);
		CScriptArray args(pVM);
		args.push(vecRotation.fX);
		args.push(vecRotation.fY);
		args.push(vecRotation.fZ);
		//CScriptArgument(args, true).push(pVM);
		return 1;
	}
	script_pushbool(pVM, false);
	return 1;
}

// isVehicleValid(vehicleid)
int CVehicleNatives::IsValid(lua_State * pVM)
{
	EntityId vehicleId;
	script_getentity(pVM, -1, &vehicleId);
	script_pushbool(pVM, g_pClient->GetVehicleManager()->Exists(vehicleId));
	return 1;
}

// getVehicleColor(vehicleid)
int CVehicleNatives::GetColor(lua_State * pVM)
{
	EntityId vehicleId;
	script_getentity(pVM, -1, &vehicleId);

	CNetworkVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(vehicleId);

	if(pVehicle)
	{
		BYTE byteColors[4];
		pVehicle->GetColors(byteColors[0], byteColors[1], byteColors[2], byteColors[3]);

		CScriptArray args(pVM);
		args.push((int)byteColors[0]);
		args.push((int)byteColors[1]);
		args.push((int)byteColors[2]);
		args.push((int)byteColors[3]);
		//CScriptArgument(args, true).push(pVM);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// getVehicleModel(vehicleid)
int CVehicleNatives::GetModel(lua_State * pVM)
{
	EntityId vehicleId;
	script_getentity(pVM, -1, &vehicleId);

	CNetworkVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(vehicleId);

	if(pVehicle)
	{
		script_pushinteger(pVM, g_pClient->GetModelManager()->ModelHashToVehicleId(pVehicle->GetModelInfo()->GetHash()));
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// getVehicleHealth(vehicleid)
int CVehicleNatives::GetHealth(lua_State * pVM)
{
	EntityId vehicleId;
	script_getentity(pVM, -1, &vehicleId);

	CNetworkVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(vehicleId);

	if(pVehicle)
	{
		script_pushinteger(pVM, pVehicle->GetHealth());
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// getVehicleEngineHealth(vehicleid)
int CVehicleNatives::GetEngineHealth(lua_State * pVM)
{
	EntityId vehicleId;
	script_getentity(pVM, -1, &vehicleId);

	CNetworkVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(vehicleId);

	if(pVehicle)
	{
		CLogFile::Printf("Function getVehicleEngineHealth is depreciated: please use getVehicleHealth.");
		script_pushfloat(pVM, (float)pVehicle->GetHealth());
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// getVehicleVelocity(vehicleid)
int CVehicleNatives::GetVelocity(lua_State * pVM)
{
	EntityId vehicleId;
	script_getentity(pVM, -1, &vehicleId);

	CNetworkVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(vehicleId);

	if(pVehicle)
	{
		CVector3 vecMoveSpeed;
		pVehicle->GetMoveSpeed(vecMoveSpeed);
		CScriptArray args(pVM);
		args.push(vecMoveSpeed.fX);
		args.push(vecMoveSpeed.fY);
		args.push(vecMoveSpeed.fZ);
		//CScriptArgument(args, true).push(pVM);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// getVehicleAngularVelocity(vehicleid)
int CVehicleNatives::GetAngularVelocity(lua_State * pVM)
{
	EntityId vehicleId;
	script_getentity(pVM, -1, &vehicleId);

	CNetworkVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(vehicleId);

	if(pVehicle)
	{
		CVector3 vecTurnSpeed;
		pVehicle->GetTurnSpeed(vecTurnSpeed);
		CScriptArray args(pVM);
		args.push(vecTurnSpeed.fX);
		args.push(vecTurnSpeed.fY);
		args.push(vecTurnSpeed.fZ);
		//CScriptArgument(args, true).push(pVM);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// getVehicleLocked(vehicleid)
int CVehicleNatives::GetLocked(lua_State * pVM)
{
	EntityId vehicleId;
	script_getentity(pVM, -1, &vehicleId);

	CNetworkVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(vehicleId);

	if(pVehicle)
	{
		script_pushinteger(pVM, pVehicle->GetDoorLockState());
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// getVehicleDirtLevel(vehicleid)
int CVehicleNatives::GetDirtLevel(lua_State * pVM)
{
	EntityId vehicleId;
	script_getentity(pVM, -1, &vehicleId);

	CNetworkVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(vehicleId);

	if(pVehicle)
	{
		script_pushfloat(pVM, pVehicle->GetDirtLevel());
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// getVehicleSirenState(vehicleid)
int CVehicleNatives::GetSirenState(lua_State * pVM)
{
	EntityId vehicleId;

	script_getentity(pVM, -1, &vehicleId);

	CNetworkVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(vehicleId);

	if(pVehicle)
	{
		script_pushbool(pVM, pVehicle->GetSirenState());
		return 1;
	}

	//script_pushbool(pVM, false);
	return 0;
}

// isVehicleOccupied(vehicleid)
int CVehicleNatives::IsOccupied(lua_State * pVM)
{
	EntityId vehicleId;
	script_getentity(pVM, -1, &vehicleId);

	CNetworkVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(vehicleId);

	if(pVehicle)
	{
		script_pushbool(pVM, pVehicle->IsOccupied());
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// getVehicleOccupants(vehicleid)
int CVehicleNatives::GetOccupants(lua_State * pVM)
{
	EntityId vehicleId;
	script_getentity(pVM, -1, &vehicleId);

	CNetworkVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(vehicleId);

	if(pVehicle)
	{
		CScriptArray args(pVM);

		for(BYTE i = 1; i < (MAX_VEHICLE_PASSENGERS + 1); i++)
		{
			args.push((int)i);
			CNetworkPlayer * pOccupant = pVehicle->GetOccupant(i-1);
			args.push(pOccupant ? (int)pOccupant->GetPlayerId() : (int)INVALID_ENTITY_ID);
		}

		//CScriptArgument(args, false).push(pVM);
		return 1;
	}

	//script_pushbool(pVM, false);
	return 0;
}

// getVehicleEngineState(vehicleid)
int CVehicleNatives::GetEngineStatus(lua_State * pVM)
{
	EntityId vehicleId;

	script_getentity(pVM, -1, &vehicleId);

	CNetworkVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(vehicleId);

	if(pVehicle)
	{
		script_pushbool(pVM, pVehicle->GetEngineState());
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// getVehicleGpsState(vehicleid)
int CVehicleNatives::GetGpsState(lua_State *pVM)
{
	EntityId vehicleId;

	script_getentity(pVM, -1, &vehicleId);

	CNetworkVehicle * pVehicle = g_pClient->GetVehicleManager()->Get(vehicleId);

	if(pVehicle)
	{
		script_pushbool(pVM, pVehicle->GetVehicleGPSState());
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}
