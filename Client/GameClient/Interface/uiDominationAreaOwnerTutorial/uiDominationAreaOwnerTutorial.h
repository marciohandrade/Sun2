#pragma once

#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
#include "uiBase.h"
#ifdef _NA_007256_20140613_AREAOWNER_ALLIENCE_GUILD_NAME_SHOW
#include "FTextRender.h"
#endif // _NA_007256_20140613_AREAOWNER_ALLIENCE_GUILD_NAME_SHOW

class uiDominationTowerMan;
class uiDominationAreaOwnerTutorial : public uiBase
{
private:
    enum UIControlPos
    {
        eControl_Picture_P002,
        eControl_Button_BT01,
        eControl_Text_S000,
        eControl_Text_S001,
#ifdef _NA_007256_20140613_AREAOWNER_ALLIENCE_GUILD_NAME_SHOW
        eControl_Text_S002,
#endif // _NA_007256_20140613_AREAOWNER_ALLIENCE_GUILD_NAME_SHOW
        eControl_Max
    };

public:
    uiDominationAreaOwnerTutorial(InterfaceManager* ui_manager_ptr);
    virtual ~uiDominationAreaOwnerTutorial(void);

    virtual void Init(CDrawBase* draw_base_ptr);
    virtual void Release();

    virtual void MessageProc(SI_MESSAGE* message);
    virtual void OnRenderSolarDialog();
    virtual void OnShowWindow(BOOL val);

public:
#ifdef _NA_007256_20140613_AREAOWNER_ALLIENCE_GUILD_NAME_SHOW
    void SetAreaOwnerGuildName(TCHAR* area_owner_guild_name, 
        const sDOMINATION_CONTINENT_LORD_ALLIENCE_INFO* allience_guild_infos = NULL, const BYTE& allience_guild_count = 0);
#else
    void SetAreaOwnerGuildName(TCHAR* area_owner_guild_name);
#endif // _NA_007256_20140613_AREAOWNER_ALLIENCE_GUILD_NAME_SHOW

private:
    bool OnLButtonClick(SI_MESSAGE* message);
    void RegisterControl();

    uiDominationTowerMan* ui_domination_manager_ptr_;

    CCtrlStaticWZ* explanation_control_ptr_;
#ifdef _NA_007256_20140613_AREAOWNER_ALLIENCE_GUILD_NAME_SHOW
    FTextRender text_render_;
    DWORD scroll_alpha_value_;
    CTimerSimple scroll_alpha_timer_;
    CCtrlStaticWZ* allience_text_control_;
#endif // _NA_007256_20140613_AREAOWNER_ALLIENCE_GUILD_NAME_SHOW
};

#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL
