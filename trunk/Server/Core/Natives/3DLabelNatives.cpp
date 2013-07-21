//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: 3DLabelNatives.cpp
// Project: Server.Core
// Author(s): XForce
// License: See LICENSE in root directory
//
//==============================================================================

#include "../Natives.h"
#include "../C3DLabels.h"
#include <SharedUtility.h>


extern C3DLabelManager * g_p3DLabelManager;

void _3DLabelNatives::Register(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterFunction("create3DLabel", Create, 7, "sfffibf");
	pScriptingManager->RegisterFunction("delete3DLabel", Delete, 1, "i");
	pScriptingManager->RegisterFunction("set3DLabelPosition", SetPosition, 4, "ifff");
	pScriptingManager->RegisterFunction("get3DLabelPosition", GetPosition, 1, "i");
	pScriptingManager->RegisterFunction("set3DLabelColor", SetColor, 2, "ii");
	pScriptingManager->RegisterFunction("get3DLabelColor", GetColor, 1, "i");
	pScriptingManager->RegisterFunction("set3DLabelDimension", SetDimension, 2, "ii");
	pScriptingManager->RegisterFunction("get3DLabelDimension", GetDimension, 1, "i");
	pScriptingManager->RegisterFunction("set3DLabelText", SetText, 2, "is");
	pScriptingManager->RegisterFunction("get3DLabelText", GetText, 1, "i");
	pScriptingManager->RegisterFunction("set3DLabelVisible", SetVisible, 2, "ib");
	pScriptingManager->RegisterFunction("get3DLabelVisible", GetVisible, 1, "i");
	pScriptingManager->RegisterFunction("set3DLabelStreamingDistance", SetStreamingDistance, 2, "if");
	pScriptingManager->RegisterFunction("get3DLabelStreamingDistance", GetStreamingDistance, 1, "i");
};

// create3DLabel(text, x, y, z, color, visible, streamingdistance)
int _3DLabelNatives::Create(lua_State * pVM)
{
	float fStreamingDistance;
	CVector3 vecPosition;
	bool bVisible;
	int iColor;
	const char * szName;
	script_getstring(pVM, -7, &szName);
	script_getvector3(pVM, -6, &vecPosition);
	script_getinteger(pVM, -3, &iColor);
	script_getbool(pVM, -2, &bVisible);
	script_getfloat(pVM, -1, &fStreamingDistance);
	bool bToggle = (bVisible != 0);
	script_pushinteger(pVM, g_p3DLabelManager->Add(szName, vecPosition, iColor, bToggle, fStreamingDistance));

	return 1;
}

// delete3DLabel(labelId)
int _3DLabelNatives::Delete(lua_State * pVM)
{
	LabelId labelId;
	script_getinteger(pVM, -1, (int*)&labelId);

	if(g_p3DLabelManager->DoesExist(labelId))
	{
		g_p3DLabelManager->Remove(labelId);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// set3DLabelPosition(labelId, x, y, z)
int _3DLabelNatives::SetPosition(lua_State * pVM)
{
	CVector3 vecPosition;
	LabelId labelId;
	script_getinteger(pVM, -4, (int*)&labelId);
	script_getvector3(pVM, -3, &vecPosition);

	if(g_p3DLabelManager->DoesExist(labelId))
	{
		g_p3DLabelManager->GetAt(labelId)->SetPosition(vecPosition);
		script_pushbool(pVM, true);
	}

	script_pushbool(pVM, false);
	return 1;
}

// get3DLabelPosition(labelId)
int _3DLabelNatives::GetPosition(lua_State * pVM)
{
	LabelId labelId;
	script_getinteger(pVM, -1, (int*)&labelId);

	if(g_p3DLabelManager->DoesExist(labelId))
	{
		CVector3 vecPosition = g_p3DLabelManager->GetAt(labelId)->GetPosition();

		CScriptArray args(pVM);
		args.push(vecPosition.fX);
		args.push(vecPosition.fY);
		args.push(vecPosition.fZ);
		//CScriptArgument(args, true).push(pVM);//script_pusharg(pVM, CScriptArgument(args, true));
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// set3DLabelColor(labelId, color)
int _3DLabelNatives::SetColor(lua_State * pVM)
{
	int iColor;
	LabelId labelId;
	script_getinteger(pVM, -2, (int*)&labelId);

	script_getinteger(pVM, -1, &iColor);

	if(g_p3DLabelManager->DoesExist(labelId))
	{
			g_p3DLabelManager->GetAt(labelId)->SetColor(iColor);
			script_pushbool(pVM, true);
			return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// get3DLabelColor(labelId)
int _3DLabelNatives::GetColor(lua_State * pVM)
{
	LabelId labelId;
	script_getinteger(pVM, -1, (int*)&labelId);

	if(g_p3DLabelManager->DoesExist(labelId))
	{
		script_pushinteger(pVM, g_p3DLabelManager->GetAt(labelId)->GetColor());
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// set3DLabelDimension(labelId, dimension)
int _3DLabelNatives::SetDimension(lua_State * pVM)
{
	int iDimension;
	LabelId labelId;
	script_getinteger(pVM, -2, (int*)&labelId);

	script_getinteger(pVM, -1, &iDimension);

	if(g_p3DLabelManager->DoesExist(labelId))
	{
			g_p3DLabelManager->GetAt(labelId)->SetDimension(iDimension);
			script_pushbool(pVM, true);
			return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// get3DLabelDimension(labelId)
int _3DLabelNatives::GetDimension(lua_State * pVM)
{
	LabelId labelId;
	script_getinteger(pVM, -1, (int*)&labelId);

	if(g_p3DLabelManager->DoesExist(labelId))
	{
		script_pushinteger(pVM, g_p3DLabelManager->GetAt(labelId)->GetDimension());
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// set3DLabelText(labelId, text)
int _3DLabelNatives::SetText(lua_State * pVM)
{
	const char* szText;
	LabelId labelId;
	script_getinteger(pVM, -2, (int*)&labelId);

	script_getstring(pVM, -1, &szText);

	if(g_p3DLabelManager->DoesExist(labelId))
	{
			g_p3DLabelManager->GetAt(labelId)->SetText(szText);
			script_pushbool(pVM, true);
			return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// get3DLabelText(labelId)
int _3DLabelNatives::GetText(lua_State * pVM)
{
	LabelId labelId;
	script_getinteger(pVM, -1, (int*)&labelId);

	if(g_p3DLabelManager->DoesExist(labelId))
	{
		script_pushlstring(pVM, g_p3DLabelManager->GetAt(labelId)->GetText(), g_p3DLabelManager->GetAt(labelId)->GetText().GetLength());
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// set3DLabelVisible(labelId, visible)
int _3DLabelNatives::SetVisible(lua_State * pVM)
{
	bool bVisible;
	LabelId labelId;
	script_getinteger(pVM, -2, (int*)&labelId);

	script_getbool(pVM, -1, &bVisible);


	if(g_p3DLabelManager->DoesExist(labelId))
	{
		bool bToggleVisible = (bVisible != 0);
		g_p3DLabelManager->GetAt(labelId)->SetVisible(bToggleVisible);
		script_pushbool(pVM, true);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// get3DLabelVisible(labelId)
int _3DLabelNatives::GetVisible(lua_State * pVM)
{
	LabelId labelId;
	script_getinteger(pVM, -1, (int*)&labelId);

	if(g_p3DLabelManager->DoesExist(labelId))
	{
		script_pushbool(pVM, g_p3DLabelManager->GetAt(labelId)->IsVisible());
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// set3DLabelStreamingDistance(labelId, streamingDistance)
int _3DLabelNatives::SetStreamingDistance(lua_State * pVM)
{
	float fStreamingDistance;
	LabelId labelId;
	script_getinteger(pVM, -2, (int*)&labelId);

	script_getfloat(pVM, -1, &fStreamingDistance);

	if(g_p3DLabelManager->DoesExist(labelId))
	{
		g_p3DLabelManager->GetAt(labelId)->SetStreamingDistance(fStreamingDistance);
			script_pushbool(pVM, true);
			return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

// get3DLabelStreamingDistance(labelId)
int _3DLabelNatives::GetStreamingDistance(lua_State * pVM)
{
	LabelId labelId;
	script_getinteger(pVM, -1, (int*)&labelId);

	if(g_p3DLabelManager->DoesExist(labelId))
	{
		script_pushfloat(pVM, g_p3DLabelManager->GetAt(labelId)->GetStreamingDistance());
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}
