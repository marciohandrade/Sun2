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

	// SunEventInfoParser�� �̸� �ε��Ǿ����� üũ
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
                    // �ؿ� ������ ������ �������� �������� ���� ���Ŀ� ��ũ��Ʈ �ε� �� �ʱ�ȭ��
                    // �̷������ ������ �ؿ� ���� ��� ������ ���ؼ��� �� �������� �˻簡 �ʿ��ϴ�.
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

// �̺�Ʈ ����ġ�� �����Ͷ�.
FLOAT		PeakTimeEvent::GetEventExp( Player* pPlayer )
{
	// �� ���� m_mapPeakTimeEvent���� �̹� ������ �̺�Ʈ���� ����ִ�.

	if( !pPlayer )
		return 0.0f;

	const eFIXEXP_USRTYPEs usrType = !!pPlayer->CanApplyPCBangBenefit( FALSE ) ? EXPUSR_ONLYPCROOM : EXPUSR_NORMAL;
	const LEVELED_EXP_ARRAY& rGroup = usrType == EXPUSR_ONLYPCROOM ?
		(LEVELED_EXP_ARRAY&)m_ExpInfo.m_ExpValues[EXPUSR_ONLYPCROOM] : (LEVELED_EXP_ARRAY&)m_ExpInfo.m_ExpValues[EXPUSR_NORMAL];

	const BASE_PLAYERINFO* pCharInfo = pPlayer->GetCharInfo();
	const LEVELTYPE charLevel = pCharInfo->m_LV;

	//	(NOTE)	rValues.EXPVAL_USERTYPE
	//	-	���� ������ ���� ���� ��� ���� ���̾�����, ���� ������ ����鼭 0���� ����
	//	-	���߿� ���� ���о��� ���������� ����Ǵ� ��츦 ����� �������� �ʴ´�.
	//	(NOTE)	rGroup[charLevel]
	//	-	�ش� ������ �´� ����ġ�� ���� ���� �̹� ���Ǿ� �����Ǿ� �ִ�.
	const sFIXEXP_VALUEs& rValues = rGroup[charLevel];
	FLOAT fAddExpRatio = rValues.EXPVAL_USERTYPE;

	#ifdef __NA_0_20061109_EXP_EVENT_
	{
		// ���� �÷��� �ð��� ���� �߰� ����ġ
		const WORD wPlayingTime = pPlayer->GetFatigueManager()->GetMinutePlayTime();
		if( wPlayingTime <= 60*2 )
			fAddExpRatio += rValues.EXPVAL_ACCUMULATED_TIME_60x2;
		else if( wPlayingTime <= 60*4 )
			fAddExpRatio += rValues.EXPVAL_ACCUMULATED_TIME_60x4;
		else if( wPlayingTime <= 60*6 )
			fAddExpRatio += rValues.EXPVAL_ACCUMULATED_TIME_60x6;
	};
	#endif

	//	������ �ּ��ѵ��� ���� �� �ֵ��� ����...-.-;;; �̰� ���� �����...
	//	�Ⱓ �� PeakTime�� �̹� ���Ǿ� �ִ�. ���� map���� �ʿ��� �͸� ��� �ִ�.
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
	//event_info�� �����
	EVENT_INFO event_info;
	ZeroMemory( &event_info, sizeof(event_info) );

    event_info.SetGMEventWithoutID(event_mode, start_date, start_time, end_date, end_time, reward_ratio);
    event_info.m_dwEventID = GetGmEventTempIndex();

    //GM�̺�Ʈ�� �߿��� �ߺ�üũ�� �Ѵ�.
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
        //��ũ��Ʈ�� ������� �̺�Ʈ�� �н�
        if (iter->second.is_gm_event_ == false) {
            ++iter;
            continue;
        }

        EVENT_INFO* registered_event = &iter->second;

        //�ߺ��ȵǴ� ���
        if( event_info.m_start_time > registered_event->m_end_time &&
            event_info.m_end_time < registered_event->m_start_time ) {
                ++iter;
        }
        //�ߺ��Ǵ� ���
        else {
            iter = m_mapPeakTimeEvent.erase(iter);
        }
    }
    return true;
}

bool PeakTimeEvent::DeleteProgressEvent()
{
    // GM�̺�Ʈ�� �ߺ���ϵ��� �ʴ´�.
    // '�������� GM�̺�Ʈ' �Ѱ��� �����ϸ� �ȴ�.
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

    //���� ��Ŷ
    MSG_CG_GMEVENT_PEAKTIME_EVENTLIST_CMD cmd_packet;
    int array_counter = 0;

    //Ȱ������ �̺�Ʈ����Ʈ
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

    // ��Ȱ������ �̺�Ʈ ����Ʈ
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

    //Ȱ������ �̺�Ʈ����Ʈ
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





























