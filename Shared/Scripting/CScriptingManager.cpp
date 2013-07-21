//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CScriptingManager.cpp
// Project: Shared
// Author(s): jenksta
//            MaVe
// License: See LICENSE in root directory
//
//==============================================================================
// TODO: UnregisterConstant(constantname)
// TODO: Check if the constant already exists in RegisterConstant
// TODO: Free allocated constant memory

#include "CScriptingManager.h"

#include "../CEvents.h"
#include "../CLogFile.h"
#include <Common.h>

// FIXUPDATE
// jenksta: HACKY!!!
#ifdef _SERVER
#include "../../Server/Core/CModuleManager.h"
extern CModuleManager * g_pModuleManager;
#endif

CScriptingManager * CScriptingManager::m_pInstance = NULL;


CScriptingManager::CScriptingManager()
{
	// Set our instance
	m_pInstance = this;
}

CScriptingManager::~CScriptingManager()
{
	// Reset our instance
	m_pInstance = NULL;
}

CScript * CScriptingManager::Load(CMetaFile & mf)
{
	CScript * pScript = new CScript();

	if(!pScript->Load(mf))
	{
		delete pScript;
		return NULL;
	}

	for(auto i = m_funcs.begin(); i != m_funcs.end(); i++)
		pScript->RegisterFunction((*i)->strName, (*i)->pfnFunction, (*i)->iParameterCount, (*i)->strTemplate);

	for(auto i = m_classes.begin(); i != m_classes.end(); i++)
		pScript->RegisterClass(*i);

	for(auto i = m_constants.begin(); i != m_constants.end(); i++)
		pScript->RegisterConstant((*i)->strName, (*i)->value);

#ifdef _SERVER
	if(g_pModuleManager)
		g_pModuleManager->ScriptLoad(pScript->GetVM());
#endif

	// not a very good way , but leave it as is for now
	for(auto i = m_scripts.begin(); i != m_scripts.end(); i++)
	{
		if((*i)->GetMetaFile().m_Meta == "common" && (*i)->GetMetaFile().m_Type == mf.m_Type)
		{
			pScript->AddExternal((*i)->GetMetaFile());
		}
	}

	if(!pScript->Execute())
	{
		delete pScript;
		CLogFile::Printf("Failed to execute script %s", mf.ToMetaAndFileString().Get());
		return NULL;
	}

	m_scripts.push_back(pScript);

	CEvents * pEvents = CEvents::GetInstance();
	pEvents->Call("scriptInit", pScript);

	CScriptArguments arguments;
	arguments.push(mf.ToMetaAndFileString());
	pEvents->Call("scriptLoad", &arguments);
	return pScript;
}

bool CScriptingManager::Unload(CScript * pScript)
{
	if(pScript)
	{
		CEvents * pEvents = CEvents::GetInstance();
		pEvents->Call("scriptExit", pScript);

		CScriptArguments pArguments;
		pArguments.push(pScript->GetMetaFile().ToMetaAndFileString());
		pEvents->Call("scriptUnload", &pArguments);
		pEvents->RemoveScript(pScript->GetVM());

#ifdef _SERVER
		if(g_pModuleManager)
			g_pModuleManager->ScriptUnload(pScript->GetVM());
#endif

		pScript->Unload();
		m_scripts.remove(pScript);
		delete pScript;
		return true;
	}

	return false;
}

bool CScriptingManager::Unload(CMetaFile & mf)
{
	CScript * pScript = Get(mf);

	if(pScript)
		return Unload(pScript);

	return false;
}

void CScriptingManager::UnloadAll()
{
	auto scripts = m_scripts;
	for(auto iter = scripts.begin(); iter != scripts.end(); iter++)
	{
		Unload((*iter)->GetMetaFile());
	}
	m_scripts.clear();
}

void CScriptingManager::RegisterFunction(String strFunctionName, lua_CFunction pfnFunction, int iParameterCount, String strFunctionTemplate)
{
	ScriptingFunction * pFunction = new ScriptingFunction;
	pFunction->strName = strFunctionName;
	pFunction->pfnFunction = pfnFunction;
	pFunction->iParameterCount = iParameterCount;
	pFunction->strTemplate = strFunctionTemplate;
	m_funcs.push_back(pFunction);

	for(auto i = m_scripts.begin(); i != m_scripts.end(); i++)
		(*i)->RegisterFunction(strFunctionName, pfnFunction, iParameterCount, strFunctionTemplate);
}

void CScriptingManager::RegisterClass(ScriptClassDecl * pClassDeclaration)
{
	m_classes.push_back(pClassDeclaration);

	for(auto i = m_scripts.begin(); i != m_scripts.end(); i++)
		(*i)->RegisterClass(pClassDeclaration);
}

void CScriptingManager::RegisterConstant(String strConstantName, CScriptArgument value)
{
	ScriptingConstant * pConstant = new ScriptingConstant;
	pConstant->strName = strConstantName;
	pConstant->value = value;
	m_constants.push_back(pConstant);

	for(auto i = m_scripts.begin(); i != m_scripts.end(); i++)
		(*i)->RegisterConstant(strConstantName, value);
}

void CScriptingManager::RegisterDefaultConstants()
{
	RegisterConstant("MAX_PLAYERS", MAX_PLAYERS);
	RegisterConstant("MAX_VEHICLES", MAX_VEHICLES);
	RegisterConstant("MAX_OBJECTS", MAX_OBJECTS);
	RegisterConstant("MAX_CHECKPOINTS", MAX_CHECKPOINTS);
	RegisterConstant("MAX_BLIPS", MAX_BLIPS);
	RegisterConstant("MAX_ACTORS", MAX_ACTORS);
	RegisterConstant("MAX_NAME_LENGTH", MAX_NAME_LENGTH);
	// TODO: Merge all into INVALID_ENTITY_ID or leave as is?
	RegisterConstant("INVALID_PLAYER_ID", INVALID_ENTITY_ID);
	RegisterConstant("INVALID_VEHICLE_ID", INVALID_ENTITY_ID);
	RegisterConstant("INVALID_OBJECT_ID", INVALID_ENTITY_ID);
	RegisterConstant("INVALID_ACTOR_ID", INVALID_ENTITY_ID);
	RegisterConstant("INVALID_CHECKPOINT_ID", INVALID_ENTITY_ID);
	RegisterConstant("INVALID_BLIP_ID", INVALID_ENTITY_ID);
	RegisterConstant("INVALID_PICKUP_ID", INVALID_ENTITY_ID);
	RegisterConstant("IVMP_VERSION", MOD_VERSION_STRING);
}

CScript * CScriptingManager::Get(CMetaFile & mf)
{
	for(auto i = m_scripts.begin(); i != m_scripts.end(); i++)
	{
		if((*i)->GetMetaFile() == mf)
			return (*i);
	}
	return NULL;
}

CScript * CScriptingManager::Get(lua_State * pVM)
{
	for(auto i = m_scripts.begin(); i != m_scripts.end(); i++)
	{
		if((*i)->GetVM() == pVM)
			return (*i);
	}
	return NULL;
}

String CScriptingManager::Refresh(std::vector<CMetaFile> & scripts, CScriptTimerManager * stm)
{
	int nLoaded=0, nUloaded=0, nRefreshed=0;

	for(auto i = scripts.begin(); i != scripts.end(); i++)
	{
		auto s = Get(*i);
		if(s == NULL)
		{
			Load(*i);
			nLoaded++;
		}
		else
		{
			CFileChecksum cs;
			cs.Calculate(i->ToFullPath().Get());
			if(cs != s->GetFileChecksum())
			{
				stm->HandleScriptUnload(s);
				Unload(s);
				Load(*i);
				nRefreshed++;
			}
		}
	}

	for(auto i = m_scripts.begin(); i != m_scripts.end(); )
	{
		if(std::find_if(begin(scripts), end(scripts), [&i](CMetaFile & mf){ return mf == (*i)->GetMetaFile(); } ) == scripts.end())
		{
			Unload((*i++)->GetMetaFile());
			nUloaded++;
		}
		else
			i++;
	}
	return String("Loaded %d    Unloaded %d    Refreshed %d", nLoaded, nUloaded, nRefreshed);

}