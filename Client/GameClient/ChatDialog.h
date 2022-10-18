//=======================================================================================================================
/// ChatDialog
/**
	@author
			유재영 < y2jinc@webzen.co.kr >
	@since
			2005. 1. 23
	@remarks
			- 채팅 필터링
			- 귓말, 공지, 		
*/

#ifndef __CHAT_DIALOG_H__
#define __CHAT_DIALOG_H__

#pragma once

#include "uiBase.h"
#include "ChatDialog_def.h"
#include "ChatMsg.h"
#include "ResultCode.h"
#include "SystemChatDialog.h"
#include "ChatPropertyDialog.h"

struct sCHAT_TOKEN
{
	eCHAT_TYPE	eChatType;
	TCHAR		szToken[MAX_CHAT_TOKEN_LEN];
};

class ChatMenuDialog;

class InterfaceManager;

class ChatDialog : public uiBase
{
	enum eCHAT_POS
	{	
		CHAT_LIST = 0,					// 채팅 리스트
		CHAT_LIST_2X,
		CHAT_EDIT,						// 채팅 입력창
		CHAT_VSCR,						// 채팅 리스트 스크롤

		CHAT_TEXT_IMAGE_SELECT_MENU,	// 채팅 속성 텍스트

		CHAT_MENU_BUTTON,				// 채팅 속성창 버튼
		CHAT_LIST_SIZE_UP,
		CHAT_LIST_SIZE_DOWN,

		CHAT_VOICE_CHAT_BUTTON,			// 보이스 채팅 컨트롤 ON/OFF 버튼
		CHAT_CHECK_ALL,
		CHAT_CHECK_PARTY,
		CHAT_CHECK_GUILD,
		CHAT_CHECK_PROPERTY,

		CHAT_CHECK_CHANNEL,
		CHAT_CHECK_LOCK_SCROLL,
		CHAT_CHECK_SYSTEM,

#ifdef	_K2P_ADD_WISPER_WINDOW
		CHAT_CHECK_WISPER,
#endif
        CHAT_SYSTEM_CHAT_BUTTON,        // 시스템창 토글버튼
		CHAT_MAX,
	};	

	enum
	{
		MAX_CHATMSG_SIZE	= MSG_CG_CONVERSATION_FIELDCHAT_SYN::_MAX_CHATMSG_SIZE+1,
	};

	enum eCHAT_BTN
	{
		eCHAT_BTN_MENU,
        eCHAT_BTN_UP,
        eCHAT_BTN_DOWN,
		eCHAT_BTN_MAX,
	};

	enum 
	{
		CHAT_TEXT_COLOR,
		CHAT_BG_COLOR,

		CHAT_MAX_COLOR,
	};


	enum eCHAT_POSITION
	{
		eNORMAL = 0,
		eBATTLEZONE,
	};

	enum 
	{
		eCHECK_ALL = 0,
		eCHECK_PARTY,
		eCHECK_GUILD,
		eCHECK_PROPERTY,
		eCHECK_CHANNEL,
		eCHECK_LOCK_SCROLL,
		eCHECK_SYSTEM,
#ifdef _K2P_ADD_WISPER_WINDOW
		eCHECK_WISPER,
#endif
		eCHECK_MAX,
	};


public:
	ChatDialog(InterfaceManager* pUIMan);
	virtual ~ChatDialog();

	virtual VOID	Init( CDrawBase * pDrawBase );
	virtual VOID	Release();
	virtual VOID	MessageProc( SI_MESSAGE * pMessage );
	virtual VOID	NetworkProc( MSG_BASE * pMsg );
	virtual VOID	OnUpdateSolarDialog();
	virtual VOID	OnShowWindow( BOOL val );


	// 채팅창은 여러군데 옮겨다닐수 있기에 필요한 함수
	VOID			MoveDefaultPosition();
	VOID			MoveBattleZonePosition( RECT & rcBattleZoneClient );

	VOID			InitChatList();
	BOOL			HaveFocusInEdit();
	
	BOOL			HaveFocusDialog();
	VOID			SetFocusChatEdit();

	VOID			SetAlwaysOnTop(BOOL bFlag);
	BOOL			IsAlwaysOnTop();

	BOOL			IsInputMessageAtChat();
	void            Command( TCHAR * pszString);

	void			SetChatMessage(TCHAR* pszString);

	// 메세지 처리 관련
	VOID			AddString( TCHAR * pszString, eCHAT_TYPE eChatType = CHAT_TYPE_SYSTEM, DWORD dwSender = 0 );
	VOID			AddString( TCHAR * pszString, eCHAT_TYPE eChatType, TCHAR * szSender, TCHAR * szRecver = NULL);

	VOID			RegisterChatMsgAtList(ChatMsg * pChatMsg, BOOL bScroll = TRUE);
	TCHAR *			GetCompleteChatMsg(ChatMsg * pChatMsg);
	VOID			ExhaustChatMsgALL();
	VOID			RenderChatMsgWithoutDialog( DWORD dwTick );

	// 패킷 관련
	BOOL			SendChatMsg( TCHAR * pszMessage );
	BOOL			SendGuildMsg( TCHAR * pszMessage, GUILDGUID guid );
	BOOL			SendShoutMsg( TCHAR * pszMessage , BYTE byShoutType = MSG_CW_CHAT_SHOUT_SYN::GENERAL_SHOUT);
	BOOL			SendWhisperMsg( TCHAR * pszMessage, TCHAR * pszToChar);
	BOOL			SendNoticeMsg( TCHAR * pszMessage );
	BOOL			SendGMNoticeMsg( TCHAR * pszMessage, BYTE byNoticeType = MSG_CW_GM_NOTICE_SYN::NOTICE_WORLD );
	BOOL			SendChatChannelMsg ( TCHAR * pszMessage);
	BOOL			SendChatPartyMsg( TCHAR * pszMessage );
	VOID			SendChatChannelCreate( TCHAR * pszMessage);
	VOID			SendChatChannelInvite( TCHAR * pszMessage);
    void SendChannelNak();
	void			SendChatChannelInfoREQ();


	TCHAR *			GetInvitedRoomName() { return this->m_ChannelInvitedRoomName;}
	void			SetExistChannel(BOOL val) { m_bExistChannel = val;}
	BOOL			GetExistChannel() { return m_bExistChannel; }
	void			ExitChannelChat();
    TCHAR*          GetChannelName() { return m_ChannelName; }

	void			SetWhisperSender( TCHAR * pszMessage);
	void			SetWhisperReceiver( TCHAR * pszMessage);
	TCHAR *			GetWhisperSender();
	TCHAR *			GetWhisperReceiver() { return m_szPendingWhisperSenderToMe;}
	void			AutoFill();
	void			autoFillWhisperSender();
	void			autoFillWhisperreceiver();

	void			AutoFillWhisperNameWhenChatListClick();
	TCHAR *			GetCurSelNameAtChatList(ChatMsg * pChatMsg);
	// 채팅 정보 
	VOID			InitChatInfo();
	// 채팅 에디팅 처리
	VOID			ChatEdit_MsgProc( SI_MESSAGE * pMessage );
	VOID			UpdateChatInfo(int iFilterMode);
	//void            UpdatePosition();

    virtual void	MoveDefaultWindowPos();

	int				GetFilterMode() { return m_iCurFilterMode; }
	TCHAR *			GetCurChatMessage();
	CCtrlDxIMEEditBox * GetCurEditControl() { return this->m_pCurEditCtrl; }
    void SetChannelWaitting(bool value){ is_channel_join_waitting_ = value; }
    TCHAR* channel_waitting_name(){ return channel_waitting_name_; }

	BOOL            m_preShow;  // "이 번 프레임에선 채팅창이 바로 안열리게 해줘" 하는 변수


    void OnProcessChatMessage( LPTSTR Message );

	void			OnNoramlChat();
	void			OnPartyChat();
	void			OnGuildChat();
	void			OnChannelChat(); 
#ifdef	_K2P_ADD_WISPER_WINDOW
	void			OnWisperChar();
#endif

	void			OnSystemChat();
	int				GetCurrentBitFilter();
	void			SetCurrentBitFilter(int filter);
	void			ChangeChatListControl(eLIST_CONTROL_TYPE eType);
	void			SetShowShout(BOOL val);
	BOOL			GetShowShout() { return m_bShowShout; }
	void			ToggleShowShout();
    int             GetGapOfDialogYPosition(){ return m_iGapOfDialogYPosition; }
    void            SetGapOfDialogYPosition(int value){ m_iGapOfDialogYPosition = value; }
    void            CloseOptionDialogs();
    void            ChatPropertyDisable();
public:
	VOID			setChatType( eCHAT_TYPE eChatType );

	BOOL			GetShowSystemChat() { return m_bShowSystemChat; }
	SystemChatDialog * GetSystemChatDialog() { return m_pSystemChatDlg;}
	void			ShowSystemChatDialog(BOOL val);
	RECT			GetListRect(){return m_pChatListCtrl->GetSize();}
    bool IsBlockMember(TCHAR *char_name);
    void BaseChannelCreate();
    void set_is_first_enter(bool val) { is_first_enter_ = val; }

protected:
	CControlWZ *	getControl( POSTYPE AtPos );
	POSTYPE			getCtrlIDToPos( WzID wzId );

	BOOL			IsPassChatType(eCHAT_TYPE eChatType);
	VOID			RefreshChatMsg();

	BOOL			checkChatToken(TCHAR* pszMessage);

	BOOL			sendChatMsgToWorld(TCHAR * pszMessage);
	BOOL			sendChatMsgToField(TCHAR * pszMessage);

	VOID			pushBackChatMsg( ChatMsg * pChatMsg );
	void			pushBackSystemChatMsg( ChatMsg * pChatMsg ); 
	void			ExhaustChatMsgList(std::deque<ChatMsg *> *pChatList);
	VOID			refreshChatFilterList(BOOL bScroll = TRUE);
	
	int				getTooltipString(POSTYPE pos);
	void			updateTooltip();
    void            clearEdit();
	void			SetCheckButtonForFilter( int index );

#ifdef 	_000541_BLOCK_CHAT_REPEAT
	BOOL			isRepeatChatting(TCHAR * pszMessage );
	void			pendingSendChatInfomation(TCHAR * pszMessage);
#endif

	void			updateDialogSize();
	void			updateWhisperSelectInput();
	void			updateScrollPosition();
	void			updateWheelScroll();
    bool            CheckSlangChatMessage(TCHAR* chat_message);
	
protected:
    // netproc
	VOID			net_proc_CW_CHAT_NORMAL(MSG_BASE * pMsg);
	VOID			net_proc_CW_WHISPER(MSG_BASE * pMsg);
	VOID			net_proc_CW_NOTICE(MSG_BASE * pMsg);
	VOID			net_proc_CW_CHAT_SHOUT(MSG_BASE * pMsg);
	VOID			net_proc_CW_GUILD_CHAT(MSG_BASE * pMsg);
	VOID			net_proc_CW_PARTY_CHAT(MSG_BASE * pMsg);
	VOID			net_proc_CW_CHAT_CHANNEL(MSG_BASE * pMsg);
	VOID			net_proc_CW_WHISPER_SET_ACK(MSG_BASE * pMsg);
	VOID			net_proc_CG_CONVERSATION_FIELDCHAT_BRD(MSG_BASE * pMsg);
#ifdef _NA_007935_20150109_GM_COMMAND_EXP_EVENT
	VOID            net_proc_CG_EVENT_PEAKTIME_BRD(MSG_BASE* message);
#endif // _NA_007935_20150109_GM_COMMAND_EXP_EVENT
	VOID			OnErrorCode( DWORD dwErrorCode );

protected:
	static WzID		m_wzId[CHAT_MAX];

private:
    int                     m_iGapOfDialogYPosition;
	BOOL					m_bFirstEnter;
	POSTYPE					m_CurChatEditPos;

	TCHAR					m_szResult[MAX_CHATMSG_SIZE+1];

	TCHAR					m_szCurChatMessage[MAX_CHATMSG_SIZE +1];
	TCHAR					m_szbuf[MAX_CHATMSG_SIZE +1];
	TCHAR					szDebugText[512];
	POINT					m_MyInitialPoint;	// 자신의 초기 위치

	int						m_ChannelJoinMember_Num;
	TCHAR					m_ChannelName[MAX_ROOMTITLE_LENGTH +1];
	TCHAR					m_ChannelInvitedRoomName[MAX_ROOMTITLE_LENGTH +1];
	TCHAR					m_PendingChannelInvitedName[MAX_CHARNAME_LENGTH +1];
	BOOL					m_bExistChannel;

	std::deque<ChatMsg *>	m_ChatMsgList;

	std::deque<SenderName *> m_WhisperSenderList;
	int						m_iWhisperDequeIndex;
	BOOL					m_bClickUPandDOWN;
	int						m_eCurChatControlType;
	int						m_iMaxChatMsgLine;

#ifdef _000541_BLOCK_CHAT_REPEAT
	DWORD					m_dwPendingSendChattingTime;
	TCHAR					m_szPendingSendChatMsg[MAX_CHATMSG_SIZE +1];
#endif
	DWORD					m_dwTextColor[CHAT_MAX_COLOR][CHAT_TYPE_MAX];
	eCHAT_TYPE				m_eCurChatType;

	int						m_eFilter[MAX_CHAT_CHECK];
	CCtrlListWZ *			m_pListControlType[LIST_CONTROL_MAX];

	CCtrlDxIMEEditBox *		m_pChatEditCtrl;
	CCtrlDxIMEEditBox *		m_pCurEditCtrl;
	CCtrlListWZ *			m_pChatListCtrl;

	CCtrlVScrollWZ *		m_pChatVScrollCtrl;
	CCtrlImageStaticWZ *	m_pCtrlCurChatType;
	ChatMenuDialog *		m_pChatMenuDlg;
	CCtrlButtonCheckWZ *	m_pMicButton;
	CCtrlButtonCheckWZ *    m_pCheckFilter[eCHECK_MAX];
	CCtrlButtonWZ*			m_pButton[eCHAT_BTN_MAX];

	BOOL					m_bShowShout;
	//내가 귓속말 보낸사람 
	TCHAR					m_szPendingWhisperSender[MAX_CHARNAME_LENGTH +1];
	TCHAR					m_szTempPendingWhisperSender[MAX_CHARNAME_LENGTH +1];
	//나에게 귓속말을 보낸사람.
	TCHAR					m_szPendingWhisperSenderToMe[MAX_CHARNAME_LENGTH +1];


	SystemChatDialog *		m_pSystemChatDlg;
	ChatPropertyDialog *	m_pPropertyDlg;
	BOOL					m_bShowSystemChat;

	int						m_iCurFilterMode;

	sScrollInfo				m_VScrollInfo;

	float					m_fDelta;
	WzColor					m_WzTempFontColor;
	BOOL					m_bAlwaysOnTop;	
	BOOL					m_bInputChatingMessage;		// 채팅 입력을 하고 있었냐?

	eCHAT_POSITION			m_eChatPosition;
	RECT					m_rcBattleZone;
	BOOL					m_bLockScroll;
    bool is_channel_join_waitting_;
    char channel_waitting_name_[MAX_CHARNAME_LENGTH+1];
    bool is_first_enter_;

	// 패킷관련
	MSG_CW_CHAT_NORMAL_SYN				m_C2W_FieldChatMsg;
	MSG_CG_CONVERSATION_FIELDCHAT_SYN	m_FieldChatMsg;
	MSG_CW_WHISPER_SYN					m_WhisperMsg;
	MSG_CW_NOTICE_SYN					m_NoticeMsg;
	MSG_CW_GM_NOTICE_SYN				M_GM_NoticeMsg;

	map<int,int>			            m_UseTooltipCtrls;
	int						            m_iTooltipStringCode;

	enum{c_Max_Chat_Save_cnt = 10};

    typedef STLX_DEQUE<TCHAR*> VEC_CHATHISTORY;

	VEC_CHATHISTORY						m_vecChatHistory;
	int									m_icurrntStrInHistory;

public:
	void								ShowChatMsgInHistory();	
};

BOOL SendWhisper( TCHAR * pszMessage, TCHAR * pszToChar);
BOOL SendNotice( TCHAR * pszMessage );
BOOL SendGMNotice( TCHAR * pszMessage, BYTE byNoticeType = MSG_CW_GM_NOTICE_SYN::NOTICE_WORLD );


inline
BOOL
ChatDialog::HaveFocusInEdit()
{
	if( m_pCurEditCtrl)
	{
		if( m_pCurEditCtrl->HaveFocus() )
		{
			return TRUE;
		}
	}

	return FALSE;
}

inline
BOOL 
ChatDialog::HaveFocusDialog()
{
	return this->GetDialogWZ()->HaveFocus();
}

VOID RenderChatMsg( DWORD dwTick );

#endif 
// __CHAT_DIALOG_H__