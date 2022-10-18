
#include "PreProcDefines.h"
#include <windows.h>
#include <dsound.h>

#ifndef __CHANNEL_H__
#define __CHANNEL_H__

struct channel
{
	//int							sig;			
    int                     playing_identifies;     // uniquely identifies the sound playing on the channel
	//int	                    sound_identifies;		// identifies which kind of sound is playing
	LPDIRECTSOUNDBUFFER		pdsb;			// pointer to the secondary buffer that was duplicated 
	LPDIRECTSOUND3DBUFFER	pds3db;		// 3D interface, only used if sound buffer created with CTRL3D flag
	int						looping;		// flag to indicate that the sound is looping
	int						vol;			// in DirectSound units
	int						priority;	// implementation dependant priority
	bool					is_voice_msg;
	DWORD		            last_position;
	bool					is_paused;

	int map_script_key;
    int map_sounds_key;
};


#define DEFAULT_MAX_CHANNELS	32
extern int MAX_CHANNELS;
extern channel* Channels;


#endif /* __CHANNEL_H__ */
