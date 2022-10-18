#pragma once

#include "AgentPartyManager.h"

/*파티룸 리스트 관리 클레스*/

//_NA_20110630_BATTLE_ZONE_RENEWAL
#include "PartyRoomWaitingList.h"

class PartyRoom;
class PartyRoomWaitingList;

//파티 룸 리스트 : 페이지 단위 전송 기능을 위해 임의의 인덱싱으로의 접근이 가능한 VECTOR사용
typedef STLX_VECTOR<PartyRoom> RoomList;

class PartyRoomList
{
    friend class AgentPartyManager;
public:
    PartyRoomList();
    ~PartyRoomList();

#ifdef _NA_004694_20120412_1202_PARTY_ROOM_KEY
    bool
        CreatePartyRoom(
        const BasePartyRoomInfo& create_basic_room_info, 
        const WORD create_party_key,
        WORD& OUT create_room_key
        );
#else
    bool
        CreatePartyRoom(const BasePartyRoomInfo& create_basic_room_info, 
                        WORD create_party_key);
#endif  // _NA_004694_20120412_1202_PARTY_ROOM_KEY

    void 
        DestroyPartyRoom(KEYTYPE destroy_key);

    bool 
        UpdatePartyRoomInfo(const PartyRoomInfo& update_room_info);

    bool 
        UpdateBasePartyRoomInfo(const BasePartyRoomInfo& update_base_room_info, 
                                WORD party_key);

    BYTE 
        CalcAllPageOfRoomList();

    BYTE 
        ArchiveRoomList(PartyRoomInfo* OUT party_room_info, 
                        BYTE request_count, 
                        BYTE request_page)const;

    bool 
        JoinPartyRoom(User* user, 
                      WORD party_key);

    bool 
        LeavePartyRoom(WORD party_key);

    void 
        SetMissionCode(WORD party_key, 
                       MAPCODE mission_code);

    MAPCODE 
        GetMapCodeOfPartyRoom(WORD party_key)const;

    BYTE 
        GetMaxPlayerCount(WORD party_key);

    bool 
        SetMaxPlayerCount(WORD party_key, 
                          BYTE max_count);

    const PartyRoom* 
        FindPartyRoom(WORD find_party_key)const;

    const PartyRoom* 
        FindPartyRoomByRoomKey(KEYTYPE find_room_key)const;

    PartyRoomWaitingList* GetStartWaitingRoomList();
    RoomList& room_list() { return party_room_list_; }


    void 
        SetPartyRoomAddInfOfRequireItem(WORD party_key, SLOTCODE item_code);

    void 
        SetPartyRoomStateForWaitingToStart(WORD party_key);

    BYTE 
        SearchRoomByWaiting(PartyRoomInfo* OUT party_room_info, 
                            BYTE request_count)const;

    BYTE
        SearchRoomByMapCode(PartyRoomInfo* OUT party_room_info, 
                            MAPCODE map_code,
                            PartyZoneType party_room_type,
                            BYTE request_count)const;

protected:
    PartyRoom* 
        _FindEditableNode(WORD find_party_key);

private:
    bool 
        _SetPartyRoomStartState(WORD party_key);

    void 
        _InsertToWaitingList(WORD insert_party_key);

    void 
        _Insert(const PartyRoom& insert_room_info);

    void 
        _Delete(WORD delete_party_key);

    void 
        _MakingRoomInfo(PartyRoom& OUT create_party_room, 
                        const BasePartyRoomInfo& base_room_info,
                        KEYTYPE room_key, 
                        WORD party_key);

    KEYTYPE 
        _AllocKey();

    void 
        _FreeKey(KEYTYPE Key);

private:
    RoomList party_room_list_;
    PartyRoomWaitingList waiting_room_list_;

public:
#ifdef _DEBUG
    void 
        Test_Insert(const PartyRoom& insert_room_info);
#endif
};

inline KEYTYPE  
PartyRoomList::_AllocKey()  
{ 
    return static_cast<KEYTYPE>(ObjKeyGender::Instance()->GetKey(OBJKEYTYPE_PARTYROOMKEY)); 
}

inline void
PartyRoomList::_FreeKey(KEYTYPE Key) 
{ 
    ObjKeyGender::Instance()->RestoreKey(OBJKEYTYPE_PARTYROOMKEY, Key);
}

inline PartyRoomWaitingList*
PartyRoomList::GetStartWaitingRoomList()
{
    return &(waiting_room_list_);
}

#ifdef _DEBUG
namespace TestPartyRoomList {
;
extern void Test_InsertRoomList(BYTE data_count);

};
#endif//_DEBUG

