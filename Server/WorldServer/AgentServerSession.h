#ifndef _AGENTSERVERSESSION_H_
#define _AGENTSERVERSESSION_H_

#include "ServerSession.h"

class FunctionMap;

//=================================================================================================
/// Agent server에 connect하기 위한 세션 클래스
//			- Agent server로 패킷을 보내고 받는다.
//			- 받은 패킷 처리는 Handler_AM에서 처리한다.
//=================================================================================================
class AgentServerSession : public ServerSession
{
public:
	AgentServerSession();
	~AgentServerSession();

	virtual VOID				Init();
	virtual VOID				Release();
	virtual VOID				Update();
	virtual eSERVER_TYPE		GetServerType() { return AGENT_SERVER; }

	virtual VOID				OnRedirect();

	inline VOID					SetChannelID( DWORD channelID ) { m_dwChannelID = channelID; }
	inline DWORD				GetChannelID() { return m_dwChannelID; }

protected:
	virtual VOID				OnConnect( BOOL bSuccess, DWORD dwNetworkIndex );
	virtual VOID				OnDisconnect();
	virtual	VOID				OnRecv( BYTE *pMsg, WORD wSize );

private:
	VOID						OnSendServerType();
	VOID						OnSendReqServerInfo();

	DWORD						m_dwChannelID;
};

#endif //_AGENTSERVERSESSION_H_