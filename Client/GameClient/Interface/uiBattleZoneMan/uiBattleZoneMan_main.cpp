//------------------------------------------------------------------------------
//  uiBattleZoneMan_main.cpp
//  (C) 2005 ckbang, y2jinc
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiBattleZoneMan.h"
#include "uiBattleZoneCreate/uiBattleZoneCreate.h"
#include "uiBattleZoneList/uiBattleZoneList.h"
#include "uiBattleZonePassword/uiBattleZonePassword.h"
#include "uiBattleZoneReadyMissionConfig/uiBattleZoneReadyMissionConfig.h"
#include "BattleLobby.h"
#include "MapInfoParser.h"
#include "RequireFieldLevelInfoParser.h"
#include "BattleScene.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "uiSystemMan/uiSystemMan.h"
#include "hero.h"


//------------------------------------------------------------------------------
/**
    sort utility
*/

bool _sort_submission_index_low_to_high(stSubMission & a, stSubMission & b)
{
    return (a.iRequireMinLV  < b.iRequireMinLV);
}


//------------------------------------------------------------------------------
/**
*/
uiBattleZoneMan::uiBattleZoneMan(InterfaceManager *pUIMan) :
    uiBaseMan(pUIMan)
{
    m_pUIBattleZoneCreate = NULL;
    m_pUIBattleZoneList = NULL;
    m_pUIBattleZonePassword = NULL;
#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
    m_pUIBattleZoneReadyMissionConfig = NULL;
#endif // _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES

	m_bWaitInputPassWord = false;
	m_pPrivateZone = NULL;
	m_bSearchMode = false;

	m_RoomSpecialType = eROOM_SPECIAL_NORMAL;
	m_dwMasterPlayerKey = 0;

    ClearCHuntingJoinedFlag();
    ClearHasJoinedFlag();
}

void 
uiBattleZoneMan::OnInitialize()
{
    _LoadUI();

    InitBattleEntries();
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneMan::InitBattleEntries()
{
    this->m_pBattleLobby	 = NULL;

	m_BattleRoomEntry.Init();
	m_vecMissionMapIndex.clear();
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneMan::OnRelease()
{
	ReleaseUserSlotSet();
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneMan::SetBattleLobby(BattleLobby* pLobby)
{
	if(!pLobby)
		return;

    m_pBattleLobby = pLobby;

    // default
    m_byMinSubMission = 1;
    m_byMaxSubMission = 2;

    const sMAPINFO * pMapInfo = MapInfoParser::Instance()->FindMapInfo((MAPCODE)m_BattleRoomEntry.m_MapCode);

    if ( !pMapInfo) 
    {
        assert (pMapInfo);
        return;
    }

    MapGroup * pMapGroup = MapInfoParser::Instance()->GetMapGroup( pMapInfo->MapKind );
    if ( !pMapGroup ) 
    {
        assert (pMapGroup);
        return;
    }

    sMAPINFO_HASH & mapHash = pMapGroup->GetMapInfoHash();
	
    m_byMaxSubMission = (BYTE)mapHash.size();
    if (m_byMaxSubMission>1)
    {
        //서브 미션은 총 갯수에서 제거
        m_byMaxSubMission -= 1;
    }

	// 서브미션 sort
	m_vecMissionMapIndex.clear();
	
	sMAPINFO_HASH::const_iterator itr = mapHash.begin();

	sMAPINFO * pMapSubInfo = NULL;
	BASE_RequireFieldLevelInfo * pInfo = NULL;
	
	int iRequireMinLV = 0;
	stSubMission sb;
	
	while (  itr != mapHash.end() )
	{
		pMapSubInfo  = itr->second;
		itr++;
		if ( pMapSubInfo->byMType > 0 )	// 서브 미션만..
		{
			pInfo = RequireFieldLevelInfoParser::Instance()->GetRequireFieldLevelInfo(
												(WORD)pMapSubInfo->MapCode,
												INVALID_DIFFICULT);

			iRequireMinLV = 0;
			if (pInfo)
			{
				iRequireMinLV = pInfo->m_wMinlv;
			}
			else
			{
				iRequireMinLV = pMapSubInfo->minLV;
			}

			ZeroMemory(&sb, sizeof(sb));
			sb.byIndex = pMapSubInfo->byMType;
			sb.iRequireMinLV = iRequireMinLV;
			m_vecMissionMapIndex.push_back(sb);
			
		}
	}

	std::sort( 
			m_vecMissionMapIndex.begin(), 
			m_vecMissionMapIndex.end(), 
			_sort_submission_index_low_to_high
			);
    
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneMan::ClearPendings()
{
    m_pendingMapCode = 0;
    m_pendingMapChangeIndex = 0;
    m_pendingDifficult = eHUNTING_MONSTER_DIFFICULT_EASY;
    m_pendingBonus = eHUNTING_BONUS_TYPE_ITEM;
    m_pendingCompetitionRule = eCOMPETITION_RULE_DUAL_PARTY;
    m_pendingCompetitionMode = eCOMPETITION_MODE_SURVIVAL;
}

//----------------------------------------------------------------------------
/**
*/
void
uiBattleZoneMan::ModifyMap(int index)
{
	assert (m_pBattleLobby);
	if(!m_pBattleLobby)
		return;

    if (this->SafePKBool(ePKBOOL_BATTLEZONE_CG_ZONE_LOBBY_CHANGE_MAP_SYN))
    {
		const sMAPINFO * pMapInfo = MapInfoParser::Instance()->FindMapInfo( static_cast<MAPCODE>( m_BattleRoomEntry.m_MapCode));

        //assert (pMapInfo);
        if ( !pMapInfo )
        {
            return;
        }

        MapGroup * pMapGroup = MapInfoParser::Instance()->GetMapGroup( pMapInfo->MapKind );
        //assert (pMapGroup);
        if ( !pMapGroup ) 
        {
            return;
        }

        sMAPINFO * pMapInfo2 = pMapGroup->GetMapInfoIndex(index);
        //assert (pMapInfo2);
        if (!pMapInfo2) 
        {
            return;
        }

        if (m_pendingMapCode != pMapInfo2->MapCode)
        {
			m_PendingMapCode = m_pendingMapCode = pMapInfo2->MapCode;
            m_pendingMapChangeIndex = index;

            MSG_CG_ZONE_LOBBY_CHANGE_MAP_SYN sync;
            sync.m_MapCode = pMapInfo2->MapCode;
            GlobalFunc::SendPacketToGameServer( &sync, sizeof(sync) );

#ifdef _DEBUG

			TCHAR szDebug[256] = {0,};
			Snprintf(szDebug, 255, _T("MSG_CG_ZONE_LOBBY_CHANGE_MAP_SYN: %d"), sync.m_MapCode);
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szDebug);


#endif
            this->SetPKBool(ePKBOOL_BATTLEZONE_CG_ZONE_LOBBY_CHANGE_MAP_SYN);
        }
    }
}

//----------------------------------------------------------------------------
/**
*/
void
uiBattleZoneMan::ModifyCHuntingMap(int index)
{
	assert (m_pBattleLobby);
	if(!m_pBattleLobby)
		return;

    if (this->SafePKBool(ePKBOOL_BATTLEZONE_CG_ZONE_LOBBY_CHANGE_MAP_SYN))
    {
		m_PendingMapCode = m_pendingMapCode = (MAPCODE)m_BattleRoomEntry.m_MapCode;

        m_pendingMapChangeIndex = 0;

        MSG_CG_ZONE_LOBBY_CHANGE_MAP_SYN sync;
        sync.m_MapCode = 0;
        GlobalFunc::SendPacketToGameServer( &sync, sizeof(sync) );

        this->SetPKBool(ePKBOOL_BATTLEZONE_CG_ZONE_LOBBY_CHANGE_MAP_SYN);
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
uiBattleZoneMan::IsReadyToModifyMissionCfg()
{
	assert (m_pBattleLobby);
	if(!m_pBattleLobby)
		return false;

    if (!GENERALPARAM->IsTestUIScene())
    {
        // is master?
		if (!IsMaster())
            return false;
    }

    // is ready packet?
    if (!SafePKBool(ePKBOOL_BATTLEZONE_CG_ZONE_LOBBY_CHANGE_MAP_SYN))
        return false;

    return true;
}


//------------------------------------------------------------------------------
/**
    check Map Change Sync
    check Hunting Config Sync
*/
bool
uiBattleZoneMan::IsReadyToModifyCHuntingCfg()
{
	assert (m_pBattleLobby);
	if(!m_pBattleLobby)
		return false;

    if (!GENERALPARAM->IsTestUIScene())
    {
        // is master?
		if (!IsMaster())
            return false;

        if (this->GetCHuntingJoinedFlag())
            return false;
    }

    // is ready packet?
    if (!SafePKBool(ePKBOOL_BATTLEZONE_CG_ZONE_LOBBY_CHUNTING_CONFIG_SYN))
        return false;

    if (!SafePKBool(ePKBOOL_BATTLEZONE_CG_ZONE_LOBBY_CHANGE_MAP_SYN))
        return false;

    return true;
}


//----------------------------------------------------------------------------
/**
    Competition Hunting Configs

    eFiled: field 1 ~ 5
    eDifficult: easy=0, normal=1, hard=2 or higher
    eBonusType: item, exp

*/
void
uiBattleZoneMan::ModifyCHuntingCfg(WORD                         eField,
                                   eHUNTING_MONSTER_DIFFICULT   eDifficult,
                                   eHUNTING_BONUS_TYPE          eBonusType,
                                   eCOMPETITION_RULE_TYPE       eCompetitionRule,
                                   eCOMPETITION_MODE_TYPE       eCompetitionMode)
{
	assert (m_pBattleLobby);
	if(!m_pBattleLobby)
		return;

    // FIXME: 서버에서 패킷을 수정하기 전까지는 모두 EASY
    eDifficult = eHUNTING_MONSTER_DIFFICULT_EASY;

    this->ModifyMap(eField);

    if (this->SafePKBool(ePKBOOL_BATTLEZONE_CG_ZONE_LOBBY_CHUNTING_CONFIG_SYN))
    {
        m_pendingDifficult          = eDifficult;
        m_pendingBonus              = eBonusType;
        m_pendingCompetitionRule    = eCompetitionRule;
        m_pendingCompetitionMode    = eCompetitionMode;

        MSG_CG_ZONE_LOBBY_CHUNTING_CONFIG_SYN sync;
        sync.m_AdditionalInfo.m_Bonus       = m_pendingBonus;
        sync.m_AdditionalInfo.m_Difficulty  = m_pendingDifficult;
        sync.m_AdditionalInfo.m_Mode        = m_pendingCompetitionMode;
        sync.m_AdditionalInfo.m_Rule        = m_pendingCompetitionRule;

        GlobalFunc::SendPacketToGameServer(&sync, sizeof (sync));

        this->SetPKBool(ePKBOOL_BATTLEZONE_CG_ZONE_LOBBY_CHUNTING_CONFIG_SYN);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneMan::ModifyPVPCfg(ePVP_RULE_TYPE eRule, ePVP_MODE_TYPE ePVPMode)
{
#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
	if (SafePKBool(ePKBOOL_BATTLEZONE_CG_ZONE_LOBBY_PVP_CONFIG_SYN))
	{
		//------------------------------------------------
		// 유효성 검사
		// VerifyConfigInfo
		assert( eRule < ePVP_RULE_MAX );
		assert( ePVPMode < ePVP_MODE_MAX );

		const sMAPINFO * pMapInfo = this->m_BattleRoomEntry.GetCurMapInfo();

		if (!pMapInfo)
		{
			assert(!"Invalid Map Info :: BattleLobby");
			return;
		}

		//------------------------------------------------
		MSG_CG_ZONE_LOBBY_PVP_CONFIG_SYN SendPacket;

		SendPacket.m_AdditionalPVPInfo.m_Rule	= (BYTE)eRule;
		SendPacket.m_AdditionalPVPInfo.m_Mode	= (BYTE)ePVPMode;

		memcpy( &this->m_pBattleLobby->m_PendingAdditionalPVPInfo, 
			&SendPacket.m_AdditionalPVPInfo, 
			sizeof(this->m_pBattleLobby->m_PendingAdditionalPVPInfo));

		GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));

		this->SetPKBool(ePKBOOL_BATTLEZONE_CG_ZONE_LOBBY_PVP_CONFIG_SYN);
	}
#endif // _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneMan::ShortCut(BYTE type)
{
    if (GetPKBool(ePKBOOL_BATTLEZONE_LIST_CG_ZONE_SHORTCUT_SYN))
    {
		TCHAR szMessage[INTERFACE_STRING_LENGTH];
		g_InterfaceManager.GetInterfaceString( eST_ALREADY_DIRECT_GOING, szMessage, INTERFACE_STRING_LENGTH );
        GlobalFunc::OutputMsg_Guide(eOUTPUT_MSG_CENTER_SHOW, szMessage);
        return;
    }

#ifdef	_EXCECPT_HUNTING_001011
    switch (type)
    {
    case 0: { this->ShortCut_Mission(); } break;
    case 1: { this->ShortCut_PVP(); } break;
    }
#else
    switch (type)
    {
    case 0: { this->ShortCut_Mission(); } break;
    case 2: { this->ShortCut_CHunting(); } break;
    case 3: { this->ShortCut_PVP(); } break;
    }
#endif
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneMan::ShortCut_Mission()
{
    if (SafePKBool(ePKBOOL_BATTLEZONE_LIST_CG_ZONE_SHORTCUT_SYN))
    {
		TCHAR	szMessage[INTERFACE_STRING_LENGTH];
        MSG_CG_ZONE_SHORTCUT_SYN    sync;
        sync.m_byRoomType = eZONETYPE_MISSION;

        GlobalFunc::SendPacketToGameServer(&sync, sizeof (sync));

        SetPKBool(ePKBOOL_BATTLEZONE_LIST_CG_ZONE_SHORTCUT_SYN, 5);

		g_InterfaceManager.GetInterfaceString(eST_MISSION_DIRECT_GOING,szMessage,INTERFACE_STRING_LENGTH);
        GlobalFunc::OutputMsg_Guide(eOUTPUT_MSG_CHAT, szMessage);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneMan::ShortCut_CHunting()
{
    if (SafePKBool(ePKBOOL_BATTLEZONE_LIST_CG_ZONE_SHORTCUT_SYN))
    {
        MSG_CG_ZONE_SHORTCUT_SYN    sync;
        sync.m_byRoomType = eZONETYPE_CHUNTING;

        GlobalFunc::SendPacketToGameServer(&sync, sizeof (sync));

        SetPKBool(ePKBOOL_BATTLEZONE_LIST_CG_ZONE_SHORTCUT_SYN, 5);

		TCHAR szMessage[INTERFACE_STRING_LENGTH];
        g_InterfaceManager.GetInterfaceString(eST_HUNTING_DIRECT_GOING,szMessage,INTERFACE_STRING_LENGTH);
        GlobalFunc::OutputMsg_Guide(eOUTPUT_MSG_CHAT, szMessage);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneMan::ShortCut_PVP()
{
    if (SafePKBool(ePKBOOL_BATTLEZONE_LIST_CG_ZONE_SHORTCUT_SYN))
    {
        MSG_CG_ZONE_SHORTCUT_SYN    sync;
        sync.m_byRoomType = eZONETYPE_PVP;

        GlobalFunc::SendPacketToGameServer(&sync, sizeof (sync));

        SetPKBool(ePKBOOL_BATTLEZONE_LIST_CG_ZONE_SHORTCUT_SYN, 5);
    
		TCHAR szMessage[INTERFACE_STRING_LENGTH];
		g_InterfaceManager.GetInterfaceString(eST_PVP_DIRECT_GOING,szMessage,INTERFACE_STRING_LENGTH);
        GlobalFunc::OutputMsg_Guide(eOUTPUT_MSG_CHAT,szMessage );
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneMan::SearchID(TCHAR* pSearchID)
{

	MSG_CG_ZONE_FIND_ROOM_FROM_CHARID_SYN sync;

	StrnCopy(sync.m_pszCharName, pSearchID, MAX_CHARNAME_LENGTH);

	GlobalFunc::SendPacketToGameServer( &sync, sizeof(sync) );
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneMan::SearchNo(SHORT no)
{
    for (size_t i=0; i<m_zoneListItem.size(); ++i)
    {
        switch (m_zoneListItem[i].iType)
        {
        case eBATTLE_ZONE_LIST_ITEM_MISSION_LOBBY:
            {
                if (m_zoneListItem[i].m_MissionLobbyRoomInfo.m_Key == no)
                {
                    m_searchedZoneListItem = m_zoneListItem[i];
                    return;
                }
            }
            break;


        case eBATTLE_ZONE_LIST_ITEM_PVP:
            {
                if (m_zoneListItem[i].m_PVPRoomInfo.m_Key == no)
                {
                    m_searchedZoneListItem = m_zoneListItem[i];
                    return;
                }
            }
            break;

        case eBATTLE_ZONE_LIST_ITEM_CHUNTING_LOBBY:
            {
                if (m_zoneListItem[i].m_CHuntingLobbyRoomInfo.m_Key == no)
                {
                    m_searchedZoneListItem = m_zoneListItem[i];
                    return;
                }
            }
            break;


		default:
			{
				assert(!"존재하지 않는 Zone Type 입니다!!!");
			}
			break;
        }
    }

	// 방 번호로 찾기 
	MSG_CG_ZONE_FIND_ROOM_FROM_NO_SYN SendPacket;
	SendPacket.m_RoomNo = no; 
	GlobalFunc::SendPacketToGameServer(&SendPacket, sizeof(SendPacket));
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneMan::ClearZoneListItems()
{
	SetSearchMode(false);

	m_zoneListItem.clear();
    m_renderedZoneListItem.clear();
    m_searchedZoneListItem.iType = -1;  // clear
}

//------------------------------------------------------------------------------
const std::vector<cBattleZoneListItem>& uiBattleZoneMan::GetZoneList(BattleRoomViewType view_type)
{
    m_renderedZoneListItem.clear();

    if (m_searchedZoneListItem.iType != -1)
    {
        m_renderedZoneListItem.push_back(m_searchedZoneListItem);
        return m_renderedZoneListItem;
    }

    if (view_type == eCHKVIEW_POSSIBLE)
    {
        for (size_t i=0; i<m_zoneListItem.size(); ++i)
        {
            if (((m_zoneListItem[i].iType == eBATTLE_ZONE_LIST_ITEM_MISSION_LOBBY) && 
                    (m_zoneListItem[i].m_MissionLobbyRoomInfo.m_RoomInfo.can_join_room_)) || 
                ((m_zoneListItem[i].iType == eBATTLE_ZONE_LIST_ITEM_PVP) && 
                    (m_zoneListItem[i].m_PVPRoomInfo.m_RoomInfo.can_join_room_)) || 
                ((m_zoneListItem[i].iType == eBATTLE_ZONE_LIST_ITEM_CHUNTING_LOBBY) && 
                    (m_zoneListItem[i].m_CHuntingLobbyRoomInfo.m_RoomInfo.can_join_room_)) 
                )
            {
                m_renderedZoneListItem.push_back(m_zoneListItem[i]);
            }
        }

        return m_renderedZoneListItem;
    }

    return m_zoneListItem;
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneMan::RequestZoneList()
{
	ClearZoneListItems();

    NET_SEND_MSG_CG_ZONE_LIST_MISSIONLOBBY_SYN(40);
	NET_SEND_MSG_CG_ZONE_LIST_PVPLOBBY_SYN(40);
    NET_SEND_MSG_CG_ZONE_LIST_CHUNTINGLOBBY_SYN(40);
}

//------------------------------------------------------------------------------
/**
*/
bool
uiBattleZoneMan::RequestJoinPrivateZone(KEYTYPE roomKey)
{
    BYTE byPublic = eZONEPUBLIC_PUBLIC;
	m_pPrivateZone = NULL;

    switch (m_searchedZoneListItem.iType)
    {
    case eBATTLE_ZONE_LIST_ITEM_MISSION_LOBBY:
        {
            if (m_searchedZoneListItem.m_MissionLobbyRoomInfo.m_Key == roomKey)
            {
                byPublic = m_searchedZoneListItem.m_MissionLobbyRoomInfo.m_byRoomPublic;
				m_pPrivateZone = &m_searchedZoneListItem;
                break;
            }
        }
        break;


    case eBATTLE_ZONE_LIST_ITEM_PVP:
        {
            if (m_searchedZoneListItem.m_PVPRoomInfo.m_Key == roomKey)
            {
                byPublic = m_searchedZoneListItem.m_PVPRoomInfo.m_byRoomPublic;
				m_pPrivateZone = &m_searchedZoneListItem;
                break;
            }
        }
        break;

    case eBATTLE_ZONE_LIST_ITEM_CHUNTING_LOBBY:
        {
            if (m_searchedZoneListItem.m_CHuntingLobbyRoomInfo.m_Key == roomKey)
            {
                byPublic = m_searchedZoneListItem.m_CHuntingLobbyRoomInfo.m_byRoomPublic;
				m_pPrivateZone = &m_searchedZoneListItem;
                break;
            }
        }
        break;


	default:
		{
			assert(!"존재하지 않는 Zone Type 입니다!!!");
		}
		break;

    }

	if(byPublic == eZONEPUBLIC_PRIVATE)
	{
		// 비번 입력 창을 show
		if(m_pUIBattleZonePassword)
		{
			ShowPasswordDlg(true);
			return true;
		}
	}

	RequestJoinZoneSearchMode(roomKey);

	return false;
}

//------------------------------------------------------------------------------
/**
*/
void uiBattleZoneMan::RequestJoinZoneSearchMode(KEYTYPE roomKey)
{
	cBattleZoneListItem* pZone = NULL;

    switch (m_searchedZoneListItem.iType)
    {
    case eBATTLE_ZONE_LIST_ITEM_MISSION_LOBBY:
        {
            if (m_searchedZoneListItem.m_MissionLobbyRoomInfo.m_Key == roomKey)
            {
                pZone = &m_searchedZoneListItem;
                break;
            }
        }
        break;


    case eBATTLE_ZONE_LIST_ITEM_PVP:
        {
            if (m_searchedZoneListItem.m_PVPRoomInfo.m_Key == roomKey)
            {
                pZone = &m_searchedZoneListItem;
                break;
            }
        }
        break;

    case eBATTLE_ZONE_LIST_ITEM_CHUNTING_LOBBY:
        {
            if (m_searchedZoneListItem.m_CHuntingLobbyRoomInfo.m_Key == roomKey)
            {
                pZone = &m_searchedZoneListItem;
                break;
            }
        }
        break;


	default:
		{
			assert(!"존재하지 않는 Zone Type 입니다!!!");
		}
		break;

    }

	if (pZone)
        this->RequestJoinZone(pZone);
}

//------------------------------------------------------------------------------
/**
*/
bool
uiBattleZoneMan::RequestJoinZone(KEYTYPE roomKey)
{
    cBattleZoneListItem* pZone = NULL;

	BYTE byPublic = eZONEPUBLIC_PUBLIC;
	m_pPrivateZone = NULL;

    size_t numItems = m_zoneListItem.size();
    for (size_t i=0; i<numItems; ++i)
    {
        switch (m_zoneListItem[i].iType)
        {
        case eBATTLE_ZONE_LIST_ITEM_MISSION_LOBBY:
            {
                if (m_zoneListItem[i].m_MissionLobbyRoomInfo.m_Key == roomKey)
                {
                    pZone = &m_zoneListItem[i];

					byPublic = m_zoneListItem[i].m_MissionLobbyRoomInfo.m_byRoomPublic;
					m_pPrivateZone = &m_zoneListItem[i];
                }
            }
            break;

        case eBATTLE_ZONE_LIST_ITEM_PVP:
            {
                if (m_zoneListItem[i].m_PVPRoomInfo.m_Key == roomKey)
                {
                    pZone = &m_zoneListItem[i];

					byPublic = m_zoneListItem[i].m_PVPRoomInfo.m_byRoomPublic;
					m_pPrivateZone = &m_zoneListItem[i];
                }
            }
            break;

        case eBATTLE_ZONE_LIST_ITEM_CHUNTING_LOBBY:
            {
                if (m_zoneListItem[i].m_CHuntingLobbyRoomInfo.m_Key == roomKey)
                {
                    pZone = &m_zoneListItem[i];

					byPublic = m_zoneListItem[i].m_CHuntingLobbyRoomInfo.m_byRoomPublic;
					m_pPrivateZone = &m_zoneListItem[i];
                }
            }
            break;

		default:
			{
				assert(!"존재하지 않는 Zone Type 입니다!!!");
			}
			break;
        }
    }

    if (pZone)
    {
        if(byPublic == eZONEPUBLIC_PRIVATE)
        {
            assert( m_pUIBattleZonePassword );

            if(m_pUIBattleZonePassword)
            {
                ShowPasswordDlg(true);
                return true;
            }
        }

        this->RequestJoinZone(pZone);

        return true;
    }

    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneMan::RequestJoinZone(cBattleZoneListItem* pZone, util::_tstring pwd/* = _T("")*/)
{
    if (!pZone)
        return;

    ClearCHuntingJoinedFlag();

    BattleRoomEntry entry;
	entry.Init();

	TCHAR szPWD[MAX_ROOMPASSWORD_LENGTH+1] = {0,};

	if(pwd.size())
	{
		Snprintf(szPWD,MAX_ROOMPASSWORD_LENGTH, _T("%s"), pwd.c_str());
	}

    switch (pZone->iType)
    {
    case eBATTLE_ZONE_LIST_ITEM_MISSION_LOBBY:		// 미션 조인
        {
            MSG_CG_ZONE_LOBBY_JOIN_SYN  sync;
            sync.m_LobbyKey = pZone->m_MissionLobbyRoomInfo.m_Key;
            sync.m_byRoomPublic = pZone->m_MissionLobbyRoomInfo.m_byRoomPublic;

			ZeroMemory(sync.m_pszRoomPWD, sizeof(sync.m_pszRoomPWD));
			StrnCopy(sync.m_pszRoomPWD, szPWD, MAX_ROOMPASSWORD_LENGTH);

            GlobalFunc::SendPacketToGameServer(&sync, sizeof (sync));

            entry.SetSerial(sync.m_LobbyKey);
            entry.SetType(eZONETYPE_MISSION);
            entry.SetPassword(sync.m_pszRoomPWD);
            entry.SetPublic((eZONEPUBLIC)sync.m_byRoomPublic);
            entry.SetTitle(pZone->m_MissionLobbyRoomInfo.m_pszRoomTitle);
            entry.SetMinLevel(pZone->m_MissionLobbyRoomInfo.m_RoomInfo.m_MinLV);
            entry.SetMaxLevel(pZone->m_MissionLobbyRoomInfo.m_RoomInfo.m_MaxLV);
            entry.SetMaxPlayer(pZone->m_MissionLobbyRoomInfo.m_MaxLimitUserNum);
            entry.SetMapCode(pZone->m_MissionLobbyRoomInfo.m_MapCode);
			entry.m_RoomInfo.m_SpecialType = pZone->m_MissionLobbyRoomInfo.m_RoomInfo.m_SpecialType;
        }
        break;


	case eBATTLE_ZONE_LIST_ITEM_PVP:
		{
			MSG_CG_ZONE_LOBBY_JOIN_SYN sync;	//pvp 조인
			sync.m_LobbyKey = pZone->m_PVPRoomInfo.m_Key;
			sync.m_byRoomPublic = pZone->m_PVPRoomInfo.m_byRoomPublic;

			ZeroMemory(sync.m_pszRoomPWD, sizeof(sync.m_pszRoomPWD));
			StrnCopy(sync.m_pszRoomPWD, szPWD, MAX_ROOMPASSWORD_LENGTH);

            GlobalFunc::SendPacketToGameServer(&sync, sizeof (sync));

            entry.SetSerial(sync.m_LobbyKey);
            entry.SetType(eZONETYPE_PVP);
            entry.SetPassword(sync.m_pszRoomPWD);
            entry.SetPublic((eZONEPUBLIC)sync.m_byRoomPublic);
            entry.SetTitle(pZone->m_PVPRoomInfo.m_pszRoomTitle);
            entry.SetMinLevel(pZone->m_PVPRoomInfo.m_RoomInfo.m_MinLV);
            entry.SetMaxLevel(pZone->m_PVPRoomInfo.m_RoomInfo.m_MaxLV);
            entry.SetMaxPlayer(pZone->m_PVPRoomInfo.m_MaxLimitUserNum);
            entry.SetMapCode(pZone->m_PVPRoomInfo.m_MapCode);
		}
		break;

    case eBATTLE_ZONE_LIST_ITEM_CHUNTING_LOBBY:		// 헌팅 조인
        {
            MSG_CG_ZONE_LOBBY_JOIN_SYN  sync;
            sync.m_LobbyKey = pZone->m_CHuntingLobbyRoomInfo.m_Key;
            sync.m_byRoomPublic = pZone->m_CHuntingLobbyRoomInfo.m_byRoomPublic;

			ZeroMemory(sync.m_pszRoomPWD, sizeof(sync.m_pszRoomPWD));
			StrnCopy(sync.m_pszRoomPWD, szPWD, MAX_ROOMPASSWORD_LENGTH);

            GlobalFunc::SendPacketToGameServer(&sync, sizeof (sync));

            entry.SetSerial(sync.m_LobbyKey);
            entry.SetType(eZONETYPE_CHUNTING);
            entry.SetPassword(sync.m_pszRoomPWD);
            entry.SetPublic((eZONEPUBLIC)sync.m_byRoomPublic);
            entry.SetTitle(pZone->m_CHuntingLobbyRoomInfo.m_pszRoomTitle);
            entry.SetMinLevel(pZone->m_CHuntingLobbyRoomInfo.m_RoomInfo.m_MinLV);
            entry.SetMaxLevel(pZone->m_CHuntingLobbyRoomInfo.m_RoomInfo.m_MaxLV);
            entry.SetMaxPlayer(pZone->m_CHuntingLobbyRoomInfo.m_MaxLimitUserNum);
            entry.SetMapCode(pZone->m_CHuntingLobbyRoomInfo.m_MapCode);
			entry.m_RoomInfo.m_SpecialType = pZone->m_CHuntingLobbyRoomInfo.m_RoomInfo.m_SpecialType;
        }
        break;
    
    }

    // fill the battleroomentry in global
    BattleScene::SetPendingBattleRoomEntry(&entry);
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneMan::ShowPasswordDlg(bool bShow)
{	
	if(m_pUIBattleZonePassword)
	{
		m_pUIBattleZonePassword->ShowInterface(bShow);
	}
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneMan::RequestJoinPrivateZone(util::_tstring pwd)
{
	RequestJoinZone(m_pPrivateZone, pwd);
}

//------------------------------------------------------------------------------
/**
*/
void
uiBattleZoneMan::GameStart()
{
    MSG_CG_ZONE_CHUNTING_CREATE_SYN SendPacket;     //이 패킷의 답변으로 CG_ZONE_CHUNTING_JOIN_ACK 가 옴.
    GlobalFunc::SendPacketToGameServer(&SendPacket, sizeof(SendPacket));
}

//------------------------------------------------------------------------------
/**
*/

BOOL uiBattleZoneMan::IsSuperRoomMaster()
{
	if( IsMaster() && GetSuperMasterType() > 0 )
		return TRUE;
	else 
		return FALSE;
}

//----------------------------------------------------------------------------
/**
*/
bool			
uiBattleZoneMan::IsMaster() const
{
	if (g_pHero)
	{
		return bool(g_pHero->GetObjectKey() == this->m_dwMasterPlayerKey);
	}

	return false;
}


//----------------------------------------------------------------------------
/** 
*/
void
uiBattleZoneMan::SetReadyUserSlot(DWORD dwObjectKey, bool bReady)
{
	int index = this->m_UserSlotSet.GetUserIndexFromKey(dwObjectKey);
	if (INVALID_USER_SLOT_INDEX == index)
	{
		return;
	}

	if(bReady)
	{
		m_UserSlotSet.SetReadyIndex(index);
	}
	else
	{
		m_UserSlotSet.SetReadyCancleIndex(index);
	}
}

//----------------------------------------------------------------------------
/** 
*/
void
uiBattleZoneMan::SetTeamIndexUserSlot(DWORD dwObjectKey, int iTeam)
{
	if(iTeam < 0)
		iTeam = 0;

	int index = this->m_UserSlotSet.GetUserIndexFromKey(dwObjectKey);
	if (INVALID_USER_SLOT_INDEX == index)
	{
		return;
	}

	m_UserSlotSet.SetTeamIndex(index, iTeam);
}

//----------------------------------------------------------------------------
/** 
*/
void
uiBattleZoneMan::SetTeamUserSlot(DWORD dwPlayerKey, int iTeam)
{
	if(iTeam < 0)
		iTeam = 0;

	UserSlot * pUserSlot = this->m_UserSlotSet.GetUserByKey(dwPlayerKey);
	if ( pUserSlot )
	{
		pUserSlot->SetTeam( iTeam );
	}
	else
	{
		assert(0);
	}
}

//----------------------------------------------------------------------------
/** 
*/
void
uiBattleZoneMan::SetTeamUserSlot(PVP_LOBBY_INFO pvpInfo)
{
	UserSlot * pUserSlot = this->m_UserSlotSet.GetUserByKey(pvpInfo.m_dwPlayerKey);
	if ( pUserSlot )
	{
		pUserSlot->SetTeam(pvpInfo.m_Team);

		if (pvpInfo.m_Ready)
		{
			pUserSlot->SetSlotState(UserSlot::READY);
		}
		else
		{
			pUserSlot->SetSlotState(UserSlot::FILL);
		}
	}
	else
	{
		assert(0);
	}
}


//----------------------------------------------------------------------------
/** 
*/
UserSlot*
uiBattleZoneMan::GetUserSlotByKey(DWORD dwPlayerKey)
{
	UserSlot * pUserSlot = this->m_UserSlotSet.GetUserByKey(dwPlayerKey);
	return pUserSlot;
}

//----------------------------------------------------------------------------
/** 
*/
UserSlot*
uiBattleZoneMan::GetUserSlotByIndex(int iIndex)
{
	UserSlot * pUserSlot = this->m_UserSlotSet.GetUserByIndex(iIndex);
	return pUserSlot;
}

//----------------------------------------------------------------------------
/** 
*/
int			
uiBattleZoneMan::GetMaxUserSlot()
{
	return (int)m_UserSlotSet.GetMaxUser();
}


//----------------------------------------------------------------------------
/** 
*/
int
uiBattleZoneMan::GetNumUser()
{
	return (int)m_UserSlotSet.GetNumUser();
}

//----------------------------------------------------------------------------
/** 
*/
int
uiBattleZoneMan::GetNonBlockUserSlotNum()
{
	return (int)m_UserSlotSet.GetNonBlockNum();
}

//----------------------------------------------------------------------------
/** 
*/
int
uiBattleZoneMan::GetBlockUserSlotNum()
{
	return (int)m_UserSlotSet.GetBlockNum();
}

//----------------------------------------------------------------------------
/** 
*/
void			
uiBattleZoneMan::SetOpenUserSlot(int index)
{
	m_UserSlotSet.SetOpenIndex(index);
}

//----------------------------------------------------------------------------
/** 
*/
void
uiBattleZoneMan::SetBlockUserSlot()
{
	if (this->m_UserSlotSet.GetNonBlockNum() > (DWORD)this->m_iPendingPlayerNumBeforeRecvAck)
	{
		// 블럭
		this->m_UserSlotSet.SetBlockIndex(m_iPendingPlayerIndex);
	}
	else 
	{
		// open
		this->m_UserSlotSet.SetOpenIndex(m_iPendingPlayerIndex);
	}
}

//----------------------------------------------------------------------------
/** 
*/
void
uiBattleZoneMan::InitializeUserSlotSet()
{
	m_UserSlotSet.Initialize(MAX_VIEW_USER_SLOT);
}

//----------------------------------------------------------------------------
/** 
*/
void
uiBattleZoneMan::ReleaseUserSlotSet()
{
	this->m_UserSlotSet.Release();
}

//----------------------------------------------------------------------------
/** 
*/
void
uiBattleZoneMan::FlushUserSlotSet()
{
	this->m_UserSlotSet.FlushUsers();
}

//----------------------------------------------------------------------------
/**
*/
void			
uiBattleZoneMan::SetNonBlockSlot(int iNonBlockSlot)	
{
	// 실제 서버에 남아 있는 NonBlock 개수 - 클라에 있는 NonBlock 개수
	int iDiffPlayerNum = iNonBlockSlot - GetNonBlockUserSlotNum();	

	if (0 == iDiffPlayerNum) 
	{
		return;
	}
	else
	if (iDiffPlayerNum < 0)
	{
		iDiffPlayerNum = abs(iDiffPlayerNum);

		// 줄였다.
		// 유저 인원 제한이 줄게되면
		// 뒤에서 부터 Open 되어있는 슬롯에 대해서 Block을 걸어준다.

		for (int i = GetMaxUserSlot()-1; i >= 1; --i)	
		{
			if (iDiffPlayerNum <= 0)
			{
				break;
			}

			UserSlot * pUserSlot = GetUserSlotByIndex(i);
			if (pUserSlot && (pUserSlot->GetSlotState() == UserSlot::OPENED))
			{
				this->m_UserSlotSet.SetBlockIndex(i);
				--iDiffPlayerNum;
			}
		}				
	}
	else 
	{
		// 늘였다. 
		// 유저 인원 제한이 늘게되면
		// 앞에서 부터 Open 되어있는 슬롯에 대해서 Open 한다.
		for (int i = 1; i < GetMaxUserSlot(); ++i)	
		{
			if (iDiffPlayerNum <= 0)
			{
				break;
			}

			UserSlot * pUserSlot = GetUserSlotByIndex(i);
			if (pUserSlot && (pUserSlot->GetSlotState() == UserSlot::BLOCK))
			{
				--iDiffPlayerNum;

				SetOpenUserSlot(i);
			}
		}
	}
}

//----------------------------------------------------------------------------
/**
*/
void
uiBattleZoneMan::SetDefaultUserSlot()
{
	assert(GetBattleRoomEntry().GetMaxPlayer() > 0);
	if (GetBattleRoomEntry().GetMaxPlayer() <= 0)
	{
		GetBattleRoomEntry().SetMaxPlayer(10);	
	};
}

//----------------------------------------------------------------------------
/** 
*/
BOOL	
uiBattleZoneMan::AddPlayer(UserSlot & EnterUserSlot)
{
	UserSlotSet::eRT_USERSLOT_SET RTCode = this->m_UserSlotSet.AddUser(EnterUserSlot);
	if (UserSlotSet::SUCCESS != RTCode)
	{
		assert(0);
		return FALSE;
	}

	return TRUE;
}

//----------------------------------------------------------------------------
/**
*/
BOOL			
uiBattleZoneMan::LeavePlayer(DWORD dwPlayerKey)
{
	return this->m_UserSlotSet.LeaveUser(dwPlayerKey);	
}

//----------------------------------------------------------------------------
/**
*/
VOID 
uiBattleZoneMan::ModifyPlayerNums(int iPlayerNums)
{
	assert(iPlayerNums <= 10);

	if (iPlayerNums <= 0)
	{
		assert(iPlayerNums > 0);
		return;
	}

	if (SafePKBool(PACKET_STATUS_SEND_PLAYER_NUM))
	{
		SetPKBool(PACKET_STATUS_SEND_PLAYER_NUM);
		MSG_CG_ZONE_LOBBY_NONBLOCKSLOTNUM_SYN	SendPacket;
		m_iPendingPlayerNumBeforeRecvAck = SendPacket.m_NonBlockSlotNum = iPlayerNums;
				
		GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));

#if defined	( _ZONE_TEST_LOBBY_IN_SINGLE )
		MSG_CG_ZONE_LOBBY_NUMBEROFPLAYER_ACK	SendPacketAck;
		g_pApplication->AddVirtualPacket(CI_GAMESERVERIDX, (BYTE *)&SendPacketAck, sizeof(SendPacketAck));
#endif		
	}	
}

BOOL uiBattleZoneMan::BattleZoneListIsVisible()
{ 
    if (m_pUIBattleZoneList != NULL)
    {
        return m_pUIBattleZoneList->IsVisible(); 
    }
    return FALSE;
}
//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------