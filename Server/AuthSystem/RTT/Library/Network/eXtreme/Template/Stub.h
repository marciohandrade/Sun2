#pragma once

#include <eXtreme/net/ServerSession.h>

class Stub : public eXtreme::ServerSession
{
public:
			Stub();
	virtual	~Stub();

	virtual	void			Update( bool activate );

protected:
	virtual	void			OnConnect( bool success, DWORD error );
	virtual	void			OnSend( int size );
	virtual	void			OnReceive( char* buffer, int size );
	virtual	void			OnClose( DWORD error );


			int				m_connectState;
			DWORD			m_disconnTick;
};