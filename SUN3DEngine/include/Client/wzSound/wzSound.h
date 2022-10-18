#ifndef __WZSOUND_H__
#define __WZSOUND_H__

#include <vector>
#include <map>

// 2006.4.10 1005
//////////////////////////////////////////////////////////////////////////

#ifndef SND_PRIORITY
#define SND_PRIORITY


////////////////////////////
// 이펙트 사운드에만 해당하는 옵션들
////////////////////////////

// 동일한 사운드에 대한 최대 출력 횟수 지정
#define SND_PRIORITY_MUST_PLAY				0	// 무제한 출력
#define SND_PRIORITY_SINGLE_INSTANCE		1		// 1개 까지만 플레이
#define SND_PRIORITY_DOUBLE_INSTANCE		2	// 2개 까지만 플레이
#define SND_PRIORITY_TRIPLE_INSTANCE		3	// 3개 까지만 플레이

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
// 앰비언트 사운드에만 해당하는 옵션들
////////////////////////////

#define SND_PRIORITY_OVERLAPPED	0	// 중복해서 무조건 소리를 낸다.
#define SND_PRIORITY_PRIMARY	1		// 이미 해당 사운드가 플레이 중이면, 중지하고 다시 플레이한다.
#define SND_PRIORITY_NODUPLICATE	2	// 이미 해당 사운드가 플레이 중이면, 중복플레이를 하지 않는다.
#define SND_PRIORITY_EXCLUSIVE		3	// 이미 플레이중인 모든 앰비언트 사운드를 중지하고, 해당 사운드를 출력한다.
#define SND_PRIORITY_QUEUE	4		// 현재 플레이중인 PRIMARY 사운드가 끝나면, 바로 뒤이어 플레이한다.

#endif

// 사운드 엔진 초기화를 위한 입력 정보
typedef struct WZSNDINFO
{
	bool use_secure_filesystem;	// 압축 암호화 파일 시스템을 사용할지 여부
	
	bool enable_debugLog;	 // 비쥬얼 스튜디오 디버그 창 또는  _bin 폴더에 들어있는  DebugView로 
							// 디버깅 로그를 출력할지를 지정 ( true  or  false )
							//  정식 릴리즈 에서는 false 로 할것을 권장합니다.
	//char pszExcelFile[MAX_PATH];	// 엑셀 파일 위치

	char pszConfigFile[MAX_PATH];	// Config 정보 파일 위치 
	char pszVoiceFile[MAX_PATH];	// Voice 정보 파일 위치 
	char pszMusicFile[MAX_PATH];	// Music 정보 파일 위치 
	char pszSoundFile[MAX_PATH];	// Sound 정보 파일 위치 

	char pszSecureFileSystem[MAX_PATH];	// 패킹 파일 위치 (*.wpk 포맷)
	char pszLocalFileSystem[MAX_PATH];	// 로컬디스크에서 사운드 데이터를 로딩할 경우, 사운드 루트 경로를 입력한다. 

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
int wzsnd_init(	LPWZSNDINFO lpWZSNDINFO);	// 사운드 초기화에 사용할 옵션 정보.

// Description	    :	uninitialize sound engine
// Return type		: void 
void wzsnd_uninit();


// Description	    :	사운드테이블 정보를 다시 로드합니다.
//				이 함수를 부르기 전에, 반드시 load된 사운드를  unload 하십시요. 
// Return type		: void 
void wzsnd_reload_soundtable();


// Description	    : 뮤직 테이블 정보를 다시 로드합니다.
// Return type		: void 
void wzsnd_reload_musictable();

// 오브젝트( ambience) 사운드 테이블을 다시 로드합니다.
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
//  3D 게임 플레이 사운드 제어 
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
	// 사운드 오브젝트 범위 설정
	float flRangeFactor;
	float flMinDistance;	
	float   flMaxDistance;
	float flObjectRadius;	// 오브젝트의 반경

	/////////////////////
	// 볼륨 설정
	float flVolumeScale;	// 사운드 스크립트에 표기한 디폴트 볼륨에서   음량을 키우고 싶을때,  
						// 최종 볼륨 =   디폴트 볼륨 X volume_scale

	BOOL bForcePlay;	//  오브젝트가 리서너에서 너무 멀거나, 볼륨스케일을 너무 작게해서 음이 안들릴 경우에도, 
						    // 강제로 음을 내고 싶을때 1로 세팅한다.
						    // (*주의: 많은 사운드를 force=1로 세팅하면, 거리가 멀어도 소리가 무조건 나기때문에 
						    //   오브젝트가 게임에서 많이 보일경우  Channel이 모잘라 다른 음이 안날 수도 있다.)

	// frequency 조절
	float flFrequency;

	SND_SOURCEDESC()
	{
		flRangeFactor = 1.0f;	// 1.0f이 몇 미터에 해당하는지. (디폴트는 1m)
		flMinDistance = -1;	// 디폴트. 엑셀 스크립트에 지정한 범위.
		flMaxDistance = -1;	// 디폴트. 엑셀 스크립트에 지정한 범위.
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
												// 초당 이동 거리 (m/sec)를 표기한다. (Dopler 연산에 사용)
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
						D3DVECTOR* pvVelocity = NULL	// listener velocity. (Dopler 연산에 사용) 
														// - default {0.0f, 0.0f, 0.0f} 정지상태. 
														// 초당 이동거리를 표시. ( 1.0f = 1m )
						);

////////////////////////////////////
//  공통 게임 플레이 사운드 제어 
////////////////////////////////////

// Description	    :  해당 사운드를 처음으로 되감는다. 
// Return type		: void 
//void wzsnd_rewind(int handle);


// Description	    :   해당 사운드를 중지
// Return type		: void 
void wzsnd_stop(int handle);


// Description	    :  현재 플레이 중인 모든 게임사운드를 중지한다.
// Return type		: void 
void wzsnd_stop_all();


// Description	    :  해당 사운드를 일시 정지한다.
// Return type		: void 
void wzsnd_pause(int handle);

// Description	    :  현재 플레이 중인 모든 게임사운드를 일시정지 한다.
// Return type		: void 
void wzsnd_pause_all();

// Description	    :  일시 정지된 게임사운드를 다시 플레이 한다.
// Return type		: void
void wzsnd_unpause(int handle);

// Description	    :  모든 일시 정지된 게임사운드를 다시 플레이 한다.
// Return type		: void 
void wzsnd_unpause_all();

// Description	    :  해당 게임사운드의 볼륨을 조정한다.
// Return type		: void 
void wzsnd_set_volume(int handle, 
						float volume);	// 0.0f ~ 1.0f

// Description	    :  모든 게임사운드의 볼륨을 조정한다.
// Return type		: void 
void wzsnd_set_volume_all(float volume);

// Description	    :  해당 게임 사운드의 루프 설정을 변경한다.
// Return type		: void 
void wzsnd_set_loop(int handle, BOOL bLoop);


// Description	    :   현재 사운드의 플레이 상태를 리턴 
// Return type		: int 
//					0    stopped
//					1    playing
int wzsnd_getState_sound(
					int handle);	// wzsnd_play 또는 wzsnd_play_3d를 호출해서 넘겨받은  핸들 


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//						AMBIENT		S O U N D
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


// Description	    :  선택된 ambient sound를 플레이한다.
// Return type		: int handle 
//				-1 : error
int wzsnd_play_ambi(int index,	// 엑셀 ambient 테이블에 표기한 인덱스 번호.
					int priority=SND_PRIORITY_OVERLAPPED,   //SND_PRIORITY_OVERLAPPED
															 //SND_PRIORITY_PRIMARY
															 //SND_PRIORITY_NODUPLICATE
															 //SND_PRIORITY_EXCLUSIVE
															 //SND_PRIORITY_QUEUE

					BOOL bLoop=-1,	// TRUE: 루프
									// FALSE:  루프 안함.
									// -1: 스크립트에 지정한 옵션대로 수행
					float vol_scale = 1.0f,
					BOOL fadein = TRUE, // fade in
					int handle =-1	// cross-fade시킬 엠비언트 사운드 핸들 (not implemented yet)
					);


// Description	    : 선택된 ambient sound를 정지한다.
// Return type		: void 
void wzsnd_stop_ambi(
				int handle,	// 엑셀 ambient 테이블에 표기한 인덱스 번호.
				BOOL fadeout = TRUE);
							     // TRUE:  ambient sound가  FADE 되면서 종료된다.
							     // FALSE:  ambient sound가  즉시 종료된다.

// Description	    :   모든 ambient sound를 종료한다.
// Return type		: void 
void wzsnd_stop_all_ambi(BOOL fadeout = TRUE);	
								 // TRUE:  ambient sound가 FADE 되면서 종료된다.
								 // FALSE:  ambient sound가 즉시 종료된다.

// Description	    :  pause a selected ambient sound
// Return type		: void 
void wzsnd_pause_ambi(
					int handle);	// wzsnd_play_ambi 에서 리턴받은 핸들

// Description	    :  pause all ambient sound
// Return type		: void 
void wzsnd_pause_all_ambi();	

// Description	    :  unpause a selected ambient sound
// Return type		: void 
void wzsnd_unpause_ambi(
					int handle);	// wzsnd_play_ambi 에서 리턴받은 스트리밍 핸들

// Description	    :  unpause all ambient sound
// Return type		: void 
void wzsnd_unpause_all_ambi();

// Description	    :  선택된 ambient sound의 볼륨을 변경한다.
// Return type		: void 
void wzsnd_set_volume_ambi(int handle,	// wzsnd_play_ambi 에서 리턴받은 핸들
							float volume);	// 볼륨 (0.0f ~1.0f)

// Description	    :  모든 ambient sound의 볼륨을 변경한다.
// Return type		: void 
void wzsnd_set_volume_all_ambi(float volume);

// Description	    :  ambient sound의 반복 설정을 변경한다.
// Return type		: void 
void wzsnd_set_loop_ambi(int handle, // wzsnd_play_ambi 에서 리턴받은 핸들
						BOOL bLoop);	// 반복 옵션


// Description	    :  현재 ambient sound의 플레이 상태를 리턴
// Return type		: int 
//					0    stopped
//					1    playing
//					2    paused
int wzsnd_getState_ambi(
				int handle); // wzsnd_play_ambi 에서 리턴받은 핸들


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//						VOICE		S O U N D
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


// Description	    :  선택된 voice을  플레이한다.
// Return type		: int handle 
int wzsnd_play_voice(
				int index,
				BOOL bLoop=FALSE,
				float vol_scale = 1.0f, // 엑셀 보이스 테이블에 표기한 볼륨 X  vol_sacle = 최종볼륨
				BOOL fadein = FALSE);

// Description	    : 선택된 voice을 정지한다.
// Return type		: void 
void wzsnd_stop_voice(
				int handle,	// wzsnd_play_voice 부터 리턴받은 핸들값
				BOOL fadeout = TRUE);	  // 1  voice사운드가 FADE 되면서 종료된다.
							     // 0  voice사운드가 즉시 종료된다.
							     //-1  DEFAULT (스크립트에 지정한 방식대로 종료한다.)


// Description	    :   모든 voice 사운드를 종료한다.
// Return type		: void 
void wzsnd_stop_all_voice(
					BOOL fadeout = TRUE);	// 1  voice사운드가 FADE 되면서 종료된다.
								 // 0  voice사운드가 즉시 종료된다.								

// Description	    :  pause a selected audio stream
// Return type		: void 
void wzsnd_pause_voice(
					int handle);	// wzsnd_play_voice 에서 리턴받은 스트리밍 핸들

// Description	    :  pause all audio streams
// Return type		: void 
void wzsnd_pause_all_voice();	

// Description	    :  unpause a selected audio stream
// Return type		: void 
void wzsnd_unpause_voice(
					int handle);	// wzsnd_play_voice 에서 리턴받은 스트리밍 핸들

// Description	    :  unpause all audio streams
// Return type		: void 
void wzsnd_unpause_all_voice();

// Description	    :  선택된 voice의 볼륨을 변경한다.
// Return type		: void 
void wzsnd_set_volume_voice(int handle,	// wzsnd_play_voice 에서 리턴받은 스트리밍 핸들
							float volume);	// 볼륨 (0.0f ~1.0f)

// Description	    :  모든 voice의 볼륨을 변경한다.
// Return type		: void 
void wzsnd_set_volume_all_voice(float volume);

// Description	    :  보이스의 반복 설정을 변경한다.
// Return type		: void 
void wzsnd_set_loop_voice(int handle, // wzsnd_play_voice 에서 리턴받은 스트리밍 핸들
						BOOL bLoop);	// 반복 옵션


// Description	    :  현재 voice의 플레이 상태를 리턴
// Return type		: int 
//					0    stopped
//					1    playing
//					2    paused
int wzsnd_getState_voice(
				int handle); // wzsnd_play_voice 에서 리턴받은 스트리밍 핸들


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//						B G M		S O U N D
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// Description	    :  선택된 BGM을  플레이한다.
// Return type		: int handle 
int wzsnd_play_bgm(
				int index,	// 엑셀 뮤직 테이블에 표기한 인덱스 번호.
				BOOL bLoop = FALSE,
				float vol_scale=1.0f,
				BOOL fadein = TRUE);


// Description	    : 선택된 BGM을 정지한다.
// Return type		: void 
void wzsnd_stop_bgm(
				int handle,	// 엑셀 뮤직 테이블에 표기한 인덱스 번호.
				BOOL fadeout = TRUE);	  // 1  BGM사운드가 FADE 되면서 종료된다.
							     // 0  BGM사운드가 즉시 종료된다.
							     //-1  DEFAULT (스크립트에 지정한 방식대로 종료한다.)


// Description	    :   모든 BGM 사운드를 종료한다.
// Return type		: void 
void wzsnd_stop_all_bgm(
					BOOL fadeout = TRUE);	// 1  BGM사운드가 FADE 되면서 종료된다.
								 // 0  BGM사운드가 즉시 종료된다.								

// Description	    :  pause a selected audio stream
// Return type		: void 
void wzsnd_pause_bgm(
					int handle);	// wzsnd_play_bgm 에서 리턴받은 스트리밍 핸들

// Description	    :  pause all audio streams
// Return type		: void 
void wzsnd_pause_all_bgm();	

// Description	    :  unpause a selected audio stream
// Return type		: void 
void wzsnd_unpause_bgm(
					int handle);	// wzsnd_play_bgm 에서 리턴받은 스트리밍 핸들

// Description	    :  unpause all audio streams
// Return type		: void 
void wzsnd_unpause_all_bgm();

// Description	    :  선택된 BGM의 볼륨을 변경한다.
// Return type		: void 
void wzsnd_set_volume_bgm(int handle,	// wzsnd_play_bgm 에서 리턴받은 스트리밍 핸들
							float volume);	// 볼륨 (0.0f ~1.0f)

// Description	    :  모든 BGM의 볼륨을 변경한다.
// Return type		: void 
void wzsnd_set_volume_all_bgm(float volume);

// Description	    :  뮤직의 반복 설정을 변경한다.
// Return type		: void 
void wzsnd_set_loop_bgm(int handle, // wzsnd_play_bgm 에서 리턴받은 스트리밍 핸들
						BOOL bLoop);	// 반복 옵션


// Description	    :  현재 BGM의 플레이 상태를 리턴
// Return type		: int 
//					0    stopped
//					1    playing
//					2    paused
int wzsnd_getState_bgm(
				int handle); // wzsnd_play_bgm 에서 리턴받은 스트리밍 핸들


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

// Description	    :  마스터 볼륨을 변경한다.
// Return type		: void 
void wzsnd_set_master_vol(int LeftVol,  // left channel volume (min 0 ~ max 100)
						int RightVol);		// right channel volume (min 0 ~ max 100)

// Description	    :  현재 마스터 볼륨 설정을 가져온다.
// Return type		: void 
void wzsnd_get_master_vol(int* pLeftVol,		// left channel volume (min 0 ~ max 100)
						int* pRightVol);	// right channel volume (min 0 ~ max 100)

// Description	    :  마스터 음소거를 세팅한다.
// Return type		: void 
void wzsnd_set_master_mute(bool mute = true);	// true: mute on,  false: mute off

// Description	    :  마스터 음소거 상태를 가져온다.
// Return type		: void 
bool wzsnd_get_master_mute();


////////////////////////
// wave mixer volume
//
// Description : 채널별로 볼륨을 할당할 수 있다.
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
void wzsnd_update_cache();  //!< 일정 간경으로 호출
void wzsnd_cache_maintain_milliseconds_set(int val);
int  wzsnd_cache_maintain_milliseconds_get();
void wzsnd_cache_on();
void wzsnd_cache_off();
bool wzsnd_cache_is();
int  wzsnd_cache_size();
//------------------------------------------------------------------------------




#endif //__WZSOUND_H__
