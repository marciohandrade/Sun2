#pragma once
//------------------------------------------------------------------------------
//
//    작업자: 이지연
//    uiCommunityChat class
//------------------------------------------------------------------------------

#include "uibase.h"

#include "ChatMsg.h"
#include "globalfunc.h"


class uiCommunityMan;

class uiCommunityChat: public uiBase
{
    enum COMMUNITY_CHAT_POS
    {
		COMMUNITY_CHAT_LIST = 0,
		COMMUNITY_CHAT_EDIT,
		COMMUNITY_CHAT_VSCR,
		COMMUNITY_BUTTON_EXIT,
		COMMUNITY_BUTTON_MINIMIZE,
		COMMUNITY_TXT_FRIEND_NAME,

		COMMUNITY_CHAT_MAX,
	};

    enum
    {
        MAX_COMMUNITY_CHATMSG_SIZE	= MSG_CW_FRIEND_CHAT_SYN::_MAX_CHATMSG_SIZE+1,
    };

public:
    uiCommunityChat(InterfaceManager *pUIMan);
    virtual ~uiCommunityChat();

    //--------------------------------	------------------------------------------
    // init/release
    //

    virtual void            Init(CDrawBase* pDrawBase);
    virtual void            Release();


    void                    InitControls();

    //--------------------------------------------------------------------------
    // helper/comms.
    //
    uiCommunityMan*			GetManager();

    //--------------------------------------------------------------------------
    // functions
    //
private:
	BOOL					_isEnable();

public:

    //--------------------------------------------------------------------------
    // dialog functions
    //
public:
	virtual void	        OnShowWindow( BOOL val );
    
	void					InitChatList();
	BOOL					HaveFocus();
	void					SetFocusChatEdit();

	// 메세지 처리 관련
	void					AddString( LPCTSTR ptszChatMsg,COLORREF TextColor); 
	void					AddString( LPCTSTR ptszFriendNameFrom, LPCTSTR ptszChatMsg, COLORREF TextColor = RGBA(255,255,255,255));
	
	// 채팅 에디팅 처리
	CCtrlDxIMEEditBox*		GetCurEditControl() { return this->m_pCommChatEditCtrl; }

	void					SetFriendName(TCHAR *ptszName);
	void					SetMyName(TCHAR *ptszName);

	TCHAR *					GetCurChatMessage();
	
	BOOL					IsHaveFocusInEdit();

protected:
    virtual void            OnUpdateSolarDialog();
    virtual void            OnRenderSolarDialog();


protected:
    CControlWZ *	        getControl( POSTYPE AtPos );
    POSTYPE			        getCtrlIDToPos( WzID wzId );

    static WzID		        m_wzId[COMMUNITY_CHAT_MAX];

    //--------------------------------------------------------------------------
    // procedure
    //
public:
    virtual void	        MessageProc( SI_MESSAGE * pMessage );   // Pure

    //--------------------------------------------------------------------------
    // member vars
    //
private:
    uiCommunityMan*			m_pCommunityMan;

	POSTYPE					m_CurChatEditPos;
	int						m_eFilterChatType;	// eCHAT_FILTER_TYPE 참조.

	CCtrlStaticWZ*			m_pCommTxtControl;
	CCtrlDxIMEEditBox*		m_pCommChatEditCtrl;
	CCtrlListWZ*			m_pCommChatListCtrl;
	CCtrlVScrollWZ*			m_pCommChatVScrollCtrl;

	TCHAR					m_ptszFriendNameTo[MAX_CHARNAME_LENGTH + 1];
	TCHAR					m_szCurChatMessage[MAX_COMMUNITY_CHATMSG_SIZE + 1];
	TCHAR					m_szbuf[MAX_COMMUNITY_CHATMSG_SIZE + 1];
	TCHAR					m_szResult[MAX_COMMUNITY_CHATMSG_SIZE + 1];

	bool					m_bShow;

	std::vector<util::_tstring>		m_vstrMsg;

    bool                    m_bControlsInitialized;
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiCommunityChat::getCtrlIDToPos( WzID wzId )
{
    for( POSTYPE i = 0 ; i < COMMUNITY_CHAT_MAX ; ++i )
    {
        if( m_wzId[i] == wzId )
        {
            return i;
        }
    }
    return COMMUNITY_CHAT_MAX;
}

//------------------------------------------------------------------------------
/**
*/
inline
CControlWZ *
uiCommunityChat::getControl( POSTYPE AtPos )
{
    ASSERT( AtPos < COMMUNITY_CHAT_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
uiCommunityMan*
uiCommunityChat::GetManager()
{
    return this->m_pCommunityMan;
}




//------------------------------------------------------------------------------
