#include "SunClientPrecompiledHeader.h"

#pragma once
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "uiBase.h"

class uiChaosZoneInfo : public uiBase
{
    enum UIControlPos
    {
        kControl_Button_B004 = 0,
        kControl_Text_S001,
        kControl_Text_S003,
        kControl_Text_S004,
        kControl_Text_S005,
        kControl_Picture_P000,
        kControl_Picture_P001,
        kControl_Size,
    };

#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    enum eStaticRect
    {
        eS001Rect = 0,
        eS003Rect,
        eS004Rect,
        eS005Rect,
        eSMaxRect
    };
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH

public:
    static const DWORD kDialog_ID;

public:
    uiChaosZoneInfo(InterfaceManager* ui_manager);
    virtual ~uiChaosZoneInfo();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();

    virtual void OnUpdateSolarDialog();
    virtual void OnRenderSolarDialog();
    virtual void MessageProc(SI_MESSAGE* message);
    virtual void NetworkProc(MSG_BASE* recv_packet);

#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
public:
    void BattleGroundSetting();
    void GoldRushSetting();
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
private:
    bool OnLButtonClick(SI_MESSAGE* message);
    bool OnRButtonClick(SI_MESSAGE* message);
    bool OnMouseOver(SI_MESSAGE* message);

#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    RECT battle_static_control_rect_[eSMaxRect]; // ¿¸¿Â static rect
    RECT gold_static_control_rect_[eSMaxRect];
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
};

#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
