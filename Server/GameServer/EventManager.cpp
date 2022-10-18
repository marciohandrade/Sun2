#include "stdafx.h"
#include "EventManager.h"
#include <SunEventInfoParser.h>

#ifdef _NA_007992_20150127_EVENT_MONSTER_INVASION
#include "MonsterInvasionEvent.h"
#endif //_NA_007992_20150127_EVENT_MONSTER_INVASION

struct QueryEventGetBuffEventIDbyNpcID : public QueryEvent
{
	QueryEventGetBuffEventIDbyNpcID( const MONSTERCODE npcCode, EventQueryResult::BuffEventIDs* INOUT pResult )
		: QueryEvent(QueryEvent::Q_GET_BUFF_EVENTID_BY_NPCID)
		, m_Arg_NpcCode(npcCode)
		, m_EventID_Info(pResult)
	{
	};
	const MONSTERCODE					m_Arg_NpcCode;
	EventQueryResult::BuffEventIDs*		m_EventID_Info;
};


//������������������������������������������������������������������������������������������������������������
//
WORK_KEY					EventManager::ms_WorkKey;
EventManager*				EventManager::ms_pEventManager;
EventManager::IEventState*	EventManager::ms_EventStates[EMS_MAX_EVENT_STATES] = { NULL, };

//������������������������������������������������������������������������������������������������������������
//
VOID	EventManager::IEventState::ChangeState( EventManager::eEVENT_MGR_STATE state )
{
	if( (EventManager::EMS_INIT <= state) && (state<EventManager::EMS_MAX_EVENT_STATES) )
	{
		IEventState* pIEventState = GetEventManager()->ms_EventStates[ state ];
		GetEventManager()->_ChangeState( pIEventState );
	}
}

//������������������������������������������������������������������������������������������������������������
//
class IEventStateInit : public EventManager::IEventState
{
private:	typedef EventManager::eEVENT_MGR_STATE	eEVENT_MGR_STATE;
public:		IEventStateInit( EventManager* pMGR ) : EventManager::IEventState(pMGR) {}

	virtual	BOOL	IEventStateUpdate( CurrentTimeInfo* pTimeInfo );

private:	EventManager*	m_pMGR;
};

//������������������������������������������������������������������������������������������������������������
//
class IEventStateIdle : public EventManager::IEventState
{
private:	enum eTIMER_MODE {
				MODE_IDLE				= 0,
				MODE_MANAGEMENT			= 1,
				MODE_EVENT_UPDATE		= 2,
				MODE_MAX				= 3,
			};
			enum eTIMER_INTERVAL {
				INTERVAL_SECOND			= 1000,
				INTERVAL_IDLE			=    5*INTERVAL_SECOND,
				INTERVAL_MANAGEMENT		=    5*INTERVAL_SECOND,
				INTERVAL_EVENT_UPDATE	=    5*INTERVAL_SECOND,
			};
public:		IEventStateIdle( EventManager* pMGR ) : EventManager::IEventState(pMGR)
			{
				m_Timer.SetTimer( this->INTERVAL_IDLE );
			}

	virtual	BOOL	IEventStateUpdate( CurrentTimeInfo* pTimeInfo );

private:	eTIMER_MODE		m_Mode;
			ITimerBase		m_Timer;
};


//������������������������������������������������������������������������������������������������������������
//
class IEventStateManagement : public EventManager::IEventState
{
public:		IEventStateManagement( EventManager* pMGR ) : EventManager::IEventState(pMGR) {}

	virtual	BOOL	IEventStateUpdate( CurrentTimeInfo* pTimeInfo );
};

//������������������������������������������������������������������������������������������������������������
//
class IEventStateEventUpdate : public EventManager::IEventState
{
public:		IEventStateEventUpdate( EventManager* pMGR ) : EventManager::IEventState(pMGR) {}

	virtual	BOOL	IEventStateUpdate( CurrentTimeInfo* pTimeInfo );
};

//������������������������������������������������������������������������������������������������������������
//
//class IEventControlBase : public EventManager::IEventControl
//{
//public:		IEventControlBase( EventManager* pMGR );
//	virtual	BOOL		IEventManagement( CurrentTimeInfo* pTimeInfo );
//	virtual	BOOL		IEventUpdate( CurrentTimeInfo* pTimeInfo );
//
//private:	BOOL		_CheckNeedMoveToSubstractList( CurrentTimeInfo* pTimeInfo );
//			BOOL		_CheckNeedMoveToPeakTimeEventList( CurrentTimeInfo* pTimeInfo );
//			BOOL		_IsPeakTime( CurrentTimeInfo* pTimeInfo, EVENT_INFO& rINFO );
//
//private:	typedef pair<CODETYPE, EVENT_INFO>		EVENT_NODE;
//			typedef STLX_LIST<EVENT_NODE>			EVENT_LIST;
//			typedef EVENT_LIST::iterator			EVENT_LIST_IT;
//
//			EVENT_LIST			m_SubtractEvents;
//};

EventManager::EventControlManager*	EventManager::ms_pEventControlManager = NULL;


//������������������������������������������������������������������������������������������������������������
//	<IEventControlSunEventInfo>
//		������ �ð��� {�̺�Ʈ}, {����/����} ó���� ����Ѵ�.
class IEventControlSunEventInfo : public EventManager::IEventControl
{
public:		IEventControlSunEventInfo( EventManager* pMGR );
	virtual	BOOL				IEventManagement( CurrentTimeInfo* pTimeInfo );
	virtual	BOOL				IEventUpdate( CurrentTimeInfo* pTimeInfo );

public:		typedef std::pair<CODETYPE, EVENT_INFO>		NOCTRL_EVENT_NODE;
			typedef STLX_LIST<NOCTRL_EVENT_NODE>	NOCTRL_EVENT_LIST;
			typedef NOCTRL_EVENT_LIST::iterator		NOCTRL_EVENT_LIST_IT;
public:
	inline	NOCTRL_EVENT_LIST&	GetSubstractedList() { return m_SubtractEvents; }

private:	BOOL				_CheckNeedMoveToSubstractList( CurrentTimeInfo* pTimeInfo );
			BOOL				_CheckNeedMoveToEventList( CurrentTimeInfo* pTimeInfo );

private:	SunEventInfoParser*		m_pSunEventInfoParser;
			typedef EventManager::EventControlManager::EVENT_CONTROL_LIST_IT	EVENT_CONTROL_LIST_IT;
			EVENT_CONTROL_LIST_IT	m_InsertedIT;
			NOCTRL_EVENT_LIST		m_SubtractEvents;
};

//������������������������������������������������������������������������������������������������������������
//	<IEventControlDeleteItem>
//		Ư�� �̺�Ʈ ó�� - ��) Quest
class IEventControlDeleteItem : public EventManager::IEventControl
{
public:		IEventControlDeleteItem( EventManager* pMGR, IEventControlSunEventInfo* pSunEventInfoControl );
	virtual	BOOL		IEventManagement( CurrentTimeInfo* pTimeInfo );
	virtual	BOOL		IEventUpdate( CurrentTimeInfo* pTimeInfo );

private:	BOOL		_CheckNeedCopyToOpenedSpecialEvents( CurrentTimeInfo* pTimeInfo );
			BOOL		_CheckNeedCopyToClosedSpecialEvents( CurrentTimeInfo* pTimeInfo );

private:	SunEventInfoParser*			m_pSunEventInfoParser;
			IEventControlSunEventInfo*	m_pSunEventInfoControl;

private:	typedef EventManager::EventControlManager::EVENT_CONTROL_LIST_IT	EVENT_CONTROL_LIST_IT;
			EVENT_CONTROL_LIST_IT		m_InsertedIT;
			typedef std::pair<CODETYPE, EVENT_INFO>			EVENT_ON_EVENT_NODE;
			typedef STLX_LIST<EVENT_ON_EVENT_NODE>		EVENT_ON_EVENT_LIST;
			typedef EVENT_ON_EVENT_LIST::iterator		EVENT_ON_EVENT_LIST_IT;
			EVENT_ON_EVENT_LIST			m_OpenedSpecialEvents;
			EVENT_ON_EVENT_LIST			m_ClosedSpecialEvents;
};

//������������������������������������������������������������������������������������������������������������
//	<IEventControlTimeEvent>
//		PeakTime�� ���� Ư�� �̺�Ʈ ó��
class IEventControlTimeEvent : public EventManager::IEventControl
{
public:		IEventControlTimeEvent( EventManager* pMGR );
			virtual	BOOL		IEventManagement( CurrentTimeInfo* pTimeInfo );
			virtual	BOOL		IEventUpdate( CurrentTimeInfo* pTimeInfo );

public:		BOOLEAN				_FindEventIDbyNpcID( const MONSTERCODE npcID, EventQueryResult::BuffEventIDs* INOUT pResult );
private:	BOOL				_CheckOpenedBuffEvents( CurrentTimeInfo* pTimeInfo );

private:	SunEventInfoParser*			m_pSunEventInfoParser;
			IEventControlSunEventInfo*	m_pSunEventInfoControl;

private:	typedef EventManager::EventControlManager::EVENT_CONTROL_LIST_IT	EVENT_CONTROL_LIST_IT;
			typedef DWORD								SYNC_COUNT;

			typedef std::pair<CODETYPE, SYNC_COUNT>			TIME_EVENT_NODE;
			typedef STLX_MAP<CODETYPE, SYNC_COUNT>		TIME_EVENT_LIST;
			typedef TIME_EVENT_LIST::iterator			TIME_EVENT_LIST_IT;

			typedef std::pair<CODETYPE, MONSTERCODE>			TIME_EVENT_NPC_NODE;
			typedef STLX_VECTOR<TIME_EVENT_NPC_NODE>	TIME_EVENT_NPC_LIST;
			typedef TIME_EVENT_NPC_LIST::iterator		TIME_EVENT_NPC_LIST_IT;

private:	EVENT_CONTROL_LIST_IT		m_InsertedIT;
			TIME_EVENT_LIST				m_OpenedEvents;
			TIME_EVENT_NPC_LIST			m_NpcList;
};


//������������������������������������������������������������������������������������������������������������
//
EventManager::EventManager()
{
}

EventManager::~EventManager()
{
}

BOOL		EventManager::DoInit()
{
	//����������������������������������������������������������������������������������
	static IEventStateInit			sIEventStateInit( this );
	static IEventStateIdle			sIEventStateIdle( this );
	static IEventStateManagement	sIEventStateManagement( this );
	static IEventStateEventUpdate	sIEventStateEventUpdate( this );

	static EventManager::EventControlManager	sEventControlManager( this );

	//����������������������������������������������������������������������������������
	ms_EventStates[this->EMS_INIT]			= &sIEventStateInit;
	ms_EventStates[this->EMS_IDLE]			= &sIEventStateIdle;
	ms_EventStates[this->EMS_MANAGEMENT]	= &sIEventStateManagement;
	ms_EventStates[this->EMS_EVENT_UPDATE]	= &sIEventStateEventUpdate;
	m_pEventState = ms_EventStates[this->EMS_INIT];
	ms_pEventControlManager = &sEventControlManager;

	//����������������������������������������������������������������������������������	
	static IEventControlSunEventInfo	sIEventControlSunEventInfo( this );
	static IEventControlPeakTime		sIEventControlPeakTime( this );
	//static IEventControlDeleteItem		sIEventControlDeleteItem( this, &sIEventControlSunEventInfo );
	static IEventControlTimeEvent		sIEventControlTimeEvent( this );

	return TRUE;
}

BOOL		EventManager::DoProcess()
{
	CurrentTimeInfo timeInfo;
	timeInfo.CurTick = GetTickCount();
	SYSTEMTIME& rSYSTIME = timeInfo.CurSysTime;
	util::TimeSync::GetLocalTime( &rSYSTIME );
	timeInfo.CurWzTimeFormat = (rSYSTIME.wHour * 100) + rSYSTIME.wMinute;
	util::TimeSync::_time64( &timeInfo.CurTimeSpec );

	return m_pEventState->IEventStateUpdate( &timeInfo );
}

BOOL		EventManager::IsComplete()
{
	return FALSE;
}

BOOL		EventManager::DoRelease()
{
	return TRUE;
}

VOID		EventManager::OnMsg( sSIG_MSG* pSIG_MSG )
{
    __UNUSED(pSIG_MSG);
}

VOID		EventManager::_ChangeState( IEventState* pChangingState )
{
	m_pEventState = pChangingState;
}

//������������������������������������������������������������������������������������������������������������
//	<IEventStateInit>
//		ó�� ���� ���۽� ó���ؾ� �� �ϵ� �˻��ϰ� ó���Ѵ�.
BOOL	IEventStateInit::IEventStateUpdate( CurrentTimeInfo* pTimeInfo )
{
    __TOUCHED(pTimeInfo);
	//	(process)
	ChangeState( EventManager::EMS_IDLE );

	return TRUE;
}

//������������������������������������������������������������������������������������������������������������
//	<IEventStateIdle>
//		���� ������ �̺�Ʈ ���� �Ǵ� ������Ʈ�� �����ϵ��� �Ѵ�.
BOOL	IEventStateIdle::IEventStateUpdate( CurrentTimeInfo* pTimeInfo )
{
    __TOUCHED(pTimeInfo);
	if( m_Timer.IsExpired() )
	{
		switch( m_Mode )
		{
		case this->MODE_IDLE:
			{
				ChangeState( EventManager::EMS_IDLE );
				m_Timer.SetInterval( this->INTERVAL_IDLE );
			}
			break;
		case this->MODE_MANAGEMENT:
			{
				ChangeState( EventManager::EMS_MANAGEMENT );
				m_Timer.SetInterval( this->INTERVAL_MANAGEMENT );
			}
			break;
		case this->MODE_EVENT_UPDATE:
			{
				ChangeState( EventManager::EMS_EVENT_UPDATE );
				m_Timer.SetInterval( this->INTERVAL_EVENT_UPDATE );
			}
			break;
		}
		m_Mode = (eTIMER_MODE)( ((INT)m_Mode + 1)%this->MODE_MAX );
	}

	return TRUE;
}

//������������������������������������������������������������������������������������������������������������
//	<IEventStateManagement>
//		���� �̺�Ʈ ó�� �� ����� �� ó���ؾ� �� ���� ���� ó��
BOOL	IEventStateManagement::IEventStateUpdate( CurrentTimeInfo* pTimeInfo )
{
	//	(process)
	GetEventManager()->GetEventController()->IEventManagement( pTimeInfo );
	ChangeState( EventManager::EMS_IDLE );
	return TRUE;
}

//������������������������������������������������������������������������������������������������������������
//	<IEventStateEventUpdate>
//		
BOOL	IEventStateEventUpdate::IEventStateUpdate( CurrentTimeInfo* pTimeInfo )
{
	//	(process)
	GetEventManager()->GetEventController()->IEventUpdate( pTimeInfo );
	ChangeState( EventManager::EMS_IDLE );
	return TRUE;
}

//������������������������������������������������������������������������������������������������������������
//	<IEventStateEventUpdate>
//		
BOOL	EventManager::EventControlManager::IEventManagement( CurrentTimeInfo* pTimeInfo )
{
	EVENT_CONTROL_LIST_IT it = m_List.begin();
	EVENT_CONTROL_LIST_IT itend = m_List.end();

	while( it != itend )
	{
		IEventControl* pIEventControl = *it++;
		pIEventControl->IEventManagement( pTimeInfo );
	}

	return TRUE;
}

BOOL	EventManager::EventControlManager::IEventUpdate( CurrentTimeInfo* pTimeInfo )
{
	EVENT_CONTROL_LIST_IT it = m_List.begin();
	EVENT_CONTROL_LIST_IT itend = m_List.end();

	while( it != itend )
	{
		IEventControl* pIEventControl = *it++;
		pIEventControl->IEventUpdate( pTimeInfo );
	}

	return TRUE;
}


//������������������������������������������������������������������������������������������������������������
//	<IEventControlSunEventInfo>
//		������ �ð��� {�̺�Ʈ}, {����/����} ó���� ����Ѵ�.
IEventControlSunEventInfo::IEventControlSunEventInfo( EventManager* pMGR ) : EventManager::IEventControl(pMGR)
{
	m_pSunEventInfoParser = SunEventInfoParser::Instance();
	m_InsertedIT = GetEventManager()->GetEventController()->RegisterEventController( this );
}

BOOL	IEventControlSunEventInfo::_CheckNeedMoveToSubstractList( CurrentTimeInfo* pTimeInfo )
{
    // SunEventInfoMap --> SubtractEvents : �̺�Ʈ ��Ȱ��ȭ
    __TOUCHED(pTimeInfo);
	typedef STLX_VECTOR<EVENTINFO_MAP_ITER>		_DELETE_WAITING_LIST;
	_DELETE_WAITING_LIST deleteList;

	EVENTINFO_MAP& rMap = *m_pSunEventInfoParser->GetEventInfoMap();
	EVENTINFO_MAP_ITER itend = rMap.end();
	EVENTINFO_MAP_ITER it = rMap.begin();
	while( it != itend )
	{
		EVENT_INFO& eventInfo = it->second;

		// �ش� PeakTimeEvent�� �Ⱓ�� �ƴϴ�. = FALSE
		if( eventInfo.CheckEvent() )
		{
			++it;
			continue;
		}
		else
		{
			m_SubtractEvents.insert( m_SubtractEvents.end(), std::make_pair(it->first, eventInfo) );
			// add-in chain events for closing
			deleteList.push_back(it++);
		}
	}

	if( !deleteList.empty() )
	{
		_DELETE_WAITING_LIST::iterator itdel = deleteList.begin();
		_DELETE_WAITING_LIST::iterator itdelend = deleteList.end();
		while( itdel != itdelend )
			rMap.erase( *itdel++ );
	}

	return TRUE;
}

BOOL	IEventControlSunEventInfo::_CheckNeedMoveToEventList( CurrentTimeInfo* pTimeInfo )
{
    // SubtractEvents --> SunEventInfoMap : �̺�Ʈ Ȱ��ȭ
    __TOUCHED(pTimeInfo);
	EVENTINFO_MAP& rMap = *m_pSunEventInfoParser->GetEventInfoMap();
	NOCTRL_EVENT_LIST_IT itend = m_SubtractEvents.end();
	NOCTRL_EVENT_LIST_IT it = m_SubtractEvents.begin();
	while( it!=itend )
	{
		NOCTRL_EVENT_NODE& rNODE = *it;
		EVENT_INFO& eventInfo = rNODE.second;

		if( FALSE == eventInfo.CheckEvent() )
			++it;
		else
		{
			rMap.insert( std::make_pair( rNODE.first, eventInfo ) );
			// add-in chain events for opening
			it = m_SubtractEvents.erase( it );
		}
	}

	return TRUE;
}

BOOL	IEventControlSunEventInfo::IEventManagement( CurrentTimeInfo* pTimeInfo )
{
	static DWORD PROCMODE = 0;

	( (PROCMODE = (PROCMODE+1)%2) == 0 ) ?
		_CheckNeedMoveToEventList( pTimeInfo )
		: _CheckNeedMoveToSubstractList( pTimeInfo );

	return TRUE;
}

BOOL	IEventControlSunEventInfo::IEventUpdate( CurrentTimeInfo* pTimeInfo )
{
    __TOUCHED(pTimeInfo);
#ifdef _NA_007992_20150127_EVENT_MONSTER_INVASION
    MonsterInvasionEvent::Instance()->EventUpdate(pTimeInfo->CurWzTimeFormat);
#endif //_NA_007992_20150127_EVENT_MONSTER_INVASION
	return TRUE;
}

//������������������������������������������������������������������������������������������������������������
//	<IEventControlPeakTime>
//		������ �ð��� {�̺�Ʈ}, {����/����} ó���� ����Ѵ�.
IEventControlPeakTime::IEventControlPeakTime( EventManager* pMGR ) : EventManager::IEventControl(pMGR)
{
	m_pPeakTimeEvent = PeakTimeEvent::Instance();
	m_InsertedIT = GetEventManager()->GetEventController()->RegisterEventController( this );

#ifdef _NA_007919_20141211_ADD_GMCOMMAND_EVENT
    m_pPeakTimeEvent->SetEventControlClass(this);
#endif //_NA_007919_20141211_ADD_GMCOMMAND_EVENT
}

BOOL	IEventControlPeakTime::_IsPeakTime( CurrentTimeInfo* pTimeInfo, EVENT_INFO& rINFO)
{
	// �Ϸ��� ��ũŸ�� �ð� üũ
	const DWORD curTotalMinute = pTimeInfo->CurWzTimeFormat;

	const DWORD startTime = rINFO.m_wPeakTimeStart;
	const DWORD endTime = rINFO.m_wPeakTimeEnd;

    return ( startTime <= curTotalMinute && curTotalMinute <= endTime ) ?
        TRUE
        : FALSE;
}

BOOL	IEventControlPeakTime::_CheckNeedMoveToSubstractList( CurrentTimeInfo* pTimeInfo )
{
    // PeakTimeEvent --> SubtractEvent ����ó�� : �̺�Ʈ ��Ȱ��ȭ
	EVENTINFO_MAP& rMap = m_pPeakTimeEvent->m_mapPeakTimeEvent;
	EVENTINFO_MAP_ITER itend = rMap.end();
	for( EVENTINFO_MAP_ITER it=rMap.begin() ; it!=itend ; )
	{
        EVENT_INFO& eventInfo = it->second;
#ifdef _NA_007919_20141211_ADD_GMCOMMAND_EVENT
        if( FALSE == eventInfo.CheckEvent() || FALSE == _IsPeakTime( pTimeInfo, eventInfo ) )
        {
            _SendEventNotify(ePEAKTIME_END, pTimeInfo, eventInfo);
            MoveEventToSubractList(it);
            continue;
        }
#else
        if( FALSE == eventInfo.CheckEvent() )
        {
            m_SubtractPeakTimeEvents.insert( m_SubtractPeakTimeEvents.end(), std::make_pair(it->first, eventInfo) );
            rMap.erase( it++ );
            continue;
        }

        if( FALSE == _IsPeakTime( pTimeInfo, eventInfo ) )
        {
            m_SubtractPeakTimeEvents.insert( m_SubtractPeakTimeEvents.end(), std::make_pair(it->first, eventInfo) );
            rMap.erase( it++ );
            continue;
        }
#endif //_NA_007919_20141211_ADD_GMCOMMAND_EVENT

		++it;
	}

	return TRUE;
}

BOOL	IEventControlPeakTime::_CheckNeedMoveToPeakTimeEventList( CurrentTimeInfo* pTimeInfo )
{
    // SubtractEvent --> PeakTimeEvent ����ó�� : �̺�ƮȰ��ȭ
	EVENTINFO_MAP& rMap = m_pPeakTimeEvent->m_mapPeakTimeEvent;
	PEAK_EVENT_LIST_IT itend = m_SubtractPeakTimeEvents.end();
	for( PEAK_EVENT_LIST_IT it=m_SubtractPeakTimeEvents.begin() ; it!=itend ; )
	{
		PEAK_EVENT_NODE& rNODE = *it;
		EVENT_INFO& eventInfo = rNODE.second;

        //CheckEvent��� ��¥�� üũ�ϴ� �Լ��� �ϳ� ��������
        _CheckPeakTimeEventStartTime(pTimeInfo, eventInfo);
		if( (TRUE == eventInfo.CheckEvent()) && (TRUE == _IsPeakTime( pTimeInfo, eventInfo )) )
		{
#ifdef _NA_007919_20141211_ADD_GMCOMMAND_EVENT
            //�̺�Ʈ �˸�
            _SendEventNotify(ePEAKTIME_START, pTimeInfo, eventInfo);

            //�̺�Ʈ �̵���Ű��
            m_pPeakTimeEvent->DeleteExistOverlapEvent(eventInfo);
#endif //_NA_007919_20141211_ADD_GMCOMMAND_EVENT
			rMap.insert( std::make_pair( rNODE.first, eventInfo ) );
			m_SubtractPeakTimeEvents.erase( it++ );
		}
		else
			++it;
	}

	return TRUE;
}



BOOL	IEventControlPeakTime::_CalculateFixExp( CurrentTimeInfo* pTimeInfo, CALC_EXP_VALUES& rValues )
{
    __TOUCHED(pTimeInfo);
	rValues.Init();

	typedef PeakTimeEvent::LEVELED_EXP_ARRAY	LEVELED_EXP_ARRAY;
	typedef PeakTimeEvent::sFIXEXP_VALUEs		sFIXEXP_VALUEs;

	EVENTINFO_MAP& rMap = m_pPeakTimeEvent->m_mapPeakTimeEvent;
	EVENTINFO_MAP_ITER itend = rMap.end();
	for( EVENTINFO_MAP_ITER it=rMap.begin() ; it!=itend ; ++it )
	{
		EVENT_INFO& eventInfo = it->second;

		// PeakTimeEvent�� �� �������� ������ �����ϴ� �̺�Ʈ�鸸 �����Ѵ�.

		// PC�� ��ũŸ�� �̺�Ʈ��� PC�� �������� üũ�Ѵ�.
		LEVELED_EXP_ARRAY& rExpLevels = ( eEVENT_PCBANG_PEAKTIME == eventInfo.m_byEventMode ) ?
			rValues[PeakTimeEvent::EXPUSR_ONLYPCROOM] : rValues[PeakTimeEvent::EXPUSR_NORMAL];

		const DWORD minLevel = eventInfo.m_byApplyToLvRng[EVENT_INFO::LEVEL_MIN];
		const DWORD maxLevel = eventInfo.m_byApplyToLvRng[EVENT_INFO::LEVEL_MAX];

		const FLOAT defaultEXP = eventInfo.m_fRewardExpRate;
		//// �ű�ĳ���̰� 30�����̸� �߰� ����ġ
		//rCurExps.PeakTimeEvent::EXPVAL_NEWCHAR_UNDER30LEVEL] += eventInfo.m_fRewardExpRate2;
#ifdef __NA_0_20061109_EXP_EVENT_
		// ���� �÷��� �ð��� ���� �߰� ����ġ
		const FLOAT actime60x2 = eventInfo.m_fPlayTimeExpRate[0];
		const FLOAT actime60x4 = eventInfo.m_fPlayTimeExpRate[1];
		const FLOAT actime60x6 = eventInfo.m_fPlayTimeExpRate[2];
#endif
		sFIXEXP_VALUEs* pStart = &rExpLevels[minLevel], *pEnd=&rExpLevels[maxLevel+1];
		for( ; pStart != pEnd ; ++pStart )
		{
			pStart->EXPVAL_USERTYPE += defaultEXP;
#ifdef __NA_0_20061109_EXP_EVENT_
			pStart->EXPVAL_ACCUMULATED_TIME_60x2 += actime60x2;
			pStart->EXPVAL_ACCUMULATED_TIME_60x4 += actime60x4;
			pStart->EXPVAL_ACCUMULATED_TIME_60x6 += actime60x6;
#endif
		}
	}

	const DWORD maxLevelInGame = Generic::GetMaxLevel( NULL );
	LEVELED_EXP_ARRAY& rSrcLevels = rValues[PeakTimeEvent::EXPUSR_NORMAL];
	LEVELED_EXP_ARRAY& rDestLevels = rValues[PeakTimeEvent::EXPUSR_ONLYPCROOM];

	sFIXEXP_VALUEs* pStart = &rDestLevels[1], *pEnd=&rDestLevels[maxLevelInGame+1];
	sFIXEXP_VALUEs* pSrcStart = &rSrcLevels[1];
	for( ; pStart != pEnd ; ++pStart, ++pSrcStart )
	{
		pStart->EXPVAL_USERTYPE += pSrcStart->EXPVAL_USERTYPE;
		pStart->EXPVAL_ACCUMULATED_TIME_60x2 += pSrcStart->EXPVAL_ACCUMULATED_TIME_60x2;
		pStart->EXPVAL_ACCUMULATED_TIME_60x4 += pSrcStart->EXPVAL_ACCUMULATED_TIME_60x4;
		pStart->EXPVAL_ACCUMULATED_TIME_60x6 += pSrcStart->EXPVAL_ACCUMULATED_TIME_60x6;
	}

	return TRUE;
}

BOOL	IEventControlPeakTime::IEventManagement( CurrentTimeInfo* pTimeInfo )
{
	static DWORD PROCMODE = 0;

	( (PROCMODE = (PROCMODE+1)%2) == 0 ) ?
		_CheckNeedMoveToPeakTimeEventList( pTimeInfo )
		: _CheckNeedMoveToSubstractList( pTimeInfo );

	return TRUE;
}

BOOL	IEventControlPeakTime::IEventUpdate( CurrentTimeInfo* pTimeInfo )
{
	_CalculateFixExp( pTimeInfo, m_pPeakTimeEvent->m_ExpInfo );
	return TRUE;
}

#ifdef _NA_007919_20141211_ADD_GMCOMMAND_EVENT
bool IEventControlPeakTime::MoveEventToSubractList( EVENTINFO_MAP_ITER& event_iter )
{
    EVENTINFO_MAP& event_map = m_pPeakTimeEvent->m_mapPeakTimeEvent;
    EVENT_INFO& eventInfo = event_iter->second;
    
    if (eventInfo.is_gm_event_ == true)    // GM�� ���� �̺�Ʈ
    {
        //�ð��ߺ��˻�
        PEAK_EVENT_LIST_IT itend = m_SubtractPeakTimeEvents.end();
        for( PEAK_EVENT_LIST_IT it=m_SubtractPeakTimeEvents.begin() ; it!=itend ; )
        {
            PEAK_EVENT_NODE& rNODE = *it;
            EVENT_INFO& sub_event_info = rNODE.second;

            //�ߺ��ȵǴ� ���
            if( eventInfo.m_start_time > sub_event_info.m_end_time &&
                eventInfo.m_end_time < sub_event_info.m_start_time ) {
                    ++it;
            }
            //�ߺ��Ǵ� ���
            else {
                it = m_SubtractPeakTimeEvents.erase(it++);
            }
        }
    }

    m_SubtractPeakTimeEvents.insert( m_SubtractPeakTimeEvents.end(), 
        std::make_pair(eventInfo.m_dwEventID, eventInfo) );
    event_map.erase( event_iter++ );
    return true;
}

void IEventControlPeakTime::_CheckPeakTimeEventStartTime(CurrentTimeInfo* pTimeInfo, EVENT_INFO& rINFO)
{
    const DWORD curTotalMinute = pTimeInfo->CurWzTimeFormat;

    tm* event_start_date = _localtime64(&rINFO.m_start_time);
    const DWORD startTime = (event_start_date->tm_hour * 100) + event_start_date->tm_min;

    tm* end_date = _localtime64(&rINFO.m_end_time);
    const DWORD endTime = (end_date->tm_hour * 100) + end_date->tm_min;

    const SYSTEMTIME cur_systime = pTimeInfo->CurSysTime;

    //��/��/�� �˻�, �Ϸ�� ���� ������ false
    if ((event_start_date->tm_year+1900) != cur_systime.wYear ||
        (event_start_date->tm_mon+1) != cur_systime.wMonth ||
        event_start_date->tm_mday != cur_systime.wDay) 
    {
        return;
    }

    // ��ũŸ�� �̺�Ʈ���þ˸�
    if (rINFO.m_byEventMode == eEVENT_PEAKTIME)
    {
        // �ð�+���� �д����� ��ȯ
        const DWORD start_time_minute = (startTime / 100) * 60 + (startTime % 100);
        const DWORD curTotalMinute_minute = (curTotalMinute / 100) * 60 + (curTotalMinute % 100);

        // ��ũŸ�� �̺�Ʈ 5���� �˸�(Ȱ��ȭ �˸�)
        if (rINFO.is_pre_notify_ == false)
        {
            //�����ϱ� 1~5����
            if ((start_time_minute - curTotalMinute_minute) <= 5 &&
                (start_time_minute - curTotalMinute_minute) > 0) 
            {
                rINFO.is_pre_notify_ = true;

                m_pPeakTimeEvent->SendEventNotifyPacket(ePEAKTIME_PRENOTIFY, rINFO.m_fRewardExpRate, 
                                                        event_start_date->tm_mday, startTime, end_date->tm_mday, endTime);
            }
        }
    }
}

VOID IEventControlPeakTime::_SendEventNotify( ePEAKTIME_EVENT_NOTIFY event_state, CurrentTimeInfo* pTimeInfo, 
                                              EVENT_INFO& rINFO )
{
    if (rINFO.m_byEventMode != eEVENT_PEAKTIME) {
        return;
    }

    tm* end_date = _localtime64(&rINFO.m_end_time);
    tm* start_date = _localtime64(&rINFO.m_start_time);

    const DWORD start_time = (start_date->tm_hour * 100) + start_date->tm_min;
    const DWORD end_time = (end_date->tm_hour * 100) + end_date->tm_min;

    if (event_state == ePEAKTIME_START)
    {
        m_pPeakTimeEvent->SendEventNotifyPacket(ePEAKTIME_START, rINFO.m_fRewardExpRate, 
            start_date->tm_mday, start_time, end_date->tm_mday, end_time);
    }
    else if (event_state == ePEAKTIME_END)
    {
        const DWORD end_time_minute = (end_time / 100) * 60 + (end_time % 100);

        const DWORD curTotalMinute = pTimeInfo->CurWzTimeFormat;
        const DWORD curTotalMinute_minute = (curTotalMinute / 100) * 60 + (curTotalMinute % 100);

        if (end_time_minute <= curTotalMinute_minute) {
            m_pPeakTimeEvent->SendEventNotifyPacket(ePEAKTIME_END, rINFO.m_fRewardExpRate);
        }
    }
}

#endif //_NA_007919_20141211_ADD_GMCOMMAND_EVENT

//������������������������������������������������������������������������������������������������������������
//	<IEventControlDeleteItem>
//		�ϵ� �ڵ��� �� ���� �ִ� Ư���� ��Ȳ�� ���� ó���� �����Ѵ�.
//		�̰� ����ϴ� ù ��ʷδ� Ư�� �̺�Ʈ_������ �̺�Ʈ_�� ���� �̺�Ʈ �Ⱓ�� ������ �ش� �������� �����ؾ�
//		�ϴ� ��츦 �� �� �ִ�.
//		�ѹ��� ��� ������ ��� �κ��丮 �����ۿ� ���ؼ� �ϰ������� �����Ѵ�... �̰� 
IEventControlDeleteItem::IEventControlDeleteItem( EventManager* pMGR, IEventControlSunEventInfo* pISunEventInfo )
	: EventManager::IEventControl(pMGR), m_pSunEventInfoControl( pISunEventInfo )
{
	m_pSunEventInfoParser = SunEventInfoParser::Instance();
	m_InsertedIT = GetEventManager()->GetEventController()->RegisterEventController( this );
}

BOOL	IEventControlDeleteItem::_CheckNeedCopyToOpenedSpecialEvents( CurrentTimeInfo* pTimeInfo )
{
    __TOUCHED(pTimeInfo);
	return TRUE;
}

BOOL	IEventControlDeleteItem::_CheckNeedCopyToClosedSpecialEvents( CurrentTimeInfo* pTimeInfo )
{
    __TOUCHED(pTimeInfo);
	return TRUE;
}

BOOL	IEventControlDeleteItem::IEventManagement( CurrentTimeInfo* pTimeInfo )
{
    __TOUCHED(pTimeInfo);
	return TRUE;
}

BOOL	IEventControlDeleteItem::IEventUpdate( CurrentTimeInfo* pTimeInfo )
{
    __TOUCHED(pTimeInfo);
	return TRUE;
}



//������������������������������������������������������������������������������������������������������������
//	<IEventControlDeleteItem>
//		�ϵ� �ڵ��� �� ���� �ִ� Ư���� ��Ȳ�� ���� ó���� �����Ѵ�.
//		�̰� ����ϴ� ù ��ʷδ� Ư�� �̺�Ʈ_������ �̺�Ʈ_�� ���� �̺�Ʈ �Ⱓ�� ������ �ش� �������� �����ؾ�
//		�ϴ� ��츦 �� �� �ִ�.
//		�ѹ��� ��� ������ ��� �κ��丮 �����ۿ� ���ؼ� �ϰ������� �����Ѵ�... �̰� 
IEventControlTimeEvent::IEventControlTimeEvent( EventManager* pMGR )
	: EventManager::IEventControl(pMGR)
{
	m_pSunEventInfoParser = SunEventInfoParser::Instance();
	EventManager::EventControlManager& rECMgr = *GetEventManager()->GetEventController();
	m_InsertedIT = rECMgr.RegisterEventController( this );
	rECMgr._RegisterTimeEventHandler( m_InsertedIT );
}

BOOLEAN		IEventControlTimeEvent::_FindEventIDbyNpcID( const MONSTERCODE npcID, EventQueryResult::BuffEventIDs* INOUT pResult )
{
	pResult->IDofNPC = npcID;
	pResult->Count;

	typedef EventQueryResult::BuffEventIDs	BuffEventIDs;
    BuffEventIDs::ARRAY::array_type& rList = pResult->List;
    ZeroMemory(&rList, sizeof(rList));
	//std::fill_n( &rList[0], (SIZE_T)BuffEventIDs::MAX_EVENT_ID_LENGTH, 0 );

	DWORD count = 0;
	FOREACH_CONTAINER( TIME_EVENT_NPC_LIST::value_type& rNode, m_NpcList, TIME_EVENT_NPC_LIST )
	{
		if( BuffEventIDs::MAX_EVENT_ID_LENGTH <= count )
			break;

		const MONSTERCODE stored_npcid = rNode.second;
		if( stored_npcid == npcID )
			rList[count++] = rNode.first;
	}

	pResult->Count = (BYTE)count;
	return !!count;
}

BOOL	IEventControlTimeEvent::_CheckOpenedBuffEvents( CurrentTimeInfo* pTimeInfo )
{
	static DWORD synchronize_value = 0;
	++synchronize_value;

	m_NpcList.clear();

	EVENTINFO_MAP& rMap = *m_pSunEventInfoParser->GetEventInfoMap();
	{
		EVENTINFO_MAP_ITER it = rMap.begin(), itend = rMap.end();
		for( ; it != itend ; ++it )
		{
			EVENT_INFO& eventInfo = it->second;

			const eEVENT_MODE eventMode = (eEVENT_MODE)eventInfo.m_byEventMode;
			if( eEVENT_BUFF_GRANT != eventMode )
				continue;
			const CODETYPE eventID = eventInfo.m_dwEventID;

			const DWORD startTime = eventInfo.m_wPeakTimeStart;
			const DWORD endTime = eventInfo.m_wPeakTimeEnd;

			const BOOL inTime = startTime <= pTimeInfo->CurWzTimeFormat && pTimeInfo->CurWzTimeFormat <= endTime;
			TIME_EVENT_LIST_IT itFind = m_OpenedEvents.find( eventID );
			if( itFind == m_OpenedEvents.end() )
			{
				if( inTime )
				{
					m_OpenedEvents.insert( std::make_pair( eventID, synchronize_value ) );
					m_NpcList.push_back( std::make_pair( eventID, (MONSTERCODE)eventInfo.m_NpcCode ) );

					MSG_AG_EVENT_BUFF_EVENT_TIME_CONTROL_SYN msgSYN;
					msgSYN.SetInfo( eventID, msgSYN.TC_START );
					g_pGameServer->SendToServer	( AGENT_SERVER, &msgSYN, sizeof(msgSYN) );
				}
			}
			else
			{
				itFind->second = synchronize_value;
				if( !inTime )
				{
					m_OpenedEvents.erase( eventID );

					MSG_AG_EVENT_BUFF_EVENT_TIME_CONTROL_SYN msgSYN;
					msgSYN.SetInfo( eventID, msgSYN.TC_END );
					g_pGameServer->SendToServer	( AGENT_SERVER, &msgSYN, sizeof(msgSYN) );
				}
				else
				{
					m_NpcList.push_back( std::make_pair( eventID, (MONSTERCODE)eventInfo.m_NpcCode ) );
				}
			}
		}
	};

	// �� �˻����� �����Ǵ� ��찡 �ִ�. �̹� �̺�Ʈ ��Ͽ��� ���ŵ� ���,
	{
		TIME_EVENT_LIST_IT it=m_OpenedEvents.begin(), itend=m_OpenedEvents.end();
		while( it!=itend )
		{
			const DWORD stored_syncvalue = it->second;
			if( stored_syncvalue == synchronize_value )
			{
				++it;
				continue;
			}

			const CODETYPE eventID = it->first;
			it = m_OpenedEvents.erase(it);

			MSG_AG_EVENT_BUFF_EVENT_TIME_CONTROL_SYN msgSYN;
			msgSYN.SetInfo( eventID, msgSYN.TC_END );
			g_pGameServer->SendToServer	( AGENT_SERVER, &msgSYN, sizeof(msgSYN) );
		}
	};

	return TRUE;
}

BOOL	IEventControlTimeEvent::IEventManagement( CurrentTimeInfo* pTimeInfo )
{
	return TRUE;
}

BOOL	IEventControlTimeEvent::IEventUpdate( CurrentTimeInfo* pTimeInfo )
{
	EventManager::EventControlManager& rECMgr = *GetEventManager()->GetEventController();
	if( rECMgr.IsFieldServer() )
		_CheckOpenedBuffEvents( pTimeInfo );

	return TRUE;
}


//������������������������������������������������������������������������������������������������������������
//	<EventManager::EventControlManager>
//	-	Query Mechanism


VOID		EventManager::EventControlManager::Query( QueryEvent* INOUT pQuery )
{
	QueryEvent::eQUERY queryType = pQuery->m_Query;
	switch( queryType )
	{
	case QueryEvent::Q_GET_BUFF_EVENTID_BY_NPCID:
		{
			QueryEventGetBuffEventIDbyNpcID* pSpQuery = static_cast<QueryEventGetBuffEventIDbyNpcID*>(pQuery);
			if( g_pGameServer->GetServerType() == FIELD_SERVER )
			{
				IEventControlTimeEvent* pTimeEvent = static_cast<IEventControlTimeEvent*>(*m_TimeEventIT);
				const BOOLEAN success = pTimeEvent->_FindEventIDbyNpcID( pSpQuery->m_Arg_NpcCode, pSpQuery->m_EventID_Info );
				pSpQuery->m_bResult = success;
			}
			else
			{
				pSpQuery->m_bResult = FALSE;
			}
		}
		break;
	case QueryEvent::Q_EMPTY:
	default:
		pQuery->m_bResult = FALSE;
		break;
	}
}


BOOLEAN		EventManager::GetBuffEventIDbyNpcID( const MONSTERCODE npcID, EventQueryResult::BuffEventIDs* INOUT pResult )
{
	QueryEventGetBuffEventIDbyNpcID query( npcID, pResult );
	GetEventController()->Query( &query );
	return (query.m_bResult != false);
}




