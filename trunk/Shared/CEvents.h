//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CEvents.h
// Project: Shared
// Author(s): mabako
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include <map>
#include <list>

#include <Scripting/CScriptingManager.h>
#include "CString.h"

// FIXUPDATE
// jenksta: this is kinda hacky :/
#ifdef _SERVER
	#include "../Server/Core/Main.h"
	#include "../Server/Core/Interfaces/CEventsInterface.h"
#endif

class CEventHandler
{
	public:
/*
		virtual lua_State *    GetScript() const	= 0;
		virtual int		       GetFunction() const	= 0;
		virtual void *         GetChunk() const 	= 0;
		virtual void *         GetPointer() const 	= 0;*/

	
	#ifdef _SERVER
		virtual EventHandler_t GetHandler()  { return 0; };
	#endif
	
		virtual bool equals(CEventHandler* other) = 0;
		virtual void Call(CScriptArguments* pArguments, CScriptArgument* pReturn) = 0;

		virtual ~CEventHandler(){}
};

// Event Handlers
class CScriptEventHandler : public CEventHandler
{
		lua_State * m_pVM;
		int			m_FunctionRef;
		void *		m_Pointer;
		String		m_WhereWasCalled; // for logging and debug purposes

	public:
		CScriptEventHandler(lua_State * pVM, int functionRef, void * pointer, const String & whereWasCalled)
		{
			m_pVM = pVM;
			m_FunctionRef = functionRef;
			m_WhereWasCalled = whereWasCalled;
			m_Pointer = pointer;
		}
		
		lua_State *			GetScript() { return m_pVM; }
		int		 			GetFunction() { return m_FunctionRef; }
		void *				GetPointer() { return m_Pointer; }

		bool equals(CEventHandler* other)
		{
			auto o = dynamic_cast<CScriptEventHandler *>(other);
			return  o != NULL && o->GetScript() && o->GetScript() == GetScript() && o->GetPointer() == m_Pointer;
		}

		void Call(CScriptArguments* pArguments, CScriptArgument* pReturn)
		{
			// Check if the VM does exist
			CScript* pScript = CScriptingManager::GetInstance()->Get(m_pVM); 
			if(pScript)
				pScript->Call(m_FunctionRef, pArguments, pReturn, m_WhereWasCalled);
		}
};

#ifdef _SERVER
// Module Event Handlers
class CModuleEventHandler : public CEventHandler
{
		EventHandler_t	m_pfnHandler;
		void *			m_pChunk;

	public:
		CModuleEventHandler(EventHandler_t pfnHandler, void* pChunk)
		{
			m_pfnHandler = pfnHandler;
			m_pChunk = pChunk;
		}

		void * GetChunk() { return m_pChunk; }

		bool equals(CEventHandler* other) 
		{
			auto o = dynamic_cast<CModuleEventHandler *>(other);
			return o != NULL && o->GetHandler() && o->GetHandler() == GetHandler() && o->GetChunk() == GetChunk();
		}

		void Call(CScriptArguments* pArguments, CScriptArgument* pReturn)
		{
			m_pfnHandler(pArguments, pReturn, m_pChunk);
		}
};
#endif

class CEvents : public std::map< String, std::list< CEventHandler* > >
#ifdef _SERVER
	, public CEventsInterface
#endif
{
	private:
		static CEvents * m_pInstance;

	public:
		String m_CurrentEventName;

		CEvents()
		{
			// Set our instance
			m_pInstance = this;
		}

		~CEvents()
		{
			clear();

			// Reset our instance
			m_pInstance = NULL;
		}

		static CEvents * GetInstance() { return m_pInstance; }

		bool Add(String strName, CEventHandler* pEventHandler)
		{
			// Any events with that name?
			CEvents::iterator iter = find(strName);
			if(iter != end())
			{
				// Event exists, check if the function is registered already
				for(std::list< CEventHandler* >::iterator iter2 = (*iter).second.begin(); iter2 != (*iter).second.end(); ++ iter2)
				{
					if(pEventHandler->equals(*iter2))
						return false;
				}
			}
			else
			{
				// new - create the event
				std::pair< std::map< String, std::list< CEventHandler* > >::iterator, bool > ret;
				ret = insert(std::pair< String, std::list< CEventHandler* > >( strName, std::list< CEventHandler* >( ) ) );

				iter = ret.first;
			}

			// insert the handler
			(*iter).second.push_back(pEventHandler);
			return true;
		}

		bool Remove(String strName, CEventHandler* pEventHandler)
		{
			// Any events with that name?
			CEvents::iterator iter = find(strName);
			if(iter != end())
			{
				// Check if it exists, if so remove it
				bool bRemoved = false;
				for(std::list< CEventHandler* >::iterator iter2 = (*iter).second.begin(); iter2 != (*iter).second.end(); )
				{
					if(pEventHandler->equals(*iter2))
					{
						(*iter).second.erase(iter2++);
						bRemoved = true;
						break;
					}
					else
						iter2 ++;
				}

				if(bRemoved)
				{
					// remove the event list if it is empty
					if((*iter).second.size() == 0)
					{
						erase(iter);
					}
				}

				return bRemoved;
			}

			// no such event - can't remove handlers
			return false;
		}

		CScriptEventHandler * FindScriptHandler(String strName, void * pointer)
		{
			// Any events with that name?
			auto i = find(strName);
			if(i != end())
			{
				for(auto j = i->second.begin(); j != i->second.end(); j++)
				{
					auto o = dynamic_cast<CScriptEventHandler *>(*j);
					if(o != NULL && o->GetPointer() == pointer)
					{
						return o;
					}
				}
			}

			// no such event 
			return NULL;
		}

		bool RemoveScript(lua_State * pVM)
		{
			for(auto i = begin(); i != end(); )
			{
				for(auto j = (*i).second.begin(); j != (*i).second.end(); )
				{
					auto o = dynamic_cast<CScriptEventHandler *>(*j);
					if(o != NULL && o->GetScript() == pVM)
						(*i).second.erase(j++);
					else
						j ++;
				}

				// remove events with no handlers
				if((*i).second.size() == 0)
					erase(i++);
				else
					i++;
			}

			return true;
		}

		bool IsEventRegistered(String eventName)
		{
			CEvents::iterator iter = find(eventName);
			return iter != end();
		}

	#ifdef _SERVER
		bool AddModuleEvent(const char* szName, EventHandler_t pfnHandler, void* pChunk = 0)
		{
			return Add(szName, new CModuleEventHandler(pfnHandler, pChunk));
		}

		bool RemoveModuleEvent(const char* szName, EventHandler_t pfnHandler, void* pChunk = 0)
		{
			return Remove(szName, &CModuleEventHandler(pfnHandler, pChunk));
		}

		void CallModuleEvent(const char* szName, ScriptArgumentsInterface* pArguments, ScriptArgumentInterface* pReturn)
		{
			Call(szName, reinterpret_cast<CScriptArguments*>(pArguments), reinterpret_cast<CScriptArgument*>(pReturn));
		}

		const char * GetCurrentEvent()
		{
			return m_CurrentEventName.Get();
		}

	#endif

		CScriptArgument Call(String strName, CScript* pScript = NULL)
		{
			CScriptArgument pReturn((int)1);
			Call(strName, &CScriptArguments(), &pReturn, pScript);
			return pReturn;
		}

		CScriptArgument Call(String strName, CScriptArguments* pArguments, CScript* pScript = NULL)
		{
			CScriptArgument pReturn((int)1);
			Call(strName, pArguments, &pReturn, pScript);
			return pReturn;
		}

		void Call(String strName, CScriptArguments* pArguments, CScriptArgument* pReturn, CScript* pScript = NULL)
		{
			m_CurrentEventName = strName;
			lua_State * pVM = pScript ? pScript->GetVM() : NULL;

			// Any events with that name?
			CEvents::iterator i = find(strName);
			if(i != end())
			{
				// loop through all handlers
				for(auto j = i->second.begin(); j != i->second.end(); ++ j)
				{
					// not for a specific script; or that script is the one we want
					auto o = dynamic_cast<CScriptEventHandler *>(*j);
					if(!pVM || (o != NULL && pVM == o->GetScript()))
					{
						(*j)->Call(pArguments, pReturn);
					}
				}
			}
	}
};
