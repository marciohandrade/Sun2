#include "stdafx.h"
#include "GameServerSession.h"
#include <PacketHandler/PacketHandler.h>

GameServerSession::GameServerSession(void)
{

}

GameServerSession::~GameServerSession(void)
{

}

// CHANGES: f110617.1L
void GameServerSession::OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg)
{
    ServerSessionEx::OnRedirect(network_index, first_contact_msg);
}

VOID GameServerSession::OnLogString( char *pszLog )
{
	_PRINT( pszLog );
}