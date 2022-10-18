//------------------------------------------------------------------------------
//	
//	(C) 2005 y2jinc
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "SoundEffect.h"


SoundEffect			g_SoundEffect;


//----------------------------------------------------------------------------
/**
*/
void 
SoundEffect::Clear()
{

}

// play 2d buffer
//
//	result :	sound handle needed to stop, modify this channel
//
SOUND_INDEX
	SoundEffect::Play(
		SOUND_INDEX Index, 
		float volume_scale	/*= 0.f*/,
		BOOL bLoop			/*= FALSE*/, 
		int iPriority		/*= SND_PRIORITY_SINGLE_INSTANCE*/, 
		int iFrequency		/*= 0*/
		)
{
	if (!Sound::Instance()->IsUseEffectSound())
	{
		return -1;
	}

	m_SoundEffectInfo.m_hSound	= Index;
	m_SoundEffectInfo.m_bLoop	= bLoop;
	m_SoundEffectInfo.m_iPriority = iPriority;
	m_SoundEffectInfo.m_fVolume = volume_scale;
	m_SoundEffectInfo.m_iFrequency = iFrequency;

	m_SoundEffectInfo.m_fVolume = 
		(0.f == volume_scale)? Sound::Instance()->GetEffectVolume(): volume_scale;

	m_SoundEffectInfo.m_hSound =
		wzsnd_play(
			Index, 
			m_SoundEffectInfo.m_bLoop, 
			m_SoundEffectInfo.m_iPriority, 
			m_SoundEffectInfo.m_fVolume, 
			m_SoundEffectInfo.m_iFrequency
			);

	if (m_SoundEffectInfo.m_hSound < 0)
	{
		m_SoundEffectInfo.m_eState = Sound_State_NONE;
	}
	else
	{
		m_SoundEffectInfo.m_eState = Sound_State_Play;
	}

	return m_SoundEffectInfo.m_hSound;
}


// 3D Sound 관련한 내용
//
/*	int wzsnd_play_3d(int n, 
		float *source_pos, 
		float *listen_pos,		// 이 리스너 pos는 없어도 될듯한디..
		float radius=0.0f, 
		float *velocity = NULL, 
		int looping = 0, 
		int priority = SND_PRIORITY_SINGLE_INSTANCE, 
		float volume_scale=1.0f,
		int frequency = 0, 
		int force = 0);

int wzsnd_play_3d(int sndnum, 
		  float xPos,		// 오브젝트 현재 위치 x
		  float yPos, // 오브젝트 현재 위치 y
		  float zPos,// 오브젝트 현재 위치 z
		  float xNextPos, // 다음 프레임에서의 오브젝트 위치 x
		  float yNextPos, // 다음 프레임에서의 오브젝트 위치 y
		  float zNextPos, // 다음 프레임에서의 오브젝트 위치 z

		  float xListenerPos,	// 리스너 현재 위치 x
		  float yListenerPos,	// 리스너 현재 위치 y
		  float zListenerPos,	// 리스너 현재 위치 z	  
		  
		  float radius=0.0f,	// 오브젝트의 반경
		  int looping = 0,	// 루프를 돌릴 지 여부
		  int priority = SND_PRIORITY_SINGLE_INSTANCE, // 사운드 타격 옵션. (동시에 같은 음을 몇번까지 허용할지)
		  float volume_scale=1.0f,	// 사운드 스크립트에 표기한 디폴트 볼륨에서   음량을 키우고 싶을때,  
									// 최종 볼륨 =   디폴트 볼륨 X volume_scale
		  int frequency = 0,	// frequency 조절
		  int force = 0);	 // 오브젝트가 리서너에서 너무 멀거나, 볼륨스케일을 너무 작게해서 음이 안들릴 경우에도, 
							// 강제로 음을 내고 싶을때 1로 세팅한다.
*/
SOUND_INDEX 
	SoundEffect::Play(
	 SOUND_INDEX Index,
	 WzVector *	pPos,
	 BOOL		bLoop		/*= FALSE*/,
	 WzVector *	pVelocity	/*= NULL*/,
	 int		iPriority	/*= SND_PRIORITY_SINGLE_INSTANCE*/,
	 float		fVolume		/*= 0.0f*/,
	 float		fRadius		/*= 0.f*/,
	 int		iFrequency	/*= 0*/,
	 int		force		/*= 1*/		
	 )	 
{
	if (!Sound::Instance()->IsUseEffectSound())
	{
		return -1;
	}

	m_SoundEffectInfo.m_hSound	= Index;
	m_SoundEffectInfo.m_bLoop	= bLoop;
	m_SoundEffectInfo.m_iPriority = iPriority;
	m_SoundEffectInfo.m_fVolume = fVolume;
	m_SoundEffectInfo.m_iFrequency = iFrequency;
	m_SoundEffectInfo.m_fRadius = fRadius;
	
	if ( pPos )
	{
		m_SoundEffectInfo.m_vPos.x = pPos->x;
		m_SoundEffectInfo.m_vPos.y = pPos->z;
		m_SoundEffectInfo.m_vPos.z = pPos->y;
	}

	if ( pVelocity )
	{
		m_SoundEffectInfo.m_vVelocity.x = pVelocity->x;
		m_SoundEffectInfo.m_vVelocity.y = pVelocity->z;
		m_SoundEffectInfo.m_vVelocity.z = pVelocity->y;
	}

	m_SoundEffectInfo.m_vVelocity.x = 0.0f;
	m_SoundEffectInfo.m_vVelocity.y = 0.0f;
	m_SoundEffectInfo.m_vVelocity.z = 0.0f;

	m_SoundEffectInfo.m_fVolume = 
		(0.f == fVolume)? Sound::Instance()->GetEffectVolume(): fVolume;

	D3DVECTOR * pVecListener = Sound::Instance()->GetListenerPos();
	assert(pVecListener);


	SND_SOURCEDESC SourceDesc;
	SourceDesc.bForcePlay		= force;
	SourceDesc.flFrequency		= (float)m_SoundEffectInfo.m_iFrequency;
	SourceDesc.flMinDistance	= -1;
	SourceDesc.flMaxDistance	= -1;
	SourceDesc.flObjectRadius	= m_SoundEffectInfo.m_fRadius;
	SourceDesc.flVolumeScale	= m_SoundEffectInfo.m_fVolume;
	SourceDesc.flRangeFactor	= 1.0f;

	m_SoundEffectInfo.m_hSound = 
		wzsnd_play_3d(
			Index, 
			pVecListener,					// listener pos
			&m_SoundEffectInfo.m_vPos,		// source pos
			&m_SoundEffectInfo.m_vVelocity,		// vel pos
			m_SoundEffectInfo.m_bLoop,
			-1,
			&SourceDesc
			);

	if (m_SoundEffectInfo.m_hSound < 0)
	{
		m_SoundEffectInfo.m_eState = Sound_State_NONE;
	}
	else
	{
		m_SoundEffectInfo.m_eState = Sound_State_Play;
	}
	

	return m_SoundEffectInfo.m_hSound;
}


//----------------------------------------------------------------------------
/**
*/
SOUND_INDEX 
SoundEffect::PlayUI(SOUND_INDEX Index )
{
    if (!Sound::Instance()->IsUseEffectSound())
    {
        return -1;
    }

#if WZENGINEVER >= 310
    return wzsnd_play_ui(Index, Sound::Instance()->GetEffectVolume());

#else
	// NEED TO : 이놈도 리턴을 했으면한다. 
	wzsnd_play_ui(Index, Sound::Instance()->GetEffectVolume());

	return 0;
#endif
}


//----------------------------------------------------------------------------
/**
*/
SOUND_INDEX 
SoundEffect::PlayVoice(SOUND_INDEX Index ,WzVector *vPos)
{
    if (!Sound::Instance()->IsUseEffectSound())
    {
        return -1;
    }

	if (!Index)
	{
		return -1;
	}

	
	int iHandle = wzsnd_play_voice(Index,FALSE,Sound::Instance()->GetEffectVolume());
	
	return iHandle;
}

//----------------------------------------------------------------------------
/**
*/
SOUND_INDEX
SoundEffect::PlayAmbient(SOUND_INDEX Index, BOOL bLoop /*= TRUE*/)
{
	if (!Index)
	{
		return -1;
	}

	int iHandle = wzsnd_play_ambi(Index, SND_INSTANCE_1, bLoop,Sound::Instance()->GetAmbientEffectVolume());

	wzsnd_set_loop_ambi(iHandle, bLoop);	

	return iHandle;
}

//----------------------------------------------------------------------------
/**
*/
void 
SoundEffect::StopVoice(int iHandle)
{    
	if (iHandle != -1) 
    {	
		wzsnd_stop_voice(iHandle);


		int iResult = wzsnd_getState_voice(iHandle);
		
		if(iResult == 1) //wzsnd_stop_voice 호출 후에도 PLAYING 상태라면
		{
			wzsnd_stop_all_voice(FALSE); //이럴 땐, fadeout 없이 바로 종료 시켜야 종료됨..(이렇게 안하면 NPC보이스 무한반복 현상 발생)
		}		

	}	
}

//----------------------------------------------------------------------------
/**
*/
void 
SoundEffect::StopAmbient(int iHandle)
{
	if (iHandle != -1) {	
		wzsnd_stop_ambi(iHandle);
	}
}


//----------------------------------------------------------------------------
/**
*/
void 
SoundEffect::Stop(BOOL bAll /*= TRUE*/)
{
// NOT YET IMPLEMENT
}

void
SoundEffect::StopUI( SOUND_INDEX handle )
{
#if WZENGINEVER >= 310
    wzsnd_stop_ui( handle );
#endif
}


//----------------------------------------------------------------------------
/**
*/
void SoundEffect::Pause()
{
// NOT YET IMPLEMENT
}