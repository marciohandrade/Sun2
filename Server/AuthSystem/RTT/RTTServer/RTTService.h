#pragma once

#include <eXtreme/net/ServerSession.h>

class RTTService : public eXtreme::ServerSession
{
public:
			RTTService();
	virtual	~RTTService();

			void			SendRTTAStart();
			void			SendRTTAStop();
			void			SendRTTAPatch();

			short			m_worldID, m_channelID, m_serverType, m_serverID;

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
			void			OnRTTRHeartBeatNTF( char* buffer, int size );
			DWORD			m_tickAlive;

			bool			m_bConnect;
};