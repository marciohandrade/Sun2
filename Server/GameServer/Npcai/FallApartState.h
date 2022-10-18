#ifndef _FallApartState_H_
#define _FallApartState_H_

#include "State.h"

// @history
// (CHANGES) (091222) (WAVERIX) code arrangement

class FallApartState : public State
{
public:
    FallApartState();
    ~FallApartState();

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
    void FallApart();

    BOOL m_bFindDestPos;
};

#endif // _FallApartState_H_