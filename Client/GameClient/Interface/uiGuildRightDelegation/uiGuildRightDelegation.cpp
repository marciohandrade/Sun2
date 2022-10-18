#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

#include "uiGuildRightDelegation.h"
#include "GuildRightInfoParser.h"

WzID uiGuildRightDelegation::m_wzID[DLGPOS_MAX] = 
{
	StrToWzID("B101"),	//DLGPOS_BTN_OK,
	StrToWzID("B102"),	//DLGPOS_BTN_CANCLE,
	StrToWzID("B100"),  //DLGPOS_BTN_EXIT
	StrToWzID("C100"),	//DLGPOS_CHECK1
	StrToWzID("CT00"),	//DLGPOS_CHECK2
	StrToWzID("CT01"),	//DLGPOS_CHECK3
	StrToWzID("CT02"),	//DLGPOS_CHECK4
	StrToWzID("CT03"),	//DLGPOS_CHECK5
	StrToWzID("CT04"),	//DLGPOS_CHECK6
	StrToWzID("T103"),	//DLGPOS_TEXT1,
	StrToWzID("T104"),	//DLGPOS_TEXT2,
	StrToWzID("ST00"),	//DLGPOS_TEXT3,
	StrToWzID("ST01"),	//DLGPOS_TEXT4,
	StrToWzID("ST02"),	//DLGPOS_TEXT5,
	StrToWzID("ST03"),	//DLGPOS_TEXT6,
};

//------------------------------------------------------------------------------
/**
*/
uiGuildRightDelegation::uiGuildRightDelegation(InterfaceManager * pUIMan) : uiBase(pUIMan)
{
	m_pGuildMan = (uiGuildMan *)GetUIMan()->GetUserInterfaceManager(UIMAN_GUILD);
	assert(m_pGuildMan);
	m_bShow = FALSE;
}
//------------------------------------------------------------------------------
/**
*/
uiGuildRightDelegation::~uiGuildRightDelegation()
{

}
//------------------------------------------------------------------------------
/**
*/
void uiGuildRightDelegation::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

	InitControl();
}

void uiGuildRightDelegation::InitControl()
{
	for( int i = 0; i <MAX_GUILD_DELEGATE_RIGHT; ++i )
	{
		m_CheckState[i].pCheck = ( CCtrlButtonCheckWZ *) getControl(DLGPOS_CHECK1 +i);
		assert( m_CheckState[i].pCheck);
		m_CheckState[i].pStatic = (CCtrlStaticWZ *) getControl(DLGPOS_TEXT1 +i);
		assert(m_CheckState[i].pStatic);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiGuildRightDelegation::Release()
{

}
//------------------------------------------------------------------------------
/**
*/
void uiGuildRightDelegation::OnUpdateSolarDialog()
{
	if( !m_bShow )
	{
		ShowInterface(FALSE);
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiGuildRightDelegation::Process(DWORD dwTick)
{

}
//------------------------------------------------------------------------------
/**
*/
void uiGuildRightDelegation::OnShowWindow(BOOL val)
{
	if( val )
	{
		m_bShow = TRUE;

		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType = eDoType_Guild_Right_Delegation;
		msg.wParam = InterfaceManager::DIALOG_GUILD_RIGHT_DELEGATION;
		msg.DoSomething = GlobalFunc::DoShowWindow;
		g_KeyQueueManager.PushBack(msg);

		MoveCenter();
		TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};
		Snprintf(szMessage, INTERFACE_STRING_LENGTH -1, (TCHAR *)g_InterfaceManager.GetInterfaceString(1228) , m_strTargetName.c_str());
		SetCaptionTitlaName(szMessage);
	}
	else
	{
		m_bShow = FALSE;
		g_KeyQueueManager.DeleteMsg(eDoType_Guild_Right_Delegation);
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiGuildRightDelegation::MessageProc(SI_MESSAGE * pMessage)
{
	switch ( pMessage->eResultMsg )
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch ( getControlIDToPos(pMessage->dwCtrlID) )
			{
			case DLGPOS_BTN_OK:
				{
					// 패킷 보내기. ( 여러개 체크해서 보낼수 있음.)
					NET_SEND_CG_GUILD_RIGHT_DELEGATION_SYN();
					m_bShow = FALSE;
				}
				break;

			case DLGPOS_BTN_CANCLE:
			case DLGPOS_BTN_EXIT:
				{
					m_bShow = FALSE;
				}
				break;
			}
		}
		break;
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiGuildRightDelegation::NetworkProc(MSG_BASE * pMsg)
{

}

void uiGuildRightDelegation::AssignCheckState(GUILD_RIGHT_USE_INFO * pRightUseInfo)
{
	if( !GetManager())
		return;


	for(int i = 0; i <MAX_GUILD_DELEGATE_RIGHT; ++i)
	{
		m_CheckState[i].pCheck->ShowWindowWZ(WZ_HIDE);
		m_CheckState[i].pStatic->ShowWindowWZ(WZ_HIDE);
		m_CheckState[i].pCheck->SetCheckState(FALSE);
	}

	int headCount = 0;
	int trailCount = MAX_GUILD_DELEGATE_RIGHT -1;
	for( int i = 0; i < MAX_GUILD_DELEGATE_RIGHT; ++i)
	{ 
		if(GuildRightInfoParser::Instance()->CheckGradeDelegateRight(pRightUseInfo[i].m_Right ,GetManager()->GetGuildInfo().m_GuildGrade) )
		{
			GuildRightInfo * pInfo = GuildRightInfoParser::Instance()->Find( pRightUseInfo[i].m_Right);
			if( pInfo )
			{
				assert(pInfo);
				m_CheckState[headCount].pCheck->ShowWindowWZ(WZ_SHOW);
				m_CheckState[headCount].pStatic->ShowWindowWZ(WZ_SHOW);
				m_CheckState[headCount].pCheck->SetCheckState(pRightUseInfo[i].m_bRightUse);
				GlobalFunc::SetCaption(m_CheckState[headCount].pStatic, (TCHAR *)g_InterfaceManager.GetInterfaceString(pInfo->m_dwStringCode));
				m_CheckState[headCount].eGuildRight = pRightUseInfo[i].m_Right;
				headCount++;
			}
		}
		else
		{
			//소팅을 위해서 뒤에서부터 삽입한다.
			m_CheckState[trailCount].eGuildRight = pRightUseInfo[i].m_Right;
			trailCount--;
		}
	}
	ShowInterface(TRUE);
}
void uiGuildRightDelegation::NET_SEND_CG_GUILD_RIGHT_DELEGATION_SYN()
{
	MSG_CG_GUILD_RIGHT_DELEGATION_SYN SendMsg;
	StrnCopy(SendMsg.m_szTargetCharName , m_strTargetName.c_str(), MAX_CHARNAME_LENGTH);

	for( int i =0; i< MAX_GUILD_DELEGATE_RIGHT; ++i )
	{
		SendMsg.m_DelegateRight[i].m_Right = m_CheckState[i].eGuildRight;
		if( m_CheckState[i].pCheck->GetState() == WZ_SHOW )
		{
			SendMsg.m_DelegateRight[i].m_bRightUse = m_CheckState[i].pCheck->GetCheckState();
		}
		else
		{
			SendMsg.m_DelegateRight[i].m_bRightUse = FALSE;
		}
	}
	GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendMsg, sizeof(SendMsg));
}

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
