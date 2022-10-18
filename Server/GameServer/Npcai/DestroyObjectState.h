#pragma once

#ifndef SUN_GAMESERVER_DESTROYOBJECTSTATE_H_
#define SUN_GAMESERVER_DESTROYOBJECTSTATE_H_

#if _MSC_VER > 1000
#pragma once
#endif

//_NA_0_20100514_NPC_DEAD_STATE_TIME

// 몬스터 삭제(연기로 사라짐) 상태

#include "State.h"

class DestroyObjectState : public State
{
public:
    DestroyObjectState() {}
    ~DestroyObjectState() {}

    virtual void Init() {}
    virtual void OnEnter(LPARAM param1, LPARAM param2, LPARAM param3);
    virtual void OnExit();
    virtual BOOL OnUpdate(DWORD dwDeltaTick);

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
#endif // SUN_GAMESERVER_DESTROYOBJECTSTATE_H_
