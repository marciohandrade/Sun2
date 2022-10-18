#ifndef __WZSOUND_H__
#define __WZSOUND_H__

#include <vector>
#include <map>

// 2006.4.10 1005
//////////////////////////////////////////////////////////////////////////

#ifndef SND_PRIORITY
#define SND_PRIORITY


////////////////////////////
// ����Ʈ ���忡�� �ش��ϴ� �ɼǵ�
////////////////////////////

// ������ ���忡 ���� �ִ� ��� Ƚ�� ����
#define SND_PRIORITY_MUST_PLAY				0	// ������ ���
#define SND_PRIORITY_SINGLE_INSTANCE		1		// 1�� ������ �÷���
#define SND_PRIORITY_DOUBLE_INSTANCE		2	// 2�� ������ �÷���
#define SND_PRIORITY_TRIPLE_INSTANCE		3	// 3�� ������ �÷���

#define SND_INSTANCE_NOLIMIT	SND_PRIORITY_MUST_PLAY
#define SND_INSTANCE_1			SND_PRIORITY_SINGLE_INSTANCE
#define SND_INSTANCE_2			SND_PRIORITY_DOUBLE_INSTANCE
#define SND_INSTANCE_3			SND_PRIORITY_TRIPLE_INSTANCE
#define SND_INSTANCE_4			4
#define SND_INSTANCE_5			5
#define SND_INSTANCE_6			6
#define SND_INSTANCE_7			7
#define SND_INSTANCE_8			8
#define SND_INSTANCE_9			9
#define SND_INSTANCE_10			10

////////////////////////////
// �ں��Ʈ ���忡�� �ش��ϴ� �ɼǵ�
////////////////////////////

#define SND_PRIORITY_OVERLAPPED	0	// �ߺ��ؼ� ������ �Ҹ��� ����.
#define SND_PRIORITY_PRIMARY	1		// �̹� �ش� ���尡 �÷��� ���̸�, �����ϰ� �ٽ� �÷����Ѵ�.
#define SND_PRIORITY_NODUPLICATE	2	// �̹� �ش� ���尡 �÷��� ���̸�, �ߺ��÷��̸� ���� �ʴ´�.
#define SND_PRIORITY_EXCLUSIVE		3	// �̹� �÷������� ��� �ں��Ʈ ���带 �����ϰ�, �ش� ���带 ����Ѵ�.
#define SND_PRIORITY_QUEUE	4		// ���� �÷������� PRIMARY ���尡 ������, �ٷ� ���̾� �÷����Ѵ�.

#endif

// ���� ���� �ʱ�ȭ�� ���� �Է� ����
typedef struct WZSNDINFO
{
	bool use_secure_filesystem;	// ���� ��ȣȭ ���� �ý����� ������� ����
	
	bool enable_debugLog;	 // ����� ��Ʃ��� ����� â �Ǵ�  _bin ������ ����ִ�  DebugView�� 
							// ����� �α׸� ��������� ���� ( true  or  false )
							//  ���� ������ ������ false �� �Ұ��� �����մϴ�.
	//char pszExcelFile[MAX_PATH];	// ���� ���� ��ġ

	char pszConfigFile[MAX_PATH];	// Config ���� ���� ��ġ 
	char pszVoiceFile[MAX_PATH];	// Voice ���� ���� ��ġ 
	char pszMusicFile[MAX_PATH];	// Music ���� ���� ��ġ 
	char pszSoundFile[MAX_PATH];	// Sound ���� ���� ��ġ 

	char pszSecureFileSystem[MAX_PATH];	// ��ŷ ���� ��ġ (*.wpk ����)
	char pszLocalFileSystem[MAX_PATH];	// ���õ�ũ���� ���� �����͸� �ε��� ���, ���� ��Ʈ ��θ� �Է��Ѵ�. 

	DWORD dwQuality;

	WZSNDINFO()
	{
		use_secure_filesystem = false;
		enable_debugLog = false;

		strcpy(pszConfigFile, "Data/Sound/wzsound_config.txt");
		strcpy(pszMusicFile, "Data/Sound/wzsound_music.txt");
		strcpy(pszSoundFile, "Data/Sound/wzsound_sound.txt");
		strcpy(pszVoiceFile, "Data/Sound/wzsound_voice.txt");

		strcpy(pszSecureFileSystem, "Data/Sound/sound.wzs");
		strcpy(pszLocalFileSystem, "Data/Sound");

		dwQuality = 0;	// 0: DS3DALG_NO_VIRTUALIZATION
					    // 1: DS3DALG_HRTF_LIGHT
					    // 2: DS3DALG_HRTF_FULL					    
	}

} WZSNDINFO, *LPWZSNDINFO;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//						I N S T A N C E
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


// Description	    :	initialize sound engine, and load sound table and music table
// Return type		: void 
int wzsnd_init(	LPWZSNDINFO lpWZSNDINFO);	// ���� �ʱ�ȭ�� ����� �ɼ� ����.

// Description	    :	uninitialize sound engine
// Return type		: void 
void wzsnd_uninit();


// Description	    :	�������̺� ������ �ٽ� �ε��մϴ�.
//				�� �Լ��� �θ��� ����, �ݵ�� load�� ���带  unload �Ͻʽÿ�. 
// Return type		: void 
void wzsnd_reload_soundtable();


// Description	    : ���� ���̺� ������ �ٽ� �ε��մϴ�.
// Return type		: void 
void wzsnd_reload_musictable();

// ������Ʈ( ambience) ���� ���̺��� �ٽ� �ε��մϴ�.
//void wzsnd_reload_objecttable();


// Function name	: wzsnd_reload_voicetable
// Description	    : 
// Return type		: void 
void wzsnd_reload_voicetable();


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//						U I	S O U N D
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// Description	    :	precache ui sound samples
// Return type		: void 
void wzsnd_load_ui();

// Description	    :	unload ui sound samples
// Return type		: void 
void wzsnd_unload_ui();

// Description	    :	play 2D user interface sound sample
// Return type		: sound handle
int wzsnd_play_ui(int index,	// sound index in sound_table text script
				float vol_scale = 1.0f);	// multiply default volume(in script) by this factor


void wzsnd_stop_ui(int handle);

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//					G A M E P L A Y		S O U N D
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


// Description	    :	precache gameplay sound samples
// Return type		: void 
void wzsnd_load_gameplay();

// Description	    :  unload gameplay sound samples
// Return type		: void 
void wzsnd_unload_gameplay();


// Description	    :  play 2D gameplay sound sample
// Return type		: int handle  ( sound handle needed to stop, modify this channel if it's looped or has long playtime. )
int wzsnd_play(int index,		// sound index which is described in excel file.
				BOOL bLoop = 0,		// looping or not.
				int maxChannel=-1, // max instance flag
									// -1 : load the parameter value from excel script
									// 0 <=  :  directly input the value, overriding the parameter value written in excel script.

				float volume_scale=1.0f,	// scale default volume by this factor. Default is no scale(1.0f).
				int frequency=0);			// frequency (0: no modulation, 100 ~10000)


////////////////////////////////////
//  3D ���� �÷��� ���� ���� 
////////////////////////////////////

// LFR (left-handed coordinate system)
#ifndef D3DVECTOR_DEFINED
typedef struct _D3DVECTOR {
    float x;
    float y;
    float z;
} D3DVECTOR;
#define D3DVECTOR_DEFINED
#endif

typedef struct SND_SOURCEDESC
{
	/////////////////////
	// ���� ������Ʈ ���� ����
	float flRangeFactor;
	float flMinDistance;	
	float   flMaxDistance;
	float flObjectRadius;	// ������Ʈ�� �ݰ�

	/////////////////////
	// ���� ����
	float flVolumeScale;	// ���� ��ũ��Ʈ�� ǥ���� ����Ʈ ��������   ������ Ű��� ������,  
						// ���� ���� =   ����Ʈ ���� X volume_scale

	BOOL bForcePlay;	//  ������Ʈ�� �����ʿ��� �ʹ� �ְų�, ������������ �ʹ� �۰��ؼ� ���� �ȵ鸱 ��쿡��, 
						    // ������ ���� ���� ������ 1�� �����Ѵ�.
						    // (*����: ���� ���带 force=1�� �����ϸ�, �Ÿ��� �־ �Ҹ��� ������ ���⶧���� 
						    //   ������Ʈ�� ���ӿ��� ���� ���ϰ��  Channel�� ���߶� �ٸ� ���� �ȳ� ���� �ִ�.)

	// frequency ����
	float flFrequency;

	SND_SOURCEDESC()
	{
		flRangeFactor = 1.0f;	// 1.0f�� �� ���Ϳ� �ش��ϴ���. (����Ʈ�� 1m)
		flMinDistance = -1;	// ����Ʈ. ���� ��ũ��Ʈ�� ������ ����.
		flMaxDistance = -1;	// ����Ʈ. ���� ��ũ��Ʈ�� ������ ����.
		flObjectRadius= 0.0f;
		
		flVolumeScale = 1.0f;
		bForcePlay = FALSE;

		flFrequency = 0.0f;
	}
	
} SND_SOURCEDESC, *LPSND_SOURCEDESC;

typedef const SND_SOURCEDESC *LPCSND_SOURCEDESC;


// Description	    :  play mono channel sample by 3d buffer
// Return type		: int handle  ( sound handle needed to stop, modify this channel if it's looped or has long playtime. )
int wzsnd_play_3d(
				int index,	// sound index which is described in excel file.
				D3DVECTOR*  pvListener,	// listener position
				D3DVECTOR* pvPosition,	// source position
				D3DVECTOR*  pvVelocity = NULL,	// source velocity. Default is no movement {0.0f, 0.0f, 0.0f}
												// �ʴ� �̵� �Ÿ� (m/sec)�� ǥ���Ѵ�. (Dopler ���꿡 ���)
				BOOL bLoop = FALSE,	// looping or not
				int maxChannel = -1,	// max instance limit
									// -1 : load the parameter value from excel script
									// 0 <=  :  directly input the value, overriding the parameter value written in excel script.
				LPSND_SOURCEDESC lpDESC = NULL);	// advanced option

// Description	    :  modulate a playing sound buffer.
// Return type	: void
void wzsnd_update_3d_source(int handle, // sound handle returned by 'wzsnd_play_3d'
							D3DVECTOR* pvPosition, 
							D3DVECTOR*  pvVelocity = NULL, 
							LPSND_SOURCEDESC lpDESC = NULL);


void wzsnd_update_listener(D3DVECTOR* pvListener,	// listener position
						D3DVECTOR* pvFront,	// listener's normalized front vector
						D3DVECTOR* pvTop,	// listener's normalized top vector
						D3DVECTOR* pvVelocity = NULL	// listener velocity. (Dopler ���꿡 ���) 
														// - default {0.0f, 0.0f, 0.0f} ��������. 
														// �ʴ� �̵��Ÿ��� ǥ��. ( 1.0f = 1m )
						);

////////////////////////////////////
//  ���� ���� �÷��� ���� ���� 
////////////////////////////////////

// Description	    :  �ش� ���带 ó������ �ǰ��´�. 
// Return type		: void 
//void wzsnd_rewind(int handle);


// Description	    :   �ش� ���带 ����
// Return type		: void 
void wzsnd_stop(int handle);


// Description	    :  ���� �÷��� ���� ��� ���ӻ��带 �����Ѵ�.
// Return type		: void 
void wzsnd_stop_all();


// Description	    :  �ش� ���带 �Ͻ� �����Ѵ�.
// Return type		: void 
void wzsnd_pause(int handle);

// Description	    :  ���� �÷��� ���� ��� ���ӻ��带 �Ͻ����� �Ѵ�.
// Return type		: void 
void wzsnd_pause_all();

// Description	    :  �Ͻ� ������ ���ӻ��带 �ٽ� �÷��� �Ѵ�.
// Return type		: void
void wzsnd_unpause(int handle);

// Description	    :  ��� �Ͻ� ������ ���ӻ��带 �ٽ� �÷��� �Ѵ�.
// Return type		: void 
void wzsnd_unpause_all();

// Description	    :  �ش� ���ӻ����� ������ �����Ѵ�.
// Return type		: void 
void wzsnd_set_volume(int handle, 
						float volume);	// 0.0f ~ 1.0f

// Description	    :  ��� ���ӻ����� ������ �����Ѵ�.
// Return type		: void 
void wzsnd_set_volume_all(float volume);

// Description	    :  �ش� ���� ������ ���� ������ �����Ѵ�.
// Return type		: void 
void wzsnd_set_loop(int handle, BOOL bLoop);


// Description	    :   ���� ������ �÷��� ���¸� ���� 
// Return type		: int 
//					0    stopped
//					1    playing
int wzsnd_getState_sound(
					int handle);	// wzsnd_play �Ǵ� wzsnd_play_3d�� ȣ���ؼ� �Ѱܹ���  �ڵ� 


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//						AMBIENT		S O U N D
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


// Description	    :  ���õ� ambient sound�� �÷����Ѵ�.
// Return type		: int handle 
//				-1 : error
int wzsnd_play_ambi(int index,	// ���� ambient ���̺� ǥ���� �ε��� ��ȣ.
					int priority=SND_PRIORITY_OVERLAPPED,   //SND_PRIORITY_OVERLAPPED
															 //SND_PRIORITY_PRIMARY
															 //SND_PRIORITY_NODUPLICATE
															 //SND_PRIORITY_EXCLUSIVE
															 //SND_PRIORITY_QUEUE

					BOOL bLoop=-1,	// TRUE: ����
									// FALSE:  ���� ����.
									// -1: ��ũ��Ʈ�� ������ �ɼǴ�� ����
					float vol_scale = 1.0f,
					BOOL fadein = TRUE, // fade in
					int handle =-1	// cross-fade��ų �����Ʈ ���� �ڵ� (not implemented yet)
					);


// Description	    : ���õ� ambient sound�� �����Ѵ�.
// Return type		: void 
void wzsnd_stop_ambi(
				int handle,	// ���� ambient ���̺� ǥ���� �ε��� ��ȣ.
				BOOL fadeout = TRUE);
							     // TRUE:  ambient sound��  FADE �Ǹ鼭 ����ȴ�.
							     // FALSE:  ambient sound��  ��� ����ȴ�.

// Description	    :   ��� ambient sound�� �����Ѵ�.
// Return type		: void 
void wzsnd_stop_all_ambi(BOOL fadeout = TRUE);	
								 // TRUE:  ambient sound�� FADE �Ǹ鼭 ����ȴ�.
								 // FALSE:  ambient sound�� ��� ����ȴ�.

// Description	    :  pause a selected ambient sound
// Return type		: void 
void wzsnd_pause_ambi(
					int handle);	// wzsnd_play_ambi ���� ���Ϲ��� �ڵ�

// Description	    :  pause all ambient sound
// Return type		: void 
void wzsnd_pause_all_ambi();	

// Description	    :  unpause a selected ambient sound
// Return type		: void 
void wzsnd_unpause_ambi(
					int handle);	// wzsnd_play_ambi ���� ���Ϲ��� ��Ʈ���� �ڵ�

// Description	    :  unpause all ambient sound
// Return type		: void 
void wzsnd_unpause_all_ambi();

// Description	    :  ���õ� ambient sound�� ������ �����Ѵ�.
// Return type		: void 
void wzsnd_set_volume_ambi(int handle,	// wzsnd_play_ambi ���� ���Ϲ��� �ڵ�
							float volume);	// ���� (0.0f ~1.0f)

// Description	    :  ��� ambient sound�� ������ �����Ѵ�.
// Return type		: void 
void wzsnd_set_volume_all_ambi(float volume);

// Description	    :  ambient sound�� �ݺ� ������ �����Ѵ�.
// Return type		: void 
void wzsnd_set_loop_ambi(int handle, // wzsnd_play_ambi ���� ���Ϲ��� �ڵ�
						BOOL bLoop);	// �ݺ� �ɼ�


// Description	    :  ���� ambient sound�� �÷��� ���¸� ����
// Return type		: int 
//					0    stopped
//					1    playing
//					2    paused
int wzsnd_getState_ambi(
				int handle); // wzsnd_play_ambi ���� ���Ϲ��� �ڵ�


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//						VOICE		S O U N D
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


// Description	    :  ���õ� voice��  �÷����Ѵ�.
// Return type		: int handle 
int wzsnd_play_voice(
				int index,
				BOOL bLoop=FALSE,
				float vol_scale = 1.0f, // ���� ���̽� ���̺� ǥ���� ���� X  vol_sacle = ��������
				BOOL fadein = FALSE);

// Description	    : ���õ� voice�� �����Ѵ�.
// Return type		: void 
void wzsnd_stop_voice(
				int handle,	// wzsnd_play_voice ���� ���Ϲ��� �ڵ鰪
				BOOL fadeout = TRUE);	  // 1  voice���尡 FADE �Ǹ鼭 ����ȴ�.
							     // 0  voice���尡 ��� ����ȴ�.
							     //-1  DEFAULT (��ũ��Ʈ�� ������ ��Ĵ�� �����Ѵ�.)


// Description	    :   ��� voice ���带 �����Ѵ�.
// Return type		: void 
void wzsnd_stop_all_voice(
					BOOL fadeout = TRUE);	// 1  voice���尡 FADE �Ǹ鼭 ����ȴ�.
								 // 0  voice���尡 ��� ����ȴ�.								

// Description	    :  pause a selected audio stream
// Return type		: void 
void wzsnd_pause_voice(
					int handle);	// wzsnd_play_voice ���� ���Ϲ��� ��Ʈ���� �ڵ�

// Description	    :  pause all audio streams
// Return type		: void 
void wzsnd_pause_all_voice();	

// Description	    :  unpause a selected audio stream
// Return type		: void 
void wzsnd_unpause_voice(
					int handle);	// wzsnd_play_voice ���� ���Ϲ��� ��Ʈ���� �ڵ�

// Description	    :  unpause all audio streams
// Return type		: void 
void wzsnd_unpause_all_voice();

// Description	    :  ���õ� voice�� ������ �����Ѵ�.
// Return type		: void 
void wzsnd_set_volume_voice(int handle,	// wzsnd_play_voice ���� ���Ϲ��� ��Ʈ���� �ڵ�
							float volume);	// ���� (0.0f ~1.0f)

// Description	    :  ��� voice�� ������ �����Ѵ�.
// Return type		: void 
void wzsnd_set_volume_all_voice(float volume);

// Description	    :  ���̽��� �ݺ� ������ �����Ѵ�.
// Return type		: void 
void wzsnd_set_loop_voice(int handle, // wzsnd_play_voice ���� ���Ϲ��� ��Ʈ���� �ڵ�
						BOOL bLoop);	// �ݺ� �ɼ�


// Description	    :  ���� voice�� �÷��� ���¸� ����
// Return type		: int 
//					0    stopped
//					1    playing
//					2    paused
int wzsnd_getState_voice(
				int handle); // wzsnd_play_voice ���� ���Ϲ��� ��Ʈ���� �ڵ�


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//						B G M		S O U N D
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// Description	    :  ���õ� BGM��  �÷����Ѵ�.
// Return type		: int handle 
int wzsnd_play_bgm(
				int index,	// ���� ���� ���̺� ǥ���� �ε��� ��ȣ.
				BOOL bLoop = FALSE,
				float vol_scale=1.0f,
				BOOL fadein = TRUE);


// Description	    : ���õ� BGM�� �����Ѵ�.
// Return type		: void 
void wzsnd_stop_bgm(
				int handle,	// ���� ���� ���̺� ǥ���� �ε��� ��ȣ.
				BOOL fadeout = TRUE);	  // 1  BGM���尡 FADE �Ǹ鼭 ����ȴ�.
							     // 0  BGM���尡 ��� ����ȴ�.
							     //-1  DEFAULT (��ũ��Ʈ�� ������ ��Ĵ�� �����Ѵ�.)


// Description	    :   ��� BGM ���带 �����Ѵ�.
// Return type		: void 
void wzsnd_stop_all_bgm(
					BOOL fadeout = TRUE);	// 1  BGM���尡 FADE �Ǹ鼭 ����ȴ�.
								 // 0  BGM���尡 ��� ����ȴ�.								

// Description	    :  pause a selected audio stream
// Return type		: void 
void wzsnd_pause_bgm(
					int handle);	// wzsnd_play_bgm ���� ���Ϲ��� ��Ʈ���� �ڵ�

// Description	    :  pause all audio streams
// Return type		: void 
void wzsnd_pause_all_bgm();	

// Description	    :  unpause a selected audio stream
// Return type		: void 
void wzsnd_unpause_bgm(
					int handle);	// wzsnd_play_bgm ���� ���Ϲ��� ��Ʈ���� �ڵ�

// Description	    :  unpause all audio streams
// Return type		: void 
void wzsnd_unpause_all_bgm();

// Description	    :  ���õ� BGM�� ������ �����Ѵ�.
// Return type		: void 
void wzsnd_set_volume_bgm(int handle,	// wzsnd_play_bgm ���� ���Ϲ��� ��Ʈ���� �ڵ�
							float volume);	// ���� (0.0f ~1.0f)

// Description	    :  ��� BGM�� ������ �����Ѵ�.
// Return type		: void 
void wzsnd_set_volume_all_bgm(float volume);

// Description	    :  ������ �ݺ� ������ �����Ѵ�.
// Return type		: void 
void wzsnd_set_loop_bgm(int handle, // wzsnd_play_bgm ���� ���Ϲ��� ��Ʈ���� �ڵ�
						BOOL bLoop);	// �ݺ� �ɼ�


// Description	    :  ���� BGM�� �÷��� ���¸� ����
// Return type		: int 
//					0    stopped
//					1    playing
//					2    paused
int wzsnd_getState_bgm(
				int handle); // wzsnd_play_bgm ���� ���Ϲ��� ��Ʈ���� �ڵ�


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//						U T I L I T Y
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#define MAX_GAMEPLAY_SOUNDS	1000
#define MAX_AMBIENT_SOUNDS	200
typedef struct soundEntry
{
	int index;
	char pszFilename[MAX_PATH];
} soundEntry;

// Description	    :  get list of parsed sound table
// Return type		: void 
int wzsnd_get_soundtable(soundEntry* sndEntries);

int wzsnd_get_soundtableArray(soundEntry* sndEntries);

int wzsnd_get_ambienttable(soundEntry* sndEntries);
int wzsnd_get_interfacesoundtable(soundEntry* sndEntries);



////////////////////////////////////
// master volume, mute control

// Description	    :  ������ ������ �����Ѵ�.
// Return type		: void 
void wzsnd_set_master_vol(int LeftVol,  // left channel volume (min 0 ~ max 100)
						int RightVol);		// right channel volume (min 0 ~ max 100)

// Description	    :  ���� ������ ���� ������ �����´�.
// Return type		: void 
void wzsnd_get_master_vol(int* pLeftVol,		// left channel volume (min 0 ~ max 100)
						int* pRightVol);	// right channel volume (min 0 ~ max 100)

// Description	    :  ������ ���ҰŸ� �����Ѵ�.
// Return type		: void 
void wzsnd_set_master_mute(bool mute = true);	// true: mute on,  false: mute off

// Description	    :  ������ ���Ұ� ���¸� �����´�.
// Return type		: void 
bool wzsnd_get_master_mute();


////////////////////////
// wave mixer volume
//
// Description : ä�κ��� ������ �Ҵ��� �� �ִ�.
void wzsnd_set_wav_vol(int LeftVol,	// min 0 ~ max 100
						int RightVol); // min 0 ~ max 100

void wzsnd_get_wav_vol(int* pLeftVol,  // min 0 ~ max 100
						int* pRightVol); // min 0 ~ max 100


// wave mute
void wzsnd_set_wav_mute(bool mute = true);
bool wzsnd_get_wav_mute();

////////////////////////
// input mic recording volume
void wzsnd_set_mic_input_vol(
		       DWORD dwVol	// min 0 ~ max 100
		       );
void wzsnd_get_mic_input_vol(
		       DWORD* pdwVol	// min 0 ~ max 100
		       );


// output mic recording volume
void wzsnd_set_mic_output_vol(
		       DWORD dwVol	// min 0 ~ max 100
		       );
void wzsnd_get_mic_output_vol(
		       DWORD* pdwVol	// min 0 ~ max 100
		       );

void wzsnd_set_mic_output_mute(bool mute = true);
bool wzsnd_get_mic_output_mute();


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//					E N V I R O N M E N T ( EAX )
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

typedef struct sound_env
{
	unsigned long id;	// sound environment ID
	float volume;
	float damping;
	float decay;
} sound_env;

// Description	    :  set EAX parameters
// Return type		: void 
void wzsnd_env_set(sound_env *se);


// sound environment
typedef enum SND_ENV_ID
{
    SND_ENV_GENERIC,
    SND_ENV_PADDEDCELL,
    SND_ENV_ROOM,
    SND_ENV_BATHROOM,
    SND_ENV_LIVINGROOM,
    SND_ENV_STONEROOM,
    SND_ENV_AUDITORIUM,
    SND_ENV_CONCERTHALL,
    SND_ENV_CAVE,
    SND_ENV_ARENA,
    SND_ENV_HANGAR,
    SND_ENV_CARPETEDHALLWAY,
    SND_ENV_HALLWAY,
    SND_ENV_STONECORRIDOR,
    SND_ENV_ALLEY,
    SND_ENV_FOREST,
    SND_ENV_CITY,
    SND_ENV_MOUNTAINS,
    SND_ENV_QUARRY,
    SND_ENV_PLAIN,
    SND_ENV_PARKINGLOT,
    SND_ENV_SEWERPIPE,
    SND_ENV_UNDERWATER,
    SND_ENV_DRUGGED,
    SND_ENV_DIZZY,
    SND_ENV_PSYCHOTIC,
} SND_ENV_ID;

// Description	    :  apply preset EAX parameters
// Return type		: void 
void wzsnd_env_set_preset(unsigned long envid);

// Description	    :  get current EAX parameters
// Return type		: void 
void wzsnd_env_get(sound_env *se);

// Description	    :  disable EAX
// Return type		: void 
void wzsnd_env_disable();

// Description	    :  check user's hardware support for EAX
// Return type		: void 
BOOL wzsnd_env_supported();


//------------------------------------------------------------------------------
void wzsnd_check_channel();
void wzsnd_update_cache();  //!< ���� �������� ȣ��
void wzsnd_cache_maintain_milliseconds_set(int val);
int  wzsnd_cache_maintain_milliseconds_get();
void wzsnd_cache_on();
void wzsnd_cache_off();
bool wzsnd_cache_is();
int  wzsnd_cache_size();
//------------------------------------------------------------------------------




#endif //__WZSOUND_H__
