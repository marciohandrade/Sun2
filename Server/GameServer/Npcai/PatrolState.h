#pragma once

#include "State.h"

// @history
// (CHANGES) (091222) (WAVERIX) code arrangement

class Monster;

class PatrolState : public State
{
public:
    PatrolState();
    ~PatrolState();

    virtual void Init();
    virtual void OnEnter(LPARAM param1, LPARAM param2, LPARAM param3);
    virtual void OnExit();
    virtual BOOL OnUpdate(DWORD dwDeltaTick);

    virtual void SetNPC(NPC* pNPC);

protected:
    virtual void OnMsgAttacked(const AI_MSG* pMsg);
    virtual void OnMsgLetsGo(const AI_MSG* pMsg) { __UNUSED(pMsg); }

private:
    BOOL DoPatrol();
    BOOL FindNextPos();
    void FindNextRandomPos(BOOL Direct);

private:
    Monster* m_pMonster;
    nsAI::HeartbeatStaticTimer m_SearchTimer;
    nsAI::HeartbeatStaticTimer m_PatrolTimer;
    int m_iIndexOfTileIndex;
    BYTE m_bFirst;
    BYTE m_bIncreaseIndex;
    BYTE m_bRandomPatrol;
    WzVector m_vDestPos;
};
