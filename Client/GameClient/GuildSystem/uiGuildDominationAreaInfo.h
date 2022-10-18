#pragma once
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#include "uiBase.h"
#include "TreeMenuGroup.h"
#include "uiDominationZoneMarkDlg/ZoneMarkRewardInfo.h"

class uiGuildDominationAreaInfo : public uiBase
{
    struct CalculatedOption
    {
        DWORD OptionKind;
        int OptionValue;
        int OptionRatio;
        DWORD OptionNC;

        CalculatedOption()
        {
            OptionKind = 0;
            OptionValue = 0;
            OptionRatio = 0;
            OptionNC = 0;
        }
    };

    enum UIControlPos
    {
        kControl_CheckBT_C001 = 0,  // 점령지역보기 탭
        kControl_CheckBT_C004,      // 옵션총합 탭

        kControl_List_L000,         // 리스트 컨트롤
        kControl_VScroll_VS02,      // 스크롤
        kControl_Button_B999,       // 닫기

        kControl_Size,
    };

    enum
    {
        kDominationAreaList = 0,
        kDominationOptionList, 

        kMaxList,
    };

    typedef STLX_MAP<DWORD, CalculatedOption> CalculatedOptionMap;

public:
    static const DWORD kDialog_ID;

public:
    uiGuildDominationAreaInfo(InterfaceManager* ui_manager);
    virtual ~uiGuildDominationAreaInfo();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual void MessageProc(SI_MESSAGE* message);
    virtual void OnShowWindow(BOOL is_show);
    virtual void OnUpdateSolarDialog();

    static void Callback_DominationAreaInfoMenuRender(CCtrlListWZ* list_control, 
        CCtrlListWZ::CListSellData* sell_data, 
        int sell_index, 
        RECT& render_area);
    void RenderDominationAreaRewardInfo(DWORD reward_index);

private:
    bool OnLButtonClick(SI_MESSAGE* message);
    bool OnVScrollUp(SI_MESSAGE* message);
    bool OnVScrollDown(SI_MESSAGE* message);
    bool OnVScrollThumb(SI_MESSAGE* message);

    void InitDominationTree();
    void UpdateRewardOptionList();
    void UpdateMouseWheel();
    void SetCurrentListIndex(DWORD list_index);

    void BuildDominationInfo();
    void RefreshDominationTree();
    void RefreshTab();

    DWORD current_list_index() const { return current_list_index_; }
    TreeMenuGroup& domination_info_menu() { return domination_info_menu_; }
    ZoneMarkRewardInfo& reward_info() { return reward_info_; }
    CalculatedOptionMap& calculated_option_map() { return calculated_option_map_; }

private:
    DWORD current_list_index_;
    TreeMenuGroup domination_info_menu_;    
    ZoneMarkRewardInfo reward_info_;
    CalculatedOptionMap calculated_option_map_;
};

#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL