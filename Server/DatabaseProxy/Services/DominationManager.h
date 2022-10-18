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
    
    // ���κ��� ������ ���� �бⰡ �Ϸ� �Ǿ����� �˻��Ѵ�.
    bool IsCompleteLoading() const;
    
    // ������ ��ũ��Ʈ �бⰡ �Ϸ�Ǹ� ��ũ��Ʈ�κ��� �⺻���� ���� ������ �����Ѵ�.
    void OnScriptsLoaded();
    
    // ���� ���� �о���⸦ �Ϸ� ���·� �����Ѵ�.
    void SetCompleteLoadAreaInfo();
    
    // ���� ������ �ʱ�ȭ�Ѵ�.
    void ClearAreaInfos();

    // ���� ������ �����Ѵ�.
    bool InsertAreaInfo(const DOMINATION_INFO& area_info);
    
    // ���� ������ ���� ������ ������.
    bool SendAreaInfo(GameServerSession* const game_server, const MAPCODE map_code) const;
    
    DOMINATION_INFO* FindDominationInfo(const MAPCODE map_code);
    
    // ���� ������ �ʱ�ȭ�Ѵ�.
    bool ClearAuctions(const MAPCODE map_code);

    // ���� ������ �����Ѵ�.
    bool InsertAuction(const MAPCODE map_code, const DominationAuctionInfo& auction);   
    
    // ���� ������ ���� ������ ������.
    bool SendAuctions(GameServerSession* const game_server, const MAPCODE map_code) const;
    
    // ������ ������ ���� �޴� ��� ������ ����� �����.   
    bool SetListForReturnHeim(
        const MAPCODE map_code, 
        DOMINATION_RETURNMONEY_INFO* OUT return_info, 
        const int max_data, 
        int& index_count) const;
    
    // ���� ���� �о���⸦ �Ϸ� ���·� �����Ѵ�.
    bool SetCompleteLoadAuctions(const MAPCODE map_code);
    
    // ���� ������ �����Ѵ�.
    bool DeleteAuction(const MAPCODE map_code, const GUILDGUID guild_guild);
    
    // ������ ���� ������ ��´�.
    WORD GetNumOfDominationArea(const GUILDGUID guild_guid) const;

    // ���� �ο� �о���⸦ �Ϸ� ���·� �����Ѵ�.
    bool SetCompleteLoadMembers(const MAPCODE map_code);
    
    // ���� �ο��� �ʱ�ȭ�Ѵ�.
    bool ClearMembers(const MAPCODE map_code);
    
    // ���� �ο��� �����Ѵ�.
    bool InsertMember(const MAPCODE map_code, const DominationMemberInfo& member);
    
    // ���� �ο��� �����Ѵ�.
    bool DeleteMember(const MAPCODE map_code, const CHARGUID member_guid);
    
    // ���� ������ ���� �ο� ������ ������.
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

    // ������ ���� ����
    struct Domination
    {
#ifdef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY 
        typedef STLX_HASH_MAP<GUILDGUID, DominationAuctionInfo> AuctionList;
#else
        typedef STLX_MULTIMAP<MONEY, DominationAuctionInfo> AuctionList;
#endif
        typedef STLX_HASH_MAP<CHARGUID, DominationMemberInfo> MemberList;
        
        MAPCODE field_code;
        DOMINATION_INFO area_info; // ���� ����
        AuctionList auctions; // ���� ����
        MemberList members; // ���� �ο� ����

        bool is_load_area_info; // ���� ���� �б� �Ϸ� ����
        bool is_load_auctions; // ���� ���� �б� �Ϸ� ����
        bool is_load_members; // ���� �ο� ���� �б� �Ϸ� ����
    
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
    // DB�� ��� ���� ���� ���Ÿ� ��û�Ѵ�.
    void UpdateContinentLordInfo(const bool send_game_server = false);
    // ���� ������ ��� ���� ������ ������.
    bool SendDominationContinentLordInfo(const MAPCODE map_code);
    // ����� ���� ������ �����Ѵ�.
    void SetDominationContinentLordInfo(const sDOMINATION_CONTINENT_LORD_INFO lord_info); 
    // ����� ���� ������ ���´�.
    const sDOMINATION_CONTINENT_LORD_INFO* GetDominationContinentLordInfo(int continent) const;
private:
    //����� �뿵���� ������ �����Ѵ�.<�����ȣ, �ε�����>
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