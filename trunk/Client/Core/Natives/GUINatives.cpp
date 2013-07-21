//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: GUINatives.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "../Natives.h"
#include "../CClient.h"
#include <SharedUtility.h>
//#include "../CD3D9Webkit.hpp"

extern CClient * g_pClient;
//extern CD3D9WebKit * g_pWebkit;

// GUIFont
_BEGIN_CLASS(GUIFont)
_MEMBER_FUNCTION(GUIFont, constructor, -1, NULL)
_MEMBER_FUNCTION(GUIFont, getTextExtent, 1, "s")
_MEMBER_FUNCTION(GUIFont, getFontHeight, 0, NULL)
_MEMBER_FUNCTION(GUIFont, drawText, 4, "ffsb")
_END_CLASS(GUIFont)

// GUIElement
_BEGIN_CLASS(GUIElement)
_MEMBER_FUNCTION(GUIElement, constructor, 1, "s")
_MEMBER_FUNCTION(GUIElement, setText, 1, "s")
_MEMBER_FUNCTION(GUIElement, setSize, 3, "ffb")
_MEMBER_FUNCTION(GUIElement, getSize, 0, NULL)
_MEMBER_FUNCTION(GUIElement, setPosition, 3, "ffb")
_MEMBER_FUNCTION(GUIElement, setRotation, 3, "fff")
_MEMBER_FUNCTION(GUIElement, setAlpha, 1, "f")
_MEMBER_FUNCTION(GUIElement, setVisible, 1, "b")
_MEMBER_FUNCTION(GUIElement, setParent, 1, "s")
_MEMBER_FUNCTION(GUIElement, getName, 0, NULL)
_MEMBER_FUNCTION(GUIElement, getText, 0, NULL)
_MEMBER_FUNCTION(GUIElement, setFont, 1, "x")
_MEMBER_FUNCTION(GUIElement, setProperty, 2, "ss")
_MEMBER_FUNCTION(GUIElement, activate, 0, NULL)
_MEMBER_FUNCTION(GUIElement, deactivate, 0, NULL)
_END_CLASS(GUIElement)

// GUIWindow
_BEGIN_CLASS(GUIWindow)
_MEMBER_FUNCTION(GUIWindow, constructor, 0, NULL)
_END_CLASS_BASE(GUIWindow, GUIElement)

// GUIText
_BEGIN_CLASS(GUIText)
_MEMBER_FUNCTION(GUIText, constructor, 0, NULL)
_MEMBER_FUNCTION(GUIText, setText, 1, "s")
_END_CLASS_BASE(GUIText, GUIElement)

// GUIButton
_BEGIN_CLASS(GUIButton)
_MEMBER_FUNCTION(GUIButton, constructor, 0, NULL)
_END_CLASS_BASE(GUIButton, GUIElement)

// GUIEditBox
_BEGIN_CLASS(GUIEditBox)
_MEMBER_FUNCTION(GUIEditBox, constructor, 0, NULL)
_END_CLASS_BASE(GUIEditBox, GUIElement)

// GUIMultiLineEditBox
_BEGIN_CLASS(GUIMultiLineEditBox)
_MEMBER_FUNCTION(GUIMultiLineEditBox, constructor, 0, NULL)
_END_CLASS_BASE(GUIMultiLineEditBox, GUIElement)

//GUIProgressBar
_BEGIN_CLASS(GUIProgressBar)
_MEMBER_FUNCTION(GUIProgressBar, constructor, 0, NULL)
_MEMBER_FUNCTION(GUIProgressBar, setValue, 1, "f")
_END_CLASS_BASE(GUIProgressBar, GUIElement)

// GUICheckBox
_BEGIN_CLASS(GUICheckBox)
_MEMBER_FUNCTION(GUICheckBox, constructor, 0, NULL)
_MEMBER_FUNCTION(GUICheckBox, setChecked, 1, "b")
_MEMBER_FUNCTION(GUICheckBox, isChecked, 0, NULL)
_END_CLASS_BASE(GUICheckBox, GUIElement)

// GUIImage
_BEGIN_CLASS(GUIImage)
_MEMBER_FUNCTION(GUIImage, constructor, 1, "s")
_END_CLASS_BASE(GUIImage, GUIElement)

/*
// GUIWebView
_BEGIN_CLASS(GUIWebView)
_MEMBER_FUNCTION(GUIWebView, constructor, 3, "iis")
_MEMBER_FUNCTION(GUIWebView, setURI, 1, "s")
_MEMBER_FUNCTION(GUIWebView, evaluateJavaScript, 1, "s")
_MEMBER_FUNCTION(GUIWebView, sendSignal, 1, "s")
_MEMBER_FUNCTION(GUIWebView, setHTML, 1, "s")
_MEMBER_FUNCTION(GUIWebView, setElementText, 2, "ss")
_MEMBER_FUNCTION(GUIWebView, getLoadInfo, 0, NULL)
_MEMBER_FUNCTION(GUIWebView, clickElement, 1, "s")
_MEMBER_FUNCTION(GUIWebView, setSize, 2, "ii")
_MEMBER_FUNCTION(GUIWebView, registerJavaScriptMethod, 1, "s")
_MEMBER_FUNCTION(GUIWebView, draw, 4, "iiii")
_END_CLASS_BASE(GUIWebView, GUIElement)
*/

void RegisterGUIElementNatives(CScriptingManager * pScriptingManager)
{
	// GUI
	pScriptingManager->RegisterClass(&_CLASS_DECL(GUIFont));
	pScriptingManager->RegisterClass(&_CLASS_DECL(GUIElement));
	pScriptingManager->RegisterClass(&_CLASS_DECL(GUIWindow));
	pScriptingManager->RegisterClass(&_CLASS_DECL(GUIText));
	pScriptingManager->RegisterClass(&_CLASS_DECL(GUIButton));
	pScriptingManager->RegisterClass(&_CLASS_DECL(GUIEditBox));
	pScriptingManager->RegisterClass(&_CLASS_DECL(GUIMultiLineEditBox));
	pScriptingManager->RegisterClass(&_CLASS_DECL(GUICheckBox));
	pScriptingManager->RegisterClass(&_CLASS_DECL(GUIImage));
	pScriptingManager->RegisterClass(&_CLASS_DECL(GUIProgressBar));
}

String GetKeyNameByScan(DWORD dwCode)
{
	switch(dwCode)
	{
	case 0x1:
		return "escape";
	case 0x2:
		return "one";
	case 0x3:
		return "two";
	case 0x4:
		return "three";
	case 0x5:
		return "four";
	case 0x6:
		return "five";
	case 0x7:
		return "six";
	case 0x8:
		return "seven";
	case 0x9:
		return "eight";
	case 0x0a:
		return "nine";
	case 0x0b:
		return "zero";
	case 0x0c:
		return "minus";
	case 0x0d:
		return "equals";
	case 0x0e:
		return "backspace";
	case 0x0f:
		return "tab";
	case 0x10:
		return "q";
	case 0x11:
		return "w";
	case 0x12:
		return "e";
	case 0x13:
		return "r";
	case 0x14:
		return "t";
	case 0x15:
		return "y";
	case 0x16:
		return "u";
	case 0x17:
		return "i";
	case 0x18:
		return "o";
	case 0x19:
		return "p";
	case 0x1a:
		return "leftbracket";
	case 0x1b:
		return "rightbracket";
	case 0x1c:
		return "return";
	case 0x1d:
		return "leftcontrol";
	case 0x1e:
		return "a";
	case 0x1f:
		return "s";
	case 0x20:
		return "d";
	case 0x21:
		return "f";
	case 0x22:
		return "g";
	case 0x23:
		return "h";
	case 0x24:
		return "j";
	case 0x25:
		return "k";
	case 0x26:
		return "l";
	case 0x27:
		return "semicolon";
	case 0x28:
		return "apostrophe";
	case 0x29:
		return "grave";
	case 0x2a:
		return "leftshift";
	case 0x2b:
		return "backslash";
	case 0x2c:
		return "z";
	case 0x2d:
		return "x";
	case 0x2e:
		return "c";
	case 0x2f:
		return "v";
	case 0x30:
		return "b";
	case 0x31:
		return "n";
	case 0x32:
		return "m";
	case 0x33:
		return "comma";
	case 0x34:
		return "period";
	case 0x35:
		return "slash";
	case 0x36:
		return "rightshift";
	case 0x37:
		return "multiply";
	case 0x38:
		return "leftalt";
	case 0x39:
		return "space";
	case 0x3a:
		return "capital";
	case 0x3b:
		return "f1";
	case 0x3c:
		return "f2";
	case 0x3d:
		return "f3";
	case 0x3e:
		return "f4";
	case 0x3f:
		return "f5";
	case 0x40:
		return "f6";
	case 0x41:
		return "f7";
	case 0x42:
		return "f8";
	case 0x43:
		return "f9";
	case 0x44:
		return "f10";
	case 0x45:
		return "numlock";
	case 0x46:
		return "scrolllock";
	case 0x47:
		return "numpad7";
	case 0x48:
		return "numpad8";
	case 0x49:
		return "numpad9";
	case 0x4a:
		return "subtract";
	case 0x4b:
		return "numpad4";
	case 0x4c:
		return "numpad5";
	case 0x4d:
		return "numpad6";
	case 0x4e:
		return "add";
	case 0x4f:
		return "numpad1";
	case 0x50:
		return "numpad2";
	case 0x51:
		return "numpad3";
	case 0x52:
		return "numpad0";
	case 0x53:
		return "decimal";
	case 0x56:
		return "oem_102";
	case 0x57:
		return "f11";
	case 0x58:
		return "f12";
	case 0x64:
		return "f13";
	case 0x65:
		return "f14";
	case 0x66:
		return "f15";
	case 0x70:
		return "kana";
	case 0x73:
		return "abnt_c1";
	case 0x79:
		return "convert";
	case 0x7b:
		return "noconvert";
	case 0x7d:
		return "yen";
	case 0x7e:
		return "abnt_c2";
	case 0x8d:
		return "numpadequals";
	case 0x90:
		return "prevtrack";
	case 0x91:
		return "at";
	case 0x92:
		return "colon";
	case 0x93:
		return "underline";
	case 0x94:
		return "kanji";
	case 0x95:
		return "stop";
	case 0x96:
		return "ax";
	case 0x97:
		return "unlabeled";
	case 0x99:
		return "nexttrack";
	case 0x9c:
		return "numpadenter";
	case 0x9d:
		return "rightcontrol";
	case 0xa0:
		return "mute";
	case 0xa1:
		return "calculator";
	case 0xa2:
		return "playpause";
	case 0xa4:
		return "mediastop";
	case 0xae:
		return "volumedown";
	case 0xb0:
		return "volumeup";
	case 0xb2:
		return "webhome";
	case 0xb3:
		return "numpadcomma";
	case 0xb5:
		return "divide";
	case 0xb7:
		return "sysrq";
	case 0xb8:
		return "rightalt";
	case 0xc5:
		return "pause";
	case 0xc7:
		return "home";
	case 0xc8:
		return "arrowup";
	case 0xc9:
		return "pageup";
	case 0xcb:
		return "arrowleft";
	case 0xcd:
		return "arrowright";
	case 0xcf:
		return "end";
	case 0xd0:
		return "arrowdown";
	case 0xd1:
		return "pagedown";
	case 0xd2:
		return "insert";
	case 0xd3:
		return "delete";
	case 0xdb:
		return "leftwindows";
	case 0xdc:
		return "rightwindows";
	case 0xdd:
		return "appmenu";
	case 0xde:
		return "power";
	case 0xdf:
		return "sleep";
	case 0xe3:
		return "wake";
	case 0xe5:
		return "websearch";
	case 0xe6:
		return "webfavorites";
	case 0xe7:
		return "webrefresh";
	case 0xe8:
		return "webstop";
	case 0xe9:
		return "webforward";
	case 0xea:
		return "webback";
	case 0xeb:
		return "mycomputer";
	case 0xec:
		return "mail";
	case 0xed:
		return "mediaselect";
	default:
		return "unknown";
	}
}

// event guiVisibleChanged(string guiName)
bool OnVisibleChanged(const CEGUI::EventArgs &eventArgs)
{	
	CEvents * pEvents = g_pClient->GetEvents();
	String eventName("guiVisibleChanged");

	if(!pEvents->IsEventRegistered(eventName))
		return false;

	const CEGUI::WindowEventArgs eArgs = static_cast<const CEGUI::WindowEventArgs&>(eventArgs);
	CEGUI::Window * pWindow = eArgs.window;
	CScript * pScript = g_pClient->GetClientScriptManager()->GetGUIManager()->GetScript(pWindow);

	CScriptArguments pArguments;
	pArguments.push(eArgs.window->getName().c_str());

	pEvents->Call(eventName, &pArguments, pScript);
	return true;
}
bool OnHidden(const CEGUI::EventArgs &eventArgs)
{
	return OnVisibleChanged(eventArgs);
}
bool OnShown(const CEGUI::EventArgs &eventArgs)
{
	return OnVisibleChanged(eventArgs);
}

// event guiKeyPress(string guiName, string key, string keyState)
bool OnKeyPress(const CEGUI::KeyEventArgs &eArgs, String keyState)
{	
	CEvents * pEvents = g_pClient->GetEvents();
	String eventName("guiKeyPress");

	if(!pEvents->IsEventRegistered(eventName))
		return false;

	CEGUI::Window * pWindow = eArgs.window;
	CScript * pScript = g_pClient->GetClientScriptManager()->GetGUIManager()->GetScript(pWindow);
	CEGUI::Key::Scan keyCode = eArgs.scancode;

	CScriptArguments pArguments;
	pArguments.push(eArgs.window->getName().c_str());
	pArguments.push(GetKeyNameByScan(eArgs.scancode));
	pArguments.push(keyState);

	CScriptArgument pReturn = pEvents->Call(eventName, &pArguments, pScript);
	return pReturn.GetInteger() != 0;
}
bool OnKeyDown(const CEGUI::EventArgs &eventArgs)
{
	const CEGUI::KeyEventArgs eArgs = static_cast<const CEGUI::KeyEventArgs&>(eventArgs);
	return OnKeyPress(eArgs, "down");
}
bool OnKeyUp(const CEGUI::EventArgs &eventArgs)
{
	const CEGUI::KeyEventArgs eArgs = static_cast<const CEGUI::KeyEventArgs&>(eventArgs);
	return OnKeyPress(eArgs, "up");
}

// event editBoxTextChanged(string guiName)
bool OnEditboxTextChanged(const CEGUI::EventArgs &eventArgs)
{
	CEvents * pEvents = g_pClient->GetEvents();
	String eventName ("editBoxTextChanged");

	if(!pEvents->IsEventRegistered(eventName))
		return false;

	const CEGUI::WindowEventArgs eArgs = static_cast<const CEGUI::WindowEventArgs&>(eventArgs);
	CEGUI::Window * pWindow = eArgs.window;
	CScript * pScript = g_pClient->GetClientScriptManager()->GetGUIManager()->GetScript(pWindow);

	CScriptArguments pArguments;
	pArguments.push(eArgs.window->getName().c_str());
	pEvents->Call(eventName, &pArguments, pScript);
	return true;
}

// event guiClick(guiName, mouseButton)
bool OnClick(const CEGUI::EventArgs &eventArgs)
{
	CEvents * pEvents = g_pClient->GetEvents();
	String eventName("guiClick");

	if(!pEvents->IsEventRegistered(eventName))
		return false;
	
	const CEGUI::MouseEventArgs eArgs = static_cast<const CEGUI::MouseEventArgs&>(eventArgs);
	CEGUI::Window * pWindow = eArgs.window;
	CScript * pScript = g_pClient->GetClientScriptManager()->GetGUIManager()->GetScript(pWindow);

	CScriptArguments pArguments;
	pArguments.push(eArgs.window->getName().c_str());
	pArguments.push(eArgs.button);
	pEvents->Call(eventName, &pArguments, pScript);
	return true;
}

// event guiMouseEnter(guiName)
bool OnMouseEnters(const CEGUI::EventArgs &eventArgs)
{
	CEvents * pEvents = g_pClient->GetEvents();
	String eventName("guiMouseEnter");

	if(!pEvents->IsEventRegistered(eventName))
		return false;

	const CEGUI::MouseEventArgs eArgs = static_cast<const CEGUI::MouseEventArgs&>(eventArgs);
	CEGUI::Window * pWindow = eArgs.window;
	CScript * pScript = g_pClient->GetClientScriptManager()->GetGUIManager()->GetScript(pWindow);
	
	CScriptArguments pArguments;
	pArguments.push(pWindow->getName().c_str());
	pEvents->Call(eventName, &pArguments, pScript);
	return true;
}

// event guiMouseMove(guiName)
bool OnMouseMove(const CEGUI::EventArgs &eventArgs)
{
	CEvents * pEvents = g_pClient->GetEvents();
	String eventName("guiMouseMove");

	if(!pEvents->IsEventRegistered(eventName))
		return false;

	const CEGUI::MouseEventArgs eArgs = static_cast<const CEGUI::MouseEventArgs&>(eventArgs);
	CEGUI::Window * pWindow = eArgs.window;
	CScript * pScript = g_pClient->GetClientScriptManager()->GetGUIManager()->GetScript(pWindow);
	
	CScriptArguments pArguments;
	pArguments.push(pWindow->getName().c_str());
	pEvents->Call(eventName, &pArguments, pScript);
	return true;
}

// event guiMouseLeave(guiName)
bool OnMouseLeaves(const CEGUI::EventArgs &eventArgs)
{
	CEvents * pEvents = g_pClient->GetEvents();
	String eventName("guiMouseLeave");

	if(!pEvents->IsEventRegistered(eventName))
		return false;

	const CEGUI::MouseEventArgs eArgs = static_cast<const CEGUI::MouseEventArgs&>(eventArgs);
	CEGUI::Window * pWindow = eArgs.window;
	CScript * pScript = g_pClient->GetClientScriptManager()->GetGUIManager()->GetScript(pWindow);
	
	CScriptArguments pArguments;
	pArguments.push(pWindow->getName().c_str());
	pEvents->Call(eventName, &pArguments, pScript);
	return true;
}

// event buttonClick(buttonName, bState)
// TODO: remove it. We have better alternative guiClick (OnClick) for any gui element
bool OnButtonClick(const CEGUI::EventArgs &eventArgs)
{
	CEvents * pEvents = g_pClient->GetEvents();
	String eventName("buttonClick");

	if(!pEvents->IsEventRegistered(eventName))
		return false;

	CEGUI::Window * pWindow = static_cast<const CEGUI::WindowEventArgs&>(eventArgs).window;
	CScript * pScript = g_pClient->GetClientScriptManager()->GetGUIManager()->GetScript(pWindow);

	CScriptArguments pArguments;
	pArguments.push(pWindow->getName().c_str());
	pArguments.push(0);	// FIXME or what?
	pEvents->Call(eventName, &pArguments, pScript);
	return true;
}

// event checkBoxCheckedChanged(checkBoxName)
bool OnCheckboxCheckStateChanged(const CEGUI::EventArgs &eventArgs)
{
	CEvents * pEvents = g_pClient->GetEvents();
	String eventName("checkBoxCheckedChanged");

	if(!pEvents->IsEventRegistered(eventName))
		return false;

	CEGUI::Window * pWindow = static_cast<const CEGUI::WindowEventArgs&>(eventArgs).window;
	CScript * pScript = g_pClient->GetClientScriptManager()->GetGUIManager()->GetScript(pWindow);

	CScriptArguments pArguments;
	pArguments.push(pWindow->getName().c_str());
	pEvents->Call(eventName, &pArguments, pScript);
	return true;
}

// event windowClose(windowName)
bool OnWindowClose(const CEGUI::EventArgs &eventArgs)
{
	CEvents * pEvents = g_pClient->GetEvents();
	String eventName("windowClose");

	if(!pEvents->IsEventRegistered(eventName))
		return false;

	CEGUI::Window * pWindow = static_cast<const CEGUI::WindowEventArgs&>(eventArgs).window;
	CScript * pScript = g_pClient->GetClientScriptManager()->GetGUIManager()->GetScript(pWindow);

	CScriptArguments pArguments;
	pArguments.push(pWindow->getName().c_str());

	// Event handler must return 1 to close window, otherwise, 0.
	CScriptArgument pReturn = pEvents->Call(eventName, &pArguments, pScript);

	if(pReturn.GetInteger())
		pWindow->hide();

	return true;
}

extern void SubscribeGuiEvents(CEGUI::Window * pWindow)
{		
	pWindow->subscribeEvent(CEGUI::Window::EventMouseClick, CEGUI::Event::Subscriber(&OnClick));
	pWindow->subscribeEvent(CEGUI::Window::EventMouseEnters, CEGUI::Event::Subscriber(&OnMouseEnters));
	pWindow->subscribeEvent(CEGUI::Window::EventMouseMove, CEGUI::Event::Subscriber(&OnMouseMove));
	pWindow->subscribeEvent(CEGUI::Window::EventMouseLeaves, CEGUI::Event::Subscriber(&OnMouseLeaves));
	pWindow->subscribeEvent(CEGUI::Window::EventShown, CEGUI::Event::Subscriber(&OnShown));
	pWindow->subscribeEvent(CEGUI::Window::EventHidden, CEGUI::Event::Subscriber(&OnHidden));
	pWindow->subscribeEvent(CEGUI::Window::EventKeyDown, CEGUI::Event::Subscriber(&OnKeyDown));
	pWindow->subscribeEvent(CEGUI::Window::EventKeyUp, CEGUI::Event::Subscriber(&OnKeyUp));
}

// Font
_MEMBER_FUNCTION_IMPL(GUIFont, constructor)
{
	if(script_gettop(pVM) < 1)
	{
		script_pushbool(pVM, false);
		return 1;
	}

	const char * szFont;
	int iSize = 8;
	bool bScaled = false;

	script_getstring(pVM, 1, &szFont);
	if(script_gettop(pVM) >= 2)
	{
		script_getinteger(pVM, 2, &iSize);
		if(iSize < 3 || iSize > 100)
		{
			script_pushbool(pVM, false);
			return 1;
		}

		if(script_gettop(pVM) >= 3)
		{
			bool sqb;
			script_getbool(pVM, 3, &sqb);
			bScaled = sqb != 0;
		}
	}


	CEGUI::Font * pFont = g_pClient->GetGUI()->GetFont(szFont, iSize, bScaled);

	if(!pFont || script_setinstance(pVM, pFont, &_CLASS_DECL(GUIFont)) != 0)
	{
		CLogFile::Printf("Can't create GUIFont.");
		script_pushnull(pVM);
		return 1;
	}

	//script_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIFont, getTextExtent)
{
	const char * text;
	script_getstring(pVM, -1, (const char **)&text);
	CEGUI::Font * pFont = script_getinstance<CEGUI::Font *>(pVM);

	if(!pFont)
	{
		script_pushnull(pVM);
		return 1;
	}

	script_pushfloat(pVM, pFont->getTextExtent(g_pClient->GetGUI()->AnsiToCeguiFriendlyString(text)));
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIFont, getFontHeight)
{
	CEGUI::Font * pFont = script_getinstance<CEGUI::Font *>(pVM);

	if(!pFont)
	{
		script_pushnull(pVM);
		return 1;
	}

	script_pushfloat(pVM, pFont->getFontHeight());
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIFont, drawText)
{
	CEGUI::Font * pFont = script_getinstance<CEGUI::Font *>(pVM);

	if(!pFont)
	{
		script_pushbool(pVM, false);
		return 1;
	}

	const char* text;
	bool sbRelative;
	float x, y;
	script_getfloat(pVM, -4, &x);
	script_getfloat(pVM, -3, &y);
	script_getstring(pVM, -2, &text);
	script_getbool(pVM, -1, &sbRelative);

	// Get our GUI instance
	CGUI * pGUI = g_pClient->GetGUI();
	
	if(sbRelative != 0) 
	{
		x *= pGUI->GetDisplayWidth();
		y *= pGUI->GetDisplayHeight();
	}

	pGUI->DrawText(text, CEGUI::Vector2( x, y ), (CEGUI::colour)D3DCOLOR_RGBA(255, 255, 255, 255), pFont, true, true);
	script_pushbool(pVM, true);
	return 1;
}

// Main GUIElement

// Destructors
_MEMBER_FUNCTION_DESTRUCTOR(GUIElement)
{
	g_pClient->GetClientScriptManager()->GetGUIManager()->Delete(script_getinstance<CEGUI::Window *>(pVM));
	return 0;
}

_MEMBER_FUNCTION_DESTRUCTOR(GUIFont)
{
	g_pClient->GetClientScriptManager()->GetGUIManager()->Delete(script_getinstance<CEGUI::Window *>(pVM));
	return 0;
}

_MEMBER_FUNCTION_DESTRUCTOR(GUIWindow)
{
	g_pClient->GetClientScriptManager()->GetGUIManager()->Delete(script_getinstance<CEGUI::Window *>(pVM));
	return 0;
}

_MEMBER_FUNCTION_DESTRUCTOR(GUIText)
{
	g_pClient->GetClientScriptManager()->GetGUIManager()->Delete(script_getinstance<CEGUI::Window *>(pVM));
	return 0;
}

_MEMBER_FUNCTION_DESTRUCTOR(GUIButton)
{
	g_pClient->GetClientScriptManager()->GetGUIManager()->Delete(script_getinstance<CEGUI::Window *>(pVM));
	return 0;
}

_MEMBER_FUNCTION_DESTRUCTOR(GUIEditBox)
{
	g_pClient->GetClientScriptManager()->GetGUIManager()->Delete(script_getinstance<CEGUI::Window *>(pVM));
	return 0;
}

_MEMBER_FUNCTION_DESTRUCTOR(GUIMultiLineEditBox)
{
	g_pClient->GetClientScriptManager()->GetGUIManager()->Delete(script_getinstance<CEGUI::Window *>(pVM));
	return 0;
}

_MEMBER_FUNCTION_DESTRUCTOR(GUICheckBox)
{
	g_pClient->GetClientScriptManager()->GetGUIManager()->Delete(script_getinstance<CEGUI::Window *>(pVM));
	return 0;
}

_MEMBER_FUNCTION_DESTRUCTOR(GUIImage)
{
	g_pClient->GetClientScriptManager()->GetGUIManager()->Delete(script_getinstance<CEGUI::Window *>(pVM));
	return 0;
}

_MEMBER_FUNCTION_DESTRUCTOR(GUIProgressBar)
{
	g_pClient->GetClientScriptManager()->GetGUIManager()->Delete(script_getinstance<CEGUI::Window *>(pVM));
	return 0;
}

// GUIWindow
_MEMBER_FUNCTION_IMPL(GUIElement, constructor)
{
	const char * name;
	script_getstring(pVM, -1, &name);
	String szName = name;

	//if(szName.IsEmpty())
	//	_SET_RELEASE_HOOK(GUIElement);

	// Get our GUI instance
	CGUI * pGUI = g_pClient->GetGUI();

	try
	{
		pGUI->GetWindowManager()->getWindow(szName.C_String());
	}
	catch(CEGUI::Exception &e)
	{
		e;
		//_SET_RELEASE_HOOK(GUIElement);
	}

	CEGUI::Window * pWindow = pGUI->GetWindowManager()->getWindow(szName.C_String());
	if(!pWindow || script_setinstance(pVM, pWindow, &_CLASS_DECL(GUIElement)) != 0)
	{
		CLogFile::Printf("Can't create GUIElement.");
		script_pushnull(pVM);
		return 1;
	}

	SubscribeGuiEvents(pWindow);

	//script_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIElement, setText)
{
	const char * text;
	script_getstring(pVM, -1, (const char **)&text);
	CGUIFrameWindow * pWindow = script_getinstance<CGUIFrameWindow *>(pVM);

	if(!pWindow)
	{
		script_pushbool(pVM, false);
		return 1;
	}

	pWindow->setText(CGUI::AnsiToCeguiFriendlyString(text, strlen(text)));
	script_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIElement, getSize)
{
	CGUIFrameWindow * pWindow = script_getinstance<CGUIFrameWindow *>(pVM);

	if(pWindow)
	{
		CEGUI::UVector2 sz = pWindow->getSize();
		CScriptArray args(pVM);
		args.push(sz.d_x.d_offset);
		args.push(sz.d_y.d_offset);
		//CScriptArgument(args, true).push(pVM);
		return 1;
	}

	//script_pushbool(pVM, false);
	return 0;
}

_MEMBER_FUNCTION_IMPL(GUIElement, setSize)
{
	float x, y;
	bool relative;
	script_getbool(pVM, -1, &relative);
	script_getfloat(pVM, -2, &y);
	script_getfloat(pVM, -3, &x);

	CGUIFrameWindow * pWindow = script_getinstance<CGUIFrameWindow *>(pVM);

	if(!pWindow)
	{
		script_pushbool(pVM, false);
		return 1;
	}

	if(relative != 0)
	{
		pWindow->setSize(CEGUI::UVector2(CEGUI::UDim(x, 0), CEGUI::UDim(y, 0)));
	}
	else
	{
		pWindow->setSize(CEGUI::UVector2(CEGUI::UDim(0, x), CEGUI::UDim(0, y)));
	}
	script_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIElement, setPosition)
{
	float x, y;
	bool relative;
	script_getbool(pVM, -1, &relative);
	script_getfloat(pVM, -2, &y);
	script_getfloat(pVM, -3, &x);

	CGUIFrameWindow * pWindow = script_getinstance<CGUIFrameWindow *>(pVM);

	if(!pWindow)
	{
		script_pushbool(pVM, false);
		return 1;
	}

	if(relative != 0)
	{
		pWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(x, 0), CEGUI::UDim(y, 0)));
	}
	else
	{
		pWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0, x), CEGUI::UDim(0, y)));
	}
	script_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIElement, setRotation)
{
	float x, y, z;
	script_getfloat(pVM, -1, &z);
	script_getfloat(pVM, -2, &y);
	script_getfloat(pVM, -3, &x);

	CGUIFrameWindow * pWindow = script_getinstance<CGUIFrameWindow *>(pVM);

	if(!pWindow)
	{
		script_pushbool(pVM, false);
		return 1;
	}

	pWindow->setRotation(CEGUI::Vector3(x, y, z));
	script_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIElement, setAlpha)
{
	float alpha;
	script_getfloat(pVM, -1, &alpha);

	CGUIFrameWindow * pWindow = script_getinstance<CGUIFrameWindow *>(pVM);

	if(!pWindow)
	{
		script_pushbool(pVM, false);
		return 1;
	}

	pWindow->setAlpha(alpha);
	script_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIElement, setVisible)
{
	bool visible;
	script_getbool(pVM, -1, &visible);

	CGUIFrameWindow * pWindow = script_getinstance<CGUIFrameWindow *>(pVM);

	if(!pWindow)
	{
		script_pushbool(pVM, false);
		return 1;
	}

	pWindow->setVisible(visible != 0);
	script_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIElement, setParent)
{
	const char * parent;
	script_getstring(pVM, -1, &parent);

	CGUIFrameWindow * pWindow = script_getinstance<CGUIFrameWindow *>(pVM);

	if(!pWindow)
	{
		script_pushbool(pVM, false);
		return 1;
	}
	
	CEGUI::Window * pParentWindow = NULL;

	try
	{
		pParentWindow = g_pClient->GetGUI()->GetWindowManager()->getWindow(parent);
	}
	catch(CEGUI::UnknownObjectException &e)
	{
		(void)e;
		script_pushbool(pVM, false);
		return 1;
	}

	pParentWindow->addChildWindow(pWindow);
	script_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIElement, getName)
{
	CGUIFrameWindow * pWindow = script_getinstance<CGUIFrameWindow *>(pVM);

	if(!pWindow)
	{
		return 0;
	}

	script_pushstring(pVM, pWindow->getName().c_str());
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIElement, getText)
{
	CGUIFrameWindow * pWindow = script_getinstance<CGUIFrameWindow *>(pVM);

	if(!pWindow)
	{
		return 0;
	}

	script_pushstring(pVM, pWindow->getText().c_str());
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIElement, setFont)
{
	CGUIFrameWindow * pWindow = script_getinstance<CGUIFrameWindow *>(pVM);
	CEGUI::Font * pFont = script_getinstance<CEGUI::Font *>(pVM, 1);

	if(!pWindow || !pFont)
	{
		script_pushbool(pVM, false);
		return 1;
	}

	pWindow->setFont(pFont);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIElement, setProperty)
{
	const char * prop;
	const char * value;
	script_getstring(pVM, -1, &value);
	script_getstring(pVM, -2, &prop);

	CGUIFrameWindow * pWindow = script_getinstance<CGUIFrameWindow *>(pVM);

	if(!pWindow)
	{
		script_pushbool(pVM, false);
		return 1;
	}

	try
	{
		pWindow->setProperty(prop, value);
	}
	catch(CEGUI::UnknownObjectException &e)
	{
		(void)e;
		script_pushbool(pVM, false);
		return 1;
	}

	script_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIElement, activate)
{
	CGUIFrameWindow * pWindow = script_getinstance<CGUIFrameWindow *>(pVM);

	if(!pWindow)
	{
		script_pushbool(pVM, false);
		return 1;
	}

	pWindow->activate();
	script_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIElement, deactivate)
{
	CGUIFrameWindow * pWindow = script_getinstance<CGUIFrameWindow *>(pVM);

	if(!pWindow)
	{
		script_pushbool(pVM, false);
		return 1;
	}

	pWindow->deactivate();
	script_pushbool(pVM, true);
	return 1;
}

// GUIWindow
_MEMBER_FUNCTION_IMPL(GUIWindow, constructor)
{
	CEGUI::Window * pWindow = g_pClient->GetGUI()->CreateGUIFrameWindow();

	if(!pWindow || script_setinstance(pVM, pWindow, &_CLASS_DECL(GUIWindow)) != 0)
	{
		CLogFile::Printf("Can't create GUIWindow.");
		//script_pushnull(pVM);
		return 0;
	}

	//_SET_RELEASE_HOOK(GUIElement);
	CClientScriptManager * pClientScriptManager = g_pClient->GetClientScriptManager();
	pClientScriptManager->GetGUIManager()->Add(pWindow, pClientScriptManager->GetScriptingManager()->Get(pVM));
	pWindow->setVisible(true);

	SubscribeGuiEvents(pWindow);
	pWindow->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked, CEGUI::Event::Subscriber(&OnWindowClose));

	//script_pushbool(pVM, true);
	return 1;
}

// GUIText
_MEMBER_FUNCTION_IMPL(GUIText, constructor)
{
	// Get our GUI
	CGUI * pGUI = g_pClient->GetGUI();

	CEGUI::Window * pWindow = pGUI->CreateGUIStaticText();

	if(!pWindow || script_setinstance(pVM, pWindow, &_CLASS_DECL(GUIText)) != 0)
	{
		CLogFile::Printf("Can't create GUIText.");
		//script_pushnull(pVM);
		return 0;
	}

	//_SET_RELEASE_HOOK(GUIElement);
	CClientScriptManager * pClientScriptManager = g_pClient->GetClientScriptManager();
	pClientScriptManager->GetGUIManager()->Add(pWindow, pClientScriptManager->GetScriptingManager()->Get(pVM));
	pWindow->setVisible(true);
	pWindow->setProperty("FrameEnabled", "false");
	pWindow->setProperty("BackgroundEnabled", "false");
	pWindow->setFont(pGUI->GetFont("tahoma-bold"));
	pWindow->setProperty("TextColours", "tl:FFFFFFFF tr:FFFFFFFF bl:FFFFFFFF br:FFFFFFFF");
	
	SubscribeGuiEvents(pWindow);

	//script_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIText, setText)
{
	const char * text;
	script_getstring(pVM, -1, (const char **)&text);
	CGUIFrameWindow * pWindow = script_getinstance<CGUIFrameWindow *>(pVM);

	if(!pWindow)
	{
		script_pushbool(pVM, false);
		return 1;
	}

	CClientScriptManager * pClientScriptManager = g_pClient->GetClientScriptManager();
	pClientScriptManager->GetGUIManager()->Add(pWindow, pClientScriptManager->GetScriptingManager()->Get(pVM));
	CEGUI::Font * pFont = g_pClient->GetGUI()->GetFont("tahoma-bold");
	float fTextWidth = pFont->getTextExtent(text);
	float fTextHeight = pFont->getFontHeight();
	pWindow->setSize(CEGUI::UVector2(CEGUI::UDim(0, fTextWidth), CEGUI::UDim(0, fTextHeight)));
	pWindow->setText(CGUI::AnsiToCeguiFriendlyString(text, strlen(text)));
	script_pushbool(pVM, true);
	return 1;
}

// GUIButton
_MEMBER_FUNCTION_IMPL(GUIButton, constructor)
{
	CEGUI::Window * pWindow = g_pClient->GetGUI()->CreateGUIButton();
	if(!pWindow || script_setinstance(pVM, pWindow, &_CLASS_DECL(GUIButton)) != 0)
	{
		CLogFile::Printf("Can't create GUIButton.");
		//script_pushnull(pVM);
		return 0;
	}

	//_SET_RELEASE_HOOK(GUIElement);
	CClientScriptManager * pClientScriptManager = g_pClient->GetClientScriptManager();
	pClientScriptManager->GetGUIManager()->Add(pWindow, pClientScriptManager->GetScriptingManager()->Get(pVM));
	pWindow->setVisible(true);

	SubscribeGuiEvents(pWindow);
	pWindow->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&OnButtonClick));

	//script_pushbool(pVM, true);
	return 1;
}

// GUIEditBox
_MEMBER_FUNCTION_IMPL(GUIEditBox, constructor)
{
	CEGUI::Window * pWindow = g_pClient->GetGUI()->CreateGUIEditBox();
	if(!pWindow || script_setinstance(pVM, pWindow, &_CLASS_DECL(GUIEditBox)) != 0)
	{
		CLogFile::Printf("Can't create GUIEditBox.");
		//script_pushnull(pVM);
		return 0;
	}

	//_SET_RELEASE_HOOK(GUIElement);
	CClientScriptManager * pClientScriptManager = g_pClient->GetClientScriptManager();
	pClientScriptManager->GetGUIManager()->Add(pWindow, pClientScriptManager->GetScriptingManager()->Get(pVM));
	pWindow->setVisible(true);	

	SubscribeGuiEvents(pWindow);
	pWindow->subscribeEvent(CEGUI::Editbox::EventTextChanged, CEGUI::Event::Subscriber(&OnEditboxTextChanged));

	//script_pushbool(pVM, true);
	return 1;
}
// GUIMultiLineEditBox
_MEMBER_FUNCTION_IMPL(GUIMultiLineEditBox, constructor)
{
	CEGUI::Window * pWindow = g_pClient->GetGUI()->CreateGUIMultiLineEditBox();
	if(!pWindow || script_setinstance(pVM, pWindow, &_CLASS_DECL(GUIMultiLineEditBox)) != 0)
	{
		CLogFile::Printf("Can't create GUIMultiLineEditBox.");
		//script_pushnull(pVM);
		return 0;
	}

	//_SET_RELEASE_HOOK(GUIElement);
	CClientScriptManager * pClientScriptManager = g_pClient->GetClientScriptManager();
	pClientScriptManager->GetGUIManager()->Add(pWindow, pClientScriptManager->GetScriptingManager()->Get(pVM));
	pWindow->setVisible(true);	

	SubscribeGuiEvents(pWindow);
	pWindow->subscribeEvent(CEGUI::Editbox::EventTextChanged, CEGUI::Event::Subscriber(&OnEditboxTextChanged));

	//script_pushbool(pVM, true);
	return 1;
}

// GUICheckBox
_MEMBER_FUNCTION_IMPL(GUICheckBox, constructor)
{
	CEGUI::Window * pWindow = g_pClient->GetGUI()->CreateGUICheckBox();
	if(!pWindow || script_setinstance(pVM, pWindow, &_CLASS_DECL(GUICheckBox)) != 0)
	{
		CLogFile::Printf("Can't create GUICheckBox.");
		//script_pushnull(pVM);
		return 0;
	}

	//_SET_RELEASE_HOOK(GUIElement);
	CClientScriptManager * pClientScriptManager = g_pClient->GetClientScriptManager();
	pClientScriptManager->GetGUIManager()->Add(pWindow, pClientScriptManager->GetScriptingManager()->Get(pVM));
	pWindow->setVisible(true);

	SubscribeGuiEvents(pWindow);
	pWindow->subscribeEvent(CEGUI::Checkbox::EventCheckStateChanged, CEGUI::Event::Subscriber(&OnCheckboxCheckStateChanged));

	//script_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUICheckBox, setChecked)
{
	bool sqbChecked;
	script_getbool(pVM, -1, &sqbChecked);

	CEGUI::Checkbox * pWindow = script_getinstance<CEGUI::Checkbox *>(pVM);

	if(!pWindow)
	{
		script_pushbool(pVM, false);
		return 1;
	}

	pWindow->setSelected(sqbChecked != 0);
	script_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUICheckBox, isChecked)
{
	CEGUI::Checkbox * pWindow = script_getinstance<CEGUI::Checkbox *>(pVM);

	if(!pWindow)
	{
		script_pushbool(pVM, false);
		return 1;
	}

	script_pushbool(pVM, pWindow->isSelected());
	return 1;
}

// GUIImage
_MEMBER_FUNCTION_IMPL(GUIImage, constructor)
{
	const char * filename;
	script_getstring(pVM, -1, &filename);

	// check file size
	/*FILE* pFile = fopen(filename, "rb");
	fseek(pFile, 0L, SEEK_END);
	int len = ftell(pFile);
	if(len == 0) {
		script_pushbool(pVM, false);
		fclose(pFile);
		return 1;
	}
	fclose(pFile);*/

	// Get our GUI
	CGUI * pGUI = g_pClient->GetGUI();

	String szName = pGUI->GetUniqueName();

	// Try to load the image
	try
	{

		CEGUI::ImagesetManager::getSingleton().createFromImageFile(szName.C_String(), filename, "resources");
		CGUIStaticImage * pImage = pGUI->CreateGUIStaticImage(CEGUI::String(szName.C_String()));
		
		pImage->setProperty("FrameEnabled", "false");
		pImage->setProperty("BackgroundEnabled", "false");
		pImage->setProperty("Image", String("set:%s image:full_image", szName.C_String()).C_String());

		if(!pImage || script_setinstance(pVM, pImage, &_CLASS_DECL(GUIImage)) != 0)
		{
			CLogFile::Printf("Can't create GUIImage.");
			//script_pushnull(pVM);
			return 0;
		}

		//_SET_RELEASE_HOOK(GUIElement);
		CClientScriptManager * pClientScriptManager = g_pClient->GetClientScriptManager();
		pClientScriptManager->GetGUIManager()->Add(pImage, pClientScriptManager->GetScriptingManager()->Get(pVM));

		SubscribeGuiEvents(pImage);

		pImage->setVisible(true);
		//script_pushbool(pVM, true);
		return 1;
	}
	catch(CEGUI::Exception e)
	{
		// Failed to load, might not exist
		CLogFile::Printf("Can't create GUIImage (%s does not exist)", filename);
		//script_pushbool(pVM, false);
		return 0;
	}
}

// GUIProgressBar
_MEMBER_FUNCTION_IMPL(GUIProgressBar, constructor)
{
	CEGUI::Window * pWindow = g_pClient->GetGUI()->CreateGUIProgressBar();
	if(!pWindow || script_setinstance(pVM, pWindow, &_CLASS_DECL(GUIProgressBar)) != 0)
	{
		CLogFile::Printf("Can't create GUIProgressBar.");
		//script_pushbool(pVM, false);
		return 0;
	}

	CClientScriptManager * pClientScriptManager = g_pClient->GetClientScriptManager();
	pClientScriptManager->GetGUIManager()->Add(pWindow, pClientScriptManager->GetScriptingManager()->Get(pVM));
 	pWindow->setVisible(true);
 	pWindow->setProperty("CurrentProgress", "0.0");

	SubscribeGuiEvents(pWindow);

 	//script_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIProgressBar, setValue)
{
 	CEGUI::Window * pWindow = script_getinstance<CEGUI::Window *>(pVM);
	float fCurrentValue = 0.0f;
	script_getfloat(pVM, 1, &fCurrentValue);

 	if ( fCurrentValue > 100.0f || fCurrentValue < 0.0f )
	{
 		CLogFile::Printf("Invalid first argument value. (Argument accept values from 0.0 to 100.0 [float], your value %f)", fCurrentValue);
 		script_pushbool(pVM, false);
		return 1;
 	}

 	char szNewProgress[4];
	sprintf(szNewProgress, "%f", fCurrentValue*0.01);
	pWindow->setProperty("CurrentProgress", szNewProgress);

	script_pushbool(pVM, true);
	return 1;
}

/*

// GUIWebView
_MEMBER_FUNCTION_IMPL(GUIWebView, constructor)
{
	SQInteger width, height;
	const char * szUrl;
	script_getinteger(pVM, -3, &width);
	script_getinteger(pVM, -2, &height);
	script_getstring(pVM, -1, &szUrl);
	CD3D9WebView * pView = g_pWebkit->CreateView(width, height, g_pGUI->GetDirect3DDevice());
	pView->GetView()->SetURI(szUrl);

	if(!pView || SQ_FAILED(script_setinstance(pVM, pView->GetWindow())))
	{
		CLogFile::Printf("Can't create GUIWebView.");
		script_pushbool(pVM, false);
		return 1;
	}

	CClientScriptManager * pClientScriptManager = g_pClient->GetClientScriptManager();
	pClientScriptManager->GetGUIManager()->Add(pView->GetWindow(), pClientScriptManager->GetScriptingManager()->Get(pVM));
	pView->GetWindow()->setVisible(true);
	script_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIWebView, setURI)
{
 	CEGUI::Window * pWindow = script_getinstance<CEGUI::Window *>(pVM);
	const char * szURL;
	script_getstring(pVM, -1, &szURL);

	CD3D9WebView * pView = g_pWebkit->GetView(pWindow);
	if(pView)
	{
		pView->GetView()->CancelLoad();
		pView->GetView()->SetURI(szURL);
	}

	script_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIWebView, evaluateJavaScript)
{
 	CEGUI::Window * pWindow = script_getinstance<CEGUI::Window *>(pVM);
	const char * script;
	script_getstring(pVM, -1, &script);

	CD3D9WebView * pView = g_pWebkit->GetView(pWindow);
	if(pView)
	{
		EA::WebKit::JavascriptValue * pValue = g_pWebkit->GetWebKit()->CreateJavaScriptValue();
		pView->GetView()->EvaluateJavaScript(script, strlen(script), pValue);
		switch(pValue->GetType())
		{
		case EA::WebKit::JavascriptValueType_Boolean:
			script_pushbool(pVM, pValue->GetBooleanValue());
			break;
		case EA::WebKit::JavascriptValueType_Number:
			script_pushfloat(pVM, (float)pValue->GetNumberValue());
			break;
		//case EA::WebKit::JavascriptValueType_String:
		//	script_pushstring(pVM, pValue->GetStringCharacters());
		default:
			script_pushbool(pVM, false);
		}
	}

	script_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIWebView, sendSignal)
{
 	CEGUI::Window * pWindow = script_getinstance<CEGUI::Window *>(pVM);
	const char * szSignal;
	script_getstring(pVM, -1, &szSignal);

	CD3D9WebView * pView = g_pWebkit->GetView(pWindow);
	if(pView)
	{
		if(strcmp(szSignal, "back") == 0)
		{
			pView->GetView()->GoBack();
		}
		else if(strcmp(szSignal, "forward") == 0)
		{
			pView->GetView()->GoForward();
		}
		else if(strcmp(szSignal, "cancel") == 0)
		{
			pView->GetView()->CancelLoad();
		}
		else if(strcmp(szSignal, "refresh") == 0)
		{
			pView->GetView()->Refresh();
		}
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIWebView, setHTML)
{
 	CEGUI::Window * pWindow = script_getinstance<CEGUI::Window *>(pVM);
	const char * szHTML;
	script_getstring(pVM, -1, &szHTML);

	CD3D9WebView * pView = g_pWebkit->GetView(pWindow);
	if(pView)
	{
		pView->GetView()->SetHTML(szHTML, strlen(szHTML));
	}

	script_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIWebView, setElementText)
{
 	CEGUI::Window * pWindow = script_getinstance<CEGUI::Window *>(pVM);
	const char * text;
	const char * id;
	script_getstring(pVM, -1, &id);
	script_getstring(pVM, -2, &text);

	CD3D9WebView * pView = g_pWebkit->GetView(pWindow);
	if(pView)
	{
		pView->GetView()->SetElementTextById(id, text);
	}

	script_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIWebView, getLoadInfo)
{
 	CEGUI::Window * pWindow = script_getinstance<CEGUI::Window *>(pVM);
	CD3D9WebView * pView = g_pWebkit->GetView(pWindow);
	if(pView)
	{
		CScriptArguments table;
		table.push("completed");
		table.push(pView->GetView()->GetLoadInfo().mbCompleted);
		table.push("content-length");
		table.push((int)pView->GetView()->GetLoadInfo().mContentLength);
		table.push("last-changed-time");
		table.push((int)pView->GetView()->GetLoadInfo().mLastChangedTime);
		table.push("title");
		table.push(0); // ADAMIX: todo
		table.push("status-code");
		table.push(pView->GetView()->GetLoadInfo().mStatusCode);
		table.push("uri");
		table.push(0); // ADAMIX: todo
		script_pusharg(pVM, CScriptArgument(table, false));
	}

	script_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIWebView, clickElement)
{
 	CEGUI::Window * pWindow = script_getinstance<CEGUI::Window *>(pVM);
	const char * szElementOrId;
	script_getstring(pVM, -1, &szElementOrId);

	CD3D9WebView * pView = g_pWebkit->GetView(pWindow);
	if(pView)
	{
		pView->GetView()->ClickElementsByIdOrClass(szElementOrId);
	}

	script_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIWebView, setSize)
{
 	CEGUI::Window * pWindow = script_getinstance<CEGUI::Window *>(pVM);
	int width, height;
	script_getinteger(pVM, -1, &width);
	script_getinteger(pVM, -2, &height);

	CD3D9WebView * pView = g_pWebkit->GetView(pWindow);
	if(pView)
	{
		pView->SetSize(width, height);
	}

	script_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIWebView, registerJavaScriptMethod)
{
 	CEGUI::Window * pWindow = script_getinstance<CEGUI::Window *>(pVM);
	const char * szMethod;
	script_getstring(pVM, -1, &szMethod);

	CD3D9WebView * pView = g_pWebkit->GetView(pWindow);
	if(pView)
	{
		pView->GetView()->RegisterJavascriptMethod(szMethod);
		pView->GetView()->RebindJavascript();
	}

	script_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(GUIWebView, draw)
{
 	CEGUI::Window * pWindow = script_getinstance<CEGUI::Window *>(pVM);
	int x, y, width, height;
	script_getinteger(pVM, -1, &x);
	script_getinteger(pVM, -2, &y);
	script_getinteger(pVM, -3, &width);
	script_getinteger(pVM, -4, &height);

	CD3D9WebView * pView = g_pWebkit->GetView(pWindow);
	if(pView)
	{
		pView->Draw(x, y, width, height);
	}

	script_pushbool(pVM, true);
	return 1;
}

*/