#pragma once
#include "CString.h"
#include "SharedUtility.h"

#define IVMP_RESOURCES_FOLDER "resources\\"

enum MetaFileType : char
{
	MetaFileType_Undefined = 0,
	MetaFileType_ServerScript = 1,
	MetaFileType_ClientScript = 2,
	MetaFileType_ClientResource = 3
};

struct CMetaFile
{
	String			m_Meta;
	String			m_File;
	MetaFileType	m_Type;

	CMetaFile(){m_Type = MetaFileType_Undefined;}

	CMetaFile(String & meta, String & file, MetaFileType type)
	{
		m_Meta = meta;
		m_File = file;
		m_Type = type;
	}

	CMetaFile(const char * meta_slash_file, MetaFileType type)
	{
		String s(meta_slash_file);
		size_t i = s.Find('/');
		m_Meta = s.SubStr(0, i);
		m_File = s.SubStr(i+1, s.GetLength()-i-1);
		m_Type = type;
	}

	CMetaFile(String & meta_slash_file, MetaFileType type)
	{
		size_t i = meta_slash_file.Find('/');
		m_Meta = meta_slash_file.SubStr(0, i);
		m_File = meta_slash_file.SubStr(i+1, meta_slash_file.GetLength()-i-1);
		m_Type = type;
	}

	String TypeToString()
	{
		switch(m_Type)
		{
			case MetaFileType_ServerScript :	return "ServerScript";
			case MetaFileType_ClientScript :	return "ClientScript";
			case MetaFileType_ClientResource :	return "ClientResource";
		}
		return "Undefined";
	}

	String ToString()
	{
		return String("%s/%s (%s)", m_Meta.Get(), m_File.Get(), TypeToString().Get());
	}

	String ToFullPath()
	{
		return SharedUtility::GetAbsolutePath(IVMP_RESOURCES_FOLDER "%s\\%s", m_Meta.Get(), m_File.Get());
	}

	String ToMetaAndFileString()
	{
		return String("%s/%s", m_Meta.Get(), m_File.Get());
	}

	bool operator == (const CMetaFile & a)
	{
		return m_Meta == a.m_Meta && m_File == a.m_File && m_Type == a.m_Type;
	}
};
