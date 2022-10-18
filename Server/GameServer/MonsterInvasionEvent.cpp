#include "StdAfx.h"
#ifdef _NA_007992_20150127_EVENT_MONSTER_INVASION
#include "MonsterInvasionEvent.h"

#include "GameZoneManager.h"
#include "GameZone.h"
#include "TriggerManagerEx.h"

#include "ServerOptionParserEx.h"

MonsterInvasionEvent::MonsterInvasionEvent()
{
    insvasion_event_map.clear();
    villages_state_map.clear();
}

void MonsterInvasionEvent::Init()
{
    SunEventInfoParser* eventinfo_parser = SunEventInfoParser::Instance();
    EVENTINFO_MAP* event_map = eventinfo_parser->GetEventInfoMap();

    EVENTINFO_MAP_ITER itend = event_map->end();
    EVENTINFO_MAP_ITER it = event_map->begin();
    while(it != itend)
    {
        EVENT_INFO& eventInfo = it->second;

        if (eEVENT_MONSTER_INVASION == eventInfo.m_byEventMode) {
            AddEvent(eventInfo);
        }
        ++it;
    }
}

BOOL MonsterInvasionEvent::AddEvent( EVENT_INFO& event_info )
{
    if (FindEvent(event_info) != NULL) {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] Aready Exist Invasion Event!! [id:%d]", 
               event_info.m_dwEventID);
        return FALSE;
    }

    insvasion_event_map.insert(EVENTINFO_PAIR(event_info.m_dwEventID, event_info));
    //여기선 npccode에 mapcode가 입력되어있다
    villages_state_map.insert(VILLAGE_EVENT_STATE_PAIR((MAPCODE)event_info.m_NpcCode, eVILLAGE_STATE_NORMAL));
    return TRUE;
}

EVENT_INFO* MonsterInvasionEvent::FindEvent( const EVENT_INFO& event_info )
{
    EVENTINFO_MAP_ITER itr = insvasion_event_map.find(event_info.m_dwEventID);

    if (itr == insvasion_event_map.end()) {
        return NULL;
    }

    return &(itr->second);
}

BOOL MonsterInvasionEvent::EventUpdate(DWORD cur_time_hhmm)
{
    EVENTINFO_MAP_ITER itr = insvasion_event_map.begin();
    while (itr != insvasion_event_map.end())
    {
        EVENT_INFO& event_info = itr->second;

        //PeakTime(당일 시작/종료시간) 검사
        if (event_info.CheckEvent() == TRUE && _isTimeToStartEvent(cur_time_hhmm, event_info) == TRUE)
        {
            StartMonsterInvasion(event_info.m_NpcCode);
        }
        else {
            _ChangeVillageEventState(event_info.m_NpcCode, eVILLAGE_STATE_NORMAL);
        }
        ++itr;
    }
    return TRUE;
}

BOOL MonsterInvasionEvent::_isTimeToStartEvent( DWORD cur_time_hhmm, EVENT_INFO& event_info )
{
    const DWORD startTime = event_info.m_wPeakTimeStart;
    const DWORD endTime = event_info.m_wPeakTimeEnd;

    return ( startTime <= cur_time_hhmm && cur_time_hhmm <= endTime ) ?
        TRUE
        : FALSE;
}

BOOL MonsterInvasionEvent::SendInvasionEventTriggerToVillage(const MAPCODE& map_code)
{
    GameZone* village_zone = g_GameZoneManager.GetFieldVillageZone(map_code);

    if (village_zone == NULL) {
        return FALSE;
    }
    if (village_zone->GetZoneType() != eZONETYPE_VILLAGE) {
        //마을이 아니면 리턴
        return FALSE;
    }

    GameField* village_field = village_zone->GetEntryField();
    if (village_field == NULL) {
        return FALSE;
    }

    //트리거메시지보낸다
    TRIGGER_SERVER_EVENT trigger_msg;
    trigger_msg.event_id = 1;
    village_field->GetTriggerManager()->OnMsg(&trigger_msg);

    return TRUE;
}

VOID MonsterInvasionEvent::_ChangeVillageEventState( MAPCODE map_code, eVILLAGE_EVENT_STATE state )
{
    VILLAGE_EVENT_STATE_MAP_ITER itr = villages_state_map.find(map_code);

    if (itr == villages_state_map.end()) {
        return ;
    }

    if (itr->second != state) {
        itr->second = state;
    }
}

MonsterInvasionEvent::eVILLAGE_EVENT_STATE MonsterInvasionEvent::GetVillageEventState( const MAPCODE& map_code )
{
    VILLAGE_EVENT_STATE_MAP_ITER itr = villages_state_map.find(map_code);

    if (itr == villages_state_map.end()) {
        return eVILLAGE_STATE_NONE;
    }

    return itr->second;
}

BOOL MonsterInvasionEvent::StartMonsterInvasion( const CODETYPE& map_code, const BOOL& is_gm_command /*= FALSE*/ )
{
    BYTE channel_id = ServerOptionParserEx::Instance()->GetServerOption().monster_invasion_channel;
    if (is_gm_command == FALSE && g_pGameServer->GetKey().GetChannelID() != channel_id)
    {
        return FALSE;
    }

    MAPCODE village_map_code = (MAPCODE)map_code;
    eVILLAGE_EVENT_STATE state = GetVillageEventState(village_map_code);
    if (state != eVILLAGE_STATE_NORMAL)
    {
        //만약 해당 마을에서 이미 이벤트가 진행중이면 실행시키지않는다
        return FALSE;
    }

    //몬스터 침공에서는 NPC code를 맵코드(마을코드)로 사용한다
    if (SendInvasionEventTriggerToVillage(village_map_code) == TRUE && is_gm_command == FALSE) {
        _ChangeVillageEventState(map_code, eVILLAGE_STATE_EVENT_PROGRESS);
        return TRUE;
    }
    else if(is_gm_command == TRUE) {
        return TRUE;
    }

    return FALSE;
}
#endif //_NA_007992_20150127_EVENT_MONSTER_INVASION