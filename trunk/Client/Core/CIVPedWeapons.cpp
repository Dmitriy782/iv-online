//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CIVPedWeapons.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CIVPedWeapons.h"
#include "CIVPed.h"
#include "CIVObject.h"
#include "CGame.h"
#include "COffsets.h"
#include <CLogFile.h>

CIVPedWeapons::CIVPedWeapons(IVPedWeapons * pPedWeapons, CIVPed * pPed) : m_pPedWeapons(pPedWeapons), m_pPed(pPed)
{
	m_pWeaponObject = new CIVObject(pPedWeapons->m_pWeaponObject);
	m_pSpecialWeapon = new CIVWeapon(m_pPedWeapons->m_pSpecialWeapon);

	for(int i = 0; i < WEAPON_SLOT_MAX; i++)
		m_pWeaponSlots[i] = new CIVPedWeaponSlot(&m_pPedWeapons->m_weapons[i], this);

	m_pWeapon = new CIVPedWeaponSlot(&m_pPedWeapons->m_weapon, this);
}

CIVPedWeapons::~CIVPedWeapons()
{
	for(int i = 0; i < WEAPON_SLOT_MAX; i++)
		SAFE_DELETE(m_pWeaponSlots[i]);

	SAFE_DELETE(m_pWeapon);
	SAFE_DELETE(m_pSpecialWeapon);
	SAFE_DELETE(m_pWeaponObject);
}

CIVWeapon * CIVPedWeapons::GetCurrentWeapon()
{
	if(m_pPedWeapons)
	{
		IVObject * pWeaponObject = m_pPedWeapons->m_pWeaponObject;

		if(m_pWeaponObject->GetObject() != pWeaponObject)
			m_pWeaponObject->SetObject(pWeaponObject);

		if(pWeaponObject)
			return m_pWeaponObject->GetWeapon();

		eWeaponSlot weaponSlot = m_pPedWeapons->m_currentWeaponSlot;

		if(weaponSlot == WEAPON_SLOT_UNARMED || weaponSlot == WEAPON_SLOT_PARACHUTE)
		{
			IVWeapon * pSpecialWeapon = m_pPedWeapons->m_pSpecialWeapon;

			if(m_pSpecialWeapon->GetWeapon() != pSpecialWeapon)
				m_pSpecialWeapon->SetWeapon(pSpecialWeapon);

			return m_pSpecialWeapon;
		}
	}

	return NULL;
}

void CIVPedWeapons::GiveWeapon(eWeaponType weaponType, DWORD dwAmmo)
{
	if(m_pPedWeapons)
	{

	}
}

eWeaponType CIVPedWeapons::GetWeaponInSlot(eWeaponSlot weaponSlot)
{
	if(m_pPedWeapons)
		return m_pWeaponSlots[weaponSlot]->GetType();

	return WEAPON_TYPE_UNARMED;
}

eWeaponSlot CIVPedWeapons::GetCurrentWeaponSlot()
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	if(m_pPedWeapons)
		return (eWeaponSlot)m_pPedWeapons->m_currentWeaponSlot;

	return WEAPON_SLOT_UNARMED;
}

eWeaponType CIVPedWeapons::GetCurrentWeaponType()
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	if(m_pPedWeapons)
		return m_pWeaponSlots[m_pPedWeapons->m_currentWeaponSlot]->GetType();

	return WEAPON_TYPE_UNARMED;
}

void CIVPedWeapons::SetCurrentWeaponBySlot(eWeaponSlot weaponSlot)
{
	IVPedWeapons * pPedWeapons = m_pPedWeapons;

	if(pPedWeapons)
	{
		if(weaponSlot >= WEAPON_SLOT_MAX)
			return;

		int iUnknown = 0;

		if(m_pPed->IsInVehicle())
			iUnknown = 19;

		IVPed * pGamePed = m_pPed->GetPed();
		DWORD dwFunc = (CGame::GetBase() + 0x9AA330); // CPedWeapons::SetCurrentWeapon
		_asm
		{
			push pGamePed
			push 1
			push weaponSlot
			push iUnknown
			mov ecx, pPedWeapons
			call dwFunc
		}
	}
}

void CIVPedWeapons::SetCurrentWeaponByType(eWeaponType weaponType)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	if(m_pPedWeapons)
	{
		CIVWeaponInfo * pWeaponInfo = CGame::GetWeaponInfo(weaponType);

		if(pWeaponInfo)
		{
			eWeaponSlot weaponSlot = pWeaponInfo->GetWeaponInfo()->m_slot;

			if(m_pWeaponSlots[weaponSlot]->GetType() == weaponType)
				SetCurrentWeaponBySlot(weaponSlot);
		}
	}
}

void CIVPedWeapons::SetCurrentWeaponVisible(bool bVisible)
{
	IVPedWeapons * pPedWeapons = m_pPedWeapons;

	if(pPedWeapons)
	{
		IVPed * pGamePed = m_pPed->GetPed();

		if(bVisible)
		{
			DWORD dwFunc = (CGame::GetBase() + 0x9AC110); // CPedWeapons::ShowWeapon
			_asm
			{
				push 0
				push -1
				push pGamePed
				mov ecx, pPedWeapons
				call dwFunc
			}
		}
		else
		{
			if(pPedWeapons->m_pWeaponObject)
			{
				DWORD dwFunc = (CGame::GetBase() + 0x9ABF00); // CPedWeapons::HideWeapon
				_asm
				{
					push 0
					push 0
					push pGamePed
					mov ecx, pPedWeapons
					call dwFunc
				}
			}
		}
	}
}

void CIVPedWeapons::RemoveWeapon(eWeaponType weaponType, int iUnknown)
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	IVPedWeapons * pPedWeapons = m_pPedWeapons;

	if(pPedWeapons)
	{
		IVPed * pGamePed = m_pPed->GetPed();
		_asm
		{
			push iUnknown
			push weaponType
			push pGamePed
			mov ecx, pPedWeapons
			call COffsets::FUNC_CPedWeapons__RemoveWeapon
		}
	}
}

void CIVPedWeapons::RemoveAllWeapons()
{
#ifdef EXT_LOG
	CLogFile::Printf(__FUNCSIG__);
#endif
	IVPedWeapons * pPedWeapons = m_pPedWeapons;

	if(pPedWeapons)
	{
		IVPed * pGamePed = m_pPed->GetPed();
		_asm
		{
			push pGamePed
			mov ecx, pPedWeapons
			call COffsets::FUNC_CPedWeapons__RemoveAllWeapons
		}
	}
}

DWORD CIVPedWeapons::GetAmmoBySlot(eWeaponSlot weaponSlot)
{
	IVPedWeapons * pPedWeapons = m_pPedWeapons;

	if(pPedWeapons)
	{
		if(weaponSlot >= WEAPON_SLOT_MAX)
			return 0;

		CIVWeapon * pCurrentWeapon = GetCurrentWeapon();

		if(pCurrentWeapon && pCurrentWeapon->GetSlot() == weaponSlot)
			m_pWeaponSlots[weaponSlot]->SetAmmo(pCurrentWeapon->GetAmmoTotal());

		return m_pWeaponSlots[weaponSlot]->GetAmmo();
	}

	return 0;
}

DWORD CIVPedWeapons::GetAmmoByType(eWeaponType weaponType)
{
	if(m_pPedWeapons)
	{
		CIVWeaponInfo * pWeaponInfo = CGame::GetWeaponInfo(weaponType);

		if(pWeaponInfo)
		{
			eWeaponSlot weaponSlot = pWeaponInfo->GetWeaponInfo()->m_slot;

			if(m_pWeaponSlots[weaponSlot]->GetType() == weaponType)
				return GetAmmoBySlot(weaponSlot);
		}
	}

	return 0;
}

void CIVPedWeapons::SetAmmoBySlot(eWeaponSlot weaponSlot, DWORD dwAmmo)
{
	if(m_pPedWeapons)
	{
		if(weaponSlot >= WEAPON_SLOT_MAX)
			return;

		SetAmmoByType(m_pWeaponSlots[weaponSlot]->GetType(), dwAmmo);
	}
}

void CIVPedWeapons::SetAmmoByType(eWeaponType weaponType, DWORD dwAmmo)
{
	IVPedWeapons * pPedWeapons = m_pPedWeapons;

	if(pPedWeapons)
	{
		DWORD dwFunc = (CGame::GetBase() + 0x9A9D60); // CPedWeapons::SetAmmoByType
		_asm
		{
			push dwAmmo
			push weaponType
			mov ecx, pPedWeapons
			call dwFunc
		}
	}
}

DWORD CIVPedWeapons::GetAmmoInClip()
{
	CIVWeapon * pCurrentWeapon = GetCurrentWeapon();

	if(m_pPedWeapons && pCurrentWeapon)
		return pCurrentWeapon->GetAmmoInClip();

	return 0;
}

void CIVPedWeapons::SetAmmoInClip(DWORD dwAmmoInClip)
{
	CIVWeapon * pCurrentWeapon = GetCurrentWeapon();

	if(m_pPedWeapons && pCurrentWeapon)
	{
		WORD wClipSize = pCurrentWeapon->GetClipSize();

		if(dwAmmoInClip > wClipSize)
			dwAmmoInClip = wClipSize;

		DWORD dwCurrentAmmoInClip = pCurrentWeapon->GetAmmoInClip();
		pCurrentWeapon->SetAmmoInClip(dwAmmoInClip);

		DWORD dwCurrentAmmo = pCurrentWeapon->GetAmmoTotal();
		DWORD dwDifference = (dwAmmoInClip - dwCurrentAmmoInClip);

		if(dwDifference >= 25000)
			pCurrentWeapon->SetAmmoTotal(25000);

		DWORD dwNewAmmo = (dwCurrentAmmo + dwDifference);

		if(dwNewAmmo <= 25000)
			pCurrentWeapon->SetAmmoTotal(dwNewAmmo);
		else
			pCurrentWeapon->SetAmmoTotal(25000);
	}
}
