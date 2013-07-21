#pragma once

#include <math.h>
#include "../../Vendor/Lua/lua.hpp"
#include "../CLogFile.h"
#include "../Common.h"

#define SCRIPT_TYPE_NULL 1
#define SCRIPT_TYPE_BOOL 2
#define SCRIPT_TYPE_INTEGER 3
#define SCRIPT_TYPE_FLOAT 4
#define SCRIPT_TYPE_STRING 5
#define SCRIPT_TYPE_TABLE 6
#define SCRIPT_TYPE_FUNCTION 7
#define SCRIPT_TYPE_DATA 8

static double round(double number)
{
	return number < 0.0 ? ceil(number - 0.5) : floor(number + 0.5);
}

// https://github.com/fjolnir/LuaCococa/blob/master/etc/lua_isinteger.c
static int lua_isinteger(lua_State *L, int idx)
{
	lua_Number the_number = lua_tonumber(L, idx);
	double rounded_number = round(the_number);
	if( fabs(the_number-rounded_number) <= FLT_EPSILON )
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

static int script_gettype(lua_State * pVM, int idx)
{
	int lua_t = lua_type(pVM, idx);

	switch(lua_t)
	{
		case LUA_TNIL:
			return SCRIPT_TYPE_NULL;// Nothing needed
		case LUA_TNUMBER:
			if(lua_isinteger(pVM, idx))
			{
				return SCRIPT_TYPE_INTEGER;
			}
			else
			{
				return SCRIPT_TYPE_FLOAT;
			}
		case LUA_TBOOLEAN:
			return SCRIPT_TYPE_BOOL;
		case LUA_TSTRING:
			return SCRIPT_TYPE_STRING;
		case LUA_TTABLE:
			return SCRIPT_TYPE_TABLE;
		case LUA_TFUNCTION:
			return SCRIPT_TYPE_FUNCTION;
		default:
			return 0;
	}
}

#define SCRIPT_FUNCTION(name) int script_##name(lua_State * pVM)

// TODO: PARAM_ERROR define, Then CHECK_PARAMS should use the PARAM_ERROR define
#define CHECK_PARAMS(szNativeName, iAmount) \
	{ \
		int iTop = lua_gettop(pVM); \
		if(iTop != iAmount) \
		{ \
			CLogFile::Printf("Invalid parameter count for function " szNativeName " (Expected: %d, Amount: %d).", iAmount, iTop); \
			return 0; \
		} \
	}

#define CHECK_PARAMS_MIN(szNativeName, iMinAmount) \
	{ \
		int iTop = lua_gettop(pVM); \
		if(iTop < iMinAmount) \
		{ \
			CLogFile::Printf("Invalid parameter count for function " szNativeName " (Expected: %d+, Amount: %d).", iMinAmount, iTop); \
			return 0; \
		} \
	}

#define CHECK_PARAMS_MIN_MAX(szNativeName, iMinAmount, iMaxAmount) \
	{ \
		int iTop = lua_gettop(pVM)/* - 1*/; \
		if(iTop < iMinAmount || iTop > iMaxAmount) \
		{ \
			CLogFile::Printf("Invalid parameter count for function " szNativeName " (Expected: %d+, Amount: %d).", iMinAmount, iTop); \
			return 0; \
		} \
	}

// TODO: TYPE_ERROR define, Then CHECK_TYPE should use the TYPE_ERROR define
// TODO: Give the type required and the type given in the log message (Types should be a string)
#define CHECK_TYPE(szNativeName, iParam, iIndex, iTypeRequired) \
	{ \
		int iType = script_gettype(pVM, iIndex); \
		if(iType != iTypeRequired) \
		{ \
			if(!(iTypeRequired == LUA_TBOOLEAN && iType == LUA_TNUMBER)/* && !(iTypeRequired == OT_FLOAT && iType == OT_INTEGER)*/) /* Allow integers for booleans and integers for floats */ \
			{ \
				CLogFile::Printf("Invalid parameter type for function " szNativeName " parameter %d.", iParam); \
				return 0; \
			} \
		} \
	}

#define script_gettop(vm)					lua_gettop(vm)
#define script_getinteger(vm, idx, pi)		*(pi) = lua_tointeger(vm, idx)
#define script_getbool(vm, idx, pb)			*(pb) = (lua_toboolean(vm, idx) != 0)
#define script_getfloat(vm, idx, pf)		*(pf) = (float)lua_tonumber(vm, idx)
#define script_getstring(vm, idx, pps)		*(pps) = lua_tostring(vm, idx)

#define script_pushnull(vm)					lua_pushnil(vm)
#define script_pushinteger(vm, i)			lua_pushinteger(vm, i)
#define script_pushfloat(vm, f)				lua_pushnumber(vm, f)
#define script_pushbool(vm, b)				lua_pushboolean(vm, b)
#define script_pushstring(vm, s)			lua_pushstring(vm, s)
#define script_pushlstring(vm, s, len)		lua_pushlstring(vm, s, len)

#define script_newtable(vm, len)			lua_createtable(vm, len, 0)
#define script_tableset(vm, i)				lua_settable(vm, i)

//#define script_newarray(vm, len)			lua_createtable(vm, len, 0)
//#define script_arrayappend(vm, ti)			lua_rawseti(vm, ti, -1)

// Zero based array helper
class CScriptArray
{
	public:
		CScriptArray(lua_State * pVM)
		{
			m_Counter = 0;
			m_VM = pVM;
			lua_createtable(m_VM, 0, 0);
		}

		void push(const unsigned int v)
		{
			lua_pushinteger(m_VM, m_Counter);
			lua_pushinteger(m_VM, v);
			lua_settable(m_VM, -3);
			m_Counter++;
		}

		void push(const int v)
		{
			lua_pushinteger(m_VM, m_Counter);
			lua_pushinteger(m_VM, v);
			lua_settable(m_VM, -3);
			m_Counter++;
		}

		void push(const float v)
		{
			lua_pushinteger(m_VM, m_Counter);
			lua_pushnumber(m_VM, v);
			lua_settable(m_VM, -3);
			m_Counter++;
		}

		void PushBool(const bool v)
		{
			lua_pushinteger(m_VM, m_Counter);
			lua_pushboolean(m_VM, v);
			lua_settable(m_VM, -3);
			m_Counter++;
		}

		void push(const String & v, size_t len)
		{
			lua_pushinteger(m_VM, m_Counter);
			lua_pushlstring(m_VM, v.Get(), len);
			lua_settable(m_VM, -3);
			m_Counter++;
		}

	private:
		lua_State *	m_VM;
		int			m_Counter;
};

static int script_throwerror(lua_State * pVM, const char * msg)
{
	lua_pushstring(pVM, msg);
	lua_error(pVM);
	return -1;
}

static void script_pushentity(lua_State * pVM, EntityId entity)
{
	lua_pushinteger(pVM, (lua_Integer)entity);
}

static int script_getentity(lua_State * pVM, lua_Integer iIndex, EntityId * entity)
{
	if(!lua_isinteger(pVM, iIndex))
	{
		return LUA_ERRERR;
	}

	lua_Integer i = lua_tointeger(pVM, iIndex);

	if(entity)
		*entity = (EntityId)i;

	return LUA_OK;
}

static int script_getvector3(lua_State * pVM, int iIndex, CVector3 * pVector)
{
	pVector->fX = (float)lua_tonumber(pVM, iIndex);
	pVector->fY = (float)lua_tonumber(pVM, iIndex + 1);
	pVector->fZ = (float)lua_tonumber(pVM, iIndex + 2);
	return LUA_OK;
}

/*
static void script_pusharg(lua_State * pVM, CScriptArgument arg)
{
	arg.push(pVM);
}

static void script_getarg(lua_State * pVM, int iIndex, CScriptArgument * arg)
{
	arg->pushFromStack(pVM, iIndex);
}
*/