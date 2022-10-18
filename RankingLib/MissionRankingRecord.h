#pragma once
#ifndef RANKING_LIB_MISSION_RANKING_RECORD_H
#define RANKING_LIB_MISSION_RANKING_RECORD_H

//==================================================================================================
/// MissionRankingRecord class
/**
    @author Kim Min Wook < taiyo@webzen.com >
    @since  2006. 8. 22
    @remark
        - 랭킹 정보 기본 단위
    @note
        - 
    @history 
        - 2006. 8. 25 : client에서도 사용하기 위해 CharName을 키로 사용
        - ~2011.05.12,
        - 2011.05.12, waverix, code rearrangements
*/
//==================================================================================================

#include "./RankingMember.h"
#include <functional_custom.h>

class ArchiveOfStream;

class RankingManager;
class MissionRanking;
//==================================================================================================
//
class MissionRankingRecord
{
    __DECL_CUSTOMPOOL_PTR(MissionRankingRecord);
public:
    typedef STLX_HASH_MAP<DWORD, RankingMember*> MEMBER_HASH;

    MissionRankingRecord();
    ~MissionRankingRecord();
    //
    void Clone(const MissionRankingRecord* record);
    void Release();
    // properties
    const MEMBER_HASH& GetMemberHash() const;

    RANKINGKEY GetKey() const;
    DWORD GetClearTime() const;
    int64_t GetRegisterTime() const;
    // 자동 계산
    DWORD GetAvrgLV() const;
    // 자동 계산
    DWORD GetTotalLV() const;
    // 클라이언트용 계산된 값
    DWORD GetAvrgLVClient() const;
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    RANKING GetRanking() const;
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR

    void AddMember(RankingMember* ranking_member);
    void AddMember_Info(BYTE reward, BYTE class_type, LEVELTYPE level,
        const TCHAR* char_name, const TCHAR* guild_name);
    RankingMember* FindMember(const TCHAR* charname) const;
    int GetMemberCount() const;

    void Serialize(ArchiveOfStream& ar, eRANKING_SERIALIZE se);
    void SerializeForClient(ArchiveOfStream& ar, eRANKING_SERIALIZE se);
    void SerializeMember(ArchiveOfStream& ar, eRANKING_SERIALIZE se);
    void Serialize(_MISSION_RANKING_TOTAL_INFO& totalInfo, eSERIALIZE se);
#ifdef _SERVER
    void Display() const;
#endif
private:
    void SetKey(RANKINGKEY key);
    void SetClearTime(DWORD clear_time);
    void SetRegisterTime(int64_t register_time);
    void SetAvrgLV(DWORD average_level);
    void SetTotalLV(DWORD total_level);
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    void SetRanking(RANKING _ranking);
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    //
    RANKINGKEY ranking_key_;
    DWORD clear_time_;
    int64_t register_time_;
    DWORD average_level_;
    DWORD total_level_;
    DWORD average_level_for_client_;
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    RANKING ranking_;
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    //
    MEMBER_HASH member_table_;
    //
    friend class RankingManager;
    friend class MissionRanking;
};

//==================================================================================================
// inline implements

inline int MissionRankingRecord::GetMemberCount() const {
    return int(member_table_.size());
}

inline const MissionRankingRecord::MEMBER_HASH& MissionRankingRecord::GetMemberHash() const {
    return member_table_;
}

inline RANKINGKEY MissionRankingRecord::GetKey() const {
    return ranking_key_;
};

inline DWORD MissionRankingRecord::GetClearTime() const {
    return clear_time_;
};

inline int64_t MissionRankingRecord::GetRegisterTime() const {
    return register_time_;
};

// 자동 계산
inline DWORD MissionRankingRecord::GetAvrgLV() const {
    return average_level_;
};

// 자동 계산
inline DWORD MissionRankingRecord::GetTotalLV() const {
    return total_level_;
};

// 클라이언트용 계산된 값
inline DWORD MissionRankingRecord::GetAvrgLVClient() const {
    return average_level_for_client_;
};

#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
inline RANKING MissionRankingRecord::GetRanking() const {
    return ranking_;
};
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR

inline void MissionRankingRecord::SetKey(RANKINGKEY key) {
    ranking_key_ = key;
};

inline void MissionRankingRecord::SetClearTime(DWORD clear_time) {
    clear_time_ = clear_time;
};

inline void MissionRankingRecord::SetRegisterTime(int64_t register_time) {
    register_time_ = register_time;
};

inline void MissionRankingRecord::SetAvrgLV(DWORD average_level) {
    average_level_ = average_level;
};

inline void MissionRankingRecord::SetTotalLV(DWORD total_level) {
    total_level_ = total_level;
};

#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
inline void MissionRankingRecord::SetRanking(RANKING _ranking) {
    ranking_ = _ranking;
};
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR

__inline void FREE_RANKING_RECORD(MissionRankingRecord* record)
{
    record->Release();
    MissionRankingRecord::FREE(record);
}

#endif //RANKING_LIB_MISSION_RANKING_RECORD_H