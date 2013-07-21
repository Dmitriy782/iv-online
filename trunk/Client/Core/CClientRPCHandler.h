//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CClientRPCHandler.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include <Network/CRPCHandler.h>

class CClientRPCHandler : public CRPCHandler
{
private:
	// Network
	// FIXUPDATE
	// TODO: NewEntity and DeleteEntity RPC's with CEntity client and server
	// class that has abstract SpawnForWorld e.t.c. to call New/DeleteEntity
	// TODO: SetEntityPosition/Rotation e.t.c. as part of CEntity with abstracted
	// funcs and stuff?
	static void JoinedGame(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void NewPlayer(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void DeletePlayer(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void NewVehicle(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void DeleteVehicle(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void NewObject(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void AttachObject(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void DetachObject(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void DeleteObject(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void NewBlip(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void DeleteBlip(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void NewActor(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void DeleteActor(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void NewCheckpoint(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void DeleteCheckpoint(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void PlayerSpawn(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void PlayerDeath(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void Chat(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void OnFootSync(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void InVehicleSync(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void PassengerSync(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void SmallSync(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void EmptyVehicleSync(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void Message(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ConnectionRefused(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void VehicleEnterExit(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void HeadMovement(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void NameChange(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void NewFile(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void DeleteFile(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void NewPickup(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void DeletePickup(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);

	// Scripting
	static void ScriptingSetPlayerHealth(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetPlayerCoordinates(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetPlayerTime(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetPlayerWeather(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingTogglePayAndSpray(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingToggleAutoAim(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	//static void SetPlayerDrunk(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingGivePlayerWeapon(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetPlayerGravity(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetSpawnLocation(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetVehicleIndicators(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSoundVehicleHorn(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingGiveHelmet(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingRemoveHelmet(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetModel(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingToggleControls(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetHeading(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetVehicleDirtLevel(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetVehicleSirenState(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetVehicleEngineState(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingTurnTaxiLights(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingControlCar(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetVehicleLights(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingRepairVehicleTyres(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingRepairVehicleWindows(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetVehicleCoordinates(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetVehicleRotation(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetVehicleColor(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingToggleRagdoll(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingGivePlayerMoney(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetPlayerMoney(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingDisplayText(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingDisplayInfoText(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingToggleFrozen(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetVehicleHealth(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetVehicleEngineHealth(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetPlayerArmour(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetPlayerMoveSpeed(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetVehicleMoveSpeed(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetVehicleTurnSpeed(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingRemoveWeapons(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetWantedLevel(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingWarpPlayerIntoVehicle(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingRemovePlayerFromVehicle(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetCameraBehindPlayer(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetPlayerDucking(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetPlayerInvincible(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetActorCoordinates(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetActorHeading(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingActorWalkToCoordinates(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetActorName(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingToggleActorNametag(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingToggleActorBlip(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetActorColor(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingToggleActorFrozen(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingToggleActorHelmet(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingWarpActorIntoVehicle(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingRemoveActorFromVehicle(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetBlipColor(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetBlipSize(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingToggleBlipShortRange(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingToggleBlipRoute(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetBlipName(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetBlipIcon(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingShowCheckpointForPlayer(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingHideCheckpointForPlayer(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingToggleHUD(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingToggleRadar(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingToggleNames(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingToggleAreaNames(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingEventCall(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetPlayerColor(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetVehicleLocked(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetPlayerClothes(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingResetPlayerClothes(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetTrafficLightState(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetVehicleComponents(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetVehicleVariation(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetObjectPosition(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetObjectRotation(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetPickupPosition(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetPickupRotation(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetPickupValue(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetPlayerCameraPos(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetPlayerCameraLookAt(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingResetPlayerCamera(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingCreateFire(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingDeleteFire(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingCreateExplosion(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingForcePlayerAnimation(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingForceActorAnimation(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingBlockWeaponScroll(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingBlockWeaponDrop(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingFadeScreenOut(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingFadeScreenIn(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingPlayPoliceReport(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingPlayMissionCompleteAudio(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingPlayGameAudio(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingRequestAnims(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingReleaseAnims(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingForceWind(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetNametags(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingAttachCam(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingDisplayHudNotification(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetVehicleFollowMode(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetVehicleFollowOffset(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetAmmoInClip(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetAmmo(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void	ScriptingCreatePlayerBlip(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void	ScriptingRemovePlayerBlip(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void	ScriptingChangePlayerBlip(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetVehicleGPSState(CBitStream * pBitStream, CPlayerSocket *pSenderSocket);
	static void ScriptingSetVehicleAlarm(CBitStream * pBitStream, CPlayerSocket *pSenderSocket);
	static void ScriptingSetVehiclePetrolTankHealth(CBitStream * pBitStream, CPlayerSocket *pSenderSocket);
	static void ScriptingSetVehicleTyreState(CBitStream * pBitStream, CPlayerSocket *pSenderSocket);
	static void ScriptingSetVehicleWindowState(CBitStream * pBitStream, CPlayerSocket *pSenderSocket);
	static void ScriptingSetPlayerUseMobilePhone(CBitStream * pBitStream, CPlayerSocket *pSenderSocket);
	static void ScriptingStopActorDriving(CBitStream * pBitStream, CPlayerSocket *pSenderSocket);
	static void ScriptingActorDriveToCoords(CBitStream * pBitStream, CPlayerSocket *pSenderSocket);
	static void ScriptingMarkVehicleAsActorVehicle(CBitStream * pBitStream, CPlayerSocket *pSenderSocket);
	static void ScriptingForcePlayerSpeech(CBitStream * pBitStream, CPlayerSocket *pSenderSocket);
	static void ScriptingForceActorSpeech(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingLetPlayerDriveAutomatic(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetPlayerDimension(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ResetVehicleEnterExit(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingTogglePlayerLabelForPlayer(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingFixVehicle(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void	ScriptingMoveObject(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void	ScriptingRotateObject(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetObjectDimension(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSetCheckpointDimension(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void	ScriptingSetObjectInterior(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingExplodeCar(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void New3DLabel(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void Delete3DLabel(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSet3DLabelPosition(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSet3DLabelText(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSet3DLabelColor(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSet3DLabelVisible(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSet3DLabelStreamingDistance(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);
	static void ScriptingSet3DLabelDimension(CBitStream * pBitStream, CPlayerSocket * pSenderSocket);

public:
	void        Register();
	void        Unregister();
};
