#ifndef _RUNAWAYSTATE_H_
#define _RUNAWAYSTATE_H_

#include "State.h"

// @history
// (CHANGES) (091222) (WAVERIX) code arrangement

class RunAwayState : public State
{
public:
    RunAwayState();
    ~RunAwayState();

    virtual void Init();
    virtual void OnEnter(LPARAM param1, LPARAM param2, LPARAM param3);
    virtual void OnExit();
    virtual BOOL OnUpdate(DWORD dwDeltaTick);

protected:
    virtual void OnMsgForceAttack(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgAttacked(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgLetsGo(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgEnemyFound(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgRunAway(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgCommandFollow(const AI_MSG* pMsg);
    virtual void OnMsgHelpRequest(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgChangeToReturnState(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgUseSkill(const AI_MSG* pMsg) { __UNUSED(pMsg); }

private:
    void RunAway();

private:
    nsAI::HeartbeatTimer m_RunAwayTimer;
    WzVector m_vTargetPos; // 도망가야할 적의 위치
    WzVector m_wvRotatedVector;
    BYTE m_bTouchedSomething;
    WORD m_wStateID;
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    eABILITY m_abilityID;
    DWORD    m_dwTargetKey;
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
};

#endif // _RUNAWAYSTATE_H_