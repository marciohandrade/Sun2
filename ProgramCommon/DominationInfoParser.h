#pragma once

/************************************************************************/
/*          DominationInfo.txt 파서                                     */
/************************************************************************/

/************************************************************************/
/*
// 상태값 
DOMINATION_EVENT_ACTION_START           : 경매시간          [경매가능, 성문셋팅가능, 입장가능(수)] 
DOMINATION_EVENT_ACTION_END             : 경매끝            [경매불가, 성문셋팅가능, 입장가능(수)]
DOMINATION_EVENT_SELECT_ATTACKGUILD     : 공격 길드 선정    [경매불가, 성문셋팅가능, 입장가능(수)]
DOMINATION_EVENT_SELECT_ATTACKGUILD_END : 공격 길드 선정 끝 [경매불가, 성문셋팅가능, 입장가능(수)]
DOMINATION_EVENT_SYSTEM_RESET           : 시스템리셋        [경매불가, 성문셋팅불가, 입장불가(공,수)]
DOMINATION_EVENT_WAIT                   : 전쟁 대기         [경매불가, 성문셋팅불가, 입장가능(공,수)]
DOMINATION_EVENT_START                  : 전쟁 시작         [경매불가, 성문셋팅불가, 입장가능(공,수)]
DOMINATION_EVENT_END                    : 전쟁 끝           [경매가능, 성문셋팅가능, 입장가능(수)]
DOMINATION_EVENT_ACTION_START2          : 존재 하지 않는 상태
DOMINATION_EVENT_MAX                    : 예외 상태
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
    int         m_DominationType;                   //"DominationType" (0: 기본, 1:몬트샤인)
#endif  //_NA_007128_20140326_MONDSCHIEN_DOMINATION
    BYTE        m_MaxUser;                          //"MaxUser"
    MONEY       m_BaseBattingMoney;                 //"BaseBattingMoney"
    BYTE        m_BattingCharge;                    //"BattingCharge" ( 백분율 )
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
    int         m_RewardDropHeimRate;               //"ReardDropHeimRate" (천분율)
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
    typedef STLX_VECTOR<MAPCODE> TakenMaps; // 점령한 지역 목록
    // 선택된 보상 옵션
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
    typedef int SelectedRewardOptionKind; // 선택된 보상 옵션 종류
    typedef STLX_MAP<SelectedRewardOptionKind, SelectedRewardOption> SelectedRewardOptions; // 선택된 보상 옵션 목록
    
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
    //현재 맵과 시간에 맞는 점령전 이벤트 상태를 가져옴(이미 등록된 이벤트인지 검사하는 용도로 사용)
    eDOMINATION_EVENT_STATE GetCurEventState(const WORD day_of_week, const DWORD time, 
                                             const DWORD map_code) const;   
    
    //GM이 등록한 점령전 이벤트를 가져옴
    const sDOMINATION_EVENT_INFO* GetGMEventInfo(const WORD day_of_week, const DWORD time,
                                                 const MAPCODE map_code) const; 
    
    //필드 코드로부터 점령전 종류를 가져옴
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

    // 맵 코드를 이용하여 지역 점령맵 코드를 가져온다.
    MAPCODE GetDominationFieldCode(const MAPCODE field_code) const;
    
    // 지역 점령 맵코드를 이용하여 필드 맵 코드를 가져온다.
    MAPCODE GetDominationLinkMapCode(const MAPCODE map_code) const;
    
    // 해당 지역 기본 배팅 하임 리턴
    MONEY GetAreaLimitMoney(const MAPCODE map_code) const;
    
    //점령전 스크립트 데이터 로드를 완료 하였는가?
    bool IsLoadComplete() const
    {
        return is_loaded_;
    }

    // 점령한 지역으로부터 적용할 보상 옵션을 얻는다.
    SelectedRewardOptions GetRewardOption(const TakenMaps& taken_maps) const;

#ifdef _SERVER
    bool GMInsertEventInfo(const sDOMINATION_EVENT_INFO& event_info);
    void GMClearEventInfo();
#endif

#if defined (_NA_20100420_CHANGE_SYSTEM_MEMO_LOGIC) || \
    defined (_NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION)
    // 해당 지역에서 입찰에 필요한 아이템코드를 가져온다
    WORD GetBattingItemCodeByMapCode(const MAPCODE field_code) const;                                        
#endif

private:
    typedef STLX_MULTIMAP<WORD, sDOMINATION_EVENT_INFO> EventInfos;
    typedef STLX_HASH_MAP<MAPCODE, MAPCODE> MapCodes;

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
public:
    //점령 지역의 맵코드로 점령 이벤트 정보들을 맵으로 만들어 리턴
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