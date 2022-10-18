#ifndef __SERVER_SESSION_EX_H__
#define __SERVER_SESSION_EX_H__

#include <PacketStruct.h>
#include <ServerSession.h>

#include "Services4Session.h"

class ServerSessionEx : public ServerSession
{
public:
	ServerSessionEx()		{};
	~ServerSessionEx()		{};

public:
	virtual eSERVER_TYPE		GetServerType() = 0;
	inline Services4Session&	Services() { return m_Services; }

protected:
	//
	//-------------------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------------------------------
	// Init & Release
	VOID						_Initialize();
	VOID						_Release();

public:
	//-------------------------------------------------------------------------------------------------
	// NetworkObject
    // CHANGES: f110617.1L
    virtual void OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg) = 0;
	virtual VOID				OnDisconnect();
protected:
    virtual void OnRecv(BYTE* pMsg, WORD wSize);

public:
	// <INTERFACEs>
	virtual VOID				Init() { _Initialize(); };
	virtual VOID				Release() { _Release(); };

public:

	inline SERVER_KEY			GetServerKey() { return m_ServerKey; }
	inline VOID					SetServerKey( SERVER_KEY serverKey ) { m_ServerKey = serverKey; }

private:
	SERVER_KEY					m_ServerKey;
	Services4Session			m_Services;
};

#endif //__SERVER_SESSION_EX_H__


