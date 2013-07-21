//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: SystemNatives.cpp
// Project: Server.Core
// Author(s): jenksta
// License: See LICENSE in root directory
//
//==============================================================================

#include "SystemNatives.h"
#include <SharedUtility.h>
#include <time.h>
#ifdef _LINUX
#include <sys/stat.h>
#include <sys/types.h>

#define mkdir(x) mkdir(x, 0777)
#else
#include <direct.h>
#endif

void CSystemNatives::Register(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterFunction("clock", Clock, 0, NULL);
	pScriptingManager->RegisterFunction("time", Time, 0, NULL);
	pScriptingManager->RegisterFunction("date", Date, -1, NULL);
	pScriptingManager->RegisterFunction("mkdir", MkDir, 1, "s");
	pScriptingManager->RegisterFunction("rmdir", RmDir, 1, "s");
	pScriptingManager->RegisterFunction("remove", Remove, 1, "s");
	pScriptingManager->RegisterFunction("rename", Rename, 2, "ss");
};

int CSystemNatives::Clock(lua_State * pVM)
{
	script_pushfloat(pVM, (((float)clock()) / (float)CLOCKS_PER_SEC));
	return 1;
}

int CSystemNatives::Time(lua_State * pVM)
{
	time_t t;
	time(&t);
	script_pushinteger(pVM, *((int *)&t));
	return 1;
}

void _set_integer_slot(lua_State * v, const char *name, int val)
{
	script_pushstring(v,name);
	script_pushinteger(v,val);
	script_tableset(v,-3);
}

int CSystemNatives::Date(lua_State * pVM)
{
	time_t t;
	int it;
	int format = 'l';

	if(script_gettop(pVM) > 0)
	{
		script_getinteger(pVM,1,&it);
		t = it;

		if(script_gettop(pVM) > 1)
			script_getinteger(pVM, 2, (int*)&format);
	}
	else
		time(&t);

	tm *date;

	if(format == 'u')
		date = gmtime(&t);
	else
		date = localtime(&t);

	if(!date)
		return script_throwerror(pVM, "crt api failure");

	script_newtable(pVM, 0);
	_set_integer_slot(pVM, ("sec"), date->tm_sec);
	_set_integer_slot(pVM, ("min"), date->tm_min);
	_set_integer_slot(pVM, ("hour"), date->tm_hour);
	_set_integer_slot(pVM, ("day"), date->tm_mday);
	_set_integer_slot(pVM, ("month"), date->tm_mon);
	_set_integer_slot(pVM, ("year"), date->tm_year+1900);
	_set_integer_slot(pVM, ("wday"), date->tm_wday);
	_set_integer_slot(pVM, ("yday"), date->tm_yday);
	return 1;
}

int CSystemNatives::MkDir(lua_State * pVM)
{
	const char * s;
	script_getstring(pVM, 1, &s);
	String strPath(s);
	SharedUtility::RemoveIllegalCharacters(strPath);

	if(mkdir(SharedUtility::GetAbsolutePath("files/%s", strPath.Get())) == -1)
		return script_throwerror(pVM, "mkdir() failed");

	script_pushbool(pVM, true);
	return 0;
}

int CSystemNatives::RmDir(lua_State * pVM)
{
	const char * s;
	script_getstring(pVM, 1, &s);
	String strPath(s);
	SharedUtility::RemoveIllegalCharacters(strPath);

	if(rmdir(SharedUtility::GetAbsolutePath("files/%s", strPath.Get())) == -1)
		return script_throwerror(pVM, "rmdir() failed");

	script_pushbool(pVM, true);
	return 0;
}

int CSystemNatives::Remove(lua_State * pVM)
{
	const char * s;
	script_getstring(pVM, 1, &s);
	String strPath(s);
	SharedUtility::RemoveIllegalCharacters(strPath);

	if(remove(SharedUtility::GetAbsolutePath("files/%s", strPath.Get())) == -1)
		return script_throwerror(pVM, "remove() failed");

	script_pushbool(pVM, true);
	return 0;
}

int CSystemNatives::Rename(lua_State * pVM)
{
	const char *oldn, *newn;
	script_getstring(pVM, 1, &oldn);
	script_getstring(pVM, 2, &newn);
	String strOldName(oldn);
	String strNewName(newn);
	SharedUtility::RemoveIllegalCharacters(strOldName);
	SharedUtility::RemoveIllegalCharacters(strNewName);

	if(rename(SharedUtility::GetAbsolutePath("files/%s", strOldName.Get()), SharedUtility::GetAbsolutePath("files/%s", strNewName.Get())) == -1)
		return script_throwerror(pVM, "rename() failed");

	script_pushbool(pVM, true);
	return 0;
}
