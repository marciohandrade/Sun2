#pragma once

#include "State.h"

// @history
// (CHANGES) (091222) (WAVERIX) code arrangement

class BlindState : public State
{
public:
    BlindState(void);
    ~BlindState(void);

    virtual void Init();
    virtual void OnEnter(LPARAM param1, LPARAM param2, LPARAM param3);
    virtual void OnExit();
    virtual BOOL OnUpdate(DWORD dwDeltaTick);

private:
    nsAI::HeartbeatStaticTimer m_BlindTimer;
};

