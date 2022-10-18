#pragma once

#include "State.h"

// @history
// (CHANGES) (091222) (WAVERIX) code arrangement

class Monster;

class ReturnState : public State
{
public:
    ReturnState();
    ~ReturnState();

    virtual void Init();
    virtual void OnEnter(LPARAM param1, LPARAM param2, LPARAM param3);
    virtual void OnExit();
    virtual BOOL OnUpdate(DWORD dwDeltaTick);

    virtual void SetNPC(NPC* pNPC);

protected:
    virtual void OnMsgForceAttack(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgAttacked(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgLetsGo(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgEnemyFound(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgHelpRequest(const AI_MSG* pMsg) { __UNUSED(pMsg); }

private:
    void DoReturn();

private:
    Monster* m_pMonster;
    nsAI::HeartbeatStaticTimer m_TrackTimer;
    BYTE m_bRegenPosSelected; // 리젠좌표로 조금씩 이동하는데 마지막으로 리젠좌표를 셋팅한 상태인가?
};
