///////////////////////////////////////////////////////////////////////////////////////////////////////
// 애니메이션에서 발생하는 이벤트(WzEvent) 
// 게임내에서 사용하는 이벤트가 아님
#pragma once


#include <client/3dengine/3dfileaccess/wzunit/wzevent.h>


///////////////////////////////////////////////////////////////////////////////////////////////////////
class SoundEventInfo
{
private:
	WzSoundEventInfo m_SoundEvent;

public:
	SoundEventInfo( const WzSoundEventInfo& SoundEvent )
		: m_SoundEvent( SoundEvent ) 
	{
	}

	int GetSoundIndex() const { return m_SoundEvent.m_iSound; }
	int GetBlendAniIndex() const { return m_SoundEvent.m_bBlendAni; }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
class EffectEventInfo
{
private:
	WzEffectEventInfo m_EffectEvent;

public:
	EffectEventInfo( const WzEffectEventInfo& EffectEvent );

	bool IsDestroy() const;
	bool IsDestroyAll() const;

	bool IsTrail_LeftToRight() const;
	bool IsTrail_RightToLeft() const;
	int GetTrailIndex() const;

	int GetEventIndex() const;
	WzID GetEffectID() const;
	int GetBoneIndex() const;
	const WzVector& GetEffectPosition() const;
	const WzVector& GetEffectScale() const;
	const WzQuaternion& GetEffectQuaternion() const;
	bool IsLocationOnly() const;
	bool IsRotate() const;
	bool IsForceTransform() const;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
class CustomEventInfo
{
private:
	WzCustomEventInfo m_CustomEvent;

public:
	CustomEventInfo( const WzCustomEventInfo& CustomEvent );

	bool IsFootStep() const;
	bool IsWaterWave() const;
	bool IsCameraWave() const;

	float GetCameraWaveDuration() const;
	float GetCameraWavePower() const;

};

///////////////////////////////////////////////////////////////////////////////////////////////////////
class EventInfo
{
private:
	WzEvent m_Event;

public:
	EventInfo( const WzEvent& Event );

	size_t GetSoundEventCount() const;
	const WzSoundEventInfo& GetSoundEvent( size_t Index ) const;

	size_t GetEffectEventCount() const;
	const WzEffectEventInfo& GetEffectEvent( size_t Index ) const;

	size_t GetCustomEventCount() const;
	const WzCustomEventInfo& GetCustomEvent( size_t Index ) const;

	bool IsGood() const;

};
