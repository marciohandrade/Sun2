
#pragma once

#include "uiBase.h"
#include "ChatMsg.h"
#include "VScrollDialog.h"


class InterfaceManager;

#define MAX_SYSTEM_CHATMESSAGE_LENGTH 256
#define MAX_LIST_LINE 150
#define MAX_SHOW_LINE 6


enum SYSTEMCHAT_POS
{
	SYSTEMCHAT_LIST,
	SYSTEMCHAT_VSCROLL,
	SYSTEMCHAT_BTN_EXIT,
	SYSTEMCHAT_MAX
};
class SystemChatDialog : public uiBase, public VScrollDialog
{
	

public:
	SystemChatDialog(InterfaceManager* pUIMan);
	~SystemChatDialog();

	VOID					Init( CDrawBase * pDrawBase );
	virtual VOID			Release();
	virtual VOID			MessageProc( SI_MESSAGE * pMessage );
	virtual VOID			NetworkProc( MSG_BASE * pMsg );
	virtual VOID    		Process(DWORD dwTick);
	virtual VOID			OnUpdateSolarDialog();
	virtual VOID			OnShowWindow(BOOL val);
	//virtual BOOL			InterSectRect( POINT point );

    //virtual void            UpdatePosition();
    int                     GetGapOfDialogYPosition(){ return m_iGapOfDialogYPosition; }
    void                    SetGapOfDialogYPosition(int value){ m_iGapOfDialogYPosition = value; }
    void                    MoveDefaultPosition();
	void					AddString(TCHAR * pszMessage);
	void					UpdateScrollBar();
	void					UpdateScrollPosition();
	void					PushBackChatMsg( ChatMsg * pChatMsg );
protected:
	void					InitControll();
	BOOL					CheckControll();
	CControlWZ *			getControl( POSTYPE AtPos );
	POSTYPE					getCtrlIDToPos( WzID wzId );

protected:
	CCtrlVScrollWZ* vscroll_control();
	bool GetScrollArea(RECT& area);
	bool OnVScrollThumb(SI_MESSAGE* message);
	bool OnVScrollUp(SI_MESSAGE* message);
	bool OnVScrollDown(SI_MESSAGE* message);

protected:
	static WzID				m_wzId[SYSTEMCHAT_MAX];
	CCtrlListWZ *			m_pChatListCtrl;
	CCtrlVScrollWZ *		m_pChatVScrollCtrl;
	BOOL					m_bInitControll;
	int						m_iCurScrollValue;
	int						m_iMaxScrollValue;
	std::deque<ChatMsg *>	m_ChatSystemMsg;
	RECT					m_Rect;
    int                     m_iGapOfDialogYPosition;
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
SystemChatDialog::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < SYSTEMCHAT_MAX ; ++i )
	{

		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}
	return SYSTEMCHAT_MAX;
}

//------------------------------------------------------------------------------
/**
*/
inline
CControlWZ *
SystemChatDialog::getControl( POSTYPE AtPos )
{
	ASSERT( AtPos < SYSTEMCHAT_MAX );

	return GetControlWZ( m_wzId[AtPos] );
}
