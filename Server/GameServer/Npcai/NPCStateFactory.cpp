#include "StdAfx.h"
#include "NPCStateFactory.h"
#include "WanderState.h"
#include "PatrolState.h"
#include "IdleState.h"
#include "TrackState.h"
#include "AttackState.h"
#include "DeadState.h"
#include "HelpState.h"
#include "ThrustState.h"
#include "FlyingState.h"
#include "KnockDownState.h"
#include "JumpState.h"
#include "FallApartState.h"
#include "ReturnState.h"
#include "RetreatState.h"
#include "RunAwayState.h"
#include "ChaosState.h"
#include "SummonIdle.h"
#include "SpawnIdleState.h"
#include "StopIdleState.h"
#include "TrackAreaState.h"
#include "BlindState.h"
#include "PathListState.h"
#include "FollowState.h"
#include "NoTransState.h"
#include "DestroyObjectState.h"
#include "SpecialDeadActionState.h"

NPCStateFactory::NPCStateFactory()
{
	//m_Router.Register<>					(STATE_ID_UNKNOWN		);
	m_Router.Register<WanderState>			(STATE_ID_WANDER		);
	m_Router.Register<IdleState>			(STATE_ID_IDLE			);
	m_Router.Register<TrackState>			(STATE_ID_TRACK			);
	m_Router.Register<AttackState>			(STATE_ID_ATTACK		);
	m_Router.Register<HelpState>			(STATE_ID_HELP			);
	m_Router.Register<ThrustState>			(STATE_ID_THRUST		);
	m_Router.Register<DeadState>			(STATE_ID_DEAD			);
	m_Router.Register<FlyingState>			(STATE_ID_FLYING		);
	m_Router.Register<KnockDownState>		(STATE_ID_KNOCKDOWN		);
	m_Router.Register<JumpState>			(STATE_ID_JUMP			);
	m_Router.Register<FallApartState>		(STATE_ID_FALL_APART	);
	m_Router.Register<ReturnState>			(STATE_ID_RETURN		);
	m_Router.Register<RetreatState>			(STATE_ID_RETREAT		);
	m_Router.Register<RunAwayState>			(STATE_ID_RUNAWAY		);
	m_Router.Register<ChaosState>			(STATE_ID_CHAOS			);
	m_Router.Register<SummonIdle>			(STATE_ID_SUMMON_IDLE	);
	m_Router.Register<PatrolState>			(STATE_ID_PATROL		);
	m_Router.Register<SpawnIdleState>		(STATE_ID_SPAWN_IDLE	);
	m_Router.Register<StopIdleState>		(STATE_ID_STOP_IDLE		);
	m_Router.Register<TrackAreaState>		(STATE_ID_TRACK_AREA	);
	m_Router.Register<BlindState>			(STATE_ID_BLIND			);
    m_Router.Register<PathListState>		(STATE_ID_PATHLIST      );
    m_Router.Register<FollowState>		    (STATE_ID_FOLLOW        );
    //_NA001385_20090924_DOMINATION_MAPNPC_AI_BUG
    m_Router.Register<NoTransState>		    (STATE_ID_NO_TRANS      );
    m_Router.Register<DestroyObjectState>	(STATE_ID_DESTROY_OBJECT);
    m_Router.Register<SpecialDeadActionState> (STATE_ID_SPECIAL_DEAD_ACTION);

	m_Router.Initialize(STATE_ID_WANDER			, 200, 200, "STATE_ID_WANDER"		, AssertionLogger4Pool_Server );
	m_Router.Initialize(STATE_ID_IDLE			, 100, 100, "STATE_ID_IDLE"			, AssertionLogger4Pool_Server );
	m_Router.Initialize(STATE_ID_TRACK			, 100, 100, "STATE_ID_TRACK"		, AssertionLogger4Pool_Server );
	m_Router.Initialize(STATE_ID_ATTACK			, 100, 100, "STATE_ID_ATTACK"		, AssertionLogger4Pool_Server );
	m_Router.Initialize(STATE_ID_HELP			, 100, 100, "STATE_ID_HELP"			, AssertionLogger4Pool_Server );
	m_Router.Initialize(STATE_ID_THRUST			, 100, 100, "STATE_ID_THRUST"		, AssertionLogger4Pool_Server );
	m_Router.Initialize(STATE_ID_DEAD			, 100, 100, "STATE_ID_DEAD"			, AssertionLogger4Pool_Server );
	m_Router.Initialize(STATE_ID_FLYING			, 100, 100, "STATE_ID_FLYING"		, AssertionLogger4Pool_Server );
	m_Router.Initialize(STATE_ID_KNOCKDOWN		, 100, 100, "STATE_ID_KNOCKDOWN"	, AssertionLogger4Pool_Server );
	m_Router.Initialize(STATE_ID_JUMP			, 100, 100, "STATE_ID_JUMP"			, AssertionLogger4Pool_Server );
	m_Router.Initialize(STATE_ID_FALL_APART		, 100, 100, "STATE_ID_FALL_APART"	, AssertionLogger4Pool_Server );
	m_Router.Initialize(STATE_ID_RETURN			, 100, 100, "STATE_ID_RETURN"		, AssertionLogger4Pool_Server );
	m_Router.Initialize(STATE_ID_RETREAT		, 100, 100, "STATE_ID_RETREAT"		, AssertionLogger4Pool_Server );
	m_Router.Initialize(STATE_ID_RUNAWAY		, 100, 100, "STATE_ID_RUNAWAY"		, AssertionLogger4Pool_Server );
	m_Router.Initialize(STATE_ID_CHAOS			, 100, 100, "STATE_ID_CHAOS"		, AssertionLogger4Pool_Server );
	m_Router.Initialize(STATE_ID_SUMMON_IDLE	, 100, 100, "STATE_ID_SUMMON_IDLE"	, AssertionLogger4Pool_Server );
	m_Router.Initialize(STATE_ID_PATROL			, 100, 100, "STATE_ID_PATROL"		, AssertionLogger4Pool_Server );
	m_Router.Initialize(STATE_ID_SPAWN_IDLE		, 100, 100, "STATE_ID_SPAWN_IDLE"	, AssertionLogger4Pool_Server );
	m_Router.Initialize(STATE_ID_STOP_IDLE		, 100, 100, "STATE_ID_STOP_IDLE"	, AssertionLogger4Pool_Server );
	m_Router.Initialize(STATE_ID_TRACK_AREA		, 100, 100, "STATE_ID_TRACK_AREA"	, AssertionLogger4Pool_Server );
	m_Router.Initialize(STATE_ID_BLIND			, 100, 100, "STATE_ID_BLIND"		, AssertionLogger4Pool_Server );
    m_Router.Initialize(STATE_ID_PATHLIST       , 100, 100, "STATE_ID_PATHLIST"		, AssertionLogger4Pool_Server );
    m_Router.Initialize(STATE_ID_FOLLOW         , 100, 100, "STATE_ID_FOLLOW"		, AssertionLogger4Pool_Server );
    m_Router.Initialize(STATE_ID_NO_TRANS       , 100, 100, "STATE_ID_NO_TRANS"		, AssertionLogger4Pool_Server );
    m_Router.Initialize(STATE_ID_DESTROY_OBJECT , 100, 100, "STATE_ID_DESTROY_OBJECT", AssertionLogger4Pool_Server );
    m_Router.Initialize(STATE_ID_SPECIAL_DEAD_ACTION , 100, 100, 
                        "STATE_ID_SPECIAL_DEAD_ACTION", AssertionLogger4Pool_Server );
}

NPCStateFactory::~NPCStateFactory()
{
	m_Router.Release();
}

State*		NPCStateFactory::Alloc( DWORD dwStateID )
{
	ENUM_STATD_ID eIDX = dwStateID && (STATE_ID_UPPERBOUND > dwStateID) ?
		(ENUM_STATD_ID)dwStateID : STATE_ID_WANDER;

	State* pState = (State*)m_Router.Alloc( eIDX );

	return pState;
}

VOID		NPCStateFactory::Free( State* pState )
{
	ENUM_STATD_ID eIDX = pState->GetLinkStateID();
	if( eIDX && (STATE_ID_UPPERBOUND > eIDX) )
	{
		m_Router.Free( eIDX, pState );
	}
	else
	{
#ifdef _DEBUG
		__debugbreak();
#endif
		SUNLOG( eCRITICAL_LOG, _T("Unexpected State Pointer %u 0x%p\n"), eIDX, pState);
	}
}

