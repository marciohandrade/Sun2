#pragma once

#include <eXtreme/net/ServerSession.h>

class Skel : public eXtreme::ServerSession
{
public:
			Skel();
	virtual	~Skel();


			void			SetType( DWORD type )		{	m_type = type;		}
			DWORD			GetType()					{	return m_type;		}
			void			SendMessage( MSG_HEADER4& packet, int size = 0 );

protected:
	virtual	void			OnAccept();
	virtual	void			OnSend( int size );
	virtual	void			OnReceive( char* buffer, int size );
	virtual	void			OnClose( DWORD error );
	virtual	void			Update();

	virtual	bool			IsValidPacket( char* ptr, DWORD recvBytes, DWORD* totalSize )
			{
				*totalSize = *( u_short* )ptr + 2;

				return ( recvBytes >= *totalSize );
			}

private:
			void			OnRTTGCertifyReq( char* buffer, int size );
			void			OnRTTGUserCountNTF( char* buffer, int size );
			void			OnRTTGDropUserAns( char* buffer, int size );
			void			OnRTTGGameAdminListReq( char* buffer, int size );
			void			OnRTTGBattleZoneInfoAns( char* buffer, int size );
			void			OnRTTGArmorKitFindHackNTF( char* buffer, int size );
			void			OnRTTGAbusingNTF( char* buffer, int size );
			void			OnRTTGUserMoneyAns( char* buffer, int size );
			void			OnRTTGServerShutdownAns( char* buffer, int size );
			void			OnRTTGPatchResultAns( char* buffer, int size );
			void			OnRTTAuthIPListReloadAns( char* buffer, int size );
			void			OnRTTGameVersionUpdateAns(char* buffer, int size);
			void			OnRTTGameStatisticsAns(char* buffer, int size);
//			void			OnRTTPatternReloadAns(char* buffer, int size );

			void			OnRTTGArmorkitDispatchAns(char* buffer, int size);
			void			OnRTTGChangeGuildNameAns(char* buffer, int size);
			void			OnRTTGSendMessageAns(char* buffer, int size);

			void			SendRTTGPatchResultReq();


			DWORD			m_type;
			DWORD			m_tickPacket, m_tickCheckStatus;
};