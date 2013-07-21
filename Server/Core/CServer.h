//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CServer.h
// Project: Server.Core
// Author(s): jenksta
//            MaVe
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include <Server/CServerInterface.h>
#include <Threading/CMutex.h>
#include <queue>
#include <vector>
#include "../../Shared/MetaFile.h"

// move start
#ifndef WIN32
#include <string.h>
#include <sys/time.h>

typedef bool BOOL;
typedef unsigned long DWORD;
typedef unsigned short WORD;
typedef char *PCHAR;
typedef void *PVOID;
typedef unsigned char BYTE;

#define TRUE (1)
#define FALSE (0)

#define stricmp strcasecmp
#define Sleep(ms) usleep(ms * 1000)
#endif
// move end

class CServer : public CServerInterface
{
	private:
		bool               m_bActive;
		bool               m_bShowFPS;
		unsigned long      m_ulLastFPSUpdateTime;
		unsigned long      m_ulFrameCount;
		unsigned long      m_ulFramesPerSecond;
		CMutex             m_inputQueueMutex; // Mutex for m_inputQueue
		std::queue<String> m_inputQueue;

		void					ProcessInputQueue();

	public:
		CServer();
		~CServer();

		int m_iResourcesLoaded;
		int m_iFailedResources;

		bool					OnLoad(int argc, char ** argv);
		void					Process();
		void					OnUnload();
		void					SetActive(bool bActive) { m_bActive = bActive; }
		bool					IsActive() { return m_bActive; }
		void					SendInput(String strInput); // Not threadsafe
		void					AddToInputQueue(String strString); // Threadsafe
		void					SetTitle(String strTitle);
		
		CMetaFile				GetMetaFile(String & meta_and_file);
		std::vector<CMetaFile>	GetServerScripts(String & meta);
		std::vector<CMetaFile>	GetClientResources(String & meta);
		
		bool					StartServerScript(CMetaFile & mf);
		bool					StartClientResource(CMetaFile & mf);
		bool					StopServerScript(CMetaFile & mf);
		bool					StopClientResource(CMetaFile & mf);
		bool					RestartServerScript(CMetaFile & mf);
		bool					RestartClientResource(CMetaFile & mf);
		
		void					StartMeta(String & name);
		void					StopMeta(String & meta);
		void					RestartMeta(String & meta);

		void					RefreshResources();
};
