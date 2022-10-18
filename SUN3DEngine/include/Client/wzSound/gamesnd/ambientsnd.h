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
int ambi_start(int index,	// ���� ambient ���̺� ǥ���� �ε��� ��ȣ.
					int priority=0,   //SND_PRIORITY_OVERLAPPED
															 //SND_PRIORITY_PRIMARY
															 //SND_PRIORITY_NODUPLICATE
															 //SND_PRIORITY_EXCLUSIVE
															 //SND_PRIORITY_QUEUE
					BOOL bLoop=-1,	// TRUE: ����
									// FALSE:  ���� ����.
									// -1: ��ũ��Ʈ�� ������ �ɼǴ�� ����
					float vol_scale = 1.0f,
					BOOL fadein = TRUE, // fade in
					int handle_2 =-1	// cross-fade��ų �����Ʈ ���� �ڵ�
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