#pragma once

#include "uiBase.h"
#include "SCPerkSlotContainer.h"
#include "SkillSystem/PerkDataContainer.h"
#include "FTextRender.h"

/*
    CG_CHARINFO_SKILL_PERK_CMD  // 퍽스킬 리스트
    CG_SKILL_PERK_CONTROL_SYN   // 퍽스킬 컨트롤 SYNC
    CG_SKILL_PERK_CONTROL_CMD   // 퍽스킬 컨트롤 CMD


*/
#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM
struct PerkGroupStruct
{    
    POSTYPE from_pos[4];

    PerkGroupStruct()
    {        
        ZeroMemory(from_pos, sizeof(POSTYPE) * 4);
        for ( unsigned int i = 0; i < 4; ++i)
        {
            from_pos[i] = INVALID_POSTYPE_VALUE;
        }
    }
    void Clear()
    {
        for ( unsigned int i = 0; i < 4; ++i)
        {
            from_pos[i] = INVALID_POSTYPE_VALUE;
        }
    }
    void operator()(const PerkGroupStruct& perk_data)
    {        
        for (unsigned int i = 0; i < 4; ++i)
        {
            this->from_pos[i] = perk_data.from_pos[i];
        }
    }
    bool operator==(const PerkGroupStruct& perk_group_struct)
    {
        for (int i = 0; i < 4; ++i)
        {
            if (this->from_pos[i] != perk_group_struct.from_pos[i])
            {                
                return false;
            }
        }
        return true;
    }
};
const int kPerkGroupMax = 2;
// 액티브 슬롯Pos를 바로바로 사용하기 위해서... Refresh_Perk_Slot_My_Data() 참고 할 것
enum 
{
    kActiveSlotPos0 = 30,
    kActiveSlotPos1 = 31,
    kActiveSlotPos2 = 32,
    kActiveSlotPos3 = 33,
    kActiveSlotPosMax,
};

enum
{
    eStylePageA = 0,
    eStylePageB,
    eStylePageMax,
};

#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM
class InterfaceManager;
class uiSkillMan;
class ItemUnitRenderer;

class uiPerkDialog
    : public uiBase, 
      public SCPerkSlotContainer,
      public nsSkill::IPerkControlEvent,
      public nsSkill::IPerkControlMessenger
{
    struct OPTION_VALUE_AND_TYPE{
        int   option_value; //옵션 효과 값
        int   option_type;  //옵션 접미사 타입
    };
    typedef std::map<const PerkInfo*, int> PERK_INFO_DUPICATE_MAP;

    typedef std::map<DWORD, OPTION_VALUE_AND_TYPE> CALCULATED_OPTION_VALUE_MAP;

    enum UIControlPos
    {
        // 탭버튼
        kControl_CheckBT_C005 = 0,
        kControl_CheckBT_C006,
        kControl_CheckBT_C000,
        // 일반Perk 슬롯 시작
        kControl_Button_B010,
        kControl_Button_B000,       
        kControl_Button_B001,
        kControl_Button_B002,
        kControl_Button_B003,

        kControl_Button_B005,
        kControl_Button_B006,
        kControl_Button_B008,
        kControl_Button_B009,
        kControl_Button_B011,

        kControl_Button_B013,
        kControl_Button_B014,
        kControl_Button_B015,
        kControl_Button_B016,
        kControl_Button_B017,

        // 캐시Perk 슬롯 시작
        kControl_Button_B004,
        kControl_Button_B012,
        kControl_Button_B018,
        
        // 액티브Perk 슬롯 시작
        kControl_Button_B019,
        kControl_Button_B020,
        kControl_Button_B021,
        kControl_Button_B022,
        kControl_Button_B023,

        // 액티브Perk 취소 버튼
        kControl_Button_B024,
        kControl_Button_B025,
        kControl_Button_B026,
        kControl_Button_B027,
        kControl_Button_B028,

        // Perk 옵션관련 텍스트 출력할 영역
        kControl_Rect_R000,
        kControl_Rect_R002,
        kControl_Rect_R004,
        kControl_Rect_R005,

#ifndef _NA_0_20120321_SKILLTREE_SECOND_RENEWAL
        // Perk 옵션내용 스크롤
        kControl_Scroll_V000,
        kControl_Picture_P000,
#endif

        // 내부 배경이미지
        kControl_Picture_P001,

        // 닫기버튼
        kControl_Button_B999,
#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM        
        kControl_CheckBT_C003,
        kControl_CheckBT_C004,
        kControl_Picture_P017, // 외곽
        kControl_Button_B005_New,
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM

        kControl_Size,
    };

    enum
    {
        kDefaultPerkButton_Start = kControl_Button_B010,
        kDefaultPerkButton_End = kControl_Button_B017 + 1,

        kCashPerkButton_Start = kControl_Button_B004,
        kCashPerkButton_End = kControl_Button_B018 + 1,

        kActivePerkButton_Start = kControl_Button_B019,
        kActivePerkButton_End = kControl_Button_B023 + 1,

        kDefaultPerkNumber = kDefaultPerkButton_End - kDefaultPerkButton_Start,
        kCashPerkNumber = kCashPerkButton_End - kCashPerkButton_Start,
        kActivePerkNumber = kActivePerkButton_End - kActivePerkButton_Start,

        kUnregisterButtonOffest = kControl_Button_B024 - kControl_Button_B019,
    };

    enum FTextRenderType
    {
        kFTextRenderTop = 0,
        kFTextRenderBottom,
        kFTextRenderOption1,
        kFTextRenderOption2,
        kFTextRenderMax,
    };

    enum
    {
        KMaxPerkLevel = 5,
    };

public:
    // 생성자/소멸자
    uiPerkDialog(InterfaceManager* ui_manager);
    virtual ~uiPerkDialog();

    // 초기화/해제
    virtual void Init(CDrawBase* pDrawBase);
    virtual void Release();

    virtual void ClearAll();
    virtual RC::eSLOT_INSERT_RESULT InsertSlot(POSTYPE at_pos, SCSlot& IN slot);
    virtual BOOL DeleteSlot(POSTYPE at_pos, SCSlot* slot_clone_before_delete);
    virtual void UpdateSlot(POSTYPE at_pos, int changed_num);

    virtual void MessageProc(SI_MESSAGE * pMessage);

    virtual ItemUnitRenderer* GetItemUnitRender() const { return unit_renderer_; }
    //nsSkill::IPerkControlEvent interfaces = {
    virtual void OnChangeAttributes();
    virtual void OnChangeSlotContainer();
    virtual void OnLevelup(); // be called once since total changes
    virtual void OnLevelup(const PerkID& perk_id) { __UNUSED(perk_id); }
    // }
    virtual SCPerkSlotContainer* GetPerkSlotContainer() { return (SCPerkSlotContainer*)this; }
    virtual Attributes* GetOwnerAttributes();
    virtual IPerkControlEvent* GetPerkControlEvent() { return (IPerkControlEvent*)this; }
    void NetworkProc(MSG_BASE* recv_msg);
    void RECV_CG_CHARINFO_SKILL_PERK_CMD(MSG_CG_CHARINFO_SKILL_PERK_CMD* recv_packet);
    void RECV_CG_SKILL_PERK_CONTROL_CMD(MSG_CG_SKILL_PERK_CONTROL_CMD* recv_packet);
    void SEND_CG_SKILL_PERK_CONTROL_SYN(MSG_CG_SKILL_PERK_CONTROL_SYN* send_packet);
    void RECV_CG_SKILL_PERK_STATUS_CMD(MSG_CG_SKILL_PERK_STATUS_CMD* recv_packet);
#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM
    void SEND_CG_SKILL_PERK_GROUP_CONTROL_SYN(MSG_CG_SKILL_PERK_GROUP_CONTROL_SYN* send_packet);
    void RECV_CG_SKILL_PERK_GROUP_CONTROL_CMD(MSG_CG_SKILL_PERK_GROUP_CONTROL_CMD* recv_packet);
    void SEND_CG_ETC_GAMEOPTION_LOAD_SYN_PERK_LIST(MSG_CG_ETC_GAMEOPTION_LOAD_SYN* send_packet);
    void SavePerkGroupListData();
    bool MakeToStream(GameOption& game_option);
    void RecvPerkGroupList(GameOption& game_option);
    bool MakeToPerkGroupList(GameOption& game_option);
    const PerkGroupStruct& get_active_group_struct();
    void PerkGroupChange_On();
    int  get_active_group_number_();
    bool get_is_wait_packet_();
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM

    void RequestPerkOperaion(PerkControl::eRequest request_operation, 
                             POSTYPE from_pos, 
                             POSTYPE to_pos);
    void ProcessResultCode(DWORD result, DWORD result_code);

    void UpdatePerkAttribute();

    static void RenderPerkLevelImage(int perk_level, RECT* draw_rect);
    IconImage* GetPerkLevelImage(int perk_level);

protected:
    virtual void OnShowWindow(BOOL val);
    virtual void OnRenderSolarDialog();
    virtual void OnUpdateSolarDialog();

private:
    void RegisterControl();

    void SetButtonControlBySlotPos();
    CControlWZ* GetButtonControlBySlotPos(POSTYPE slot_pos);

    bool OnLButtonClick(SI_MESSAGE* message);
    bool OnMouseOver(SI_MESSAGE* message);

#ifndef _NA_0_20120321_SKILLTREE_SECOND_RENEWAL
    bool OnUpScroll(SI_MESSAGE* message);
    bool OnDownScroll(SI_MESSAGE* message);
    bool OnThumbScroll(SI_MESSAGE* message);
#endif
    void RefreshTab();

    void OnClickPerkPalette(DWORD control_position, DWORD control_id);
    void OnClickActivePerk(DWORD control_position, DWORD control_id);
    void OnClickUnregisterButton(DWORD control_position, DWORD control_id);
#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM
    void OnClickAGroupButton(DWORD control_position, DWORD control_id);
    void OnClickBGroupButton(DWORD control_position, DWORD control_id);
    void Refresh_PerkSlock_My_Data();
    void ChangeGroupPerkLogic(); // 실제 액티브 슬롯 그룹을 보여주는 로직.
    void ChangeGroupPageLogic(); // 페이지에 따라서 실제 액티브 슬롯 그룹을 보여주지 않는 로직.
    void set_active_group_number(const int& active_group_number);    
    void hero_gauge_style_setting(const int& style);
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM

    POSTYPE GetSlotPositionByControlPosition(DWORD control_postion);

    void RollbackPerkItemAtHand();

    bool CheckRegisterActivePerk();
    void RefreshOptionText();

    void RefreshTopMessage();
    void RefreshBottomMessage();
    void RefreshOptionList();

    void CalculateOptionList();

    void LoadPerkLevelImage();

    bool IsEnableSetting();
    bool IsSwitchablePerkState();
private:
    uiSkillMan* ui_skill_manager_;
    ItemUnitRenderer* unit_renderer_;
    map<POSTYPE, CControlWZ*> button_control_map_;

    FTextRender text_render_[kFTextRenderMax];

    CALCULATED_OPTION_VALUE_MAP benefit_option_map_;
    CALCULATED_OPTION_VALUE_MAP penalty_option_map_;

    bool is_mouse_over_in_perk_slot_;
    POSTYPE mouse_over_perk_slot_;

    bool is_store_perk_data_;

    std::vector<IconImage*> perk_level_image;
#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM    
    int current_page_;
    std::vector<PerkGroupStruct> my_perk_data_;
    std::vector<PerkGroupStruct> my_prototype_perk_data_;
    PerkGroupStruct current_active_perk_my_data_;
    int active_group_number_;
    int active_group_number_prototype_;
    BOOL is_prototype_receive; // 처음으로 옵션 정보를 받았을 경우
    bool is_wait_packet_; // 혹시 꼬일지도 모를 상황을 대비한다.
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM
};

inline void uiPerkDialog::UpdatePerkAttribute()
{
    if (is_store_perk_data_ == true)
    {
        OnEnterField();
    }
}

inline IconImage* uiPerkDialog::GetPerkLevelImage(int perk_level)
{
    return perk_level_image[(perk_level - 1)];
}

