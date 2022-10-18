#ifndef _FLYINGSTATE_H_
#define _FLYINGSTATE_H_

#include "State.h"

// @history
// (CHANGES) (091222) (WAVERIX) code arrangement

class FlyingState : public State
{
public:
    FlyingState();
    ~FlyingState();

    virtual void Init();
    virtual void OnEnter(LPARAM param1, LPARAM param2, LPARAM param3);
    virtual void OnExit();
    virtual BOOL OnUpdate(DWORD dwDeltaTick);

protected:
    virtual void OnMsgForceAttack(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgHelpRequest(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgFlying(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgThrust(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgKnockDown(const AI_MSG* pMsg);
    virtual void OnMsgStun(const AI_MSG* pMsg);
    virtual void OnMsgLetsGo(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgEnemyFound(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgCommandFollow(const AI_MSG* pMsg);
    virtual void OnMsgUseSkill(const AI_MSG* pMsg) { __UNUSED(pMsg); }

private:
    nsAI::HeartbeatTimer m_FlyingTimer;
    BOOL m_bKnockDown;
};

#endif // _THRUSTSTATE_H_