#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
#include <PacketStruct_AG.h>
#include <PacketStruct_CG.h>
#include <MapInfoParser.h>
#include <MapPartialLoadingParser.h>

#include <AgentParty/AgentPartyManager.h>
#include <AgentParty/AgentParty.h>

#include <Zone/RoomInterface.h>
#include <Zone/ZoneManager.h>
#include <Zone/LobbyRoom.h>
#include <Zone/PortalManager.h>

#include "BattleOpenTimeManager.h"

//_NA_20110630_BATTLE_ZONE_RENEWAL
#include "Party/PartyRoom.h"

RoomInterface::RoomInterface(void)
    : m_pMasterUser(NULL)
{
    SetRoomState(eROOMSTATE_DESTROY);
    m_RelayUserHash.Initialize(MAX_PARTYMEMBER_NUM*2);
}

RoomInterface::~RoomInterface(void)
{
}

void
RoomInterface::ChangePolicy(PolicymentForRoom::POLICY_T newPolicySet)
{
    RoomPolicyment.SetField(newPolicySet);
}

//void
//RoomInterface::SetMapCode(MAPCODE code)
//{
//    m_MapCode = code;
//}

void
RoomInterface::On_create()
{
    m_DestroyTimeout.SetTimer(MAX_DESTROY_TIMEOUT_TIME);
    m_MasterEnterTimeout.SetTimer(MAX_DESTROY_TIMEOUT_TIME);
    SetRoomState(eROOMSTATE_READY);

    m_NumberOfExpectedPlayers = 0; //__NA000000_090622_MOB_ABILITY_CONTROL_CHANGE__
}

void
RoomInterface::On_destroy()
{
    SetRoomState(eROOMSTATE_DESTROY);

    //_NA_20110630_BATTLE_ZONE_RENEWAL 
    AgentPartyManager* const party_manager = AgentPartyManager::Instance();
    const AgentParty* party = 
        static_cast<const AgentParty*>(party_manager->FindParty(GetLinkParty()));
    if (party)
    {
        if (party->GetType() == ePARTY_ROOM)
        {
            party_manager->SetIsMissionStart(GetLinkParty(), false);
        }
    }

    VestUserWithMaster(NULL);

    util::SolarHashTable<User*>& relay_user = GetRelayHash();
    util::SolarHashTable<User*>::iterator it = relay_user.begin(),
                                          end = relay_user.end();
    while (it != end)
    {
        it = relay_user.erase(it);
    };
    m_RelayUserHash.RemoveAll();

    RoomPolicyment.Clear();
}

void
RoomInterface::On_readyuser(User* pUser)
{
    // ������ ���� ����
    RemoveRelayUser(pUser->GetPlayerKey());
    //SetRoomState(eROOMSTATE_READY);
}

void
RoomInterface::On_enteruser(User* pUser)
{
    ZoneInterface::On_enteruser(pUser);

    m_DestroyTimeout.DisableCheckTime();
    if(GetMaster() == pUser)
        m_MasterEnterTimeout.DisableCheckTime();

    SetRoomState(eROOMSTATE_GENERNAL);

    // �濡 ���ö� ��Ƽ���� ó���� �Ѵ�.
    AgentPartyManager::Instance()->EnterZone(pUser, this);
    PortalManager::OnEnteredRoom(pUser, this);
}

void
RoomInterface::On_leaveuser(User* pUser)
{
    AgentPartyManager * const party_manager = AgentPartyManager::Instance();
    //_NA_20110630_BATTLE_ZONE_RENEWAL
    WORD party_key = pUser->GetPartyState().GetPartyKey();
    if (pUser->GetPartyState().IsPartyMember())
    {
        BaseParty* party = const_cast<BaseParty*>(party_manager->FindParty(party_key));
        if (party)
        {
            if (party->GetType() == ePARTY_ROOM)
            {
                if (GetUserNum() == 0)
                {
                    SetRoomState(eROOMSTATE_DESTROY);
                }
            }
            else
            {
                if (GetUserNum() == 0)
                {
                    SetRoomState(eROOMSTATE_DESTROY);
                }
                else if(GetMaster() == pUser)
                {
                    VestUserWithMaster(GetFirstUser());
                }
                
            }
            party_manager->LeaveZone(pUser, this);
        }
        else
        {
            if (GetUserNum() == 0)
            {
                SetRoomState(eROOMSTATE_DESTROY);
            }
            SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Invalid party state : %d", party_key);
        }
    }
    else
    {
        if (GetUserNum() == 0)
        {
            SetRoomState(eROOMSTATE_DESTROY);
        }
    }
}

void
RoomInterface::VestUserWithMaster(User* pExpectedMaster)
{
    if(GetMaster())
        GetMaster()->SetMaster(FALSE);

    if(GetMaster() && pExpectedMaster) 
    {
        // ���� ���� ����
        MSG_AG_ZONE_MASTER_CHANGED_CMD msg;
        msg.m_dwKey        = pExpectedMaster->GetUserKey();
        msg.m_RoomKey    = GetKey();
        SendToLinkedServer(&msg, sizeof(msg));

        MSG_CG_ZONE_MASTER_CHANGED_BRD bmsg;
        bmsg.m_dwMasterPlayerKey = pExpectedMaster->GetPlayerKey();
        SendToAll(&bmsg, sizeof(bmsg));

        // ��Ƽ�嵵 �ٲ۴�. 
        // �κ񿡼� ����, �̼Ƿ����� ���� ���� ������ �ٲ�� �ֱ� ������ �� �κ��� �����Ǳ� ������ �ּ��� ���Ƶд�.
        //PartyManager::Instance()->ChangeMaster(GetMaster()->GetPartyState().GetPartyKey(), GetMaster()->GetZoneKey(), pExpectedMaster->GetUserKey());
    }

    m_pMasterUser = pExpectedMaster;
    
    if(pExpectedMaster) 
    {
        pExpectedMaster->SetReady(FALSE);
        pExpectedMaster->SetMaster(TRUE);
    }
}

BOOL
RoomInterface::Process()
{
    BOOL rt = m_DestroyTimeout.IsExpiredManual(FALSE);
    if(GetRoomState() == eROOMSTATE_DESTROY || rt)
    {
        if(rt)
        {
            SUNLOG(eCRITICAL_LOG, "Room(%u,type:%u,code:%u) Destroy For Timeout", 
                GetKey(), GetType(), GetMapCode());
        }
        return FALSE;
    }

    ;{
        util::SolarHashTable<User*>& relay_user = GetRelayHash();
        util::SolarHashTable<User *>::iterator it = relay_user.begin(),
                                               end = relay_user.end();
        while (it != end)
        {
            User* const user = (*it);
            if (user->IsExpired())
            {
                it = relay_user.erase(it);
                SUNLOG(eCRITICAL_LOG,
                       _T("User(U:%u,C:%u,room:%u,type:%u,code:%u) Expired For Timeout"),
                       user->GetUserGUID(), user->GetSelectedCharGuid(),
                       GetKey(), GetType(), GetMapCode());
                continue;
            };
            ++it;
        };
    };

    if(m_MasterEnterTimeout.IsExpired())
    {
        if(GetUserNum() != 0)
            VestUserWithMaster(GetFirstUser());
    }

    return TRUE;
}

void
RoomInterface::RelayUser(RoomInterface* pPreviousRoom)
{
    const ZONE_USER_LIST& rList = pPreviousRoom->GetUserList();
    FOREACH_CONTAINER(const ZONE_USER_LIST::value_type& rNode, rList, ZONE_USER_LIST)
    {
        User* const pUser = rNode.second;
        pUser->StartTimer(MAX_RELAY_TIMEOUT_TIME);
        AddRelayUser(pUser, pUser->GetPlayerKey());
    }
    //__NA000000_090622_MOB_ABILITY_CONTROL_CHANGE__
    BYTE number_of_players = BYTE(m_RelayUserHash.GetDataNum());
    SetNumberOfExpectedPlayers(number_of_players);
}

RC::eROOM_RESULT
RoomInterface::Migrate(KEYTYPE key, RoomInterface* pPreviousRoom, ServerSession* pServerSession)
{
    SetKey(key);
    SetMapCode(pPreviousRoom->GetMapCode());
    
    const sMAPINFO* pInfo = MapInfoParser::Instance()->FindMapInfo(pPreviousRoom->GetMapCode());
    ASSERT(pInfo);
    if(pPreviousRoom->GetLimitMaxUserNum() > pInfo->byMaxUserNum)
        SetLimitMaxUserNum(pInfo->byMaxUserNum);
    else
        SetLimitMaxUserNum(pPreviousRoom->GetLimitMaxUserNum());

    SetZonePublic(pPreviousRoom->GetZonePublic());
    SetRoomTitle(pPreviousRoom->GetRoomTitle());
    SetRoomPWD(pPreviousRoom->GetRoomPWD());
    SetRoomInfo(pPreviousRoom->GetRoomInfo());

    VestUserWithMaster(pPreviousRoom->GetMaster());
    SetLinkedServer(pServerSession);

    // �ʱ�ȭ
    SetReusedKey(FALSE);
    SetReusedServerSession(FALSE);

    return RC::RC_ROOM_SUCCESS;
}

void
RoomInterface::Create(User* user, KEYTYPE key, MAPCODE map_code, eZONEPUBLIC zone_public, 
                      TCHAR* room_title, TCHAR* room_password, BASE_ROOMINFO& IN base_room_info, 
                      ServerSession* server_session)
{

    SetKey(key);
    SetMapCode(map_code);

    const sMAPINFO* map_info = GetMapInfo();
    if (!FlowControl::FCAssert(map_info))
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Map info is null : %d", map_code);
        return;
    }

    SetLimitMaxUserNum(map_info->byMaxUserNum);
    // ZoneType == 13 && IndunKind == 6 �� ����������,������ �����Ѵ�. (�ִ��ο��� 20)
    if ( (map_info->byMKind != eZONETYPE_INSTANCE && map_info->byMType != INDUN_KIND_CHAOS_ZONE) && map_info->byMaxUserNum > 10)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : User count is over : %d", map_info->byMaxUserNum);
    }

    SetZonePublic(zone_public);
    SetRoomTitle(room_title);
    SetRoomPWD(room_password);
    SetRoomInfo(&base_room_info);

    //_NA_20110630_BATTLE_ZONE_RENEWAL
    if (user)
    {
        WORD party_key = user->GetPartyState().GetPartyKey();
        const BaseParty* party = AgentPartyManager::Instance()->FindParty(party_key);
        if (party)
        {
            if (party->GetType() != ePARTY_ROOM)
            {
                VestUserWithMaster(user);
            }
        }
    }

    SetLinkedServer(server_session);

    // �ʱ�ȭ
    SetReusedKey(FALSE);
    SetReusedServerSession(FALSE);
}

//{__NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__
RC::eROOM_RESULT
RoomInterface::MigratePortalUserFirstEnter(KEYTYPE key,
                                           const RoomArgumentForRoomSetup& rRoomArgument,
                                           User* pUser, ServerSession* pServerSession)
{
    // (WAVERIX) (090217) (NOTE) �ڵ� �ߺ��ǰ� �ִ�. ���� ��ų �ʿ� �ִ�.
    SetKey(key);
    const sMAPINFO* pMapInfo = rRoomArgument.m_pMapInfo;
    SetMapCode(pMapInfo->MapCode);
    SetLimitMaxUserNum(pMapInfo->byMaxUserNum);
    SetZonePublic(rRoomArgument.RoomExportLevel);
    SetRoomTitle("portal");
    SetRoomPWD(rRoomArgument.Password);
    SetRoomInfo(const_cast<BASE_ROOMINFO*>(&rRoomArgument.m_RoomInfo));

    VestUserWithMaster(pUser);
    SetLinkedServer(pServerSession);

    // �ʱ�ȭ
    SetReusedKey(FALSE);
    SetReusedServerSession(FALSE);

    return RC::RC_ROOM_SUCCESS;
}
//}

RC::eROOM_RESULT
RoomInterface::CanCreate(User* pUser, MAPCODE mapCode, eZONETYPE eZoneType, eZONEPUBLIC ePublic,
                         BASE_ROOMINFO& IN baseRoomInfo, BOOL needCheckOpenTime)
{
    if(GetRoomState() != eROOMSTATE_DESTROY)
        return RC::RC_ROOM_INVALIDPREVLOBBY;

    const sMAPINFO* pInfo = MapInfoParser::Instance()->FindMapInfo(mapCode);
    if(pInfo == 0)
        return RC::RC_ROOM_INVALID_MAPCODE;
    if(pInfo->GetZoneType4SharedCode() != eZoneType)
        return RC::RC_ROOM_INVALID_MAPCODE;

    MAP_PARTIAL_LOADING* pLoadingInfo = 0;
    {
        pLoadingInfo = MapPartialLoadingParser::Instance()->FindMapInfo(pInfo->MapKind);
        if(pLoadingInfo && !pLoadingInfo->m_bLoading)
            return RC::RC_ROOM_INVALID_MAPCODE;
    };

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
    // PC������ üũ
    if(baseRoomInfo.IsSpecialType(eROOM_SPECIAL_PCBANG))
    {
        if(!pUser->CanApplyPCBangBenefit(TRUE))
            return RC::RC_ROOM_ISNOT_PCBANG_USER;
    }
    // ���۹��� �������� �����ϰ� �ִ��� üũ
    if(baseRoomInfo.IsSpecialType(eROOM_SPECIAL_ITEM))
    {
        if(!pUser->GetItemOwnFlag().FindItemInfo(eITEMTYPE_SUPERMASTER))
            return RC::RC_ROOM_NOT_HAVE_SUPER_MASTER_ITEM;
    }
#else
    // PC������ üũ
    if(baseRoomInfo.m_SpecialType == eROOM_SPECIAL_PCBANG_SUPER_ROOM)
    {
        if(!pUser->CanApplyPCBangBenefit(TRUE))
            return RC::RC_ROOM_ISNOT_PCBANG_USER;
    }
    // ���۹��� �������� �����ϰ� �ִ��� üũ
    else if(baseRoomInfo.m_SpecialType == eROOM_SPECIAL_ITEM1_SUPER_ROOM)
    {
        if(!pUser->GetItemOwnFlag().FindItemInfo(eITEMTYPE_SUPERMASTER))
            return RC::RC_ROOM_NOT_HAVE_SUPER_MASTER_ITEM;
    }
#endif

    // (WAVERIX) (090522) (CHANGES)
    // ����� �� �������� ��� �Դ��� üũ
    BOOLEAN isValidLimitLevel = baseRoomInfo.m_MinLV <= baseRoomInfo.m_MaxLV &&
        pInfo->minLV <= baseRoomInfo.m_MinLV && baseRoomInfo.m_MaxLV <= pInfo->maxLV;

    if(isValidLimitLevel == 0)
        return RC::RC_ROOM_INVALID_LIMITLEVEL;

    LEVELTYPE level = pUser->GetSelectedCharLV();
    isValidLimitLevel = baseRoomInfo.m_MinLV <= level &&
                                                level <= baseRoomInfo.m_MaxLV;
    if(isValidLimitLevel == 0)
        return RC::RC_ROOM_INVALID_LIMITLEVEL;

    // ����� �� Ŭ���������� �̷�� ������ üũ
    for(int i = 0; i < eCHAR_TYPE_MAX - 1; ++i)
    {
        if((pInfo->byClass & (eCLASS_BERSERKER << i)) == 0)
            if(baseRoomInfo.m_ClassLimit & (eCLASS_BERSERKER << i))
                return RC::RC_ROOM_INVALID_LIMITCLASS;
    }

    // ����� �ð� üũ�� �Ѵ�
    if(needCheckOpenTime && pInfo->wTimeLimit != 0)
    {
        if(!(g_BattleOpenTimeMng.IsOpenDungeon(pInfo->wTimeLimit)))
            return RC::RC_ROOM_NOT_OPEN_TIME;
    }

    return RC::RC_ROOM_SUCCESS;
}

RC::eROOM_RESULT
RoomInterface::IsRecommendRoom(User* pUser)
{
    if(GetRoomState() != eROOMSTATE_GENERNAL)
        return RC::RC_ROOM_INVALIDPREVLOBBY;

    RC::eROOM_RESULT rt = CanJoin(pUser, eZONEPUBLIC_PUBLIC, NULL);
    if(RC::RC_ROOM_SUCCESS == rt)
    {
        if(RecommendLevel(pUser->GetSelectedCharLV()))
            return RC::RC_ROOM_SUCCESS_FOR_RECOMMEND_JOIN;
        else
            return RC::RC_ROOM_SUCCESS_FOR_GENERAL_JOIN;
    }

    return rt;
}

BOOL
RoomInterface::CheckLimitUser(User* pUser)
{
    // �ִ��ο� ����
#ifdef _NA_008334_20150608_SONNENSCHEIN
    // �ִ��ο��� 0���̸� �ο����Ѿ���
    if (GetLimitMaxUserNum() == 0) {
        return TRUE;
    }
    else if(GetCurUserNum() >= GetLimitMaxUserNum()) {
        return FALSE;
    }
#else
    if(GetCurUserNum() >= GetLimitMaxUserNum())
        return FALSE;
#endif //_NA_008334_20150608_SONNENSCHEIN
    

    // (WAVERIX) (090522) (NOTE) �ִ� �ο� ���� �˻�� ���⼭ �������� ����. ���� ������ ���迡
    //���ϸ�, �̰� ���� ȣ���ϴ� ���� �� ������ ���� �޶��� �� �ֱ� �����̴�.

    return TRUE;
}


RC::eROOM_RESULT
RoomInterface::_CanJoinAll(User* pUser, eZONEPUBLIC roomPublic, TCHAR* pszRoomPWD)
{
    if(GetRoomState() != eROOMSTATE_GENERNAL)
        return RC::RC_ROOM_INVALIDPREVLOBBY;

    if(NULL == FindRelayUser(pUser->GetPlayerKey()))
    {
        if(!CheckLimitUser(pUser))
            return RC::RC_ROOM_FULL_ROOM;
    }

    if(GetZonePublic() != roomPublic)
        return RC::RC_ROOM_INVALID_ROOM_TYPE;

    if(roomPublic == eZONEPUBLIC_PRIVATE)
    {
        if(0 != _tcsncmp(m_pszRoomPWD, pszRoomPWD, MAX_ROOMPASSWORD_LENGTH))
            return RC::RC_ROOM_NOTEQUAL_PASSWORD;
    }

    if(!ValidCharClass((eCHAR_TYPE)pUser->GetSelectedCharClass()))
        return RC::RC_ROOM_INVALID_LIMITCLASS_FOR_ME;

    if(!ValidLevel(pUser->GetSelectedCharLV()))
        return RC::RC_ROOM_INVALID_LIMITLEVEL_FOR_ME;

#if defined(__NA_000000_20070528_TAIWAN_GUILD_PVP) && defined(__TW_0_20080521_GUILD_PVP_BUG_FIX) // ������� ���÷뿡�� ������ ������ �����Ƿ� ��ü�� üũ����
    if(IsSpecialType(eROOM_SPECIAL_GUILD_PVP))
    {    
        RC::eROOM_RESULT rcResult = CanJoinGuildPVP(pUser);
        if(rcResult != RC::RC_ROOM_SUCCESS)
            return rcResult;
    }
#endif
    return RC::RC_ROOM_SUCCESS;
}

BOOL RoomInterface::CanShowAtRoomList(User* user, eZONEPUBLIC room_type, TCHAR* room_password)
{
    //_NA_000038_20100316_VIEW_TO_JOINABLE_ROOM_IN_LOBBY
    // (CHANGES)(20100318)(hukim)(REQ_0038) ���⼭ �濡 ���� ���� ���θ� �����Ѵ�.
    // �ٸ� Ÿ���� ���� �߰� �Ǵ��� ���⼭ �������� �˻��Ͽ� �����Ѵ�.
    // ������ �ּ�ȭ �ϱ� ���� ���� ���� �÷ο�� �÷ο� ��ü�� �����ϰ� �߰� ������
    // ���ϴ� �������� �����ߴ�.

    CheckMoveZonePolicy* const check_move_zone_policy = 
        MoveZonePolicy<MZTF_VILLAGE_TO_LOBBY>::AlignInfo(
            user, eZONETYPE_VILLAGE, eZONETYPE_LOBBY, user->GetZoneKey(), GetKey());
    
    const RoomArgument room_argument(room_type, ((room_type == eZONEPUBLIC_PRIVATE) ? room_password : NULL));
    m_RoomInfo.can_join_room_ = 
        (g_ZoneManager.IsMoveableZone(check_move_zone_policy, room_argument) == RC::RC_ROOM_SUCCESS);
    
    return TRUE; // by __NA_0_20080328_ROOMLIST_ALL_VIEW
}

static BOOL
CanJoinZoneType(const sMAPINFO* pMapInfo, eZONETYPE enteringZoneType, MAPCODE enteringMapCode)
{
    const sMAPINFO* pEnterMapInfo = MapInfoParser::Instance()->FindMapInfo(enteringMapCode);

    switch (enteringZoneType)
    {
    case eZONETYPE_MISSION:
        for (WORD i = 0 ; i < sMAPINFO::MAX_MISSION_MAP_NUM_V1_3; ++i) 
        {
            if (pMapInfo->byMissionMapVec[i] == pEnterMapInfo->MapKind) {
                return true;
            };
        };
        break;
    case eZONETYPE_PVP:
        for (WORD i = 0 ; i < sMAPINFO::MAX_PVP_MAP_NUM_V1_3; ++i) 
        {
            if (pMapInfo->wPVPMapVec[i] == pEnterMapInfo->MapKind) {
                return true;
            };
        };
        break;
    case eZONETYPE_CHUNTING:
        for (WORD i = 0 ; i < sMAPINFO::MAX_CHUNTING_MAP_NUM_V1_3; ++i) 
        {
            if (pMapInfo->wCHuntingMapVec[i] == pEnterMapInfo->MapKind) {
                return true;
            };
        };
        // TODO: warnings, custom code returns success if request information can't find.
        break;
    case eZONETYPE_INSTANCE:
        return true;
    case eZONETYPE_HUNTING:
    default:
        SUNLOG(eFULL_LOG, "ValidJoinMap:��� �� �� ���� ����");
        return false;
    }
    return false;
}

BOOL
RoomInterface::ValidJoinMap(User* pUser)
{
    // (WAVERIX) (090521) (CHANGES) �˻� ���� ����
    // (NOTE) �ش� �Լ��� ȣ��Ǵ� ���� �̰� ���� ������ ��ģ ���̴�. ���� ���º���
    // Ÿ�� ���������� �ذ��� �� �ִ�.
    eZONE_STATE userZoneState = pUser->GetZoneState();
    MAPCODE userMapCode = pUser->GetSelectedCharMapCode();
    BOOLEAN isAcceptable = userZoneState == ePRS_AT_VILLAGE;
    if(isAcceptable == 0)
    {
        isAcceptable = userZoneState == ePRS_AT_FIELD &&
                       userMapCode == HardCode::MAPCODE_for_TUTORIAL_FIELD;
    }

    if(isAcceptable == 0)
        return TRUE;

    const sMAPINFO* pFromMapInfo = MapInfoParser::Instance()->FindMapInfo(userMapCode);

    eZONETYPE toZoneType = GetType();
    MAPCODE toMapCode = GetMapCode();

    if(toZoneType == eZONETYPE_LOBBY)
        toZoneType = static_cast<LobbyRoom*>(this)->GetLobbyType();

    return CanJoinZoneType(pFromMapInfo, toZoneType, toMapCode);
}



BOOL RoomInterface::ValidCharClass(eCHAR_TYPE type)
{
    switch(type)
    {
    case eCHAR_BERSERKER:
        {
            if(!(m_RoomInfo.m_ClassLimit & eCLASS_BERSERKER))
                return FALSE;
        }
        break;
    case eCHAR_DRAGON:        
        {
            if(!(m_RoomInfo.m_ClassLimit & eCLASS_DRAGON))
                return FALSE;
        }
        break;
    case eCHAR_SHADOW:    
        {
            if(!(m_RoomInfo.m_ClassLimit & eCLASS_SHADOW))
                return FALSE;
        }
        break;
    case eCHAR_VALKYRIE:    
        {
            if(!(m_RoomInfo.m_ClassLimit & eCLASS_VALKYRIE))
                return FALSE;
        }
        break;
    case eCHAR_ELEMENTALIST:
        {
            if(!(m_RoomInfo.m_ClassLimit & eCLASS_ELEMENTALIST))
                return FALSE;
        }
        break;
    case eCHAR_MYSTIC: // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
        {
            if (!(m_RoomInfo.m_ClassLimit & eCLASS_MYSTIC))
                return FALSE;
        }
        break;
    case eCHAR_HELLROID: //_NA_000000_20130812_NEW_CHARACTER_HELLROID
        {
            if (!(m_RoomInfo.m_ClassLimit & eCLASS_HELLROID))
                return FALSE;
        }
        break;
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    case eCHAR_WITCHBLADE:
        {
            if (!(m_RoomInfo.m_ClassLimit & eCLASS_WITCHBLADE))
                return FALSE;
        }
        break;
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

    }
    return TRUE;
}

BOOL
RoomInterface::ValidLevel(LEVELTYPE level)
{
    if(level < m_RoomInfo.m_MinLV || m_RoomInfo.m_MaxLV < level)
    {
        return FALSE;
    }
    return TRUE;
}

LEVELTYPE
RoomInterface::GetAvgLevel()
{
    const ZONE_USER_LIST& rList = GetUserList();
    if(rList.empty())
        return 0;
    //  (WAVERIX) (080726) (NOTE) ��������... interface�� ��Ī�Ǵ� ������� ���� �򰡴� �� �Լ�
    // ���ο��� �Ǵ��ϱ� �����. �̷� �͵��� �۾��ڿ� ���ؼ� ��������, �����ڵ��� ���߿�����
    // �ʿ��ؼ� Ȯ���ϴ� ���°� �ǰ� �Ѵ�. ���� ���� �������� ����� ������?
    DWORD sumV = 0;
    FOREACH_CONTAINER(const ZONE_USER_LIST::value_type& rNode, rList, ZONE_USER_LIST)
    {
        User* const pUser = rNode.second;
        sumV += pUser->GetSelectedCharLV();
        // ����, �ջ��� �����÷� ������ �ʰ���...? 65536/130 = 504...
        // ���� ������ �Ѱ�ġ + RoomInterface(...�ִ� 100?) ... �� ���ɼ��� ����ϳ�...
    }

    const DWORD userCount   = DWORD(rList.size());
    const LEVELTYPE average = LEVELTYPE(sumV / userCount);

    return average;
}

void
RoomInterface::GetCHuntingScore(BYTE& byAvgWinPercent, DWORD& totalCount)
{
    const ZONE_USER_LIST& rList = GetUserList();
    if(rList.empty())
        return;

    DWORD totalCountCache = 0;
    FLOAT averageByWinPercent = .0f;
    FOREACH_CONTAINER(const ZONE_USER_LIST::value_type& rNode, rList, ZONE_USER_LIST)
    {
        User* const pUser = rNode.second;
        const DWORD cHuntingTotalCount = pUser->GetSelectedCharCHuntingTotCount();
        if(!cHuntingTotalCount)
            continue;

        const FLOAT cHuntingVCount = (FLOAT)pUser->GetSelectedCharCHuntingVCount();
        averageByWinPercent += cHuntingVCount / cHuntingTotalCount;
        totalCountCache += cHuntingTotalCount;
    }

    const DWORD userCount = (DWORD)rList.size();
    averageByWinPercent = (averageByWinPercent * 100 / userCount);

    ASSERT(averageByWinPercent <= 100.f);    // add-in (WAVERIX)(080726)
    byAvgWinPercent = (BYTE)averageByWinPercent;
    totalCount = totalCountCache;
}

User*
RoomInterface::GetNextPriorityMaster(RoomInterface* pRoom, DWORD curMasterKey,
                                     eROOM_SPECIAL_TYPE* OUT pRoomSpecialTypeByUser)
{   // �ڵ� ������ �ؾ� �ϴ� ��� ������ ���� ��Ƽ���� ã�´�.
    // ���۹� ������ ����Ǵ� ��Ʋ��, �κ����� PC�� ������ 1����, 2���� ���۹��� Item������,
    // �׵� �ƴϸ� �Ϲ� ����...
    const DWORD ApplyRoom = (1 << eZONETYPE_MISSION) | (1 << eZONETYPE_INSTANCE);

    *pRoomSpecialTypeByUser = eROOM_SPECIAL_NORMAL;

    BOOLEAN bCanApply = FALSE;
    eZONETYPE zoneType = pRoom->GetType();
    if(zoneType == eZONETYPE_LOBBY)
    {
        LobbyRoom* pLobbyRoom = static_cast<LobbyRoom*>(pRoom);
        zoneType = pLobbyRoom->GetLobbyType();
    }

    bCanApply = !!(ApplyRoom & (1 << zoneType));

    if(bCanApply == 0)
    {
        return 0; //
    }

    const ZONE_USER_LIST& rList = pRoom->GetUserList();
    if(rList.empty())
        return 0;

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    User* const current_master_user = FindUser(curMasterKey);
    if (current_master_user == NULL)
    {
        return 0;
    }
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

    User* pBestUser = 0;
    eROOM_SPECIAL_TYPE bBestUserGrade = eROOM_SPECIAL_TYPE(0);
    FOREACH_CONTAINER(const ZONE_USER_LIST::value_type& rNode, rList, ZONE_USER_LIST)
    {
        User* pUser = rNode.second;
        DWORD playerKey = pUser->GetPlayerKey();
        if(playerKey == curMasterKey)
            continue;
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
        // ���忡���� �������� 2���� ��Ƽ�� �����ϹǷ� ��ƼŰ�� �����ؼ� ���� ��Ƽ���� ã�´�. 
        if (current_master_user->GetPartyState().GetPartyKey() != pUser->GetPartyState().GetPartyKey())
            continue;
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

        if(pUser->CanApplyPCBangBenefit(TRUE))
        {
            pBestUser = pUser;
            bBestUserGrade = eROOM_SPECIAL_PCBANG_SUPER_ROOM;
        #ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
            if(pUser->GetItemOwnFlag().FindItemInfo(eITEMTYPE_SUPERMASTER))
                bBestUserGrade = eROOM_SPECIAL_ITEM1_SUPER_ROOM;
        #endif
            break;
        }

        if(pUser->GetItemOwnFlag().FindItemInfo(eITEMTYPE_SUPERMASTER))
        {
            if(bBestUserGrade == 0)
                pBestUser = pUser;
            bBestUserGrade = eROOM_SPECIAL_ITEM1_SUPER_ROOM;
        }

        if(bBestUserGrade != eROOM_SPECIAL_ITEM1_SUPER_ROOM)
            pBestUser = pUser;
    }

    *pRoomSpecialTypeByUser = bBestUserGrade;

    return pBestUser;
}

User*
RoomInterface::ChangeSpecialType(BOOLEAN isEnterEvent, User* pMaster)
{   // assert "BattleZone Method"
    BASE_ROOMINFO* pBaseRoomInfo = GetRoomInfo();

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
    BOOLEAN isApplySuper = !!pBaseRoomInfo->IsSpecialType(eROOM_SPECIAL_SUPER);
    BOOLEAN isSuperRoomPc = !!pBaseRoomInfo->IsSpecialType(eROOM_SPECIAL_PCBANG_SUPER_ROOM);
    BOOLEAN isSuperRoomItem = !!pBaseRoomInfo->IsSpecialType(eROOM_SPECIAL_ITEM1_SUPER_ROOM);
#else
    BOOLEAN isApplySuper = pBaseRoomInfo->m_SpecialType != eROOM_SPECIAL_NORMAL;
    BOOLEAN isSuperRoomPc = pBaseRoomInfo->m_SpecialType == eROOM_SPECIAL_PCBANG_SUPER_ROOM;
    BOOLEAN isSuperRoomItem = pBaseRoomInfo->m_SpecialType == eROOM_SPECIAL_ITEM1_SUPER_ROOM;
#endif

    eROOM_SPECIAL_TYPE specialType = eROOM_SPECIAL_TYPE(0);
    User* pPoleUser = 0;
    if(isEnterEvent)
    {   // assert(isEnterEvent && pMaster && IsMaster(pMaster))
        // assert(called by Zone->EnterUser)
        //if(!FlowControl::FCAssert(isApplySuper == 0))
        //    return;

        pPoleUser = pMaster;
        // ���߿� ������ ����(��Ƽ��)�� PC�� �����̰ų� ���۹��� ������ �������� ���
        isSuperRoomPc = !!pPoleUser->CanApplyPCBangBenefit(TRUE);
        isSuperRoomItem = !!pPoleUser->GetItemOwnFlag().FindItemInfo(eITEMTYPE_SUPERMASTER);
        isApplySuper = isSuperRoomPc || isSuperRoomItem;

        if(isApplySuper != 0)
        {
        #ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
            if(isSuperRoomPc)
                specialType = eROOM_SPECIAL_PCBANG_SUPER_ROOM;
            if(isSuperRoomItem)
                specialType = eROOM_SPECIAL_ITEM1_SUPER_ROOM;
        #else
            specialType = isSuperRoomPc ? eROOM_SPECIAL_PCBANG_SUPER_ROOM
                                        : eROOM_SPECIAL_ITEM1_SUPER_ROOM;
        #endif
        }
        pBaseRoomInfo->m_SpecialType = specialType != 0 ? specialType : eROOM_SPECIAL_NORMAL;
    }
    else
    {
        DWORD currentMasterKey = pMaster ? pMaster->GetPlayerKey() : 0;
        pPoleUser = GetNextPriorityMaster(this, currentMasterKey, &specialType);
        if(pPoleUser == 0)
            return 0;

        pBaseRoomInfo->m_SpecialType = specialType;

    //#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
    //    if(isSuperRoomPc)
    //        pBaseRoomInfo->m_SpecialType &= ~eROOM_SPECIAL_PCBANG_SUPER_ROOM;
    //    if(isSuperRoomItem)
    //        pBaseRoomInfo->m_SpecialType &= ~eROOM_SPECIAL_ITEM1_SUPER_ROOM;
    //#else
    //    pBaseRoomInfo->m_SpecialType = eROOM_SPECIAL_NORMAL;
    //#endif
    }

    // changed! - assert(pPoleUser)
    {
        MSG_AG_ZONE_CHANGE_SPECIALTYPE_CMD msg;
        msg.m_RoomKey = GetKey();
        msg.m_SpecialType = pBaseRoomInfo->m_SpecialType;
        pPoleUser->SendToLinkedServer(&msg, sizeof(msg));
    };
    {
        MSG_CG_ZONE_CHANGE_SPECIALTYPE_CMD msg;
        msg.m_SpecialType = pBaseRoomInfo->m_SpecialType;
        SendToAll(&msg, sizeof(msg));
    };

    return pPoleUser;
    //////////////////////////////////////////////////////////////////////////
}

#if defined(__NA_000000_20070528_TAIWAN_GUILD_PVP) && defined(__TW_0_20080521_GUILD_PVP_BUG_FIX)    //�븸 ����� üũ�� ���÷뿡�� ������ ������ �����Ƿ�.
RC::eROOM_RESULT
RoomInterface::CanJoinGuildPVP(User* pUser)
{
    GUILDGUID guildGuid = pUser->GetSelectedCharGuildGuid();
    // ��� �Ҽ� üũ
    if(guildGuid == 0)
        return RC::RC_ROOM_CANNOT_CREATE_ROOM_FOR_NO_GUILD;

    // ����� ���� ������� üũ
    if(User* pMaster = GetMaster())
    {
        if(pMaster->GetSelectedCharGuildGuid() != guildGuid)
            return RC::RC_ROOM_CANNOT_JOIN_ROOM_FOR_DIFF_GUILD;
    }

    // ����� ���� üũ
    const sMAPINFO* pInfo = MapInfoParser::Instance()->FindMapInfo(GetMapCode());
    if(pUser->GetItemOwnFlag().GetItemNum(eITEMTYPE_GUILD_PVP_ENTRANCE) < pInfo->EntranceItemCount)
        return RC::RC_ROOM_SHORT_ENTRANCE_CARD_OF_GUILD_PVP;

    return RC::RC_ROOM_SUCCESS;
}
#endif


bool 
RoomInterface::ExceptionProcessByLobbyMember(const MSG_AG_ZONE_CAN_CREATE_ROOM_ACK* check_msg,
                                             User* request_user)
{
    typedef MSG_AG_ZONE_CAN_CREATE_ROOM_ACK Packet;
    const Packet::CheckArgMembers& check_member = check_msg->m_CheckArgMembers;

    const Packet::MemberInfo* it_src = check_member.members;
    const Packet::MemberInfo* it_end = &check_member.members[_countof(check_member.members)];
    for(; it_src != it_end; ++it_src)
    {
        if(it_src->objectKey == 0)
            break;
    }
    
    it_end = it_src;
    it_src = check_member.members;

    Packet::CheckArgMembers member_result;
    ZeroMemory(&member_result, sizeof(member_result));
    Packet::MemberInfo* it_dest = member_result.members;
    const ZoneInterface::ZONE_USER_LIST& user_list = GetUserList();

    DWORD search_number = 0, result_number = 0, valid_number = 0;
    BOOLEAN requester_notified = FALSE;

    MSG_CG_ZONE_MISSION_CREATE_NAK nak_msg;

    for (; it_src != it_end; ++it_src, ++it_dest)
    {
        it_dest->objectKey = it_src->objectKey;
        ZoneInterface::ZONE_USER_LIST::const_iterator find_it = \
            user_list.find(it_src->objectKey);
        User* user = (find_it != user_list.end()) ? find_it->second : 0;
        if (user == 0)
        {
            it_dest->status = check_msg->eStatus_PartialFail;
        }
        else
        {
            it_dest->status = it_src->status;
            ++result_number;
        }
        ++search_number;

        if (it_dest->status == check_msg->eStatus_PartialOk)
        {
            if(user->IsBeginTransaction())
                it_dest->status = check_msg->eStatus_Transaction;
            else
                ++valid_number;
        }

        if (it_dest->status != check_msg->eStatus_PartialOk)
        {
            switch (it_dest->status)
            {
            case Packet::eStatus_PartialFail:
                nak_msg.m_dwErrorCode = RC::RC_ROOM_INVALID_PLAYER_STATE;
                break;
            case Packet::eStatus_Transaction:
                nak_msg.m_dwErrorCode = RC::RC_ROOM_ALREADY_DOING_TRANSACTION;
                break;
            case Packet::eStatus_LevelViolation:
                nak_msg.m_dwErrorCode = RC::RC_ROOM_INVALID_LIMITLEVEL_FOR_ME;
                break;
            case Packet::eStatus_RequireQuestClear:
                nak_msg.m_dwErrorCode = RC::RC_ROOM_PREVIOUS_QUEST_CLEAR;
                break;
            case Packet::eStatus_RequireMissionClear:
                nak_msg.m_dwErrorCode = RC::RC_ROOM_NEED2_CLEAR_PRE_MISSION;
                break;
            }
            if (user)
            {
                //���������� ������ ������.
                requester_notified = user == request_user;
                user->SendPacket(&nak_msg, sizeof(nak_msg));
            }
        }
    }

    if (search_number == 0 || search_number != result_number || result_number != valid_number)
    {   
        if (requester_notified == FALSE)
        {
            nak_msg.m_dwErrorCode = RC::RC_ROOM_CANNOT_JOIN_SOMEONE_PARTY_MEMBER;
            request_user->SendPacket(&nak_msg, sizeof(nak_msg));
        }
        return false;
    }

    return true;
}
