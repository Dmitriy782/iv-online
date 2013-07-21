//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CModule.h
// Project: Server.Core
// Author(s): MaVe
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "../../Vendor/Lua/lua.hpp"
#include "CLibrary.h"
#include "Interfaces/InterfaceCommon.h"
#include <CLogFile.h>

static void * FunctionContainer[] =
{
	(void *)lua_absindex,
	(void *)lua_arith,
	(void *)lua_atpanic,
	(void *)lua_callk,
	(void *)lua_checkstack,
	(void *)lua_close,
	(void *)lua_compare,
	(void *)lua_concat,
	(void *)lua_copy,
	(void *)lua_createtable,
	(void *)lua_dump,
	(void *)lua_error,
	(void *)lua_gc,
	(void *)lua_getallocf,
	(void *)lua_getctx,
	(void *)lua_getfield,
	(void *)lua_getglobal,
	(void *)lua_gethook,
	(void *)lua_gethookcount,
	(void *)lua_gethookmask,
	(void *)lua_getinfo,
	(void *)lua_getlocal,
	(void *)lua_getmetatable,
	(void *)lua_getstack,
	(void *)lua_gettable,
	(void *)lua_gettop,
	(void *)lua_getupvalue,
	(void *)lua_getuservalue,
	(void *)lua_insert,
	(void *)lua_iscfunction,
	(void *)lua_isnumber,
	(void *)lua_isstring,
	(void *)lua_isuserdata,
	(void *)lua_len,
	(void *)lua_load,
	(void *)lua_newstate,
	(void *)lua_newthread,
	(void *)lua_newuserdata,
	(void *)lua_next,
	(void *)lua_pcallk,
	(void *)lua_pushboolean,
	(void *)lua_pushcclosure,
	(void *)lua_pushfstring,
	(void *)lua_pushinteger,
	(void *)lua_pushlightuserdata,
	(void *)lua_pushlstring,
	(void *)lua_pushnil,
	(void *)lua_pushnumber,
	(void *)lua_pushstring,
	(void *)lua_pushthread,
	(void *)lua_pushunsigned,
	(void *)lua_pushvalue,
	(void *)lua_pushvfstring,
	(void *)lua_rawequal,
	(void *)lua_rawget,
	(void *)lua_rawgeti,
	(void *)lua_rawgetp,
	(void *)lua_rawlen,
	(void *)lua_rawset,
	(void *)lua_rawseti,
	(void *)lua_rawsetp,
	(void *)lua_remove,
	(void *)lua_replace,
	(void *)lua_resume,
	(void *)lua_setallocf,
	(void *)lua_setfield,
	(void *)lua_setglobal,
	(void *)lua_sethook,
	(void *)lua_setlocal,
	(void *)lua_setmetatable,
	(void *)lua_settable,
	(void *)lua_settop,
	(void *)lua_setupvalue,
	(void *)lua_setuservalue,
	(void *)lua_status,
	(void *)lua_toboolean,
	(void *)lua_tocfunction,
	(void *)lua_tointegerx,
	(void *)lua_tolstring,
	(void *)lua_tonumberx,
	(void *)lua_topointer,
	(void *)lua_tothread,
	(void *)lua_tounsignedx,
	(void *)lua_touserdata,
	(void *)lua_type,
	(void *)lua_typename,
	(void *)lua_upvalueid,
	(void *)lua_upvaluejoin,
	(void *)lua_version,
	(void *)lua_xmove,
	(void *)lua_yieldk,
	(void *)CLogFile::Printf
};

static void * InterfacesContainer[32];
static void * NewInterfaceContainer[32];

typedef void (* SetupFunctions_t)(void *pContainer);
typedef void (* SetupInterfaces_t)(void *pContainer);
typedef void (* SetupNewInterfaces_t)(void *pContainer);
typedef bool (* InitModule_t)(char *);
typedef void (* ScriptLoad_t)(lua_State *);
typedef void (* ScriptUnload_t)(lua_State *);
typedef void (* Pulse_t)();

struct ModuleFunctions
{
	SetupFunctions_t pfnSetupFunctions;
	SetupInterfaces_t pfnSetupInterfaces;
	SetupNewInterfaces_t pfnSetupNewInterfaces;
	InitModule_t pfnInitModule;
	ScriptLoad_t pfnScriptLoad;
	ScriptUnload_t pfnScriptUnload;
	Pulse_t pfnPulse;
};

class CModule : public CModuleInterface
{
public:
	CModule(const char *szName);
	~CModule();

	bool IsValid();
	void ScriptLoad(lua_State * pVM);
	void ScriptUnload(lua_State * pVM);
	void Pulse();

private:
	CLibrary * m_pLibrary;
	ModuleFunctions m_ModuleFunctions;
};