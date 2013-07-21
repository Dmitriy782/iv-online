//============== Copyright © 2010 IV:MP Team. All rights reserved. ==============
// File: Functions.h
//===============================================================================

#pragma once

#include "../../../SDK/SDK.h"

// Here forward declarations for events handlers for this module:
void event_playerSpawn(ScriptArgumentsInterface* pArguments, ScriptArgumentInterface* pReturn, void* pChunk);

// Insert forward declarations for your scripting functions here
int lua_helloworld(lua_State * pVM);
int lua_createVehicleEx(lua_State * pVM);