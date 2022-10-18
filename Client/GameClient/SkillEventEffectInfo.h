#pragma once

#include "Camera.h"

class cSkillPositionData
{
public:
    cSkillPositionData( eSKILLEFFECT_POS Position, DWORD BoneIndex )
    {
        m_EffectPositionData = Position;
        m_BoneIndex = BoneIndex;
    };

    eSKILLEFFECT_POS GetPositionType() const { return m_EffectPositionData; }
    DWORD GetBoneIndex() const { return m_BoneIndex; }

private:
    eSKILLEFFECT_POS m_EffectPositionData;
    DWORD m_BoneIndex;
};


class cSkillEventEffectInfoData
{
public:
    enum eEventType
    {
        EVENT_TYPE_START,
        EVENT_TYPE_END,
        EVENT_TYPE_BUFF,
        EVENT_TYPE_DAMAGE,
        EVENT_TYPE_ANIMATION_EVENT,
        EVENT_TYPE_FIELD,
        EVENT_TYPE_MAX,
    };

    enum eActionType
    {
        ACTION_TYPE_CREATEEFFECT,
        ACTION_TYPE_SHAKE_CAMERA,
        ACTION_TYPE_MAX,
    };

    enum ePositionType
    {
        POSITION_TYPE_USER_POSITION,          // 사용자의 위치
        POSITION_TYPE_USER_ARROW_POSITION,    // 대상의 위치 (몸통)
        POSITION_TYPE_TARGET_POSITION,        // 대상의 위치
        POSITION_TYPE_TARGET_ARROW,           // 대상의 위치 (몸통)
        POSITION_TYPE_CUSTOM,
        POSITION_TYPE_MAX,
    };

    enum eScaleType
    {
        SCALE_TYPE_NONE,
        SCALE_TYPE_USER,
        SCALE_TYPE_TARGET,
        SCALE_TYPE_MAX,
    };

private:

    eEventType m_EventType;
    eActionType m_ActionType;
    ePositionType m_PositionType;
    eScaleType m_ScaleType;
    DWORD m_HitEventNumber;
    WzID m_EventCode;

    BOOL m_ChaseTarget;
    DWORD m_LifeTime;
    cSkillPositionData m_PositionData;
    BOOL m_StaticRotate;

    Camera::VIBE_TYPE m_CameraViberationType;
    float m_CameraViberationPower;

    DWORD m_ActiveDelayTime;

public:

    cSkillEventEffectInfoData();


    DWORD GetDelayTime() const { return m_ActiveDelayTime; }
    void SetDelayTime(DWORD value) { m_ActiveDelayTime = value; }


    DWORD GetLifeTime() const { return m_LifeTime; }
    void SetLifeTime(DWORD value) { m_LifeTime = value; }

    BOOL IsChaseTarget() const { return m_ChaseTarget; }
    void SetChaseTarget(BOOL value) { m_ChaseTarget = value; }

    BOOL IsStaticRotate() const { return m_StaticRotate; }
    void SetStaticRotate(BOOL value) { m_StaticRotate = value; }


    cSkillPositionData& GetPositionData() { return m_PositionData; }
    const cSkillPositionData& GetPositionData() const { return m_PositionData; }
    void SetPositionData(const cSkillPositionData& value) { m_PositionData = value; }

    eEventType GetEventType() const { return m_EventType; }
    void SetEventType(eEventType value) { m_EventType = value; }

    eActionType GetActionType() const { return m_ActionType; }
    void SetActionType(eActionType value) { m_ActionType = value; }

    ePositionType GetPositionType() const { return m_PositionType; }
    void SetPositionType(ePositionType value) { m_PositionType = value; }

    eScaleType GetScaleType() const { return m_ScaleType; }
    void SetScaleType(eScaleType value) { m_ScaleType = value; }

    DWORD GetHitEventNumber() const { return m_HitEventNumber; }
    void SetHitEventNumber(DWORD value) { m_HitEventNumber = value; }

    WzID GetEventCode() const { return m_EventCode; }
    void SetEventCode(WzID value) { m_EventCode = value; }


    Camera::VIBE_TYPE GetCameraViberationType() const { return m_CameraViberationType; }
    void SetCameraViberationType(Camera::VIBE_TYPE value) { m_CameraViberationType = value; }

    float GetCameraViberationPower() const { return m_CameraViberationPower; }
    void SetCameraViberationPower(float value) { m_CameraViberationPower = value; }

    float GetCameraViberationTime() const { return (float)(m_LifeTime); }

    BOOL HasCameraViberation() const { return m_CameraViberationType != Camera::VIBE_TYPE_NONE && m_CameraViberationPower > 0.0f; }
};

class cSkillEventEffectInfo
{
private:
    DWORD m_SkillClassCode;

    std::vector<cSkillEventEffectInfoData*> m_DataArray;

public:
    cSkillEventEffectInfo();
	~cSkillEventEffectInfo();

    size_t GetEventDataCount() { return m_DataArray.size(); }

    const cSkillEventEffectInfoData* GetEventData( size_t index ) { return m_DataArray.at(index); }

    DWORD GetSkillClassCode() const { return m_SkillClassCode; }
    void SetSkillClassCode(DWORD value) { m_SkillClassCode = value; }

    cSkillEventEffectInfoData* AddData( const std::string& EventType, const std::string& PositionType, const std::string& ScaleType, LPCTSTR EventCode );
};