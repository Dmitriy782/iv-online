//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: PlayerNatives.h
// Project: Server.Core
// Author(s): jenksta
//            MaVe
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "../Natives.h"

class CPlayerNatives
{
private:
	static int IsConnected(lua_State * pVM);
	static int GetName(lua_State * pVM);
	static int SetName(lua_State * pVM);
	static int SetHealth(lua_State * pVM);
	static int GetHealth(lua_State * pVM);
	static int SetArmour(lua_State * pVM);
	static int GetArmour(lua_State * pVM);
	static int SetCoordinates(lua_State * pVM);
	static int GetCoordinates(lua_State * pVM);
	static int SetTime(lua_State * pVM);
	static int SetWeather(lua_State * pVM);
	static int SetGravity(lua_State * pVM);
	static int SendMessage(lua_State * pVM);
	static int SendMessageToAll(lua_State * pVM);
	static int IsInAnyVehicle(lua_State * pVM);
	static int IsInVehicle(lua_State * pVM);
	static int GetVehicleId(lua_State * pVM);
	static int GetSeatId(lua_State * pVM);
	static int IsOnFoot(lua_State * pVM);
	static int TogglePayAndSpray(lua_State * pVM);
	static int ToggleAutoAim(lua_State * pVM);
	//static int SetPlayerDrunk(lua_State * pVM);
	static int GiveWeapon(lua_State * pVM);
	static int RemoveWeapons(lua_State * pVM);
	static int SetSpawnLocation(lua_State * pVM);
	static int SetModel(lua_State * pVM);
	static int GetModel(lua_State * pVM);
	static int ToggleControls(lua_State * pVM);
	static int IsSpawned(lua_State * pVM);
	static int SetHeading(lua_State * pVM);
	static int GetHeading(lua_State * pVM);
	static int TogglePhysics(lua_State * pVM);
	static int Kick(lua_State * pVM);
	static int Ban(lua_State * pVM);
	static int GetIp(lua_State * pVM);
	static int GiveMoney(lua_State * pVM);
	static int SetMoney(lua_State * pVM);
	static int ResetMoney(lua_State * pVM);
	static int GetMoney(lua_State * pVM);
	static int DisplayText(lua_State * pVM);
	static int DisplayTextForAll(lua_State * pVM);
	static int DisplayInfoText(lua_State * pVM);
	static int DisplayInfoTextForAll(lua_State * pVM);
	static int ToggleFrozen(lua_State * pVM);
	static int GetState(lua_State * pVM);
	static int SetVelocity(lua_State * pVM);
	static int GetVelocity(lua_State * pVM);
	static int GetWantedLevel(lua_State * pVM);
	static int SetWantedLevel(lua_State * pVM);
	static int WarpIntoVehicle(lua_State * pVM);
	static int RemoveFromVehicle(lua_State * pVM);
	static int GetWeapon(lua_State * pVM);
	static int GetAmmo(lua_State * pVM);
	static int GetSerial(lua_State * pVM);
	static int SetCameraBehind(lua_State * pVM);
	static int SetDucking(lua_State * pVM);
	static int IsDucking(lua_State * pVM);
	static int SetInvincible(lua_State * pVM);
	static int ToggleHUD(lua_State * pVM);
	static int ToggleRadar(lua_State * pVM);
	static int ToggleNames(lua_State * pVM);
	static int ToggleAreaNames(lua_State * pVM);
	static int GetEmptyControlState(lua_State * pVM);
	static int GetPreviousControlState(lua_State * pVM);
	static int GetControlState(lua_State * pVM);
	static int TriggerEvent(lua_State * pVM);
	static int ToggleNametagForPlayer(lua_State * pVM);
	static int GetColor(lua_State * pVM);
	static int SetColor(lua_State * pVM);
	static int GetPing(lua_State * pVM);
	static int SetClothes(lua_State * pVM);
	static int GetClothes(lua_State * pVM);
	static int ResetClothes(lua_State * pVM);
	static int Respawn(lua_State * pVM);
	static int GiveHelmet(lua_State * pVM);
	static int RemoveHelmet(lua_State * pVM);
	static int ToggleHelmet(lua_State * pVM);
	static int SetCameraPos(lua_State * pVM);
	static int SetCameraLookAt(lua_State * pVM);
	static int ResetCamera(lua_State * pVM);
	static int forceAnim(lua_State * pVM);
	static int requestAnim(lua_State * pVM);
	static int releaseAnim(lua_State * pVM);
	static int triggerAudioEvent(lua_State * pVM);
	static int triggerMissionCompleteAudio(lua_State * pVM);
	static int triggerPoliceReport(lua_State * pVM);
	static int fadeScreenIn(lua_State * pVM);
	static int fadeScreenOut(lua_State * pVM);
	static int blockWeaponChange(lua_State * pVM);
	static int blockWeaponDrop(lua_State * pVM);
	static int AttachCamToPlayer(lua_State * pVM);
	static int AttachCamToVehicle(lua_State * pVM);
	static int DisplayHudNotification(lua_State * pVM);
	static int FollowVehicleMode(lua_State * pVM);
	static int FollowVehicleOffset(lua_State * pVM);
	static int SetAmmoInClip(lua_State * pVM);
	static int SetAmmo(lua_State * pVM);
	static int SetMobilePhone(lua_State * pVM);
	static int SaySpeech(lua_State * pVM);
	static int DriveAutomatic(lua_State * pVM);
	static int GetFileChecksum(lua_State * pVM);

	static int SetDimension(lua_State * pVM);
	static int GetDimension(lua_State * pVM);

public:
	static void      Register(CScriptingManager * pScriptingManager);
};
