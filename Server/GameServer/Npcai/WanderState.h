#pragma once

#include "State.h"

// @history
// (CHANGES) (091222) (WAVERIX) code arrangement

class Monster;

class WanderState : public State
{
public:
    WanderState();
    ~WanderState();

    virtual void Init();
    virtual void OnEnter(LPARAM param1, LPARAM param2, LPARAM param3);
    virtual void OnExit();
    virtual BOOL OnUpdate(DWORD dwDeltaTick);

    virtual void SetNPC(NPC* pNPC);

protected:
    virtual void OnMsgAttacked(const AI_MSG* pMsg);
    virtual void OnMsgLetsGo(const AI_MSG* pMsg);

private:
    BOOL DoWander();
    BOOL DoWanderPatrol();

    inline void _UpdateTouchedSomething(BOOL bSet);

    Monster* m_pMonster;
    nsAI::HeartbeatStaticTimer m_SearchTimer;
    WzVector m_wvRotatedVector;
    BYTE m_bDestSelected;
    BYTE m_bTouchedSomething;
};


