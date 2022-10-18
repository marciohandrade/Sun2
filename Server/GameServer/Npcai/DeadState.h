#ifndef _DEADSTATE_H_
#define _DEADSTATE_H_

#include "State.h"

// @history
// (CHANGES) (091222) (WAVERIX) code arrangement

// 몬스터의 사망 상태
// special action 이나 삭제(연기로 사라짐) 상태로 전이
class DeadState : public State
{
public:
    DeadState();
    ~DeadState();

    virtual void Init();
    virtual void OnEnter(LPARAM param1, LPARAM param2, LPARAM param3);
    virtual void OnExit();
    virtual BOOL OnUpdate(DWORD dwDeltaTick);
    virtual void OnAIMsg(const AI_MSG* pMsg);
    void SetNextTimeoutInterval(ulong value);

protected:
    virtual void OnMsgForceAttack(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgHelpRequest(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgKnockDown(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgStun(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgLetsGo(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgEnemyFound(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgRunAway(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgChaos(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgChangeToReturnState(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgCommandFollow(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgUseSkill(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgLeaveField(const AI_MSG* pMsg);

private:
    nsAI::HeartbeatTimer m_DeadTimer;
};

inline void
DeadState::SetNextTimeoutInterval(ulong value) {
    m_DeadTimer.SetTimer(value);
}

#endif // _DEADSTATE_H_