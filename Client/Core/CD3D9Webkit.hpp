//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CD3D9Webkit.hpp
// Project: Client.Core
// Author(s): adamix
// License: See LICENSE in root directory
//
//==============================================================================

#define _CHAR16T
#include <d3d9.h>
#include <d3dx9.h>
#include <string.h>
#include <stdio.h>

#include <EAWebkit/EAWebkit.h>
#include <EAWebkit/EAWebkitAllocator.h>
#include <EAWebKit/EAWebKitTextInterface.h>
#include <EAWebkit/EAWebKitViewNotification.h>


#include <CLibrary.h>
#include <SharedUtility.h>

#include "CGUI.h"
#include "CMainMenu.h"
#include <CLogFile.h>

class CD3D9WebkitNotification : public EA::WebKit::ViewNotification
{
public:
	virtual bool ViewUpdate      		(EA::WebKit::ViewUpdateInfo&);
	virtual bool LoadUpdate      		(EA::WebKit::LoadInfo&);
	virtual bool TextInputState  		(EA::WebKit::TextInputStateInfo&);
	virtual bool JavascriptMethodInvoked(EA::WebKit::JavascriptMethodInvokedInfo&);
};

class CD3D9WebView
{
public:
	CD3D9WebView(int width, int height, EA::WebKit::View * view);
	void RecreateTexture();
	void SetSize(int width, int height);
	void SetPosition(CEGUI::UVector2 & vec);
	void SetData(void * buffer);
	void Draw(int x, int y, int width, int height);
	EA::WebKit::View * GetView();
	IDirect3DTexture9 * GetTexture();
	CEGUI::Window * GetWindow();
	String GetName();
private:
	LPDIRECT3DDEVICE9 device;
	IDirect3DTexture9 * texture;
	ID3DXSprite * sprite;
	CGUIStaticImage * image;
	String name;

	EA::WebKit::View * view;

	int width;
	int height;
	CEGUI::UVector2 pos;

	int texturenum;
};


class CD3D9WebKit
{
public:
	CD3D9WebKit();
	~CD3D9WebKit();
	void RenderAll(bool bSetData = true, bool bTick = true);
	EA::WebKit::IEAWebkit * GetWebKit();
	CD3D9WebView * GetView(CEGUI::Window * window);
	CD3D9WebView * GetView(EA::WebKit::View * view);
	CD3D9WebView * GetView(String name);
	CD3D9WebView * CreateView(int width, int height, LPDIRECT3DDEVICE9 device);
	void DestroyView(CD3D9WebView * webView);
private:
	EA::WebKit::IEAWebkit * webkit;
	EA::WebKit::IGlyphCache * glyph_cache;
	EA::WebKit::IFontServer * font_server;
	EA::WebKit::IFontStyle * font_style;
	EA::Raster::IEARaster * raster;

	CLibrary * m_pLibrary;

	unsigned long tickCount;
	
	std::list<CD3D9WebView*> views;

	CD3D9WebkitNotification * m_pNotification;
};
