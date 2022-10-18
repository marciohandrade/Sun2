#pragma once
#ifdef _NA_007992_20150127_EVENT_MONSTER_INVASION

#include "Singleton.h"
#include "SunEventInfoParser.h"

class MonsterInvasionEvent : public util::Singleton<MonsterInvasionEvent>
{
    enum eVILLAGE_EVENT_STATE 
    {
        eVILLAGE_STATE_NONE = 0,
        eVILLAGE_STATE_NORMAL,
        eVILLAGE_STATE_EVENT_PROGRESS,  //이벤트 진행상태

        eVILLAGE_STATE_MAX
    };

    typedef std::map<MAPCODE,eVILLAGE_EVENT_STATE> VILLAGE_EVENT_STATE_MAP;
    typedef std::map<MAPCODE,eVILLAGE_EVENT_STATE>::iterator VILLAGE_EVENT_STATE_MAP_ITER;
    typedef std::pair<MAPCODE,eVILLAGE_EVENT_STATE> VILLAGE_EVENT_STATE_PAIR;
public:
    MonsterInvasionEvent();
    ~MonsterInvasionEvent() {}

    void Init();
    BOOL EventUpdate(DWORD cur_time_hhmm);
    
    BOOL StartMonsterInvasion(const CODETYPE& map_code, const BOOL& is_gm_command = FALSE);

    eVILLAGE_EVENT_STATE GetVillageEventState(const MAPCODE& map_code);

    //map_code의 마을에 몬스터침공 트리거메시지를 보낸다
    BOOL SendInvasionEventTriggerToVillage(const MAPCODE& map_code);

    BOOL AddEvent(EVENT_INFO& event_info);
    EVENT_INFO* FindEvent(const EVENT_INFO& event_info);

private:
    BOOL    _isTimeToStartEvent(DWORD cur_time_hhmm, EVENT_INFO& event_info);
    VOID    _ChangeVillageEventState(MAPCODE map_code, eVILLAGE_EVENT_STATE state);

private:
    EVENTINFO_MAP insvasion_event_map;
    VILLAGE_EVENT_STATE_MAP villages_state_map;

};
#endif //_NA_007992_20150127_EVENT_MONSTER_INVASION
