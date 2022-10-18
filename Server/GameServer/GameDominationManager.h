#pragma once

/*���� ������ ���� ���� �޴��� Ŭ����*/
#include "DominationInfoParser.h"
#include "SystemMemo.h"

class DominationArea;

//<���� ���� ���ڵ�, ������ ���� ����>
typedef STLX_HASH_MAP<MAPCODE, DominationArea*> AreaInfoMap;
typedef AreaInfoMap::iterator AreaInfoMapIterator;
typedef AreaInfoMap::const_iterator ConstAreaInfoMapIterator;

//<���, ���� ���� ���ڵ�>
typedef STLX_MULTIMAP<GUILDGUID, MAPCODE> AreaInGuildMap;
typedef AreaInGuildMap::iterator AreaInGuildMapIterator;
#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
typedef AreaInGuildMap::const_iterator ConstAreaInGuildMapIterator;
#endif  //_NA_007128_20140326_MONDSCHIEN_DOMINATION
typedef std::pair<AreaInGuildMapIterator, AreaInGuildMapIterator> AreaInGuildMapPair;

//<������ ��ũ��Ʈ ��>
typedef DominationInfoParser::DOMINATION_SCRIPT_INFO_MAP ScriptDataMap;
typedef ScriptDataMap::iterator ScriptDataMapIterator;

class GameGuild;

class GameDominationManager : public util::Singleton<GameDominationManager>
{
public:
    enum LoadingStatus
    {
        INFO_IS_EMPTY,              //������ �����Ͱ� �������
        AREAINFO_LOADING,           //������ ���� ������ �ε���
        AREAINFO_LOAD_SUCCESS,      //������ ���� ������ �ε� ����
        AUCTIONINFO_LOADING,        //������ ��� ������ �ε���
        AUCTIONINFO_LOAD_SUCCESS,   //������ ��� ������ �ε� ����
        INFO_LOAD_SUCCESS,          //��� ������ ������ �ε��� ����
    };

    GameDominationManager();
    ~GameDominationManager();

    //�������� �ε� Ÿ�̸� Ȯ��
    bool DominationTimerIsExpired();
    
    //���� ���� �ε� ���μ���
    void LoadProcess();

    //��� ���� Insert
    void SetAuctionData(MAPCODE map_code, const DominationAuctionInfo* auction_info, int data_count);
    
    //��� ���� Update ��û
    bool UpdateAuctionData(Player* player, MAPCODE map_code, const DominationAuctionInfo& auction_info);
    
    //��� ����Ʈ ��û
    void ResponseAuctionApply(Player* player, MAPCODE map_code)const;
    
    //������ ���� ��û
    const DOMINATION_INFO* GetDominationInfo(MAPCODE map_code)const;
    
    //�ش� ���� ��� ���� �ʱ�ȭ
    void ClearAuctionInfo(MAPCODE map_code);

    //MAPCODE�� �ش��ϴ� ������ ��� ������ 
    MONEY GetTenderCost(MAPCODE map_code, GUILDGUID guild_guid)const;

    //MAPCODE�� �ش��ϴ� ������ �ְ� ���� �ݾ�
    MONEY GetBestTenderCost(MAPCODE map_code)const;

    //MAPCODE�� �ش��ϴ� �������� ���� ã��
    const DominationArea* FindAreaInfo(MAPCODE map_code)const;

    //MAPCODE�� �ش��ϴ� �������� ���� ����
    DominationArea* GetAreaInfo(MAPCODE map_code);

    //������ ���� Update Game->Guild->DBP
    void SendUpdateDominationInfo(const DOMINATION_INFO& domination_info, 
                                  eDOMINATION_EVENT_TYPE event_type)const;

    //������ ���� ���� : Event Type�� �°� ������ ������ SendUpdateDominationInfo ȣ��
    void SetDominationGuildInfo(MAPCODE map_code, eDOMINATION_EVENT_TYPE event_type, 
                                DOMINATION_REWARD_EVENT_INFO reward_event);
    void SetDominationGuildInfo(MAPCODE map_code, eDOMINATION_EVENT_TYPE event_type);
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
    void SetDominationGuildInfo(MAPCODE map_code, eDOMINATION_EVENT_TYPE event_type, 
        DOMINATION_REWARD_EVENT_INFO reward_event, Player* player);
#endif // _NA_006826_20130722_DOMINATION_RENEWAL

    //���� �ð� ���
    void GetCurrentDateTime(TCHAR* OUT result_time, size_t time_length) const;

    //������ ���� ������Ʈ : �̺�Ʈ�� ��Ʈ��
    void UpdateDominationInfo(const DOMINATION_INFO& domination_info, eDOMINATION_EVENT_TYPE event_type);

    //MAPCODE�� �ش��ϴ� ������ ���� ��� ���� ���
    void GetInGuildInfo(MAPCODE map_code, DOMINATION_GUILD_INFO& guild_info);

    //�����̴ϸʿ� ǥ�õ� ������ ���� ���
    BYTE GetMiniMapInfo(DOMINATION_GUILD_INFO* guild_info, BYTE max_count)const;

    //MAPCODE�� �ش��ϴ� ������ �溮 ���� ��� : ���� ���� �� ����Ʈ ��
    BYTE GetDefenseWall(MAPCODE map_code, BYTE* deffens_wall);

    //MAPCODE�� �ش��ϴ� ������ �溮 ���� ����
    void SetDefenseWall(MAPCODE map_code, const BYTE* deffens_wall);

    //�溮 ������ üũ : �� ����Ʈ���� ���� �溮 ���� ���� ������ Ȯ��
    bool CheckDefenseWall(MAPCODE map_code, const BYTE* deffens_wall);

    //��� ���� ���濡 ���� �������� ���� ����
    void ChangeGuildMark(GUILDGUID guild_guid, GUILDMARKIDX pattern_index, GUILDMARKIDX background_index);
    void ChangeGuildName(GUILDGUID guild_guid, TCHAR* guild_name);
    void ChangeGuildMaster(GUILDGUID guild_guid, TCHAR* master_name);

    //��� ��������� �ɼ� ó��(�ѱ���,���������) : ���� ���� ����, ���Խ�, Ż��� 
    void ApplyAllRewardOption(Player* const player, const bool is_add = true);

    // �ʵ� ���� ó�� :  �ʵ� ����, ����, ��� ����, Ż���
    void ApplyRewardField(Player* const player, const MAPCODE map_code, const bool is_add = true) const;

    //���� ���� ������ ���� : ��������, ��� ���Խ�
    void SendAreaInfoToPlayer(Player* player, bool reward_flag = true);

    //������ ������ ��� �ݿ�
    void SendAreaInfoToGuild(GUILDGUID guild_guid);

    //����ž, ��峻 ���� ������û�� �������� �Ǵ� ���� Ȯ��
    void CheckNotice(Player* player, MAPCODE map_code, MSG_CG_DOMINATION_NOTICE_ACK& msg);

    //MAPCODE�� �ش��ϴ� ���� ���� ��� ������ �˸�
    void SetDominationInfoDisGuild(MAPCODE map_code);

    //MAPCODE�� �ش��ϴ� ���� ��� ���� �ʱ�ȭ : Game -> Guild -> DBP
    void SendInitActionInfo(MAPCODE map_code);

    //�ش� �÷��̾�� MAPCODE�� �ش��ϴ� ���� ���� ������ ������.
    void SendDominationNotice(Player* player, MAPCODE map_code);

    //�溮 ������ ���濡 ���� ������ �������� üũ
    RC::eDOMINATION_RESULT CanSaveGateState(Player* player, MAPCODE map_code, const BYTE* deffens_wall);

    //�ش� �������� ������ ���� �� �˸�
    RC::eITEM_RESULT RewardItem(Player* player, int amount_cost, SLOTCODE item_code);
    
    //GUILGUID�� �ش��ϴ� ��尡 ������ ���� ���� 
    WORD GetAreaCount(const GUILDGUID guild_guid);

    //��� ���� ������ ����
    bool RemoveAuctionItem(Player* player, POSTYPE token_pos = 0, int amount_cost = 0, SLOTCODE item_code = 0) const;

    //��� ������ ��ȿ�� üũ
    bool CheckDominationTokken(Player* player, int amount_token, POSTYPE pos, MAPCODE map_code) const;

    //���� ������ ������ �´°�?
    bool IsDominationArea(MAPCODE map_code)const;

    //��� ������ ������ ���� ���� ���� ������Ʈ
    //_NA_20100604_BETTER_DOMINATION_MANAGER 
    void UpdateGuildInfoByDomination(const GUILDGUID guild_guid, const eDOMINATION_ADD_GUILD_INFO_TYPE type);

    //��ſ� ������ �����Ѱ�
#ifdef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
    RC::eDOMINATION_RESULT CanEntryAuctionCost(Player* player, MAPCODE map_code, int entry_money = 0, POSTYPE pos = 0) const;
#else
    RC::eDOMINATION_RESULT CanEntryAuctionCost(Player* player, MAPCODE map_code, MONEY entry_money) const;
#endif


    //_NA_0_20100208_DOMINATION_AREA_INFO
    void SerializeAreaInfo(const GUILDGUID guild_guid, DOMINATION_STATE_TOTAL_INFO& OUT state_total_info);

    //_NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION
    //�ý��� �޸� �ν��Ͻ� ���
    SystemMemo* GetSystemMemo();
    //���� �޸� ������
    bool SendRewardMemo(const DOMINATION_INFO* domination_info, MAPCODE map_code);
    //_NA_20110216_INCREASE_DOMINATION_DEFFENSEWALL_POINT
    //�溮 ����Ʈ�� ���� ��Ų��.
    bool IncreaseDeffenseWallPoint(Player* player, SLOTCODE item_code, MAPCODE map_code);

    //�溮 ����Ʈ�� ���� ��ų�� �ִ°�?
    RC::eDOMINATION_RESULT CanIncreaseDeffenseWallPoint(Player* player, MAPCODE map_code);

    //_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
    //������ �ɹ� ����Ʈ�� ������ : ������ ��Ŷ�� Ŭ���̾�Ʈ�� ����Ʈ â �����ֱ⸦ �����ϴ� flag ����
    void SendMemberList(Player* player, MAPCODE map_code)const;

    //�������� ���������� �����ΰ�?
    RC::eDOMINATION_RESULT CanJoinMember(Player* player, MAPCODE map_code)const;

    //������ ���� ��� ������ �����
    bool MakeMemberInfo(Player* player, MAPCODE map_code, DominationMemberInfo& OUT info)const;

    //������ �ɹ� ����Ʈ Ŭ����
    void InitMemberList();

    //�������� ���� �ɹ��� �´°�?
    bool IsJoinMember(CHARGUID char_guid, MAPCODE map_code)const;

    //������ �ʿ� ���ü� �ִ��� üũ
    bool CanEnterDominationField(Player* player, MAPCODE map_code)const;

    //������ ���� �ɹ��� ���� �� �� �ִ���
    bool IsDeleteMember(MAPCODE map_code) const;

    void DeleteMemberList(MAPCODE map_code);
    void GuildMasterNameChange(GUILDGUID guild_guid, CHARGUID char_guid, const TCHAR* character_name);
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
    bool CheckDominationIsProgressing();
    void ChangeAreaProgressState(MAPCODE map_code, const eDOMINATION_PROGRESS_STATE area_state);
    void ChangeAllAreaProgressState(const eDOMINATION_PROGRESS_STATE area_state);
    // ���������� ��NPC�� �׾��� ����� ó��
    void OnDeadDominationMapNPC(const MAPCODE map_code, const MONSTERCODE monster_code);
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
    //����� ���� ������ �����Ѵ�.
    void SetDominationContinentLordInfo(const sDOMINATION_CONTINENT_LORD_INFO lord_info); 
    //����� ���� ������ ���´�.
    const sDOMINATION_CONTINENT_LORD_INFO* GetDominationContinentLordInfo(int continent) const;
#endif  //_NA_007128_20140326_MONDSCHIEN_DOMINATION
#ifdef _NA_007488_20140811_MONDSCHUEN_INITIALIZE_N_REWARD
    // ��������� �ɼ��� ��� �������� ���� ��Ų��.
    void ApplyAllMemberRewardOption(const GUILDGUID guild_guid, const bool is_add);
#endif // _NA_007488_20140811_MONDSCHUEN_INITIALIZE_N_REWARD

#ifdef _NA_008334_20150608_SONNENSCHEIN
    VOID GetGuildTakenMaps(GUILDGUID guild_guid, OUT DominationInfoParser::TakenMaps* taken_maps);
#endif //_NA_008334_20150608_SONNENSCHEIN

private:
    //Manager �ʱ�ȭ
    void _Initialize();

    //��ũ��Ʈ ������ �������� �������� Map����
    void _CreateArea();
    
    //�������� ���� �ε� ���� üũ�� ���û
    void _RequestAreaInfoLoad()const;

    //�������� �⺻ ���� insert
    void _SetDominationInfo(const DOMINATION_INFO& domination_info);
        
    //��ũ��Ʈ �ε� Ȯ��
    bool _IsLoadedScript()const;
    
    //������ ���� UPDATE
    void _UpdateDominationInfo(const DOMINATION_INFO& domination_info);
    
    //��� ���� ���� �����ΰ�?
    bool _IsAvailableAuctionArea(MAPCODE map_code)const;
    
    //���� ������ �̺�Ʈ ���
    eDOMINATION_EVENT_STATE _GetCurrentDominationEvent(MAPCODE map_code)const;
    
    //��Ű� ������ �ð��ΰ�?
    bool _IsApplyAuction(Player* player, eDOMINATION_EVENT_STATE event_type)const;
    
    //��� ������ �����Ѱ�?
    bool _CanAuctionBatting(Player* player, MAPCODE map_code)const;
    
    //��� ����Ʈ ��û
    BYTE _GetAuctionList(MAPCODE map_code, DOMINATION_ACTION_INFO* auction_list, BYTE max_count)const;
    
    //GUILDMEMBER ���� ��û
    GameGuildMember* _GetGuildMemberInfo(Player* player)const;
    
    //���� ��������� ���� ����Ʈ���� �����Ѵ� : ���� ����Ʈ �ۼ��� ����
    void _DeleteGuildInfoInAuction(const DOMINATION_INFO& domination_info);

    //���� ��� ���� ����
    void _RemoveDisGuildInfo(MAPCODE map_code, bool is_swap = false);

    //AreaInGuildMap ���� 
    void _InsertGuildAreaInfo(const MAPCODE map_code, GUILDGUID win_guild_guid);
    void _RemoveGuildAreaInfo(const MAPCODE map_code, GUILDGUID loser_guild_guid);
#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
    void _RemoveGuildAreaInfoFromMapCode(const MAPCODE map_code);
    //�������� �ִ� ��� �ΰ�?
    bool _IsAreaInGuild(const GUILDGUID guild_guid) const;
    //����� �뿵���� ������ �����Ѵ�.<�����ȣ, �ε�����>
    typedef STLX_MULTIMAP<int, sDOMINATION_CONTINENT_LORD_INFO> ContinentLordInfos;
    ContinentLordInfos continent_lord_infos_;
#endif  //_NA_007128_20140326_MONDSCHIEN_DOMINATION

    //�������� ������ ���� �ɼ� ó��
    void _ApplyRewardOption(
        Player* const player, const MAPCODE map_code, const bool is_add = true) const;
    
    //�ʵ� ���� ���� �ɼ� ó��
    void _AcceptRewardField(
        Player* const player, const MAPCODE map_code, const bool is_add = true) const;  

    //������ ������ ���� ��忡�� ��� �˸�
    void _ResultNotify(MAPCODE map_code, GUILDGUID win_guild_guid, GUILDGUID lose_guild_guid);
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
    void _ResultNotifyAllUser(MAPCODE map_code, GUILDGUID win_guild_guid, GUILDGUID lose_guild_guid);
#endif // _NA_006826_20130722_DOMINATION_RENEWAL

    //MAPCODE�� �ش��ϴ� ���� ��ü �溮 ����Ʈ ���
    int _GetDefenseWallTotalPoint(MAPCODE map_code);

    //������ ���� ������ �������� üũ�Ѵ� : ������ ���� ��ġ, ��� ����, ���� �̺�Ʈ �ð�
    bool _IsEnterArea(Player* player, eDOMINATION_EVENT_STATE current_event)const;

    //������ ���� ������� ��� �������� üũ : ���ɱ�� �����͸� ����, ������ �߿��� �ȵ�
    bool _IsGateManage(Player* player, eDOMINATION_EVENT_STATE current_event, MAPCODE map_code );

    //��� �ݾ� ȯ�� ��û
    void _ReturnDominationActionHeim(const DOMINATION_INFO& domination_info);

    //MAPCODE�� �ش��ϴ� ���� �溮 ����Ʈ �ʱ�ȭ
    void _InitDefenseWall(MAPCODE map_code);

    //���� ���� ���� Delete
    void _DeleteArea();

    //������ ���� �ε� ���� Ȯ��
    bool _CheckLoadedStatus();

    //DominationField ������ ���� ��� ���� ����
    void _InsertDomiationInfo();

    //GUILGUID�� ��尡 �������� ������ ������ ���� �ϴ°�? : true(����) / false(�������� ����)
    MAPCODE _DidApplyGuild(GUILDGUID guild_guid)const;

    //_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
    //Player�� ���� ������ ���� ��û�� ������ �����ΰ�?
    bool _IsJoinMember(Player* player, MAPCODE map_code)const;

    //Player�� �������� ������ ������ ���� �ϴ°�? : return 0�̸� ���� ���� ����
    MAPCODE _DidJoinArea(CHARGUID member_guid)const;

    //������ ���� �ɹ� ����Ʈ�� Ŭ���̾�Ʈ â�� ǥ�� ������ ����
    bool _IsShowMemberList(MAPCODE map_code)const;

    //_NA_20110216_INCREASE_DOMINATION_DEFFENSEWALL_POINT
    //�ʿ� ������ ���� �� �˸�
    bool RemovePointItem(Player* player, SLOTCODE item_code);

#ifdef _NA_000000_20130429_DOMINATION_PENALTY
    void ProcessDominationPenalty(GUILDGUID guild_guid, MAPCODE map_code, int penalty_days);
#endif //_NA_000000_20130429_DOMINATION_PENALTY

private:
    util::Timer domination_update_timer_;//���� ������ �⺻ ������ ��� ���� Update Timer
    AreaInGuildMap area_in_guild_map;//��� - ���ɸ� ������ MAP
    AreaInfoMap area_info_map_;//�������� ������ MAP
    bool is_initialize_;//���� ������ �ʱ�ȭ ����
    SystemMemo system_memo_;//�ý��� �޸�
    //_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
    bool is_init_member_;
};

inline SystemMemo* 
GameDominationManager::GetSystemMemo()
{
    return &system_memo_;
}

