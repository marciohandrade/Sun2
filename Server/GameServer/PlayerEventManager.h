#pragma once

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<PROTOTYPEs>
//
class Player;

typedef STLX_MAP<DWORD, EVENT_STATUS_EX>		EVENT_STATUS_MAP;
typedef EVENT_STATUS_MAP::iterator				EVENT_STATUS_MAP_ITER;
typedef std::pair<DWORD, EVENT_STATUS_EX>		EVENT_STATUS_MAP_PAIR;


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<PROTOTYPEs>
//	(071120) 쓸모 있으면 공용으로 옮기자.
class sTransaction
{
public:		sTransaction() : m_Trans(FALSE) {}
			__forceinline VOID		Enter() { m_Trans = TRUE; }
			__forceinline VOID		Leave() { m_Trans = FALSE; }
			__forceinline BOOL		IsTransaction() { return m_Trans; }
private:	BYTE	m_Trans;
};


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<IPlayerEventD>
//
class IPlayerEventD
{
public:		IPlayerEventD() : m_pImpl(NULL) {}  
			~IPlayerEventD();

			VOID					Init();
			VOID					Release();

			// 이벤트 관련
			WORD					GetEventNum();
			VOID					AddEventStatus( const EVENT_STATUS_EX& rEventStatus );
			EVENT_STATUS_EX*		FindEventStatus( const DWORD eventID );
			sTransaction&			EventTransaction() { return m_Transaction; }

			// for Client Data Serializing
			BOOL					GetEventStatus( const DWORD size, EVENT_STATUS* OUT pEventStatus );
			BOOL					GetEventStatus( const DWORD size, EVENT_STATUS_EX* OUT pEventStatus );

private:	class PlayerEventManager;
			sTransaction			m_Transaction;
			PlayerEventManager*		m_pImpl;
};


