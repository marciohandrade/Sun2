#pragma once
#ifdef _NA_008012_20150130_RANKING_SYSTEM
#ifndef RANKING_LIB_RANKING_MANAGER_H
#define RANKING_LIB_RANKING_MANAGER_H

//==================================================================================================
/// RankingManager class
/**
    @author Kim Min Wook < taiyo@webzen.com >
    @since  2006. 8. 22
    @remark
        - 랭킹을 관리하는 클래스, 현재는 미션만
    @note
        - 랭킹의 종류 : 명예의 전당, 지난주 순위, 현재주 순위
        - 클라이언트+서버 공유 모듈
    @history 
        - ~2011.05.12,
        - 2011.05.12, waverix, code rearrangements
*/
//==================================================================================================

#include <ResultCode.h>
#include <ScriptCode.IParser.h>

#include "./RankingDefine.h"
#include "./RankingStruct.h"
#include "./MissionRanking.h"
#include "./MissionRankingRecord.h"
#include "./RankingMember.h"

//==================================================================================================

enum eRANKING_TYPE
{
#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
// eRANKING_TYPE은 지난주/현재주의 구간을 표시
// eRANKING_SYSTEM_TYPE은 1~5번으로 미션/포인트/전장랭킹을 나타낸다
#else
// 1303패치 이후로 베스트오브베스트 항목은 삭제된다
    eRANKING_FAME,
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM
    eRANKING_LASTWEEK,
    eRANKING_TODAYWEEK,
    eRANKING_MAX,
#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
    eRANKING_MYRANKING,
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM
};

enum eRANKING_STATUS 
{
    eRANKING_STATUS_INIT,
    eRANKING_STATUS_SUSPENDING,
    eRANKING_STATUS_NORMAL,
    //
    eRANKING_STATUS_COUNTS // EOR
};

#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
class GameRankingManager;
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
//==================================================================================================
//
class RankingManager : public IParser
{
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    friend GameRankingManager;
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
// BEGIN ENUM, STRUCT ------------------------------------------------------------------------------
public:
    enum {
        MAX_TODAYWEEK_CUTLINE_NUM = 50,
        MAX_FAME_CUTLINE_NUM = 1,
    };

    struct LEVEL_AREA_BASE
    {
#if defined (_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)
        RANKINGID m_Id;
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM        
        RANKINGNO m_MissionNo;
        LEVELTYPE m_MinLV;
        LEVELTYPE m_MaxLV;
        DWORD     m_MaxMonsterKill;
        BYTE      m_RankingType;
        WORD      m_PointRankingIndex;
#if defined (_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)
        WORD      m_GroupString;
        BYTE      m_GroupOrder;
        WORD      m_MissionString;
        BYTE      m_MissionOrder;
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM
    };

    struct REWARD_BASE
    {
        RANKINGNO   m_MissionNo;
        BYTE        m_Class;        //< 클래스
        WORD        m_Ranking;      //< 순위
#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
        BYTE        reward_type_;  //< 보상 타입
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM
        DWORD       m_RewardCode;   //< 보상 코드
    };
// END ENUM, STRUCT---------------------------------------------------------------------------------
// BEGIN TYPEDEF -----------------------------------------------------------------------------------
public:
    typedef const LEVEL_AREA_BASE LEVEL_AREA;
    typedef const REWARD_BASE REWARD;
#if defined (_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)
    typedef STLX_HASH_MAP<RANKINGID, LEVEL_AREA*> MISSION_LEVEL_AREA_HASH;
#else
    typedef STLX_HASH_MAP<RANKINGNO, LEVEL_AREA*> MISSION_LEVEL_AREA_HASH;
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM

private:
    static const ulong kUpdateInterval = 5 * 1000;
    //
    typedef STLX_HASH_MAP<RANKINGNO, MissionRanking*> MISSION_RANKING;
    typedef STLX_HASH_MAP<DWORD, MissionRankingRecord*> TMP_MISSION_RECORD_HASH;
    //typedef STLX_HASH_MAP<RANKINGKEY, MissionRankingRecord *> MISSION_RECORD_HASH_FOR_KEY;
    typedef STLX_HASH_MAP<DWORD, REWARD*> MISSION_REWARD_HASH;
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    typedef STLX_HASH_MAP<RANKINGID, MissionRanking*> BATTLEGROUND_RANKING;
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    struct SettleInfo; // about ranking settlements
// END TYPEDEF-------------------------------------------------------------------------------------
// BEGIN FUNCTION-----------------------------------------------------------------------------------
public:
    //
    RankingManager();
    virtual ~RankingManager();

    //----------------------------------------------------------------------------------------------
    virtual void Initialize();
    virtual void Update(); // CHANGES: f110511.1L

    // IParser interfaces
    virtual BOOL LoadScript(eSCRIPT_CODE script_code, BOOL reloaded);
    virtual void Release();
    // properties
    const MISSION_LEVEL_AREA_HASH& GetMissionLevelAreaTable() const;
    //----------------------------------------------------------------------------------------------
    // acquisition interfaces using script data = {
#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
    LEVEL_AREA* FindLVArea(RANKINGNO no, RANKINGTYPE ranking_type = 0) const;
    LEVEL_AREA* FindLVArea_ID(RANKINGID id) const;
    REWARD* FindReward(RANKINGNO mission_no, BYTE class_type, WORD ranking, BYTE reward_type) const;
    const RANKINGID FindRankingID(RANKINGNO mission_no, RANKINGTYPE ranking_type) const;
#else
    LEVEL_AREA* FindLVArea(RANKINGNO no) const;
    REWARD* FindReward(RANKINGNO mission_no, BYTE class_type, WORD ranking) const;
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM
    eRANKING_SYSTEM_TYPE GetRankingType(MAPCODE map_code) const;
    WORD GetPointRankingIndex(MAPCODE map_code) const;
    // }
    // 정산 시작 날짜(ymd y:m:s), 정산 주기(날수)
    void InitComputation(int year, int month, int day, int hour, int minute, int second,
                         int period, int suspend_time);
    void ClearRanking();

    //----------------------------------------------------------------------------------------------
    // 바이트 스트림 직렬화 하는 메서드 : 랭킹 정보 요청에 대한 응답용
    //void Serialize(BYTE* INOUT stream, WORD& INOUT size, eRANKING_SERIALIZE se);
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    void Serialize(eRANKING_TYPE type, RANKINGNO no, BYTE page, 
        BYTE* INOUT stream, WORD& INOUT size,
        eRANKING_SERIALIZE se, RANKINGTYPE ranking_type = 0);
    void SerializeForClient(eRANKING_TYPE type, RANKINGNO no, BYTE page, 
        BYTE* INOUT stream, WORD& INOUT size,
        eRANKING_SERIALIZE se, RANKINGTYPE ranking_type = 0);
#else
    void Serialize(eRANKING_TYPE type, RANKINGNO no, BYTE page, 
        BYTE* INOUT stream, WORD& INOUT size,
        eRANKING_SERIALIZE se);
    void SerializeForClient(eRANKING_TYPE type, RANKINGNO no, BYTE page, 
        BYTE* INOUT stream, WORD& INOUT size,
        eRANKING_SERIALIZE se);
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    //----------------------------------------------------------------------------------------------
    // 미션 클리어시 기록용 메서드
    // called by 'CanRegisterMissionRecord' x 2
    // known inputs = { MISSION_RANK_MEMBER_INFO, _MISSION_RANK_MEMBER_INFO }
    template<class MEMBER_INFO_TYPE>
    RC::eRANKING_RESULT CanRegisterMissionRecord(RANKINGNO mission_no,
        BYTE number_of_members, const MEMBER_INFO_TYPE* const member_info_list) const;

    // 서버용
    RC::eRANKING_RESULT CanRegisterMissionRecord(
        const _MISSION_RANKING_TOTAL_INFO& ranking_total_info) const;
    //
    DWORD RegisterMissionRecordInTemp(_MISSION_RANKING_TOTAL_INFO& rTotalInfo, DWORD GenKey = 0);
    MissionRankingRecord* FindTmpMissionRankingRecord(RANKINGKEY key) const;
    // 현재주 랭킹에 미션랭킹 등록
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    DWORD OnRegisterMissionRecordOnTodayweek(BOOL bSuccess, RANKINGNO no, RANKINGKEY key, 
                                             RANKINGKEY NewKey, RANKINGTYPE ranking_type = 0);
#else
    DWORD OnRegisterMissionRecordOnTodayweek(BOOL bSuccess, RANKINGNO no, RANKINGKEY key, 
                                             RANKINGKEY NewKey);
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    void RegisterMissionRecordOnTodayweek(RANKINGKEY key, _MISSION_RANKING_TOTAL_INFO& rTotalInfo);

    //----------------------------------------------------------------------------------------------
    // 보상 요청
#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
    RC::eRANKING_RESULT CanReward(RANKINGNO mission_no, RANKINGKEY key, const TCHAR* char_name,
        RankingMember** const result_ranking_member, RANKINGTYPE ranking_type = 0) const;
    RC::eRANKING_RESULT DoReward(RANKINGNO no, RANKINGKEY key, const TCHAR* charname, 
                                 RANKINGTYPE ranking_type = 0);
    virtual void OnReward(BOOL bSuccess, RANKINGNO no, RANKINGKEY key, const TCHAR* charname, 
                          RANKINGTYPE ranking_type = 0);
#else
    RC::eRANKING_RESULT CanReward(RANKINGNO mission_no, RANKINGKEY key, const TCHAR* char_name,
        RankingMember** const result_ranking_member) const;
    RC::eRANKING_RESULT DoReward(RANKINGNO no, RANKINGKEY key, const TCHAR* charname);
    virtual void OnReward(BOOL bSuccess, RANKINGNO no, RANKINGKEY key, const TCHAR* charname);
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM

    //----------------------------------------------------------------------------------------------
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    MissionRankingRecord* FindMissionRecord(eRANKING_TYPE type,
                          RANKINGNO mission_no, RANKINGKEY key, RANKINGTYPE ranking_type = 0) const;
    MissionRanking* FindMissionRanking(eRANKING_TYPE type, RANKINGNO mission_no, 
                                       RANKINGTYPE ranking_type = 0) const;
#else
    MissionRankingRecord* FindMissionRecord(eRANKING_TYPE type,
        RANKINGNO mission_no, RANKINGKEY key) const;
    MissionRanking* FindMissionRanking(eRANKING_TYPE type, RANKINGNO mission_no) const;
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    //MISSION_RANKING& FindRankingHashForType(eRANKING_TYPE type);
    int GetDanglingRankingCount();
    //
#if defined (_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)
    const int GetMaxLow() const;
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM
#ifdef _SERVER
    void Display() const;
    void DisplayRanking() const;
#endif
    void SortAll();
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    MissionRanking* CreateRanking(eRANKING_TYPE type, RANKINGNO no, RANKINGTYPE ranking_type = 0);
#else
    MissionRanking* CreateRanking(eRANKING_TYPE type, RANKINGNO no);
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR

    //----------------------------------------------------------------------------------------------
    // about ranking settlements
    eRANKING_STATUS GetStatus() const;
    void SetStatus(eRANKING_STATUS s);

    // 20060904
    DWORD GetOldSettledDate();
    DWORD GetSettledDate();

    int GetOldSettledDate_Year();
    int GetOldSettledDate_Month();
    int GetOldSettledDate_Day();

    // minute
    int GetSuspendTime();

    // 14day
    int GetPeriod();

    // 테트용 임시 함수
    void DoTestCurSettle();

#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
    //이번 주간에서 자신의 최고 랭킹의 레코드를 찾는다
    MissionRankingRecord* FindMyBestRanking(const TCHAR* _charname, eRANKING_TYPE _ranking_type , 
                                            WORD _missionNo, eRANKING_SYSTEM_TYPE _ranking_system_type);

    void Serialize_MyRanking(const TCHAR* charname, RANKINGID id, BYTE* INOUT stream, WORD& INOUT size, WORD& INOUT myranking);
    void SerializeForClient_MyRanking(const TCHAR* charname, RANKINGID id, BYTE* INOUT stream, WORD& INOUT size, WORD& INOUT myranking);

    void GetRewardableMissionList(const TCHAR* _charname,REWARDABLE_MISSION_LIST* rewardable_mission_list);
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM

protected:
    //----------------------------------------------------------------------------------------------
    // about ranking settlements
    // 상속 받은 클래스에서 구현해야 하는것
    virtual void OnChangedStatus() {}
    virtual void OnSettled() {}
#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
    //게임서버의 RankingManager의 결산일을 업데이트시키기위한 함수
    void UpdateSettleTime() { this->NextCalcSettleTime(); }

    const MISSION_RANKING* GetRankingTables() const {return ranking_tables_;}
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    const BATTLEGROUND_RANKING* GetBattleGroundRankingTables() const {return battleground_ranking_tables_;}
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM

private:
    //
    bool _LoadLVAreaInfo();
    bool _LoadRewardInfo();
    //
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    RANKING _RegisterRecord(eRANKING_TYPE type, BYTE MaxRanking,
        RANKINGNO no, MissionRankingRecord* pRankingRecord, RANKINGTYPE ranking_type = 0);
#else
    DWORD _RegisterRecord(eRANKING_TYPE type, BYTE MaxRanking,
        RANKINGNO no, MissionRankingRecord* pRankingRecord);
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR

    //----------------------------------------------------------------------------------------------
    // about ranking settlements = {
    void UpdateSuspending();
    void UpdateNormal();
    void NextCalcSettleTime();
    void DoSettle();
    // }
    void UnLoad();
#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
    DWORD Make_Mission_RewardKey(RANKINGNO mission_no, BYTE class_type, WORD ranking, BYTE reward_type) const;
#else
    DWORD Make_Mission_RewardKey(RANKINGNO mission_no, BYTE class_type, WORD ranking) const;
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM
    //
    SettleInfo* settle_data_impl_;
    eRANKING_STATUS ranking_status_;

    ITimerBase update_timer_;
    // 미션 별 랭킹 관리
    MISSION_RANKING ranking_tables_[eRANKING_MAX];
#if defined(_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)
    MISSION_RANKING my_ranking_table_;
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    BATTLEGROUND_RANKING battleground_ranking_tables_[eRANKING_MAX];
    BATTLEGROUND_RANKING my_battleground_table_;
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    TMP_MISSION_RECORD_HASH temp_ranking_table_;
    //  MISSION_RECORD_HASH_FOR_KEY m_TodayRankingRecordHash;   //< Helper Hash for search
    // about script management related to ranking
    MISSION_LEVEL_AREA_HASH mission_level_area_table_;
    MISSION_REWARD_HASH mission_reward_table_;

    util::CMemoryPoolFactory<MissionRanking>        m_MissionRankingPool;
    util::CMemoryPoolFactory<MissionRankingRecord>  m_MissionRankingRecordPool;
    util::CMemoryPoolFactory<RankingMember>         m_RankingMemberPool;
    //
    REWARD_BASE* mission_ranking_reward_array_;
    //
    __DISABLE_COPY(RankingManager);
};

//==================================================================================================
// inline implements


//inline MISSION_RANKING& RankingManager::FindRankingHashForType(eRANKING_TYPE type)
//{
//    ASSERT(type < eRANKING_MAX);
//    return ranking_tables_[type];
//}

inline int RankingManager::GetDanglingRankingCount() {
    return (int)temp_ranking_table_.size();
}

inline eRANKING_STATUS RankingManager::GetStatus() const {
    return ranking_status_;
}

inline void RankingManager::SetStatus(eRANKING_STATUS s) {
    ranking_status_ = s;
}

#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
inline DWORD RankingManager::Make_Mission_RewardKey(
    RANKINGNO mission_no, BYTE class_type, WORD ranking, BYTE reward_type) const
{
    return MAKEDWORD(mission_no,MAKEWORD(class_type, ranking)) * reward_type;
}
#else
inline DWORD RankingManager::Make_Mission_RewardKey(
    RANKINGNO mission_no, BYTE class_type, WORD ranking) const
{
    return MAKEDWORD(mission_no, MAKEWORD(class_type, ranking));
}
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM

inline const RankingManager::MISSION_LEVEL_AREA_HASH&
RankingManager::GetMissionLevelAreaTable() const {
    return mission_level_area_table_;
};

#if defined(_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)
inline const int RankingManager::GetMaxLow() const {
    return this->GetRowSize();
}

#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM

#endif //RANKING_LIB_RANKING_MANAGER_H
#endif // _NA_008012_20150130_RANKING_SYSTEM