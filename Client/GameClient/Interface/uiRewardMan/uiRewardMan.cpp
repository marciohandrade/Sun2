#include "SunClientPrecompiledHeader.h"
#include "uirewardman.h"
#include "interfacemanager.h"
#include "RewardDialog.h"
uiRewardMan::uiRewardMan(InterfaceManager *pUIMan):
uiBaseMan(pUIMan)
{
    m_pRewardDlg = NULL;
}

void uiRewardMan::OnInitialize()
{
    m_pRewardDlg = new RewardDialog;

    if( !g_InterfaceManager.LoadInterface( 
        m_pRewardDlg, 
        "Data\\Interface\\83_Mission_Reward.iwz", 
        StrToWzID("0830"), 
        FALSE ) )
    {

    }

    m_pRewardDlg->SetDialogKey( StrToWzID("0830") );
}

