#pragma once

#ifndef SUN_GAMESERVER_NOTRANSSTATS_H_
#define SUN_GAMESERVER_NOTRANSSTATS_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "State.h"

// 상태 전이가 없는 상태.
// 완전 정지형, 공격이 없는 형태, 죽어도 사라지지 않는 형태의 몬스터 (톱니바퀴등.)
class NoTransState : public State
{
public:
    NoTransState() {}
    ~NoTransState() {}

    virtual void Init() {}
    virtual void OnEnter(LPARAM param1, LPARAM param2, LPARAM param3);
    virtual void OnExit();
    virtual BOOL OnUpdate(DWORD dwDeltaTick) { return true; }

protected:
    virtual void OnMsgForceAttack(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgThrust(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgFlying(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgKnockDown(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgStun(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgEnemyFound(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgChaos(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgChangeToRetreatState(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgCommandFollow(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgUseSkill(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgNONPVP(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgAttacked(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgHelpRequest(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgLetsGo(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgRunAway(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgChangeToReturnState(const AI_MSG* pMsg) { __UNUSED(pMsg); }
    virtual void OnMsgLeaveField(const AI_MSG* pMsg) { __UNUSED(pMsg); }
};

#endif // SUN_GAMESERVER_PATHLISTSTATE_H_
