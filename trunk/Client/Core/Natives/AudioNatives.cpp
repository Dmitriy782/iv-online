//============== IV: Multiplayer - http://code.iv-multiplayer.com ==============
//
// File: AudioNatives.cpp
// Project: Client.Core
// Author(s): RootKiller
// License: See LICENSE in root directory
//
//==============================================================================

#include "../Natives.h"
#include "AudioNatives.h"
#include <Scripting/CScriptingManager.h>
#include "../CClient.h"

extern CClient * g_pClient;

// Audio
_BEGIN_CLASS(Audio)
_MEMBER_FUNCTION(Audio, constructor, 3, "bbs")
_MEMBER_FUNCTION(Audio, deleteSound, 0, NULL)
_MEMBER_FUNCTION(Audio, play, 0, NULL)
_MEMBER_FUNCTION(Audio, stop, 0, NULL)
_MEMBER_FUNCTION(Audio, pause, 0, NULL)
_MEMBER_FUNCTION(Audio, isStarted, 0, NULL)
_MEMBER_FUNCTION(Audio, isPlaying, 0, NULL)
_MEMBER_FUNCTION(Audio, isPaused, 0, NULL)
_MEMBER_FUNCTION(Audio, isStalled, 0, NULL)
_MEMBER_FUNCTION(Audio, setVolume, 1, "f")
_MEMBER_FUNCTION(Audio, getVolume, 0, NULL)
_MEMBER_FUNCTION(Audio, restoreVolume, 0, NULL)
_MEMBER_FUNCTION(Audio, setPosition, 4, "ffff")
_MEMBER_FUNCTION(Audio, clearPosition, 0, NULL)
_MEMBER_FUNCTION(Audio, usePositionSystem, 1, "b")
_END_CLASS(Audio)

void RegisterAudioNatives(CScriptingManager * pScriptingManager)
{
	// GUI
	pScriptingManager->RegisterClass(&_CLASS_DECL(Audio));
}

_MEMBER_FUNCTION_IMPL(Audio, constructor)
{
	bool sqbIsOnlineStream;
	script_getbool(pVM, -3, &sqbIsOnlineStream);
	bool bIsOnlineStream = (sqbIsOnlineStream != 0);

	bool sqbReplay;
	script_getbool(pVM, -2, &sqbReplay);
	bool bReplay = (sqbReplay != 0);

	const char * szSoundName;
	script_getstring(pVM, -1, &szSoundName);

	CAudio * pAudio = new CAudio(szSoundName, bReplay, bIsOnlineStream);

	///SQ metatable = null
	if(!pAudio || !pAudio->Load() || script_setinstance(pVM, pAudio, &_CLASS_DECL(Audio)) != 0)
	{
		CLogFile::Printf("Failed to load audio from file %s",szSoundName);
		SAFE_DELETE(pAudio);
		script_pushnull(pVM);
		return 1;
	}

	g_pClient->GetAudioManager()->Add(pAudio);
	//script_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_DESTRUCTOR(Audio)
{
	delete script_getinstance<CAudio *>(pVM);
	return 1;
}

_MEMBER_FUNCTION_IMPL(Audio, deleteSound)
{
	CAudio * pAudio = script_getinstance<CAudio *>(pVM);
	g_pClient->GetAudioManager()->Remove(pAudio);
	script_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(Audio, play)
{
	CAudio * pAudio = script_getinstance<CAudio *>(pVM);
	script_pushbool(pVM, pAudio->Play());
	return 1;
}

_MEMBER_FUNCTION_IMPL(Audio, stop)
{
	CAudio * pAudio = script_getinstance<CAudio *>(pVM);
	pAudio->Stop();
	script_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(Audio, pause)
{
	CAudio * pAudio = script_getinstance<CAudio *>(pVM);
	pAudio->Pause();
	script_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(Audio, isStarted)
{
	CAudio * pAudio = script_getinstance<CAudio *>(pVM);
	script_pushbool(pVM, (pAudio->IsPlaying() || pAudio->IsStalled()));
	return 1;
}

_MEMBER_FUNCTION_IMPL(Audio, isPlaying)
{
	CAudio * pAudio = script_getinstance<CAudio *>(pVM);
	script_pushbool( pVM, pAudio->IsPlaying() );
	return 1;
}

_MEMBER_FUNCTION_IMPL(Audio, isPaused)
{
	CAudio * pAudio = script_getinstance<CAudio *>(pVM);
	script_pushbool( pVM, pAudio->IsPaused() );
	return 1;
}

_MEMBER_FUNCTION_IMPL(Audio, isStalled)
{
	CAudio * pAudio = script_getinstance<CAudio *>(pVM);
	script_pushbool( pVM, pAudio->IsStalled() );
	return 1;
}

_MEMBER_FUNCTION_IMPL(Audio, setVolume)
{
	CAudio * pAudio = script_getinstance<CAudio *>(pVM);
	float fVolume;
	script_getfloat ( pVM, 1, &fVolume );
	
	pAudio->SetVolume(fVolume);

	script_pushbool ( pVM, true );
	return 1;
}

_MEMBER_FUNCTION_IMPL(Audio, restoreVolume)
{
	CAudio * pAudio = script_getinstance<CAudio *>(pVM);

	pAudio->Unmute( );

	script_pushbool ( pVM, true );
	return 1;
}

_MEMBER_FUNCTION_IMPL(Audio, getVolume)
{
	CAudio * pAudio = script_getinstance<CAudio *>(pVM);
	script_pushfloat ( pVM, pAudio->GetVolume( ) );
	return 1;
}

_MEMBER_FUNCTION_IMPL(Audio, setPosition)
{
	CAudio * pAudio = script_getinstance<CAudio *>(pVM);

	float fX, fY, fZ, fR;

	script_getfloat ( pVM, 1, &fX );
	script_getfloat ( pVM, 2, &fY );
	script_getfloat ( pVM, 3, &fZ );
	script_getfloat ( pVM, 4, &fR );

	pAudio->SetPosition (CVector3(fX, fY, fZ), fR );

	script_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(Audio, clearPosition)
{
	CAudio * pAudio = script_getinstance<CAudio *>(pVM);

	pAudio->SetUsePosition(false);

	script_pushbool(pVM, true);
	return 1;
}

_MEMBER_FUNCTION_IMPL(Audio, usePositionSystem)
{
	CAudio * pAudio = script_getinstance<CAudio *>(pVM);
	bool bGet;
	script_getbool(pVM, -1, &bGet );
	bool bToggle = (bGet != 0);
	
	pAudio->SetUsePosition(bToggle);

	script_pushbool ( pVM, true );
	return 1;
}
