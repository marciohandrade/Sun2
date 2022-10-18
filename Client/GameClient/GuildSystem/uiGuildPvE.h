#pragma once
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#ifdef _NA_008334_20150608_SONNENSCHEIN

#include "uiBase.h"
#include "VScrollDialog.h"
#include "TreeMenuGroup.h"
//#include "FTextRender.h"
#include "GuildSystem/GuildSystemData.h"

#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
  #include "uiBase.h"
#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION

using namespace GuildSystem;

class MultiVScrollDialog;
class uiGuildPvE : public uiBase, public VScrollDialog
{
    typedef union
    {
        BYTE menu_depth[4];
        DWORD menu_index;
    } TREE_INDEX;

    enum UIControlPos
    {
        kControl_CheckBT_C000 = 0,
        kControl_CheckBT_C001,
        kControl_CheckBT_C003,
        kControl_CheckBT_C004,
        kControl_CheckBT_C008,
        kControl_CheckBT_C002,

        kControl_Button_B000,
        kControl_Button_B001,
        kControl_Button_B002,
        kControl_Button_B004,
        kControl_Button_B009,

        kControl_List_L000,
        //kControl_List_L001,
        kControl_List_L002,

        kControl_VScroll_V000,
        //kControl_VScroll_V001,
        kControl_VScroll_V002,

        kControl_Static_S003,

        kControl_Size,
    };

public:
    static const DWORD kDialog_ID;

public:
    uiGuildPvE(InterfaceManager* ui_manager);
    virtual ~uiGuildPvE();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();

    virtual void MessageProc(SI_MESSAGE* message);
    virtual void NetworkProc(MSG_BASE* recv_packet);

    virtual void OnShowWindow(BOOL is_show);
    virtual void OnUpdateSolarDialog();
    virtual void OnRenderSolarDialog();
    virtual void Process(DWORD tick_count);
   
    void WaitGague(bool is_show, int second = 5);
    void SetGuildMemberList(MSG_BASE* recv_message);
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
    void SetGuildMemberList_OneDaysGuildMission(MSG_BASE* recv_message);
#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION

protected:
    CCtrlVScrollWZ* vscroll_control();
    bool GetScrollArea(RECT& area);
    bool OnVScrollThumb(SI_MESSAGE* msg);   // 미션트리 스크롤
    bool OnVScrollUp(SI_MESSAGE* msg);
    bool OnVScrollDown(SI_MESSAGE* msg);
/*
    bool OnVScrollThumb_MissionInfo(SI_MESSAGE* msg);   // 미션내용 스크롤
    bool OnVScrollUp_MissionInfo(SI_MESSAGE* msg);
    bool OnVScrollDown_MissionInfo(SI_MESSAGE* msg);*/


    //MultiVScrollDialog* mission_info_vscroll(){ return mission_info_vscroll_; }
    MultiVScrollDialog* member_vscroll(){ return member_vscroll_; }
    //void set_mission_info_vscroll(MultiVScrollDialog* vscoll_control){ mission_info_vscroll_ = vscoll_control; }
    void set_member_vscroll(MultiVScrollDialog* vscoll_control){ member_vscroll_ = vscoll_control; }

    
private:
    bool OnLButtonClick(SI_MESSAGE* message);
    bool OnRButtonClick(SI_MESSAGE* message);
    bool OnMouseOver(SI_MESSAGE* message);

    TreeMenuGroup& tree_menu() { return tree_menu_; }
    void InitMissionTree();
    void MakeGuildPvETree(BYTE depth_3, BYTE depth_2, BYTE depth, DWORD string_code, TCHAR* string, const DWORD* font_colors, MAPCODE map_code);
    bool SelectIndex(TreeMenuGroup::TreeObject* tree_object);


    void RefreshTab();
    SLOTCODE GetRequireItem(const MAPCODE map_code);
    //void UpdateGuildMissionInfo(MAPCODE map_code);
    void UpdateGuildMemberList();
    void RequestGuildMemberList();
    void SortGuildMember(GuildSystemData::GuildMemberSortType sort_type, bool force_high_case = false);
    void RenderTooltip();

#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
    void ClickStart();
#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION

    bool init_mission_tree_;
    TreeMenuGroup tree_menu_;    
    TREE_INDEX select_tree_;
    //MultiVScrollDialog* mission_info_vscroll_;
    MultiVScrollDialog* member_vscroll_;
    //FTextRender	ftext_render_;
    GuildSystemData::GuildMemberContainer member_container_;


    CTimerSimple wait_timer_; // 로딩 전 캐스팅 타이머
    int wait_second_;

#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
    ChaosZoneType select_zone_type_;
    STLX_VECTOR<CHARGUID> disconnect_member_list_;
    DWORD onedays_mission_end_time_;
    BYTE onedays_mission_event_state_;
    CTimerSimple list_refresh_timer_;       // 시간정보 재요청 타이머
#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION

};

#endif //_NA_008334_20150608_SONNENSCHEIN
#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
