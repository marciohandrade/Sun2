#include "SunClientPrecompiledHeader.h"

#include "uiBattleZoneDoing.h"
#include "uiBattleZoneMan/uiBattleZoneMan.h"
#include "Hero.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "uiSystemMan/uiSystemMan.h"
#include "uiRadarMan/uiRadarMan.h"



//------------------------------------------------------------------------------
WzID uiBattleZoneDoing::m_wzId[DIALOG_MAX] = 
{
        StrToWzID("B001"),	//DLGPOS_BLUE
		StrToWzID("B002"),	//DLGPOS_RED
};

//------------------------------------------------------------------------------
/**
*/
uiBattleZoneDoing::uiBattleZoneDoing(InterfaceManager *pUIMan) :
    uiBase(pUIMan),
    m_bControlsInitialized(false)
{
    this->m_pBattleZoneMan =
        static_cast<uiBattleZoneMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_BATTLEZONE));
    assert (m_pBattleZoneMan);

    if (!this->m_pBattleZoneMan)
    {
#ifdef _DEBUG

		uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
		if(pSystemMan)
			pSystemMan->Sys_VerifyLock(_T("m_pBattleZoneMan이 없습니다."));

#endif
        this->ShowInterface(FALSE);
        return;
    }

	m_iHomeKillPoint = 0;
	m_iAwayKillPoint = 0;

    for (int i=0; i<POS_BTN_SIZE; ++i)
    {
        this->m_pBtnControls[i] = NULL;
    }
}

//------------------------------------------------------------------------------
/**
*/
uiBattleZoneDoing::~uiBattleZoneDoing()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void uiBattleZoneDoing::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

    this->InitControls();
}

//------------------------------------------------------------------------------
/**
*/
void uiBattleZoneDoing::Release()
{
}

//------------------------------------------------------------------------------
/**
*/
void uiBattleZoneDoing::InitControls()
{
	for (int i=0; i<POS_BTN_SIZE; ++i)
    {
        this->m_pBtnControls[i] =
            static_cast<CCtrlButtonWZ *>(this->getControl(DLGPOS_BLUE + i));
        assert (this->m_pBtnControls[i]);

		m_pBtnControls[i]->ShowWindowWZ(WZ_HIDE);
    }
    
	if (m_pBtnControls[DLGPOS_BLUE])
	{
		m_rcInit = m_pBtnControls[DLGPOS_BLUE]->GetSize();
	}

	m_WinPos.left = 0;
	m_WinPos.right = 0;
	m_WinPos.top = 0;
	m_WinPos.bottom = 0;

    this->m_bControlsInitialized = true;
}


//------------------------------------------------------------------------------
void uiBattleZoneDoing::OnShowWindow( BOOL val )
{
    if (val)
    {
        if (!this->m_bControlsInitialized)
        {
            this->ShowInterface(FALSE);
            return;
		}

		RECT rcMain;
		GetDialogWZ()->GetClientRect(&rcMain);
		int width = rcMain.right - rcMain.left;

		if(m_WinPos.left == 0)
		{
			if (GET_DIALOG(IM_RADAR_MANAGER::RADAR_DIALOG_PLUS)->IsVisible())
			{
				uiBase* pDlg = GET_DIALOG(IM_RADAR_MANAGER::RADAR_DIALOG_PLUS);
				pDlg->GetDialogWZ()->GetClientRect(&m_WinPos);
				GetDialogWZ()->MoveWindowWZ((float)(m_WinPos.left), (float)(m_WinPos.bottom + 25)); 

			}
			else if (GET_DIALOG(IM_RADAR_MANAGER::RADAR_DIALOG)->IsVisible())
			{
				uiBase* pDlg = GET_DIALOG(IM_RADAR_MANAGER::RADAR_DIALOG);
				pDlg->GetDialogWZ()->GetClientRect(&m_WinPos);
				GetDialogWZ()->MoveWindowWZ((float)(m_WinPos.left), (float)(m_WinPos.bottom + 25)); 
			}
			else
			{
				return;
			}

		}
		else
		{
			GetDialogWZ()->MoveWindowWZ((float)(m_WinPos.left), (float)(m_WinPos.bottom + 10)); 
		}
	}
    else
    {
        m_iHomeKillPoint = 0;
		m_iAwayKillPoint = 0;
	
    }
}

//------------------------------------------------------------------------------
/**
*/
BOOL uiBattleZoneDoing::_isEnable()
{
	if(GENERALPARAM->IsNet())
	{
		if (this->IsLock())
			return FALSE;

		if(!g_pHero)
			return FALSE;

		if( g_pHero && g_pHero->IsShowFullMap() )
			return FALSE;

		if(!GetManager())
			return FALSE;

		return TRUE;
	}
	else
	{
		return TRUE;
	}

}

//------------------------------------------------------------------------------
/**
*/
void uiBattleZoneDoing::MessageProc( SI_MESSAGE * pMessage )
{
    if (!_isEnable())
        return;
}

//------------------------------------------------------------------------------
/**
*/
void uiBattleZoneDoing::OnUpdateSolarDialog()
{
    if (!_isEnable())
        return;
}

//------------------------------------------------------------------------------
/**
*/
void uiBattleZoneDoing::OnRenderSolarDialog()
{
	if (!_isEnable())
        return;

	//비율정하기
	float fTotal = (float)m_iHomeKillPoint + (float)m_iAwayKillPoint;

    for (int i=0; i<POS_BTN_SIZE; ++i)
    {
        m_pBtnControls[i]->ShowWindowWZ(WZ_SHOW);
    }

	if(m_iHomeKillPoint <= 0)
	{
		m_pBtnControls[DLGPOS_BLUE]->ShowWindowWZ(WZ_HIDE);
	}
	
	if(m_iAwayKillPoint <= 0)
	{
		m_pBtnControls[DLGPOS_RED]->ShowWindowWZ(WZ_HIDE);
	}

	RECT rcGauge = this->m_rcInit;
	float fGaugeSize = (float)rcGauge.right - (float)rcGauge.left;
	float width, height, width2;
	height = (float)rcGauge.bottom - (float)rcGauge.top;
	LONG LeftGap = 10;

	RECT rc;
	GetDialogWZ()->GetClientRect(&rc);

	float fBlueRate = 0;
	float fRedRate =0;
	if(m_iHomeKillPoint>0)
	{
		fBlueRate = (m_iHomeKillPoint/fTotal) * 100.f;

		width = (fGaugeSize/100.f) * fBlueRate;
		m_pBtnControls[DLGPOS_BLUE]->MoveWindowWZ( (float)(rc.left - LeftGap), (float)rc.top, width, height );
	}
	
	if(m_iAwayKillPoint>0)
	{
		fRedRate = (m_iAwayKillPoint/fTotal) * 100.f;

		width2 = (fGaugeSize/100.f) * fRedRate;
		
		float fGapX = fGaugeSize - width2;

		m_pBtnControls[DLGPOS_RED]->MoveWindowWZ( (float)(rc.left - LeftGap + fGapX), (float)rc.top, width2, height );
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiBattleZoneDoing::SetKillPoint(int iHomeKillPoint, int iAwayKillPoint)
{
	m_iHomeKillPoint = iHomeKillPoint;
	m_iAwayKillPoint = iAwayKillPoint;
}

//------------------------------------------------------------------------------
/**
*/
void uiBattleZoneDoing::NetworkProc( MSG_BASE * pMsg )
{
	
}

