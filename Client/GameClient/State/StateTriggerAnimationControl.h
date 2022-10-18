#pragma once

class StateBase;

class StateTriggerAnimationControl : public StateBase
{
    StateTriggerAnimationControl(void);
public:
    ~StateTriggerAnimationControl(void);
    __forceinline static StateTriggerAnimationControl* GetInstance()
    {
        static StateTriggerAnimationControl d;
        return &d;
    };

    virtual int GetStateKind() { return STATE::TRIGGER_ANIMATION_CONTROL; }
    virtual BOOL CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);

    virtual STATECHANGE::TYPE OnStart(Object* pOwner, DWORD CurTime);
    virtual STATEPROCESS::TYPE OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
    virtual STATECHANGE::TYPE OnEnd(Object* pOwner, DWORD CurTime);
};
