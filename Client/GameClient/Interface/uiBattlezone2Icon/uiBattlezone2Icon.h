#pragma once
#ifdef _DH_BATTLEZONE2_
#include "uiBase.h"
#include "DropdownControl.h"

class uiBattlezone2Man;
class uiBattlezone2Reservation;
class uiBattlezone2Icon : public uiBase
{
    enum UIControlPos
    {
        kControl_Picture_P002 = 0,//클릭시
        kControl_Picture_P001,//오버시
        kControl_Picture_P000,//평상시
        kControl_Picture_P003,//평상시활성화
        kControl_Picture_P004,//오버시
        kControl_Picture_P005,//평상시
        kControl_Picture_P006,//평상시활성화
        kControl_Size,
    };

    enum Icon_Number
    {
        kIcon_Battlezone = 0,
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
        kIcon_BattleRoom,//전장
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
        kIcon_Max,
    };
public:
    uiBattlezone2Icon(InterfaceManager* ui_manager);
    virtual ~uiBattlezone2Icon(void);

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual void OnUpdateSolarDialog();
    virtual void MessageProc(SI_MESSAGE* message);
    virtual void MouseOver_MsgProc(SI_MESSAGE* message);
    virtual void OnShowWindow(BOOL is_show);
    virtual void Process(DWORD tick_count);
    void RegisterControl();
    uiBattlezone2Man* GetManager();
private:
    bool OnLButtonClick(SI_MESSAGE* message);
    void ViewIcon(BYTE index);
    BYTE GetMouseoverIconIndex();
    RECT is_first_click_rect_;
    bool is_activate_;
    bool is_over_;
    uiBattlezone2Man* ui_battlezone2_manager_;
    CCtrlPictureWZ* pictures_[kControl_Size];
    CTimerSimple animation_refresh_timer_;
    uiBattlezone2Reservation* battlezone_reservation_dialog_;
    RECT is_icon_rect_[kIcon_Max];
};
#endif//_DH_BATTLEZONE2_