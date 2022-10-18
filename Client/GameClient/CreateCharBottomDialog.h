#pragma once
#include "uiBase.h"

class InterfaceManager;

class CreateCharBottomDialog :
	public uiBase
{

public:

	enum CCB_CONTROL
	{
        CCB_CREATE = 0,
        CCB_CANCEL,
		CCB_MAX,
	};
	CreateCharBottomDialog(InterfaceManager* pUIman);
	virtual ~CreateCharBottomDialog(void);

public:
	virtual VOID	Init( CDrawBase * pDrawBase );
	virtual VOID	Release();

    virtual void    OnShowWindow(BOOL val);
	virtual VOID	MessageProc( SI_MESSAGE * pMessage );

	CControlWZ *	getControl( POSTYPE AtPos );
	POSTYPE			getCtrlIDToPos( WzID wzId );

	virtual BOOL    OnClickCreate();

private:
	static	WzID	m_wzId[CCB_MAX];
};
