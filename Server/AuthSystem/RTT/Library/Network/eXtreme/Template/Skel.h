#pragma once

#include <eXtreme/net/ServerSession.h>

class Skel : public eXtreme::ServerSession
{
public:
			Skel();
	virtual	~Skel();

	virtual	void			Update( bool activate );

protected:
	virtual	void			OnAccept();
	virtual	void			OnReceive( char* buffer, int size );
	virtual	void			OnClose( DWORD error );

	virtual	bool			IsValidPacket( WORD packetSize, DWORD recvBytes, WORD* totalSize )
			{
				*totalSize = packetSize + 2;

				return ( recvBytes >= ( DWORD )packetSize + 2 );
			}
};