#ifndef _HELPSTATE_H_
#define _HELPSTATE_H_

#include "State.h"

// @history
// (CHANGES) (091222) (WAVERIX) code arrangement

class HelpState : public State
{
public:
    HelpState();
    ~HelpState();

    virtual void Init();
    virtual void OnEnter(LPARAM param1, LPARAM param2, LPARAM param3);
    virtual void OnExit();
    virtual BOOL OnUpdate(DWORD dwDeltaTick);
    // (WARNING) invalid... declaration
    virtual DWORD GetStateID() { return STATE_ID_HELP; } // TODO(WAVERIX)

protected:
    virtual void OnMsgForceAttack(const AI_MSG* pMsg);
    virtual void OnMsgAttacked(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgHelpRequest(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgLetsGo(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgEnemyFound(const AI_MSG* pMsg) { __UNUSED(pMsg); }

private:
    void DoTrack();

    nsAI::HeartbeatTimer m_HelpTimer;
};

#endif // _TRACKSTATE_H_