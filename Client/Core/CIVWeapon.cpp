//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CIVWeapon.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CIVWeapon.h"
#include "CGame.h"

CIVWeapon::CIVWeapon(IVWeapon * pWeapon) : m_pWeapon(pWeapon)
{

}

CIVWeapon::~CIVWeapon()
{

}

void CIVWeapon::Initialize(eWeaponType weaponType, DWORD dwAmmo, bool bIsPedWeapon)
{
	IVWeapon * pWeapon = m_pWeapon;
	DWORD dwFunc = (CGame::GetBase() + 0x976C20); // CWeapon::Initialize
	_asm
	{
		push bIsPedWeapon
		push dwAmmo
		push weaponType
		mov ecx, pWeapon
		call dwFunc
	}
}

eWeaponType CIVWeapon::GetType()
{
	if(m_pWeapon)
		return m_pWeapon->m_weaponType;

	return (eWeaponType)0;
}

void CIVWeapon::SetType(eWeaponType weaponType)
{
	if(m_pWeapon)
		m_pWeapon->m_weaponType = weaponType;
}

eWeaponSlot CIVWeapon::GetSlot()
{
	if(m_pWeapon)
	{
		CIVWeaponInfo * pWeaponInfo = CGame::GetWeaponInfo(m_pWeapon->m_weaponType);

		if(pWeaponInfo)
			return pWeaponInfo->GetWeaponInfo()->m_slot;
	}

	return WEAPON_SLOT_UNARMED;
}

eWeaponState CIVWeapon::GetState()
{
	if(m_pWeapon)
		return m_pWeapon->m_weaponState;

	return (eWeaponState)0;
}

void CIVWeapon::SetState(eWeaponState weaponState)
{
	if(m_pWeapon)
		m_pWeapon->m_weaponState = weaponState;
}

bool CIVWeapon::IsPedWeapon()
{
	if(m_pWeapon)
		return (m_pWeapon->m_byteIsPedWeapon != 0);

	return false;
}

WORD CIVWeapon::GetClipSize()
{
	if(m_pWeapon)
	{
		CIVWeaponInfo * pWeaponInfo = CGame::GetWeaponInfo(m_pWeapon->m_weaponType);

		if(pWeaponInfo)
			return pWeaponInfo->GetWeaponInfo()->m_wClipSize;
	}

	return 0;
}

DWORD CIVWeapon::GetAmmoInClip()
{
	DWORD dwAmmoInClip = 0;

	if(m_pWeapon)
		XLivePBufferGetDWORD(m_pWeapon->m_ammoInClip, 0, &dwAmmoInClip);

	return dwAmmoInClip;
}

void CIVWeapon::SetAmmoInClip(DWORD dwAmmoInClip)
{
	if(m_pWeapon)
	{
		WORD wClipSize = GetClipSize();

		if(dwAmmoInClip > wClipSize)
			dwAmmoInClip = wClipSize;

		XLivePBufferSetDWORD(m_pWeapon->m_ammoInClip, 0, dwAmmoInClip);
	}
}

DWORD CIVWeapon::GetAmmoTotal()
{
	DWORD dwAmmoTotal = 0;

	if(m_pWeapon)
		XLivePBufferGetDWORD(m_pWeapon->m_ammoTotal, 0, &dwAmmoTotal);

	return dwAmmoTotal;
}

void CIVWeapon::SetAmmoTotal(DWORD dwAmmoTotal)
{
	if(m_pWeapon)
	{
		if(dwAmmoTotal > 25000)
			dwAmmoTotal = 25000;

		XLivePBufferSetDWORD(m_pWeapon->m_ammoTotal, 0, dwAmmoTotal);
	}
}

void CIVWeapon::Reload()
{
	DWORD dwAmmoTotal = GetAmmoTotal();

	if(dwAmmoTotal)
	{
		WORD wClipSize = GetClipSize();

		if(dwAmmoTotal >= wClipSize)
			SetAmmoInClip(wClipSize);
		else
			SetAmmoInClip(dwAmmoTotal);

		SetState(WEAPON_STATE_READY);
	}
	else
		SetState(WEAPON_STATE_OUT_OF_AMMO);
}
