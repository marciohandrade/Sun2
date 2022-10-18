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


// 3D Sound ������ ����
//
/*	int wzsnd_play_3d(int n, 
		float *source_pos, 
		float *listen_pos,		// �� ������ pos�� ��� �ɵ��ѵ�..
		float radius=0.0f, 
		float *velocity = NULL, 
		int looping = 0, 
		int priority = SND_PRIORITY_SINGLE_INSTANCE, 
		float volume_scale=1.0f,
		int frequency = 0, 
		int force = 0);

int wzsnd_play_3d(int sndnum, 
		  float xPos,		// ������Ʈ ���� ��ġ x
		  float yPos, // ������Ʈ ���� ��ġ y
		  float zPos,// ������Ʈ ���� ��ġ z
		  float xNextPos, // ���� �����ӿ����� ������Ʈ ��ġ x
		  float yNextPos, // ���� �����ӿ����� ������Ʈ ��ġ y
		  float zNextPos, // ���� �����ӿ����� ������Ʈ ��ġ z

		  float xListenerPos,	// ������ ���� ��ġ x
		  float yListenerPos,	// ������ ���� ��ġ y
		  float zListenerPos,	// ������ ���� ��ġ z	  
		  
		  float radius=0.0f,	// ������Ʈ�� �ݰ�
		  int looping = 0,	// ������ ���� �� ����
		  int priority = SND_PRIORITY_SINGLE_INSTANCE, // ���� Ÿ�� �ɼ�. (���ÿ� ���� ���� ������� �������)
		  float volume_scale=1.0f,	// ���� ��ũ��Ʈ�� ǥ���� ����Ʈ ��������   ������ Ű��� ������,  
									// ���� ���� =   ����Ʈ ���� X volume_scale
		  int frequency = 0,	// frequency ����
		  int force = 0);	 // ������Ʈ�� �����ʿ��� �ʹ� �ְų�, ������������ �ʹ� �۰��ؼ� ���� �ȵ鸱 ��쿡��, 
							// ������ ���� ���� ������ 1�� �����Ѵ�.
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
	// NEED TO : �̳� ������ �������Ѵ�. 
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
		
		if(iResult == 1) //wzsnd_stop_voice ȣ�� �Ŀ��� PLAYING ���¶��
		{
			wzsnd_stop_all_voice(FALSE); //�̷� ��, fadeout ���� �ٷ� ���� ���Ѿ� �����..(�̷��� ���ϸ� NPC���̽� ���ѹݺ� ���� �߻�)
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