#pragma once
#include "uiBase.h"
#include "ContinentMapDlg.h"

class uiDominationBettingList : public uiBase
{
    enum UIControlPos
    {
        kControl_CheckBT_C000, // 점령전 신청 탭
        kControl_CheckBT_C001, // 출전 신청 탭
        kControl_Button_B001, // 성문관리 버튼
        kControl_Button_B999, // 닫기 버튼
        kControl_List_L000, // 입찰 리스트
        kControl_VScroll_V002, // 리스트 스크롤
        kControl_Text_S000, // 길드명 텍스트
        kControl_Text_S001, // 입찰금액 텍스트

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
#else
        kControl_Button_BT00, // 출전 신청 버튼
        kControl_Button_BT01, // 입찰하기 버튼
#endif //_NA_006826_20130722_DOMINATION_RENEWAL

        kControl_Size,
    };

public:
    uiDominationBettingList(InterfaceManager* ui_manager);
    virtual ~uiDominationBettingList();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual void MessageProc(SI_MESSAGE * message);

    void SetShowButtonFlag(MSG_CG_DOMINATION_NOTICE_ACK* recv_packet);
    void SetBettingListData(MSG_CG_DOMINATION_APPLY_ACK* recv_packet);
#ifdef _TEMP_0068_20130820_DOMINATION_MANAGE_PACKET_CONTROL
    void SetManageListData(MSG_CG_DOMINATION_MANAGE_ACK* recv_packet);
#endif //_TEMP_0068_20130820_DOMINATION_MANAGE_PACKET_CONTROL

#ifdef _DEV_VER //_DH_FULL_MAP_PORTAL
    void GetMapPortalFromControl(int continent_map_number, int continent_positon);
#endif //_DEV_VER //_DH_FULL_MAP_PORTAL

    static void CallBackESCKey(DWORD wParam,DWORD lParam);

protected:
    virtual void OnShowWindow(BOOL val);

private:
    void RegisterControl();
    bool OnLButtonClick(SI_MESSAGE* message);
    bool OnLButtonDClick(SI_MESSAGE* message);

    void OnClickBettingListTab();
    void OnClickJoinListTab();
    void OnClickApplyButton();
    void OnClickGateManageButton();
    void OnClickCloseButton();

    void OnDClickList();

#ifdef _DEV_VER //_DH_FULL_MAP_PORTAL
    bool OnLButtonClickForPortal(SI_MESSAGE* message);
    void DoubleClickMapPortal();
#endif //_DEV_VER //_DH_FULL_MAP_PORTAL

private:
#ifdef _DEV_VER //_DH_FULL_MAP_PORTAL
    bool is_portal_mode_;
    MAPCODE map_portal_infomations_[kContinentMapPortalMax];
#endif //_DEV_VER //_DH_FULL_MAP_PORTAL
};

