#pragma once
#ifndef DBPROXY_DOMINATION_MANAGER_H
#define DBPROXY_DOMINATION_MANAGER_H

//==================================================================================================
//
class DominationManager
{
public:
    enum {MAX_DOMINATIONINFOSET_TIME = 10*1000};

    static DominationManager* Instance();
    
    bool Update();
    
    // 디비로부터 점령전 정보 읽기가 완료 되었는지 검사한다.
    bool IsCompleteLoading() const;
    
    // 점령전 스크립트 읽기가 완료되면 스크립트로부터 기본적인 지역 정보를 설정한다.
    void OnScriptsLoaded();
    
    // 지역 정보 읽어오기를 완료 상태로 설정한다.
    void SetCompleteLoadAreaInfo();
    
    // 지역 정보를 초기화한다.
    void ClearAreaInfos();

    // 지역 정보를 설정한다.
    bool InsertAreaInfo(const DOMINATION_INFO& area_info);
    
    // 게임 서버로 지역 정보를 보낸다.
    bool SendAreaInfo(GameServerSession* const game_server, const MAPCODE map_code) const;
    
    DOMINATION_INFO* FindDominationInfo(const MAPCODE map_code);
    
    // 입찰 정보를 초기화한다.
    bool ClearAuctions(const MAPCODE map_code);

    // 입찰 정보를 삽입한다.
    bool InsertAuction(const MAPCODE map_code, const DominationAuctionInfo& auction);   
    
    // 게임 서버로 입찰 정보를 보낸다.
    bool SendAuctions(GameServerSession* const game_server, const MAPCODE map_code) const;
    
    // 입찰한 하임을 돌려 받는 길드 마스터 목록을 만든다.   
    bool SetListForReturnHeim(
        const MAPCODE map_code, 
        DOMINATION_RETURNMONEY_INFO* OUT return_info, 
        const int max_data, 
        int& index_count) const;
    
    // 입찰 정보 읽어오기를 완료 상태로 설정한다.
    bool SetCompleteLoadAuctions(const MAPCODE map_code);
    
    // 입찰 정보를 삭제한다.
    bool DeleteAuction(const MAPCODE map_code, const GUILDGUID guild_guild);
    
    // 점령한 지역 개수를 얻는다.
    WORD GetNumOfDominationArea(const GUILDGUID guild_guid) const;

    // 참여 인원 읽어오기를 완료 상태로 설정한다.
    bool SetCompleteLoadMembers(const MAPCODE map_code);
    
    // 참여 인원을 초기화한다.
    bool ClearMembers(const MAPCODE map_code);
    
    // 참여 인원을 삽입한다.
    bool InsertMember(const MAPCODE map_code, const DominationMemberInfo& member);
    
    // 참여 인원을 삭제한다.
    bool DeleteMember(const MAPCODE map_code, const CHARGUID member_guid);
    
    // 게임 서버로 참여 인원 정보를 보낸다.
    bool SendMembers(GameServerSession* const game_server, const MAPCODE map_code) const;

#ifdef _NA_003923_20120130_GUILD_RENEWAL
    void DominationInGameGuildLog(
        DOMINATION_INFO update_info, 
        eDOMINATION_EVENT_TYPE event_type
        );
#endif
    
private:
    DominationManager();
    ~DominationManager();
    static DominationManager* CreateInstance();
    
    enum eServiceMode 
    {
        eServiceMode_None = 0x00,
        eServiceMode_Instanced = 0x01,  // instanced,
        eServiceMode_ScriptLoaded       = 0x02, // need the mode control if scripts receive from SolarAuth
        eServiceMode_AreaInfoLoaded     = 0x04,
        eServiceMode_AuctionInfoLoaded  = 0x08,
        eServiceMode_MemberInfoLoaded   = 0x10,
        eServiceMode_InformationLoading = 0x80, // db transaction
        //
        eServiceMode_Loaded = eServiceMode_Instanced
                            | eServiceMode_ScriptLoaded
                            | eServiceMode_AreaInfoLoaded
                            | eServiceMode_AuctionInfoLoaded
                            //_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
                            | eServiceMode_MemberInfoLoaded
    };
    eServiceMode service_mode_;

    // 점령전 통합 정보
    struct Domination
    {
#ifdef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY 
        typedef STLX_HASH_MAP<GUILDGUID, DominationAuctionInfo> AuctionList;
#else
        typedef STLX_MULTIMAP<MONEY, DominationAuctionInfo> AuctionList;
#endif
        typedef STLX_HASH_MAP<CHARGUID, DominationMemberInfo> MemberList;
        
        MAPCODE field_code;
        DOMINATION_INFO area_info; // 지역 정보
        AuctionList auctions; // 입찰 정보
        MemberList members; // 참여 인원 정보

        bool is_load_area_info; // 지역 정보 읽기 완료 여부
        bool is_load_auctions; // 입찰 정보 읽기 완료 여부
        bool is_load_members; // 참여 인원 정보 읽기 완료 여부
    
        Domination() : 
            field_code(0), is_load_area_info(false), is_load_auctions(false), is_load_members(false)
        {
            ::ZeroMemory(&area_info, sizeof(area_info));
        }
        void ClearAreaInfo();
        void ClearAuctions();
        void ClearMembers();
    };
    typedef STLX_HASH_MAP<MAPCODE, Domination> DominationMap;
    DominationMap domination_map_;


#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
public:
    // DB에 대륙 영주 정보 갱신를 요청한다.
    void UpdateContinentLordInfo(const bool send_game_server = false);
    // 게임 서버로 대륙 영주 정보를 보낸다.
    bool SendDominationContinentLordInfo(const MAPCODE map_code);
    // 대륙의 영주 정보를 저장한다.
    void SetDominationContinentLordInfo(const sDOMINATION_CONTINENT_LORD_INFO lord_info); 
    // 대륙의 영주 정보를 얻어온다.
    const sDOMINATION_CONTINENT_LORD_INFO* GetDominationContinentLordInfo(int continent) const;
private:
    //대륙의 대영주의 정보를 저장한다.<대륙번호, 로드정보>
    typedef STLX_MULTIMAP<int, sDOMINATION_CONTINENT_LORD_INFO> ContinentLordInfos;
    ContinentLordInfos continent_lord_infos_;
#endif //_NA_007128_20140326_MONDSCHIEN_DOMINATION

    // last order...
    static DominationManager* instance_static_;
    //
    __DISABLE_COPY(DominationManager);
};

//==================================================================================================
inline DominationManager* DominationManager::Instance()
{
    if (instance_static_ != NULL) 
    {
        return instance_static_;
    }
    return CreateInstance();
}

// control interfaces
inline bool DominationManager::IsCompleteLoading() const 
{
    return (service_mode_ == eServiceMode_Loaded);
}
//

inline void DominationManager::Domination::ClearAreaInfo()
{
    ::ZeroMemory(&area_info, sizeof(area_info));
    area_info.m_FieldCode = field_code;
    area_info.m_TotalPoint = DOMINATION_DEFFENSWALL_POINT;
    is_load_area_info = false;
}

inline void DominationManager::Domination::ClearAuctions()
{
    auctions.clear();
    is_load_auctions = false;
}

inline void DominationManager::Domination::ClearMembers()
{
    members.clear();
    is_load_members = false;
}

#endif //DBPROXY_DOMINATION_MANAGER_H