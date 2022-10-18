#pragma once
#include "uiBase.h"
#include "ContinentMapDlg.h"

class uiDominationBettingList : public uiBase
{
    enum UIControlPos
    {
        kControl_CheckBT_C000, // ������ ��û ��
        kControl_CheckBT_C001, // ���� ��û ��
        kControl_Button_B001, // �������� ��ư
        kControl_Button_B999, // �ݱ� ��ư
        kControl_List_L000, // ���� ����Ʈ
        kControl_VScroll_V002, // ����Ʈ ��ũ��
        kControl_Text_S000, // ���� �ؽ�Ʈ
        kControl_Text_S001, // �����ݾ� �ؽ�Ʈ

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
#else
        kControl_Button_BT00, // ���� ��û ��ư
        kControl_Button_BT01, // �����ϱ� ��ư
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

