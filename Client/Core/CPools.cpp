//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CPools.cpp
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "CPools.h"
#include "COffsets.h"

CPools::CPools()
	: m_pPedPool(NULL),
	m_pVehiclePool(NULL),
	m_pTaskPool(NULL),
	m_pCamPool(NULL)
{
	// Clear our custom checkpoint array
	memset(&m_checkpoints, 0, sizeof(m_checkpoints));

	// Modify checkpoint rendering to use our custom array
	/*
	VAR_RenderCheckpoints_FirstCP                   = (uiBase +  0x855966);
	VAR_RenderCheckpoints_LastCP                    = (uiBase +  0x856705);
	*/
	*(DWORD *)(COffsets::VAR_RenderCheckpoints_FirstCP) = (DWORD)m_checkpoints + 0x18;
	*(DWORD *)(COffsets::VAR_RenderCheckpoints_LastCP) = (DWORD)m_checkpoints + 0x18 + CHECKPOINT_ARRAY_SIZE * sizeof(IVCheckpoint);
}

CPools::~CPools()
{
	// Delete game pools
	SAFE_DELETE(m_pCamPool);
	SAFE_DELETE(m_pTaskPool);
	SAFE_DELETE(m_pVehiclePool);
	SAFE_DELETE(m_pPedPool);
}

void CPools::Initialize()
{
    // Initialize game pools
    m_pPedPool = new CIVPool<IVPed>(*(IVPool **)COffsets::VAR_PedPool);
    m_pVehiclePool = new CIVPool<IVVehicle>(*(IVPool **)COffsets::VAR_VehiclePool);
    m_pTaskPool = new CIVPool<IVTask>(*(IVPool **)COffsets::VAR_TaskPool);
    m_pCamPool = new CIVPool<IVCam>(*(IVPool **)COffsets::VAR_CamPool);
    //m_pTrainPool = new CIVPool<IVTrain>(*(IVPool **)COffsets::VAR_TrainPool);
}

IVPlayerInfo * CPools::GetPlayerInfoFromIndex(unsigned int uiIndex)
{
	// Is the index more than the player info array size?
	if(uiIndex > PLAYER_INFO_ARRAY_SIZE)
		return NULL;

	// Get the player info pointer array
	IVPlayerInfo ** pPlayerInfos = (IVPlayerInfo **)COffsets::VAR_PlayerInfoArray;

	// Return the player info pointer
	return pPlayerInfos[uiIndex];
}

IVPlayerInfo * CPools::GetPlayerInfoFromPlayerPed(IVPlayerPed * pPlayerPed)
{
	// Loop through all player info indexes
	for(unsigned int i = 0; i < PLAYER_INFO_ARRAY_SIZE; i++)
	{
		// Get the current player info
		IVPlayerInfo * pPlayerInfo = GetPlayerInfoFromIndex(i);

		// Does the current player info exist?
		if(pPlayerInfo)
		{
			// Is the current player infos player ped the player ped?
			if(pPlayerInfo->m_pPlayerPed == pPlayerPed)
				return pPlayerInfo;
		}
	}

	// Player info not found for player ped
	return NULL;
}

unsigned int CPools::GetIndexFromPlayerInfo(IVPlayerInfo * pPlayerInfo)
{
	// Loop through all player info indexes
	for(unsigned int i = 0; i < PLAYER_INFO_ARRAY_SIZE; i++)
	{
		// Is the current index the player info?
		if(GetPlayerInfoFromIndex(i) == pPlayerInfo)
			return i;
	}

	// Player info not found in array
	return INVALID_PLAYER_PED;
}

void CPools::SetPlayerInfoAtIndex(unsigned int uiIndex, IVPlayerInfo * pPlayerInfo)
{
	// Is the index more than the player info array size?
	if(uiIndex > PLAYER_INFO_ARRAY_SIZE)
		return;

	// Get the player info pointer array
	IVPlayerInfo ** pPlayerInfos = (IVPlayerInfo **)COffsets::VAR_PlayerInfoArray;

	// Set the player info pointer
	pPlayerInfos[uiIndex] = pPlayerInfo;
}

unsigned int CPools::FindFreePlayerInfoIndex()
{
	// Loop through all player info indexes
	for(unsigned int i = 0; i < PLAYER_INFO_ARRAY_SIZE; i++)
	{
		// Is the current index free?
		if(!GetPlayerInfoFromIndex(i))
			return i;
	}

	// No free player info indexes found
	return INVALID_PLAYER_PED;
}

unsigned int CPools::GetLocalPlayerIndex()
{
	return *(unsigned int *)COffsets::VAR_LocalPlayerId;
}

void CPools::SetLocalPlayerIndex(unsigned int uiIndex)
{
	*(unsigned int *)COffsets::VAR_LocalPlayerId = uiIndex;
}

IVCheckpoint * CPools::GetCheckpointFromIndex(unsigned int uiIndex)
{
	// Is the index more than the checkpoint array size?
	if(uiIndex > CHECKPOINT_ARRAY_SIZE)
		return NULL;

	// Return the checkpoint pointer
	return &m_checkpoints[uiIndex];
}

unsigned int CPools::FindFreeCheckpointIndex()
{
	// Loop through all checkpoint indexes
	for(unsigned int i = 0; i < CHECKPOINT_ARRAY_SIZE; i++)
	{
		// Is the current index free?
		if(m_checkpoints[i].m_wType == CHECKPOINT_TYPE_NONE)
			return i;
	}

	// No free checkpoint indexes found
	return INVALID_CHECKPOINT;
}