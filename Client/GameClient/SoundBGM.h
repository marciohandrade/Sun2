
//------------------------------------------------------------------------------
/**

    게임 음악을 플레이하고 관리하는 class 

    @author
		유재영 (y2jinc@webzen.co.kr)
    @since

    @remarks
*/

#ifndef _SOUNDBGM_H_
#define _SOUNDBGM_H_

#include "SoundEffect.h"

class SoundBGM : public SoundEffect
{
public:
	SoundBGM() {}
	~SoundBGM()	{}

	void		Play(SOUND_INDEX Index, BOOL bLoop = TRUE, BOOL bFadeIn = TRUE);
	void		Seek(int iPos = 0);
	void		Resume();

	void		Pause();
	void		Stop(BOOL bAll = TRUE);
	void		SetVolume(float fVolume);

private:
	SOUND_EFFECT_INFO	m_NextSoundEffect;	// 다음 연주될 음악	// 크로싱할때만 사용
	FILE * m_pfp;
};

extern SoundBGM	g_SoundBGM;

#endif
// _SOUNDBGM_H_