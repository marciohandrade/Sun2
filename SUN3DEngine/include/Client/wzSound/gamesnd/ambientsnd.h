#ifndef __AMBIENTSND_H__
#define __AMBIENTSND_H__
// ambientsnd.h



typedef struct AmbientSnd
{
	int sig;	// sound index
	char	filename[MAX_PATH];
	float	default_volume;		// range: 0.0 -> 1.0
	int loop;
	int	flags;
	BYTE fadeout_on_close;
	
//	BYTE FadeInDelay;
//	BYTE FadeOutDelay;
//	CMusicSegment* pSegment;

} AmbientSnd;

void ambi_init();
void ambi_close();
int ambi_start(int index,	// 엑셀 ambient 테이블에 표기한 인덱스 번호.
					int priority=0,   //SND_PRIORITY_OVERLAPPED
															 //SND_PRIORITY_PRIMARY
															 //SND_PRIORITY_NODUPLICATE
															 //SND_PRIORITY_EXCLUSIVE
															 //SND_PRIORITY_QUEUE
					BOOL bLoop=-1,	// TRUE: 루프
									// FALSE:  루프 안함.
									// -1: 스크립트에 지정한 옵션대로 수행
					float vol_scale = 1.0f,
					BOOL fadein = TRUE, // fade in
					int handle_2 =-1	// cross-fade시킬 엠비언트 사운드 핸들
					);

void ambi_stop(int handle, int fadeout=1);
void ambi_stop_all(int fadeout=1);
void ambi_pause(int handle);
void ambi_pause_all();
void ambi_unpause(int handle);
void ambi_unpause_all();
void ambi_set_volume(int handle, float volume);
void ambi_set_volume_all(float volume);
void ambi_set_loop(int handle, BOOL bLoop);
int ambi_state(int handle);

#endif //__AMBIENTSND_H__