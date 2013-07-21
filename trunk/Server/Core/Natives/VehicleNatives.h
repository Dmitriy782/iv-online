//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: VehicleNatives.h
// Project: Server.Core
// Author(s): jenksta
//            MaVe
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "../Natives.h"

class CVehicleNatives
{
private:
	static int Create(lua_State * pVM);
	static int Delete(lua_State * pVM);
	static int SetCoordinates(lua_State * pVM);
	static int GetCoordinates(lua_State * pVM);
	static int SetRotation(lua_State * pVM);
	static int GetRotation(lua_State * pVM);
	static int IsValid(lua_State * pVM);
	static int SetColor(lua_State * pVM);
	static int GetColor(lua_State * pVM);
	static int GetModel(lua_State * pVM);
	static int SetHealth(lua_State * pVM);
	static int GetHealth(lua_State * pVM);
	static int SetEngineHealth(lua_State * pVM);
	static int GetEngineHealth(lua_State * pVM);
	static int SetVelocity(lua_State * pVM);
	static int GetVelocity(lua_State * pVM);
	static int SetAngularVelocity(lua_State * pVM);
	static int GetAngularVelocity(lua_State * pVM);
	static int Respawn(lua_State * pVM);
	static int IsOccupied(lua_State * pVM);
	static int GetOccupants(lua_State * pVM);
	static int SetDirtLevel(lua_State * pVM);
	static int GetDirtLevel(lua_State * pVM);
	static int SetSirenState(lua_State * pVM);
	static int GetSirenState(lua_State * pVM);
	static int SoundHorn(lua_State * pVM);
	static int SetLocked(lua_State * pVM);
	static int GetLocked(lua_State * pVM);
	static int SetIndicators(lua_State * pVM);
	static int GetIndicators(lua_State * pVM);
	static int SetComponent(lua_State * pVM);
	static int ResetComponents(lua_State * pVM);
	static int GetComponents(lua_State * pVM);
	static int SetVariation(lua_State * pVM);
	static int GetVariation(lua_State * pVM);
	static int SetEngineStatus(lua_State * pVM);
	static int GetEngineStatus(lua_State * pVM);
	static int SwitchTaxiLights(lua_State * pVM);
	static int GetTaxiLights(lua_State * pVM);
	static int ControlCar(lua_State * pVM);
	static int SetLights(lua_State * pVM);
	static int GetLights(lua_State * pVM);
	static int RepairWheels(lua_State * pVM);
	static int RepairWindows(lua_State * pVM);
	static int SetGpsState(lua_State * pVM);
	static int GetGpsState(lua_State * pVM);
	static int SetAlarm(lua_State * pVM);
	static int MarkVehicle(lua_State * pVM);
	static int RepairVehicle(lua_State * pVM);
	
	static int SetRespawnDelay(lua_State * pVM);
	static int GetRespawnDelay(lua_State * pVM);
	static int SetDimension(lua_State * pVM);
	static int GetDimension(lua_State * pVM);

public:
	static void      Register(CScriptingManager * pScriptingManager);
};
