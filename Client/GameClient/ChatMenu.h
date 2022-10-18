//=======================================================================================================================
/// ChatMenu
/**
	@author
			유재영 < y2jinc@webzen.co.kr >
	@since
			2005. 8. 1
	@remarks
			- 		
*/

#pragma once

#include "uiBase.h"
#include "chatdialog.h"

class InterfaceManager;

class ChatMenuDialog :
	public uiBase
{
public:
	ChatMenuDialog(InterfaceManager* pUIMan);
	virtual ~ChatMenuDialog(void);

	enum eCHATMENU_POS
	{
		eCHATMENU_CHK_NORMAL = 0,
		eCHATMENU_CHK_GUILD,
		eCHATMENU_CHK_PARTY,
		
		eCHATMENU_BTN_WHISPER,
		eCHATMENU_BTN_SHOUT,

		Dialog_Max
	};

	enum eCHATMENU_NUM
	{
		eCHATMENU_MAX = 3
	};

	// 기본 virtual methods
	VOID			Init( CDrawBase * pDrawBase );
	virtual VOID	Release();

	virtual VOID	MessageProc( SI_MESSAGE * pMessage );
	virtual VOID	NetworkProc( MSG_BASE * pMsg );


	virtual VOID	OnShowWindow( BOOL val );

	VOID			GetControls();

	VOID			SetChatDialog(ChatDialog * pChatDialog);

	VOID			SetCurType(eCHAT_TYPE eChatType)
	{
		m_eChatType = eChatType;
	}

	eCHAT_TYPE		GetCurType()
	{
		return m_eChatType;
	}

	// 기본 proected methods
protected:
	CControlWZ *	getControl( POSTYPE AtPos );
	POSTYPE			getCtrlIDToPos( WzID wzId );

	// 기본 attributes
private:
	static WzID	    m_wzId[Dialog_Max];

	CCtrlButtonCheckWZ *	m_pCtrlChatType[eCHATMENU_MAX];

	eCHAT_TYPE		m_eChatType;
	ChatDialog *	m_pChatDialog; 
};

inline
VOID
ChatMenuDialog::SetChatDialog(ChatDialog * pChatDialog)
{
	m_pChatDialog = pChatDialog;
}