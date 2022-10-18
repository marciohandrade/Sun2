#include "SunClientPrecompiledHeader.h"
#include "uiquickcontainerman.h"
#include "interfacemanager.h"
#include "QuickContainer.h"
#include "StyleContainer.h"

const DWORD IM_QUICK_CONTAINER_MANAGER::QUICK_CONTAINER = StrToWzID("0230");
const DWORD IM_QUICK_CONTAINER_MANAGER::STYLE_CONTAINER = StrToWzID("0290");


uiQuickContainerMan::uiQuickContainerMan(InterfaceManager *pUIMan):
uiBaseMan(pUIMan)
{
    m_pQuickDlg = (NULL);
    m_pStyleDlg = NULL;
}

void uiQuickContainerMan::OnInitialize()
{
    m_pQuickDlg = new QuickContainer();
    if( !g_InterfaceManager.LoadInterface( 
        m_pQuickDlg, 
        "Data\\Interface\\23_QuickSlot.iwz", 
        StrToWzID("0230"), 
        FALSE ) )
    {

    }

    m_pQuickDlg->SetDialogKey( StrToWzID("0230") );
    m_pQuickDlg->SetDialogResourceName( "Data\\Interface\\23_QuickSlot.iwz" );
}

//------------------------------------------------------------------------------
/**
*/

void	uiQuickContainerMan::SetQuickContainerCurIndex(int iIndex)
{
	m_pQuickDlg->SetCurQuickSlot(iIndex);
}

//------------------------------------------------------------------------------
/**
*/
void	uiQuickContainerMan::PrevQuickContainerTab()
{
	m_pQuickDlg->PrevQuickTab();
}

//------------------------------------------------------------------------------
/**
*/
void	uiQuickContainerMan::NextQuickContainerTab()
{
	m_pQuickDlg->NextQuickTab();
}

void uiQuickContainerMan::ToggleSlot()
{
    if (m_pQuickDlg != NULL)
    {
        m_pQuickDlg->ToggleSlot();
    }
}

void uiQuickContainerMan::ToggleChangeSlotSize()
{
    if (m_pQuickDlg != NULL)
    {
        m_pQuickDlg->ToggleChangeSlotSize();
    }
}
