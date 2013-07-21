//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: COffsets.h
// Project: Client.Core
// Author(s): mabako
//            jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include <assert.h>
#include "CGame.h"

enum eGameVersion
{
	GAME_VERSION_UNKNOWN, // ?.?.?.?
	GAME_VERSION_4,       // 1.0.4.0
	GAME_VERSION_5,       // 1.0.0.4
	GAME_VERSION_6,       // 1.0.6.0
	GAME_VERSION_7,       // 1.0.7.0
};

class COffsets
{
private:
	static eGameVersion m_GameVersion;

public:
	static void         Init(unsigned int uiBase);
	static eGameVersion GetVersion() { return m_GameVersion; }

	// Players
	static unsigned int FUNC_CreatePlayerPed;
	static unsigned int FUNC_SetupPedIntelligence;
	static unsigned int FUNC_CPlayerPed__SetModelIndex;
	static unsigned int VAR_Ped_Factory;
	static unsigned int FUNC_Setup_Ped;
	static unsigned int FUNC_CPlayerPed__Constructor;
	static unsigned int FUNC_CPedIntelligence__Reset;
	static unsigned int FUNC_CPed__ScalarDeletingDestructor;

	// Game
	static unsigned int VAR_MenuActive_1;
	static unsigned int VAR_MenuActive_2;

	// Script
	static unsigned int FUNC_GetRunningScriptThread;
	static unsigned int CALL_CScriptVM__Process;
	static unsigned int VAR_ScrVM__ThreadPool;
	static unsigned int VAR_ScrVM__ActiveThread;
	static unsigned int FUNC_ScrVM__FindNativeAddress;
	static unsigned int VAR_ScrVM__GlobalVariablesPool;

	// Weather
	static unsigned int FUNC_SetWeather;
	static unsigned int VAR_CurrentWeather;

	// Clock
	static unsigned int FUNC_SetTimeOfDay;
	static unsigned int VAR_TimeOfDay;
	static unsigned int VAR_CClock__Hour;
	static unsigned int VAR_CClock__Minute;
	static unsigned int VAR_CClock__LockedHour;
	static unsigned int VAR_CClock__LockedMinute;
	static unsigned int VAR_CClock__DayOfWeek;

	// HUD
	static unsigned int VAR_HudEnabled;
	static unsigned int VAR_RadarVisible;
	static unsigned int VAR_PlayerNamesVisible;
	static unsigned int VAR_AreaNamesDisabled;

	// World
	static unsigned int FUNC_CWorld__AddEntity;
	static unsigned int FUNC_CWorld__RemoveEntity;
	static unsigned int FUNC_CEntity__IsTouchingEntity;
	static unsigned int FUNC_CMatrix__ConvertFromEulerAngles;
	static unsigned int FUNC_CMatrix__ConvertFromQuaternion;
	static unsigned int FUNC_CQuaternion__ConvertFromMatrix;

	// Vehicle
	static unsigned int FUNC_CVehicle__SetLoadCollision;
	static unsigned int FUNC_CVehicle__TurnEngineOn;
	static unsigned int FUNC_CVehicle__TurnEngineOff;
	static unsigned int FUNC_CVehicle__RefreshColours;

	// Misc
	static unsigned int VAR_CurrentWantedLevel;
	static unsigned int VAR_ScrollBarText;
	static unsigned int VAR_ScrollBarColor;
	static unsigned int VAR_CameraManager;
	static unsigned int VAR_LocalPlayerState;
	static unsigned int FUNC_GetLocalPlayerSpawnPosition;
	static unsigned int CALL_SpawnLocalPlayer;
	static unsigned int FUNC_SpawnPlayer;
	static unsigned int FUNC_GetTrafficLightState1;
	static unsigned int FUNC_GetTrafficLightState2;
	static unsigned int VAR_CurrentAreaName;
	static unsigned int VAR_CurrentStreetName;
	static unsigned int VAR_NumLoadingScreens;
	static unsigned int VAR_FirstLoadingScreenDuration;
	static unsigned int VAR_FirstLoadingScreenType;
	static unsigned int VAR_HWnd;
	static unsigned int VAR_Time;

	// Input
	static unsigned int VAR_Pads;
	static unsigned int FUNC_CPad__Constructor;
	static unsigned int FUNC_CPad__Initialize;
	static unsigned int FUNC_CPad__Destructor;

	// Key Sync
	static unsigned int VAR_CPlayerPed__VFTable;
	static unsigned int VAR_CAutomobile__VFTable;
	static unsigned int VAR_CBike__VFTable;
	static unsigned int VAR_CBoat__VFTable;
	static unsigned int VAR_CTrain__VFTable;
	static unsigned int VAR_CHeli__VFTable;
	static unsigned int VAR_CPlane__VFTable;
	static unsigned int FUNC_CPlayerPed__ProcessInput;
	static unsigned int FUNC_CAutomobile__ProcessInput;
	static unsigned int FUNC_CBike__ProcessInput;
	static unsigned int FUNC_CBoat__ProcessInput;
	static unsigned int FUNC_CTrain__ProcessInput;
	static unsigned int FUNC_CHeli__ProcessInput;
	static unsigned int FUNC_CPlane__ProcessInput;

	// Checkpoints
	static unsigned int VAR_RenderCheckpoints_FirstCP;
	static unsigned int VAR_RenderCheckpoints_LastCP;

	// Pools
	static unsigned int VAR_PedPool;
	static unsigned int VAR_VehiclePool;
	static unsigned int VAR_PtrNodeSinglePool;
	static unsigned int VAR_PtrNodeDoublePool;
	static unsigned int VAR_EntryInfoNodesPool;
	static unsigned int VAR_AnimatedBuildingPool;
	static unsigned int VAR_BuildingPool;
	static unsigned int VAR_ObjectPool;
	static unsigned int VAR_DummyObjectPool;
	static unsigned int VAR_InteriorInstPool;
	static unsigned int VAR_PortalInstPool;
	static unsigned int VAR_AnimBlenderPool;
	static unsigned int VAR_AtdNodeAnimChangePooledObjectPool;
	static unsigned int VAR_AtdNodeAnimPlayerPool;
	static unsigned int VAR_crFrameFilterBoneMaskPool;
	static unsigned int VAR_crFrameFilterBoneAnaloguePool;
	static unsigned int VAR_crExpressionProcessorPool;
	static unsigned int VAR_crmtObserverPool;
	static unsigned int VAR_TaskPool;
	static unsigned int VAR_DummyTaskPool;
	static unsigned int VAR_EventPool;
	static unsigned int VAR_PointRoutePool;
	static unsigned int VAR_PatrolRoutePool;
	static unsigned int VAR_NavMeshRoutePool;
	static unsigned int VAR_CamPool;
	static unsigned int VAR_ExplosionTypePool;
	static unsigned int VAR_PedMoveBlendPool;
	static unsigned int VAR_ParticleSystemPool;
	static unsigned int VAR_VehicleStructPool;
	static unsigned int VAR_PedBasePool;
	static unsigned int VAR_TaskInfoPool;
	static unsigned int VAR_PedAttractorPool;
	static unsigned int VAR_TargettingPool;
	static unsigned int FUNC_CPool__Allocate;
	static unsigned int FUNC_CPool__Release;
	static unsigned int FUNC_CPool__AtHandle;
	static unsigned int FUNC_CPool__HandleOf;
	static unsigned int FUNC_CPlayer__GetPlayerByNumber;
	static unsigned int VAR_PlayerInfoArray;
	static unsigned int VAR_LocalPlayerId;

	// Physical
	static unsigned int FUNC_CPhysical__SetMoveSpeed;
	static unsigned int FUNC_CPhysical__GetMoveSpeed;
	static unsigned int FUNC_CPhysical__SetTurnSpeed;
	static unsigned int FUNC_CPhysical__GetTurnSpeed;
	static unsigned int FUNC_CPhysical__SetLastDamageEntity;

	// Indicators
	static unsigned int PATCH_CVehicle__HazzardLightsOn;
	static unsigned int FUNC_CVehicle__DrawIndicator;
	static unsigned int RETURN_CVehicle__DrawIndicator;
	static unsigned int FUNC_CVehicleAudio__SetIndicators;

	// Streaming
	static unsigned int VAR_ResourceTypeWdrIndex;
	static unsigned int VAR_ResourceTypeWadIndex;
	static unsigned int VAR_ResourceTypeScoIndex;
	static unsigned int VAR_ResourceTypeWtdIndex;
	static unsigned int VAR_ResourceTypeCutIndex;
	static unsigned int VAR_ResourceTypeWddIndex;
	static unsigned int VAR_ResourceTypeWnvIndex;
	static unsigned int VAR_ResourceTypeRrrIndex;
	static unsigned int VAR_ResourceTypeWbdIndex;
	static unsigned int VAR_ResourceTypeIplIndex;
	static unsigned int VAR_ResourceTypeWbnIndex;
	static unsigned int VAR_ResourceTypeWbsIndex;
	static unsigned int FUNC_RequestResource;
	static unsigned int FUNC_LoadAllResources;
	static unsigned int FUNC_HasResourceLoaded;
	static unsigned int FUNC_ReleaseResource;
	static unsigned int FUNC_GetModelIndexFromHash;
	static unsigned int FUNC_CAnimStore__GetIndexFromHash;
	static unsigned int FUNC_LoadWorldAtPosition;

	// Ped
	static unsigned int FUNC_CPed__SetDucking;
	static unsigned int FUNC_CPed__IsDucking;
	static unsigned int FUNC_CPed__DisableRagdoll;

	// Tasks
	static unsigned int FUNC_CPedTaskManager__SetTaskPriority;
	static unsigned int FUNC_CPedTaskManager__SetTaskSecondary;
	static unsigned int FUNC_CPedTaskManager__SetTaskMovement;
	static unsigned int VAR_CTask__VFTable;
	static unsigned int RETURN_CTask__Destructor;
	static unsigned int FUNC_CTaskComplexNewGetInVehicle__Constructor;
	static unsigned int FUNC_CTaskComplexNewExitVehicle__Constructor;
	static unsigned int FUNC_CTaskComplexDie__Constructor;
	static unsigned int FUNC_CTaskSimpleDead__Constructor;
	static unsigned int FUNC_CTaskSimpleCarSetPedInVehicle__Constructor;
	static unsigned int FUNC_CTaskSimpleCarSetPedOut__Constructor;
	static unsigned int FUNC_CTaskSimpleStopWalking__Constructor;
	static unsigned int FUNC_CTaskSimpleStartWalking__Constructor;
	static unsigned int FUNC_CTaskComplexJump__Constructor;
	static unsigned int FUNC_CTaskSimpleTriggerLookAt__Constructor;
	static unsigned int FUNC_CTaskComplexPlayerOnFoot__Constructor;

	// Weapons
	static unsigned int FUNC_CPedWeapons__RemoveWeapon;
	static unsigned int FUNC_CPedWeapons__RemoveAllWeapons;
};
