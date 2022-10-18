#ifndef SOUNDFACTORY_H
#define SOUNDFACTORY_H

#pragma once

#include "SoundEffect.h"	// for SOUND_EFFECT_TYPE

class SoundEffect;
class SoundBGM;



class SoundFactory
{
public:
	SoundFactory(void);
	virtual ~SoundFactory(void);

	SoundEffect * MakeSoundEffect(SOUND_EFFECT_TYPE eType);
};

#endif
// SOUNDFACTORY_H