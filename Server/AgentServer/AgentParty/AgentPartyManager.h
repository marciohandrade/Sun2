#pragma once

#include <ObjKeyGenerator.h>
#include "BasePartyManager.h"
#include <Singleton.h>
#include "ClosingUserList.h"

class AgentParty;
class ZoneInterface;	class User;
class RoomInterface;
class InstanceDungeon;
struct MSG_AG_PARTY_NAVI_INFO_BRD;
struct MSG_AG_STATUS_CHAR_STAT_CHANGE_CMD;

//_NA_20110630_BATTLE_ZONE_RENEWAL
class PartyRoomFinder;
class PartyRoomList;
class PartyRoom;
struct MSG_AG_ZONE_CAN_CREATE_ROOM_OF_PARTY_ACK;
struct MSG_AG_PARTY_CAN_CHANGE_OPTION_ACK;

class AgentPartyManager : public util::Singleton<AgentPartyManager>, public BasePartyManager
{
public:
	AgentPartyManager();
	~AgentPartyManager();

	virtual void Initialize(DWORD party_pool_size);

	void Update();

	RC::ePARTY_RESULT DestroyParty(
        WORD party_key
        );

	RC::ePARTY_RESULT ChangeMaster(
        WORD party_key, 
        DWORD next_master_key
        );
    RC::ePARTY_RESULT ChangeMaster_inInstanceDungeon(
        InstanceDungeon* dungeon,
        User* user);

    BaseParty* CreateParty(
        DWORD master_key, 
        ePARTY_TYPE party_type
        );

	RC::ePARTY_RESULT CanJoinParty(
        User* user
        );

	RC::ePARTY_RESULT JoinParty(
        WORD party_key, 
        User* user
        );

	RC::ePARTY_RESULT LeaveParty(
        User* user, 
        eREASON_LEAVE_PARTY leave_to_reason
        );

	RC::ePARTY_RESULT CanRequestJoin(
        User* request_user, 
        User* response_user
        );

	RC::ePARTY_RESULT InviteParty(
        User* master_user, 
        User* join_user
        );

	RC::ePARTY_RESULT CanRequestLeave(
        User* request_user, 
        User* leave_user
        );

	RC::ePARTY_RESULT ForcedExpulsion(
        User* master_user, 
        User* leave_user
        );

	RC::ePARTY_RESULT SelectTarget(
        User* master_user, 
        DWORD object_key
        );

	RC::ePARTY_RESULT SelectMinimap(
        User* user, 
        float PosX, 
        float PosY
        );

	RC::ePARTY_RESULT ChangeDistributionType(
        User* user, 
        eITEM_DISTRIBUTION_TYPE distribution_type
        );

    //파티 복귀 관련 함수
	bool CanJoinReturnParty(
        WORD party_key, 
        User* user
        );

	RC::ePARTY_RESULT AcceptReturnParty(
        WORD party_key, 
        User* user
        );

	RC::ePARTY_RESULT RejectReturnParty(
        WORD party_key, 
        User* user
        );

    void UpdateReturnUser(
        WORD party_key, 
        ClosingUserInfo& closing_user_info
        );

    void RemoveClosingUser(
        WORD party_key, 
        const DWORD char_guid
        );

    void RemoveClosingUser(
        WORD party_key, 
        const TCHAR* char_name
        );

    void AddClosingUser(
        User* user, 
        const _PARTY_MEMBER_INFO& member_info
        );

	// 멤버 수치, 상태변화를 알린다.
	void MemberLevelUp(
        User* user, 
        FLOAT fHPRatio, 
        FLOAT fMPRatio
        );

	void MemberResurrection(
        User* user, 
        FLOAT fHPRatio, 
        FLOAT fMPRatio
        );

	void MemberChangeChaoState(
        User* user
        );

	void MemberDead(
        User* user
        );

    void ChangedMembersInfo(
        WORD party_key,
        const MSG_AG_PARTY_NAVI_INFO_BRD* navi,
        const MSG_AG_STATUS_CHAR_STAT_CHANGE_CMD* hpmp
        );

	// 존 연관 파티 관리
	void CreateZone(
        User* master_user, 
        ZoneInterface* pCurZone, 
        ZoneInterface* pCreateZone
        );

    void EnterZoneByPortal(
        PORTALKEY portalKey, 
        User* user, 
        ZoneInterface* pCurZone, 
        ZoneInterface* pToCreateZone
        );

	void EnterZone(
        User* user, 
        ZoneInterface* pEnterZone
        );

	void LeaveZone(
        User* user, 
        ZoneInterface* pLeaveZone
        );

	void MigrateCHuntingRoom(
        User* master_user, 
        ZoneInterface* pCurZone, 
        ZoneInterface* pEnterZone
        );

	// 기타 구현에 필요한 기능들
	bool IsMaster(
        User* user
        );

	void AddRelayUser(
        RoomInterface* pRoom
        );

	DWORD GetMemberNum(
        WORD party_key
        );

	void SetOtherMemberKey(
        User* pRequestUser, 
        User* pResponseUser
        );

	void ResetOtherMemberKey(
        User* pRequestUser, 
        User* pResponseUser
        );

	RC::eROOM_RESULT InstanceDungeonCheck(
        WORD party_key, 
        const sMAPINFO* psMAPINFO
        );

	RC::eROOM_RESULT TopOfTrialCheck(
        WORD party_key
        );

    RC::eROOM_RESULT MigrateToInstanceDungeon(
        User* pRequestUser, 
        InstanceDungeon* indun
        );

	void SendPacketAllMember(
        WORD party_key, 
        MSG_BASE* pMsg, 
        WORD wSize, 
        DWORD ExceptKey=0, 
        KEYTYPE CheckZoneKey=0
        );

	void SendPacketAllLinkedServer(
        WORD party_key, 
        MSG_BASE* pMsg, 
        WORD wSize
        );

	void				Display();

    RC::ePARTY_RESULT RequestInviteParty(
        User* request_user, 
        User* response_user
        );

    RC::ePARTY_RESULT ResponseInviteParty(
        User* response_user
        );

    bool CheckReturnPartyInfo(
        User* pole_user, 
        User* return_user);

    //_NA_20110630_BATTLE_ZONE_RENEWAL
    static void FixStringOfPartyRoomInfo(
        BasePartyRoomInfo* raw_msg
        );

    PartyRoomFinder* GetPartyRoomFinder();
    PartyRoomList* GetPartyRoomList();

    RC::ePARTY_RESULT CanCreatePartyRoom(
        User* user,
        const BasePartyRoomInfo* party_room_info
        )const;

    RC::ePARTY_RESULT CanStartRoom(
        User* user
        );

    RC::ePARTY_RESULT CreatePartyRoom(
        BasePartyRoomInfo& party_room_info, 
        User* user
        );

    void CreateNoneTypePartyRoom(
        User* master_user, 
        const BasePartyRoomInfo& party_room_info
        );

    void DestroyPartyRoom(
        WORD party_key
        );

#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS 
    RC::ePARTY_RESULT LeavePartyRoom(
        User* user, 
        eREASON_LEAVE_PARTY reason_to_leave
        );
#else
    RC::ePARTY_RESULT LeavePartyRoom(
        User* user
        );
#endif

    RC::ePARTY_RESULT JoinPartyRoom(
        User* user, 
        WORD party_key, 
        PartyZoneType party_room_type);

    RC::ePARTY_RESULT ChangePartyRoomOption(
        User* user, 
        WORD party_key, 
        const MSG_AG_PARTY_CAN_CHANGE_OPTION_ACK* check_msg
        );

    const PartyRoom* FindPartyRoom(
        WORD party_key
        )const;

    void SetMemberReadyState(
        User* user, 
        bool ready_state
        );

    bool GetMemberReadyState(
        User* user
        );

    BYTE GetMemberTeam(
        User* user
        );

    void SetMemberTeam(
        User* user, 
        BYTE selected_team
        );

    RC::ePARTY_RESULT   FoceLeavePartyRoom(
        User* request_user, 
        User* leave_user
        );

    KEYTYPE GetPartyRoomKey(
        WORD party_key
        );

    const PartyRoomInfo* GetPartyRoomInfoByMemberKey(
        DWORD member_key_
        );

    bool ExceptionProcessByPartyMember(
        const MSG_AG_ZONE_CAN_CREATE_ROOM_OF_PARTY_ACK* check_msg, 
        User* request_user
        );

    RC::ePARTY_RESULT CheckTermsByPartyRoomType(
        User* request_user, 
        const BasePartyRoomInfo* party_room_info
        )const;

    void SetIsMissionStart(
        WORD party_key, 
        bool is_start
        );

    void ResetPartyRoomReadyState(
        User* master_user, 
        WORD party_key
        );

    void KickPartyMemberForChangeOption(
        const MSG_AG_PARTY_CAN_CHANGE_OPTION_ACK* check_msg, 
        User* master_user
        );

    void NotifyMemberMoveArea(
        User* user, 
        MAPCODE map_code
        );

    void DoStartToWaitingRoom();
    

    void StartPartyRoom(
        User* request_user
        );

    void SetPartyRoomStartState(
        WORD party_key
        );

    void AddPartyRoomToWaitList(
        WORD add_party_key
        );

    void ChangeMasterOfPartyRoom(
        User* next_party_master
        );

private:
	void AddMemberToSession(
        AgentParty* pAgentParty, 
        DWORD ToLinkServerIndex, 
        User* user
        );

	void RemoveMemberToSession(
        AgentParty* pAgentParty, 
        DWORD FromLinkServerIndex, 
        DWORD ToLinkServerIndex, 
        User* user
        );

	bool MemberChangeState(
        User* user
        );

    //_NA_20110630_BATTLE_ZONE_RENEWAL
    void  ResetRoomMissionCode(
        WORD party_key
        );

    void CheckLevelForMission(
        WORD party_key, 
        MAPCODE map_code
        );

private:
	util::CObjKeyGenerator<WORD> key_generater_;

    struct InvitePartyKey 
    {
        enum { Response_Limit_Time = 8000, };

        uint request_user_key;
        uint response_user_key;
        uint response_time;
    };
    typedef STLX_HASH_MAP<uint, InvitePartyKey> InvitePartyKeyHashmap;
    InvitePartyKeyHashmap invite_party_key_;

    //_NA_20110630_BATTLE_ZONE_RENEWAL
    PartyRoomFinder* party_room_finder_;
    PartyRoomList* party_room_list_;
    util::Timer check_waiting_room_timer_;
};


