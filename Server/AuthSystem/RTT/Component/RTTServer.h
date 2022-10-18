#pragma once

#include <eXtreme/net/ServerSession.h>

class RTTServer : public eXtreme::ServerSession
{
public:
			RTTServer();
	virtual	~RTTServer();

protected:
	virtual	void			Update();

	virtual	void			OnConnect( bool success, DWORD error );
	virtual	void			OnReceive( char* buffer, int size );
	virtual	void			OnClose( DWORD error );

	virtual	bool			IsValidPacket( char* ptr, DWORD recvBytes, DWORD* totalSize )
			{
				*totalSize = *( DWORD* )ptr + 8;

				return ( recvBytes >= *totalSize );
			}

private:
			void			CheckDisconnected();

			void			SendServerListReq();
			void			SendGameNoticeReq();
			void			SendGameAdminListReq();
			void			SendGameAdminAddNTF();
			void			SendRTTAStartReq();
			void			SendRTTAStopReq();
			void			SendRTTAPatchNTF();
			void			SendSystemRebootNTF();
			void			SendProcessControlNTF();
			void			SendConnectionTypeNTF();
			void			SendServerShutdownReq();
			void			SendLaunchPatcherReq();
			void			SendLaunchProcessReq();
			void			SendFuncrestrictReloadNTF();
			void			SendRTTG_DROPUSER_REQ();
			void			SendAuthIPListReloadReq();
			void			SendGameVersionUpdateReq();
			void			SendArmorkitDispatchNTF();
			void			SendChangeGuildNameReq();
			void			SendSendMessageReq();
			void			SendDropUserFromWebReq();


			void			OnRTTWServerListAns( char* buffer, int size );
			void			OnRTTWGameNoticeAns( char* buffer, int size );
			void			OnRTTWGameAdminListAns( char* buffer, int size );
			void			OnRTTWGameAdminRemoveNTF( char* buffer, int size );
			void			OnRTTWRTTAStartAns( char* buffer, int size );
			void			OnRTTWRTTAStopAns( char* buffer, int size );
			void			OnRTTWErrorNotifyNTF( char* buffer, int size );
			void			OnRTTWPatchResultAns( char* buffer, int size );
			void			OnRTTWLaunchProcessAns( char* buffer, int size );
			void			OnRTTWServerShutdownAns( char* buffer, int size );
			void			OnRTTWAuthIPListReloadAns( char* buffer, int size );
			void			OnRTTW_DROPUSER_Ans( char* buffer, int size );
			void			OnRTTWArmorkitDispatchAns( char* buffer, int size );
			void			OnRTTWChangeGuildNameAns( char* buffer, int size );
			void			OnRTTWSendMessageAns( char* buffer, int size );
			void			OnRTTH_DROPUSER_Ans( char* buffer, int size );

			int				m_connectState;
			DWORD			m_disconnTick;
};