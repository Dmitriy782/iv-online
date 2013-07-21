//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: PlayerNatives.cpp
// Project: Server.Core
// Author(s): jenksta
//            Einstein
//            Sebihunter
// License: See LICENSE in root directory
//
//==============================================================================

#include "../Natives.h"
#include "Scripting/CScriptingManager.h"
#include "../CVehicleManager.h"
#include "../CPlayerManager.h"
#include "../CNetworkManager.h"
#include <Game/CTime.h>
#include "CEvents.h"

extern CPlayerManager * g_pPlayerManager;
extern CVehicleManager * g_pVehicleManager;
extern CNetworkManager * g_pNetworkManager;
extern CTime * g_pTime;
extern CEvents * g_pEvents;

// Player functions

void CPlayerNatives::Register(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterFunction("isPlayerConnected", IsConnected, 1, "i");

	pScriptingManager->RegisterFunction("getPlayerName", GetName, 1, "i");
	pScriptingManager->RegisterFunction("setPlayerName", SetName, 2, "is");

	pScriptingManager->RegisterFunction("setPlayerHealth", SetHealth, 2, "ii");
	pScriptingManager->RegisterFunction("getPlayerHealth", GetHealth, 1, "i");

	pScriptingManager->RegisterFunction("setPlayerArmour", SetArmour, 2, "ii");
	pScriptingManager->RegisterFunction("getPlayerArmour", GetArmour, 1, "i");

	pScriptingManager->RegisterFunction("setPlayerCoordinates", SetCoordinates, 4, "ifff");
	pScriptingManager->RegisterFunction("getPlayerCoordinates", GetCoordinates, 1, "i");

	pScriptingManager->RegisterFunction("setPlayerPosition", SetCoordinates, 4, "ifff");
	pScriptingManager->RegisterFunction("getPlayerPosition", GetCoordinates, 1, "i");

	// World stuffs
	pScriptingManager->RegisterFunction("setPlayerTime", SetTime, 3, "iii");
	pScriptingManager->RegisterFunction("setPlayerWeather", SetWeather, 2, "ii");
	pScriptingManager->RegisterFunction("setPlayerGravity", SetGravity, 2, "if");

	pScriptingManager->RegisterFunction("sendPlayerMessage", SendMessage, -1, NULL);
	pScriptingManager->RegisterFunction("sendMessageToAll", SendMessageToAll, -1, NULL);
	pScriptingManager->RegisterFunction("isPlayerInAnyVehicle", IsInAnyVehicle, 1, "i");
	pScriptingManager->RegisterFunction("isPlayerInVehicle", IsInVehicle, 2, "ii");
	pScriptingManager->RegisterFunction("getPlayerVehicleId", GetVehicleId, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerSeatId", GetSeatId, 1, "i");
	pScriptingManager->RegisterFunction("isPlayerOnFoot", IsOnFoot, 1, "i");
	pScriptingManager->RegisterFunction("togglePlayerPayAndSpray", TogglePayAndSpray, 2, "ib");
	pScriptingManager->RegisterFunction("togglePlayerAutoAim", ToggleAutoAim, 2, "ib");
	//pScriptingManager->RegisterFunction("setPlayerDrunk", SetPlayerDrunk, 2, "ii");
	pScriptingManager->RegisterFunction("givePlayerWeapon", GiveWeapon, 3, "iii");
	pScriptingManager->RegisterFunction("removePlayerWeapons", RemoveWeapons, 1, "i");
	pScriptingManager->RegisterFunction("setPlayerSpawnLocation", SetSpawnLocation, 5, "iffff");
	pScriptingManager->RegisterFunction("setPlayerModel", SetModel, 2, "ii");
	pScriptingManager->RegisterFunction("getPlayerModel", GetModel, 1, "i");
	pScriptingManager->RegisterFunction("togglePlayerControls", ToggleControls, 2, "ib");
	pScriptingManager->RegisterFunction("isPlayerSpawned", IsSpawned, 1, "i");
	pScriptingManager->RegisterFunction("setPlayerHeading", SetHeading, 2, "if");
	pScriptingManager->RegisterFunction("getPlayerHeading", GetHeading, 1, "i");
	pScriptingManager->RegisterFunction("togglePlayerPhysics", TogglePhysics, 2, "ib");
	pScriptingManager->RegisterFunction("kickPlayer", Kick, 2, "ib");
	pScriptingManager->RegisterFunction("banPlayer", Ban, 2, "ii");
	pScriptingManager->RegisterFunction("getPlayerIp", GetIp, 1, "i");
	pScriptingManager->RegisterFunction("givePlayerMoney", GiveMoney, 2, "ii");
	pScriptingManager->RegisterFunction("setPlayerMoney", SetMoney, 2, "ii");
	pScriptingManager->RegisterFunction("resetPlayerMoney", ResetMoney, 1, "ii");
	pScriptingManager->RegisterFunction("getPlayerMoney", GetMoney, 1, "i");
	pScriptingManager->RegisterFunction("displayPlayerText", DisplayText, 5, "iffsi");
	pScriptingManager->RegisterFunction("displayTextForAll", DisplayTextForAll, 4, "ffsi");
	pScriptingManager->RegisterFunction("displayPlayerInfoText", DisplayInfoText, 3, "isi");
	pScriptingManager->RegisterFunction("displayInfoTextForAll", DisplayInfoTextForAll, 2, "si");
	pScriptingManager->RegisterFunction("togglePlayerFrozen", ToggleFrozen, 2, "ib");
	pScriptingManager->RegisterFunction("getPlayerState", GetState, 1, "i");
	pScriptingManager->RegisterFunction("setPlayerVelocity", SetVelocity, 4, "ifff");
	pScriptingManager->RegisterFunction("getPlayerVelocity", GetVelocity, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerWantedLevel", GetWantedLevel, 1, "i");
	pScriptingManager->RegisterFunction("setPlayerWantedLevel", SetWantedLevel, 2, "ii");
	pScriptingManager->RegisterFunction("warpPlayerIntoVehicle", WarpIntoVehicle, -1, NULL);
	pScriptingManager->RegisterFunction("removePlayerFromVehicle", RemoveFromVehicle, -1, NULL);
	pScriptingManager->RegisterFunction("getPlayerWeapon", GetWeapon, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerAmmo", GetAmmo, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerSerial", GetSerial, 1, "i");
	pScriptingManager->RegisterFunction("setCameraBehindPlayer", SetCameraBehind, 1, "i");
	pScriptingManager->RegisterFunction("setPlayerDucking", SetDucking, 2, "ib");
	pScriptingManager->RegisterFunction("isPlayerDucking", IsDucking, 1, "i");
	pScriptingManager->RegisterFunction("setPlayerInvincible", SetInvincible, 2, "ib");
	pScriptingManager->RegisterFunction("togglePlayerHud", ToggleHUD, 2, "ib");
	pScriptingManager->RegisterFunction("togglePlayerRadar", ToggleRadar, 2, "ib");
	pScriptingManager->RegisterFunction("togglePlayerNames", ToggleNames, 2, "ib");
	pScriptingManager->RegisterFunction("togglePlayerAreaNames", ToggleAreaNames, 2, "ib");
	pScriptingManager->RegisterFunction("getEmptyPlayerPadState", GetEmptyControlState, 0, NULL);
	pScriptingManager->RegisterFunction("getPlayerPreviousPadState", GetPreviousControlState, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerPadState", GetControlState, 1, "i");
	pScriptingManager->RegisterFunction("getEmptyPlayerControlState", GetEmptyControlState, 0, NULL);
	pScriptingManager->RegisterFunction("getPlayerPreviousControlState", GetPreviousControlState, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerControlState", GetControlState, 1, "i");
	pScriptingManager->RegisterFunction("setPlayerColor", SetColor, 2, "ii");
	pScriptingManager->RegisterFunction("getPlayerColor", GetColor, 1, "i");
	pScriptingManager->RegisterFunction("getPlayerPing", GetPing, 1, "i");
	pScriptingManager->RegisterFunction("givePlayerHelmet", GiveHelmet, 1, "i");
	pScriptingManager->RegisterFunction("removePlayerHelmet", RemoveHelmet, 1, "i");
	pScriptingManager->RegisterFunction("togglePlayerHelmet", ToggleHelmet, 2, "ib");
	pScriptingManager->RegisterFunction("setPlayerClothes", SetClothes, 3, "iii");
	pScriptingManager->RegisterFunction("getPlayerClothes", GetClothes, 1, "i");
	pScriptingManager->RegisterFunction("resetPlayerClothes", ResetClothes, 1, "i");
	pScriptingManager->RegisterFunction("respawnPlayer", Respawn, 1, "i");
	pScriptingManager->RegisterFunction("setPlayerCameraPos", SetCameraPos, 4, "ifff");
	pScriptingManager->RegisterFunction("setPlayerCameraLookAt", SetCameraLookAt, 4, "ifff");
	pScriptingManager->RegisterFunction("resetPlayerCamera", ResetCamera, 1, "i");
	pScriptingManager->RegisterFunction("forcePlayerPlayAnimation", forceAnim, 3, "iss");
	pScriptingManager->RegisterFunction("triggerPlayerPoliceReport", triggerPoliceReport, 2, "is");
	pScriptingManager->RegisterFunction("triggerPlayerGameSound", triggerAudioEvent, 2, "is");
	pScriptingManager->RegisterFunction("triggerPlayerMissionSound", triggerMissionCompleteAudio, 2, "ii");
	pScriptingManager->RegisterFunction("fadePlayerScreenIn",fadeScreenIn, 2, "ii");
	pScriptingManager->RegisterFunction("fadePlayerScreenOut", fadeScreenOut, 2, "ii");
	pScriptingManager->RegisterFunction("blockPlayerDropWeaponsAtDeath",blockWeaponDrop, 2, "ib");
	pScriptingManager->RegisterFunction("blockPlayerWeaponScroll", blockWeaponChange, 2, "ib");
	pScriptingManager->RegisterFunction("requestPlayerAnimations", requestAnim, 2, "is");
	pScriptingManager->RegisterFunction("releasePlayerAnimations", releaseAnim, 2, "is");
	pScriptingManager->RegisterFunction("attachPlayerCameraToPlayer", AttachCamToPlayer, 6, "iiifff");
	pScriptingManager->RegisterFunction("attachPlayerCameraToVehicle", AttachCamToVehicle, 6, "iiifff");
	pScriptingManager->RegisterFunction("displayHudNotification", DisplayHudNotification, 3, "iis");
	pScriptingManager->RegisterFunction("setPlayerFollowVehicleMode", FollowVehicleMode, 2, "ii");
	pScriptingManager->RegisterFunction("setPlayerFollowVehicleOffset", FollowVehicleOffset, 5, "iifff");
	pScriptingManager->RegisterFunction("setPlayerAmmoInClip", SetAmmoInClip, 2, "ii");
	pScriptingManager->RegisterFunction("setPlayerAmmo", SetAmmo, 3, "iii");
	pScriptingManager->RegisterFunction("setPlayerUseMobilePhone", SetMobilePhone, 2, "ib");
	pScriptingManager->RegisterFunction("sayPlayerSpeech", SaySpeech, 3, "iss");
	pScriptingManager->RegisterFunction("letPlayerDriveAutomaticAtCoords", DriveAutomatic, 7, "iiffffi");
	pScriptingManager->RegisterFunction("togglePlayerNametagForPlayer",ToggleNametagForPlayer, 3, "iib");
	pScriptingManager->RegisterFunction("triggerClientEvent", TriggerEvent, -1, NULL);
	pScriptingManager->RegisterFunction("getPlayerFileChecksum", GetFileChecksum, 2, "ii");
	
	pScriptingManager->RegisterFunction("setPlayerDimension", SetDimension, 2, "ii");
	pScriptingManager->RegisterFunction("getPlayerDimension", GetDimension, 1, "i");
}

// isPlayerConnected(playerid)
int CPlayerNatives::IsConnected(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -1, &playerId);
	if(g_pPlayerManager->DoesExist(playerId) && g_pPlayerManager->GetAt(playerId)->IsJoined())
		script_pushbool(pVM, true);
	else
		script_pushbool(pVM, false);
	return 1;
}

// getPlayerName(playerid)
int CPlayerNatives::GetName(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -1, &playerId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		script_pushstring(pVM, pPlayer->GetName());
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

int CPlayerNatives::SetName(lua_State * pVM)
{
	EntityId playerId;
	const char* szName;
	script_getentity(pVM, -2, &playerId);
	script_getstring(pVM, -1, &szName);

	CPlayer* pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		String Checkstring = szName;
		CScriptArguments nameCheckArguments;
		nameCheckArguments.push(playerId);
		nameCheckArguments.push(Checkstring);

		if(g_pEvents->Call("playerNameCheck", &nameCheckArguments).GetInteger() != 1)
		{
			CLogFile::Printf("Can't change the name from player %d (Invalid Characters)",playerId);

			script_pushbool(pVM, false);
			return 0;
		} 

		script_pushbool(pVM, pPlayer->SetName(szName));
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// givePlayerWeapon(playerid, weaponid, ammo)
int CPlayerNatives::GiveWeapon(lua_State * pVM)
{
	EntityId playerId;
	int iWeaponId;
	int iAmmo;
	script_getentity(pVM, -3, &playerId);
	script_getinteger(pVM, -2, &iWeaponId);
	script_getinteger(pVM, -1, &iAmmo);

	if(g_pPlayerManager->DoesExist(playerId) && iWeaponId > 0 && iWeaponId < 21 && iWeaponId != 6)
	{
		CBitStream bsSend;
		bsSend.Write(iWeaponId);
		bsSend.Write(iAmmo);
		g_pNetworkManager->RPC(RPC_ScriptingGivePlayerWeapon, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// removePlayerWeapons(playerid)
int CPlayerNatives::RemoveWeapons(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -1, &playerId);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		g_pNetworkManager->RPC(RPC_ScriptingRemoveWeapons, NULL, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

//getPlayerWantedLevel(playerid, wantedlevel)
int CPlayerNatives::GetWantedLevel(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -1, &playerId);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		script_pushinteger(pVM, g_pPlayerManager->GetAt(playerId)->GetWantedLevel());
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

//setPlayerWantedLevel(playerid, wantedlevel)
int CPlayerNatives::SetWantedLevel(lua_State * pVM)
{
	EntityId playerId;
	int iWantedLevel;
	script_getentity(pVM, -2, &playerId);
	script_getinteger(pVM, -1, &iWantedLevel);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		g_pPlayerManager->GetAt(playerId)->SetWantedLevel(iWantedLevel);
		CBitStream bsSend;
		bsSend.Write(iWantedLevel);
		g_pNetworkManager->RPC(RPC_ScriptingSetWantedLevel, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// setPlayerHealth(playerid, health)
int CPlayerNatives::SetHealth(lua_State * pVM)
{
	EntityId playerId;
	int iHealth;
	script_getentity(pVM, -2, &playerId);
	script_getinteger(pVM, -1, &iHealth);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write(iHealth);
		g_pNetworkManager->RPC(RPC_ScriptingSetPlayerHealth, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		script_pushbool(pVM, true);
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

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		script_pushinteger(pVM, (pPlayer->GetHealth() - 100));
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// setPlayerArmour(playerid)
int CPlayerNatives::SetArmour(lua_State * pVM)
{
	EntityId playerId;
	int iArmour;
	script_getentity(pVM, -2, &playerId);
	script_getinteger(pVM, -1, &iArmour);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write(iArmour);
		g_pNetworkManager->RPC(RPC_ScriptingSetPlayerArmour, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		script_pushbool(pVM, true);
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

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		script_pushinteger(pVM, pPlayer->GetArmour());
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// setPlayerCoordinates(playerid, x, y, z)
int CPlayerNatives::SetCoordinates(lua_State * pVM)
{
	EntityId playerId;
	CVector3 vecPos;
	script_getentity(pVM, -4, &playerId);
	script_getvector3(pVM, -3, &vecPos);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write(vecPos);
		g_pNetworkManager->RPC(RPC_ScriptingSetPlayerCoordinates, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		script_pushbool(pVM, true);
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

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		CVector3 vecPosition;
		pPlayer->GetPosition(vecPosition);
		CScriptArray args(pVM);
		args.push(vecPosition.fX);
		args.push(vecPosition.fY);
		args.push(vecPosition.fZ);
		//script_pusharg(pVM, CScriptArgument(args, true));
		//CScriptArgument(args, true).push(pVM);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// setPlayerTime(playerid, hour, minute)
int CPlayerNatives::SetTime(lua_State * pVM)
{
	EntityId playerId;
	int iHour;
	int iMinute;
	script_getinteger(pVM, -1, &iMinute);
	script_getinteger(pVM, -2, &iHour);
	script_getentity(pVM, -3, &playerId);

	if(g_pPlayerManager->DoesExist(playerId) && (iHour >= 0 && iHour < 24) && (iMinute >= 0 && iMinute < 60))
	{
		CBitStream bsSend;
		bsSend.Write((unsigned char)iHour);
		bsSend.Write((unsigned char)iMinute);
		if(g_pTime->GetMinuteDuration() != CTime::DEFAULT_MINUTE_DURATION)
			bsSend.Write(g_pTime->GetMinuteDuration());

		g_pNetworkManager->RPC(RPC_ScriptingSetPlayerTime, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// setPlayerTime(playerid, weather)
int CPlayerNatives::SetWeather(lua_State * pVM)
{
	EntityId playerId;
	int iWeather;
	script_getinteger(pVM, -1, &iWeather);
	script_getentity(pVM, -2, &playerId);

	if(g_pPlayerManager->DoesExist(playerId) && (iWeather >= 1 && iWeather < 10))
	{
		CBitStream bsSend;
		bsSend.Write((unsigned char)iWeather);
		g_pNetworkManager->RPC(RPC_ScriptingSetPlayerWeather, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// setPlayerGravity(playerid, gravity)
int CPlayerNatives::SetGravity(lua_State * pVM)
{
	EntityId playerId;
	float fGravity;
	script_getentity(pVM, -2, &playerId);
	script_getfloat(pVM, -1, &fGravity);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write(fGravity);
		g_pNetworkManager->RPC(RPC_ScriptingSetPlayerGravity, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// sendPlayerMessage(playerid, message [, color = 0xFFFFFFAA, allowformatting = true])
int CPlayerNatives::SendMessage(lua_State * pVM)
{
	int iTop = (script_gettop(pVM)/* - 1*/);

	if(iTop < 2 || iTop > 4)
	{
		CHECK_PARAMS("sendPlayerMessage", 2);
	}

	EntityId playerId;
	int iColor = 0xFFFFFFFF;
	const char * szMessage = NULL;
	bool sqbAllowFormatting = false;

	if(iTop > 3)
	{
		CHECK_TYPE("sendPlayerMessage", 1, -4, SCRIPT_TYPE_INTEGER);
		CHECK_TYPE("sendPlayerMessage", 2, -3, SCRIPT_TYPE_STRING);
		CHECK_TYPE("sendPlayerMessage", 3, -2, SCRIPT_TYPE_INTEGER);
		CHECK_TYPE("sendPlayerMessage", 4, -1, SCRIPT_TYPE_BOOL);
		script_getentity(pVM, -4, &playerId);
		script_getstring(pVM, -3, &szMessage);
		script_getinteger(pVM, -2, &iColor);
		script_getbool(pVM, -1, &sqbAllowFormatting);
	}
	else if(iTop > 2)
	{
		CHECK_TYPE("sendPlayerMessage", 1, -3, SCRIPT_TYPE_INTEGER);
		CHECK_TYPE("sendPlayerMessage", 2, -2, SCRIPT_TYPE_STRING);
		CHECK_TYPE("sendPlayerMessage", 3, -1, SCRIPT_TYPE_INTEGER);
		script_getentity(pVM, -3, &playerId);
		script_getstring(pVM, -2, &szMessage);
		script_getinteger(pVM, -1, &iColor);
	}
	else
	{
		CHECK_TYPE("sendPlayerMessage", 1, -2, SCRIPT_TYPE_INTEGER);
		CHECK_TYPE("sendPlayerMessage", 2, -1, SCRIPT_TYPE_STRING);
		script_getentity(pVM, -2, &playerId);
		script_getstring(pVM, -1, &szMessage);
	}

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write((DWORD)iColor);
		bsSend.Write(String(szMessage));
		bool bAllowFormatting = (sqbAllowFormatting != 0);
		bsSend.Write(bAllowFormatting);
		g_pNetworkManager->RPC(RPC_Message, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// sendMessageToAll(message [, color = 0xFFFFFFAA, allowformatting = true])
int CPlayerNatives::SendMessageToAll(lua_State * pVM)
{
	int vtop = (script_gettop(pVM)/* - 1*/);

	if(vtop > 3 || vtop < 1)
	{
		CHECK_PARAMS("sendMessageToAll", 1);
	}

	int iColor = 0xFFFFFFAA;
	const char * szMessage = NULL;
	bool sqbAllowFormatting = false;

	if(vtop > 2)
	{
		CHECK_TYPE("sendMessageToAll", 1, -3, SCRIPT_TYPE_STRING);
		CHECK_TYPE("sendMessageToAll", 2, -2, SCRIPT_TYPE_INTEGER);
		CHECK_TYPE("sendMessageToAll", 3, -1, SCRIPT_TYPE_BOOL);
		script_getstring(pVM, -3, &szMessage);
		script_getinteger(pVM, -2, &iColor);
		script_getbool(pVM, -1, &sqbAllowFormatting);
	}
	else if(vtop > 1)
	{
		CHECK_TYPE("sendMessageToAll", 1, -2, SCRIPT_TYPE_STRING);
		CHECK_TYPE("sendMessageToAll", 2, -1, SCRIPT_TYPE_INTEGER);
		script_getstring(pVM, -2, &szMessage);
		script_getinteger(pVM, -1, &iColor);
	}
	else
	{
		CHECK_TYPE("sendMessageToAll", 1, -1, SCRIPT_TYPE_STRING);
		script_getstring(pVM, -1, &szMessage);
	}

	if(g_pPlayerManager->GetPlayerCount() > 0)
	{
		CBitStream bsSend;
		bsSend.Write((DWORD)iColor);
		bsSend.Write(String(szMessage));
		bool bAllowFormatting = (sqbAllowFormatting != 0);
		bsSend.Write(bAllowFormatting);
		g_pNetworkManager->RPC(RPC_Message, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
		script_pushbool(pVM, true);
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

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

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

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		if(pPlayer->IsInVehicle())
		{
			script_pushbool(pVM, (pPlayer->GetVehicle()->GetVehicleId() == vehicleId));
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

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		if(pPlayer->IsInVehicle())
		{
			script_pushinteger(pVM, pPlayer->GetVehicle()->GetVehicleId());
			return 1;
		}
	}

	script_pushbool(pVM, false);
	return 1;
}

// getPlayerSeatId(playerid)
int CPlayerNatives::GetSeatId(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -1, &playerId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

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

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		script_pushbool(pVM, pPlayer->IsOnFoot());
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// togglePlayerPayAndSpray(playerid, toggle)
int CPlayerNatives::TogglePayAndSpray(lua_State * pVM)
{
	EntityId playerId;
	bool sqbToggle;
	script_getentity(pVM, -2, &playerId);
	script_getbool(pVM, -1, &sqbToggle);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bool bToggle = (sqbToggle != 0);
		bsSend.Write(bToggle);
		g_pNetworkManager->RPC(RPC_ScriptingTogglePayAndSpray, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE, playerId, false);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// togglePlayerAutoAim(playerid, toggle)
int CPlayerNatives::ToggleAutoAim(lua_State * pVM)
{
	EntityId playerId;
	bool sqbToggle;
	script_getentity(pVM, -2, &playerId);
	script_getbool(pVM, -1, &sqbToggle);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bool bToggle = (sqbToggle != 0);
		bsSend.Write(bToggle);
		g_pNetworkManager->RPC(RPC_ScriptingToggleAutoAim, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE, playerId, false);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// setPlayerDrunk(playerid, toggle)
/*int CPlayerNatives::SetPlayerDrunk(lua_State * pVM)
{
	int playerid;
	bool toggle;
	script_getinteger(vm, -2, &playerid);
	script_getinteger(vm, -1, (int*)&toggle);
	CBitStream bsSend;
	bsSend.Write(toggle);
	g_pNetworkManager->RPC("SetPlayerDrunk", &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerid, false);
	script_pushbool(vm, true);
	return 1;
}*/

// setPlayerSpawnLocation(playerid, x, y, z, r)
int CPlayerNatives::SetSpawnLocation(lua_State * pVM)
{
	EntityId playerId;
	CVector3 vecPos;
	float fRotation;
	script_getentity(pVM, -5, &playerId);
	script_getvector3(pVM, -4, &vecPos);
	script_getfloat(pVM, -1, &fRotation);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		pPlayer->SetSpawnLocation(vecPos, fRotation);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// setPlayerModel(playerid, model)
int CPlayerNatives::SetModel(lua_State * pVM)
{
	EntityId playerId;
	int iModelId;
	script_getentity(pVM, -2, &playerId);
	script_getinteger(pVM, -1, &iModelId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		script_pushbool(pVM, pPlayer->SetModel(iModelId));
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

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		script_pushinteger(pVM, pPlayer->GetModel());
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// togglePlayerControls(playerid, toggle)
int CPlayerNatives::ToggleControls(lua_State * pVM)
{
	EntityId playerId;
	bool sqbControls;
	script_getentity(pVM, -2, &playerId);
	script_getbool(pVM, -1, &sqbControls);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bool bControls = (sqbControls != 0);
		bsSend.Write(bControls);
		g_pNetworkManager->RPC(RPC_ScriptingToggleControls, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		script_pushbool(pVM, true);
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

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		script_pushbool(pVM, pPlayer->IsSpawned());
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// setPlayerHeading(playerid, heading)
int CPlayerNatives::SetHeading(lua_State * pVM)
{
	EntityId playerId;
	float fHeading;
	script_getentity(pVM, -2, &playerId);
	script_getfloat(pVM, -1, &fHeading);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write(fHeading);
		g_pNetworkManager->RPC(RPC_ScriptingSetHeading, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		script_pushbool(pVM, true);
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

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		script_pushfloat(pVM, pPlayer->GetCurrentHeading());
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// togglePlayerPhysics(playerid, toggle)
int CPlayerNatives::TogglePhysics(lua_State * pVM)
{
	EntityId playerId;
	bool sqbToggle;
	script_getentity(pVM, -2, &playerId);
	script_getbool(pVM, -1, &sqbToggle);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bool bToggle = (sqbToggle != 0);
		bsSend.Write(bToggle);
		g_pNetworkManager->RPC(RPC_ScriptingToggleRagdoll, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// kickPlayer(playerid, sendkickmessage)
int CPlayerNatives::Kick(lua_State * pVM)
{
	EntityId playerId;
	bool sqbKickMessage;
	script_getentity(pVM, -2, &playerId);
	script_getbool(pVM, -1, &sqbKickMessage);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		pPlayer->Kick(sqbKickMessage != 0);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// banPlayer(playerid, milliseconds)
int CPlayerNatives::Ban(lua_State * pVM)
{
	EntityId playerId;
	int uiSeconds;
	script_getentity(pVM, -2, &playerId);
	script_getinteger(pVM, -1, &uiSeconds);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		pPlayer->Ban(uiSeconds);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// getPlayerIp(playerid)
int CPlayerNatives::GetIp(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -1, &playerId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		String ip = pPlayer->GetIp();
		script_pushlstring(pVM, ip.Get(), ip.GetLength());
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// givePlayerMoney(playerid, money)
int CPlayerNatives::GiveMoney(lua_State * pVM)
{
	EntityId playerId;
	int iMoney;
	script_getentity(pVM, -2, &playerId);
	script_getinteger(pVM, -1, &iMoney);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		script_pushbool(pVM, pPlayer->GiveMoney(iMoney));
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// setPlayerMoney(playerid, money)
int CPlayerNatives::SetMoney(lua_State * pVM)
{
	EntityId playerId;
	int iMoney;
	script_getentity(pVM, -2, &playerId);
	script_getinteger(pVM, -1, &iMoney);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		script_pushbool(pVM, pPlayer->SetMoney(iMoney));
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// resetPlayerMoney(playerid)
int CPlayerNatives::ResetMoney(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -1, &playerId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		script_pushbool(pVM, pPlayer->SetMoney(0));
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

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		script_pushinteger(pVM, pPlayer->GetMoney());
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}


// togglePlayerFrozen(playerid, playerfrozen, camerafrozen)
int CPlayerNatives::ToggleFrozen(lua_State * pVM)
{
	EntityId playerId;
	bool sqbPlayerFrozen;
	bool sqbCameraFrozen;
	script_getentity(pVM, -2, &playerId);
	// jenksta: is there a reason these are both the same?
	script_getbool(pVM, -1, &sqbPlayerFrozen);
	script_getbool(pVM, -1, &sqbCameraFrozen);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bool bPlayerFrozen = (sqbPlayerFrozen != 0);
		bsSend.Write(bPlayerFrozen);
		bool bCameraFrozen = (sqbCameraFrozen != 0);
		bsSend.Write(bCameraFrozen);
		g_pNetworkManager->RPC(RPC_ScriptingToggleFrozen, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		script_pushbool(pVM, true);
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

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

		if(pPlayer)
		{
			script_pushinteger(pVM, pPlayer->GetState());
			return 1;
		}
	}
	else if(playerId >= 0 && playerId < MAX_PLAYERS)
	{
		script_pushinteger(pVM, STATE_TYPE_DISCONNECT);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// displayPlayerText(playerid, x, y, text, time)
int CPlayerNatives::DisplayText(lua_State * pVM)
{
	EntityId playerId;
	float fPos[2];
	const char * szText;
	int iTime;
	script_getentity(pVM, -5, &playerId);
	script_getfloat(pVM, -4, &fPos[0]);
	script_getfloat(pVM, -3, &fPos[1]);
	script_getstring(pVM, -2, &szText);
	script_getinteger(pVM, -1, &iTime);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write(fPos[0]);
		bsSend.Write(fPos[1]);
		bsSend.Write(String(szText));
		bsSend.Write((int)iTime);
		g_pNetworkManager->RPC(RPC_ScriptingDisplayText, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// displayTextForAll(x, y, text, time)
int CPlayerNatives::DisplayTextForAll(lua_State * pVM)
{
	float fPos[2];
	const char * szText;
	int iTime;
	script_getfloat(pVM, -4, &fPos[0]);
	script_getfloat(pVM, -3, &fPos[1]);
	script_getstring(pVM, -2, &szText);
	script_getinteger(pVM, -1, &iTime);
	CBitStream bsSend;
	bsSend.Write(fPos[0]);
	bsSend.Write(fPos[1]);
	bsSend.Write(String(szText));
	bsSend.Write((int)iTime);
	g_pNetworkManager->RPC(RPC_ScriptingDisplayText, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
	script_pushbool(pVM, true);
	return 1;
}

// displayInfoText(playerid, text, time)
int CPlayerNatives::DisplayInfoText(lua_State * pVM)
{
	EntityId playerId;
	const char * szText;
	int iTime;
	script_getentity(pVM, -3, &playerId);
	script_getstring(pVM, -2, &szText);
	script_getinteger(pVM, -1, &iTime);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write(String(szText));
		bsSend.Write((int)iTime);
		g_pNetworkManager->RPC(RPC_ScriptingDisplayInfoText, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// displayInfoTextForAll(playerid, text, time)
int CPlayerNatives::DisplayInfoTextForAll(lua_State * pVM)
{
	const char * szText;
	int iTime;
	script_getstring(pVM, -2, &szText);
	script_getinteger(pVM, -1, &iTime);
	CBitStream bsSend;
	bsSend.Write(String(szText));
	bsSend.Write((int)iTime);
	g_pNetworkManager->RPC(RPC_ScriptingDisplayInfoText, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
	script_pushbool(pVM, true);
	return 1;
}

// setPlayerVelocity(playerid, x, y, z)
int CPlayerNatives::SetVelocity(lua_State * pVM)
{
	EntityId playerId;
	CVector3 vecMoveSpeed;
	script_getentity(pVM, -4, &playerId);
	script_getvector3(pVM, -3, &vecMoveSpeed);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write(vecMoveSpeed);
		g_pNetworkManager->RPC(RPC_ScriptingSetPlayerMoveSpeed, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		script_pushbool(pVM, true);
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

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		CVector3 vecMoveSpeed;
		pPlayer->GetMoveSpeed(vecMoveSpeed);
		CScriptArray args(pVM);
		args.push(vecMoveSpeed.fX);
		args.push(vecMoveSpeed.fY);
		args.push(vecMoveSpeed.fZ);
		//script_pusharg(pVM, CScriptArgument(args, true));
		//CScriptArgument(args, true).push(pVM);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// warpPlayerIntoVehicle(playerid, vehicleid, seatid = 0)
int CPlayerNatives::WarpIntoVehicle(lua_State * pVM)
{
	int iParams = script_gettop(pVM)/* - 1*/;

	CHECK_PARAMS_MIN("warpPlayerIntoVehicle", 2);
	CHECK_TYPE("warpPlayerIntoVehicle", 1, -iParams, SCRIPT_TYPE_INTEGER);
	CHECK_TYPE("warpPlayerIntoVehicle", 2, -iParams + 1, SCRIPT_TYPE_INTEGER);

	int iSeatId = 0;

	if(iParams >= 3)
	{
		CHECK_TYPE("warpPlayerIntoVehicle", 3, -iParams + 2, SCRIPT_TYPE_INTEGER);
		script_getinteger(pVM, -iParams + 2, &iSeatId);
	}

	EntityId playerId;
	EntityId vehicleId;
	script_getentity(pVM, -iParams, &playerId);
	script_getentity(pVM, -iParams + 1, &vehicleId);

	if(g_pPlayerManager->DoesExist(playerId) && g_pVehicleManager->DoesExist(vehicleId))
	{
		CBitStream bsSend;
		bsSend.Write((EntityId)vehicleId);
		bsSend.Write((BYTE)iSeatId);
		g_pNetworkManager->RPC(RPC_ScriptingWarpPlayerIntoVehicle, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// removePlayerFromVehicle(playerid)
int CPlayerNatives::RemoveFromVehicle(lua_State * pVM)
{
	CHECK_PARAMS_MIN("removePlayerFromVehicle", 1);
	CHECK_TYPE("removePlayerFromVehicle", 1, 1, SCRIPT_TYPE_INTEGER);

	EntityId playerId;
	script_getentity(pVM, 1, &playerId);

	if(!g_pPlayerManager->DoesExist(playerId))
	{
		script_pushbool(pVM, false);
		return 1;
	}

	bool bGraceful = false;
	if(script_gettop(pVM) >= 3)
	{
		CHECK_TYPE("removePlayerFromVehicle", 2, 2, SCRIPT_TYPE_BOOL);
		bool b;
		script_getbool(pVM, 2, &b);
		bGraceful = (b != 0);
	}

	CBitStream bsSend;
	bsSend.WriteBit(bGraceful);
	g_pNetworkManager->RPC(RPC_ScriptingRemovePlayerFromVehicle, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
	script_pushbool(pVM, true);
	return 1;
}

// getPlayerWeapon(playerid)
int CPlayerNatives::GetWeapon(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -1, &playerId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		script_pushinteger(pVM, pPlayer->GetWeapon());
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

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		script_pushinteger(pVM, pPlayer->GetAmmo());
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// getPlayerSerial(playerid)
int CPlayerNatives::GetSerial(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -1, &playerId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(!pPlayer)
	{
		script_pushbool(pVM, false);
		return 1;
	}

	script_pushstring(pVM, pPlayer->GetSerial().Get());
	return 1;
}

// setCameraBehindPlayer(playerid)
int CPlayerNatives::SetCameraBehind(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -1, &playerId);

	if(!g_pPlayerManager->DoesExist(playerId))
	{
		script_pushbool(pVM, false);
		return 1;
	}

	CBitStream bsSend;
	g_pNetworkManager->RPC(RPC_ScriptingSetCameraBehindPlayer, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
	script_pushbool(pVM, true);

	return 1;
}

// setPlayerDucking(playerid, ducking)
int CPlayerNatives::SetDucking(lua_State * pVM)
{
	EntityId playerId;
	bool sqbDucking;
	script_getentity(pVM, -2, &playerId);
	script_getbool(pVM, -1, &sqbDucking);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write(sqbDucking != 0);
		g_pNetworkManager->RPC(RPC_ScriptingSetPlayerDucking, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		script_pushbool(pVM, true);
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

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		script_pushbool(pVM, pPlayer->IsDucking());
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// setPlayerInvincible(playerid, invincible)
int CPlayerNatives::SetInvincible(lua_State * pVM)
{
	EntityId playerId;
	bool sqbInvincible;
	script_getentity(pVM, -2, &playerId);
	script_getbool(pVM, -1, &sqbInvincible);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write(sqbInvincible != 0);
		g_pNetworkManager->RPC(RPC_ScriptingSetPlayerInvincible, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// togglePlayerHUD(playerid, toggle)
int CPlayerNatives::ToggleHUD(lua_State * pVM)
{
	EntityId playerId;
	bool sqbToggle;
	script_getentity(pVM, -2, &playerId);
	script_getbool(pVM, -1, &sqbToggle);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write(sqbToggle != 0);
		g_pNetworkManager->RPC(RPC_ScriptingToggleHUD, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// togglePlayerRadar(playerid, toggle)
int CPlayerNatives::ToggleRadar(lua_State * pVM)
{
	EntityId playerId;
	bool sqbToggle;
	script_getentity(pVM, -2, &playerId);
	script_getbool(pVM, -1, &sqbToggle);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bool bToggle = (sqbToggle != 0);
		bsSend.Write(bToggle);
		g_pNetworkManager->RPC(RPC_ScriptingToggleRadar, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// togglePlayerNames(playerid, toggle)
int CPlayerNatives::ToggleNames(lua_State * pVM)
{
	EntityId playerId;
	bool sqbToggle;
	script_getentity(pVM, -2, &playerId);
	script_getbool(pVM, -1, &sqbToggle);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bool bToggle = (sqbToggle != 0);
		bsSend.Write(bToggle);
		g_pNetworkManager->RPC(RPC_ScriptingToggleNames, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// togglePlayerAreaNames(playerid, toggle)
int CPlayerNatives::ToggleAreaNames(lua_State * pVM)
{
	EntityId playerId;
	bool sqbToggle;
	script_getentity(pVM, -2, &playerId);
	script_getbool(pVM, -1, &sqbToggle);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bool bToggle = (sqbToggle != 0);
		bsSend.Write(bToggle);
		g_pNetworkManager->RPC(RPC_ScriptingToggleAreaNames, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// getEmptyPlayerControlState()
int CPlayerNatives::GetEmptyControlState(lua_State * pVM)
{
	// Create a new control state
	CControlState controlState;

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
	//script_pusharg(pVM, CScriptArgument(table, false));
	CScriptArgument(table).push(pVM);
	return 1;
}

// getPlayerPreviousControlState(playerid)
int CPlayerNatives::GetPreviousControlState(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -1, &playerId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

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
		//script_pusharg(pVM, CScriptArgument(table, false));
		CScriptArgument(table).push(pVM);
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

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

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
		//script_pusharg(pVM, CScriptArgument(table, false));
		CScriptArgument(table).push(pVM);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// triggerClientEvent(playerid, eventname, ...)
int CPlayerNatives::TriggerEvent(lua_State * pVM)
{
	CHECK_PARAMS_MIN("triggerClientEvent", 2);
	CHECK_TYPE("triggerClientEvent", 1, 1, SCRIPT_TYPE_INTEGER);
	CHECK_TYPE("triggerClientEvent", 2, 2, SCRIPT_TYPE_STRING);

	int playerId;
	script_getinteger(pVM, 1, &playerId);
	CScriptArguments arguments(pVM, 2);

	CBitStream bsSend;
	arguments.serialize(&bsSend);
	g_pNetworkManager->RPC(RPC_ScriptingEventCall, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
	script_pushbool(pVM, true);
	return 1;
}

// setPlayerColor(playerid, rgba)
int CPlayerNatives::SetColor(lua_State * pVM)
{
	EntityId playerId;
	int color;
	script_getentity(pVM, -2, &playerId);
	script_getinteger(pVM, -1, &color);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		pPlayer->SetColor(color);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

int CPlayerNatives::GetColor(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, 1, &playerId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		script_pushinteger(pVM, pPlayer->GetColor());
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

int CPlayerNatives::GetPing(lua_State* pVM)
{
	EntityId playerId;
	script_getentity(pVM, 1, &playerId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		script_pushinteger(pVM, pPlayer->GetPing());
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

int CPlayerNatives::SetClothes(lua_State * pVM)
{
	int iPlayerId, iBodyPart, iClothes;
	script_getinteger(pVM, 1, &iPlayerId);
	script_getinteger(pVM, 2, &iBodyPart);
	script_getinteger(pVM, 3, &iClothes);

	if(g_pPlayerManager->DoesExist(iPlayerId) && iBodyPart >= 0 && iBodyPart <= 10 && iClothes >= 0 && iClothes <= 255)
	{
		g_pPlayerManager->GetAt(iPlayerId)->SetClothes((unsigned char)iBodyPart, (unsigned char)iClothes);
		script_pushbool(pVM, true);
		return 1;
	}
	
	script_pushbool(pVM, false);
	return 1;
}

int CPlayerNatives::GetClothes(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, 1, &playerId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		CScriptArray a(pVM);

		for(unsigned char uc = 0; uc < 11; uc ++ )
		{
			a.push(pPlayer->GetClothes(uc));
			//script_arrayappend(pVM, -2);
		}

		return 1;
	}
	
	script_pushbool(pVM, false);
	return 1;
}

int CPlayerNatives::ResetClothes(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, 1, &playerId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		pPlayer->ResetClothes();
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

int CPlayerNatives::Respawn(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, 1, &playerId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		CBitStream bitStream;
		bitStream.WriteCompressed(playerId);
		g_pNetworkManager->RPC(RPC_PlayerSpawn, &bitStream, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, pPlayer->GetPlayerId(), false);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

int CPlayerNatives::GiveHelmet(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -1, &playerId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		pPlayer->GiveHelmet();
		script_pushbool(pVM, true);
		return 1;
	}
	
	script_pushbool(pVM, false);
	return 1;
}

int CPlayerNatives::RemoveHelmet(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -1, &playerId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		pPlayer->RemoveHelmet();
		script_pushbool(pVM, true);
		return 1;
	}
	
	script_pushbool(pVM, false);
	return 1;
}

// togglePlayerHelmet(playerid, toggle)
int CPlayerNatives::ToggleHelmet(lua_State * pVM)
{
	EntityId playerId;
	bool sqbToggle;
	script_getentity(pVM, -2, &playerId);
	script_getbool(pVM, -1, &sqbToggle);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);

	if(pPlayer)
	{
		bool bToggle = (sqbToggle != 0);
		if(bToggle)
			pPlayer->GiveHelmet();
		else
			pPlayer->RemoveHelmet();

		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// setPlayerCameraPos(playerid, x, y, z)
int CPlayerNatives::SetCameraPos(lua_State * pVM)
{
	EntityId playerId;
	CVector3 vecPos;
	script_getentity(pVM, -4, &playerId);
	script_getvector3(pVM, -3, &vecPos);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write(vecPos);
		g_pNetworkManager->RPC(RPC_ScriptingSetPlayerCameraPos, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// setPlayerCameraLookAt(playerid, x, y, z)
int CPlayerNatives::SetCameraLookAt(lua_State * pVM)
{
	EntityId playerId;
	CVector3 vecPos;
	script_getentity(pVM, -4, &playerId);
	script_getvector3(pVM, -3, &vecPos);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write(vecPos);
		g_pNetworkManager->RPC(RPC_ScriptingSetPlayerCameraLookAt, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// resetPlayerCamera(playerid)
int CPlayerNatives::ResetCamera(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -1, &playerId);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		g_pNetworkManager->RPC(RPC_ScriptingResetPlayerCamera, NULL, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

int CPlayerNatives::forceAnim(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -3, &playerId);

	const char *szGroup = NULL;
	script_getstring(pVM,-2,&szGroup);

	const char *szAnim = NULL;
	script_getstring(pVM,-1,&szAnim);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write(playerId);
		bsSend.Write(String(szGroup));
		bsSend.Write(String(szAnim));
		g_pNetworkManager->RPC(RPC_ScriptingForcePlayerAnim, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

int CPlayerNatives::requestAnim(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -2, &playerId);
	
	const char *szAnim = NULL;
	script_getstring(pVM,-1,&szAnim);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write(String(szAnim));
		g_pNetworkManager->RPC(RPC_ScriptingRequestAnims, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		script_pushbool(pVM, true);
		return 1;
	}
	script_pushbool(pVM, false);
	return 1;
}

int CPlayerNatives::releaseAnim(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -2, &playerId);
	
	const char *szAnim = NULL;
	script_getstring(pVM,-1,&szAnim);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write(String(szAnim));
		g_pNetworkManager->RPC(RPC_ScriptingReleaseAnims, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		script_pushbool(pVM, true);
		return 1;
	}
	script_pushbool(pVM, false);
	return 1;
}

int CPlayerNatives::triggerAudioEvent(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -2, &playerId);

	const char *szAudio = NULL;
	script_getstring(pVM,-1,&szAudio);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write(String(szAudio));
		g_pNetworkManager->RPC(RPC_ScriptingPlayGameAudio, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		script_pushbool(pVM, true);
		return 1;
	}
	script_pushbool(pVM, false);
	return 1;
}

int CPlayerNatives::triggerMissionCompleteAudio(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -2, &playerId);

	int szMission;
	script_getinteger(pVM,-1,&szMission);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write((int)szMission);
		g_pNetworkManager->RPC(RPC_ScriptingPlayMissionCompleteAudio, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		script_pushbool(pVM, true);
		return 1;
	}
	script_pushbool(pVM, false);
	return 1;
}

int CPlayerNatives::triggerPoliceReport(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -2, &playerId);

	const char *szAudio = NULL;
	script_getstring(pVM,-1,&szAudio);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write(String(szAudio));
		g_pNetworkManager->RPC(RPC_ScriptingPlayPoliceReport, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		script_pushbool(pVM, true);
		return 1;
	}
	script_pushbool(pVM, false);
	return 1;
}

int CPlayerNatives::fadeScreenIn(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -2, &playerId);

	int iDuration;
	script_getinteger(pVM,-1,&iDuration);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write((int)iDuration);
		g_pNetworkManager->RPC(RPC_ScriptingFadeScreenIn, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		script_pushbool(pVM, true);
		return 1;
	}
	script_pushbool(pVM, false);
	return 1;
}

int CPlayerNatives::fadeScreenOut(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -2, &playerId);

	int iDuration;
	script_getinteger(pVM,-1,&iDuration);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write((int)iDuration);
		g_pNetworkManager->RPC(RPC_ScriptingFadeScreenOut, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		script_pushbool(pVM, true);
		return 1;
	}
	script_pushbool(pVM, false);
	return 1;
}

int CPlayerNatives::blockWeaponChange(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -2, &playerId);

	bool bToggle;
	script_getbool(pVM,-1,&bToggle);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		bool bSwitch = (bToggle != 0);
		CBitStream bsSend;
		bsSend.Write(bSwitch);
		g_pNetworkManager->RPC(RPC_ScriptingBlockWeaponChange, NULL, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		script_pushbool(pVM, true);
		return 1;
	}
	script_pushbool(pVM, false);
	return 1;
}

int CPlayerNatives::blockWeaponDrop(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -2, &playerId);

	bool bToggle;
	script_getbool(pVM,-1,&bToggle);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		bool bSwitch = (bToggle != 0);
		CBitStream bsSend;
		bsSend.Write(bSwitch);
		g_pNetworkManager->RPC(RPC_ScriptingBlockWeaponDrop, NULL, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		script_pushbool(pVM, true);
		return 1;
	}
	script_pushbool(pVM, false);
	return 1;
}

int CPlayerNatives::AttachCamToPlayer(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -6, &playerId);

	EntityId toPlayerId;
	script_getentity(pVM, -5, &toPlayerId);

	int iPointType;
	script_getinteger(pVM, -4, &iPointType);

	CVector3 vecPos;
	script_getfloat(pVM, -3, &vecPos.fX);
	script_getfloat(pVM, -2, &vecPos.fY);
	script_getfloat(pVM, -1, &vecPos.fZ);

	if(g_pPlayerManager->DoesExist(playerId) && g_pPlayerManager->DoesExist(toPlayerId))
	{
		CBitStream bsSend;
		bsSend.WriteCompressed(toPlayerId);
		bsSend.Write(vecPos);
		bsSend.Write(iPointType);
		bsSend.Write0();
		g_pNetworkManager->RPC(RPC_ScriptingAttachCam, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		script_pushbool(pVM, true);
		return 1;
	}
	script_pushbool(pVM, false);
	return 1;
}

int CPlayerNatives::AttachCamToVehicle(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -6, &playerId);

	EntityId toVehicleId;
	script_getentity(pVM, -5, &toVehicleId);

	int iPointType;
	script_getinteger(pVM, -4, &iPointType);

	CVector3 vecPos;
	script_getfloat(pVM, -3, &vecPos.fX);
	script_getfloat(pVM, -2, &vecPos.fY);
	script_getfloat(pVM, -1, &vecPos.fZ);


	if(g_pPlayerManager->DoesExist(playerId) && g_pVehicleManager->DoesExist(toVehicleId))
	{
		CBitStream bsSend;
		bsSend.WriteCompressed(toVehicleId);
		bsSend.Write(vecPos);
		bsSend.Write(iPointType);
		bsSend.Write1();
		g_pNetworkManager->RPC(RPC_ScriptingAttachCam, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		script_pushbool(pVM, true);
		return 1;
	}
	script_pushbool(pVM, false);
	return 1;
}

int CPlayerNatives::DisplayHudNotification(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -3, &playerId);
	
	int iMode;
	script_getinteger(pVM, -2, &iMode);

	const char * szMessage;
	script_getstring(pVM, -1, &szMessage);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write((int)iMode);
		bsSend.Write(String(szMessage));
		g_pNetworkManager->RPC(RPC_ScriptingDisplayHudNotification, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		script_pushbool(pVM, true);
		return 1;
	}
	script_pushbool(pVM, false);
	return 1;
}

int CPlayerNatives::FollowVehicleMode(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -2, &playerId);
	
	int iMode;
	script_getinteger(pVM, -1, &iMode);

	if(iMode < 0 || iMode > 5)
	{
		CLogFile::Print("Vehicle follow modes are only supported from 0 to 5!");
		script_pushbool(pVM,false);
		return 1;
	}

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write((int)iMode);
		g_pNetworkManager->RPC(RPC_ScriptingSetVehicleFollowMode, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		script_pushbool(pVM, true);
		return 1;
	}
	script_pushbool(pVM, false);
	return 1;
}

int CPlayerNatives::FollowVehicleOffset(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -5, &playerId);

	EntityId vehicleId;
	script_getentity(pVM, -4, &vehicleId);

	CVector3 vecPos;
	script_getfloat(pVM, -3, &vecPos.fX);
	script_getfloat(pVM, -2, &vecPos.fY);
	script_getfloat(pVM, -1, &vecPos.fZ);

	if(g_pPlayerManager->DoesExist(playerId) && g_pVehicleManager->DoesExist(vehicleId))
	{
		CBitStream bsSend;
		bsSend.Write(vehicleId);
		bsSend.Write(vecPos);
		g_pNetworkManager->RPC(RPC_ScriptingSetVehicleFollowOffset, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		script_pushbool(pVM, true);
		return 1;
	}
	script_pushbool(pVM, false);
	return 1;
}

int CPlayerNatives::SetAmmoInClip(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -2, &playerId);

	int iAmmoInClip;
	script_getinteger(pVM, -1, &iAmmoInClip);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write((int)iAmmoInClip);
		g_pNetworkManager->RPC(RPC_ScriptingSetAmmoInClip, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		script_pushbool(pVM, true);
		return 1;
	}
	script_pushbool(pVM, false);
	return 1;
}

int CPlayerNatives::SetAmmo(lua_State * pVM)
{
	EntityId playerId;
	int iWeaponId;
	int iAmmo;

	script_getentity(pVM, -3, &playerId);
	script_getinteger(pVM, -2, &iWeaponId);
	script_getinteger(pVM, -1, &iAmmo);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write((int)iWeaponId);
		bsSend.Write((int)iAmmo);
		g_pNetworkManager->RPC(RPC_ScriptingSetAmmo, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		script_pushbool(pVM, true);
		return 1;
	}
	script_pushbool(pVM, false);
	return 1;
}

int CPlayerNatives::SetMobilePhone(lua_State * pVM)
{
	EntityId playerId;
	bool bUse;

	script_getentity(pVM, -2, &playerId);
	script_getbool(pVM, -1, &bUse);

	bool bToggle = (bUse != 0);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		g_pPlayerManager->GetAt(playerId)->UseMobilePhone(bToggle);
		CBitStream bsSend;
		bsSend.Write(playerId);
		bsSend.Write(bToggle);
		g_pNetworkManager->RPC(RPC_ScriptingSetPlayerUseMobilePhone, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
		script_pushbool(pVM, true);
		return 1;
	}
	script_pushbool(pVM, false);
	return 1;
}

int CPlayerNatives::SaySpeech(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM, -3, &playerId);
	
	const char * szVoice;
	script_getstring(pVM, -2, &szVoice);

	const char * szText;
	script_getstring(pVM, -1, &szText);

	if(g_pPlayerManager->DoesExist(playerId))
	{
		CBitStream bsSend;
		bsSend.Write(playerId);
		bsSend.Write(String(szVoice));
		bsSend.Write(String(szText));
		g_pNetworkManager->RPC(RPC_ScriptingPlayerSaySpeech, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
		script_pushbool(pVM, true);
		return 1;
	}
	script_pushbool(pVM, false);
	return 1;
}

int CPlayerNatives::ToggleNametagForPlayer(lua_State * pVM)
{
	EntityId playerId;
	EntityId forPlayerId;
	bool bToggle;

	script_getentity(pVM, -3, &playerId);
	script_getentity(pVM, -2, &forPlayerId);
	script_getbool(pVM, -1, &bToggle);

	if(g_pPlayerManager->DoesExist(playerId) && g_pPlayerManager->DoesExist(forPlayerId))
	{
		bool bShow = (bToggle != 0);

		CBitStream bsSend;
		bsSend.Write(forPlayerId);
		bsSend.Write(bShow);
		g_pNetworkManager->RPC(RPC_ScriptingTogglePlayerLabelForPlayer, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
		
		script_pushbool(pVM, true);
		return 1;
	}
	script_pushbool(pVM, false);
	return 1;
	
}

int CPlayerNatives::DriveAutomatic(lua_State * pVM)
{
	EntityId playerId;
	script_getentity(pVM,-7,&playerId);

	EntityId vehicleId;
	script_getentity(pVM,-6,&vehicleId);

	CVector3 vecPos;
	script_getfloat(pVM,-5,&vecPos.fX);
	script_getfloat(pVM,-4,&vecPos.fY);
	script_getfloat(pVM,-3,&vecPos.fZ);

	float fSpeed;
	script_getfloat(pVM,-2,&fSpeed);

	int iDrivingStyle;
	script_getinteger(pVM,-1,&iDrivingStyle);

	// Check if we have a valid drivingstyle
	if(iDrivingStyle < 0 || iDrivingStyle > 3)
	{
		CLogFile::Print("Failed to activate automatic vehicle drive(wrong drivingstyle(supported from 0 to 3))");
		script_pushbool(pVM,false);
		return false;
	}

	// Check if we have a valid vehicle
	if(!g_pVehicleManager->DoesExist(vehicleId))
	{
		CLogFile::Print("Failed to activate automatic vehicle drive(wrong vehicle(not valid))");
		script_pushbool(pVM,false);
		return false;
	}

	// Check if we are in your vehicle
	if(g_pVehicleManager->GetAt(vehicleId)->GetDriver() != NULL)
	{
		if(g_pVehicleManager->GetAt(vehicleId)->GetDriver()->GetPlayerId() != playerId)
		{
			CLogFile::Print("Failed to activate automatic vehicle drive(given player is not as driver in given vehicle)");
			script_pushbool(pVM,false);
			return false;
		}
		else {
			CBitStream bsSend;
			bsSend.Write(playerId);
			bsSend.Write(vehicleId);
			bsSend.Write(vecPos);
			bsSend.Write(fSpeed);
			bsSend.Write((int)iDrivingStyle);
			g_pNetworkManager->RPC(RPC_ScriptingLetPlayerDriveAutomatic, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
			script_pushbool(pVM, true);
			return 1;
		}
	}
	script_pushbool(pVM, false);
	return 1;
}

int CPlayerNatives::SetDimension(lua_State * pVM)
{
	int iDimension;
	EntityId playerId;

	script_getinteger(pVM, -1, &iDimension);
	script_getentity(pVM, -2, &playerId);
	
	CPlayer* pPlayer = g_pPlayerManager->GetAt(playerId);
	if(pPlayer) {
		pPlayer->SetDimension(iDimension);
		CBitStream bsSend;
		bsSend.Write(playerId);
		bsSend.Write(iDimension);

		g_pNetworkManager->RPC(RPC_ScriptingSetPlayerDimension, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
		script_pushbool(pVM, true);
		return true;
	}
	script_pushbool(pVM, false);
	return false;
}

int CPlayerNatives::GetDimension(lua_State * pVM)
{ 
	EntityId playerId;

	script_getentity(pVM, -1, &playerId);

	CPlayer * pPlayer = g_pPlayerManager->GetAt(playerId);
	if(pPlayer)
	{
		script_pushinteger(pVM, pPlayer->GetDimension());
		return 1;
	}

	script_pushinteger(pVM, -1);
	return 1;
}

int CPlayerNatives::GetFileChecksum(lua_State * pVM)
{ 
	EntityId playerId;
	int iFile;

	script_getentity(pVM, -2, &playerId);
	script_getinteger(pVM, -1, &iFile);
	if(iFile >= 0 && iFile < 2) {
		if(g_pPlayerManager->DoesExist(playerId)) {
			script_pushinteger(pVM, g_pPlayerManager->GetAt(playerId)->GetFileChecksum(iFile));
			return 1;
		}
		else {
			script_pushbool(pVM, false);
			return 1;
		}
	}
	script_pushbool(pVM, false);
	return 1;
}