#include "StdAfx.h"
#include ".\MissionRankingRecord.h"

#include <ArchiveOfStream.h>

#include <algorithm>

//==================================================================================================

__IMPL_CUSTOMPOOL_PTR(MissionRankingRecord);

//==================================================================================================

MissionRankingRecord::MissionRankingRecord()
    : ranking_key_(0)
    , clear_time_(0)
    , register_time_(0)
    , average_level_(0)
    , total_level_(0)
    , average_level_for_client_(0)
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    , ranking_(0)
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
{
}

MissionRankingRecord::~MissionRankingRecord()
{
}

void MissionRankingRecord::Clone(const MissionRankingRecord* record)
{
    this->Release();
    ;{
        ranking_key_ = record->ranking_key_;
        clear_time_  = record->clear_time_;
        register_time_ = record->register_time_;
        average_level_ = record->average_level_;
        total_level_ = record->total_level_;
        average_level_for_client_ = record->average_level_for_client_;
    };

    const MEMBER_HASH& src_member_table = record->member_table_;
    FOREACH_CONTAINER(const MEMBER_HASH::value_type& node, src_member_table, MEMBER_HASH)
    {
        const RankingMember* const src_member = node.second;
        RankingMember* const dest_member = RankingMember::ALLOC();
        dest_member->Clone(src_member);
        AddMember(dest_member);
    }
}

void MissionRankingRecord::Release()
{
    ranking_key_ = 0;
    clear_time_ = 0;
    register_time_ = 0;
    average_level_ = 0;
    total_level_ = 0;
    average_level_for_client_ = 0;

    FOREACH_CONTAINER(const MEMBER_HASH::value_type& node, member_table_, MEMBER_HASH)
    {
        RankingMember* member = node.second;
        FREE_MEMBER(member);
    };
    member_table_.clear();
}

RankingMember* MissionRankingRecord::FindMember(const TCHAR* charname) const
{
    DWORD char_name_hash = RankingMember::MakeCharNameHashKey(charname);
    if (char_name_hash == 0) {
        return NULL;
    };
    MEMBER_HASH::const_iterator it = member_table_.find(char_name_hash);
    if (it == member_table_.end()) {
        return NULL;
    };
    RankingMember* member = it->second;
    return member;
}

void MissionRankingRecord::Serialize(_MISSION_RANKING_TOTAL_INFO& totalInfo, eSERIALIZE se)
{
    if (se == SERIALIZE_STORE)
    {
        Release();

        clear_time_ = totalInfo.m_Info.m_ClearTime;
        register_time_ = totalInfo.m_Info.m_RegisterTime;

        for (BYTE c = 0 ; c < totalInfo.m_Count ; ++c)
        {
            RankingMember* ranking_member = RankingMember::ALLOC();
            ranking_member->Serialize(totalInfo.m_Member[c], se);
            AddMember(ranking_member);
        }
    }
    else if (se == SERIALIZE_LOAD)
    {
        totalInfo.m_Info.m_ClearTime    = clear_time_;
        totalInfo.m_Info.m_RegisterTime = register_time_;
        totalInfo.m_Count = (BYTE)member_table_.size();

        MEMBER_HASH::iterator it = member_table_.begin(), end = member_table_.end();
        for (BYTE c = 0 ; it != end ; ++it, ++c)
        {
            (it->second)->Serialize(totalInfo.m_Member[c], se);
        }
    }
}

void MissionRankingRecord::AddMember(RankingMember* ranking_member)
{
    DWORD char_name_hash = ranking_member->GetCharNameHash();
    if (FlowControl::FCAssert(char_name_hash != 0) == false) {
        return;
    };
    MEMBER_HASH::_Pairib result = member_table_.insert(
        MEMBER_HASH::value_type(char_name_hash, ranking_member));
    ASSERT(result.second && "랭킹맴버 추가 실패");
    if (result.second) {
        total_level_ += ranking_member->GetLV();
        average_level_ = static_cast<DWORD>(total_level_ / member_table_.size());
    };
}

void MissionRankingRecord::AddMember_Info(
    BYTE reward, BYTE class_type, LEVELTYPE level,
    const TCHAR* char_name, const TCHAR* guild_name)
{
    RankingMember* ranking_member = RankingMember::ALLOC();
    ranking_member->Release();
    ranking_member->rewarded_ = reward;
    ranking_member->class_type_ = class_type;
    ranking_member->level_ = level;
    ranking_member->SetCharName(char_name);
    ranking_member->SetGuildName(guild_name);
    AddMember(ranking_member);
}

void MissionRankingRecord::Serialize(ArchiveOfStream& ar, eRANKING_SERIALIZE se)
{
    if (se == RANKING_SERIALIZE_SAVE)
    {
        this->Release();

        ar >> ranking_key_;
        ar >> clear_time_;
        ar >> register_time_;
        BYTE number_of_members = 0;
        ar >> number_of_members;
        for (int countdown = number_of_members; countdown != 0; --countdown)
        {
            RankingMember* ranking_member = RankingMember::ALLOC();
            ranking_member->Serialize(ar, se);
            AddMember(ranking_member);
        };
    }
    else if (se == RANKING_SERIALIZE_LOAD)
    {
        ar << ranking_key_;
        ar << clear_time_;
        ar << register_time_;
        ar << BYTE(member_table_.size());
        FOREACH_CONTAINER(const MEMBER_HASH::value_type& node, member_table_, MEMBER_HASH)
        {
            RankingMember* member = node.second;
            member->Serialize(ar, se);
        };
    }
}

// LEVELTYPE의 레벨이 255를 넘어갈경우 변경 필요!!!!!
void MissionRankingRecord::SerializeForClient(ArchiveOfStream& ar, eRANKING_SERIALIZE se)
{
    if (se == RANKING_SERIALIZE_CLIENT_SAVE)
    {
        this->Release();

        ar >> ranking_key_;
        ar >> clear_time_;
        BYTE average_level = 0;
        ar >> average_level; //< 클라이언트만 사용(1byte)캐스팅
        average_level_for_client_ = average_level;

    }
    else if (se == RANKING_SERIALIZE_CLIENT_LOAD)
    {
        ar << ranking_key_;
        ar << clear_time_;
        assert((average_level_ <= UCHAR_MAX));
        ar << (BYTE)average_level_;               //< 클라이언트만 사용(1byte)캐스팅
    }
}

//SerializeForClient() 다음에 SerializeMember()가 사용되어야 한다.
void MissionRankingRecord::SerializeMember(ArchiveOfStream& ar, eRANKING_SERIALIZE se)
{
    if (se == RANKING_SERIALIZE_CLIENT_SAVE)
    {
        total_level_   = 0;
        average_level_    = 0;
        BYTE number_of_members = 0;
        ar >> number_of_members;

        for (int countdown = number_of_members; countdown != 0; --countdown)
        {
            RankingMember* ranking_member = RankingMember::ALLOC();
            ranking_member->Serialize(ar, se);
            AddMember(ranking_member);
        };
    }
    else if (se == RANKING_SERIALIZE_CLIENT_LOAD)
    {
        // 맴버수
        ar << BYTE(member_table_.size());
        FOREACH_CONTAINER(const MEMBER_HASH::value_type& node, member_table_, MEMBER_HASH)
        {
            RankingMember* member = node.second;
            member->Serialize(ar, se);
        };
    }
}

#ifdef _SERVER
void MissionRankingRecord::Display() const
{
    CONSOLELOG(eCRITICAL_LOG,
        _T("RankingRecord: Key=%d, ClearTime=%d, RegisterTime=%I64d,"),
        ranking_key_, clear_time_, register_time_);

    FOREACH_CONTAINER(const MEMBER_HASH::value_type& node, member_table_, MEMBER_HASH)
    {
        const RankingMember* member = node.second;
        member->Display();
    };
}
#endif //_SERVER