//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CClientFileManager.cpp
// Project: Server.Core
// Author(s): mabako
//            jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CClientFileManager.h"
#include "CNetworkManager.h"
#include "CWebserver.h"
#include <map>
#include <CLogFile.h>

extern CNetworkManager	* g_pNetworkManager;
extern CWebServer		* g_pWebserver;
extern CServer			* g_pServer;

CClientFileManager::CClientFileManager()
{
}

bool CClientFileManager::Start(CMetaFile & mf)
{
	if(Exists(mf))
		return false;

	CFileChecksum fileChecksum;
	
	if(!g_pWebserver->FileCopy(mf, fileChecksum))
	{
		CLogFile::Printf("Failed to copy client file %s/%s to web server.\n", mf.m_Meta.Get(), mf.m_File.Get());
		return false;
	}

	CClientFile cf;
	cf.m_MetaFile = mf;
	cf.m_Checksum = fileChecksum;

	m_Files.push_back(cf);
	CBitStream bsSend;
	bsSend.Write((char)mf.m_Type);
	bsSend.Write(mf.m_Meta);
	bsSend.Write(mf.m_File);
	bsSend.Write((char *)&fileChecksum, sizeof(CFileChecksum));
	g_pNetworkManager->RPC(RPC_NewFile, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
	return true;
}

bool CClientFileManager::Stop(CMetaFile & mf)
{
	for(auto i = m_Files.begin(); i != m_Files.end(); ++i)
	{
		if(i->m_MetaFile == mf)
		{
			// If server isn't active don't delete file - client will do it itself
			if(g_pServer->IsActive())
			{
				CBitStream bsSend;
				bsSend.Write((char)mf.m_Type);
				bsSend.Write(mf.m_Meta);
				bsSend.Write(mf.m_File);
				g_pNetworkManager->RPC(RPC_DeleteFile, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
			}
			m_Files.erase(i);
			return true;
		}
	}
	return false;
}

bool CClientFileManager::Restart(CMetaFile & mf)
{
	if(!Exists(mf))
		return false;

	Stop(mf);
	
	return Start(mf);
}

bool CClientFileManager::Exists(CMetaFile & mf)
{
	for(auto i = m_Files.begin(); i != m_Files.end(); ++i)
	{
		if(i->m_MetaFile == mf)
			return true;
	}
	return false;
}

CClientFile * CClientFileManager::Get(CMetaFile & mf)
{
	for(auto i = m_Files.begin(); i != m_Files.end(); ++i)
	{
		if(i->m_MetaFile == mf)
			return &(*i);
	}
	return NULL;
}

void CClientFileManager::HandleClientJoin(EntityId playerId)
{
	CBitStream bsSend;

	for(auto i = m_Files.begin(); i != m_Files.end(); ++i)
	{
		bsSend.Write((char)i->m_MetaFile.m_Type);
		bsSend.Write(i->m_MetaFile.m_Meta);
		bsSend.Write(i->m_MetaFile.m_File);

		// Write the file checksum
		bsSend.Write((char *)&(i->m_Checksum), sizeof(CFileChecksum));
	}

	// Send the rpc
	g_pNetworkManager->RPC(RPC_NewFile, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
}

String CClientFileManager::Refresh(std::vector<CMetaFile> & files)
{
	int nLoaded=0, nUloaded=0, nRefreshed=0;

	for(auto i = files.begin(); i != files.end(); i++)
	{
		auto s = Get(*i);
		if(s == NULL)
		{
			Start(*i);
			nLoaded++;
		}
		else
		{
			CFileChecksum cs;
			cs.Calculate(i->ToFullPath().Get());
			if(cs != s->m_Checksum)
			{
				Stop(*i);
				Start(*i);
				nRefreshed++;
			}
		}
	}

	for(auto i = m_Files.begin(); i != m_Files.end(); )
	{
		if(std::find_if(begin(files), end(files), [&i](CMetaFile & mf){ return mf == i->m_MetaFile; } ) == files.end())
		{
			Stop((i++)->m_MetaFile);
			nUloaded++;
		}
		else
			i++;
	}
	
	return String("Loaded %d    Unloaded %d    Refreshed %d", nLoaded, nUloaded, nRefreshed);
}
