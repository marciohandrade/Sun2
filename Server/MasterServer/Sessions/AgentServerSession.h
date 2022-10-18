#ifndef _AGENTSERVERSESSION_H_
#define _AGENTSERVERSESSION_H_

#include "ServerSessionEx.h"

struct MSG_AM_CONNECTION_CCU_REPORT_NTF;

//=================================================================================================
// Agent server에 connect하기 위한 세션 클래스
//=================================================================================================
class AgentServerSession : public ServerSessionEx
{
public:
			AgentServerSession() {};
			~AgentServerSession() {};

			virtual eSERVER_TYPE	GetServerType() { return AGENT_SERVER; }

public:
			virtual VOID			Init();

public:		VOID					CCUReport( MSG_AM_CONNECTION_CCU_REPORT_NTF* IN pINFO );
			DWORD					GetCCU() { return m_CCUofCHANNEL; }
protected:
    // CHANGES: f110617.1L
    virtual void OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg);
			DWORD					m_CCUofCHANNEL;
};

#endif //_AGENTSERVERSESSION_H_