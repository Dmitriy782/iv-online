//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CModuleManager.h
// Project: Server.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include <list>
#include "CModule.h"
#include "Interfaces/InterfaceCommon.h"

class CModuleManager : public CModuleManagerInterface
{
public:
	CModuleManager();
	~CModuleManager();

	CModule * LoadModule(const char * szName);
	void ScriptLoad(lua_State * pVM);
	void ScriptUnload(lua_State * pVM);
	void Pulse();

private:
	std::list<CModule *> m_lstModules;
};