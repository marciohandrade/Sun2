#include "SunClientPrecompiledHeader.h"
#ifdef _YJ_FISHING_SYSTEM_000933
#include "InterfaceManager.h"
#include "uiFishingAction/uiFishingAction.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

WzID uiFishingAction::m_wzID[eDIALOG_FISHING_ACTION_MAX] = 
{
	StrToWzID("B001"),//	eDIALOG_FISHING_ACTION_EXIT_BTN,
	StrToWzID("PI00"),//	eDIALOG_FISHING_ACTION_GAuGE_RECT_PIC,
	StrToWzID("ST00"),//	eDIALOG_FISHING_ACTION_RESULT_TEXT,
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

uiFishingAction::uiFishingAction(InterfaceManager * pUIMan) : uiBase(pUIMan)
{
	m_pctrlGaugeRectPic = NULL;
	m_pctrlResultText	= NULL;
	m_bShow			  = FALSE;
	m_fGaugeSize    = 0.f;
	m_hGaugeTexture = g_pSunRenderer->x_pManagerTexture->LoadTexture( "Data\\Interface\\etc\\FishingWindow_Loding_bar.tga" ,TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

uiFishingAction::~uiFishingAction()
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiFishingAction::Init(CDrawBase* pDrawBase)
{
	uiBase::Init(pDrawBase);
	m_bShow			  = FALSE;

	InitControl();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiFishingAction::Release()
{
	if( m_hGaugeTexture != INVALID_HANDLE_VALUE )
	{
		g_pSunRenderer->x_pManagerTexture->DestroyTexture( m_hGaugeTexture );
		m_hGaugeTexture = INVALID_HANDLE_VALUE;
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiFishingAction::OnUpdateSolarDialog()
{
	if( ! m_bShow )
	{
		ShowDialog(FALSE);
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiFishingAction::OnRenderSolarDialog()
{
	if(m_hGaugeTexture && m_pctrlGaugeRectPic)
	{
		RECT rtGauge = m_pctrlGaugeRectPic->GetSizeRect();

		float fSrcX = 199.f;
		float fSrcY = 14.f;
		float fDstX = (float)(m_DwRealGauge * fSrcX) / 1000.f;
		float fDstY = 14.f;
		g_pSunRenderer->RenderTexture(m_hGaugeTexture, (float)rtGauge.left, (float)rtGauge.top, fDstX, fDstY, 0, 0, fSrcX, fSrcY );
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiFishingAction::SetTimer(DWORD dwTime)
{
	m_dwGaugeSpeed = dwTime;
	m_dwTempGauge = 0;
	m_DwRealGauge = 0;
	m_dwOldTime	  = 0;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiFishingAction::Process(DWORD dwTick)
{
	DWORD dwCurTime = timeGetTime();
	float fTemp = 0.f;
	if(0 == m_dwOldTime)
		m_dwOldTime = dwCurTime;

	if(m_dwOldTime < dwCurTime)
	{
		fTemp = ((float)(dwCurTime - m_dwOldTime)/10.f);
	}
	m_dwOldTime = dwCurTime;

	m_dwTempGauge += (DWORD)(m_dwGaugeSpeed * fTemp);
	if(2000 > m_dwTempGauge)
	{
		if(1000 > m_dwTempGauge)
		{
			m_DwRealGauge += (DWORD)(m_dwGaugeSpeed * fTemp);
		}
		else
		{
			if((DWORD)(m_dwGaugeSpeed * fTemp) < m_DwRealGauge)
				m_DwRealGauge -= (DWORD)(m_dwGaugeSpeed * fTemp);
		}
	}
	else
	{
		m_dwTempGauge = 0;
	}

}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiFishingAction::OnShowWindow(BOOL val)
{
	if( val )
	{
		m_bShow = TRUE;
	}
	else
	{
		m_bShow   = FALSE;
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiFishingAction::MessageProc(SI_MESSAGE * pMessage)
{
	switch ( pMessage->eResultMsg )
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch ( getControlIDToPos( pMessage->dwCtrlID ) )
			{
			case eDIALOG_FISHING_ACTION_EXIT_BTN:
				{
					OnShowWindow(FALSE);
				}
				break;
			case eDIALOG_FISHING_ACTION_GAUGE_RECT_PIC:
				{
				}
				break;
			case eDIALOG_FISHING_ACTION_RESULT_TEXT:
				{
				}
				break;
			}
		}
		break;

	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void uiFishingAction::InitControl()
{
	m_pctrlResultText = (CCtrlStaticWZ*)getControl(eDIALOG_FISHING_ACTION_RESULT_TEXT);
	assert(m_pctrlResultText);
	if(m_pctrlResultText)
	{
		m_pctrlResultText->SetTextWZ("");
	}
	m_pctrlGaugeRectPic = (CCtrlPictureWZ*)getControl(eDIALOG_FISHING_ACTION_GAUGE_RECT_PIC);
	assert(m_pctrlGaugeRectPic);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#endif //_YJ_FISHING_SYSTEM_000933