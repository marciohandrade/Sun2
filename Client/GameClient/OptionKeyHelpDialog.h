#ifndef		OPTIONKEYHELPDIALOG_H
#define		OPTIONKEYHELPDIALOG_H

#include "uiBase.h"

#ifdef _NA_000000_20130114_RENEWER_UI
#else

class OptionKeyHelpDialog : public uiBase
{
	enum eOPTION_KEYHELP_POS
	{
		OPTION_KEYHELP_BT_EXIT,					//	버튼 - 종료

		OPTION_KEYHELP_MAX
	};

protected:
	static WzID				m_wzId[OPTION_KEYHELP_MAX];


public:
	OptionKeyHelpDialog( InterfaceManager * pUIMan );
	virtual ~OptionKeyHelpDialog();

public:
	VOID			Init( CDrawBase * pDrawBase );
	virtual VOID	Release();

public:
	virtual VOID	MessageProc( SI_MESSAGE * pMessage );
	virtual VOID	NetworkProc( MSG_BASE * pMsg );

protected:
	CControlWZ *	getControl( POSTYPE AtPos );
	POSTYPE			getCtrlIDToPos( WzID wzId );

	VOID			OnShowWindow( BOOL val );
	//void            UpdatePosition();

};

#endif
#endif	//	OPTIONKEYHELPDIALOG_H





