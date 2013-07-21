//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: CWebserver.h
// Project: Server.Core
// Author(s): Einstein
// License: See LICENSE in root directory
//
//==============================================================================

#pragma once

#include "CServer.h"
#include "SharedUtility.h"
#include <mongoose/mongoose.h>
#include <CFileChecksum.h>
#include "MetaFile.h"

class CWebServer
{
private:
	mg_context * m_pMongooseContext;

	//static void * MongooseEventHandler(mg_event event, mg_connection * conn);

public:
	CWebServer(unsigned short usHTTPPort);
	~CWebServer();

	bool FileCopy(CMetaFile & mf, CFileChecksum &fileChecksum);
};
