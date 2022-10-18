//------------------------------------------------------------------------------
//  uiBattleZoneMan_net.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiBattleZoneMan.h"
#include "uiBattleZoneCreate/uiBattleZoneCreate.h"
#include "uiBattleZoneCreateEp2\uiBattleZoneCreateEp2.h"
#include "Map.h"
#include "uiBattleZoneReadyCHuntingConfig/uiBattleZoneReadyCHuntingConfig.h"
#include "VillageScene.h"
#include "BattleLobby.h"
#include "UserSlot.h"
#include "ObjectManager.h"
#include "Hero.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "uiSystemMan/uiSystemMan.h"
#include "ItemManager.h"

//------------------------------------------------------------------------------
/**
*/

DWORD	g_dwKickPlayerKey = 0;
//------------------------------------------------------------------------------
/**
*/
void _Confirm_KickMember(bool bYes)
{
	if(bYes && g_dwKickPlayerKey && g_pHero)
	{
		uiBattleZoneMan* pZoneMan = static_cast<uiBattleZoneMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLEZONE));
		if(pZoneMan)
		{
			pZoneMan->Send_LOBBY_KICKPLAYER_SYN(g_dwKickPlayerKey);
		}
	}

	g_dwKickPlayerKey = 0;
}

void
uiBattleZoneMan::InitPKBooleans()
{
    this->m_vPKBool.clear();

    for (int i=ePKBOOL_BATTLEZONE_BASE; i<ePKBOOL_BATTLEZONE_MAX; ++i)
    {
        uicPacketControl packetControl;
        this->m_vPKBool.push_back(packetControl);
    }
}

//------------------------------------------------------------------------------
/**
    create battlezone lobby to any type

    - MISSION:  mission
    - HUNTING:  hunting
    - PVP: player vs player
    - CHUNTING: competition hunting
*/
void					
uiBattleZoneMan::NET_SEND_MSG_CG_ZONE_LOBBY_CREATE_SYN()
{
    if (SafePKBool(ePKBOOL_BATTLEZONE_CREATE_CG_ZONE_LOBBY_CREATE_SYN))
    {
        // sync create zone
        MSG_CG_ZONE_LOBBY_CREATE_SYN sync;

        int continent_number = 1;

        if(g_pMap)
        {
            const sMAPINFO* map_info = MapInfoParser::Instance()->FindMapInfo((MAPCODE)g_pMap->GetMapID());
            if(map_info)
            {
                continent_number = map_info->ContienetNumber;
            }
        }

        const uicBattleZoneCreate_Configs* pcfg =
            (continent_number == 1)?&m_pUIBattleZoneCreate->GetCfgs():
                                    &ui_battlezone_create_ep2_->GetCfgs();

		sync.m_byMaxLimitUserNum = pcfg->peopleType + 1;

		switch (pcfg->roomType)
        {
#ifdef _CN_EXCECPT_MISSION
		//no operation	
#else
        case eCREATE_BATTLE_MISSION_TYPE:
			sync.m_byRoomType = eZONETYPE_MISSION;
			break;
#endif//_CN_EXCECPT_MISSION
        case eCREATE_BATTLE_PVP_TYPE: 
			{ 
				sync.m_byRoomType = eZONETYPE_PVP; 
				m_RoomSpecialType = eROOM_SPECIAL_NORMAL;
			} 
			break;
#ifndef _EXCECPT_HUNTING_001011
        case eCREATE_BATTLE_CHUNTING:
			sync.m_byRoomType = eZONETYPE_CHUNTING;
			break;
#endif
        default: { assert (!"WTF roomType"); } return;
        }

        sync.m_MapCode = pcfg->mapCode;
        if (!lstrlen(pcfg->strPassword))
            sync.m_byRoomPublic = eZONEPUBLIC_PUBLIC;
        else
            sync.m_byRoomPublic = eZONEPUBLIC_PRIVATE;

		ZeroMemory(sync.m_pszRoomTitle, sizeof(sync.m_pszRoomTitle));
		StrnCopy(sync.m_pszRoomTitle, pcfg->strTitle, MAX_ROOMTITLE_LENGTH-1);
        ZeroMemory(sync.m_pszRoomPWD, sizeof(sync.m_pszRoomPWD));
        StrnCopy(sync.m_pszRoomPWD, pcfg->strPassword, MAX_ROOMPASSWORD_LENGTH-1);

        sync.m_RoomInfo.m_ClassLimit = pcfg->classLimit;
        sync.m_RoomInfo.m_MinLV = (BYTE)pcfg->minLevel;
        sync.m_RoomInfo.m_MaxLV = (BYTE)pcfg->maxLevel;
		sync.m_RoomInfo.m_SpecialType = m_RoomSpecialType;

		// fill the battleroomentry in global
        // base type for create zone
        BattleRoomEntry entry;
		entry.Init();
        entry.SetType((eZONETYPE)sync.m_byRoomType);
        entry.SetTitle(sync.m_pszRoomTitle);
		entry.SetPassword(sync.m_pszRoomPWD);
        entry.SetPublic((eZONEPUBLIC)sync.m_byRoomPublic);
        entry.SetMinLevel(sync.m_RoomInfo.m_MinLV);
        entry.SetMaxLevel(sync.m_RoomInfo.m_MaxLV);
        entry.SetMaxPlayer(pcfg->peopleType + 1);
        entry.SetMapCode(pcfg->mapCode);
		entry.m_RoomInfo.m_SpecialType = m_RoomSpecialType;

        BattleScene::SetPendingBattleRoomEntry(&entry);

		sync.Encode();

        GlobalFunc::SendPacketToGameServer(&sync, sizeof (sync));

        SetPKBool(ePKBOOL_BATTLEZONE_CREATE_CG_ZONE_LOBBY_CREATE_SYN, 10);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneMan::NET_RECV_MSG_CG_ZONE_LOBBY_CREATE_ACK( MSG_BASE * pMsg )
{
    this->UnsetPKBool(ePKBOOL_BATTLEZONE_CREATE_CG_ZONE_LOBBY_CREATE_SYN);
    
    MSG_CG_ZONE_LOBBY_CREATE_ACK* msg =
        (MSG_CG_ZONE_LOBBY_CREATE_ACK *)pMsg;

    if (!g_pHero)
		return;

    BattleRoomEntry * pRoomEntry = BattleScene::GetPendingBattleRoomEntry();
    assert (pRoomEntry);

	if(!pRoomEntry)
		return;

    // roomkey toss to dialog (battle lobby)
	pRoomEntry->SetSerial(msg->m_LobbyKey);

	SetBattleRoomEntry( pRoomEntry );
	SetLobbyMasterPlayerKey(g_pHero->GetObjectKey());
	SetNonBlockSlot(pRoomEntry->GetMaxPlayer());

	UserSlot EnterUser;
	EnterUser.SetPlayerKey( g_pHero->GetObjectKey() );
	AddPlayer(EnterUser);
	
    g_ObjectManager.DeleteObjectsByType(MONSTER_OBJECT);
    g_ObjectManager.DeletePlayersWithoutHero( g_pHero->GetObjectKey() );

	BattleScene::SetLobbyFlag(TRUE);
	BattleScene::SetLobbyInPending(TRUE);
	BattleScene::SendPlayerEnterSyn();

    m_pUIBattleZoneCreate->ShowInterface(FALSE);
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneMan::NET_RECV_MSG_CG_ZONE_LOBBY_CREATE_NAK( MSG_BASE * pMsg )
{
    this->UnsetPKBool(ePKBOOL_BATTLEZONE_CREATE_CG_ZONE_LOBBY_CREATE_SYN);

    MSG_CG_ZONE_LOBBY_CREATE_NAK* msg =
        (MSG_CG_ZONE_LOBBY_CREATE_NAK *)pMsg;

	BattleScene::SetLobbyFlag(FALSE);
	BattleScene::SetLobbyInPending(FALSE);

    TCHAR   szMessage[INTERFACE_STRING_LENGTH];
    g_InterfaceManager.GetStringZoneError(msg->m_dwErrorCode, szMessage, INTERFACE_STRING_LENGTH );

	uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
	if(pSystemMan)
		pSystemMan->Sys_VerifyLock(szMessage);

    m_pUIBattleZoneCreate->Clear();
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneMan::NET_RECV_MSG_CG_ZONE_SHORTCUT_MISSIONLOBBY_ACK( MSG_BASE * pMsg )
{
    this->UnsetPKBool(ePKBOOL_BATTLEZONE_LIST_CG_ZONE_SHORTCUT_SYN);

    MSG_CG_ZONE_SHORTCUT_MISSIONLOBBY_ACK * pRecvMsg = (MSG_CG_ZONE_SHORTCUT_MISSIONLOBBY_ACK *)pMsg;

    BattleRoomEntry BattleRoom;
    BattleRoom.Init();

    BattleRoom.SetSerial(pRecvMsg->m_RoomInfo.m_Key);
    BattleRoom.SetType(eZONETYPE_MISSION);
    BattleRoom.SetPublic((eZONEPUBLIC)pRecvMsg->m_RoomInfo.m_byRoomPublic);		//< eZONEPUBLIC
    BattleRoom.SetMapCode(pRecvMsg->m_RoomInfo.m_MapCode);
    BattleRoom.SetFieldIndex(0); // 미션은 무조건 0으로!
    BattleRoom.SetTitle(pRecvMsg->m_RoomInfo.m_pszRoomTitle);
    BattleRoom.SetBaseInfo(&pRecvMsg->m_RoomInfo.m_RoomInfo);

    BattleScene::SetPendingBattleRoomEntry(&BattleRoom);

    MSG_CG_ZONE_LOBBY_JOIN_SYN	SendPacket;
    SendPacket.m_LobbyKey		= pRecvMsg->m_RoomInfo.m_Key;
    SendPacket.m_byRoomPublic	= pRecvMsg->m_RoomInfo.m_byRoomPublic;		//< eZONEPUBLIC
    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, SendPacket.GetSize());

	GET_DIALOG( IM_BATTLEZONE_MANAGER::BATTLEZONE_CREATE )->ShowInterface( FALSE );
    GET_DIALOG( IM_BATTLEZONE_MANAGER::BATTLEZONE_CREATE_EP2 )->ShowInterface(FALSE);    
	GET_DIALOG( IM_BATTLEZONE_MANAGER::BATTLEZONE_LIST )->ShowInterface( FALSE );
}


//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneMan::NET_RECV_MSG_CG_ZONE_SHORTCUT_PVPLOBBY_ACK( MSG_BASE * pMsg )
{
    this->UnsetPKBool(ePKBOOL_BATTLEZONE_LIST_CG_ZONE_SHORTCUT_SYN);

    MSG_CG_ZONE_SHORTCUT_PVPLOBBY_ACK * pRecvMsg = (MSG_CG_ZONE_SHORTCUT_PVPLOBBY_ACK *)pMsg;

    BattleRoomEntry BattleRoom;
    BattleRoom.Init();
    BattleRoom.SetSerial(pRecvMsg->m_RoomInfo.m_Key);
    BattleRoom.SetType(eZONETYPE_PVP);
    BattleRoom.SetPublic((eZONEPUBLIC)pRecvMsg->m_RoomInfo.m_byRoomPublic);
    BattleRoom.SetTitle(pRecvMsg->m_RoomInfo.m_pszRoomTitle);
    BattleRoom.SetMapCode(pRecvMsg->m_RoomInfo.m_MapCode);
    BattleRoom.SetBaseInfo(&pRecvMsg->m_RoomInfo.m_RoomInfo);

    BattleRoom.SetAdditionalPVPInfo(&pRecvMsg->m_RoomInfo.m_AdditionalPVPInfo);

    BattleScene::SetPendingBattleRoomEntry(&BattleRoom);

    MSG_CG_ZONE_LOBBY_JOIN_SYN	SendPacket;
    SendPacket.m_LobbyKey		= pRecvMsg->m_RoomInfo.m_Key;
    SendPacket.m_byRoomPublic	= pRecvMsg->m_RoomInfo.m_byRoomPublic;
    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, SendPacket.GetSize());			
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneMan::NET_RECV_MSG_CG_ZONE_SHORTCUT_CHUNTING_ACK( MSG_BASE * pMsg )
{
    this->UnsetPKBool(ePKBOOL_BATTLEZONE_LIST_CG_ZONE_SHORTCUT_SYN);

    MSG_CG_ZONE_SHORTCUT_CHUNTINGLOBBY_ACK * pRecvMsg = (MSG_CG_ZONE_SHORTCUT_CHUNTINGLOBBY_ACK *)pMsg;

    BattleRoomEntry BattleRoom;
    BattleRoom.Init();
    BattleRoom.SetSerial(pRecvMsg->m_RoomInfo.m_Key);
    BattleRoom.SetType(eZONETYPE_CHUNTING);
    BattleRoom.SetPublic((eZONEPUBLIC)pRecvMsg->m_RoomInfo.m_byRoomPublic);
    BattleRoom.SetTitle(pRecvMsg->m_RoomInfo.m_pszRoomTitle);
    BattleRoom.SetMapCode(pRecvMsg->m_RoomInfo.m_MapCode);
    BattleRoom.SetBaseInfo(&pRecvMsg->m_RoomInfo.m_RoomInfo);

    BattleScene::SetPendingBattleRoomEntry(&BattleRoom);

    MSG_CG_ZONE_LOBBY_JOIN_SYN	SendPacket;
    SendPacket.m_LobbyKey		= pRecvMsg->m_RoomInfo.m_Key;
    SendPacket.m_byRoomPublic	= pRecvMsg->m_RoomInfo.m_byRoomPublic;
    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, SendPacket.GetSize());			

    GET_DIALOG( IM_BATTLEZONE_MANAGER::BATTLEZONE_CREATE )->ShowInterface(FALSE);
    GET_DIALOG( IM_BATTLEZONE_MANAGER::BATTLEZONE_CREATE )->ShowInterface(FALSE);
	GET_DIALOG( IM_BATTLEZONE_MANAGER::BATTLEZONE_LIST )->ShowInterface(FALSE);
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneMan::NET_RECV_MSG_CG_ZONE_SHORTCUT_NAK( MSG_BASE * pMsg )
{
    this->UnsetPKBool(ePKBOOL_BATTLEZONE_LIST_CG_ZONE_SHORTCUT_SYN);

    MSG_CG_ZONE_SHORTCUT_NAK * pRecvMsg = (MSG_CG_ZONE_SHORTCUT_NAK *)pMsg;		

    TCHAR   szMessage[INTERFACE_STRING_LENGTH];

    g_InterfaceManager.GetStringZoneError(pRecvMsg->m_dwErrorCode, szMessage, INTERFACE_STRING_LENGTH );
    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
}

void
uiBattleZoneMan::NET_SEND_MSG_CG_ZONE_LIST_MISSIONLOBBY_SYN( BYTE count )
{
    if (SafePKBool(ePKBOOL_BATTLEZONE_LIST_CG_ZONE_LIST_MISSIONLOBBY_SYN))
    {
        MSG_CG_ZONE_LIST_MISSIONLOBBY_SYN   sync;
        sync.m_Count = count;

        GlobalFunc::SendPacketToGameServer(&sync, sizeof (sync));

        SetPKBool(ePKBOOL_BATTLEZONE_LIST_CG_ZONE_LIST_MISSIONLOBBY_SYN, 3);
    }
}


void
uiBattleZoneMan::NET_SEND_MSG_CG_ZONE_LIST_PVPLOBBY_SYN( BYTE count )
{
    if (SafePKBool(ePKBOOL_BATTLEZONE_LIST_CG_ZONE_LIST_PVPLOBBY_SYN))
    {
        MSG_CG_ZONE_LIST_PVPLOBBY_SYN   sync;
        sync.m_Count = count;

        GlobalFunc::SendPacketToGameServer(&sync, sizeof (sync));

        SetPKBool(ePKBOOL_BATTLEZONE_LIST_CG_ZONE_LIST_PVPLOBBY_SYN, 3);
    }
}

void
uiBattleZoneMan::NET_SEND_MSG_CG_ZONE_LIST_CHUNTINGLOBBY_SYN( BYTE count )
{
    if (SafePKBool(ePKBOOL_BATTLEZONE_LIST_CG_ZONE_LIST_CHUNTINGLOBBY_SYN))
    {
        MSG_CG_ZONE_LIST_CHUNTINGLOBBY_SYN   sync;
        sync.m_Count = count;

        GlobalFunc::SendPacketToGameServer(&sync, sizeof (sync));

        SetPKBool(ePKBOOL_BATTLEZONE_LIST_CG_ZONE_LIST_CHUNTINGLOBBY_SYN, 3);
    }
}

void
uiBattleZoneMan::NET_SEND_MSG_CG_ZONE_LIST_CHUNTING_SYN( BYTE count )
{
    if (SafePKBool(ePKBOOL_BATTLEZONE_LIST_CG_ZONE_LIST_CHUNTING_SYN))
    {
        MSG_CG_ZONE_LIST_CHUNTING_SYN   sync;
        sync.m_Count = count;

        GlobalFunc::SendPacketToGameServer(&sync, sizeof (sync));

        SetPKBool(ePKBOOL_BATTLEZONE_LIST_CG_ZONE_LIST_CHUNTING_SYN, 3);
    }
}

void
uiBattleZoneMan::NET_RECV_MSG_CG_ZONE_LIST_MISSIONLOBBY_ACK( MSG_BASE * pMsg )
{
    this->UnsetPKBool(ePKBOOL_BATTLEZONE_LIST_CG_ZONE_LIST_MISSIONLOBBY_SYN);

    MSG_CG_ZONE_LIST_MISSIONLOBBY_ACK* pRecvMsg = (MSG_CG_ZONE_LIST_MISSIONLOBBY_ACK *)pMsg;

    for (int i=0; i<pRecvMsg->m_Count; ++i)
    {
        cBattleZoneListItem item;
        item.setM (pRecvMsg->m_RoomInfo[i]);
        bool needUpdate = true;

        for (size_t i=0; i<m_zoneListItem.size(); ++i)
        {
            if (m_zoneListItem[i] == item)
            {
                m_zoneListItem[i] = item;
                needUpdate = false;
            }
        }

        if (needUpdate)
            m_zoneListItem.push_back(item);
    }
}

void
uiBattleZoneMan::NET_RECV_MSG_CG_ZONE_LIST_PVPLOBBY_ACK( MSG_BASE * pMsg )
{
    this->UnsetPKBool(ePKBOOL_BATTLEZONE_LIST_CG_ZONE_LIST_PVPLOBBY_SYN);

    MSG_CG_ZONE_LIST_PVPLOBBY_ACK* pRecvMsg = (MSG_CG_ZONE_LIST_PVPLOBBY_ACK *)pMsg;

    for (int i=0; i<pRecvMsg->m_Count; ++i)
    {
        cBattleZoneListItem item;
        item.setP (pRecvMsg->m_RoomInfo[i]);
        bool needUpdate = true;

        for (size_t i=0; i<m_zoneListItem.size(); ++i)
        {
            if (m_zoneListItem[i] == item)
            {
                m_zoneListItem[i] = item;
                needUpdate = false;
            }
        }

        if (needUpdate)
            m_zoneListItem.push_back(item);
    }
}

void
uiBattleZoneMan::NET_RECV_MSG_CG_ZONE_LIST_CHUNTINGLOBBY_ACK( MSG_BASE * pMsg )
{
    this->UnsetPKBool(ePKBOOL_BATTLEZONE_LIST_CG_ZONE_LIST_CHUNTINGLOBBY_SYN);

    MSG_CG_ZONE_LIST_CHUNTINGLOBBY_ACK* pRecvMsg = (MSG_CG_ZONE_LIST_CHUNTINGLOBBY_ACK *)pMsg;

    for (int i=0; i<pRecvMsg->m_Count; ++i)
    {
        cBattleZoneListItem item;
        item.setCH (pRecvMsg->m_RoomInfo[i], 0);
        bool needUpdate = true;

        for (size_t i=0; i<m_zoneListItem.size(); ++i)
        {
            if (m_zoneListItem[i] == item)
            {
                m_zoneListItem[i] = item;
                needUpdate = false;
            }
        }

        if (needUpdate)
            m_zoneListItem.push_back(item);
    }
}


void
uiBattleZoneMan::NET_RECV_MSG_CG_ZONE_LIST_NAK( MSG_BASE * pMsg )
{
    /* nothing to do */
}

//----------------------------------------------------------------------------
/**
*/
void
uiBattleZoneMan::Send_LOBBY_KICKPLAYER_SYN(DWORD dwPlayerKey)
{
	if (SafePKBool(PACKET_STATUS_SEND_BAN))
	{
		SetPKBool(PACKET_STATUS_SEND_BAN);

		MSG_CG_ZONE_LOBBY_KICKPLAYER_SYN	SendPacket;
		SendPacket.m_PlayerKey = dwPlayerKey;

		GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));
	}		
}

//----------------------------------------------------------------------------
/**
	@remark
		- [prev]방장을 제외하고 타슬롯은 뻥~ 차버릴수있다.
*/
BOOL 
uiBattleZoneMan::BanPlayerByIndex(int index)
{
	assert(index >= 1 && index < MAX_VIEW_USER_SLOT-1);		

	if(IsMaster())
	{
		// 방장만 내보내기를 한다.
		UserSlot * pUserSlot = GetUserSlotByIndex(index);

		if (!pUserSlot)
		{
			assert(0);
			return FALSE;
		}

		switch (pUserSlot->GetSlotState() )
		{
		case UserSlot::READY:
		case UserSlot::FILL:
			{
				TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};
				TCHAR szTemp[INTERFACE_STRING_LENGTH] ={0,};
				g_dwKickPlayerKey = pUserSlot->GetPlayerKey();

				// %s님을 파티에서 탈퇴시키겠습니까?
				g_InterfaceManager.GetInterfaceString(5215, szTemp, INTERFACE_STRING_LENGTH);
				Object	*pObj = (Object *)g_ObjectManager.GetObject(g_dwKickPlayerKey);

				uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
				if(pSystemMan && pObj)
				{
					Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp, pObj->GetName());
					pSystemMan->Sys_ConfirmLock(szMessage, _Confirm_KickMember);
				}
			}
			break;

		default:
			{
				// 열려있거나 블럭된 슬롯이다.
				return FALSE;
			}
			break;
		}
	}
	else
	{
		assert(!"방장아니면 됐거든");
		return FALSE;
	}

	return TRUE;
}

//----------------------------------------------------------------------------
/**
*/
VOID			
uiBattleZoneMan::LeaveLobby()
{
	// 로비서 나간다.
	if (SafePKBool(PACKET_STATUS_SEND_LEAVE))
	{
		SetPKBool(PACKET_STATUS_SEND_LEAVE);
		//로비 리브 패킷을 날린다.

		MSG_CG_ZONE_LOBBY_LEAVE_SYN	SendPacket;
		GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));

#if defined	( _ZONE_TEST_LOBBY_IN_SINGLE )
		MSG_CG_ZONE_LOBBY_LEAVE_ACK		SendPacketAck;
		g_pApplication->AddVirtualPacket(CI_GAMESERVERIDX, (BYTE *)&SendPacketAck, sizeof(SendPacketAck));
#endif		

		if(m_pUIBattleZoneCreate && m_pUIBattleZoneCreate->IsVisible())
			m_pUIBattleZoneCreate->ShowInterface(FALSE);
	}
}

//----------------------------------------------------------------------------
/**
*/
VOID			
uiBattleZoneMan::ReadyBattle()
{
	if(!g_pHero)
		return;

	if(IsMaster())
	{
		assert(0);
		return;
	}

	UserSlot * pUserSlot = GetUserSlotByKey(g_pHero->GetObjectKey());

	if (!pUserSlot)
	{
		assert(!"슬롯정보가 비워있다!");
		return;
	}

	if (pUserSlot->GetSlotState() == UserSlot::READY)	// 레디 상태면 레디 취소를 한다.
	{
		if (SafePKBool(PACKET_STATUS_SEND_READY_CANCLE))
		{
			SetPKBool(PACKET_STATUS_SEND_READY_CANCLE);
			
			MSG_CG_ZONE_LOBBY_READY_CANCEL_SYN	SendPacket;
			GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));
		}
	}
	else
	{
		// 레디한다.
		if (SafePKBool(PACKET_STATUS_SEND_READY))
		{
			SetPKBool(PACKET_STATUS_SEND_READY);

			// Ready!!
			MSG_CG_ZONE_LOBBY_READY_SYN SendPacket;
			GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));
		}
	}
}


//----------------------------------------------------------------------------
/**
*/
VOID 
uiBattleZoneMan::StartBattle()
{
    if (!IsMaster())
    {
        assert(IsMaster());
        return;
    }

	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};

	if (!IsReadyForStart())
	{
		g_InterfaceManager.GetInterfaceString( 70243, szMessage, INTERFACE_STRING_LENGTH );
		g_InterfaceManager.ShowMessageBox(szMessage, 10000, NULL, 0);
		return;
	}

	const sMAPINFO * pMapInfo = GetBattleRoomEntry().GetCurMapInfo();

	if (!pMapInfo)
	{
		WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_FILE | c_wOUTPUT_LOG_CENTER, _T("Error: pMapInfo is NULL"));
		assert(!"멥정보가 잘못되었다!!");
		return;
	}

	switch (GetBattleRoomEntry().GetType())
	{
    case eZONETYPE_CHUNTING:
        {
			if (SafePKBool(PACKET_STATUS_SEND_CREATE))
            {
				SetPKBool(PACKET_STATUS_SEND_CREATE, 60);

                ADDITIONAL_CHUNTINGINFO info;
#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
                if (m_pUIBattleZoneReadyCHuntingConfig)
                {
                    info.m_Bonus = m_pUIBattleZoneReadyCHuntingConfig->GetCfgs().bonusType;
                    info.m_Difficulty = m_pUIBattleZoneReadyCHuntingConfig->GetCfgs().diffcultType;
                    info.m_Mode = m_pUIBattleZoneReadyCHuntingConfig->GetCfgs().compModeType;
                    info.m_Rule = m_pUIBattleZoneReadyCHuntingConfig->GetCfgs().compRuleType;
                }
#endif//#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)

				GetBattleRoomEntry().SetAdditionalCHuntingInfo(&info);
                BattleScene::SetPendingBattleRoomEntry( &GetBattleRoomEntry() );

				GameStart();                
            }
        }
        break;



	case eZONETYPE_MISSION:
		{
			if (SafePKBool(PACKET_STATUS_SEND_CREATE))
			{

				SetPKBool(PACKET_STATUS_SEND_CREATE, 60);

				BattleScene::SetPendingBattleRoomEntry( &GetBattleRoomEntry() );


				MSG_CG_ZONE_MISSION_CREATE_SYN SendPacket;			//이 패킷의 답변으로 CG_ZONE_MISSION_JOIN_ACK 가 옴.
				GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));

			}
		}
		break;

	case eZONETYPE_PVP:
		{
			if (SafePKBool(PACKET_STATUS_SEND_CREATE))
			{
				
				SetPKBool(PACKET_STATUS_SEND_CREATE, 60);

				BattleScene::SetPendingBattleRoomEntry( &GetBattleRoomEntry() );

				MSG_CG_ZONE_PVP_CREATE_SYN SendPacket;			//이 패킷의 답변으로 CG_ZONE_PVP_JOIN_ACK 가 옴.
				GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));			

				
			}
		}
		break;

	case eZONETYPE_LOBBY:	// 여기가 로비당
	default:
		{
			WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_FILE | c_wOUTPUT_LOG_CENTER, _T("GetBattleRoomEntry().GetType() == %d"), GetBattleRoomEntry().GetType());
			assert(0);
		}
		break;
	}
}

//----------------------------------------------------------------------------
/**
	@remark
		- [prev]방장을 제외하고 타슬롯은 뻥~ 차버릴수있다.
*/
BOOL			
uiBattleZoneMan::IsReadyForStart()
{
	// 방장을 제외한 타유저들이 레디를 걸고있는지 check
	for (int i = 1; i < GetBattleRoomEntry().GetCurLimitMaxUser(); ++i)
	{
		UserSlot * pSlot = GetUserSlotByIndex(i);

		if(!pSlot)
			continue;

		if (pSlot->GetSlotState() == UserSlot::OPENED)
		{
			// 유저가 들어와있지 않을때는 방장의 권한으로 방생성은 할수있다.
			continue;
		}
		else
		if (pSlot->GetSlotState() == UserSlot::BLOCK)
		{
			// 블럭된 슬롯은 논외
			continue;
		}
		else
		if (pSlot->GetSlotState() != UserSlot::READY)
		{
			return FALSE;
		}
	}

	return TRUE;
}

//----------------------------------------------------------------------------
/**
	@Desc : 팀 교체 
*/
VOID			
uiBattleZoneMan::ChangeTeam()
{
	MSG_CG_ZONE_LOBBY_CHANGE_TEAM_SYN SendPacket;

	if (SafePKBool(PACKET_STATUS_SEND_CHANGE_TEAM))
	{	
		if ( g_pHero )
		{
			UserSlot * pUserSlot = GetUserSlotByKey(g_pHero->GetObjectKey());	

			if ( pUserSlot )
			{
				if ( pUserSlot->GetTeam() > 0 )
				{
					m_PendingiTeam = SendPacket.m_Team = (pUserSlot->GetTeam() == 1)? 2 : 1;
					SetPKBool(PACKET_STATUS_SEND_CHANGE_TEAM);
					GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));
				}
				else
				{
					// 팀이 정해지지 않았는데 팀을 바꾸려 들었다.
				}
			}
		}
	}
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
