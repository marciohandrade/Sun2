#pragma once
#ifndef RANKING_LIB_MISSION_RANKING_H
#define RANKING_LIB_MISSION_RANKING_H

//==================================================================================================
/// MissionRanking class
/**
    @author Kim Min Wook < taiyo@webzen.com >
    @since  2006. 8. 22
    @remark
        - 미션 하나당 랭킹 정보의 단위 클래스
    @note
        - 초기화시 순위로 정렬된 상태로 존재한다.
    @history 
        - ~2011.05.12,
        - 2011.05.12, waverix, code rearrangements
*/
//==================================================================================================

#include "./MissionRankingRecord.h"

class ArchiveOfStream;
class RankingManager;
//==================================================================================================
//
class MissionRanking
{
    __DECL_CUSTOMPOOL_PTR(MissionRanking);
public:
    typedef STLX_VECTOR<MissionRankingRecord*> RANKING_INFO_LIST;
    //
    MissionRanking();
    ~MissionRanking();

    void Release();

    int GetRecordCount() const;
    MissionRankingRecord* FindRanking(RANKINGKEY key) const;
    // properties
    RANKINGNO GetMissionNo() const;
    eRANKING_SYSTEM_TYPE GetRankingType() const;

    // ranking information building interfaces using streaming
    void Serialize(BYTE page, ArchiveOfStream& ar, eRANKING_SERIALIZE se);
    void SerializeForClient(BYTE page, ArchiveOfStream& ar, eRANKING_SERIALIZE se);

    // ranking information building interfaces
    void AddRanking(RANKINGKEY key, MissionRankingRecord* record);
    MissionRankingRecord* AddRanking_Info(RANKINGKEY key, DWORD clear_time, int64_t register_time);
    void Sort();
    void RemoveTailOrderRecord(int count = 1);
    RANKING GetRanking(RANKINGKEY key) const;
    MissionRankingRecord* GetRecordOrder1() const;
    RANKING_INFO_LIST& GetMemberHash();
#ifdef _SERVER
    void Display() const;
#endif

#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
    //자신이 속한 랭킹레코드(파티)를 찾는다.
    MissionRankingRecord* FindMyBestRankingRecord(const TCHAR* _charname, eRANKING_SYSTEM_TYPE _ranking_system_type);
    //보상받을수있는 미션리스트중 아직 보상 안받은 리스트를 가져온다.
    void GetRewardableMissionList(const TCHAR* _charname, RANKINGID id, 
                                  STLX_VECTOR<REWARDABLE_MISSION_LIST>& mission_list_container);
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM

private:
    typedef STLX_HASH_MAP<RANKINGKEY, MissionRankingRecord*> RANKING_INFO_HASH;

    void SetMissionNo(RANKINGNO mission_number);
    void SetRankingType(eRANKING_SYSTEM_TYPE ranking_type);

#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    bool SetRankingAllRecords();
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    //
    // data fields...
    RANKINGNO mission_number_;
    uint8_t ranking_type_;

    RANKING_INFO_HASH ranking_info_table_;
    RANKING_INFO_LIST ranking_info_list_;
    //
    friend class RankingManager;
};

//==================================================================================================
// inline implements

inline int MissionRanking::GetRecordCount() const {
    return static_cast<int>(ranking_info_list_.size());
}

inline MissionRankingRecord* MissionRanking::FindRanking(RANKINGKEY key) const
{
    RANKING_INFO_HASH::const_iterator it = ranking_info_table_.find(key);
    if (it == ranking_info_table_.end()) {
        return NULL;
    };
    MissionRankingRecord* record = it->second;
    return record;
}

inline MissionRankingRecord* MissionRanking::GetRecordOrder1() const
{
    return ranking_info_list_.front();
}

inline MissionRanking::RANKING_INFO_LIST&
MissionRanking::GetMemberHash() {
    return ranking_info_list_;
}

inline RANKINGNO MissionRanking::GetMissionNo() const {
    return mission_number_;
};

inline eRANKING_SYSTEM_TYPE MissionRanking::GetRankingType() const {
    return static_cast<eRANKING_SYSTEM_TYPE>(ranking_type_);
}

inline void MissionRanking::SetMissionNo(RANKINGNO mission_number) {
    mission_number_ = mission_number;
}

inline void MissionRanking::SetRankingType(eRANKING_SYSTEM_TYPE ranking_type) {
    ranking_type_ = static_cast<uint8_t>(ranking_type);
}

//
__inline void FREE_RANKING(MissionRanking* p)
{
    p->Release();
    MissionRanking::FREE(p);
}

#endif //RANKING_LIB_MISSION_RANKING_H