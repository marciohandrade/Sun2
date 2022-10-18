#pragma once

#include "State.h"

// @history
// (CHANGES) (091222) (WAVERIX) code arrangement
//==================================================================================================

class AttackState : public State
{
public:
    AttackState();
    ~AttackState();

    virtual void Init();
    virtual void OnEnter(LPARAM param1, LPARAM param2, LPARAM param3);
    virtual void OnExit();
    virtual BOOL OnUpdate(DWORD dwDeltaTick);

protected:
    // 메세지 처리 오버라이딩
    virtual void OnMsgForceAttack(const AI_MSG* pMsg);
    virtual void OnMsgHelpRequest(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgLetsGo(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgEnemyFound(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgChangeToRetreatState(const AI_MSG* pMsg);

private:
    void DoAction();

    void DoNormalAttack();
    BOOL UseSkill(SLOTCODE skillCode); 

    // skill utility functions
    BOOL IsReachableSkillRange(Character* pTargetChar, float fSkillRange);
    void ActionAfterUseSkill();

    nsAI::Npc75PercentageRegulator interval_regulator_;
    nsAI::HeartbeatTimer m_ActionDelay;
    nsAI::HeartbeatStaticTimer m_UpdateTimer;
    nsAI::HeartbeatStaticTimer m_FallApartTimer;
    nsAI::HeartbeatStaticTimer m_RetreatTimer;

    static const DWORD FALL_APART_DELAY;
};

//==================================================================================================
// (CHANGES) (091216) (WAVERIX) move header code block to code-file
//class SendBattlePointOpr

