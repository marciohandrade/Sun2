#include "stdafx.h"
#include "./Guild.h"

#include <Struct.h>
#include <PacketStruct_DZ.h>
#include <SerializeStream.h>

#include <GuildRankInfoParser.h>
#include <MaxGuildMemberInfoParser.h>

#include "./GuildMember.h"
#include "Services/Guild/GuildManager.h"

//_NA_20100604_BETTER_DOMINATION_MANAGER
#include "Services/DominationManager.h"

#ifdef _NA_003923_20120130_GUILD_RENEWAL
#include "GuildInGameLog.h"
#endif

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#include "PacketStruct_GZ.h"
#include "GuildFacilities.h"
#include "GuildFacilityInfoParser.h"
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
//==================================================================================================
// check rules
__IMPL_GUILDPOOL(Guild);
//==================================================================================================

Guild::Guild()
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
: guild_facilities_(NULL)
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
{
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    guild_facilities_ = NULL;
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
}

Guild::~Guild()
{
}

void 
Guild::InitializeGuild()
{
    master_change_time_ = 0;
    ZeroMemory(&guild_info_, sizeof(guild_info_));
    ZeroMemory(guild_notice_, sizeof(guild_notice_));
#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
    memset(&m_ChuntingPVPInfo, 0, sizeof(CHUNTING_PVP_INFO));
#endif
#ifdef _NA_004034_20120102_GUILD_INTRODUCTION
    memset(guild_introduction_, 0, sizeof(guild_introduction_));
#endif //_NA_004034_20120102_GUILD_INTRODUCTION
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    guild_join_request_list_.clear();
    guild_facilities_ = new GuildFacilities;
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
}

void 
Guild::ReleaseGuild()
{
    guild_relation_entry_.RelationRelease();
#ifdef _NA_003923_20120130_GUILD_RENEWAL
    SAFE_DELETE(guild_ingame_log_);
#endif
    
    GUILD_CORPS_MAP::const_iterator it = guild_corps_list_.begin();
    const GUILD_CORPS_MAP::const_iterator end = guild_corps_list_.end();
    for (; it != end; ++it)
    {
        GuildCorps* guild_corps = it->second;
        if (guild_corps)
        {
            guild_corps->Destroy();
            delete guild_corps;
        }
    }

    guild_corps_list_.clear();

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    guild_join_request_list_.clear();
    SAFE_DELETE(guild_facilities_);
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
}

void 
Guild::SetGuildInfo(BASE_GUILD_INFO & IN rInfo)
{
    SerializeBaseGuildInfo(rInfo, SERIALIZE_STORE);
}

GuildMember* 
Guild::FindMember(CHARGUID char_guid) const
{
    GUILD_CORPS_MAP::const_iterator it = guild_corps_list_.begin();
    const GUILD_CORPS_MAP::const_iterator end = guild_corps_list_.end();
    for (; it != end; ++it)
    {
        GuildCorps* guild_corps = it->second;
        if (guild_corps == NULL) {
            continue;
        };
        GuildMember* guild_member = guild_corps->FindMember(char_guid);
        if (guild_member == NULL) {
            continue;
        };
        return guild_member;
    }

    return NULL;
}

// 이름으로 찾기
GuildMember* 
Guild::FindMemberByCharName(const TCHAR* char_name) const
{
    GUILD_CORPS_MAP::const_iterator it = guild_corps_list_.begin();
    const GUILD_CORPS_MAP::const_iterator end = guild_corps_list_.end();
    for (; it != end; ++it)
    {
        GuildCorps* guild_corps = it->second;
        if (guild_corps == NULL) {
            continue;
        };
        GuildMember* guild_member = guild_corps->FindMember(char_name);
        if (guild_member) {
            return guild_member;
        };
    }

    return NULL;
}

RC::eGUILD_RESULT 
Guild::CanRankUp(OUT GuildRankInfo& result_guild_rank_info) const
{
#ifdef _NA_003923_20120130_GUILD_RENEWAL
    return RC::RC_GUILD_FAILED;
#else
    BYTE byGuildMaxGrade = GuildRankInfoParser::Instance()->GetGuildMaxGrade();

    // 현재 길드의 등급이 최대 등급이거나, 그 이상이면 더 이상 랭크업 할 수 없다.
    if (guild_info_.m_GuildGrade >= byGuildMaxGrade) {
        return RC::RC_GUILD_MAX_GUILD_RANK;
    };
    GuildRankInfo* guild_rank_info = 
        GuildRankInfoParser::Instance()->FindGuildRankInfo(guild_info_.m_GuildGrade + 1);
    if (guild_rank_info == NULL) {
        return RC::RC_GUILD_NOT_FOUND_GUILD_INFO_FOR_GRADE;
    };
    //1. UP 계산
    if (guild_info_.m_UserPoint < guild_rank_info->m_UP) {
        return RC::RC_GUILD_NOT_ENOUGH_UP;
    };
    //2. GP 계산
    if (guild_info_.m_GuildPoint < guild_rank_info->m_GP) {
        return RC::RC_GUILD_NOT_ENOUGH_GP;
    };
    // OUT 반환
    result_guild_rank_info = *guild_rank_info;

    return RC::RC_GUILD_SUCCESS;
#endif //_NA_003923_20120130_GUILD_RENEWAL
}

void 
Guild::RankUp()
{
    BYTE prevGrade = (BYTE)GetGuildGrade();

    const GuildRankInfo* guild_rank_info = 
        GuildRankInfoParser::Instance()->FindGuildRankInfo(guild_info_.m_GuildGrade + 1);
    if (guild_rank_info == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "[Guild::RankUp()] Error!! CurGuildGrade = %d",
               guild_info_.m_GuildGrade);
        return;
    }

    SetGuildGrade(GetGuildGrade() + 1);

#ifdef _NA_003923_20120130_GUILD_RENEWAL
    SetGuildGP(0);
    SetGuildUP(0);

    GetGuildInGameLog()->WriteGuildLog(
        GuildLevelUp,
        GetGuildGuid(),
        NULL,
        GetGuildGrade()
        );
#else
    SetGuildUP(guild_info_.m_UserPoint - guild_rank_info->m_UP);
    SetGuildGP(guild_info_.m_GuildPoint - guild_rank_info->m_GP);
#endif

    GUILDLOG->LogRankUp(this, prevGrade);
}


//길드 직책
bool 
Guild::SetMemberDuty(CHARGUID char_guid, eGUILD_DUTY guild_duty)
{ 
    GuildMember* guild_member = FindMember(char_guid);
    if (guild_member == NULL) {
        return false;
    }
    eGUILD_DUTY prevDuty = guild_member->GetDuty();
    guild_member->SetDuty(guild_duty);

    GUILDLOG->LogDutyChange(this, guild_member, prevDuty);

#ifdef _NA_003923_20120130_GUILD_RENEWAL
    GetGuildInGameLog()->WriteGuildLog(
        GuildDutyChange,
        GetGuildGuid(),
        guild_member->GetCharName(),
        guild_duty
        );
#endif
    return true;
}

bool 
Guild::IsExistMemberDuty(eGUILD_DUTY guild_duty, BYTE corps_number) const
{
    __UNUSED(guild_duty);
    GuildCorps* guild_corps = FindCorps(corps_number);
    if (guild_corps == NULL) 
    {
        return false;
    };

#ifdef _NA_003923_20120130_GUILD_RENEWAL
    const GuildMember* guild_member = FindMemberByDuty(guild_duty);
    if (guild_member != NULL)
    {
        return true;
    }
#endif

        return false;
}

#ifdef _NA_003923_20120130_GUILD_RENEWAL
bool
Guild::IsFullMemberOfDuty(eGUILD_DUTY guild_duty)
{
    WORD count = 0;

    GUILD_CORPS_MAP::const_iterator it = guild_corps_list_.begin();
    const GUILD_CORPS_MAP::const_iterator end = guild_corps_list_.end();
    for (; it != end; ++it)
    {
        GuildCorps* guild_corps = it->second;
        if (guild_corps == NULL) 
        {
            continue;
        }
        count += guild_corps->GetMemberCountByDuty(guild_duty);
    }

    GuildRankInfoParser* const rank_info_parser = GuildRankInfoParser::Instance();
    const GuildRankInfo* rank_info = rank_info_parser->FindGuildRankInfo(GetGuildGrade());
    if (rank_info == NULL)
    {
        SUNLOG(
            eCRITICAL_LOG,
            __FUNCTION__" : Not found guild rank info : name = %s, grade = %d",
            GetGuildName(),
            GetGuildGrade()
            );

        return true;
    }

    if (count >= rank_info->max_member_of_duty_[guild_duty])
    {
        return true;
    }

    return false;
}
#endif


void 
Guild::SetGuildNotice(const util::StrArrayForGuildNotice& guild_notice)
{
    strncpy(guild_notice_, guild_notice, _countof(guild_notice_));
    guild_notice_[_countof(guild_notice_) - 1] = _T('\0');
}


void 
Guild::SetGuildName(const TCHAR* guild_name)
{
    strncpy(guild_info_.m_tszGuildName, guild_name, _countof(guild_info_.m_tszGuildName));
    guild_info_.m_tszGuildName[_countof(guild_info_.m_tszGuildName) - 1] = '\0';
}

GuildCorps* 
Guild::FindCorps(int corps_number) const
{
    GUILD_CORPS_MAP::const_iterator iter = guild_corps_list_.find(corps_number);     
    if (iter != guild_corps_list_.end())
    {
        GuildCorps* guild_corps = iter->second;
        if (guild_corps != NULL)
        {
            return guild_corps;
        }
    }
    return NULL;
}

WORD 
Guild::GetMemberCount() const
{
    WORD wCount = 0;

    GUILD_CORPS_MAP::const_iterator it = guild_corps_list_.begin();
    const GUILD_CORPS_MAP::const_iterator end = guild_corps_list_.end();
    for (; it != end; ++it)
    {
        GuildCorps* guild_corps = it->second;
        if (guild_corps == NULL) {
            continue;
        };
        wCount += guild_corps->GetMemberCount();
    }

    return wCount;  
}

//DB에서 군단을 생성하고 그 결과 값을 세팅한다.
GuildCorps* 
Guild::CreateCorps(BYTE corps_number)
{
    GuildCorps* guild_corps = FindCorps(corps_number);
    if (guild_corps)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=Already Corps Create, ")
               _T("GuildGuid = %u, CoprsNum = %d"), GetGuildGuid(), corps_number);
        return NULL;
    }

    GuildCorps* new_guild_corps = new GuildCorps;
    new_guild_corps->SetNumber(corps_number);
    guild_corps_list_.insert(std::make_pair(corps_number, new_guild_corps));
    return new_guild_corps;
}

void 
Guild::DestroyCorps(BYTE corps_number)
{
    GUILD_CORPS_MAP::iterator it = guild_corps_list_.find(corps_number);
    if (it != guild_corps_list_.end())
    {
        GuildCorps* guild_corps = it->second;
        if (guild_corps) {
            delete guild_corps;
        };
        guild_corps_list_.erase(it);
    }
}

//현재 생성 가능한 군단을 생성한다. (순차적으로 군단을 생성한다.)
GuildCorps* Guild::CreateCorpsBySequence()
{
    BYTE corps_number = GetEmptyCorpsNumBySequence();
    if (corps_number == 0) {
        return NULL;
    };
    return CreateCorps(corps_number);
}

#ifdef _NA_003923_20120130_GUILD_RENEWAL
BYTE 
Guild::GetEmptyCorpsNumBySequence() const
{
    const GuildRankInfo* guild_rank_info = 
        GuildRankInfoParser::Instance()->FindGuildRankInfo(GetGuildGrade());

    if (guild_rank_info == NULL) 
    {
        return 0;
    }

    return 0;
}
#else
//이 함수는 현재 길드에서 창설할 수 있는 군단의 번호를 리턴한다.
//현재 해당 군단이 존재 하지 않으면 0를 리턴한다.
//이 함수로는 직할 군단의 번호를 얻을 수 없다.
BYTE 
Guild::GetEmptyCorpsNumBySequence() const
{
    //1. 현재 길드레벨에서 가능한 군단의 개수를 가져온다.
    GuildRankInfo* guild_rank_info = \
        GuildRankInfoParser::Instance()->FindGuildRankInfo(GetGuildGrade());
    if (guild_rank_info == NULL) {
        return NULL;
    };
    // 0은 직할 군단이다. 따라서 1부터 시작한다. 군단 최대 개수는 직할군단을 포함하는 개수이다.
    // 따라서 "<" 비교한다.
    for (BYTE i = 1; i < guild_rank_info->m_byCreateCopsCount; ++i)
    {
        GUILD_CORPS_MAP::const_iterator iter = guild_corps_list_.find(i);  

        // i 군단이 존재하면
        if (iter != guild_corps_list_.end()) {
            continue;
        };
        return i;
    }

    return 0;
}
#endif

GuildMember* 
Guild::FindMaster() const
{
    GuildCorps* guild_corps = FindCorps(0);
    if (guild_corps == NULL)
    {
        return NULL;
    }
    return guild_corps->FindCommander();
}

const TCHAR* 
Guild::GetMasterCharName() const
{
    GuildMember* guild_master = FindMaster();
    if (guild_master == NULL) {
        return NULL;
    };
    return guild_master->GetCharName();
}

GuildMember* 
Guild::FindMemberByDuty(eGUILD_DUTY guild_duty) const
{
    GUILD_CORPS_MAP::const_iterator it = guild_corps_list_.begin();
    const GUILD_CORPS_MAP::const_iterator end = guild_corps_list_.end();
    for (; it != end; ++it)
    {
        GuildCorps* guild_corps = it->second;
        if (guild_corps)
        {
            GuildMember* guild_member = guild_corps->FindMemberByDuty(guild_duty);
            if (guild_member) {
                return guild_member;
            }
            else {
                continue;
            };
        }
    }

    return NULL;
}

void 
Guild::CreateGuild(GUILDGUID guild_guid, 
                   const TCHAR* guild_name, 
                   BYTE corps_number,
                   CHARGUID master_char_guid, 
                   const TCHAR* master_char_name, 
                   eGUILD_DUTY master_guild_duty,
                   BYTE master_char_class, 
                   LEVELTYPE master_char_level, 
                   BYTE byChaoState,
                   int64_t nGuildPanaltyTime, 
                   int64_t nDestroyRequestTime, 
                   int64_t nMasterChangeTime)
{
    InitializeGuild();

    SetGuildGuid(guild_guid);
    SetGuildName(guild_name);
    SetGuildGrade(GRADE_AT_GUILD_CREATE);
    SetGuildGP(0);  
    SetGuildUP(0);
    SetPenaltyTime(nGuildPanaltyTime);
    SetDestroyTime(nDestroyRequestTime);
    SetMasterChangeTime(nMasterChangeTime);
    GuildCorps* guild_corps = CreateCorps(corps_number);     //직할 군단 생성
    if (guild_corps == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=Corps Create Fail, ")
               _T("GuildGuid = %u, master_char_guid = %u"), guild_guid, master_char_guid);
        return;
    }

    GuildMember* guild_master = guild_corps->CreateMember(master_char_guid, master_char_name,
        master_guild_duty, master_char_class, master_char_level, byChaoState, true);
    if (!guild_master)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=Corps Create GuildMaster, |")
               _T("|GuildGuid = %u, master_char_guid = %u"), guild_guid, master_char_guid);
        return;
    }
#ifdef _NA_003923_20120130_GUILD_RENEWAL
    guild_ingame_log_ = new GuildInGameLog(GetGuildGuid());
#endif
}

BYTE 
Guild::GetEmptyCommanderInfo(BYTE* pEmptyCommderInfo) const
{
    BYTE byCount = 0;

    GUILD_CORPS_MAP::const_iterator it = guild_corps_list_.begin();
    const GUILD_CORPS_MAP::const_iterator end = guild_corps_list_.end();
    for (; it != end; ++it)
    {
        GuildCorps* guild_corps = it->second;
        if (guild_corps && !guild_corps->FindCommander())
        {
            pEmptyCommderInfo[byCount] = guild_corps->GetNumber();
            byCount++;      
            if (byCount >= MAX_GUILD_CORPS_NUM)
            {
                SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=Overflow CorpsInfo, GuildGuid = %u"),
                       GetGuildGuid());
                return 0;
            }
        }
    }

    return byCount;
}

//길드 가입
RC::eGUILD_RESULT 
Guild::JoinGuild(BYTE corps_number, 
                 CHARGUID char_guid,
                 const TCHAR* tszCharName, 
                 eGUILD_DUTY guild_duty,
                 BYTE byClass, 
                 LEVELTYPE Level, 
                 BYTE byChaoState)
{
    GuildCorps* guild_corps = FindCorps(corps_number);
    if (guild_corps == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "[Guild::JoinGuild] Corps(%d) Not Exist", corps_number);
        return RC::RC_GUILD_CORPS_NOT_FOUND;
    }

    //현재 가입한 놈은 로그인한 상태 이므로 로그인 상태로 유지한다.
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    const DBCharacter* const db_character = g_DBUserManager.FindConnectedCharacter(char_guid);
    bool is_login = false;
    if (db_character)
    {
        is_login = true;
    }
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    GuildMember* guild_member = guild_corps->CreateMember(char_guid, tszCharName,
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        guild_duty, byClass, Level, byChaoState, is_login);
#else  ////_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        guild_duty, byClass, Level, byChaoState, true);
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    if (guild_member == NULL) {
        return RC::RC_GUILD_FAILED;
    };
    return RC::RC_GUILD_SUCCESS;
}

RC::eGUILD_RESULT 
Guild::LeaveGuild(CHARGUID char_guid)
{
    GuildMember* guild_member = FindMember(char_guid);
    if (guild_member == NULL) {
        return RC::RC_GUILD_NOT_GUILD_MEMBER;
    };
    GuildCorps* guild_corps = FindCorps(guild_member->GetCorpsNumber());
    if (guild_corps == NULL) {
        return RC::RC_GUILD_CORPS_NOT_FOUND;
    };
    guild_corps->DestroyMember(char_guid);

    return RC::RC_GUILD_SUCCESS;
}

RC::eGUILD_RESULT 
Guild::ChangeCorps(CHARGUID char_guid, BYTE new_corps_number)
{
    GuildMember* guild_member = FindMember(char_guid);
    if (guild_member == NULL) {
        return RC::RC_GUILD_NOT_GUILD_MEMBER;
    };
    //군단번호가 같다면 성공한것으로 처리한다.
    if (new_corps_number == guild_member->GetCorpsNumber()) {
        return RC::RC_GUILD_SUCCESS;
    };
    GuildCorps* guild_corps_old = FindCorps(guild_member->GetCorpsNumber());
    if (!guild_corps_old) {
        return RC::RC_GUILD_CORPS_NOT_FOUND;
    };
    GuildCorps* guild_corps_new = FindCorps(new_corps_number);
    if (!guild_corps_new) {
        return RC::RC_GUILD_CORPS_NOT_FOUND;
    };
    guild_corps_old->DeleteMember(char_guid);
    guild_corps_new->InsertMember(guild_member);
    guild_member->SetCorpsNumber(new_corps_number);

    return RC::RC_GUILD_SUCCESS;
}

// 군단별 풀 멤버체크.
bool 
Guild::IsFullCorpsMemberByDuty(BYTE corps_number, eGUILD_DUTY guild_duty) const
{
    GuildCorps* guild_corps = FindCorps(corps_number);
    if (guild_corps == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "|["__FUNCTION__"]|Msg=Not Exist Corps, Corps = %d, guild_duty = %d",
               corps_number, guild_duty);
        return true;
    }

    BYTE byCurCount = guild_corps->GetMemberCountByDuty(guild_duty);
    
#ifdef _NA_003923_20120130_GUILD_RENEWAL 
    BYTE byMaxCount = 
        GuildRankInfoParser::Instance()->GetMaxMemberByGrade(
        GetGuildGrade(),
        guild_duty
        );
#else
    BYTE byMaxCount = 
        MaxGuildMemberInfoParser::Instance()->GetMaxCorpsMemberByDuty(
        GetGuildGrade(),
        corps_number,
        guild_duty
        );
#endif

    if (byCurCount >= byMaxCount) {
        return true;
    };
    return false;
}

bool 
Guild::CreateRearCamp(BYTE corps_number)
{
    GuildCorps* guild_corps = FindCorps(corps_number);
    if (guild_corps == NULL) {
        return false;
    };
    guild_corps->SetRearCamp(true);

    return true;
}

bool 
Guild::DestroyRearCamp(BYTE corps_number)
{
    GuildCorps* guild_corps = FindCorps(corps_number);
    if (guild_corps == NULL) {
        return false;
    };
    guild_corps->SetRearCamp(false);
    return true;
}

//길드에서 해당 멤버를 제거 한다.
RC::eGUILD_RESULT 
Guild::DestroyMember(CHARGUID char_guid)
{
    GuildMember* guild_member = FindMember(char_guid);
    if (guild_member == NULL) {
        return RC::RC_GUILD_NOT_GUILD_MEMBER;
    };
    GuildCorps* guild_corps = FindCorps(guild_member->GetCorpsNumber());
    if (guild_corps == NULL) {
        return RC::RC_GUILD_CORPS_NOT_FOUND;
    };
    guild_corps->DestroyMember(char_guid);
    return RC::RC_GUILD_SUCCESS;
}

void 
Guild::SerializeBaseGuildInfo(INOUT BASE_GUILD_INFO& base_guild_info, eSERIALIZE se)
{
    if (se == SERIALIZE_LOAD)
    {
        base_guild_info.m_GuildGuid  = GetGuildGuid();
        base_guild_info.m_GuildGrade = (BYTE)GetGuildGrade();
        memcpy(base_guild_info.m_tszGuildName, GetGuildName(), MAX_GUILDNAME_LENGTH*sizeof(TCHAR));
#ifdef _NA_008334_20150608_SONNENSCHEIN
        base_guild_info.sonnenschein_phase_ = GetSonnenScheinPhase();
#endif //_NA_008334_20150608_SONNENSCHEIN
    }
    else
    {
        SetGuildGuid(base_guild_info.m_GuildGuid);
        SetGuildGrade(base_guild_info.m_GuildGrade);
        SetGuildName(base_guild_info.m_tszGuildName);
#ifdef _NA_008334_20150608_SONNENSCHEIN
        SetSonnenScheinPhase(base_guild_info.sonnenschein_phase_);
#endif //_NA_008334_20150608_SONNENSCHEIN
    }
}

void 
Guild::SerializeGameGuildInfo(INOUT GAME_GUILD_INFO& game_guild_info, eSERIALIZE se)
{
    SerializeBaseGuildInfo(game_guild_info, se);

    if (se == SERIALIZE_LOAD)
    {
        AcquireGuildMark(game_guild_info.m_BackGroundIdx, game_guild_info.m_PatternIdx);

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
        game_guild_info.m_ChuntingPVPInfo = m_ChuntingPVPInfo;
#endif
    }
    else
    {
#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
        m_ChuntingPVPInfo   = game_guild_info.m_ChuntingPVPInfo;
#endif
    }
}

void 
Guild::SerializeGuildInfo(INOUT GUILD_INFO& guild_info, eSERIALIZE se)
{
    SerializeGameGuildInfo(guild_info, se);

    if (se == SERIALIZE_LOAD)
    {
        guild_info.m_GuildPoint = GetGuildGP();
        guild_info.m_UserPoint  = GetGuildUP();

        guild_info.m_GuildDestroyWaitFlag = CheckGuildDeleteState();
        guild_info.m_GuildPenaltyTime    = GetPenaltyTime();
        guild_info.m_GuildDestroyTime    = GetDestroyTime();
        AcquireGuildMark(guild_info.m_BackGroundIdx, guild_info.m_PatternIdx);
        guild_info.m_nMasterChangeTime   = GetMasterChangeTime();
        guild_info.m_DominationMapCount = 
            DominationManager::Instance()->GetNumOfDominationArea(guild_info.m_GuildGuid);
        guild_info.m_DominationJoinCount = GetDominationJoinCount();
        guild_info.m_DominationDefenseCount = GetDominationDefenseCount();
        guild_info.m_DominationHuntingCount = GetDominationHuntingCount();
#ifdef _NA_003923_20120130_GUILD_RENEWAL
        guild_info.m_GuildCoin = GetGuildCoin();
        guild_info.m_GuildGP   = GetGuildPoint();
#endif //_NA004034_20120102_GUILD_POINT_SYSTEM
#ifdef _NA_008334_20150608_SONNENSCHEIN
        guild_info.sonnenschein_phase_ = GetSonnenScheinPhase();
#endif //_NA_008334_20150608_SONNENSCHEIN
    }
    else
    {
        SetGuildGP(guild_info.m_GuildPoint);
        SetGuildUP(guild_info.m_UserPoint);

        SetDeleteGuildState(guild_info.m_GuildDestroyWaitFlag);
        SetPenaltyTime(guild_info.m_GuildPenaltyTime);
        SetDestroyTime(guild_info.m_GuildDestroyTime);
        RegisterGuildMark(guild_info.m_BackGroundIdx, guild_info.m_PatternIdx);
        SetMasterChangeTime(guild_info.m_nMasterChangeTime);
        SetDominationJointCount(guild_info.m_DominationJoinCount);
        SetDominationDefenseCount(guild_info.m_DominationDefenseCount);
        SetDominationHuntingCount(guild_info.m_DominationHuntingCount);
#ifdef _NA_008334_20150608_SONNENSCHEIN
        SetSonnenScheinPhase(guild_info.sonnenschein_phase_);
#endif //_NA_008334_20150608_SONNENSCHEIN
#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
        // 여기서 설정 하지 않고, GuildPointSystem에서 한다.
        //SetGuildCoin(guild_info.m_GuildCoin);
        //SetGuildPoint(guild_info.m_GuildGP);
#endif //_NA004034_20120102_GUILD_POINT_SYSTEM
#ifdef _NA_003923_20120130_GUILD_RENEWAL
        guild_ingame_log_ = new GuildInGameLog(GetGuildGuid());
#endif
    }
}

void 
Guild::SerializeCorpsInfo(INOUT GUILD_CORPS_TOTAL_INFO& corps_total_info, eSERIALIZE se)
{
    if (se == SERIALIZE_LOAD)
    {
        ZeroMemory(&corps_total_info, sizeof(corps_total_info));

        GUILD_CORPS_MAP::const_iterator it = guild_corps_list_.begin();
        const GUILD_CORPS_MAP::const_iterator end = guild_corps_list_.end();
        for (; it != end; ++it)
        {
            GuildCorps* guild_corps = it->second;
            if (guild_corps == NULL) {
                continue;
            };
            BYTE corps_number = guild_corps->GetNumber();

            corps_total_info.m_CorpsInfo[corps_number].m_bCreate = true;
            corps_total_info.m_CorpsInfo[corps_number].m_byCorpsNum = corps_number;
            corps_total_info.m_CorpsInfo[corps_number].m_bRearCamp = guild_corps->IsExistRearCamp();
        }

    }
    else //STORE
    {
        for (int i = 0 ; i < MAX_GUILD_CORPS_NUM; i++)
        {
            if (corps_total_info.m_CorpsInfo[i].m_bCreate)
            {
                GuildCorps* guild_corps = FindCorps(i);
                if (guild_corps)
                {
                    guild_corps->SetRearCamp(corps_total_info.m_CorpsInfo[i].m_bRearCamp);
                }
                else
                {
                    //DB에서 날라오는데 현재 군단이 생성되어 있지 않으면...
                    GuildCorps* new_guild_corps =
                        CreateCorps(corps_total_info.m_CorpsInfo[i].m_byCorpsNum);
                    if (new_guild_corps) {
                        new_guild_corps->SetRearCamp(corps_total_info.m_CorpsInfo[i].m_bRearCamp);
                    };
                }
            }
        }
    }   

}

void 
Guild::SerializeBaseMemberTotalInfo(INOUT BASE_GUILD_MEMBER_TOTAL_INFO& member_total_info, 
                                    eSERIALIZE se)
{
    if (se == SERIALIZE_LOAD)
    {
        member_total_info.m_byMemberCount = 0;
        GUILD_CORPS_MAP::const_iterator it = guild_corps_list_.begin();
        const GUILD_CORPS_MAP::const_iterator end = guild_corps_list_.end();
        for (; it != end; ++it)
        {
            GuildCorps* guild_corps = it->second;
            if (guild_corps == NULL) {
                continue;
            };
            guild_corps->SerializeBaseMemberTotalInfo(member_total_info, se);
        }

    }
    else
    {
        SUNLOG(eCRITICAL_LOG, "[Guild::SerializeMemberInfo] 미구현");
    }

}

void 
Guild::SerializeMemberTotalInfo(INOUT GUILD_MEMBER_TOTAL_INFO& member_total_info, eSERIALIZE se)
{
    if (se == SERIALIZE_LOAD)
    {
        member_total_info.m_byMemberCount = 0;

        GUILD_CORPS_MAP::const_iterator it = guild_corps_list_.begin();
        const GUILD_CORPS_MAP::const_iterator end = guild_corps_list_.end();
        for (; it != end; ++it)
        {
            GuildCorps* guild_corps = it->second;
            if (guild_corps == NULL) {
                continue;
            };
            guild_corps->SerializeMemberTotalInfo(member_total_info, se);
        }
    }
    else
    {
        //멤버는 반드시 해당 군단이 먼저 창설되어야 한다.
        GUILD_CORPS_MAP::const_iterator it = guild_corps_list_.begin();
        const GUILD_CORPS_MAP::const_iterator end = guild_corps_list_.end();
        for (; it != end; ++it)
        {
            GuildCorps* guild_corps = it->second;
            if (guild_corps == NULL) {
                continue;
            };
            guild_corps->SerializeMemberTotalInfo(member_total_info, se);
        }
    }
}

void 
Guild::SerializeServerMemberTotalInfo(INOUT SERVER_GUILD_MEMBER_TOTAL_INFO& member_total_info, 
                                      eSERIALIZE se)
{
    if (se == SERIALIZE_LOAD)
    {
        member_total_info.m_byMemberCount = 0;

        GUILD_CORPS_MAP::const_iterator it = guild_corps_list_.begin();
        const GUILD_CORPS_MAP::const_iterator end = guild_corps_list_.end();
        for (; it != end; ++it)
        {
            GuildCorps* guild_corps = it->second;
            if (guild_corps == NULL) {
                continue;
            };
            guild_corps->SerializeServerMemberTotalInfo(member_total_info, se);
        }
    }
    else
    {
        //멤버는 반드시 해당 군단이 먼저 창설되어야 한다.
        GUILD_CORPS_MAP::const_iterator it = guild_corps_list_.begin();
        const GUILD_CORPS_MAP::const_iterator end = guild_corps_list_.end();
        for (; it != end; ++it)
        {
            GuildCorps* guild_corps = it->second;
            if (guild_corps == NULL) {
                continue;
            };
            guild_corps->SerializeServerMemberTotalInfo(member_total_info, se);
        }
    }
}

bool 
Guild::GetBaseMemberInfo(CHARGUID char_guid, OUT BASE_GUILD_MEMBER_INFO& member_info) const
{
    GuildMember* guild_member = FindMember(char_guid);
    if (guild_member == NULL) {
        return false;
    };
    guild_member->SerializeBaseMemberInfo(member_info, SERIALIZE_LOAD);

    return true;
}

bool 
Guild::GetMemberInfo(CHARGUID char_guid, OUT GUILD_MEMBER_INFO& member_info) const
{
    GuildMember* guild_member = FindMember(char_guid);
    if (guild_member == NULL) {
        return false;
    };
    guild_member->SerializeMemberInfo(member_info, SERIALIZE_LOAD);

    return true;
}

void 
Guild::GetWorldGuildInfo(WORLD_GUILD_INFO& world_guild_info)
{
    SerializeBaseGuildInfo(world_guild_info, SERIALIZE_LOAD);
    BOOST_STATIC_ASSERT(sizeof(world_guild_info.m_szGuildNotice) == sizeof(guild_notice_));

    CopyMemory(world_guild_info.m_szGuildNotice, guild_notice_,
               sizeof(world_guild_info.m_szGuildNotice));
}

bool 
Guild::IsFullMember() const
{
    const GuildRankInfo* guild_rank_info = 
        GuildRankInfoParser::Instance()->FindGuildRankInfo(guild_info_.m_GuildGrade);
    if (guild_rank_info == NULL) {
        return false;
    };
    return (GetMemberCount() >= guild_rank_info->max_member_count_);
}

void 
Guild::ChangeGuildName(const TCHAR* guild_name)
{
    util::GUILDNAMECOPY(guild_info_.m_tszGuildName, guild_name);
}

int 
Guild::GetRelationGuid(STLX_VECTOR<GUILDGUID>& vGuildGuid,
                           eGUILD_RELATION guild_relation) const
{
    int size = guild_relation_entry_.GetRelationCount(guild_relation);
    vGuildGuid.reserve(size);

    return guild_relation_entry_.GetRelationGuid(vGuildGuid, guild_relation);
}

#ifdef _NA_004034_20120102_GUILD_INTRODUCTION
void Guild::SetGuildIntroduction( const CHAR* introduction )
{
    memset(guild_introduction_, 0, sizeof(guild_introduction_));
    strncpy(guild_introduction_, introduction, MAX_GUILD_INTRODUCTION_LENGTH);
}

const CHAR* Guild::GetGuildIntroduction() const
{
    return guild_introduction_;
}
#endif //_NA_004034_20120102_GUILD_INTRODUCTION

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

const RC::eGUILD_RESULT Guild::_CanGuildJoinRequest( const CHARGUID char_guid ) const
{
    GUILD_JOIN_REQUEST_MAP_CONST_ITERATOR iterator = guild_join_request_list_.find(char_guid);
    if (iterator != guild_join_request_list_.end())
    {
        return RC::RC_GUILD_JOIN_REQUEST_ALREADY_EXIST;
    }

    if (guild_join_request_list_.size() >= MAX_GUILD_JOIN_REQUEST_NUM)
    {
        return RC::RC_GUILD_JOIN_REQUEST_FULL_LIST;
    }

    return RC::RC_GUILD_SUCCESS;
}

const RC::eGUILD_RESULT Guild::_InsertGuildJoinRequest( GUILD_JOIN_REQUEST_INFO* guild_join_request_info )
{
    RC::eGUILD_RESULT result = _CanGuildJoinRequest(guild_join_request_info->m_char_guid);
    if (result != RC::RC_GUILD_SUCCESS)
    {
        return result;
    }

    guild_join_request_list_.insert(std::make_pair<CHARGUID,GUILD_JOIN_REQUEST_INFO>
                                    (guild_join_request_info->m_char_guid, *guild_join_request_info));

    return RC::RC_GUILD_SUCCESS;
}

const RC::eGUILD_RESULT Guild::_DeleteGuildJoinRequest( const CHARGUID char_guid )
{
    GUILD_JOIN_REQUEST_MAP_ITERATOR iterator = guild_join_request_list_.find(char_guid);
    if (iterator == guild_join_request_list_.end())
    {
        return RC::RC_GUILD_JOIN_REQUEST_NOT_EXIST;
    }

    guild_join_request_list_.erase(iterator);
    
    return RC::RC_GUILD_SUCCESS;
}

const void Guild::_UpdateGuildJoinRequest( const time_t current_time )
{
    GUILD_JOIN_REQUEST_MAP_ITERATOR iterator = guild_join_request_list_.begin();
    GUILD_JOIN_REQUEST_MAP_ITERATOR iter_end = guild_join_request_list_.end();

    STLX_VECTOR<GUILD_JOIN_REQUEST_INFO> temp_guild_join_request_cancel;

    for (; iterator != iter_end; ++iterator)
    {
        CHARGUID char_guid = iterator->first;
        GUILD_JOIN_REQUEST_INFO* guild_join_request_info = &(iterator->second);

        double diff_time = difftime(current_time, guild_join_request_info->m_request_time);
        guild_join_request_info->m_remain_time = MAX_GUILD_JOIN_REQUEST_TIME_MAX - diff_time;

        if (diff_time > MAX_GUILD_JOIN_REQUEST_TIME_MAX)
        {
            temp_guild_join_request_cancel.push_back(iterator->second);
        }
    }

    STLX_VECTOR<GUILD_JOIN_REQUEST_INFO>::iterator temp_cur_iter = temp_guild_join_request_cancel.begin();
    STLX_VECTOR<GUILD_JOIN_REQUEST_INFO>::iterator temp_end_iter = temp_guild_join_request_cancel.end();

    for (; temp_cur_iter != temp_end_iter; ++temp_cur_iter)
    {
        MSG_GZ_GUILD_JOIN_REQUEST_REJECT_SYN msg;
        msg.reject_type = eGUILD_JOIN_REJECT_TYPE_TIMEOUT;
        msg.guild_guid = temp_cur_iter->m_guild_guid;
        msg.target_guid = temp_cur_iter->m_char_guid;
        g_DBProxyServer.SendToDBPServer(&msg, sizeof(msg));
    }

    temp_guild_join_request_cancel.clear();
}

const void Guild::_GetGuildJoinRequestTotalInfo( OUT GUILD_JOIN_REQUEST_TOTAL_INFO& guild_join_request_total_info )
{
    int count = 0;

    GUILD_JOIN_REQUEST_MAP_ITERATOR iterator = guild_join_request_list_.begin();
    GUILD_JOIN_REQUEST_MAP_ITERATOR iter_end = guild_join_request_list_.end();
    for (; iterator != iter_end; ++iterator)
    {
        CHARGUID char_guid = iterator->first;
        GUILD_JOIN_REQUEST_INFO* guild_join_request_info = &(iterator->second);

        guild_join_request_total_info.m_GuildJoinRequestInfo[count] = *guild_join_request_info;

        count++;
        if (count >= MAX_GUILD_JOIN_REQUEST_NUM)
        {
            break;
        }
    }
}

const RC::eGUILD_RESULT Guild::_GetGuildJoinRequest( const CHARGUID char_guid, OUT GUILD_JOIN_REQUEST_INFO& info ) const
{
    GUILD_JOIN_REQUEST_MAP_CONST_ITERATOR iterator = guild_join_request_list_.find(char_guid);
    if (iterator == guild_join_request_list_.end())
    {
        return RC::RC_GUILD_JOIN_REQUEST_NOT_EXIST;
    }

    info = (iterator->second);

    return RC::RC_GUILD_SUCCESS;
}

GuildFacilities*const Guild::GetGuildFacilities()
{
    return guild_facilities_;
}

const void Guild::FindMembersByDuty( eGUILD_DUTY guild_duty, CorpsMemberMap& member_map )
{
    GUILD_CORPS_MAP::const_iterator it = guild_corps_list_.begin();
    const GUILD_CORPS_MAP::const_iterator end = guild_corps_list_.end();
    for (; it != end; ++it)
    {
        GuildCorps* guild_corps = it->second;
        if (guild_corps)
        {
            guild_corps->FindMembersByDuty(guild_duty, member_map);
        }
    }
}

#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION