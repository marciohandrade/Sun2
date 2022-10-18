#pragma once

#ifndef SUN_GAMESERVER_PATHLISTSTATE_H_
#define SUN_GAMESERVER_PATHLISTSTATE_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "State.h"

// implemented by _NA_0_20100222_UNIT_TRIGGERS_PATHLIST

class Monster;

// ��� Ž�� ����. 
// �������� ������ ��θ� ���� �̵��Ѵ�. 
// �̵� �Ϸ��� ������ ����� ������ �̵��Ѵ�.

class PathListState : public State
{
public:
    static const INT kAroundDestPos = 2;

public:
    PathListState();
    ~PathListState();

    virtual void Init();
    virtual void OnEnter(LPARAM param1, LPARAM param2, LPARAM param3);
    virtual void OnExit();
    virtual BOOL OnUpdate(DWORD dwDeltaTick);

    virtual void SetNPC(NPC* pNPC);

protected:
    virtual void OnMsgAttacked(const AI_MSG* pMsg);
    virtual void OnMsgLetsGo(const AI_MSG* /*pMsg*/) {}

private:
    bool DoTrackPath();
    void ChangeMovePosIndex();
    const WzVector& GetDestPos();
    WzVector FindNextRandomPos();

private:
    Monster* monster_;
    nsAI::HeartbeatStaticTimer search_timer_;
    nsAI::HeartbeatStaticTimer track_timer_;
    bool is_success_track_;
    bool is_arrive_;
    bool is_pos_index_back_; // ��� �̵� �Ϸ��� patrol. 
                             // ��ȸ�� ������ ������ ���൵�� ���º���� �������.(���������� ����)
    bool is_arrived_last_position_; // ������ ��ġ�� ���� �ߴ��� ����
};

#endif // SUN_GAMESERVER_PATHLISTSTATE_H_
