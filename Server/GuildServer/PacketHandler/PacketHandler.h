#pragma once

#include <FunctionMap.h>

//
// PACKET STRUCTUREs
#include <PacketStruct_AZ.h>
#include <PacketStruct_DZ.h>
#include <PacketStruct_GZ.h>
#include <PacketStruct_MZ.h>
#include <PacketStruct_WZ.h>
//
#include <Sessions/AgentServerSession.h>
#include <Sessions/DBProxyServerSession.h>
#include <Sessions/GameServerSession.h>
#include <Sessions/MasterServerSession.h>
#include <Sessions/WorldServerSession.h>
//

struct MSG_BASE;
class ServerSession;
class PacketHandler;

//=======================================================================================================================
// TYPE DEFINITION
typedef VOID (*PH_fnHandler)( ServerSession* pSession, MSG_BASE* pMsg, WORD wSize );

struct sPACKET_HANDLER_INFO
{
	WORD			wCATEGORYnPROTOCOL;
	ePHASE_SHIFT	ePHASE_SHIFT_T;
	PH_fnHandler	pfnHandler;
};

struct FUNC_GENERIC : public util::BASE_FUNC
{
	ePHASE_SHIFT	m_PHASE_SHIFT_T;
	PH_fnHandler	m_fnHandler;
};

typedef Delegator<PacketHandler>					PACKET_HANDLER_DELEGATOR;
typedef Delegator<PacketHandler>*					PACKET_HANDLER_DELEGATOR_P;

typedef Delegator<util::FunctionMap>				FUNCTION_MAP_DELEGATOR;
typedef Delegator<FUNC_GENERIC>						FUNC_GENERIC_DELEGATOR;

//
class PacketHandler
{
public:
	PacketHandler();
	~PacketHandler();

	static VOID				RegisterHandler();
	static VOID				ReleaseHandler();

	static PACKET_HANDLER_DELEGATOR	GetHandler();

	//-------------------------------------------------------------------------------------
	// ��Ŷ �Ľ�
	inline BOOL		CanDoProcess( FUNC_GENERIC_DELEGATOR& funcDelegator );

	BOOL			ParsePacket( eSERVER_TYPE serverType, ServerSession* pSession, MSG_BASE* pMsg, WORD wSize )
	{
		return _ParsePacket( serverType, pSession, pMsg, wSize );
	}

	BOOL			ParsePacket( eSERVER_TYPE serverType, ServerSession* pSession, MSG_BASE_INTERNAL* pMsg, WORD wSize )
	{
		return _ParsePacket( serverType, pSession, pMsg, wSize );
	}

private:
	BOOL			_ParsePacket( eSERVER_TYPE serverType, ServerSession* pSession, MSG_BASE* pMsg, WORD wSize );
	BOOL			_ParsePacket( eSERVER_TYPE serverType, ServerSession* pSession, MSG_BASE_INTERNAL* pMsg, WORD wSize );
	// �� �ִٸ� ��(060815) ����� Connection �迭�� ���̴�.
	//-------------------------------------------------------------------------------------

public:
	class Phase
	{
	private:
		ePHASE_SHIFT	m_Phase;
	public:
		Phase() : m_Phase(PHASE_NOACTION) {}

		BOOL		Shift( ePHASE_SHIFT	phase );
		BOOL		RegisterHandler( sPACKET_HANDLER_INFO* pINFOs, WORD wCounts );
	};

public:
	template<eSERVER_TYPE serverType>
		BOOL			RegisterHandler_( sPACKET_HANDLER_INFO* pINFOs )
	{
		return _RegisterHandler( serverType, pINFOs );
	}

private:
	BOOL			_RegisterHandler( eSERVER_TYPE serverType, sPACKET_HANDLER_INFO* pINFOs );

private:
	VOID			_Initialize();

	//FUNC_HASH_DELEGATOR		GetHandler( ServerSessionEx* pSession );

private:
	VOID			_LockInit() { m_bLock = FALSE; }
	BOOL			_IsLock() { return m_bLock; }
	VOID			_Locking() { m_bLock = TRUE; }
	VOID			_UnLocking() { m_bLock = FALSE; }
private:
	//
	static PACKET_HANDLER_DELEGATOR		ms_pInstance;

	BOOL						m_bLock;
	// <COMMON HANDLER CONTAINER>
	FUNCTION_MAP_DELEGATOR		m_FuncMaps[MAX_SERVER];




	//TEMP
	BOOL Resister();
};

