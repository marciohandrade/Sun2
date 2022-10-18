#pragma once
//=============================================================================================================================
/// TriggerLastedState class
/**
*/
//=============================================================================================================================

#include "..\\..\\SolarTrigger\\TriggerCommon.h"

// <트리거 최종 상태 정리>

//struct sTRIGGER_ID_CONTAINER
//{
//	INT			m_dwTriggerID;
//
//	sTRIGGER_ID_CONTAINER() : m_dwTriggerID(0), m_dwObjectID(0), m_dwValue(0) {}
//	~sTRIGGER_ID_CONTAINER() {}
//
//	inline INT&				TRIGGER_ID() { return m_dwTriggerID; }
//};

class TriggerLastedState
{
	typedef STLX_VECTOR< DWORD >						OBJECT_STATES;
public:		TriggerLastedState();
			~TriggerLastedState();

	inline	VOID				Init() { Clear(); }
			VOID				Clear();
			VOID				Change( DWORD dwTriggerID );
			BOOL				SendInfo( Player* pPlayer );

public:
	static	TriggerLastedState*	Create();
	static	VOID				Destroy( TriggerLastedState* const ptr );

private:	OBJECT_STATES		m_LatestList;
};

//////////////////////////////////////////////////////////////////////////
// <트리거 최종 상태 정리><대리자>
class TriggerLastedStateDelegator
{
	TriggerLastedState*			m_pInfo;
public:
	TriggerLastedStateDelegator() : m_pInfo(NULL) {}
	TriggerLastedStateDelegator( const TriggerLastedState* pInfo ) : m_pInfo((TriggerLastedState*)pInfo) {}
	TriggerLastedStateDelegator( const TriggerLastedStateDelegator& pInfoDele ) : m_pInfo(pInfoDele.m_pInfo) {}
	~TriggerLastedStateDelegator() { m_pInfo = NULL; }

	inline VOID					Init() { if( m_pInfo ) m_pInfo->Init(); }
	inline VOID					Clear() { if( m_pInfo ) m_pInfo->Clear(); }
	inline VOID					Change( DWORD dwTriggerID )
	{ if( m_pInfo ) m_pInfo->Change( dwTriggerID ); }
	inline BOOL					SendInfo( Player* pPlayer )
	{ if( m_pInfo ) return m_pInfo->SendInfo( pPlayer ); return FALSE; }
	inline TriggerLastedState*&	REFERENCE() { return m_pInfo; }
};

