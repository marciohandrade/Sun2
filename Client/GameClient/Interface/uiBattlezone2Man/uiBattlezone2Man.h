#pragma once

#ifdef _DH_BATTLEZONE2_
#include "uiBaseMan.h"
#include "UserSlotSet.h"
#include "UserSlot.h"

#define MAX_VIEW_USER_SLOT 10
#define BASE_ROOM_KEY 15000

class uiBattlezone2List;
class uiBattlezone2Tree;
class uiBattlezone2Lobby;
class uiBattlezone2Reservation;
class uiBattlezone2Icon;
class uiBattlezone2Password;

struct IM_BATTLEZONE2_MANAGER
{
    static const DWORD BATTLEZONE2_LIST;
    static const DWORD BATTLEZONE2_TREE;
    static const DWORD BATTLEZONE2_LOBBY;
    static const DWORD BATTLEZONE2_ICON;
    static const DWORD BATTLEZONE2_RESERVATION;   
    static const DWORD BATTLEZONE2_PASSWORD;
};

typedef union _TREE_INDEX
{
    BYTE menu_depth[4];
    DWORD menu_index;
}BATTLEZONE2_TREE;

class uiBattlezone2Man : public uiBaseMan
{
public:
    enum
    {
        ePKBOOL_BATTLEZONE_BASE = 0,
        ePKBOOL_BATTLEZONE_LIST_CG_ZONE_SHORTCUT_SYN,
        ePKBOOL_BATTLEZONE_LIST_CG_ZONE_FIND_ROOM_FROM_CHARID_SYN,
        ePKBOOL_BATTLEZONE_LIST_CG_ZONE_LIST_MISSIONLOBBY_SYN,
        ePKBOOL_BATTLEZONE_LIST_CG_ZONE_LIST_PVPLOBBY_SYN,
        ePKBOOL_BATTLEZONE_LIST_CG_ZONE_LIST_CHUNTINGLOBBY_SYN,
        ePKBOOL_BATTLEZONE_LIST_CG_ZONE_LIST_CHUNTING_SYN,
        ePKBOOL_BATTLEZONE_CREATE_CG_ZONE_LOBBY_CREATE_SYN,
        ePKBOOL_BATTLEZONE_CG_ZONE_LOBBY_CHANGE_MAP_SYN,
        ePKBOOL_BATTLEZONE_CG_ZONE_LOBBY_CHUNTING_CONFIG_SYN,
        ePKBOOL_BATTLEZONE_CG_ZONE_LOBBY_PVP_CONFIG_SYN,

        PACKET_STATUS_NON,
        PACKET_STATUS_SEND_BAN,				// 강퇴 패킷
        PACKET_STATUS_SEND_LEAVE,			// 나가기 패킷
        PACKET_STATUS_SEND_READY,			// 준비 패킷
        PACKET_STATUS_SEND_CREATE,			// 미션/헌팅 만들기
        PACKET_STATUS_SEND_PLAYER_NUM,		// 유저 인원수 체크
        PACKET_STATUS_SEND_READY_CANCLE,	// 준비 취소 패킷
        PACKET_STATUS_SEND_CHANGE_TEAM,		// 팀 교체
        PACKET_STATUS_SEND_REQUEST_MEMBER, // 멤버 정보 재요청
        PACKET_STATUS_SEND_REQUEST_RENDER, // 멤버 랜더 정보 재요청
#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
        PACKET_STATUS_SEND_DISCONNECTED_USER_BAN,  // 비정상 접속종료자 강퇴 패킷
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS

        ePKBOOL_BATTLEZONE_MAX,
    };

    uiBattlezone2Man(InterfaceManager *pUIMan);

    //--------------------------------------------------------------------------
    // init/release
    //
public:
    enum ROOM_STATE
    {
        ROOM_STATE_LIST = 0,//리스트, 일반
        ROOM_STATE_LOBBY,
        ROOM_STATE_MISSION,
        ROOM_STATE_MAX,
    };

    enum BATTLEZONE_DIALOG
    {
        BATTLEZONE2_DIALOG_LIST = 0,
        BATTLEZONE2_DIALOG_TREE,
        BATTLEZONE2_DIALOG_LOBBY,
        BATTLEZONE2_DIALOG_RESERVATION,
        BATTLEZONE2_DIALOG_ICON,
        BATTLEZONE2_DIALOG_MAX,
    };
    virtual void OnRelease();
    virtual void Update();
    virtual void OnInitialize();
    void Clear();
    virtual void NetworkProc(MSG_BASE* message_ptr);

    bool AddPlayer(UserSlot& enter_user_slot);
    UserSlot* FindPlayer(DWORD object_key);
    BYTE battlezone_room_state(){ return battlezone_room_state_; }
    void set_battlezone_room_state(BYTE state);
    bool is_wait(){ return is_wait_; }
    void set_is_wait(bool val){ is_wait_ = val; }
    void ShowBattlezoneDialog(DWORD type, bool is_show);
    void SetLobbyMasterPlayerKey(DWORD master_key)	{ master_player_key_ = master_key; }
    DWORD master_player_key(){ return master_player_key_; }
    UserSlot* GetUserSlotByIndex(int index);
    int GetNonBlockUserSlotNum();
    int GetBlockUserSlotNum();
    int	GetMaxUserSlot();
    int	GetUserSlotNum();
    bool IsMaster() const;
    bool LeavePlayer(DWORD player_key);
    void set_current_room_info(PartyRoomInfo* room_info);
    void set_current_room_max_user_num(BYTE number);
    PartyRoomInfo* current_room_info(){ return &current_room_info_; }
    void SetCancleAllReady();
    void SetShareType(BYTE share_type);

    void InitializeUserSlotSet();
    void ReleaseUserSlotSet();
    void FlushUserSlotSet();
    void SetReadyUserSlot(DWORD object_key, bool state);
    bool IsReadyForStart();
    bool StartBattle();
    void ModifyPlayerNumber(int player_numbers);
    void SetNonBlockSlot(int nonblock_slot);
    void SetBlockUserSlot();
    void Send_CG_PARTY_ROOM_FORCE_LEAVE_SYN();
    //void SaveSlotObject();
    void SwapUserSlot(DWORD dest_key, DWORD src_key);
    void StateClear();

    static void ToggleBattlezoneDialog();

    void DeleteDisConnectedUser(char* character_name);

    // 맵이동 및 ObjectManager가 클리어 되었을때 호출
    void ClearObjectKey();
    void MoveBattleZoneIcon();  // 아이콘 위치를 미니맵 옆으로 조정

private:
    void ToggleDialog();
    void InitPKBooleans();

    uiBattlezone2List* ui_battlezone2_list_;
    uiBattlezone2Tree* ui_battlezone2_tree_;
    uiBattlezone2Lobby* ui_battlezone2_lobby_;
    uiBattlezone2Reservation* ui_battlezone2_reservation_;
#ifdef _NA_000000_20130114_RENEWER_UI
#else
    uiBattlezone2Icon* ui_battlezone2_icon_;
#endif   
	uiBattlezone2Password* ui_battlezone2_password_;
    UserSlotSet user_slot_set_;
    PartyRoomInfo current_room_info_;
    BYTE battlezone_room_state_;
    DWORD master_player_key_;
    BYTE pending_player_number_;
    Object player_objects_[MAX_VIEW_USER_SLOT];
    bool is_wait_;
};

inline void uiBattlezone2Man::set_battlezone_room_state(BYTE state)
{ 
    battlezone_room_state_ = state; 
    is_wait_ = false; 
}
#endif//_DH_BATTLEZONE2_