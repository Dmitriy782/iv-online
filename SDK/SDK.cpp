//============== Copyright © 2010 IV:MP Team. All rights reserved. ==============
// File: SDK.cpp
//===============================================================================

#include "SDK.h"

InterfaceContainer_t	InterfaceContainer;
NewInterfaceContainer_t NewInterfaceContainer;
FuncContainer_t			FuncContainer;

EXPORT void SetupFunctions(FuncContainer_t * pContainer)
{
	FuncContainer = *pContainer;
}

EXPORT void SetupInterfaces(InterfaceContainer_t * pContainer)
{
	InterfaceContainer = *pContainer;
}

EXPORT void SetupNewInterfaces(NewInterfaceContainer_t * pContainer)
{
	NewInterfaceContainer = *pContainer;
}

void RegisterFunction(lua_State * pVM, const char * szName, lua_CFunction pfnFunction)
{
	lua_register(pVM, szName, pfnFunction);
}

void RegisterConstant(lua_State * pVM, const char *szName, int value)
{
	lua_pushinteger(pVM, value);
	lua_setglobal(pVM, szName);
}

void RegisterConstant(lua_State * pVM, const char *szName, bool value)
{
	lua_pushboolean(pVM, value);
	lua_setglobal(pVM, szName);
}

void RegisterConstant(lua_State * pVM, const char *szName, const char* value, int len)
{
	lua_pushlstring(pVM, value, (size_t)len);
	lua_setglobal(pVM, szName);
}

void RegisterConstant(lua_State * pVM, const char *szName, float value)
{
	lua_pushnumber(pVM, value);
	lua_setglobal(pVM, szName);
}
