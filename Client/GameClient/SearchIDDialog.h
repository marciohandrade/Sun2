#pragma once
#include "solardialog.h"

//enum ANSWER
//{
//	Answer_NO = 0,
//	Anser_YES,
//};
//
//class SearchIDDialog : public SolarDialog
//{
//public:
//	enum SEARCHID_POS
//	{
//		B001 = 0,
//		B002,
//		E001,	
//
//		SRCH_MAX
//	};
//
//public:
//	SearchIDDialog(void);
//	virtual ~SearchIDDialog(void);
//
//public:
//	virtual VOID	Init( CDrawBase * pDrawBase );
//	virtual VOID	Release();
//
//	virtual VOID	MessageProc( SI_MESSAGE * pMessage );
//	virtual VOID	NetworkProc( MSG_BASE * pMsg );
//	virtual VOID	update();
//
//
//public:
//	CControlWZ *	getControl( POSTYPE AtPos );
//	POSTYPE			getCtrlIDToPos( WzID wzId );
//
//private:
//	util::_tstring			m_strID;
//	ANSWER			m_eAnswer;
//	static	WzID	m_wzId[SRCH_MAX];
//};
