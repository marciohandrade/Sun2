#include "SunClientPrecompiledHeader.h"
#include ".\soundfactory.h"
#include "SoundEffect.h"
#include "SoundBGM.h"

SoundFactory::SoundFactory(void)
{
}

SoundFactory::~SoundFactory(void)
{
}


SoundEffect * SoundFactory::MakeSoundEffect(SOUND_EFFECT_TYPE eType)
{
	switch(eType) 
	{
		case Sound_Effect_Type_BGM:
			{
				return new SoundBGM;
			}
			break;
		case Sound_Effect_Type_WAV:
			{
				return new SoundEffect;
			}
			break;
		case Sound_Effect_Type_SEG:
			{
				; //
			}
			break;
		case Sound_Effect_Type_None:
	default:
		ASSERT(!"Invalid Sound Type Error!!");
	}

	return NULL;
}