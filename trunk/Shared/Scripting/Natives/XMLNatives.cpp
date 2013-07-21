//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: XMLNatives.cpp
// Project: Shared
// Author(s): Einstein
// License: See LICENSE in root directory
//
//==============================================================================

#include "Natives.h"
#include "../../CXML.h"
#include "../CScriptingManager.h"
#include <SharedUtility.h>

// XML
_BEGIN_CLASS(xml)
_MEMBER_FUNCTION(xml, constructor, 1, "s")
_MEMBER_FUNCTION(xml, save, 0, NULL)
_MEMBER_FUNCTION(xml, setTabSize, 1, "i")
_MEMBER_FUNCTION(xml, nodeAttribute, 1, "s")
_MEMBER_FUNCTION(xml, nodeRemoveAttribute, 1, "s")
_MEMBER_FUNCTION(xml, nodeSetAttribute, 2, "ss")
_MEMBER_FUNCTION(xml, nodeRoot, 0, NULL)
_MEMBER_FUNCTION(xml, nodeNext, 0, NULL)
_MEMBER_FUNCTION(xml, nodePrevious, 0, NULL)
_MEMBER_FUNCTION(xml, nodeClear, 0, NULL)
_MEMBER_FUNCTION(xml, nodeNew, 2, "bs")
_MEMBER_FUNCTION(xml, nodeFind, 1, "s")
_MEMBER_FUNCTION(xml, nodeFirstChild, 0, NULL)
_MEMBER_FUNCTION(xml, nodeParent, 0, NULL)
_MEMBER_FUNCTION(xml, nodeName, 0, NULL)
_MEMBER_FUNCTION(xml, nodeContent, 0, NULL)
_MEMBER_FUNCTION(xml, nodeSetContent, 1, "s")
_MEMBER_FUNCTION(xml, commentNew, 1, "s")
_MEMBER_FUNCTION(xml, lastError, 0, NULL)
_MEMBER_FUNCTION(xml, isComment, 0, NULL)
_END_CLASS(xml)

void RegisterXMLNatives(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterClass(&_CLASS_DECL(xml));
}

_MEMBER_FUNCTION_DESTRUCTOR(xml)
{
/*
	CXML * pXML = (CXML *)pInst;
	delete pXML;
	return 1;*/
	CXML * pXML = script_getinstance<CXML *>(pVM);
	delete pXML;
	return 0;
}

_MEMBER_FUNCTION_IMPL(xml, constructor)
{
	const char * filename;
	script_getstring(pVM, -1, &filename);

	if(filename)
	{
		CXML * pXML = new CXML();
		String strFileName(filename);
		SharedUtility::RemoveIllegalCharacters(strFileName);
		String strPath(SharedUtility::GetAbsolutePath("files/%s", strFileName.Get()));
		pXML->load(strPath);

		if(!pXML || script_setinstance(pVM, pXML, &_CLASS_DECL(xml)) != 0 )
		{
			CLogFile::Print("Failed to load the xml.");
			SAFE_DELETE(pXML);
			script_pushnull(pVM);
			return 1;
		}

		//_SET_RELEASE_HOOK(xml);
		//script_pushbool(pVM, true);
		return 1;
	}

	script_pushnull(pVM);
	return 1;
}

_MEMBER_FUNCTION_IMPL(xml, save)
{
	CXML * pXML = script_getinstance<CXML *>(pVM);

	if(!pXML)
	{
		CLogFile::Print("Failed to get the XML instance.");
		script_pushbool(pVM, false);
		return 1;
	}

	script_pushbool(pVM, pXML->save());
	return 1;
}

_MEMBER_FUNCTION_IMPL(xml, setTabSize)
{
	CXML * pXML = script_getinstance<CXML *>(pVM);

	if(!pXML)
	{
		CLogFile::Print("Failed to get the XML instance.");
		script_pushbool(pVM, false);
		return 1;
	}

	int tabSize;
	script_getinteger(pVM, -1, &tabSize);

	pXML->setTabSize(tabSize);
	script_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(xml, nodeAttribute)
{
	CXML * pXML = script_getinstance<CXML *>(pVM);

	if(!pXML)
	{
		CLogFile::Print("Failed to get the XML instance.");
		script_pushbool(pVM, false);
		return 1;
	}

	const char * attributeName;
	script_getstring(pVM, -1, &attributeName);
	const char * attribute = pXML->getAttribute(attributeName);
	if(attribute != NULL)
		script_pushlstring(pVM, attribute, strlen(attribute));
	else
		script_pushnull(pVM);
	return 1;
}

_MEMBER_FUNCTION_IMPL(xml, nodeRemoveAttribute)
{
	CXML * pXML = script_getinstance<CXML *>(pVM);

	if(!pXML)
	{
		CLogFile::Print("Failed to get the XML instance.");
		script_pushbool(pVM, false);
		return 1;
	}

	const char * attributeName;
	script_getstring(pVM, -1, &attributeName);
	pXML->removeAttribute(attributeName);
	script_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(xml, nodeSetAttribute)
{
	CXML * pXML = script_getinstance<CXML *>(pVM);

	if(!pXML)
	{
		CLogFile::Print("Failed to get the XML instance.");
		script_pushbool(pVM, false);
		return 1;
	}

	const char * attributeName;
	const char * attributeContent;
	script_getstring(pVM, -1, &attributeContent);
	script_getstring(pVM, -2, &attributeName);

	pXML->setAttribute(attributeName, attributeContent);
	script_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(xml, nodeRoot)
{
	CXML * pXML = script_getinstance<CXML *>(pVM);

	if(!pXML)
	{
		CLogFile::Print("Failed to get the XML instance.");
		script_pushbool(pVM, false);
		return 1;
	}
	pXML->nodeToRoot();

	script_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(xml, nodeNext)
{
	CXML * pXML = script_getinstance<CXML *>(pVM);

	if(!pXML)
	{
		CLogFile::Print("Failed to get the XML instance.");
		script_pushbool(pVM, false);
		return 1;
	}

	script_pushbool(pVM, pXML->nextNode());
	return 1;
}

_MEMBER_FUNCTION_IMPL(xml, nodePrevious)
{
	CXML * pXML = script_getinstance<CXML *>(pVM);

	if(!pXML)
	{
		CLogFile::Print("Failed to get the XML instance.");
		script_pushbool(pVM, false);
		return 1;
	}

	script_pushbool(pVM, pXML->previousNode());
	return 1;
}

_MEMBER_FUNCTION_IMPL(xml, nodeClear)
{
	CXML * pXML = script_getinstance<CXML *>(pVM);

	if(!pXML)
	{
		CLogFile::Print("Failed to get the XML instance.");
		script_pushbool(pVM, false);
		return 1;
	}
	pXML->nodeClear();

	script_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(xml, nodeNew) 
{
	CXML * pXML = script_getinstance<CXML *>(pVM);

	if(!pXML)
	{
		CLogFile::Print("Failed to get the XML instance.");
		script_pushnull(pVM);
		return 1;
	}
	
	bool linkNewXml;
	const char * XmlName;

	script_getbool(pVM, -2, &linkNewXml);
	script_getstring(pVM, -1, &XmlName);

#pragma warning(disable:4800)
	bool linkXml = (bool)linkNewXml;

	pXML->newNode(XmlName, linkXml);
	//script_pushbool(pVM, true);
	return 1;

}

_MEMBER_FUNCTION_IMPL(xml, nodeFind)
{
	CXML * pXML = script_getinstance<CXML *>(pVM);

	if(!pXML)
	{
		CLogFile::Print("Failed to get the XML instance.");
		script_pushbool(pVM, false);
		return 1;
	}

	const char * nodeName;
	script_getstring(pVM, -1, &nodeName);

	script_pushbool(pVM, pXML->findNode(nodeName));
	return 1;

}

_MEMBER_FUNCTION_IMPL(xml, nodeFirstChild)
{
	CXML * pXML = script_getinstance<CXML *>(pVM);

	if(!pXML)
	{
		CLogFile::Print("Failed to get the XML instance.");
		script_pushbool(pVM, false);
		return 1;
	}

	script_pushbool(pVM, pXML->childNodeFirst());
	return 1;
}

_MEMBER_FUNCTION_IMPL(xml, nodeParent)
{
	CXML * pXML = script_getinstance<CXML *>(pVM);

	if(!pXML)
	{
		CLogFile::Print("Failed to get the XML instance.");
		script_pushbool(pVM, false);
		return 1;
	}

	pXML->nodeParent();

	script_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(xml, nodeName)
{
	CXML * pXML = script_getinstance<CXML *>(pVM);

	if(!pXML)
	{
		CLogFile::Print("Failed to get the XML instance.");
		script_pushnull(pVM);
		return 1;
	}

	const char * name = pXML->nodeName();
	if(name == 0) script_pushnull(pVM);
	else script_pushlstring(pVM, name, strlen(name));
	return 1;
}

_MEMBER_FUNCTION_IMPL(xml, nodeContent)
{
	CXML * pXML = script_getinstance<CXML *>(pVM);

	if(!pXML)
	{
		CLogFile::Print("Failed to get the XML instance.");
		script_pushnull(pVM);
		return 1;
	}

	const char * content = pXML->nodeContent();
	if(content == 0) script_pushnull(pVM);
	else script_pushlstring(pVM, content, strlen(content));
	return 1;
}

_MEMBER_FUNCTION_IMPL(xml, nodeSetContent)
{
	CXML * pXML = script_getinstance<CXML *>(pVM);

	if(!pXML)
	{
		CLogFile::Print("Failed to get the XML instance.");
		script_pushbool(pVM, false);
		return 1;
	}

	const char * content;
	script_getstring(pVM, -1, &content);

	pXML->nodeSetContent(content);

	script_pushbool(pVM, true);
	return 1;

}

_MEMBER_FUNCTION_IMPL(xml, commentNew)
{
	CXML * pXML = script_getinstance<CXML *>(pVM);

	if(!pXML)
	{
		CLogFile::Print("Failed to get the XML instance.");
		script_pushbool(pVM, false);
		return 1;
	}

	const char * val;
	script_getstring(pVM, -1, &val);

	pXML->newComment(val);

	script_pushbool(pVM, true);
	return 1;

}

_MEMBER_FUNCTION_IMPL(xml, lastError)
{
	CXML * pXML = script_getinstance<CXML *>(pVM);

	if(!pXML)
	{
		CLogFile::Print("Failed to get the XML instance.");
		script_pushnull(pVM);
		return 1;
	}

	const char * error = pXML->lastError(0, 0);
	if(error == 0) script_pushnull(pVM);
	else script_pushlstring(pVM, error, strlen(error));
	return 1;
}

_MEMBER_FUNCTION_IMPL(xml, isComment)
{
	CXML * pXML = script_getinstance<CXML *>(pVM);

	if(!pXML)
	{
		CLogFile::Print("Failed to get the XML instance.");
		script_pushbool(pVM, false);
		return 1;
	}

	script_pushbool(pVM, pXML->isComment());
	return 1;
}
