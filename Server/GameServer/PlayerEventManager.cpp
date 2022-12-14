#include "stdafx.h"
#include "PlayerEventManager.h"

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	<SHARED INFO>
//

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	<PlayerEventManager>
//
class IPlayerEventD::PlayerEventManager
{
public:		PlayerEventManager() {}

			// Status Operation
private:	WORD						GetEventNum();
			VOID						AddEventStatus( const EVENT_STATUS_EX& rEventStatus );
			EVENT_STATUS_EX*			FindEventStatus( const DWORD eventID );

			// for Client Data Serializing
			BOOL						GetEventStatus( const DWORD size, EVENT_STATUS* OUT pEventStatus );
			BOOL						GetEventStatus( const DWORD size, EVENT_STATUS_EX* OUT pEventStatus );

			VOID						Clear();
private:
	friend class IPlayerEventD;
	typedef util::CMemoryPoolFactory<PlayerEventManager>	MEMORY_POOL;
			// Control Operation
private:	static BOOL					_InitPool();
			static PlayerEventManager*	_AllocPool();
			static BOOL					_FreePool( PlayerEventManager* ptr );

private:	static MEMORY_POOL			ms_Pool;
//public:		PROPERTY_PTR<Player*>		PLAYER;
private:	EVENT_STATUS_MAP			m_EventMap;
};

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	<PlayerEventManager>
//		<Static><Implementation>
IPlayerEventD::PlayerEventManager::MEMORY_POOL
IPlayerEventD::PlayerEventManager::ms_Pool;

BOOL
IPlayerEventD::PlayerEventManager::_InitPool()
{
	ms_Pool.Initialize( 10, 100, "PlayerEventManager", NULL );
	return TRUE;
}

IPlayerEventD::PlayerEventManager*
IPlayerEventD::PlayerEventManager::_AllocPool()
{
	return (PlayerEventManager*)ms_Pool.Alloc();
}

BOOL
IPlayerEventD::PlayerEventManager::_FreePool( PlayerEventManager* ptr )
{
	return ms_Pool.Free( ptr );
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	<PlayerEventManager>
//		<Implementation>

// Status Operation
WORD
IPlayerEventD::PlayerEventManager::GetEventNum()
{
	return (WORD)m_EventMap.size();
}

VOID
IPlayerEventD::PlayerEventManager::AddEventStatus( const EVENT_STATUS_EX& rEventStatus )
{
	const DWORD eventID = rEventStatus.m_dwEventID;
	m_EventMap[eventID] = rEventStatus;
}

EVENT_STATUS_EX*
IPlayerEventD::PlayerEventManager::FindEventStatus( const DWORD eventID )
{
	EVENT_STATUS_MAP_ITER itFIND = m_EventMap.find( eventID );
	return itFIND != m_EventMap.end() ? &itFIND->second : NULL;
}

// for Client Data Serializing
BOOL
IPlayerEventD::PlayerEventManager::GetEventStatus( const DWORD size, EVENT_STATUS* OUT pEventStatus )
{
	EVENT_STATUS* pOutput = pEventStatus;
	EVENT_STATUS_MAP_ITER it=m_EventMap.begin(), itend = m_EventMap.end();

	DWORD count = size>_MAX_EVENT_ROW_NUM ? _MAX_EVENT_ROW_NUM : size ;
	for( ; count && it!=itend ; ++it, --count )
	{
		EVENT_STATUS_EX& rSrc = it->second;
		EVENT_STATUS& rDest = *pOutput++;
		rDest.m_dwEventID = rSrc.m_dwEventID;
		rDest.m_wRegisterCnt = rSrc.m_wRegisterCnt;
	}
	return TRUE;
}

BOOL
IPlayerEventD::PlayerEventManager::GetEventStatus( const DWORD size, EVENT_STATUS_EX* OUT pEventStatus )
{
	EVENT_STATUS* pOutput = pEventStatus;
	EVENT_STATUS_MAP_ITER it=m_EventMap.begin(), itend = m_EventMap.end();

	DWORD count = size>_MAX_EVENT_ROW_NUM ? _MAX_EVENT_ROW_NUM : size ;
	for( ; count && it!=itend ; ++it, --count )
		*pOutput++ = it->second;

	return TRUE;
}

VOID
IPlayerEventD::PlayerEventManager::Clear()
{
	//PLAYER.Set( NULL );
	m_EventMap.clear();
}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	<IPlayerEventD>
//
IPlayerEventD::~IPlayerEventD()
{
	Release();
}

VOID				IPlayerEventD::Init()
{
	static BOOL sbInit = PlayerEventManager::_InitPool();

	if( !m_pImpl )
		m_pImpl = PlayerEventManager::_AllocPool();
	m_pImpl->Clear();
	m_Transaction.Leave();
}

VOID				IPlayerEventD::Release()
{
	if( !m_pImpl )
		return;

	m_pImpl->Clear();
	PlayerEventManager::_FreePool( m_pImpl );
	m_pImpl = NULL;
	m_Transaction.Leave();
}

//	<BUG>	Irony - 翕晦?? 僥薯 : m_pImpl擎 瞰渠 NULL檜 腎憮朝 寰腎雖虜,
//	-	Release陛 脹 檜?? PlayerManager縑朝 player陛 襄營ж朝 鼻?窒□?
//	-	DB煎睡攪曖 雖翱脹 唸婁陛 螢 唳辦 僥薯陛 嫦儅и棻.
//	<FIX>
//	-	if( !m_pImpl )	return; 擊 陝 л熱葆棻 蹺陛ж晦煎 и棻. (no Define)
//	-	⑷ 衛薄 僥薯朝 AddEventStatus縑憮虜 嫦儅й 熱 氈雖虜 檜?? 棻艇 僥薯紫 撿晦腆 陛棟撩紫 氈戲嘎煎 賅舒 蕨諼籀葬и棻.
//
WORD				IPlayerEventD::GetEventNum()
{
	if( !m_pImpl )
		return 0;

	PlayerEventManager& rPMgr = *m_pImpl;
	return rPMgr.GetEventNum();
}

VOID				IPlayerEventD::AddEventStatus( const EVENT_STATUS_EX& rEventStatus )
{
	if( !m_pImpl )
		return;

	PlayerEventManager& rPMgr = *m_pImpl;
	return rPMgr.AddEventStatus( rEventStatus );
}

EVENT_STATUS_EX*	IPlayerEventD::FindEventStatus( const DWORD eventID )
{
	if( !m_pImpl )
		return NULL;

	PlayerEventManager& rPMgr = *m_pImpl;
	return rPMgr.FindEventStatus( eventID );
}

// for Client Data Serializing
BOOL				IPlayerEventD::GetEventStatus( const DWORD size, EVENT_STATUS* OUT pEventStatus )
{
	if( !m_pImpl )
		return FALSE;

	PlayerEventManager& rPMgr = *m_pImpl;
	return rPMgr.GetEventStatus( size, pEventStatus );
}

BOOL				IPlayerEventD::GetEventStatus( const DWORD size, EVENT_STATUS_EX* OUT pEventStatus )
{
	if( !m_pImpl )
		return FALSE;

	PlayerEventManager& rPMgr = *m_pImpl;
	return rPMgr.GetEventStatus( size, pEventStatus );
}



