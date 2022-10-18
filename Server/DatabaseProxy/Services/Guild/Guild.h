#pragma once
#ifndef DBPROXY_GUILD_H
#define DBPROXY_GUILD_H
//==================================================================================================
/// Guild class
/**
    @author Kim Min Wook < taiyo@webzen.co.kr >
    @since  2005. 12. 26
    @remark
        - 길드를 나타내는 클래스
    @note
        - 길드는 실시간 처리를 한다.
        - XXX 와 OnXXX 계열의 함수는 서로 1:1 대응된다. 
        - OnXXX 계열의 함수는 DBR로 결과값이 온 후 호출되는 콜벡스타일의 함수이다.
        - 길드를 받아 오게 되면, 모든 서버에게 broadcast한다.
        - 수정 변경 사항도 모든 서버에게 broadcast한다.
    @history 
        - ~2011.05.18
        - 2011.05.18, waverix, The Guild System is migrated from GuildServer
        - 2011.07.22, waverix, code improvements
*/
//==================================================================================================

#include "GuildCommon.h"

#include "GuildCorps.h"
#include "GuildRelation.h"
#include "GuildSmallContentEntry.h"

struct MSG_BASE;
struct BASE_GUILD_INFO;
struct BASE_GUILD_MEMBER_INFO;

class GuildMember;
class GuildRankInfo;

#ifdef _NA_003923_20120130_GUILD_RENEWAL
class GuildInGameLog;
#endif

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
class GuildFacilities;
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION

//==================================================================================================

class Guild
{
    __DECL_GUILDPOOL(Guild);
public:
    Guild();
    virtual ~Guild();

public:
    void InitializeGuild();

    void ReleaseGuild();

    bool CanDestroy();

    void LogOutMember(
        CHARGUID char_guid
        );
    
    void SetGuildInfo(
        BASE_GUILD_INFO& IN base_guild_info
        );
    
    GuildMember* LogInMember(
        CHARGUID char_guid
        );

    GuildMember* FindMember(
        CHARGUID char_guid
        ) const;

    GuildMember* FindMemberByCharName(
        const TCHAR *char_name
        ) const;

    void RankUp();

    RC::eGUILD_RESULT CanRankUp(
        OUT GuildRankInfo& guild_rank_info
        ) const;

    bool IsFullMember() const;

    bool SetMemberDuty(
        CHARGUID char_guid, 
        eGUILD_DUTY guild_duty
        );

    bool IsExistMemberDuty(
        eGUILD_DUTY guild_duty, 
        BYTE corps_number
        ) const;

    void CreateGuild(
        GUILDGUID guild_guid, 
        const TCHAR* guild_name, 
        BYTE corps_number,
        CHARGUID master_char_guid, 
        const TCHAR* master_char_name,
        eGUILD_DUTY master_guild_duty,
        BYTE master_char_class, 
        LEVELTYPE master_char_level,
        BYTE chao_state,
        int64_t guild_Panalty_time,
        int64_t destroy_request_time,
        int64_t master_change_time
        );

    WORD GetMemberCount() const;

    GuildCorps* CreateCorps(
        BYTE corps_number
        );

    void DestroyCorps(
        BYTE corps_number
        );

    GuildCorps* CreateCorpsBySequence();

    BYTE GetEmptyCorpsNumBySequence() const;
    
    GuildCorps* FindCorps(
        int corps_number
        ) const; // corps_number < MAX_GUILD_CORPS_NUM

    BYTE GetCorpsCount() const;

    GuildMember* FindMaster() const;
    
    const TCHAR* GetMasterCharName() const;
    
    GuildMember* FindMemberByDuty(
        eGUILD_DUTY guild_duty
        ) const;

    BYTE GetEmptyCommanderInfo(
        BYTE* empty_commander_info
        ) const;

    RC::eGUILD_RESULT JoinGuild(
        BYTE corps_number, 
        CHARGUID char_guid, 
        const TCHAR* char_name,
        eGUILD_DUTY guild_duty,
        BYTE char_class, 
        LEVELTYPE char_level,
        BYTE chao_state
        );

    RC::eGUILD_RESULT LeaveGuild(
        CHARGUID char_guid
        );

    RC::eGUILD_RESULT ChangeCorps(
        CHARGUID char_guid, 
        BYTE new_corps_number
        );

    bool IsFullCorpsMemberByDuty(
        BYTE corps_number, 
        eGUILD_DUTY guild_duty
        ) const;

    bool CreateRearCamp(
        BYTE corps_number
        );

    bool DestroyRearCamp(
        BYTE corps_number
        );

    RC::eGUILD_RESULT DestroyMember(
        CHARGUID char_guid
        );

    bool GetBaseMemberInfo(
        CHARGUID char_guid, 
        OUT BASE_GUILD_MEMBER_INFO& info
        ) const;

    bool GetMemberInfo(
        CHARGUID char_guid, 
        OUT GUILD_MEMBER_INFO& info
        ) const;

    void ChangeGuildName(
        const TCHAR* guild_name
        );

    int GetRelationGuid(
        STLX_VECTOR<GUILDGUID>& guild_guid_vector,
        eGUILD_RELATION relation_type = eGUILD_RELATION_NONE
        ) const;

    void SetGuildName(
        const TCHAR* guild_name
        );

    void SetGuildGP(
        GPTYPE gp
        );

    void SetGuildUP(
        UPTYPE up
        );

    void SetGuildGrade(
        BYTE grade
        );

    void SetGuildGuid(
        GUILDGUID guild_guid
        );

    GPTYPE GetGuildGP() const;

    UPTYPE GetGuildUP() const;
    
    BYTE GetGuildGrade() const;
    
    GUILDGUID GetGuildGuid() const;
    
    const TCHAR* GetGuildName() const;
    
    void GetWorldGuildInfo(
        WORLD_GUILD_INFO& rInfo
        );

    void SetMasterChangeTime(
        int64_t nChangeTime
        );

    int64_t GetMasterChangeTime() const;

    //Serialize
    void SerializeBaseGuildInfo(
        INOUT BASE_GUILD_INFO& base_guild_info, 
        eSERIALIZE se
        );

    void SerializeGameGuildInfo(
        INOUT GAME_GUILD_INFO& game_guild_info, 
        eSERIALIZE se
        );

    void SerializeGuildInfo(
        INOUT GUILD_INFO& guild_info, 
        eSERIALIZE se
        );

    void SerializeCorpsInfo(
        INOUT GUILD_CORPS_TOTAL_INFO& corps_total_info, 
        eSERIALIZE se
        );

    void SerializeBaseMemberTotalInfo(
        INOUT BASE_GUILD_MEMBER_TOTAL_INFO& base_member_total_info, 
        eSERIALIZE se
        );

    void SerializeMemberTotalInfo(
        INOUT GUILD_MEMBER_TOTAL_INFO& member_total_info, 
        eSERIALIZE se
        );

    void SerializeServerMemberTotalInfo(
        INOUT SERVER_GUILD_MEMBER_TOTAL_INFO& server_member_total_info, 
        eSERIALIZE se
        );

    const TCHAR* GetGuildNotice() const;

    void SetGuildNotice(
        const util::StrArrayForGuildNotice& guild_notice
        );

    void SetPenaltyTime(
        GuildSmallContentEntry::GuildPenalty::PenaltyTime PenaltyTime
        );

    void SetDestroyTime(
        GuildSmallContentEntry::GuildPenalty::PenaltyTime DestroyTime
        );

    void SetDeleteGuildState(
        eGUILD_DELCHK eDelChk
        );

    void DeleteGuildMark();
    
    void RegisterGuildMark(GUILDMARKIDX i, GUILDMARKIDX p);
    
    void AcquireGuildMark(GUILDMARKIDX& b, GUILDMARKIDX& p);
    
    GuildSmallContentEntry::GuildPenalty::PenaltyTime GetPenaltyTime() const;
    
    GuildSmallContentEntry::GuildPenalty::PenaltyTime GetDestroyTime() const;
    
    GuildSmallContentEntry::GuildPenalty::PenaltyTime GetDeleteGuildState() const;
    
    GuildRelationSystem::GuildRelationEntry* GetRelationEntry();
    
    eGUILD_DELCHK CheckGuildDeleteState() const;

#ifdef _NA_003923_20120130_GUILD_RENEWAL
    bool IsFullMemberOfDuty(
        eGUILD_DUTY guild_duty
        );

    GuildInGameLog* GetGuildInGameLog();
#endif

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
    CHUNTING_PVP_INFO* GetChuntingPVPInfo() { return &m_ChuntingPVPInfo; }
#endif

    //_NA_20100604_BETTER_DOMINATION_MANAGER
    void SetDominationJointCount(
        WORD count
        );

    void SetDominationDefenseCount(
        WORD count
        );

    void SetDominationHuntingCount(
        WORD count
        );

    WORD GetDominationJoinCount() const;
    
    WORD GetDominationDefenseCount() const;
    
    WORD GetDominationHuntingCount() const;
#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
    void  SetGuildCoin(DWORD guild_coin);
    DWORD GetGuildCoin() const;
    void  SetGuildPoint(DWORD guild_point);
    DWORD GetGuildPoint() const;
#endif //_NA004034_20120102_GUILD_POINT_SYSTEM

#ifdef _NA_004034_20120102_GUILD_INTRODUCTION
    void SetGuildIntroduction(const CHAR* introduction);
    const CHAR* GetGuildIntroduction() const;
#endif //_NA_004034_20120102_GUILD_INTRODUCTION
#ifdef _NA_008334_20150608_SONNENSCHEIN
    VOID SetSonnenScheinPhase(const BYTE phase) {guild_info_.sonnenschein_phase_ = phase;}
    const BYTE GetSonnenScheinPhase() {return guild_info_.sonnenschein_phase_;}
#endif //_NA_008334_20150608_SONNENSCHEIN

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    GuildFacilities*const   GetGuildFacilities();
    const void FindMembersByDuty(eGUILD_DUTY guild_duty, CorpsMemberMap& member_map);
    // 길드 가입 요청 목록 인터페이스 // 길드매니저의 컨테이너와 함께 삽입/삭제가 이루어 져야한다.
    const RC::eGUILD_RESULT _GetGuildJoinRequest(const CHARGUID char_guid, OUT GUILD_JOIN_REQUEST_INFO& info) const;
    const RC::eGUILD_RESULT _InsertGuildJoinRequest(GUILD_JOIN_REQUEST_INFO* guild_join_request_info);
    const RC::eGUILD_RESULT _DeleteGuildJoinRequest(const CHARGUID char_guid);
    const RC::eGUILD_RESULT _CanGuildJoinRequest(const CHARGUID char_guid) const;

    const void _UpdateGuildJoinRequest(const time_t current_time);
    const void _GetGuildJoinRequestTotalInfo(OUT GUILD_JOIN_REQUEST_TOTAL_INFO& guild_join_request_total_info);
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
private:
    typedef STLX_MAP<int, GuildCorps*> GUILD_CORPS_MAP; // changes type to use shared address
    //typedef GUILD_CORPS_MAP::iterator GUILD_CORPS_MAP_ITER;

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
    CHUNTING_PVP_INFO m_ChuntingPVPInfo;
#endif

    GUILD_INFO guild_info_;
    int64_t master_change_time_; // 이 값 사용하지 말것... 그냥 보존용이다.
    GUILD_CORPS_MAP guild_corps_list_;
    GuildRelationSystem::GuildRelationEntry guild_relation_entry_;
    TCHAR guild_notice_[MAX_GUILDNOTICE_LENGTH + 1];
    //
    friend class GuildManager;

#ifdef _NA_003923_20120130_GUILD_RENEWAL
    GuildInGameLog* guild_ingame_log_;
#endif
#ifdef _NA_004034_20120102_GUILD_INTRODUCTION
    CHAR guild_introduction_[MAX_GUILD_INTRODUCTION_LENGTH+1];
#endif //_NA_004034_20120102_GUILD_INTRODUCTION

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    // 길드 가입 요청 목록 컨테이너
    typedef STLX_MAP<CHARGUID, GUILD_JOIN_REQUEST_INFO> GUILD_JOIN_REQUEST_MAP;
    typedef GUILD_JOIN_REQUEST_MAP::iterator GUILD_JOIN_REQUEST_MAP_ITERATOR;
    typedef GUILD_JOIN_REQUEST_MAP::const_iterator GUILD_JOIN_REQUEST_MAP_CONST_ITERATOR;
    GUILD_JOIN_REQUEST_MAP  guild_join_request_list_;

    
    // 길드 시설
    GuildFacilities* guild_facilities_;
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    __DISABLE_COPY(Guild);
};

//==================================================================================================

//현재 군단개수
inline BYTE 
Guild::GetCorpsCount() const 
{
    return static_cast<BYTE>(guild_corps_list_.size());
}

inline void 
Guild::SetGuildGP(GPTYPE gp) 
{
    guild_info_.m_GuildPoint = gp;
}

inline void 
Guild::SetGuildUP(UPTYPE up) 
{
    guild_info_.m_UserPoint = up;
}

inline void
Guild::SetGuildGrade(BYTE grade) 
{
    guild_info_.m_GuildGrade = static_cast<BYTE>(grade);
}

inline void 
Guild::SetGuildGuid(GUILDGUID Guid) 
{
    guild_info_.m_GuildGuid = Guid;
}

inline GPTYPE 
Guild::GetGuildGP() const 
{
    return guild_info_.m_GuildPoint;
}

inline UPTYPE 
Guild::GetGuildUP() const 
{
    return guild_info_.m_UserPoint;
}

inline BYTE 
Guild::GetGuildGrade() const 
{
    return guild_info_.m_GuildGrade;
}

inline GUILDGUID 
Guild::GetGuildGuid() const 
{
    return guild_info_.m_GuildGuid;
}

inline const TCHAR* 
Guild::GetGuildName() const 
{
    return guild_info_.m_tszGuildName;
}

inline void 
Guild::SetMasterChangeTime(int64_t nChangeTime)
{
    master_change_time_ = nChangeTime;
}

inline int64_t 
Guild::GetMasterChangeTime() const 
{
    return master_change_time_;
}

//_NA_20100604_BETTER_DOMINATION_MANAGER
inline void 
Guild::SetDominationJointCount(WORD count)
{
    guild_info_.m_DominationJoinCount = count;
}

inline void 
Guild::SetDominationDefenseCount(WORD count)
{
    guild_info_.m_DominationDefenseCount = count;
}

inline void 
Guild::SetDominationHuntingCount(WORD count)
{
    guild_info_.m_DominationHuntingCount = count;
}

inline WORD 
Guild::GetDominationJoinCount() const
{
    return guild_info_.m_DominationJoinCount;
}

inline WORD 
Guild::GetDominationDefenseCount() const
{
    return guild_info_.m_DominationDefenseCount;
}

inline WORD 
Guild::GetDominationHuntingCount() const
{
    return guild_info_.m_DominationHuntingCount;
}

inline const TCHAR* 
Guild::GetGuildNotice() const
{
    return guild_notice_;
}

inline void 
Guild::SetPenaltyTime(GuildSmallContentEntry::GuildPenalty::PenaltyTime PenaltyTime)
{
    guild_info_.m_GuildPenaltyTime = PenaltyTime;
}

inline void 
Guild::SetDestroyTime(GuildSmallContentEntry::GuildPenalty::PenaltyTime DestroyTime)
{
    guild_info_.m_GuildDestroyTime = DestroyTime;
}

inline void 
Guild::SetDeleteGuildState(eGUILD_DELCHK eDelChk)
{
    guild_info_.m_GuildDestroyWaitFlag = eDelChk;
}

inline void 
Guild::DeleteGuildMark()
{ 
    guild_info_.m_PatternIdx= 0; 
    guild_info_.m_BackGroundIdx = 0; 
}

inline void 
Guild::RegisterGuildMark(GUILDMARKIDX i, GUILDMARKIDX p)
{
    guild_info_.m_PatternIdx = p; 
    guild_info_.m_BackGroundIdx = i; 
}

inline void 
Guild::AcquireGuildMark(GUILDMARKIDX& b, GUILDMARKIDX& p)
{ 
    p = guild_info_.m_PatternIdx; 
    b = guild_info_.m_BackGroundIdx; 
}

inline GuildSmallContentEntry::GuildPenalty::PenaltyTime
Guild::GetPenaltyTime() const
{
    return guild_info_.m_GuildPenaltyTime;
}

inline GuildSmallContentEntry::GuildPenalty::PenaltyTime
Guild::GetDestroyTime() const
{
    return guild_info_.m_GuildDestroyTime;
}

inline GuildSmallContentEntry::GuildPenalty::PenaltyTime
Guild::GetDeleteGuildState() const
{
    return guild_info_.m_GuildDestroyWaitFlag;
}

inline GuildRelationSystem::GuildRelationEntry* 
Guild::GetRelationEntry()
{
    return &guild_relation_entry_;
}

inline eGUILD_DELCHK 
Guild::CheckGuildDeleteState() const
{ 
    return guild_info_.m_GuildDestroyWaitFlag;
    //return eGUILD_DELCHK_NORMAL;
}

#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
inline void Guild::SetGuildCoin(DWORD guild_coin) 
{
    guild_info_.m_GuildCoin = guild_coin;
}

inline DWORD Guild::GetGuildCoin() const 
{
    return guild_info_.m_GuildCoin;
}

inline void Guild::SetGuildPoint(DWORD guild_point)
{
    guild_info_.m_GuildGP = guild_point;
}

inline DWORD Guild::GetGuildPoint() const
{
    return guild_info_.m_GuildGP;
}
#endif //_NA004034_20120102_GUILD_POINT_SYSTEM

#ifdef _NA_003923_20120130_GUILD_RENEWAL
inline GuildInGameLog* 
Guild::GetGuildInGameLog()
{
    return guild_ingame_log_;
}
#endif//_NA_003923_20120130_GUILD_RENEWAL

#endif //DBPROXY_GUILD_H