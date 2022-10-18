#pragma once

/**************************************************************************************************
작성일: 2008-07-08
설명: 세션 리스트를 관리 하는 객체
**************************************************************************************************/

#include <WBANetwork/Net/SessionManager.h>
#include <WBANetwork/Net/ServerSession.h>

class CSessionMgr : public WBANetwork::SessionManager
{
public:
	CSessionMgr(void);
	~CSessionMgr(void);

	void BindClient(WBANetwork::ServerSession* ClinetSession);

protected:	
	virtual	WBANetwork::ServerSession*	CreateSession();
	virtual	void						DeleteSession( WBANetwork::ServerSession* session );

private:
	WBANetwork::ServerSession* m_ClinetSession;
};
