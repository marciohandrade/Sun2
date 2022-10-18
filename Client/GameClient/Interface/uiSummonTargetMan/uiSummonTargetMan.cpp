#include "SunClientPrecompiledHeader.h"
#include "uisummontargetman.h"
#include "interfacemanager.h"
#include "SummonTargetDialog.h"
#include "Application.h"

const DWORD IM_SUMMON_TARGET_MANAGER::SUMMON_TARGET_DIALOG = StrToWzID("0300");

uiSummonTargetMan::uiSummonTargetMan(InterfaceManager *pUIMan):
uiBaseMan(pUIMan)
{
    m_pSummonTargetDlg = NULL;	
}

void uiSummonTargetMan::OnInitialize()
{
    m_pSummonTargetDlg = CreateUIDialog<SummonTargetDialog>(IM_SUMMON_TARGET_MANAGER::SUMMON_TARGET_DIALOG,
        "Data\\Interface\\30_Summon.iwz",
        this, TRUE);

    assert(m_pSummonTargetDlg);
}


//------------------------------------------------------------------------------
/**
*/
void uiSummonTargetMan::MoveSummonTargetDialog(int value)
{
    int gap_value = 0;
    if (value == 1)
    {
        gap_value = c_iOneLineYPostionToQuickSlot;
    }
    else// value = 2
    {
        gap_value = c_iTwoLineYPostionToQuickSlot;
    }

    if (m_pSummonTargetDlg != NULL)
    {
        POINT pt = g_pApplication->Get2DModeMapping();
        RECT rcClient;
        POINT my_point;
        m_pSummonTargetDlg->GetDialogWZ()->GetClientRect(&rcClient);
        my_point.x = (pt.x - (rcClient.right - rcClient.left))/2;
        my_point.y = pt.y - (rcClient.bottom - rcClient.top + gap_value + 11);
        m_pSummonTargetDlg->MoveWindowWZ((float)my_point.x, (float)my_point.y);
    }
}
