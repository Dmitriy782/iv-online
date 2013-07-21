//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CServer.cpp
// Project: Server.Core
// Author(s): jenksta
//            Einstein
//            Sebihunter
//            MaVe
// License: See LICENSE in root directory
//
//==============================================================================

#include "CServer.h"
#include <stdarg.h>
#include <queue>
#include "CNetworkManager.h"
#include "CPlayerManager.h"
#include "CVehicleManager.h"
#include "CObjectManager.h"
#include "CBlipManager.h"
#include "CActorManager.h"
#include "CCheckpointManager.h"
#include "CPickupManager.h"
#include "Scripting/CScriptingManager.h"
#include "CClientFileManager.h"
#include "Natives.h"
#include "CModuleManager.h"
#include "Scripting/CGlobalsManager.h"
#include "Scripting/CScriptTimerManager.h"
#include "CMasterList.h"
#include "tinyxml/tinyxml.h"
#include "tinyxml/ticpp.h"
#include "SharedUtility.h"
#include "CWebserver.h"
#include <CSettings.h>
#include <Game/CTime.h>
#include "CEvents.h"
#include <Game/CTrafficLights.h>
#include <Network/CNetworkModule.h>
#include <Threading/CMutex.h>
#include <Threading/CThread.h>
#include "CQuery.h"
#include <CExceptionHandler.h>
#include "ModuleNatives/ModuleNatives.h"
#include "C3DLabels.h"

#define SERVER_TITLE MOD_NAME " v" MOD_VERSION_STRING " Server [" OS_STRING "]" DEBUG_IDENTIFIER

#define HEIPHEN_GEN(string, stringname) \
	{ \
		stringname[0] = '\0'; \
		\
		for(int i = 0; i < sizeof(string) + 1; i++) \
			strcat(stringname, "-"); \
	}

CNetworkManager                   * g_pNetworkManager = NULL;
CPlayerManager                    * g_pPlayerManager = NULL;
CVehicleManager                   * g_pVehicleManager = NULL;
CObjectManager                    * g_pObjectManager = NULL;
CBlipManager                      * g_pBlipManager = NULL;
CActorManager                     * g_pActorManager = NULL;
CPickupManager                    * g_pPickupManager = NULL;
CCheckpointManager                * g_pCheckpointManager = NULL;
CScriptingManager                 * g_pScriptingManager = NULL;
CScriptTimerManager               * g_pScriptTimerManager = NULL;
CClientFileManager                * g_pClientResourceFileManager = NULL;
CModuleManager                    * g_pModuleManager = NULL;
CMasterList                       * g_pMasterList = NULL;
CWebServer                        * g_pWebserver = NULL;
CTime                             * g_pTime = NULL;
CTrafficLights                    * g_pTrafficLights = NULL;
CEvents                           * g_pEvents = NULL;
unsigned long                       g_ulStartTick = 0;
CQuery                            * g_pQuery = NULL;
CGlobalsManager					  * g_pGlobalsManager = NULL;

C3DLabelManager		              * g_p3DLabelManager = NULL;

Modules::CActorModuleNatives      * g_pActorModuleNatives;
Modules::CBlipModuleNatives       * g_pBlipModuleNatives;
Modules::CCheckpointModuleNatives * g_pCheckpointModuleNatives;
Modules::CObjectModuleNatives     * g_pObjectModuleNatives;
Modules::CPickupModuleNatives     * g_pPickupModuleNatives;
Modules::CPlayerModuleNatives     * g_pPlayerModuleNatives;
Modules::CServerModuleNatives     * g_pServerModuleNatives;
Modules::CVehicleModuleNatives    * g_pVehicleModuleNatives;
Modules::CScriptModuleNatives     * g_pScriptModuleNatives;
Modules::CAreaModuleNatives       * g_pAreaModuleNatives;
Modules::CHashModuleNatives       * g_pHashModuleNatives;
Modules::CWorldModuleNatives      * g_pWorldModuleNatives;

CServer::CServer()
{
	m_bActive = true;
	m_bShowFPS = true;
	m_ulLastFPSUpdateTime = 0;
	m_ulFrameCount = 0;
	m_ulFramesPerSecond = 0;
	m_iResourcesLoaded = 0;
	m_iFailedResources = 0;
}

CServer::~CServer()
{

}

bool CServer::OnLoad(int argc, char ** argv)
{
	SetTitle(SERVER_TITLE);

	// Install the exception handler
	CExceptionHandler::Install();

	// Open the log file
	CLogFile::Open("ivmp-svr.log", true);

	// Open the settings file
	if(!CSettings::Open(SharedUtility::GetAbsolutePath("settings.xml"), false, false))
	{
		CLogFile::Print("Failed to open settings.xml, server will shut down in 3 seconds..");
		Sleep(3000);
		return false;
	}

	// Parse the command line
	CSettings::ParseCommandLine(argc, argv);

	char heiphens[128];

	HEIPHEN_GEN(" " VERSION_IDENTIFIER " " OS_STRING " Server", heiphens);

#ifdef WIN32
	// Color stuff
	CONSOLE_SCREEN_BUFFER_INFO csbiScreen;
	WORD wOldColAttr;	// For input process.

	GetConsoleScreenBufferInfo((HANDLE)GetStdHandle(STD_OUTPUT_HANDLE), &csbiScreen);
	wOldColAttr = csbiScreen.wAttributes;

	SetConsoleTextAttribute((HANDLE)GetStdHandle(STD_OUTPUT_HANDLE), wOldColAttr | FOREGROUND_INTENSITY);

	// Print message to console.
	SetConsoleTextAttribute((HANDLE)GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
#endif

#ifdef WIN32
	CLogFile::Print("");
	CLogFile::Print("====================================================================");
	SetConsoleTextAttribute((HANDLE)GetStdHandle(STD_OUTPUT_HANDLE), wOldColAttr | FOREGROUND_INTENSITY);
#else
	CLogFile::Print("");
	CLogFile::Print("====================================================================");
#endif
	CLogFile::Print(" " VERSION_IDENTIFIER " " OS_STRING " Server");
	CLogFile::Print(" Copyright (C) 2009-2013 IV:MP Team");
	CLogFile::Printf(" Port: %d", CVAR_GET_INTEGER("port"));
	CLogFile::Printf(" HTTP Port: %d", CVAR_GET_INTEGER("httpport"));
	CLogFile::Printf(" Query Port: %d", (CVAR_GET_INTEGER("port") + QUERY_PORT_OFFSET));

	if(CVAR_GET_STRING("hostaddress").IsNotEmpty())
		CLogFile::Printf(" Host Address: %s", CVAR_GET_STRING("hostaddress").Get());

	if(CVAR_GET_STRING("httpserver").IsNotEmpty())
		CLogFile::Printf(" HTTP Server: %s", CVAR_GET_STRING("httpserver").Get());

	CLogFile::Printf(" Max Players: %d", CVAR_GET_INTEGER("maxplayers"));

#ifdef WIN32
	SetConsoleTextAttribute((HANDLE)GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	CLogFile::Print("====================================================================");
	SetConsoleTextAttribute((HANDLE)GetStdHandle(STD_OUTPUT_HANDLE), wOldColAttr | FOREGROUND_INTENSITY);
#else
	CLogFile::Print("====================================================================");
#endif
	//----------------------------------------------------------

	g_pEvents = new CEvents();
	g_pScriptingManager = new CScriptingManager();
	//g_pClientScriptFileManager = new CClientFileManager();
	g_pClientResourceFileManager = new CClientFileManager();

	// Initialize the network module, if it fails, exit
	if(!CNetworkModule::Init())
	{
		CLogFile::Print("Failed to initialize the network module!\n Server shuts down in 3 seconds");
		Sleep(3000);
		return false;
	}

	g_pNetworkManager = new CNetworkManager();

	// Startup the network manager, if it fails exit
	if(!g_pNetworkManager->Startup(CVAR_GET_INTEGER("port"), CVAR_GET_INTEGER("maxplayers"), CVAR_GET_STRING("password"), CVAR_GET_STRING("hostaddress")))
	{
		CLogFile::Print("Failed to startup the network manager!\n Server shuts down in 3 seconds");
		Sleep(3000);
		return false;
	}

	g_pPlayerManager = new CPlayerManager();
	g_pVehicleManager = new CVehicleManager();
	g_pObjectManager = new CObjectManager();
	g_pPickupManager = new CPickupManager();
	g_pBlipManager = new CBlipManager();
	g_pActorManager = new CActorManager();
	g_pCheckpointManager = new CCheckpointManager();
	g_pModuleManager = new CModuleManager();
	g_pScriptTimerManager = new CScriptTimerManager();
	g_pGlobalsManager = new CGlobalsManager();
	g_p3DLabelManager = new C3DLabelManager();

	g_pWebserver = new CWebServer(CVAR_GET_INTEGER("httpport"));
	g_pTime = new CTime();
	g_pTrafficLights = new CTrafficLights();

	g_pPickupModuleNatives = new Modules::CPickupModuleNatives;
	g_pActorModuleNatives = new Modules::CActorModuleNatives;
	g_pBlipModuleNatives = new Modules::CBlipModuleNatives;
	g_pCheckpointModuleNatives = new Modules::CCheckpointModuleNatives;
	g_pObjectModuleNatives = new Modules::CObjectModuleNatives;
	g_pPlayerModuleNatives = new Modules::CPlayerModuleNatives;
	g_pServerModuleNatives = new Modules::CServerModuleNatives;
	g_pVehicleModuleNatives = new Modules::CVehicleModuleNatives;
	g_pScriptModuleNatives = new Modules::CScriptModuleNatives;
	g_pAreaModuleNatives = new Modules::CAreaModuleNatives;
	g_pHashModuleNatives = new Modules::CHashModuleNatives;
	g_pWorldModuleNatives = new Modules::CWorldModuleNatives;

	std::list<String> modules = CVAR_GET_LIST("module");
	if(modules.size() > 0)
	{
#ifdef WIN32
		SetConsoleTextAttribute((HANDLE)GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
		CLogFile::Print("");
		CLogFile::Print("============ Loading Modules ===========");
		SetConsoleTextAttribute((HANDLE)GetStdHandle(STD_OUTPUT_HANDLE), wOldColAttr | FOREGROUND_INTENSITY);
#else
		CLogFile::Print("");
		CLogFile::Print("============ Loading Modules ===========");
#endif

		for(auto iter = modules.begin(); iter != modules.end(); ++iter)
		{
			CLogFile::Printf("Loading module %s.", iter->C_String());
			String strModule = *iter;
			if(!strModule.ToLower().EndsWith(".so") && !strModule.ToLower().EndsWith(".dll"))
			{
				// If no extension specified in module name string, load SO for linux and DLL for Win
#ifdef WIN32
				strModule.Append(".dll");
#else
				strModule.Append(".so");
#endif		
			}
			CModule * pModule = g_pModuleManager->LoadModule(strModule);
			
			if(!pModule)
				CLogFile::Printf("Warning: Failed to load module %s.", iter->C_String());
		}

		CLogFile::Print("");
	}

	// Register the system natives
	CSystemNatives::Register(g_pScriptingManager);

	// Register the server natives
	CServerNatives::Register(g_pScriptingManager);

	// Register the world natives
	CWorldNatives::Register(g_pScriptingManager);

	// Register the player natives
	CPlayerNatives::Register(g_pScriptingManager);

	// Register the vehicle natives
	CVehicleNatives::Register(g_pScriptingManager);

	// Register the object natives
	CObjectNatives::Register(g_pScriptingManager);

	// Register the blip natives
	CBlipNatives::Register(g_pScriptingManager);

	// Register the actor natives
	CActorNatives::Register(g_pScriptingManager);

	// Register the checkpoint natives
	CCheckpointNatives::Register(g_pScriptingManager);

	// Register the pickup natives
	CPickupNatives::Register(g_pScriptingManager);

	// Register the area natives
	CAreaNatives::Register(g_pScriptingManager);

	// Register the hash natives
	CHashNatives::Register(g_pScriptingManager);

	// Register the event natives
	CEventNatives::Register(g_pScriptingManager);

	// Register the 3dlabel natives
	_3DLabelNatives::Register(g_pScriptingManager);

	// Register the script natives
	RegisterScriptNatives(g_pScriptingManager);

	// Register the SQLite natives
	RegisterSQLiteNatives(g_pScriptingManager);

	// Register the XML natives
	RegisterXMLNatives(g_pScriptingManager);

	// Register the timer natives
	RegisterTimerNatives(g_pScriptingManager);

	// Register the global vairables natives
	CGlobalsManager::RegisterNatives(g_pScriptingManager);

	// Register the default constants
	g_pScriptingManager->RegisterDefaultConstants();

	g_ulStartTick = SharedUtility::GetTime();

#ifdef WIN32
	SetConsoleTextAttribute((HANDLE)GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
	CLogFile::Print("");
	CLogFile::Print("============ Loading Resources ===========");
	SetConsoleTextAttribute((HANDLE)GetStdHandle(STD_OUTPUT_HANDLE), wOldColAttr | FOREGROUND_INTENSITY);
#else
	CLogFile::Print("");
	CLogFile::Print("============ Loading Resources ===========");
#endif

	if(CVAR_GET_BOOL("query"))
		g_pQuery = new CQuery(CVAR_GET_INTEGER("port"), CVAR_GET_STRING("hostaddress"));

	auto resources = CVAR_GET_LIST("resource");
	for(auto i = resources.begin(); i != resources.end(); i++)
	{
		StartMeta(*i);
	}

#ifdef WIN32
	SetConsoleTextAttribute((HANDLE)GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
	CLogFile::Printf("Successfully loaded %d resources (%d failed).", m_iResourcesLoaded, m_iFailedResources);
	SetConsoleTextAttribute((HANDLE)GetStdHandle(STD_OUTPUT_HANDLE), wOldColAttr | FOREGROUND_INTENSITY);
#else
	CLogFile::Printf("Successfully loaded %d resources (%d failed).", m_iResourcesLoaded, m_iFailedResources);
#endif

	if(CVAR_GET_BOOL("listed"))
		g_pMasterList = new CMasterList(MASTERLIST_ADDRESS, MASTERLIST_VERSION, MASTERLIST_TIMEOUT, CVAR_GET_INTEGER("port"));

#ifdef WIN32
	SetConsoleTextAttribute((HANDLE)GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	CLogFile::Print("");
	CLogFile::Print("====================================================================");
	CLogFile::Print("");
	SetConsoleTextAttribute((HANDLE)GetStdHandle(STD_OUTPUT_HANDLE), wOldColAttr | FOREGROUND_INTENSITY);
#else
	CLogFile::Print("");
	CLogFile::Print("====================================================================");
	CLogFile::Print("");
#endif
	return true;
}

std::vector<CMetaFile> CServer::GetServerScripts(String & meta)
{
	std::vector<CMetaFile> s_scripts;
	auto path = SharedUtility::GetAbsolutePath("resources/%s/meta.xml", meta.Get());

	TiXmlDocument doc;
	if(doc.LoadFile(path.Get()))
	{
		for(TiXmlNode * pNode = doc.RootElement()->FirstChildElement(); pNode; pNode = pNode->NextSibling())
		{
			// Is this not an element node?
			if(pNode->Type() != TiXmlNode::ELEMENT)
				continue;

			// Get the setting and value
			String xname = pNode->Value();
			String xvalue = pNode->ToElement()->Attribute("src");

			if(xname == "serverscript")
			{
				s_scripts.push_back(CMetaFile(meta, xvalue, MetaFileType_ServerScript));
			}
		}
	}
	else
	{
		CLogFile::Printf("Failed to open meta file: %s", path.Get());
	}
	return s_scripts;
}

std::vector<CMetaFile> CServer::GetClientResources(String & meta)
{
	std::vector<CMetaFile> c_resources;
	auto path = SharedUtility::GetAbsolutePath("resources/%s/meta.xml", meta.Get());

	TiXmlDocument doc;
	if(doc.LoadFile(path.Get()))
	{
		for(TiXmlNode * pNode = doc.RootElement()->FirstChildElement(); pNode; pNode = pNode->NextSibling())
		{
			// Is this not an element node?
			if(pNode->Type() != TiXmlNode::ELEMENT)
				continue;

			// Get the setting and value
			String xname = pNode->Value();
			String xvalue = pNode->ToElement()->Attribute("src");

			if(xname == "clientscript")
			{
				c_resources.push_back(CMetaFile(meta, xvalue, MetaFileType_ClientScript));
			}
			else if(xname == "clientresource")
			{
				c_resources.push_back(CMetaFile(meta, xvalue, MetaFileType_ClientResource));
			}
		}
	}
	else
	{
		CLogFile::Printf("Failed to open meta file: %s", path.Get());
	}
	return c_resources;
}

bool CServer::StartServerScript(CMetaFile & mf)
{
	if(g_pScriptingManager->Get(mf) == NULL)
	{
		if(g_pScriptingManager->Load(mf))
		{
			CLogFile::Printf("[Server] Started: %s", mf.ToString().Get());
			return true;
		}
		else
			CLogFile::Printf("[Server] Failed to start: %s", mf.ToString().Get());
	}
	return false;
}

bool CServer::StartClientResource(CMetaFile & mf)
{
	if(!g_pClientResourceFileManager->Exists(mf)) // some server script probably already loaded script using ScriptNatives calls
	{
		if(g_pClientResourceFileManager->Start(mf))
		{
			CLogFile::Printf("[Server] Started: %s", mf.ToString().Get());
			return true;
		}
		else
			CLogFile::Printf("[Server] Failed to start: %s", mf.ToString().Get());
	}
	return false;
}

bool CServer::StopServerScript(CMetaFile & mf)
{
	CScript * pScript = g_pScriptingManager->Get(mf);

	if(pScript)
	{
		g_pScriptTimerManager->HandleScriptUnload(pScript);
		g_pScriptingManager->Unload(pScript);
		CLogFile::Printf("[Server] Stopped: %s", mf.ToString().Get());
		return true;
	}
	else
		CLogFile::Printf("[Server] Failed to stop: %s (Script is not loaded).", mf.ToString().Get());
	return false;
}

bool CServer::StopClientResource(CMetaFile & mf)
{
	if(!g_pClientResourceFileManager->Exists(mf))
		CLogFile::Printf("[Server] Failed to stop: %s (Resource is not loaded).", mf.ToString().Get());
	else if(!g_pClientResourceFileManager->Stop(mf))
		CLogFile::Printf("[Server] Failed to stop %s", mf.ToString().Get());
	else
	{
		CLogFile::Printf("[Server] Stopped: %s", mf.ToString().Get());
		return true;
	}
	return false;
}

bool CServer::RestartServerScript(CMetaFile & mf)
{
	CScript * pScript = g_pScriptingManager->Get(mf);

	if(pScript)
	{
		g_pScriptTimerManager->HandleScriptUnload(pScript);

		if(g_pScriptingManager->Unload(mf))
		{
			pScript = g_pScriptingManager->Load(mf);

			if(pScript)
			{
				CLogFile::Printf("[Server] Restarted: %s", mf.ToString().Get());
				return true;
			}
			else
				CLogFile::Printf("[Server] Failed to restart: %s (Failed to load script)", mf.ToString().Get());
		}
		else
			CLogFile::Printf("[Server] Failed to restart: %s (Failed to unload script)", mf.ToString().Get());
	}
	else
		CLogFile::Printf("[Server] Failed to restart: %s (Script is not loaded)", mf.ToString().Get());
	return false;
}

bool CServer::RestartClientResource(CMetaFile & mf)
{
	if(!g_pClientResourceFileManager->Exists(mf))
		CLogFile::Printf("[Server] Failed to restart: %s (Resource is not loaded)", mf.ToString().Get());
	else if(!g_pClientResourceFileManager->Restart(mf))
		CLogFile::Printf("[Server] Failed to restart: %s", mf.ToString().Get());
	else
	{
		CLogFile::Printf("[Server] Restarted: %s", mf.ToString().Get());
		return true;
	}
	return false;
}

void CServer::StartMeta(String & meta)
{
	std::vector<CMetaFile> s_scripts	= GetServerScripts(meta);
	std::vector<CMetaFile> c_resources	= GetClientResources(meta);

	for(auto i = s_scripts.begin(); i != s_scripts.end(); i++)
	{
		StartServerScript(*i);
	}

	for(auto i = c_resources.begin(); i != c_resources.end(); i++)
	{
		StartClientResource(*i);
	}
}

void CServer::StopMeta(String & meta)
{
	std::vector<CMetaFile> s_scripts	= GetServerScripts(meta);
	std::vector<CMetaFile> c_resources	= GetClientResources(meta);

	for(auto i = s_scripts.begin(); i != s_scripts.end(); i++)
	{
		StopServerScript(*i);
	}

	for(auto i = c_resources.begin(); i != c_resources.end(); i++)
	{
		StopClientResource(*i);
	}
}

void CServer::RestartMeta(String & meta)
{
	std::vector<CMetaFile> s_scripts	= GetServerScripts(meta);
	std::vector<CMetaFile> c_resources	= GetClientResources(meta);

	for(auto i = s_scripts.begin(); i != s_scripts.end(); i++)
	{
		RestartServerScript(*i);
	}

	for(auto i = c_resources.begin(); i != c_resources.end(); i++)
	{
		RestartClientResource(*i);
	}
}

CMetaFile CServer::GetMetaFile(String & meta_and_file)
{
	CMetaFile mf(meta_and_file, MetaFileType_Undefined);

	auto resources = CVAR_GET_LIST("resource");
	for(auto i = resources.begin(); i != resources.end(); i++)
	{
		String metapath(SharedUtility::GetAbsolutePath("resources/%s/meta.xml", mf.m_Meta.Get()));

		TiXmlDocument doc;
		if(doc.LoadFile(metapath.Get()))
		{
			for(TiXmlNode * xNode = doc.RootElement()->FirstChildElement(); xNode; xNode = xNode->NextSibling())
			{
				// Is this not an element node?
				if(xNode->Type() != TiXmlNode::ELEMENT)
					continue;

				// Get the setting and value
				String xname = xNode->Value();
				String xvalue = xNode->ToElement()->Attribute("src");

				if(xvalue == mf.m_File)
				{
					if(xname == "serverscript")
					{
						mf.m_Type = MetaFileType_ServerScript;
						break;
					}
					else if(xname == "clientscript")
					{
						mf.m_Type = MetaFileType_ClientScript;
						break;
					}
					else if(xname == "clientresource")
					{
						mf.m_Type = MetaFileType_ClientResource;
						break;
					}
				}
			}
		}
	}
	return mf;
}

void CServer::RefreshResources()
{
	std::vector<CMetaFile> s_scripts;
	std::vector<CMetaFile> c_resources;

	auto resources = CVAR_GET_LIST("resource");
	for(auto ri = resources.begin(); ri != resources.end(); ri++)
	{
		String meta = *ri; 
		
		auto s = GetServerScripts(meta);
		auto c = GetClientResources(meta);

		s_scripts.insert(s_scripts.end(), s.begin(), s.end());
		c_resources.insert(c_resources.end(), c.begin(), c.end());
	}

	//Refresh methods return some stat as String
	CLogFile::Printf("[Server] Server scripts:    %s", g_pScriptingManager->Refresh(s_scripts, g_pScriptTimerManager).Get());
	CLogFile::Printf("[Server] Client resources:  %s", g_pClientResourceFileManager->Refresh(c_resources).Get());
}

void CServer::Process()
{
	// Get the current time
	unsigned long ulTime = SharedUtility::GetTime();

	// Is show fps enabled?
	if(m_bShowFPS)
	{
		// Get the amount of time elapsed since the last fps update
		unsigned long ulTimeElapsed = (ulTime - m_ulLastFPSUpdateTime);

		// Update the fps if a second or more has passed
		if(ulTimeElapsed >= 1000)
		{
			// Set the fps
			m_ulFramesPerSecond = m_ulFrameCount;

			// Set the server title
#ifdef WIN32
			String strTitle(SERVER_TITLE " - FPS: %d", m_ulFramesPerSecond);
			SetTitle(strTitle.C_String());
#endif

			// Reset the frame count
			m_ulFrameCount = 0;

			// Set the last fps update tick count
			m_ulLastFPSUpdateTime = ulTime;
		}

		// Increment the frame count
		m_ulFrameCount++;
	}

	g_pNetworkManager->Process();

	g_pVehicleManager->Process();

	if(g_pQuery)
		g_pQuery->Process();

	if(g_pMasterList)
		g_pMasterList->Pulse();

	g_pScriptTimerManager->Pulse();
	g_pModuleManager->Pulse();

	if(CVAR_GET_BOOL("frequentevents"))
		g_pEvents->Call("serverPulse");

	// Process the input queue
	ProcessInputQueue();
}

void CServer::OnUnload()
{
	// Unload all loaded scripts
	g_pScriptingManager->UnloadAll();

	CLogFile::Print(" ===== IV:MP Server shutting down. ===== ");

	SAFE_DELETE(g_pMasterList);
	SAFE_DELETE(g_pQuery);
	SAFE_DELETE(g_pScriptTimerManager);
	SAFE_DELETE(g_pModuleManager);
	SAFE_DELETE(g_pCheckpointManager);
	SAFE_DELETE(g_pPickupManager);
	SAFE_DELETE(g_pObjectManager);
	SAFE_DELETE(g_pActorManager);
	SAFE_DELETE(g_pVehicleManager);
	SAFE_DELETE(g_pPlayerManager);
	SAFE_DELETE(g_pBlipManager);
	SAFE_DELETE(g_pNetworkManager);
	CNetworkModule::Shutdown();
	SAFE_DELETE(g_pClientResourceFileManager);
	//SAFE_DELETE(g_pClientScriptFileManager);
	SAFE_DELETE(g_pScriptingManager);
	SAFE_DELETE(g_pWebserver);
	SAFE_DELETE(g_pTime);
	SAFE_DELETE(g_pTrafficLights);
	SAFE_DELETE(g_pEvents);
	CSettings::Close();
	CLogFile::Close();
}

void CServer::SendInput(String strInput)
{
	if(strInput.IsNotEmpty())
	{
		// Print the input to the log
		CLogFile::PrintfToFile("[INPUT] %s", strInput.Get());

		// Get the command and parameters
		size_t sSplit = strInput.Find(' ', 0);
		String strCommand = strInput.SubStr(0, sSplit++);
		String strParameters = strInput.SubStr(sSplit, (strInput.GetLength() - sSplit));

		// Execute predefined command, or call script event:
		if(strCommand.IsEmpty())
			return;
		else if(strCommand == "help" || strCommand == "?" || strCommand == "--usage")
		{
			printf("========== Available console commands: ==========\n");
			printf("say <text>\n");
			printf("uptime\n");
			printf("scripts\n");
			printf("players\n");
			printf("loadscript <meta>/<name>\n");
			printf("loadclientscript <meta>/<name>\n");
			printf("loadresource <meta>/<name>\n");
			printf("reloadscript <meta>/<name>\n");
			printf("reloadclientscript <meta>/<name>\n");
			printf("reloadresource <meta>/<name>\n");
			printf("unloadscript <meta>/<name>\n");
			printf("unloadclientscript <meta>/<name>\n");
			printf("unloadresource <meta>/<name>\n");
			printf("exit\n");
		}
		else if(strCommand == "say" || strCommand == "echo" || strCommand == "print")
		{
			if(strParameters.IsNotEmpty())
			{
				CBitStream bsSend;
				bsSend.Write((DWORD)0x00c83d);
				bsSend.Write(String("Server: %s",strParameters.Get()));
				g_pNetworkManager->RPC(RPC_Message, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
				CLogFile::Printf("[Server] Sent message to all players: 'Server: %s'", strParameters.Get());
			}
		}
		else if(strCommand == "start")
		{
			if(strParameters.IsNotEmpty())
			{
				if(strParameters.Contains('/'))
				{
					CMetaFile mf = GetMetaFile(strParameters);
					
					if(mf.m_Type == MetaFileType_ServerScript)
						StartServerScript(mf);
					else if(mf.m_Type == MetaFileType_ClientScript || mf.m_Type == MetaFileType_ClientResource)
						StartClientResource(mf);
					else
						CLogFile::Printf("[Server] Resource not exists or not defined in the meta.xml: %s", strParameters.Get());
				} 
				else
				{
					StartMeta(strParameters);
				}
			}
		}
		else if(strCommand == "stop")
		{
			if(strParameters.IsNotEmpty())
			{
				if(strParameters.Contains('/'))
				{
					CMetaFile mf = GetMetaFile(strParameters);
					
					if(mf.m_Type == MetaFileType_ServerScript)
						StopServerScript(mf);
					else if(mf.m_Type == MetaFileType_ClientScript || mf.m_Type == MetaFileType_ClientResource)
						StopClientResource(mf);
					else
						CLogFile::Printf("[Server] Resource not exists or not defined in the meta.xml: %s", strParameters.Get());
				} 
				else
				{
					StopMeta(strParameters);
				}
			}
		}
		else if(strCommand == "restart")
		{
			if(strParameters.IsNotEmpty())
			{
				if(strParameters.Contains('/'))
				{
					CMetaFile mf = GetMetaFile(strParameters);
					
					if(mf.m_Type == MetaFileType_ServerScript)
						RestartServerScript(mf);
					else if(mf.m_Type == MetaFileType_ClientScript || mf.m_Type == MetaFileType_ClientResource)
						RestartClientResource(mf);
					else
						CLogFile::Printf("[Server] Resource not exists or not defined in the meta.xml: %s", strParameters.Get());
				} 
				else
				{
					RestartMeta(strParameters);
				}
			}
		}
		else if(strCommand == "refresh")
		{
			RefreshResources();
		}
		else if(strCommand == "players")
		{
			int iPlayersCount = g_pPlayerManager->GetPlayerCount();
			for(int i = 0; i < MAX_PLAYERS; i++)
			{
				CPlayer* pPlayer = g_pPlayerManager->GetAt(i);
				if(!pPlayer)
					continue;

				CLogFile::Printf("[Server] Player: %s(%d, %s)", pPlayer->GetName().Get(), i, pPlayer->GetIp().Get());
			}

			CLogFile::Printf("[Server] Players online: %d/%d", iPlayersCount, CVAR_GET_INTEGER("maxplayers"));
		}
		else if(strCommand == "resources")
		{
			int iServerLoaded = 0;
			int iClientLoaded = 0;

			CLogFile::Printf("[Server] Server:");
			for(auto i = g_pScriptingManager->GetScriptList()->begin(); i != g_pScriptingManager->GetScriptList()->end(); i++)
			{
				CLogFile::Printf("[Server]     %-30s (Checksum: 0x%p)", (*i)->GetMetaFile().ToMetaAndFileString().Get(), (*i)->GetFileChecksum().GetChecksum());
				iServerLoaded++;
			}
			CLogFile::Printf("[Server] Client:");
			for(auto i = g_pClientResourceFileManager->m_Files.begin(); i != g_pClientResourceFileManager->m_Files.end(); ++ i)
			{
				CLogFile::Printf("[Server]     %-30s (Checksum: 0x%p)", i->m_MetaFile.ToMetaAndFileString().Get(), i->m_Checksum.GetChecksum());
				iClientLoaded++;
			}
			CLogFile::Printf("[Server] %d server resource(s), %d client resource(s)", iServerLoaded, iClientLoaded);
		}
		else if(strCommand == "uptime")
		{
			CLogFile::Printf("[Server] Server has been online for %s.", SharedUtility::GetTimePassedFromTime(g_ulStartTick).Get());
		}
		else if(strCommand == "quit" || strCommand == "exit")
		{
			CLogFile::Print("[Server] Server is going to shutdown NOW ....");
			SetActive(false);
		}
		else
		{
			CScriptArguments pArguments;
			pArguments.push(strInput);
			g_pEvents->Call("consoleInput", &pArguments);
		}
	}
}

void CServer::AddToInputQueue(String strString)
{
	m_inputQueueMutex.Lock();
	m_inputQueue.push(strString);
	m_inputQueueMutex.Unlock();
}

void CServer::SetTitle(String strTitle)
{
#ifdef WIN32
	SetConsoleTitle(strTitle);
#endif
}

void CServer::ProcessInputQueue()
{
	if(m_inputQueueMutex.TryLock())
	{
		while(!m_inputQueue.empty())
		{
			// Get the input string
			String strInput = m_inputQueue.front();

			// Send the input
			SendInput(strInput);

			// Erase the input string from the input queue
			m_inputQueue.pop();
		}

		m_inputQueueMutex.Unlock();
	}
}
