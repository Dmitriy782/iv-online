//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CScriptTimer.h
// Project: Shared
// Author(s): mabako
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "CScript.h"
#include "CScriptArguments.h"

class CScriptTimer
{
private:
	CScript* m_pScript;
	int m_pFunction;
	int m_uiInterval;
	int m_iRepeations;
	CScriptArguments* m_pArguments;
	unsigned int m_uiLastTick;
	bool m_bIsDead;
	bool m_bTraditional;

public:
	CScriptTimer(CScript* pScript, int pFunction, int uiInterval, int iRepeations, CScriptArguments* pArguments);
	~CScriptTimer();

	bool Pulse();
	CScript* GetScript();
	void Kill();
	bool IsDead();
	void SetTraditionalBehavior(bool bTraditional);
};
