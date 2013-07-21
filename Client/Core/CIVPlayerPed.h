//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CIVPlayerPed.h
// Project: Client.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "CIVPed.h"

class IVPlayerPed : public IVPed
{
public:
	// 000-F00
};

class CIVPlayerPed : public CIVPed
{
public:
	CIVPlayerPed();
	CIVPlayerPed(IVPlayerPed * pPlayerPed);
	~CIVPlayerPed();

	void          SetPlayerPed(IVPlayerPed * pPlayerPed);
	IVPlayerPed * GetPlayerPed();
};
