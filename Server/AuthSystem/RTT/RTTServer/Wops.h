#pragma once

#include <eXtreme/net/ServerSession.h>

class Wops : public eXtreme::ServerSession
{
public:
			Wops();
	virtual	~Wops();

			void			SendMessage( MSG_HEADER8& packet, int size = 0 );

protected:
	virtual	void			OnAccept();
	virtual	void			OnSend( int size );
	virtual	void			OnReceive( char* buffer, int size );
	virtual	void			OnClose( DWORD error );
	virtual	void			Update();

	virtual	bool			IsValidPacket( char* ptr, DWORD recvBytes, DWORD* totalSize )
			{
				*totalSize = *( DWORD* )ptr + sizeof( MSG_HEADER8 );

				return ( recvBytes >= *totalSize );
			}


			void			OnRTTWServerListReq( char* buffer, int size );
			void			OnRTTWGameNoticeReq( char* buffer, int size );
			void			OnRTTWGameAdminListReq( char* buffer, int size );
			void			OnRTTWGameAdminAddNTF( char* buffer, int size );
			void			OnRTTWGameAdminRemoveNTF( char* buffer, int size );
			void			OnRTTWDropUserReq( char* buffer, int size );
			void			OnRTTWRTTAStartReq( char* buffer, int size );
			void			OnRTTWRTTAStopReq( char* buffer, int size );
			void			OnRTTWRTTAPatchNTF( char* buffer, int size );
			void			OnRTTWSystemRebootReq( char* buffer, int size );
			void			OnRTTWProcessControlNTF( char* buffer, int size );
			void			OnRTTWConnectionTypeNTF( char* buffer, int size );
			void			OnRTTWServerShutdownReq( char* buffer, int size );
			void			OnRTTWLaunchPatcherReq( char* buffer, int size );
			void			OnRTTWLaunchProcessReq( char* buffer, int size );
			void			OnRTTWFuncrestrictReloadNTF(char* buffer, int size );
			void			OnRTTAuthIPListReloadReq(char* buffer, int size );
			void			OnRTTGameVersionUpdateReq(char* buffer, int size );			
//			void			OnRTTPatternReloadReq(char* buffer, int size);
			void			OnRTTWArmorkitDispatchNTF(char* buffer, int size);
			void			OnRTTWChangeGuildNameReq(char* buffer, int size);
			void			OnRTTWSendMessageReq(char* buffer, int size);
			
};