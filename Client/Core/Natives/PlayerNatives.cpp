//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: PlayerNatives.cpp
// Project: Client.Core
// Author(s): jenksta
//            Einstein
//            Sebihunter
// License: See LICENSE in root directory
//
//==============================================================================

#include "../Natives.h"
#include "Scripting/CScriptingManager.h"
#include "../CClient.h"
#include "../CRemotePlayer.h"

extern CClient * g_pClient;

// Player functions

void CPlayerNatives::Register(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterFunction("isPlayerConnected", IsConnected, 1, "i");
	pScriptingManager->RegisterFunction("getLocalPlayer", GetLocal, 0, NULL);
	pScriptingManager->RegisterFunction("getPlayerName", GetName, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerHealth", GetHealth, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerArmour", GetArmour, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerCoordinates", GetCoordinates, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerPosition", GetCoordinates, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerVelocity", GetVelocity, 1, "i");
	pScriptingManager->RegisterFunction("isPlayerInAnyVehicle", IsInAnyVehicle, 1, "i");
	pScriptingManager->RegisterFunction("isPlayerInVehicle", IsInVehicle, 2, "ii");
	pScriptingManager->RegisterFunction("getPlayerVehicleId", GetVehicleId, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerSeatId", GetSeatId, 1, "i");
	pScriptingManager->RegisterFunction("isPlayerOnFoot", IsOnFoot, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerModel", GetModel, 1, "i");
	pScriptingManager->RegisterFunction("isPlayerSpawned", IsSpawned, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerHeading", GetHeading, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerMoney", GetMoney, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerState", GetState, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerWeapon", GetWeapon, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerAmmo", GetAmmo, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerInterior", GetInterior, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerPadState", GetControlState, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerPreviousPadState", GetPreviousControlState, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerControlState", GetControlState, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerPreviousControlState", GetPreviousControlState, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerPing", GetPing, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerColor", GetColor, 1, "i");
	pScriptingManager->RegisterFunction("isPlayerDucking", IsDucking, 1, "i");
	pScriptingManager->RegisterFunction("isPlayerJackingAVehicle", IsJackingAVehicle, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerWeaponSlot", GetWeaponSlot, 2, "ii");
	pScriptingManager->RegisterFunction("getPlayerAmmoInClip", GetAmmoInClip, 1, "i");
	pScriptingManager->RegisterFunction("setPlayerDoorLockState", SetDoorLockState, 7, "isfffbf");
	pScriptingManager->RegisterFunction("switchPlayerPhysics", TogglePhysics, 3, "bbb");
	pScriptingManager->RegisterFunction("getWayPointCoordinates", GetWayPointCoords, 0, NULL);
}

// isPlayerConnected(playerid)
int CPlayerNatives::IsConnected(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -1, &playerId);
	script_pushbool(pVM, g_pClient->GetPlayerManager()->DoesExist(playerId));
	return 1;
}

// getLocalPlayer()
int CPlayerNatives::GetLocal(lua_State * pVM)
{
	// Get our local player
	CLocalPlayer * pLocalPlayer = g_pClient->GetLocalPlayer();

	if(pLocalPlayer->IsNetworkPlayer())
		script_pushentity(pVM, pLocalPlayer->GetPlayerId());
	else
		script_pushinteger(pVM,-1);
	
	return 1;
}

// getPlayerName(playerid)
int CPlayerNatives::GetName(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -1, &playerId);

	CNetworkPlayer * pPlayer = g_pClient->GetPlayerManager()->GetAt(playerId);

	if(pPlayer)
	{
		script_pushstring(pVM, pPlayer->GetName());
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// getPlayerHealth(playerid)
int CPlayerNatives::GetHealth(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -1, &playerId);

	CNetworkPlayer * pPlayer = g_pClient->GetPlayerManager()->GetAt(playerId);

	if(pPlayer)
	{
		script_pushinteger(pVM, (pPlayer->GetHealth() - 100));
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// getPlayerArmour(playerid)
int CPlayerNatives::GetArmour(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -1, &playerId);

	CNetworkPlayer * pPlayer = g_pClient->GetPlayerManager()->GetAt(playerId);

	if(pPlayer)
	{
		script_pushinteger(pVM, pPlayer->GetArmour());
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// getPlayerCoordinates(playerid)
int CPlayerNatives::GetCoordinates(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -1, &playerId);

	CNetworkPlayer * pPlayer = g_pClient->GetPlayerManager()->GetAt(playerId);

	if(pPlayer)
	{
		CVector3 vecPosition;
		pPlayer->GetPosition(vecPosition);
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

// getPlayerVelocity(playerid)
int CPlayerNatives::GetVelocity(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -1, &playerId);

	CNetworkPlayer * pPlayer = g_pClient->GetPlayerManager()->GetAt(playerId);

	if(pPlayer)
	{
		CVector3 vecMoveSpeed;
		pPlayer->GetMoveSpeed(vecMoveSpeed);
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

// isPlayerInAnyVehicle(playerid)
int CPlayerNatives::IsInAnyVehicle(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -1, &playerId);

	CNetworkPlayer * pPlayer = g_pClient->GetPlayerManager()->GetAt(playerId);

	if(pPlayer)
	{
		script_pushbool(pVM, pPlayer->IsInVehicle());
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// isPlayerInVehicle(playerid, vehicleid)
int CPlayerNatives::IsInVehicle(lua_State * pVM)
{
	EntityId playerId;
	EntityId vehicleId;
	script_getentity(pVM, -2, &playerId);
	script_getentity(pVM, -1, &vehicleId);

	CNetworkPlayer * pPlayer = g_pClient->GetPlayerManager()->GetAt(playerId);

	if(pPlayer)
	{
		CNetworkVehicle * pVehicle = pPlayer->GetVehicle();

		if(pVehicle)
		{
			script_pushbool(pVM, (pVehicle->GetVehicleId() == vehicleId));
			return 1;
		}
	}

	script_pushbool(pVM, false);
	return 1;
}

// getPlayerVehicleId(playerid)
int CPlayerNatives::GetVehicleId(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -1, &playerId);

	CNetworkPlayer * pPlayer = g_pClient->GetPlayerManager()->GetAt(playerId);

	if(pPlayer)
	{
		CNetworkVehicle * pVehicle = pPlayer->GetVehicle();

		if(pVehicle)
			script_pushentity(pVM, pVehicle->GetVehicleId());
		else
			script_pushentity(pVM, INVALID_ENTITY_ID);

		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// getPlayerSeatId(playerid)
int CPlayerNatives::GetSeatId(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -1, &playerId);

	CNetworkPlayer * pPlayer = g_pClient->GetPlayerManager()->GetAt(playerId);

	if(pPlayer && pPlayer->IsInVehicle())
	{
		script_pushinteger(pVM, pPlayer->GetVehicleSeatId());
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// isPlayerOnFoot(playerid)
int CPlayerNatives::IsOnFoot(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -1, &playerId);

	CNetworkPlayer * pPlayer = g_pClient->GetPlayerManager()->GetAt(playerId);

	if(pPlayer)
	{
		script_pushbool(pVM, pPlayer->IsOnFoot());
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// getPlayerModel(playerid)
int CPlayerNatives::GetModel(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -1, &playerId);

	CNetworkPlayer * pPlayer = g_pClient->GetPlayerManager()->GetAt(playerId);

	if(pPlayer)
	{
		script_pushinteger(pVM, ModelHashToSkinId(pPlayer->GetModelInfo()->GetHash()));
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// isPlayerSpawned(playerid)
int CPlayerNatives::IsSpawned(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -1, &playerId);

	CNetworkPlayer * pPlayer = g_pClient->GetPlayerManager()->GetAt(playerId);

	if(pPlayer)
	{
		script_pushbool(pVM, pPlayer->IsSpawned());
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// getPlayerHeading(playerid)
int CPlayerNatives::GetHeading(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -1, &playerId);

	CNetworkPlayer * pPlayer = g_pClient->GetPlayerManager()->GetAt(playerId);

	if(pPlayer)
	{
		script_pushfloat(pVM, pPlayer->GetCurrentHeading());
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// getPlayerMoney(playerid)
int CPlayerNatives::GetMoney(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -1, &playerId);

	CNetworkPlayer * pPlayer = g_pClient->GetPlayerManager()->GetAt(playerId);

	if(pPlayer)
	{
		script_pushinteger(pVM, pPlayer->GetMoney());
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// getPlayerState(playerid)
int CPlayerNatives::GetState(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -1, &playerId);

	CNetworkPlayer * pPlayer = g_pClient->GetPlayerManager()->GetAt(playerId);

	if(pPlayer && !pPlayer->IsLocalPlayer())
	{
		CRemotePlayer * pRemotePlayer = reinterpret_cast<CRemotePlayer *>(pPlayer);

		if(pRemotePlayer)
		{
			script_pushinteger(pVM, pRemotePlayer->GetStateType());
			return 1;
		}
	}
	else if(playerId < MAX_PLAYERS)
	{
		script_pushinteger(pVM, STATE_TYPE_DISCONNECT);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// getPlayerWeapon(playerid)
int CPlayerNatives::GetWeapon(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -1, &playerId);

	CNetworkPlayer * pPlayer = g_pClient->GetPlayerManager()->GetAt(playerId);

	if(pPlayer)
	{
		script_pushinteger(pVM, pPlayer->GetCurrentWeapon());
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

//getPlayerAmmoInClip(playerid, weaponId)
int CPlayerNatives::GetAmmoInClip(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -2, &playerId);

	CNetworkPlayer * pPlayer = g_pClient->GetPlayerManager()->GetAt(playerId);

	if(pPlayer)
	{
		script_pushinteger(pVM, pPlayer->GetAmmoInClip());
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// getPlayerAmmo(playerid)
int CPlayerNatives::GetAmmo(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -1, &playerId);

	CNetworkPlayer * pPlayer = g_pClient->GetPlayerManager()->GetAt(playerId);

	if(pPlayer)
	{
		script_pushinteger(pVM, pPlayer->GetAmmo(pPlayer->GetCurrentWeapon()));
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// getPlayerInterior(playerid)
int CPlayerNatives::GetInterior(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -1, &playerId);

	CNetworkPlayer * pPlayer = g_pClient->GetPlayerManager()->GetAt(playerId);

	if(pPlayer)
	{
		script_pushinteger(pVM, pPlayer->GetInterior());
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// getPlayerControlState(playerid)
int CPlayerNatives::GetControlState(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -1, &playerId);

	CNetworkPlayer * pPlayer = g_pClient->GetPlayerManager()->GetAt(playerId);

	if(pPlayer)
	{
		// Get the player control state
		CControlState controlState;
		pPlayer->GetControlState(&controlState);

		// Create the table and array
		CScriptArguments table;
		CScriptArguments array;

		// Create the 'onFootMove' array
		table.push("onFootMove");
		array.reset();

		for(int i = 0; i < 4; i++)
		{
			array.push(i);
			array.push(controlState.ucOnFootMove[i]);
		}
		table.push(array);

		// Create the 'inVehicleMove' array
		table.push("inVehicleMove");
		array.reset();

		for(int i = 0; i < 4; i++)
		{
			array.push(i);
			array.push(controlState.ucInVehicleMove[i]);
		}
		table.push(array);

		// Create the 'inVehicleTriggers' array
		table.push("inVehicleTriggers");
		array.reset();

		for(int i = 0; i < 2; i++)
		{
			array.push(i);
			array.push(controlState.ucInVehicleTriggers[i]);
		}
		table.push(array);

		// Create the on foot keys slots
		table.push("enterExitVehicle");
		table.push(controlState.keys.bEnterExitVehicle);
		table.push("sprint");
		table.push(controlState.keys.bSprint);
		table.push("jump");
		table.push(controlState.keys.bJump);
		table.push("attack");
		table.push(controlState.keys.bAttack);
		table.push("attack2");
		table.push(controlState.keys.bAttack2);
		table.push("aim");
		table.push(controlState.keys.bAim);
		table.push("freeAim");
		table.push(controlState.keys.bFreeAim);
		table.push("meleeAttack1");
		table.push(controlState.keys.bMeleeAttack1);
		table.push("meleeAttack2");
		table.push(controlState.keys.bMeleeAttack2);
		table.push("meleeKick");
		table.push(controlState.keys.bMeleeKick);
		table.push("meleeBlock");
		table.push(controlState.keys.bMeleeBlock);

		// Create the in vehicle keys slots
		table.push("handbrake");
		table.push(controlState.keys.bHandbrake);
		table.push("handbrake2");
		table.push(controlState.keys.bHandbrake2);
		table.push("horn");
		table.push(controlState.keys.bHorn);
		table.push("driveBy");
		table.push(controlState.keys.bDriveBy);
		table.push("heliPrimaryFire");
		table.push(controlState.keys.bHeliPrimaryFire);

		// Push the table to the VM
		CScriptArgument(table).push(pVM);
		return 1;
	}

	//script_pushbool(pVM, false);
	return 0;
}

// getPlayerPreviousControlState(playerid)
int CPlayerNatives::GetPreviousControlState(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -1, &playerId);

	CNetworkPlayer * pPlayer = g_pClient->GetPlayerManager()->GetAt(playerId);

	if(pPlayer)
	{
		// Get the player control state
		CControlState controlState;
		pPlayer->GetPreviousControlState(&controlState);

		// Create the table and array
		CScriptArguments table;
		CScriptArguments array;

		// Create the 'onFootMove' array
		table.push("onFootMove");
		array.reset();

		for(int i = 0; i < 4; i++)
		{
			array.push(i);
			array.push(controlState.ucOnFootMove[i]);
		}
		table.push(array);

		// Create the 'inVehicleMove' array
		table.push("inVehicleMove");
		array.reset();

		for(int i = 0; i < 4; i++)
		{
			array.push(i);
			array.push(controlState.ucInVehicleMove[i]);
		}
		table.push(array);

		// Create the 'inVehicleTriggers' array
		table.push("inVehicleTriggers");
		array.reset();

		for(int i = 0; i < 2; i++)
		{
			array.push(i);
			array.push(controlState.ucInVehicleTriggers[i]);
		}
		table.push(array);

		// Create the on foot keys slots
		table.push("enterExitVehicle");
		table.push(controlState.keys.bEnterExitVehicle);
		table.push("sprint");
		table.push(controlState.keys.bSprint);
		table.push("jump");
		table.push(controlState.keys.bJump);
		table.push("attack");
		table.push(controlState.keys.bAttack);
		table.push("attack2");
		table.push(controlState.keys.bAttack2);
		table.push("aim");
		table.push(controlState.keys.bAim);
		table.push("freeAim");
		table.push(controlState.keys.bFreeAim);
		table.push("meleeAttack1");
		table.push(controlState.keys.bMeleeAttack1);
		table.push("meleeAttack2");
		table.push(controlState.keys.bMeleeAttack2);
		table.push("meleeKick");
		table.push(controlState.keys.bMeleeKick);
		table.push("meleeBlock");
		table.push(controlState.keys.bMeleeBlock);

		// Create the in vehicle keys slots
		table.push("handbrake");
		table.push(controlState.keys.bHandbrake);
		table.push("handbrake2");
		table.push(controlState.keys.bHandbrake2);
		table.push("horn");
		table.push(controlState.keys.bHorn);
		table.push("driveBy");
		table.push(controlState.keys.bDriveBy);
		table.push("heliPrimaryFire");
		table.push(controlState.keys.bHeliPrimaryFire);

		// Push the table to the VM
		CScriptArgument(table).push(pVM);
		return 1;
	}

	//script_pushbool(pVM, false);
	return 0;
}

// getPlayerPing(playerid)
int CPlayerNatives::GetPing(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, 1, &playerId);

	// Get our local player
	CLocalPlayer * pLocalPlayer = g_pClient->GetLocalPlayer();

	if(pLocalPlayer->GetPlayerId() == playerId) {
		script_pushinteger(pVM, pLocalPlayer->GetPing());
		return 1;
	}

	CNetworkPlayer * pPlayer = g_pClient->GetPlayerManager()->GetAt(playerId);
	if(pPlayer)
	{
		script_pushinteger(pVM, pPlayer->GetPing());
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// getPlayerColor(playerid)
int CPlayerNatives::GetColor(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, 1, &playerId);

	CNetworkPlayer * pPlayer = g_pClient->GetPlayerManager()->GetAt(playerId);

	if(pPlayer)
	{
		script_pushinteger(pVM, pPlayer->GetColor());
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// isPlayerDucking(playerid)
int CPlayerNatives::IsDucking(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -1, &playerId);

	CNetworkPlayer * pPlayer = g_pClient->GetPlayerManager()->GetAt(playerId);

	if(pPlayer)
	{
		script_pushbool(pVM, pPlayer->IsDucking());
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// isPlayerJackingAVehicle(playerid)
int CPlayerNatives::IsJackingAVehicle(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -1, &playerId);

	CNetworkPlayer * pPlayer = g_pClient->GetPlayerManager()->GetAt(playerId);

	if(pPlayer)
	{
		script_pushbool(pVM, pPlayer->IsJackingAVehicle());
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// getPlayerWeaponSlots(playerid, slot)
int CPlayerNatives::GetWeaponSlot(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -2, &playerId);

	int iSlot;
	script_getinteger(pVM, -1, &iSlot);

	CNetworkPlayer * pPlayer = g_pClient->GetPlayerManager()->GetAt(playerId);

	if(pPlayer)
	{
		unsigned int uiWeap, uiAmmo;
 		pPlayer->GetWeaponInSlot(iSlot, uiWeap, uiAmmo);

		CScriptArray args(pVM);
		args.push((int)uiWeap);
		args.push((int)uiAmmo);
		//CScriptArgument(args, true).push(pVM);
		return 1;
	}
	script_pushbool(pVM, false);
	return 1;
}

int CPlayerNatives::SetDoorLockState(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -7, &playerId);

	const char *szString;
	script_getstring(pVM, -6, &szString);

	CVector3 vecPos;
	script_getfloat(pVM, -5, &vecPos.fX);
	script_getfloat(pVM, -4, &vecPos.fY);
	script_getfloat(pVM, -3, &vecPos.fZ);

	bool bState;
	script_getbool(pVM, -2, &bState);
	bool bToggle = (bState != 0);

	float fSwing;
	script_getfloat(pVM, -1, &fSwing);

	CNetworkPlayer * pPlayer = g_pClient->GetPlayerManager()->GetAt(playerId);
	if(pPlayer)
	{
		DWORD dwHash = Scripting::GetHashKey(szString);
		Scripting::SetStateOfClosestDoorOfType(dwHash, vecPos.fX, vecPos.fY, vecPos.fZ, bToggle, fSwing);
		script_pushbool(pVM,true);
		return 1;
	}
	script_pushbool(pVM, false);
	return 1;
}

int CPlayerNatives::TogglePhysics(lua_State * pVM)
{
	bool bInvincible = false;
	bool bWindScreen = false;
	bool bNockedOffBike = false;

	// Get the local player
	CLocalPlayer * pLocalPlayer = g_pClient->GetLocalPlayer();

	if(pLocalPlayer && pLocalPlayer->IsSpawned())
	{
		bool bToggle = (bInvincible != 0);
		Scripting::SetCharInvincible(pLocalPlayer->GetScriptingHandle(), bToggle);
		bToggle = (bWindScreen != 0);
		Scripting::SetCharWillFlyThroughWindscreen(pLocalPlayer->GetScriptingHandle(), bToggle);
		bToggle = (bNockedOffBike != 0);
		Scripting::SetCharCanBeKnockedOffBike(pLocalPlayer->GetScriptingHandle(), bToggle);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

int CPlayerNatives::GetWayPointCoords(lua_State * pVM)
{
	// Get the local player
	CLocalPlayer * pLocalPlayer = g_pClient->GetLocalPlayer();

	if(pLocalPlayer && pLocalPlayer->IsSpawned())
	{
		unsigned int uiWayPointHandle = -1;
		Scripting::GetFirstBlipInfoId(Scripting::BLIP_WAYPOINT);

		if(uiWayPointHandle != -1)
		{
			CVector3 vecPos;
			Scripting::GetBlipCoords(uiWayPointHandle, &vecPos);

			if(vecPos.fZ == 0.0)
				Scripting::GetGroundZFor3DCoord(vecPos.fX, vecPos.fY, 1000, &vecPos.fZ);

			CScriptArray args(pVM);
			args.push(vecPos.fX);
			args.push(vecPos.fY);
			args.push(vecPos.fZ);
			//CScriptArgument(args, true).push(pVM);
			return 1;
		}
	}

	script_pushbool(pVM, false);
	return 1;
}