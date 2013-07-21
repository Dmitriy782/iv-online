//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: KeySync.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "KeySync.h"
#include "COffsets.h"
#include <Patcher/CPatcher.h>
#include "CIVPed.h"
#include "CIVVehicle.h"
#include "CContextDataManager.h"
#include "CPools.h"
#include <CLogFile.h>

IVPed      * g_pPed = NULL;
IVVehicle  * g_pKeySyncIVVehicle = NULL;
unsigned int g_uiLocalPlayerIndex = 0;
IVPad        g_localPad;
bool         g_bInLocalContext = true;

void ContextSwitch(IVPed * pPed, bool bPost)
{
	// Do we have a valid ped pointer?
	if(pPed)
	{
		// Get the remote players context data
		CContextData * pContextData = CContextDataManager::GetContextData((IVPlayerPed *)pPed);

		// Do we have a valid context data?
		if(pContextData)
		{
			// Is this not the local player?
			if(pContextData->GetPlayerInfo()->GetPlayerNumber() != 0)
			{
				if(!bPost && !g_bInLocalContext)
				{
					CLogFile::Printf("Not switching due to not being in local context!");
					return;
				}

				if(bPost && g_bInLocalContext)
				{
					CLogFile::Printf("Not switching due to being in local context!");
					return;
				}

				// Get the game pad
				CIVPad * pPad = CGame::GetPad();

				if(!bPost)
				{
					// Store the local players index
					g_uiLocalPlayerIndex = CGame::GetPools()->GetLocalPlayerIndex();

					// Store the local players pad
					memcpy(&g_localPad, pPad->GetPad(), sizeof(IVPad));

					// Swap the local player index with the remote players index
					CGame::GetPools()->SetLocalPlayerIndex(pContextData->GetPlayerInfo()->GetPlayerNumber());

					// Set the history values
					for(int i = 0; i < INPUT_COUNT; i++)
					{
						IVPadData * pPadData = &pContextData->GetPad()->GetPad()->m_padData[i];

						if(pPadData->m_pHistory)
						{
							pPadData->m_byteHistoryIndex++;

							if(pPadData->m_byteHistoryIndex >= MAX_HISTORY_ITEMS)
								pPadData->m_byteHistoryIndex = 0;

							pPadData->m_pHistory->m_historyItems[pPadData->m_byteHistoryIndex].m_byteValue = pPadData->m_byteLastValue;
							pPadData->m_pHistory->m_historyItems[pPadData->m_byteHistoryIndex].m_dwLastUpdateTime = CGame::GetTime();
						}
					}

					// Swap the local players pad with the remote players pad
					memcpy(pPad->GetPad(), pContextData->GetPad()->GetPad(), sizeof(IVPad));

					// Flag ourselves as no longer in local context
					g_bInLocalContext = false;
				}
				else
				{
					// Restore the local players pad
					memcpy(pPad->GetPad(), &g_localPad, sizeof(IVPad));

					// Restore the local players index
					CGame::GetPools()->SetLocalPlayerIndex(g_uiLocalPlayerIndex);

					// Flag ourselves as back in local context
					g_bInLocalContext = true;
				}
			}
		}
		//else
			//CLogFile::Printf("ContextSwitch Warning: Invalid Player Ped");
	}
}

void _declspec(naked) CPlayerPed__ProcessInput_Hook()
{
	_asm
	{
		mov g_pPed, ecx
		pushad
	}

	ContextSwitch(g_pPed, false);

	_asm
	{
		popad
		call COffsets::FUNC_CPlayerPed__ProcessInput
		pushad
	}

	ContextSwitch(g_pPed, true);

	_asm
	{
		popad
		ret
	}
}

void _declspec(naked) CAutomobile_ProcessInput_Hook()
{
	_asm
	{
		mov g_pKeySyncIVVehicle, ecx
		pushad
	}

	ContextSwitch(g_pKeySyncIVVehicle->m_pDriver, false);

	_asm
	{
		popad
		call COffsets::FUNC_CAutomobile__ProcessInput
		pushad
	}

	ContextSwitch(g_pKeySyncIVVehicle->m_pDriver, true);

	_asm
	{
		popad
		ret
	}
}

void _declspec(naked) CBike_ProcessInput_Hook()
{
	_asm
	{
		mov g_pKeySyncIVVehicle, ecx
		pushad
	}

	ContextSwitch(g_pKeySyncIVVehicle->m_pDriver, false);

	_asm
	{
		popad
		call COffsets::FUNC_CBike__ProcessInput
		pushad
	}

	ContextSwitch(g_pKeySyncIVVehicle->m_pDriver, true);

	_asm
	{
		popad
		ret
	}
}

void _declspec(naked) CBoat_ProcessInput_Hook()
{
	_asm
	{
		mov g_pKeySyncIVVehicle, ecx
		pushad
	}

	ContextSwitch(g_pKeySyncIVVehicle->m_pDriver, false);

	_asm
	{
		popad
		call COffsets::FUNC_CBoat__ProcessInput
		pushad
	}

	ContextSwitch(g_pKeySyncIVVehicle->m_pDriver, true);

	_asm
	{
		popad
		ret
	}
}

void _declspec(naked) CTrain_ProcessInput_Hook()
{
	_asm
	{
		mov g_pKeySyncIVVehicle, ecx
		pushad
	}

	ContextSwitch(g_pKeySyncIVVehicle->m_pDriver, false);

	_asm
	{
		popad
		call COffsets::FUNC_CTrain__ProcessInput
		pushad
	}

	ContextSwitch(g_pKeySyncIVVehicle->m_pDriver, true);

	_asm
	{
		popad
		ret
	}
}

void _declspec(naked) CHeli_ProcessInput_Hook()
{
	_asm
	{
		mov g_pKeySyncIVVehicle, ecx
		pushad
	}

	ContextSwitch(g_pKeySyncIVVehicle->m_pDriver, false);

	_asm
	{
		popad
		call COffsets::FUNC_CHeli__ProcessInput
		pushad
	}

	ContextSwitch(g_pKeySyncIVVehicle->m_pDriver, true);

	_asm
	{
		popad
		ret
	}
}

void _declspec(naked) CPlane_ProcessInput_Hook()
{
	_asm
	{
		mov g_pKeySyncIVVehicle, ecx
		pushad
	}

	ContextSwitch(g_pKeySyncIVVehicle->m_pDriver, false);

	_asm
	{
		popad
		call COffsets::FUNC_CPlane__ProcessInput
		pushad
	}

	ContextSwitch(g_pKeySyncIVVehicle->m_pDriver, true);

	_asm
	{
		popad
		ret
	}
}

void InstallKeySyncHooks()
{
	// CPlayerPed::ProcessInput
	CPatcher::InstallMethodPatch((COffsets::VAR_CPlayerPed__VFTable + 0x74), (DWORD)CPlayerPed__ProcessInput_Hook);

	// CAutomobile::ProcessInput
	CPatcher::InstallMethodPatch((COffsets::VAR_CAutomobile__VFTable + 0x74), (DWORD)CAutomobile_ProcessInput_Hook);

	// CBike::ProcessInput
	CPatcher::InstallMethodPatch((COffsets::VAR_CBike__VFTable + 0x74), (DWORD)CBike_ProcessInput_Hook);

	// CBoat::ProcessInput
	CPatcher::InstallMethodPatch((COffsets::VAR_CBoat__VFTable + 0x74), (DWORD)CBoat_ProcessInput_Hook);

	// CTrain::ProcessInput
	CPatcher::InstallMethodPatch((COffsets::VAR_CTrain__VFTable + 0x74), (DWORD)CTrain_ProcessInput_Hook);

	// CHeli::ProcessInput
	CPatcher::InstallMethodPatch((COffsets::VAR_CHeli__VFTable + 0x74), (DWORD)CHeli_ProcessInput_Hook);

	// CPlane::ProcessInput
	CPatcher::InstallMethodPatch((COffsets::VAR_CPlane__VFTable + 0x74), (DWORD)CPlane_ProcessInput_Hook);
}
