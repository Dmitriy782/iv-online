//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: ScriptNatives.cpp
// Project: Server.Core
// Author(s): mabako
// License: See LICENSE in root directory
//
//==============================================================================

#include "../Natives.h"
#include "../CClientFileManager.h"
#include "Scripting/CScriptTimerManager.h"

extern CScriptingManager * g_pScriptingManager;
//extern CClientFileManager * g_pClientScriptFileManager;
extern CClientFileManager * g_pClientResourceFileManager;
extern CScriptTimerManager * g_pScriptTimerManager;

// Script functions

void RegisterScriptNatives(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterFunction("getScripts", script_server_getscripts, 0, NULL);
	pScriptingManager->RegisterFunction("getClientScripts", script_server_getclientscripts, 0, NULL);
	pScriptingManager->RegisterFunction("getScriptName", script_server_getscriptname, 0, NULL);
	pScriptingManager->RegisterFunction("loadScript", script_server_loadscript, 1, "s");
	pScriptingManager->RegisterFunction("unloadScript", script_server_unloadscript, 1, "s");
	pScriptingManager->RegisterFunction("reloadScript", script_server_reloadscript, 1, "s");
	pScriptingManager->RegisterFunction("loadClientScript", script_server_loadclientscript, 1, "s");
	pScriptingManager->RegisterFunction("unloadClientScript", script_server_unloadclientscript, 1, "s");
	pScriptingManager->RegisterFunction("reloadClientScript", script_server_reloadclientscript, 1, "s");
	pScriptingManager->RegisterFunction("loadClientResource", script_server_loadclientresource, 1, "s");
	pScriptingManager->RegisterFunction("unloadClientResource", script_server_unloadclientresource, 1, "s");
	pScriptingManager->RegisterFunction("reloadClientResource", script_server_reloadclientresource, 1, "s");
}

// getScripts()
int script_server_getscripts(lua_State * pVM)
{
	CScriptArray a(pVM);
	std::list<CScript*>* scripts = g_pScriptingManager->GetScriptList();
	for(auto i = scripts->begin(); i != scripts->end(); ++ i)
	{
		auto name = (*i)->GetMetaFile().ToMetaAndFileString();
		a.push(name.C_String(), name.GetLength());
		//script_arrayappend(pVM, -2);
	}
	return 1;
}

// getClientScripts()
int script_server_getclientscripts(lua_State * pVM)
{
	CScriptArray a(pVM);
	for(auto i = g_pClientResourceFileManager->m_Files.begin(); i != g_pClientResourceFileManager->m_Files.end(); ++i)
	{
		if(i->m_MetaFile.m_Type == MetaFileType_ClientScript)
		{
			auto name = i->m_MetaFile.ToMetaAndFileString();
			a.push(name.C_String(), name.GetLength());
		}
		//script_arrayappend(pVM, -2);
	}
	return 1;
}

// getClientResources()
int script_server_getclientresources(lua_State * pVM)
{
	CScriptArray a(pVM);
	for(auto i = g_pClientResourceFileManager->m_Files.begin(); i != g_pClientResourceFileManager->m_Files.end(); ++i)
	{
		if(i->m_MetaFile.m_Type == MetaFileType_ClientResource)
		{
			auto name = i->m_MetaFile.ToMetaAndFileString();
			a.push(name.C_String(), name.GetLength());
		}
		//script_arrayappend(pVM, -2);
	}
	return 1;
}

// getScriptName()
int script_server_getscriptname(lua_State * pVM)
{
	String strScriptName = g_pScriptingManager->Get(pVM)->GetMetaFile().ToMetaAndFileString();
	script_pushlstring(pVM, strScriptName.Get(), strScriptName.GetLength());
	return 1;
}

// loadScript(script)
int script_server_loadscript(lua_State * pVM)
{
	const char* szScript;
	script_getstring(pVM, 1, &szScript);
	CMetaFile mf(szScript, MetaFileType_ServerScript);

	if(!g_pScriptingManager->Get(mf))
	{
		//String strPath(SharedUtility::GetAbsolutePath("%s\\%s", mf.m_Meta.Get(), mf.m_File.Get()));
		CScript * pScript = g_pScriptingManager->Load(mf);

		if(!pScript)
			CLogFile::Printf("Failed to load script %s (Script does not exist/Script compilation failed).", szScript);
		else
			CLogFile::Printf("Loaded script %s.", szScript);

		script_pushbool(pVM, (pScript != NULL));
	}
	else
		script_pushbool(pVM, false);
	return 1;
}

// unloadScript(script)
int script_server_unloadscript(lua_State * pVM)
{
	const char* szScript;
	script_getstring(pVM, 1, &szScript);
	CMetaFile mf(szScript, MetaFileType_ServerScript);

	// TODO: script can't unload itself or pure virtual function call (VM destroyed while executing a command?)
	CScript* pScript = g_pScriptingManager->Get(mf);
	if(pScript && pScript->GetVM() != pVM)
	{
		g_pScriptTimerManager->HandleScriptUnload(pScript);
		script_pushbool(pVM, g_pScriptingManager->Unload(mf));
		CLogFile::Printf("Unloaded script %s.", szScript);
	}
	else
		script_pushbool(pVM, false);
	return 1;
}

// reloadScript(script)
int script_server_reloadscript(lua_State * pVM)
{
	const char* szScript;
	script_getstring(pVM, 1, &szScript);
	CMetaFile mf(szScript, MetaFileType_ServerScript);

	// TODO: script can't unload itself or pure virtual function call (VM destroyed while executing a command?)
	CScript* pScript = g_pScriptingManager->Get(mf);

	if(pScript && pScript->GetVM() != pVM)
	{
		g_pScriptTimerManager->HandleScriptUnload(pScript);
		//String strPath(SharedUtility::GetAbsolutePath("scripts\\%s", szScript));
		script_pushbool(pVM, g_pScriptingManager->Unload(mf) && g_pScriptingManager->Load(mf));
		CLogFile::Printf("Reloaded script %s.", szScript);
	}
	else
		script_pushbool(pVM, false);

	return 1;
}

// loadClientScript(script)
int script_server_loadclientscript(lua_State * pVM)
{
	const char* szScript;
	script_getstring(pVM, 1, &szScript);

	CMetaFile mf(szScript, MetaFileType_ClientScript);

	if(!g_pClientResourceFileManager->Exists(mf))
	{
		script_pushbool(pVM, g_pClientResourceFileManager->Start(mf));
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// unloadClientScript(script)
int script_server_unloadclientscript(lua_State * pVM)
{
	const char* szScript;
	script_getstring(pVM, 1, &szScript);

	CMetaFile mf(szScript, MetaFileType_ClientScript);

	if(g_pClientResourceFileManager->Exists(mf))
	{
		script_pushbool(pVM, g_pClientResourceFileManager->Stop(mf));
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// reloadClientScript(script)
int script_server_reloadclientscript(lua_State * pVM)
{
	const char* szScript;
	script_getstring(pVM, 1, &szScript);

	CMetaFile mf(szScript, MetaFileType_ClientScript);

	if(g_pClientResourceFileManager->Exists(mf))
	{
		script_pushbool(pVM, g_pClientResourceFileManager->Restart(mf));
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// loadClientResource(resource)
int script_server_loadclientresource(lua_State * pVM)
{
	const char* szResource;
	script_getstring(pVM, 1, &szResource);

	CMetaFile mf(szResource, MetaFileType_ClientResource);

	if(!g_pClientResourceFileManager->Exists(mf))
	{
		script_pushbool(pVM, g_pClientResourceFileManager->Start(mf));
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// unloadClientResource(resource)
int script_server_unloadclientresource(lua_State * pVM)
{
	const char* szResource;
	script_getstring(pVM, 1, &szResource);

	CMetaFile mf(szResource, MetaFileType_ClientResource);

	if(g_pClientResourceFileManager->Exists(mf))
	{
		script_pushbool(pVM, g_pClientResourceFileManager->Stop(mf));
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// reloadClientResource(resource)
int script_server_reloadclientresource(lua_State * pVM)
{
	const char* szResource;
	script_getstring(pVM, 1, &szResource);

	CMetaFile mf(szResource, MetaFileType_ClientResource);

	if(g_pClientResourceFileManager->Exists(mf))
	{
		script_pushbool(pVM, g_pClientResourceFileManager->Restart(mf));
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}
