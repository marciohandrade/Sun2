#pragma once
#include "solardialog.h"

class DummyDlg : public SolarDialog
{
public:
	DummyDlg(void);
	virtual ~DummyDlg(void);

	// �⺻ enum
	enum eDummy_POS
	{
		Dialog_Max
	};

	// �⺻ virtual methods
	VOID			Init( CDrawBase * pDrawBase );
	virtual VOID	Release();

	virtual VOID	MessageProc( SI_MESSAGE * pMessage );
	virtual VOID	NetworkProc( MSG_BASE * pMsg );

	virtual VOID	OnShowWindow(BOOL val);

	// �⺻ proected methods
protected:
	CControlWZ *	getControl( POSTYPE AtPos );
	POSTYPE			getCtrlIDToPos( WzID wzId );

	// �⺻ attributes
private:
	static WzID	    m_wzId[Dialog_Max];
};
