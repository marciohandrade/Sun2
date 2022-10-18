#pragma once
#include "uibase.h"

class InterfaceManager;

class CreateCharSelect :
	public uiBase
{
public:

	enum CCS_CONTROL
	{
		CCS_RETURN = 0,
        CCS_OK,
		CCS_MAX,
	};

	CreateCharSelect(InterfaceManager * pUIman);
	virtual ~CreateCharSelect(void);

	virtual VOID	Init( CDrawBase * pDrawBase );
	virtual VOID	Release();

    virtual void    OnShowWindow(BOOL val);
	virtual VOID	MessageProc( SI_MESSAGE * pMessage );

	CControlWZ *	getControl( POSTYPE AtPos );
	POSTYPE			getCtrlIDToPos( WzID wzId );

private:
	static	WzID	m_wzId[CCS_MAX];
};
