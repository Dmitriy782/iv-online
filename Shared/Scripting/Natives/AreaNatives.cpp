//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: AreaNatives.cpp
// Project: Shared
// Author(s): Einstein
// License: See LICENSE in root directory
//
//==============================================================================

#include "Natives.h"
#include "../CScriptingManager.h"

// Area functions

void CAreaNatives::Register(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterFunction("getDistanceBetweenPoints2D", Points2D, 4, "ffff");
	pScriptingManager->RegisterFunction("getDistanceBetweenPoints3D", Points3D, 6, "ffffff");
	pScriptingManager->RegisterFunction("isPointInCircle", PointInCircle, 5, "fffff");
	pScriptingManager->RegisterFunction("isPointInTube", PointInTube, 8, "ffffffff");
	pScriptingManager->RegisterFunction("isPointInBall", PointInBall, 7, "fffffff");
	pScriptingManager->RegisterFunction("isPointInArea", PointInArea, 6, "ffffff");
	pScriptingManager->RegisterFunction("isPointInCuboid", PointInCuboid, 9, "fffffffff");
	pScriptingManager->RegisterFunction("isPointInPolygon", PointInPolygon, -1, NULL);
}

// getDistanceBetweenPoints2D(x, y, xx, yy);
int CAreaNatives::Points2D(lua_State * pVM)
{
	float x, y, xx, yy;
	script_getfloat(pVM, -4, &x);
	script_getfloat(pVM, -3, &y);
	script_getfloat(pVM, -2, &xx);
	script_getfloat(pVM, -1, &yy);

	script_pushfloat(pVM, Math::GetDistanceBetweenPoints2D(x, y, xx, yy));
	return 1;
}

// getDistanceBetweenPoints3D(x, y, z, xx, yy, zz);
int CAreaNatives::Points3D(lua_State * pVM)
{
	float x, y, z, xx, yy, zz;
	script_getfloat(pVM, -6, &x);
	script_getfloat(pVM, -5, &y);
	script_getfloat(pVM, -4, &z);
	script_getfloat(pVM, -3, &xx);
	script_getfloat(pVM, -2, &yy);
	script_getfloat(pVM, -1, &zz);

	script_pushfloat(pVM, Math::GetDistanceBetweenPoints3D(x, y, z, xx, yy, zz));
	return 1;
}

// isPointInCircle(x, y, xx, yy, distance)
int CAreaNatives::PointInCircle(lua_State * pVM)
{
	float x, y, xx, yy, distance;

	script_getfloat(pVM, -5, &x);
	script_getfloat(pVM, -4, &y);
	script_getfloat(pVM, -3, &xx);
	script_getfloat(pVM, -2, &yy);
	script_getfloat(pVM, -1, &distance);

	script_pushbool(pVM, Math::IsPointInCircle(x, y, distance, xx, yy));
	return 1;
}

//isPointInTube(tubeX, tubeY, tubeZ, tubeHeight, tubeRadius, x, y, z)
int CAreaNatives::PointInTube(lua_State * pVM)
{
	float tubex, tubey, tubez, tubeheight, tubedistance, x, y, z;

	script_getfloat(pVM, -8, &z);
	script_getfloat(pVM, -7, &y);
	script_getfloat(pVM, -6, &x);
	script_getfloat(pVM, -5, &tubedistance);
	script_getfloat(pVM, -4, &tubeheight);
	script_getfloat(pVM, -3, &tubez);
	script_getfloat(pVM, -2, &tubey);
	script_getfloat(pVM, -1, &tubex);	

	script_pushbool(pVM, Math::IsPointInTube(tubex, tubey, tubez, tubeheight, tubedistance, x, y, z));
	return 1;
}

// isPointInBall(x, y, z, xx, yy, zz, distance)
int CAreaNatives::PointInBall(lua_State * pVM)
{
	float x, y, z, xx, yy, zz, distance;

	script_getfloat(pVM, -7, &x);
	script_getfloat(pVM, -6, &y);
	script_getfloat(pVM, -5, &z);
	script_getfloat(pVM, -4, &xx);
	script_getfloat(pVM, -3, &yy);
	script_getfloat(pVM, -2, &zz);
	script_getfloat(pVM, -1, &distance);

	script_pushbool(pVM, Math::IsPointInBall(x, y, z, distance, xx, yy, zz));
	return 1;
}

// isPointInArea(areaX, areaY, areaX2, areaY2, pointX, pointY)
int CAreaNatives::PointInArea(lua_State * pVM)
{
	float areax, areay, areaxx, areayy, pointx, pointy;

	script_getfloat(pVM, -6, &areax);
	script_getfloat(pVM, -5, &areay);
	script_getfloat(pVM, -4, &areaxx);
	script_getfloat(pVM, -3, &areayy);
	script_getfloat(pVM, -2, &pointx);
	script_getfloat(pVM, -1, &pointy);

	script_pushbool(pVM, Math::IsPointInArea(areax, areay, areaxx, areayy, pointx, pointy));
	return 1;
}

// isPointInCuboid(areaX, areaY, areaZ, areaX2, areaY2, areaZ2, pointX, pointY, pointZ)
int CAreaNatives::PointInCuboid(lua_State * pVM)
{
	float areax, areay, areaz, areaxx, areayy, areazz, pointx, pointy, pointz;

	script_getfloat(pVM, -9, &areax);
	script_getfloat(pVM, -8, &areay);
	script_getfloat(pVM, -7, &areaz);
	script_getfloat(pVM, -6, &areaxx);
	script_getfloat(pVM, -5, &areayy);
	script_getfloat(pVM, -4, &areazz);
	script_getfloat(pVM, -3, &pointx);
	script_getfloat(pVM, -2, &pointy);
	script_getfloat(pVM, -1, &pointz);

	script_pushbool(pVM, Math::IsPointInCuboid(areax, areay, areaz, areaxx, areayy, areazz, pointx, pointy, pointz));
	return 1;
}

//isPointInPolygon(pointX, pointY, ...)
int CAreaNatives::PointInPolygon(lua_State * pVM)
{
	CHECK_PARAMS_MIN("isPointInPolygon", 4);

	int iTop = script_gettop(pVM)/* - 1*/;

	CHECK_TYPE("isPointInPolygon", 1, -iTop, SCRIPT_TYPE_FLOAT);
	CHECK_TYPE("isPointInPolygon", 2, -(iTop - 1), SCRIPT_TYPE_FLOAT);

	float pointX, pointY;
	script_getfloat(pVM, -iTop, &pointX);
	script_getfloat(pVM, -(iTop + 1), &pointY);

	float polyX[32], polyY[32];
	int currentPoly = 0;

	int iArgCount = iTop - 2;
	if(iArgCount > 0)
	{
		for(int i = 2; i < iTop; ++i)
		{
			if(i&1) {
				script_getfloat(pVM, -(iTop - i), &polyX[currentPoly]);
			} else {
				script_getfloat(pVM, -(iTop - i), &polyY[currentPoly]);
				currentPoly++;
			}
		}
	}

	script_pushbool(pVM, Math::IsPointInPolygon(currentPoly, polyX, polyY, pointX, pointY));
	return 1;
}
