//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CChatWindow.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include <winsock2.h>
#include <windows.h>
#include <d3d9.h>
#include "CGUI.h"
#include <Common.h>

#define NUM_PAGES 4
#define MAX_DISPLAYED_MESSAGES 10
#define MAX_MESSAGE MAX_DISPLAYED_MESSAGES * NUM_PAGES
#define MAX_PAGES MAX_MESSAGE / MAX_DISPLAYED_MESSAGES
#define MAX_MESSAGE_LENGTH 128
#define MESSAGE_CHAT_COLOR (CEGUI::colour)D3DCOLOR_RGBA(255, 255, 255, 255)
#define MESSAGE_INFO_COLOR (CEGUI::colour)D3DCOLOR_RGBA(0, 225, 225, 225)
#define MESSAGE_ERROR_COLOR (CEGUI::colour)D3DCOLOR_RGBA(255, 0, 0, 225)
#define MESSAGE_NETWORK_COLOR (CEGUI::colour)D3DCOLOR_RGBA(0, 255, 0, 255)
#define MESSAGE_BACKGROUND_COLOR (CEGUI::colour)D3DCOLOR_RGBA(0, 0, 0, 255)

struct CHAT_MESSAGE
{
	char          szMessage[MAX_MESSAGE_LENGTH + 1];
	char          szName[MAX_NAME_LENGTH + 3];
	CEGUI::colour messageColor;
	CEGUI::colour nameColor;
	float         fNameExtent;
	bool          bAllowFormatting;
};

class CChatWindow
{
private:
	CHAT_MESSAGE  m_chatMessages[MAX_MESSAGE];
	bool          m_bEnabled;
	int           m_iCurrentPage;
	int           m_iMessageAmount;
	unsigned long m_ulBackgroundColor;
	CEGUI::Font * m_pFont;

	void MoveUp();

public:
	CChatWindow();
	~CChatWindow();

	void Draw();
	void AddChatMessage(EntityId playerid, const char * szMessage);
	void AddInfoMessage(const char * szFormat, ...);
	void AddErrorMessage(const char * szFormat, ...);
	void AddNetworkMessage(const char * szFormat, ...);
	void AddInfoDebugMessage(const char * szFormat, ...);
	void AddMessage(DWORD dwColor, bool bAllowFormatting, const char * szMessage, ...);
	bool IsEnabled() { return m_bEnabled; }
	void SetEnabled(bool bEnabled) { m_bEnabled = bEnabled; }
	void PageUp();
	void PageDown();
	void InitFontAndBackground();
};
