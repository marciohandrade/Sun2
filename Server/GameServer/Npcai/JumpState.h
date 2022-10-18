#pragma once

#include "State.h"

// @history
// (CHANGES) (091222) (WAVERIX) code arrangement

class JumpState : public State
{
public:
    JumpState();
    ~JumpState();

    virtual void Init();
    virtual void OnEnter(LPARAM param1, LPARAM param2, LPARAM param3);
    virtual void OnExit();
    virtual BOOL OnUpdate(DWORD dwDeltaTick);
    virtual void OnAIMsg(const AI_MSG* pMsg);

protected:
    virtual void OnMsgForceAttack(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgHelpRequest(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgKnockDown(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgLetsGo(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgEnemyFound(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgRunAway(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgChaos(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgChangeToReturnState(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgCommandFollow(const AI_MSG* pMsg);
    virtual void OnMsgUseSkill(const AI_MSG* pMsg) { __UNUSED(pMsg); }

private:
    BOOLEAN IsAttackDistance();
    void AttackResult();

    template<typename _TYPE>
    void SetJumpAttack(_TYPE bSet);
    BOOLEAN IsJumpAttack() const;
    void SetSendAttackResult(BOOL bSet);
    BOOLEAN IsSendAttackResult() const;

    //
    nsAI::HeartbeatTimer m_JumpTimer;
    BYTE m_bJumpAttack;
    BYTE m_bSendAttackResult;
    DWORD m_dwTotDistance2;
    WzVector m_wvDestPos;
};


template<typename _TYPE>
inline void
JumpState::SetJumpAttack(_TYPE bSet) {
    m_bJumpAttack = !!bSet;
}

inline BOOLEAN
JumpState::IsJumpAttack() const {
    return !!m_bJumpAttack;
}

inline void
JumpState::SetSendAttackResult(BOOL bSet) {
    m_bSendAttackResult = !!bSet;
}

inline BOOLEAN
JumpState::IsSendAttackResult() const {
    return m_bSendAttackResult;
}

