#pragma once
#include "uiBase.h"
#include "Achievement.h"
#include "AreaConquestInfo.h"
#include "FTextRender.h"
#include "DropdownControl.h"

// class AchievementPart;
// class AchievementInfo;

#ifdef _NA_006541_RENEWER_ACDIALOG
    #include "TreeController.h"
#endif


struct Object_Info
{
    CODETYPE    object_code;
    bool        is_complete;
    WORD        achievement;	//�޼� ��ǥ �� (�� �� �̻��̸� �޼�)
    WORD        value;			    //������ ���� ��      
};

class uiAreaConquestDialog : public uiBase
{
    enum UIControlPos
    {
        kControl_Button_Close = 0,   

#ifdef _NA_006541_RENEWER_ACDIALOG
#else
        kControl_Button_BT00,       // �������ù�ư
        kControl_Button_BT01,       // ���¼��ù�ư
#endif

#ifdef _NA_002884_AC_DIALOG_RENEWAL
        kControl_Picture_BackGround,// ��� �̹���

        kControl_Button_Object_01,  // AC�׸��ư#1
        kControl_Button_Object_02,
        kControl_Button_Object_03,
        kControl_Button_Object_04,
        kControl_Button_Object_05,
        kControl_Button_Object_06,
        kControl_Button_Object_07,
        kControl_Button_Object_08,
        kControl_Button_Object_09,
        kControl_Button_Object_10,
        kControl_Button_Object_11,
        kControl_Button_Object_12,
        kControl_Button_Object_13,
        kControl_Button_Object_14,
        kControl_Button_Object_15,
        kControl_Button_Object_16,
        kControl_Button_Object_17,
        kControl_Button_Object_18,
        kControl_Button_Object_19,
        kControl_Button_Object_20,  // AC�׸��ư#20

        kControl_Button_Image_01,  // AC�׸������#1
        kControl_Button_Image_02,
        kControl_Button_Image_03,
        kControl_Button_Image_04,
        kControl_Button_Image_05,
        kControl_Button_Image_06,
        kControl_Button_Image_07,
        kControl_Button_Image_08,
        kControl_Button_Image_09,
        kControl_Button_Image_10,
        kControl_Button_Image_11,
        kControl_Button_Image_12,
        kControl_Button_Image_13,
        kControl_Button_Image_14,
        kControl_Button_Image_15,
        kControl_Button_Image_16,
        kControl_Button_Image_17,
        kControl_Button_Image_18,
        kControl_Button_Image_19,
        kControl_Button_Image_20,  // AC�׸������#20

        kControl_Button_Reward_01,  // AC�׸񺸻��ư#1
        kControl_Button_Reward_02,
        kControl_Button_Reward_03,
        kControl_Button_Reward_04,
        kControl_Button_Reward_05,
        kControl_Button_Reward_06,
        kControl_Button_Reward_07,
        kControl_Button_Reward_08,
        kControl_Button_Reward_09,
        kControl_Button_Reward_10,
        kControl_Button_Reward_11,
        kControl_Button_Reward_12,
        kControl_Button_Reward_13,
        kControl_Button_Reward_14,
        kControl_Button_Reward_15,
        kControl_Button_Reward_16,
        kControl_Button_Reward_17,
        kControl_Button_Reward_18,
        kControl_Button_Reward_19,
        kControl_Button_Reward_20,  // AC�׸񺸻��ư#20
#else
        kControl_Button_Object_01,  // AC�׸��ư#1
        kControl_Button_Object_02,
        kControl_Button_Object_03,
        kControl_Button_Object_04,
        kControl_Button_Object_05,
        kControl_Button_Object_06,
        kControl_Button_Object_07,
        kControl_Button_Object_08,
        kControl_Button_Object_09,
        kControl_Button_Object_10,  // AC�׸��ư#10
        kControl_Button_Image_01,   // AC�׸������#1
        kControl_Button_Image_02,
        kControl_Button_Image_03,
        kControl_Button_Image_04,
        kControl_Button_Image_05,
        kControl_Button_Image_06,
        kControl_Button_Image_07,
        kControl_Button_Image_08,
        kControl_Button_Image_09,
        kControl_Button_Image_10,   // AC�׸������#10
        kControl_Button_Reward_01,  // AC�׸񺸻��ư#1
        kControl_Button_Reward_02,
        kControl_Button_Reward_03,
        kControl_Button_Reward_04,
        kControl_Button_Reward_05,
        kControl_Button_Reward_06,
        kControl_Button_Reward_07,
        kControl_Button_Reward_08,
        kControl_Button_Reward_09,
        kControl_Button_Reward_10,  // AC�׸񺸻��ư#10
#endif //_NA_002884_AC_DIALOG_RENEWAL

        kControl_Button_Object_Detail, // ���������
        kControl_Button_Detail_Reward, // �����������ư

#ifdef _NA_006541_RENEWER_ACDIALOG
        kControl_Button_Picture_01,  // AC�׸񺸻��ư#1
        kControl_Button_Picture_02,
        kControl_Button_Picture_03,
        kControl_Button_Picture_04,
        kControl_Button_Picture_05,
        kControl_Button_Picture_06,
        kControl_Button_Picture_07,
        kControl_Button_Picture_08,
        kControl_Button_Picture_09,
        kControl_Button_Picture_10,
        kControl_Button_Picture_11,
        kControl_Button_Picture_12,
        kControl_Button_Picture_13,
        kControl_Button_Picture_14,
        kControl_Button_Picture_15,
        kControl_Button_Picture_16,
        kControl_Button_Picture_17,
        kControl_Button_Picture_18,
        kControl_Button_Picture_19,
        kControl_Button_Picture_20,  // AC�׸񺸻��ư#20
#endif

#ifdef _NA_006541_RENEWER_ACDIALOG
        kControl_List_L000,
        kControl_VScroll,           // AC�׸�ũ��
#else
#ifdef _SCJ_111202_UI_RENEWAL
        kControl_Button_B005,       // ���������� ��ư
        kControl_Button_B006,       // ���û��¸� ��ư
#else
        kControl_Text_T201,         // ����������
        kControl_Text_ST01,         // ���û��¸�
#endif //_SCJ_111202_UI_RENEWAL
        kControl_Text_ST02,         // �Ϸ�ī��Ʈ
        kControl_Text_S000,         // �޼���
        kControl_Picture_P000,      // �޼���������
        kControl_List_LS00,         // �������ø���Ʈ
        kControl_List_LS01,         // ���¼��ø���Ʈ
        kControl_VScroll,           // AC�׸�ũ��
        kControl_Edit_E000,         // Edit�ڽ���Ʈ��
#endif

        kControl_Size,
    };

    enum
    {
        kDisplay_AC_Info_Num = kControl_Button_Image_01 - kControl_Button_Object_01,
    };

public:

    enum
    {
        kComplete_Text_Color = 0,
        kInComplete_Text_Color,
        kEnable_Text_Color,
        kDisable_Text_Color,

        KMax_Text_Color,
    };

public:
    uiAreaConquestDialog(InterfaceManager* ui_manager);
    virtual ~uiAreaConquestDialog(void);

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual void OnUpdateSolarDialog();
    
    virtual void OnShowWindow(BOOL is_show);
    virtual void MessageProc(SI_MESSAGE* message);

    void RegisterControl();
    void InitACDialog();
    void OnSelectArea(WORD area_code);
    void OnSelectACInfoState(WORD select_State);
    void OnSelectAcinfoIndex(WORD info_index);
    void OnUpdateACInfo();

    WORD GetCurrentAreaCode();
    WORD GetPendingRewardACIndex();

    DropdownControl& dropdown_select_area() { return dropdown_select_area_; }
    DropdownControl& dropdown_select_stats() { return dropdown_select_stats_; }

    void UpdateValue();
    void AllClearACDialog(int selected_item = -1);
private:
    bool OnLButtonClick(SI_MESSAGE* message);
    bool OnUpScroll(SI_MESSAGE* message);
    bool OnDownScroll(SI_MESSAGE* message);
    bool OnThumbScroll(SI_MESSAGE* message);
    void OnMouseWheelScroll(BYTE mouse_wheel_state);

    void OnClickACButton(SI_MESSAGE* message);
    void OnClickACInfo(DWORD list_index);
    void OnClickACInfoDetail();
    void OnClickACReward(DWORD list_index);
    void OnClickACIcon(DWORD list_index);

#ifdef _NA_006541_RENEWER_ACDIALOG
    void MakeCompoisiteTreeitem(HTREEITEM perentItem,void* Item,bool percent);
#else
    void SetAreaProgressData(DWORD ac_ratio, DWORD total_count, DWORD complete_count, bool is_show_ratio);
#endif

    void SetACData(int index_display, int index_info);
    bool SetACDetailData(int index_display);
    void SetRectACControl(int index_display, bool enable_reward);
    void SetACDataString(int index_display, AchievementInfo* ac_info);    
    void GetACObjectText(AchievementInfo* ac_info, TCHAR* ac_object_string);
    void GetACRewardText(AchievementInfo* ac_info, TCHAR* ac_reward_string);
    bool GetACObjectDetailText(WORD object_type, CODETYPE object_code, TCHAR* object_detail_string,
        WORD achievement, WORD value);  
    void ClearAcInfoToDisplay(int index_display);
    
    void RefreshACInfo();
    void RefreshScroll(WORD ac_index_index);

    bool IsShowControl(POSTYPE control_pos);
    void ShowControl(POSTYPE control_pos, bool is_show);
    void ToggleShowControl(POSTYPE control_pos);
    void HideACInfoDetail();

    void RenderACInfoDetailControl();
    

#ifdef _NA_006541_RENEWER_ACDIALOG
    void ReflushForceMove(int Cell,RECT size);
    void ReflushList();
    void InitControlReflush();
    bool isInRecttoRect(const RECT& from,const RECT& to);

    
    bool CheckNationTree(const HTREEITEM item,HTREEITEM* Outdata,int selected_item);
#endif

    static void CallBackRenderACDetailInfo(CControlWZ* control_ptr, void* parameter);

    AreaConquestInfo* area_conquest_info() { return &area_conquest_info_; }

#ifdef _NA_002884_AC_DIALOG_RENEWAL
#else
    void RenderACInfoText(DWORD index_display);
    
    static void CallBackRenderACInfo(CControlWZ* control_ptr, void* parameter);
#endif //_NA_002884_AC_DIALOG_RENEWAL

private:
    uiAreaConquestMan* ui_ac_manager_;
    
#ifdef _NA_002884_AC_DIALOG_RENEWAL
    STLX_VECTOR<int> ac_info_control_width_;
#else
    std::vector<FTextRender> ac_info_text_render_;
    HANDLE info_control_texture_;
#endif //_NA_002884_AC_DIALOG_RENEWAL
    FTextRender info_detail_text_;

    WORD selected_area_code_;
    WORD selected_info_state_;

    int current_scroll_position_;
    POSTYPE ac_detail_display_index_;
    WORD pending_reward_ac_index_;
    float ac_gauge_control_size_;
    float ac_gauge_texture_size_;

    AreaConquestInfo area_conquest_info_;
    AchievementInfo* selected_achievement_info_;

    DropdownControl dropdown_select_area_;
    DropdownControl dropdown_select_stats_;
#ifdef _NA_006541_RENEWER_ACDIALOG
    TreeController*  tree_control_ptr_;
    int              control_size_;
    LONG             height_item_;
    int              scroller_index_;
    int              expend_item_;
    int              check_expend_item_;
    int              post_extend_item_scroll_;
    int              string_height_;
    //�����ư�� Ư�������� ���� ������ ��ư�� ó����.
    std::vector<int> hide_control_reward;
#endif
};

inline WORD uiAreaConquestDialog::GetCurrentAreaCode()
{
    WORD area_code = 0;
    if (selected_achievement_info_ != NULL)
    {
        area_code = selected_achievement_info_->GetLocationCode();
    }
    return area_code;
}

inline WORD uiAreaConquestDialog::GetPendingRewardACIndex()
{
    WORD info_index = 0;
    if (selected_achievement_info_ != NULL)
    {
        info_index = selected_achievement_info_->GetIndex();
    }
    return info_index;
}

inline void uiAreaConquestDialog::ShowControl(POSTYPE control_pos, bool is_show)
{
    CControlWZ* control = GetControlWZ(GetControlID_byPosition(control_pos));
    if (control != NULL)
    {
        control->ShowWindowWZ((is_show == true) ? WZ_SHOW : WZ_HIDE);
    }
}

inline void uiAreaConquestDialog::ToggleShowControl(POSTYPE control_pos)
{
    CControlWZ* control = GetControlWZ(GetControlID_byPosition(control_pos));
    if (control != NULL)
    {
        ENUM_STATEWZ control_state = control->GetState();
        control->ShowWindowWZ((control_state == WZ_HIDE) ? WZ_SHOW : WZ_HIDE);
    }
}

inline void uiAreaConquestDialog::HideACInfoDetail()
{
    ShowControl(kControl_Button_Object_Detail, false);
    ShowControl(kControl_Button_Detail_Reward, false);


#ifdef _NA_006541_RENEWER_ACDIALOG
    ac_detail_display_index_ = INVALID_SLOT_POS;
    selected_achievement_info_ = NULL;

    CCtrlVScrollWZ* scroll_control = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ(GetControlID_byPosition(kControl_VScroll)));
    if (scroll_control == NULL)
    {
        return;
    }

    int scroll_min = 0; 
    int scroll_max = 0;
    int info_list_num = area_conquest_info()->GeAchievementInfoSize();
    int current_scroll_pos = scroll_control->GetScrollPosWZ();
    scroll_control->GetScrollRangeWZ(&scroll_min, &scroll_max);
    scroll_max = max(0, (info_list_num - kDisplay_AC_Info_Num));

    scroll_control->SetScrollRangeWZ(scroll_min, scroll_max);

    //��ũ���� ��ġ ����
    //������ ��ġ�� �Ʒ���.
    //��ũ���� ���� ������ �ø�.
    int pos = 0;
    if(  pos = (current_scroll_pos - post_extend_item_scroll_)> 0)
    {
        for (int i = 0 ; i < pos ; ++i)
        {
            OnUpScroll(NULL);
        }
    }
    //Ȯ�� ��ũ���� �Ʒ���
    else if( pos = (post_extend_item_scroll_ - current_scroll_pos) > 0)
    {
        for (int i = 0 ; i < pos ; ++i)
        {
            OnDownScroll(NULL);
        }
    }

    expend_item_ = 0;

#endif

}
