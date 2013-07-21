//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CBlipManager.cpp
// Project: Server.Core
// Author(s): jenksta
//            Sebihunter
// License: See LICENSE in root directory
//
//==============================================================================

#include "CBlipManager.h"
#include "CNetworkManager.h"
#include "CEvents.h"
#include "CModuleManager.h"
#include "CPlayerManager.h"

extern CNetworkManager * g_pNetworkManager;
extern CEvents * g_pEvents;
extern CModuleManager * g_pModuleManager;
extern CPlayerManager * g_pPlayerManager;

CBlipManager::CBlipManager()
{
	for(EntityId x = 0; x < MAX_BLIPS; x++)
		m_bActive[x] = false;

	for(EntityId y = 0; y < MAX_PLAYERS; y++)
		m_bPlayerActive[y] = false;
}

CBlipManager::~CBlipManager()
{
	for(EntityId x = 0; x < MAX_BLIPS; x++)
	{
		if(m_bActive[x])
			Delete(x);

		m_bActive[x] = false;
	}

	for(EntityId y = 0; y < MAX_PLAYERS; y++)
	{
		if(m_bPlayerActive[y])
			DeleteForPlayer(y);

		m_bPlayerActive[y] = false;
	}
}

EntityId CBlipManager::Create(int iSprite, CVector3 vecPosition, bool bShow)
{
	for(EntityId x = 0; x < MAX_BLIPS; x++)
	{
		if(!m_bActive[x])
		{
			CBitStream bsSend;
			bsSend.WriteCompressed(x);
			bsSend.Write(iSprite);
			bsSend.Write(vecPosition);
			m_Blips[x].uiColor = 0xFFFFFFFF;
			m_Blips[x].fSize = 1.0f;
			m_Blips[x].bRouteBlip = false;
			m_Blips[x].bShortRange = false;
			m_Blips[x].bShow = bShow;
			m_Blips[x].strName = "";
			bsSend.Write(m_Blips[x].uiColor);
			bsSend.Write(m_Blips[x].fSize);
			bsSend.Write(m_Blips[x].bRouteBlip);
			bsSend.Write(m_Blips[x].bShortRange);
			bsSend.Write(m_Blips[x].bShow);
			bsSend.Write(m_Blips[x].strName);
			g_pNetworkManager->RPC(RPC_NewBlip, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
			m_Blips[x].iSprite = iSprite;
			m_Blips[x].vecSpawnPos = vecPosition;
			m_bActive[x] = true;

			CScriptArguments pArguments;
			pArguments.push(x);
			g_pEvents->Call("blipCreate", &pArguments);

			return x;
		}
	}

	return INVALID_ENTITY_ID;
}

void CBlipManager::Delete(EntityId blipId)
{
	if(!DoesExist(blipId))
		return;

	CScriptArguments pArguments;
	pArguments.push(blipId);
	g_pEvents->Call("blipDelete", &pArguments);

	CBitStream bsSend;
	bsSend.WriteCompressed(blipId);
	g_pNetworkManager->RPC(RPC_DeleteBlip, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
	m_bActive[blipId] = false;
}

void CBlipManager::SetPosition(EntityId blipId, CVector3 vecPosition)
{
	if(DoesExist(blipId))
	{
		//Update serverside position
		m_Blips[blipId].vecSpawnPos = vecPosition;
		//Set blip position for clients, therefore delete...
		CBitStream bsSend;
		bsSend.WriteCompressed(blipId);
		g_pNetworkManager->RPC(RPC_DeleteBlip, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
		//...and create a new one
		CBitStream bsSend2;
		bsSend2.WriteCompressed(blipId);
		bsSend2.Write(m_Blips[blipId].iSprite);
		bsSend2.Write(vecPosition);
		bsSend2.Write(m_Blips[blipId].uiColor);
		bsSend2.Write(m_Blips[blipId].fSize);
		bsSend2.Write(m_Blips[blipId].bShortRange);
		bsSend2.Write(m_Blips[blipId].bRouteBlip);
		bsSend2.Write(m_Blips[blipId].bShow);
		bsSend2.Write(m_Blips[blipId].strName);
		g_pNetworkManager->RPC(RPC_NewBlip, &bsSend2, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
	}
}

CVector3 CBlipManager::GetPosition(EntityId blipId)
{
	if(DoesExist(blipId))
		return m_Blips[blipId].vecSpawnPos;

	return CVector3(0.0f, 0.0f, 0.0f);
}

void CBlipManager::SetColor(EntityId blipId, unsigned int uiColor) 
{
	if(DoesExist(blipId))
	{
		m_Blips[blipId].uiColor = uiColor;

		CBitStream bsSend;
		bsSend.Write(blipId);
		bsSend.Write(uiColor);
		g_pNetworkManager->RPC(RPC_ScriptingSetBlipColor, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
	}
}

unsigned int CBlipManager::GetColor(EntityId blipId)
{
	if(DoesExist(blipId))
		return m_Blips[blipId].uiColor;

	return 0;
}

void CBlipManager::SetSize(EntityId blipId, float fSize)
{
	if(DoesExist(blipId) && fSize > 0.0f && fSize <= 4.0f)
	{
		m_Blips[blipId].fSize = fSize;

		CBitStream bsSend;
		bsSend.Write(blipId);
		bsSend.Write(fSize);
		g_pNetworkManager->RPC(RPC_ScriptingSetBlipSize, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
	}
}

float CBlipManager::GetSize(EntityId blipId)
{
	if(DoesExist(blipId))
		return m_Blips[blipId].fSize;

	return 0.0f;
}

void CBlipManager::ToggleShortRange(EntityId blipId, bool bShortRange)
{
	if(DoesExist(blipId))
	{
		m_Blips[blipId].bShortRange = bShortRange;

		CBitStream bsSend;
		bsSend.Write(blipId);
		bsSend.Write(bShortRange);
		g_pNetworkManager->RPC(RPC_ScriptingToggleBlipShortRange, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
	}
}

void CBlipManager::ToggleRoute(EntityId blipId, bool bRoute)
{
	if(DoesExist(blipId))
	{
		m_Blips[blipId].bRouteBlip = bRoute;

		CBitStream bsSend;
		bsSend.Write(blipId);
		bsSend.Write(bRoute);
		g_pNetworkManager->RPC(RPC_ScriptingToggleBlipRoute, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
	}
}

void CBlipManager::SetName(EntityId blipId, String strName)
{
	if(DoesExist(blipId))
	{
		m_Blips[blipId].strName = strName;
		CBitStream bsSend;
		bsSend.Write(blipId);
		bsSend.Write(strName);
		g_pNetworkManager->RPC(RPC_ScriptingSetBlipName, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
	}
}

String CBlipManager::GetName(EntityId blipId)
{
	if(DoesExist(blipId))
		return m_Blips[blipId].strName;

	return "";
}

void CBlipManager::HandleClientJoin(EntityId playerId)
{
	if(GetBlipCount() > 0)
	{
		CBitStream bsSend;

		for(EntityId x = 0; x < MAX_BLIPS; x++)
		{
			if(m_bActive[x])
			{
				bsSend.WriteCompressed(x);
				bsSend.Write(m_Blips[x].iSprite);
				bsSend.Write(m_Blips[x].vecSpawnPos);
				bsSend.Write(m_Blips[x].uiColor);
				bsSend.Write(m_Blips[x].fSize);
				bsSend.Write(m_Blips[x].bShortRange);
				bsSend.Write(m_Blips[x].bRouteBlip);
				bsSend.Write(m_Blips[x].bShow);
				bsSend.Write(m_Blips[x].strName);
				g_pNetworkManager->RPC(RPC_NewBlip, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
				bsSend.Reset();
			}
		}
	}

	if(g_pPlayerManager->GetPlayerCount() > 0)
	{
		CBitStream bsSend;
		for(EntityId y = 0; y < MAX_PLAYERS; y++)
		{
			if(m_bPlayerActive[y])
			{
				bsSend.WriteCompressed(y);
				bsSend.Write(m_PlayerBlips[y].iSprite);
				bsSend.Write(m_PlayerBlips[y].bShortRange);
				bsSend.Write(m_PlayerBlips[y].bShow);
				g_pNetworkManager->RPC(RPC_ScriptingCreatePlayerBlip, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
				bsSend.Reset();
			}
		}
	}
}

bool CBlipManager::DoesExist(EntityId blipId)
{
	if(blipId < 0 || blipId > MAX_BLIPS)
		return false;

	return m_bActive[blipId];
}

EntityId CBlipManager::GetBlipCount()
{
	EntityId blipCount = 0;

	for(EntityId x = 0; x < MAX_BLIPS; x++)
	{
		if(m_bActive[x])
			blipCount++;
	}

	return blipCount;
}

void CBlipManager::SwitchIcon(EntityId blipId, bool bShow, EntityId playerId)
{
	if(DoesExist(blipId))
	{
		m_Blips[blipId].bShow = bShow;
		CBitStream bsSend;
		bsSend.Write(blipId);
		bsSend.Write(bShow);
		if(playerId == INVALID_ENTITY_ID)
			g_pNetworkManager->RPC(RPC_ScriptingSetBlipIcon, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
		else
			g_pNetworkManager->RPC(RPC_ScriptingSetBlipIcon, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, playerId, false);
	}
}

void CBlipManager::CreateForPlayer(EntityId playerId, int iSprite, bool bShow)
{
	// Check if we already have a blip for the player
	if(m_bPlayerActive[playerId])
		return;

	m_bPlayerActive[playerId] = true;
	m_PlayerBlips[playerId].bShortRange = false;
	m_PlayerBlips[playerId].iSprite = iSprite;
	m_PlayerBlips[playerId].bShow = bShow;

	CBitStream bsSend;
	bsSend.WriteCompressed(playerId);
	bsSend.Write(m_PlayerBlips[playerId].iSprite);
	bsSend.Write(m_PlayerBlips[playerId].bShortRange);
	bsSend.Write(m_PlayerBlips[playerId].bShow);
	g_pNetworkManager->RPC(RPC_ScriptingCreatePlayerBlip, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);

}

void CBlipManager::DeleteForPlayer(EntityId playerId)
{
	// Check if we have a blip for the player
	if(!m_bPlayerActive[playerId])
		return;

	CBitStream bsSend;
	bsSend.WriteCompressed(playerId);
	g_pNetworkManager->RPC(RPC_ScriptingRemovePlayerBlip, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
}

void CBlipManager::TogglePlayerShortRange(EntityId playerId, bool bToggle)
{
	// Check if we have a blip for the player
	if(!m_bPlayerActive[playerId])
		return;

	m_PlayerBlips[playerId].bShortRange = bToggle;

	CBitStream bsSend;
	bsSend.WriteCompressed(playerId);
	bsSend.Write(m_PlayerBlips[playerId].iSprite);
	bsSend.Write(m_PlayerBlips[playerId].bShortRange);
	bsSend.Write(m_PlayerBlips[playerId].bShow);
	g_pNetworkManager->RPC(RPC_ScriptingChangePlayerBlip, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
}

void CBlipManager::SetSpriteForPlayer(EntityId playerId, int iToggle)
{
	// Check if we have a blip for the player
	if(!m_bPlayerActive[playerId])
		return;

	m_PlayerBlips[playerId].iSprite = iToggle;

	CBitStream bsSend;
	bsSend.WriteCompressed(playerId);
	bsSend.Write(m_PlayerBlips[playerId].iSprite);
	bsSend.Write(m_PlayerBlips[playerId].bShortRange);
	bsSend.Write(m_PlayerBlips[playerId].bShow);
	g_pNetworkManager->RPC(RPC_ScriptingChangePlayerBlip, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
}

void CBlipManager::TogglePlayerDisplay(EntityId playerId, bool bToggle)
{
	// Check if we have a blip for the player
	if(!m_bPlayerActive[playerId])
		return;

	m_PlayerBlips[playerId].bShow = bToggle;

	CBitStream bsSend;
	bsSend.WriteCompressed(playerId);
	bsSend.Write(m_PlayerBlips[playerId].iSprite);
	bsSend.Write(m_PlayerBlips[playerId].bShortRange);
	bsSend.Write(m_PlayerBlips[playerId].bShow);
	g_pNetworkManager->RPC(RPC_ScriptingChangePlayerBlip, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, INVALID_ENTITY_ID, true);
}

void CBlipManager::TogglePlayerDisplayForPlayer(EntityId playerId, EntityId toPlayerId, bool bToggle)
{
	// Check if we have a blip for the player
	if(!m_bPlayerActive[playerId])
		return;

	CBitStream bsSend;
	bsSend.WriteCompressed(playerId);
	bsSend.Write(m_PlayerBlips[playerId].iSprite);
	bsSend.Write(m_PlayerBlips[playerId].bShortRange);
	bsSend.Write(bToggle);
	g_pNetworkManager->RPC(RPC_ScriptingChangePlayerBlip, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, toPlayerId, false);
}

void CBlipManager::TogglePlayerShortRangeForPlayer(EntityId playerId, EntityId toPlayerId, bool bToggle)
{
	// Check if we have a blip for the player
	if(!m_bPlayerActive[playerId])
		return;

	CBitStream bsSend;
	bsSend.WriteCompressed(playerId);
	bsSend.Write(m_PlayerBlips[playerId].iSprite);
	bsSend.Write(bToggle);
	bsSend.Write(m_PlayerBlips[playerId].bShow);
	g_pNetworkManager->RPC(RPC_ScriptingChangePlayerBlip, &bsSend, PRIORITY_HIGH, RELIABILITY_RELIABLE_ORDERED, toPlayerId, false);
}