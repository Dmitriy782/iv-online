//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: EventNatives.cpp
// Project: Client.Core
// Author(s): jenksta
//            Einstein
// License: See LICENSE in root directory
//
//==============================================================================

#include "Natives.h"
#include "../CScriptingManager.h"
#include "CEvents.h"

// Event functions

void CEventNatives::Register(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterFunction("addEvent", Add, -1, NULL);
	pScriptingManager->RegisterFunction("callEvent", Call, -1, NULL);
	pScriptingManager->RegisterFunction("removeEvent", Remove, 2, "sc");
}

// addEvent(eventname, function, ...)
int CEventNatives::Add(lua_State * pVM)
{
	const char * szEventName;
	
	script_getstring(pVM, 1, &szEventName);
		
	void * pointer = (void *)lua_topointer(pVM, -1);
	int ref = luaL_ref(pVM, LUA_REGISTRYINDEX);

	luaL_where(pVM, 1);
	String w = lua_tostring(pVM, -1);

	if(strcmp(w, "M:\\Xenicgames\\IVMP\\Binary\\resources\\main\\c_scoreboard.lua:59: ") == 0)
	{
		//w=w;
		//int a = lua_checkstack(m_pVM, 10);
		//CLogFile::Print("59!!!");
	}

	script_pushbool(pVM, CEvents::GetInstance()->Add(szEventName, new CScriptEventHandler(pVM, ref, pointer, w)));
	return 1;
}

// TODO: 'Call for this script only' argument
// callEvent(eventname, defaultretval, ...)
int CEventNatives::Call(lua_State * pVM)
{
	CHECK_PARAMS_MIN("callEvent", 2);
	CHECK_TYPE("callEvent", 1, 1, SCRIPT_TYPE_STRING);
	//second is defaultretval

	int iTop = script_gettop(pVM);

	// Get the name
	const char * szEventName;
	script_getstring(pVM, 1, &szEventName);
	
	// Grab all arguments
	CScriptArguments pArguments;
	for(int i = 3; i <= iTop; ++ i)
		pArguments.pushFromStack(pVM, i);

	// Call the event
	CScriptArgument pReturn = CEvents::GetInstance()->Call(szEventName, &pArguments);
	pReturn.push(pVM);
	return 1;
}

// removeEvent(eventname, function)
int CEventNatives::Remove(lua_State * pVM)
{
	const char * szEventName;
	script_getstring(pVM, -2, &szEventName);
	void * pointer = (void *)lua_topointer(pVM, -1);

	//const char * type = lua_typename(pVM, lua_type(pVM, -1));

	auto h = CEvents::GetInstance()->FindScriptHandler(szEventName, pointer);
	if(h != NULL)
	{
		luaL_unref(pVM, LUA_REGISTRYINDEX, h->GetFunction());
		script_pushbool(pVM, CEvents::GetInstance()->Remove(szEventName, h));
		delete h; // see line 44
	}
	else
	{
		script_pushbool(pVM, false);
	}

	return 1;
}
