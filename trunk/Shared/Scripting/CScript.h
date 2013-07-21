//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CScript.h
// Project: Shared
// Author(s): jenksta
//            MaVe
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include <assert.h>
#include <stdlib.h>
#include "CScriptArguments.h"
#include <Threading/CMutex.h>
#include "../MetaFile.h"
#include "../CFileChecksum.h"

#if defined(WIN32) && defined(RegisterClass)
#undef RegisterClass
#endif

struct ScriptClassMemberDecl 
{
	char *			szFunctionName;
	lua_CFunction	sqFunc;
//	int				iParameterCount;
//	char *			szFunctionTemplate;
};

struct ScriptClassDecl
{
	char * name;
	char * metatable;
	char * base;
	ScriptClassMemberDecl * members;
};

class CScript
{
private:
	lua_State *		m_pVM;
	CMutex			m_squirrelCallMutex;
	CMetaFile		m_MetaFile;
	CFileChecksum	m_FileChecksum;

	void			 PrintLuaError(const char * szFormat, ...);
	void			 PrintError(bool showStack, const char * szFormat, ...);
	static void      CompilerErrorFunction(lua_State * pVM, const char * szError, const char * szSource, int iLine, int iColumn);
	static int		 PrintErrorFunction(lua_State * pVM);

public:
	lua_State *      GetVM() { return m_pVM; }
	CMetaFile &      GetMetaFile() { return m_MetaFile; }
	CFileChecksum &  GetFileChecksum() { return m_FileChecksum; }
	bool             Load(CMetaFile & mf);
	bool             Execute();
	void             Unload();
	void             RegisterFunction(String strFunctionName, lua_CFunction pfnFunction, int iParameterCount, String strFunctionTemplate);
	bool             RegisterClass(ScriptClassDecl * pClassDecl);
	void             RegisterConstant(String strConstantName, CScriptArgument value);
	void             Call(int functionRef, CScriptArguments * pArguments, CScriptArgument * pReturn, const String & comment);
	bool			 AddExternal(CMetaFile & mf);
};
