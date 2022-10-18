#pragma once

#include <ServerSession.h>
#include <string>
#include <Timer.h>

struct MSG_BASE;
class ServerSessionEx	: public ServerSession
	, public ISession4StartServiceWorkNode
{
public:
	ServerSessionEx();
	~ServerSessionEx();	

protected:
	virtual VOID		Init();
	virtual VOID		OnDisconnect();
public:
	virtual VOID		Release();

public:
	virtual BOOL		IsValidObject( DWORD dwCheckValue );
	virtual BOOL		IsValidState( DWORD dwStatusValue );
	virtual BOOL		SendPacket4Interface( MSG_BASE* pMsg, WORD wSize );

	// �̰��� WorkLoad���� �ش� ���ǿ� ���� ��ȿ���� �Ǻ��ϱ� ���ؼ� ����Ѵ�.
	inline VOID			SetSyncKey( DWORD dwSyncValue ) { m_dwSyncKey = dwSyncValue; }
	inline DWORD		GetSyncKey() { return m_dwSyncKey; }

private:
	DWORD						m_dwSyncKey;
public:
	WORK_LOAD_CONTAINER_SESSION_WORK&	GetWorkLoadContainer() { return m_WorkLoad; }
	WorkLoadPacketStream&		GetWorkLoadPacketStream() { return m_WorkLoadPacketStream; }
private:
	WorkLoadPacketStream				m_WorkLoadPacketStream;
	WORK_LOAD_CONTAINER_SESSION_WORK	m_WorkLoad;
};

