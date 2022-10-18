#pragma once
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
#include "uiBaseMan.h"

struct IM_FP_GAUGE_MANAGER
{
    static const DWORD FP_GAUGE_DIALOG;
};

class uiFPGaugeDialog;
class uiFPGaugeMan : public uiBaseMan
{
public:
    uiFPGaugeMan(InterfaceManager* ui_manager_ptr);
    virtual ~uiFPGaugeMan(void);

public:
    virtual void         OnInitialize();

private:
    uiFPGaugeDialog*     ui_fp_gauge_dialog_ptr_;
};
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
