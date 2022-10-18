#pragma once

#include <ServerSession.h>

class RemoteServerSession : public ServerSession
#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	, public ISession4StartServiceWorkNode
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
{
public:
	RemoteServerSession();
	~RemoteServerSession();

protected:
	// Network Object
    // CHANGES: f110617.1L
    virtual void OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg) = 0;
	virtual VOID		OnDisconnect();
	virtual VOID		OnLogString( CHAR* pszLog );

protected:
	virtual VOID		Init();

	

public:
	virtual VOID		Release();

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
public:
	virtual BOOL		IsValidObject( DWORD dwCheckValue );
	virtual BOOL		IsValidState( DWORD dwStatusValue );
	virtual BOOL		SendPacket4Interface( MSG_BASE* pMsg, WORD wSize );
#endif //

	// 이것은 WorkLoad에서 해당 세션에 대한 유효성을 판별하기 위해서 사용한다.
	inline VOID			SetSyncKey( DWORD dwSyncValue ) { m_dwSyncKey = dwSyncValue; }
	inline DWORD		GetSyncKey() { return m_dwSyncKey; }

private:
	DWORD								m_dwSyncKey;
public:
	WORK_LOAD_CONTAINER_SESSION_WORK&	GetWorkLoadContainer() { return m_WorkLoad; }
	WorkLoadPacketStream&				GetWorkLoadPacketStream() { return m_WorkLoadPacketStream; }
private:
	WorkLoadPacketStream				m_WorkLoadPacketStream;
	WORK_LOAD_CONTAINER_SESSION_WORK	m_WorkLoad;
};


