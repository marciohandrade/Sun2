#pragma once

#include "State.h"

// @history
// (CHANGES) (091222) (WAVERIX) code arrangement

class StopIdleState : public State
{
public:
    StopIdleState();
    ~StopIdleState();

    virtual void Init();
    virtual void OnEnter(LPARAM param1, LPARAM param2, LPARAM param3);
    virtual void OnExit();
    virtual BOOL OnUpdate(DWORD dwDeltaTick);

protected:
    virtual void OnMsgAttacked(const AI_MSG* pMsg);
    virtual void OnMsgHelpRequest(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgLetsGo(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgRunAway(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgChangeToReturnState(const AI_MSG* pMsg) { __UNUSED(pMsg); }

private:
    nsAI::HeartbeatStaticTimer m_SearchTimer;
};

