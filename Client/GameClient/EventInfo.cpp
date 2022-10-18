#include "SunClientPrecompiledHeader.h"
#include "EventInfo.h"
#include "ObjectEffectData.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////
const int TRAIL_LEFT = 100;
const int TRAIL_RIGHT = 101;
const int DESTROY = 102;
const int FORCE_TRANSFROM = -2;     // 본인덱스가 -2라면 기본위치에 이펙트를 생성 후 이펙트의 offset 만큼 이동
const int DEFAULT_LOCATION = -1;    // 본인덱스가 -1이라면 기본위치에 이펙트를 생성


EffectEventInfo::EffectEventInfo( const WzEffectEventInfo& EffectEvent )
: m_EffectEvent(EffectEvent)
{
}

bool EffectEventInfo::IsDestroy() const
{
	return m_EffectEvent.m_byEffectFlag & EFF_DESTROY;
}

bool EffectEventInfo::IsDestroyAll() const
{
	return m_EffectEvent.m_byEffectFlag & EFF_DESTROY && m_EffectEvent.m_iEffectType == DESTROY;
}

bool EffectEventInfo::IsTrail_LeftToRight() const
{
	return m_EffectEvent.m_iEffectType == TRAIL_RIGHT;
}

bool EffectEventInfo::IsTrail_RightToLeft() const
{
	return m_EffectEvent.m_iEffectType == TRAIL_LEFT;
}

int EffectEventInfo::GetTrailIndex() const
{
	return m_EffectEvent.m_iEffectType;
}

int EffectEventInfo::GetEventIndex() const
{
	return min( max(m_EffectEvent.m_iEffectType, 0 ), MAX_OBJECT_EFFECT -1 );
}

bool EffectEventInfo::IsLocationOnly() const
{
	return m_EffectEvent.m_iEffectType == 200;
}

bool EffectEventInfo::IsRotate() const
{
	if( m_EffectEvent.m_byEffectFlag & EFF_USEROTATE )
		return true;
	return false;
}

bool EffectEventInfo::IsForceTransform() const
{
	return m_EffectEvent.m_iEffectBoneIndex == FORCE_TRANSFROM;
}

WzID EffectEventInfo::GetEffectID() const
{
	return m_EffectEvent.m_wiEffectWzID;
}

int EffectEventInfo::GetBoneIndex() const
{
	return m_EffectEvent.m_iEffectBoneIndex;
}

const WzVector& EffectEventInfo::GetEffectPosition() const
{
	return m_EffectEvent.m_wvEffectPos;
}

const WzVector& EffectEventInfo::GetEffectScale() const
{
	return m_EffectEvent.m_wvEffectScale;
}

const WzQuaternion& EffectEventInfo::GetEffectQuaternion() const
{
	return m_EffectEvent.m_wqEffectQuat;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
const int FOOT_STEP = 2;
const int WATER_WAVE = 3;
const int CAMERA_WAVE = 500;

CustomEventInfo::CustomEventInfo( const WzCustomEventInfo& CustomEvent )
: m_CustomEvent(CustomEvent)
{
}

bool CustomEventInfo::IsFootStep() const
{
	return m_CustomEvent.m_iaParam3[0] == FOOT_STEP;
}

bool CustomEventInfo::IsWaterWave() const
{
	return m_CustomEvent.m_iaParam3[0] == WATER_WAVE;
}

bool CustomEventInfo::IsCameraWave() const
{
	return m_CustomEvent.m_iaParam3[0] == CAMERA_WAVE;
}

float CustomEventInfo::GetCameraWaveDuration() const
{
	return static_cast<float>(m_CustomEvent.m_iaParam3[2]);
}

float CustomEventInfo::GetCameraWavePower() const
{
	float wave_power = max(m_CustomEvent.m_iaParam3[1], 5.0f);
	wave_power /= 1000.0f;

	return wave_power;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////

EventInfo::EventInfo( const WzEvent& Event )
: m_Event(Event)
{

}

size_t EventInfo::GetSoundEventCount() const
{
	return min( (size_t)(m_Event.m_iNumSound), MAX_EVENT_ELEMENT_SND );
}

const WzSoundEventInfo& EventInfo::GetSoundEvent( size_t Index ) const
{
	return m_Event.m_Sound[Index];
}

size_t EventInfo::GetEffectEventCount() const
{
	return min( (size_t)(m_Event.m_iNumEffect), MAX_EVENT_ELEMENT );
}

const WzEffectEventInfo& EventInfo::GetEffectEvent( size_t Index ) const
{
	return m_Event.m_Effect[Index];
}

size_t EventInfo::GetCustomEventCount() const
{
	return min( (size_t)(m_Event.m_iNumCustom), MAX_EVENT_ELEMENT );
}

const WzCustomEventInfo& EventInfo::GetCustomEvent( size_t Index ) const
{
	return m_Event.m_Custom[Index];
}

bool EventInfo::IsGood() const
{
	if( m_Event.m_iNumSound < 0 || m_Event.m_iNumSound >= MAX_EVENT_ELEMENT_SND )
		return false;

	if( m_Event.m_iNumEffect < 0 || m_Event.m_iNumEffect >= MAX_EVENT_ELEMENT )
		return false;

	if( m_Event.m_iNumCustom < 0 || m_Event.m_iNumCustom >= MAX_EVENT_ELEMENT )
		return false;

	return true;
}
