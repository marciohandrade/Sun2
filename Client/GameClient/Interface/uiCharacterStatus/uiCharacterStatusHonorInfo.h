#pragma once

#ifdef _NA_003027_20111013_HONOR_SYSTEM

#include "uiBase.h"
#include "TreeMenuGroup.h"

class uiCharacterStatusHonorInfo : public uiBase
{
    enum UIControlPos
    {
        kControl_CheckBT_C000 = 0,  // 캐릭터정보창 탭
#ifdef _NA_008472_20150903_IMPROVED_BALANCE
		kControl_CheckBT_C004, 
#endif // _NA_008472_20150903_IMPROVED_BALANCE
        kControl_CheckBT_C001,
        kControl_CheckBT_C002,
        kControl_CheckBT_C003,
        kControl_List_L000,         // 명성/평판 정보리스트
        kControl_VScroll_V000,      // 스크롤

        kControl_Size,
    };
public:
    static const DWORD kDialog_ID;

public:
    uiCharacterStatusHonorInfo(InterfaceManager* interface_manager);
    ~uiCharacterStatusHonorInfo();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual void OnShowWindow(BOOL is_show);
    virtual void MessageProc(SI_MESSAGE* message);

    void InitHonorListInfo();
    void OnChangeHonorInfo(DWORD changed_data);

    // 칭호 리스트 렌더링 함수
    static void Callback_HonorInfoMenuRender(CCtrlListWZ* list_control, 
        CCtrlListWZ::CListSellData* sell_data, 
        int sell_index, 
        RECT& render_area);

private:
    virtual void OnUpdateSolarDialog();

    void RefreshTab();
    
    bool OnLButtonClick(SI_MESSAGE* message);
    void OnClickHonorInfo(BASE_TreeMenuInfo* tree_menu_info);

    TreeMenuGroup& honor_info_menu() { return honor_info_menu_; }

private:
    TreeMenuGroup honor_info_menu_;

};

#endif //_NA_003027_20111013_HONOR_SYSTEM
