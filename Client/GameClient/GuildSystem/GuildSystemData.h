#pragma once
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL

namespace GuildSystem{

class GuildSystemData
{
public:
    enum GuildMemberSortType
    {
        kSortByName,
        kSortByLevel,
        kSortByClass,
        kSortByDuty,
        kSortByGuildCoin,
        kSortByLogin,
        kMaxType,
    };

    struct GuildLogInfo
    {
        DWORD guild_log_category;
        GuildInGameLogData guild_log_data;
    };

    struct GuildInfoForClient
    {
        GAME_GUILD_INFO game_guild_info_;
        GUILD_INFO_EXTRA guild_info_extra_;

        GuildInfoForClient(GAME_GUILD_INFO game_guild_info, GUILD_INFO_EXTRA guild_info_extra)
            :game_guild_info_(game_guild_info), guild_info_extra_(guild_info_extra)
        {
        }
    };

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    struct GuildFacilityInfoForClient
    {
        FCODE facility_code_;
        FLEVEL facility_level_;
        bool facility_active_;

        GuildFacilityInfoForClient()
        {
            facility_code_ = 0;
            facility_level_ = 0;
            facility_active_ = false;
        }
    };
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

    typedef STLX_VECTOR<GUILD_MEMBER_INFO> GuildMemberContainer;
    typedef STLX_VECTOR<GUILD_RELATION> GuildRelationContainer;
    typedef STLX_LIST<STRUCT_GUILDWAREHOUSE_LOGINFO> GuildWareHouseLogContainer;
    typedef STLX_MULTIMAP<INT64, GuildLogInfo, std::greater<INT64> > GuildLogInfoContainer;
    typedef STLX_VECTOR<GUILD_RANKING_INFO> GuildRankingInfoContainer;
    typedef STLX_MAP<GUILDGUID, GuildInfoForClient> GuildRankingGuildInfoContainer;
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    typedef STLX_VECTOR<GUILD_JOIN_REQUEST_INFO> GuildJoinRequestInfoContainer;
    typedef STLX_MAP<FCODE, GuildFacilityInfoForClient> GuildFacilityContainer;
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

public:
    GuildSystemData();
    ~GuildSystemData();

    void Clear();

    // 기본 길드 정보
    void SetGuildInfo(GUILD_INFO& guild_info);
    GUILD_INFO& GetGuildInfo() { return guild_info_; }
    bool is_recv_guild_info() const { return recv_guild_info_; }
    bool is_recv_total_info() const { return recv_total_info_; }

    // 길드원 정보
    void SetGuildMemberInfo(GUILD_MEMBER_TOTAL_INFO& guild_total_member_info);
    void AddGuildMemberInfo(GUILD_MEMBER_INFO& add_member_info);
    void DeleteGuildMemberInfo(TCHAR* member_name);
    GUILD_MEMBER_INFO* GetGuildMemberInfo(const TCHAR* member_name);
    GUILD_MEMBER_INFO* GetGuildMemberInfo(CHARGUID member_guid);
    void SortGuildMember(GuildMemberSortType sort_type);

    // 길드명
    void SetGuildName(const char* guild_name);
    const char* GetGuildName() { return guild_name_.c_str(); }
    
    // 길드마스터
    void SetMasterName(const char* master_name);
    const char* GetMasterName() { return master_name_.c_str(); }

    // 길드멤버
    GuildMemberContainer& guild_member_container() { return guild_member_container_; }

    // 길드마크 텍스쳐
    void LoadGuildMarkTexture();
    HANDLE GetGuildMarkTexture();

    // 길드 관계
    void SetGuildRelationInfo(BASE_GUILD_ADD_INFO& guild_add_info);
    void AddGuildRelation(GUILD_RELATION& guild_relation);
    void DeleteGuildRelation(GUILD_RELATION& guild_relation);
    void DeleteGuildRelation(GUILDGUID guild_guid, eGUILD_RELATION guild_relation);
    GUILD_RELATION* GetGuildRelation(GUILDGUID guild_guid);
    GUILD_RELATION* GetGuildRelation(const TCHAR* guild_name);
    void GetRelationGuildName(GUILDGUID guild_guid, TCHAR* OUT guild_name);
    void ClearGuildRelation();
    GUILDGUID GetGuildGUID(const TCHAR* guild_name);
    bool IsHaveGuildRelation();
    int GetRelationProclaimCount(); // 적대선포한 횟수

    // 길드로그
    void ClearWareHouseLog();
    void AddWareHouseLog(STRUCT_GUILDWAREHOUSE_LOGINFO* warehouse_log_info, DWORD num_of_log);
    void ClearGuildLog();
    void AddGuildLog(DWORD guild_log_category, GuildInGameLogData* guild_log_data, DWORD num_of_log);

    // 길드랭킹
    void AddGuildRankingInfo(GUILD_RANKING_INFO* guild_ranking_info, int num_of_info);

    WORD guild_rank() const { return guild_rank_; }
    void set_guild_rank(WORD value) { guild_rank_ = value; }
#ifdef _NA_004034_20120102_GUILD_INTRODUCTION
    void set_guild_intro(TCHAR* string) { strncpy(guild_intro_, string, MAX_GUILD_INTRODUCTION_LENGTH+1); }
    TCHAR* guild_intro(){ return guild_intro_; }
#endif//_NA_004034_20120102_GUILD_INTRODUCTION
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    void   set_guild_join_request_total_info_(GUILD_JOIN_REQUEST_TOTAL_INFO* guild_join_request_total_info);    
    bool get_guildname_for_guild_join_request(const GUILDGUID& guild_guid, OUT TCHAR* guild_name);

    void init_guild_facility_container();
    void set_guild_facility_container(const GUILD_FACILITY_TOTAL_INFO& facility_total_info);
    void set_element_guild_facility(const GUILD_FACILITY_INFO& facility_info);
    void set_element_guild_facility_active(const FCODE& facility_code, const bool& is_active);
    bool get_guild_facility_info(const FCODE& facility_code, OUT GuildFacilityInfoForClient& facility_info);
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

    GuildRelationContainer& alliance_container() { return alliance_container_; }
    GuildRelationContainer& hostility_container() { return hostility_container_; }
    GuildRelationContainer& wait_hostility_container() { return wait_hostility_container_; }

    GuildWareHouseLogContainer& wareshoue_deposit_log() { return wareshoue_deposit_log_; }
    GuildWareHouseLogContainer& wareshoue_withdraw_log() { return wareshoue_withdraw_log_; }
    GuildLogInfoContainer& guild_log_info_container() { return guild_log_info_container_; }

    GuildRankingInfoContainer& guild_ranking_info_container() { return guild_ranking_info_container_; }
    GuildRankingGuildInfoContainer& guild_ranking_guild_info_container() { return guild_ranking_guild_info_container_; }
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    GuildJoinRequestInfoContainer& guild_join_request_info_container() { return guild_join_request_info_container_; }
    GuildFacilityContainer& guild_facility_container() { return guild_facility_container_; }
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

    DWORD& verify_dialog_key() { return verify_dialog_key_; }

#ifdef _NA_008334_20150608_SONNENSCHEIN
    BYTE get_sonnenschein_phase();
    void set_sonnenschein_phase(BYTE phase) { GetGuildInfo().sonnenschein_phase_ = phase; }
#endif //_NA_008334_20150608_SONNENSCHEIN
private:
    void set_recv_guild_info(bool value) { recv_guild_info_ = value; }    
    void set_recv_total_info(bool value) { recv_total_info_ = value; }

private:
    bool recv_guild_info_;
    bool recv_total_info_;
    
    GUILD_INFO guild_info_;
    WORD guild_rank_;
    GuildMemberContainer guild_member_container_;   
    STLX_STRING guild_name_;
    STLX_STRING master_name_;    
    HANDLE guild_mark_texture_;
    TCHAR guild_intro_[MAX_GUILD_INTRODUCTION_LENGTH+1];

    // 길드관계
    GuildRelationContainer alliance_container_;
    GuildRelationContainer hostility_container_;
    GuildRelationContainer wait_hostility_container_;

    // 길드로그
    GuildWareHouseLogContainer wareshoue_deposit_log_;
    GuildWareHouseLogContainer wareshoue_withdraw_log_;
    GuildLogInfoContainer guild_log_info_container_;    

    // 길드랭킹
    GuildRankingInfoContainer guild_ranking_info_container_;
    GuildRankingGuildInfoContainer guild_ranking_guild_info_container_;

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    // 길드 가입 요청 목록
    GuildJoinRequestInfoContainer guild_join_request_info_container_;
    GuildFacilityContainer guild_facility_container_;
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

    // 안내창 다이얼로그 키
    DWORD verify_dialog_key_;
};

    GuildSystemData& GetGuildSystemData();

}; //namespace GuildSystem

#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL