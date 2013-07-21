//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CIVCam.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CIVCam.h"
#include <CLogFile.h>

CIVCam::CIVCam()
	: m_pCam(NULL)
{
}

CIVCam::CIVCam(IVCam * pCam)
	: m_pCam(pCam)
{
}

CIVCam::~CIVCam()
{
}

void CIVCam::SetPosition(const CVector3& vecPosition)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	if(m_pCam)
		memcpy(&m_pCam->m_data1.m_matMatrix.vecPosition, &vecPosition, sizeof(CVector3));
}

void CIVCam::GetPosition(CVector3& vecPosition)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	if(m_pCam)
		memcpy(&vecPosition, &m_pCam->m_data1.m_matMatrix.vecPosition, sizeof(CVector3));
}
