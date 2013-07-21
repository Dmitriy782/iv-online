//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CIVObject.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CIVObject.h"

CIVObject::CIVObject(IVObject * pObject) : m_pObject(pObject)
{
	m_pWeapon = new CIVWeapon(pObject ? pObject->m_pWeapon : NULL);
}

CIVObject::~CIVObject()
{
	SAFE_DELETE(m_pWeapon);
}

CIVWeapon * CIVObject::GetWeapon()
{
	if(m_pObject)
	{
		if(m_pWeapon->GetWeapon() != m_pObject->m_pWeapon)
			m_pWeapon->SetWeapon(m_pObject->m_pWeapon);

		return m_pWeapon;
	}

	return NULL;
}
