#pragma once

#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM

#include "uiBase.h"
#include "TreeMenuGroup.h"
#include "CurrencyInfoParser.h"

class uiCharacterStatusCurrencyInfo : public uiBase
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
        kControl_List_L000,         // 화폐/수지포인트 정보리스트
        kControl_VScroll_V000,      // 스크롤

        kControl_Size,
    };

public:
    struct CurrencyPointInfo
    {
        DWORD currency_code;
        DWORD currency_point;
        CurrencyInfoParser::CurrencyInfo* currency_info;
        HANDLE currency_icon_texture;
    };

    static const DWORD kDialog_ID;

private:
    typedef STLX_MAP<DWORD, CurrencyPointInfo> CurrencyPointInfoContainer;
    typedef STLX_MAP<DWORD, HANDLE> CurrencyIconTextureContainer;

public:
    uiCharacterStatusCurrencyInfo(InterfaceManager* interface_manager);
    ~uiCharacterStatusCurrencyInfo();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual void OnShowWindow(BOOL is_show);
    virtual void MessageProc(SI_MESSAGE* message);
    virtual void NetworkProc(MSG_BASE* recv_packet);
	virtual void OnUpdateSolarDialog();

    void InitCurrencyInfo();

    CurrencyPointInfo* GetCurrencyPointInfo(DWORD currency_code);

        // 칭호 리스트 렌더링 함수
    static void Callback_CurrencyInfoMenuRender(CCtrlListWZ* list_control, 
        CCtrlListWZ::CListSellData* sell_data, 
        int sell_index, 
        RECT& render_area);

private:
    virtual void update();

    void RefreshTab();
    
    bool OnLButtonClick(SI_MESSAGE* message);

    void BuildCurrencyPointInfo();
    void RefreshCurrencyInfoMenu();
    void LoadCurrencyIconTexture();

    TreeMenuGroup& currency_info_menu() { return currency_info_menu_; }

    CurrencyPointInfoContainer& currency_info_container() { return currency_info_container_; }

    CurrencyIconTextureContainer& currency_icon_texture_container() { return currency_icon_texture_container_; }

private:
    TreeMenuGroup currency_info_menu_;

    CurrencyPointInfoContainer currency_info_container_;

    CurrencyIconTextureContainer currency_icon_texture_container_;
};

#endif //_NA004034_20120102_POINT_WALLET_SYSTEM
