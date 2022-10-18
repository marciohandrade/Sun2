#pragma once

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
#else

#include "uiBase.h"

class uiDominationJoinList1 : public uiBase
{
    enum UIControlPos
    {
        kControl_Static_T999, // 타이틀
        kControl_Button_B031, // 새로고침 버튼
        kControl_Button_B999, // 닫기 버튼

        kControl_Button_BT01, // 아군 클래스 버튼
        kControl_Button_B001, // 아군 레벨 버튼
        kControl_Button_B003, // 아군 캐릭터명 버튼

        kControl_Button_B000, // 적군 클래스 버튼
        kControl_Button_B002, // 적군 레벨 버튼
        kControl_Button_B004, // 적군 캐릭터명 버튼

        kControl_Friend_Class_Image_Start, // 아군 클래스 리스트1
        kControl_Friend_Class_Image_End  = kControl_Friend_Class_Image_Start + (MAX_DOMINATION_MEMBER - 1),   // 아군 클래스 리스트20

        kControl_Friend_Level_Start, // 아군 레벨 리스트1
        kControl_Friend_Level_End = kControl_Friend_Level_Start + (MAX_DOMINATION_MEMBER - 1),   // 아군 레벨 리스트20

        kControl_Friend_Member_Name_Start, // 아군 참가 캐릭터 이름 리스트1
        kControl_Friend_Member_Name_End = kControl_Friend_Member_Name_Start + (MAX_DOMINATION_MEMBER - 1),   // 아군 참가 캐릭터 이름 리스트20

        kControl_Enemy_Class_Image_Start, // 적군 클래스 리스트1
        kControl_Enemy_Class_Image_End = kControl_Enemy_Class_Image_Start + (MAX_DOMINATION_MEMBER - 1),   // 적군 클래스 리스트20

        kControl_Enemy_Level_Start, // 적군 레벨 리스트1
        kControl_Enemy_Level_End = kControl_Enemy_Level_Start + (MAX_DOMINATION_MEMBER - 1),   // 적군 레벨 리스트20

        kControl_Enemy_Member_Name_Start, // 적군 참가 캐릭터 이름 리스트1
        kControl_Enemy_Member_Name_End = kControl_Enemy_Member_Name_Start + (MAX_DOMINATION_MEMBER - 1),   // 적군 참가 캐릭터 이름 리스트20

        kControl_Member_Leave_Button_Start, // 참가 취소 버튼1
        kControl_Member_Leave_Button_End = kControl_Member_Leave_Button_Start + (MAX_DOMINATION_MEMBER - 1),   // 참가 취소 버튼20

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
