#include "SunClientPrecompiledHeader.h"
#include "Sound.h"
#include <time.h>
#include "Camera.h"
#include "Hero.h"
#include "Map.h"
#include "Interface/uiPartyMan/uiPartyMan.h"
#include <atltime.h>


//------------------------------------------------------------------------------
/**
*/
Sound::Sound( void ) :	m_bSoundFlag(USE_SOUND_LIBRARY)
						,m_bIsUseEAX(TRUE)
                        ,calltick_updatelistener_(500)
                        ,calltick_cache_(300000)        //!< 5 * 60 * 1000 = 5분
                        //,debug_file_sound_cache_("")
{
}

//------------------------------------------------------------------------------
/**
*/
Sound::~Sound( void )
{
}

//------------------------------------------------------------------------------
/**
*/
BOOL Sound::Create( WZSNDINFO * pWzSndInfo )
{    
	///////////////
	// initialize sound engine

	int iRet = 0;
	if (m_bSoundFlag)
	{
		iRet = wzsnd_init(pWzSndInfo);
	}
	else
	{
		iRet = -1;
	}

	if (iRet < 0)
	{
		m_bIsUseBGMSound	= FALSE;
		m_bIsUseEffectSound = FALSE;
		m_bIsUseEAX			= FALSE;
	}
	else
	{
		// TODO : 이부분은 LoadInfo로 이동될것이다.

		// 이 값들을 디폴트로하자.
		m_fEffectVolumeScale		= 0.8f;		
		m_fBGMVolumeScale			= 0.5f;	
		m_fAmbientEffectVolumeScale = 0.7f;

		m_fMasterVolumeScale		= 0.5f;
		m_fWaveVolumeScale			= 0.5f;
		m_fMicVolumeScale			= 0.5f;

		// EAX 사용 가능 여부
		if (wzsnd_env_supported())
		{
			this->m_bIsUseEAX = TRUE;
		}
		else
		{
			this->m_bIsUseEAX = FALSE;
		}

		///////////////
		//  cache sound buffers
		wzsnd_load_ui();
		wzsnd_load_gameplay();

		wzsnd_set_mic_output_mute(true);	//마이크 재생 mute (자신의 목소리 자신에게 안들리게 하는것: 녹음패널 mute가 아님!)
	}

    if (PROGRAMPARAM->is_sound_caching())
    {
        set_calltick_updatelistener(0);
        set_calltick_cache(5 * 60 * 1000);
        CacheOn(30 * 60 * 1000);
    }

	return TRUE;
}


//------------------------------------------------------------------------------
/**
*/
void Sound::Destroy( void )
{

	//	웹젠 사운드 라이브러리 해제

	if (m_bSoundFlag)
	{
		// uninitialize sound engine
		wzsnd_uninit();
	}
}

//------------------------------------------------------------------------------
/**
*/
void Sound::SetUseSoundLibrary(BOOL bFlag)
{
	this->m_bSoundFlag = bFlag;
}


//------------------------------------------------------------------------------
/**
*/
void Sound::SetUseEAXSound(BOOL bFlag)
{
	this->m_bIsUseEAX = bFlag;

	// TODO: 라이브러리상에 세팅하는 함수가 disable만 존재한다. Enable도 달라고한다.

	if (!wzsnd_env_supported())
	{
		return;
	}

	if (m_bIsUseEAX)
	{
		// 요청한다.
	}
	else
	{
		wzsnd_env_disable();
	}
}


//------------------------------------------------------------------------------
/**
*/
void Sound::SetUseEffectSound(BOOL bChoice)
{
	m_bIsUseEffectSound	= bChoice;
}


//------------------------------------------------------------------------------
/**
*/
void Sound::SetUseBGMSound(BOOL bChoice)
{
	m_bIsUseBGMSound = bChoice;
}


//------------------------------------------------------------------------------
/**
*/
void 
Sound::SetEnvironmentID( SND_ENV_ID SndID)
{
	if ( this->IsUseEAX() )
	{
		wzsnd_env_set_preset(SndID);
	}
}

//------------------------------------------------------------------------------
/**
*/
void			
Sound::DisableEnvironment()
{
	if ( this->IsUseEAX() )
	{
		wzsnd_env_disable();
	}
}

//------------------------------------------------------------------------------
/**
*/
void 
Sound::SetListenerPos(float x, float y, float z)
{
	// 오른손 좌표계되로 입력된것을 왼손좌표계로 변환하여 입력한다.
	this->m_vListenerPos.x = x;
	this->m_vListenerPos.y = z;
	this->m_vListenerPos.z = y;
}

//------------------------------------------------------------------------------
/**
*/
void 
Sound::SetListernerOrientation( float fDirX, float fDirY, float fDirZ, 
								float fUpX, float fUpY, float fUpZ )
{
	if (!IsUseEffectSound())
	{
		return;
	}

	this->m_vListenerDir.x = fDirX;
	this->m_vListenerDir.y = fDirZ;
	this->m_vListenerDir.z = fDirY;

	this->m_vListenerUp.x = fDirX;
	this->m_vListenerUp.y = fDirZ;
	this->m_vListenerUp.z = fDirY;
}


//------------------------------------------------------------------------------
/**
*/
D3DVECTOR * 
Sound::GetListenerPos()
{
	WzVector VecPos;
	if (g_pHero) {
		VecPos = g_pHero->GetVisiblePos();
	}
	else
	{
		VecPos.x = 0.0f;
		VecPos.y = 0.0f;
		VecPos.z = 0.0f;
	}

	m_vListenerPos.x = VecPos.x;
	m_vListenerPos.y = VecPos.z;
	m_vListenerPos.z = VecPos.y;

	return &m_vListenerPos;
}


//------------------------------------------------------------------------------
/**
*/
void 
Sound::GetListenerPos( WzVector * pWzPos )
{
	assert(pWzPos);
	
	WzVector wzPos;

	if (g_pHero) 
	{
		wzPos = g_pHero->GetVisiblePos();
	}
	else
	{
		wzPos.x = 0.0f;
		wzPos.y = 0.0f;
		wzPos.z = 0.0f;

	}

	if (pWzPos)
	{
		pWzPos->x = wzPos.x;
		pWzPos->y = wzPos.y;
		pWzPos->z = wzPos.z;
	}
	else
	{
		m_vListenerPos.x = wzPos.x;
		m_vListenerPos.y = wzPos.z;
		m_vListenerPos.z = wzPos.y;
	}
}

//------------------------------------------------------------------------------
/**
*/
D3DVECTOR * 
Sound::GetListenerDir(void)
{
	if (!g_pSunRenderer)
	{
		return NULL;
	}

	St_Camera * pCameraInfo = g_pSunRenderer->GetCameraInfo();

	m_vListenerDir.x = pCameraInfo->m_wvDir.x;
	m_vListenerDir.y = pCameraInfo->m_wvDir.z;
	m_vListenerDir.z = pCameraInfo->m_wvDir.y;
	
	return &m_vListenerDir;
}


//------------------------------------------------------------------------------
/**
*/
D3DVECTOR * 
Sound::GetListenerUp(void)
{
	if (!g_pSunRenderer)
	{
		return NULL;
	}

	St_Camera * pCameraInfo = g_pSunRenderer->GetCameraInfo();
	assert(pCameraInfo && "CameraInfo Is NULL!!");

	m_vListenerUp.x = pCameraInfo->m_wvUp.x;
	m_vListenerUp.y = pCameraInfo->m_wvUp.z;
	m_vListenerUp.z = pCameraInfo->m_wvUp.y;

	return &m_vListenerUp;
}


//----------------------------------------------------------------------------
/** 현재 플레이 중인 모든 게임사운드를 중지한다.
*/
void			
Sound::StopEffectSoundAll()
{
	if ( !IsUseEffectSound() )
	{	
		return;
	}

	wzsnd_stop_all();
}

void Sound::UnloadSounds()
{
	wzsnd_unload_gameplay();
	wzsnd_unload_ui();
	wzsnd_stop_all();
}


//------------------------------------------------------------------------------
void Sound::Update()
{
    if (NULL == g_pHero)
    {
        return;
    }

    //------------------------------------------------------------------------------
    //! 비어 있는 채널 정리
    //{
    //    wzsnd_check_channel();
    //}

    //! listener 갱신
    {
        static DWORD sound_update_listener_time = timeGetTime();
        static DWORD sound_current_listener_time = timeGetTime();
        sound_current_listener_time = timeGetTime();

        if (sound_current_listener_time > (sound_update_listener_time + calltick_updatelistener()))
        {
            wzsnd_update_listener(GetListenerPos(), GetListenerDir(), GetListenerUp(), NULL);
            sound_update_listener_time = sound_current_listener_time;
        }
    }

    //! 캐시 갱신
    if (wzsnd_cache_is())
    {
        static DWORD sound_updatecache_previous_time = timeGetTime();
        static DWORD sound_current_time = timeGetTime();
        sound_current_time = timeGetTime();

        if (sound_current_time > (sound_updatecache_previous_time + calltick_cache()))
        {
            wzsnd_update_cache();
            sound_updatecache_previous_time = sound_current_time;
        }
    }
}

//------------------------------------------------------------------------------
void Sound::CacheOn(DWORD maintain_milliseconds)
{
    wzsnd_cache_on();
    wzsnd_cache_maintain_milliseconds_set(maintain_milliseconds);
}
//------------------------------------------------------------------------------
void Sound::CacheOff()
{
    wzsnd_cache_off();
}


//------------------------------------------------------------------------------
void Sound::SetMasterVolume(float fVolume, BOOL bSave)
{	
	if(bSave)
		m_fMasterVolumeScale = fVolume;
	
	
	int iVol = (int)(fVolume * 100);
	wzsnd_set_master_vol(iVol, iVol);
}

//------------------------------------------------------------------------------
void Sound::SetWaveVolume(float fVolume, BOOL bSave)
{
	if(bSave)
		m_fWaveVolumeScale = fVolume;

	int iVol = (int)(fVolume * 100);
	wzsnd_set_wav_vol(iVol, iVol);
}

//------------------------------------------------------------------------------
int Sound::GetMasterVolume()
{
	int iVolume;
	wzsnd_get_master_vol(&iVolume,&iVolume);
	return iVolume;
}

//------------------------------------------------------------------------------
int Sound::GetWaveVolume()
{
	int iVolume;
	wzsnd_get_wav_vol(&iVolume,&iVolume);
	return iVolume;
}
