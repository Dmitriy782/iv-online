//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CScript.cpp
// Project: Shared
// Author(s): jenksta
//            MaVe
// License: See LICENSE in root directory
//
//==============================================================================

#include <stdarg.h>
#include "CScriptingManager.h"
#include "../SharedUtility.h"
#include "../CEvents.h"
#include "../CLogFile.h"
#include "CScript.h"


void CScript::PrintError(bool showStack, const char * szFormat, ...)
{
	va_list args;
	char msg[4096];
	va_start(args, szFormat);
	vsnprintf(msg, sizeof(msg), szFormat, args);
	va_end(args);

	CLogFile::Printf("ERROR:");
	CLogFile::Printf("   Message: %s", msg);
}

void CScript::PrintLuaError(const char * szFormat, ...)
{
	va_list args;
	char msg[4096];
	va_start(args, szFormat);
	vsnprintf(msg, sizeof(msg), szFormat, args);
	va_end(args);

	const char * erorr;
	script_getstring(m_pVM, -1, &erorr);

	CLogFile::Printf("ERROR:");
	CLogFile::Printf("   Message: %s", msg);
	CLogFile::Printf("   Lua:     %s", erorr);
}

bool CScript::AddExternal(CMetaFile & mf)
{
	if(luaL_loadfile(m_pVM, mf.ToFullPath().Get()) != 0)
	{
		PrintLuaError("External script load, %s", mf.ToFullPath().Get());
		return false;
	}
	if(lua_pcall(m_pVM, 0, LUA_MULTRET, 0) == 0)
	{
		return true;
	}
	else
	{
		PrintLuaError("External script call, %s", mf.ToFullPath().Get());
		return false;
	}
	return true;
}

bool CScript::Load(CMetaFile & mf)
{
	m_MetaFile = mf;
	m_FileChecksum.Calculate(mf.ToFullPath());

	// Check if the script exists
	if(!SharedUtility::Exists(m_MetaFile.ToFullPath().Get()))
		return false;

	m_pVM = luaL_newstate();
	luaL_openlibs(m_pVM);

	if(luaL_loadfile(m_pVM, m_MetaFile.ToFullPath().Get()) != 0)
	{
		PrintLuaError("Script load, %s", m_MetaFile.ToFullPath().Get());
		return false;
	}

	//CLogFile::Printf("Loaded: %s", m_strName.Get());
	return true;
}

bool CScript::Execute()
{
	// Add the script name constant
	RegisterConstant("SCRIPT_NAME", m_MetaFile.m_File);

	// Add the script path constant
	RegisterConstant("SCRIPT_PATH", m_MetaFile.ToFullPath());

	if(lua_pcall(m_pVM, 0, LUA_MULTRET, 0) == 0)
	{
		return true;
	}
	else
	{
		PrintLuaError("Script execute, %s", m_MetaFile.ToFullPath().Get());
		return false;
	}
}

void CScript::Unload()
{
	// Pop the root table from the stack
	///SQ sq_pop(m_pVM, 1);

	// Close the squirrel VM
	lua_close(m_pVM);
	m_pVM = NULL;
}

void CScript::RegisterFunction(String strFunctionName, lua_CFunction pfnFunction, int iParameterCount, String strFunctionTemplate)
{
	lua_register(m_pVM, strFunctionName.Get(), pfnFunction);
}

bool CScript::RegisterClass(ScriptClassDecl * pClassDecl)
{

/*
	luaL_Reg sFooRegs[] =
	{
		{ "new", l_Foo_constructor },
		{ "add", l_Foo_add },
		{ "__gc", l_Foo_destructor },
		{ NULL, NULL }
	};
*/
	// Create a luaL metatable. This metatable is not
	// exposed to Lua. The "luaL_Foo" label is used by luaL
	// internally to identity things.
	
	if(luaL_newmetatable(m_pVM, pClassDecl->metatable) != 1)
	{
		PrintError("Can`t create metatable: %s", pClassDecl->metatable);
	}
	
	// Register the C functions _into_ the metatable we just created.
	luaL_setfuncs(m_pVM, (luaL_Reg *)pClassDecl->members, 0);
	// The Lua stack at this point looks like this:
	//
	// 1| metatable "luaL_Foo" |-1
	lua_pushvalue(m_pVM, -1);
	// The Lua stack at this point looks like this:
	//
	// 2| metatable "luaL_Foo" |-1
	// 1| metatable "luaL_Foo" |-2
	// Set the "__index" field of the metatable to point to itself
	// This pops the stack
	lua_setfield(m_pVM, -1, "__index");
	// The Lua stack at this point looks like this:
	//
	// 1| metatable "luaL_Foo" |-1
	// The luaL_Foo metatable now has the following fields
	// - __gc
	// - __index
	// - add
	// - new
	// Now we use setglobal to officially expose the luaL_Foo metatable
	// to Lua. And we use the name "Foo".
	//
	// This allows Lua scripts to _override_ the metatable of Foo.
	// For high security code this may not be called for but
	// we'll do this to get greater flexibility.
	lua_setglobal(m_pVM, pClassDecl->name);

	///SQ
/*
	// Get the stack top
	int oldtop = sq_gettop(m_pVM);

	// Push the class name onto the stack
	sq_pushstring(m_pVM, pClassDecl->name, -1);

	// Do we have a base class?
	if(pClassDecl->base)
	{
		// Push the base class name onto the stack
		sq_pushstring(m_pVM, pClassDecl->base, -1);

		// Attempt to get the base class
		if(SQ_FAILED(sq_get(m_pVM, -3)))
		{
			// Failed to get the base class
			sq_settop(m_pVM, oldtop);
			return false;
		}
	}

	// Create the class
	if(SQ_FAILED(sq_newclass(m_pVM, pClassDecl->base ? 1 : 0)))
	{
		// Failed to create the class, Restore the stack top
		sq_settop(m_pVM, oldtop);
		return false;
	}

	// Register the class members
	const ScriptClassMemberDecl * pMembers = pClassDecl->members;

	for(int x = 0; pMembers[x].szFunctionName; x++)
		RegisterFunction(pMembers[x].szFunctionName, pMembers[x].sqFunc, pMembers[x].iParameterCount, 
			pMembers[x].szFunctionTemplate);

	// Create a new slot
	sq_createslot(m_pVM, -3);*/
	return true;
}

void CScript::RegisterConstant(String strConstantName, CScriptArgument value)
{
	value.push(m_pVM);

	lua_setglobal(m_pVM, strConstantName.Get());
}

void CScript::Call(int functionRef, CScriptArguments * pArguments, CScriptArgument * pReturn, const String & comment)
{
	// Lock the squirrel call mutex
	m_squirrelCallMutex.TryLock(10);

	// Get the stack top
	//int iTop = lua_gettop(m_pVM);

	CLogFile::Printf("Script Call: %s", comment.Get());
/*
	if(comment == "M:\\Xenicgames\\IVMP\\Binary\\resources\\main\\c_scoreboard.lua:59: ")
	{
		CLogFile::Print("59!!!");
	}*/

	lua_rawgeti(m_pVM, LUA_REGISTRYINDEX, functionRef);
	if(lua_type(m_pVM, -1) == LUA_TFUNCTION)
	{
		// Process the parameters if needed
		int iParams = 0;

		if(pArguments)
		{
			pArguments->push_to_vm(m_pVM);
			iParams += pArguments->size();
		}

		if(lua_pcall(m_pVM, iParams, LUA_MULTRET, 0) == 0)
		{
			// Set the return value if needed
			if(pReturn)
				pReturn->pushFromStack(m_pVM, -1);
		}
		else
		{
			PrintLuaError("Function call", comment.Get());
		}
	}
	else
		PrintError(true, "Function ref is invalid, Ref: %d, %s", functionRef, comment.Get());

	// Restore the stack top
	lua_settop(m_pVM, 0);

	// Unlock the squirrel call mutex
	m_squirrelCallMutex.Unlock();
}
