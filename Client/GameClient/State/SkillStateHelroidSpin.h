#pragma once

class StateSkill;

class SkillStateHellroidSpin : public StateSkill
{
public:

    SkillStateHellroidSpin();
    ~SkillStateHellroidSpin();

    STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
    STATECHANGE::TYPE OnStart(Object* pOwner, DWORD CurTime);

#ifdef _NA_008296_20150518_HELLOID_SPIN_CANCLE_BUG_FIX
    BOOL CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);
#endif //_NA_008296_20150518_HELLOID_SPIN_CANCLE_BUG_FIX
    
    void SetHeroKeyboardMove();

    BOOL SendKeyboardMovePacket(Object* pObject, DWORD dwTick);
    void CalculateTurnAccelerate(Object* pPlayerObject, DWORD dwTick);

    void RecvKeyboardMovePacket(Object* pOwner, BYTE MoveState, const WzVector& CurrentPosition, float fAngle );

    BOOL SetTargetPosition(Object* pOwner, const WzVector& NewDirection);

private:
    DWORD m_EndTime;
    DWORD m_StartTime;
    DWORD m_AnimateState;

    WzVector m_MoveTargetPosition;

    DWORD m_MoveState;
};
