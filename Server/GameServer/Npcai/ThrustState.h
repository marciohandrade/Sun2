#ifndef _THRUSTSTATE_H_
#define _THRUSTSTATE_H_

#include "State.h"

// @history
// (CHANGES) (091222) (WAVERIX) code arrangement

class ThrustState : public State
{
public:
    ThrustState();
    ~ThrustState();

    virtual void Init();
    virtual void OnEnter(LPARAM param1, LPARAM param2, LPARAM param3);
    virtual void OnExit();
    virtual BOOL OnUpdate(DWORD dwDeltaTick);

protected:
    virtual void OnMsgForceAttack(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgFlying(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgKnockDown(const AI_MSG* pMsg);
    virtual void OnMsgStun(const AI_MSG* pMsg);
    virtual void OnMsgLetsGo(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgEnemyFound(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgHelpRequest(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgCommandFollow(const AI_MSG* pMsg);
    virtual void OnMsgUseSkill(const AI_MSG* pMsg) { __UNUSED(pMsg); }
#ifdef _NA_007009_20140102_ATTACK_AFTER_THRUSTSTATE
    virtual void OnMsgAttacked(const AI_MSG* pMsg);
#endif //_NA_007009_20140102_ATTACK_AFTER_THRUSTSTATE

private:
    nsAI::HeartbeatStaticTimer m_ThrustTimer;
    DWORD m_dwDownTimeAfterThrust;
    BYTE m_bDownAfterThrust;
};

#endif // _THRUSTSTATE_H_