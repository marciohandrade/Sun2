#pragma once
//=============================================================================================================================
/*

DOMINATION_EVENT_END:
 입장 가능(길드홀 상태)

DOMINATION_EVENT_SYSTEM_RESET:
 입장 블락
 현재 유저 강퇴
 공격 길드 셋팅
 필드 파괴, 생성 (트리거 셋팅)

DOMINATION_EVENT_WAIT:
 입장 가능
 전쟁 타이머 시간 셋팅(스톱상태)
- 입장 유저에 시간 동기화

DOMINATION_EVENT_START:
 입장 가능
 전쟁 타이머 시작
- 입장 유저에 시간 동기화

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
//=============================================================================================================================

#include "ZoneInterface.h"

typedef std::pair<GUILDGUID, INT> GUILD_COUNT;
typedef std::pair<DWORD, DWORD>    OUTSIDER;    //<key, count>
typedef STLX_LIST<OUTSIDER>        OUTSIDERS;
typedef OUTSIDERS::iterator        OUTSIDERS_IT;

typedef std::pair<GUILDGUID, GUILDGUID> KICK_GUILD;

class DominationField : public ZoneInterface
{
    _DECLARE_POOL(DominationField)

    enum { empty_guild_idx = -1 };
    enum { offense_guild, defense_guild, max_guild };
    enum { OUTSIDER_UPDATE_TIME = 100, EVENT_UPDATE_TIME = 500, KICK_TIME = 10*1000 }; 
    // 0.1초, 0.5초, 10초

public:
    DominationField(void);
    virtual ~DominationField(void);

    VOID Create(KEYTYPE key, MAPCODE MapCode, ServerSession* pServerSession);
    inline virtual eZONETYPE GetType() { return eZONETYPE_DOMINATION_FIELD; }

    virtual BOOL Process();
    virtual VOID ReadyUser( User * pUser, eSEND_TYPE type, MAPCODE FieldCode = 0,
                            DWORD dwAreaID = 0, WzVector * pwvStartPos = NULL, bool bFirstJoin = false );
    VOID SetGuildGuid(GUILDGUID offense, GUILDGUID defense, eDOMINATION_EVENT_TYPE eSetType);
    VOID KickUser(DWORD playerKey);

protected:
    virtual VOID On_create();
    virtual VOID On_destroy();
    virtual VOID On_readyuser(User* pUser);
    virtual VOID On_enteruser(User* pUser);
    virtual VOID On_leaveuser(User* pUser);
    virtual RC::eROOM_RESULT _CanJoinEach(User* pUser, eZONEPUBLIC RoomPublic, TCHAR* pszRoomPWD);


private:
#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
    typedef STLX_MULTIMAP<WORD, sDOMINATION_EVENT_INFO> EventInfos;
    //파서로 부터 현재 필드의 점령전 이벤트를 가져와 EventInfos에 저장
    VOID _SetEventInfos(MAPCODE map_code); 
    //자신의 EventInfos에서 현재 점령전 상태를 가져옴
    eDOMINATION_EVENT_STATE _GetCurEventState(const WORD day_of_week, const DWORD time) const;
    EventInfos event_infos_;
#endif //_NA_007128_20140326_MONDSCHIEN_DOMINATION

    RC::eROOM_RESULT _GuildCount(User* pUser, INT count = 1);
    std::pair<BOOL,INT> _Counting(GUILDGUID guildGuid, INT count );

    BOOL _ReturnToEntrance(User* pUser);
    INT  _GetGuildCountIndex(GUILDGUID guildGuid);
    VOID _ReSetDomination();
    VOID _KickUser(GUILDGUID guildGuid);
    VOID _UpdateTimeEvent();

    BYTE _CountingPlayer(GUILDGUID guild_guid);

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
    void _KickUsers_ExceptWinGuild(GUILDGUID winner_guild);
#endif //_NA_006826_20130722_DOMINATION_RENEWAL

public:
    static BOOL is_gm_observer;         // gm을 옵저버로 입장 시킬것인가. gm 명령어로 사용.

private:
    GUILD_COUNT guild_count_[max_guild];
    OUTSIDERS   outSider_;              // 잘못 들어온 유저 
    OUTSIDERS   outSiderOb_;            // 옵저버 유저(GM)

    util::Timer	outsider_updatetimer_;  // outSider_ 처리 타이머    //_0_20100419_DOMINATION_RETURN_TO_ENTRANCE_BUG

    BOOL        bReCreate_;    
    util::Timer	kickTimer_;            // 강퇴 대기시간 (10초),
    KICK_GUILD  pairKickGuild_;    

    util::Timer	eventUpdateTimer_;     // 분단위 이벤트 (초단위로 체킹)
    eDOMINATION_EVENT_STATE curEventState_;
};

