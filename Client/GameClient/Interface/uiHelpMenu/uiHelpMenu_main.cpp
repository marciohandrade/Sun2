#include "SunClientPrecompiledHeader.h"

#include "uiHelpMenu.h"
#include "uiHelpMan/uiHelpMan.h"

#include "RadarInfoParser.h"
#include "radardialog.h"
#include "Hero.h"
#include "interfacemanager.h"
#include "uiSystemMan/uiSystemMan.h"
#include "uiBattleZoneMan/uiBattleZoneMan_def.h"
#include "uiRadarMan/uiRadarMan.h"

//------------------------------------------------------------------------------
WzID uiHelpMenu::m_wzId[DIALOG_MAX] = 
{
	StrToWzID("B003"),	//DLGPOS_BTN_ICON1,
	StrToWzID("B002"),	//DLGPOS_BTN_ICON2,
	StrToWzID("B001"),	//DLGPOS_BTN_ICON3,
};

//------------------------------------------------------------------------------
/**
*/
uiHelpMenu::uiHelpMenu(InterfaceManager *pUIMan) :
    uiBase(pUIMan),
    m_bControlsInitialized(false)
{
    this->m_pHelpMan =
        static_cast<uiHelpMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_HELP));
    assert (m_pHelpMan);

    if (!this->m_pHelpMan)
    {
#ifdef _DEBUG

		uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
		if(pSystemMan)
			pSystemMan->Sys_VerifyLock(_T("m_pHelpMan이 없습니다."));

#endif
        this->ShowInterface(FALSE);
        return;
    }

    memset ((CCtrlButtonWZ *)this->m_pBtnControls, NULL, sizeof (this->m_pBtnControls));
}

//------------------------------------------------------------------------------
/**
*/
uiHelpMenu::~uiHelpMenu()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void uiHelpMenu::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

    this->InitControls();
}

//------------------------------------------------------------------------------
/**
*/
void uiHelpMenu::Release()
{
}

//------------------------------------------------------------------------------
/**
*/
void uiHelpMenu::InitControls()
{
    for (int i=0; i<POS_BTN_SIZE; ++i)
    {
        this->m_pBtnControls[i] =
				static_cast<CCtrlButtonWZ *>(this->getControl(DLGPOS_BTN_ICON1 + i));
        assert (this->m_pBtnControls[i]);

        this->m_pBtnControls[i]->ShowWindowWZ(WZ_HIDE);
		
    }

    this->m_bControlsInitialized = true;
}


//------------------------------------------------------------------------------
void uiHelpMenu::OnShowWindow( BOOL val )
{
    if (val)
    {
        if (!this->m_bControlsInitialized)
        {
            this->ShowInterface(FALSE);
            return;
        }

		//위치 잡아주기
        UpdatePosition();
    }
    
}

//------------------------------------------------------------------------------
/**
*/
void uiHelpMenu::UpdatePosition()
{
    ////SolarDialog::UpdatePosition();


	RECT rectRadar;
	if (GET_DIALOG(IM_RADAR_MANAGER::RADAR_DIALOG)->IsVisible())
	{
		uiBase* pDlg = GET_DIALOG(IM_RADAR_MANAGER::RADAR_DIALOG);
		pDlg->GetDialogWZ()->GetClientRect( &rectRadar );

		RECT rc;
		GetDialogWZ()->GetClientRect(&rc);
		int width = rc.right - rc.left;
		GetDialogWZ()->MoveWindowWZ((float)(rectRadar.left - width - 3), (float)(rectRadar.top - 3)); 
	}
	else if (GET_DIALOG(IM_RADAR_MANAGER::RADAR_DIALOG_PLUS)->IsVisible())
	{
		uiBase* pDlg = GET_DIALOG(IM_RADAR_MANAGER::RADAR_DIALOG_PLUS);
		pDlg->GetDialogWZ()->GetClientRect(&rectRadar);

		RECT rc;
		GetDialogWZ()->GetClientRect(&rc);
		int width = rc.right - rc.left;
		GetDialogWZ()->MoveWindowWZ((float)(rectRadar.left - width - 3), (float)(rectRadar.top - 3)); 
		
	}
}	

//------------------------------------------------------------------------------
/**
*/
void uiHelpMenu::MessageProc( SI_MESSAGE * pMessage )
{
	if(!_isEnable())
		return;

	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};

	int iDlgPos = getCtrlIDToPos( pMessage->dwCtrlID );

    switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_LBUTTONCLICK:
        {
            switch( iDlgPos )
            {
            case DLGPOS_BTN_ICON1:
			case DLGPOS_BTN_ICON2:
			case DLGPOS_BTN_ICON3:
				{
            		GetManager()->ShowHelpWindow(iDlgPos);
				}
                break;
            }
        }
        break;

	default:
		{
			SolarDialog::MessageProc( pMessage );
		}
		break;
    }
}

//------------------------------------------------------------------------------
/**
*/
BOOL uiHelpMenu::_isEnable()
{
	if (this->IsLock())
        return FALSE;

	if(!g_pHero)
			return FALSE;

	if( g_pHero && g_pHero->IsShowFullMap() )
		return FALSE;

	if(!GetManager())
		return FALSE;

	if(!GAMEOPTION->GetShowHelp())
	{
		
		if(this->IsVisible())
		{
			ShowInterface(FALSE);
		}

		return FALSE;
	}


	if( GET_DIALOG( IM_BATTLEZONE_MANAGER::BATTLEZONE_CREATE )->IsVisible() ||
        GET_DIALOG( IM_BATTLEZONE_MANAGER::BATTLEZONE_CREATE_EP2 )->IsVisible() ||
		GET_DIALOG( IM_BATTLEZONE_MANAGER::BATTLEZONE_LIST )->IsVisible() ||
		this->GetUIMan()->IsShowDialog( InterfaceManager::DIALOG_BATTLE_LOBBY ))
	{
		if(this->IsVisible())
		{
			ShowInterface(FALSE);
		}

        return FALSE;
	}


	return TRUE;
}


//------------------------------------------------------------------------------
/**
*/
void uiHelpMenu::OnRenderSolarDialog()
{
	if(!_isEnable())
		return;

	int iHelpCnt = GetManager()->m_vecNewHelp.size();

	if(iHelpCnt > POS_BTN_SIZE) 
		iHelpCnt = POS_BTN_SIZE;

	for (int i=0; i<iHelpCnt; ++i)
	{
		this->m_pBtnControls[i]->ShowWindowWZ(WZ_SHOW);
	}
	
	for(int i=iHelpCnt; i<POS_BTN_SIZE; ++i)
	{
		this->m_pBtnControls[i]->ShowWindowWZ(WZ_HIDE);
	}
}


//------------------------------------------------------------------------------
/**
*/
void uiHelpMenu::NetworkProc( MSG_BASE * pMsg )
{
	
}

//------------------------------------------------------------------------------
/**
*/
void uiHelpMenu::Process(DWORD dwTick)
{

}