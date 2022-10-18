#include "SunClientPrecompiledHeader.h"
#include "uiCommunityInfo.h"
#include "uiCommunityMan/uiCommunityMan.h"
#include "uiSystemMan/uiSystemMan.h"
#include "Hero.h"
#include "GlobalFunc.h"
#include "interfacemanager.h"
#include "GameFramework.h"
#include "globalfunc.h"
#include "MapInfoParser.h"
#include "SceneBase.h"
#include "GuildRankInfoParser.h"
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
#include "GuildSystem/GuildSystemFunction.h"
#include "GuildSystem/GuildSystemData.h"
#else
#include "uiGuildMan/uiGuildMan.h"
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#include "AppearanceResourceCode.h"

//------------------------------------------------------------------------------
WzID uiCommunityInfo::m_wzId[COMMUNITY_INFO_DIALOG_MAX] = 
{
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
    StrToWzID("B999"),//COMMUNITY_INFO_DIALOG_EXIT
#else
    StrToWzID("B001"),//COMMUNITY_INFO_DIALOG_EXIT
#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL    
	
	StrToWzID("L001"),//COMMUNITY_INFO_DIALOG_LIST
	
    StrToWzID("B002"),//COMMUNITY_INFO_DIALOG_BTN_GAME  
    StrToWzID("B003"),//COMMUNITY_INFO_DIALOG_BTN_MSG  
	StrToWzID("B004"),//COMMUNITY_INFO_DIALOG_BTN_REFRESH
	StrToWzID("B005"),//COMMUNITY_INFO_DIALOG_BTN_CHAT

#ifdef _NA_0_20100527_ADD_CHAR_RELATION_MEMO
    StrToWzID("E001"),  //COMMUNITY_INFO_DIALOG_EDIT_MEMO_INPUT
    StrToWzID("B006"),  //COMMUNITY_INFO_DIALOG_BTN_MEMO_ENABLE
    StrToWzID("B000"),  //COMMUNITY_INFO_DIALOG_BTN_MEMO_UPDATE
#endif //_NA_0_20100527_ADD_CHAR_RELATION_MEMO

    StrToWzID("V000"), 
    StrToWzID("P003"), 
};

//------------------------------------------------------------------------------
/**
*/
uiCommunityInfo::uiCommunityInfo(InterfaceManager *pUIMan) 
: uiBase(pUIMan)
, m_bControlsInitialized(false)
, request_uimanager_id_(0)
, multilineedit_control_(NULL)
{
    this->m_pCommunityMan =
        static_cast<uiCommunityMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_COMMUNITY));
    assert (m_pCommunityMan);

    if (!this->m_pCommunityMan)
    {
#ifdef _DEBUG
		 uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
			if(pSystemMan)
				pSystemMan->Sys_VerifyLock(_T("m_pCommunityMan이 없습니다."));
#endif
        this->ShowInterface(FALSE);
        return;
    }

	//m_bShow = false;
}

//------------------------------------------------------------------------------
/**
*/
uiCommunityInfo::~uiCommunityInfo()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityInfo::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

    this->InitControls();
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityInfo::Release()
{
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityInfo::InitControls()
{
    m_pCommunityInfoList	= static_cast<CCtrlListWZ *>(getControl(COMMUNITY_INFO_DIALOG_LIST));

	for(int i=0; i<COMMUNITY_INFO_BTN_MAX; ++i)
	{
		m_pBtnCtrls[i] = static_cast<CCtrlButtonWZ *>(getControl(COMMUNITY_INFO_DIALOG_BTN_GAME + i));
		assert(m_pBtnCtrls[i]);
	}

#ifdef _NA_0_20100527_ADD_CHAR_RELATION_MEMO
    if (getControl(COMMUNITY_INFO_DIALOG_EDIT_MEMO_INPUT) != NULL)
    {
        if (getControl(COMMUNITY_INFO_DIALOG_EDIT_MEMO_INPUT)->GetCtrlStyle() & CTRL_ES_MULTILINE)
        {
            multilineedit_control_ = static_cast<CCtrlMultilineEditWZ*>(getControl(COMMUNITY_INFO_DIALOG_EDIT_MEMO_INPUT));
            multilineedit_control()->SetLimitText(MAX_FRIEND_MEMO_LENGHT);
			multilineedit_control()->SetLimitLine(4);
            multilineedit_control()->GetVScroll()->ShowWindowWZ(WZ_HIDE);
			multilineedit_control()->SetUseInputEnterKey(false);

        }
    }
#endif //_NA_0_20100527_ADD_CHAR_RELATION_MEMO
	
    this->m_bControlsInitialized = true;
}


//------------------------------------------------------------------------------
void uiCommunityInfo::OnShowWindow( BOOL val )
{
    if (val)
    {
        //this->MoveDefaultWindowPos();

        if (!this->m_bControlsInitialized)
        {
            this->ShowInterface(FALSE);
            return;
        }
    
		//m_bShow = true;

        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = eDoType_dialog_community_info;
        msg.wParam = InterfaceManager::DIALOG_COMMUNITY_INFO;
        msg.DoSomething = GlobalFunc::DoShowWindow;
        g_KeyQueueManager.PushBack(msg);

		_prepareShow();
        FriendMemo_InputButtonState(kFriendMemoButtonStatus_Disable);
		RefreshList();
	}
    else
    {
		//m_bShow = false;
        g_KeyQueueManager.DeleteMsg(eDoType_dialog_community_info);
    }
}
//------------------------------------------------------------------------------
void  uiCommunityInfo::OnButtonClick_UpdateMemo()
{
#ifdef _NA_0_20100527_ADD_CHAR_RELATION_MEMO
    if (multilineedit_control() != NULL)
    {
        TCHAR memo[MAX_FRIEND_MEMO_LENGHT] = {NULL, };
        ZeroMemory(memo, sizeof(memo));
        multilineedit_control()->GetTextWZ(memo, MAX_FRIEND_MEMO_LENGHT);

        if (_tcscmp(memo, GetManager()->m_FriendInfo.m_ptszMemo) != 0)
        {
            GetManager()->SEND_CW_FRIEND_MEMO_UPDATE_SYN(GetManager()->m_FriendInfo.m_dwCharGuid, memo);

            //! 업데이터 해 놓는다.
            StrnCopy(GetManager()->m_FriendInfo.m_ptszMemo, memo, MAX_FRIEND_MEMO_LENGHT);

            //! 버튼 상태 변경
            FriendMemo_InputButtonState(kFriendMemoButtonStatus_Lock);
        }
        else
        {
            //! multieditbox 에 있는 내용을 원상 복구 한다
            multilineedit_control()->SetTextWZ(GetManager()->m_FriendInfo.m_ptszMemo);

            //! 버튼 상태 변경
            FriendMemo_InputButtonState(kFriendMemoButtonStatus_Disable);
        }
    }
#endif //_NA_0_20100527_ADD_CHAR_RELATION_MEMO
}

//------------------------------------------------------------------------------
void  uiCommunityInfo::FriendMemo_InputButtonState(FriendMemoButtonStatus status)
{
#ifdef _NA_0_20100527_ADD_CHAR_RELATION_MEMO
    CControlWZ* control = NULL;

    if (status == kFriendMemoButtonStatus_Modify)
    {
        control = getControl(COMMUNITY_INFO_DIALOG_BTN_MEMO_ENABLE);
        if (control != NULL)
        {
            control->ShowWindowWZ(WZ_HIDE);
            control->EnableWindowWZ(true);
        }
        control = getControl(COMMUNITY_INFO_DIALOG_BTN_MEMO_UPDATE);
        if (control != NULL)
        {
            control->ShowWindowWZ(WZ_SHOW);
            control->EnableWindowWZ(true);
        }
        control = getControl(COMMUNITY_INFO_DIALOG_EDIT_MEMO_INPUT);
        if (control != NULL)
        {
            control->EnableWindowWZ(true);
            control->OnFocusIn();
        }
    }
    else if (status == kFriendMemoButtonStatus_Disable)
    {
        control = getControl(COMMUNITY_INFO_DIALOG_BTN_MEMO_ENABLE);
        if (control != NULL)
        {
            control->ShowWindowWZ(WZ_SHOW);
            control->EnableWindowWZ(true);
        }
        control = getControl(COMMUNITY_INFO_DIALOG_BTN_MEMO_UPDATE);
        if (control != NULL)
        {
            control->ShowWindowWZ(WZ_HIDE);
            control->EnableWindowWZ(true);
        }
        control = getControl(COMMUNITY_INFO_DIALOG_EDIT_MEMO_INPUT);
        if (control != NULL)
        {
            control->EnableWindowWZ(false);
            control->OnFocusOut();
        }
    }
    else if (status == kFriendMemoButtonStatus_Lock)
    {
        control = getControl(COMMUNITY_INFO_DIALOG_BTN_MEMO_ENABLE);
        if (control != NULL)
        {
            control->ShowWindowWZ(WZ_SHOW);
            control->EnableWindowWZ(false);
        }
        control = getControl(COMMUNITY_INFO_DIALOG_BTN_MEMO_UPDATE);
        if (control != NULL)
        {
            control->ShowWindowWZ(WZ_HIDE);
            control->EnableWindowWZ(false);
        }
        control = getControl(COMMUNITY_INFO_DIALOG_EDIT_MEMO_INPUT);
        if (control != NULL)
        {
            control->EnableWindowWZ(false);
            control->OnFocusOut();
        }
    }

#endif //_NA_0_20100527_ADD_CHAR_RELATION_MEMO
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityInfo::_prepareShow()
{
	if(!GetManager())
    {
		return;
    }

    if (GetManager()->IsOnline(GetManager()->m_FriendInfo.m_ptszCharName) == FALSE)
    {
        // COMMUNITY_INFO_DIALOG_BTN_GAME	: 게임 같이 하기 표시
        if (m_pBtnCtrls[COMMUNITY_INFO_BTN_GAME])
        {
            m_pBtnCtrls[COMMUNITY_INFO_BTN_GAME]->EnableWindowWZ(false);
        }

        // COMMUNITY_INFO_DIALOG_BTN_MSG	: 쪽지 보내기 표시
        if (m_pBtnCtrls[COMMUNITY_INFO_BTN_MSG])
        {
            m_pBtnCtrls[COMMUNITY_INFO_BTN_MSG]->EnableWindowWZ(true);	
        }

        // COMMUNITY_INFO_DIALOG_BTN_CHAT	: 대화하기 표시
        if (m_pBtnCtrls[COMMUNITY_INFO_BTN_CHAT])
        {
            m_pBtnCtrls[COMMUNITY_INFO_BTN_CHAT]->EnableWindowWZ(false);	
        }
        return;
    }
    

#ifdef _YMS_GUILD_DETAL_SPEC
    if (request_uimanager_id() == UIMAN_GUILD)
    {
        // COMMUNITY_INFO_DIALOG_BTN_GAME	: 게임 같이 하기 표시
        if (m_pBtnCtrls[COMMUNITY_INFO_BTN_GAME])
        {
            m_pBtnCtrls[COMMUNITY_INFO_BTN_GAME]->EnableWindowWZ(true); //ShowWindowWZ(WZ_SHOW);	
        }

        // COMMUNITY_INFO_DIALOG_BTN_MSG	: 쪽지 보내기 표시
        if (m_pBtnCtrls[COMMUNITY_INFO_BTN_MSG])
        {
            m_pBtnCtrls[COMMUNITY_INFO_BTN_MSG]->EnableWindowWZ(true); //ShowWindowWZ(WZ_SHOW);	
        }

        // COMMUNITY_INFO_DIALOG_BTN_CHAT	: 대화하기 표시
        if (m_pBtnCtrls[COMMUNITY_INFO_BTN_CHAT])
        {
            m_pBtnCtrls[COMMUNITY_INFO_BTN_CHAT]->EnableWindowWZ(false); //ShowWindowWZ(WZ_HIDE);	
        }

        return;
    }
#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
    else if (request_uimanager_id() == UIMAN_CONVENIENCESTORE)
    {
        // COMMUNITY_INFO_DIALOG_BTN_GAME	: 게임 같이 하기 표시
        if (m_pBtnCtrls[COMMUNITY_INFO_BTN_GAME])
        {
            m_pBtnCtrls[COMMUNITY_INFO_BTN_GAME]->EnableWindowWZ(false); //ShowWindowWZ(WZ_SHOW);	
        }

        // COMMUNITY_INFO_DIALOG_BTN_MSG	: 쪽지 보내기 표시
        if (m_pBtnCtrls[COMMUNITY_INFO_BTN_MSG])
        {
            m_pBtnCtrls[COMMUNITY_INFO_BTN_MSG]->EnableWindowWZ(false); //ShowWindowWZ(WZ_SHOW);	
        }

        // COMMUNITY_INFO_DIALOG_BTN_CHAT	: 대화하기 표시
        if (m_pBtnCtrls[COMMUNITY_INFO_BTN_CHAT])
        {
            m_pBtnCtrls[COMMUNITY_INFO_BTN_CHAT]->EnableWindowWZ(false); //ShowWindowWZ(WZ_HIDE);	
        }

        return;
    }
#endif //_NA_0_20110613_GAME_SHOP_RENEWAL
#endif

	if(GetManager()->GetPendingInfoMode() == eCM_FRIEND|| GetManager()->GetPendingInfoMode() == eCM_GROUP)
	{
		// COMMUNITY_INFO_DIALOG_BTN_GAME	: 게임 같이 하기 표시
		if(m_pBtnCtrls[COMMUNITY_INFO_BTN_GAME])
		{
			m_pBtnCtrls[COMMUNITY_INFO_BTN_GAME]->EnableWindowWZ(true); //ShowWindowWZ(WZ_SHOW);	
		}

		// COMMUNITY_INFO_DIALOG_BTN_MSG	: 쪽지 보내기 표시
		if(m_pBtnCtrls[COMMUNITY_INFO_BTN_MSG])
		{
			m_pBtnCtrls[COMMUNITY_INFO_BTN_MSG]->EnableWindowWZ(true); //ShowWindowWZ(WZ_SHOW);	
		}

		// COMMUNITY_INFO_DIALOG_BTN_CHAT	: 대화하기 표시
		if(m_pBtnCtrls[COMMUNITY_INFO_BTN_CHAT])
		{
			m_pBtnCtrls[COMMUNITY_INFO_BTN_CHAT]->EnableWindowWZ(true); //ShowWindowWZ(WZ_SHOW);	
		}
		
	}
	else if(GetManager()->GetPendingInfoMode() == eCM_REVENGE)
	{
		// COMMUNITY_INFO_DIALOG_BTN_GAME	: 게임 같이 하기 숨김
		if(m_pBtnCtrls[COMMUNITY_INFO_BTN_GAME])
		{
			m_pBtnCtrls[COMMUNITY_INFO_BTN_GAME]->EnableWindowWZ(false); //ShowWindowWZ(WZ_HIDE);	
		}

		// COMMUNITY_INFO_DIALOG_BTN_MSG	: 쪽지 보내기 숨김
		if(m_pBtnCtrls[COMMUNITY_INFO_BTN_MSG])
		{
			m_pBtnCtrls[COMMUNITY_INFO_BTN_MSG]->EnableWindowWZ(false); //ShowWindowWZ(WZ_HIDE);	
		}

		// COMMUNITY_INFO_DIALOG_BTN_CHAT	: 대화하기 숨김
		if(m_pBtnCtrls[COMMUNITY_INFO_BTN_CHAT])
		{
			m_pBtnCtrls[COMMUNITY_INFO_BTN_CHAT]->EnableWindowWZ(false); //ShowWindowWZ(WZ_HIDE);	
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
BOOL uiCommunityInfo::_isEnable()
{
	if(GENERALPARAM->IsNet())
	{
		if (this->IsLock())
			return FALSE;

		if(!g_pHero)
			return FALSE;

		if(g_pHero && g_pHero->IsShowFullMap())
			return FALSE;

		if(!GetManager())
			return FALSE;

		return TRUE;
	}
	else
	{
		return TRUE;
	}

}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityInfo::MessageProc( SI_MESSAGE * pMessage )
{
    if (!_isEnable())
        return;

    if (GetManager() == NULL)
    {
        return;
    }

	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};

    switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_LBUTTONCLICK:
        {
            switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
#ifdef _NA_0_20100527_ADD_CHAR_RELATION_MEMO
            case COMMUNITY_INFO_DIALOG_BTN_MEMO_ENABLE:	// 메모 입력 활성화
                {
                    FriendMemo_InputButtonState(kFriendMemoButtonStatus_Modify);
                }
                break;

            case COMMUNITY_INFO_DIALOG_BTN_MEMO_UPDATE:
                {
                    OnButtonClick_UpdateMemo();
                }
                break;
#endif //_NA_0_20100527_ADD_CHAR_RELATION_MEMO

            case COMMUNITY_INFO_DIALOG_BTN_GAME:	// 같이 게임하기
				{
					if(GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_VILLAGE)
					{
						GetManager()->Send_CW_FRIEND_STATUS_JOIN_SYN();
					}
					else
					{
						//1424 마을에서만 할수 있다
						g_InterfaceManager.GetInterfaceString( 1424, szMessage, INTERFACE_STRING_LENGTH );
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
					}
				}
				break;

			case COMMUNITY_INFO_DIALOG_BTN_MSG:
				{
					// 쪽지 쓰기 
					GetManager()->ShowWriteMailDlg(GetManager()->m_FriendInfo.m_ptszCharName);
				}
				break;

			case COMMUNITY_INFO_DIALOG_BTN_CHAT:
				{
					GetManager()->RequestFriendChat(true);
				}
				break;


			case COMMUNITY_INFO_DIALOG_BTN_REFRESH:	//새로고침
				{
					GetManager()->RqstRefreshFriendInfo();
				}
				break;

			case COMMUNITY_INFO_DIALOG_EXIT:
				{
					//m_bShow = false;
                    ShowInterface(FALSE);
				}
				break;
            }
        }
        break;
    case RT_MSG_SB_LINETOP:
        {
            if(getCtrlIDToPos( pMessage->dwCtrlID )
                == COMMUNITY_VSCROLL_V000)
            {
                UpScroll();
            }
        }
        break;

    case RT_MSG_SB_LINEBOTTOM:
        {
            if(getCtrlIDToPos( pMessage->dwCtrlID )
                == COMMUNITY_VSCROLL_V000)
            {
                DownScroll();
            }
        }
        break;

    case RT_MSG_SB_THUMBTRACK:      //!< 썸을 드래그 하고 있다.
    case RT_MSG_SB_THUMBPOSITION:   //!< 썸을 드래그한후 마우스를 놓았다.
    case RT_MSG_SB_ENDSCROLL:       //!< 스크롤이 종료 되었다.
        {
            if(getCtrlIDToPos( pMessage->dwCtrlID )
                == COMMUNITY_VSCROLL_V000)
            {
                ThumbScroll();
            }
        }
        break;

	default:
		{
			SolarDialog::MessageProc( pMessage );
		}
		break;
    }
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityInfo::OnUpdateSolarDialog()
{
	//if(!m_bShow)	
	//{
	//	ShowInterface(FALSE);
	//	return;
	//}

    if (!_isEnable())
    {
        return;
    }

#ifdef _NA_0_20100527_ADD_CHAR_RELATION_MEMO
    if (multilineedit_control() != NULL)
    {
		TCHAR memo[MAX_FRIEND_MEMO_LENGHT + 20] = {NULL, };
		ZeroMemory(memo, sizeof(memo));
		multilineedit_control()->GetTextWZ(memo, MAX_FRIEND_MEMO_LENGHT + 20);

		//! 길이 체크
		if(_tcslen(memo) > MAX_FRIEND_MEMO_LENGHT)
		{
			ZeroMemory(memo, sizeof(memo));
			multilineedit_control()->GetTextWZ(memo, MAX_FRIEND_MEMO_LENGHT);
			multilineedit_control()->SetTextWZ(memo);
		}
    }
#endif //_NA_0_20100527_ADD_CHAR_RELATION_MEMO

    CCtrlVScrollWZ* pControl = 
        static_cast<CCtrlVScrollWZ*>(getControl(COMMUNITY_VSCROLL_V000));

    if (m_pCommunityInfoList != NULL && pControl != NULL)
    {
        RECT area = m_pCommunityInfoList->GetSizeRect();
        BYTE wheel_msg = uiBase::GetMouseWheelStatus(&area);

        switch (wheel_msg)
        {
        case MOUSE_WHEEL_UP:    return UpScroll();
        case MOUSE_WHEEL_DOWN:  return DownScroll();
        }
    }
}


//------------------------------------------------------------------------------
/**
*/
void uiCommunityInfo::_addListItem(int idx, COLORREF TextColor, TCHAR *pMsg)
{
	DWORD c_dwBackColor = RGBA(0,0,0,0);

	GlobalFunc::SetItemText(m_pCommunityInfoList, idx, pMsg);
	
	m_pCommunityInfoList->SetItemFontID(idx, 0, StrToWzID("m212"));
				
	m_pCommunityInfoList->SetItemTextColor(
			idx, 
			0, 
			TextColor
			);

	m_pCommunityInfoList->SetItemBKColor(
			idx, 
			0, 
			c_dwBackColor
			);

	m_pCommunityInfoList->SetItemSelTextColor(
			idx, 
			0, 
			TextColor
			);
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityInfo::RefreshList()
{
	if(!m_pCommunityInfoList)
		return;

	if(!GetManager())
		return;

	m_pCommunityInfoList->DeleteItemAll();

	DWORD c_dwCategoryTextColor = RGBA(0, 128, 128, 255);
	DWORD c_dwTextColor = RGBA(255, 255, 255, 255);	

	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp2[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};

	// 1671	%s님의 기본 정보
	g_InterfaceManager.GetInterfaceString(1671, szTemp, INTERFACE_STRING_LENGTH);
	Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp, GetManager()->m_FriendInfo.m_ptszCharName);
	int idx = m_pCommunityInfoList->InsertItemBack();
	_addListItem(idx, c_dwCategoryTextColor, szMessage);


    LEVELTYPE character_level = GetSelectPlayerLevel(GetManager()->m_FriendInfo.m_ptszCharName);
    BYTE character_class = GetSelectPlayerClass(GetManager()->m_FriendInfo.m_ptszCharName);

    // 1672	레벨: %d
    g_InterfaceManager.GetInterfaceString(1672, szTemp, INTERFACE_STRING_LENGTH);
    Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp, character_level);
    idx = m_pCommunityInfoList->InsertItemBack();
    _addListItem(idx, c_dwTextColor, szMessage);


#ifdef _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY
    // 1673	종족: %s
    GenderType::Value gender_type = GetSelectPlayerGender(GetManager()->m_FriendInfo.m_ptszCharName);
    eCHAR_TYPE race_type = ResourceCode::GetImageClassType((eCHAR_TYPE)character_class, gender_type );

    g_InterfaceManager.GetInterfaceString(1673, szTemp, INTERFACE_STRING_LENGTH);
    Snprintf(szMessage, 
        INTERFACE_STRING_LENGTH-1, 
        szTemp, 
        GlobalFunc::GetRaceString(race_type));
    idx = m_pCommunityInfoList->InsertItemBack();
    _addListItem(idx, c_dwTextColor, szMessage);

#endif //_NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY

    // 1730 클래스: %s
    g_InterfaceManager.GetInterfaceString(1730, szTemp, INTERFACE_STRING_LENGTH);
    Snprintf(szMessage, 
        INTERFACE_STRING_LENGTH-1, 
        szTemp, 
        GlobalFunc::GetClassString((eCHAR_TYPE)character_class));
    idx = m_pCommunityInfoList->InsertItemBack();
    _addListItem(idx, c_dwTextColor, szMessage);

	// 1674	길드: [%s]의 %s
	if(_tcslen(GetManager()->m_FriendInfo.m_tszGuildName))
	{
		g_InterfaceManager.GetInterfaceString(1674, szTemp, INTERFACE_STRING_LENGTH);
		
		int iStringIndex = _getMemberStringIndex((eGUILD_DUTY)GetManager()->m_FriendInfo.m_byDuty);
		if(iStringIndex > 0)
		{
			g_InterfaceManager.GetInterfaceString(iStringIndex, szTemp2, INTERFACE_STRING_LENGTH);

			Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp, GetManager()->m_FriendInfo.m_tszGuildName, szTemp2);
		}
		else
		{
			//1095	길드: %s
			g_InterfaceManager.GetInterfaceString(1095, szTemp);
			Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp, GetManager()->m_FriendInfo.m_tszGuildName);
		}

		
		idx = m_pCommunityInfoList->InsertItemBack();
		_addListItem(idx, c_dwTextColor, szMessage);
	}


	// 공백 한줄
	Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, _T(""));
	idx = m_pCommunityInfoList->InsertItemBack();
	_addListItem(idx, c_dwTextColor, szMessage);


	// 1684	접속정보
	g_InterfaceManager.GetInterfaceString(1684, szMessage, INTERFACE_STRING_LENGTH);
	idx = m_pCommunityInfoList->InsertItemBack();
	_addListItem(idx, c_dwCategoryTextColor, szMessage);


    if (GetManager()->IsOnline(GetManager()->m_FriendInfo.m_ptszCharName) == TRUE)
    {
        // 1677	접속중인 채널: %d 채널
        g_InterfaceManager.GetInterfaceString(1677, szTemp, INTERFACE_STRING_LENGTH);
        Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp, GetManager()->m_FriendInfo.m_byChannel);
        idx = m_pCommunityInfoList->InsertItemBack();
        _addListItem(idx, c_dwTextColor, szMessage);

        // 1678	캐릭터 상태: %s
        g_InterfaceManager.GetInterfaceString(1678, szTemp, INTERFACE_STRING_LENGTH);

        //캐릭터 상태:
        int iZoneType = 685;//알수없음
        switch(GetManager()->m_FriendInfo.m_byZoneType)
        {
        case eZONETYPE_LOBBY:		iZoneType =	1679; break;//로비
        case eZONETYPE_VILLAGE:		iZoneType = 1336; break;//마을
        case eZONETYPE_CHARSELECT:	iZoneType =	50;	  break;//캐릭터씬 
        case eZONETYPE_FIELD:		iZoneType =	1680; break;//필드
        case eZONETYPE_CHUNTING:	iZoneType = 641;  break;//헌팅
        case eZONETYPE_MISSION:		iZoneType = 640;  break;//미션
        case eZONETYPE_PVP:			iZoneType = 685;  break;//PVP
        case eZONETYPE_INSTANCE:	iZoneType = 5455; break;//던젼
        }
        g_InterfaceManager.GetInterfaceString(iZoneType, szTemp2, INTERFACE_STRING_LENGTH ); // 알 수 없음


        Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp, szTemp2);
        idx = m_pCommunityInfoList->InsertItemBack();
        _addListItem(idx, c_dwTextColor, szMessage);

        if( GetManager()->m_FriendInfo.m_byZoneType == eZONETYPE_LOBBY || 
            GetManager()->m_FriendInfo.m_byZoneType == eZONETYPE_CHUNTING ||
            GetManager()->m_FriendInfo.m_byZoneType == eZONETYPE_MISSION || 
            GetManager()->m_FriendInfo.m_byZoneType == eZONETYPE_PVP)
        {
            // 1682	진행중인 배틀존 번호: %d
            if(GetManager()->m_FriendInfo.m_dwZoneKey)
            {
                g_InterfaceManager.GetInterfaceString(1682, szTemp, INTERFACE_STRING_LENGTH);
                Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp, GetManager()->m_FriendInfo.m_dwZoneKey);
                idx = m_pCommunityInfoList->InsertItemBack();
                _addListItem(idx, c_dwTextColor, szMessage);
            }
        }

        // 1683	상세위치: %s
        int iStrIndx = _getMapNameStringIndex(GetManager()->m_FriendInfo.m_MapCode);
        if(iStrIndx)
        {
            g_InterfaceManager.GetInterfaceString(1683, szTemp, INTERFACE_STRING_LENGTH);
            g_InterfaceManager.GetInterfaceString(iStrIndx, szTemp2, INTERFACE_STRING_LENGTH);
            Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp, szTemp2);
            idx = m_pCommunityInfoList->InsertItemBack();
            _addListItem(idx, c_dwTextColor, szMessage);
        }
    }
    else
    {
        //! offline
        g_InterfaceManager.GetInterfaceString(70501, szMessage ,INTERFACE_STRING_LENGTH);
        idx = m_pCommunityInfoList->InsertItemBack();
        _addListItem(idx, c_dwTextColor, szMessage);
    }
    
#ifdef _NA_0_20100527_ADD_CHAR_RELATION_MEMO
    if (multilineedit_control() != NULL)
    {
        multilineedit_control()->SetTextWZ(GetManager()->m_FriendInfo.m_ptszMemo);
    }
#endif //_NA_0_20100527_ADD_CHAR_RELATION_MEMO
    
    resetScroll();

}

//------------------------------------------------------------------------------
/**
*/
int uiCommunityInfo::_getMapNameStringIndex(CODETYPE mapCode)
{
	const sMAPINFO * pMapInfo = MapInfoParser::Instance()->FindMapInfo((MAPCODE)mapCode);
	if(pMapInfo)
	{
		return pMapInfo->dwNCode;
	}

	return 0;
}

//------------------------------------------------------------------------------
/**
*/
int uiCommunityInfo::_getMemberStringIndex(eGUILD_DUTY eDuty)
{
	DWORD dwStringIndex = 0;
	uiGuildMan * pGuildMan = (uiGuildMan *) g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUILD);
	if( pGuildMan )
	{
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
        dwStringIndex = GuildSystem::Function::GetGuildDutyStringCode(eDuty);
#else
        GUILD_DUTY_USE_INFO * pDutyUseInfo = GuildRankInfoParser::Instance()->GetGuildDutyUseInfo(GRADE_AT_GUILD_CREATE, eDuty);
        if(pDutyUseInfo)
            dwStringIndex = pDutyUseInfo->m_dwStringCode;
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
	}
	return dwStringIndex;
}

//------------------------------------------------------------------------------
LEVELTYPE uiCommunityInfo::GetSelectPlayerLevel(TCHAR* character_name)
{
    if (request_uimanager_id() == UIMAN_GUILD)
    {
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
        GUILD_MEMBER_INFO* guild_member_info = 
            GuildSystem::GetGuildSystemData().GetGuildMemberInfo(character_name);
        if (guild_member_info != NULL)
        {
            return guild_member_info->m_Level;
        }
#else
        uiGuildMan* guild_manager = static_cast<uiGuildMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUILD));
        if (guild_manager)
        {
            stGUILD_MEMBER_INFO* member_info = guild_manager->GetGuildMemberInfo(character_name);
            if (member_info)
            {
                return member_info->m_Level;
            }
        }
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER        
        return 0;
    }

    return GetManager()->GetLevel();
}

#ifdef _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY
GenderType::Value uiCommunityInfo::GetSelectPlayerGender(TCHAR* character_name)
{
    return GetManager()->GetGender();
}
#endif //_NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY

//------------------------------------------------------------------------------
BYTE uiCommunityInfo::GetSelectPlayerClass(TCHAR* character_name)
{
    if (request_uimanager_id() == UIMAN_GUILD)
    {
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
        GUILD_MEMBER_INFO* guild_member_info = 
            GuildSystem::GetGuildSystemData().GetGuildMemberInfo(character_name);
        if (guild_member_info != NULL)
        {
            return guild_member_info->m_byClass;
        }
#else
        uiGuildMan* guild_manager = static_cast<uiGuildMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUILD));
        if (guild_manager)
        {
            stGUILD_MEMBER_INFO* member_info = guild_manager->GetGuildMemberInfo(character_name);
            if (member_info)
            {
                return member_info->m_byClass;
            }
        }
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER        
        return 0;
    }

    return GetManager()->GetClass();
}

void uiCommunityInfo::UpScroll()
{
    CCtrlVScrollWZ* pControl = 
        static_cast<CCtrlVScrollWZ*>(getControl(COMMUNITY_VSCROLL_V000));

    if(pControl == NULL)
    {
        return;
    }

    int pos = pControl->GetScrollPosWZ();
    int mmin = 0, mmax = 0;
    pControl->GetScrollRangeWZ(&mmin, &mmax);
    if(pos > mmin)
    {
        --pos;
        pControl->SetScrollPosWZ(pos);
    }

    m_pCommunityInfoList->SetBeginDrawIndex(pos);
}

void uiCommunityInfo::DownScroll()
{
    CCtrlVScrollWZ* pControl = 
        static_cast<CCtrlVScrollWZ*>(getControl(COMMUNITY_VSCROLL_V000));

    if(pControl == NULL)
    {
        return;
    }

    int pos = pControl->GetScrollPosWZ();
    int mmin = 0, mmax = 0;
    pControl->GetScrollRangeWZ(&mmin, &mmax);

    if (pos < mmax)
    {
        ++pos;
        pControl->SetScrollPosWZ(pos);
    }

    m_pCommunityInfoList->SetBeginDrawIndex(pos);
}

void uiCommunityInfo::ThumbScroll()
{
    CCtrlVScrollWZ* pControl = 
        static_cast<CCtrlVScrollWZ*>(getControl(COMMUNITY_VSCROLL_V000));

    if(pControl == NULL)
    {
        return;
    }

    int pos2 = pControl->GetScrollPosWZ();
    int pos = pos2;
    int mmin = 0, mmax = 0;
    pControl->GetScrollRangeWZ(&mmin, &mmax);

    pos = max(mmin, pos);
    pos = min(mmax, pos);

    if(pos != pos2)
    {
        pControl->SetScrollPosWZ(pos);
    }

    m_pCommunityInfoList->SetBeginDrawIndex(pos);
}

void uiCommunityInfo::resetScroll()
{
    CCtrlVScrollWZ* pControl = 
        static_cast<CCtrlVScrollWZ*>(getControl(COMMUNITY_VSCROLL_V000));

    if(pControl == NULL)
    {
        return;
    }

    pControl->ShowWindowWZ(WZ_SHOW);
    if(CControlWZ* pPictureControl = getControl(COMMUNITY_PICTURE_P003))
    {
        pPictureControl->ShowWindowWZ(WZ_SHOW);
    }

    int scroll_max_num = max(0, m_pCommunityInfoList->GetListRowCnt() - m_pCommunityInfoList->GetDrawRowCount());

    if(scroll_max_num == 0)
    {
        pControl->ShowWindowWZ(WZ_HIDE);
        if(CControlWZ* pPictureControl = getControl(COMMUNITY_PICTURE_P003))
        {
            pPictureControl->ShowWindowWZ(WZ_HIDE);
        }

        return;
    }

    int thumb_size = 100 / (scroll_max_num + 1);
    thumb_size = max(10, thumb_size);
    pControl->SetChangeSizeThumb(thumb_size);
    pControl->SetScrollRangeWZ(0, scroll_max_num);

    int begin_index = m_pCommunityInfoList->GetBeginDrawIndex();

    int new_draw_index = begin_index <= scroll_max_num ? begin_index : scroll_max_num;

    m_pCommunityInfoList->SetBeginDrawIndex(new_draw_index);
    pControl->SetScrollPosWZ(new_draw_index);
}