#include "SunClientPrecompiledHeader.h"

#include "uiOptionMan.h"
#include "uiOptionView.h"
#include "uiOptionKeyBinding.h"
#include "InterfaceManager.h"

//------------------------------------------------------------------------------ 
const DWORD IM_OPTION_MANAGER::kOptionView_ID = StrToWzID("0451");
const DWORD IM_OPTION_MANAGER::kOptionKeyBindingView_ID = StrToWzID("0456");

//------------------------------------------------------------------------------ 
uiOptionMan::uiOptionMan(InterfaceManager* manager) : 
uiBaseMan(manager)
,tap_Page_(0)

{
}
//------------------------------------------------------------------------------ 
void uiOptionMan::OnInitialize()
{
    CreateUIDialog<uiOptionView>(IM_OPTION_MANAGER::kOptionView_ID,  "Data\\Interface\\45_1_Game_Option.iwz", this, TRUE);
    CreateUIDialog<uiOptionKeyBinding>(IM_OPTION_MANAGER::kOptionKeyBindingView_ID,  "Data\\Interface\\45_6_Keybinding.iwz", this, TRUE);
}

//------------------------------------------------------------------------------ 
void uiOptionMan::NetworkProc(MSG_BASE* pk)
{

}

void uiOptionMan::ShowOptionDialog( int ActiveTabIndex )
{
    if( g_InterfaceManager.IsShowDialog( IM_OPTION_MANAGER::kOptionKeyBindingView_ID ) )
    {
        SolarDialog* pOptionViewDialog = g_InterfaceManager.GetDialog( IM_OPTION_MANAGER::kOptionView_ID );
        SolarDialog* pKeyBindingDialog = g_InterfaceManager.GetDialog( IM_OPTION_MANAGER::kOptionKeyBindingView_ID);

        if( pOptionViewDialog == NULL || pKeyBindingDialog == NULL )
            return;

        CDialogWZ* pOptionViewDialogWZ = pOptionViewDialog->GetDialogWZ();
        CDialogWZ* pKeyBindingDialogWZ = pKeyBindingDialog->GetDialogWZ();

        RECT ClientRect;
        pKeyBindingDialogWZ->GetClientRect(&ClientRect);
        pOptionViewDialogWZ->MoveWindowWZ((float)ClientRect.left, (float)ClientRect.top, (float)ClientRect.right - (float)ClientRect.left, (float)ClientRect.bottom - (float)ClientRect.top );
    }

    g_InterfaceManager.ShowDialog( IM_OPTION_MANAGER::kOptionView_ID, TRUE );
    g_InterfaceManager.ShowDialog( IM_OPTION_MANAGER::kOptionKeyBindingView_ID, FALSE );

    uiOptionView* pViewDialog = (uiOptionView*)g_InterfaceManager.GetDialog( IM_OPTION_MANAGER::kOptionView_ID );
    pViewDialog->SetActivePageIndex(ActiveTabIndex);

	uiOptionMan* optionMan = (uiOptionMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_OPTION2);

	if(optionMan)
	{
		optionMan->tap_Page_ = 0;
	}
}

void uiOptionMan::ShowKeyBindingDialog()
{
    if( g_InterfaceManager.IsShowDialog( IM_OPTION_MANAGER::kOptionView_ID ) )
    {
        SolarDialog* pOptionViewDialog = g_InterfaceManager.GetDialog( IM_OPTION_MANAGER::kOptionView_ID );
        SolarDialog* pKeyBindingDialog = g_InterfaceManager.GetDialog( IM_OPTION_MANAGER::kOptionKeyBindingView_ID);

        if( pOptionViewDialog == NULL || pKeyBindingDialog == NULL )
            return;

        CDialogWZ* pOptionViewDialogWZ = pOptionViewDialog->GetDialogWZ();
        CDialogWZ* pKeyBindingDialogWZ = pKeyBindingDialog->GetDialogWZ();

        RECT ClientRect;
        pOptionViewDialogWZ->GetClientRect(&ClientRect);
        pKeyBindingDialogWZ->MoveWindowWZ((float)ClientRect.left, (float)ClientRect.top, (float)ClientRect.right - (float)ClientRect.left, (float)ClientRect.bottom - (float)ClientRect.top );
    }

    g_InterfaceManager.ShowDialog( IM_OPTION_MANAGER::kOptionView_ID, FALSE );
    g_InterfaceManager.ShowDialog( IM_OPTION_MANAGER::kOptionKeyBindingView_ID, TRUE );

	uiOptionMan* optionMan = (uiOptionMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_OPTION2);

	if(optionMan)
	{
		optionMan->tap_Page_ = 1;
	}
}

void uiOptionMan::ToggleShowActionTap()
{
	uiOptionMan* optionMan = (uiOptionMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_OPTION2);

	if(optionMan == NULL)
	{
		return;
	}

	//3번 즉 키맵의 경우에만 예외처리
	SolarDialog* pKeyBindingDialog = g_InterfaceManager.GetDialog( IM_OPTION_MANAGER::kOptionKeyBindingView_ID);

	if(pKeyBindingDialog && optionMan->tap_Page_ == 1)
	{
		if(pKeyBindingDialog->IsVisible())
		{
			pKeyBindingDialog->ShowDialog(FALSE);
		}
		else
		{
			pKeyBindingDialog->ShowDialog(TRUE);
		}
		return;
	}
		

	uiOptionView* pOptionViewDialog = (uiOptionView*)g_InterfaceManager.GetDialog( IM_OPTION_MANAGER::kOptionView_ID );

	if(pOptionViewDialog)
	{
		if(pOptionViewDialog->IsVisible())
		{
			pOptionViewDialog->ShowDialog(FALSE);
		}
		else
		{
			pOptionViewDialog->ShowDialog(TRUE);
			pOptionViewDialog->SetActivePageIndex(pOptionViewDialog->GetPageIndex());
		}
	}
}