//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: SQLiteNatives.cpp
// Project: Shared
// Author(s): jenksta
//            Einstein
// License: See LICENSE in root directory
//
//==============================================================================
// TODO: Move to server

#include "Natives.h"
#include "../CScriptingManager.h"
#include "../../CSQLite.h"
#include "sqlite/sqlite3.h"
#include <SharedUtility.h>

// SQLite Database
_BEGIN_CLASS(db)
_MEMBER_FUNCTION(db, constructor, 1, "s")
_MEMBER_FUNCTION(db, query, 1, "s")
_MEMBER_FUNCTION(db, close, 0, NULL)
_END_CLASS(db)

void RegisterSQLiteNatives(CScriptingManager * pScriptingManager)
{
	pScriptingManager->RegisterClass(&_CLASS_DECL(db));
}

_MEMBER_FUNCTION_DESTRUCTOR(db)
{
	//CSQLite * pSQLite = (CSQLite *)pInst;
	CSQLite * pSQLite = script_getinstance<CSQLite *>(pVM);
	pSQLite->close();
	delete pSQLite;
	return 1;
}

_MEMBER_FUNCTION_IMPL(db, constructor)
{
	const char * filename;
	script_getstring(pVM, -1, &filename);

	if(filename)
	{
		CSQLite * pSQLite = new CSQLite();
		String strFileName(filename);
		SharedUtility::RemoveIllegalCharacters(strFileName);
		String strPath(SharedUtility::GetAbsolutePath("files/%s", strFileName.Get()));
		pSQLite->open(strPath);

		if(script_setinstance(pVM, pSQLite, &_CLASS_DECL(db)) == 0)
		{
			return 1;
		}
		else
		{
			CLogFile::Print("Failed to set the database instance.");
			SAFE_DELETE(pSQLite);
		}
	}

	script_pushnull(pVM);
	return 1;
}

// TODO: Fix this function
_MEMBER_FUNCTION_IMPL(db, query)
{
	char * query;
	script_getstring(pVM, -1, (const char **)&query);
	if(query)
	{
		CSQLite * pSQLite = script_getinstance<CSQLite *>(pVM);

		if(!pSQLite)
		{
			CLogFile::Print("Failed to get the database instance.");
			script_pushbool(pVM, false);
			return 1;
		}

		sqlite3 * database = pSQLite->getDatabase();
		sqlite3_stmt * stmt;

		if(sqlite3_prepare(database, query, strlen(query) + 1, &stmt, 0) != SQLITE_OK)
		{
			//const char * errmsg = sqlite3_errmsg(database); // TODO: let the user get the error message using a seperate function
			script_pushbool(pVM, false);
			//script_pushstring(vm, errmsg, sizeof(errmsg));
			return 1;
		}

		script_newtable(pVM, 0);

		int rowCount = 0;
		//sqlite3_step(stmt);

		while(sqlite3_step(stmt) == SQLITE_ROW)
		{
			rowCount++;
			int colCount = sqlite3_column_count(stmt);

			script_pushinteger(pVM, rowCount);
			script_newtable(pVM, 0);

			for(int i = 0; i < colCount; i++)
			{
				int columnType = sqlite3_column_type(stmt, i);
				const char * columnName = sqlite3_column_name(stmt, i);

				script_pushlstring(pVM, columnName, strlen(columnName));

				int type = sqlite3_column_type(stmt, i);

				switch(type)
				{
				case SQLITE_NULL:
					break;
				case SQLITE_INTEGER:
					script_pushinteger(pVM, sqlite3_column_int(stmt, i));
					break;
				case SQLITE_FLOAT:
					script_pushfloat(pVM, (float)sqlite3_column_double(stmt, i));
					break;
				case SQLITE_BLOB:
					/*int length = sqlite3_column_bytes(stmt, i);
					if(length > 0) {
					new unsigned char val[length];
					memcpy(val, (const void *)sqlite3_column_blob(stmt, i), length);
					script_pushstring(vm, val, sizeof(val));
					}*/
					break;
				default:
					/*int length = sqlite3_column_bytes(stmt, i) + 1;
					new unsigned char val[length];
					memcpy(val, sqlite3_column_text(stmt, i), length);

					script_pushstring(vm, val, sizeof(val));*/
					break;

				}

				script_tableset(pVM, -3);
			}

			script_tableset(pVM, -3);
		}

		sqlite3_finalize(stmt);
		return 1;
	}

	script_pushbool(pVM, false);
	return 1;
}

_MEMBER_FUNCTION_IMPL(db, close)
{
	CSQLite * pSQLite = script_getinstance<CSQLite *>(pVM);

	if(!pSQLite)
	{
		CLogFile::Print("Failed to get the database instance.");
		script_pushbool(pVM, false);
		return 1;
	}

	script_pushbool(pVM, pSQLite->close());
	return 1;
}
