//------------------------------------------------------------------------------
//  uiGuildMan_main.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

#include "uiGuildMan.h"
#include "uiGuildMain/uiGuildMain.h"
#include "uiGuildCreate/uiGuildCreate.h"
#include "uiGuildMenu/uiGuildMenu.h"
#include "uiGuildWindow/uiGuildWindow.h"
#include "uitargetman/uitargetman.h"
#include "TargetDialog.h"
#include "ObjectManager.h"
#include "globalfunc.h"
#include "InterfaceManager.h"
#include "uiSystemMan/uiSystemMan.h"
#include "uiGuildConvertGP.h"
#include "uiGuildDonateUP.h"
#include "uiGuildRankup.h"
#include "GuildRankInfoParser.h"
#include "uiGuildCorpsChange/uiGuildCorpsChange.h"
#ifdef _NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
    #include "GuildSystem/uiGuildWareHouse.h"
#else
    #include "uiGuildWareHouse/uiGuildWareHouse.h"
#endif //_NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
#include "uiGuildMarkCombination/uiGuildMarkCombination.h"
#include "uiGuildMarkRegisteConfirm/uiGuildMarkRegisteConfirm.h"

void _Callback_Guild_WithDraw_Warning(bool bYes)
{
	if(bYes)
	{
		MSG_CG_GUILD_WITHDRAW_SYN sync;
		GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sizeof (sync));
	}
}

template <class T, T stGUILD_MEMBER_INFO::*p>
bool sortGuildMemberByTypeHigh(stGUILD_MEMBER_INFO & lhw, stGUILD_MEMBER_INFO& rhw)
{
	return lhw.*p < rhw.*p;
}

template <class T, T stGUILD_MEMBER_INFO::*p>
bool sortGuildMemberByTypeLow(stGUILD_MEMBER_INFO & lhw, stGUILD_MEMBER_INFO& rhw)
{
	return lhw.*p > rhw.*p;
}

//! vs2008 에서는
//! 기본 클래스의 멤버 포인터에서 파생 클래스의 멤버 포인터로의 표준 변환은 템플릿 인수에 적용되지 않습니다.
template <class T, T BASE_GUILD_MEMBER_INFO::*p>
bool sortGuildMemberByTypeHigh_Base(stGUILD_MEMBER_INFO & lhw, stGUILD_MEMBER_INFO& rhw)
{
    return lhw.*p < rhw.*p;
}

template <class T, T BASE_GUILD_MEMBER_INFO::*p>
bool sortGuildMemberByTypeLow_Base(stGUILD_MEMBER_INFO & lhw, stGUILD_MEMBER_INFO& rhw)
{
    return lhw.*p > rhw.*p;
}

//스트링 StrCmp이용한 정렬
bool sortGuildMemberByNameHigh(stGUILD_MEMBER_INFO & lhw, stGUILD_MEMBER_INFO & rhw)
{
	int returnValue =  StrnCmp(lhw.m_tszCharName , rhw.m_tszCharName, MAX_CHARNAME_LENGTH);
	if( returnValue < 0 )
		return true;
	else
		return false;
}

bool sortGuildMemberByNameLow(stGUILD_MEMBER_INFO & lhw, stGUILD_MEMBER_INFO & rhw)
{
	int returnValue =  StrnCmp(lhw.m_tszCharName , rhw.m_tszCharName, MAX_CHARNAME_LENGTH);
	if( returnValue > 0 )
		return true;
	else
		return false;
}

//------------------------------------------------------------------------------
/**
*/
void _KickGuildMember(bool bYes)
{
	if(bYes)
	{
		uiGuildMan* guildMan =
                    static_cast<uiGuildMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUILD));

		if(guildMan && guildMan->GetKickUser())
		{
			MSG_CG_GUILD_KICKPLAYER_SYN sync;
			StrnCopy(sync.m_ptszKickCharName, guildMan->GetKickUser(), MAX_CHARNAME_LENGTH);
            sync.m_ptszKickCharName[_countof(sync.m_ptszKickCharName) - 1] = '\0';
			GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sizeof (sync));
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
uiGuildMan::uiGuildMan(InterfaceManager *pUIMan) :
        uiBaseMan(pUIMan)
        ,request_uibase_guild_totalinfo_(0)
{
    this->m_bGuildInfo = false;
    this->m_bSilentGuildInfo = false;
    this->m_bShowGuildMain = false;
	this->m_bGuildMemberInfo = false;

    m_pUIGuildMain = NULL;
    m_pUIGuildCreate = NULL;
    m_pUIGuildMenu = NULL;
    m_pUIGuildWindow = NULL;
    active_tab_ = NULL;

	m_strGuildName.clear();
	m_strGuildMasterName.clear();
	ZeroMemory(m_tszKickMember, sizeof(m_tszKickMember));
	ZeroMemory(m_tszNotice, sizeof(m_tszNotice));

    this->m_timerGuildMemberInfoGet.Off();

	m_bForSort = TRUE;

	ZeroMemory(&m_GuildCorpsTotalInfo,sizeof(m_GuildCorpsTotalInfo));

	m_hGuildMarkTexture = g_pSunRenderer->x_pManagerTexture->LoadTexture( "Data\\Interface\\all_GuildMark.tga" ,TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);

	ZeroMemory(&m_GuildInfo, sizeof(m_GuildInfo));
}

//------------------------------------------------------------------------------
/**
*/
uiGuildMan::~uiGuildMan()
{
    active_tab_ = NULL;
}

//------------------------------------------------------------------------------
/**
*/
void
uiGuildMan::OnInitialize()
{  
    _LoadUI();
}

//------------------------------------------------------------------------------
/**
*/
void
uiGuildMan::OnRelease()
{  
	DelRelationCon();

	if( m_hGuildMarkTexture != INVALID_HANDLE_VALUE )
	{
		g_pSunRenderer->x_pManagerTexture->DestroyTexture( m_hGuildMarkTexture );
		m_hGuildMarkTexture = INVALID_HANDLE_VALUE;
	}
}

//------------------------------------------------------------------------------
void uiGuildMan::OnReset()
{
    set_active_tab(NULL);
}


//------------------------------------------------------------------------------
/**
*/
void
uiGuildMan::Clear()
{
    this->m_bGuildInfo = false;
	this->m_bGuildMemberInfo = false;

	m_strGuildName.clear();
	m_strGuildMasterName.clear();
	ZeroMemory(m_tszKickMember, sizeof(m_tszKickMember));
	
	ZeroMemory(&m_GuildInfo, sizeof(m_GuildInfo));
	if (g_pHero)
	{
		g_pHero->ClearGuildRenderInfo();
	}

}

//------------------------------------------------------------------------------
/**
*/
bool
uiGuildMan::CheckCreatCondition()
{
    TCHAR   szMessage[INTERFACE_STRING_LENGTH], szText[INTERFACE_STRING_LENGTH];

    if (this->IsGuildInfo())
    { 
		//	이미 '%s' 길드에 속해 있읍니다.
		g_InterfaceManager.GetInterfaceString( eST_ALREADY_NAMECODE_GUILD_MEMBER, szText, INTERFACE_STRING_LENGTH );
        Sprintf (szMessage, szText, this->m_GuildInfo.m_tszGuildName );

		uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
		if(pSystemMan)
			pSystemMan->Sys_VerifyLock(szMessage, FALSE);

        return false;
    }

    if (!g_pHero)
    {
        GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT, _T("My 캐릭터가 없습니다. (g_pHero == NULL)"));
        return false;
    }

    if (g_pHero->GetLevel() < CREATE_GUILD_LIMIT_LEVEL)
    {
		//	길드 창설자의 레벨은 20 이상이어야 합니다.
		g_InterfaceManager.GetInterfaceString( eST_CHECK_GUILD_MASTER_LEVEL_20, szText, INTERFACE_STRING_LENGTH );

		uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
		if(pSystemMan)
			pSystemMan->Sys_VerifyLock(szText, FALSE);


        return false;
    }

    BASE_PLAYERINFO* bp = g_pHero->GetCharInfo();

    if (!bp)
    {
        GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT, _T("My 캐릭터의 BASE_PLAYERINFO 가 NULL 입니다."));
        return false;
    }

	MONEY	money = -1;

	GuildRankInfo	* pRankInfo = GuildRankInfoParser::Instance()->FindGuildRankInfo( GRADE_AT_GUILD_CREATE );
	
	if( pRankInfo != NULL )
		money = pRankInfo->m_dwHeim;

	if( bp->m_Money < static_cast<INT64>(money) )
    {
		//	자금이 부족해 길드를 창설할 수 없습니다!!
		g_InterfaceManager.GetInterfaceString( eST_NOT_ENOUGH_MONEY_FOR_CREATE_GUILD, szText, INTERFACE_STRING_LENGTH );

		uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
		if(pSystemMan)
			pSystemMan->Sys_VerifyLock(szText, FALSE);

		return false;
    }

    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
uiGuildMan::SEND_CG_GUILD_DESTROY_SYN()
{
    MSG_CG_GUILD_DESTROY_SYN send;
    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send, sizeof (send));
}

//------------------------------------------------------------------------------
/**
*/
void uiGuildMan::SEND_CG_GUILD_TOTALINFO_REQ_SYN(uiBase* base)
{
    request_uibase_guild_totalinfo_ = base;

	if(eGUILD_DELCHK_DESTROY_WAIT == GetGuildDelChk())
	{
		TCHAR szMessage[INTERFACE_STRING_LENGTH + 1] = {0,};
		//6236	길드 해산 대기 중입니다.
		g_InterfaceManager.GetInterfaceString(6236, szMessage);
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
		return;
	}

	if( IsGuildMemberInfo() == FALSE )
	{
		MSG_CG_GUILD_TOTALINFO_REQ_SYN SendPacket;
		GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));
		GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT, _T("MSG_CG_GUILD_TOTALINFO_REQ_SYN"));
	}
	else
	{
        if (base == NULL)
        {
            ShowGuildWindow();
        }
	}

}

//------------------------------------------------------------------------------
/**
*/
void uiGuildMan::ShowGuildWindow()
{
    ShowTab();
    //if (active_tab() != NULL)
    //{
    //    active_tab()->ShowInterface(TRUE);
    //}
    //else if ((m_pUIGuildMain != NULL) && (m_pUIGuildMain->IsVisibleWindow() == FALSE))
    //{
    //    m_pUIGuildMain->ShowInterface(TRUE);
    //}
}

//------------------------------------------------------------------------------
/**
*/
void uiGuildMan::SEND_CG_GUILD_INVITE_SYN( LPCTSTR szName )
{
	if( szName == NULL || _tcslen( szName ) == 0 )
		return;

	MSG_CG_GUILD_INVITE_SYN send;

	_tcsncpy( send.m_TargetCharName, szName, MAX_CHARNAME_LENGTH );

	 GlobalFunc::SendPacket( CI_GAMESERVERIDX, &send, sizeof( send ) );
}

//------------------------------------------------------------------------------
/**
*/
void uiGuildMan::SEND_CG_GUILD_GUILDMARK_DELETE_SYN()
{
	if( GetGuildInfo().m_PatternIdx && GetGuildInfo().m_BackGroundIdx )
	{
		MSG_CG_GUILD_GUILDMARK_DELETE_SYN SendMsg;
		SendMsg.m_GuildIndex = GetGuildInfo().m_GuildGuid;
		SendMsg.m_BackGroundIndex = GetGuildInfo().m_BackGroundIdx;
		SendMsg.m_PatternIndex  = GetGuildInfo().m_PatternIdx;
		GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendMsg, sizeof(SendMsg));
	}
	else
	{
		uiSystemMan* pSystemMan =
			static_cast<uiSystemMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM));
		if( pSystemMan )
		{
			//6272	길드마크가 등록되어 있지않습니다.
			pSystemMan->Sys_Verify(g_InterfaceManager.GetInterfaceString(6272), FALSE);
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiGuildMan::SetGuildMarkBackGround(int iBackGround)
{
	m_GuildInfo.m_BackGroundIdx = iBackGround;

	if( g_pHero )
	{
		g_pHero->GetGuildRenderInfo().m_BackGroundIndex = iBackGround;
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiGuildMan::SetGuildMarkPattern(int iPattern)
{
	m_GuildInfo.m_PatternIdx = iPattern;
	if( g_pHero )
	{
		g_pHero->GetGuildRenderInfo().m_PatternIndex = iPattern;
	}
}


//------------------------------------------------------------------------------
/**
*/
void
uiGuildMan::SEND_CG_GUILD_WITHDRAW_SYN()
{
    MSG_CG_GUILD_WITHDRAW_SYN send;
    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send, sizeof (send));
}


//------------------------------------------------------------------------------
/**
*/
void uiGuildMan::SEND_CG_GUILD_RANKUP_SYN()
{
	MSG_CG_GUILD_RANKUP_SYN send;
	GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send, sizeof (send));
	GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT, _T("SEND_CG_GUILD_RANKUP_SYN"));
}

//------------------------------------------------------------------------------
/**
*/
void uiGuildMan::SEND_CG_GUILD_CONVERT_UP2GP_SYN()
{
	MSG_CG_GUILD_CONVERT_UP2GP_SYN send;
	send.m_ConvertGP = m_pUIGuildConvertGP->GetConverGP();
	GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send, sizeof (send));
	GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT, _T("SEND_CG_GUILD_CONVERT_UP2GP_SYN"));
}

//------------------------------------------------------------------------------
/**
*/
void uiGuildMan::SEND_CG_GUILD_DONATE_UP_SYN()
{
	MSG_CG_GUILD_DONATE_UP_SYN send;
	send.m_DonateUP	= m_pUIGuildDonateUP->GetDonateUP();
	GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send, sizeof (send));
	GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT, _T("SEND_CG_GUILD_DONATE_UP_SYN"));
}

//------------------------------------------------------------------------------
/**
*/
void uiGuildMan::SEND_CG_GUILD_WAREHOUSE_OPEN_SYN()
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH];

	if (m_pUIGuildWareHouse->IsVisible())
	{
		//	창고가 열린 상태입니다.
		g_InterfaceManager.GetInterfaceString( eST_WAREHOUSE_ALREADY_OPEN, szMessage, INTERFACE_STRING_LENGTH );
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
		return;
	}

	MSG_CG_GUILD_WAREHOUSE_OPEN_SYN send;
	send.m_GuildIndex = GetGuildInfo().m_GuildGuid;
	GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send, sizeof(send));
	GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT, _T("SEND_CG_GUILD_WAREHOUSE_OPEN_SYN"));
}

void uiGuildMan::SEND_CG_GUILD_WAREHOUSE_CLOSE_SYN()
{
	MSG_CG_GUILD_WAREHOUSE_CLOSE_SYN send;
	send.m_GuildIndex = GetGuildInfo().m_GuildGuid;
	BOOL bVal = GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send, sizeof(send));
	if(bVal)
		GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT, _T("SEND_CG_GUILD_WAREHOUSE_CLOSE_SYN"));
	else
		GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT, _T("SEND_CG_GUILD_WAREHOUSE_CLOSE_SYN 못보냈다."));
}

BOOL uiGuildMan::IsGuildWareHouseWindow()
{
	if(m_pUIGuildWareHouse)
		return m_pUIGuildWareHouse->IsVisible();
	else
		return FALSE;
}

BOOL uiGuildMan::IsGuildRankUpWindow()
{
	if(m_pUIGuildRankup)
		return m_pUIGuildRankup->IsVisible();
	else
		return FALSE;
}

void uiGuildMan::ShowGuildRankUpWindow(BOOL bVal)
{
	m_pUIGuildRankup->ShowInterface(bVal);
}


//------------------------------------------------------------------------------
/**
*/
bool uiGuildMan::Invite(const TCHAR* pname)
{
	TCHAR       szMessage[INTERFACE_STRING_LENGTH] = {0,};

    if (pname == NULL && !(GET_DIALOG(IM_TARGET_MANAGER::TARGET_DIALOG)->IsVisible()))
    {
        //	선택된 플레이어가 없습니다.
        this->GetUIMan()->GetInterfaceString( eST_NOT_SELECTED_PLAYER, szMessage, INTERFACE_STRING_LENGTH );
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
        return false;
    }

	if(!_isEnableInvite())
	{
		return false;
	}

    DWORD       dwTargetKey = 0;
    Object *    pTargetObj = NULL;
	TCHAR		szName[32];

    if (pname == NULL)
    {
        // 타겟창에 있는 상대와의 거래

        TargetDialog * pTargetDlg = GET_CAST_DIALOG(TargetDialog, IM_TARGET_MANAGER::TARGET_DIALOG);
        assert (pTargetDlg);

		dwTargetKey = pTargetDlg->GetTargetKey();
		pTargetObj = g_ObjectManager.GetObject(dwTargetKey);
		assert (pTargetObj);
		if ( pTargetObj == NULL)
		{
			TCHAR* pInviteName = pTargetDlg->GetTargetName();
			assert (pInviteName);
			_tcsncpy( szName, pInviteName, 32 );
		}
		else
			_tcsncpy( szName, pTargetObj->GetName(), 32 );
        
    }
    else
    {
        // 입력한 아이디와의 거래

#ifdef _K2P_OBJECT_MANAGER_MODIFIDY
		if( g_ObjectManager.GetObject( pname ) == NULL )
		{
            // '%s' 님을 찾을 수 없습니다.
			g_InterfaceManager.GetInterfaceString( eST_NAMECODE_NOT_FOUND, szMessage, INTERFACE_STRING_LENGTH );
            GlobalFunc::OutputMsg_Warning(eOUTPUT_MSG_CHAT, szMessage, pname);
            return false;
		}
#endif
		_tcsncpy( szName, pname, 32 );
    }

	if( _tcslen( szName ) == 0 )
		return false;
   
	this->SEND_CG_GUILD_INVITE_SYN( szName );

    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool					
uiGuildMan::_isEnableInvite()
{
	if(!g_pHero)
		return false;

	TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};

	if (!IsGuildInfo())
	{
		//5165	길드에 가입되어 있지 않습니다.
		g_InterfaceManager.GetInterfaceString( 5165, szMessage, INTERFACE_STRING_LENGTH );
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
		return false;
	}

	return true;
}

//------------------------------------------------------------------------------
/**
*/
bool					
uiGuildMan::_isEnableNotice()
{
	if(!g_pHero)
		return false;

	TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};

	if (!IsGuildInfo())
	{
		//5165	길드에 가입되어 있지 않습니다.
		g_InterfaceManager.GetInterfaceString( 5165, szMessage, INTERFACE_STRING_LENGTH );
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
		return false;
	}

	return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
uiGuildMan::_isEnableKick()
{
	if(!g_pHero)
		return false;

	TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};

	if (!IsGuildInfo())
	{
		//5165	길드에 가입되어 있지 않습니다.
		g_InterfaceManager.GetInterfaceString( 5165, szMessage, INTERFACE_STRING_LENGTH );
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
		return false;
	}

	return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
uiGuildMan::Withdraw()	//탈퇴
{
	if(!g_pHero)
		return false;

	TCHAR szMessage[INTERFACE_STRING_LENGTH + 1] ={0,};

	if (!IsGuildInfo())
	{
		//5165	길드에 가입되어 있지 않습니다.
		g_InterfaceManager.GetInterfaceString( 5165, szMessage);
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
		return false;
	}

	//6209 길드를 탈퇴하게 되면 3일간 길드 가입이 가능하지 않습니다. 정말 탈퇴하시겠습니까?
	g_InterfaceManager.GetInterfaceString( 6209, szMessage);
	GlobalFunc::SysConfirmBox( szMessage, _Callback_Guild_WithDraw_Warning );

	return true;
}

//------------------------------------------------------------------------------
/**
*/

bool
uiGuildMan::Kick(const TCHAR* pname)
{
	if(pname == NULL)
		return false;

	if(!_isEnableKick())
	{
		return false;
	}

	TCHAR   szMessage[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR	szTemp[INTERFACE_STRING_LENGTH] = {0,};

	StrnCopy(m_tszKickMember, pname, MAX_CHARNAME_LENGTH);
    m_tszKickMember[_countof(m_tszKickMember) - 1] = '\0';

	//7220	길드원을 제명하게 되면 3일간 가입을 권유할 수 없습니다.%s 길드원을 정말 제명하시겠습니까?
	g_InterfaceManager.GetInterfaceString(7220, szTemp);
	Snprintf(szMessage, INTERFACE_STRING_LENGTH -1,  szTemp, m_tszKickMember);
	
	uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
	if(pSystemMan)
		pSystemMan->Sys_Confirm(szMessage, _KickGuildMember);

	return true;
}


//------------------------------------------------------------------------------
/**
*/
bool
uiGuildMan::Notice(TCHAR* pmsg)
{
	if(pmsg == NULL)
		return false;

	if(!_isEnableNotice())
	{
		return false;
	}
	
	if(GlobalFunc::IsSlang(pmsg, true) == TRUE)
	{
		return false;
	}

	TCHAR szWideString[MAX_GUILDNOTICE_LENGTH+1] ={0,};
	StrnCopy(szWideString , pmsg, MAX_GUILDNOTICE_LENGTH);
	szWideString[MAX_GUILDNOTICE_LENGTH] = NULL;

	MSG_CW_GUILD_CHANGE_NOTICE_SYN sync;
	ZeroMemory(sync.m_tszGuildNotice, sizeof(sync.m_tszGuildNotice));
	StrnCopy(sync.m_tszGuildNotice, szWideString, MAX_GUILDNOTICE_LENGTH-1);
	sync.m_byNoticeLen = strlen(sync.m_tszGuildNotice);
	GlobalFunc::SendPacket(CI_CHATSERVERIDX, &sync, sizeof(sync));

	return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
uiGuildMan::IsShowWindow()
{
	bool bShow = false;

	if(m_pUIGuildMain && m_pUIGuildMain->IsVisible())
	{
		bShow = true;
	}

	if(m_pUIGuildWindow && m_pUIGuildWindow->IsVisible())
	{
		bShow = true;
	}

	return bShow;
}

//------------------------------------------------------------------------------
/**
*/
void
uiGuildMan::CloseWin()
{
	if(m_pUIGuildMain && m_pUIGuildMain->IsVisible())
	{
		m_pUIGuildMain->ShowInterface(FALSE);

        set_active_tab(m_pUIGuildMain);
	}

	if(m_pUIGuildWindow && m_pUIGuildWindow->IsVisible())
	{
		m_pUIGuildWindow->ShowInterface(FALSE);

        set_active_tab(m_pUIGuildWindow);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiGuildMan::_addGuildMember(GUILD_MEMBER_INFO member)
{
	m_vMembers.push_back(member);
}

//------------------------------------------------------------------------------
/**
*/
void uiGuildMan::SetWindowFocus()
{
	// 길드원정보창
	if(m_pUIGuildMain && m_pUIGuildMain->IsVisibleWindow())
	{
		m_pUIGuildMain->GetDialogWZ()->ShowWindowWZ( WZ_SHOW);
	}
	
	// 길드정보창
	if(m_pUIGuildWindow && m_pUIGuildWindow->IsVisibleWindow())
	{
		m_pUIGuildWindow->GetDialogWZ()->ShowWindowWZ( WZ_SHOW);
	}
}

//------------------------------------------------------------------------------
/**
*/
//관계 컨테이너 삭제
void uiGuildMan::DelRelationCon()
{
	std::list<GUILD_RELATION*>::iterator iter;
	for(iter = m_listGuildRelation_Alliance.begin(); iter != m_listGuildRelation_Alliance.end(); ++iter)
	{
		GUILD_RELATION* ptemp = (*iter);
		if(ptemp)
			SAFE_DELETE(ptemp);
	}
	m_listGuildRelation_Alliance.clear();

	for(iter = m_listGuildRelation_Hostility.begin(); iter != m_listGuildRelation_Hostility.end(); ++iter)
	{
		GUILD_RELATION* ptemp = (*iter);
		if(ptemp)
			SAFE_DELETE(ptemp);
	}
	m_listGuildRelation_Hostility.clear();

	for(iter = m_listGuildRelation_OneSideHostility.begin(); iter != m_listGuildRelation_OneSideHostility.end(); ++iter)
	{
		GUILD_RELATION* ptemp = (*iter);
		if(ptemp)
			SAFE_DELETE(ptemp);
	}
	m_listGuildRelation_OneSideHostility.clear();
}


bool GuildRelationSort_GuildName_Top( const GUILD_RELATION* pGuildRelation1, const GUILD_RELATION* pGuildRelation2)
{
	// 길드네임
	DWORD dwGuildName1 = StrToWzID(pGuildRelation1->m_GuildName);
	DWORD dwGuildName2 = StrToWzID(pGuildRelation2->m_GuildName);

	if(dwGuildName1 <= dwGuildName2)
		return true;
	else
		return false;

	ASSERT( !"들어 올 수 없다!!!" );

	return true;
}

bool GuildRelationSort_GuildName_Bottom( const GUILD_RELATION* pGuildRelation1, const GUILD_RELATION* pGuildRelation2)
{	
	// 길드네임
	DWORD dwGuildName1 = StrToWzID(pGuildRelation1->m_GuildName);
	DWORD dwGuildName2 = StrToWzID(pGuildRelation2->m_GuildName);

	if(dwGuildName1 >= dwGuildName2)
		return true;
	else
		return false;

	ASSERT( !"들어 올 수 없다!!!" );

	return true;
}

bool GuildRelationSort_State_Top(const GUILD_RELATION* pGuildRelation1, const GUILD_RELATION* pGuildRelation2)
{
	//현재 상태
	if(pGuildRelation1->m_eRelationType <= pGuildRelation2->m_eRelationType)
		return true;
	else
		return false;

	ASSERT( !"들어 올 수 없다!!!" );

	return true;
}

bool GuildRelationSort_State_Bottom(const GUILD_RELATION* pGuildRelation1, const GUILD_RELATION* pGuildRelation2)
{
	//현재 상태
	if(pGuildRelation1->m_eRelationType >= pGuildRelation2->m_eRelationType)
		return true;
	else
		return false;

	ASSERT( !"들어 올 수 없다!!!" );

	return true;
}

void uiGuildMan::SortAllianceCon(BOOL bOrder)
{
	if(bOrder)
		m_listGuildRelation_Alliance.sort(GuildRelationSort_GuildName_Top);
	else
		m_listGuildRelation_Alliance.sort(GuildRelationSort_GuildName_Bottom);
}

void uiGuildMan::SortHostilityCon(BOOL bOrder, BOOL bState)
{
	if(bState)
	{
		if(bOrder)
			m_listGuildRelation_Hostility.sort(GuildRelationSort_GuildName_Top);
		else
			m_listGuildRelation_Hostility.sort(GuildRelationSort_GuildName_Bottom);
	}
	else
	{
		if(bOrder)
			m_listGuildRelation_Hostility.sort(GuildRelationSort_State_Top);
		else
			m_listGuildRelation_Hostility.sort(GuildRelationSort_State_Bottom);
	}
}

void uiGuildMan::SortOneSideHostilityCon(BOOL bOrder)
{
	if(bOrder)
		m_listGuildRelation_OneSideHostility.sort(GuildRelationSort_GuildName_Top);
	else
		m_listGuildRelation_OneSideHostility.sort(GuildRelationSort_GuildName_Bottom);
}

//------------------------------------------------------------------------------ 
stGUILD_MEMBER_INFO* const uiGuildMan::GetGuildMemberInfo(int index)
{
    if (index >= 0 && index < static_cast<int>(m_vMembers.size()))
    {
        return &m_vMembers[index];
    }
    return NULL;
}
//------------------------------------------------------------------------------ 
stGUILD_MEMBER_INFO * uiGuildMan::GetGuildMemberInfo(const TCHAR * pszName)
{
	for( size_t i = 0; i < m_vMembers.size(); ++i )
	{
		if( StrnCmp(pszName, m_vMembers[i].m_tszCharName, MAX_CHARNAME_LENGTH) == 0 )
		{
			return &m_vMembers[i];
		}
	}
	return NULL;
}

void uiGuildMan::SortMember(eMEMBER_SORT_TYPE byType)
{
	static bool bHighCase = true; // default 오름 차순
	static bool bLowCase = false; // default 내림 차순


	// 다른 소팅 탭 눌렀을 때, 기본 소팅 방식으로 셋팅되도록 함.
	static eMEMBER_SORT_TYPE ePrevious = eSORT_MAX;
	if( ePrevious != byType )
	{
		bHighCase = true;
		bLowCase = false;
	}
	ePrevious = byType;

	switch (byType)
	{
	case eSORT_NAME:
		{
			if(bHighCase )
			{
				sort(m_vMembers.begin(),m_vMembers.end(), sortGuildMemberByNameHigh);
			}
			else
			{
				sort(m_vMembers.begin(),m_vMembers.end(), sortGuildMemberByNameLow);
			}
			bHighCase ^= 1;
		}
		break;


	case eSORT_CLASS:
		{
			if( bHighCase )
			{
				sort(m_vMembers.begin(), 
                    m_vMembers.end(), 
                    sortGuildMemberByTypeHigh<BYTE, &stGUILD_MEMBER_INFO::m_byClass>);	
			}
			else
			{
				sort(m_vMembers.begin(), 
                    m_vMembers.end(), 
                    sortGuildMemberByTypeLow<BYTE, &stGUILD_MEMBER_INFO::m_byClass>);	
			}
			bHighCase ^= 1;
		}
		break;
	case eSORT_DUTY:
		{
            if( bHighCase )
            {
                sort(m_vMembers.begin(), 
                    m_vMembers.end(), 
                    sortGuildMemberByTypeHigh_Base<eGUILD_DUTY, &stGUILD_MEMBER_INFO::m_eGuildDuty>);
            }
            else
            {
                sort(m_vMembers.begin(), 
                    m_vMembers.end(), 
                    sortGuildMemberByTypeLow_Base<eGUILD_DUTY, &stGUILD_MEMBER_INFO::m_eGuildDuty>);
            }
			bHighCase ^= 1;	
		}
		break;

	case eSORT_LV:
		{
			if( bHighCase )
			{
				sort(m_vMembers.begin(),
                    m_vMembers.end(), 
                    sortGuildMemberByTypeHigh<LEVELTYPE , &GUILD_MEMBER_INFO::m_Level >);
			}
			else
			{
				sort(m_vMembers.begin(),
                    m_vMembers.end(), 
                    sortGuildMemberByTypeLow<LEVELTYPE , &GUILD_MEMBER_INFO::m_Level >);
			}

			bHighCase ^= 1;
		}
		break;

	case eSORT_CONNETION:
		{
			if( bLowCase )
			{
				sort( m_vMembers.begin(), m_vMembers.end(), sortGuildMemberByTypeHigh<BOOL, &GUILD_MEMBER_INFO::m_bLogin>);
			}
			else
			{
				sort( m_vMembers.begin(), m_vMembers.end(), sortGuildMemberByTypeLow<BOOL, &GUILD_MEMBER_INFO::m_bLogin>);
			}
			bLowCase ^= 1;	
		}
		break;
	}
}

void uiGuildMan::SEND_CG_GUILD_DESTROY_REAR_CAMP_SYN()
{
	if( !g_pHero)
		return;

 	stGUILD_MEMBER_INFO * pMemberInfo =	GetGuildMemberInfo(g_pHero->GetName());
	if( pMemberInfo )
	{
		MSG_CG_GUILD_DESTROY_REAR_CAMP_SYN SendMsg;
		SendMsg.m_byCorpsNum = pMemberInfo->m_byCorpsNum;
		GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendMsg, sizeof(SendMsg));
	}
}
void uiGuildMan::ShowGuildDestoryWindow()
{
	m_pUIGuildCorpsDestory->InitAllCheckButton();
	int index = 0;
	for( int i =1; i< MAX_GUILD_CORPS_NUM; ++i)
	{
		if( GetCorpsTotalInfo().m_CorpsInfo[i].m_bCreate)
		{
			m_pUIGuildCorpsDestory->ShowAndInsertCheckCorps(index, i);
			index++;
		}
	}

	if( index == 0 )
	{
		//7163	해당 길드에 군단이 생성되지 않았습니다.
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(7163));
		m_pUIGuildCorpsDestory->ShowInterface(FALSE);
	}
	else
	{
		m_pUIGuildCorpsDestory->ShowInterface(TRUE);
	}
}
int	uiGuildMan::GetCorpMemberNum(int iCorps)
{
	if( ! m_vMembers.size() )
		return 0;

	int iNumCorpsMember = 0;
	for( size_t i =0; i < m_vMembers.size(); ++i)
	{
		if( m_vMembers[i].m_byCorpsNum == iCorps)
		{
			iNumCorpsMember++;
		}
	}
	return iNumCorpsMember;
}
void uiGuildMan::CloseAllOpenedGuildWindow()
{
	if( m_pUIGuildCorpsDestory && m_pUIGuildCorpsDestory->IsVisibleWindow())
	{
		m_pUIGuildCorpsDestory->ShowInterface(FALSE);
	}

}
void uiGuildMan::SetRelationNoneByGuildMember()
{
	//길드를 탈퇴했으므로 관계를 초기화 한다.
	Object_Map_Itr	itr = g_ObjectManager.GetBegin();
	while( g_ObjectManager.GetEnd() != itr )
	{
		Object *pObj = (Object *)itr->second;

		if( pObj && pObj->IsKindOfObject(PLAYER_OBJECT) )
		{
			Player * pPlayer = (Player *) pObj;
			if( pPlayer )
			{
				pPlayer->GetGuildRenderInfo().m_RelationType = eGUILD_RELATION_NONE;
			}
		}
		++itr;
	}

	//길드 탈퇴,제명당한 캐릭의 길드멤버 정보 날린다.
	m_vMembers.clear();
}

//------------------------------------------------------------------------------
void uiGuildMan::Send_CW_FRIEND_STATUS_REQ(char* character_name, CHARGUID char_guid)
{
    MSG_CW_FRIEND_STATUS_REQ sync;

#ifdef _NA_0_20100527_ADD_CHAR_RELATION_MEMO
    if (char_guid <= 0)
    {
        return;
    }
    sync.m_dwToCharGuid = char_guid;
#else
    StrnCopy(sync.m_ptszCharName, character_name, MAX_CHARNAME_LENGTH);
    sync.m_ptszCharName[_countof(sync.m_ptszCharName) - 1] = '\0';
#endif //_NA_0_20100527_ADD_CHAR_RELATION_MEMO

    GlobalFunc::SendPacket(CI_CHATSERVERIDX, &sync, sizeof(sync));	
}

//------------------------------------------------------------------------------ 
void uiGuildMan::ChangeTab(uiBase* ui_base)
{
    float cx = -1.0f;
    float cy = -1.0f;

    if (active_tab() != NULL)
    {
        if ((active_tab() == ui_base) && (active_tab()->IsVisible() == TRUE))
        {
            return;
        }

        cx = active_tab()->GetDialogWZ()->GetDlgX();
        cy = active_tab()->GetDialogWZ()->GetDlgY();

        active_tab()->ShowInterface(FALSE);
    }

    if (ui_base == NULL)
    {
        return;
    }

    set_active_tab(ui_base);

    if (active_tab() != NULL)
    {
        active_tab()->ShowInterface(TRUE);

        if (cx >= 0.0f && cy >= 0.0f)
        {
            active_tab()->MoveWindowWZ(cx, cy);
        }
    }
}
//------------------------------------------------------------------------------ 
void uiGuildMan::ShowTab()
{
    if (active_tab() != NULL)
    {
        active_tab()->ShowInterface(TRUE);
    }
    else
    {
        //! 기본.
        ChangeTab(m_pUIGuildMain);
    }
}

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------

