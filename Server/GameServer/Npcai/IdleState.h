#ifndef _IDLESTATE_H_
#define _IDLESTATE_H_

#include "State.h"

// @history
// (CHANGES) (091222) (WAVERIX) code arrangement

class IdleState : public State
{
public:
    IdleState();
    ~IdleState();

    virtual void Init();
    virtual void OnEnter(LPARAM param1, LPARAM param2, LPARAM param3);
    virtual void OnExit();
    virtual BOOL OnUpdate(DWORD dwDeltaTick);

protected:
    virtual void OnMsgAttacked(const AI_MSG* pMsg);
    virtual void OnMsgLetsGo(const AI_MSG* pMsg);

private:
    nsAI::HeartbeatStaticTimer m_IdleTimer;
    nsAI::HeartbeatStaticTimer m_SearchTimer;
};

#endif // _IDLESTATE_H_