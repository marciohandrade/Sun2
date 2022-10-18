#include "SunClientPrecompiledHeader.h"

#include "chaosystemdialog.h"
#include "SceneBase.h"
#include "Application.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "Mouse.h"
#include "Hero.h"


WzID ChaoSystemDialog::m_wzId[CHAOSYSTEM_DIALOG_MAX] =
{
	StrToWzID("T001"),
	StrToWzID("PI00"),
	StrToWzID("PI01"),
};

ChaoSystemDialog::ChaoSystemDialog(void)
{
	m_pStatic  = NULL;
	m_pChaoPicture = NULL;
	m_pFreeChaoPicture = NULL;
	m_fPictureHeight = 0.0f;
}


ChaoSystemDialog::~ChaoSystemDialog(void)
{

}

void ChaoSystemDialog::Release()
{
}


void ChaoSystemDialog::Init( CDrawBase * pDrawBase )
{
	m_pStatic = static_cast< CCtrlStaticWZ*>(getControl(CHAOSYSTEM_TIME));
	m_pChaoPicture = static_cast<CCtrlPictureWZ*>(getControl(CHAOSYSTEM_CHAO_PICTURE));
	m_pFreeChaoPicture = static_cast<CCtrlPictureWZ*>(getControl(CHAOSYSTEM_FREECHAO_PICTURE));

	m_pChaoPicture->ShowWindowWZ(WZ_HIDE);
	m_pFreeChaoPicture->ShowWindowWZ(WZ_HIDE);

	m_rtPicture = m_pChaoPicture->GetSizeRect();
	m_rtSizePicture = m_pChaoPicture->GetSize();

	this->m_fPictureWidth = (float)m_rtPicture.right -m_rtPicture.left;
	this->m_fPictureHeight = (float)m_rtSizePicture.bottom - m_rtSizePicture.top ;
	m_bMouseOver = FALSE;
	m_iTickSum   =0;
	m_dwTime=0;
}

void ChaoSystemDialog::MessageProc( SI_MESSAGE * pMessage )
{
	SolarDialog::MessageProc( pMessage );
}
void ChaoSystemDialog::NetworkProc( MSG_BASE * pMsg )
{
}

void ChaoSystemDialog::Process(DWORD dwTick)
{
	if(m_dwTime>0)
	{
		m_iTickSum+=dwTick;

		if(m_iTickSum>=1000)
		{
			DWORD sec=m_iTickSum/1000;

			if(m_dwTime>sec)
				m_dwTime-=sec;
			else
				m_dwTime=0;

			m_iTickSum-=sec*1000;
		}
	}
}

void ChaoSystemDialog::OnRenderSolarDialog()
{
	RenderSystemTime();
	UpdateToolTip();
	UpdatePictureGaugeBar();
}

CControlWZ * ChaoSystemDialog::getControl( POSTYPE AtPos )
{
	assert( AtPos < CHAOSYSTEM_DIALOG_MAX );
	return GetControlWZ( m_wzId[AtPos] );
}


POSTYPE ChaoSystemDialog::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < CHAOSYSTEM_DIALOG_MAX; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}

	return CHAOSYSTEM_DIALOG_MAX;
}


void ChaoSystemDialog::UpdatePictureGaugeBar()
{
	RECT rtTiredDialog;

	CControlWZ* pControl = 	this->getControl(CHAOSYSTEM_TIME);
	rtTiredDialog = pControl->GetSizeRect(); 

	if(g_pHero&&g_pHero->GetChaoState()==eCHAO_STATE_CHAO)
	{
		m_pChaoPicture->ShowWindowWZ(WZ_SHOW);
		m_pFreeChaoPicture->ShowWindowWZ(WZ_HIDE);

	}
	else if(g_pHero&&g_pHero->GetChaoState()==eCHAO_STATE_PREV_CHAO)
	{
		m_pChaoPicture->ShowWindowWZ(WZ_HIDE);
		m_pFreeChaoPicture->ShowWindowWZ(WZ_SHOW);
	}	
}


void ChaoSystemDialog::RenderSystemTime()
{
	TCHAR	szText[128] = {0,};
	DWORD tempTime=m_dwTime;

	int iHour=tempTime/3600;
	int iMinute=(tempTime-iHour*3600)/60;
	int iSecond=tempTime-iHour*3600-iMinute*60;

	if(iMinute < 10 && iSecond <10)
	{
		Snprintf(szText, 128-1, _T("%d:0%d:0%d"), iHour, iMinute, iSecond);
	}
	// Minute 앞에 "0" 추가.
	else if(iMinute< 10)
	{
		Snprintf(szText, 128-1, _T("%d:0%d:%d"), iHour, iMinute, iSecond);
	}
	// Second 앞에 "0" 추가
	else if(iSecond < 10)
	{
		Snprintf(szText, 128-1, _T("%d:%d:0%d"), iHour, iMinute, iSecond);
	}
	else
	{
		Snprintf(szText, 128-1, _T("%d:%d:%d"), iHour, iMinute, iSecond);

	}

	m_pStatic->SetTextWZ(szText);	
}


void ChaoSystemDialog::UpdateToolTip()
{
	int iMouseX = Mouse::Instance()->GetMouseX();
	int iMouseY = Mouse::Instance()->GetMouseY();


	m_bMouseOver = FALSE;
	CControlWZ * pControl = getControl(CHAOSYSTEM_TIME);

	if(pControl)
	{
		RECT rc = pControl->GetSizeRect();

		if (rc.left <= iMouseX && rc.right >= iMouseX)
		{
			if (rc.top <= iMouseY && rc.bottom >= iMouseY)
			{
				m_bMouseOver = TRUE;
				uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
				if(pUIMan&&g_pHero&&g_pHero->IsChao())
				{
					pUIMan->SetTooltipDialogID(GetDialogKey());
					if(g_pHero->GetChaoState()==eCHAO_STATE_CHAO)
						pUIMan->RegisterChaoSystemToolTip( m_dwTime ,TRUE);				
					else
						pUIMan->RegisterChaoSystemToolTip( m_dwTime ,FALSE);
				}
			}
		}
	}

}

