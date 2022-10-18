#pragma once

#include "PeakTimeEvent.h"

struct EventQueryResult
{
	struct BuffEventIDs;
};

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<EventManager>
//		이벤트 관련 처리 관리자.
//		PeakTime, CheckEvent와 같이 할 때마다 모든 이벤트에 대해 루프를 돌면서 시간 계산을 수행하는 경우나
//		기타 특수 이벤트에 대한 검사를 구간별로 수행할 수 있도록 한다.
//	<Purpose>
//		무의미한 연산 부하 감소
//		정리되지 않은 이벤트 처리 개선
//		등등...
class EventManager : public IWorkNode
{
public:		enum eEVENT_MGR_STATE
			{
				EMS_INIT				= 0,
				EMS_IDLE				,
				EMS_MANAGEMENT			,
				EMS_EVENT_UPDATE		,
				EMS_MAX_EVENT_STATES	,
			};

			typedef __time64_t	TIME_SPEC;

			struct CurrentTimeInfo
			{
				DWORD			CurTick;
				DWORD			CurWzTimeFormat;	// hour*100 + minute
				TIME_SPEC		CurTimeSpec;
				SYSTEMTIME		CurSysTime;
			};

			//─────────────────────────────────────────
			//
			class IEventControl
			{
			public:		IEventControl( EventManager* pMGR ) : m_pMGR(pMGR) {}
				virtual	~IEventControl() {}
				// 각각은 시간별 {이벤트}, {삽입/삭제} 처리를 담당한다.
				virtual	BOOL	IEventManagement( CurrentTimeInfo* pTimeInfo ) = 0;
				virtual	BOOL	IEventUpdate( CurrentTimeInfo* pTimeInfo ) = 0;
			protected:
				inline	EventManager*	GetEventManager() { return m_pMGR; }
			private:	EventManager*	m_pMGR;
			};

			//─────────────────────────────────────────
			//
public:		class IEventState
			{
			public:		IEventState( EventManager* pMGR ) : m_pMGR(pMGR) {}
				virtual	~IEventState() {}
						VOID	ChangeState( eEVENT_MGR_STATE state );
				virtual BOOL	IEventStateUpdate( CurrentTimeInfo* pTimeInfo ) = 0;
			protected:
				inline	EventManager*	GetEventManager() { return m_pMGR; }
			private:	EventManager*	m_pMGR;
			};
			friend class IEventState;

			class EventControlManager;

public:		EventManager();
			~EventManager();
			//─────────────────────────────────────────
			//	<Specialization><Methods>
public:		BOOLEAN					GetBuffEventIDbyNpcID( const MONSTERCODE npcID, EventQueryResult::BuffEventIDs* INOUT pResult );
			//─────────────────────────────────────────
			//
public:
	inline static WORK_KEY			GetWorkKey() { return ms_WorkKey; }
	inline static VOID				SetWorkKey( WORK_KEY wKey ) { ms_WorkKey = wKey; }
	inline static EventManager*		GetEventManager() { return ms_pEventManager; }
	inline static EventControlManager*	GetEventController() { return ms_pEventControlManager; }
public:	
	//─────────────────────────────────────────
	//
	virtual	BOOL					DoInit();
	virtual	BOOL					DoProcess();
	virtual BOOL					IsComplete();
	virtual	BOOL					DoRelease();
	virtual	VOID					OnMsg( sSIG_MSG* pSIG_MSG );

	//─────────────────────────────────────────
	//
private:	VOID					_ChangeState( IEventState* pChangingState );

private:	VOID					_UpdateEventControl( IEventControl* pIEventC );

//	이벤트 관련 모든 정보를 관리하며 처리할 수 있도록 하자.
//	시간 검사를 위해 SunEventInfoParser를 직접적으로 활용하는 형식의 지원은 피하자.
private:
	static const DWORD				UPDATE_INTERVAL =  100;
private:	IEventState*			m_pEventState;
			ITimerBase				m_UpdateTimer;
	static	WORK_KEY				ms_WorkKey;
	static	EventManager*			ms_pEventManager;
	static	EventControlManager*	ms_pEventControlManager;
	static	IEventState*			ms_EventStates[EMS_MAX_EVENT_STATES];
};
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
typedef EventManager::CurrentTimeInfo	CurrentTimeInfo;

struct EventQueryResult::BuffEventIDs
{
	enum { MAX_EVENT_ID_LENGTH = 10, };
	MONSTERCODE			IDofNPC;
	BYTE				Count;
	typedef ::util::ARRAY_TYPEINFO< CODETYPE, 0, MAX_EVENT_ID_LENGTH >	ARRAY;
	ARRAY::array_type	List;
};

struct QueryEvent
{
    enum eQUERY {
        Q_EMPTY							= 0,
        Q_GET_BUFF_EVENTID_BY_NPCID		= 10,
    };
    const eQUERY		m_Query;
    BOOL				m_bResult;

    QueryEvent( const eQUERY query ) : m_Query(query), m_bResult(FALSE) {}
};

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
class EventManager::EventControlManager : public EventManager::IEventControl
{
public:		EventControlManager( EventManager* pMGR ) : EventManager::IEventControl(pMGR), m_bFieldServer(g_pGameServer->GetServerType()==FIELD_SERVER) {}
            ~EventControlManager() {}

            virtual	BOOL		IEventManagement( CurrentTimeInfo* pTimeInfo );
            virtual	BOOL		IEventUpdate( CurrentTimeInfo* pTimeInfo );

public:
    typedef STLX_LIST<EventManager::IEventControl*>		EVENT_CONTROL_LIST;
    typedef EVENT_CONTROL_LIST::iterator				EVENT_CONTROL_LIST_IT;
    inline	EVENT_CONTROL_LIST_IT	RegisterEventController( EventManager::IEventControl* pIEventControl )
    {	return m_List.insert( m_List.end(), pIEventControl );	}
    inline	VOID					DeRegisterEventController( EVENT_CONTROL_LIST_IT insertedIT )
    {	m_List.erase( insertedIT );		}
    VOID					Query( QueryEvent* INOUT pQuery );
    inline	VOID					_RegisterTimeEventHandler( EVENT_CONTROL_LIST_IT timeEventIt ) { m_TimeEventIT = timeEventIt; }
    inline	BOOL					IsFieldServer() { return m_bFieldServer; }

private:	EVENT_CONTROL_LIST		m_List;
            BOOL					m_bFieldServer;
            EVENT_CONTROL_LIST_IT	m_TimeEventIT;
};

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<IEventControlPeakTime>
//		각각은 시간별 {이벤트}, {삽입/삭제} 처리를 담당한다.
class IEventControlPeakTime : public EventManager::IEventControl
{
public:	typedef EventManager::EventControlManager::EVENT_CONTROL_LIST_IT	EVENT_CONTROL_LIST_IT;
        EVENT_CONTROL_LIST_IT	m_InsertedIT;
        typedef std::pair<CODETYPE, EVENT_INFO>		PEAK_EVENT_NODE;
        typedef STLX_LIST<PEAK_EVENT_NODE>		PEAK_EVENT_LIST;
        typedef PEAK_EVENT_LIST::iterator		PEAK_EVENT_LIST_IT;

public:		IEventControlPeakTime( EventManager* pMGR );
            virtual	BOOL		IEventManagement( CurrentTimeInfo* pTimeInfo );
            virtual	BOOL		IEventUpdate( CurrentTimeInfo* pTimeInfo );
#ifdef _NA_007919_20141211_ADD_GMCOMMAND_EVENT
            PEAK_EVENT_LIST*    GetSubtractEventList() {return &m_SubtractPeakTimeEvents; }
#endif //_NA_007919_20141211_ADD_GMCOMMAND_EVENT

private:	BOOL		_CheckNeedMoveToSubstractList( CurrentTimeInfo* pTimeInfo );
            BOOL		_CheckNeedMoveToPeakTimeEventList( CurrentTimeInfo* pTimeInfo );	
            typedef PeakTimeEvent::CalcedExp			CALC_EXP_VALUES;
            BOOL		_CalculateFixExp( CurrentTimeInfo* pTimeInfo, CALC_EXP_VALUES& rValues );

            BOOL		_IsPeakTime( CurrentTimeInfo* pTimeInfo, EVENT_INFO& rINFO );
#ifdef _NA_007919_20141211_ADD_GMCOMMAND_EVENT 
            // PeakTimeEvent맵에서 SubtractList로 이벤트정보를 이동시킨다.
            // gm이 생성한 이벤트일 경우, 중복체크를 한다
            bool        MoveEventToSubractList(EVENTINFO_MAP_ITER& event_iter); 
            // 이벤트시작 5분전에 알리기위한 시간검사함수
            // 현재 시간검사방식상 5분전 알림을 하려면 별도로 검사를 해주어야 함
            void        _CheckPeakTimeEventStartTime(CurrentTimeInfo* pTimeInfo, EVENT_INFO& rINFO); 

            VOID        _SendEventNotify(ePEAKTIME_EVENT_NOTIFY event_state, CurrentTimeInfo* pTimeInfo, 
                                         EVENT_INFO& rINFO);
#endif //_NA_007919_20141211_ADD_GMCOMMAND_EVENT

private:	PeakTimeEvent*		m_pPeakTimeEvent;
private:
    PEAK_EVENT_LIST		m_SubtractPeakTimeEvents;       
    // 아직 시간이 되지 않은 이벤트를 관리하는 리스트
    // 시간이 된 이벤트들은 다시 PeakTimeEvent의 리스트로 이동시킨다
};

