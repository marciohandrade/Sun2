#pragma once

/*지역 점령전 정보 관리 메니저 클레스*/
#include "DominationInfoParser.h"
#include "SystemMemo.h"

class DominationArea;

//<지역 점령 맵코드, 점령전 지역 정보>
typedef STLX_HASH_MAP<MAPCODE, DominationArea*> AreaInfoMap;
typedef AreaInfoMap::iterator AreaInfoMapIterator;
typedef AreaInfoMap::const_iterator ConstAreaInfoMapIterator;

//<길드, 지역 점령 맵코드>
typedef STLX_MULTIMAP<GUILDGUID, MAPCODE> AreaInGuildMap;
typedef AreaInGuildMap::iterator AreaInGuildMapIterator;
#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
typedef AreaInGuildMap::const_iterator ConstAreaInGuildMapIterator;
#endif  //_NA_007128_20140326_MONDSCHIEN_DOMINATION
typedef std::pair<AreaInGuildMapIterator, AreaInGuildMapIterator> AreaInGuildMapPair;

//<점령전 스크립트 맵>
typedef DominationInfoParser::DOMINATION_SCRIPT_INFO_MAP ScriptDataMap;
typedef ScriptDataMap::iterator ScriptDataMapIterator;

class GameGuild;

class GameDominationManager : public util::Singleton<GameDominationManager>
{
public:
    enum LoadingStatus
    {
        INFO_IS_EMPTY,              //점령전 데이터가 비어있음
        AREAINFO_LOADING,           //점령전 지역 데이터 로딩중
        AREAINFO_LOAD_SUCCESS,      //점령전 지역 데이터 로딩 성공
        AUCTIONINFO_LOADING,        //점령전 경매 데이터 로딩중
        AUCTIONINFO_LOAD_SUCCESS,   //점령전 경매 데이터 로딩 성공
        INFO_LOAD_SUCCESS,          //모든 점령전 데이터 로딩이 성공
    };

    GameDominationManager();
    ~GameDominationManager();

    //점령정보 로딩 타이머 확인
    bool DominationTimerIsExpired();
    
    //지역 정보 로드 프로세서
    void LoadProcess();

    //경매 정보 Insert
    void SetAuctionData(MAPCODE map_code, const DominationAuctionInfo* auction_info, int data_count);
    
    //경매 정보 Update 요청
    bool UpdateAuctionData(Player* player, MAPCODE map_code, const DominationAuctionInfo& auction_info);
    
    //경매 리스트 요청
    void ResponseAuctionApply(Player* player, MAPCODE map_code)const;
    
    //점령전 정보 요청
    const DOMINATION_INFO* GetDominationInfo(MAPCODE map_code)const;
    
    //해당 지역 경매 정보 초기화
    void ClearAuctionInfo(MAPCODE map_code);

    //MAPCODE에 해당하는 지역의 길드 입찰금 
    MONEY GetTenderCost(MAPCODE map_code, GUILDGUID guild_guid)const;

    //MAPCODE에 해당하는 지역의 최고 입찰 금액
    MONEY GetBestTenderCost(MAPCODE map_code)const;

    //MAPCODE에 해당하는 점령지역 정보 찾기
    const DominationArea* FindAreaInfo(MAPCODE map_code)const;

    //MAPCODE에 해당하는 점령지역 정보 접근
    DominationArea* GetAreaInfo(MAPCODE map_code);

    //점령전 정보 Update Game->Guild->DBP
    void SendUpdateDominationInfo(const DOMINATION_INFO& domination_info, 
                                  eDOMINATION_EVENT_TYPE event_type)const;

    //점령전 정보 세팅 : Event Type에 맞게 정보를 세팅후 SendUpdateDominationInfo 호출
    void SetDominationGuildInfo(MAPCODE map_code, eDOMINATION_EVENT_TYPE event_type, 
                                DOMINATION_REWARD_EVENT_INFO reward_event);
    void SetDominationGuildInfo(MAPCODE map_code, eDOMINATION_EVENT_TYPE event_type);
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
    void SetDominationGuildInfo(MAPCODE map_code, eDOMINATION_EVENT_TYPE event_type, 
        DOMINATION_REWARD_EVENT_INFO reward_event, Player* player);
#endif // _NA_006826_20130722_DOMINATION_RENEWAL

    //현재 시간 얻기
    void GetCurrentDateTime(TCHAR* OUT result_time, size_t time_length) const;

    //점령전 정보 업데이트 : 이벤트별 컨트롤
    void UpdateDominationInfo(const DOMINATION_INFO& domination_info, eDOMINATION_EVENT_TYPE event_type);

    //MAPCODE에 해당하는 지역의 소유 길드 정보 얻기
    void GetInGuildInfo(MAPCODE map_code, DOMINATION_GUILD_INFO& guild_info);

    //유저미니맵에 표시될 점령전 정보 얻기
    BYTE GetMiniMapInfo(DOMINATION_GUILD_INFO* guild_info, BYTE max_count)const;

    //MAPCODE에 해당하는 지역의 방벽 정보 얻기 : 리턴 값은 총 포인트 값
    BYTE GetDefenseWall(MAPCODE map_code, BYTE* deffens_wall);

    //MAPCODE에 해당하는 지역의 방벽 정보 세팅
    void SetDefenseWall(MAPCODE map_code, const BYTE* deffens_wall);

    //방벽 데이터 체크 : 총 포인트보다 현제 방벽 세팅 값이 높은지 확인
    bool CheckDefenseWall(MAPCODE map_code, const BYTE* deffens_wall);

    //길드 정보 변경에 의한 점령지역 정보 변경
    void ChangeGuildMark(GUILDGUID guild_guid, GUILDMARKIDX pattern_index, GUILDMARKIDX background_index);
    void ChangeGuildName(GUILDGUID guild_guid, TCHAR* guild_name);
    void ChangeGuildMaster(GUILDGUID guild_guid, TCHAR* master_name);

    //모든 길드점령지 옵션 처리(한길드원,모든점령지) : 길드원 최초 입장, 가입시, 탈퇴시 
    void ApplyAllRewardOption(Player* const player, const bool is_add = true);

    // 필드 보상 처리 :  필드 입장, 퇴장, 길드 가입, 탈퇴시
    void ApplyRewardField(Player* const player, const MAPCODE map_code, const bool is_add = true) const;

    //점령 지역 정보를 전송 : 최초접속, 길드 가입시
    void SendAreaInfoToPlayer(Player* player, bool reward_flag = true);

    //점령전 진행후 결과 반영
    void SendAreaInfoToGuild(GUILDGUID guild_guid);

    //점령탑, 길드내 정보 관리요청시 보여지게 되는 정보 확인
    void CheckNotice(Player* player, MAPCODE map_code, MSG_CG_DOMINATION_NOTICE_ACK& msg);

    //MAPCODE에 해당하는 지역 공격 길드 선정및 알림
    void SetDominationInfoDisGuild(MAPCODE map_code);

    //MAPCODE에 해당하는 지역 경매 정보 초기화 : Game -> Guild -> DBP
    void SendInitActionInfo(MAPCODE map_code);

    //해당 플레이어에게 MAPCODE에 해당하는 점령 공지 정보를 보낸다.
    void SendDominationNotice(Player* player, MAPCODE map_code);

    //방벽 데이터 변경에 따른 저장이 가능한지 체크
    RC::eDOMINATION_RESULT CanSaveGateState(Player* player, MAPCODE map_code, const BYTE* deffens_wall);

    //해당 유저에게 아이템 보상 및 알림
    RC::eITEM_RESULT RewardItem(Player* player, int amount_cost, SLOTCODE item_code);
    
    //GUILGUID에 해당하는 길드가 점령한 지역 갯수 
    WORD GetAreaCount(const GUILDGUID guild_guid);

    //경매 입찰 아이템 삭제
    bool RemoveAuctionItem(Player* player, POSTYPE token_pos = 0, int amount_cost = 0, SLOTCODE item_code = 0) const;

    //경매 아이템 유효성 체크
    bool CheckDominationTokken(Player* player, int amount_token, POSTYPE pos, MAPCODE map_code) const;

    //지역 점령전 지역이 맞는가?
    bool IsDominationArea(MAPCODE map_code)const;

    //길드 정보내 점령전 참여 관련 정보 업데이트
    //_NA_20100604_BETTER_DOMINATION_MANAGER 
    void UpdateGuildInfoByDomination(const GUILDGUID guild_guid, const eDOMINATION_ADD_GUILD_INFO_TYPE type);

    //경매에 입찰이 가능한가
#ifdef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
    RC::eDOMINATION_RESULT CanEntryAuctionCost(Player* player, MAPCODE map_code, int entry_money = 0, POSTYPE pos = 0) const;
#else
    RC::eDOMINATION_RESULT CanEntryAuctionCost(Player* player, MAPCODE map_code, MONEY entry_money) const;
#endif


    //_NA_0_20100208_DOMINATION_AREA_INFO
    void SerializeAreaInfo(const GUILDGUID guild_guid, DOMINATION_STATE_TOTAL_INFO& OUT state_total_info);

    //_NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION
    //시스템 메모 인스턴스 얻기
    SystemMemo* GetSystemMemo();
    //보상 메모 보내기
    bool SendRewardMemo(const DOMINATION_INFO* domination_info, MAPCODE map_code);
    //_NA_20110216_INCREASE_DOMINATION_DEFFENSEWALL_POINT
    //방벽 포인트를 증가 시킨다.
    bool IncreaseDeffenseWallPoint(Player* player, SLOTCODE item_code, MAPCODE map_code);

    //방벽 포인트를 증가 시킬수 있는가?
    RC::eDOMINATION_RESULT CanIncreaseDeffenseWallPoint(Player* player, MAPCODE map_code);

    //_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
    //점령전 맴버 리스트를 보낸다 : 보내는 패킷에 클라이어트가 리스트 창 보여주기를 결정하는 flag 존재
    void SendMemberList(Player* player, MAPCODE map_code)const;

    //점령전에 참여가능한 상태인가?
    RC::eDOMINATION_RESULT CanJoinMember(Player* player, MAPCODE map_code)const;

    //점령전 참여 멤버 데이터 만들기
    bool MakeMemberInfo(Player* player, MAPCODE map_code, DominationMemberInfo& OUT info)const;

    //점령전 맴버 리스트 클리어
    void InitMemberList();

    //점령지역 참여 맴버가 맞는가?
    bool IsJoinMember(CHARGUID char_guid, MAPCODE map_code)const;

    //점령전 맵에 들어올수 있는지 체크
    bool CanEnterDominationField(Player* player, MAPCODE map_code)const;

    //점령전 참여 맴버를 삭제 할 수 있는지
    bool IsDeleteMember(MAPCODE map_code) const;

    void DeleteMemberList(MAPCODE map_code);
    void GuildMasterNameChange(GUILDGUID guild_guid, CHARGUID char_guid, const TCHAR* character_name);
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
    bool CheckDominationIsProgressing();
    void ChangeAreaProgressState(MAPCODE map_code, const eDOMINATION_PROGRESS_STATE area_state);
    void ChangeAllAreaProgressState(const eDOMINATION_PROGRESS_STATE area_state);
    // 점령지역의 맵NPC가 죽었을 경우의 처리
    void OnDeadDominationMapNPC(const MAPCODE map_code, const MONSTERCODE monster_code);
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
    //대륙의 영주 정보를 저장한다.
    void SetDominationContinentLordInfo(const sDOMINATION_CONTINENT_LORD_INFO lord_info); 
    //대륙의 영주 정보를 얻어온다.
    const sDOMINATION_CONTINENT_LORD_INFO* GetDominationContinentLordInfo(int continent) const;
#endif  //_NA_007128_20140326_MONDSCHIEN_DOMINATION
#ifdef _NA_007488_20140811_MONDSCHUEN_INITIALIZE_N_REWARD
    // 길드점령지 옵션을 모든 길드원에게 적용 시킨다.
    void ApplyAllMemberRewardOption(const GUILDGUID guild_guid, const bool is_add);
#endif // _NA_007488_20140811_MONDSCHUEN_INITIALIZE_N_REWARD

#ifdef _NA_008334_20150608_SONNENSCHEIN
    VOID GetGuildTakenMaps(GUILDGUID guild_guid, OUT DominationInfoParser::TakenMaps* taken_maps);
#endif //_NA_008334_20150608_SONNENSCHEIN

private:
    //Manager 초기화
    void _Initialize();

    //스크립트 데이터 기준으로 점령지역 Map생성
    void _CreateArea();
    
    //점령지역 정보 로드 상태 체크및 재요청
    void _RequestAreaInfoLoad()const;

    //점령지역 기본 정보 insert
    void _SetDominationInfo(const DOMINATION_INFO& domination_info);
        
    //스크립트 로드 확인
    bool _IsLoadedScript()const;
    
    //점령전 정보 UPDATE
    void _UpdateDominationInfo(const DOMINATION_INFO& domination_info);
    
    //경매 입찰 가능 지역인가?
    bool _IsAvailableAuctionArea(MAPCODE map_code)const;
    
    //현재 점령전 이벤트 얻기
    eDOMINATION_EVENT_STATE _GetCurrentDominationEvent(MAPCODE map_code)const;
    
    //경매가 가능한 시간인가?
    bool _IsApplyAuction(Player* player, eDOMINATION_EVENT_STATE event_type)const;
    
    //경매 배팅이 가능한가?
    bool _CanAuctionBatting(Player* player, MAPCODE map_code)const;
    
    //경매 리스트 요청
    BYTE _GetAuctionList(MAPCODE map_code, DOMINATION_ACTION_INFO* auction_list, BYTE max_count)const;
    
    //GUILDMEMBER 정보 요청
    GameGuildMember* _GetGuildMemberInfo(Player* player)const;
    
    //선정 길드정보를 입찰 리스트에서 삭제한다 : 보상 리스트 작성을 위해
    void _DeleteGuildInfoInAuction(const DOMINATION_INFO& domination_info);

    //공격 길드 정보 삭제
    void _RemoveDisGuildInfo(MAPCODE map_code, bool is_swap = false);

    //AreaInGuildMap 관리 
    void _InsertGuildAreaInfo(const MAPCODE map_code, GUILDGUID win_guild_guid);
    void _RemoveGuildAreaInfo(const MAPCODE map_code, GUILDGUID loser_guild_guid);
#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
    void _RemoveGuildAreaInfoFromMapCode(const MAPCODE map_code);
    //점령지가 있는 길드 인가?
    bool _IsAreaInGuild(const GUILDGUID guild_guid) const;
    //대륙의 대영주의 정보를 저장한다.<대륙번호, 로드정보>
    typedef STLX_MULTIMAP<int, sDOMINATION_CONTINENT_LORD_INFO> ContinentLordInfos;
    ContinentLordInfos continent_lord_infos_;
#endif  //_NA_007128_20140326_MONDSCHIEN_DOMINATION

    //길드원에게 점령전 보상 옵션 처리
    void _ApplyRewardOption(
        Player* const player, const MAPCODE map_code, const bool is_add = true) const;
    
    //필드 관련 보상 옵션 처리
    void _AcceptRewardField(
        Player* const player, const MAPCODE map_code, const bool is_add = true) const;  

    //점령전 종료후 승패 길드에게 결과 알림
    void _ResultNotify(MAPCODE map_code, GUILDGUID win_guild_guid, GUILDGUID lose_guild_guid);
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
    void _ResultNotifyAllUser(MAPCODE map_code, GUILDGUID win_guild_guid, GUILDGUID lose_guild_guid);
#endif // _NA_006826_20130722_DOMINATION_RENEWAL

    //MAPCODE에 해당하는 지역 전체 방벽 포인트 얻기
    int _GetDefenseWallTotalPoint(MAPCODE map_code);

    //점령전 입장 가능한 유저인지 체크한다 : 유저의 현재 위치, 길드 유무, 현재 이벤트 시간
    bool _IsEnterArea(Player* player, eDOMINATION_EVENT_STATE current_event)const;

    //점령전 성문 관리기능 사용 가능한지 체크 : 점령길드 마스터만 가능, 점령전 중에는 안됨
    bool _IsGateManage(Player* player, eDOMINATION_EVENT_STATE current_event, MAPCODE map_code );

    //경매 금액 환급 요청
    void _ReturnDominationActionHeim(const DOMINATION_INFO& domination_info);

    //MAPCODE에 해당하는 지역 방벽 포인트 초기화
    void _InitDefenseWall(MAPCODE map_code);

    //점령 지역 정보 Delete
    void _DeleteArea();

    //점령전 정보 로드 상태 확인
    bool _CheckLoadedStatus();

    //DominationField 정보내 점령 길드 정보 셋팅
    void _InsertDomiationInfo();

    //GUILGUID의 길드가 점령전에 참여한 내역이 존재 하는가? : true(존재) / false(존재하지 않음)
    MAPCODE _DidApplyGuild(GUILDGUID guild_guid)const;

    //_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
    //Player가 현재 점령전 참여 신청이 가능한 상태인가?
    bool _IsJoinMember(Player* player, MAPCODE map_code)const;

    //Player가 점령전에 참여한 내역이 존재 하는가? : return 0이면 참가 지역 없음
    MAPCODE _DidJoinArea(CHARGUID member_guid)const;

    //점령전 참여 맴버 리스트를 클라이언트 창에 표시 할지를 결정
    bool _IsShowMemberList(MAPCODE map_code)const;

    //_NA_20110216_INCREASE_DOMINATION_DEFFENSEWALL_POINT
    //필요 아이템 삭제 및 알림
    bool RemovePointItem(Player* player, SLOTCODE item_code);

#ifdef _NA_000000_20130429_DOMINATION_PENALTY
    void ProcessDominationPenalty(GUILDGUID guild_guid, MAPCODE map_code, int penalty_days);
#endif //_NA_000000_20130429_DOMINATION_PENALTY

private:
    util::Timer domination_update_timer_;//지역 점령전 기본 정보및 경매 정보 Update Timer
    AreaInGuildMap area_in_guild_map;//길드 - 점령맵 구조의 MAP
    AreaInfoMap area_info_map_;//점령지역 데이터 MAP
    bool is_initialize_;//지역 점령전 초기화 상태
    SystemMemo system_memo_;//시스템 메모
    //_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
    bool is_init_member_;
};

inline SystemMemo* 
GameDominationManager::GetSystemMemo()
{
    return &system_memo_;
}

