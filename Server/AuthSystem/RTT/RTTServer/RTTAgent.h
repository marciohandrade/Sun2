#pragma once

#include <eXtreme/net/ServerSession.h>

#include "Database.h"

typedef struct sErrorInsertIndex
{
	DWORD dwConnectParentHistoryIndex;
	DWORD dwPageFaultParentHistoryIndex;

	sErrorInsertIndex() : dwConnectParentHistoryIndex(0), dwPageFaultParentHistoryIndex(0)
	{
	}

} ERROR_INSERT_INDEX, *LPERROR_INSERT_INDEX;

class RTTAgent : public eXtreme::ServerSession
{
public:
			RTTAgent();
	virtual	~RTTAgent();

			void			SendMessage( MSG_HEADER8& packet, int size = 0 );
			DWORD			GetContainType()				{		return m_containType;	}

			void			SendBattleZoneInfoReq();
			void			SendUserMoneyReq();
			void			SendGameStatisticsReq();
			void			SendPatchLauncherInfoNTF();

			short			m_worldID, m_channelID, m_serverType, m_serverID;

	static	DWORD			m_updateBattleZoneInfoTick, m_updateUserMoneyTick;
			
			bool			IsConnected(){ return m_bConnect; };

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

private:
			void			OnRTTWCertifyReq( char* buffer, int size );
			void			OnRTTWDisCertifyNTF( char* buffer, int size );
			void			OnRTTWGameNoticeAns( char* buffer, int size );
			void			OnRTTWDropUserAns( char* buffer, int size );
			void			OnRTTWGameAdminListReq( char* buffer, int size );
			void			OnRTTWUserCountNTF( char* buffer, int size );
			void			OnRTTWBattleZoneInfoAns( char* buffer, int size );
			void			OnRTTWArmorKitFindHackNTF( char* buffer, int size );
			void			OnRTTWAbusingNTF( char* buffer, int size );
			void			OnRTTWUserMoneyAns( char* buffer, int size );
			void			OnRTTWServerShutdownAns( char* buffer, int size );
			void			OnRTTWPatchResultAns( char* buffer, int size );
			void			OnRTTWLaunchProcessAns( char* buffer, int size );

			void			OnRTTRResourceNTF( char* buffer, int size );
			void			OnRTTRProcessListReq( char* buffer, int size );
			void			OnRTTRProcessMonitorNTF( char* buffer, int size );
			void			OnRTTRWebUrlReq( char* buffer, int size );
			void			OnRTTRWebRequestNTF( char* buffer, int size );
			void			OnRTTWFuncrestrictReloadAns( char* buffer, int size );
			void			OnRTTAuthIPListReloadAns( char* buffer, int size );
			void			OnRTTGameVersionUpdateAns( char* buffer, int size );
			void			OnRTTGameStatisticsAns( char* buffer, int size );
//			void			OnRTTPatternReloadAns( char* buffer, int size );

			void			OnRTTRHeartBeatNTF( char* buffer, int size );
			void			OnRTTWArmorkitDispatchANS( char* buffer, int size );
			void			OnRTTWChangeGuildNameAns(char* buffer, int size);
			void			OnRTTWSendMessageAns(char* buffer, int size);

			void			OnRTTHDropUserAns( char* buffer, int size );		// 중국 웹 상에서 유저 강제 종료 요청 결과(Wops의 명령과 다르다!!)

			DWORD			m_containType;				//  RTTAgent가 어떤 서버와 통신하는지를 명시한다.
			ServerInfo*		m_ownServerList[50];		//  RTTAgent가 설치된 Local의 서버목록이다.
			DWORD			m_ownServerCount;			//  m_ownServerList의 개수이다.

			ERROR_INSERT_INDEX	m_ErrorIndexList[10];	//  감시할 URL 리스트
			DWORD				m_ErrorIndexCount;		//  감시할 URL 개수

			DWORD			m_tickAlive;

			bool			m_bConnect;
};