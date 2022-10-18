//=======================================================================================================================
/// LobbyMember
/**
	@author
			À¯Àç¿µ < y2jinc@webzen.co.kr >
	@since

	@remarks
			- 		
*/


#ifndef LOBBYMEMBER_H
#define LOBBYMEMBER_H

#include "solardialog.h"

#if !defined ( _ZONE_TEST )

class LobbyMember :	public SolarDialog
{
public:
	enum eLobbyMember
	{
		LOBBYMEMBER_EXIT = 0,

		Dialog_Max
	};

public:
	LobbyMember(void);
	virtual ~LobbyMember(void);



public:
	VOID			Init( CDrawBase * pDrawBase );
	virtual VOID	Release();

public:
	virtual VOID	MessageProc( SI_MESSAGE * pMessage );
	virtual VOID	NetworkProc( MSG_BASE * pMsg );

	void			SendPacketFlag(BOOL bFlag)
	{
		m_bSendFlag = bFlag;
	}

protected:
	CControlWZ *	getControl( POSTYPE AtPos );
	POSTYPE			getCtrlIDToPos( WzID wzId );

public:
	BOOL			m_bSendFlag;

private:		
	static WzID     m_wzId[Dialog_Max];
};


#endif 
// #if !defined ( _ZONE_TEST )

#endif 
//LOBBYMEMBER_H