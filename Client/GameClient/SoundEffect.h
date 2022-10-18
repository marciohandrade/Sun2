
//------------------------------------------------------------------------------
/**

    게임 이펙트 사운드를 플레이하고 관리하는 class 

    @author
		유재영 (y2jinc@webzen.co.kr)
    @since

    @remarks
*/

#ifndef SOUNDEFFECT_H
#define SOUNDEFFECT_H

#include "Sound.h"

enum SOUND_STATE
{
	Sound_State_NONE = 0,
	Sound_State_Play,
	Sound_State_Stopped,
};

typedef struct SoundEffectInfo
{	
	SoundEffectInfo() {}

	~SoundEffectInfo() {}

	SOUND_STATE				m_eState;		// 사운드 핸들의 상태
	SOUND_INDEX				m_hSound;		// 사운드 핸들 번호
	BOOL					m_bLoop;		// TRUE / FALSE
	int						m_iPriority;	//
	int						m_iFrequency;
	float					m_fVolume;		// 사운드당 볼륨 0이면 Sound 에 셋되어있는 볼륨을 적용한다. 
	float					m_fRadius;		 
	D3DVECTOR				m_vPos;
	D3DVECTOR				m_vVelocity;

} SOUND_EFFECT_INFO;


class SoundEffect
{
public:
	SoundEffect() {}
	SoundEffect(SOUND_EFFECT_INFO & rSoundEffectData) {}
	~SoundEffect() {}

	void Clear();

	virtual SOUND_INDEX 	
		Play(
			SOUND_INDEX Index, 
			float volume_scale=0.f,
			BOOL bLoop		= FALSE, 
			int iPriority	= SND_PRIORITY_DOUBLE_INSTANCE, 
			int frequency	= 0
			);

	virtual SOUND_INDEX 
		Play(
			SOUND_INDEX Index,
			WzVector *	pPos,
			BOOL		bLoop		= FALSE,
			WzVector *	pVelocity	= NULL,
			int			iPriority	= SND_PRIORITY_DOUBLE_INSTANCE,
			float		fVolume		= 0.f,
			float		fRadius		= 0.f,
			int			iFrequency	= 0,
			int			force		= 0
			);

	// NEED TO : wzsnd_play_ui 함수도 볼륨조절이 필요하다. 

	SOUND_INDEX 
		PlayUI(SOUND_INDEX Index );

	SOUND_INDEX 
		PlayVoice(SOUND_INDEX Index ,WzVector *	pPos);

	SOUND_INDEX
		PlayAmbient(SOUND_INDEX Index, BOOL bLoop = TRUE);

    void                    StopUI( SOUND_INDEX handle );
	virtual void			Stop(BOOL bAll = TRUE);
	void					StopVoice(int iHandle);
	void					StopAmbient(int iHandle);


	virtual void			Pause();

protected:
	SOUND_EFFECT_INFO		m_SoundEffectInfo;
};

extern SoundEffect			g_SoundEffect;


#endif // SOUNDEFFECT_H