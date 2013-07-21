//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: TimerNatives.cpp
// Project: Shared
// Author(s): jenksta
//            MaVe
//            mabako
// License: See LICENSE in root directory
//
//==============================================================================

#include "Natives.h"
#include "../CScript.h"
#include "../CScriptingManager.h"
#include "../CScriptTimerManager.h"
#include "../CScriptArguments.h"
#include "../CScriptTimer.h"

// Timer functions
_BEGIN_CLASS(timer)
_MEMBER_FUNCTION(timer, constructor, -1, NULL)
_MEMBER_FUNCTION(timer, isActive, 0, NULL)
_MEMBER_FUNCTION(timer, kill, 0, NULL)
_MEMBER_FUNCTION(timer, setTraditionalBehavior, 1, "b")
_END_CLASS(timer)

void RegisterTimerNatives(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterClass(&_CLASS_DECL(timer));
}

_MEMBER_FUNCTION_DESTRUCTOR(timer)
{
	//CScriptTimer * pTimer = (CScriptTimer *)pInst;
	CScriptTimer * pTimer = script_getinstance<CScriptTimer *>(pVM);

	if(pTimer != NULL && CScriptTimerManager::GetInstance()->contains(pTimer))
		pTimer->Kill();

	return 1;
}

_MEMBER_FUNCTION_IMPL(timer, constructor)
{
	CHECK_PARAMS_MIN("timer", 3);

//	if(script_gettype(pVM, 2) != OT_NATIVECLOSURE)
//		CHECK_TYPE("timer", 1, 2, OT_CLOSURE);
	CHECK_TYPE("timer", 1, 1, SCRIPT_TYPE_FUNCTION);
	CHECK_TYPE("timer", 2, 2, SCRIPT_TYPE_INTEGER);
	CHECK_TYPE("timer", 3, 3, SCRIPT_TYPE_INTEGER);

	int interval = 0;
	int repeations = 0;
	script_getinteger(pVM, 2, &interval);
	script_getinteger(pVM, 3, &repeations);

	// Ensure we have a valid repeations value
	if(repeations < -1 || repeations == 0)
	{
		script_pushbool(pVM, false);
		return 1;
	}

	//SQObjectPtr pFunction;
	//pFunction = stack_get(pVM, 2);
	//const char * pFunction;
	//script_getstring(pVM, 1, &pFunction);
	int pFunction = luaL_ref(pVM, LUA_REGISTRYINDEX);

	CScriptArguments * pArguments = new CScriptArguments(pVM, 5);

	CScriptTimer * pTimer = new CScriptTimer(CScriptingManager::GetInstance()->Get(pVM), pFunction, interval, repeations, pArguments);

	if(script_setinstance(pVM, pTimer, &_CLASS_DECL(timer)) != 0)
	{
		delete pTimer;
		delete pArguments;
		script_pushnull(pVM);
		return 1;
	}

	CScriptTimerManager::GetInstance()->push_back(pTimer);
	return 1;
}

_MEMBER_FUNCTION_IMPL(timer, isActive)
{
	CScriptTimer * pTimer = script_getinstance<CScriptTimer *>(pVM);

	if(!pTimer)
	{
		CLogFile::Print("Failed to get the timer instance.");
		script_pushbool(pVM, false);
		return 1;
	}

	if(CScriptTimerManager::GetInstance()->contains(pTimer))
	{
		if(pTimer->IsDead())
		{
			// destructor?
			// script_setinstance(pVM, NULL, &_CLASS_DECL(timer));
			script_pushbool(pVM, false);
			return 1;
		}

		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

_MEMBER_FUNCTION_IMPL(timer, kill)
{
	CScriptTimer * pTimer = script_getinstance<CScriptTimer *>(pVM);

	if(!pTimer)
	{
		CLogFile::Print("Failed to get the timer instance.");
		script_pushbool(pVM, false);
		return 1;
	}

	if(CScriptTimerManager::GetInstance()->contains(pTimer) && !pTimer->IsDead())
	{
		pTimer->Kill();
		script_pushbool(pVM, true);
		// destructor?
		// script_setinstance(pVM, NULL, &_CLASS_DECL(timer));
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

_MEMBER_FUNCTION_IMPL(timer, setTraditionalBehavior)
{
	CScriptTimer * pTimer = script_getinstance<CScriptTimer *>(pVM);

	if(!pTimer)
	{
		CLogFile::Print("Failed to get the timer instance.");
		script_pushbool(pVM, false);
		return 1;
	}

	if(pTimer != 0 && CScriptTimerManager::GetInstance()->contains(pTimer) && !pTimer->IsDead())
	{
		bool b;
		script_getbool(pVM, 1, (bool*)&b);
		pTimer->SetTraditionalBehavior(b);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}
