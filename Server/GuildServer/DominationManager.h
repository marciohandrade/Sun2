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
        DOMINATION_INFO_EMPTY,             //������ �����Ͱ� ����ִ�.
        DOMINATION_INFO_LOADING,           //������ ������ �ε����̴�.
        DOMINATION_INFO_SUCCESS,           //������ ������ �ε��� �����ߴ�.
    };
    DominationManager();
    ~DominationManager();
    
    //Ư���ð� ���� ������ ������ ������Ʈ �Ѵ�.
    void LoadDominationInfo();

    //�ε���� ���� ���� ���� ��û
    void RequestDominationAuctionInfo();
    
    //���� ���� �⺻ ������ �ִ´�.
    void InsertDominationInfo(const DOMINATION_INFO& domination_info);
    
    //���� ������ ������ ���� ������ ������.
    void SendDominationInfo(GameServerSession* server_session, MAPCODE map_code);
    
    //���� ������ ������ �����͸� �����Ѵ�.
    DOMINATION_INFO* FindDominationInfo(MAPCODE map_code);
    
    //���� ���� ��� �⺻ ������ Map�� ����
    bool InsertActionInfo(MAPCODE map_code, const DominationAuctionInfo& domination_info);   
    
    //���� ���� ��� ���� send to GameServer      
    void SendActionList(GameServerSession* server_session, MAPCODE map_code);
    
    //���� ������ ��� ������ �����.     
    void ClearDominationActionInfo(MAPCODE map_code);
    
    //��� ������ ���� �޴� ��� ������ ����Ʈ �����.   
    bool SetListForReturnHeim(MAPCODE map_code, DOMINATION_RETURNMONEY_INFO* OUT return_info, 
                              DWORD max_data, INT& index_count);
    
    //������ ��� ���� �ε� ������ üũ 
    void SetCheckActionInfo(MAPCODE map_code);
    
    //��� ���� ����Ʈ���� �ش� CharGuid������ ����    
    bool DeleteActionList(MAPCODE map_code, GUILDGUID guild_guild);
    
    //������ ���� �ε� �Ϸ� ���� �޼ҵ�
    void SetLoadDominationInfo(bool is_success);
    bool GetLoadDominationInfo()const;

    //��� ���� �ε� �Ϸ� ���� �޼ҵ�
    void SetLoadDominationAuctionInfo(bool is_success);
    bool GetLoadDominationAuctionInfo()const;

#ifdef _NA_20100604_BETTER_DOMINATION_MANAGER 
    //����� ���� ���� �� ����
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
    //�ʱ�ȭ!
    void Initialize();

    //������ ���� �ε� ���¿����� �ؾ��� ���� �����Ѵ�.
    void DominationInfoStatusProc();
    
    //���� ������ �⺻ ������ �ε� �Ѵ�.   
    void RequestDominationAreaInfo();
    
    //������ ���������� ������ ��Ҹ� �����.
    bool InsertDominationMapInfo(MAPCODE map_code);
    
    //��� �ε� ������ �ʱ�ȭ�Ѵ�.
    void SetCheckDominationActionMap();
    
    //������ �⺻ ����(����,��)�� �ε� �ߴ��� üũ, �ε� ���� �������� �ش� �κ� �ε�.
    bool CheckLoadingStatus();
    
    //������ ��ũ��Ʈ�������� ����ִ� ������ ������ �ʱ�ȭ
    void SetEmptyDominationInfo();
    
    //DBP�� ��Ŷ Send
    bool SendToDBProxy(MSG_BASE* msg, WORD size)const;

    //������ ���� �ε� Ÿ�̸� ���� �޼ҵ�
    void LoadingTimerSet();
    void LoadingTimerDisable(); 

    //�ʱ�ȭ �÷��� ���� �޼ҵ�
    bool GetIsinit();
    void SetIsInit(bool flag);

    //������ �ε� ���� ���� �޼ҵ�
    void SetDominationLoadingStatus(DominationLoadingStatus status); 
    DominationLoadingStatus GetDominationInfoStatus(); 

    //���� �������� �ʿ��� ��� ������ �ε带 �Ϸ� �Ͽ���?
    bool IsCompleteLoad();

    //��� ������ �ε带 �Ϸ� �Ͽ���?
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
