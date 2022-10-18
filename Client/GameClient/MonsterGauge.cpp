#include "SunClientPrecompiledHeader.h"
#include "monstergauge.h"

#include "GlobalFunc.h"

WzID MonsterGauge::m_wzId[MGAUGE_MAX] = 
{ 
	StrToWzID("PMON"),
	StrToWzID("SMNM"),
};

MonsterGauge::MonsterGauge()
{

}

MonsterGauge::~MonsterGauge()
{
}

VOID MonsterGauge::Init( CDrawBase * pDrawBase )
{
	MoveWindowWZ( 300.f, 0.f );
	SetMonsterName( "이름 없음" );

	m_fGaugeSize = (float)getControl(MGAUGE_PMON)->GetSize().right;

	SetMonsterHP( 100 );
}

VOID MonsterGauge::MessageProc( SI_MESSAGE * pMessage )
{
	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_LBUTTONCLICK:
		{
		}
		break;

	default:
		{
			SolarDialog::MessageProc( pMessage );
		}
		break;
	}
}
VOID MonsterGauge::SetMonsterHP( float fHPPercent )
{
	if(100.f < fHPPercent)
	{
#ifdef _DEV_VER
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, "[Warning]MonsterGauge::HPPercent = %f", fHPPercent);
#endif 
		fHPPercent = 100.f;
	}

	RECT rect;
	rect = getControl(MGAUGE_PMON)->GetSizeRect();
	float width;

	float height = 0;
	RECT rcGauge = getControl(MGAUGE_PMON)->GetSize();
	height = (float)rcGauge.bottom;
	rcGauge = getControl(MGAUGE_PMON)->GetSizeRect();

	if (fHPPercent >= 0.0f )
	{
		width = (m_fGaugeSize/100.f) * fHPPercent;
		getControl(MGAUGE_PMON)->MoveWindowWZ( (float)rcGauge.left, (float)rcGauge.top, width, height );
	}
}

VOID MonsterGauge::SetMonsterXYZ( float fX, float fY, float fZ )
{
	
}

VOID MonsterGauge::SetMonsterName( char * pszMonsterName )
{
}


VOID MonsterGauge::NetworkProc( MSG_BASE * pMsg )
{

}

POSTYPE MonsterGauge::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < MGAUGE_MAX ; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}
	return MGAUGE_MAX;
}

CControlWZ * MonsterGauge::getControl( POSTYPE AtPos )
{
	assert( AtPos < MGAUGE_MAX );

	return GetControlWZ( m_wzId[AtPos] );
}