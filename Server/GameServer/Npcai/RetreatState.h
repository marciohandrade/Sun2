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
    BYTE m_bTrialCount; // ������ ��ǥã�⸦ �õ��� ȸ��
    BYTE m_bIdleAfterRetreat; // ������ �� ������ ���ΰ�
    float m_fMaxDistance; // �������� �ִ�Ÿ�
};

#endif // _RetreatState_H_