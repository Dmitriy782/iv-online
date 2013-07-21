//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: ScriptNatives.cpp
// Project: Server.Core
// Author(s): mabako
// License: See LICENSE in root directory
//
//==============================================================================

#include "ModuleNatives.h"
#include "../CClientFileManager.h"
#include "Scripting/CScriptTimerManager.h"
#include "Scripting/CScriptingManager.h"

extern CScriptingManager * g_pScriptingManager;
//extern CClientFileManager * g_pClientScriptFileManager;
extern CClientFileManager * g_pClientResourceFileManager;
extern CScriptTimerManager * g_pScriptTimerManager;

// Script functions
namespace Modules
{
	// getScripts()
	std::list<String> * CScriptModuleNatives::GetScripts()
	{
		std::list<CScript*>* scripts = g_pScriptingManager->GetScriptList();
		std::list<String> * list = new std::list<String>;
		for(std::list<CScript*>::iterator iter = scripts->begin(); iter != scripts->end(); ++ iter)
		{
			list->push_back((*iter)->GetMetaFile().ToMetaAndFileString());
		}
		return list;
	}

	// getClientScripts()
	std::list<String> * CScriptModuleNatives::GetClientScripts()
	{
		std::list<String> * list = new std::list<String>;
		for(auto i = g_pClientResourceFileManager->m_Files.begin(); i != g_pClientResourceFileManager->m_Files.end(); ++ i)
		{
			if(i->m_MetaFile.m_Type == MetaFileType_ClientScript)
			{
				String s;
				s.Format("%s", i->m_MetaFile.ToMetaAndFileString().Get());
				list->push_back(s);
			}
		}
		return list;
	}

	// getClientResources()
	std::list<String> * CScriptModuleNatives::GetClientResources()
	{
		std::list<String> * list = new std::list<String>;
		for(auto i = g_pClientResourceFileManager->m_Files.begin(); i != g_pClientResourceFileManager->m_Files.end(); ++ i)
		{
			if(i->m_MetaFile.m_Type == MetaFileType_ClientResource)
			{
				String s;
				s.Format("%s", i->m_MetaFile.ToMetaAndFileString().Get());
				list->push_back(s);
			}
		}
		return list;
	}

	// getScriptName()
	const char * CScriptModuleNatives::GetScriptName(void * pVM)
	{
		String strScriptName = g_pScriptingManager->Get((lua_State *)pVM)->GetMetaFile().ToMetaAndFileString();
		return strScriptName.CopyData();
	}

	// loadScript(script)
	bool CScriptModuleNatives::LoadScript(const char * szScript)
	{
		CMetaFile mf(szScript, MetaFileType_ServerScript);

		if(!g_pScriptingManager->Get(mf))
		{
			//String strPath(SharedUtility::GetAbsolutePath("scripts\\%s", szScript));
			CScript * pScript = g_pScriptingManager->Load(mf);

			if(!pScript)
				CLogFile::Printf("Failed to load script %s (Script does not exist/Script compilation failed).", szScript);
			else
				CLogFile::Printf("Loaded script %s.", szScript);

			return (pScript != NULL);
		}

		return false;
	}

	// unloadScript(script)
	bool CScriptModuleNatives::UnloadScript(const char * szScript)
	{
		CMetaFile mf(szScript, MetaFileType_ServerScript);
	
		// TODO: script can't unload itself or pure virtual function call (VM destroyed while executing a command?)
		CScript* pScript = g_pScriptingManager->Get(mf);
		if(pScript)
		{
			g_pScriptTimerManager->HandleScriptUnload(pScript);
			CLogFile::Printf("Unloaded script %s.", szScript);
			return g_pScriptingManager->Unload(mf);
		}

		return false;
	}

	// reloadScript(script)
	bool CScriptModuleNatives::ReloadScript(const char * szScript)
	{
		CMetaFile mf(szScript, MetaFileType_ServerScript);

		// TODO: script can't unload itself or pure virtual function call (VM destroyed while executing a command?)
		CScript* pScript = g_pScriptingManager->Get(mf);

		if(pScript)
		{
			g_pScriptTimerManager->HandleScriptUnload(pScript);
			//String strPath(SharedUtility::GetAbsolutePath("scripts\\%s", mf));
			CLogFile::Printf("Reloaded script %s.", szScript);
			return (g_pScriptingManager->Unload(mf) && g_pScriptingManager->Load(mf));
		}

		return false;
	}

	// loadClientScript(script)
	bool CScriptModuleNatives::LoadClientScript(const char * szScript)
	{
		CMetaFile mf(szScript, MetaFileType_ClientScript);
		if(!g_pClientResourceFileManager->Exists(mf))
		{
			return g_pClientResourceFileManager->Start(mf);
		}

		return false;
	}

	// unloadClientScript(script)
	bool CScriptModuleNatives::UnloadClientScript(const char * szScript)
	{
		CMetaFile mf(szScript, MetaFileType_ClientScript);
		if(g_pClientResourceFileManager->Exists(mf))
		{
			return g_pClientResourceFileManager->Stop(mf);
		}

		return false;
	}

	// reloadClientScript(script)
	bool CScriptModuleNatives::ReloadClientScript(const char * szScript)
	{
		CMetaFile mf(szScript, MetaFileType_ClientScript);
		if(g_pClientResourceFileManager->Exists(mf))
		{
			return g_pClientResourceFileManager->Restart(mf);
		}

		return false;
	}

	// loadClientResource(resource)
	bool CScriptModuleNatives::LoadClientResource(const char * szResource)
	{
		CMetaFile mf(szResource, MetaFileType_ClientResource);
		if(!g_pClientResourceFileManager->Exists(mf))
		{
			return g_pClientResourceFileManager->Start(mf);
		}

		return false;
	}

	// unloadClientResource(resource)
	bool CScriptModuleNatives::UnloadClientResource(const char * szResource)
	{
		CMetaFile mf(szResource, MetaFileType_ClientResource);
		if(g_pClientResourceFileManager->Exists(mf))
		{
			return g_pClientResourceFileManager->Stop(mf);
		}

		return false;
	}

	// reloadClientResource(resource)
	bool CScriptModuleNatives::ReloadClientResource(const char * szResource)
	{
		CMetaFile mf(szResource, MetaFileType_ClientResource);
		if(g_pClientResourceFileManager->Exists(mf))
		{
			return g_pClientResourceFileManager->Restart(mf);
		}

		return false;
	}
}