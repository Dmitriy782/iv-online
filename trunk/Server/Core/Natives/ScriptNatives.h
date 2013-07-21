//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: ScriptNatives.h
// Project: Server.Core
// Author(s): mabako
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "../Natives.h"

void RegisterScriptNatives(CScriptingManager * pScriptingManager);

SCRIPT_FUNCTION(server_getscripts);
SCRIPT_FUNCTION(server_getclientscripts);
SCRIPT_FUNCTION(server_getclientresources);
SCRIPT_FUNCTION(server_getscriptname);
SCRIPT_FUNCTION(server_loadscript);
SCRIPT_FUNCTION(server_unloadscript);
SCRIPT_FUNCTION(server_reloadscript);
SCRIPT_FUNCTION(server_loadclientscript);
SCRIPT_FUNCTION(server_unloadclientscript);
SCRIPT_FUNCTION(server_reloadclientscript);
SCRIPT_FUNCTION(server_loadclientresource);
SCRIPT_FUNCTION(server_unloadclientresource);
SCRIPT_FUNCTION(server_reloadclientresource);
