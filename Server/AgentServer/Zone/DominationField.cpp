#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
#include <PacketStruct_AG.h>
#include ".\DominationField.h"
#include <AgentParty/AgentPartyManager.h>
#include "MapInfoParser.h"
#include "DominationInfoParser.h"
#include <Zone/ZoneManager.h>
#include <SolarDateTime.h>

BOOL DominationField::is_gm_observer = TRUE;

_IMPL_POOL(DominationField)
DominationField::DominationField(void)
: bReCreate_(FALSE)
, curEventState_(DOMINATION_EVENT_MAX)    // ��ȿ���� ���� ��
{
    guild_count_[offense_guild] = std::make_pair(INVALID_GUILDGUID,0);
    guild_count_[defense_guild] = std::make_pair(INVALID_GUILDGUID,0);
    pairKickGuild_ = std::make_pair(INVALID_GUILDGUID,INVALID_GUILDGUID);

    eventUpdateTimer_.SetTimer(EVENT_UPDATE_TIME);
    outsider_updatetimer_.SetTimer(OUTSIDER_UPDATE_TIME);
}

DominationField::~DominationField(void)
{
    outSider_.clear();
}

VOID
DominationField::Create(KEYTYPE key, MAPCODE mapCode, ServerSession* pServerSession)
{
    SetKey(key);
    SetMapCode(mapCode);
    SetLinkedServer(pServerSession);

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
    _SetEventInfos(mapCode);
#endif //_NA_007128_20140326_MONDSCHIEN_DOMINATION

    On_create();
}

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
VOID DominationField::_SetEventInfos(MAPCODE map_code)
{
    DominationInfoParser* pParser = DominationInfoParser::Instance();
    if(pParser == NULL)
    {
        return;
    }

    event_infos_ = pParser->GetEventInfosFromMapCode(map_code);
}


eDOMINATION_EVENT_STATE DominationField::_GetCurEventState(const WORD day_of_week, const DWORD time) const
{
    //GM�̺�Ʈ�� ��ϵǾ��ִٸ� ó���Ѵ�.
    DominationInfoParser* pParser = DominationInfoParser::Instance();
    const sDOMINATION_EVENT_INFO* const event_info = pParser->GetGMEventInfo(day_of_week, time, GetMapCode());
    if (event_info != NULL)
    {
        const DWORD* times = event_info->m_dwTime;
        for (int cur_event = DOMINATION_EVENT_AUCTION_START; 
            cur_event < DOMINATION_EVENT_AUCTION_START2; ++cur_event, ++times)
        {
            const DWORD start_time = *times;
            const DWORD end_time = *(times+1);
            const bool is_include = Between(time, start_time, end_time);
            if (is_include)
            {
                return (eDOMINATION_EVENT_STATE)cur_event;
            }
        }
    }

    EventInfos::const_iterator event_infos_it =  event_infos_.begin();
    EventInfos::const_iterator event_infos_end = event_infos_.end();

    //��ϵǾ��� �̺�Ʈ�� ��ȸ�Ѵ�.
    for ( ;event_infos_it != event_infos_end; ++event_infos_it)
    {
        const sDOMINATION_EVENT_INFO& event_info = event_infos_it->second;
        //���ð� ���� �������� üũ�Ѵ�.
        if (event_info.m_wDayOfWeek != day_of_week)
        {
            continue;
        }
        //���� �ð��� �̺�Ʈ�� �ð��� ���ԵǴ��� Ȯ���Ѵ�.
        const DWORD start_time = event_info.m_dwTime[DOMINATION_EVENT_AUCTION_START];
        const DWORD end_time = event_info.m_dwTime[DOMINATION_EVENT_AUCTION_START2];
        const bool is_include = Between(time, start_time, end_time);
        if (is_include == false)
        {
            continue;
        }
        //�̺�Ʈ�� �ð��� ���Եȴٸ� ���� �ð��� �ش��ϴ� �̺�Ʈ ���¸� �����Ѵ�
        const DWORD* times = event_info.m_dwTime;
        for (int cur_event = DOMINATION_EVENT_AUCTION_START; 
            cur_event < DOMINATION_EVENT_AUCTION_START2; ++cur_event, ++times)
        {
            const DWORD start_time = *times;
            const DWORD end_time = *(times+1);
            const bool is_include_current = Between(time, start_time, end_time);
            if (is_include_current == false)
            {
                continue;
            }
            return (eDOMINATION_EVENT_STATE)cur_event;
        }
    }

    //��ϵǾ��� ������ �̺�Ʈ�� ���ٸ� ���尡���� ��� �����̴�.
    return DOMINATION_EVENT_END;
}
#endif

VOID
DominationField::On_create()
{
    MSG_AG_ZONE_DOMINATION_FIELD_CREATE_CMD msg;
    msg.m_dwKey		= 0;
    msg.m_MapCode	= GetMapCode();
    msg.m_RoomKey	= GetKey();
    SendToLinkedServer(&msg, sizeof(msg));

    // ���� ���� ���� ���Ӽ��� ��⵿�� ����ȭ ����.
    MSG_AG_ZONE_DOMINATION_FIELD_INFO_SYN msgSyn;
    msgSyn.m_RoomKey = GetKey();
    msgSyn.m_byEventState = curEventState_;

    SendToLinkedServer(&msgSyn, sizeof(msgSyn));
}


VOID
DominationField::_ReSetDomination()
{
    if (FALSE == bReCreate_)
    {
        if (pairKickGuild_.first)
        {
            _KickUser(pairKickGuild_.first);
            pairKickGuild_.first = 0;
        }

        if (pairKickGuild_.second)
        {
            _KickUser(pairKickGuild_.second);
            pairKickGuild_.second = 0;
        }
    }
    else
    {        
        bReCreate_ = FALSE;

        _KickUser(INVALID_GUILDGUID);
        pairKickGuild_ = std::make_pair(INVALID_GUILDGUID,INVALID_GUILDGUID);

        //.//

        KEYTYPE key   = GetKey();
        MAPCODE mCode = GetMapCode();
        ServerSession* pSession = GetLinkedServer();

        Destroy();
        Create(key,mCode,pSession); 
    }

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
    if (curEventState_ == DOMINATION_EVENT_END) 
    {
        GUILDGUID winner_guild = guild_count_[defense_guild].first;
        _KickUsers_ExceptWinGuild(winner_guild);
    }
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
}
    
VOID
DominationField::On_destroy()
{
    guild_count_[offense_guild] = std::make_pair(0,0);
    guild_count_[defense_guild] = std::make_pair(0,0);
}

VOID 
DominationField::ReadyUser( User * pUser, eSEND_TYPE type, MAPCODE FieldCode, DWORD dwAreaID, WzVector * pwvStartPos, bool bFirstJoin )
{
    if (!dwAreaID)
    {                           // (CHANGES) 0902���� ������ �ʵ��� ��� assert(GetMapInfo() != NULL)
        const sMAPINFO* psMAPINFO = GetMapInfo();// MapInfoParser::Instance()->FindMapInfo(GetMapCode());
        const GUILDGUID guildGuid = pUser->GetSelectedCharGuildGuid();
        const BYTE guildIdx = _GetGuildCountIndex(guildGuid);

        // ���� ���� ��ġ�� GameServer���� �����ȴ�.
        if (defense_guild == guildIdx)
            dwAreaID = psMAPINFO->wStartAreaID;
        else if(offense_guild == guildIdx)
            dwAreaID = psMAPINFO->wStartAreaID2;
        else
        {
            SUNLOG( eDEV_LOG, "[DominationField::ReadyUser] MapCode[%d] CharName[%s]", 
                GetMapCode(), pUser->GetSelectedCharName());
        }
    }

    ZoneInterface::ReadyUser( pUser, type, FieldCode, dwAreaID, pwvStartPos, bFirstJoin );

    //���� ������ �ش� ������ ���� ���ӷα׷� �����(�߱� ��û).
    BYTE player_count = _CountingPlayer(pUser->GetSelectedCharGuildGuid());
    GAMELOG->LogDominationFieldEnterCount(ACT_DOMINATION_FIELD_JOIN_COUNT, pUser, 
                                          player_count, GetMapCode());
}

VOID
DominationField::On_readyuser(User* pUser)
{
    pUser->SetZoneState(ePRS_BEFORE_ENTER_DOMINATION_FIELD);
    pUser->SetZoneKey(GetKey());
    pUser->SetZoneType(eZONETYPE_DOMINATION_FIELD);
}

VOID
DominationField::On_enteruser(User* pUser)
{
    ZoneInterface::On_enteruser(pUser);

    pUser->SetZoneState(ePRS_AT_DOMINATION_FIELD);
    pUser->SetSelectedCharMapCode(GetMapCode());

    AgentPartyManager::Instance()->EnterZone(pUser, this);

    // ���� �����ڿ� ���ؼ� _CanJoinEach ������ ��ġ�� �ʴ´�.
    // ��ǻ� ��� �ü� �ִ� ���� GM ��ɾ �̿��ϴ� ���

    RC::eROOM_RESULT join_result = _CanJoinEach(pUser,(eZONEPUBLIC)0,NULL);
    RC::eROOM_RESULT count_result = _GuildCount(pUser,+1);
    if (RC::RC_ROOM_SUCCESS != join_result || RC::RC_ROOM_SUCCESS != count_result)
    {
        const byte bySts    = pUser->GetSts();
        const bool bGMUser  = (eGM_GRADE_BASE<bySts && bySts<eGM_GRADE_MAX) ? true : false;
        OUTSIDERS* outSider = NULL;

        if (is_gm_observer && bGMUser)
            outSider = &outSiderOb_;
        else
            outSider = &outSider_;

        outSider->push_back( std::make_pair(pUser->GetPlayerKey(),0) );

        SUNLOG( eCRITICAL_LOG, 
                "[DominationField::On_enteruser] outSider_ MapCode[%d] NAME[%s], JOINCODE[%d], COUNTCODE[%d]", 
                GetMapCode(), pUser->GetSelectedCharName(), join_result, count_result);
        return;
    }
    SUNLOG( eDEV_LOG, "[DominationField::On_enteruser] InSider_ MapCode[%d] NAME[%s]", 
        GetMapCode(), pUser->GetSelectedCharName());
}

VOID
DominationField::On_leaveuser(User* pUser)
{
    if (pUser->GetPartyState().IsPartyMember() == true)
    {
        AgentPartyManager::Instance()->LeaveZone(pUser, this);
    }

    if (FALSE == _GuildCount(pUser,-1))
    {
        SUNLOG( eDEV_LOG, "[DominationField::On_leaveuser] OutSider_ MapCode[%d] GUILDGUID[%d]", 
            GetMapCode(), pUser->GetSelectedCharGuildGuid());
        return;
    }
    SUNLOG( eDEV_LOG, "[DominationField::On_leaveuser] InSider_ MapCode[%d] GUILDGUID[%d]", 
        GetMapCode(), pUser->GetSelectedCharGuildGuid());
}

//_NA001385_20090924_DOMINATION_EVENT_CONTROLL
// ���� ���� üũ

RC::eROOM_RESULT DominationField::_CanJoinEach(User* pUser, eZONEPUBLIC roomPublic, TCHAR* pszRoomPWD)
{
    if (DOMINATION_EVENT_SYSTEM_RESET != curEventState_)
    {
        const byte bySts   = pUser->GetSts();
        const bool bGMUser = (eGM_GRADE_BASE<bySts && bySts<eGM_GRADE_MAX) ? true : false;

        if (is_gm_observer && bGMUser)
        {
            SUNLOG( eDEV_LOG, "[DominationField::_CanJoinEach] MapCode[%d] CharName[%s] - GM Entered", 
                GetMapCode(), pUser->GetSelectedCharName());
            return RC::RC_ROOM_SUCCESS;
        }
    }

    // ��Ƽ�� ���� MAX_FIELD_PARTYMEMBER_NUM �̻��̸� ���� �� ����.
    DWORD MemberNum = AgentPartyManager::Instance()->GetMemberNum(pUser->GetPartyState().GetPartyKey());
    if(MemberNum > MAX_FIELD_PARTYMEMBER_NUM)
    {
        return RC::RC_ROOM_PARTY_MEMBER_ACCESS;
    }

    const sDOMINATION_INFO* pInfo = DominationInfoParser::Instance()->GetDominationInfo(GetMapCode());
    if (NULL == pInfo)
        return RC::RC_ROOM_INVALIDROOMTYPE;

    const GUILDGUID guildGuid = pUser->GetSelectedCharGuildGuid();
    const INT guildIdx = _GetGuildCountIndex(guildGuid);

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
    // ��尡 ������ ���� �Ҽ� ����
    if (guildGuid == INVALID_GUILDGUID) {
        return RC::RC_ROOM_MISMATCH_JOIN_GUILD;
    }
#else
    if (guildIdx  == empty_guild_idx)
    {
        return RC::RC_ROOM_MISMATCH_JOIN_GUILD;
    }
#endif //_NA_006826_20130722_DOMINATION_RENEWAL

    RC::eROOM_RESULT result = RC::RC_ROOM_SUCCESS;
    switch(curEventState_)
    {
    case DOMINATION_EVENT_SYSTEM_RESET:                     // ��, �� ���� �Ұ�
        result = RC::RC_ROOM_NOT_OPEN_TIME_FOR_DOMINATION;
        break;

    case DOMINATION_EVENT_WAIT:                             // ��, �� ���� ����(ī��������)
    case DOMINATION_EVENT_START:
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
        // �ƹ� ��峪 ���� ����
#else
        if (guild_count_[guildIdx].second >= pInfo->m_MaxUser)
            result = RC::RC_ROOM_LIMIT_MAX_GUILDMEMBER;
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
        break;
    case DOMINATION_EVENT_SELECT_ATTACKGUILD:               //gamelulu : ���������� �̺�Ʈ �߰�
    case DOMINATION_EVENT_SELECT_ATTACKGUILD_END:
    case DOMINATION_EVENT_AUCTION_START:                    // ����(���α��) ������ ���� ����
    case DOMINATION_EVENT_AUCTION_END:
    case DOMINATION_EVENT_END:
        if (guildIdx != defense_guild)
            result = RC::RC_ROOM_MISMATCH_JOIN_GUILD;
        break;
    default:
        SUNLOG( eDEV_LOG, "[DominationField::_CanJoinEach] MapCode[%d] CharName[%s] curEventState[%d]", 
            GetMapCode(), pUser->GetSelectedCharName(), curEventState_);
        result = RC::RC_ROOM_NOT_OPEN_TIME_FOR_DOMINATION;
        break;
    }

    return result;
}

// ���� ������ �ʵ� ������ �־�����, ACK�޽������� ��� ������ ���ŵȴ�.
//
// SYN ������ Create�� curEventState_ �ʱ�ȭ ���� ���� �ɶ� �߻�.
// SYN ������ ��� �������� ���� ������ �����Ͱ� ����ȭ �ɶ� �߻�.
// SYN ������ _UpdateTimeEvent�� curEventState_ ���� ���� �ɶ� �߻�.(ReCreate�� ��������)
// SYN ������ ��.�� �� Ȯ�� �ɶ� �߻�

VOID DominationField::SetGuildGuid(GUILDGUID offense, GUILDGUID defense, eDOMINATION_EVENT_TYPE eSetType)
{
    const GUILDGUID oldOffense = guild_count_[offense_guild].first;
    const GUILDGUID oldDefense = guild_count_[defense_guild].first;

    switch(eSetType)
    {
    case DOMINATION_INFO_SET:   // ������ ������
    case DOMINATION_INIT:       // ���� �����, gm��ɾ�, ���� ����  -  ���� ������ ������ kick �Ѵ�
        if (oldDefense != defense)
        {
            pairKickGuild_.first = oldDefense;
            guild_count_[defense_guild] = std::make_pair(defense,0);

            SUNLOG( eDEV_LOG, "[Domi::SetGuild] MapCode[%d] defenseArg[%d] defenseMem[%d] EventState[%d]", 
                GetMapCode(), defense, guild_count_[defense_guild].first, curEventState_);
        }

        if (oldOffense != offense)
        {
            pairKickGuild_.second = oldOffense;
            guild_count_[offense_guild] = std::make_pair(offense,0);

            SUNLOG( eDEV_LOG, "[Domi::SetGuild] MapCode[%d] offenseArg[%d] offenseMem[%d] EventState[%d]", 
                GetMapCode(), offense, guild_count_[offense_guild].first, curEventState_);
        }
        break;

    case DOMINATION_SUCCESS_DEFENCE:    // ��� ����
        pairKickGuild_.second = offense;
        break;

    case DOMINATION_SUCCESS_OFFENCE:    // ���� ����
        pairKickGuild_.first = defense;
        std::swap(guild_count_[offense_guild],guild_count_[defense_guild]);
        break;
#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION 
    case DOMINATION_TURN_OVER:  //��Ʈ���� ���� ����
        if (oldDefense != defense)
        {
            pairKickGuild_.first = oldDefense;
            guild_count_[defense_guild] = std::make_pair(defense,0);

            SUNLOG( eDEV_LOG, "[Domi::SetGuild] MapCode[%d] defenseArg[%d] defenseMem[%d] EventState[%d]", 
                GetMapCode(), defense, guild_count_[defense_guild].first, curEventState_);
        }
        break;
#endif
    }

    switch(eSetType)
    {
    case DOMINATION_SUCCESS_DEFENCE:   
    case DOMINATION_SUCCESS_OFFENCE:
        {
            guild_count_[offense_guild] = std::make_pair(INVALID_GUILDGUID,0);

            MSG_AG_ZONE_DOMINATION_FIELD_INFO_SYN msg;
            msg.m_RoomKey = GetKey();
            msg.m_byEventState = curEventState_ = DOMINATION_EVENT_END;

            SendToLinkedServer(&msg, sizeof(msg));
        }
        break;
    }


#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION 
    //�Ͽ����� ��� űŸ�̸Ӹ� Set���� �ʴ´�
    if (0 == kickTimer_.GetRemainedTime() && eSetType != DOMINATION_TURN_OVER)
    {
        SUNLOG( eCRITICAL_LOG, "[DominationField::SetGuildGuid] Domination Expired !!!");
        kickTimer_.SetTimer(KICK_TIME);
    }
#else
    if (0 == kickTimer_.GetRemainedTime())
    {
        SUNLOG( eCRITICAL_LOG, "[DominationField::SetGuildGuid] Domination Expired !!!");
        kickTimer_.SetTimer(KICK_TIME);
    }
#endif
}

//////////////////////////////////////////////////////////////////////////

INT DominationField::_GetGuildCountIndex(const GUILDGUID guildGuid) 
{
    ASSERT(guildGuid != INVALID_GUILDGUID);

    if (guildGuid == guild_count_[offense_guild].first)
        return offense_guild;

    if (guildGuid == guild_count_[defense_guild].first)
        return defense_guild;

    return empty_guild_idx;
}


BOOL DominationField::_ReturnToEntrance(User* pUser)
{
    S_TRY
    {
        if(pUser->IsBeginTransaction())
            S_THROW(1);

        if(pUser->GetBehaveState() != PLAYER_BEHAVE_IDLE_STATE)
            S_THROW(2);

        DominationInfoParser* pDomiParser = DominationInfoParser::Instance();
        const sDOMINATION_INFO* pDomiInfo = pDomiParser->GetDominationInfo(GetMapCode());
        if (NULL == pDomiInfo)
            S_THROW(3);

        MapInfoParser* pMapParser = MapInfoParser::Instance();
        const WORD IdxPortal = pMapParser->GetCurrentMapPortalIndexVer2(pDomiInfo->m_DominationMapCode, pDomiInfo->m_LinkMapCode);
        if (0 == IdxPortal)
            S_THROW(4);

        sMAP_ENTERENCE_PORTAL* pFromInfo = pMapParser->GetMapPortal(IdxPortal);
        if(!pFromInfo)
            S_THROW(5);

        ZoneInterface* pToZone = g_ZoneManager.FindStaticZone(pFromInfo->wWorldTo);
        if(!pToZone)
            S_THROW(6);

        RC::eROOM_RESULT rt = pToZone->CanJoin(pUser, (eZONEPUBLIC)0, NULL);
        if(RC::RC_ROOM_SUCCESS != rt)
            S_THROW(7);

        CheckMoveZonePolicy* const pCheckMoveZonePolicy =
            MoveZonePolicy<MZTF_STATIC_TO_STATIC>::AlignInfo(
            pUser
            //{		From		|			To		}
            , pUser->GetZoneType(),	pToZone->GetType()	//	eZONETYPE
            , pUser->GetZoneKey(),	pToZone->GetKey()	//	ZoneKey
            );
        RoomArgument args(eZONEPUBLIC_PUBLIC, NULL, pFromInfo->wFieldTo, pFromInfo->m_dwToAreaID);
        rt = g_ZoneManager.MoveZone(pCheckMoveZonePolicy, args);
        if (RC::RC_ROOM_SUCCESS != rt)
            S_THROW(8);

        MSG_CG_ZONE_GMPORTAL_FIELD_MOVE_CMD msgCMD;
        msgCMD.m_MapCode = pDomiInfo->m_LinkMapCode;
        pUser->SendPacket(&msgCMD, sizeof(msgCMD));

        //�ʵ��̵� �α�
        GAMELOG->MoveVillage(pUser, pFromInfo->wWorldFrom, pFromInfo->wWorldTo, FALSE);
    }
    S_CATCH(DWORD, err)
    {
        SUNLOG( eCRITICAL_LOG, "[DominationField::_ReturnToEntrance_error] "
                               "MapCode[%d] GUILDGUID[%d] NAME[%s] err[%d]", 
                               GetMapCode(), pUser->GetSelectedCharGuildGuid(),
                               pUser->GetSelectedCharName(), err);

        return FALSE;
    }
    
    return TRUE;
}

BYTE DominationField::_CountingPlayer(GUILDGUID guild_guid)
{
    const ZONE_USER_LIST& user_list = GetUserList();

    BYTE user_num = 0;

    FOREACH_CONTAINER(const ZONE_USER_LIST::value_type& node, user_list, ZONE_USER_LIST)
    {
        User* const user = node.second;
        if (user->GetSelectedCharGuildGuid()  == guild_guid)
        {
            ++user_num;
        }
    }

    return user_num;
}

//������ �ʿ��� �ش��ϴ� ����� 
RC::eROOM_RESULT DominationField::_GuildCount(User* pUser, INT count)
{
    ASSERT(pUser);

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
    return RC::RC_ROOM_SUCCESS;
#endif //_NA_006826_20130722_DOMINATION_RENEWAL

    const GUILDGUID guildGuid = pUser->GetSelectedCharGuildGuid();
    if (INVALID_GUILDGUID == guildGuid)
        return RC::RC_ROOM_NOT_INCLUDE_GUILD;

    std::pair<BOOL,INT> guildCount = _Counting(guildGuid, count);
    
    if (FALSE == guildCount.first)
        return RC::RC_ROOM_MISMATCH_JOIN_GUILD;

    if (DOMINATION_EVENT_WAIT == curEventState_ ||
        DOMINATION_EVENT_START== curEventState_ )       //������ ����ø� Ŭ�� ī���� �Ѵ�.
    {
        const sDOMINATION_INFO* pInfo = 
            DominationInfoParser::Instance()->GetDominationInfo(GetMapCode());

        if ((pInfo != NULL && 
            guildCount.second > pInfo->m_MaxUser) ||
            guildCount.second < 0 )
        {
            SUNLOG( eCRITICAL_LOG, "[DominationField::_GuildCount]"
                "error MapCode[%d] GUILDGUID[%d] CUR_COUNT[%d] MAX[%d]", 
                GetMapCode(), guildGuid, guildCount.second, pInfo->m_MaxUser );

            //return RC::RC_ROOM_LIMIT_MAX_GUILDMEMBER;   // ��� ī������ ���� �ִ� ���� ����. �� [4/20/2010 lst1024]
        }
        // NOTE: f110817.2L, the client usage about brdCount.m_byCount only show
        // the UI presentation without offset control like an array
        MSG_CG_DOMINATION_COUNT_NOSES_BRD brdCount;
        brdCount.m_byCount = guildCount.second;

        const ZONE_USER_LIST& rList = GetUserList();
        FOREACH_CONTAINER(const ZONE_USER_LIST::value_type& rNode, rList, ZONE_USER_LIST)
        {
            User* const pBrdUser = rNode.second;
            if (pBrdUser->GetSelectedCharGuildGuid() == guildGuid)
                pBrdUser->SendPacket(&brdCount, sizeof(brdCount));
        }
    }

    return RC::RC_ROOM_SUCCESS;
}

std::pair<BOOL,INT> DominationField::_Counting(GUILDGUID guildGuid, INT count)
{
    ASSERT(guildGuid);

    for(int i = 0; i < max_guild ; ++i)
    {
        if (guild_count_[i].first == guildGuid)
        { 
            INT& _count = guild_count_[i].second;

            _count += count;

            return std::make_pair(TRUE,_count);
        }
    }

    return std::make_pair(FALSE,0);
}

VOID DominationField::_KickUser(GUILDGUID guildGuid)
{
    const ZONE_USER_LIST& rList = GetUserList();
    ZONE_USER_LIST::const_iterator it	= rList.begin();
    ZONE_USER_LIST::const_iterator itend= rList.end();
    
    while (it!=itend)
    {
        User* const pUser = it->second;
        ++it;
        
        if (NULL == pUser)
            continue;

        if (INVALID_GUILDGUID == guildGuid)
        {
            _ReturnToEntrance(pUser);   // ��� ���� �̵�
        }
        else
        {
            if (pUser->GetSelectedCharGuildGuid() == guildGuid)
            {
                const DWORD dwPlayerKey = pUser->GetPlayerKey();
                outSider_.push_back(std::make_pair(dwPlayerKey,0));
            }
        }
    }
}

VOID DominationField::KickUser(DWORD playerKey)
{
    outSider_.push_back(std::make_pair(playerKey,0));
}


VOID DominationField::_UpdateTimeEvent()
{
    if (NULL == GetLinkedServer()) 
        return;

    if (eventUpdateTimer_.IsExpired())
    {
        DWORD dwTime = 0;
        const WORD wDayOfWeek = util::GetDateTime_Week_HHMMSS(dwTime);
        DominationInfoParser* pParser = DominationInfoParser::Instance();

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
        //�ļ����� �̺�Ʈ ������ �������� �ʰ� �ڽ��� EventInfos���� ������ ������ ��´�
        eDOMINATION_EVENT_STATE curEventState = _GetCurEventState(wDayOfWeek, dwTime);
#else    
        eDOMINATION_EVENT_STATE curEventState = pParser->GetCurEventState(wDayOfWeek,dwTime);
#endif
        if (curEventState_ == curEventState)
            return;

        if (DOMINATION_EVENT_END   == curEventState_ &&
            DOMINATION_EVENT_START == curEventState)
            return;

        // ���̺꿡�� �̺�Ʈ �αװ� �ʿ��Ҷ��� �߻� �ϹǷ� eCRITICAL_LOG �� ������ �����.
        const char* event_string = curEventState == DOMINATION_EVENT_AUCTION_START ? "AUCTION_START" 
                                 : curEventState == DOMINATION_EVENT_AUCTION_START2? "AUCTION_START2"
                                 : curEventState == DOMINATION_EVENT_SYSTEM_RESET ? "SYSTEM_RESET"
                                 : curEventState == DOMINATION_EVENT_AUCTION_END  ? "AUCTION_END"
                                 : curEventState == DOMINATION_EVENT_SELECT_ATTACKGUILD ? "SELECT_ATTACKGUILD"
                                 : curEventState == DOMINATION_EVENT_SELECT_ATTACKGUILD_END ? "SELECT_ATTACKGUILD_END"
                                 : curEventState == DOMINATION_EVENT_WAIT  ? "WAIT"
                                 : curEventState == DOMINATION_EVENT_START ? "START"
                                 : curEventState == DOMINATION_EVENT_END   ? "END"
                                 : curEventState == DOMINATION_EVENT_MAX   ? "MAX" 
                                 : "EMPTY";

        SUNLOG( eCRITICAL_LOG, "[DominationField::_UpdateTimeEvent] map[%d] Event[%s] off[%d] def[%d]"
                               ,GetMapCode(), event_string, 
                               guild_count_[offense_guild].first, guild_count_[defense_guild].first );

        if (DOMINATION_EVENT_MAX != curEventState_ &&
            DOMINATION_EVENT_SYSTEM_RESET == curEventState)
        {
            bReCreate_ = TRUE;
            pairKickGuild_.first  = guild_count_[defense_guild].first;
            pairKickGuild_.second = guild_count_[offense_guild].first;

            kickTimer_.SetTimer(KICK_TIME);
        }


        curEventState_ = curEventState;

        MSG_AG_ZONE_DOMINATION_FIELD_INFO_SYN msg;
        msg.m_RoomKey = GetKey();
        msg.m_byEventState = curEventState;

        SendToLinkedServer(&msg, sizeof(msg));
    }
}

BOOL DominationField::Process()
{
    _UpdateTimeEvent();

    if (outsider_updatetimer_.IsExpired() &&
        false == outSider_.empty())
    {
        OUTSIDER out_sider = outSider_.front();
        outSider_.pop_front();
        User* pUser = FindUser(out_sider.first);

        if (pUser)
        {
            if (FALSE == _ReturnToEntrance(pUser))
            {
                if (out_sider.second++ > 10)  // //���� ����.
                {
                    SUNLOG(eCRITICAL_LOG, "[DominationField::Process] 10ȸ �õ� �� ���� ���� ��"
                                          "MapCode[%d] GUILDGUID[%d] NAME[%s]",
                                          GetMapCode(), pUser->GetSelectedCharGuildGuid(),
                                          pUser->GetSelectedCharName());

                    pUser->SetDisconnectCode(RC::RC_DISCONNECTION_INVALID_CHAR_STATE);
                    pUser->DisconnectUser();
                }
                else
                {
                    outSider_.push_back(out_sider);
                }
            }
            else
            {
                SUNLOG(eFULL_LOG, "[DominationField::Process] out_sider_ReturnToEntrance ",
                                  "MapCode[%d] GUILDGUID[%d] NAME[%s]",
                                  GetMapCode(), pUser->GetSelectedCharGuildGuid(),
                                  pUser->GetSelectedCharName());
            }
        }
        else
        {
            SUNLOG( eCRITICAL_LOG, "[DominationField::Process] not found user key[%d] count[%d]",
                                   out_sider.first, out_sider.second );
        }
    }

    if (kickTimer_.IsExpiredManual(FALSE))
    {
        _ReSetDomination();
    }

    return TRUE;
}

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
void DominationField::_KickUsers_ExceptWinGuild(GUILDGUID winner_guild)
{
    const ZONE_USER_LIST& rList = GetUserList();
    ZONE_USER_LIST::const_iterator it	= rList.begin();

    while( it != rList.end() )
    {
        User* const pUser = it->second;
        ++it;

        if (pUser->GetSelectedCharGuildGuid() != winner_guild)
        {
            _ReturnToEntrance(pUser);   // ��� ���� �̵�
        }
    }
}
#endif //_NA_006826_20130722_DOMINATION_RENEWAL