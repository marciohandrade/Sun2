#include "SunClientPrecompiledHeader.h"

#include "uiBattleZoneWinDialog.h"
#include "uiBattleZoneMan/uiBattleZoneMan.h"
#include "ResourceManager.h"
#include "IconMgr.h"
#include "IconImage.h"
#include "Hero.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "uiSystemMan/uiSystemMan.h"

//------------------------------------------------------------------------------
WzID uiBattleZoneWinDialog::m_wzId[DIALOG_MAX] = 
{
        StrToWzID("T001"),	//DLGPOS_WIN		//9307	%d win
		StrToWzID("T002"),	//DLGPOS_EXP		//9309	경험치 +%d
		StrToWzID("T003"),	//DLGPOS_MONEY		//9310	하임  + %d
		StrToWzID("BT00"),	//DLGPOS_ITEM		//보상 아이템

		StrToWzID("BT01"),	//DLGPOS_BTN_EXIT 닫기
};

//------------------------------------------------------------------------------
/**
*/
uiBattleZoneWinDialog::uiBattleZoneWinDialog(InterfaceManager *pUIMan) :
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

	for (int i=0; i<POS_TXT_SIZE; ++i)
    {
        this->m_pTxtControls[i] = NULL;
    }

	ZeroMemory(&m_RewardItem, sizeof(m_RewardItem));

}

//------------------------------------------------------------------------------
/**
*/
uiBattleZoneWinDialog::~uiBattleZoneWinDialog()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void uiBattleZoneWinDialog::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

    this->InitControls();
}

//------------------------------------------------------------------------------
/**
*/
void uiBattleZoneWinDialog::Release()
{
	if(m_RewardItem.pImage)
	{
		ICONMGR()->RemoveIcon(m_RewardItem.pImage);
		m_RewardItem.pImage = NULL;
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiBattleZoneWinDialog::InitControls()
{
    for (int i=0; i<POS_TXT_SIZE; ++i)
    {
        this->m_pTxtControls[i] =
            static_cast<CCtrlStaticWZ *>(this->getControl(DLGPOS_WIN + i));
        assert (this->m_pTxtControls[i]);

    }

    this->m_bControlsInitialized = true;
}


//------------------------------------------------------------------------------
void uiBattleZoneWinDialog::OnShowWindow( BOOL val )
{
    if (val)
    {
        SolarDialog::MoveCenter();

        if (!this->m_bControlsInitialized)
        {
            this->ShowInterface(FALSE);
            return;
        }
    
        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = eDoType_dialog_chunting_win;
        msg.wParam = InterfaceManager::DIALOG_BATTLEZONE_CHUNTING_WIN;
        msg.DoSomething = GlobalFunc::DoShowWindow;
        g_KeyQueueManager.PushBack(msg);
	}
    else
    {
        g_KeyQueueManager.DeleteMsg(eDoType_dialog_chunting_win);

		if(m_RewardItem.pImage)
		{
			ICONMGR()->RemoveIcon(m_RewardItem.pImage);
			m_RewardItem.pImage = NULL;
		}
    }
}

//------------------------------------------------------------------------------
/**
*/
BOOL uiBattleZoneWinDialog::_isEnable()
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
void uiBattleZoneWinDialog::_setResult(int iWin, int iExp, int iMoney, CODETYPE dwCode, int iItemCount)
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR	szTemp[INTERFACE_STRING_LENGTH] = {0,};

	Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, _T("%d WIN POINT"), iWin);
	GlobalFunc::SetCaption (m_pTxtControls[DLGPOS_WIN], szMessage);

	g_InterfaceManager.GetInterfaceString( 9309, szTemp, INTERFACE_STRING_LENGTH );
	Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp, iExp);
	GlobalFunc::SetCaption (m_pTxtControls[DLGPOS_EXP], szMessage);

    g_InterfaceManager.GetInterfaceString( 9310, szTemp, INTERFACE_STRING_LENGTH );
	Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp, iMoney);
	GlobalFunc::SetCaption (m_pTxtControls[DLGPOS_MONEY], szMessage);

	// 보상아이템
	m_RewardItem.dwCode = dwCode;
	m_RewardItem.dwCount = iItemCount;
	
	if(m_RewardItem.pImage)
	{
		ICONMGR()->RemoveIcon(m_RewardItem.pImage);
		m_RewardItem.pImage = NULL;
	}

	RESOURCECODE code = 0;

	if(dwCode)
		code = ResourceManager::Instance()->GetItemSlotImageResourceCode(dwCode);

	if (code != 0)
	{
		m_RewardItem.pImage  = ICONMGR()->Add(code);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiBattleZoneWinDialog::ShowResultWin(int iWin, int iExp, int iMoney, CODETYPE dwCode, int iItemCount)
{

	_setResult(iWin, iExp, iMoney, dwCode, iItemCount);

	ShowInterface(TRUE);
}

//------------------------------------------------------------------------------
/**
*/
void uiBattleZoneWinDialog::MessageProc( SI_MESSAGE * pMessage )
{
    if (!_isEnable())
        return;

	switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_LBUTTONCLICK:
        {
            switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
            case DLGPOS_BTN_EXIT:
                {
                    this->ShowInterface(FALSE);
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
void uiBattleZoneWinDialog::OnUpdateSolarDialog()
{
    if (!_isEnable())
        return;
}

//------------------------------------------------------------------------------
/**
*/
void uiBattleZoneWinDialog::OnRenderSolarDialog()
{
	if (!_isEnable())
        return;

	CControlWZ *pControl = getControl(DLGPOS_ITEM);
	if (m_RewardItem.pImage && m_RewardItem.dwCode)
	{
		if (pControl) 
		{
			pControl->ShowWindowWZ(WZ_SHOW);
			RECT rc = pControl->GetSizeRect();
			m_RewardItem.pImage->Render(&rc);
			if(m_RewardItem.dwCount > 1)
			{
				TCHAR szCount[128] = {0,};
				Sprintf(szCount,_T("%d") ,m_RewardItem.dwCount);
				WzColor color = WzColor_RGBA(255,255,255,255);

				rc.top += 20;
				rc.bottom += 20;			
			
				g_pSunRenderer->x_pManagerTextOut->DrawText(szCount,&rc,color);
			}
		}
	}
	else
	{
		if(pControl)
		{
			pControl->ShowWindowWZ(WZ_HIDE);
		}
	}
}


//------------------------------------------------------------------------------
/**
*/
void uiBattleZoneWinDialog::NetworkProc( MSG_BASE * pMsg )
{
	
}
