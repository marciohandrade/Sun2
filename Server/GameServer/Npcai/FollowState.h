#pragma once

#ifndef SUN_GAMESERVER_FOLLOWSTATE_H_
#define SUN_GAMESERVER_FOLLOWSTATE_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "State.h"

// implemented by _NA_0_20100222_UNIT_TRIGGERS_PATHLIST

class Monster;
class Character;

class FollowState : public State
{
public:
    static const INT kDistanceRunFollow = 4;

public:
    FollowState();
    ~FollowState();

    virtual void Init();
    virtual void OnEnter(LPARAM param1, LPARAM param2, LPARAM param3);
    virtual void OnExit();
    virtual BOOL OnUpdate(DWORD dwDeltaTick);
    virtual void SetNPC(NPC* pNPC);

protected:
    WzVector FindRandomPos();
    eCHAR_MOVE_STATE GetMoveState(const WzVector& follow_pos, const WzVector& cur_pos);

    virtual void OnMsgForceAttack(const AI_MSG* pMsg);
    virtual void OnMsgHelpRequest(const AI_MSG* pMsg) {__UNUSED(pMsg);}
    virtual void OnMsgLetsGo(const AI_MSG* pMsg) {}
    virtual void OnMsgEnemyFound(const AI_MSG* pMsg) {__UNUSED(pMsg);}

private:
    void DoTrack();

    Monster* monster_;
    Character* follow_char_;
    nsAI::HeartbeatStaticTimer search_timer_;
    nsAI::HeartbeatStaticTimer track_timer_;
};

#endif // SUN_GAMESERVER_FOLLOWSTATE_H_
