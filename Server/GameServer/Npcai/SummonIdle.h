#pragma once

#include "State.h"

// @history
// (CHANGES) (091222) (WAVERIX) code arrangement

class Summoned;

// ��ȯü�� Idle���´� �ܼ����� ���°� �ƴ϶� ��ȯ�ڸ� ������� ���¸� ���Ѵ�.
class SummonIdle : public State
{
public:
    SummonIdle();
    ~SummonIdle();

    virtual void Init();
    virtual void OnEnter(LPARAM param1, LPARAM param2, LPARAM param3);
    virtual void OnExit();
    virtual BOOL OnUpdate(DWORD dwDeltaTick);

    virtual void SetNPC(NPC* pNPC);

protected:
    virtual void OnMsgAttacked(const AI_MSG* pMsg);

    // ��ȯü�̹Ƿ� ���� �޼������� �����Ѵ�.
    virtual void OnMsgHelpRequest(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgLetsGo(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgEnemyFound(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgChangeToReturnState(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgGroupCommand(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgGroupCommand_StopAttack() {}
    virtual void OnMsgCommandFollow(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgGroupCommand_Attack(DWORD dwTargetKey) { __UNUSED(dwTargetKey); }

private:
    void FollowSummoner(Character* pSummoner); // ��ȯ�ڸ� ������.
    void Teleport(WzVector &vDestPos);
    void DecideMovement();

#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    //������ ��ų ���(���� ���� ��ų)
    void UseConstantSkill();
#endif //_NA_007667_20141001_WITCHBLADE_SKILL

private:
    Summoned* m_pSummoned;

    util::Timer m_SearchTimer;
    util::Timer m_TrackTimer;
    util::Timer m_SpawnTimer; // �����ִϸ� �����ϵ��� ��ٷ��ش�.
    WzVector m_vMovement; // ��ȯ�� ��ó�� ��ġ����
    WzVector m_vSummonedPos, m_vSummonerPos, m_vSummonerDestPos;
};




