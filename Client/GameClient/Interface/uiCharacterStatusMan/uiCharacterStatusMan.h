#pragma once
#include "uiBaseMan.h"
#ifdef _NA_003027_20111013_HONOR_SYSTEM
#include "TabCheckButtonControlManager.h"
#endif //_NA_003027_20111013_HONOR_SYSTEM

class uiCharaterAccumulatePointDialog;

struct IM_CHARACTERSTATUS_MANAGER 
{
	static const DWORD CHARACTER_STATUS_DLG;
    static const DWORD CHARACTER_ACCUMULATEPOINT_DLG;
};

class uiCharacterStatusMan : public uiBaseMan
{
    enum ePacketPending
    {
        PKBOOL_CHARACTER_STATUS_BASE = 0,
        PKBOOL_CHARACTER_STATUS_USE_POINT_SYN,
        PKBOOL_CHARACTER_STATUS_ACCEPT_REWARD_SYN,        
        PKBOOL_CHARACTER_STATUS_MAX,
    };
    const static DWORD TIMEOUT_PACKET_PENDING = 3;

public:

#ifdef _NA_003027_20111013_HONOR_SYSTEM
    enum TabDialog
    {
        kStatus_Info, 
#ifdef _NA_008472_20150903_IMPROVED_BALANCE
		kStatus_Info2, 
#endif // _NA_008472_20150903_IMPROVED_BALANCE
        kSokect_Info,
        kHonor_Info,
#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
        kCurrency_Info,
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM

        kTab_Max,
    };
#endif //_NA_003027_20111013_HONOR_SYSTEM

public:
	uiCharacterStatusMan(InterfaceManager *pUIMan);

    //--------------------------------------------------------------------------
    // init/release
    //
public:
    virtual void            OnInitialize();
    
    void Clear();
    void set_accumulate_time(DWORD accumulate_time)  {   accumulate_time_ = accumulate_time; }
    DWORD accumulate_time()  {   return accumulate_time_; }
    void set_accumulate_point(DWORD accumulate_point)  {   accumulate_point_ = accumulate_point; }
    DWORD accumulate_point()  {   return accumulate_point_; }
    DWORD require_point()  {   return require_point_; }

    static void ShowGainAccumulatePoint(DWORD new_point);

    // 패킷관련 함수
    virtual void NetworkProc(MSG_BASE * pMsg);

    virtual void InitPKBooleans();
    void RECV_MSG_CG_CHARINFO_ACCUMULATE_POINT_CMD(MSG_CG_CHARINFO_ACCUMULATE_POINT_CMD* recv_msg);
    void SEND_MSG_CG_ITEM_QUERY_ACCUMULATE_POINT_ITEM_SYN(bool is_first);
    void SEND_MSG_CG_ITEM_USE_ACCUMULATE_POINT_SYN();

    static void ShowCharacterStatusDialog(bool is_show);
    static void ToggleCharacterStatusDialog();
    static PlayerAttributes* GetSimulationPlayerAttributes();
    static void RefreshSimulationPlayerAttributes();
    static void ResetSimulationStatCount();
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    TabCheckButtonControllManager& status_dialog_tab_manager() { return status_dialog_tab_manager_; }
#endif //_NA_003027_20111013_HONOR_SYSTEM

private:

    void ProcessErrorCode(DWORD error_code);

private:
    uiCharaterAccumulatePointDialog* accumulate_point_dialog_;

    DWORD accumulate_time_;
    DWORD accumulate_point_;
    DWORD require_point_;

#ifdef _NA_003027_20111013_HONOR_SYSTEM
    TabCheckButtonControllManager status_dialog_tab_manager_;    
#endif //_NA_003027_20111013_HONOR_SYSTEM    
};
