#include "SunClientPrecompiledHeader.h"
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
#include "uiFPGaugeMan.h"
#include "uiFPGaugeDialog/uiFPGaugeDialog.h"

const DWORD IM_FP_GAUGE_MANAGER::FP_GAUGE_DIALOG = StrToWzID("0215");

uiFPGaugeMan::uiFPGaugeMan(InterfaceManager* ui_manager_ptr) : 
uiBaseMan(ui_manager_ptr)
{
}

uiFPGaugeMan::~uiFPGaugeMan(void)
{
}

void uiFPGaugeMan::OnInitialize()
{
    ui_fp_gauge_dialog_ptr_ = new uiFPGaugeDialog;

    if (!g_InterfaceManager.LoadInterface(
        ui_fp_gauge_dialog_ptr_,
        "Data\\Interface\\21_5_Wich_FP.iwz",
        StrToWzID("0215"),
        FALSE ) )
    {
        return;
    }

    if (ui_fp_gauge_dialog_ptr_)
    {
        ui_fp_gauge_dialog_ptr_->SetDialogResourceName("Data\\Interface\\21_5_Wich_FP.iwz");
        ui_fp_gauge_dialog_ptr_->SetDialogKey(StrToWzID("0215"));
    }
}
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
