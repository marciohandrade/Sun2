//------------------------------------------------------------------------------
//  uiPartyMan_main.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiPartyMan.h"
#include "ObjectManager.h"
#include "uiPartyMember/uiPartyMember.h"
#include "uiPartyLeavePopup/uiPartyLeavePopup.h"
#include "uiPartyOption/uiPartyOption.h"
#include "ItemShareSystem.h"
#include "TargetDialog.h"
#include "uiTargetMan/uiTargetMan.h"
#include "Hero.h"
#include "globalfunc.h"
#include "GameFramework.h"
#include "BattleScene.h"
#include "InterfaceManager.h"
#include "uiSystemMan/uiSystemMan.h"
#include "uiBattleZoneList/uiBattleZoneList.h"
#include "uiBattleZoneCreate/uiBattleZoneCreate.h"
#include "uiBattlezone2Man/uiBattlezone2Man.h"
DWORD	g_dwTargetKey = 0;

//------------------------------------------------------------------------------
/**
*/
void _Confirm_ForcedExpulsion(bool bYes)
{
	if(bYes && g_dwTargetKey)
	{
		TargetDialog * pTargetDlg = GET_CAST_DIALOG(TargetDialog, IM_TARGET_MANAGER::TARGET_DIALOG);
		if (g_pHero && pTargetDlg && pTargetDlg->IsVisibleWindow())
		{
#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
            uiPartyMan* party_manager = GET_DIALOG_MANAGER(uiPartyMan, UIMAN_PARTY);
            if (party_manager)
            {
                party_manager->Send_CG_PARTY_ROOM_FORCE_LEAVE_SYN();
            }
#else
            if(g_dwTargetKey == pTargetDlg->GetTargetKey())
            {
                MSG_CG_PARTY_FORCED_EXPULSION_SYN sendMsg;
                sendMsg.m_dwObjKey      = g_dwTargetKey;

                GlobalFunc::SendPacket( CI_GAMESERVERIDX, &sendMsg, sizeof (MSG_CG_PARTY_FORCED_EXPULSION_SYN) );
            }
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
		}
	}

	g_dwTargetKey = 0;
}


//------------------------------------------------------------------------------
/**
*/
uiPartyMan::uiPartyMan(InterfaceManager *pUIMan) :
    uiBaseMan(pUIMan)
{
    m_bImTheLeader = true;
    m_bShowBuffInfos = true;

    for (int i=0; i<c_sMAX_PARTY_MEMBER_NUM; ++i)
    {
        m_pUIPartyMembers[i] = NULL;
    }

    m_pUIPartyOption = NULL;
	m_PartyKey = NULL;
	m_MasterKey = NULL;
    m_pUIPartyLeavePopup = NULL;
	m_bIsOpenPartyJoinWinodw = FALSE;
	m_dwPendingPartyKey = 0;
	m_dequePartyReturn.clear();
	m_pPartyNaviInfo = NULL;


	m_bIsShowFace = true;
	m_bIsShowGague = true;
	m_bIsShowBuf = true;

#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
    is_opened_return_party_window_ = false;
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
}

void
uiPartyMan::OnInitialize()
{
    m_pPartyNaviInfo = new PartyNaviInfo;
    assert(m_pPartyNaviInfo);

#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
    is_disconnected_uesr_kick_ = false;
    ZeroMemory(kick_player_name_, sizeof(kick_player_name_));
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS

    _LoadUI();
}

//------------------------------------------------------------------------------
/**
*/
void
uiPartyMan::OnRelease()
{
	if (m_pPartyNaviInfo)
    {
		delete m_pPartyNaviInfo;
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiPartyMan::uiClear(short idx)
{
    assert (m_pUIPartyMembers[idx]);
    if (m_pUIPartyMembers[idx])
    {
        m_pUIPartyMembers[idx]->ResetHPGauge();
		m_pUIPartyMembers[idx]->ResetMPGauge();

		m_pUIPartyMembers[idx]->ShowInterface(FALSE);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiPartyMan::Process(DWORD dwTick)
{
	if (m_pPartyNaviInfo)
	{
		m_pPartyNaviInfo->Process(dwTick);
	}	
}

//------------------------------------------------------------------------------
/**
*/
void
uiPartyMan::uiClearAll()
{
    for (int i=0; i<MAX_PARTYMEMBER_NUM - 1; ++i)
    {
        this->uiClear(i);
    }

    this->m_pUIPartyOption->ShowInterface(FALSE);
}

//------------------------------------------------------------------------------
/**
*/
void
uiPartyMan::uiShowAll()
{
    int realSize;
	if (this->GetPartyNum())
    {
        realSize = this->GetPartyNum() - 1;
    }
    else
    {
        realSize = 0;
    }

    this->uiClearAll();


    for (int i=0; i<realSize; ++i)
    {
		assert (m_pUIPartyMembers[i]);
        if (m_pUIPartyMembers[i])
		{
            m_pUIPartyMembers[i]->ShowInterface(TRUE);
			m_pUIPartyMembers[i]->SetPartView();
#ifdef _DH_BATTLEZONE2_
            m_pUIPartyMembers[i]->CheckOutputZoneType();
#endif //_DH_BATTLEZONE2_
		}

    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiPartyMan::uiSetPartView()
{
    int realSize;
	if (this->GetPartyNum())
    {
        realSize = this->GetPartyNum() - 1;
    }
    else
    {
        realSize = 0;
    }

    for (int i=0; i<realSize; ++i)
    {
		assert (m_pUIPartyMembers[i]);
        if (m_pUIPartyMembers[i])
            m_pUIPartyMembers[i]->SetPartView();
    }
}

void 					
uiPartyMan::SetShowGague(bool val)
{ 
    m_bIsShowGague = val;
    int realSize;
	if (this->GetPartyNum())
    {
        realSize = this->GetPartyNum() - 1;
    }
    else
    {
        realSize = 0;
    }

    for (int i=0; i<realSize; ++i)
    {
		assert (m_pUIPartyMembers[i]);
        if (m_pUIPartyMembers[i])
		{
            m_pUIPartyMembers[i]->ShowGagueControl(val);
		}
    }
}


void
uiPartyMan::SetShowFace(bool val)
{ 
    m_bIsShowFace = val;
    int realSize;
	if (this->GetPartyNum())
    {
        realSize = this->GetPartyNum() - 1;
    }
    else
    {
        realSize = 0;
    }

    for (int i=0; i<realSize; ++i)
    {
		assert (m_pUIPartyMembers[i]);
        if (m_pUIPartyMembers[i])
		{
            m_pUIPartyMembers[i]->ShowFaceControl(val);
		}
    }
}
//------------------------------------------------------------------------------
/**
*/
bool
uiPartyMan::Comp_HeroKey(DWORD dwKey)
{
    if (!g_pHero)
        return false;

    return (g_pHero->GetObjectKey() == dwKey);
}

//------------------------------------------------------------------------------
/**
    sort utility
*/
bool _sort_class_high_to_low( uicPartyMemberInfoPlus memInfo1, uicPartyMemberInfoPlus memInfo2 )
{
    return (memInfo1.Get().m_byClassType > memInfo2.Get().m_byClassType);
}

bool _sort_class_low_to_high( uicPartyMemberInfoPlus memInfo1, uicPartyMemberInfoPlus memInfo2 )
{
    return (memInfo1.Get().m_byClassType < memInfo2.Get().m_byClassType);
}

bool _sort_level_high_to_low( uicPartyMemberInfoPlus memInfo1, uicPartyMemberInfoPlus memInfo2 )
{
    return (memInfo1.Get().m_wLevel > memInfo2.Get().m_wLevel);
}

bool _sort_level_low_to_high( uicPartyMemberInfoPlus memInfo1, uicPartyMemberInfoPlus memInfo2 )
{
    return (memInfo1.Get().m_wLevel < memInfo2.Get().m_wLevel);
}
//------------------------------------------------------------------------------
/**
*/
void
uiPartyMan::uiPartySort(int sortType/* =eUIPARTY_SORT_NORMAL */)
{
	if( m_vParty.empty() )
		return;

    STLX_VECTOR<uicPartyMemberInfoPlus> sortPartys;
    uicPartyMemberInfoPlus myParty = this->m_vParty[0];


    for (size_t i=1; i<this->m_vParty.size(); ++i)
    {
        sortPartys.push_back(this->m_vParty[i]);
    }

	switch (sortType)
    {
    default:
    case eUIPARTY_SORT_NORMAL:
        {
        }
        break;

    case eUIPARTY_SORT_CLASS_HIGH_TO_LOW:
        {
            sort( sortPartys.begin(), sortPartys.end(), _sort_class_high_to_low );
        }
        break;

    case eUIPARTY_SORT_CLASS_LOW_TO_HIGH:
        {
            sort( sortPartys.begin(), sortPartys.end(), _sort_class_low_to_high );
        }
        break;

    case eUIPARTY_SORT_LEVEL_HIGH_TO_LOW:
        {
            sort( sortPartys.begin(), sortPartys.end(), _sort_level_high_to_low );
        }
        break;

    case eUIPARTY_SORT_LEVEL_LOW_TO_HIGH:
        {
            sort( sortPartys.begin(), sortPartys.end(), _sort_level_low_to_high );
        }
        break;
    }

    m_vParty.clear();
    m_vParty.push_back(myParty);

    for (int i=0; i<sortPartys.size(); ++i)
    {
        m_vParty.push_back(sortPartys[i]);
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiPartyMan::PutMsg_PlayerEntered(uicPartyMemberInfoPlus memInfo)
{
	TCHAR	szText[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};

	if (false == this->Comp_HeroKey(memInfo.Get().m_dwMemberKey))
    {
        if ((GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_CHUNTING)
         || (GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_MISSION))
        {
            
            //	%s 님이 입장하셨습니다.
            this->GetUIMan()->GetInterfaceString(eST_PARTY_MEMBER_ENTER_NOTICE_1, szText, INTERFACE_STRING_LENGTH);
			Snprintf (szMessage,INTERFACE_STRING_LENGTH-1, szText, memInfo.Get().m_szCharName);
            
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);

            //	몬스터의 HP와 얻을 수 있는 경험치가 증가합니다.
            this->GetUIMan()->GetInterfaceString(eST_PARTY_MEMBER_ENTER_NOTICE_2, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void uiPartyMan::ForcedExpulsion()		// 파티 추방
{ 
	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};

	TargetDialog* pTargetDlg = GET_CAST_DIALOG(TargetDialog, IM_TARGET_MANAGER::TARGET_DIALOG);

    if (g_pHero && pTargetDlg && pTargetDlg->IsVisibleWindow())
    {
        if ((g_pHero->GetObjectKey() != pTargetDlg->GetTargetKey()))
        {
			TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};
			TCHAR szTemp[INTERFACE_STRING_LENGTH] ={0,};

			g_dwTargetKey = pTargetDlg->GetTargetKey();
			// %s님을 파티에서 탈퇴시키겠습니까?
			g_InterfaceManager.GetInterfaceString(5215, szTemp, INTERFACE_STRING_LENGTH);
			uiSystemMan* pSystemMan  = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);

			if (pSystemMan)
			{
				uiPartyMan* partyMan = GET_DIALOG_MANAGER(uiPartyMan, UIMAN_PARTY);

				if (partyMan->GetPartyMemberInfo(g_dwTargetKey))
				{
					_PARTY_MEMBER_INFO rMember = (partyMan->GetPartyMemberInfo(g_dwTargetKey))->Get();
					Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp, rMember.m_szCharName);

#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
                    if (rMember.m_isConnected == true)
                    {
                        ZeroMemory(kick_player_name_, sizeof(kick_player_name_));
                        is_disconnected_uesr_kick_ = false;
                    }
                    else
                    {
                        StrnCopy(kick_player_name_,
                            rMember.m_szCharName,
                            MAX_CHARNAME_LENGTH);
                        is_disconnected_uesr_kick_ = true;
                    }
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS

					pSystemMan->Sys_Confirm(szMessage, _Confirm_ForcedExpulsion);
				}

			}

        }
        else
        {
			//	강제 탈퇴는 타인에게만 가능합니다. 탈퇴를 하세요. -0-
			g_InterfaceManager.GetInterfaceString(eST_PARTY_FORCED_EXPULSION_HELP, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
        }
    }
    else
    {
		//	강제 퇴장은 타겟을 설정하신 후 사용하십시오
		g_InterfaceManager.GetInterfaceString(eST_PARTY_TARGET_FORCED_EXPULSION_HELP, szMessage, INTERFACE_STRING_LENGTH);
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
    }
}

//------------------------------------------------------------------------------
/**
*/
void uiPartyMan::ChangeMaster()		
{ 
	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};

	TargetDialog * pTargetDlg = GET_CAST_DIALOG(TargetDialog, IM_TARGET_MANAGER::TARGET_DIALOG);

    if (pTargetDlg && pTargetDlg->IsVisibleWindow())
    {
        MSG_CG_PARTY_CHANGE_MASTER_SYN sendMsg;
        sendMsg.m_dwObjKey      = pTargetDlg->GetTargetKey();
        GlobalFunc::SendPacket( CI_GAMESERVERIDX, &sendMsg, sizeof (MSG_CG_PARTY_CHANGE_MASTER_SYN) );
    }
    else
    {
		//	파티장 위임은 타겟을 설정하신 후 사용하십시오
		g_InterfaceManager.GetInterfaceString( eST_PARTY_CHANGE_MASTER_TARGET, szMessage, INTERFACE_STRING_LENGTH );
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
    }	
}

//------------------------------------------------------------------------------
/**
*/
void
uiPartyMan::LeaveParty()
{
    MSG_CG_PARTY_LEAVE_SYN sendMsg;
    GlobalFunc::SendPacket( CI_GAMESERVERIDX, &sendMsg, sizeof (sendMsg) );
}

//------------------------------------------------------------------------------
/**
*/

void uiPartyMan::AskJoinParty( const char * szName )
{
	//assert(dwObjKey);

#ifdef _KIKI_MODIFY_PARTY_MEMBER_NUM
	if ( GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_FIELD )
	{
		// 파티원 제한: 5명
		//
		if(GetPartyNum() >= MAX_FIELD_PARTYMEMBER_NUM)
		{
			//< 파티원 수가 최대치라서 더이상 파티원을 받을 수 없다.
			TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
			g_InterfaceManager.GetInterfaceString(5330,szMessage,INTERFACE_STRING_LENGTH);
			GlobalFunc::OutputMsg_Warning(eOUTPUT_MSG_CHAT, szMessage);
			return;
		}
	}
	else
	{
		// 파티원 제한: 10명
		//
		if(GetPartyNum() >= MAX_PARTYMEMBER_NUM)
		{
			//< 파티원 수가 최대치라서 더이상 파티원을 받을 수 없다.
			TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
			g_InterfaceManager.GetInterfaceString(5330,szMessage,INTERFACE_STRING_LENGTH);
			GlobalFunc::OutputMsg_Warning(eOUTPUT_MSG_CHAT, szMessage);
			return;
		}
	}
#endif

    MSG_CG_PARTY_JOIN_SYN   sync;
	_tcsncpy(sync.m_TargetCharName, szName, MAX_CHARNAME_LENGTH);
    GlobalFunc::SendPacketToGameServer(&sync, sizeof (sync));
}

//------------------------------------------------------------------------------
/**
*/

void uiPartyMan::UpdatePartyItemShareSystem()
{
	SCENE_TYPE eCurScene = GameFramework::GetCurrentScene()->GetType();

	BOOL bApply = FALSE;
	bool create_battle_zone = false;

	if( eCurScene == SCENE_TYPE_FIELD)
		bApply =TRUE;

    uiBattleZoneList* battlezone_list_dialog=(uiBattleZoneList*)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_BATTLEZONE_LIST);
    if (battlezone_list_dialog)
    {
        if(battlezone_list_dialog->IsVisible())
        {
            create_battle_zone = true;
        }
    }
    uiBattleZoneCreate* battlezon_create_dialog = (uiBattleZoneCreate*)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_BATTLEZONE_CREATE);
    if (battlezon_create_dialog)
    {
        if(battlezon_create_dialog->IsVisible())
        {
            create_battle_zone = true;
        }
    }

    if (create_battle_zone)
    {
        g_InterfaceManager.DeAttachFirstDialog(IM_PARTY_MANAGER::PARTY_ITEM_SHARE_SYSTEM);
        return;
    }

	if( IsParty() && GetPartyNum() > 1 )
	{
		if (m_pUIItemShareSystem->IsVisibleWindow() == FALSE)
        {
			m_pUIItemShareSystem->ShowInterface(TRUE);
        }
        {
            m_pUIItemShareSystem->ShowDivideItemControls(WZ_SHOW);//필드면 보인다.
        }
	}
	else
	{
		if (m_pUIItemShareSystem->IsVisibleWindow() == TRUE)
        {
			m_pUIItemShareSystem->ShowInterface(FALSE);
        }
	}
	m_pUIItemShareSystem->SetPartyLeader(IsLeader());
}

void uiPartyMan::SetTargetingMember(int party_member_index)
{
    if (party_member_index >= (GetPartyNum() - 1)) // 자신이 포함되어 있어 1을 뺀다
    {
        return;
    }
    uiPartyMember* party_member = m_pUIPartyMembers[party_member_index];
    if (party_member != NULL)
    {
        party_member->SetTargeting();
    }
}

void uiPartyMan::NET_SEND_CG_PARTY_RETURN_C2G_LEADER_QUERY_ACK(byte byAnswer)
{
	if( m_dequePartyReturn.size() <= 0)
		return;

	// 큐의 첫번째 놈을 처리한다.
	stPendingUnexpectedStatus stUnexpectedPlyaer = *(m_dequePartyReturn.begin());
	MSG_CG_PARTY_RETURN_C2G_LEADER_QUERY_ACK SendMsg;
	SendMsg.m_dwPlayerKey =   stUnexpectedPlyaer.m_dwReturnPlayerKey;// 서버에서 받은값 다시 돌려준다.
	SendMsg.m_byAnswer = byAnswer;
	GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendMsg, sizeof(SendMsg) );

	
	//처리 됐으니깐 비정상 종료 파티원 삭제한다.
	DeleteUnExpectedPartyMember(stUnexpectedPlyaer.m_szReturnPlayerName);
#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
    uiBattlezone2Man* battlezone2_manager =
        GET_DIALOG_MANAGER(uiBattlezone2Man, UIMAN_BATTLE_ZONE_MANAGER);
    if (battlezone2_manager)
    {
        battlezone2_manager->DeleteDisConnectedUser(stUnexpectedPlyaer.m_szReturnPlayerName);
    }
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS

	// 처리완료된 놈은 큐에서 뺀다
	m_dequePartyReturn.pop_front();

	// 아직도 큐에 복귀할 놈이 남아 있으면 다음 놈을 처리한다.
	if( m_dequePartyReturn.size() > 0)
	{
		TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};
		stUnexpectedPlyaer = *(m_dequePartyReturn.begin());
		//5237 <%s>님이 재접속 하였습니다. 파티로 복귀시키겠습니까?
		Snprintf(szMessage, INTERFACE_STRING_LENGTH -1, g_InterfaceManager.GetInterfaceString( 5237 ), 
			stUnexpectedPlyaer.m_szReturnPlayerName );
		uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
		if( pSystemMan )
		{
			pSystemMan->Sys_Confirm(szMessage, _callBack_Recall_PartyMember);
		}
	}
}

void uiPartyMan::NET_SEND_CG_PARTY_RETURN_C2G_MEMBER_QUERY_ACK(bool val)
{
	MSG_CG_PARTY_RETURN_C2G_MEMBER_QUERY_ACK SendMsg;
	SendMsg.m_wPartyKey = static_cast<WORD>(m_dwPendingPartyKey);	//// 서버에서 받은값 다시 돌려준다.

	if (val)
	{
		SendMsg.m_byAnswer = MSG_CG_PARTY_RETURN_C2G_MEMBER_QUERY_ACK::ACCEPT_JOIN;
	}
	else
	{
		SendMsg.m_byAnswer = MSG_CG_PARTY_RETURN_C2G_MEMBER_QUERY_ACK::REJECT_JOIN;
	}

	GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendMsg, sizeof(SendMsg));
}

void uiPartyMan::DeleteUnExpectedPartyMember(TCHAR * pszCharName)
{
	for ( size_t i = 0; i< m_vParty.size(); ++i)
	{
		uicPartyMemberInfoPlus & rMemberInfo = m_vParty[i];
#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
        if (rMemberInfo.IsConnected() == false)
#else
		if( rMemberInfo.IsUnexpectedTerminate())
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
		{
			if( StrnCmp(rMemberInfo.Get().m_szCharName , pszCharName , MAX_CHARNAME_LENGTH) == 0 )
			{
				m_vParty.erase(m_vParty.begin() + i);
				break;
			}
		}
	}
	this->uiPartySort();
	this->uiShowAll();
}

void uiPartyMan::UpdateStatusIcon()
{
    uiPartyMember* ui_party_member = NULL;
    for (int party_member_index = 0; party_member_index < c_sMAX_PARTY_MEMBER_NUM; ++party_member_index)
    {
        ui_party_member = m_pUIPartyMembers[party_member_index];
        if (ui_party_member != NULL &&
            ui_party_member->IsVisible() == TRUE)
        {
            ui_party_member->UpdateStatusIcon();
        }
    }
}

#ifdef _DH_BATTLEZONE2_
void uiPartyMan::SetBattleZoneReady(int object_key, bool is_ready)
{
    uicPartyMemberInfoPlus* member_info = GetPartyMemberInfo(object_key);
    if (member_info != NULL)
    {
        member_info->set_is_ready(is_ready);
    }
}
void uiPartyMan::AllCancleBattleZoneReady()
{
    for (size_t i = 0; i< m_vParty.size(); ++i)
    {
        m_vParty[i].set_is_ready(false);
    }
}
void uiPartyMan::CheckOutputZoneType()
{
    int real_size;

    if (this->GetPartyNum())
    {
        real_size = this->GetPartyNum() - 1;
    }
    else
    {
        real_size = 0;
    }
    for (int i = 0; i < real_size; ++i)
    {
        m_pUIPartyMembers[i]->CheckOutputZoneType();
    }
}
#endif //_DH_BATTLEZONE2_

void uiPartyMan::Send_CG_PARTY_ROOM_FORCE_LEAVE_SYN()
{
#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
    if (is_disconnected_uesr_kick_ == false)
    {
        //정상 유저 강퇴
        TargetDialog* target_dialog = GET_CAST_DIALOG(TargetDialog, IM_TARGET_MANAGER::TARGET_DIALOG);
        if(g_dwTargetKey == target_dialog->GetTargetKey())
        {
            MSG_CG_PARTY_FORCED_EXPULSION_SYN send_message;
            send_message.m_dwObjKey = g_dwTargetKey;

            GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_message, sizeof (MSG_CG_PARTY_FORCED_EXPULSION_SYN));
        }
    }
    else
    {
        // 비정상 종료자 강퇴 요청
        MSG_CG_PARTY_RETURN_G2C_MEMBER_REMOVE_SYN send_packet;
        send_packet.request_party_key_ = GetPartyKey();
        StrnCopy(send_packet.leave_member_name_,
            kick_player_name_,
            MAX_CHARNAME_LENGTH);

        GlobalFunc::SendPacket(CI_GAMESERVERIDX,
            &send_packet,
            sizeof(send_packet));
    }
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
}
//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
