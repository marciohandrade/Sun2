#ifndef __SERVER_SESSION_H__
#define __SERVER_SESSION_H__

#pragma once

#include <NetworkObject.h>
#include <PacketStruct_CG.h>

#include <ServerSession.h>

#include "Services4Session.h"

class ServerSessionEx : public ServerSession
{
public:
	ServerSessionEx();
	~ServerSessionEx();

	//-------------------------------------------------------------------------------------------------
	// NetworkObject
	virtual VOID				OnDisconnect();
	virtual VOID				OnLogString( char* pszLog );
	//
	//-------------------------------------------------------------------------------------------------
public:
	virtual eSERVER_TYPE		GetServerType() = 0;
	Services4Session&			Services() { return m_Services; }

protected:
    // CHANGES: f110617.1L
    virtual void OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg) = 0;
	virtual	VOID				OnRecv( BYTE* pMsg, WORD wSize );
	//
	//-------------------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------------------------------
	// Init & Release
	VOID						_Initialize();
	VOID						_Release();

public:
	// <INTERFACEs>
	virtual VOID				Init() { _Initialize(); };
	virtual VOID				Release() { _Release(); };

	// 변수의 초기화 ( 외부에서 사용하는 변수들 포함 ) : Factory의 Alloc()시에 호출, init()와 용도가 다름! 주의!
	virtual VOID				Clear();

public:

private:
	Services4Session			m_Services;
};

#endif // __SERVER_SESSION_H__

