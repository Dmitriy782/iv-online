//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: VehicleNatives.cpp
// Project: Server.Core
// Author(s): jenksta
//            Einstein
// License: See LICENSE in root directory
//
//==============================================================================

#include "../Natives.h"
#include "../CNetworkManager.h"
#include "Scripting/CScriptingManager.h"
#include "../CVehicleManager.h"
#include "../CPlayer.h"

extern CVehicleManager * g_pVehicleManager;
extern CNetworkManager * g_pNetworkManager;

// Vehicle functions

void CVehicleNatives::Register(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterFunction("createVehicle", Create, -1, NULL);
	pScriptingManager->RegisterFunction("deleteVehicle", Delete, 1, "i");
	pScriptingManager->RegisterFunction("setVehicleCoordinates", SetCoordinates, 4, "ifff");
	pScriptingManager->RegisterFunction("getVehicleCoordinates", GetCoordinates, 1, "i");
	pScriptingManager->RegisterFunction("setVehiclePosition", SetCoordinates, 4, "ifff");
	pScriptingManager->RegisterFunction("getVehiclePosition", GetCoordinates, 1, "i");
	pScriptingManager->RegisterFunction("setVehicleRotation", SetRotation, 4, "ifff");
	pScriptingManager->RegisterFunction("setVehicleSirenState", SetSirenState, 2, "ib");
	pScriptingManager->RegisterFunction("getVehicleSirenState", GetSirenState, 1, "i");
	pScriptingManager->RegisterFunction("setVehicleDirtLevel", SetDirtLevel, 2, "if");
	pScriptingManager->RegisterFunction("getVehicleDirtLevel", GetDirtLevel, 1, "i");
	pScriptingManager->RegisterFunction("soundVehicleHorn", SoundHorn, 2, "ii");
	pScriptingManager->RegisterFunction("getVehicleRotation", GetRotation, 1, "i");
	pScriptingManager->RegisterFunction("isVehicleValid", IsValid, 1, "i");
	pScriptingManager->RegisterFunction("setVehicleColor", SetColor, -1, NULL);
	pScriptingManager->RegisterFunction("getVehicleColor", GetColor, 1, "i");
	pScriptingManager->RegisterFunction("getVehicleModel", GetModel, 1, "i");
	pScriptingManager->RegisterFunction("setVehicleHealth", SetHealth, 2, "ii");
	pScriptingManager->RegisterFunction("getVehicleHealth", GetHealth, 1, "i");
	pScriptingManager->RegisterFunction("setVehicleEngineHealth", SetEngineHealth, 2, "ii");
	pScriptingManager->RegisterFunction("getVehicleEngineHealth", GetEngineHealth, 1, "i");
	pScriptingManager->RegisterFunction("setVehicleVelocity", SetVelocity, 4, "ifff");
	pScriptingManager->RegisterFunction("getVehicleVelocity", GetVelocity, 1, "i");
	pScriptingManager->RegisterFunction("setVehicleAngularVelocity", SetAngularVelocity, 4, "ifff");
	pScriptingManager->RegisterFunction("getVehicleAngularVelocity", GetAngularVelocity, 1, "i");
	pScriptingManager->RegisterFunction("respawnVehicle", Respawn, 1, "i");
	pScriptingManager->RegisterFunction("isVehicleOccupied", IsOccupied, 1, "i");
	pScriptingManager->RegisterFunction("getVehicleOccupants", GetOccupants, 1, "i");
	pScriptingManager->RegisterFunction("setVehicleLocked", SetLocked, 2, "ii");
	pScriptingManager->RegisterFunction("getVehicleLocked", GetLocked, 1, "i");
	pScriptingManager->RegisterFunction("setVehicleIndicators", SetIndicators, 5, "ibbbb");
	pScriptingManager->RegisterFunction("getVehicleIndicators", GetIndicators, 1, "i");
	pScriptingManager->RegisterFunction("setVehicleComponent", SetComponent, 3, "iib");
	pScriptingManager->RegisterFunction("getVehicleComponents", GetComponents, 1, "i");
	pScriptingManager->RegisterFunction("resetVehicleComponents", ResetComponents, 1, "i");
	pScriptingManager->RegisterFunction("setVehicleVariation", SetVariation, 2, "ii");
	pScriptingManager->RegisterFunction("getVehicleVariation", GetVariation, 1, "i");
	pScriptingManager->RegisterFunction("setVehicleTaxiLights", SwitchTaxiLights, 2, "ib");
	pScriptingManager->RegisterFunction("getVehicleTaxiLights", GetTaxiLights, 1, "i");
	pScriptingManager->RegisterFunction("controlCarDoors", ControlCar, 4, "iibf");
	pScriptingManager->RegisterFunction("setVehicleEngineState", SetEngineStatus, 2, "ib");
	pScriptingManager->RegisterFunction("getVehicleEngineState", GetEngineStatus, 1, "i");
	pScriptingManager->RegisterFunction("setVehicleLights",SetLights,2,"ib");
	pScriptingManager->RegisterFunction("getVehicleLights",GetLights,1,"i");
	pScriptingManager->RegisterFunction("repairVehicleWindows", RepairWindows, 1, "i");
	pScriptingManager->RegisterFunction("repairVehicleWheels", RepairWheels, 1, "i");
	pScriptingManager->RegisterFunction("setVehicleGpsState", SetGpsState, 2, "ib");
	pScriptingManager->RegisterFunction("getVehicleGpsState", GetGpsState, 1, "i");
	pScriptingManager->RegisterFunction("setVehicleAlarm", SetAlarm, 2, "ii");
	pScriptingManager->RegisterFunction("markVehicleAsActorVehicle", MarkVehicle, 2, "ib");
	pScriptingManager->RegisterFunction("repairVehicle", RepairVehicle, 1, "i");

	pScriptingManager->RegisterFunction("setVehicleRespawnDelay", SetRespawnDelay, 2, "ii");
	pScriptingManager->RegisterFunction("getVehicleRespawnDelay", GetRespawnDelay, -1, NULL);
	pScriptingManager->RegisterFunction("setVehicleDimension", SetDimension, 2, "ii");
	pScriptingManager->RegisterFunction("getVehicleDimension", GetDimension, 1, "i");
}

// createVehicle(model, x, y, z, rx, ry, rz, color1, color2, color3, color4, respawn_delay)
// WIP: Rotation and colors optional
int CVehicleNatives::Create(lua_State * pVM)
{
	int iModelId;
	CVector3 vecPosition;
	CVector3 vecRotation;
	int respawn_delay = -1;
	int color1 = 0, color2 = 0, color3 = 0, color4 = 0;
	script_getinteger(pVM, 1, &iModelId);

	if(iModelId < 0 || iModelId == 41 || iModelId == 96 || iModelId == 107 || iModelId == 111 || iModelId > 123)
	{
		#ifdef IVMP_DEBUG
			CLogFile::Printf("Invalid vehicle model (%d)", iModelId);
		#endif
		script_pushinteger(pVM, INVALID_ENTITY_ID);
		return 1;
	}
	
	script_getvector3(pVM, 2, &vecPosition); // 3.4.5
	
	if(script_gettop(pVM) >= 7)
	{
		script_getvector3(pVM, 5, &vecRotation); // 6.7.8

		if(script_gettop(pVM) >= 9)
		{
			script_getinteger(pVM, 8, &color1);
			script_getinteger(pVM, 9, &color2);

			if(script_gettop(pVM) >= 11)
			{
				script_getinteger(pVM, 10, &color3);
				script_getinteger(pVM, 11, &color4);

				if(script_gettop(pVM) >= 12)
					script_getinteger(pVM, 12, &respawn_delay);
			}
		}
	}

	EntityId vehicleId = g_pVehicleManager->Add(iModelId, vecPosition, vecRotation, color1, color2, color3, color4, respawn_delay);
	script_pushinteger(pVM, vehicleId);
	return 1;
}

// deleteVehicle(vehicleid)
int CVehicleNatives::Delete(lua_State * pVM)
{
	int vehicleid;
	script_getinteger(pVM, 1, &vehicleid);
	if(g_pVehicleManager->DoesExist(vehicleid))
	{
		g_pVehicleManager->Remove(vehicleid);
		script_pushbool(pVM, true);
		return 1;
	}
	script_pushbool(pVM, false);
	return 1;
}

// setVehicleCoordinates(vehicleid, x, y, z)
int CVehicleNatives::SetCoordinates(lua_State * pVM)
{
	int vehicleid;
	float x, y, z;
	script_getinteger(pVM, -4, &vehicleid);
	script_getfloat(pVM, -3, &x);
	script_getfloat(pVM, -2, &y);
	script_getfloat(pVM, -1, &z);

	if(g_pVehicleManager->DoesExist(vehicleid))
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleid);
		
		if(pVehicle)
		{
			pVehicle->SetPosition(CVector3(x, y, z));
			script_pushbool(pVM, true);
			return 1;
		}
	}

	script_pushbool(pVM, false);
	return 1;
}

// getVehicleCoordinates(vehicleid)
int CVehicleNatives::GetCoordinates(lua_State * pVM)
{
	int vehicleid;
	script_getinteger(pVM, -1, &vehicleid);
	if(g_pVehicleManager->DoesExist(vehicleid))
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleid);

		if(pVehicle)
		{
			CVector3 vecPosition;
			pVehicle->GetPosition(vecPosition);
			CScriptArray args(pVM);
			args.push(vecPosition.fX);
			args.push(vecPosition.fY);
			args.push(vecPosition.fZ);
			//script_pusharg(pVM, CScriptArgument(args, true));
			//CScriptArgument(args, true).push(pVM);
			return 1;
		}
	}

	script_pushbool(pVM, false);
	return 1;
}

// setVehicleRotation(vehicleid, rotation)
int CVehicleNatives::SetRotation(lua_State * pVM)
{
	EntityId vehicleId;
	CVector3 vecRotation;
	script_getentity(pVM, -4, &vehicleId);
	script_getvector3(pVM, -3, &vecRotation);

	if(g_pVehicleManager->DoesExist(vehicleId))
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

		if(pVehicle)
		{
			pVehicle->SetRotation(vecRotation);
			script_pushbool(pVM, true);
			return 1;
		}
	}

	script_pushbool(pVM, false);
	return 1;
}

// setVehicleDirtLevel(vehicleid, level)
int CVehicleNatives::SetDirtLevel(lua_State * pVM)
{
	int vehicleid;

	float fLevel;

	script_getinteger(pVM, -2, &vehicleid);

	script_getfloat(pVM, -1, &fLevel);

	if(g_pVehicleManager->DoesExist(vehicleid))
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleid);

		if(pVehicle)
		{
			pVehicle->SetDirtLevel(fLevel);

			script_pushbool(pVM, true);

			return 1;
		}
	}
	script_pushbool(pVM, false);
	return 1;
}

int CVehicleNatives::GetDirtLevel(lua_State * pVM)
{
	EntityId vehicleId;
	script_getentity(pVM, -1, &vehicleId);

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		script_pushfloat(pVM, pVehicle->GetDirtLevel());
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// setVehicleSirenState(vehicleid, state)
int CVehicleNatives::SetSirenState(lua_State * pVM)
{
	EntityId vehicleId;
	bool sqbState;

	script_getentity(pVM, -2, &vehicleId);
	script_getbool(pVM, -1, &sqbState);

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		pVehicle->SetSirenState(sqbState != 0);
		script_pushbool(pVM, true);
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

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		script_pushbool(pVM, pVehicle->GetSirenState());
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// soundVehicleHorn(vehicleid, duration)
int CVehicleNatives::SoundHorn(lua_State * pVM)
{
	EntityId vehicleId;
	int iDuration;

	script_getentity(pVM, -2, &vehicleId);
	script_getinteger(pVM, -1, &iDuration);

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		pVehicle->SoundHorn(iDuration);
		script_pushbool(pVM, true);
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

	if(g_pVehicleManager->DoesExist(vehicleId))
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

		if(pVehicle)
		{
			CVector3 vecRotation;
			pVehicle->GetRotation(vecRotation);
			CScriptArray args(pVM);
			args.push(vecRotation.fX);
			args.push(vecRotation.fY);
			args.push(vecRotation.fZ);
			//script_pusharg(pVM, CScriptArgument(args, true));
			//CScriptArgument(args, true).push(pVM);
			return 1;
		}
	}

	script_pushbool(pVM, false);
	return 1;
}

// isVehicleValid(vehicleid)
int CVehicleNatives::IsValid(lua_State * pVM)
{
	int vehicleid;
	script_getinteger(pVM, -1, &vehicleid);
	script_pushbool(pVM, g_pVehicleManager->DoesExist(vehicleid));
	return 1;
}

// setVehicleColor(vehicleid, color1, color2, color3, color4)
// TODO: Only require two colors
int CVehicleNatives::SetColor(lua_State * pVM)
{
	int vehicleid;
	int colors[4];
	script_getinteger(pVM, -5, &vehicleid);
	script_getinteger(pVM, -4, &colors[0]);
	script_getinteger(pVM, -3, &colors[1]);
	script_getinteger(pVM, -2, &colors[2]);
	script_getinteger(pVM, -1, &colors[3]);
	if(g_pVehicleManager->DoesExist(vehicleid))
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleid);

		if(pVehicle)
		{
			pVehicle->SetColors(colors[0], colors[1], colors[2], colors[3]);
			script_pushbool(pVM, true);
			return 1;
		}
	}
	script_pushbool(pVM, false);
	return 1;
}

// getVehicleColor(vehicleid)
int CVehicleNatives::GetColor(lua_State * pVM)
{
	int vehicleid;
	script_getinteger(pVM, -1, &vehicleid);
	if(g_pVehicleManager->DoesExist(vehicleid))
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleid);

		if(pVehicle)
		{
			BYTE byteColors[4];
			pVehicle->GetColors(byteColors[0], byteColors[1], byteColors[2], byteColors[3]);
			CScriptArray args(pVM);
			args.push((int)byteColors[0]);
			args.push((int)byteColors[1]);
			args.push((int)byteColors[2]);
			args.push((int)byteColors[3]);
			//script_pusharg(pVM, CScriptArgument(args, true));
			//CScriptArgument(args, true).push(pVM);
			return 1;
		}
	}

	script_pushbool(pVM, false);
	return 1;
}

// setVehicleRespawnDelay(vehicleid, respawn_delay)
int CVehicleNatives::SetRespawnDelay(lua_State * pVM)
{
	int vehicleid;
	int respawn_delay;
	script_getinteger(pVM, -2, &vehicleid);
	script_getinteger(pVM, -1, &respawn_delay);
	if(g_pVehicleManager->DoesExist(vehicleid))
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleid);

		if(pVehicle)
		{
			pVehicle->SetRespawnDelay(respawn_delay);
			script_pushbool(pVM, true);
			return 1;
		}
	}
	script_pushbool(pVM, false);
	return 1;
}

// getVehicleRespawnDelay(vehicleid)
int CVehicleNatives::GetRespawnDelay(lua_State * pVM)
{
	int vehicleid;
	script_getinteger(pVM, -1, &vehicleid);
	if(g_pVehicleManager->DoesExist(vehicleid))
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleid);

		if(pVehicle)
		{
			script_pushinteger(pVM, pVehicle->GetRespawnDelay());
			return 1;
		}
	}
	script_pushbool(pVM, false);
	return 1;
}

// getVehicleModel(vehicleid)
int CVehicleNatives::GetModel(lua_State * pVM)
{
	int vehicleid;
	script_getinteger(pVM, -1, &vehicleid);
	if(g_pVehicleManager->DoesExist(vehicleid))
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleid);

		if(pVehicle)
		{
			script_pushinteger(pVM, pVehicle->GetModel());
			return 1;
		}
	}
	script_pushbool(pVM, false);
	return 1;
}

// setVehicleHealth(vehicleid, health)
int CVehicleNatives::SetHealth(lua_State * pVM)
{
	int vehicleid;
	int health;
	script_getinteger(pVM, -2, &vehicleid);
	script_getinteger(pVM, -1, &health);
	if(g_pVehicleManager->DoesExist(vehicleid))
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleid);

		if(pVehicle)
		{
			pVehicle->SetHealth(health);
			script_pushbool(pVM, true);
			return 1;
		}
	}
	script_pushbool(pVM, false);
	return 1;
}

// getVehicleHealth(vehicleid)
int CVehicleNatives::GetHealth(lua_State * pVM)
{
	int vehicleid;
	script_getinteger(pVM, -1, &vehicleid);
	if(g_pVehicleManager->DoesExist(vehicleid))
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleid);

		if(pVehicle)
		{
			script_pushinteger(pVM, pVehicle->GetHealth());
			return 1;
		}
	}
	script_pushbool(pVM, false);
	return 1;
}

// setVehicleEngineHealth(vehicleid, enginehealth)
int CVehicleNatives::SetEngineHealth(lua_State * pVM)
{
	int vehicleid;
	int health;
	script_getinteger(pVM, -2, &vehicleid);
	script_getinteger(pVM, -1, &health);
	if(g_pVehicleManager->DoesExist(vehicleid))
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleid);

		if(pVehicle)
		{
			CLogFile::Printf("Function setVehicleEngineHealth is depreciated: please use setVehicleHealth.");
			pVehicle->SetHealth(health);
			script_pushbool(pVM, true);
			return 1;
		}
	}
	script_pushbool(pVM, false);
	return 1;
}

// getVehicleEngineHealth(vehicleid)
int CVehicleNatives::GetEngineHealth(lua_State * pVM)
{
	int vehicleid;
	script_getinteger(pVM, -1, &vehicleid);
	if(g_pVehicleManager->DoesExist(vehicleid))
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleid);

		if(pVehicle)
		{
			CLogFile::Printf("Function getVehicleEngineHealth is depreciated: please use getVehicleHealth.");
			script_pushinteger(pVM, pVehicle->GetHealth());
			return 1;
		}
	}
	script_pushbool(pVM, false);
	return 1;
}

// setVehicleVelocity(vehicleid, x, y, z)
int CVehicleNatives::SetVelocity(lua_State * pVM)
{
	int vehicleid;
	CVector3 vecMoveSpeed;
	script_getinteger(pVM, -4, &vehicleid);
	script_getvector3(pVM, -3, &vecMoveSpeed);

	if(g_pVehicleManager->DoesExist(vehicleid))
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleid);

		if(pVehicle && pVehicle->GetDriver())
		{
			if(pVehicle->GetDriver()->GetVehicle()->GetVehicleId() == pVehicle->GetVehicleId()) {
				pVehicle->SetMoveSpeed(vecMoveSpeed);
				script_pushbool(pVM, true);
				return 1;
			}
		}
	}

	script_pushbool(pVM, false);
	return 1;
}

// getVehicleVelocity(vehicleid)
int CVehicleNatives::GetVelocity(lua_State * pVM)
{
	int vehicleid;
	script_getinteger(pVM, -1, &vehicleid);

	if(g_pVehicleManager->DoesExist(vehicleid))
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleid);

		if(pVehicle)
		{
			CVector3 vecMoveSpeed;
			pVehicle->GetMoveSpeed(vecMoveSpeed);
			CScriptArray args(pVM);
			args.push(vecMoveSpeed.fX);
			args.push(vecMoveSpeed.fY);
			args.push(vecMoveSpeed.fZ);
			//script_pusharg(pVM, CScriptArgument(args, true));
			//CScriptArgument(args, true).push(pVM);
			return 1;
		}
	}

	script_pushbool(pVM, false);
	return 1;
}

// setVehicleAngularVelocity(vehicleid, x, y, z)
int CVehicleNatives::SetAngularVelocity(lua_State * pVM)
{
	int vehicleid;
	CVector3 vecTurnSpeed;
	script_getinteger(pVM, -4, &vehicleid);
	script_getvector3(pVM, -3, &vecTurnSpeed);

	if(g_pVehicleManager->DoesExist(vehicleid))
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleid);

		if(pVehicle)
		{
			pVehicle->SetTurnSpeed(vecTurnSpeed);
			script_pushbool(pVM, true);
			return 1;
		}
	}

	script_pushbool(pVM, false);
	return 1;
}

// getVehicleAngularVelocity(vehicleid)
int CVehicleNatives::GetAngularVelocity(lua_State * pVM)
{
	int vehicleid;
	script_getinteger(pVM, -1, &vehicleid);

	if(g_pVehicleManager->DoesExist(vehicleid))
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleid);

		if(pVehicle)
		{
			CVector3 vecTurnSpeed;
			pVehicle->GetTurnSpeed(vecTurnSpeed);
			CScriptArray args(pVM);
			args.push(vecTurnSpeed.fX);
			args.push(vecTurnSpeed.fY);
			args.push(vecTurnSpeed.fZ);
			//script_pusharg(pVM, CScriptArgument(args, true));
			//CScriptArgument(args, true).push(pVM);
			return 1;
		}
	}

	script_pushbool(pVM, false);
	return 1;
}

// respawnVehicle(vehicleid)
int CVehicleNatives::Respawn(lua_State * pVM)
{
	EntityId vehicleId;
	script_getentity(pVM, -1, &vehicleId);

	if(g_pVehicleManager->DoesExist(vehicleId))
	{
		CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

		if(pVehicle)
		{
			pVehicle->Respawn();
			script_pushbool(pVM, true);
			return 1;
		}
	}

	script_pushbool(pVM, false);
	return 1;
}

// isVehicleOccupied(vehicleid)
int CVehicleNatives::IsOccupied(lua_State * pVM)
{
	EntityId vehicleId;
	script_getentity(pVM, -1, &vehicleId);

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

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

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		CScriptArguments args;

		for(BYTE i = 0; i < (MAX_VEHICLE_PASSENGERS + 1); i++)
		{
			args.push((int)(i + 1));
			CPlayer * pOccupant = pVehicle->GetOccupant(i);
			args.push(pOccupant ? (int)pOccupant->GetPlayerId() : (int)INVALID_ENTITY_ID);
		}

		//script_pusharg(pVM, CScriptArgument(args, false));
		CScriptArgument(args).push(pVM);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// setVehicleLocked(vehicleid, locked)
int CVehicleNatives::SetLocked(lua_State * pVM)
{
	EntityId vehicleId;
	int iLocked;

	script_getentity(pVM, -2, &vehicleId);
	script_getinteger(pVM, -1, &iLocked);

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		script_pushbool(pVM, pVehicle->SetLocked(iLocked));
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

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		script_pushinteger(pVM, pVehicle->GetLocked());
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// setVehicleIndicators(vehicleid, frontleft, frontright, backleft, backright)
int CVehicleNatives::SetIndicators(lua_State * pVM)
{
	EntityId vehicleId;
	bool bFrontLeft, bFrontRight, bBackLeft, bBackRight;
	bool sqbOn;

	script_getentity(pVM, 1, &vehicleId);

	script_getbool(pVM, 2, &sqbOn);
	bFrontLeft = sqbOn != 0;

	script_getbool(pVM, 3, &sqbOn);
	bFrontRight = sqbOn != 0;

	script_getbool(pVM, 4, &sqbOn);
	bBackLeft = sqbOn != 0;

	script_getbool(pVM, 5, &sqbOn);
	bBackRight = sqbOn != 0;

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		pVehicle->SetIndicatorState(bFrontLeft, bFrontRight, bBackLeft, bBackRight);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

int CVehicleNatives::GetIndicators(lua_State * pVM)
{
	EntityId vehicleId;
	script_getentity(pVM, 1, &vehicleId);

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		//sq_newarray(pVM, 0);
		CScriptArray a(pVM);
		for(unsigned char uc = 0; uc <= 3; ++ uc)
		{
			a.push(pVehicle->GetIndicatorState(uc));
			//sq_arrayappend(pVM, -2);
			//script_arrayappend(pVM, -2);
		}
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

int CVehicleNatives::SetComponent(lua_State * pVM)
{
	EntityId vehicleId;
	script_getentity(pVM, 1, &vehicleId);
	
	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);
	
	if(pVehicle)
	{
		int iSlot;
		bool bOn;
		script_getinteger(pVM, 2, &iSlot);
		script_getbool(pVM, 3, &bOn);
		
		if(iSlot >= 0 && iSlot <= 8)
		{
			pVehicle->SetComponentState((unsigned char)iSlot, bOn != 0);
			script_pushbool(pVM, true);
			return 1;
		}
	}

	script_pushbool(pVM, false);
	return 1;
}

int CVehicleNatives::ResetComponents(lua_State * pVM)
{
	EntityId vehicleId;
	script_getentity(pVM, 1, &vehicleId);
	
	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);
	
	if(pVehicle)
	{
		pVehicle->ResetComponents();
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

int CVehicleNatives::GetComponents(lua_State * pVM)
{
	EntityId vehicleId;
	script_getentity(pVM, 1, &vehicleId);
	
	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);
	
	if(pVehicle)
	{
		CScriptArray a(pVM);
		for(unsigned char i = 0; i < 9; ++ i)
		{
			a.push(pVehicle->GetComponentState(i));
			//script_arrayappend(pVM, -2);
		}
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// setVehicleVariation(vehicleid, variation)
int CVehicleNatives::SetVariation(lua_State * pVM)
{
	EntityId vehicleId;
	script_getentity(pVM, -2, &vehicleId);

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		int iVariation;
		script_getinteger(pVM, -1, &iVariation);
		pVehicle->SetVariation((unsigned char)iVariation);

		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// getVehicleVariation(vehicleid)
int CVehicleNatives::GetVariation(lua_State * pVM)
{
	EntityId vehicleId;
	script_getentity(pVM, 1, &vehicleId);
	
	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);
	
	if(pVehicle)
	{
		script_pushinteger(pVM, pVehicle->GetVariation());
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// setVehicleEngineState(vehicleid, turned_on?)
int CVehicleNatives::SetEngineStatus(lua_State * pVM)
{
	EntityId vehicleId;
	script_getentity(pVM, -2, &vehicleId);

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		bool bEngineStatusx;
		script_getbool(pVM, -1, &bEngineStatusx);
		pVehicle->SetEngineStatus(bEngineStatusx != 0);

		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// getVehicleEngineState(vehicleid)
int CVehicleNatives::GetEngineStatus(lua_State * pVM)
{
	EntityId vehicleId;
	script_getentity(pVM, -1, &vehicleId);
	
	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);
	
	if(pVehicle)
	{
		script_pushbool(pVM, pVehicle->GetEngineStatus());
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

int CVehicleNatives::SwitchTaxiLights(lua_State *pVM)
{
	EntityId vehicleId;
	script_getentity(pVM, -2,&vehicleId);

	bool check;
	script_getbool(pVM, -1, &check);

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);
	
	if(pVehicle)
	{
		bool bToggle = (check != 0);
		pVehicle->TurnTaxiLights(bToggle);
		script_pushbool(pVM,true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 0;
}

// controlCarDoors
int CVehicleNatives::ControlCar(lua_State *pVM)
{
	EntityId vehicleId;
	script_getentity(pVM,-4,&vehicleId);

	int iDoor;
	script_getinteger(pVM,-3,&iDoor);

	bool bDoor;
	script_getbool(pVM,-2,&bDoor);

	float fAngle;
	script_getfloat(pVM,-1,&fAngle); 

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);
	
	if(pVehicle)
	{
		bool bToggle = (bDoor != 0);
		pVehicle->SetCarDoorAngle(iDoor,bToggle,fAngle);
		return 1;
	}

	script_pushbool(pVM, false);
	return 0;
}

int CVehicleNatives::SetLights(lua_State * pVM)
{
	EntityId vehicleId;
	script_getentity(pVM, -2, &vehicleId);

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		bool bLights;
		script_getbool(pVM, -1, &bLights);
		bool bToggle = (bLights != 0);
		pVehicle->SetLights(bToggle);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 0;
}

int CVehicleNatives::GetLights(lua_State * pVM)
{
	EntityId vehicleId;
	script_getentity(pVM, -1, &vehicleId);

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		script_pushbool(pVM,pVehicle->GetLights());
		return 1;
	}

	script_pushbool(pVM, false);
	return 0;
}

int CVehicleNatives::GetTaxiLights(lua_State * pVM)
{
	EntityId vehicleId;
	script_getentity(pVM, -1, &vehicleId);

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		script_pushbool(pVM,pVehicle->GetTaxiLights());
		return 1;
	}

	script_pushbool(pVM, false);
	return 0;
}

// repairVehicleWheels(vehicleid)
int CVehicleNatives::RepairWheels(lua_State * pVM)
{
	EntityId vehicleId;
	script_getentity(pVM, -1, &vehicleId);

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		pVehicle->RepairWheels();
		script_pushbool(pVM,true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 0;
}

// repairVehicleWindows(vehicleid)
int CVehicleNatives::RepairWindows(lua_State * pVM)
{
	EntityId vehicleId;
	script_getentity(pVM, -1, &vehicleId);

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);

	if(pVehicle)
	{
		pVehicle->RepairWindows();
		script_pushbool(pVM,true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 0;
}

// setVehicleGpsState(vehicleid, state)
int CVehicleNatives::SetGpsState(lua_State *pVM)
{
	EntityId vehicleId;
	bool bState;

	script_getentity(pVM, -1, &vehicleId);
	script_getbool(pVM, -2, &bState);

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);
	if(pVehicle)
	{
		pVehicle->SetVehicleGPSState(bState);
		script_pushbool(pVM, true);
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

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);
	if(pVehicle)
	{
		script_pushbool(pVM, pVehicle->GetVehicleGPSState());
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// setVehicleAlarm(vehicleid, alarmduration)
int CVehicleNatives::SetAlarm(lua_State * pVM)
{
	EntityId vehicleId;
	script_getentity(pVM,-2, &vehicleId);

	int iDuration;
	script_getinteger(pVM, -1, &iDuration);

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);
	if(pVehicle)
	{
		pVehicle->SetAlarm((int)iDuration);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}


// markVehicleAsActorVehicle(vehicleid)
int CVehicleNatives::MarkVehicle(lua_State * pVM)
{
	EntityId vehicleId;
	script_getentity(pVM,-2, &vehicleId);

	bool bToggle;
	script_getbool(pVM, -1, &bToggle);

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);
	if(pVehicle)
	{
		bool bSwitch = (bToggle != 0);
		pVehicle->MarkVehicle(bSwitch);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// repairVehicle(vehicleid)
int CVehicleNatives::RepairVehicle(lua_State * pVM)
{
	EntityId vehicleId;
	script_getentity(pVM, -1, &vehicleId);

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);
	if(pVehicle)
	{
		pVehicle->RepairVehicle();
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// setVehicleDimension(vehicleid, demnsion)
int CVehicleNatives::SetDimension(lua_State * pVM)
{
	int iDimension;
	EntityId playerId;

	script_getinteger(pVM, -1, &iDimension);
	script_getentity(pVM, -2, &playerId);
	
	CVehicle* pVehicle = g_pVehicleManager->GetAt(playerId);
	if(pVehicle == NULL) {
		script_pushbool(pVM, false);
		CLogFile::Print("SetDimension failed");
		return false;
	}

	pVehicle->SetDimension(iDimension);
	CBitStream bsSend;
	bsSend.Write(playerId);
	bsSend.Write(iDimension);

	//CLogFile::Printf("Set dimension of player(%i) to %i", (int)playerId, iDimension);
	g_pNetworkManager->RPC(RPC_ScriptingSetPlayerDimension, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
	script_pushbool(pVM, true);
	return 1;
}

// getVehicleDimension(vehicleid)
int CVehicleNatives::GetDimension(lua_State * pVM)
{ 
	EntityId vehicleId;

	script_getentity(pVM, -1, &vehicleId);

	CVehicle * pVehicle = g_pVehicleManager->GetAt(vehicleId);
	if(pVehicle)
	{
		script_pushinteger(pVM, (int)pVehicle->GetDimension());
		CLogFile::Printf("GetDimension %i", (int)pVehicle->GetDimension());
		return 1;
	}

	script_pushinteger(pVM, -1);
	return 1;
}