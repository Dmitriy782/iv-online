//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CScriptArguments.cpp
// Project: Server.Core
// Author(s): mabako
// License: See LICENSE in root directory
//
//==============================================================================

#include "CScriptArguments.h"

CScriptArgument::CScriptArgument(const CScriptArguments &array)
{
	type = SCRIPT_TYPE_TABLE;
	data.pTable = new CScriptArguments(array);
}

CScriptArgument::CScriptArgument(CBitStream * pBitStream)
{
	deserialize(pBitStream);
}

CScriptArgument::CScriptArgument(const CScriptArgument& p)
{
	set(p);
}

CScriptArgument::~CScriptArgument()
{
	reset();
}

String CScriptArgument::GetTypeString() const
{
	switch(type)
	{
		case SCRIPT_TYPE_NULL:
			return "Null";
			break;
		case SCRIPT_TYPE_INTEGER:
			return "Integer";
			break;
		case SCRIPT_TYPE_FLOAT:
			return "Floating Point";
			break;
		case SCRIPT_TYPE_BOOL:
			return "Boolean";
			break;
		case SCRIPT_TYPE_STRING:
			return "String";
			break;
		case SCRIPT_TYPE_FUNCTION:
			return "CFunction";
			break;
		case SCRIPT_TYPE_DATA:
			return "UserData";
			break;
	}

	return "Unknown";
}

void CScriptArgument::reset()
{
	if(type == SCRIPT_TYPE_STRING)
		delete data.str;
	else if(type == SCRIPT_TYPE_TABLE)
		delete data.pTable;

	type = LUA_TNIL;
}

bool CScriptArgument::push(lua_State * pVM)
{
	switch(type)
	{
		case SCRIPT_TYPE_NULL:
			script_pushnull(pVM);
			break;
		case SCRIPT_TYPE_INTEGER:
			script_pushinteger(pVM, data.i);
			break;
		case SCRIPT_TYPE_BOOL:
			script_pushbool(pVM, data.b);
			break;
		case SCRIPT_TYPE_FLOAT:
			script_pushfloat(pVM, data.f);
			break;
		case SCRIPT_TYPE_STRING:
			script_pushlstring(pVM, data.str->Get(), data.str->GetLength());
			break;
/*
		case SCRIPT_TYPE_ARRAY:
			{
				///SQ sq_newarray(pVM, 0);
				lua_newtable(pVM);

				int i = 1;
				for(CScriptArguments::iterator iter = data.pTable->begin(); iter != data.pTable->end(); iter++)
				{
					(*iter)->push(pVM);
					///SQ sq_arrayappend(pVM, -2);
					lua_rawseti(pVM, -2, i++);  / * set table at key `i' * /
				}

				break;
			}*/
		case SCRIPT_TYPE_TABLE:
			{
				assert(data.pTable->size() % 2 == 0);
				lua_newtable(pVM);

				for(CScriptArguments::iterator iter = data.pTable->begin(); iter != data.pTable->end(); iter++)
				{
					(*iter)->push(pVM);
					++iter;
					(*iter)->push(pVM);
					//sq_createslot(pVM, -3);
					lua_settable(pVM, -3);
				}

				break;
			}
			break;
		case SCRIPT_TYPE_FUNCTION:
		//case OT_NATIVECLOSURE:
			//sq_pushobject(pVM, data.sqObject);
			lua_pushcfunction(pVM, data.pCFunction);
			break;
		case SCRIPT_TYPE_DATA:
			{
				//SQObject obj;
				//obj._type = OT_INSTANCE;
				//obj._unVal.pInstance = data.pInstance;
				//sq_pushobject(pVM, obj);
				lua_pushlightuserdata(pVM, data.pUserData);
			}
			break;
		default:
			lua_pushnil(pVM); // whatsoever. do not even care. fix it if you dare.
			assert(0);
			return false;
	}

	return true;
}

bool CScriptArgument::pushFromStack(lua_State * pVM, int idx)
{
	reset();
	///SQ SQObjectPtr obj = stack_get(pVM, idx);
	int lua_t = lua_type(pVM, idx);

	switch(lua_t)
	{
	case LUA_TNIL:
		type = SCRIPT_TYPE_NULL;// Nothing needed
		break;
	case LUA_TNUMBER:
		if(lua_isinteger(pVM, idx))
		{
			script_getinteger(pVM, idx, &data.i);
			type = SCRIPT_TYPE_INTEGER;
		}
		else
		{
			script_getfloat(pVM, idx, &data.f);
			type = SCRIPT_TYPE_FLOAT;
		}
		break;
	case LUA_TBOOLEAN:
		script_getbool(pVM, idx, &data.b);
		type = SCRIPT_TYPE_BOOL;
		break;
	case LUA_TSTRING:
		data.str = new String(lua_tostring(pVM, idx));
		type = SCRIPT_TYPE_STRING;
		break;

	case LUA_TTABLE:
		{
			data.pTable  = new CScriptArguments();

			lua_pushvalue(pVM, idx); // table at -2
			lua_pushnil(pVM);  /* first key = null at -1*/
			
			while(lua_next(pVM, -2) != 0) 
			{
				/* uses 'key' (at index -2) and 'value' (at index -1) */
				data.pTable->pushFromStack(pVM, -2); // key
				data.pTable->pushFromStack(pVM, -1); // value
				lua_pop(pVM, 1);/* removes 'value'; keeps 'key' for next iteration */
			}
			lua_pop(pVM, 1);

/*			sq_push(pVM, idx);
			sq_pushnull(pVM);

			while(SQ_SUCCEEDED(sq_next(pVM, -2)))
			{
				if(!pArguments->pushFromStack(pVM, -2) || !pArguments->pushFromStack(pVM, -1))
				{
					sq_pop(pVM, 4);
					delete pArguments;
					return false;
				}

				sq_pop(pVM, 2);
			}
			sq_pop(pVM, 2);
			data.pArray = pArguments;*/
		}
		break;
	case LUA_TFUNCTION:
		data.pCFunction = lua_tocfunction(pVM, idx);
		type = SCRIPT_TYPE_FUNCTION;
		break;
	case LUA_TUSERDATA:
	case LUA_TLIGHTUSERDATA:
		data.pUserData = lua_touserdata(pVM, idx);
		type = SCRIPT_TYPE_DATA;
		break;
	default:
		return false;
		break;
	}

	return true;
}

void CScriptArgument::serialize(CBitStream * pBitStream)
{
	switch(type)
	{
		case SCRIPT_TYPE_NULL:
		case SCRIPT_TYPE_FUNCTION:
		//case OT_NATIVECLOSURE:
			pBitStream->Write((unsigned char)NET_SQ_NULL);
			break;
		case SCRIPT_TYPE_INTEGER:
			if(data.i >= 0)
			{
				if(data.i <= 0xFF-NET_SQ_MAX)
				{
					pBitStream->Write((unsigned char)(data.i + NET_SQ_MAX));
				}
				else if(data.i < 0x200-NET_SQ_MAX)
				{
					pBitStream->Write((unsigned char)NET_SQ_BYTE_POS);
					pBitStream->Write((unsigned char)(data.i - 0x100 + NET_SQ_MAX));
				}
				else if(data.i < 0x10200-NET_SQ_MAX)
				{
					pBitStream->Write((unsigned char)NET_SQ_SHORT_POS);
					pBitStream->Write((unsigned short)(data.i - 0x10200 + NET_SQ_MAX));
				}
				else
				{
					pBitStream->Write((unsigned char)NET_SQ_INT);
					pBitStream->Write(data.i);
				}
			}
			else
			{
				if(data.i >= -0xFF)
				{
					pBitStream->Write((unsigned char)NET_SQ_BYTE_NEG);
					pBitStream->Write((unsigned char)(-data.i));
				}
				else if(data.i >= -0xFFFF)
				{
					pBitStream->Write((unsigned char)NET_SQ_SHORT_NEG);
					pBitStream->Write((unsigned short)(-data.i));
				}
				else
				{
					pBitStream->Write((unsigned char)NET_SQ_INT);
					pBitStream->Write(data.i);
				}
			}
			break;
		case SCRIPT_TYPE_BOOL:
			pBitStream->Write((unsigned char)(data.b?NET_SQ_BOOL_TRUE:NET_SQ_BOOL_FALSE));
			break;
		case SCRIPT_TYPE_FLOAT:
			pBitStream->Write((unsigned char)NET_SQ_FLOAT);
			pBitStream->Write(data.f);
			break;
		case SCRIPT_TYPE_STRING:
			{
				size_t size = data.str->GetLength();
				if(size <= 0xFF)
				{
					pBitStream->Write((unsigned char)NET_SQ_STRING_TINY);
					pBitStream->Write((unsigned char)size);
				}
				else if(size <= 0xFFFF)
				{
					pBitStream->Write((unsigned char)NET_SQ_STRING_SMALL);
					pBitStream->Write((unsigned short)size);
				}
				else
				{
					pBitStream->Write((unsigned char)NET_SQ_STRING);
					pBitStream->Write(size);
				}

				pBitStream->Write(data.str->C_String(), data.str->GetLength());
			}
			break;
		case SCRIPT_TYPE_TABLE:
			pBitStream->Write((unsigned char)(type == NET_SQ_TABLE));
			data.pTable->serialize(pBitStream);
			break;
		default:
			assert(0 && "Unknown Data Type to Serialize");
	}
}

void CScriptArgument::deserialize(CBitStream * pBitStream)
{
	unsigned char type;
	pBitStream->Read(type);

	if(type >= NET_SQ_MAX)
	{
		this->type = SCRIPT_TYPE_INTEGER;
		data.i = type - NET_SQ_MAX;
	}
	else
	{
		switch(type)
		{
		case NET_SQ_NULL:
			{
				this->type = SCRIPT_TYPE_NULL;
			}
			break;
		case NET_SQ_BYTE_POS:
			{
				this->type = SCRIPT_TYPE_INTEGER;
				unsigned char temp;
				pBitStream->Read(temp);
				data.i = 0x100 - NET_SQ_MAX + temp;
			}
			break;
		case NET_SQ_BYTE_NEG:
			{
				this->type = SCRIPT_TYPE_INTEGER;
				unsigned char temp;
				pBitStream->Read(temp);
				data.i = -(int)temp;
			}
			break;
		case NET_SQ_SHORT_POS:
			{
				this->type = SCRIPT_TYPE_INTEGER;
				unsigned short temp;
				pBitStream->Read(temp);
				data.i = 0x200 - NET_SQ_MAX + temp;
			}
			break;
		case NET_SQ_SHORT_NEG:
			{
				this->type = SCRIPT_TYPE_INTEGER;
				unsigned short temp;
				pBitStream->Read(temp);
				data.i = -(int)temp;
			}
			break;
		case NET_SQ_INT:
			{
				this->type = SCRIPT_TYPE_INTEGER;
				pBitStream->Read(data.i);
			}
			break;
		case NET_SQ_FLOAT:
			{
				this->type = SCRIPT_TYPE_FLOAT;
				pBitStream->Read(data.f);
			}
			break;
		case NET_SQ_BOOL_TRUE:
		case NET_SQ_BOOL_FALSE:
			{
				this->type = SCRIPT_TYPE_BOOL;
				data.b = (type == NET_SQ_BOOL_TRUE);
			}
			break;
		case NET_SQ_STRING_TINY:
		case NET_SQ_STRING_SMALL:
		case NET_SQ_STRING:
			{
				this->type = SCRIPT_TYPE_STRING;
				size_t size = 0;

				if(type == NET_SQ_STRING_TINY)
				{
					unsigned char temp;
					pBitStream->Read(temp);
					size = temp;
				}
				else if(type == NET_SQ_STRING_SMALL)
				{
					unsigned short temp;
					pBitStream->Read(temp);
					size = temp;
				}
				else
					pBitStream->Read(size);

				data.str = new String();
				data.str->Resize(size);
				char * szString = data.str->GetData();
				pBitStream->Read(szString, size);
				data.str->Truncate(size);
			}
			break;
		case NET_SQ_TABLE:
			this->type = SCRIPT_TYPE_TABLE;
			data.pTable = new CScriptArguments(pBitStream);
			break;
/*
		case NET_SQ_ARRAY:
			this->type = SCRIPT_TYPE_ARRAY;
			data.pArray = new CScriptArguments(pBitStream);
			break;
*/
		default:
			assert( 0 && "Invalid Data Type to Unserialize" );
		}
	}
}

void CScriptArgument::set(const CScriptArgument& p)
{
	type = p.type;

	switch(type)
	{
	case SCRIPT_TYPE_NULL:
		break;
	case SCRIPT_TYPE_INTEGER:
		data.i = p.data.i;
		break;
	case SCRIPT_TYPE_BOOL:
		data.b = p.data.b;
		break;
	case SCRIPT_TYPE_FLOAT:
		data.f = p.data.f;
		break;
	case SCRIPT_TYPE_STRING:
		data.str = new String(p.data.str->Get());
		break;
	case SCRIPT_TYPE_TABLE:
		data.pTable = new CScriptArguments(*p.data.pTable);
		break;
	case SCRIPT_TYPE_FUNCTION:
	//case OT_NATIVECLOSURE:
		data.pCFunction = p.data.pCFunction;
		break;
	case SCRIPT_TYPE_DATA:
		data.pUserData = p.data.pUserData;
		break;
	}
}

//==============================================================================

CScriptArguments::CScriptArguments(lua_State * pVM, int idx)
{
	int top = lua_gettop(pVM);

	for(int i = idx; i <= top; i++)
		pushFromStack(pVM, i);
}

CScriptArguments::CScriptArguments(CBitStream * pBitStream)
{
	deserialize(pBitStream);
}

CScriptArguments::CScriptArguments(const CScriptArguments& p)
{
	for(CScriptArguments::const_iterator iter = p.begin(); iter != p.end(); ++ iter)
		push_back(new CScriptArgument(**iter));
}

CScriptArguments::~CScriptArguments()
{
	reset();
}

void CScriptArguments::reset()
{
	for(iterator iter = begin(); iter != end(); iter++)
		delete *iter;

	clear();
}

void CScriptArguments::push_to_vm(lua_State * pVM)
{
	for(iterator iter = begin(); iter != end(); ++ iter)
		(*iter)->push(pVM);
}

void CScriptArguments::push()
{
	push_back(new CScriptArgument());
}

/*
void CScriptArguments::pushObject(SQObject o)
{
	push_back(new CScriptArgument(o));
}*/

void CScriptArguments::push(int i)
{
	push_back(new CScriptArgument(i));
}

void CScriptArguments::push(bool b)
{
	push_back(new CScriptArgument(b));
}

void CScriptArguments::push(float f)
{
	push_back(new CScriptArgument(f));
}

void CScriptArguments::push(const char* c)
{
	push_back(new CScriptArgument(new String(c)));
}

void CScriptArguments::push(String str)
{
	push_back(new CScriptArgument(new String(str)));
}

void CScriptArguments::push(const CScriptArgument &arg)
{
	push_back(new CScriptArgument(arg));
}

void CScriptArguments::push(const CScriptArguments &array)
{
	push_back(new CScriptArgument(array));
}

void CScriptArguments::push(CScriptArguments* pArray)
{
	push_back(new CScriptArgument(pArray));
}

void CScriptArguments::pushVector3(const CVector3 &vec3)
{
	push_back(new CScriptArgument(vec3.fX));
	push_back(new CScriptArgument(vec3.fY));
	push_back(new CScriptArgument(vec3.fZ));
}

bool CScriptArguments::pushFromStack(lua_State* pVM, int idx)
{
	CScriptArgument * arg = new CScriptArgument();

	if(arg->pushFromStack(pVM, idx))
	{
		push_back(arg);
		return true;
	}

	delete arg;
	return false;
}

CScriptArgument CScriptArguments::pop()
{
	// Do we have an argument to pop?
	if(size() > 0)
	{
		// Get an argument from the front
		CScriptArgument * pArgument = front();
		pop_front();

		// Create a new instance of the argument
		CScriptArgument argument(*pArgument);

		// Delete the argument
		delete pArgument;

		// Return the new argument instance
		return argument;
	}

	// Nothing we can do, return a NULL argument
	return CScriptArgument();
}

bool CScriptArguments::popVector3(CVector3 &vec3)
{
	// Do we have 3 arguments to pop?
	if(size() >= 3)
	{
		// Get 3 arguments from the front
		CScriptArgument * pArguments[3];

		for(int i = 0; i < 3; i++)
		{
			pArguments[i] = front();

			// Ensure this argument is a number
			if(pArguments[i]->GetType() != SCRIPT_TYPE_FLOAT && pArguments[i]->GetType() != SCRIPT_TYPE_INTEGER)
				return false;

			pop_front();
		}

		// Set the vector
		vec3.fX = pArguments[0]->GetFloat();
		vec3.fY = pArguments[1]->GetFloat();
		vec3.fZ = pArguments[2]->GetFloat();
		return true;
	}

	// Not enough arguments
	return false;
}

void CScriptArguments::serialize(CBitStream * pBitStream)
{
	size_t size = this->size();

	/*
	First byte:
		00-7f: 1 byte size (0-127)
		80-fe: 2 byte size (including this one)
		ff: 4 byte size (following), 5 bytes in total. Assuming you sent over 32k arguments at once or an array with 32k elements or a table with 16k elements, this wastes one byte of bandwith.
	*/
	if(size < 0x80)
		pBitStream->Write((unsigned char)size);
	else if(size < 0x7f80)
		pBitStream->Write((unsigned short)(size+0x7f80));
	else
	{
		pBitStream->Write((unsigned char)0xff);
		pBitStream->Write(size);
	}

	// --

	for(iterator iter = begin(); iter != end(); ++ iter)
		(*iter)->serialize(pBitStream);
}

void CScriptArguments::deserialize(CBitStream * pBitStream)
{
	size_t size;
	unsigned char temp;
	pBitStream->Read(temp);

	if(temp < 0x80)
		size = temp;
	else if(temp < 0xFF)
	{
		size = temp << 8;
		pBitStream->Read(temp);
		size += temp;
		size -= 0x7f80;
	}
	else
		pBitStream->Read(size);

	for(size_t i = 0; i < size; ++i)
		push_back(new CScriptArgument(pBitStream));
}

#ifdef _SERVER
ScriptArgumentInterface* CScriptArguments::Get(unsigned int i) const
{
	if(i >= size())
		return 0;

	unsigned int j = 0;
	for(const_iterator iter = begin(); iter != end(); ++ iter)
	{
		if(j == i)
			return *iter;
		
		++ j;
	}

	return 0;
}
#endif
