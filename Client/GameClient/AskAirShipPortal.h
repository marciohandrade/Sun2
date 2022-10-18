#pragma once
#include "askdialog.h"

class AskAirShipPortal:
    public AskDialog
{
public:
    AskAirShipPortal(InterfaceManager* ui_man);
    virtual ~AskAirShipPortal(void);

    bool airship_zone() const { return airship_zone_; }
    void set_airship_zone(bool val) { airship_zone_ = val; }

    WORD portal_index() const { return portal_index_; }
    void set_portal_index(WORD val) { portal_index_ = val; }
    bool AirshipErrorMessage(BYTE error_code);

    void set_air_ship_target_object_key_(const DWORD& air_ship_target_object_key);

protected:
    virtual void OnUpdateSolarDialog();
    virtual void MessageProc(SI_MESSAGE* pMessage);
    virtual VOID OnShowWindow(BOOL val);

    void SetTexts();
    void DoIt();
    void Cancel();

private:
    bool airship_zone_;
    WORD portal_index_;
    MAPCODE map_code_;    
    DWORD air_ship_target_object_key_;
};
