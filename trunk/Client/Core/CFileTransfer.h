//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CFileTransfer.h
// Project: Client.Core
// Author(s):	jenksta
//				CrackHD
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "CFileChecksum.h"
#include <Network\CHttpClient.h>
#include "../../Shared/MetaFile.h"

class CFileTransfer
{
private:
	bool			m_bIsResource;
	CMetaFile		m_MetaFile;
	//String			m_strFolderName;
	CHttpClient		m_httpClient;
	String			m_strHttpUrl;
	String			m_strPath;
	CFileChecksum	m_checksum;
	bool			m_bComplete;
	bool			m_bSucceeded;
	String			m_strError;

public:
	CFileTransfer(String strHost, unsigned short usPort, CMetaFile & mf, CFileChecksum checksum);
	~CFileTransfer();

	CMetaFile & GetMetaFile() { return m_MetaFile; }
	String GetHttpUrl() { return m_strHttpUrl; }
	String GetPath() { return m_strPath; }
	bool   IsComplete() { return m_bComplete; }
	bool   HasSucceeded() { return m_bSucceeded; }
	String GetError() { return m_strError; }
	bool   Download();
};
