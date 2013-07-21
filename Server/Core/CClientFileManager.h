//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CClientFileManager.h
// Project: Server.Core
// Author(s): mabako
// License: See LICENSE in root directory
//
//==============================================================================

#include <list>
#include "CServer.h"
#include <CFileChecksum.h>
#include "MetaFile.h"

struct CClientFile
{
	CMetaFile		m_MetaFile;
	CFileChecksum	m_Checksum;
};

class CClientFileManager
{
	public:
		std::list<CClientFile>		m_Files;

		bool Start(CMetaFile & mf);
		bool Stop(CMetaFile & mf);
		bool Restart(CMetaFile & mf);
		bool Exists(CMetaFile & mf);
		void HandleClientJoin(EntityId playerId);
		CClientFile * Get(CMetaFile & mf);
		String Refresh(std::vector<CMetaFile> & files);

		CClientFileManager();
		~CClientFileManager() { };
private:
};
