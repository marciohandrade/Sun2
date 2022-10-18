//=======================================================================================================================
/// LobbyMaster
/**
	@author
			À¯Àç¿µ < y2jinc@webzen.co.kr >
	@since

	@remarks
			- 		
*/

#pragma once
#include "solardialog.h"

#if !defined ( _ZONE_TEST )

class LobbyMaster :
	public SolarDialog
{
public:
	enum eLobbyMember
	{
		LOBBY_MASTER_START,
		LOBBY_MASTER_EXIT,

		Dialog_Max
	};

	LobbyMaster(void);
	virtual ~LobbyMaster(void);

public:
	VOID			Init( CDrawBase * pDrawBase );
	virtual VOID	Release();

public:
	virtual VOID	MessageProc( SI_MESSAGE * pMessage );
	virtual VOID	NetworkProc( MSG_BASE * pMsg );

protected:
	CControlWZ *	getControl( POSTYPE AtPos );
	POSTYPE			getCtrlIDToPos( WzID wzId );

private:		
	static WzID     m_wzId[Dialog_Max];
};

#endif