//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CScriptingManager.h
// Project: Shared
// Author(s): jenksta
//            MaVe
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include <list>
#include <vector>
#include <string>
#include <algorithm>

#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif

#include "CScript.h"
#include "CScriptTimerManager.h"

template <typename T>
static int script_setinstance(lua_State * pVM, T pInstance, ScriptClassDecl * cd/*, int iIndex = 1*/)
{
	T * udata = (T *)lua_newuserdata(pVM, sizeof(T *));
	*udata = pInstance;
	luaL_getmetatable(pVM, cd->metatable);
	lua_setmetatable(pVM, -2);
	return 0;
}

// Now a function
template <typename T>
static T script_getinstance(lua_State * pVM, int iIndex = 1)
{
	return *(T *)lua_touserdata(pVM, iIndex);
}

//#define _SET_RELEASE_HOOK(classname) 
//	sq_setreleasehook(pVM, 1, __##classname##_releasehook);

#define _MEMBER_FUNCTION_IMPL(classname, name) \
	int __##classname##_##name(lua_State * pVM)

#define _MEMBER_FUNCTION_DESTRUCTOR(classname) \
	int __##classname##_destructor(lua_State * pVM)

/*#define _BEGIN_CLASS(classname)  \
	int __##classname##__typeof(lua_State * pVM) \
	{ \
		lua_pushstring(pVM, #classname); \
		return 1; \
	} \*/
#define _BEGIN_CLASS(classname)  \
	int __##classname##_destructor(lua_State * pVM);\
	struct ScriptClassMemberDecl __##classname##_members[] = \
	{ \
		{ "__gc", __##classname##_destructor },//{ "_typeof", __##classname##__typeof },

#define _MEMBER_FUNCTION(classname, name, parametercount, functiontemplate) \
		{ #name, __##classname##_##name/*, parametercount, functiontemplate */},

#define _END_CLASS(classname) \
		{ NULL, NULL } \
	}; \
	struct ScriptClassDecl __##classname##_decl = \
		{  #classname, "metatable_"#classname, NULL, __##classname##_members }; 

#define _END_CLASS_BASE(classname, baseclassname) \
		{ NULL, NULL } \
	}; \
	struct ScriptClassDecl __##classname##_decl = \
{  #classname, "metatable_"#classname, #baseclassname, __##classname##_members }; 

#define _CLASS_DECL(classname) \
	__##classname##_decl

struct ScriptingFunction
{
	String        strName;
	lua_CFunction pfnFunction;
	int			  iParameterCount;
	String		  strTemplate;
};

struct ScriptingConstant
{
	String            strName;
	CScriptArgument value;
};

class CScriptingManager
{
	private:
		static CScriptingManager     *	m_pInstance;
		std::list<CScript *>			m_scripts;
		std::list<ScriptingFunction *>	m_funcs;
		std::list<ScriptClassDecl *>	m_classes;
		std::list<ScriptingConstant *>	m_constants;

	public:
		CScriptingManager();
		~CScriptingManager();

		static CScriptingManager *	GetInstance() { return m_pInstance; }
		CScript					 *	Load(CMetaFile & mf);
		bool						Unload(CScript * pScript);
		bool						Unload(CMetaFile & mf);
		void						UnloadAll();
		void						RegisterFunction(String strFunctionName, lua_CFunction pfnFunction, int iParameterCount, String strFunctionTemplate);
		void						RegisterClass(ScriptClassDecl * pClassDeclaration);
		void						RegisterConstant(String strConstantName, CScriptArgument value);
		void						RegisterDefaultConstants();
		CScript					 *	Get(CMetaFile & mf);
		CScript					 *	Get(lua_State * pVM);
		std::list<CScript *>	 *	GetScriptList() { return &m_scripts; }
		unsigned int				GetScriptCount() { return m_scripts.size(); }
		String						Refresh(std::vector<CMetaFile> & scripts, CScriptTimerManager * stm);
};
