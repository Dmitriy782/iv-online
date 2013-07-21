//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CFileTransfer.cpp
// Project: Client.Core
// Author(s):	jenksta
//				CrackHD
// License: See LICENSE in root directory
//
//==============================================================================

#include "CFileTransfer.h"
#include <CLogFile.h>

CFileTransfer::CFileTransfer(String strHost, unsigned short usPort, CMetaFile & mf, CFileChecksum checksum)
							: m_MetaFile(mf), m_checksum(checksum), 
							  m_bComplete(false), m_bSucceeded(false), m_strError("None")
{
	// Set up a local file/directory for this resource
/*
	String m_strFolderName = "clientscripts";

	if(bIsResource)
		m_strFolderName = "resources";
*/
	m_strPath.Set(SharedUtility::GetAbsolutePath("clientfiles\\"IVMP_RESOURCES_FOLDER"\\%s\\%s", m_MetaFile.m_Meta.Get(), m_MetaFile.m_File.Get()));

	// Set up our http client:
	m_httpClient = CHttpClient();
	m_httpClient.SetHost(strHost);
	m_httpClient.SetPort(usPort);
	m_strHttpUrl.Format("/resources/%s/%s", mf.m_Meta.Get(), m_MetaFile.m_File.Get());
}

CFileTransfer::~CFileTransfer() { }

bool CFileTransfer::Download()
{
	// Does the file already exist?
	if(SharedUtility::Exists(m_strPath))
	{
		// Does the checksum of our existing file match our target checksum?
		CFileChecksum fileChecksum;
		fileChecksum.Calculate(m_strPath);

		if(fileChecksum == m_checksum)
		{
			m_bComplete = true;
			m_bSucceeded = true;
			m_strError = "None";
			return true;
		}
	}

	auto metafolder = SharedUtility::GetAbsolutePath("clientfiles\\"IVMP_RESOURCES_FOLDER"\\%s", m_MetaFile.m_Meta.Get());
	if(!SharedUtility::Exists(metafolder))
		SharedUtility::CreateDirectoryA(metafolder);

	// Attempt to open our destination file
	FILE * fFile = fopen(m_strPath.Get(), "wb");

	if(!fFile)
	{
		m_bComplete = true;
		m_bSucceeded = false;
		m_strError.Format("Failed to open destination file %s for file %s", m_MetaFile.ToMetaAndFileString().Get(), m_strPath.Get());
		return false;
	}

	// Send a request:
	if(!m_httpClient.Get(m_strHttpUrl))
	{
		m_bComplete = true;
		m_bSucceeded = false;
		m_strError.Format("Http GET failed for file %s (%s)", m_MetaFile.ToMetaAndFileString().Get(), m_httpClient.GetLastErrorString().Get());
		return false;
	}
	
	// Download data to file:
	m_httpClient.SetFile(fFile);
	while(m_httpClient.IsBusy())
			m_httpClient.Process();

	// Close our downloaded file
	fclose(fFile);

	// Calculate our downloaded file checksum
	CFileChecksum fileChecksum;
	fileChecksum.Calculate(m_strPath);

	// Check the checksum of the downloaded file against our target checksum
	if(fileChecksum != m_checksum)
	{
		m_bComplete = true;
		m_bSucceeded = false;
		m_strError.Format("Checksum mismatch for file %s (Expected 0x%x, Got 0x%x)", m_MetaFile.ToMetaAndFileString().Get(), m_checksum.GetChecksum(), fileChecksum.GetChecksum());
		return false;
	}

	// File was downloaded successfully
	m_bComplete = true;
	m_bSucceeded = true;
	m_strError = "None";
	return true;
}
