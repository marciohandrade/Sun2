#pragma once
#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
#include "uibase.h"

class uiChaosZonePassword : public uiBase
{
    enum UIControlPos
    {
        DLGPOS_EDT_PASSWORD=0,
        DLGPOS_BTN_OK,
        DLGPOS_BTN_CANCEL,
        //kControl_Size,
    };

public:
    uiChaosZonePassword(InterfaceManager* ui_manager);
    virtual ~uiChaosZonePassword(void);

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual void OnShowWindow(BOOL is_show);
    virtual void OnUpdateSolarDialog();
    virtual void MessageProc(SI_MESSAGE* message);
    virtual void MouseOver_MsgProc(SI_MESSAGE* message);
    
    virtual void Process(DWORD tick_count);
    void RegisterControl();
    
    KEYTYPE room_key(){ return room_key_; }
    void set_room_key(KEYTYPE room_key){ room_key_ = room_key; }
    
private:
    bool OnLButtonClick(SI_MESSAGE* message);

    KEYTYPE room_key_;
};
#endif//_NA_006088_20120910_CHAOS_ZONE_FREE_PVP
