#ifndef __VOICESND_H__
#define __VOICESND_H__

#include <Windows.h>
#include "PreProcDefines.h"
#include "gamesnd.h"

//////////////////////////////////////////////////////////////////////////

/*typedef struct VoiceSnd
{
	int sig;	// sound index
	char	filename[MAX_PATH];
	float	default_volume;		// range: 0.0 -> 1.0
	int loop;
	int	flags;
//	BYTE FadeInDelay;
//	BYTE FadeOutDelay;

	CMusicSegment* pSegment;
} VoiceSnd;*/


typedef struct VoiceSnd 
{
	char filename[MAX_FILENAME_LEN];	// name music is stored on disk as
	int loop;
	int fadeout_on_close;
	float default_volume;				// name music is known by		
} VoiceSnd;

#define MAX_VOICE_SOUND	1024

extern int Num_voice_files;
extern VoiceSnd voice_snd[MAX_VOICE_SOUND];

void voice_init();
void voice_close();
//int voice_start(int index,	// 엑셀 ambient 테이블에 표기한 인덱스 번호.
//					int priority=SND_PRIORITY_OVERLAPPED,   //SND_PRIORITY_OVERLAPPED
//															 //SND_PRIORITY_PRIMARY
//															 //SND_PRIORITY_NODUPLICATE
//															 //SND_PRIORITY_EXCLUSIVE
//															 //SND_PRIORITY_QUEUE
//
//					BOOL bLoop=-1,	// TRUE: 루프
//									// FALSE:  루프 안함.
//									// -1: 스크립트에 지정한 옵션대로 수행
//
//					BOOL fadein = TRUE, // fade in
//					int handle =-1	// cross-fade시킬 엠비언트 사운드 핸들
//					);

int voice_start(int n, float vol_scale, BOOL fadein);
void voice_stop(int handle, int fadeout=1);
void voice_stop_all(int fadeout=1);
void voice_pause(int handle);
void voice_pause_all();
void voice_unpause(int handle);
void voice_unpause_all();
void voice_set_volume(int handle, float volume);
void voice_set_volume_all(float volume);
void voice_set_loop(int handle, BOOL bLoop);
int voice_state(int handle);

#endif //__VOICESND_H__