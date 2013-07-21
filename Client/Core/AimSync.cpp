//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: AimSync.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "AimSync.h"
#include "CClient.h"
#include "COffsets.h"
#include <Patcher/CPatcher.h>
#include "CIVPed.h"
#include "CIVPlayerPed.h"
#include "CContextDataManager.h"
#include <CLogFile.h>

extern CClient * g_pClient;

DWORD      dwFunc = NULL;
IVPed    * g_pIKPed = NULL;
float      g_fArmHeading = 0.0f;
float      g_fArmUpDown = 0.0f;
IVPed    * g_pWeaponPed = NULL;
CVector3 * g_vecWeaponShotSource = NULL;
CVector3 * g_vecWeaponShotTarget = NULL;

void StoreArmHeadingUpDown(IVPed * pPed, float * fArmHeading, float * fArmUpDown)
{
	// Do we have a valid ped pointer?
	if(pPed)
	{
		// Get the remote players context data
		CContextData * pContextData = CContextDataManager::GetContextData((IVPlayerPed *)pPed);

		// Do we have a valid context data?
		if(pContextData)
		{
			// Is this the local player?
			if(pContextData->GetPlayerInfo()->GetPlayerNumber() == 0)
			{
				pContextData->SetArmHeading(*fArmHeading);
				pContextData->SetArmUpDown(*fArmUpDown);
			}
			else
			{
				pContextData->GetArmHeading(*fArmHeading);
				pContextData->GetArmUpDown(*fArmUpDown);
			}
		}
		else
			CLogFile::PrintDebugf("StoreArmHeadingUpDown Warning: Invalid Player Ped");
	}
}

void StoreShotSourceTarget(IVPed * pPed, CVector3 * pWeaponSource, CVector3 * pWeaponTarget)
{
	// Do we have a valid ped pointer, source pointer and target pointer?
	if(pPed && pWeaponSource && pWeaponTarget)
	{
		// Get the remote players context data
		CContextData * pContextData = CContextDataManager::GetContextData((IVPlayerPed *)pPed);

		// Do we have a valid context data?
		if(pContextData)
		{
			// Is this the local player?
			if(pContextData->GetPlayerInfo()->GetPlayerNumber() == 0)
			{
				pContextData->SetWeaponShotSource(*pWeaponSource);
				pContextData->SetWeaponShotTarget(*pWeaponTarget);
			}
			else
			{
				pContextData->GetWeaponShotSource(*pWeaponSource);
				pContextData->GetWeaponShotTarget(*pWeaponTarget);
			}
		}
		else
			CLogFile::PrintDebugf("StoreShotSourceTarget Warning: Invalid Player Ped");
	}
}

// Hook for CIkManager::PointArms func to sync the arm heading and up/down for aim sync
void _declspec(naked) CIkManager__PointArms_Hook()
{
	_asm
	{
		// Get the ped pointer from the ik manager
		mov eax, [ecx+40h] // CIkManager + 0x40 = CPed * pPed
		mov g_pIKPed, eax
		// Get the function arguments
		mov eax, [esp+4]
		mov g_fArmHeading, eax
		mov eax, [esp+8]
		mov g_fArmUpDown, eax
		pushad
	}

	// Store our values
	StoreArmHeadingUpDown(g_pIKPed, &g_fArmHeading, &g_fArmUpDown);
	dwFunc = (CGame::GetBase() + 0x94E866);

	_asm
	{
		popad
		// Store our values in case they have been changed
		mov eax, g_fArmHeading
		mov [esp+4], eax
		mov eax, g_fArmUpDown
		mov [esp+8], eax
		// Execute original code we overwrote for the hook
		movss xmm0, [esp+4]
		// Jump back to the original function
		jmp dwFunc
	}
}

// Hook for CWeapon::Fire func to sync the shot source and target vectors for shot sync
void _declspec(naked) CWeapon__Fire_Hook()
{
	_asm
	{
		push ebp
		mov ebp, esp
		// Get the ped pointer and other function arguments
		mov eax, [ebp+8h]
		mov g_pWeaponPed, eax
		// [ebp+0Ch] = pSourceMatrix (Matrix34 *)
		mov eax, [ebp+10h]
		mov g_vecWeaponShotSource, eax
		mov eax, [ebp+14h]
		mov g_vecWeaponShotTarget, eax
		pop ebp
		pushad
	}

	// Store our values
	StoreShotSourceTarget(g_pWeaponPed, g_vecWeaponShotSource, g_vecWeaponShotTarget);
	dwFunc = (CGame::GetBase() + 0x97D7C6);

	_asm
	{
		popad
		// Execute original code we overwrote for the hook
		push ebp
		mov ebp, esp
		and esp, 0FFFFFFF0h
		// Jump back to the original function
		jmp dwFunc
	}
}

void InstallAimSyncHooks()
{
	// Hook for the CIkManager::PointArms function
	CPatcher::InstallJmpPatch((CGame::GetBase() + 0x94E860), (DWORD)CIkManager__PointArms_Hook);

	// Hook for the CWeapon::Fire function
	CPatcher::InstallJmpPatch((CGame::GetBase() + 0x97D7C0), (DWORD)CWeapon__Fire_Hook);
}
