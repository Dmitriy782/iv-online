//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: ClientNatives.cpp
// Project: Client.Core
// Author(s): jenksta
//            Einstein
//            Sebihunter
// License: See LICENSE in root directory
//
//==============================================================================

#include "../Natives.h"
#include "../CClient.h"
#include "../Scripting.h"
#include "../CIVWeather.h"

extern CClient * g_pClient;

// Client functions

void RegisterClientNatives(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterFunction("addChatMessage", script_addChatMessage, -1, NULL);
	pScriptingManager->RegisterFunction("toggleChatWindow", script_toggleChatWindow, 1, "b");
	pScriptingManager->RegisterFunction("isChatWindowVisible", script_isChatWindowVisible, 0, NULL);
	pScriptingManager->RegisterFunction("getWeather", script_getWeather, 0, NULL);
	pScriptingManager->RegisterFunction("setWeather", script_setWeather, 1, "i");
	pScriptingManager->RegisterFunction("getGameScrollBarText", script_getGameScrollBarText, 1, "i");
	pScriptingManager->RegisterFunction("setGameScrollBarText", script_setGameScrollBarText, 2, "is");
	pScriptingManager->RegisterFunction("resetGameScrollBarText", script_resetGameScrollBarText, 1, "i");
	pScriptingManager->RegisterFunction("getGameScrollBarColor", script_getGameScrollBarColor, 0, NULL);
	pScriptingManager->RegisterFunction("setGameScrollBarColor", script_setGameScrollBarColor, 1, "i");
	pScriptingManager->RegisterFunction("getMoney", script_getMoney, 0, NULL);
	pScriptingManager->RegisterFunction("getCurrentAreaName", script_getCurrentAreaName, 0, NULL);
	pScriptingManager->RegisterFunction("getCurrentStreetName", script_getCurrentStreetName, 0, NULL);
	pScriptingManager->RegisterFunction("getVehicleName", script_getVehicleName, 1, "i");
	pScriptingManager->RegisterFunction("getFPS", script_getFPS, 0, NULL);
	pScriptingManager->RegisterFunction("isGameFocused", script_isGameFocused, 0, NULL);
	pScriptingManager->RegisterFunction("setRadarZoom", script_setRadarZoom, 1, "f");
	pScriptingManager->RegisterFunction("drawLightWithRange", script_drawLightWithRange, 8, "fffiiiff");
	pScriptingManager->RegisterFunction("getScreenPositionFromWorldPosition", script_getScreenPositionFromWorldPosition, 3, "fff");
	pScriptingManager->RegisterFunction("getActorPosition", script_getActorCoordinates, 1, "i");
	pScriptingManager->RegisterFunction("tuneRadio", script_tuneRadio, 2, "bi");
	pScriptingManager->RegisterFunction("triggerServerEvent", script_triggerServerEvent, -1, NULL);
	pScriptingManager->RegisterFunction("getGameLanguage", script_getGameLanguage, -1, NULL);
	pScriptingManager->RegisterFunction("importAndLoadGameFile", script_importAndLoadGameFile, 1, "s");
}

// addChatMessage(string)
int script_addChatMessage(lua_State * pVM)
{
	CHECK_PARAMS_MIN_MAX("addChatMessage", 1, 3);
	CHECK_TYPE("timer", 1, 1, SCRIPT_TYPE_STRING);

	const char* text = NULL;
	script_getstring(pVM, 1, &text);

	int color = 0xFFFFFFAA;
	bool bAllowFormatting = false;
	int iTop = script_gettop(pVM);

	if(iTop >= 2)
	{
		if(script_gettype(pVM, 2) != SCRIPT_TYPE_INTEGER)
		{
			script_pushbool(pVM, false);
			return 1;
		}

		script_getinteger(pVM, 2, &color);

		if(iTop >= 3)
		{
			if(script_gettype(pVM, 3) != SCRIPT_TYPE_BOOL)
			{
				script_pushbool(pVM, false);
				return 1;
			}

			script_getbool(pVM, 3, &bAllowFormatting);
		}
	}

	g_pClient->GetChatWindow()->AddMessage(color, (bAllowFormatting != 0), text);
	script_pushbool(pVM, true);
	return 1;
}

// toggleChatWindow(toggle)
int script_toggleChatWindow(lua_State * pVM)
{
	bool bToggle;
	script_getbool(pVM, 1, &bToggle);
	g_pClient->GetChatWindow()->SetEnabled(bToggle != 0);
	script_pushbool(pVM, true);
	return 1;
}

// isChatWindowVisible()
int script_isChatWindowVisible(lua_State * pVM)
{
	script_pushbool(pVM, g_pClient->GetChatWindow()->IsEnabled());
	return 1;
}

// GUI functions

void RegisterGUINatives(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterFunction("guiShowMessageBox", script_guiShowMessageBox, 2, "ss");
	pScriptingManager->RegisterFunction("guiToggleCursor", script_guiToggleCursor, 1, "b");
	pScriptingManager->RegisterFunction("isCursorVisible", script_guiIsCursorVisible, 0, NULL);
	pScriptingManager->RegisterFunction("guiSetCursorPosition", script_guiSetCursorPosition, 2, "ff");
	pScriptingManager->RegisterFunction("guiGetCursorPosition", script_guiGetCursorPosition, 0, NULL);
	pScriptingManager->RegisterFunction("guiGetScreenSize", script_guiGetScreenSize, 0, NULL);
	pScriptingManager->RegisterFunction("guiDrawRectangle", script_guiDrawRectangle,  6, "ffffib");
}

// guiShowMessageBox(string text, string title)
int script_guiShowMessageBox(lua_State * pVM)
{
	const char* text;
	const char* title;
	script_getstring(pVM, -2, &text);
	script_getstring(pVM, -1, &title);
	script_pushbool(pVM, true);

	g_pClient->GetGUI()->ShowMessageBox(text, title);
	CGame::SetInputState(false);
	return 1;
}	

// guiToggleCursor(bool toggle)
int script_guiToggleCursor(lua_State * pVM)
{	
	bool sbBool;
	script_getbool(pVM, -1, &sbBool);
	bool bBool = (sbBool != 0);

	// Get our GUI instance
	CGUI * pGUI = g_pClient->GetGUI();

	pGUI->SetScriptedCursorVisible(bBool);
	pGUI->SetCursorVisible(bBool);

	script_pushbool(pVM, true);
	return 1;
}

// isCursorVisible()
int script_guiIsCursorVisible(lua_State * pVM)
{
	script_pushbool(pVM, g_pClient->GetGUI()->IsCursorVisible(true));
	return 1;
}

// setCursorPosition(float posX, float posY)
int script_guiSetCursorPosition(lua_State * pVM)
{
	float posX;
	float posY;
	script_getfloat(pVM, -2, &posX);
	script_getfloat(pVM, -1, &posY);

	g_pClient->GetGUI()->SetCursorPosition(posX, posY);
	return 1;
}

// guiGetCursorPosition()
int script_guiGetCursorPosition(lua_State * pVM)
{	
	// Returning mouse cursor position to script vm:
	// Cursor position is array [X, Y]
	RECT curPos = g_pClient->GetGUI()->GetCursorPosition();
	CScriptArray args(pVM);
	args.push(curPos.left);
	args.push(curPos.top);
	//CScriptArgument(args, true).push(pVM);
	return 1;
}

// guiGetScreenSize()
int script_guiGetScreenSize(lua_State * pVM)
{
	// Get our GUI instance
	CGUI * pGUI = g_pClient->GetGUI();

	CScriptArray a(pVM);
	a.push((float)pGUI->GetDisplayWidth());
	//script_arrayappend(pVM, -2);
	a.push((float)pGUI->GetDisplayHeight());
	//script_arrayappend(pVM, -2);
	return 1;
}

// guiDrawRectangle(x1, y1, x2, y2, color, relative)
int script_guiDrawRectangle(lua_State * pVM)
{
	float x,y,x1,y1;
	int color;
	bool sbRelative;
	script_getfloat(pVM, -6, &x);
	script_getfloat(pVM, -5, &y);
	script_getfloat(pVM, -4, &x1);
	script_getfloat(pVM, -3, &y1);
	script_getinteger(pVM, -2, &color);
	script_getbool(pVM, -1, &sbRelative);

	bool bRelative = (sbRelative != 0);

	if(bRelative)
	{
		// Get our GUI instance
		CGUI * pGUI = g_pClient->GetGUI();

		x *= pGUI->GetDisplayHeight();
		y *= pGUI->GetDisplayHeight();
		x1 *= pGUI->GetDisplayWidth();
		y1 *= pGUI->GetDisplayHeight();
	}

	g_pClient->GetGraphics()->DrawRect(x, y, x1, y1, ( color >> 8 ) + ( ( color & 0xFF ) << 24 ));
	script_pushbool(pVM, true);
	return 1;
}	

int script_triggerServerEvent(lua_State * pVM)
{
	CScriptArguments arguments;
	for(int i = 1; i <= script_gettop(pVM); ++ i )
	{
		if(!arguments.pushFromStack(pVM, i))
			return 1;
	}

	CBitStream bsSend;
	arguments.serialize(&bsSend);
	g_pClient->GetNetworkManager()->RPC(RPC_ScriptingEventCall, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, PACKET_CHANNEL_SCRIPT);
	return 1;
}

// Scripting functions

void RegisterScriptNatives(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterFunction("getClientScripts", script_getScripts, 0, NULL);
	pScriptingManager->RegisterFunction("getScriptName", script_getScriptName, 0, NULL);
}

// getClientScripts()
int script_getScripts(lua_State * pVM)
{
	CScriptArray a(pVM);
	
	std::list<CScript*> * scripts = g_pClient->GetScriptingManager()->GetScriptList();
	for(auto iter = scripts->begin(); iter != scripts->end(); ++ iter)
	{
		auto name = (*iter)->GetMetaFile().ToMetaAndFileString();
		a.push(name.C_String(), name.GetLength());
		//script_arrayappend(pVM, -2);
	}
	return scripts->size();
}

// getScriptName()
int script_getScriptName(lua_State * pVM)
{	
	String strScriptName = g_pClient->GetScriptingManager()->Get(pVM)->GetMetaFile().ToMetaAndFileString();
	script_pushlstring(pVM, strScriptName.Get(), strScriptName.GetLength());
	return 1;
}

// getWeather()
int script_getWeather(lua_State * pVM)
{
	script_pushinteger(pVM, (CGame::GetWeather()->GetWeather() + 1));
	return 1;
}

// setWeather()
int script_setWeather(lua_State * pVM)
{
	int iWeather;
	script_getinteger(pVM, 1, &iWeather);
	if(iWeather >= 1 && iWeather <= 10)
	{
		CGame::GetWeather()->SetWeather((eWeather)(iWeather - 1));
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// getGameScrollBarText(id)
int script_getGameScrollBarText(lua_State * pVM)
{
	int iID;
	script_getinteger(pVM, 1, &iID);

	char szText[1300] = {0};
	CGame::GetScrollBarText(iID, szText);

	script_pushlstring(pVM, szText, strlen(szText));
	return 1;
}

// setGameScrollBarText(id, text)
int script_setGameScrollBarText(lua_State * pVM)
{
	int iID;
	const char* szText;
	script_getinteger(pVM, 1, &iID);
	script_getstring(pVM, 2, &szText);

	if(iID >= 0 && iID != 2 && iID <= 7)
		script_pushbool(pVM, CGame::SetScrollBarText(iID, szText));
	else
		script_pushbool(pVM, false);
	return 1;
}

// resetGameScrollBarText(id)
int script_resetGameScrollBarText(lua_State * pVM)
{
	int iID;
	script_getinteger(pVM, 1, &iID);

	if(iID >= -1 && iID <= 7)
	{
		CGame::ResetScrollBars(iID);
		script_pushbool(pVM, true);
	}
	else
		script_pushbool(pVM, false);
	return 1;
}

// setGameScrollBarColor(color)
int script_setGameScrollBarColor(lua_State * pVM)
{
	int iColor;
	script_getinteger(pVM, 1, &iColor);

	CGame::SetScrollBarColor(iColor);

	script_pushbool(pVM, true);
	return 1;
}

// getGameScrollBarColor()
int script_getGameScrollBarColor(lua_State * pVM)
{
	script_pushinteger(pVM, CGame::GetScrollBarColor());
	return 1;
}

// getMoney()
int script_getMoney(lua_State * pVM)
{
	script_pushinteger(pVM, g_pClient->GetLocalPlayer()->GetMoney());
	return 1;
}

// getCurrentAreaName()
int script_getCurrentAreaName(lua_State * pVM)
{
	script_pushstring(pVM, CGame::GetCurrentAreaName().Get());
	return 1;
}

// getCurrentStreetName()
int script_getCurrentStreetName(lua_State * pVM)
{
	script_pushstring(pVM, CGame::GetCurrentStreetName().Get());
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
int script_getVehicleName(lua_State * pVM)
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

// getFPS()
int script_getFPS(lua_State * pVM)
{
	script_pushinteger(pVM, g_pClient->GetFPSCounter()->Get());
	return 1;
}

// getScreenPositionFromWorldPosition(x, y, z)
int script_getScreenPositionFromWorldPosition(lua_State * pVM)
{
	CVector3 vecWorldPos;
	Vector2 vecScreenPos;
	script_getvector3(pVM, -1, &vecWorldPos);
	CGame::GetScreenPositionFromWorldPosition(vecWorldPos, vecScreenPos);
	
	CScriptArray a(pVM);
	a.push(vecScreenPos.fX);
	//script_arrayappend(pVM, -2);
	a.push(vecScreenPos.fY);
	//script_arrayappend(pVM, -2);
	return 1;
}

// isGameFocused()
int script_isGameFocused(lua_State * pVM)
{
	script_pushbool(pVM, CGame::IsFocused());
	return 1;
}

// setRadarZoom(fZoom)
int script_setRadarZoom(lua_State * pVM)
{
	float fZoom;
	script_getfloat(pVM,-1,&fZoom);

	Scripting::SetRadarZoom(fZoom);
	script_pushbool(pVM,true);
	return 1;
}

int script_drawLightWithRange(lua_State * pVM)
{
	CVector3 vecPos;
	int R,G,B;
	float fDensity, fRange;
	script_getfloat(pVM, -8, &vecPos.fX);
	script_getfloat(pVM, -7, &vecPos.fY);
	script_getfloat(pVM, -6, &vecPos.fZ);
	script_getinteger(pVM, -5, &R);
	script_getinteger(pVM, -4, &G);
	script_getinteger(pVM, -3, &B);
	script_getfloat(pVM, -2, &fDensity);
	script_getfloat(pVM, -1, &fRange);

	Scripting::DrawLightWithRange(vecPos.fX, vecPos.fY, vecPos.fZ, R, G, B, fDensity, fRange);
	script_pushbool(pVM,true);
	return 1;
}

// getActorCoordinates(actorid) // pls take a look at the wiki ;)
int script_getActorCoordinates(lua_State * pVM)
{
	EntityId actorId;
	script_getentity(pVM,-1,&actorId);

	// Get our actor manager
	CActorManager * pActorManager = g_pClient->GetActorManager();

	if(pActorManager->DoesExist(actorId))
	{
		CVector3 vecPosition;
		Scripting::GetCharCoordinates(pActorManager->GetScriptingHandle(actorId), &vecPosition.fX, &vecPosition.fY, &vecPosition.fZ);
		CScriptArray args(pVM);
		args.push(vecPosition.fX);
		args.push(vecPosition.fY);
		args.push(vecPosition.fZ);
		//CScriptArgument(args, true).push(pVM);
		return 1;
	}

	//script_pushbool(pVM,false);
	return 0;
}

int script_tuneRadio(lua_State * pVM)
{
	int iStationIndex;
	script_getinteger(pVM, -1, &iStationIndex);

	bool bToggle;
	script_getbool(pVM, -2, &bToggle);

	bool bSwitch = (bToggle != 0);

	if(bSwitch)
	{
		Scripting::SetMobileRadioEnabledDuringGameplay(1);
		Scripting::SetMobilePhoneRadioState(1);
		Scripting::RetuneRadioToStationIndex((Scripting::eRadioStation)iStationIndex);
	}
	else
	{
		Scripting::SetMobileRadioEnabledDuringGameplay(0);
		Scripting::SetMobilePhoneRadioState(0);
	}
	script_pushbool(pVM,true);
	return 1;
}

int script_getGameLanguage(lua_State * pVM)
{
	int iLanguage = Scripting::GetCurrentLanguage();
	String strLanguage;

	switch(iLanguage)
	{
	case 0:
		strLanguage = "AMERICAN";
	case 1:
		strLanguage = "FRENCH";
	case 2:
		strLanguage = "GERMAN";
	case 3:
		strLanguage = "ITALIAN";
	case 4:
		strLanguage = "SPANISH";
	case 5:
		strLanguage = "RUSSIAN";
	default:
		strLanguage = "UNKOWN";
	}

	script_pushstring(pVM,strLanguage.Get());
	return 1;
}

int script_importAndLoadGameFile(lua_State * pVM)
{
	const char * szFile;
	script_getstring(pVM, -1, &szFile);
	String strFile = szFile;

	String strAbsolutePath;
	strAbsolutePath = SharedUtility::GetAbsolutePath("clientfiles");
	if(!strcmp(szFile,"hud.dat") || !strcmp(szFile,"RadioLogo.dat")) {
		std::ifstream szFileExist(String("%s\\resources\\%s", strAbsolutePath.Get(), strFile.Get()).Get());
		if(!szFileExist) {
			CLogFile::Printf("[IMPORT] Failed to import file %s into GTA [File does not exist]",strFile.Get());
			script_pushbool(pVM,false);
			return 1;
		}

		char szInstallDirectory[MAX_PATH];
		if(!SharedUtility::ReadRegistryString(HKEY_LOCAL_MACHINE, "Software\\Rockstar Games\\Grand Theft Auto IV", 
			"InstallFolder", NULL, szInstallDirectory, sizeof(szInstallDirectory)) || !SharedUtility::Exists(szInstallDirectory))
		{
			if(!SharedUtility::ReadRegistryString(HKEY_CURRENT_USER, "Software\\IVMP", "gtaivdir", NULL, 
				szInstallDirectory, sizeof(szInstallDirectory)) || 
				!SharedUtility::Exists(szInstallDirectory))
			{
				CLogFile::Printf("[IMPORT] Failed to import file %s into GTA [Failed to find gta iv directory]",strFile.Get());
				script_pushbool(pVM,false);
				return 1;
			}
		}
		
		String strCopyPath = String("%s\\common\\data",szInstallDirectory);
		if(!SharedUtility::Exists(strCopyPath.Get()))
			SharedUtility::CreateDirectoryA(strCopyPath.Get());

		String strFolderName = SharedUtility::GetAbsolutePath("clientfiles");
		strFolderName.AppendF("\\resources\\%s",strFile.Get());
		
		if(!strcmp(szFile,"hud.dat"))
			strCopyPath.Append("\\hud_ivmp.dat");
		else if(!strcmp(szFile,"RadioLogo.dat"))
			strCopyPath.Append("\\RadioLogo_ivmp.dat");

		if(g_pClient->GetChatWindow())
			g_pClient->GetChatWindow()->AddInfoMessage("IMPORT from: %s | to: %s",strFolderName.Get(), strCopyPath.Get());

		if(CopyFileA(strFolderName.Get(),strCopyPath.Get(),false))
		{
			if(!strcmp(szFile,"hud.dat"))
			{
				// Load custom HUD.dat
				CGame::LoadHUD("common:/DATA/HUD_IVMP.DAT");
				script_pushbool(pVM, true);
				return 1;
			}
			else if(!strcmp(szFile,"RadioLogo.dat"))
			{
				// Load custom RadioLogo.dat
				CGame::LoadRadioLogo("common:/DATA/RadioLogo_ivmp.DAT");
				script_pushbool(pVM, true);
				return 1;
			}
			else
			{
				CLogFile::Printf("[IMPORT] Failed to import file %s into GTA [Not supported file]",strFile.Get());
				script_pushbool(pVM, false);
				return 1;
			}
		}
		else
		{
			CLogFile::Printf("[IMPORT] Failed to import file %s into GTA [Copy failed]",strFile.Get());
			script_pushbool(pVM, false);
			return 1;
		}
	}
	else
	{	
		CLogFile::Printf("[IMPORT] Failed to import file %s into GTA [Not supported file]",strFile.Get());
		script_pushbool(pVM, false);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}