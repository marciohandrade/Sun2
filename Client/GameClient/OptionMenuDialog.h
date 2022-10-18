#ifndef		OPTIONMENUDIALOG_H
#define		OPTIONMENUDIALOG_H

#include "uiBase.h"
#ifdef _NA_000000_20130114_RENEWER_UI
#else
class OptionMenuDialog : public uiBase
{
	enum eOPTION_MENU_POS
	{
		OPTION_MENU_BT_SYSTEM_OPTION,		//	�׷��� �ɼ� ���̾�α�
		OPTION_MENU_BT_GAME_OPTION,			//	���� �ɼ� ���̾�α�
		OPTION_MENU_BT_EXIT,				//	��ư - ����
		OPTION_MENU_BT_SOUND,				//	���� �ɼ�
		OPTION_MENU_BT_KEY_OPTION,			//	Ű���� ��ư
		

		OPTION_MENU_MAX
	};

protected:
	static WzID				m_wzId[OPTION_MENU_MAX];


public:
	OptionMenuDialog( InterfaceManager * pUIMan );
	virtual ~OptionMenuDialog();

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

#endif	//	OPTIONMENUDIALOG_H




