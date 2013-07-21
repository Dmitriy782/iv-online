//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CScriptInterface.h
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once
#include "InterfaceCommon.h"

class CScriptInterface
{
public:
	virtual SQVM * GetVM() = 0;
};