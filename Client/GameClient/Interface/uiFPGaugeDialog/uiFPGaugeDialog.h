#pragma once
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
#include "uiBase.h"

class InterfaceManager;
class uiFPGaugeMan;
class uiToolTipMan;
class uiFPGaugeDialog : public uiBase
{
    enum eFP_CONTROL_POS
    {
        eControl_Picture_P000,
        eControl_Picture_P001,
        eControl_Picture_P002,
        eControl_Picture_P003,
        eControl_Picture_P004,
        eControl_Picture_P005,
        eControl_Picture_P006,
        eControl_Picture_P007,
        eControl_Picture_P008,
        eControl_Picture_P009,
        eControl_Picture_P010,
        eControl_Button_B000,

        eControl_Max,
    };
public:
    uiFPGaugeDialog();
    virtual ~uiFPGaugeDialog(void);

public:
    void            DefaultDialogPos();
    void            SetFPGauge(const PLAYERFP& fp);
    void            ClearFP();
    void            PrintTooltip();

private:
    CControlWZ*     getControl( POSTYPE pos );

public:
    VOID            Init( CDrawBase* draw_base_ptr );
    virtual void    OnUpdateSolarDialog();
    virtual void    Release();

    virtual VOID    MessageProc( SI_MESSAGE* message );
    virtual void    OnShowWindow(BOOL is_show);
    virtual void    MoveDefaultWindowPos();

private:
    uiFPGaugeMan*   ui_fp_gauge_manager_ptr_;
    static WzID     wz_id_[eControl_Max];
    uiToolTipMan*   tooltip_manager_ptr_;
};
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
