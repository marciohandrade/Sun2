#pragma once
#include "solardialog.h"

class DummyDlg : public SolarDialog
{
public:
	DummyDlg(void);
	virtual ~DummyDlg(void);

	// 기본 enum
	enum eDummy_POS
	{
		Dialog_Max
	};

	// 기본 virtual methods
	VOID			Init( CDrawBase * pDrawBase );
	virtual VOID	Release();

	virtual VOID	MessageProc( SI_MESSAGE * pMessage );
	virtual VOID	NetworkProc( MSG_BASE * pMsg );

	virtual VOID	OnShowWindow(BOOL val);

	// 기본 proected methods
protected:
	CControlWZ *	getControl( POSTYPE AtPos );
	POSTYPE			getCtrlIDToPos( WzID wzId );

	// 기본 attributes
private:
	static WzID	    m_wzId[Dialog_Max];
};
