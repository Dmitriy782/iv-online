//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CClientScriptManager.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include <list>
#include <Scripting\CScriptingManager.h>
#include <Scripting\CScriptTimerManager.h>
#include "CClientScriptGUIManager.h"

class CClientScriptManager
{
private:
	std::list<CMetaFile>	m_Scripts;
	CScriptingManager       * m_pScripting;
	CScriptTimerManager     * m_pScriptTimerManager;
	CClientScriptGUIManager * m_pGUIManager;

public:
	CClientScriptManager();
	~CClientScriptManager();

	CScriptingManager       * GetScriptingManager() { return m_pScripting; }
	CScriptTimerManager     * GetScriptTimerManager() { return m_pScriptTimerManager; }
	CClientScriptGUIManager * GetGUIManager() { return m_pGUIManager; }

	void                      AddScript(CMetaFile & mf);
	void                      RemoveScript(CMetaFile & mf);
	void                      Load(CMetaFile & mf);
	void                      Unload(CMetaFile & mf);
	bool                      Exists(CMetaFile & mf);
	void                      LoadAll();
	void					  RemoveAll();
};
