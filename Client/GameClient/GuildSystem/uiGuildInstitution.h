#pragma once
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#include "uiBase.h"

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
struct BASE_GuildFacilityImageInfo;
class uiToolTipMan;
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
class uiGuildInstitution : public uiBase
{
    enum UIControlPos
    {
        kControl_CheckBT_C000 = 0,  // 길드소개탭
        kControl_CheckBT_C001,      // 길드원정보탭
        kControl_CheckBT_C003,      // 길드시설탭
        kControl_CheckBT_C004,      // 길드운영탭
#ifdef _NA_008334_20150608_SONNENSCHEIN
        kControl_CheckBT_C005,      // 길드PvE
#endif //_NA_008334_20150608_SONNENSCHEIN
        kControl_CheckBT_C002,      // 길드로그탭

        kControl_Button_B001,       // 시설1 업그레이드
        kControl_Button_B002,       // 시설2 업그레이드
        kControl_Button_B005,       // 시설3 업그레이드
        kControl_Button_B000,       // 시설1 활성화
        kControl_Button_B004,       // 시설2 활성화
        kControl_Button_B007,       // 시설3 활성화

        kControl_TextImage_I000,    // 시설1이름
        kControl_TextImage_I001,    // 시설2이름
        kControl_TextImage_I002,    // 시설3이름

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        kControl_TextImage_I003,    // 특수효과1
        kControl_TextImage_I004,    // 특수효과2
        kControl_TextImage_I005,    // 특수효과3
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

        kControl_Text_S000,         // 현재 길드포인트
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        kControl_List_L000,         // 일반 효과 리스트1
        kControl_List_L004,         // 일반 효과 리스트2
        kControl_List_L005,         // 일반 효과 리스트3
        //
        kControl_List_L001,         // 특수 효과 리스트1
        kControl_List_L002,         // 특수 효과 리스트2
        kControl_List_L003,         // 특수 효과 리스트3
#else
        kControl_Text_S002,         // 시설1설명
        kControl_Text_S003,         // 시설2설명
        kControl_Text_S004,         // 시설3설명
        kControl_List_L001,         // 시설1옵션
        kControl_List_L002,         // 시설2옵션
        kControl_List_L003,         // 시설3옵션
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        
        kControl_Picture_P012,      // 시설1이미지
        kControl_Picture_P015,      // 시설1이미지
        kControl_Picture_P016,      // 시설1이미지

        kControl_VScroll_V000,

        kControl_Size,
    };

    enum
    {
        kListNumber = 3, 
    };

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    enum
    {
        kMaxFacilityLevel = 5, // 길드 시설 최고 레벨
    };
    enum FACILITY_TOOLTIP_TYPE
    {
        TOOLTIP_TYPE_NONE = 0,
        TOOLTIP_TYPE_IMAGE,
        TOOLTIP_TYPE_NORMAL_EFFECT,
        TOOLTIP_TYPE_SPECIAL_EFFECT,
        TOOLTIP_TYPE_UPGRADE,
        TOOLTIP_TYPE_ACTIVE,
        TOOLTIP_TYPE_MAX
    };
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

public:
    static const DWORD kDialog_ID;

public:
    uiGuildInstitution(InterfaceManager* ui_manager);
    virtual ~uiGuildInstitution();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual void MessageProc(SI_MESSAGE* message);
    virtual void OnShowWindow(BOOL is_show);
    virtual void OnUpdateSolarDialog();
    virtual void OnRenderSolarDialog();

private:
    bool OnLButtonClick(SI_MESSAGE* message);
    bool OnVScrollUp(SI_MESSAGE* message);
    bool OnVScrollDown(SI_MESSAGE* message);
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    bool OnMouseOver(SI_MESSAGE* message);
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

    void UpdateInstitutionInfo();
    void UpdateButtonState();
    void UpdateWheelScroll();
    void RefreshTab();
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    void UpdateFacilityListInfo();
    void UpdateActiveButtonState();
    void ProcessGuildPointTooltip();
    void ProcessTooltip(const int& facility_order, const FACILITY_TOOLTIP_TYPE& type);
    void ProcessResultTooltip(const int& facility_name_index, const int& facility_explanation_index);
    void ProcessResultTooltip(const int& facility_name_index, const int& facility_explanation_index, const int& facility_order, const FACILITY_TOOLTIP_TYPE& type);
    void ProcessButtonClick(const DWORD& control_position);
    void RenderTooltip(const int& scroll_position);

public:
    void UpdateAll(); // 모든 Update를 한번에 모아놓은 함수

private:
    bool HaveItem(const CODETYPE& item_code);
    DWORD HaveItemNum(const CODETYPE& item_code); // 몇 개의 아이템을 가지고 있는지 검사
    void AskFacilityUpgrade(const DWORD& facility_code);
    void AskFacilityActive(const DWORD& facility_code, const DWORD& facility_level);
    FLEVEL GetCurrentFacilityActiveLevel(const FCODE& facility_index);

protected:
    POSTYPE			        getCtrlIDToPos( WzID wzId );
private:
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

    HANDLE& institution_image_handle() { return institution_image_handle_; }

private:
    HANDLE institution_image_handle_;
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    DWORD         guild_facility_max_size_;
    uiToolTipMan* tooltip_manager_ptr_;
    static WzID   wz_id_[kControl_Size];
    POSTYPE       mouse_over_pos_;
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
};

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
inline
POSTYPE
uiGuildInstitution::getCtrlIDToPos( WzID wzId )
{
    for( POSTYPE i = 0 ; i < kControl_Size ; ++i )
    {
        if( wz_id_[i] == wzId )
        {
            return i;
        }
    }
    return kControl_Size;
}
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL