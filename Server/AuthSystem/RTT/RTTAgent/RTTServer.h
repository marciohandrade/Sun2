#pragma once

#include <eXtreme/net/ServerSession.h>

#define TIME_HEARTBEAT	10000

class RTTServer : public eXtreme::ServerSession
{
public:
			RTTServer();
	virtual	~RTTServer();

			void			SendMessage( MSG_HEADER8& packet, int size = 0 );


	static	RTTServer*		instance;
	static	char			m_patchLauncherPath[MAX_PATH];
	static	char			m_patchLauncherBinary[60];

protected:
	virtual	void			OnConnect( bool success, DWORD error );
	virtual	void			OnSend( int size );
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
			void			SendRTTWCertifyReq();
			void			SendRTTRProcessListReq();
			void			SendRTTRWebUrlReq();
			void			SendHeartBeat();

			void			OnRTTWCertifyAns( char* buffer, int size );
			void			OnRTTWGameNoticeReq( char* buffer, int size );
			void			OnRTTWDropUserReq( char* buffer, int size );
			void			OnRTTWGameAdminListAns( char* buffer, int size );
			void			OnRTTWGameAdminAddNTF( char* buffer, int size );
			void			OnRTTWGameAdminRemoveNTF( char* buffer, int size );
			void			OnRTTWProecssControlNTF( char* buffer, int size );
			void			OnRTTWBattleZoneInfoReq( char* buffer, int size );
			void			OnRTTWUserMoneyReq( char* buffer, int size );
			void			OnRTTWSystemRebootNTF( char* buffer, int size );
			void			OnRTTWServerShutdownReq( char* buffer, int size );
			void			OnRTTWLaunchPatcherReq( char* buffer, int size );
			void			OnRTTWLaunchProcessReq( char* buffer, int size );

			void			OnRTTRProcessListAns( char* buffer, int size );
			void			OnRTTRPatchLauncherInfoNTF( char* buffer, int size );
			void			OnRTTRWebUrlAns( char* buffer, int size );
			
			void			OnRTTWFuncrestrictReloadNTF(char* buffer, int size );
			void			OnRTTAuthIPListReloadReq(char* buffer, int size );
			void			OnRTTWGameVersionUpdateReq(char* buffer, int size );
			void			OnRTTWGameStatisticsReq(char* buffer, int size );
//			void			OnRTTPatternReloadReq(char* buffer, int size );

			void			OnRTTWArmorkitDispatchNTF(char* buffer, int size );
			void			OnRTTWChangeGuildNameReq(char* buffer, int size);
			void			OnRTTWSendMessageReq(char* buffer, int size);
			void			OnRTTHDropUserReq( char* buffer, int size );



			int				m_connectState;
			DWORD			m_disconnTick;
			DWORD			m_serverKey;

			DWORD			m_dwTicHeartBeat;
};