#pragma once
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#include "uiBase.h"

class uiGuildJoinRequestList : public uiBase
{
    enum UIControlPos
    {
        kControl_Button_B999, // x ��ư
        kControl_Button_BT00,
        kControl_Button_BT02,
        kControl_Button_BT01,
        kControl_Button_B003,
        kControl_Button_B000, // �ݱ�

        kControl_List_L001, // list

        kControl_Button_B001, // ����1
        kControl_Button_B002, // ����1
        kControl_Button_B010, // ����2
        kControl_Button_B011, // ����2
        kControl_Button_B012, // ����3
        kControl_Button_B013, // ����3
        kControl_Button_B014, // ����4
        kControl_Button_B015, // ����4
        kControl_Button_B016, // ����5
        kControl_Button_B017, // ����5
        kControl_Button_B018, // ����6
        kControl_Button_B019, // ����6

        kControl_Button_B023, // ���� ������ ��ư
        kControl_Button_B024, // ���� ������ ��ư

        kControl_Text_S000, // ������ Text

        kControl_Size,
    };

    enum
    {
        kMaxPage = 5,
        kInfoPerPage = 6,
    };

    static const int kMaxListLine = 6;

    struct ACCEPT_REJECTION_CONTROL
    {        
        CCtrlButtonWZ* accept_control;
        CCtrlButtonWZ* rejection_control;
        int guild_container_index;
        
        ACCEPT_REJECTION_CONTROL()
        {
            accept_control = NULL;
            rejection_control = NULL;
            guild_container_index = -1;
        }
    };
    
public:
    static const DWORD kDialog_ID;

public:
public:
    uiGuildJoinRequestList(InterfaceManager* ui_manager_ptr);
    virtual ~uiGuildJoinRequestList(void);

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual void MessageProc(SI_MESSAGE* message);
    virtual void OnShowWindow(BOOL is_show);
    virtual void OnUpdateSolarDialog();

    void UpdateJoinRequestList();
    void UpdateRemainTime();

private:    
    bool OnLButtonClick(SI_MESSAGE* message);
    bool OnLButtonDClick(SI_MESSAGE* message);
    void OnResultButtonClick(SI_MESSAGE* message);
    void OnClickPrevPageButton();
    void OnClickNextPageButton();
    void SetAcceptAndRejectionButtonShow(const int& index, const ENUM_STATEWZ& is_state, const int& guild_container_index);
    void SetAcceptAndRejectionButtonEnable(const int& index, const bool& is_enable, const int& guild_container_index);
    void ShowCharacterInfo();
    bool IsAccessJoinRequestAcceptAndReject();

private:
    int current_page() const { return current_page_; }
    void set_current_page(const int& page) { current_page_ = page; }
    int get_max_page();

private:
    int current_page_;
    ACCEPT_REJECTION_CONTROL accept_rejection_control[kMaxListLine];
};
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
