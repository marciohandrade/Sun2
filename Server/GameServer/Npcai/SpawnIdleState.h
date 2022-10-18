#pragma once

#include "State.h"

// @history
// (CHANGES) (091222) (WAVERIX) code arrangement

class Monster;

class SpawnIdleState : public State
{
public:
    SpawnIdleState();
    ~SpawnIdleState();

    virtual void Init();
    virtual void OnEnter(LPARAM param1, LPARAM param2, LPARAM param3);
    virtual void OnExit();
    virtual BOOL OnUpdate(DWORD dwDeltaTick);

    virtual void SetNPC(NPC* pNPC);

protected:
    virtual void OnMsgAttacked(const AI_MSG* pMsg);
    virtual void OnMsgLetsGo(const AI_MSG* pMsg) {}

private:
    Monster* m_pMonster;
    nsAI::HeartbeatStaticTimer m_SearchTimer;
};

