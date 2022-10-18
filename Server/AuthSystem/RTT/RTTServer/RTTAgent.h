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

			void			OnRTTHDropUserAns( char* buffer, int size );		// �߱� �� �󿡼� ���� ���� ���� ��û ���(Wops�� ��ɰ� �ٸ���!!)

			DWORD			m_containType;				//  RTTAgent�� � ������ ����ϴ����� ����Ѵ�.
			ServerInfo*		m_ownServerList[50];		//  RTTAgent�� ��ġ�� Local�� ��������̴�.
			DWORD			m_ownServerCount;			//  m_ownServerList�� �����̴�.

			ERROR_INSERT_INDEX	m_ErrorIndexList[10];	//  ������ URL ����Ʈ
			DWORD				m_ErrorIndexCount;		//  ������ URL ����

			DWORD			m_tickAlive;

			bool			m_bConnect;
};