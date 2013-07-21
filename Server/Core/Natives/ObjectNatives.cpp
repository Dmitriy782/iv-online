//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: ObjectNatives.cpp
// Project: Server.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "../Natives.h"
#include "Scripting/CScriptingManager.h"
#include "../CObjectManager.h"
#include "../CNetworkManager.h"

extern CObjectManager * g_pObjectManager;
extern CNetworkManager * g_pNetworkManager;

// Object functions

void CObjectNatives::Register(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterFunction("createObject", Create, 7, "iffffff");
	pScriptingManager->RegisterFunction("deleteObject", Delete, 1, "i");
	pScriptingManager->RegisterFunction("getObjectModel", GetModel, 1, "i");
	pScriptingManager->RegisterFunction("setObjectCoordinates", SetCoordinates, 4, "ifff");
	pScriptingManager->RegisterFunction("getObjectCoordinates", GetCoordinates, 1, "i");
	pScriptingManager->RegisterFunction("setObjectPosition", SetCoordinates, 4, "ifff");
	pScriptingManager->RegisterFunction("getObjectPosition", GetCoordinates, 1, "i");
	pScriptingManager->RegisterFunction("setObjectRotation", SetRotation, 4, "ifff");
	pScriptingManager->RegisterFunction("getObjectRotation", GetRotation, 1, "i");
	pScriptingManager->RegisterFunction("createExplosion", CreateExplosion, 4, "ffff");
	pScriptingManager->RegisterFunction("createFire", CreateFire, 4, "ffff");
	pScriptingManager->RegisterFunction("deleteFire", DeleteFire, 1, "i");
	pScriptingManager->RegisterFunction("attachObjectToPlayer", AttachPed, 8, "iiffffff");
	pScriptingManager->RegisterFunction("attachObjectToPlayerBone", AttachPedBone, 9, "iiffffffi");
	pScriptingManager->RegisterFunction("attachObjectToVehicle", AttachVehicle, 8, "iiffffff");
	pScriptingManager->RegisterFunction("detachObject", DetachObject, 1, "i");
	pScriptingManager->RegisterFunction("moveObject", MoveObject, -1, NULL);
	pScriptingManager->RegisterFunction("rotateObject", RotateObject, 5, "ifffi");
	pScriptingManager->RegisterFunction("setObjectDimension", SetDimension, 2, "ii");
	pScriptingManager->RegisterFunction("getObjectDimension", GetDimension, 1, "i");
	pScriptingManager->RegisterFunction("setObjectInterior", SetInterior, 2, "ii");
	pScriptingManager->RegisterFunction("getObjectInterior", GetInterior, 1, "i");
}

// createObject(modelhash, x, y, z, rx, ry, rz)
int CObjectNatives::Create(lua_State * pVM)
{
	int modelhash;
	CVector3 vecPosition;
	CVector3 vecRotation;
	script_getinteger(pVM, -7, &modelhash);
	script_getvector3(pVM, -6, &vecPosition);
	script_getvector3(pVM, -3, &vecRotation);
	script_pushentity(pVM, g_pObjectManager->Create(modelhash, vecPosition, vecRotation));
	return 1;
}

// deleteObject(objectid)
int CObjectNatives::Delete(lua_State * pVM)
{
	EntityId objectId;
	script_getentity(pVM, -1, &objectId);

	if(g_pObjectManager->DoesExist(objectId))
	{
		g_pObjectManager->Delete(objectId);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// getObjectModel(objectid)
int CObjectNatives::GetModel(lua_State * pVM)
{
	EntityId objectId;
	script_getentity(pVM, 1, &objectId);

	if(g_pObjectManager->DoesExist(objectId))
	{
		script_pushinteger(pVM, g_pObjectManager->GetModel(objectId));
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// setObjectCoordinates(objectid, x, y, z)
int CObjectNatives::SetCoordinates(lua_State * pVM)
{
	EntityId objectId;
	script_getentity(pVM, 1, &objectId);

	if(g_pObjectManager->DoesExist(objectId))
	{
		CVector3 vecPosition;
		script_getvector3(pVM, 2, &vecPosition);
		script_pushbool(pVM, g_pObjectManager->SetPosition(objectId, vecPosition));
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// getObjectCoordinates(objectid)
int CObjectNatives::GetCoordinates(lua_State * pVM)
{
	EntityId objectId;
	script_getentity(pVM, 1, &objectId);

	if(g_pObjectManager->DoesExist(objectId))
	{
		CVector3 vecPosition;

		if(g_pObjectManager->GetPosition(objectId, vecPosition))
		{
			CScriptArray args(pVM);
			args.push(vecPosition.fX);
			args.push(vecPosition.fY);
			args.push(vecPosition.fZ);
			//script_pusharg(pVM, CScriptArgument(args, true));
			//CScriptArgument(args, true).push(pVM);
			return 1;
		}
	}

	script_pushbool(pVM, false);
	return 1;
}


// setObjectRotation(objectid, x, y, z)
int CObjectNatives::SetRotation(lua_State * pVM)
{
	EntityId objectId;
	script_getentity(pVM, 1, &objectId);

	if(g_pObjectManager->DoesExist(objectId))
	{
		CVector3 vecPosition;
		script_getvector3(pVM, 2, &vecPosition);
		script_pushbool(pVM, g_pObjectManager->SetRotation(objectId, vecPosition));
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// getObjectRotation(objectid)
int CObjectNatives::GetRotation(lua_State * pVM)
{
	EntityId objectId;
	script_getentity(pVM, 1, &objectId);

	if(g_pObjectManager->DoesExist(objectId))
	{
		CVector3 vecRotation;

		if(g_pObjectManager->GetRotation(objectId, vecRotation))
		{
			CScriptArray args(pVM);
			args.push(vecRotation.fX);
			args.push(vecRotation.fY);
			args.push(vecRotation.fZ);
			//script_pusharg(pVM, CScriptArgument(args, true));
			//CScriptArgument(args, true).push(pVM);
			return 1;
		}
	}

	script_pushbool(pVM, false);
	return 1;
}

int CObjectNatives::CreateExplosion(lua_State * pVM)
{
	CVector3 vecPos;
	float fdensity;
	script_getfloat(pVM,-4,&vecPos.fX);
	script_getfloat(pVM,-3,&vecPos.fY);
	script_getfloat(pVM,-2,&vecPos.fZ);
	script_getfloat(pVM,-1,&fdensity);
	g_pObjectManager->CreateExplosion(vecPos, fdensity);

	script_pushbool(pVM,true);
	return 1;
}

int CObjectNatives::CreateFire(lua_State *pVM)
{
	CVector3 vecPos;
	float fdensity;
	script_getfloat(pVM,-4,&vecPos.fX);
	script_getfloat(pVM,-3,&vecPos.fY);
	script_getfloat(pVM,-2,&vecPos.fZ);
	script_getfloat(pVM,-1,&fdensity);

	script_pushentity(pVM, g_pObjectManager->CreateFire(vecPos,fdensity));
	return 1;
}

int CObjectNatives::DeleteFire(lua_State *pVM)
{
	EntityId fireId;
	script_getentity(pVM,-1,&fireId);

	g_pObjectManager->DeleteFire(fireId);
	return 1;
}

int CObjectNatives::AttachPed(lua_State *pVM)
{
	EntityId	objectId;
	EntityId	playerId;
	CVector3	vecPos;
	CVector3	vecRot;

	script_getentity(pVM,-8,&objectId);
	script_getentity(pVM,-7,&playerId);
	script_getfloat(pVM,-6,&vecPos.fX);
	script_getfloat(pVM,-5,&vecPos.fY);
	script_getfloat(pVM,-4,&vecPos.fZ);
	script_getfloat(pVM,-3,&vecRot.fX);
	script_getfloat(pVM,-2,&vecRot.fY);
	script_getfloat(pVM,-1,&vecRot.fZ);

	if(g_pObjectManager->DoesExist(objectId))
	{
		g_pObjectManager->AttachToPlayer(objectId,playerId,vecPos,vecRot);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

int CObjectNatives::AttachPedBone(lua_State *pVM)
{
	EntityId	objectId;
	EntityId	playerId;
	CVector3	vecPos;
	CVector3	vecRot;
	int			iBone;

	script_getentity(pVM,-9,&objectId);
	script_getentity(pVM,-8,&playerId);
	script_getfloat(pVM,-7,&vecPos.fX);
	script_getfloat(pVM,-6,&vecPos.fY);
	script_getfloat(pVM,-5,&vecPos.fZ);
	script_getfloat(pVM,-4,&vecRot.fX);
	script_getfloat(pVM,-3,&vecRot.fY);
	script_getfloat(pVM,-2,&vecRot.fZ);
	script_getinteger(pVM,-1,&iBone);

	if(g_pObjectManager->DoesExist(objectId))
	{
		g_pObjectManager->AttachToPlayer(objectId,playerId,vecPos,vecRot, iBone);
		script_pushbool(pVM,true);
		return 1;
	}

	script_pushbool(pVM,false);
	return 1;
}

int CObjectNatives::AttachVehicle(lua_State * pVM)
{
	EntityId	objectId;
	EntityId	vehicleId;
	CVector3	vecPos;
	CVector3	vecRot;

	script_getentity(pVM,-8,&objectId);
	script_getentity(pVM,-7,&vehicleId);
	script_getfloat(pVM,-6,&vecPos.fX);
	script_getfloat(pVM,-5,&vecPos.fY);
	script_getfloat(pVM,-4,&vecPos.fZ);
	script_getfloat(pVM,-3,&vecRot.fX);
	script_getfloat(pVM,-2,&vecRot.fY);
	script_getfloat(pVM,-1,&vecRot.fZ);

	if(g_pObjectManager->DoesExist(objectId))
	{
		g_pObjectManager->AttachToVehicle(objectId,vehicleId,vecPos,vecRot);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

int CObjectNatives::DetachObject(lua_State *pVM)
{
	EntityId	objectId;
	script_getentity(pVM, -1, &objectId);

	if(g_pObjectManager->DoesExist(objectId) && g_pObjectManager->GetAttachState(objectId))
	{
		g_pObjectManager->Detach(objectId);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM,false);
	return 1;
}

int CObjectNatives::MoveObject(lua_State * pVM)
{
	int iParams = script_gettop(pVM)/* - 1*/;

	CHECK_PARAMS_MIN("moveObject", 5);
	CHECK_TYPE("moveObject", 1, -iParams, SCRIPT_TYPE_INTEGER);
	CHECK_TYPE("moveObject", 2, -iParams + 1, SCRIPT_TYPE_FLOAT);
	CHECK_TYPE("moveObject", 3, -iParams + 2, SCRIPT_TYPE_FLOAT);
	CHECK_TYPE("moveObject", 4, -iParams + 3, SCRIPT_TYPE_FLOAT);
	CHECK_TYPE("moveObject", 5, -iParams + 4, SCRIPT_TYPE_INTEGER);

	if(iParams >= 6)
	{
		CHECK_TYPE("moveObject", 6, -iParams + 5, SCRIPT_TYPE_FLOAT);
		CHECK_TYPE("moveObject", 7, -iParams + 6, SCRIPT_TYPE_FLOAT);
		CHECK_TYPE("moveObject", 8, -iParams + 7, SCRIPT_TYPE_FLOAT);
	}

	EntityId objectId;
	CVector3 vecMoveTarget;
	CVector3 vecMoveRot;
	int iTime;

	script_getentity(pVM, 1, &objectId);
	script_getfloat(pVM, 2, &vecMoveTarget.fX);
	script_getfloat(pVM, 3, &vecMoveTarget.fY);
	script_getfloat(pVM, 4, &vecMoveTarget.fZ);
	script_getinteger(pVM, 5, &iTime);

	if(g_pObjectManager->DoesExist(objectId))
	{
		if(script_gettop(pVM) > 4) // was 5 why??
		{
			script_getfloat(pVM, 6, &vecMoveRot.fX);
			script_getfloat(pVM, 7, &vecMoveRot.fY);
			script_getfloat(pVM, 8, &vecMoveRot.fZ);
			g_pObjectManager->MoveObject(objectId, vecMoveTarget, iTime, true, vecMoveRot);
		}
		else
			g_pObjectManager->MoveObject(objectId, vecMoveTarget, iTime);

		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

int CObjectNatives::RotateObject(lua_State * pVM)
{
	EntityId objectId;
	CVector3 vecMoveRot;
	int iTime;
	script_getentity(pVM, -5, &objectId);
	script_getfloat(pVM, -4, &vecMoveRot.fX);
	script_getfloat(pVM, -3, &vecMoveRot.fY);
	script_getfloat(pVM, -2, &vecMoveRot.fZ);
	script_getinteger(pVM, -1, &iTime);

	if(g_pObjectManager->DoesExist(objectId))
	{
		g_pObjectManager->RotateObject(objectId, vecMoveRot, iTime);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}


int CObjectNatives::SetDimension(lua_State * pVM)
{
	int iDimension;
	EntityId objectId;

	script_getinteger(pVM, -1, &iDimension);
	script_getentity(pVM, -2, &objectId);
	
	g_pObjectManager->SetDimension(objectId, iDimension);

	script_pushbool(pVM, true);
	return 1;
}

int CObjectNatives::GetDimension(lua_State * pVM)
{ 
	EntityId objectId;

	script_getentity(pVM, -1, &objectId);

	script_pushinteger(pVM, (int)g_pObjectManager->GetDimension(objectId));
	return 1;
}

int CObjectNatives::SetInterior(lua_State * pVM)
{
	EntityId objectId;
	int iInterior;

	script_getentity(pVM, -2, &objectId);
	script_getinteger(pVM, -1, &iInterior);

	if(g_pObjectManager->DoesExist(objectId))
	{
		g_pObjectManager->SetInterior(objectId, iInterior);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

int CObjectNatives::GetInterior(lua_State * pVM)
{
	EntityId objectId;
	script_getentity(pVM, -2, &objectId);

	if(g_pObjectManager->DoesExist(objectId)) {
		script_pushinteger(pVM, g_pObjectManager->GetInterior(objectId));
		return 1;
	}
	script_pushbool(pVM, false);
	return 1;
}