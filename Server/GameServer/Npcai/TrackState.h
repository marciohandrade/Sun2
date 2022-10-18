#pragma once

#include "State.h"

// @history
// (CHANGES) (091222) (WAVERIX) code arrangement

class TrackState : public State
{
public:
    TrackState();
    ~TrackState();

    virtual void Init();
    virtual void OnEnter(LPARAM param1, LPARAM param2, LPARAM param3);
    virtual void OnExit();
    virtual BOOL OnUpdate(DWORD dwDeltaTick);

protected:
    virtual void OnMsgForceAttack(const AI_MSG* pMsg);
    virtual void OnMsgHelpRequest(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgLetsGo(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgEnemyFound(const AI_MSG* pMsg) { __UNUSED(pMsg); }

private:
    void DoTrack();

    nsAI::HeartbeatStaticTimer m_TrackTimer;
    nsAI::HeartbeatStaticTimer m_ReturnCheckTimer;
    BYTE m_JumpRatio; // (CHANGES) (091223) add field about enable Jump 
    BOOLEAN m_bJumpTry; // 점프시도
    WORD m_TrackCount;
};



