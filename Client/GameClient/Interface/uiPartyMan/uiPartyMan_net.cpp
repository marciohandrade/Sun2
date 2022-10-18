//------------------------------------------------------------------------------
//  uiPartyMan_net.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiPartyMan.h"

#include "ResultCode.h"
using namespace RC;

#include "BattleScene.h"
#include "Hero.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "uiSystemMan/uiSystemMan.h"
#include "ObjectManager.h"

#include "uiMissionRankMan/uiMissionRankMan.h"
#include "GameFramework.h"
#include "ItemShareSystem.h"

#include "Itemmanager.h"

#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
  #include "DiceCastManager.h"
  #include "Item.h"
  #include "ItemInfoParser.h"
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING

#ifdef _DH_BATTLEZONE2_
#include "uiBattlezone2Man/uiBattlezone2Man.h"
#include "uiBattlezone2Lobby/uiBattlezone2Lobby.h"
#endif//_DH_BATTLEZONE2_

#pragma warning ( push )
#pragma warning(disable : 4800)

//------------------------------------------------------------------------------
/**
*/
void uiPartyMan::InitInfo(MSG_CG_PARTY_CREATE_CMD *pMsg)
{
	assert (pMsg);
	if (!pMsg)
    {
		return;
    }

	this->ClearParty();

	// 아이템분배 정보 처리 필요
	m_pUIItemShareSystem->SetToggleName(pMsg->m_DistributionType);

	for (int i=0; i < pMsg->m_MemberTotalInfo.m_Count; ++i)
	{
		uicPartyMemberInfoPlus memInfoPlus;
		memInfoPlus = pMsg->m_MemberTotalInfo.m_Slot[i];

#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
        // 비정상종료 + 자신의 아이디면 추가안함
        // (비정상종료 작업하면서 맴버에 비정상종료된 자신 정보를 받을수가 있다)
        if (g_pHero &&
            (StrnCmp(g_pHero->GetName(), memInfoPlus.Get().m_szCharName,
            MAX_CHARNAME_LENGTH) == 0))
        {
            if (memInfoPlus.IsConnected() == false)
            {
                continue;
            }
        }
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
		
		Insert_MemberInfo(memInfoPlus);

#ifdef _DH_BATTLEZONE2_
        uiBattlezone2Lobby* battlezone2_lobby = GET_CAST_DIALOG(uiBattlezone2Lobby, IM_BATTLEZONE2_MANAGER::BATTLEZONE2_LOBBY);
        if (battlezone2_lobby)
        {
            const _PARTY_MEMBER_INFO& member_info = memInfoPlus.Get();
            battlezone2_lobby->SetTeam(member_info.m_dwMemberKey, memInfoPlus.GetTeam());
        }
#endif//_DH_BATTLEZONE2_
	}
	m_PartyKey	= pMsg->m_PartyKey;
	m_MasterKey = pMsg->m_MasterKey;

	
	if (Comp_HeroKey(m_MasterKey))
	{
		m_bImTheLeader = true;
	}
	else
	{
		m_bImTheLeader = false;
	}

	this->uiPartySort(eUIPARTY_SORT_LEVEL_HIGH_TO_LOW);
	this->uiShowAll();
}


//------------------------------------------------------------------------------
/**
*/
void uiPartyMan::RefreshPartyHideInfo(BOOL bClear)
{
    //파티가 존재했을경우
    for (int i=0; i<this->GetPartyNum(); ++i)
    {
        ChangePartyHideInfo(m_vParty[i].Get().m_dwMemberKey,bClear);
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiPartyMan::ChangePartyHideInfo(DWORD dwObjKey,BOOL bClear)
{
    Object *pObj = g_ObjectManager.GetObject(dwObjKey);

    if(pObj && pObj->IsKindOfObject(PLAYER_OBJECT))
    {   
        Player *pPlayer = static_cast<Player*>(pObj);
        pPlayer->RefreshHideState(bClear);
    }
}
//------------------------------------------------------------------------------
/**
*/
void
uiPartyMan::AddInfo(MSG_CG_PARTY_JOIN_BRD* pMsg)
{
    assert (pMsg);
    if (!pMsg)
        return;

	uicPartyMemberInfoPlus memInfoPlus;
	memInfoPlus = pMsg->m_NewMemberInfo;
	Insert_MemberInfo(memInfoPlus);

    this->uiPartySort();
    this->uiShowAll();

#ifdef _DH_BATTLEZONE2_
    uiBattlezone2Lobby* battlezone2_lobby = GET_CAST_DIALOG(uiBattlezone2Lobby, IM_BATTLEZONE2_MANAGER::BATTLEZONE2_LOBBY);
    if (battlezone2_lobby)
    {
        const _PARTY_MEMBER_INFO& member_info = memInfoPlus.Get();
        battlezone2_lobby->SetTeam(member_info.m_dwMemberKey, memInfoPlus.GetTeam());
    }
#endif//_DH_BATTLEZONE2_
}

//------------------------------------------------------------------------------
/**
*/
void
uiPartyMan::Migration(DWORD dwObjKey)
{
#ifdef _DH_BATTLEZONE2_
    //파티장 변경 시 전 파티장- 레디 해제, 새로운 파티장 - 레디 해제
    uiBattlezone2Man* battlezone2_manager = GET_DIALOG_MANAGER(uiBattlezone2Man, UIMAN_BATTLE_ZONE_MANAGER);
    if (battlezone2_manager && battlezone2_manager->battlezone_room_state() != uiBattlezone2Man::ROOM_STATE_LIST)
    {
        SetBattleZoneReady(m_MasterKey, false);
        SetBattleZoneReady(dwObjKey, false);

        uiBattlezone2Lobby* battlezone2_lobby = GET_CAST_DIALOG(uiBattlezone2Lobby, IM_BATTLEZONE2_MANAGER::BATTLEZONE2_LOBBY);
        if (battlezone2_lobby)
        {
            battlezone2_lobby->ChangeMaster(dwObjKey);
        }
    }
#endif //_DH_BATTLEZONE2_

    this->m_bImTheLeader = Comp_HeroKey(dwObjKey);

	m_MasterKey = dwObjKey;

	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};
    g_InterfaceManager.GetInterfaceString(eST_PARTY_MIGRATION, szMessage, INTERFACE_STRING_LENGTH);
    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);

}
//------------------------------------------------------------------------------
/**
*/
void uiPartyMan::DestroyParty()
{
    RefreshPartyHideInfo(TRUE);

    this->ClearParty();

	m_PartyKey = NULL;
	m_MasterKey = NULL;

    if ((GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_CHUNTING)
     || (GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_MISSION)
     || (GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_PVP)
	 || (GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_INSTANCE_DUNGEON))
    {
        BattleScene::BackToTheVillageSyn();
    }

	m_dequePartyReturn.clear();
	m_pPartyNaviInfo->Clear();
    ItemManager::Instance()->ClearOwnerList();
    ItemManager::Instance()->UpdateMyItem();
}

//------------------------------------------------------------------------------
/**
*/
void uiPartyMan::Insert_MemberInfo(uicPartyMemberInfoPlus memInfo)
{
	DeleteUnExpectedPartyMember((TCHAR *)memInfo.Get().m_szCharName);

	assert (this->m_vParty.size() < MAX_PARTYMEMBER_NUM);
	if (this->m_vParty.size() < MAX_PARTYMEMBER_NUM)
	{
		PutMsg_PlayerEntered(memInfo);

		DWORD compKey = memInfo.Get().m_dwMemberKey;
		if (Comp_HeroKey(compKey))
		{
			std::deque<uicPartyMemberInfoPlus> dqMemInfos;
			size_t i;

			for (i=0; i<this->m_vParty.size(); ++i)
			{
				dqMemInfos.push_back(this->m_vParty[i]);
			}

			dqMemInfos.push_front(memInfo);

			this->ClearParty();

			for (i=0; i<dqMemInfos.size(); ++i)
			{
				this->m_vParty.push_back(dqMemInfos[i]);
			}
		}
		else
		{
			this->m_vParty.push_back(memInfo);
		}
	}
}

//-------------------------------------------------------------------------------------------
/**
*/
void 
uiPartyMan::Remove_MemberInfo(DWORD dwObjKey, BYTE byReason)
{
	NoticeReasonLeaveParty(dwObjKey, byReason);

	for (int i=0; i<this->GetPartyNum(); ++i)
	{
		if (this->m_vParty[i].CompKeySimple(dwObjKey))
		{
			// 지우는 멤버가 나라면
			if (Comp_HeroKey(dwObjKey))
			{
				this->DestroyParty();
				return;
			}            

			bool bZoneTypeAccept = (
#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
#else
									GameFramework::GetCurrentScene()->GetType() != SCENE_TYPE_FIELD &&
									GameFramework::GetCurrentScene()->GetType() != SCENE_TYPE_VILLAGE &&
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
									GameFramework::GetCurrentScene()->GetType() != SCENE_TYPE_PVP &&
                                    GameFramework::GetCurrentScene()->GetType() != SCENE_TYPE_DOMINATION
                                    );

			if ((byReason == eREASON_LEAVE_PARTY_ABNORMAL) && bZoneTypeAccept) // 비정상 종료자는 따로 모아서 관리한다.
			{
#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
                //아직 오브젝트 키 값이 유효하므로 여기서 레디를 꺼주자.
                uiBattlezone2Man* battlezone2_manager =
                    GET_DIALOG_MANAGER(uiBattlezone2Man, UIMAN_BATTLE_ZONE_MANAGER);
                if (battlezone2_manager)
                {
                    battlezone2_manager->SetReadyUserSlot(dwObjKey, false);
                }
                m_vParty[i].SetIsConnected(false);
#else
                m_vParty[i].SetUnexpectedTerminate(true);
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
			}
			else
			{
#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
                m_vParty[i].SetIsConnected(true);
#else
				m_vParty[i].SetUnexpectedTerminate(false);
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
				this->m_vParty.erase(this->m_vParty.begin() + i);
			}
			if (this->m_vParty.empty())
            {
				break;
            }

			m_pPartyNaviInfo->RemoveNaviInfo(dwObjKey);

		}
	}

	this->uiPartySort();
	this->uiShowAll();
    ItemManager::Instance()->ClearOwnerList();
    ItemManager::Instance()->UpdateMyItem();
}
//------------------------------------------------------------------------------
/**
*/
void
uiPartyMan::Reason(BYTE byErrorCode)
{
    TCHAR   szMessage[INTERFACE_STRING_LENGTH];

    switch (byErrorCode)
    {
    default:
		g_InterfaceManager.GetInterfaceString(eST_UNKNOWN_PARTY_ERROR_CODE,szMessage,INTERFACE_STRING_LENGTH);
        break;

	case RC_PARTY_NEXTMASTER_NOTEXIST:
		{	
			//< 5200	파티원에게만 파티장을 위임할수있습니다.
			g_InterfaceManager.GetInterfaceString(5200,szMessage,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_PARTY_COOLTIME_ERROR:
		{  
		    //파티장,파티원 소환 실패	
			//< 쿨타임이 끝난 다음 사용하세요
			g_InterfaceManager.GetInterfaceString(eST_SKILL_FAIL_COOLTIME_NOT_EXPIRED,szMessage,INTERFACE_STRING_LENGTH);

		}
		break;

    case RC_PARTY_PLAYER_NOTEXIST:
        {
            //< 그런 플레이어가 존재하지 않는다.
			g_InterfaceManager.GetInterfaceString(eST_PARTY_PLAYER_NOTEXIST,szMessage,INTERFACE_STRING_LENGTH);
        }
        break;

    case RC_PARTY_ISNOT_MASTEROFPARTY:
        {
            //< 파티장이 아니다.
			g_InterfaceManager.GetInterfaceString(eST_PARTY_ISNOT_MASTEROFPARTY,szMessage,INTERFACE_STRING_LENGTH);
        }
        break;

    case RC_PARTY_ISNOT_MEMBEROFPARTY:
        {
            //< 파티원이 아니다.
			g_InterfaceManager.GetInterfaceString(eST_PARTY_ISNOT_MEMBEROFPARTY,szMessage,INTERFACE_STRING_LENGTH);
        }
        break;

    case RC_PARTY_PARTY_NOTEXIST:
        {
            //< 파티가 존재하지 않는다.
			g_InterfaceManager.GetInterfaceString(eST_PARTY_PARTY_NOTEXIST,szMessage,INTERFACE_STRING_LENGTH);
        }
        break;

    case RC_PARTY_REJECT_INVITEMENT:
        {
            //< 상대방이 파티초대 요청을 거절하다.
			g_InterfaceManager.GetInterfaceString(eST_PARTY_REJECT_INVITEMENT,szMessage,INTERFACE_STRING_LENGTH);
        }
        break;

    case RC_PARTY_ISNOT_QUALIFIED_LEVEL:
        {
            //< 레벨조건이 맞지 않는다.

			g_InterfaceManager.GetInterfaceString(eST_PARTY_ISNOT_QUALIFIED_LEVEL,szMessage,INTERFACE_STRING_LENGTH);
#ifdef _DH_BATTLEZONE2_
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
#endif//_DH_BATTLEZONE2_
        }
        break;

    case RC_PARTY_INVALID_ROOMTYPE:
        {
            //< 잘못된 방 상태이다.
			g_InterfaceManager.GetInterfaceString(eST_PARTY_INVALID_ROOMTYPE,szMessage,INTERFACE_STRING_LENGTH);
#ifdef _DH_BATTLEZONE2_
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
#endif//_DH_BATTLEZONE2_
        }
        break;

    case RC_PARTY_CANNOT_CHANGE_MASTER:
        {
            //< 파티장을 변경할 수 없다.
			g_InterfaceManager.GetInterfaceString(eST_PARTY_CANNOT_CHANGE_MASTER,szMessage,INTERFACE_STRING_LENGTH);
        }
        break;

    case RC_PARTY_ALREADY_IS_MEMBEROFPARTY:
        {
            //< 이미 파티에 소속되어 있다.
			g_InterfaceManager.GetInterfaceString(eST_PARTY_ALREADY_IS_MEMBEROFPARTY,szMessage,INTERFACE_STRING_LENGTH);
#ifdef _DH_BATTLEZONE2_
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
#endif//_DH_BATTLEZONE2_
        }
        break;

    case RC_PARTY_WASNOT_REQUEST_PARTY:
        {
            //< 상대방이 파티를 요청하지 않았다.
			g_InterfaceManager.GetInterfaceString(eST_PARTY_WASNOT_REQUEST_PARTY,szMessage,INTERFACE_STRING_LENGTH);
        }
        break;

    case RC_PARTY_INVALID_DATATYPE:
        {
            //< 데이타 타입이 맞지 않는다.( 유효하지 않은 값이다. )
			g_InterfaceManager.GetInterfaceString(eST_PARTY_INVALID_DATATYPE,szMessage,INTERFACE_STRING_LENGTH);
        }
        break;

    case RC_PARTY_PLAYER_NOTEXIST_TO_FIELD:
        {
            //< 플레이어가 필드에 존재하지 않는다.
			g_InterfaceManager.GetInterfaceString(eST_PARTY_PLAYER_NOTEXIST_TO_FIELD,szMessage,INTERFACE_STRING_LENGTH);
        }
        break;

    case RC_PARTY_CANNOT_FIND_TARGET:
        {
            //< 타겟을 찾을 수 없다.
			g_InterfaceManager.GetInterfaceString(eST_PARTY_CANNOT_FIND_TARGET,szMessage,INTERFACE_STRING_LENGTH);
        }
        break;

    case RC_PARTY_TARGET_ISNOT_ENEMY:
        {
            //< 타겟이 아군이다.
			g_InterfaceManager.GetInterfaceString(eST_PARTY_TARGET_ISNOT_ENEMY,szMessage,INTERFACE_STRING_LENGTH);
        }
        break;

    case RC_PARTY_PLAYER_OBSERVER_MODE:
        {
            //< 플레이어가 투명모드라서 파티에 소속되지 않는다.
			g_InterfaceManager.GetInterfaceString(PARTY_PLAYER_OBSERVER_MODE,szMessage,INTERFACE_STRING_LENGTH);
        }
        break;

	case RC_PARTY_PARTYMEMBER_NUM_FULL:
		{
			//< 파티원 수가 최대치라서 더이상 파티원을 받을 수 없다.
			g_InterfaceManager.GetInterfaceString(5330,szMessage,INTERFACE_STRING_LENGTH);
			
		}
		break;


	case RC_PARTY_CHANGE_ITEM_DISTRIBUTION_FAIL:
		{
			// 아이템 분배 타입 변경에 실패하였습니다 5233
			Snprintf(szMessage, INTERFACE_STRING_LENGTH -1 , g_InterfaceManager.GetInterfaceString( 5233 ) );
		}
		break;

	case RC_PARTY_CANNOT_INVITE_HOSTILITY:
		{
			//7231	적대 관계인 길드의 길드원과는 파티를 맺을 수 없습니다.
			Snprintf(szMessage, INTERFACE_STRING_LENGTH -1, g_InterfaceManager.GetInterfaceString( 7231 ) );
		}
		break;

	case RC_PARTY_CANNOT_INVITE_NEW_MEMBER:
		{
			//5389	새로운 파티원을 받을수없습니다.
			Snprintf(szMessage, INTERFACE_STRING_LENGTH -1, g_InterfaceManager.GetInterfaceString(5389));
		}
		break;
    case RC_PARTY_BEING_INVITE_REQUEST:
        {
            //65018 파티 초대 요청 중이다.
            Snprintf(szMessage, INTERFACE_STRING_LENGTH -1, g_InterfaceManager.GetInterfaceString(65018));
        }
        break;
    case RC_PARTY_BEING_INVITE_RESPONSE:
        {
            //65019 파티 초대 받는 중이다.
            Snprintf(szMessage, INTERFACE_STRING_LENGTH -1, g_InterfaceManager.GetInterfaceString(65019));
        }
        break;
    case RC_PARTY_INVITE_RESPONSE_TIME_OVER:
        {
            //65020 파티 초대 응답시간 8초가 지나 취소되었습니다.
            Snprintf(szMessage, INTERFACE_STRING_LENGTH -1, g_InterfaceManager.GetInterfaceString(65020));
        }
        break;
    case RC_PARTY_PORTAL_ENTERANCE_MEMBER:
        {
            //_NA003129_QUEST_PORTAL_CONVENIENCE_EXTENTION
            // 70550	상대방이 현재 파티상태로 미션포탈에 입장 대기중인 대상입니다.\n파티에 초대할 수 없습니다.
            Snprintf(szMessage, INTERFACE_STRING_LENGTH -1, g_InterfaceManager.GetInterfaceString(70550));
        }
        break;
#ifdef _DH_BATTLEZONE2_
    case RC_PARTY_FAIL_CREATE_PARTY_ROOM:
        {
            //2440 방생성에 실패 하였습니다.
            g_InterfaceManager.GetInterfaceString(2440, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_KICKOUT_PARTY_ROOM:
        {
            //2441 강제로 파티룸에서 킥
            g_InterfaceManager.GetInterfaceString(2441, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_NOT_OPEN_TIME:
        {
            //2442 해당 룸을 열수 있는 시간이 아니다.
            g_InterfaceManager.GetInterfaceString(2442, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_NOTEQUAL_PASSWORD:
        {
           //2494 비밀 번호를 잘못 입력하였습니다.
            g_InterfaceManager.GetInterfaceString(2494, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_NOT_MATCHING_USER_LEVEL:
        {
            //2493 레벨 조건에 맞지 않는다.
            g_InterfaceManager.GetInterfaceString(2493, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_HAVE_NOT_REQUIRE_ITEM:
        {
            //2487 방 생성에 필요한 아이템이 없습니다.
            g_InterfaceManager.GetInterfaceString(2487, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_NOT_ALLMEMBER_READY:
        {
            // 모든 멤버가 준비되지 않았다.
            g_InterfaceManager.GetInterfaceString(2440, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_PREVIOUS_QUEST_CLEAR:
        {
            // 2496 연결된 퀘스트 먼저 클리어 해야 합니다.
            g_InterfaceManager.GetInterfaceString(2496, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_NEED_CLEAR_PRE_MISSION:
        {
            // 2497 이전 미션을 클리어 해야합니다.
            g_InterfaceManager.GetInterfaceString(2497, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_FAIL_MAX_MEMBER_COUNT:
        {
            // 2477 입장 인원이 초과 되었습니다.
            g_InterfaceManager.GetInterfaceString(2477, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_FAIL_DESTROY_PARTY_ROOM:
        {
            // 70653 파티룸 제거에 실패했습니다.
            g_InterfaceManager.GetInterfaceString(70653, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_FAIL_DESTROY_PARTY_LIST:
        {
            // 70654 파티룸 리스트 제거에 실패했습니다.
            g_InterfaceManager.GetInterfaceString(70654, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_FAIL_LEAVE_PARTY_ROOM:
        {
            // 70655 파티 나가기에 실패했습니다.
            g_InterfaceManager.GetInterfaceString(70655, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_FAIL_JOIN_PARTY_ROOM:
        {
            // 70656 파티룸 참여에 실패했습니다.
            g_InterfaceManager.GetInterfaceString(70656, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_FAIL_START_MISSION:
        {
            // 70657 파티룸 미션 시작을 실패했습니다.
            g_InterfaceManager.GetInterfaceString(70657, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_FAIL_RESET_MISSION_CODE:
        {
            // 70658 미션 코드 초기화에 실패했습니다.
            g_InterfaceManager.GetInterfaceString(70658, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_FAIL_CHANGE_ROOM_OPTION:
        {
            // 70659 파티룸 옵션 변경에 실패했습니다.
            g_InterfaceManager.GetInterfaceString(70659, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_FAIL_CREATE_ROOM:
        {
            // 70660 파티룸 생성에 실패했습니다.
            g_InterfaceManager.GetInterfaceString(70660, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_NOT_EXIST_PARTY:
        {
            // 70661 파티가 존재하지 않습니다.
            g_InterfaceManager.GetInterfaceString(70661, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_NOT_INCORRECT_MAPCODE:
        {
            // 70662 잘못된 미션 코드 입니다.
            g_InterfaceManager.GetInterfaceString(70662, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_NOT_FOUND_PARTYROOM:
        {
            // 70663 파티룸 정보가 존재하지 않습니다.
            g_InterfaceManager.GetInterfaceString(70663, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_IS_NOT_PARTY_ROOM:
        {
            // 70664 룸 형식의 파티가 아닙니다.
            g_InterfaceManager.GetInterfaceString(70664, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_INVALID_PARTY_ROOM_MAPCODE:
        {
            // 70665 잘못된 파티룸의 맵정보 입니다.
            g_InterfaceManager.GetInterfaceString(70665, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_NOT_MISSION_REWARD:
        {
            // 70666 미션 보상을 받아야 합니다.
            g_InterfaceManager.GetInterfaceString(70666, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_INVALID_ITEM:
        {
            // 70667 아이템 사용 조건이 맞지 않습니다.
            g_InterfaceManager.GetInterfaceString(70667, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_NOT_EXIST_ITEM:
        {
            // 70668 필요한 아이템을 가지고 있지 않습니다.
            g_InterfaceManager.GetInterfaceString(70668, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_ROOM_NEED_UNEQUIP_WEAPON:
        {
            // 70669 파티룸에 입장할 때 장비 정보가 잘못되어 있습니다.
            g_InterfaceManager.GetInterfaceString(70669, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_CANNOT_JOIN_SOMEONE_MEMBER:
        {
            // 70670 파티룸에 입장하지 못하는 멤버가 존재합니다.
            g_InterfaceManager.GetInterfaceString(70670, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_NOT_FOUND_USER:
        {
            // 70671 유저를 찾을 수 없습니다.
            g_InterfaceManager.GetInterfaceString(70671, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_NOT_CAHNGE_OPTION_PLAYING:
        {
            // 70672 미션 플레이 중에는 변경할 수 없습니다.
            g_InterfaceManager.GetInterfaceString(70672, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_FAIL_CHECK_TERMS:
        {
            // 70673 조건 검사에서 실패했습니다.
            g_InterfaceManager.GetInterfaceString(70673, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_INVALID_PARTY_STATE:
        {
            // 70674 파티의 상태가 정상이 아닙니다.
            g_InterfaceManager.GetInterfaceString(70674, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_NOT_FOUND_ROOM:
        {
            // 70675 파티룸이 존재하지 않습니다.
            g_InterfaceManager.GetInterfaceString(70675, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
#endif//_DH_BATTLEZONE2_
    case RC_PARTY_ALREADY_USE_IN_CHAOS_ZONE:
        {
            // 2745 전장 예약중엔 파티를 생성 할 수 없습니다.
            g_InterfaceManager.GetInterfaceString(2745, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    }

    GlobalFunc::OutputMsg_Warning(eOUTPUT_MSG_CHAT, szMessage);
}

void _Confirm_ReponseJoinParty(bool bYes)
{
	uiPartyMan * pMan = (uiPartyMan * ) g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY);
	assert(pMan);

    if (bYes)
    {
		pMan->SetOpenPartyJoinWindow(FALSE);
        MSG_CG_PARTY_JOIN_RESPONSE_ACK sync;
        GlobalFunc::SendPacketToGameServer(&sync, sizeof (sync));
    }
    else
    {
		pMan->SetOpenPartyJoinWindow(FALSE);
        MSG_CG_PARTY_JOIN_RESPONSE_NAK sync;
        GlobalFunc::SendPacketToGameServer(&sync, sizeof (sync));
    }

	if(g_pHero)
		g_pHero->SetBeHaveState(PLAYER_BEHAVE_IDLE_STATE);
}

void
uiPartyMan::ResponseJoinParty(char *pszMasterName)
{
	// 이미 파티창이 열려 있으면 리턴
	if( m_bIsOpenPartyJoinWinodw )
		return;

	m_bIsOpenPartyJoinWinodw = TRUE;

	TCHAR   szMessage[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR   szResult[INTERFACE_STRING_LENGTH] = {0,};

    const int c_iAskJoinParty = 100;
	g_InterfaceManager.GetInterfaceString(eST_PARTY_INVITED_ALLOW,szMessage,INTERFACE_STRING_LENGTH);
    Snprintf (szResult,INTERFACE_STRING_LENGTH-1, szMessage, pszMasterName);

	uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
	if(pSystemMan)
		pSystemMan->Sys_Confirm(szResult, _Confirm_ReponseJoinParty);

	if(g_pHero)
		g_pHero->SetBeHaveState(PLAYER_BEHAVE_PARTY_THINKING_JOIN);
}

//------------------------------------------------------------------------------
/**
*/
void uiPartyMan::ChangeLevel(DWORD dwObjKey, WORD wLevel)
{
	for (int i=0; i<this->GetPartyNum(); ++i)
    {
        if (this->m_vParty[i].CompKeySimple(dwObjKey))
        {
            this->m_vParty[i].SetLevel(wLevel);
        }
    }	
}

//------------------------------------------------------------------------------
/**
*/
void uiPartyMan::ChangeHP(DWORD dwObjKey, BYTE byHpRatio)
{
	for (int i=0; i<this->GetPartyNum(); ++i)
    {
        if (this->m_vParty[i].CompKeySimple(dwObjKey))
        {
            this->m_vParty[i].SetHpRatio(byHpRatio);
        }
    }	
}

//------------------------------------------------------------------------------
/**
*/
void uiPartyMan::ChangeChaoState(DWORD dwObjKey, BYTE byChaoState)
{
	for (int i=0; i<this->GetPartyNum(); ++i)
	{
		if (this->m_vParty[i].CompKeySimple(dwObjKey))
		{
			this->m_vParty[i].SetChaoState(byChaoState);
		}
	}	
}

//------------------------------------------------------------------------------
/**
*/
void uiPartyMan::NET_RECV_MSG_CG_PARTY_NAVI_INFO_BRD(MSG_CG_PARTY_NAVI_INFO_BRD* pMsg_NaviInfo)
{
	if (m_pPartyNaviInfo)
	{
		m_pPartyNaviInfo->Recv_NaviInfo(pMsg_NaviInfo);
	}
}



//------------------------------------------------------------------------------
/**
*/
void uiPartyMan::ChangeMP(DWORD dwObjKey, BYTE byMpRatio)
{
	for (int i=0; i<this->GetPartyNum(); ++i)
    {
        if (this->m_vParty[i].CompKeySimple(dwObjKey))
        {
            this->m_vParty[i].SetMpRatio(byMpRatio);
        }
    }	
}
//------------------------------------------------------------------------------
/**
*/
#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
void uiPartyMan::ChangeSD(DWORD object_key, BYTE sd_ratio)
{
    for (int i=0; i<this->GetPartyNum(); ++i)
    {
        if (this->m_vParty[i].CompKeySimple(object_key))
        {
            this->m_vParty[i].SetSDRatio(sd_ratio);
        }
    }
}
#endif //_NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
//------------------------------------------------------------------------------
/**
*/
void uiPartyMan::DeadPartyMembaer(DWORD dwObjKey)
{
	for (int i=0; i<this->GetPartyNum(); ++i)
    {
        if (this->m_vParty[i].CompKeySimple(dwObjKey))
        {
            this->m_vParty[i].SetDead(true);
        }
    }	
}

//------------------------------------------------------------------------------
/**
*/
void uiPartyMan::ResurrectionPartyMember(DWORD dwObjKey)
{
	for (int i=0; i<this->GetPartyNum(); ++i)
    {
        if (this->m_vParty[i].CompKeySimple(dwObjKey))
        {
            this->m_vParty[i].SetDead(false);
        }
    }	
}
//-------------------------------------------------------------------------------------------
/**
*/
void uiPartyMan::NoticeReasonLeaveParty(DWORD dwObjKey, BYTE byReason)
{
    Object * pObject = g_ObjectManager.GetObject(dwObjKey);
    if (pObject)
    {
        eREASON_LEAVE_PARTY eReason = (eREASON_LEAVE_PARTY)byReason;

        TCHAR szMessage[INTERFACE_STRING_LENGTH];
        switch ( eReason )
        {
        case eREASON_LEAVE_PARTY_INTEND:
            {
				if(g_pHero->GetObjectKey() == dwObjKey)
				{
					// 5244	파티를 탈퇴하셨습니다.	
					GetUIMan()->GetInterfaceString(5244, szMessage, INTERFACE_STRING_LENGTH);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage); 
				}
				else
				{
					// 5620 [%s]님이 의도적으로 방을 나가셨습니다.
					GetUIMan()->GetInterfaceString(5620, szMessage, INTERFACE_STRING_LENGTH);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage, pObject->GetName() ); 
				}
            }
            break;                                           

        case eREASON_LEAVE_PARTY_BAN:
            {                    
				if(g_pHero->GetObjectKey() == dwObjKey)
				{
					// 5245	파티에서 강제 탈퇴되었습니다.		
					GetUIMan()->GetInterfaceString(5245, szMessage, INTERFACE_STRING_LENGTH);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage); 
				}
				else
				{
					// 5618 [%s]님이 강퇴 당하셨습니다.
					GetUIMan()->GetInterfaceString(5618, szMessage, INTERFACE_STRING_LENGTH);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage, pObject->GetName() ); 
				}
            }
            break;

        case eREASON_LEAVE_PARTY_ABNORMAL:
            {
				if(g_pHero->GetObjectKey() == dwObjKey)
				{
					// 5246	파티에서 비정상적으로 나가셨습니다.		
					GetUIMan()->GetInterfaceString(5246, szMessage, INTERFACE_STRING_LENGTH);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage); 
				}
				else
				{
					// 5619 [%s]님이 비정상적으로 방을 나가셨습니다.
					GetUIMan()->GetInterfaceString(5619, szMessage, INTERFACE_STRING_LENGTH);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage, pObject->GetName() ); 
				}
            }
            break;

		case eREASON_LEAVE_PARTY_BY_GUILDRELATION:
			{
				if(g_pHero->GetObjectKey() == dwObjKey)
				{
					// 5247	파티원과의 적대 관계 형성으로 강제 탈퇴되었습니다.		
					GetUIMan()->GetInterfaceString(5247, szMessage, INTERFACE_STRING_LENGTH);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage); 
				}
				else
				{
					//5243 [%s]님이 파티원과 적대관계를 맺어 강퇴 당하셨습니다.
					GetUIMan()->GetInterfaceString(5243, szMessage, INTERFACE_STRING_LENGTH);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage, pObject->GetName() ); 
				}
			}
			break;

        case eREASON_LEAVE_PARTY_NOREASON:
        default:
            break;
        }
    }    
}
//-------------------------------------------------------------------------------------------
/**
*/
#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
void uiPartyMan::NET_RECV_MSG_CG_PARTY_DICECAST_RESULT_CMD(MSG_CG_PARTY_DICECAST_RESULT_CMD* dicecast_result)
{
    DiceCastManager* dice_manager = g_InterfaceManager.GetDiceCastManager();
    if (dice_manager == NULL)
    {
        return;
    }

    //결과 값 내림차순 정렬
    for (int i=0; i<MAX_PARTYMEMBER_NUM; ++i)
    {
        for (int j=0; j<MAX_PARTYMEMBER_NUM-1; ++j)
        {
            if (dicecast_result->result[j].random_num < dicecast_result->result[j+1].random_num)
            {
                _DICECAST_RESULT temp_result;
                temp_result.member_key = dicecast_result->result[j].member_key;
                temp_result.random_num = dicecast_result->result[j].random_num;

                dicecast_result->result[j].member_key = dicecast_result->result[j+1].member_key;
                dicecast_result->result[j].random_num = dicecast_result->result[j+1].random_num;

                dicecast_result->result[j+1].member_key = temp_result.member_key;
                dicecast_result->result[j+1].random_num = temp_result.random_num;
            }
        }
    }

    // 해당 아이템 오너키 변경 + 주사위 속성 제거
    Object* object_ptr = g_ObjectManager.GetObject(dicecast_result->item_object_key);
    if (object_ptr)
    {
        Item* item_ptr = static_cast<Item *>(object_ptr);
        if (item_ptr)
        {
            item_ptr->set_is_dice_item(false);
            item_ptr->SetOwner(dicecast_result->winner_key);
        }
    }
    dice_manager->set_high_number_player_key(dicecast_result->result[0].member_key);

    // 센터 메세지 설정
    TCHAR player_name[INTERFACE_STRING_LENGTH] = {0,};
    uicPartyMemberInfoPlus*	member_info = GetPartyMemberInfo(dicecast_result->result[0].member_key);
    if (member_info != NULL)
    {
        TCHAR value_string[INTERFACE_STRING_LENGTH] = {0,};
        Sprintf(value_string, _T("%d"),dicecast_result->result[0].random_num);

        TCHAR item_name[INTERFACE_STRING_LENGTH] = {0,};
        BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(dicecast_result->item_code);
        if (item_info)
        {
            g_InterfaceManager.GetItemName(item_info->m_Code, item_name, INTERFACE_STRING_LENGTH);
        }

        TCHAR temp_message[INTERFACE_STRING_LENGTH];
        // 70576 [%s] 님이 [%s] 값으로 [%s] 아이템을 획득하였습니다.
        g_InterfaceManager.GetInterfaceString(70576, temp_message, INTERFACE_STRING_LENGTH);

        TCHAR center_message[INTERFACE_STRING_LENGTH];
        Sprintf(center_message, temp_message, member_info->Get().m_szCharName, value_string, item_name);
        dice_manager->SetCenterMessage(center_message);
    }


    // 결과 값 세팅
    dice_manager->SetDiceDrawInfo(dicecast_result->result, MAX_PARTYMEMBER_NUM);
    g_InterfaceManager.SetDiceAngle();


    // 시스템창에 결과 값 출력
    // 70575 주사위 결과 :
    TCHAR	string_message[INTERFACE_STRING_LENGTH] = {0,};
    g_InterfaceManager.GetInterfaceString(70575,string_message,INTERFACE_STRING_LENGTH);

    TCHAR temp_message[INTERFACE_STRING_LENGTH] = {0,};
    for (int i=0; i<MAX_PARTYMEMBER_NUM; ++i)
    {
        if (dicecast_result->result[i].random_num > 0)
        {
            uicPartyMemberInfoPlus* member_info =
                GetPartyMemberInfo(dicecast_result->result[i].member_key);
            if (member_info == NULL)
            {
                continue;
            }

            Sprintf(temp_message,
                "%s%s [%d]",
                ((i != 0) ? ", " : " "),
                member_info->Get().m_szCharName,
                dicecast_result->result[i].random_num);

            StrCat(string_message, temp_message);
        }
    }
    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, string_message);

}
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING

#pragma warning ( pop )

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
