#pragma once

class Skill;
class BaseStatus;
class Ability;


//==================================================================================================
//  <AnimationDelayControl>
//
class AnimationDelayControl
{
public:
    struct sDelayInfo
    {
        PVOID    RelatedObject;
        ITimeout Timeout;
    };
    typedef STLX_DEQUE<sDelayInfo> ANIMATION_DELAY_QUEUE;

public:
    AnimationDelayControl() : m_pQueue(NULL) {}
    ~AnimationDelayControl() { Clear(); }

    inline VOID Clear();
    //////////////////////////////////////////////////////////////////////////
    //  TRUE : Processing Client Animation
    //  FALSE : C/S 동기화를 위한 예외처리 할 필요 없음.
    inline BOOLEAN NeedSkipProcessByAnimationDelay(Skill* const pSkill);
    inline BOOLEAN NeedSkipProcessByAnimationDelay(BaseStatus* const pStatus);
    inline BOOLEAN NeedSkipProcessByAnimationDelay(Ability* const pAbility);
    inline BOOLEAN SetAnimationDelay(Skill* const pSkill, const DWORD delayTick);
    inline BOOLEAN SetAnimationDelay(BaseStatus* const pStatus, const DWORD delayTick);
    inline BOOLEAN SetAnimationDelay(Ability* const pAbility, const DWORD delayTick);

private:
    BOOLEAN NeedSkipProcessByAnimationDelay(const PVOID pObject = NULL);
    BOOLEAN SetAnimationDelay(const PVOID pObject, const DWORD delayTick);

    //  (FIELDs)
    ANIMATION_DELAY_QUEUE*        m_pQueue;
};

//
//
//==================================================================================================
//
//

inline VOID
AnimationDelayControl::Clear()
{
    if (m_pQueue) {
        TAllocDelete(ANIMATION_DELAY_QUEUE, m_pQueue);
    };
    m_pQueue = NULL;
}

//////////////////////////////////////////////////////////////////////////
//  TRUE : Processing Client Animation
//  FALSE : C/S 동기화를 위한 예외처리 할 필요 없음.
inline BOOLEAN
AnimationDelayControl::NeedSkipProcessByAnimationDelay(Skill* const pSkill)
{
    return NeedSkipProcessByAnimationDelay(reinterpret_cast<PVOID>(pSkill));
}

inline BOOLEAN
AnimationDelayControl::NeedSkipProcessByAnimationDelay(BaseStatus* const pStatus)
{
    return NeedSkipProcessByAnimationDelay(reinterpret_cast<PVOID>(pStatus));
}

inline BOOLEAN
AnimationDelayControl::NeedSkipProcessByAnimationDelay(Ability* const pAbility)
{
    return NeedSkipProcessByAnimationDelay(reinterpret_cast<PVOID>(pAbility));
}

inline BOOLEAN
AnimationDelayControl::SetAnimationDelay(Skill* const pSkill, const DWORD delayTick)
{
    return SetAnimationDelay(reinterpret_cast<PVOID>(pSkill), delayTick);
}

inline BOOLEAN
AnimationDelayControl::SetAnimationDelay(BaseStatus* const pStatus, const DWORD delayTick)
{
    return SetAnimationDelay(reinterpret_cast<PVOID>(pStatus), delayTick);
}

inline BOOLEAN
AnimationDelayControl::SetAnimationDelay(Ability* const pAbility, const DWORD delayTick)
{
    return SetAnimationDelay(reinterpret_cast<PVOID>(pAbility), delayTick);
}

