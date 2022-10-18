#include "stdafx.h"

#include "AnimationDelayControl.h"

BOOLEAN
AnimationDelayControl::NeedSkipProcessByAnimationDelay(const PVOID pObject)
{
    const BOOLEAN RET_NEED_SKIP = TRUE;
    const BOOLEAN RET_NORMAL = FALSE;

    if(m_pQueue == 0)
        return RET_NORMAL;

    ANIMATION_DELAY_QUEUE& rList = *m_pQueue;

    while(rList.empty() == false)
    {
        sDelayInfo& rInfo = rList.front();

        const BOOL isExpired = rInfo.Timeout.IsExpired();
        if(isExpired == 0)
            break;

        const BOOLEAN finded = pObject && (pObject == rInfo.RelatedObject);
        rList.pop_front();
        if(finded)
            return RET_NORMAL;
    }

    if(rList.empty())
    {
        rList.clear();
        return RET_NORMAL;
    }

    sDelayInfo& rInfo = rList.front();
    return (pObject && (pObject == rInfo.RelatedObject)) ? RET_NORMAL : RET_NEED_SKIP;
}

BOOLEAN
AnimationDelayControl::SetAnimationDelay(const PVOID pObject, const DWORD delayTick)
{
    const BOOLEAN RET_NEED_SKIP = FALSE;
    const BOOLEAN RET_NORMAL = TRUE;

    if((pObject && delayTick) == 0)
        return RET_NORMAL;

    const DWORD curTick = GetTickCount();
    const DWORD exp_next = curTick + delayTick;

    if(m_pQueue == 0)
        m_pQueue = TAllocNew(ANIMATION_DELAY_QUEUE);

    ANIMATION_DELAY_QUEUE& rList = *m_pQueue;
    ANIMATION_DELAY_QUEUE::iterator it = rList.begin();
    while(it != rList.end())
    {
        sDelayInfo& rInfo = *it;
        if(rInfo.Timeout.IsExpired())
            it = rList.erase(it);
        else
            ++it;
    }

    if(rList.empty())
    {
        sDelayInfo info;
        info.RelatedObject = pObject;
        info.Timeout.SetNextTimeoutByCurTickBased(delayTick);
        m_pQueue->push_back(info);
        return RET_NORMAL;
    }

    sDelayInfo& lastInfo = m_pQueue->back();
    const DWORD lastTimeout = lastInfo.Timeout.GetTimeoutTicks();
    if(lastTimeout < exp_next)
    {
        sDelayInfo info;
        info.RelatedObject = pObject;
        info.Timeout.SetNextTimeoutByCurTickBased(delayTick);
        m_pQueue->push_back(info);
    }
    return RET_NEED_SKIP;
}

