#pragma once
#include "solardialog.h"

class LoadingDialog : public SolarDialog
{
public:
	enum LOADING_POS
	{
		Loading_BG = 0,
		Loading_Max
	};

public:
	LoadingDialog(void);
	virtual ~LoadingDialog(void);

public:
	VOID			Init( CDrawBase * pDrawBase );
	virtual VOID	Release();

public:
	virtual VOID	MessageProc( SI_MESSAGE * pMessage );
	virtual VOID	NetworkProc( MSG_BASE * pMsg );

public:
	virtual VOID	Update();

protected:
	CControlWZ *	getControl( POSTYPE AtPos );
	POSTYPE			getCtrlIDToPos( WzID wzId );

private:		
	static WzID		m_wzId[Loading_Max];
};
