//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: ClientNatives.h
// Project: Client.Core
// Author(s): jenksta
//            Einstein
//            Sebihunter
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "../Natives.h"

void RegisterClientNatives(CScriptingManager * pScriptingManager);
void RegisterGUINatives(CScriptingManager * pScriptingManager);
void RegisterScriptNatives(CScriptingManager * pScriptingManager);

SCRIPT_FUNCTION(addChatMessage);
SCRIPT_FUNCTION(toggleChatWindow);
SCRIPT_FUNCTION(isChatWindowVisible);
SCRIPT_FUNCTION(guiShowMessageBox);
SCRIPT_FUNCTION(guiSetCursorPosition);
SCRIPT_FUNCTION(guiGetCursorPosition);
SCRIPT_FUNCTION(guiToggleCursor);
SCRIPT_FUNCTION(guiGetScreenSize);
SCRIPT_FUNCTION(guiDrawRectangle);
SCRIPT_FUNCTION(triggerServerEvent);
SCRIPT_FUNCTION(guiIsCursorVisible);
SCRIPT_FUNCTION(getScripts);
SCRIPT_FUNCTION(getScriptName);
SCRIPT_FUNCTION(getWeather);
SCRIPT_FUNCTION(setWeather);
SCRIPT_FUNCTION(getGameScrollBarText);
SCRIPT_FUNCTION(setGameScrollBarText);
SCRIPT_FUNCTION(resetGameScrollBarText);
SCRIPT_FUNCTION(getGameScrollBarColor);
SCRIPT_FUNCTION(setGameScrollBarColor);
SCRIPT_FUNCTION(getMoney);
SCRIPT_FUNCTION(getCurrentAreaName);
SCRIPT_FUNCTION(getCurrentStreetName);
SCRIPT_FUNCTION(getVehicleName);
SCRIPT_FUNCTION(getFPS);
SCRIPT_FUNCTION(getScreenPositionFromWorldPosition);
SCRIPT_FUNCTION(isGameFocused);
SCRIPT_FUNCTION(setRadarZoom);
SCRIPT_FUNCTION(getActorCoordinates);
SCRIPT_FUNCTION(drawLightWithRange);
SCRIPT_FUNCTION(tuneRadio);
SCRIPT_FUNCTION(getGameLanguage);
SCRIPT_FUNCTION(importAndLoadGameFile);