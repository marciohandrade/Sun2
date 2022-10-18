#pragma once

/*
- ������ �̼��� �����ϰ� �Ǹ� �����ð�(2011.11.03 : 10��)���� ��� �̼� ���� ��� ���¸� ��ģ��.
- ������ ������ �� PartyRoom���� �̼ǽ����� ��û�ϰ� �Ǹ� �� Ŭ������ ����Ʈ�� ����Ͽ� ���¸� �ľ�
  �ؾ� �ϴ� ������� ����Ѵ�. AgentManager���� ����� �Ǵ� PartyRoom�� ���� ���¸� �˻��Ͽ� ó��.
- PartyRoom���� ������ �Ϸ� �Ǹ� ��� ����� ���� �Ѵ�.
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