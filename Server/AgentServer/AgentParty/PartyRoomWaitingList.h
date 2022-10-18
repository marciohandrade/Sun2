#pragma once

/*
- 유저가 미션을 시작하게 되면 일정시간(2011.11.03 : 10초)동안 대기 미션 시작 대기 상태를 거친다.
- 유저가 생성한 각 PartyRoom에서 미션시작을 요청하게 되면 이 클래스의 리스트에 등록하여 상태를 파악
  해야 하는 대상으로 등록한다. AgentManager에서 대상이 되는 PartyRoom의 시작 상태를 검사하여 처리.
- PartyRoom에서 시작이 완료 되면 대상 등록을 해제 한다.
*/

//_NA_20110630_BATTLE_ZONE_RENEWAL

#include "AgentPartyManager.h"

class PartyRoomWaitingList
{
public:
    typedef STLX_LIST<WORD> StartWaitingRoomList;

    bool 
        InsertPartyRoomToWaitingList(WORD insert_party_key);

    StartWaitingRoomList*
        GetEditableWaitingRoomList();

    WORD
        GetWaitingRoomListCount();

private:
    void 
        _Insert(WORD insert_party_key);

    void 
        _Delete(WORD delete_party_key);

    const WORD*
        _Find(WORD find_party_key)const;

private:
    StartWaitingRoomList wait_room_list_;
};