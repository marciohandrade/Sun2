#pragma once

#include <ServerSession.h>
#include "ServerSessionCommon.h"

struct MSG_BASE;
class ServerSessionEx : public ServerSession, public ISession4StartServiceWorkNode
{
public:
	ServerSessionEx();
	~ServerSessionEx();	

protected:
    // CHANGES: f110617.1L
    virtual void OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg) = 0;
	virtual VOID				OnRecv( BYTE *pMsg, WORD wSize );
	virtual VOID				OnDisconnect();
	//virtual VOID				OnLogString( char *pszLog );

public:
	inline virtual eSERVER_TYPE	GetServerType()		{ return UNKNOWN_SERVER; }

	virtual VOID				Init();
	virtual VOID				Release();
	VOID						Update();	//< ��ƽ���� ȣ���� �Ǿ�� �ϴ� �κ�!!!!
	VOID						Clear();

	BOOL						SendPacket( MSG_BASE * pMsg, WORD wSize );
protected:
	VOID						SendHeartbeatPacket();
private:

public:
	virtual BOOL				IsValidObject( DWORD dwCheckValue );
	virtual BOOL				IsValidState( DWORD dwStatusValue );
	virtual BOOL				SendPacket4Interface( MSG_BASE* pMsg, WORD wSize );

	// �̰��� WorkLoad���� �ش� ���ǿ� ���� ��ȿ���� �Ǻ��ϱ� ���ؼ� ����Ѵ�.
	inline VOID					SetSyncKey( DWORD dwSyncValue ) { m_dwSyncKey = dwSyncValue; }
	inline DWORD				GetSyncKey() { return m_dwSyncKey; }

private:
	DWORD								m_dwSyncKey;
public:
	WORK_LOAD_CONTAINER_SESSION_WORK&	GetWorkLoadContainer() { return m_WorkLoad; }
	WorkLoadPacketStream&				GetWorkLoadPacketStream() { return m_WorkLoadPacketStream; }
private:
	WorkLoadPacketStream				m_WorkLoadPacketStream;
	WORK_LOAD_CONTAINER_SESSION_WORK	m_WorkLoad;
};

