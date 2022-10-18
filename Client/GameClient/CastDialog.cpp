#include "SunClientPrecompiledHeader.h"
#include "CastDialog.h"
#include "ObjectManager.h"
#include "Player.h"
#include "Monster.h"
#include "InterfaceManager.h"
#include "GlobalFunc.h"

WzID CastDialog::m_wzId[] = 
{
	StrToWzID("P000"),
	StrToWzID("ST00"), //CAST_TEXT
	
};

CastDialog::CastDialog(void) 
{
    Cast_Type_ = DO_MAX;
}


CastDialog::~CastDialog(void)
{
}


VOID CastDialog::Init( CDrawBase * pDrawBase )
{
	
    m_fGaugeSize = (float)getControl(CAST_PIC_GAUGE)->GetSize().right;

    SetGage(0);
}


VOID CastDialog::Release()
{

}

VOID CastDialog::MessageProc( SI_MESSAGE * pMessage )
{
	switch ( pMessage->eResultMsg )
	{
	case 0:
	default:
		{
			SolarDialog::MessageProc( pMessage );
		}
		break;
	}
}


VOID CastDialog::NetworkProc( MSG_BASE * pMsg )
{

}


CControlWZ * CastDialog::getControl( POSTYPE AtPos )
{
	if( AtPos < Dialog_Max )
	{
		return GetControlWZ( m_wzId[AtPos] );
	}
	else
	{
		return NULL;
	}
}


POSTYPE CastDialog::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < Dialog_Max; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}

	return Dialog_Max;
}

VOID CastDialog::SetGage( float fHPPercent )
{
	if(100.f < fHPPercent)
	{
		WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_SYSCHAT, "[Warning]CastDialog::HPPercent = %f", fHPPercent);
		
		fHPPercent = 100.f;
	}

    RECT rcGauge = getControl(CAST_PIC_GAUGE)->GetSizeRect();

    if (fHPPercent >= 0.0f )
    {
        float width = (m_fGaugeSize/100.f) * fHPPercent;
		float height = (float)rcGauge.bottom - (float)rcGauge.top;
        getControl(CAST_PIC_GAUGE)->MoveWindowWZ( (float)rcGauge.left, (float)rcGauge.top, width, height );
    }

}

VOID CastDialog::SetText(TCHAR* szMessage)
{
	CCtrlStaticWZ* pCtrl = (CCtrlStaticWZ*)getControl(CAST_TEXT);
	if(pCtrl)
	{
		pCtrl->SetTextWZ(szMessage);
	}
}

VOID CastDialog::OnShowWindow( BOOL val )
{
	if (val)
	{
	}
	else
	{
        Cast_Type_ = DO_MAX;
		g_KeyQueueManager.DeleteMsg(eDoType_cast_cancel);
	}
}
