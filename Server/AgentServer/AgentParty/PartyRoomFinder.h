#pragma once
/*
유저가 적합한 파티를 찾을수 있게 파티키및 예약 유저 리스트를 관리해주는 클래스
*/
//_NA_20110630_BATTLE_ZONE_RENEWAL

typedef STLX_LIST<WORD> PartyKeyList;
typedef STLX_LIST<CHARGUID> CharGuidList;

struct PartyRoomListInfo
{
    PartyKeyList* party_key_list_;
    CharGuidList* char_guid_list_;
};
typedef STLX_MAP<MAPCODE, PartyRoomListInfo> MissionPartyInfoMap;

class PartyRoomFinder
{
public:
    PartyRoomFinder();
    ~PartyRoomFinder();

    WORD GetPartyKeyInMissionList(User* user, const MAPCODE* map_code_array, BYTE check_map_num)const;

    bool InsertCharGuidToMissionList(CHARGUID insert_char_guid, 
                                     const MAPCODE* mission_list, BYTE count);

    void DeleteAllCharGuidFromMissionList(CHARGUID delete_char_guid);

    bool InsertPartyKeyToMissionList(MAPCODE map_code, WORD insert_party_key);
    
    void DeletePartyKeyFromMissionList(MAPCODE map_code, WORD delete_party_key);
#ifdef _NA_004706_20120413_1202_SECRET_ROOM_DONOT_SEND_MESSAGE
    bool UpdateSelectedMapCodeOfParty(MAPCODE delete_map_code, MAPCODE insert_map_code, 
                                      const WORD party_key, const BOOL check_secret_number);
#else
    bool UpdateSelectedMapCodeOfParty(MAPCODE delete_map_code, 
                                      MAPCODE insert_map_code, WORD party_key);
#endif  // _NA_004706_20120413_1202_SECRET_ROOM_DONOT_SEND_MESSAGE

    void NotifyCreatePartyRoom(MAPCODE map_code, WORD party_key);

private:
    bool _CanJoinParty(User* user, WORD party_key)const;
    void _Realese();    

    const PartyKeyList* _FindPartyKeyList(MAPCODE map_code)const;
    PartyKeyList* _FindEditablePartyKeyList(MAPCODE map_code);
    void _InsertPartyKey(MAPCODE map_code, WORD insert_party_key);
    void _DeletePartyKey(MAPCODE map_code, WORD delete_party_key);
    bool _IsCharGuid(MAPCODE map_code, CHARGUID char_guid)const;

    const CharGuidList* _FindCharGuidList(MAPCODE map_code)const;
    CharGuidList* _FindEditableCharGuidList(MAPCODE map_code);
    void _InsertCharGuid(MAPCODE map_code, CHARGUID insert_char_guid);
    bool _DeleteCharGuid(MAPCODE map_code, CHARGUID delete_char_guid);

    void _Allocate(PartyRoomListInfo* party_room_info_list);
    void _DeAllocate(PartyRoomListInfo* party_room_info_list);

    void _NotifyCreateRoomToStandByUser(MAPCODE map_code, WORD party_key);

private:
    MissionPartyInfoMap mission_party_map_;
};
