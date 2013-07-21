//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: ServerNatives.cpp
// Project: Server.Core
// Author(s): jenksta
//            Einstein
//            Sebihunter
// License: See LICENSE in root directory
//
//==============================================================================

#include "../Natives.h"
#include "../CServer.h"
#include "Scripting/CScriptTimerManager.h"
#include "Scripting/CScriptingManager.h"
#include "../CNetworkManager.h"
#include "../CPlayerManager.h"
#include "tinyxml/tinyxml.h"
#include "tinyxml/ticpp.h"
#include <CSettings.h>
#include "../CQuery.h"
#include <SharedUtility.h>

extern CServer           * g_pServer;
extern CPlayerManager    * g_pPlayerManager;
extern CNetworkManager   * g_pNetworkManager;
extern CQuery            * g_pQuery;
extern CScriptingManager * g_pScriptingManager;

// Server functions

void CServerNatives::Register(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterFunction("log", Log, 1, "s");
	pScriptingManager->RegisterFunction("logf", LogF, 1, "s");
	pScriptingManager->RegisterFunction("addRule", AddRule, 2, "ss");
	pScriptingManager->RegisterFunction("removeRule", RemoveRule, 1, "s");
	pScriptingManager->RegisterFunction("setRule", SetRule, 2, "ss");
	pScriptingManager->RegisterFunction("getRule", GetRule, 1, "s");
	pScriptingManager->RegisterFunction("shutdown", Shutdown, 0, NULL);
	pScriptingManager->RegisterFunction("setServerPassword", SetPassword, 1, "s");
	pScriptingManager->RegisterFunction("getServerPassword", GetPassword, 0, NULL);
	pScriptingManager->RegisterFunction("getPlayerCount", GetPlayerCount, 0, NULL);
	pScriptingManager->RegisterFunction("getPlayers", GetPlayers, 0, NULL);
	pScriptingManager->RegisterFunction("getPlayerSlots", GetPlayerSlots, 0, NULL);
	pScriptingManager->RegisterFunction("getTickCount", GetTickCount, 0, NULL);
	pScriptingManager->RegisterFunction("setHostname", SetHostName, 1, "s");
	pScriptingManager->RegisterFunction("getHostname", GetHostName, 0, NULL);
	pScriptingManager->RegisterFunction("togglePayAndSpray", TogglePayAndSpray, 1, "b");
	pScriptingManager->RegisterFunction("toggleAutoAim", ToggleAutoAim, 1, "b");
	pScriptingManager->RegisterFunction("sendConsoleInput", SendInput, 1, "s");
	pScriptingManager->RegisterFunction("getConfig", GetConfig, 0, NULL);
	pScriptingManager->RegisterFunction("getWeaponName", GetWeaponName, 1, "i");
	pScriptingManager->RegisterFunction("getVehicleName", GetVehicleName, 1, "i");
	pScriptingManager->RegisterFunction("toggleFrequentEvents", ToggleFrequentEvents, 1, "b");
	pScriptingManager->RegisterFunction("areFrequentEventsEnabled", AreFrequentEventsEnabled, 0, NULL);
	pScriptingManager->RegisterFunction("forceWind",ForceWind,1,"f");
	pScriptingManager->RegisterFunction("setWeather", SetWeather, 1, "i");
	pScriptingManager->RegisterFunction("getWeather", GetWeather, 0, NULL);
}

// log(string)
int CServerNatives::Log(lua_State * pVM)
{
	const char* text;
	script_getstring(pVM, -1, &text);
	CLogFile::Print(text);
	script_pushbool(pVM, true);
	return 1;
}

// logf(string)
int CServerNatives::LogF(lua_State * pVM)
{
	const char * text;
	script_getstring(pVM, -1, &text);
	CLogFile::PrintfToFile(text);
	script_pushbool(pVM, true);
	return 1;
}

// addRule(name, value)
int CServerNatives::AddRule(lua_State * pVM)
{
	if(!g_pQuery)
	{
		// If <query> is disabled on server.
		script_pushbool(pVM, false);
		return 1;
	}

	const char* rule;
	const char* value;
	script_getstring(pVM, -2, &rule);
	script_getstring(pVM, -1, &value);
	script_pushbool(pVM, g_pQuery->AddRule(rule, value));
	return 1;
}

// removeRule(name)
int CServerNatives::RemoveRule(lua_State * pVM)
{
	if(!g_pQuery)
	{
		// If <query> is disabled on server.
		script_pushbool(pVM, false);
		return 1;
	}

	const char* rule;
	script_getstring(pVM, -1, &rule);
	script_pushbool(pVM, g_pQuery->RemoveRule(rule));
	return 1;
}

// setRule(name, value)
int CServerNatives::SetRule(lua_State * pVM)
{	
	if(!g_pQuery)
	{
		// If <query> is disabled on server.
		script_pushbool(pVM, false);
		return 1;
	}

	const char* rule;
	const char* value;
	script_getstring(pVM, -2, &rule);
	script_getstring(pVM, -1, &value);
	script_pushbool(pVM, g_pQuery->SetRule(rule, value));
	return 1;
}

// getRule(name)
int CServerNatives::GetRule(lua_State * pVM)
{
	if(!g_pQuery)
	{
		// If <query> is disabled on server.
		script_pushbool(pVM, false);
		return 1;
	}

	const char* rule;
	script_getstring(pVM, -1, &rule);
	QueryRule * pRule = g_pQuery->GetRule(rule);

	if(pRule)
		script_pushstring(pVM, pRule->strValue.Get());
	else
		script_pushbool(pVM, false);

	return 1;
}

// shutdown()
int CServerNatives::Shutdown(lua_State * pVM)
{
	g_pServer->SetActive(false);
	script_pushbool(pVM, true);
	return 1;
}

// setServerPassword(password)
int CServerNatives::SetPassword(lua_State * pVM)
{
	const char* pass;
	script_getstring(pVM, -1, &pass);
	g_pNetworkManager->GetNetServer()->SetPassword(pass);
	CVAR_SET_STRING("password",String(pass));
	script_pushbool(pVM, true);
	return 1;
}

// getServerPassword()
int CServerNatives::GetPassword(lua_State * pVM)
{
	String sPass = CVAR_GET_STRING("password");
	script_pushlstring(pVM, sPass.C_String(), sPass.GetLength());
	return 1;
}

// getPlayerCount()
int CServerNatives::GetPlayerCount(lua_State * pVM)
{
	script_pushinteger(pVM, g_pPlayerManager->GetPlayerCount());
	return 1;
}

// getPlayers()
int CServerNatives::GetPlayers(lua_State * pVM)
{
	int iCount = 0;

	script_newtable(pVM, 0);

	for(EntityId i = 0; i < MAX_PLAYERS; i++)
	{
		if(g_pPlayerManager->DoesExist(i))
		{
			CPlayer * pPlayer = g_pPlayerManager->GetAt(i);

			if(pPlayer)
			{
				script_pushinteger(pVM, i);
				script_pushstring(pVM, pPlayer->GetName());
				script_tableset(pVM, -3);
				++iCount;
			}
		}
	}

	return 1;
}

// getPlayerSlots()
int CServerNatives::GetPlayerSlots(lua_State * pVM)
{
	script_pushinteger(pVM, CVAR_GET_INTEGER("maxplayers"));
	return 1;
}

// getTickCount()
int CServerNatives::GetTickCount(lua_State * pVM)
{
	script_pushinteger(pVM, SharedUtility::GetTime());
	return 1;
}

// setHostname(hostname)
int CServerNatives::SetHostName(lua_State * pVM)
{
	const char * szHostname;
	script_getstring(pVM, -1, &szHostname);
	CVAR_SET_STRING("hostname", String(szHostname));
	script_pushbool(pVM, true);
	return 1;
}

// getHostname()
int CServerNatives::GetHostName(lua_State * pVM)
{
	script_pushstring(pVM, CVAR_GET_STRING("hostname"));
	return 1;
}

// togglePayAndSpray(toggle)
int CServerNatives::TogglePayAndSpray(lua_State * pVM)
{
	bool bSwitch;
	script_getbool(pVM, -1, &bSwitch);

	bool bToggle = (bSwitch != 0);
	CVAR_SET_BOOL("paynspray", bToggle);

	CBitStream bsSend;
	bsSend.Write(bToggle);
	g_pNetworkManager->RPC(RPC_ScriptingTogglePayAndSpray, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE, INVALID_ENTITY_ID, true);
	return 1;
}

// toggleAutoAim(toggle)
int CServerNatives::ToggleAutoAim(lua_State * pVM)
{
	bool bSwitch;
	script_getbool(pVM, -1, &bSwitch);

	bool bToggle = (bSwitch != 0);
	CVAR_SET_BOOL("autoaim", bToggle);

	CBitStream bsSend;
	bsSend.Write(bToggle);
	g_pNetworkManager->RPC(RPC_ScriptingToggleAutoAim, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE, INVALID_ENTITY_ID, true);
	return 1;
}

// sendConsoleInput(input)
int CServerNatives::SendInput(lua_State * pVM)
{
	const char * input;
	script_getstring(pVM, -1, &input);

	if(g_pServer)
		g_pServer->SendInput(input);

	return 1;
}

// getConfig()
int CServerNatives::GetConfig(lua_State * pVM)
{
	// Create a new table
	script_newtable(pVM, 0);

	for(std::map<String, SettingsValue *>::iterator iter = CSettings::GetValues()->begin(); iter != CSettings::GetValues()->end(); iter++)
	{
		// Get the setting
		SettingsValue * setting = iter->second;

		// Push the setting name onto the stack
		script_pushlstring(pVM, iter->first.Get(), iter->first.GetLength());

		// Push the value onto the stack
		if(setting->IsBool())
			script_pushbool(pVM, setting->bValue);
		else if(setting->IsInteger())
			script_pushinteger(pVM, setting->iValue);
		else if(setting->IsFloat())
			script_pushfloat(pVM, setting->fValue);
		else if(setting->IsString())
			script_pushlstring(pVM, setting->strValue, setting->strValue.GetLength());
		else if(setting->IsList())
		{
			// Create a new array
			CScriptArray a(pVM);

			for(std::list<String>::iterator iter2 = setting->listValue.begin(); iter2 != setting->listValue.end(); iter2++)
			{
				// Push the list value onto the stack
				a.push(iter2->Get(), iter2->GetLength());

				// Create a new array slot
				//script_arrayappend(pVM, -2);
			}
		}

		// Create a new table slot
		script_tableset(pVM, -3);
	}

	return 1;
}

// getWeaponName(weaponid)
int CServerNatives::GetWeaponName(lua_State * pVM)
{
	int iWeaponId;
	script_getinteger(pVM, -1, &iWeaponId);

	switch(iWeaponId)
	{
	case 0:
		script_pushstring(pVM, "Fists");
		break;
	case 1:
		script_pushstring(pVM, "Baseball Bat");
		break;
	case 2:
		script_pushstring(pVM, "Pool Cue");
		break;
	case 3:
		script_pushstring(pVM, "Knife");
		break;
	case 4:
		script_pushstring(pVM, "Grenade");
		break;
	case 5:
		script_pushstring(pVM, "Molotov Cocktail");
		break;
	case 7:
		script_pushstring(pVM, "Pistol");
		break;
	case 9:
		script_pushstring(pVM, "Desert Eagle");
		break;
	case 10:
		script_pushstring(pVM, "Shotgun");
		break;
	case 11:
		script_pushstring(pVM, "Baretta");
		break;
	case 12:
		script_pushstring(pVM, "Micro UZI");
		break;
	case 13:
		script_pushstring(pVM, "MP5");
		break;
	case 14:
		script_pushstring(pVM, "AK-47");
		break;
	case 15:
		script_pushstring(pVM, "M4");
		break;
	case 16:
		script_pushstring(pVM, "Sniper Rifle");
		break;
	case 17:
		script_pushstring(pVM, "M40-A1");
		break;
	case 18:
		script_pushstring(pVM, "Rocket Launcher");
		break;
	case 19:
		script_pushstring(pVM, "Flame Thrower");
		break;
	case 20:
		script_pushstring(pVM, "Minigun");
		break;
	default:
		script_pushbool(pVM, false);
		break;
	}
	
	return 1;
}


char szVehicleNames[124][32] = 
{
	"Admiral", "Airtug", "Ambulance", "Banshee", "Benson", "Biff", "Blista Compact", "Bobcat", "Boxville",
	"Buccaneer", "Burrito", "Burrito", "Bus", "Cabby", "Cavalcade", "Chavos", "Cognoscenti", "Comet",
	"Coquette", "DF8", "Dilettante", "Dukes", "E109", "Emperor", "Emperor", "Esperanto", "Faction", "FBI",
	"Feltzer", "Feroci", "Feroci", "Firetruck", "Flatbed", "Fortune", "Forklift", "Futo", "FXT", "Habanero",
	"Hakumai", "Huntley", "Infernus", "Ingot", "Intruder", "Landstalker", "Lokus", "Manana", "Marbella",
	"Merit", "Minivan", "Moonbeam", "Mr Tasty", "Mule", "Noose", "Noose Stockade", "Oracle", "Packer", "Patriot",
	"Perennial", "Perennial", "Peyote", "Phantom", "Pinnacle", "PMP600", "Police", "Police", "Police Patriot",
	"Pony", "Premier", "Pres", "Primo", "Police Stockade", "Rancher", "Rebla", "Ripley", "Romero", "Rom",
	"Ruiner", "Sabre", "Sabre", "Sabre GT", "Schafter", "Sentinel", "Solair", "Speedo", "Stalion", "Steed",
	"Stockade", "Stratum", "Stretch", "Sultan", "Sultan RS", "Super GT", "Taxi", "Taxi", "Trash", "Turismo",
	"Uranus", "Vigero", "Vigero", "Vincent", "Virgo", "Voodoo", "Washington", "Willard", "Yankee", "Freeway",
	"Faggio", "Hellfury", "NRG 900", "PCJ600", "Sanchez", "Zombie", "Annihilator", "Maverick", "Police Maverick",
	"Tour Maverick", "Dinghy", "Jetmax", "Marquis", "Predator", "Reefer", "Squalo", "Tug", "Tropic",
};

// getVehicleName(modelid)
int CServerNatives::GetVehicleName(lua_State * pVM)
{
	int iModelId;
	script_getinteger(pVM, -1, &iModelId);

	if(iModelId < 0 || iModelId > 123)
	{
		script_pushbool(pVM, false);
		return 1;
	}

	script_pushstring(pVM, szVehicleNames[iModelId]);
	return 1;
}

// toggleFrequentEvents(toggle)
int CServerNatives::ToggleFrequentEvents(lua_State * pVM)
{
	int bToggle;
	script_getinteger(pVM, -1, &bToggle);
	CVAR_SET_BOOL("frequentevents", bToggle != 0);
	script_pushbool(pVM, true);
	return 1;
}

// areFrequentEventsEnabled()
int CServerNatives::AreFrequentEventsEnabled(lua_State * pVM)
{
	script_pushbool(pVM, CVAR_GET_BOOL("frequentevents"));
	return 1;
}

// setWeather(weather)
int CServerNatives::SetWeather(lua_State * pVM)
{
	int iWeather;
	script_getinteger(pVM, 1, &iWeather);
	if(iWeather >= 1 && iWeather <= 10 && iWeather != CVAR_GET_INTEGER("weather"))
	{
		CVAR_SET_INTEGER("weather", (int)iWeather);

		CBitStream bsSend;
		bsSend.Write((unsigned char)iWeather);
		g_pNetworkManager->RPC(RPC_ScriptingSetPlayerWeather, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);

		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// getWeather(weather)
int CServerNatives::GetWeather(lua_State * pVM)
{
	script_pushinteger(pVM, CVAR_GET_INTEGER("weather"));
	return 1;
}

// setWeather(weather)
int CServerNatives::ForceWind(lua_State * pVM)
{
	float fWind;
	script_getfloat(pVM, 1, &fWind);
	if(fWind != CVAR_GET_INTEGER("wind"))
	{
		CVAR_SET_FLOAT("wind", (float)fWind);

		CBitStream bsSend;
		bsSend.Write((float)fWind);
		g_pNetworkManager->RPC(RPC_ScriptingForceWind, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);

		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}
