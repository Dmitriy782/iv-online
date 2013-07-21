//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CClientScriptManager.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CClientScriptManager.h"
#include <SharedUtility.h>
#include "Natives.h"
#include <CLogFile.h>

CClientScriptManager::CClientScriptManager()
{
	m_pScripting = new CScriptingManager();
	m_pScriptTimerManager = new CScriptTimerManager();
	m_pGUIManager = new CClientScriptGUIManager();

	// Register the client natives
	RegisterClientNatives(m_pScripting);

	// Register the world natives
	CWorldNatives::Register(m_pScripting);

	// Register the GUI natives
	RegisterGUINatives(m_pScripting);

	// Register the event natives
	CEventNatives::Register(m_pScripting);

	// Register the player natives
	CPlayerNatives::Register(m_pScripting);

	// Register the vehicle natives
	CVehicleNatives::Register(m_pScripting);

	// Register the area natives
	CAreaNatives::Register(m_pScripting);

	// Register the hash natives
	CHashNatives::Register(m_pScripting);

	// Register the script natives
	RegisterScriptNatives(m_pScripting);

	// Register the XML natives
	RegisterXMLNatives(m_pScripting);

	// Register the timer natives
	RegisterTimerNatives(m_pScripting);

	// Register the default constants
	m_pScripting->RegisterDefaultConstants();

	RegisterGUIElementNatives(m_pScripting);

	RegisterAudioNatives(m_pScripting);

	#ifdef IVMP_WEBKIT
		//m_pScripting->RegisterClass(&_CLASS_DECL(GUIWebView));
	#endif
}

CClientScriptManager::~CClientScriptManager()
{
	RemoveAll();
	SAFE_DELETE(m_pScriptTimerManager);
	SAFE_DELETE(m_pScripting);
	SAFE_DELETE(m_pGUIManager);
}

void CClientScriptManager::AddScript(CMetaFile & mf)
{
	m_Scripts.push_back(mf);
	CLogFile::Printf("ClientScript %s added.", mf.ToMetaAndFileString().Get());
}

void CClientScriptManager::RemoveScript(CMetaFile & mf)
{
	for(auto i = m_Scripts.begin(); i != m_Scripts.end(); i++)
	{
		if(*i == mf)
		{
			m_Scripts.erase(i);
			CLogFile::Printf("ClientScript %s removed.", mf.ToMetaAndFileString().Get());
			return;
		}
	}
}

void CClientScriptManager::Load(CMetaFile & mf)
{
	if(!Exists(mf))
		return;

	for(auto i = m_Scripts.begin(); i != m_Scripts.end(); i++)
	{
		if(*i == mf)
		{
			CScript* pScript = m_pScripting->Load(mf);
			if(pScript)
				CLogFile::Printf("ClientScript %s loaded.", mf.ToMetaAndFileString().Get());
			return;
		}
	}
}

void CClientScriptManager::Unload(CMetaFile & mf)
{
	if(!Exists(mf))
		return;

	for(auto i = m_Scripts.begin(); i != m_Scripts.end(); i++)
	{
		if(*i == mf)
		{
			auto s = m_pScripting->Get(mf);
			m_pScriptTimerManager->HandleScriptUnload(s);
			m_pScripting->Unload(s);
			CLogFile::Printf("ClientScript %s unloaded.", mf.ToMetaAndFileString().Get());
			return;
		}
	}
}

bool CClientScriptManager::Exists(CMetaFile & mf)
{
	for(auto i = m_Scripts.begin(); i != m_Scripts.end(); i++)
	{
		if(*i == mf)
			return true;
	}
	return false;
}

void CClientScriptManager::LoadAll()
{
	for(auto i = m_Scripts.begin(); i != m_Scripts.end(); i++)
	{
		Load(*i);
	}
}

void CClientScriptManager::RemoveAll()
{
	m_pScripting->UnloadAll();
	m_Scripts.clear();
}