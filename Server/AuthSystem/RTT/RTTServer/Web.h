#pragma once

#include <eXtreme/net/ServerSession.h>

class Web : public eXtreme::ServerSession
{
public:
			Web();
	virtual	~Web();

			bool			IsConnected(){ return m_bConnect; };
protected:
	virtual	void			OnAccept();
	virtual	void			OnReceive( char* buffer, int size );
	virtual	void			OnClose( DWORD error );
	virtual	void			Update();

	virtual	bool			IsValidPacket( char* ptr, DWORD recvBytes, DWORD* totalSize )
			{
				*totalSize = *( DWORD* )ptr + sizeof( MSG_HEADER8 );

				return ( recvBytes >= *totalSize );
			}
	
			void			SendMessage( MSG_HEADER8& packet, int size = 0 );

private:
			void			OnRTTHDropUserReq( char* buffer, int size );
			DWORD			m_tickAlive;

			bool			m_bConnect;
};