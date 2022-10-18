#pragma once

#include "gamezone.h"
#include "FieldPvP.h"

#include <boost/tuple/tuple.hpp>

//_NA_20100604_BETTER_DOMINATION_MANAGER
#include "GameDominationManager.h"

struct NotifyTimeInfo
{
    eDOMINATION_NOTIFY_TIME type_of_time_;
    uint8 notify_time_;
};

struct MatchCheckCodeFilters;

class GameDominationField : public GameZone
{
    enum eGuildType 
    { 
        empty_idx = -1, 
        offense_guild = 0, 
        defense_guild, 
        max_guild 
    };

    enum eDataType  
    { 
        idx_guildguid, 
        crystalwarp_cooltime, 
        crystalwarp_1st, 
        crystalwarp_2nd, 
        max_crystalwarp_tuple_idx 
    };

    enum{dist_randomposinaround = 0};
    enum{time_synk = 30*1000, start_notify = 10*60*1000, event_reward = 10*1000};

    typedef boost::tuples::tuple<GUILDGUID, util::Timer, CrystalWarp*, CrystalWarp*> CrystalWarpTuple;

    typedef STLX_MULTIMAP<eDOMINATION_NOTIFY_TIME, NotifyTimeInfo> NotifyTime;
    typedef NotifyTime::iterator NotifyTimeIterator;
   
    typedef std::pair<eGuildType, eDataType> CrystalWarpIndex;

    __DECL_CUSTOMPOOL_PTR(GameDominationField)

public:
    GameDominationField(void);
    virtual ~GameDominationField(void);

    virtual void Update(DWORD dwDeltaTick);
    virtual void OnCreate();
    virtual void OnDestroy();
    virtual void Ready(Player * pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector * pwvStartPos);
    virtual BOOL Join(Player * pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector * pwvStartPos);
    virtual BOOL MoveField(Player * pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector * pwvStartPos);
    virtual	void SetZone(Object *pObject, MAPCODE FieldCode, WzID AreaID, WzVector * pwvStartPos);
    virtual	void Leave(Player * pPlayer);
    virtual	void LeaveForWarp(Player* pPlayer);
    virtual void ResetTrigger();
    virtual RC::eSTATUS_RESULT ResurrectionAtSafezone( Player *pPlayer );
    virtual eZONETYPE GetZoneType();
    virtual BOOL CanPVP(Player* pAttacker, Player* pTarget, OUT RC::eSKILL_RESULT* pErrCode);
    virtual BOOL CanPVP(Player* pAttacker, Player* pTarget, OUT RC::eBATTLE_RESULT* pErrCode);
    //_NA_008706_20160229_TEXTLOG_PK
    virtual void KillPlayer(Player* pAttacker, Player* pDeadPlayer, BOOL bSummoned = FALSE, DAMAGETYPE damage_value = 0);
    virtual BOOL IsPvPAtt(Player* pPlayer, WORD PTATroubld);
    virtual eUSER_RELATION_TYPE	IsFriendPVP(Player* pAttacker,Player* pTarget);
    virtual void GetArea(GameField* pField, Player* pPlayer, MAPCODE FieldCode, 
                         WzID& AreaID, WzVector& rwvStartPos );
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
    void SuccessStamp(Player* player);
#else
    void SuccessStamp();
#endif // _NA_006826_20130722_DOMINATION_RENEWAL
    void ReqKickPlayer(Player* pPlayer);
    void SetRewardEventInfo(DOMINATION_REWARD_EVENT_INFO reward_event);
    void EndEventTrigger();
    void GMRewardEventReset(const WORD wDayOfWeek, const DWORD dwTime);
    util::Timer* GetCoolTimer(Character* pOwnerChar);

    void SetEventState(eDOMINATION_EVENT_STATE eventState);
    eDOMINATION_EVENT_STATE GetEventState();

    void SetGuildGuid(GUILDGUID defense, GUILDGUID offense, eDOMINATION_EVENT_TYPE eSetType);
    GUILDGUID GetOffenseGuild();
    GUILDGUID GetDefenseGuild();
    //_NA_20100604_BETTER_DOMINATION_MANAGER
    void UpdateGuildInfoByDomination(MAPCODE map_code);

    BOOL InsertCrystalWarp(GUILDGUID guildGuid, CrystalWarp* pCrystalWarp);
    void OnDeadCrystalWarp(DWORD dwObjkey);
    RC::eSUMMON_RESULT SetCrystalWarpCommand(eCRYSTALWARP_COMMAND eCommand, GUILDGUID guildGuid, 
                                             WzVector& wvPos, DWORD dwObjKey, Player* pPlayer);
    bool DestroyCrystalWarp(Player* pPlayer, DWORD dwObjkey);
    void InsertDominationInfo(const DOMINATION_INFO* domination_info);

    //_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
    void SetStampStartTime();
    void InitStampCheckTime();
    bool IsSuccessStamp();
    //_NA_003457_20111013_EXCEPT_DOMINATION_REWARD
    void ApplyAllRewardOption(const bool is_reward = true) const;

private:
    void _Init(int idxWarp);
    void _InitAllNPC();

    void _SendDominationNotify(eDOMINATION_NOTIFY_TIME notify_type);
    void _SendKickNotify();
    void _SendAllPlayerTimeSynk(eDOMINATION_EVENT_STATE eventState);

    void _SetWarTime();
    void _EndWarTime();
    void _StartWarTime(eDOMINATION_EVENT_STATE eventState);

    BOOL _EventTriggerActive();
    void _StartTriggerActive();
    void _EndTriggerActive();

    CrystalWarpIndex& _Flip(CrystalWarpIndex& idx);
    CrystalWarp* _GetCrystalWarp(const CrystalWarpIndex& pairIdx);
    CrystalWarpIndex _FindCrystalWarpIdx(GUILDGUID guildGuid, DWORD dwObjkey);
    BOOL _IsCrystalCountFull(GUILDGUID guildGuid);
    bool _DestroyCrystalWarp(const CrystalWarpIndex& pairIdx);
    bool _CrystalWarpCommandSkill(Player* pPlayer, SLOTCODE skillCode, 
                                 DWORD dwMainTargetKey, WzVector& wvPos);
    void _NotifyCrystalWarpState(GUILDGUID guildGuid, 
                                 DWORD dwCrystalWarpKey, eCRYSTALWARP_STATE eState);

    void _SetAttrGate();
    void _StartRewardEvent(const WORD wDayOfWeek, const DWORD dwTime);
    WzID _GetStartingArea(const GUILDGUID guildGuid);
    void _ResultUpdate(eDOMINATION_EVENT_TYPE result_type);
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
    void _ResultUpdate(eDOMINATION_EVENT_TYPE result_type, Player* player);
#endif // _NA_006826_20130722_DOMINATION_RENEWAL
#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
    typedef STLX_MULTIMAP<WORD, sDOMINATION_EVENT_INFO> EventInfos;
    //�ļ��� ���� ���� �ʵ��� ������ �̺�Ʈ�� ������ EventInfos�� ����
    VOID _SetEventInfos(MAPCODE map_code); 
    //�ڽ��� EventInfos���� ���� ������ �̺�Ʈ�� ������
    const sDOMINATION_EVENT_INFO* _GetCurEventInfo(const WORD day_of_week, const DWORD time) const;
    //���� �������� ���� �ð��� ����
    DWORD _GetWarTime(const WORD day_of_week, const DWORD time) const;
    //���� ������ �̺�Ʈ �ð��� ����
    DWORD _GetRemindEventTime(
        const WORD day_of_week, const DWORD time, const eDOMINATION_EVENT_STATE event_state) const;
    //��Ʈ���� ������ ��� ������Ʈ
    bool ResultUpdateForMondshien(); 
    //��Ʈ���� ���������� ���� ������ ���� ����
    bool _TurnOver(Player* player);
    //���� ���� ��Ŷ ����
    void _SendDefenseGuildAllienceListToAll(GUILDGUID defense_guild_guid);
    //��� �÷��̾���� ���� �������� �̵�
    void _AllPlayerTeleportToStartingArea();
    //�ڽ��� ������ �̺�Ʈ ����
    EventInfos event_infos_;
    //������ ������ ���� ���� �÷���
    void _SetUpdatedResult(const bool updated);
    bool _GetUpdatedResult();
    bool updated_result_;
private:
#endif  //_NA_007128_20140326_MONDSCHIEN_DOMINATION
    void _FailStamp();
    int _FindGuildIdx(GUILDGUID guildGuid);
    //���� �˸� �޼����� �����Ѵ�.(�˸� ���� , �˸� Ƚ��, �˸� ����(��))
    void _SetNotifyTime(const eDOMINATION_NOTIFY_TIME notify_type, const uint8 count_notify, const uint8 interval);
    const NotifyTimeInfo* _FindNotifyTimeInfo(eDOMINATION_NOTIFY_TIME notify_type,uint8 notify_time)const;
    void _DeleteNotifyTimeInfo(eDOMINATION_NOTIFY_TIME notify_type,uint8 notify_time);
    eDOMINATION_EVENT_STATE _GetEventbyNotifytype(const eDOMINATION_NOTIFY_TIME notify_type)const;

private:
    FieldPvPRuleManager fieldPvPRule_;
    MatchCheckCodeFilters* pCheckFilters_;
    CrystalWarpTuple tupleWarp[max_guild];

    DWORD dwWarTime_;
    util::Timer	warTimer_;//������ ������ 100�и��� ���� (ms üŷ)
    util::Timer	synkTimer_;//Ŭ���̾�Ʈ�� ������ ������ Ÿ�̸� ����ȭ�� (30��)
    util::Timer	reward_event_timer_;//���� �̺�Ʈ�� Ÿ�̸�(_NA_0_20100616_DOMINATION_EVENT)

    BOOL bGateHPTunning_;
    DWORD dwStartTriggerID_;
    DWORD dwEndTriggerID_;
    DWORD dwEventTriggerID_;

    DOMINATION_REWARD_EVENT_INFO reward_event_;
    DOMINATION_REWARD_EVENT_INFO::EventStats event_stats_;//���˵����� �̺�Ʈ�� ����� ���, �̺�Ʈ ���������� ���δ�.
    
    NotifyTime startNotifyTime_;
    eDOMINATION_EVENT_STATE curEventState_;
    DWORD stamp_start_time_;//������ ���� ���� �ð�
};

inline int 
GameDominationField::_FindGuildIdx(GUILDGUID guildGuid) {
#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION 
    //������尡 �ƴϸ� ��� ������� �̴�.
    if (guildGuid == GetDefenseGuild())
    {
        return defense_guild;
    }
    else
    {
        return offense_guild;
    }
#else
    return _FindCrystalWarpIdx(guildGuid,0).first;
#endif  //_NA_007128_20140326_MONDSCHIEN_DOMINATION
}

inline GameDominationField::CrystalWarpIndex& 
GameDominationField::_Flip(CrystalWarpIndex& idx) {
    idx.second = (idx.second == crystalwarp_1st ? crystalwarp_2nd : crystalwarp_1st);
    return idx;
}

inline CrystalWarp* 
GameDominationField::_GetCrystalWarp(const CrystalWarpIndex& pairIdx)
{
    return pairIdx.second == crystalwarp_1st 
        ? tupleWarp[pairIdx.first].get<crystalwarp_1st>()
#ifdef _NA_006710_20130509_CRYSTALWARP_BUG
        : pairIdx.second == crystalwarp_2nd 
        ? tupleWarp[pairIdx.first].get<crystalwarp_2nd>()
        : NULL;
#else
        : tupleWarp[pairIdx.first].get<crystalwarp_2nd>();
#endif // _NA_006710_20130509_CRYSTALWARP_BUG

}

inline void 
GameDominationField::SetRewardEventInfo(DOMINATION_REWARD_EVENT_INFO reward_event) 
{ 
    reward_event_ = reward_event; 
}

inline eZONETYPE 
GameDominationField::GetZoneType() 
{ 
    return eZONETYPE_DOMINATION_FIELD; 
}

inline GUILDGUID 
GameDominationField::GetOffenseGuild() 
{ 
    return tupleWarp[offense_guild].get<idx_guildguid>(); 
}

inline GUILDGUID 
GameDominationField::GetDefenseGuild() 
{ 
    return tupleWarp[defense_guild].get<idx_guildguid>(); 
}

inline eDOMINATION_EVENT_STATE 
GameDominationField::GetEventState()
{ 
    return curEventState_; 
}