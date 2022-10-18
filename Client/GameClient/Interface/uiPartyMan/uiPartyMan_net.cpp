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

	// �����ۺй� ���� ó�� �ʿ�
	m_pUIItemShareSystem->SetToggleName(pMsg->m_DistributionType);

	for (int i=0; i < pMsg->m_MemberTotalInfo.m_Count; ++i)
	{
		uicPartyMemberInfoPlus memInfoPlus;
		memInfoPlus = pMsg->m_MemberTotalInfo.m_Slot[i];

#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
        // ���������� + �ڽ��� ���̵�� �߰�����
        // (���������� �۾��ϸ鼭 �ɹ��� ����������� �ڽ� ������ �������� �ִ�)
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
    //��Ƽ�� �����������
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
    //��Ƽ�� ���� �� �� ��Ƽ��- ���� ����, ���ο� ��Ƽ�� - ���� ����
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
			// ����� ����� �����
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

			if ((byReason == eREASON_LEAVE_PARTY_ABNORMAL) && bZoneTypeAccept) // ������ �����ڴ� ���� ��Ƽ� �����Ѵ�.
			{
#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
                //���� ������Ʈ Ű ���� ��ȿ�ϹǷ� ���⼭ ���� ������.
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
			//< 5200	��Ƽ�����Ը� ��Ƽ���� �����Ҽ��ֽ��ϴ�.
			g_InterfaceManager.GetInterfaceString(5200,szMessage,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_PARTY_COOLTIME_ERROR:
		{  
		    //��Ƽ��,��Ƽ�� ��ȯ ����	
			//< ��Ÿ���� ���� ���� ����ϼ���
			g_InterfaceManager.GetInterfaceString(eST_SKILL_FAIL_COOLTIME_NOT_EXPIRED,szMessage,INTERFACE_STRING_LENGTH);

		}
		break;

    case RC_PARTY_PLAYER_NOTEXIST:
        {
            //< �׷� �÷��̾ �������� �ʴ´�.
			g_InterfaceManager.GetInterfaceString(eST_PARTY_PLAYER_NOTEXIST,szMessage,INTERFACE_STRING_LENGTH);
        }
        break;

    case RC_PARTY_ISNOT_MASTEROFPARTY:
        {
            //< ��Ƽ���� �ƴϴ�.
			g_InterfaceManager.GetInterfaceString(eST_PARTY_ISNOT_MASTEROFPARTY,szMessage,INTERFACE_STRING_LENGTH);
        }
        break;

    case RC_PARTY_ISNOT_MEMBEROFPARTY:
        {
            //< ��Ƽ���� �ƴϴ�.
			g_InterfaceManager.GetInterfaceString(eST_PARTY_ISNOT_MEMBEROFPARTY,szMessage,INTERFACE_STRING_LENGTH);
        }
        break;

    case RC_PARTY_PARTY_NOTEXIST:
        {
            //< ��Ƽ�� �������� �ʴ´�.
			g_InterfaceManager.GetInterfaceString(eST_PARTY_PARTY_NOTEXIST,szMessage,INTERFACE_STRING_LENGTH);
        }
        break;

    case RC_PARTY_REJECT_INVITEMENT:
        {
            //< ������ ��Ƽ�ʴ� ��û�� �����ϴ�.
			g_InterfaceManager.GetInterfaceString(eST_PARTY_REJECT_INVITEMENT,szMessage,INTERFACE_STRING_LENGTH);
        }
        break;

    case RC_PARTY_ISNOT_QUALIFIED_LEVEL:
        {
            //< ���������� ���� �ʴ´�.

			g_InterfaceManager.GetInterfaceString(eST_PARTY_ISNOT_QUALIFIED_LEVEL,szMessage,INTERFACE_STRING_LENGTH);
#ifdef _DH_BATTLEZONE2_
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
#endif//_DH_BATTLEZONE2_
        }
        break;

    case RC_PARTY_INVALID_ROOMTYPE:
        {
            //< �߸��� �� �����̴�.
			g_InterfaceManager.GetInterfaceString(eST_PARTY_INVALID_ROOMTYPE,szMessage,INTERFACE_STRING_LENGTH);
#ifdef _DH_BATTLEZONE2_
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
#endif//_DH_BATTLEZONE2_
        }
        break;

    case RC_PARTY_CANNOT_CHANGE_MASTER:
        {
            //< ��Ƽ���� ������ �� ����.
			g_InterfaceManager.GetInterfaceString(eST_PARTY_CANNOT_CHANGE_MASTER,szMessage,INTERFACE_STRING_LENGTH);
        }
        break;

    case RC_PARTY_ALREADY_IS_MEMBEROFPARTY:
        {
            //< �̹� ��Ƽ�� �ҼӵǾ� �ִ�.
			g_InterfaceManager.GetInterfaceString(eST_PARTY_ALREADY_IS_MEMBEROFPARTY,szMessage,INTERFACE_STRING_LENGTH);
#ifdef _DH_BATTLEZONE2_
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
#endif//_DH_BATTLEZONE2_
        }
        break;

    case RC_PARTY_WASNOT_REQUEST_PARTY:
        {
            //< ������ ��Ƽ�� ��û���� �ʾҴ�.
			g_InterfaceManager.GetInterfaceString(eST_PARTY_WASNOT_REQUEST_PARTY,szMessage,INTERFACE_STRING_LENGTH);
        }
        break;

    case RC_PARTY_INVALID_DATATYPE:
        {
            //< ����Ÿ Ÿ���� ���� �ʴ´�.( ��ȿ���� ���� ���̴�. )
			g_InterfaceManager.GetInterfaceString(eST_PARTY_INVALID_DATATYPE,szMessage,INTERFACE_STRING_LENGTH);
        }
        break;

    case RC_PARTY_PLAYER_NOTEXIST_TO_FIELD:
        {
            //< �÷��̾ �ʵ忡 �������� �ʴ´�.
			g_InterfaceManager.GetInterfaceString(eST_PARTY_PLAYER_NOTEXIST_TO_FIELD,szMessage,INTERFACE_STRING_LENGTH);
        }
        break;

    case RC_PARTY_CANNOT_FIND_TARGET:
        {
            //< Ÿ���� ã�� �� ����.
			g_InterfaceManager.GetInterfaceString(eST_PARTY_CANNOT_FIND_TARGET,szMessage,INTERFACE_STRING_LENGTH);
        }
        break;

    case RC_PARTY_TARGET_ISNOT_ENEMY:
        {
            //< Ÿ���� �Ʊ��̴�.
			g_InterfaceManager.GetInterfaceString(eST_PARTY_TARGET_ISNOT_ENEMY,szMessage,INTERFACE_STRING_LENGTH);
        }
        break;

    case RC_PARTY_PLAYER_OBSERVER_MODE:
        {
            //< �÷��̾ ������� ��Ƽ�� �Ҽӵ��� �ʴ´�.
			g_InterfaceManager.GetInterfaceString(PARTY_PLAYER_OBSERVER_MODE,szMessage,INTERFACE_STRING_LENGTH);
        }
        break;

	case RC_PARTY_PARTYMEMBER_NUM_FULL:
		{
			//< ��Ƽ�� ���� �ִ�ġ�� ���̻� ��Ƽ���� ���� �� ����.
			g_InterfaceManager.GetInterfaceString(5330,szMessage,INTERFACE_STRING_LENGTH);
			
		}
		break;


	case RC_PARTY_CHANGE_ITEM_DISTRIBUTION_FAIL:
		{
			// ������ �й� Ÿ�� ���濡 �����Ͽ����ϴ� 5233
			Snprintf(szMessage, INTERFACE_STRING_LENGTH -1 , g_InterfaceManager.GetInterfaceString( 5233 ) );
		}
		break;

	case RC_PARTY_CANNOT_INVITE_HOSTILITY:
		{
			//7231	���� ������ ����� �������� ��Ƽ�� ���� �� �����ϴ�.
			Snprintf(szMessage, INTERFACE_STRING_LENGTH -1, g_InterfaceManager.GetInterfaceString( 7231 ) );
		}
		break;

	case RC_PARTY_CANNOT_INVITE_NEW_MEMBER:
		{
			//5389	���ο� ��Ƽ���� �����������ϴ�.
			Snprintf(szMessage, INTERFACE_STRING_LENGTH -1, g_InterfaceManager.GetInterfaceString(5389));
		}
		break;
    case RC_PARTY_BEING_INVITE_REQUEST:
        {
            //65018 ��Ƽ �ʴ� ��û ���̴�.
            Snprintf(szMessage, INTERFACE_STRING_LENGTH -1, g_InterfaceManager.GetInterfaceString(65018));
        }
        break;
    case RC_PARTY_BEING_INVITE_RESPONSE:
        {
            //65019 ��Ƽ �ʴ� �޴� ���̴�.
            Snprintf(szMessage, INTERFACE_STRING_LENGTH -1, g_InterfaceManager.GetInterfaceString(65019));
        }
        break;
    case RC_PARTY_INVITE_RESPONSE_TIME_OVER:
        {
            //65020 ��Ƽ �ʴ� ����ð� 8�ʰ� ���� ��ҵǾ����ϴ�.
            Snprintf(szMessage, INTERFACE_STRING_LENGTH -1, g_InterfaceManager.GetInterfaceString(65020));
        }
        break;
    case RC_PARTY_PORTAL_ENTERANCE_MEMBER:
        {
            //_NA003129_QUEST_PORTAL_CONVENIENCE_EXTENTION
            // 70550	������ ���� ��Ƽ���·� �̼���Ż�� ���� ������� ����Դϴ�.\n��Ƽ�� �ʴ��� �� �����ϴ�.
            Snprintf(szMessage, INTERFACE_STRING_LENGTH -1, g_InterfaceManager.GetInterfaceString(70550));
        }
        break;
#ifdef _DH_BATTLEZONE2_
    case RC_PARTY_FAIL_CREATE_PARTY_ROOM:
        {
            //2440 ������� ���� �Ͽ����ϴ�.
            g_InterfaceManager.GetInterfaceString(2440, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_KICKOUT_PARTY_ROOM:
        {
            //2441 ������ ��Ƽ�뿡�� ű
            g_InterfaceManager.GetInterfaceString(2441, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_NOT_OPEN_TIME:
        {
            //2442 �ش� ���� ���� �ִ� �ð��� �ƴϴ�.
            g_InterfaceManager.GetInterfaceString(2442, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_NOTEQUAL_PASSWORD:
        {
           //2494 ��� ��ȣ�� �߸� �Է��Ͽ����ϴ�.
            g_InterfaceManager.GetInterfaceString(2494, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_NOT_MATCHING_USER_LEVEL:
        {
            //2493 ���� ���ǿ� ���� �ʴ´�.
            g_InterfaceManager.GetInterfaceString(2493, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_HAVE_NOT_REQUIRE_ITEM:
        {
            //2487 �� ������ �ʿ��� �������� �����ϴ�.
            g_InterfaceManager.GetInterfaceString(2487, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_NOT_ALLMEMBER_READY:
        {
            // ��� ����� �غ���� �ʾҴ�.
            g_InterfaceManager.GetInterfaceString(2440, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_PREVIOUS_QUEST_CLEAR:
        {
            // 2496 ����� ����Ʈ ���� Ŭ���� �ؾ� �մϴ�.
            g_InterfaceManager.GetInterfaceString(2496, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_NEED_CLEAR_PRE_MISSION:
        {
            // 2497 ���� �̼��� Ŭ���� �ؾ��մϴ�.
            g_InterfaceManager.GetInterfaceString(2497, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_FAIL_MAX_MEMBER_COUNT:
        {
            // 2477 ���� �ο��� �ʰ� �Ǿ����ϴ�.
            g_InterfaceManager.GetInterfaceString(2477, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_FAIL_DESTROY_PARTY_ROOM:
        {
            // 70653 ��Ƽ�� ���ſ� �����߽��ϴ�.
            g_InterfaceManager.GetInterfaceString(70653, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_FAIL_DESTROY_PARTY_LIST:
        {
            // 70654 ��Ƽ�� ����Ʈ ���ſ� �����߽��ϴ�.
            g_InterfaceManager.GetInterfaceString(70654, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_FAIL_LEAVE_PARTY_ROOM:
        {
            // 70655 ��Ƽ �����⿡ �����߽��ϴ�.
            g_InterfaceManager.GetInterfaceString(70655, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_FAIL_JOIN_PARTY_ROOM:
        {
            // 70656 ��Ƽ�� ������ �����߽��ϴ�.
            g_InterfaceManager.GetInterfaceString(70656, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_FAIL_START_MISSION:
        {
            // 70657 ��Ƽ�� �̼� ������ �����߽��ϴ�.
            g_InterfaceManager.GetInterfaceString(70657, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_FAIL_RESET_MISSION_CODE:
        {
            // 70658 �̼� �ڵ� �ʱ�ȭ�� �����߽��ϴ�.
            g_InterfaceManager.GetInterfaceString(70658, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_FAIL_CHANGE_ROOM_OPTION:
        {
            // 70659 ��Ƽ�� �ɼ� ���濡 �����߽��ϴ�.
            g_InterfaceManager.GetInterfaceString(70659, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_FAIL_CREATE_ROOM:
        {
            // 70660 ��Ƽ�� ������ �����߽��ϴ�.
            g_InterfaceManager.GetInterfaceString(70660, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_NOT_EXIST_PARTY:
        {
            // 70661 ��Ƽ�� �������� �ʽ��ϴ�.
            g_InterfaceManager.GetInterfaceString(70661, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_NOT_INCORRECT_MAPCODE:
        {
            // 70662 �߸��� �̼� �ڵ� �Դϴ�.
            g_InterfaceManager.GetInterfaceString(70662, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_NOT_FOUND_PARTYROOM:
        {
            // 70663 ��Ƽ�� ������ �������� �ʽ��ϴ�.
            g_InterfaceManager.GetInterfaceString(70663, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_IS_NOT_PARTY_ROOM:
        {
            // 70664 �� ������ ��Ƽ�� �ƴմϴ�.
            g_InterfaceManager.GetInterfaceString(70664, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_INVALID_PARTY_ROOM_MAPCODE:
        {
            // 70665 �߸��� ��Ƽ���� ������ �Դϴ�.
            g_InterfaceManager.GetInterfaceString(70665, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_NOT_MISSION_REWARD:
        {
            // 70666 �̼� ������ �޾ƾ� �մϴ�.
            g_InterfaceManager.GetInterfaceString(70666, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_INVALID_ITEM:
        {
            // 70667 ������ ��� ������ ���� �ʽ��ϴ�.
            g_InterfaceManager.GetInterfaceString(70667, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_NOT_EXIST_ITEM:
        {
            // 70668 �ʿ��� �������� ������ ���� �ʽ��ϴ�.
            g_InterfaceManager.GetInterfaceString(70668, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_ROOM_NEED_UNEQUIP_WEAPON:
        {
            // 70669 ��Ƽ�뿡 ������ �� ��� ������ �߸��Ǿ� �ֽ��ϴ�.
            g_InterfaceManager.GetInterfaceString(70669, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_CANNOT_JOIN_SOMEONE_MEMBER:
        {
            // 70670 ��Ƽ�뿡 �������� ���ϴ� ����� �����մϴ�.
            g_InterfaceManager.GetInterfaceString(70670, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_NOT_FOUND_USER:
        {
            // 70671 ������ ã�� �� �����ϴ�.
            g_InterfaceManager.GetInterfaceString(70671, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_NOT_CAHNGE_OPTION_PLAYING:
        {
            // 70672 �̼� �÷��� �߿��� ������ �� �����ϴ�.
            g_InterfaceManager.GetInterfaceString(70672, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_FAIL_CHECK_TERMS:
        {
            // 70673 ���� �˻翡�� �����߽��ϴ�.
            g_InterfaceManager.GetInterfaceString(70673, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_INVALID_PARTY_STATE:
        {
            // 70674 ��Ƽ�� ���°� ������ �ƴմϴ�.
            g_InterfaceManager.GetInterfaceString(70674, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
    case RC_PARTY_NOT_FOUND_ROOM:
        {
            // 70675 ��Ƽ���� �������� �ʽ��ϴ�.
            g_InterfaceManager.GetInterfaceString(70675, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        }
        break;
#endif//_DH_BATTLEZONE2_
    case RC_PARTY_ALREADY_USE_IN_CHAOS_ZONE:
        {
            // 2745 ���� �����߿� ��Ƽ�� ���� �� �� �����ϴ�.
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
	// �̹� ��Ƽâ�� ���� ������ ����
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
					// 5244	��Ƽ�� Ż���ϼ̽��ϴ�.	
					GetUIMan()->GetInterfaceString(5244, szMessage, INTERFACE_STRING_LENGTH);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage); 
				}
				else
				{
					// 5620 [%s]���� �ǵ������� ���� �����̽��ϴ�.
					GetUIMan()->GetInterfaceString(5620, szMessage, INTERFACE_STRING_LENGTH);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage, pObject->GetName() ); 
				}
            }
            break;                                           

        case eREASON_LEAVE_PARTY_BAN:
            {                    
				if(g_pHero->GetObjectKey() == dwObjKey)
				{
					// 5245	��Ƽ���� ���� Ż��Ǿ����ϴ�.		
					GetUIMan()->GetInterfaceString(5245, szMessage, INTERFACE_STRING_LENGTH);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage); 
				}
				else
				{
					// 5618 [%s]���� ���� ���ϼ̽��ϴ�.
					GetUIMan()->GetInterfaceString(5618, szMessage, INTERFACE_STRING_LENGTH);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage, pObject->GetName() ); 
				}
            }
            break;

        case eREASON_LEAVE_PARTY_ABNORMAL:
            {
				if(g_pHero->GetObjectKey() == dwObjKey)
				{
					// 5246	��Ƽ���� ������������ �����̽��ϴ�.		
					GetUIMan()->GetInterfaceString(5246, szMessage, INTERFACE_STRING_LENGTH);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage); 
				}
				else
				{
					// 5619 [%s]���� ������������ ���� �����̽��ϴ�.
					GetUIMan()->GetInterfaceString(5619, szMessage, INTERFACE_STRING_LENGTH);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage, pObject->GetName() ); 
				}
            }
            break;

		case eREASON_LEAVE_PARTY_BY_GUILDRELATION:
			{
				if(g_pHero->GetObjectKey() == dwObjKey)
				{
					// 5247	��Ƽ������ ���� ���� �������� ���� Ż��Ǿ����ϴ�.		
					GetUIMan()->GetInterfaceString(5247, szMessage, INTERFACE_STRING_LENGTH);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage); 
				}
				else
				{
					//5243 [%s]���� ��Ƽ���� ������踦 �ξ� ���� ���ϼ̽��ϴ�.
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

    //��� �� �������� ����
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

    // �ش� ������ ����Ű ���� + �ֻ��� �Ӽ� ����
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

    // ���� �޼��� ����
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
        // 70576 [%s] ���� [%s] ������ [%s] �������� ȹ���Ͽ����ϴ�.
        g_InterfaceManager.GetInterfaceString(70576, temp_message, INTERFACE_STRING_LENGTH);

        TCHAR center_message[INTERFACE_STRING_LENGTH];
        Sprintf(center_message, temp_message, member_info->Get().m_szCharName, value_string, item_name);
        dice_manager->SetCenterMessage(center_message);
    }


    // ��� �� ����
    dice_manager->SetDiceDrawInfo(dicecast_result->result, MAX_PARTYMEMBER_NUM);
    g_InterfaceManager.SetDiceAngle();


    // �ý���â�� ��� �� ���
    // 70575 �ֻ��� ��� :
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
