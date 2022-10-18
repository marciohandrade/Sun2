#pragma once
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#include "uiBase.h"
#include "TabCheckButtonControlManager.h"

class uiGuildInfoOther : public uiBase
{
    enum UIControlPos
    {
        kControl_TextImage_TI02 = 0,// ��忬��
        kControl_TextImage_I006,    // ����
        kControl_TextImage_I007,    // ��帶����
        kControl_TextImage_I008,    // ��巩ŷ
        kControl_TextImage_I009,    // ��巹��
        kControl_TextImage_I010,    // �������

        kControl_CheckBT_C000,      // ��尡�Կ�û����üũ��ư
        kControl_Text_S004,         // ��尡�Կ�û����
        kControl_Text_S005,         // �������

        kControl_Button_B999,       // �ݱ�
        kControl_Button_BT01,       // ���α�� �� ���Կ�û

        kControl_Picture_PI00,      // ��帶ũ        

        kControl_Size,
    };

public:
    static const DWORD kDialog_ID;

public:
    uiGuildInfoOther(InterfaceManager* ui_manager);
    virtual ~uiGuildInfoOther();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual void MessageProc(SI_MESSAGE* message);
    virtual void OnShowWindow(BOOL is_show);
    virtual void OnUpdateSolarDialog();
    virtual void OnRenderSolarDialog();

    void SetGuildInfo(GUILD_INFO& value);
    void MoveSubDialog();
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    void OnAnswerJoinRequest();
    void OnClickRequestJoin();
    void SetJoinRequestButton(const eGUILD_JOIN_REQUEST& guild_join_request_state);
    void RequestJoinButtonReset();
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

    void set_guild_rank_index(int value) { guild_rank_index_ = value; }

private:
    bool OnLButtonClick(SI_MESSAGE* message);
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#else
    void OnClickRequestJoin();
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

    void UpdateGuildInfo();
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#else
    void UpdateButtonState();
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

    int guild_rank_index() const { return guild_rank_index_; }

private:
    bool show_flag_;
    int guild_rank_index_;
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    eGUILD_JOIN_REQUEST guild_join_request_state_;
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
};

#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL