#pragma once

/*���� ��Ƽ ������Ʈ�� �߰��Ǵ� ��Ƽ �� ���� Ŭ���� : ��Ʋ�� ������ �۾����� ���� �ۼ�
   - ���� �κ� �� ���� ����� �ݸ� ��Ƽ���� ��Ƽ ������ �������� �����ϴ� �����̴�.*/
 
//_NA_20110630_BATTLE_ZONE_RENEWAL

//��Ƽ�� �߰� ���� ���� : ������ �������� PartyRoom�� ��� �޴� ������ Room���������� 
//�����丵�� �ʿ������� ����ν� ������ 1���� ���̶� �Ʒ��� ���� �߰� ������ �Է��Ѵ�.
struct PartyRoomAdditionalInfo
{
    SLOTCODE require_item_;

    inline void clear()
    {
        require_item_ = 0;
    }
};

class PartyRoom
{
public:

    enum PartyRoomState
    {
        PartyRoomStateNormal,//��Ƽ ������ �ϰ� �ִ� ���� ����
        PartyRoomStateWaitToStart,//��ŸƮ ��ư�� ������ ���� �ð��� ��ٸ��� �ִ� ����

        //_NA_000000_20120527_ARENA
        //Agent���� Game ������ ��Ŷ�� ������ ���� ������
        PartyRoomStateSendPacketToGameServer,
    };

    PartyRoom();
    ~PartyRoom();

    void JoinRoom();
    void LeaveRoom();

    const PartyRoomInfo* GetPartyRoomInfo()const;
    void SetPartyRoomInfo(
        const PartyRoomInfo& party_room_info
        );

    PartyRoomInfo* GetEditablePartyRoomInfo();
    void SetBasePartyRoomInfo(
        const BasePartyRoomInfo& base_party_room_info
        );
   
    bool SetPlayerMaxCount(
        BYTE number_of_slot
        );

    MAPCODE GetMapCode()const;
    void SetMapCode(
        MAPCODE map_code
        );

    PartyZoneType GetPartyRoomType()const;
    void SetPartyRoomType(
        PartyZoneType set_zone_type
        );

    ePVP_MODE_TYPE GetPvPMode()const;
    void SetPvPMode(
        ePVP_MODE_TYPE pvp_mode_type
        );

    bool GetIsMissionStart()const;
    void SetIsMissionStart(
        bool is_start
        );

    const PartyRoomAdditionalInfo* GetPartyRoomAdditionalInfo()const;
    void SetPartyRoomAddInfoOfRequireItem(
        SLOTCODE require_item
        );

    eZONETYPE GetZoneType()const;

    KEYTYPE GetRoomKey()const;

    WORD GetPartyKey()const;

    BYTE SelectPVPTeam(
        BYTE number_of_team
        );

    bool CheckPassword(
        const TCHAR* pass_word
        )const;

    bool CheckLevel(
        LEVELTYPE user_level
        )const;

    bool IsPrivateRoom()const;

    void SetSuperRoomInfo(
        User* user
        );

    bool CanJoinPartyRoomByMemberCount()const;

    bool CanReturnJoinPartyRoomByMemberCount()const;

    void SetPartyRoomToStartState(
        BYTE count
        );

    bool IsEndRoomForWaitingToStart();

    bool IsStartWaitingRoom()const;

//private:
public:
    PartyRoomState _GetPartyRoomState()const;
    void _SetPartyRoomState(
        PartyRoomState party_room_state
        );

private:
    PartyRoomInfo party_room_info_;
    ePVP_MODE_TYPE pvp_mode_type_;
    PartyRoomState party_room_state_;
    util::Timer party_room_wait_to_start_timer_;
    PartyRoomAdditionalInfo additional_info_;
};

inline void 
PartyRoom::SetPartyRoomAddInfoOfRequireItem(SLOTCODE require_item)
{
    additional_info_.require_item_ = require_item;
}

inline const PartyRoomAdditionalInfo*
PartyRoom::GetPartyRoomAdditionalInfo()const
{
    return &(additional_info_);
}

inline void
PartyRoom::SetIsMissionStart(bool is_start)
{
    party_room_info_.is_mission_start_ = is_start;
}

inline bool
PartyRoom::GetIsMissionStart()const
{
    return party_room_info_.is_mission_start_;
}

inline ePVP_MODE_TYPE 
PartyRoom::GetPvPMode()const
{
    return pvp_mode_type_;
}

inline void 
PartyRoom::SetPvPMode(ePVP_MODE_TYPE pvp_mode_type)
{
    pvp_mode_type_ = pvp_mode_type;
}

inline WORD 
PartyRoom::GetPartyKey()const
{
    return party_room_info_.link_party_key_;
}

inline KEYTYPE
PartyRoom::GetRoomKey()const
{
    return party_room_info_.room_key_;
}

inline const PartyRoomInfo*
PartyRoom::GetPartyRoomInfo()const
{
    return &party_room_info_;
}

inline PartyRoomInfo*
PartyRoom::GetEditablePartyRoomInfo()
{
    return &party_room_info_;
}

inline void
PartyRoom::SetPartyRoomInfo(const PartyRoomInfo& party_room_info)
{
    party_room_info_ = party_room_info;
}

inline void
PartyRoom::SetBasePartyRoomInfo(const BasePartyRoomInfo& base_party_room_info)
{
    party_room_info_.base_party_room_info = base_party_room_info;
}

inline void 
PartyRoom::SetMapCode(MAPCODE map_code)
{
    party_room_info_.base_party_room_info.map_code_ = map_code;
}

inline MAPCODE
PartyRoom::GetMapCode()const
{
    return party_room_info_.base_party_room_info.map_code_;
}

inline PartyZoneType
PartyRoom::GetPartyRoomType()const
{
    return party_room_info_.base_party_room_info.party_room_type_;
}

inline void 
PartyRoom::SetPartyRoomType(PartyZoneType set_zone_type)
{
    party_room_info_.base_party_room_info.party_room_type_ = set_zone_type;
}

inline void 
PartyRoom::_SetPartyRoomState(PartyRoomState party_room_state)
{
    party_room_state_ = party_room_state;
}

inline PartyRoom::PartyRoomState 
PartyRoom::_GetPartyRoomState()const
{
    return party_room_state_;
}