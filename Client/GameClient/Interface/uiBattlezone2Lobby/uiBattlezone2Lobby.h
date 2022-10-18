#pragma once
#ifdef _DH_BATTLEZONE2_
#include "uiBase.h"
#include "DropdownControl.h"

class uiBattlezone2Man;
class uiBattlezone2Lobby : public uiBase
{
    enum UseTexture
    {
        USE_TEXTURE_READY = 0,
        MAX_USE_TEXTURE,
    };

    enum UIControlPos
    {
        kControl_Button_B004 = 0,
        kControl_Button_B000,
        kControl_Button_B006,
        kControl_Button_B010,
        kControl_Button_B001,// user_slot_1
        kControl_Button_B025,
        kControl_Button_B026,
        kControl_Button_B027,
        kControl_Button_B028,
        kControl_Button_B029,
        kControl_Button_B030,
        kControl_Button_B031,
        kControl_Button_B032,
        kControl_Button_B033,// user_slot_10
        kControl_Button_B002,// ban_1
        kControl_Button_B034,
        kControl_Button_B035,
        kControl_Button_B036,
        kControl_Button_B037,
        kControl_Button_B038,
        kControl_Button_B039,
        kControl_Button_B040,
        kControl_Button_B041,
        kControl_Button_B042,// ban_10
        kControl_Button_B003,
        kControl_Button_B005,
        kControl_Text_T007,
        kControl_Text_S001,
        kControl_Text_S000,
        kControl_Text_T001,
        kControl_Text_T002,
        kControl_Text_T003,
        kControl_Text_T008,
        kControl_Text_T010,
        kControl_Text_T009,
        kControl_Text_T011,
        kControl_Text_T004,
        kControl_Text_T201,//level_1        
        kControl_Text_T202,
        kControl_Text_T203,
        kControl_Text_T204,
        kControl_Text_T205,
        kControl_Text_T206,
        kControl_Text_T207,
        kControl_Text_T208,
        kControl_Text_T209,
        kControl_Text_T210,//level_10
        kControl_Text_T101,//name_1
        kControl_Text_T102,
        kControl_Text_T103,
        kControl_Text_T104,
        kControl_Text_T105,
        kControl_Text_T106,
        kControl_Text_T107,
        kControl_Text_T108,
        kControl_Text_T109,
        kControl_Text_T110,//name_10
        //kControl_Text_S002,
        kControl_Button_B007,
        kControl_List_L000,
        kControl_Picture_P003,
        kControl_Picture_P002,
        kControl_Picture_P000,
        kControl_Picture_P001,
        kControl_Picture_P020,
        kControl_Picture_P006,
        kControl_Picture_P021,
        kControl_Picture_P022,
        kControl_Picture_P023,
        kControl_Picture_P024,
        kControl_Picture_P025,
        kControl_Picture_P026,
        kControl_Picture_P027,
        kControl_Picture_P028,
        kControl_Picture_P004,
        kControl_Picture_P007,
        kControl_Picture_P008,
        kControl_Picture_P009,
        kControl_Picture_P010,
        kControl_Picture_P011,
        kControl_Picture_P012,
        kControl_Picture_P013,
        kControl_Picture_P014,
        kControl_Picture_P015,
        kControl_Picture_P005,
        kControl_Picture_P016,
        kControl_Picture_P017,
        kControl_Picture_P018,
        kControl_Picture_P019,
        kControl_Picture_P029,
        kControl_Picture_P030,
        kControl_CheckBT_C001,
        kControl_CheckBT_C002,
        kControl_CheckBT_C003,
        kControl_CheckBT_C004,
        kControl_CheckBT_C005,
        kControl_CheckBT_C006,
        kControl_CheckBT_C007,
        kControl_CheckBT_C008,
        kControl_CheckBT_C009,
        kControl_Picture_P031,
        kControl_Picture_P041,
        kControl_Picture_P042,
        kControl_Picture_P043,
        kControl_Picture_P044,
        kControl_Picture_P045,
        kControl_Picture_P046,
        kControl_Picture_P047,
        kControl_Picture_P048,
        kControl_Picture_P049,

        kControl_Picture_P032,//highlight 1
        kControl_Picture_P033,
        kControl_Picture_P034,
        kControl_Picture_P035,
        kControl_Picture_P036,
        kControl_Picture_P037,
        kControl_Picture_P038,
        kControl_Picture_P039,
        kControl_Picture_P040,
        kControl_Picture_P050,//highlight 10
                             
        kControl_Picture_P051,//master icon 1
        kControl_Picture_P052,
        kControl_Picture_P053,
        kControl_Picture_P054,
        kControl_Picture_P055,
        kControl_Picture_P056,
        kControl_Picture_P057,
        kControl_Picture_P058,
        kControl_Picture_P059,
        kControl_Picture_P060,//master icon 10

        kControl_Size,
    };

public:
    uiBattlezone2Lobby(InterfaceManager* ui_manager);
    virtual ~uiBattlezone2Lobby(void);

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual void OnUpdateSolarDialog();
    virtual void OnRenderSolarDialog();
    virtual void MessageProc(SI_MESSAGE* message);
    virtual void MouseOver_MsgProc(SI_MESSAGE* message);
    virtual void OnShowWindow(BOOL is_show);
    virtual void Process(DWORD tick_count);
    virtual void NetworkProc(MSG_BASE* message_ptr);
    void Clear();
    bool SendGameReady();
    bool SendGameStart();
    void SetBaseInfo(PartyRoomInfo* base_mission_infomation);
    bool SendGameExit();
    void SetShareType(BYTE share_type);
    DWORD kick_player_key(){ return kick_player_key_; }
    void Send_CG_PARTY_ROOM_FORCE_LEAVE_SYN();
    void SetTeam(DWORD member_key, BYTE team);
    uiBattlezone2Man* GetManager();
    BYTE clicked_slot_index(){ return clicked_slot_index_; }
    void ResetLobby();
    void ChangeMaster(DWORD master_key);
    void set_is_ready(bool val){ is_ready_ = val; }
    void Unlocks();

    DropdownControl& dropdown_item_share() { return dropdown_item_share_; }

private:
    bool SendRequestMember();
    bool SendRequestRenderInfo(WORD party_key, DWORD party_member_key);
    void RegisterControl();
    bool OnLButtonClick(SI_MESSAGE* message);
    bool OnRButtonClick(SI_MESSAGE* message);
    bool OnMouseOver(SI_MESSAGE* message);
    void SetControlByPlayerMove(BYTE index, bool is_enter);
    bool BlockPlayer(int index);
    void ChangeSetting();
    void SettingForMaster();
    void SettingForMember();
    void SetBattleRoom(PartyRoomInfo* recv_packet);
    bool BanPlayerByIndex(int index);
    void LeaveHero();
    bool SendChangeTeam();
    void WaitGague(bool is_show);

    uiBattlezone2Man* ui_battlezone2_manager_;
    CCtrlButtonCheckWZ* control_close_slots_[MAX_VIEW_USER_SLOT];
    CCtrlButtonWZ* control_ban_[MAX_VIEW_USER_SLOT];
    CCtrlButtonWZ* control_user_slots_[MAX_VIEW_USER_SLOT];
    CCtrlStaticWZ* control_user_levels_[MAX_VIEW_USER_SLOT];
    CCtrlStaticWZ* control_user_names_[MAX_VIEW_USER_SLOT];
    CCtrlButtonWZ* control_ready_start_;
    CCtrlPictureWZ* control_playing_user_[MAX_VIEW_USER_SLOT];
    CCtrlPictureWZ* control_highlight_user_[MAX_VIEW_USER_SLOT];
    CCtrlPictureWZ* control_master_mark_[MAX_VIEW_USER_SLOT];

    DropdownControl dropdown_item_share_;

    bool is_ready_;
    bool is_ready_button_enable_;
    BYTE is_open_slot_count_;
    DWORD kick_player_key_;
    BYTE clicked_slot_index_;

    HANDLE handle_texture_[MAX_USE_TEXTURE];
    St_TextureContainer* texture_information_[MAX_USE_TEXTURE];
    CTimerSimple wait_timer_;
    DWORD map_code_;

#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
    bool is_disconnected_uesr_kick_;
    char kick_player_name_[MAX_CHARNAME_LENGTH+1];
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
#ifdef _NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
    bool is_party_obtain_room_;
#endif //_NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN
};
#endif//_DEV_VER //_DH_GM_ITEM_WINDOW