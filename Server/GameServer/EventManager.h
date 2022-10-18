#pragma once

#include "PeakTimeEvent.h"

struct EventQueryResult
{
	struct BuffEventIDs;
};

//������������������������������������������������������������������������������������������������������������
//	<EventManager>
//		�̺�Ʈ ���� ó�� ������.
//		PeakTime, CheckEvent�� ���� �� ������ ��� �̺�Ʈ�� ���� ������ ���鼭 �ð� ����� �����ϴ� ��쳪
//		��Ÿ Ư�� �̺�Ʈ�� ���� �˻縦 �������� ������ �� �ֵ��� �Ѵ�.
//	<Purpose>
//		���ǹ��� ���� ���� ����
//		�������� ���� �̺�Ʈ ó�� ����
//		���...
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

			//����������������������������������������������������������������������������������
			//
			class IEventControl
			{
			public:		IEventControl( EventManager* pMGR ) : m_pMGR(pMGR) {}
				virtual	~IEventControl() {}
				// ������ �ð��� {�̺�Ʈ}, {����/����} ó���� ����Ѵ�.
				virtual	BOOL	IEventManagement( CurrentTimeInfo* pTimeInfo ) = 0;
				virtual	BOOL	IEventUpdate( CurrentTimeInfo* pTimeInfo ) = 0;
			protected:
				inline	EventManager*	GetEventManager() { return m_pMGR; }
			private:	EventManager*	m_pMGR;
			};

			//����������������������������������������������������������������������������������
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
			//����������������������������������������������������������������������������������
			//	<Specialization><Methods>
public:		BOOLEAN					GetBuffEventIDbyNpcID( const MONSTERCODE npcID, EventQueryResult::BuffEventIDs* INOUT pResult );
			//����������������������������������������������������������������������������������
			//
public:
	inline static WORK_KEY			GetWorkKey() { return ms_WorkKey; }
	inline static VOID				SetWorkKey( WORK_KEY wKey ) { ms_WorkKey = wKey; }
	inline static EventManager*		GetEventManager() { return ms_pEventManager; }
	inline static EventControlManager*	GetEventController() { return ms_pEventControlManager; }
public:	
	//����������������������������������������������������������������������������������
	//
	virtual	BOOL					DoInit();
	virtual	BOOL					DoProcess();
	virtual BOOL					IsComplete();
	virtual	BOOL					DoRelease();
	virtual	VOID					OnMsg( sSIG_MSG* pSIG_MSG );

	//����������������������������������������������������������������������������������
	//
private:	VOID					_ChangeState( IEventState* pChangingState );

private:	VOID					_UpdateEventControl( IEventControl* pIEventC );

//	�̺�Ʈ ���� ��� ������ �����ϸ� ó���� �� �ֵ��� ����.
//	�ð� �˻縦 ���� SunEventInfoParser�� ���������� Ȱ���ϴ� ������ ������ ������.
private:
	static const DWORD				UPDATE_INTERVAL =  100;
private:	IEventState*			m_pEventState;
			ITimerBase				m_UpdateTimer;
	static	WORK_KEY				ms_WorkKey;
	static	EventManager*			ms_pEventManager;
	static	EventControlManager*	ms_pEventControlManager;
	static	IEventState*			ms_EventStates[EMS_MAX_EVENT_STATES];
};
//������������������������������������������������������������������������������������������������������������
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

//������������������������������������������������������������������������������������������������������������
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

//������������������������������������������������������������������������������������������������������������
//	<IEventControlPeakTime>
//		������ �ð��� {�̺�Ʈ}, {����/����} ó���� ����Ѵ�.
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
            // PeakTimeEvent�ʿ��� SubtractList�� �̺�Ʈ������ �̵���Ų��.
            // gm�� ������ �̺�Ʈ�� ���, �ߺ�üũ�� �Ѵ�
            bool        MoveEventToSubractList(EVENTINFO_MAP_ITER& event_iter); 
            // �̺�Ʈ���� 5������ �˸������� �ð��˻��Լ�
            // ���� �ð��˻��Ļ� 5���� �˸��� �Ϸ��� ������ �˻縦 ���־�� ��
            void        _CheckPeakTimeEventStartTime(CurrentTimeInfo* pTimeInfo, EVENT_INFO& rINFO); 

            VOID        _SendEventNotify(ePEAKTIME_EVENT_NOTIFY event_state, CurrentTimeInfo* pTimeInfo, 
                                         EVENT_INFO& rINFO);
#endif //_NA_007919_20141211_ADD_GMCOMMAND_EVENT

private:	PeakTimeEvent*		m_pPeakTimeEvent;
private:
    PEAK_EVENT_LIST		m_SubtractPeakTimeEvents;       
    // ���� �ð��� ���� ���� �̺�Ʈ�� �����ϴ� ����Ʈ
    // �ð��� �� �̺�Ʈ���� �ٽ� PeakTimeEvent�� ����Ʈ�� �̵���Ų��
};

