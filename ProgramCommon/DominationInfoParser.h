#pragma once

/************************************************************************/
/*          DominationInfo.txt �ļ�                                     */
/************************************************************************/

/************************************************************************/
/*
// ���°� 
DOMINATION_EVENT_ACTION_START           : ��Žð�          [��Ű���, �������ð���, ���尡��(��)] 
DOMINATION_EVENT_ACTION_END             : ��ų�            [��źҰ�, �������ð���, ���尡��(��)]
DOMINATION_EVENT_SELECT_ATTACKGUILD     : ���� ��� ����    [��źҰ�, �������ð���, ���尡��(��)]
DOMINATION_EVENT_SELECT_ATTACKGUILD_END : ���� ��� ���� �� [��źҰ�, �������ð���, ���尡��(��)]
DOMINATION_EVENT_SYSTEM_RESET           : �ý��۸���        [��źҰ�, �������úҰ�, ����Ұ�(��,��)]
DOMINATION_EVENT_WAIT                   : ���� ���         [��źҰ�, �������úҰ�, ���尡��(��,��)]
DOMINATION_EVENT_START                  : ���� ����         [��źҰ�, �������úҰ�, ���尡��(��,��)]
DOMINATION_EVENT_END                    : ���� ��           [��Ű���, �������ð���, ���尡��(��)]
DOMINATION_EVENT_ACTION_START2          : ���� ���� �ʴ� ����
DOMINATION_EVENT_MAX                    : ���� ����
*/
/************************************************************************/

#include <Singleton.h>
#include <ScriptCode.IParser.h>

struct sDOMINATION_INFO
{
    enum
    { 
#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
        REWARD_OPTION_MAX = 15,
#endif  //_NA_007128_20140326_MONDSCHIEN_DOMINATION
        LIMIT_MAX = 5, 
        TRIGGERID_MAX = 2, 
        GATEID_MAX = 3
    };
 
    MAPCODE     m_DominationMapCode;                //"MapCode"   (HASH_KEY)
    MAPCODE     m_LinkMapCode;                      //"FieldCode"
    MAPCODE     m_SubMapCode;                       //"SubFieldCode"
    int         m_Index;                            //"Index"
    CHAR        m_MapName[MAX_MAPNAME_LENGTH];      //"Name"
#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
    int         m_DominationType;                   //"DominationType" (0: �⺻, 1:��Ʈ����)
#endif  //_NA_007128_20140326_MONDSCHIEN_DOMINATION
    BYTE        m_MaxUser;                          //"MaxUser"
    MONEY       m_BaseBattingMoney;                 //"BaseBattingMoney"
    BYTE        m_BattingCharge;                    //"BattingCharge" ( ����� )
    // added by _NA_0_20100705_DOMINATION_EVENT = {
    BYTE        m_byUseFixEvent;                    //"UseFixEvent"
    WORD        m_wEventDayOfWeek;                  //"EventDayOfWeek"
    DWORD       m_dwEventTime;                      //"EventTime"
    DWORD       m_dwEventTriggerID;                 //"EventTriggerID"
    // }
    DWORD       m_StartTriggerID;                   //"StartTriggerID"
    DWORD       m_EndTriggerID;                     //"EndTriggerID"
    DWORD       m_GateID[GATEID_MAX];               //"GateID1"
    DWORD       m_GateHPTunning;                    //"GateHPTunning"
    DWORD       m_RespawnDelay;                     //"RespawnDelay"    (second)
    SLOTCODE    m_LimitSkillGroup[LIMIT_MAX];       //"LimitSkillCroup"
    SLOTCODE    m_LimitItemGroup[LIMIT_MAX];        //"LimitItemGroup0"
#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
    AttrOptionInfo m_RewardOption[REWARD_OPTION_MAX];          //"RewardBuffCode"    
#else
    AttrOptionInfo m_RewardOption[LIMIT_MAX];          //"RewardBuffCode"    
#endif  //_NA_007128_20140326_MONDSCHIEN_DOMINATION
    int         m_RewardDropHeimRate;               //"ReardDropHeimRate" (õ����)
#if defined (_NA_20100420_CHANGE_SYSTEM_MEMO_LOGIC) || \
    defined (_NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION)
    WORD        m_BattingItemCode;                  //"BattingItemCode"
#endif
#ifdef _NA_20120511_WORK_TO_DOMINATION_ADDITIONAL_REQUEST
    BYTE        m_BattingGuildLevelLimit;           //"BattingGuildLevelLimit"
#endif

    sDOMINATION_INFO()
    {
        ::ZeroMemory(this, sizeof(*this));
    }
};


class DominationInfoParser : public util::Singleton<DominationInfoParser>, public IParser
{
public:
    typedef STLX_HASH_MAP<MAPCODE, sDOMINATION_INFO> DOMINATION_SCRIPT_INFO_MAP;
    typedef STLX_VECTOR<MAPCODE> TakenMaps; // ������ ���� ���
    // ���õ� ���� �ɼ�
    struct SelectedRewardOption
    {
        SelectedRewardOption(
            const sDOMINATION_INFO* const in_domination_info = NULL, 
            const int in_option_index = 0, 
            const AttrOptionInfo* const in_option_info = NULL
        ) : 
            domination_info(in_domination_info), 
            option_index(in_option_index), 
            option_info(in_option_info)
        {
        }
        ~SelectedRewardOption()
        {
        }
        const sDOMINATION_INFO* domination_info;
        int option_index;
        const AttrOptionInfo* option_info;
    };
    typedef int SelectedRewardOptionKind; // ���õ� ���� �ɼ� ����
    typedef STLX_MAP<SelectedRewardOptionKind, SelectedRewardOption> SelectedRewardOptions; // ���õ� ���� �ɼ� ���
    
public:
    DominationInfoParser();
    ~DominationInfoParser();

public:
    virtual void Release()
    {
    }
    virtual BOOL LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload );	
    
public:
    const sDOMINATION_INFO* GetDominationInfo(const MAPCODE field_code) const;

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
    //���� �ʰ� �ð��� �´� ������ �̺�Ʈ ���¸� ������(�̹� ��ϵ� �̺�Ʈ���� �˻��ϴ� �뵵�� ���)
    eDOMINATION_EVENT_STATE GetCurEventState(const WORD day_of_week, const DWORD time, 
                                             const DWORD map_code) const;   
    
    //GM�� ����� ������ �̺�Ʈ�� ������
    const sDOMINATION_EVENT_INFO* GetGMEventInfo(const WORD day_of_week, const DWORD time,
                                                 const MAPCODE map_code) const; 
    
    //�ʵ� �ڵ�κ��� ������ ������ ������
    const eDOMINATION_TYPE GetDominationTypeFromFieldCode(const MAPCODE field_code) const;

    const MAPCODE GetMondScheinMapCode();

#else
    eDOMINATION_EVENT_STATE GetCurEventState(const WORD day_of_week, const DWORD time) const;
#endif  //_NA_007128_20140326_MONDSCHIEN_DOMINATION
    
    DWORD GetWarTime(const WORD day_of_week, const DWORD time) const;
    
    //_NA_0_20100705_DOMINATION_EVENT
    DWORD GetRandomAuctionTime(const WORD day_of_week) const; 
    
    DWORD GetRemindEventTime(
        const WORD day_of_week, const DWORD time, const eDOMINATION_EVENT_STATE event_state) const;

    const DOMINATION_SCRIPT_INFO_MAP& GetDominationInfoMap() const
    { 
        return script_infos_; 
    }

    // �� �ڵ带 �̿��Ͽ� ���� ���ɸ� �ڵ带 �����´�.
    MAPCODE GetDominationFieldCode(const MAPCODE field_code) const;
    
    // ���� ���� ���ڵ带 �̿��Ͽ� �ʵ� �� �ڵ带 �����´�.
    MAPCODE GetDominationLinkMapCode(const MAPCODE map_code) const;
    
    // �ش� ���� �⺻ ���� ���� ����
    MONEY GetAreaLimitMoney(const MAPCODE map_code) const;
    
    //������ ��ũ��Ʈ ������ �ε带 �Ϸ� �Ͽ��°�?
    bool IsLoadComplete() const
    {
        return is_loaded_;
    }

    // ������ �������κ��� ������ ���� �ɼ��� ��´�.
    SelectedRewardOptions GetRewardOption(const TakenMaps& taken_maps) const;

#ifdef _SERVER
    bool GMInsertEventInfo(const sDOMINATION_EVENT_INFO& event_info);
    void GMClearEventInfo();
#endif

#if defined (_NA_20100420_CHANGE_SYSTEM_MEMO_LOGIC) || \
    defined (_NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION)
    // �ش� �������� ������ �ʿ��� �������ڵ带 �����´�
    WORD GetBattingItemCodeByMapCode(const MAPCODE field_code) const;                                        
#endif

private:
    typedef STLX_MULTIMAP<WORD, sDOMINATION_EVENT_INFO> EventInfos;
    typedef STLX_HASH_MAP<MAPCODE, MAPCODE> MapCodes;

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
public:
    //���� ������ ���ڵ�� ���� �̺�Ʈ �������� ������ ����� ����
    EventInfos GetEventInfosFromMapCode(MAPCODE map_code);
    sDOMINATION_EVENT_INFO GetMondScheinEventInfo();
#endif  //_NA_007128_20140326_MONDSCHIEN_DOMINATION
    
private:
    bool LoadDominationInfo(const bool is_reload);
    bool LoadEventInfo(const bool is_reload);
    bool IsValidEventInfo(const sDOMINATION_EVENT_INFO& event_info) const;
    bool IsValidDominationInfo(const sDOMINATION_INFO& domination_info, const int row) const;
    const sDOMINATION_EVENT_INFO* GetEventInfo(const WORD day_of_week, const DWORD time) const;

private:
    DOMINATION_SCRIPT_INFO_MAP script_infos_;
    MapCodes map_codes_;
    EventInfos event_infos_;
    bool is_loaded_;

#ifdef _SERVER
    EventInfos gm_event_infos_;
    bool is_script_mode_;
#endif
};