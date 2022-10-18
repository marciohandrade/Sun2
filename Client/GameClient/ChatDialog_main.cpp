#include "SunClientPrecompiledHeader.h"
#include "ChatDialog.h"
#include "InterfaceManager.h"
#include "ChatMenu.h"
#include "ChatPersonalOption.h"
#include "GlobalFunc.h"
#include "GameConst_Color.h"
#include "uiChatMan/uiChatMan_def.h"
#include "BattleLobby.h"
#include "QuickContainer.h"
#include "hero.h"
#include "uichatman/uiChatMan.h"

//----------------------------------------------------------------------------
/**
*/

ChatDialog::ChatDialog(InterfaceManager* pUIMan):
uiBase(pUIMan),
m_bAlwaysOnTop(0),
m_pCurEditCtrl(NULL),
m_pChatEditCtrl(NULL),
m_pChatListCtrl(NULL)
{
	for(int i=0; i<eCHAT_BTN_MAX; ++i)
	{
		m_pButton[i] = NULL;
	}

	m_pMicButton =  NULL;

	m_UseTooltipCtrls.clear();
	m_iTooltipStringCode = 0;

	m_bShowShout = TRUE;
	m_UseTooltipCtrls[CHAT_CHECK_PROPERTY]			= eST_CHAT_PROPERTY_OFF;
	m_pSystemChatDlg = NULL;
	m_UseTooltipCtrls[CHAT_CHECK_LOCK_SCROLL]		= 1716;

	m_ChannelJoinMember_Num = 0;

	m_ChannelName[0] = 0;
	m_bExistChannel = FALSE;
	m_ChannelInvitedRoomName[0] = 0;
	m_PendingChannelInvitedName[0] = 0;
	m_UseTooltipCtrls[CHAT_CHECK_CHANNEL]			= 235;

	m_szPendingWhisperSender[0] = 0;
	m_szPendingWhisperSenderToMe[0] = 0;

#ifdef 	_000541_BLOCK_CHAT_REPEAT
	m_dwPendingSendChattingTime = 0;
	m_szPendingSendChatMsg[0] = 0;
#endif

	m_bLockScroll			= FALSE;
	m_iWhisperDequeIndex = 0;
	m_bClickUPandDOWN = FALSE;
	m_bShowSystemChat = TRUE;
    m_iGapOfDialogYPosition = c_iOneLineYPostionToQuickSlot;
    m_UseTooltipCtrls[CHAT_SYSTEM_CHAT_BUTTON] = 3420;
    is_first_enter_ = true;
}


//----------------------------------------------------------------------------
/**
*/
ChatDialog::~ChatDialog()
{
	m_UseTooltipCtrls.clear();
}


//----------------------------------------------------------------------------
/**
*/
VOID 
ChatDialog::InitChatList()
{
	if (!m_pChatListCtrl)
	{
		assert(!"왜 리스트가 없어!!");
	}

	m_pChatListCtrl->DeleteItemAll();
	m_pChatVScrollCtrl = (CCtrlVScrollWZ *)getControl( CHAT_VSCR );
	m_pChatVScrollCtrl->SetScrollRangeWZ(0,m_ChatMsgList.size());
	m_pChatVScrollCtrl->SetScrollPosWZ(0);
}

//----------------------------------------------------------------------------
/**
*/
VOID 
ChatDialog::Init( CDrawBase * pDrawBase )
{
	uiBase::Init(pDrawBase);

	memset(&m_VScrollInfo, 0, sizeof(m_VScrollInfo));
	setChatType( CHAT_TYPE_GENERAL );
	m_eChatPosition = eNORMAL;
	this->MoveDefaultPosition();

	for( int i = 0; i< LIST_CONTROL_MAX ; i ++)
	{
		m_pListControlType[i] = (CCtrlListWZ * ) getControl(i);
		m_pListControlType[i]->SetColorSelBar(RGBA(0,0,0,0));
		m_pListControlType[i]->SetColorSelBarOnText(RGBA(0,0,0,0));

        m_pListControlType[i]->SetSellHeight(16);
        m_pListControlType[i]->SetStartLineGapTop(7);
        m_pListControlType[i]->SetTextKind(ETK_SHADOW);
        m_pListControlType[i]->SetStartLineGapTop(5);
        m_pListControlType[i]->SetStartLineGapLeft(3);

		m_pListControlType[i]->SetUseRButtonSelect(true);

		if( i != LIST_CONTROL_1X)
		{
			m_pListControlType[i]->ShowWindowWZ(WZ_HIDE);
		}
	}
	m_pChatListCtrl = m_pListControlType[LIST_CONTROL_1X];
	// EditBox 설정
	m_pChatEditCtrl = (CCtrlDxIMEEditBox *)getControl( CHAT_EDIT );
	if (m_pChatEditCtrl)
	{
		m_pCurEditCtrl = m_pChatEditCtrl;
	}

	m_pCurEditCtrl->SetLimitText(c_iLIMITED_EDIT_TEXT);

	for(int i = 0 ; i < eCHECK_MAX ; ++i )
	{
		m_pCheckFilter[i] = (CCtrlButtonCheckWZ * ) getControl(i + CHAT_CHECK_ALL);
	}
	m_pCheckFilter[eCHECK_ALL]->SetCheckState(TRUE);
	m_pCheckFilter[eCHECK_PARTY]->SetCheckState(FALSE);
	m_pCheckFilter[eCHECK_GUILD]->SetCheckState(FALSE);
	m_pCheckFilter[eCHECK_PROPERTY]->SetCheckState(FALSE);
#ifdef	_K2P_ADD_WISPER_WINDOW
	m_pCheckFilter[eCHECK_WISPER]->SetCheckState(FALSE);
#endif

	m_pCtrlCurChatType		= (CCtrlImageStaticWZ *)getControl(CHAT_TEXT_IMAGE_SELECT_MENU);

	m_pButton[eCHAT_BTN_MENU] = static_cast<CCtrlButtonWZ*>(getControl(CHAT_MENU_BUTTON));
    m_pButton[eCHAT_BTN_UP] = static_cast<CCtrlButtonWZ*>(getControl(CHAT_LIST_SIZE_UP));
    m_pButton[eCHAT_BTN_DOWN] = static_cast<CCtrlButtonWZ*>(getControl(CHAT_LIST_SIZE_DOWN));
	m_pMicButton =  static_cast<CCtrlButtonCheckWZ*>(getControl(CHAT_VOICE_CHAT_BUTTON));

    //m_pButton[eCHAT_BTN_UP] = static_cast<CCtrlButtonWZ*>(getControl(CHAT_LIST_SIZE_UP));
    m_pButton[eCHAT_BTN_DOWN]->ShowWindowWZ(WZ_HIDE);

	//TODO: 음성채팅용 컨트롤이 UI에서 제거되면 이 코드도 제거해야 함
	if(m_pMicButton)
		m_pMicButton->ShowWindowWZ(WZ_HIDE);	// 버튼 없애버리자 


	m_pChatEditCtrl->SetFontID(StrToWzID("mn12"));
	m_pChatEditCtrl->SetColorText( c_Chat_DefaultColor );
	m_pChatEditCtrl->SetColorCaret( RGBA( 255, 255, 255, 255) );
	m_pChatEditCtrl->SetColorSelText( RGBA( 0, 0, 255, 255) );

	InitChatList();

	m_dwTextColor[CHAT_TEXT_COLOR][ CHAT_TYPE_GENERAL ]		= c_Chat_DefaultColor;
	m_dwTextColor[CHAT_TEXT_COLOR][ CHAT_TYPE_WHISPER ]		= c_Chat_WhisperColor;
	m_dwTextColor[CHAT_TEXT_COLOR][ CHAT_TYPE_GUILD ]		= c_Chat_GuildColor;
	m_dwTextColor[CHAT_TEXT_COLOR][ CHAT_TYPE_TRADE ]		= c_Chat_TradeColor;
	m_dwTextColor[CHAT_TEXT_COLOR][ CHAT_TYPE_SYSTEM ]		= c_Chat_SystemColor;
    m_dwTextColor[CHAT_TEXT_COLOR][ CHAT_TYPE_GM ]			= c_Chat_GMColor;
    m_dwTextColor[CHAT_TEXT_COLOR][ CHAT_TYPE_GM_WHISPER ]	= c_Chat_GMWhisperColor;
    m_dwTextColor[CHAT_TEXT_COLOR][ CHAT_TYPE_GM_NOTICE ]	= c_Chat_GMColor;   // GM 공지 채팅메시지 컬러
    
	m_dwTextColor[CHAT_TEXT_COLOR][ CHAT_TYPE_HELP ]		= c_Chat_HelpColor;
	m_dwTextColor[CHAT_TEXT_COLOR][ CHAT_TYPE_SHOUT ]		= c_Chat_ShoutColor;

	m_dwTextColor[CHAT_TEXT_COLOR][ CHAT_TYPE_PARTY ]		= c_Chat_PartyColor;
	m_dwTextColor[CHAT_TEXT_COLOR][ CHAT_TYPE_CHANNEL ]		= c_Chat_ChannelColor;

	m_dwTextColor[CHAT_TEXT_COLOR][CHAT_TYPE_WORLD_SHOUT]	= c_WorldShoutColor;
	m_dwTextColor[CHAT_BG_COLOR][CHAT_TYPE_WORLD_SHOUT]		= WzColor_RGBA(255,255,255,0);
	m_dwTextColor[CHAT_TEXT_COLOR][CHAT_TYPE_SUPER_LOBBY_MASTER_SHOUT] = c_SuperLobbyMasterColor;
	m_dwTextColor[CHAT_BG_COLOR][CHAT_TYPE_SUPER_LOBBY_MASTER_SHOUT] = WzColor_RGBA(255,255,255,0);

	m_dwTextColor[CHAT_BG_COLOR][ CHAT_TYPE_PARTY ]			= WzColor_RGBA(255,255,255,0);
	m_dwTextColor[CHAT_BG_COLOR][ CHAT_TYPE_GENERAL ]		= WzColor_RGBA(255,255,255,0);
	m_dwTextColor[CHAT_BG_COLOR][ CHAT_TYPE_WHISPER ]		= WzColor_RGBA(255,255,255,0);
	m_dwTextColor[CHAT_BG_COLOR][ CHAT_TYPE_GUILD ]			= WzColor_RGBA(255,255,255,0);
	m_dwTextColor[CHAT_BG_COLOR][ CHAT_TYPE_TRADE ]			= WzColor_RGBA(255,255,255,0);
	m_dwTextColor[CHAT_BG_COLOR][ CHAT_TYPE_SYSTEM ]		= WzColor_RGBA(255,255,255,0);
	m_dwTextColor[CHAT_BG_COLOR][ CHAT_TYPE_GM ]			= WzColor_RGBA(255,255,255,0);
	m_dwTextColor[CHAT_BG_COLOR][ CHAT_TYPE_GM_WHISPER ]	= WzColor_RGBA(255,255,255,0);
	m_dwTextColor[CHAT_BG_COLOR][ CHAT_TYPE_HELP ]			= WzColor_RGBA(255,255,255,0);
	m_dwTextColor[CHAT_BG_COLOR][ CHAT_TYPE_SHOUT ]			= WzColor_RGBA(255,255,255,0);
	m_dwTextColor[CHAT_BG_COLOR][ CHAT_TYPE_CHANNEL ]		= WzColor_RGBA(255,255,255,0);
    m_dwTextColor[CHAT_BG_COLOR][ CHAT_TYPE_GM_NOTICE ]	    = WzColor_RGBA(255,255,255,0);   // GM 공지 채팅메시지 컬러
    // _100416_LTJ_MODIFY_CHAT_FILTERING
    m_dwTextColor[CHAT_TEXT_COLOR][CHAT_TYPE_SLANG_FILTERING] = WzColor_RGBA(197, 47, 47, 255);
    m_dwTextColor[CHAT_BG_COLOR][CHAT_TYPE_SLANG_FILTERING] = WzColor_RGBA(255,255,255,0);
    m_dwTextColor[CHAT_TEXT_COLOR][CHAT_TYPE_SLANG_FILTERING_NOTICE] = c_Chat_SystemColor;
    m_dwTextColor[CHAT_BG_COLOR][CHAT_TYPE_SLANG_FILTERING_NOTICE] = WzColor_RGBA(255,255,255,0);

	// size set

	m_preShow=FALSE;

	// chat menu dialog 를 받아놓는다.
	m_pChatMenuDlg = GET_CAST_DIALOG(ChatMenuDialog, IM_CHAT_MANAGER::CHAT_MENU_DIALOG);
	m_pChatMenuDlg->SetChatDialog(this);

	m_iCurFilterMode = eLFM_NORMAL;	

	m_pSystemChatDlg = GET_CAST_DIALOG(SystemChatDialog, IM_CHAT_MANAGER::SYSTEM_CHAT_DIALOG);

	m_pPropertyDlg = GET_CAST_DIALOG(ChatPropertyDialog, IM_CHAT_MANAGER::CHAT_PROPERTY_DIALOG);
	m_pCtrlCurChatType->SetTextID(eST_NORMAL_CHAT);

    m_eFilter[0] = GAMEOPTION->GetChatFilterNormal();
    m_eFilter[1] = GAMEOPTION->GetChatFilterParty();
    m_eFilter[2] = GAMEOPTION->GetChatFilterGuild();
    m_eFilter[3] = GAMEOPTION->GetChatFilterChannel();
    m_eFilter[4] = GAMEOPTION->GetChatFilterSystem();
#ifdef _K2P_ADD_WISPER_WINDOW
    m_eFilter[5] = GAMEOPTION->GetChatFilterWshiper();
#endif

	m_pPropertyDlg->SetPropertyDlgFilter(eLFM_NORMAL , m_eFilter[eLFM_NORMAL]);
	m_eCurChatControlType = LIST_CONTROL_1X;
    m_iMaxChatMsgLine = SHOW_DEFAULT_CHAT_MSG_LINE;
	m_icurrntStrInHistory = -1;


    g_slangFilter.SetReplaceString(PROGRAMPARAM->GetReplaceWordForSlangWord(), 
                                   static_cast<FILTER_REPLACE_TYPE>(PROGRAMPARAM->GetReplaceType()));
    is_channel_join_waitting_ = false;
    sprintf(channel_waitting_name_, "");
}


//----------------------------------------------------------------------------
/**
*/
VOID 
ChatDialog::Release()
{
	ExhaustChatMsgALL();

	std::deque<SenderName *>::iterator itr = m_WhisperSenderList.begin();

	while( itr != m_WhisperSenderList.end() )
	{
		SenderName * pSender = *itr;
		SAFE_DELETE(pSender);
		itr++;
	}
	m_WhisperSenderList.clear();

	// 채팅 히스토리 해제
	TCHAR* pTemp = NULL;
	int iSize = m_vecChatHistory.size() - 1;
	for(int i = iSize; i >=0; i--)
	{
		pTemp = m_vecChatHistory[i];
		SAFE_DELETE_ARRAY(pTemp);
	}
	m_vecChatHistory.clear();
	// 채팅 히스토리

    GAMEOPTION->SetChatFilterNormal(m_eFilter[0]);
    GAMEOPTION->SetChatFilterParty(m_eFilter[1]);
    GAMEOPTION->SetChatFilterGuild(m_eFilter[2]);
    GAMEOPTION->SetChatFilterChannel(m_eFilter[3]);
    GAMEOPTION->SetChatFilterSystem( m_eFilter[4]);
#ifdef _K2P_ADD_WISPER_WINDOW
    GAMEOPTION->SetChatFilterWshiper(m_eFilter[5]);
#endif 
	GAMEOPTION->Save();
}

//----------------------------------------------------------------------------
/**
*/
VOID 
ChatDialog::OnUpdateSolarDialog()
{
	AutoFill();

	updateTooltip();
	updateScrollPosition();
	updateWhisperSelectInput();
	ShowChatMsgInHistory();
	updateWheelScroll();

	// 채팅 중인 지 Check
	m_bInputChatingMessage = BOOL( _tcslen( this->GetCurChatMessage() ) );
}


//----------------------------------------------------------------------------
/**
*/
VOID 
ChatDialog::OnShowWindow( BOOL val )
{
	if (val)
	{
		m_bLockScroll = FALSE;
		m_pCheckFilter[eCHECK_LOCK_SCROLL]->SetCheckState(FALSE);
		m_iWhisperDequeIndex = m_WhisperSenderList.size();
		m_pChatEditCtrl = (CCtrlDxIMEEditBox *) getControl(CHAT_EDIT);

		this->InitChatInfo();
		this->SetFocusChatEdit();
		m_bFirstEnter = TRUE;
	
		clearEdit();
		CCtrlVScrollWZ * pScroll = (CCtrlVScrollWZ *)getControl( CHAT_VSCR );
		if( pScroll)
		{
			int iMax = ( m_pChatListCtrl->GetListRowCnt() - (m_iMaxChatMsgLine) ) < 0 ? 0 : m_pChatListCtrl->GetListRowCnt() - (m_iMaxChatMsgLine);
			pScroll->SetScrollRangeWZ(0,iMax);
		}
		updateDialogSize();

		m_icurrntStrInHistory = -1;

		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType		= eDoType_dialog_chat;
		msg.wParam		= InterfaceManager::DIALOG_CHAT;
		msg.DoSomething = GlobalFunc::DoShowWindow;
		g_KeyQueueManager.PushBack(msg);	

		BattleLobby * pBattleLobby = (BattleLobby *)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_BATTLE_LOBBY);

		//gs_bug_fix:배틀존이 로비창이 열린상태에서는 키큐메니저에 삽입하지말자
		//			  esc를 눌러두 채팅창이 닫히지않게 하기위해			
		if(pBattleLobby && pBattleLobby->IsVisibleWindow())
		{
			g_KeyQueueManager.DeleteMsg(eDoType_dialog_chat);
		}

        if (g_pHero != NULL)
        {
            uiChatMan* char_man = GET_DIALOG_MANAGER(uiChatMan, UIMAN_CHAT);
            QuickContainer* quick_container = g_pHero->GetQuickContainer();
            if (char_man != NULL && quick_container != NULL)
            {
                if (quick_container->double_size() == true)
                {
                    char_man->MoveChatDialog(2);
                }
                else
                {
                    char_man->MoveChatDialog(1);
                }
            }
        }
	}
	else
	{
#if defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
        MoveDefaultPosition();
#endif // _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES
		CCtrlVScrollWZ * pScroll = (CCtrlVScrollWZ *)getControl( CHAT_VSCR );
		this->m_pChatMenuDlg->ShowWindow(FALSE);
		this->SetAlwaysOnTop(FALSE);
		g_Input.ResetKeyBuffer();
		g_Input.Reset();
        CloseOptionDialogs();
		m_bFirstEnter = FALSE;

		g_KeyQueueManager.DeleteMsg(eDoType_dialog_chat);
	}
}

//----------------------------------------------------------------------------
/**
*/
void ChatDialog::CloseOptionDialogs()
{
	uiBase* pDlg = GET_DIALOG(IM_CHAT_MANAGER::CHAT_PERSONAL_OPTION);
	if(pDlg && pDlg->IsVisibleWindow())
    {
		pDlg->ShowWindow(FALSE);
    }
    pDlg = GET_DIALOG(IM_CHAT_MANAGER::CHAT_PROPERTY_DIALOG);
    if (pDlg && pDlg->IsVisibleWindow())
    {
		pDlg->ShowWindow(FALSE);
    }
    if (pDlg && pDlg->IsVisibleWindow())
    { 
        pDlg = GET_DIALOG(IM_CHAT_MANAGER::CHAT_MENU_DIALOG);
    }
}
