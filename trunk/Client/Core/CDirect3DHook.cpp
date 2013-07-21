//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CDirect3DHook.cpp
// Project: Client.Core
// Author(s): jenksta, XForce
// License: See LICENSE in root directory
//
//==============================================================================

#include "CDirect3DHook.h"
#include <Patcher/CPatcher.h>
#include <windowsx.h>
#include <CLogFile.h>
#include "Input.h"
#include "CClient.h"

extern CClient * g_pClient;

bool CDirect3DHook::m_bHookInstalled = false;
bool CDirect3DHook::m_bInitialized = false;

CDirect3DHook::Reset_t CDirect3DHook::m_pReset;
CDirect3DHook::EndScene_t CDirect3DHook::m_pEndScene;

extern WNDPROC m_wWndProc;

DWORD dwReset = NULL;
DWORD dwEndScene = NULL;

bool bDataCompare(const BYTE* pData, const BYTE* bMask, const char* szMask)
{
	for(;*szMask;++szMask,++pData,++bMask)
		if(*szMask=='x' && *pData!=*bMask )
			return false;
	return (*szMask) == NULL;
}


DWORD dwFindPattern(DWORD dwAddress,DWORD dwLen,BYTE *bMask,char * szMask)
{
	for(DWORD i=0; i < dwLen; i++)
		if( bDataCompare( (BYTE*)( dwAddress+i ),bMask,szMask) )
			return (DWORD)(dwAddress+i);
	return 0;
}


void CDirect3DHook::Install()
{
	if(!m_bHookInstalled)
	{
		HMODULE hModule = NULL;
		char buffer[MAX_PATH];

		// Getting path to system dir and to d3d9.dll
		::GetSystemDirectory(buffer,MAX_PATH);

		// Append dll name
		strcat(buffer,"\\d3d9.dll");
		LoadLibrary(buffer);
		while( !hModule )
		{
			hModule = GetModuleHandleA(buffer);
			Sleep(100);
		}

		DWORD* VTableStart = 0;
		DWORD DevicePointer = dwFindPattern((DWORD)hModule, 0x128000,
			(PBYTE)"\xC7\x06\x00\x00\x00\x00\x89\x86\x00\x00\x00\x00\x89\x86", "xx????xx????xx");
		memcpy(&VTableStart, (void*)(DevicePointer+2), 4);

		dwReset = (DWORD)VTableStart[16];
		dwEndScene = (DWORD)VTableStart[42];

		m_pReset = (Reset_t)DetourFunction((PBYTE)dwReset,(PBYTE)hkReset);
		m_pEndScene = (EndScene_t)DetourFunction((PBYTE)dwEndScene,(PBYTE)hkEndScene);

		CLogFile::Printf("Hooked 'Direct3D Reset and EndScene");
	

		m_bHookInstalled = true;
	}
}

void CDirect3DHook::Uninstall()
{
	if(m_bHookInstalled)
	{
		m_bHookInstalled = false;
	}
}
bool bReset = false;

HRESULT WINAPI CDirect3DHook::hkEndScene(LPDIRECT3DDEVICE9 pDevice)
{
	if(m_bInitialized == false)
	{
		HWND hFocusWindow = FindWindow(NULL,"GTAIV");
		if(hFocusWindow != NULL) {
			SetWindowText(hFocusWindow, MOD_NAME DEBUG_IDENTIFIER);

			// Disable minimize & maximize box
			SetWindowLong(hFocusWindow, GWL_STYLE, GetWindowLong(hFocusWindow, GWL_STYLE) & ~WS_MINIMIZEBOX & ~WS_MAXIMIZEBOX);

			if(!m_wWndProc)
			{
				CLogFile::Printf("SubclassWindow");
				m_wWndProc = SubclassWindow(hFocusWindow, WndProc_Hook);
				// jenksta: wtf?
				g_pClient->OnD3DLostDevice();
				g_pClient->OnD3DResetDevice();
				m_bInitialized = true;
			}
		}
	}

	if(!bReset)
		g_pClient->OnD3DEndScene();

	return m_pEndScene(g_pClient->GetDevice());
}

D3DPRESENT_PARAMETERS parameters;
HRESULT WINAPI CDirect3DHook::hkReset(LPDIRECT3DDEVICE9 pDevice,D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	// jenksta: todo: CreateDevice hook that sets this
	if(!g_pClient->GetDevice())
		g_pClient->SetDevice(pDevice);

	CLogFile::Printf("PreD3DReset");
	
	// Call our lost device function
	g_pClient->OnD3DLostDevice();
	
	if(g_pClient->IsWindowedMode())
	{
		pPresentationParameters->Windowed = 1;
		pPresentationParameters->Flags = 0;
		pPresentationParameters->FullScreen_RefreshRateInHz = 0;
		pPresentationParameters->PresentationInterval = 0;
		LONG_PTR style = GetWindowLongPtr(pPresentationParameters->hDeviceWindow, GWL_STYLE);
		SetWindowLongPtr(pPresentationParameters->hDeviceWindow, GWL_STYLE, style | WS_POPUPWINDOW | WS_CAPTION | WS_THICKFRAME);
		SetWindowPos(pPresentationParameters->hDeviceWindow, HWND_NOTOPMOST, 0, 0, pPresentationParameters->BackBufferWidth, pPresentationParameters->BackBufferHeight, SWP_SHOWWINDOW);
	}

	HRESULT hr = m_pReset(g_pClient->GetDevice(), pPresentationParameters);

	if(SUCCEEDED(hr))
	{
		// Call our reset device function
		g_pClient->OnD3DResetDevice();

		CLogFile::Printf("PostD3DReset(1)");
	}
	else
	{
		CLogFile::Printf("PostD3DReset(0)");
	}
	return hr;
}