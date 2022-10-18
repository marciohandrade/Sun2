#include "StdAfx.h"
#include "PeakTimeEvent.h"
#include "Player.h"
#include "FatigueManager.h"

#ifdef _NA_007919_20141211_ADD_GMCOMMAND_EVENT
#include "EventManager.h"

int PeakTimeEvent::gmevent_eventid_counter_ = 1000;
#endif //_NA_007919_20141211_ADD_GMCOMMAND_EVENT

PeakTimeEvent::PeakTimeEvent(VOID)
{
}

PeakTimeEvent::~PeakTimeEvent(VOID)
{
}

VOID		PeakTimeEvent::Init()
{
	m_mapPeakTimeEvent.clear();

	EVENTINFO_MAP* pEventInfoMap = SunEventInfoParser::Instance()->GetEventInfoMap();

	// SunEventInfoParser가 미리 로딩되었는지 체크
	if( !pEventInfoMap->size() )
	{
//		SUNLOG( eCRITICAL_LOG, "[PeakTimeEvent::Init] Call After SunEventInfoParser!!" );
	}
    //_NA_002316_20110310_WORLD_SPECIFIC_PEAKTIME_EVENT
    const BYTE cur_world_id = (g_pGameServer->GetKey()).GetWorldID();

	EVENTINFO_MAP_ITER itend = pEventInfoMap->end();
	for( EVENTINFO_MAP_ITER it = pEventInfoMap->begin(); it != itend ; ++it )
	{
		EVENT_INFO& rEventInfo = it->second;

		switch( rEventInfo.m_byEventMode )
		{
		case eEVENT_PEAKTIME:
		case eEVENT_PCBANG_PEAKTIME:
			{
                // added filter by _NA_002316_20110310_WORLD_SPECIFIC_PEAKTIME_EVENT
                if (cur_world_id != 0)
                {
                    // 해외 버전은 마스터 서버에서 서버정보 받은 이후에 스크립트 로딩 및 초기화가
                    // 이루어지기 때문에 해외 버전 기능 지원을 위해서는 이 시점에서 검사가 필요하다.
                    const BYTE event_world_id = static_cast<BYTE>(rEventInfo.m_NpcCode);
                    if (event_world_id != 0 && event_world_id != cur_world_id)
                    {
                        SUNLOG(eCRITICAL_LOG, 
                               _T("[PeakTimeEvent] Dropped event#%d for world#%d, on world#%d"), 
                               rEventInfo.m_dwEventID, 
                               event_world_id, 
                               cur_world_id);
                        continue;
                    }
                }
				AddEvent( rEventInfo );
			}
			break;
		}
	} //end for
}

// implemented by _NA_002316_20110310_WORLD_SPECIFIC_PEAKTIME_EVENT
void PeakTimeEvent::SieveEventsByWorldId()
{
    BYTE my_world_id = g_pGameServer->GetKey().GetWorldID();
    EVENTINFO_MAP_ITER it = m_mapPeakTimeEvent.begin();
    while (it != m_mapPeakTimeEvent.end())
    {
        EVENT_INFO& event_info = it->second;
        ASSERT(event_info.m_byEventMode == eEVENT_PEAKTIME ||
            event_info.m_byEventMode == eEVENT_PCBANG_PEAKTIME);
        BYTE target_world_id = (BYTE)event_info.m_NpcCode;
        if (target_world_id != 0 && target_world_id != my_world_id)
        {
            it = m_mapPeakTimeEvent.erase(it);
            SUNLOG(eCRITICAL_LOG, "[PeakTimeEvent] Dropped event#%d for world#%d, on world#%d", 
                event_info.m_dwEventID, target_world_id, my_world_id);
        }
        else {
            ++it;
        }
    }
}

VOID		PeakTimeEvent::AddEvent( EVENT_INFO& rEventInfo )
{
	if( FindEvent( rEventInfo.m_dwEventID ) )
		return;

	m_mapPeakTimeEvent.insert( EVENTINFO_PAIR(rEventInfo.m_dwEventID, rEventInfo) );
}

EVENT_INFO*	PeakTimeEvent::FindEvent( DWORD dwEventID )
{
	EVENTINFO_MAP_ITER it = m_mapPeakTimeEvent.find( dwEventID );

	if( it != m_mapPeakTimeEvent.end() )
	{
		return &(it->second);
	}

	return NULL;
}

// 이벤트 경험치를 가져와라.
FLOAT		PeakTimeEvent::GetEventExp( Player* pPlayer )
{
	// 현 시점 m_mapPeakTimeEvent에는 이미 적합한 이벤트만이 담겨있다.

	if( !pPlayer )
		return 0.0f;

	const eFIXEXP_USRTYPEs usrType = !!pPlayer->CanApplyPCBangBenefit( FALSE ) ? EXPUSR_ONLYPCROOM : EXPUSR_NORMAL;
	const LEVELED_EXP_ARRAY& rGroup = usrType == EXPUSR_ONLYPCROOM ?
		(LEVELED_EXP_ARRAY&)m_ExpInfo.m_ExpValues[EXPUSR_ONLYPCROOM] : (LEVELED_EXP_ARRAY&)m_ExpInfo.m_ExpValues[EXPUSR_NORMAL];

	const BASE_PLAYERINFO* pCharInfo = pPlayer->GetCharInfo();
	const LEVELTYPE charLevel = pCharInfo->m_LV;

	//	(NOTE)	rValues.EXPVAL_USERTYPE
	//	-	레벨 구분이 없을 때는 모두 계산된 값이었지만, 레벨 구분이 생기면서 0으로 설정
	//	-	나중에 레벨 구분없이 공통적으로 적용되는 경우를 대비해 제거하지 않는다.
	//	(NOTE)	rGroup[charLevel]
	//	-	해당 레벨에 맞는 경험치에 대한 값이 이미 계산되어 설정되어 있다.
	const sFIXEXP_VALUEs& rValues = rGroup[charLevel];
	FLOAT fAddExpRatio = rValues.EXPVAL_USERTYPE;

	#ifdef __NA_0_20061109_EXP_EVENT_
	{
		// 누적 플레이 시간에 따른 추가 경험치
		const WORD wPlayingTime = pPlayer->GetFatigueManager()->GetMinutePlayTime();
		if( wPlayingTime <= 60*2 )
			fAddExpRatio += rValues.EXPVAL_ACCUMULATED_TIME_60x2;
		else if( wPlayingTime <= 60*4 )
			fAddExpRatio += rValues.EXPVAL_ACCUMULATED_TIME_60x4;
		else if( wPlayingTime <= 60*6 )
			fAddExpRatio += rValues.EXPVAL_ACCUMULATED_TIME_60x6;
	};
	#endif

	//	루프를 최소한도로 줄일 수 있도록 하자...-.-;;; 이걸 줄일 방법은...
	//	기간 및 PeakTime은 이미 계산되어 있다. 현재 map에는 필요한 것만 들어 있다.
	EVENTINFO_MAP_ITER itend = m_mapPeakTimeEvent.end();
	for( EVENTINFO_MAP_ITER it = m_mapPeakTimeEvent.begin(); it != itend ; ++it )
	{
		EVENT_INFO& rEventInfo = it->second;

		const BOOL bNewChar = (rEventInfo.m_start_time <= pCharInfo->m_CreateTime) && (pCharInfo->m_CreateTime <= rEventInfo.m_end_time);
		if( bNewChar )
			fAddExpRatio += rEventInfo.m_fRewardExpRate2;
		//fAddExpRatio += rValues.EXPVAL_NEWCHAR_UNDER30LEVEL;
	}

	return fAddExpRatio;
}

#ifdef _NA_007919_20141211_ADD_GMCOMMAND_EVENT
bool PeakTimeEvent::AddGmEvent( eEVENT_MODE event_mode, int start_date, WORD start_time, int end_date, 
                                WORD end_time, int reward_ratio )
{
	//event_info를 만든다
	EVENT_INFO event_info;
	ZeroMemory( &event_info, sizeof(event_info) );

    event_info.SetGMEventWithoutID(event_mode, start_date, start_time, end_date, end_time, reward_ratio);
    event_info.m_dwEventID = GetGmEventTempIndex();

    //GM이벤트들 중에서 중복체크를 한다.
    DeleteExistOverlapEvent(event_info);
	m_mapPeakTimeEvent.insert( EVENTINFO_PAIR(event_info.m_dwEventID, event_info) );

	return true;
}

bool PeakTimeEvent::AddGmEvent(EVENT_INFO event_info )
{
    event_info.m_dwEventID = GetGmEventTempIndex();

    DeleteExistOverlapEvent(event_info);
    m_mapPeakTimeEvent.insert(EVENTINFO_PAIR(event_info.m_dwEventID, event_info));

    return true;
}

bool PeakTimeEvent::DeleteExistOverlapEvent( EVENT_INFO& event_info )
{
    EVENTINFO_MAP_ITER iter = m_mapPeakTimeEvent.begin();
    while(iter != m_mapPeakTimeEvent.end())
    {
        //스크립트로 만들어진 이벤트는 패스
        if (iter->second.is_gm_event_ == false) {
            ++iter;
            continue;
        }

        EVENT_INFO* registered_event = &iter->second;

        //중복안되는 경우
        if( event_info.m_start_time > registered_event->m_end_time &&
            event_info.m_end_time < registered_event->m_start_time ) {
                ++iter;
        }
        //중복되는 경우
        else {
            iter = m_mapPeakTimeEvent.erase(iter);
        }
    }
    return true;
}

bool PeakTimeEvent::DeleteProgressEvent()
{
    // GM이벤트는 중복등록되지 않는다.
    // '진행중인 GM이벤트' 한개만 제거하면 된다.
    EVENT_INFO* registered_event = NULL;

    EVENTINFO_MAP_ITER iter = m_mapPeakTimeEvent.begin();
    while(iter != m_mapPeakTimeEvent.end())
    {
        registered_event = &iter->second;

        if (registered_event->is_gm_event_ == true && registered_event->CheckEvent() == TRUE)
        {
            SendEventNotifyPacket(ePEAKTIME_END, registered_event->m_fRewardExpRate);
            m_mapPeakTimeEvent.erase(iter);
            return true;
        }
        else {
            ++iter;
        }
    }
    return false;
}

bool PeakTimeEvent::SendGMEventList( Player* gm_player )
{
    IEventControlPeakTime* event_control = GetEventControlClass();
    IEventControlPeakTime::PEAK_EVENT_LIST* subtract_event_list = event_control->GetSubtractEventList();

    //보낼 패킷
    MSG_CG_GMEVENT_PEAKTIME_EVENTLIST_CMD cmd_packet;
    int array_counter = 0;

    //활성중인 이벤트리스트
    EVENTINFO_MAP_ITER iter = m_mapPeakTimeEvent.begin();
    while(iter != m_mapPeakTimeEvent.end())
    {
        if (array_counter >= cmd_packet.event_data_max_count) {
            break;
        }

        EVENT_INFO* registered_event = &iter->second;
        if (registered_event->is_gm_event_ == false) {
            ++iter;
            continue;
        }
 
        cmd_packet.event_data[array_counter].start_date = registered_event->m_start_time;
        cmd_packet.event_data[array_counter].end_date = registered_event->m_end_time;
        cmd_packet.event_data[array_counter].exp_ratio = registered_event->m_fRewardExpRate;
        ++array_counter;

        ++iter;
    }

    // 비활성중인 이벤트 리스트
    IEventControlPeakTime::PEAK_EVENT_LIST_IT list_it = subtract_event_list->begin();
    while(list_it != subtract_event_list->end())
    {
        if (array_counter >= cmd_packet.event_data_max_count) {
            break;
        }

        EVENT_INFO& event_info = list_it->second;

        if (event_info.is_gm_event_ == false ) {
            ++list_it;
            continue;
        }

        cmd_packet.event_data[array_counter].start_date = event_info.m_start_time;
        cmd_packet.event_data[array_counter].end_date = event_info.m_end_time;
        cmd_packet.event_data[array_counter].exp_ratio = event_info.m_fRewardExpRate;
        ++array_counter;

        ++list_it;

    }

    cmd_packet.data_count = array_counter;
    gm_player->SendPacket(&cmd_packet, sizeof(cmd_packet));
    
    return true;
}

VOID PeakTimeEvent::SendEventNotifyPacket( ePEAKTIME_EVENT_NOTIFY type, FLOAT exp_ratio, 
                                           BYTE start_day /*=0*/, DWORD start_time /*=0*/, 
                                           BYTE end_day /*=0*/, DWORD end_time /*=0*/ )
{
    MSG_CG_EVENT_PEAKTIME_NOTIFY_BRD brd_msg;
    brd_msg.notify_type = type;

    if (type != ePEAKTIME_END) {
        brd_msg.start_day = start_day;
        brd_msg.start_time = start_time;

        brd_msg.end_day = end_day;
        brd_msg.end_time = end_time;
    }

    brd_msg.exp_ratio = exp_ratio;

    PlayerManager::Instance()->SendPacketToAllPlayer(&brd_msg, sizeof(brd_msg));
}

VOID PeakTimeEvent::SendProgressEventExpRateInfo( Player* player )
{
    if (player == NULL) {
        return;
    }

    MSG_CG_GMEVENT_PEAKTIME_PROGRESS_EVENTINFO_CMD cmd_msg;

    //활성중인 이벤트리스트
    EVENTINFO_MAP_ITER iter = m_mapPeakTimeEvent.begin();
    while(iter != m_mapPeakTimeEvent.end())
    {
        EVENT_INFO* registered_event = &iter->second;
        if (registered_event->is_gm_event_ == false) {
            ++iter;
            continue;
        }
        cmd_msg.exp_ratio = registered_event->m_fRewardExpRate;

        ++iter;
    }

    player->SendPacket(&cmd_msg, sizeof(cmd_msg));
}

#endif //_NA_007919_20141211_ADD_GMCOMMAND_EVENT





























