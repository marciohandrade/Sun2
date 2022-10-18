#pragma once

#include "State.h"

// @history
// (CHANGES) (091222) (WAVERIX) code arrangement

class TrackAreaState : public State
{
public:
    TrackAreaState();
    ~TrackAreaState();

    virtual void Init();
    virtual void OnEnter(LPARAM param1, LPARAM param2, LPARAM param3);
    virtual void OnExit();
    virtual BOOL OnUpdate(DWORD dwDeltaTick);

protected:
    virtual void OnMsgAttacked(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgLetsGo(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgHelpRequest(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgEnemyFound(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgChangeToReturnState(const AI_MSG* pMsg) { __UNUSED(pMsg); }

private:
    void DoTrackArea();

    nsAI::HeartbeatStaticTimer m_SearchTimer;
    nsAI::HeartbeatStaticTimer m_TrackTimer;

    WzVector m_vAreaPos; // ���� ���� ��ǥ��
    BYTE m_bIsSetAreaPos;
    BYTE m_bRegenPosSelected; // ������ǥ�� ���ݾ� �̵��ϴµ� ���������� ������ǥ�� ������ �����ΰ�?
};


