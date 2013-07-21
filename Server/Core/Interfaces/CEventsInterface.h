//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CEventsInterface.h
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "InterfaceCommon.h"
//#include "../../../Vendor/Lua/lua.hpp"

//class ScriptArgumentsInterface;

class ScriptArgumentInterface
{
public:
	/* Returns the argument's type */
	virtual int			 GetType()    const = 0;

	/* Retrieves the argument as either of these types */
	        bool         IsNull()     const { return GetType() == 0; }
	virtual int          GetInteger() const = 0;
	virtual bool         GetBool()    const = 0;
	virtual float        GetFloat()   const = 0;
	virtual const char * GetString()  const = 0;

	/* Retrieves a list with all array elements */
	//virtual ScriptArgumentsInterface* GetArray() = 0;

	/* Retrieves a list with all table entries
			t = { a = 3, b = 5 }
		will result in
			t = ...->GetTable()
			t.Get(0).GetString() = a
			t.Get(1).GetInteger() = 3
			t.Get(2).GetString() = b
			t.Get(3).GetInteger() = 5
		etc. - so even indices are the table keys, odd indices are the values
	*/
	//virtual ScriptArgumentsInterface* GetTable() = 0;

	/* Set various different stuffs */
	virtual void SetNull   ()              = 0;
	virtual void SetInteger(int i)         = 0;
	virtual void SetBool   (bool b)        = 0;
	virtual void SetFloat  (float f)       = 0;
	virtual void SetString (const char* s) = 0;

	//virtual void SetArray  (ScriptArgumentsInterface* p) = 0;
	//virtual void SetTable  (ScriptArgumentsInterface* p) = 0;
};

class ScriptArgumentsInterface
{
public:
	/* Retrieves the i-th parameter or returns 0 if none */
	virtual ScriptArgumentInterface* Get(unsigned int i) const = 0;

	/* Retrieves the number of arguments */
	virtual unsigned int GetSize() const = 0;

	/* Adds an element at the back (which is null) */
	virtual ScriptArgumentInterface* Add() = 0;

	/* Removes the last element */
	virtual void Remove() = 0;
};

class ScriptArgumentManager
{
public:
	/* Creates and deletes argument(s) - should ONLY ever use this on your own argument stuff, e.g. for calling events */
	virtual ScriptArgumentInterface* CreateArgument() const = 0;
	virtual ScriptArgumentsInterface* CreateArguments() const = 0;

	virtual void DeleteArgument(ScriptArgumentInterface* p) const = 0;
	virtual void DeleteArguments(ScriptArgumentsInterface* p) const = 0;
};

typedef void (* EventHandler_t)(ScriptArgumentsInterface * pArguments, ScriptArgumentInterface * pReturn, void * pChunk);

class CEventsInterface
{
public:
	virtual bool AddModuleEvent(const char* szName, EventHandler_t pfnHandler, void* pChunk = 0) = 0;
	virtual bool RemoveModuleEvent(const char* szName, EventHandler_t pfnHandler, void* pChunk = 0) = 0;
	virtual void CallModuleEvent(const char* szName, ScriptArgumentsInterface* pArguments, ScriptArgumentInterface* pReturn) = 0;
	virtual const char* GetCurrentEvent() = 0;
};
