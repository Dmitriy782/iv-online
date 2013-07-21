//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CActorManager.h
// Project: Server.Core
// Author(s): jenksta
//            Sebihunter
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "CServer.h"
#include "Interfaces/InterfaceCommon.h"

struct _Actor
{
	int				iModelId;
	CVector3		vecPosition;
	float			fHeading;
	String			strName;
	bool			bTogglename;
	bool			bBlip;
	unsigned int	iColor;
	bool			bFrozen;
	bool			bHelmet;
	bool			bStateincar;
	int				iSeat;

	bool			bDrivingAutomatic;
	EntityId		vehicleId;
	CVector3		vecDrivePos;
	CVector3		vecDriveRot;
	CVector3		vecDriveFinalPos;
};

class CActorManager : public CActorManagerInterface
{
private:
	bool m_bActive[MAX_ACTORS];
	_Actor m_Actors[MAX_ACTORS];

public:
	CActorManager();
	~CActorManager();

	EntityId	Create(int iModelId, CVector3 vecPosition, float fHeading);
	void		Delete(EntityId actorId);
	void		SetPosition(EntityId actorId, CVector3 vecPosition);
	CVector3	GetPosition(EntityId actorId);
	void		SetHeading(EntityId actorId, float fHeading);
	float		GetHeading(EntityId actorId) { return m_Actors[actorId].fHeading; }
	int			GetModel(EntityId actorId) { return m_Actors[actorId].iModelId; }
	void		HandleClientJoin(EntityId playerId);
	void		SetActorName(EntityId actorId, String strName);
	String		GetActorName(EntityId actorId);
	void		SetColor(EntityId actorId, unsigned int iColor);
	unsigned int GetColor(EntityId actorId) { return m_Actors[actorId].iColor; }
	bool		ToggleNametag(EntityId actorId, bool bShow);
	bool		ToggleBlip(EntityId actorId, bool bShow);
	bool		ToggleFrozen(EntityId actorId, bool bFrozen);
	bool		ToggleHelmet(EntityId actorId, bool bHelmet);
	void		WarpIntoVehicle(EntityId actorId, EntityId vehicleId, int iSeatid);
	void		RemoveFromVehicle(EntityId actorId);
	bool		DoesExist(EntityId actorId);
	bool		DriveToCoordinates(EntityId actorId, CVector3 vecDriveTo, CVector3 vecDriveRot);
	bool		UpdateDrivePos(EntityId actorId, CVector3 vecDrivePos, CVector3 vecDriveRot, bool bStopDriving);
	EntityId	GetVehicle(EntityId actorId) { return m_Actors[actorId].vehicleId; }
	EntityId	GetActorCount();
};
