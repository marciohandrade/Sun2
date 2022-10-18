#pragma once

#include <eXtreme/net/ServerSession.h>
#include "../common/Protocol_RTT.h"

class RTTServer : public eXtreme::ServerSession
{
public:
			RTTServer();
	virtual	~RTTServer();

	virtual	void			Update();

protected:
	virtual	void			OnConnect( bool success, DWORD error );
	virtual	void			OnReceive( char* buffer, int size );
	virtual	void			OnClose( DWORD error );

	virtual	bool			IsValidPacket( char* ptr, DWORD recvBytes, DWORD* totalSize )
			{
				*totalSize = *( u_short* )ptr + 2;

				return ( recvBytes >= *totalSize );
			}

private:
			void			CheckDisconnected();

			void			SendHeartBeat();

			void			SendRTTGCertifyReq();
			void			SendRTTGGameAdminListReq();
			void			SendRTTGDropUserAns();
			void			SendRTTGUserCount();
			void			SendRTTGBattleZoneInfoAns();
			void			SendRTTGArmorKitFindHackNTF();
			void			SendRTTGAbusingNTF();

			void			SendTempRTTGArmorKitFindHackNTF();

			void			OnRTTGCertifyAns( char* buffer, int size );
			void			OnRTTGGameNoticeReq( char* buffer, int size );
			void			OnRTTGGameAdminListAns( char* buffer, int size );
			void			OnRTTGGameAdminAddNTF( char* buffer, int size );
			void			OnRTTGGameAdminRemoveNTF( char* buffer, int size );
			void			OnRTTGDropUserReq( char* buffer, int size );
			void			OnRTTGBattleZoneInfoReq( char* buffer, int size );
			void			OnRTTGUserMoneyReq( char* buffer, int size );
			void			OnRTTGServerShutdownReq( char* buffer, int size );
			void			OnRTTGameStatisticsReq( char* buffer, int size );
			void			OnRTTGArmorkitDispatchNTF( char* buffer, int size );
			void			OnRTTGChangeGuildNameReq( char* buffer, int size );
			void			OnRTTGSendMessageReq( char* buffer, int size );

			int				m_connectState;
			DWORD			m_disconnTick;
			DWORD			m_AliveTick;

			class MSG_RTTR_HEARTBEAT_NTF : public MSG_HEADER4
			{
			public:
				MSG_RTTR_HEARTBEAT_NTF()
				{
					wSize	= sizeof( MSG_RTTR_HEARTBEAT_NTF ) - SIZEOFHDR;
					wID	= RTTR_HEARTBEAT_NTF;
				}
				~MSG_RTTR_HEARTBEAT_NTF()
				{
				}
			};
};