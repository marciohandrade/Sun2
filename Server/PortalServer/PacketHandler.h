#pragma once

#include <Singleton.h>
#include <FunctionMap.h>

enum ePACKET_HANDLER
{
	PACKET_HANDLER_AGENT,
	PACKET_HANDLER_GAME,
	PACKET_HANDLER_MASTER,
	PACKET_HANDLER_DBPROXY,
	PACKET_HANDLER_MAX,
};

struct MSG_BASE;
class ServerSession;

class PacketHandler : public util::Singleton<PacketHandler>
{

	typedef VOID (*fnHandler)( ServerSession * pSession, MSG_BASE *pMsg, WORD wSize );

public:
	PacketHandler();
	~PacketHandler();


	BOOL				Resister();
	BOOL				AddHandler( ePACKET_HANDLER PHandler, BYTE category, BYTE protocol, fnHandler fnHandler );
	VOID				ParsePacket( ePACKET_HANDLER PHandler, ServerSession * pSession, MSG_BASE *pMsg, WORD wSize );

private:
	struct HANDLER_FUNC : public util::BASE_FUNC
	{
		fnHandler		m_fnHandler;
	};

	util::FunctionMap	m_FuncMap[PACKET_HANDLER_MAX];
};
