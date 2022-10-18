#pragma once

#include "State.h"

// @history
// (CHANGES) (091222) (WAVERIX) code arrangement

class Summoned;

// 소환체의 Idle상태는 단순정지 상태가 아니라 소환자를 따라댕기는 상태를 말한다.
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

    // 소환체이므로 이하 메세지들은 무시한다.
    virtual void OnMsgHelpRequest(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgLetsGo(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgEnemyFound(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgChangeToReturnState(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgGroupCommand(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgGroupCommand_StopAttack() {}
    virtual void OnMsgCommandFollow(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgGroupCommand_Attack(DWORD dwTargetKey) { __UNUSED(dwTargetKey); }

private:
    void FollowSummoner(Character* pSummoner); // 소환자를 따른다.
    void Teleport(WzVector &vDestPos);
    void DecideMovement();

#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    //지속형 스킬 사용(오라 같은 스킬)
    void UseConstantSkill();
#endif //_NA_007667_20141001_WITCHBLADE_SKILL

private:
    Summoned* m_pSummoned;

    util::Timer m_SearchTimer;
    util::Timer m_TrackTimer;
    util::Timer m_SpawnTimer; // 스폰애니를 연출하도록 기다려준다.
    WzVector m_vMovement; // 소환자 근처의 위치벡터
    WzVector m_vSummonedPos, m_vSummonerPos, m_vSummonerDestPos;
};




