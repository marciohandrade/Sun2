#pragma once

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
#else

#include "uiBase.h"

class uiDominationJoinList1 : public uiBase
{
    enum UIControlPos
    {
        kControl_Static_T999, // Ÿ��Ʋ
        kControl_Button_B031, // ���ΰ�ħ ��ư
        kControl_Button_B999, // �ݱ� ��ư

        kControl_Button_BT01, // �Ʊ� Ŭ���� ��ư
        kControl_Button_B001, // �Ʊ� ���� ��ư
        kControl_Button_B003, // �Ʊ� ĳ���͸� ��ư

        kControl_Button_B000, // ���� Ŭ���� ��ư
        kControl_Button_B002, // ���� ���� ��ư
        kControl_Button_B004, // ���� ĳ���͸� ��ư

        kControl_Friend_Class_Image_Start, // �Ʊ� Ŭ���� ����Ʈ1
        kControl_Friend_Class_Image_End  = kControl_Friend_Class_Image_Start + (MAX_DOMINATION_MEMBER - 1),   // �Ʊ� Ŭ���� ����Ʈ20

        kControl_Friend_Level_Start, // �Ʊ� ���� ����Ʈ1
        kControl_Friend_Level_End = kControl_Friend_Level_Start + (MAX_DOMINATION_MEMBER - 1),   // �Ʊ� ���� ����Ʈ20

        kControl_Friend_Member_Name_Start, // �Ʊ� ���� ĳ���� �̸� ����Ʈ1
        kControl_Friend_Member_Name_End = kControl_Friend_Member_Name_Start + (MAX_DOMINATION_MEMBER - 1),   // �Ʊ� ���� ĳ���� �̸� ����Ʈ20

        kControl_Enemy_Class_Image_Start, // ���� Ŭ���� ����Ʈ1
        kControl_Enemy_Class_Image_End = kControl_Enemy_Class_Image_Start + (MAX_DOMINATION_MEMBER - 1),   // ���� Ŭ���� ����Ʈ20

        kControl_Enemy_Level_Start, // ���� ���� ����Ʈ1
        kControl_Enemy_Level_End = kControl_Enemy_Level_Start + (MAX_DOMINATION_MEMBER - 1),   // ���� ���� ����Ʈ20

        kControl_Enemy_Member_Name_Start, // ���� ���� ĳ���� �̸� ����Ʈ1
        kControl_Enemy_Member_Name_End = kControl_Enemy_Member_Name_Start + (MAX_DOMINATION_MEMBER - 1),   // ���� ���� ĳ���� �̸� ����Ʈ20

        kControl_Member_Leave_Button_Start, // ���� ��� ��ư1
        kControl_Member_Leave_Button_End = kControl_Member_Leave_Button_Start + (MAX_DOMINATION_MEMBER - 1),   // ���� ��� ��ư20

        kControl_Size,
    };

public:
    uiDominationJoinList1(InterfaceManager* ui_manager);
    virtual ~uiDominationJoinList1();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual void MessageProc(SI_MESSAGE * message);

    void SetDominationJoinList(MSG_CG_DOMINATION_MEMBERLIST_NTF* recv_packet);

    DominationMemberInfo* GetDominationMemberInfo(CHARGUID char_guid);

protected:
    virtual void OnShowWindow(BOOL val);

private:
    void RegisterControl();
    bool OnLButtonClick(SI_MESSAGE* message);
    bool OnLButtonDClick(SI_MESSAGE* message);

    void OnClickMemberLeaveButton(DWORD control_position);
    void OnClickRefreshListButton();
    void OnClickCloseButton();

    void SetDialogTitle();
    void SortMemberList();
    void SetMemberList();
    void SetClassImage(DWORD control_position, BYTE member_class);
    void SetMemberLevel(DWORD control_position, LEVELTYPE member_level);
    void SetMemberName(DWORD control_position, TCHAR* member_name);

    void ShowControl(DWORD control_position, bool is_show);

private:
    std::vector<DominationMemberInfo> friend_member_list_;
    std::vector<DominationMemberInfo> enemy_member_list_;

    bool is_enable_leave_member_;
};

#endif //_NA_006826_20130722_DOMINATION_RENEWAL
