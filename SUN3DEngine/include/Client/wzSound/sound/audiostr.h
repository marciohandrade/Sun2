#ifndef _AUDIOSTR_H
#define _AUDIOSTR_H

// type of audio stream
#define ASF_SOUNDFX			0
#define ASF_EVENTMUSIC		1
#define ASF_VOICE				2
#define ASF_AMBIENT				3
#define ASF_NONE				4		// used to catch errors

#define NEED_STRHDL		// for STRHTL struct in audiostr.h
#ifdef NEED_STRHDL

#include <mmsystem.h>

#include "cfile/cfile.h"		// needed for cf_get_path
#include "sound/ogg/ogg.h"

#pragma warning(disable: 4786)
#include <map>
typedef std::map<int, int, std::less<int> > int2int_map;
typedef int2int_map::iterator int2int_iter;

// audio stream file handle information
typedef struct 
{
	HMMIO hmmio;		// handle for mmio
	CFILE* cfp;

	long true_offset;	// true offset of file into VP
	uint size;			// total size of file being read

	// for OGGs
	OggVorbis_File vorbis_file;	// vorbis file info
} STRHDL;
#endif

// Initializes the audio streaming library.  Called
// automatically when the sound stuff is inited.
void audiostream_init();

// Closes down the audio streaming library
void audiostream_close();

// Opens a wave file but doesn't play it.
int audiostream_open( char * filename, int type, float fBaseVolume, BOOL bEncrypted = FALSE );

// Closes the opened wave file.  This doesn't have to be
// called between songs, because when you open the next
// song, it will call this internally.
void audiostream_close_file(int i, int fade = 1);

void audiostream_close_all(int fade, int type=-1);

// Plays the currently opened wave file
void audiostream_play(int i, float volume = -1.0f, int looping = 1, BOOL bFadeIn = FALSE);

// See if a particular stream is playing
int audiostream_is_playing(int i);

// Stops the currently opened wave file
void audiostream_stop(int i, int rewind = 1, int paused = 0);

// set the volume for every audio stream of a particular type
void audiostream_set_volume_all(float volume_scale, int type);

// set the volume for a particular audio stream
void audiostream_set_volume(int i, float volume_scale);

// see if a particular stream is paused
int audiostream_is_paused(int i);

void audiostream_set_looping(int i, BOOL bLoop);

// set the number of samples that the sound should cutoff after
void audiostream_set_sample_cutoff(int i, unsigned int cutoff);

// return the number of samples streamed to the Direct Sound buffer so far
unsigned int audiostream_get_samples_committed(int i);

// check if the streaming has read all the bytes from disk yet
int audiostream_done_reading(int i);

// return if audiostream has initialized ok
int audiostream_is_inited();

void audiostream_pause(int i);	// pause a particular stream
void audiostream_pause_all(int type=-1);	// pause all audio streams											

void audiostream_unpause(int i);	// unpause a particular stream
void audiostream_unpause_all(int type=-1);	// unpause all audio streams


#endif // _AUDIOSTR_H
