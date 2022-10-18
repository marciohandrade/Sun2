//------------------------------------------------------------------------------
//	SoundBGM
//	(C) 2005 y2jinc
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "SoundBGM.h"

SoundBGM	g_SoundBGM;

//------------------------------------------------------------------------------
/**
*/
void SoundBGM::Play(SOUND_INDEX Index, BOOL bLoop /* = TRUE*/, BOOL bFadeIn /*= TRUE*/)
{
	if (!Sound::Instance()->IsUseBGMSound())
	{
		return;
	}

	Stop();

	m_SoundEffectInfo.m_hSound = wzsnd_play_bgm(Index, bLoop, Sound::Instance()->GetBGMVolume(), bFadeIn);
}


void SoundBGM::Seek(int iPos /*= 0*/)
{
	if (!Sound::Instance()->IsUseBGMSound())
	{
		return;
	}

	// TODO :
	// NOT YET IMP
}


void SoundBGM::Pause()
{	
	if (!Sound::Instance()->IsUseBGMSound())
	{
		return;
	}

	wzsnd_pause_bgm(m_SoundEffectInfo.m_hSound);
}

void SoundBGM::Resume()
{
	// NOT YET IMP
}

void SoundBGM::Stop(BOOL bAll)
{
	if (!Sound::Instance()->IsUseBGMSound())
	{
		return;
	}

	if (bAll)
	{
		wzsnd_stop_all_bgm(TRUE);	
	}
	else
	{
		wzsnd_stop_bgm(m_SoundEffectInfo.m_hSound, TRUE);
	}
}

void SoundBGM::SetVolume(float fVolume)
{
	if (!Sound::Instance()->IsUseBGMSound())
	{
		return;
	}

	wzsnd_set_volume_all_bgm(fVolume);

}