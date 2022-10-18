#pragma once

#include <TempServerSession.h>
#include "ServerSession.h"
#include "Services4Session.h"

class MasterServerSession;

class TempServerSessionEx : public TempServerSession
{
public:
	TempServerSessionEx();
	~TempServerSessionEx();

	virtual VOID				Init();

	// Copy (Temp*) to (Master*)
	// Redirect, SessionIndex
	// Services Members,
	// CryptInfos
	VOID						MigrateObject( MasterServerSession* pTargetMaster );
public:
	inline DWORD&				AccessCount() { return m_dwAccessCount; }
	inline Services4Session&	Services() { return m_Services; }

protected:
    // CHANGES: f110617.1L
    virtual void OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg);
	virtual VOID				OnAccept( DWORD dwNetworkIndex );
	virtual VOID				OnDisconnect();
	virtual	VOID				OnRecv( BYTE* pMsg, WORD wSize );
public:
	virtual BOOL				SendPacket( MSG_BASE* pMsg, WORD wSize );

public:
	virtual VOID				Release();

private:
	DWORD					m_dwPacketStrictSequence;
	DWORD					m_dwAccessCount;
	Services4Session		m_Services;
};

inline void TempServerSessionEx::OnRedirect(DWORD, MSG_SERVER_TYPE* const)
{
    assert(!"this session can't use this method");
};
