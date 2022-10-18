#include "SunClientPrecompiledHeader.h"
#include "uisummonskillman.h"
#include "interfacemanager.h"
#include "SummonSkillContainer.h"
#include "Application.h"

uiSummonSkillMan::uiSummonSkillMan(InterfaceManager *pUIMan):
uiBaseMan(pUIMan)
{
    m_pSummonSkillDlg = NULL;
}


//------------------------------------------------------------------------------
/**
*/
void	uiSummonSkillMan::OnInitialize()
{
#ifdef _NA_000000_20130521_NOT_SUMMON_SKILL
#else
    m_pSummonSkillDlg = new SummonSkillContainer();
    if( !g_InterfaceManager.LoadInterface( 
        m_pSummonSkillDlg, 
        "Data\\Interface\\30_1_summon_slot.iwz", 
        StrToWzID("0301"), 
        FALSE ) )
    {

    }

	m_pSummonSkillDlg->SetDialogKey( StrToWzID("0301") );
#endif
}

void uiSummonSkillMan::MoveSummonSkillDialog(int value)
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

    if (m_pSummonSkillDlg != NULL)
    {
        POINT pt = g_pApplication->Get2DModeMapping();
        RECT rcClient;
        POINT my_point;
        m_pSummonSkillDlg->GetDialogWZ()->GetClientRect(&rcClient);
        my_point.x = (pt.x - (rcClient.right - rcClient.left))/2;
        my_point.y = pt.y - (rcClient.bottom - rcClient.top + gap_value + 5);
        m_pSummonSkillDlg->MoveWindowWZ((float)my_point.x, (float)my_point.y);
    }
}
