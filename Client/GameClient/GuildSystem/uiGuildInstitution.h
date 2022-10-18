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
        kControl_CheckBT_C000 = 0,  // ���Ұ���
        kControl_CheckBT_C001,      // ����������
        kControl_CheckBT_C003,      // ���ü���
        kControl_CheckBT_C004,      // �����
#ifdef _NA_008334_20150608_SONNENSCHEIN
        kControl_CheckBT_C005,      // ���PvE
#endif //_NA_008334_20150608_SONNENSCHEIN
        kControl_CheckBT_C002,      // ���α���

        kControl_Button_B001,       // �ü�1 ���׷��̵�
        kControl_Button_B002,       // �ü�2 ���׷��̵�
        kControl_Button_B005,       // �ü�3 ���׷��̵�
        kControl_Button_B000,       // �ü�1 Ȱ��ȭ
        kControl_Button_B004,       // �ü�2 Ȱ��ȭ
        kControl_Button_B007,       // �ü�3 Ȱ��ȭ

        kControl_TextImage_I000,    // �ü�1�̸�
        kControl_TextImage_I001,    // �ü�2�̸�
        kControl_TextImage_I002,    // �ü�3�̸�

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        kControl_TextImage_I003,    // Ư��ȿ��1
        kControl_TextImage_I004,    // Ư��ȿ��2
        kControl_TextImage_I005,    // Ư��ȿ��3
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

        kControl_Text_S000,         // ���� �������Ʈ
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        kControl_List_L000,         // �Ϲ� ȿ�� ����Ʈ1
        kControl_List_L004,         // �Ϲ� ȿ�� ����Ʈ2
        kControl_List_L005,         // �Ϲ� ȿ�� ����Ʈ3
        //
        kControl_List_L001,         // Ư�� ȿ�� ����Ʈ1
        kControl_List_L002,         // Ư�� ȿ�� ����Ʈ2
        kControl_List_L003,         // Ư�� ȿ�� ����Ʈ3
#else
        kControl_Text_S002,         // �ü�1����
        kControl_Text_S003,         // �ü�2����
        kControl_Text_S004,         // �ü�3����
        kControl_List_L001,         // �ü�1�ɼ�
        kControl_List_L002,         // �ü�2�ɼ�
        kControl_List_L003,         // �ü�3�ɼ�
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        
        kControl_Picture_P012,      // �ü�1�̹���
        kControl_Picture_P015,      // �ü�1�̹���
        kControl_Picture_P016,      // �ü�1�̹���

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
        kMaxFacilityLevel = 5, // ��� �ü� �ְ� ����
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
    void UpdateAll(); // ��� Update�� �ѹ��� ��Ƴ��� �Լ�

private:
    bool HaveItem(const CODETYPE& item_code);
    DWORD HaveItemNum(const CODETYPE& item_code); // �� ���� �������� ������ �ִ��� �˻�
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