#include "SunClientPrecompiledHeader.h"

#pragma once
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "uiBase.h"
#include "VScrollDialog.h"
#include "struct.h"

const int refresh_room_list_time = 30;
const int refresh_animation_time = 1;
const int refresh_animation_frame = 12;
const float refresh_animation_frame_width = 40;
const float refresh_anumation_frame_height = 40;

const int free_pvp_room_list_count = 10;
const int free_pvp_list_height = 20;
const int free_pvp_icon_height= 16;

const int default_room_name_start_string = 1981;  // �⺻ ������ ���� ��Ʈ��
const int default_room_name_string_count = 19;   // �⺻ ������ �ִ� ����

class uiChaosZonePVP : public uiBase, public VScrollDialog
{
private:
    enum UIControlPos
    {
        kControl_Button_B999 = 0,
        kControl_Button_B005, //�����ϱ�
        kControl_Button_B000,
        kControl_Button_B001,
        kControl_Button_B002,
        kControl_Button_B003,
        kControl_Button_B004,
        kControl_Button_B007,
        kControl_Button_B006, //�游���
        kControl_Button_B008, //���ΰ�ħ
        kControl_Text_T999,
        kControl_List_L000,
        //kControl_List_L001,
        kControl_VScroll_V001,
        kControl_Text_S000,
        //kControl_Text_S001, //������
        kControl_Text_S002,
        kControl_Text_S003,
        kControl_Edit_E000,
        kControl_Edit_E001,
        kControl_Size,
    };

    enum
    {
        FREEPVP_LIST_NUM,
        FREEPVP_LIST_TITLE,
        FREEPVP_LIST_MEMBER,
        FREEPVP_LIST_MAX,
    };

public:
    uiChaosZonePVP(InterfaceManager* ui_manager);
    virtual ~uiChaosZonePVP();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual void OnShowWindow(BOOL is_show);
    virtual void MessageProc(SI_MESSAGE* message);
    virtual void NetworkProc(MSG_BASE* recv_packet);
    virtual void OnRenderSolarDialog();
    virtual void OnUpdateSolarDialog();

public:
    void ShowAnimation(bool is_value);
    void SendFreePVPJoinPacket(); // �� ���� ��Ŷ ����
    void SendFreePVPCreatePacket(); // �� ���� ��Ŷ ����
    void SetPVPRoomList(); // �� ����Ʈ ����
    void ClearEditText();
private:
    bool OnLButtonClick(SI_MESSAGE* message);
    bool OnRButtonClick(SI_MESSAGE* message);
    bool OnDoubleButtonClick(SI_MESSAGE* message);
    bool OnTapKeyDown(SI_MESSAGE* message);

    bool OnMouseOver(SI_MESSAGE* message);
    void RenderTooltip();
    CCtrlVScrollWZ* vscroll_control();
    bool GetScrollArea(RECT& area);
    bool OnVScrollThumb(SI_MESSAGE* msg);
    bool OnVScrollUp(SI_MESSAGE* msg);
    bool OnVScrollDown(SI_MESSAGE* msg);

    void RequestRoomList(bool is_fast = false); // �� ����Ʈ ��û
    void EnterRoom(); // �� ����
    void CreateRoom(); // �� ����
    void PlayAnimation();
    void RenderIcon();
    void InitTextures();
    void ShowTitleEditControl(bool is_show);
    void ShowPasswordEditControl(bool is_show);
    
private:
    CCtrlListWZ* list_control_;
    CCtrlVScrollWZ* scroll_control_;
    CTimerSimple list_refresh_timer_;
    CTimerSimple animation_refresh_timer_;

    HANDLE effect_texture_;
    FRECT texture_rectangles_[refresh_animation_frame];
    int last_scroll_position_;
    bool animatino_show_;
    bool first_request_;

    CCtrlDxEditBox* title_edit_control_;
    CCtrlStaticWZ* title_static_control_;
    CCtrlDxEditBox* password_edit_control_;
    CCtrlStaticWZ* password_static_control_;
};

#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
