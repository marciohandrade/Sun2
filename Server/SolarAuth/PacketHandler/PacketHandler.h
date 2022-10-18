#pragma once

#ifndef __PACKET_HANDLER_H
#define __PACKET_HANDLER_H


//=============================================================================================================================
/// PacketHandler class
/**
	@author	Lee Eun-pa <waverix@webzen.co.kr>
	@since	2006. 08. 15
	@remark
	- PacketHandler
	@note
	- 
	@history 
	-  
*/
//=============================================================================================================================

#include <FunctionMap.h>

struct MSG_BASE;
class ServerSession;
class PacketHandler;

class HandlerFromMasterServer;
class HandlerFromTempServer;

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
	// 약자정의 이후 변경될 수도 있다.
	// (S)olar auth server
	// t(E)mp server
	// (M)aster server
public:
	PacketHandler();
	~PacketHandler();

	static VOID				RegisterHandler();
	static VOID				ClosePacketHandler();

	//-------------------------------------------------------------------------------------
	// 패킷 파싱
	inline BOOL		CanDoProcess( FUNC_GENERIC_DELEGATOR& funcDelegator );
	BOOL			ParsePacket( eSERVER_TYPE serverType, ServerSession* pSession, MSG_BASE* pMsg, WORD wSize )
	{
		return _ParsePacket( serverType, pSession, pMsg, wSize );
	}

private:
	BOOL			_ParsePacket( eSERVER_TYPE serverType, ServerSession* pSession, MSG_BASE* pMsg, WORD wSize );
	// 또 있다면 현(060815) 설계상 Connection 계열일 것이다.
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
	static PACKET_HANDLER_DELEGATOR	ms_pInstance;

	BOOL						m_bLock;
	// <COMMON HANDLER CONTAINER>
	FUNCTION_MAP_DELEGATOR		m_FuncMaps[MAX_SERVER];
};

#endif //__PACKET_HANDLER_H

