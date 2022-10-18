#pragma once

#include <Singleton.h>
#include "DominationInfoParser.h"
#include <Guild/GuildCommon.h>

typedef DominationInfoParser::DOMINATION_SCRIPT_INFO_MAP ScriptInfoMap;
typedef DominationInfoParser::DOMINATION_SCRIPT_INFO_ITR ScriptInfoIterator;

class DominationManager : public  util::Singleton<DominationManager>
{
public:
    enum {MAX_DOMINATIONINFOSET_TIME = 10*1000};

    enum DominationLoadingStatus
    {
        DOMINATION_INFO_EMPTY,             //점령전 데이터가 비어있다.
        DOMINATION_INFO_LOADING,           //점령전 데이터 로딩중이다.
        DOMINATION_INFO_SUCCESS,           //점령전 데이터 로딩이 성공했다.
    };
    DominationManager();
    ~DominationManager();
    
    //특정시간 마다 점령전 정보를 업데이트 한다.
    void LoadDominationInfo();

    //로드되지 못한 지역 정보 요청
    void RequestDominationAuctionInfo();
    
    //지역 점령 기본 정보를 넣는다.
    void InsertDominationInfo(const DOMINATION_INFO& domination_info);
    
    //지역 점령전 정보를 게임 서버로 보낸다.
    void SendDominationInfo(GameServerSession* server_session, MAPCODE map_code);
    
    //지역 점령전 정보의 포인터를 리턴한다.
    DOMINATION_INFO* FindDominationInfo(MAPCODE map_code);
    
    //지역 점령 경매 기본 정보를 Map에 세팅
    bool InsertActionInfo(MAPCODE map_code, const DominationAuctionInfo& domination_info);   
    
    //지역 점령 경매 정보 send to GameServer      
    void SendActionList(GameServerSession* server_session, MAPCODE map_code);
    
    //지역 점령전 경매 정보를 지운다.     
    void ClearDominationActionInfo(MAPCODE map_code);
    
    //경매 하임을 돌려 받는 길드 마스터 리스트 만들기.   
    bool SetListForReturnHeim(MAPCODE map_code, DOMINATION_RETURNMONEY_INFO* OUT return_info, 
                              DWORD max_data, INT& index_count);
    
    //지역별 경매 정보 로드 성공시 체크 
    void SetCheckActionInfo(MAPCODE map_code);
    
    //경매 정보 리스트에서 해당 CharGuid정보를 삭제    
    bool DeleteActionList(MAPCODE map_code, GUILDGUID guild_guild);
    
    //점령전 정보 로드 완료 설정 메소드
    void SetLoadDominationInfo(bool is_success);
    bool GetLoadDominationInfo()const;

    //경매 정보 로드 완료 설정 메소드
    void SetLoadDominationAuctionInfo(bool is_success);
    bool GetLoadDominationAuctionInfo()const;

#ifdef _NA_20100604_BETTER_DOMINATION_MANAGER 
    //길드의 점령 지역 수 리턴
    WORD NumberOfDominationAreaByGuild(const GUILDGUID guild_guid);
#endif

#ifdef _NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
    void SetLoadDominationMemberInfo(bool is_success);
    bool GetLoadDominationMemberInfo()const;
    void SetCheckMemberInfo(MAPCODE map_code);
    void SetCheckDominationMemberMap();
    void RequestDominationMemberInfo();
    void InsertDominationMemberInfo(MAPCODE map_code, const DominationMemberInfo& member_info);
    void DeleteDominationMemberInfo(MAPCODE map_code, CHARGUID char_guid);
    void SendMemberList(GameServerSession* server_session, MAPCODE map_code)const;
    void CheckCompleteLoadMemberData();
#endif

private:
    //초기화!
    void Initialize();

    //점령전 정보 로드 상태에따라 해야할 일을 결정한다.
    void DominationInfoStatusProc();
    
    //지역 점령전 기본 정보를 로드 한다.   
    void RequestDominationAreaInfo();
    
    //점령전 지역정보를 저장할 장소를 만든다.
    bool InsertDominationMapInfo(MAPCODE map_code);
    
    //경매 로드 정보를 초기화한다.
    void SetCheckDominationActionMap();
    
    //점령전 기본 정보(지역,맵)를 로드 했는지 체크, 로드 하지 못했으면 해당 부분 로드.
    bool CheckLoadingStatus();
    
    //점령전 스크립트기준으로 비어있는 점령전 정보를 초기화
    void SetEmptyDominationInfo();
    
    //DBP로 패킷 Send
    bool SendToDBProxy(MSG_BASE* msg, WORD size)const;

    //점령전 정보 로드 타이머 설정 메소드
    void LoadingTimerSet();
    void LoadingTimerDisable(); 

    //초기화 플레그 설정 메소드
    bool GetIsinit();
    void SetIsInit(bool flag);

    //점령전 로딩 상테 설정 메소드
    void SetDominationLoadingStatus(DominationLoadingStatus status); 
    DominationLoadingStatus GetDominationInfoStatus(); 

    //현재 점령전에 필요한 모든 데이터 로드를 완료 하였나?
    bool IsCompleteLoad();

    //경매 데이터 로드를 완료 하였나?
    void CheckCompleteLoadAuctionData();

private:
    bool is_load_domination_area_info_;
    bool is_load_domination_auction_info_;
    AreaInfoMap domination_area_info_map_;
    AllAreaAuctionInfoMap all_area_auction_info_map_;
    AuctionInfoLoadingCheckMap auction_info_loading_check_map_;
    util::Timer loading_timer_;
    DominationLoadingStatus loading_status_;
    bool is_init_;
#ifdef _NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
    bool is_load_domination_member_info_;
    MemberMap member_info_map_;
    MemberLoadCheckMap member_info_loading_check_map_;
#endif
};


inline void 
DominationManager::SetLoadDominationInfo(bool is_success) 
{
    is_load_domination_area_info_ = is_success;
}

inline bool 
DominationManager::GetLoadDominationInfo()const 
{
    return is_load_domination_area_info_;
}

inline void 
DominationManager::SetLoadDominationAuctionInfo(bool is_success) 
{
    is_load_domination_auction_info_ = is_success;
}

inline bool 
DominationManager::GetLoadDominationAuctionInfo()const 
{
    return is_load_domination_auction_info_;
}

inline void 
DominationManager::LoadingTimerSet() 
{ 
    loading_timer_.SetTimer(MAX_DOMINATIONINFOSET_TIME); 
}

inline void 
DominationManager::LoadingTimerDisable() 
{ 
    loading_timer_.DisableCheckTime();
}

inline bool 
DominationManager::GetIsinit()
{
    return is_init_;
}

inline void 
DominationManager::SetIsInit(bool flag)
{
    is_init_ = flag;
}

inline void 
DominationManager::SetDominationLoadingStatus(DominationLoadingStatus status) 
{ 
    loading_status_ = status; 
}

inline DominationManager::DominationLoadingStatus 
DominationManager::GetDominationInfoStatus() 
{ 
    return loading_status_; 
}

#ifdef _NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
inline void 
DominationManager::SetLoadDominationMemberInfo(bool is_success)
{
    is_load_domination_member_info_ = is_success;
}

inline bool 
DominationManager::GetLoadDominationMemberInfo()const
{
    return is_load_domination_member_info_;
}
#endif//_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
