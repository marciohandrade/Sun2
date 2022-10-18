#pragma once
#ifdef _DH_BATTLEZONE2_
#include "uibase.h"

class uiBattlezone2Man;
class uiBattlezone2Password: public uiBase
{
    enum UIControlPos
    {
        DLGPOS_EDT_PASSWORD=0,
        DLGPOS_BTN_OK,
        DLGPOS_BTN_CANCEL,
        //kControl_Size,
    };

public:
    uiBattlezone2Password(InterfaceManager* ui_manager);
    virtual ~uiBattlezone2Password(void);

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual void OnUpdateSolarDialog();
    virtual void MessageProc(SI_MESSAGE* message);
    virtual void MouseOver_MsgProc(SI_MESSAGE* message);
    virtual void OnShowWindow(BOOL is_show);
    virtual void Process(DWORD tick_count);
    void RegisterControl();
    uiBattlezone2Man* GetManager();

    WORD save_party_key(){ return save_party_key_; }
    void set_save_party_key(WORD val){ save_party_key_ = val; }
    void set_party_info(PartyRoomInfo* val){ set_party_info_ = val; }
private:
    bool OnLButtonClick(SI_MESSAGE* message);
    uiBattlezone2Man* ui_battlezone2_manager_;

//    CCtrlStaticWZ *         getCtrlText(POSTYPE pos);
 
    util::_tstring             m_strPassword;
    WORD save_party_key_;
    PartyRoomInfo* set_party_info_;
};
#endif//_DH_BATTLEZONE2_