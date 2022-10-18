#pragma once

#include <eXtreme/net/ServerSession.h>

#define TIME_HEARTBEAT	10000

class RTTServer : public eXtreme::ServerSession
{
public:
			RTTServer();
	virtual	~RTTServer();

			void			SendMessage( MSG_HEADER8& packet, int size = 0 );

protected:
	virtual	void			OnConnect( bool success, DWORD error );
	virtual	void			OnReceive( char* buffer, int size );
	virtual	void			OnClose( DWORD error );
	virtual	void			Update();

	virtual	bool			IsValidPacket( char* ptr, DWORD recvBytes, DWORD* totalSize )
			{
				*totalSize = *( DWORD* )ptr + sizeof( MSG_HEADER8 );

				return ( recvBytes >= *totalSize );
			}

private:
			void			CheckDisconnected();
			
			void			SendHeartBeat();

			void			OnRTTAStartNTF( char* buffer, int size );
			void			OnRTTAStopNTF( char* buffer, int size );
			void			OnRTTAPatchNTF( char* buffer, int size );


			int				m_connectState;
			DWORD			m_disconnTick;

			DWORD			m_dwTicHeartBeat;
};