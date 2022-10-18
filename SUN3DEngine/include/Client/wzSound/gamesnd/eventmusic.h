#ifndef __EVENT_MUSIC_H__
#define __EVENT_MUSIC_H__

#include "PreProcDefines.h"


////////////////////////////////////////////

typedef struct EventMusic 
{
	char filename[MAX_FILENAME_LEN];
	int loop;
	int fadeout_on_close;
	float default_volume;
} EventMusic;

#define MAX_EVENT_MUSIC	100 //2008-12-04 30 ==> 100 으로 변경
extern int Num_music_files;
extern EventMusic Event_music[MAX_EVENT_MUSIC];

BOOL event_music_init();
void event_music_close();
int event_music_start(int n, float vol_scale, BOOL fadein);
void event_music_stop(int handle, int fade=1);
void event_music_stop_all(int fade=1);
void event_music_pause(int handle);
void event_music_pause_all();
void event_music_unpause(int handle);
void event_music_unpause_all();
void event_music_set_volume(int handle, float volume);
void event_music_set_volume_all(float volume);
void event_music_set_loop(int handle, BOOL bLoop);
int event_music_state(int handle);


#endif /* __EVENT_MUSIC_H__  */
