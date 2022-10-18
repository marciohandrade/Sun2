#ifndef __MONSTER_GAUGE_H__
#define __MONSTER_GAUGE_H__

#pragma once

#include "solardialog.h"

class MonsterGauge : public SolarDialog
{
	enum eMGAUGE_POS
	{	
		MGAUGE_PMON = 0,
		MGAUGE_SMNM,
		MGAUGE_MAX,
	};	

public:
	MonsterGauge();
	~MonsterGauge();

	VOID			Init( CDrawBase * pDrawBase );
	virtual VOID	MessageProc( SI_MESSAGE * pMessage );
	virtual VOID	NetworkProc( MSG_BASE * pMsg );


	VOID			SetMonsterXYZ( float fX, float fY, float fZ );
	VOID			SetMonsterHP( float fHPPercent );
	VOID			SetMonsterName( char * pszMonsterName );

protected:
	CControlWZ *	getControl( POSTYPE AtPos );
	POSTYPE			getCtrlIDToPos( WzID wzId );
	static WzID		m_wzId[MGAUGE_MAX];


	float			m_fGaugeSize;
};


#endif // __MONSTER_GAUGE_H__