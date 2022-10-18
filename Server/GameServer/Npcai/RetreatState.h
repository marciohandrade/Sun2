#ifndef _RetreatState_H_
#define _RetreatState_H_

#include "State.h"

// @history
// (CHANGES) (091222) (WAVERIX) code arrangement

class RetreatState : public State
{
public:
    RetreatState();
    ~RetreatState();

    virtual void Init();
    virtual void OnEnter(LPARAM param1, LPARAM param2, LPARAM param3);
    virtual void OnExit();
    virtual BOOL OnUpdate(DWORD dwDeltaTick);

protected:
    virtual void OnMsgForceAttack(const AI_MSG* pMsg);
    virtual void OnMsgLetsGo(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgEnemyFound(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgHelpRequest(const AI_MSG* pMsg) { __UNUSED(pMsg); }

    void Retreat();

private:
    BYTE m_bFindDestPos;
    BYTE m_bTrialCount; // 목적지 좌표찾기를 시도한 회수
    BYTE m_bIdleAfterRetreat; // 물러난 후 공격할 것인가
    float m_fMaxDistance; // 물러나는 최대거리
};

#endif // _RetreatState_H_