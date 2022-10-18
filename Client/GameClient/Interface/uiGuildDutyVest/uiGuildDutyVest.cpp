#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

#include "uiGuildDutyVest.h"
#include "uiGuildMan/uiGuildMan.h"
#include "Hero.h"
#include "GuildRankInfoParser.h"
#include "GameConst_Color.h"
#include "uiGuildMain/uiGuildMain.h"


WzID uiGuildDutyVest::m_wzID[DLGPOS_MAX] = 
{
	StrToWzID("B101"),	//DLGPOS_BTN_OK,
	StrToWzID("B102"),	//DLGPOS_BTN_CANCLE,
	StrToWzID("B100"),  //DLGPOS_BTN_EXIT

	StrToWzID("C100"),	//DLGPOS_CHECK1,
	StrToWzID("CT00"),	//DLGPOS_CHECK2,
	StrToWzID("CT01"),	//DLGPOS_CHECK3,
	StrToWzID("CT02"),	//DLGPOS_CHECK4,
	StrToWzID("CT03"),	//DLGPOS_CHECK5,
	StrToWzID("CT04"),	//DLGPOS_CHECK6

	StrToWzID("T103"),  //DLGPOS_CHECK_TEXT1,
	StrToWzID("T104"),	//DLGPOS_CHECK_TEXT2,
	StrToWzID("T105"),	//DLGPOS_CHECK_TEXT3,
	StrToWzID("T106"),	//DLGPOS_CHECK_TEXT4,
	StrToWzID("ST00"),	//DLGPOS_CHECK_TEXT5,
	StrToWzID("ST01"),  //DLGPOS_CHECK_TEXT6

	StrToWzID("LS00"),	//DLGPOS_LIST,
};
//------------------------------------------------------------------------------
/**
*/
uiGuildDutyVest::uiGuildDutyVest(InterfaceManager * pUIMan) : uiBase(pUIMan)
{
	m_pGuildMan = (uiGuildMan *)GetUIMan()->GetUserInterfaceManager(UIMAN_GUILD);
	assert(m_pGuildMan);
	m_bShow = FALSE;

}
//------------------------------------------------------------------------------
/**
*/
uiGuildDutyVest::~uiGuildDutyVest()
{

}
//------------------------------------------------------------------------------
/**
*/
void uiGuildDutyVest::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);
	InitControl();
}

void uiGuildDutyVest::InitControl()
{
	for( int i = 0; i < NUM_CHECK_CONTROL; ++i )
	{
		m_CheckState[i].pCheck = (CCtrlButtonCheckWZ *)getControl(DLGPOS_CHECK1 +i);
		assert( m_CheckState[i].pCheck);
		m_CheckState[i].pStatic = ( CCtrlStaticWZ *) getControl(DLGPOS_CHECK_TEXT1 +i);
		assert(m_CheckState[i].pStatic);
		m_CheckState[i].eGuildDuty = eGUILD_DUTY_NONE;
	}

	m_pList = (CCtrlListWZ *) getControl(DLGPOS_LIST);
	m_pList->SetColorSelBar( RGBA(150,10,10,180) );
	m_pList->SetStartLineGapTop(6);
	assert(m_pList);
}


//------------------------------------------------------------------------------
/**
*/
void uiGuildDutyVest::Release()
{

}
//------------------------------------------------------------------------------
/**
*/
void uiGuildDutyVest::OnUpdateSolarDialog()
{
    if( ! m_bShow )
    {
        ShowInterface(FALSE);
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiGuildDutyVest::Process(DWORD dwTick)
{

}
//------------------------------------------------------------------------------
/**
*/
void uiGuildDutyVest::OnShowWindow(BOOL val)
{
	if( val )
	{
		m_bShow = TRUE;
		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType = eDoType_Guild_Duty_Vest;
		msg.wParam = InterfaceManager::DIALOG_GUILD_DUTY_VEST;
		msg.DoSomething = GlobalFunc::DoShowWindow;
		g_KeyQueueManager.PushBack(msg);

		MoveCenter();
		TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};
		Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, (TCHAR *)g_InterfaceManager.GetInterfaceString(1227), m_strTargetName.c_str());
		SetCaptionTitleName(szMessage);
		AssignCheckState();
	}
	else
	{
		m_bShow = FALSE;
		g_KeyQueueManager.DeleteMsg(eDoType_Guild_Duty_Vest);
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiGuildDutyVest::MessageProc(SI_MESSAGE * pMessage)
{
	switch ( pMessage->eResultMsg )
	{
	case RT_MSG_LBUTTONCLICK:
		{
			int iSelControlPos =  getControlIDToPos(pMessage->dwCtrlID);
			switch ( iSelControlPos )
			{
			case DLGPOS_BTN_OK:
				{
					if( NET_SEND_CG_GUILD_CHANGE_DUTY_SYN() )
						m_bShow = FALSE;
				}
				break;
			case DLGPOS_BTN_CANCLE:
			case DLGPOS_BTN_EXIT:
				{
					m_bShow = FALSE;
				}
				break;

			case DLGPOS_CHECK1:
			case DLGPOS_CHECK2:
			case DLGPOS_CHECK3:
			case DLGPOS_CHECK4:
			case DLGPOS_CHECK5:
			case DLGPOS_CHECK6:
				{
					int index = iSelControlPos - DLGPOS_CHECK1;
					SetCheckState(index);
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
void uiGuildDutyVest::NetworkProc(MSG_BASE * pMsg)
{

}

void uiGuildDutyVest::AssignCheckState()
{
	if( !g_pHero)
		return;
	if( !GetManager() )
		return;
	
	InitCheckState();

	const stGUILD_MEMBER_INFO * pMyGuildMemberInfo = GetManager()->GetGuildMemberInfo(g_pHero->GetName());
	if( pMyGuildMemberInfo )
	{
		GuildRankInfo* pGuildRankInfo =	 GuildRankInfoParser::Instance()->FindGuildRankInfo(GetManager()->GetGuildInfo().m_GuildGrade);
		if( !pGuildRankInfo)
			return;

		// 체크 리스트 항목들 등록
		eGUILD_DUTY eMyGuildDuty = pMyGuildMemberInfo->m_eGuildDuty;

		bool bInvalidValue = eMyGuildDuty < eGUILD_DUTY_MASTER || eMyGuildDuty > eGUILD_DUTY_CAMP_SOLDIER;
		if( bInvalidValue )
			return;

		int index = 0;

		eGUILD_DUTY eSelectedMemberDuty = eGUILD_DUTY_NONE;

		uiGuildMain * pGuildMain = GET_CAST_DIALOG( uiGuildMain, IM_GUILD_MANAGER::GUILD_MAIN );

		if( pGuildMain )
		{
			stGUILD_MEMBER_INFO * pSelectedMember = pGuildMain->GetSelectedPacketForGuildMember();
			if( pSelectedMember )
			{
				eSelectedMemberDuty = pSelectedMember->m_eGuildDuty;
			}
		}
		 
		for( int i = eMyGuildDuty + 1; i < eGUILD_DUTY_MAX; ++i )
		{
			if( index  >=NUM_CHECK_CONTROL)
				return;

			// 선택된 멤버와 같은 직책은 등록하지 않는다.
			if( eSelectedMemberDuty == i)
				continue;

			GUILD_DUTY_USE_INFO * pDutyUseInfo = pGuildRankInfo->Find((eGUILD_DUTY)i);
			if( pDutyUseInfo && pDutyUseInfo->m_dwStringCode  )
			{
				m_CheckState[index].pCheck->ShowWindowWZ(WZ_SHOW);
				m_CheckState[index].pStatic->ShowWindowWZ(WZ_SHOW);
				GlobalFunc::SetCaption(m_CheckState[index].pStatic,(TCHAR *) g_InterfaceManager.GetInterfaceString(pDutyUseInfo->m_dwStringCode) );
				m_CheckState[index].eGuildDuty = (eGUILD_DUTY)i;
				index++;
			}

		}

		if( index == 0 )
		{
			//7227	부여할 직책이 존재하지 않습니다.
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(7227));
			ShowInterface(FALSE);
		}

		TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};
		m_pList->DeleteItemAll();

		const GUILD_CORPS_TOTAL_INFO & rCorpsTotalInfo = GetManager()->GetCorpsTotalInfo();
		for( int i =1; i < MAX_GUILD_CORPS_NUM; ++i)
		{
			//6285	%s 군단
			if(rCorpsTotalInfo.m_CorpsInfo[i].m_bCreate)
			{
				Snprintf(szMessage, INTERFACE_STRING_LENGTH -1, g_InterfaceManager.GetInterfaceString(6285) , 
					GlobalFunc::GetCorpsName(rCorpsTotalInfo.m_CorpsInfo[i].m_byCorpsNum));

				int iIndex = m_pList->InsertItemBack();
				GlobalFunc::SetItem(m_pList, iIndex, 0, szMessage,StrToWzID("mn12"), DT_CENTER | DT_VCENTER ,
							c_InterfaceBKColor,c_InterfaceTextColor,c_InterfaceSelColor);
				m_pList->SetItemData(iIndex, (void *)&rCorpsTotalInfo.m_CorpsInfo[i].m_byCorpsNum);
			}
		}
	}
}
void uiGuildDutyVest::InitCheckState()
{
	for( int i =0; i< NUM_CHECK_CONTROL; ++i)
	{
		m_CheckState[i].eGuildDuty = eGUILD_DUTY_NONE;
		m_CheckState[i].pCheck->ShowWindowWZ(WZ_HIDE);
		m_CheckState[i].pStatic->ShowWindowWZ(WZ_HIDE);
		m_CheckState[i].pCheck->SetCheckState(false);
	}

	m_pList->ShowWindowWZ(WZ_HIDE);
	m_pList->SetCurSel(-1);
}

void uiGuildDutyVest::SetCheckState(int index )
{
	if( index >= NUM_CHECK_CONTROL)
		return;

	bool oldValue = m_CheckState[index].pCheck->GetCheckState();
	for( int i = 0; i < NUM_CHECK_CONTROL; ++i)
	{
		m_CheckState[i].pCheck->SetCheckState(false);
	}
	m_CheckState[index].pCheck->SetCheckState(oldValue);


	if( m_CheckState[index].eGuildDuty == eGUILD_DUTY_COMMANDER)
	{
		if( m_pList->GetListRowCnt() > 0 )
		{
			m_pList->ShowWindowWZ(WZ_SHOW);
		}
		else
		{
			m_CheckState[index].pCheck->SetCheckState(false);
			//7163	해당 길드에 군단이 생성되지 않았습니다.
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(7163));
			m_pList->ShowWindowWZ(WZ_HIDE);
		}
	}
	else
	{
		m_pList->ShowWindowWZ(WZ_HIDE);
	}
}

bool uiGuildDutyVest::NET_SEND_CG_GUILD_CHANGE_DUTY_SYN()
{
	for(int i =0; i < NUM_CHECK_CONTROL; ++i)
	{
		if( m_CheckState[i].pCheck->GetCheckState() )
		{
			//군단장 임명 패킷 보낸다
			if( m_CheckState[i].eGuildDuty == eGUILD_DUTY_COMMANDER)
			{
				int curListSel = m_pList->GetCurSel();
				if( curListSel >= 0)
				{
					MSG_CG_GUILD_CHANGE_COMMANDER_SYN SendMsg;
					StrnCopy(SendMsg.m_szTargetCharName, m_strTargetName.c_str(), MAX_CHARNAME_LENGTH);
					BYTE *pData = (BYTE *)m_pList->GetItemData(curListSel);
					if( pData )
					{
						SendMsg.m_byCorpsNum = *pData;
					}
					GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendMsg, sizeof(SendMsg));
				}
				else
				{
					//6256	이동하실 군단을 선택하셔야 합니다
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(6256));
					return FALSE;
				}
			}
			else
			{
				MSG_CG_GUILD_CHANGE_DUTY_SYN SendMsg;
				StrnCopy(SendMsg.m_szTargetCharName, m_strTargetName.c_str(),MAX_CHARNAME_LENGTH);
				SendMsg.m_byDuty = m_CheckState[i].eGuildDuty;
				GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendMsg, sizeof(SendMsg) );
			}
			return TRUE;
		}
	}

	//6258	임명하실 직책을 선택해야 합니다.
	GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(6258));
	return FALSE;
}

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
