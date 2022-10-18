#pragma once


class ServerSession;
struct MSG_BASE;

class Handler_Common
{
public:
	Handler_Common(void);
	~Handler_Common(void);


	static VOID OnSERVERCOMMON_HEARTBEAT( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
};
