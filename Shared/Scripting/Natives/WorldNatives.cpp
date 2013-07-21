//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: WorldNatives.cpp
// Project: Shared
// Author(s): mabako
// License: See LICENSE in root directory
//
//==============================================================================

#include "WorldNatives.h"
#include "../../Game/CTime.h"
#include "../../Game/CTrafficLights.h"
#include "../../CLogFile.h"

void CWorldNatives::Register(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterFunction("setTime", SetTime, 2, "ii");
	pScriptingManager->RegisterFunction("getTime", GetTime, 0, NULL);
	pScriptingManager->RegisterFunction("setMinuteDuration", SetMinuteDuration, 1, "i");
	pScriptingManager->RegisterFunction("getMinuteDuration", GetMinuteDuration, 0, NULL);
	pScriptingManager->RegisterFunction("setDayOfWeek", SetDayOfWeek, 1, "i");
	pScriptingManager->RegisterFunction("getDayOfWeek", GetDayOfWeek, 0, NULL);
	pScriptingManager->RegisterFunction("setTrafficLightsState", SetTrafficLightsState, 1, "i");
	pScriptingManager->RegisterFunction("getTrafficLightsState", GetTrafficLightsState, 0, NULL);
	pScriptingManager->RegisterFunction("setTrafficLightsLocked", SetTrafficLightsLocked, 1, "b");
	pScriptingManager->RegisterFunction("areTrafficLightsLocked", AreTrafficLightsLocked, 0, NULL);
	pScriptingManager->RegisterFunction("setTrafficLightsPhaseDuration", SetTrafficLightsPhaseDuration, 2, "ii");
	pScriptingManager->RegisterFunction("getTrafficLightsPhaseDuration", GetTrafficLightsPhaseDuration, 0, NULL);
	pScriptingManager->RegisterFunction("resetTrafficLightsPhaseDuration",ResetTrafficLightsPhaseDuration,0,NULL);
}

// setTime(hour, minute)
int CWorldNatives::SetTime(lua_State * pVM)
{
	int iHour;
	int iMinute;
	script_getinteger(pVM, 1, &iHour);
	script_getinteger(pVM, 2, &iMinute);

	if(iHour >= 0 && iHour <= 23 && iMinute >= 0 && iMinute <= 59)
	{
		CTime::GetInstance()->SetTime(iHour, iMinute);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// getTime()
int CWorldNatives::GetTime(lua_State * pVM)
{
	unsigned char ucHour = 0, ucMinute = 0;
	CTime::GetInstance()->GetTime(&ucHour, &ucMinute);

	//script_newarray(pVM, 0);
	//script_pushinteger(pVM, ucHour);
	//script_arrayappend(pVM, -2);
	//script_pushinteger(pVM, ucMinute);
	//script_arrayappend(pVM, -2);

	CScriptArray a(pVM);
	a.push(ucHour);
	a.push(ucMinute);

	return 1;
}

// setMinuteDuration(milliseconds)
int CWorldNatives::SetMinuteDuration(lua_State * pVM)
{
	int iMinuteDuration;
	script_getinteger(pVM, 1, &iMinuteDuration);

	if(iMinuteDuration > 0)
	{
		CTime::GetInstance()->SetMinuteDuration(iMinuteDuration);
		script_pushbool(pVM, true);
		return 1;
	}
	else if(iMinuteDuration < 1)
	{
		CLogFile::Printf("Failed to set MinuteDuration for %d ms(Minimum 1ms)", iMinuteDuration);
		script_pushbool(pVM, false);
		return 1;
	}

	return 1;
}

// getMinuteDuration()
int CWorldNatives::GetMinuteDuration(lua_State * pVM)
{
	script_pushinteger(pVM, CTime::GetInstance()->GetMinuteDuration());
	return 1;
}

// setDayOfWeek(day)
int CWorldNatives::SetDayOfWeek(lua_State * pVM)
{
	int iDay;
	script_getinteger(pVM, 1, &iDay);

	if(iDay >= 0 && iDay <= 6)
	{
		CTime::GetInstance()->SetDayOfWeek(iDay);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// getDayOfWeek()
int CWorldNatives::GetDayOfWeek(lua_State * pVM)
{
	script_pushinteger(pVM, CTime::GetInstance()->GetDayOfWeek());
	return 1;
}

// setTrafficLightsState(state)
int CWorldNatives::SetTrafficLightsState(lua_State * pVM)
{
	int iState;
	script_getinteger(pVM, 1, &iState);
	if(iState > 0 && iState <= 3)
	{
		script_pushbool(pVM, CTrafficLights::GetInstance()->SetState((CTrafficLights::eTrafficLightState)iState));
		return 1;
	}
	else
	{
		CLogFile::Printf("Failed to set TrafficLightsState to %d(Only available from 0 to 3)",iState);
		script_pushbool(pVM,false);
		return 1;
	}
}

// getTrafficLightsState()
int CWorldNatives::GetTrafficLightsState(lua_State * pVM)
{
	script_pushinteger(pVM, (int)CTrafficLights::GetInstance()->GetState());
	return 1;
}

// setTrafficLightsLocked(bool locked)
int CWorldNatives::SetTrafficLightsLocked(lua_State * pVM)
{
	bool b;
	script_getbool(pVM, 1, &b);
	
	CTrafficLights::GetInstance()->SetLocked(b != 0);
	script_pushbool(pVM, true);
	return 1;
}

// areTrafficLightsLocked()
int CWorldNatives::AreTrafficLightsLocked(lua_State * pVM)
{
	script_pushbool(pVM, CTrafficLights::GetInstance()->IsLocked());
	return 1;
}

// setTrafficLightsPhaseDuration(phase, duration)
int CWorldNatives::SetTrafficLightsPhaseDuration(lua_State * pVM)
{
	int iPhase, iDuration;
	script_getinteger(pVM, 1, &iPhase);
	script_getinteger(pVM, 2, &iDuration);

	if(iDuration > 0)
	{
		if(iPhase > 0 && iPhase <= 3)
		{
			switch(iPhase)
			{
				case 1:
					CTrafficLights::GetInstance()->SetGreenDuration(iDuration);
					break;
				case 2:
					CTrafficLights::GetInstance()->SetYellowDuration(iDuration);
					break;
				case 3:
					CTrafficLights::GetInstance()->SetRedDuration(iDuration);
					break;
			}

			script_pushbool(pVM, true);
			return 1;
		}
		else
		{
			CLogFile::Printf("Failed to set TrafficLightsPhaseDuration from the trafficlight %d to %d ms(Trafficlights are only supported from 1 to 3)",iPhase,iDuration);
			script_pushbool(pVM, false);
			return 1;
		}
	}

	CLogFile::Printf("Failed to set TrafficLightsPhaseDuration from the trafficlight %d to %d ms(Minimum duration: 1ms)",iPhase,iDuration);
	script_pushbool(pVM, false);
	return 1;
}

// getTrafficLightsPhaseDuration()
int CWorldNatives::GetTrafficLightsPhaseDuration(lua_State * pVM)
{

/*
	script_newarray(pVM, 0);

	script_pushbool(pVM, CTrafficLights::GetInstance()->IsUsingDefaultDurations());
	script_arrayappend(pVM, -2);

	script_pushinteger(pVM, CTrafficLights::GetInstance()->GetGreenDuration());
	script_arrayappend(pVM, -2);

	script_pushinteger(pVM, CTrafficLights::GetInstance()->GetYellowDuration());
	script_arrayappend(pVM, -2);

	script_pushinteger(pVM, CTrafficLights::GetInstance()->GetRedDuration());
	script_arrayappend(pVM, -2);
*/
	CScriptArray a(pVM);
	a.PushBool(CTrafficLights::GetInstance()->IsUsingDefaultDurations());
	a.push(CTrafficLights::GetInstance()->GetGreenDuration());
	a.push(CTrafficLights::GetInstance()->GetYellowDuration());
	a.push(CTrafficLights::GetInstance()->GetRedDuration());

	return 1;
}

// resetTrafficLightsPhaseDuration()
int CWorldNatives::ResetTrafficLightsPhaseDuration(lua_State * pVM)
{
	CTrafficLights::GetInstance()->ResetDefaultDurations();
	script_pushbool(pVM, true);
	return 1;
}
