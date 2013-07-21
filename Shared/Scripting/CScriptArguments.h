//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CScriptArguments.h
// Project: Server.Core
// Author(s): mabako
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include <list>
#include <assert.h>
// FIXUPDATE
// jenksta: this is very hacky :/
#ifdef _SERVER
	#include "../../Server/Core/Interfaces/CEventsInterface.h"
#endif
#include "../Network/CBitStream.h"
#include "../../Vendor/Lua/lua.hpp"
#include "Lua.h"

class CScriptArguments;

class CScriptArgument
#ifdef _SERVER
	: public ScriptArgumentInterface
#endif
{
	int type;
public:
	union
	{
		int i;
		bool b;
		float f;
		String * str;
		CScriptArguments * pTable;
		lua_CFunction	pCFunction;
		void * pUserData;
		//SQObject sqObject;
		//SQInstance * pInstance;
	} data;

	enum
	{
		NET_SQ_INVALID,
		NET_SQ_NULL,
		NET_SQ_BYTE_POS,
		NET_SQ_BYTE_NEG,
		NET_SQ_SHORT_POS,
		NET_SQ_SHORT_NEG,
		NET_SQ_INT,
		NET_SQ_BOOL_TRUE,
		NET_SQ_BOOL_FALSE,
		NET_SQ_FLOAT,
		NET_SQ_STRING_TINY,
		NET_SQ_STRING_SMALL,
		NET_SQ_STRING,
		NET_SQ_TABLE,
		//NET_SQ_ARRAY,
		NET_SQ_MAX
	};

	CScriptArgument() { type = SCRIPT_TYPE_NULL; }
	CScriptArgument(int i) { type = SCRIPT_TYPE_INTEGER; data.i = i; }
	CScriptArgument(bool b) { type = SCRIPT_TYPE_BOOL; data.b = b; }
	CScriptArgument(float f) { type = SCRIPT_TYPE_FLOAT; data.f = f; }
	CScriptArgument(String str) { type = SCRIPT_TYPE_STRING; data.str = new String(str); }
	CScriptArgument(String* str) { type = SCRIPT_TYPE_STRING; data.str = str; }
	CScriptArgument(const CScriptArguments &array);
	CScriptArgument(CScriptArguments * pArray) { type = SCRIPT_TYPE_TABLE; data.pTable = pArray; }
	//CScriptArgument(SQObject o);
	CScriptArgument(void * pInstance) { type = SCRIPT_TYPE_DATA; data.pUserData = pInstance; }
	CScriptArgument(CBitStream * pBitStream);
	CScriptArgument(const CScriptArgument& p);
	~CScriptArgument();

	int				     GetType() const { return type; }
	String               GetTypeString() const;

	void                 reset();

	bool                 push(lua_State * pVM);
	bool                 pushFromStack(lua_State * pVM, int idx);

	void                 serialize(CBitStream * pBitStream);
	void                 deserialize(CBitStream * pBitStream);

	void                 set(const CScriptArgument& p);
	void                 SetNull()                 { reset(); type = SCRIPT_TYPE_NULL; }
	void                 SetInteger(int i)         { reset(); type = SCRIPT_TYPE_INTEGER; data.i = i; }
	void                 SetBool   (bool b)        { reset(); type = SCRIPT_TYPE_BOOL; data.b = b; }
	void                 SetFloat  (float f)       { reset(); type = SCRIPT_TYPE_FLOAT; data.f = f; }
	void                 SetString (const char* s) { reset(); type = SCRIPT_TYPE_STRING; data.str = new String(s); }
	//void                 SetArray(CScriptArguments * pArray) { reset(); type = SCRIPT_TYPE_ARRAY; data.pTable = pArray; }
	void                 SetTable(CScriptArguments * pTable) { reset(); type = SCRIPT_TYPE_TABLE; data.pTable = pTable; }
//	void                 SetInstance(SQInstance * pInstance) { reset(); type = OT_INSTANCE; data.pInstance = pInstance; }

	int                  GetInteger() const { return (type == SCRIPT_TYPE_INTEGER) ? data.i : 0; }
	bool                 GetBool()    const { return (type == SCRIPT_TYPE_BOOL)    ? data.b : false; }
	float                GetFloat()   const { return (type == SCRIPT_TYPE_FLOAT)   ? data.f : 0.0f; }
	const char         * GetString()  const { return (type == SCRIPT_TYPE_STRING)  ? data.str->Get() : NULL; }
	CScriptArguments * GetTable() const { return (type == SCRIPT_TYPE_TABLE) ? data.pTable : NULL; }
	void		       * GetInstance() const { return (type == SCRIPT_TYPE_DATA) ? data.pUserData : NULL; }
};

class CScriptArguments : public std::list<CScriptArgument *>
#ifdef _SERVER
	, public ScriptArgumentsInterface
#endif
{
public:
	CScriptArguments(){}
	CScriptArguments(lua_State * pVM, int idx);
	CScriptArguments(CBitStream * pBitStream);
	CScriptArguments(const CScriptArguments& p);
	~CScriptArguments();

	void reset();

	void push_to_vm(lua_State * pVM);

	void push();
	//void pushObject(SQObject o);
	void push(int i);
	void push(bool b);
	void push(float f);
	void push(const char* c);
	void push(String str);
	void push(const CScriptArgument &arg);
	void push(const CScriptArguments &array);
	void push(CScriptArguments* pArray);
	void pushVector3(const CVector3 &vec3);
	bool pushFromStack(lua_State* pVM, int idx);

	CScriptArgument pop();
	bool popVector3(CVector3 &vec3);

	void serialize(CBitStream * pBitStream);
	void deserialize(CBitStream * pBitStream);

#ifdef _SERVER
	ScriptArgumentInterface* Get(unsigned int i) const;
	unsigned int GetSize() const { return size(); }

	ScriptArgumentInterface* Add() { push(); return back(); }
	void Remove() { pop_back(); }
#endif
};

#ifdef _SERVER
class CScriptArgumentManager : public ScriptArgumentManager
{
	ScriptArgumentInterface *  CreateArgument() const { return new CScriptArgument; }
	ScriptArgumentsInterface * CreateArguments() const { return new CScriptArguments; }

	void                         DeleteArgument(ScriptArgumentInterface* p) const { delete reinterpret_cast<CScriptArgument *>(p); }
	void                         DeleteArguments(ScriptArgumentsInterface* p) const { delete reinterpret_cast<CScriptArguments *>(p); }
};
#endif
