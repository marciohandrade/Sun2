#pragma once
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#include "uiBase.h"
#include "VScrollDialog.h"
class MultiVScrollDialog;
class uiGuildLog: public uiBase, public VScrollDialog
{
    enum UIControlPos
    {
        kControl_CheckBT_C000 = 0,
        kControl_CheckBT_C001,
        kControl_CheckBT_C003,
        kControl_CheckBT_C004,
#ifdef _NA_008334_20150608_SONNENSCHEIN
        kControl_CheckBT_C011,
#endif //_NA_008334_20150608_SONNENSCHEIN
        kControl_CheckBT_C002,

        kControl_CheckBT_C005,  // 전체로그
        kControl_CheckBT_C007,  // 성장로그
        kControl_CheckBT_C008,  // 길드원로그
        kControl_CheckBT_C009,  // 관계로그
        kControl_CheckBT_C006,  // 점령전로그
        
        kControl_CheckBT_C012,  // 창고 입고탭
        kControl_CheckBT_C010,  // 창고 출고탭
        
        kControl_List_L000,
        kControl_List_L001,
        kControl_VScroll_V000,
        kControl_VScroll_V001,
        kControl_Size,
    };

    enum
    {
        GUILD_LOG_UP_LIST_TIME,
        GUILD_LOG_UP_LIST_DISCRIPTION,
        GUILD_LOG_UP_LIST_MAX,
    };

    enum GuildLogInfoType
    {
        kAllLog = 0, 
        kGrowthLog, 
        kMemberLog, 
        kRelationLog, 
        kDominationLog, 

        kMaxGuildLog, 
    };

    enum WareHouseLogType
    {
        kDepositLog = 0,
        kWithdrawLog,

        kMaxWareHouseLog,
    };

    enum 
    {
        kGuildLogRefreshTime = (1000 * 10), // 10초마다 길드로그 갱신
    };

public:
    static const DWORD kDialog_ID;

public:
    uiGuildLog(InterfaceManager* ui_manager);
    virtual ~uiGuildLog();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();

    virtual void MessageProc(SI_MESSAGE* message);
    virtual void NetworkProc(MSG_BASE* recv_packet);

    virtual void OnShowWindow(BOOL is_show);
    virtual void OnUpdateSolarDialog();
    virtual void Process(DWORD dwTick);

    void SetGuildLogUpList();
    void UpdateGuildWareHouseLog();

protected:
    CCtrlVScrollWZ* vscroll_control();
    bool GetScrollArea(RECT& area);
    bool OnVScrollThumb(SI_MESSAGE* msg);
    bool OnVScrollUp(SI_MESSAGE* msg);
    bool OnVScrollDown(SI_MESSAGE* msg);

    CCtrlVScrollWZ* warehouse_vscroll_control();
    bool GetWareHouseScrollArea(RECT& area);
    bool OnWareHouseVScrollThumb(SI_MESSAGE* msg);
    bool WareHouseVScrollUp(SI_MESSAGE* msg);
    bool WareHouseVScrollDown(SI_MESSAGE* msg);
    bool OnWareHouseVScrollUp(SI_MESSAGE* msg);
    bool OnWareHouseVScrollDown(SI_MESSAGE* msg);
    void WareHouseVScrollUpdate();
    bool WareHouseVScrollThumb(SI_MESSAGE* msg);

    MultiVScrollDialog* ware_house_vscroll(){ return ware_house_vscroll_; }
    void set_ware_house_vscroll(MultiVScrollDialog* multi_vscroll){ ware_house_vscroll_ = multi_vscroll; }
    
private:
    bool OnLButtonClick(SI_MESSAGE* message);
    bool OnRButtonClick(SI_MESSAGE* message);
    bool OnMouseOver(SI_MESSAGE* message);

    void RefreshTab();
    void InitGuildInfoLog();
    void InitGuildWareHouseLog();    
    void GetGuildLogInfoString(GuildInGameLogData& IN log_info, 
                               TCHAR* OUT info_string, 
                               int size_buffer);
    DWORD GetGuildLogColor(BYTE log_type);
    void GetWareHouseLogInfoString(STRUCT_GUILDWAREHOUSE_LOGINFO& IN log_info, 
                                   TCHAR* OUT info_string, 
                                   int size_buffer);
    void GetLogDateString(INT64 log_time, 
                          TCHAR* OUT info_string, 
                          int size_buffer);

private:
    WareHouseLogType warehouse_log_type() const { return warehouse_log_type_; }
    void set_warehouse_log_type(WareHouseLogType value) { warehouse_log_type_ = value; }
    GuildLogInfoType guild_info_log_type() const { return guild_info_log_type_; }
    void set_guild_info_log_type(GuildLogInfoType value) { guild_info_log_type_ = value; }
    DWORD refresh_wait_time() const { return refresh_wait_time_; }
    void set_refresh_wait_time(DWORD value) { refresh_wait_time_ = value; }

    MultiVScrollDialog* ware_house_vscroll_;
    CCtrlListWZ* list_control_;

    WareHouseLogType warehouse_log_type_;
    GuildLogInfoType guild_info_log_type_;

    DWORD refresh_wait_time_;
};

#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
