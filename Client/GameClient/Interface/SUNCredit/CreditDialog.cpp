#include "SunClientPrecompiledHeader.h"

#include "gamefunc.h"
#include "Application.h"
#include "ResourceManager.h"
#include "InterfaceManager.h"
#include "Mouse.h"
#include "SoundEffect.h"
#include "globalfunc.h"
#include "GameFramework.h"
#include "SUNCredit\CreditDialog.h"
#include "uiMenuMan\uimenuman.h"

WzID CreditDialog::m_wzId[CREDIT_MAX] = 
{ 
	StrToWzID("PI00"),//CREDIT_MAP_IMAGE,
	StrToWzID("T001"),//CREDIT_MAP_NAME,
	StrToWzID("C001"),//CREDIT_PLUS,
	StrToWzID("B999"),//CREDIT_EXIT,
	StrToWzID("PI01"),//CREDIT_PIC_LAYER_BACKSIZE,
	StrToWzID("PI02"),//CREDIT_PIC_LAYER_SIZE,
	StrToWzID("PI03"),//CREDIT_PIC_LAYER_COUNT,
	StrToWzID("ST00"),//CREDIT_TXT_LAYER_COUNT,
};


CreditDialog::CreditDialog(InterfaceManager *pUIMan):
	uiBase(pUIMan)
{
	m_hRenderTargetTexture = INVALID_HANDLE_VALUE;
	m_iState = eFIS_IDLE;
	m_hBink = NULL;
    m_pPicCtrlLayerBackSize = NULL;
    //m_dwAccmulateTick = 0;
}


CreditDialog::~CreditDialog()
{
	_asm nop;
}

VOID CreditDialog::Release()
{
	ReleaseBink();
	if( m_hRenderTargetTexture )
	{
		g_pSunRenderer->x_pManagerTexture->DestroyTexture(m_hRenderTargetTexture);
		m_hRenderTargetTexture = INVALID_HANDLE_VALUE;	
	}
}

VOID CreditDialog::Init( CDrawBase * pDrawBase )
{
	MoveCenter();
	InitControls();
}

void CreditDialog::InitControls()
{
	m_pPicCtrlLayer = static_cast<CCtrlStaticWZ *>(getControl(CREDIT_MAP_NAME));
	assert(m_pPicCtrlLayer);
	
	m_pTxtCtrlLayer = static_cast<CCtrlStaticWZ *>(getControl(CREDIT_TXT_LAYER_COUNT));
	assert(m_pTxtCtrlLayer);

	m_pPicCtrlLayerSize = static_cast<CCtrlPictureWZ *>(getControl(CREDIT_PIC_LAYER_SIZE));
	assert(m_pPicCtrlLayerSize);

	m_pPicCtrlLayerBackSize = static_cast<CCtrlPictureWZ *>(getControl(CREDIT_PIC_LAYER_BACKSIZE));
	assert(m_pPicCtrlLayerBackSize);

	if(m_pPicCtrlLayer)
		m_pPicCtrlLayer->ShowWindowWZ(WZ_HIDE);
	if(m_pTxtCtrlLayer)
		m_pTxtCtrlLayer->ShowWindowWZ(WZ_HIDE);
	if(m_pPicCtrlLayerSize)
		m_pPicCtrlLayerSize->ShowWindowWZ(WZ_HIDE);
	if(m_pPicCtrlLayerBackSize)
		m_pPicCtrlLayerBackSize->ShowWindowWZ(WZ_HIDE);
}

VOID CreditDialog::MessageProc( SI_MESSAGE * pMessage )
{
	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
			{
			case CREDIT_EXIT:
				{
					ShowDialog(FALSE);
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




VOID CreditDialog::NetworkProc( MSG_BASE * pMsg )
{
}

POSTYPE CreditDialog::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < CREDIT_MAX ; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}
	return CREDIT_MAX;
}

CControlWZ * CreditDialog::getControl( POSTYPE AtPos )
{
	assert( AtPos < CREDIT_MAX );
	return GetControlWZ( m_wzId[AtPos] );
}

BOOL CreditDialog::IsVisibleWindow()
{
	return GetDialogWZ()->IsState( WZ_SHOW );
}

VOID CreditDialog::OnShowWindow( BOOL val )
{
	ENUM_STATEWZ v = ( val == TRUE ? WZ_SHOW: WZ_HIDE );
	GetDialogWZ()->ShowWindowWZ( v );
		
	if( val )
	{
		m_iState = eFIS_MOVIE_RUNNING;
		InitBink(false);
        keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType=eDoType_dialog_credit;
		msg.wParam=IM_MENU_MANAGER::CREDIT_DLG;
		msg.DoSomething=GlobalFunc::DoShowWindowEx;
		g_KeyQueueManager.PushBack(msg);
	}
	else
	{
		ReleaseBink();
        g_KeyQueueManager.DeleteMsg(eDoType_dialog_credit);
	}

}
BOOL CreditDialog::InterSectRect( POINT point )
{
	RECT rc;

	GetDialogWZ()->GetClientRect(&rc);

	if(DEF_IntersectRect(rc,point))
	{
		return TRUE;	
	}
	return FALSE;
}

void CreditDialog::OnUpdateSolarDialog()
{
	MoveCenter();
}

void CreditDialog::OnRenderSolarDialog()
{
	RenderMovie();

	// 무비를 그리고 컨트롤을 그린다.
	renderframe();
}

void CreditDialog::renderframe()
{
	CControlWZ *pFrameControl = getControl(CREDIT_PIC_LAYER_BACKSIZE);
	if (pFrameControl) 
	{
		RECT rc = pFrameControl->GetSizeRect();
		SI_TFORMAT * pFormat = m_pPicCtrlLayerBackSize->GetCtrlImageFormat();
		if (m_pPicCtrlLayerBackSize != INVALID_HANDLE_VALUE && m_hBink)
		{
            g_pSunRenderer->RenderTexture( 
				pFormat->m_sImageState[E_BASE].m_pVoid,
				(float)rc.left,
				(float)rc.top, 
				(float)m_hBink->Width, 
				(float)m_hBink->Height);
		}
	}

	pFrameControl = getControl(CREDIT_EXIT);
	if (pFrameControl) 
	{
		RECT rc = pFrameControl->GetSizeRect();
		CCtrlButtonWZ * pPicCtrlLayerBackSize = (CCtrlButtonWZ *)getControl( CREDIT_EXIT );
		SI_TFORMAT * pFormat = pPicCtrlLayerBackSize->GetCtrlImageFormat();
		if (pPicCtrlLayerBackSize != INVALID_HANDLE_VALUE && m_hBink)
		{
			g_pSunRenderer->RenderTexture(
				pFormat->m_sImageState[E_BASE].m_pVoid,
				(float)rc.left,
				(float)rc.top, 
				(float)16,
				(float)16,
				pFormat->m_sImageState[E_BASE].m_sFRect.fTopX,
				pFormat->m_sImageState[E_BASE].m_sFRect.fTopY,
				pFormat->m_sImageState[E_BASE].m_sFRect.fHeight,
				pFormat->m_sImageState[E_BASE].m_sFRect.fWidth
				);
		}
	}
}
//========================================================================================

//------------------------------------------------------------------------------
/**
*/
void CreditDialog::Process(DWORD dwTick)
{
	//ProcessCaptionEffect(dwTick);

	static DWORD dwAccmulateTick = 0;
	dwAccmulateTick += dwTick;

	static DWORD dwSecondAccmulateTick = 0;
	dwSecondAccmulateTick += dwTick;
	if( dwSecondAccmulateTick > MOVIE_FRAME_UPDATE_TIME)
	{
		UpdateMovie();
		dwSecondAccmulateTick =dwSecondAccmulateTick - MOVIE_FRAME_UPDATE_TIME;
	}
}

//------------------------------------------------------------------------------
/**
*/
void CreditDialog::InitBink(bool bConsciousness)
{
	if( m_hBink )
	{
		ReleaseBink();
	}

	m_hBink = BinkOpen("Data\\movie\\SUN_Credit.bik",0);
	assert(m_hBink);

	if( m_hBink  && m_hRenderTargetTexture == INVALID_HANDLE_VALUE )
	{
		//	3 - RGB     4 - RGBA
		m_hRenderTargetTexture = g_pSunRenderer->x_pManagerTexture->GenerateEmptyTexture(
			m_hBink->Width, m_hBink->Height, 3,
			TLS_NO_MIPMAP | TLS_EMPTY_TEXTURE |TLS_NO_MULTITHREAD );
		BinkSoundUseDirectSound( 0 );
		m_hBink->FrameNum = 0;
	}
}

//------------------------------------------------------------------------------
/**
*/
void CreditDialog::ReleaseBink()
{
	if( m_hBink )
	{
		BinkClose(m_hBink);
		m_hBink =NULL;
	}
}

//------------------------------------------------------------------------------
/**
*/
void CreditDialog::RenderMovie()
{
	if( !m_hBink)
		return;

	if(!m_hRenderTargetTexture)
		return;

	RECT rtPicture = m_pPicCtrlLayerBackSize->GetSizeRect();
	g_pSunRenderer->RenderTexture(	m_hRenderTargetTexture,
		(float)rtPicture.left, (float)rtPicture.top,
		(float)rtPicture.right - rtPicture.left, 
		(float)rtPicture.bottom - rtPicture.top,
		0.0f,0.0f);
}

//------------------------------------------------------------------------------
/**
*/
void CreditDialog::UpdateMovie()
{
	if(!m_hBink)
		return;

	// Decompress the Bink frame.
	BinkDoFrame( m_hBink );

	// Lock the BinkBuffer so that we can copy the decompressed frame into it.
	long lPitch;
	int iComponent;
	VOID* pByte = g_pSunRenderer->x_pManagerTexture->LockTexture( m_hRenderTargetTexture, &lPitch, &iComponent );

	if( pByte )
	{
		BinkCopyToBuffer( m_hBink, pByte, lPitch, m_hBink->Height, 0,0, BINKSURFACE32 );
		g_pSunRenderer->x_pManagerTexture->UnlockTexture( m_hRenderTargetTexture );
	}
	else
	{
		m_iState = eFIS_END;
		return;
	}

	// Are we at the end of the movie?
	if ( m_hBink->FrameNum == m_hBink->Frames )
	{
		if( m_iState == eFIS_MOVIE_RUNNING)
		{
			m_iState = eFIS_END;
			InitBink(FALSE);
		}
		if( m_iState == eFIS_IDLE || m_iState == eFIS_END)
		{
			//기본 동영상 무한반복
			//m_hBink->FrameNum = 0;
			ShowDialog(FALSE);
		}
	}
	else
	{
		BinkNextFrame( m_hBink );
	}
}

