#pragma once

#include "uiBase.h"
#include "VScrollDialog.h"

/*
    0. ����Ʈ ���� / ���� ���� / ĳ���� ����
    1. ���� ��� ���� ���ô�� UI����
    2. �ʱ�ȭ : ����Ʈ�� ����
    3. ���� ����
    4. Ű���� �Է� ó��
    5. ���� ó��
    6. ���� ���� ó��
    7. ���� �����ִ��� Ȯ��
    8. ���� ������ ������ ����/��������
*/
class uiHotKeyDialog : public uiBase, public VScrollDialog
{
    enum UIControlPos
    {
        // ��� ��ư
        kControl_Button_B999 = 0,
        kControl_Button_B005,

        kControl_Button_B001, // �ʱ�ȭ ��ư
        kControl_Button_B003, // ���� ��ư
        kControl_Button_B004, // Ȯ��(����)��ư
        kControl_CheckButton_C000, // ĳ���� ���� üũ��ư
        kControl_Text_S016, // ĳ���� ���� �ؽ�Ʈ
        kControl_Text_S002, // �ȳ� �޽���
        kControl_VScroll_V002, // ��ũ��

        // ��ɸ�
        kControl_Text_S000,
        kControl_Text_S005,
        kControl_Text_S006,
        kControl_Text_S009,
        kControl_Text_S008,
        kControl_Text_S007,
        kControl_Text_S015,
        kControl_Text_S014,
        kControl_Text_S013,
        kControl_Text_S012,
        kControl_Text_S011,
        kControl_Text_S010,
        kControl_Text_S021,
        kControl_Text_S020,
        kControl_Text_S019,
        kControl_Text_S018,
        kControl_Text_S017,

        // ���δ���Ű
        kControl_Button_B006,
        kControl_Button_B007,
        kControl_Button_B011,
        kControl_Button_B009,
        kControl_Button_B020,
        kControl_Button_B017,
        kControl_Button_B015,
        kControl_Button_B013,
        kControl_Button_B035,
        kControl_Button_B033,
        kControl_Button_B031,
        kControl_Button_B029,
        kControl_Button_B028,
        kControl_Button_B025,
        kControl_Button_B023,
        kControl_Button_B021,
        kControl_Button_B037,

        // ���� ����Ű
        kControl_Button_BT00,
        kControl_Button_B008,
        kControl_Button_B012,
        kControl_Button_B010,
        kControl_Button_B019,
        kControl_Button_B018,
        kControl_Button_B016,
        kControl_Button_B014,
        kControl_Button_B036,
        kControl_Button_B034,
        kControl_Button_B032,
        kControl_Button_B030,
        kControl_Button_B027,
        kControl_Button_B026,
        kControl_Button_B024,
        kControl_Button_B022,
        kControl_Button_B038,

        kControl_Size,
    };

    enum
    {
        kNumOfDisplay = 17,

        kCaptionControlBegin = kControl_Text_S000, 
        kCaptionControlEnd = kCaptionControlBegin + kNumOfDisplay,

        kMainHotKeyControlBegin = kControl_Button_B006,
        kMainHotKeyControlEnd = kMainHotKeyControlBegin + kNumOfDisplay,

        kSubHotKeyControlBegin = kControl_Button_BT00,
        kSubHotKeyControlEnd = kSubHotKeyControlBegin + kNumOfDisplay,
    };

    struct InputKeyInfo 
    {
        DWORD function_index;
        int list_index;
        WORD hot_key_code;
        BYTE bind_pair_index;

        void Clear()
        {
            function_index = 0;
            list_index = -1;
            hot_key_code = 0;
            bind_pair_index = 0;
        }
    };

public:
    uiHotKeyDialog(InterfaceManager* ui_manager);
    ~uiHotKeyDialog();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual void OnUpdateSolarDialog();
    virtual void OnRenderSolarDialog();
    virtual void MessageProc(SI_MESSAGE* message);
    virtual void NetworkProc(MSG_BASE* recv_packet);

    void RecvUserKeySetting(WORD hot_key_code); // HotKeyManager �κ��� Ű�Է»��¸� �ݹ����� �޾ƿ�

private:
    virtual void OnShowWindow(BOOL is_show);

    virtual bool GetScrollArea(RECT& area);
    virtual CCtrlVScrollWZ* vscroll_control();

    virtual bool OnVScrollDown(SI_MESSAGE* msg);
    virtual bool OnVScrollUp(SI_MESSAGE* msg);
    virtual bool OnVScrollThumb(SI_MESSAGE* msg);

    bool OnLButtonClick(SI_MESSAGE* message);

    void InitializeKeyBindInfo();
    void RefreshBindInfo(); // ���� ��ũ�� ��ġ���� ������������ UI�� Ű���� ���� ���
    void ApplayHotKey();

    void ShowNoticeString(const TCHAR* notice_string);
    void DisableControls();
    void EnableControls(bool is_enable);
    void EnableControl(UIControlPos control_position, bool is_enable);
    
    void RegisterKeyBindInfo(DWORD index, BYTE bind_pair_index, WORD hot_key_code);
    void ReleaseKeyBindInfo(DWORD index, BYTE bind_pair_index);

    void RenderInputHighLight();

    STLX_VECTOR<DWORD>& function_index_vector() { return function_index_vector_; }
    WORD current_ui_index() const { return current_ui_index_; }
    void set_current_ui_index(WORD val) { current_ui_index_ = val; }
    InputKeyInfo& input_key_info() { return input_key_info_; }

private:
    STLX_VECTOR<DWORD> function_index_vector_;
    WORD current_ui_index_;
    InputKeyInfo input_key_info_;
};

