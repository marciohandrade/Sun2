#include "stdafx.h"

#include <algorithm>

#include "ManagerClient.h"

ManagerClient::ManagerClient()
{
}

ManagerClient::~ManagerClient()
{
}

bool ManagerClient::AddClient( RTTAgent* client )
{
	m_listClient.push_back( client );
#ifdef _DEBUG
	OutputDebug("[ManagerClient::AddClient] %s(%d) \n", client->GetIPAddress(), m_listClient.size());
#endif
	return true;
}

void ManagerClient::RemoveClient( RTTAgent* client )
{
	for(ListClient::iterator iter = m_listClient.begin() ; iter != m_listClient.end() ; iter++ )
	{
		if( *iter == client )
		{
			m_listClient.erase( iter );
			break;
		}
	}
#ifdef _DEBUG
	OutputDebug("[ManagerClient::RemoveClient] %s(%d) \n", client->GetIPAddress(), m_listClient.size());
#endif
}


void ManagerClient::AddMasterServerIP(DWORD dwWorldID, char* pIP)
{
	eXtreme::Guard	< eXtreme::Mutex >	lock( m_mutexMasterClient );

	MapMasterClient::iterator iter = m_mapMasterClientIP.find(dwWorldID);

	if( iter == m_mapMasterClientIP.end() )
	{
		char* szIP = new char[IP_MAX_LEN];
		memset(szIP, 0, IP_MAX_LEN);
		strncpy(szIP, pIP, IP_MAX_LEN);
		szIP[IP_MAX_LEN-1] = '\0';
		m_mapMasterClientIP.insert( MapMasterClient::value_type( dwWorldID, szIP ) );

		OutputDebug("[ManagerClient::AddMasterServerIP] %d World(%s), Total %d IP(s)\n", dwWorldID, szIP, m_mapMasterClientIP.size());
	}
}

void ManagerClient::MakeClientList( RTTAgent* list[], DWORD bufferSize, DWORD* listCount )
{
	*listCount = 0;
	for(ListClient::iterator iter = m_listClient.begin() ; iter != m_listClient.end() ; iter++ )
	{
		if( *listCount >= bufferSize )
			break;

		list[*listCount] = *iter;
		( *listCount )++;
	}
}

RTTAgent* ManagerClient::FindClient( DWORD type )
{
	RTTAgent*				client = NULL;

	for(ListClient::iterator iter = m_listClient.begin() ; iter != m_listClient.end() ; iter++ )
	{
		client = *iter;

		if( ( client->GetContainType() & type ) == type )
			return client;
	}

	return NULL;
}

RTTAgent* ManagerClient::FindClient( char* ipAddress )
{
	RTTAgent*				client = NULL;

	for(ListClient::iterator iter = m_listClient.begin() ; iter != m_listClient.end() ; iter++ )
	{
		client = *iter;

		if( strncmp( client->GetIPAddress(), ipAddress, IP_MAX_LEN ) == 0 )
			return client;
	}

	return NULL;
}

RTTAgent* ManagerClient::FindClient( DWORD dwWorldID, DWORD dwChannelID, DWORD dwServerType, DWORD dwServerID )
{
	RTTAgent*				pRTTAgent = NULL;

	for(ListClient::iterator iter = m_listClient.begin() ; iter != m_listClient.end() ; iter++ )
	{
		pRTTAgent = *iter;
		if(pRTTAgent->m_worldID		== (short)dwWorldID &&
		   pRTTAgent->m_channelID	== (short)dwChannelID &&
		   pRTTAgent->m_serverType	== (short)dwServerType &&
		   pRTTAgent->m_serverID	== (short)dwServerID)
		{
			return pRTTAgent;
		}
	}

	return NULL;
}

void ManagerClient::Broadcast( char* buffer, int size )
{
	RTTAgent*				client = NULL;

	for(ListClient::iterator iter = m_listClient.begin() ; iter != m_listClient.end() ; iter++ )
	{
		client = *iter;
		client->SendPacket( buffer, size );
	}
}

void ManagerClient::Broadcast( DWORD worldID, DWORD channelID, char* buffer, int size )
{
	RTTAgent*				client = NULL;

	for(ListClient::iterator iter = m_listClient.begin() ; iter != m_listClient.end() ; iter++ )
	{
		client = *iter;
		
		if( worldID == 0 ||				//  전체 World에 보낸다.
										//  해당 World의 전체 Channel에 보낸다.
			( worldID != 0 && channelID == CHANNEL_ALL && client->m_worldID == (short)worldID ) ||
										//  World와 Channel을 필터링한다.
			( worldID != 0 && channelID != CHANNEL_ALL && client->m_worldID == (short)worldID && client->m_channelID == (short)channelID ) )
		{
			OutputDebug( "[RTTAgent Broadcast info] %d, %d, %d, %d, %s\n",
				client->m_worldID, client->m_channelID, client->m_serverType, client->m_serverID,
				client->GetIPAddress() );
			client->SendPacket( buffer, size );
		}
	}
}

void ManagerClient::Broadcast( DWORD worldID, DWORD channelID, DWORD serverType, DWORD serverID, char* buffer, int size )
{
	RTTAgent*				client = NULL;
	
	bool bBroadCast = false;	// broadcast 대상이 검색되었는지 여부

	for(ListClient::iterator iter = m_listClient.begin() ; iter != m_listClient.end() ; iter++ ){
		client = *iter;
		bBroadCast = false;

		if(worldID==0 || (short)worldID == client->m_worldID)							// 전체 world 또는 특정 world
		{
			if(channelID==CHANNEL_ALL || (short)channelID==client->m_channelID)			// 전체 channel 또는 특정 channel
			{
				if(serverType==0 || (short)serverType==client->m_serverType)			// 전체 servertype 또는 특정 servertype
				{
					if(serverID==0 || (short)serverID==client->m_serverID)				// 전체 serverid 또는 특정 serverid
					{
						bBroadCast = true;
					}
				}
			}
		}	

		if(bBroadCast==true){
			OutputDebug( "[RTTAgent Broadcast info] %d, %d, %d, %d, %s\n",
												client->m_worldID, 
												client->m_channelID, 
												client->m_serverType, 
												client->m_serverID,
												client->GetIPAddress() );
			
			client->SendPacket( buffer, size );
		}
	}
}


bool ManagerClient::SendToMasterClient( MSG_HEADER8& packet, DWORD dwWorldID )
{
	RTTAgent* pMaster = NULL;

	bool IsSend = false;

	// 월드가 0 이거나 마스터 서버가 1개면 브로드캐스트, 월드가 1 이상이면서 마스터가 2개 이상이면 해당 월드만...
	if( (0 != dwWorldID) && (1 < static_cast<int>( m_mapMasterClientIP.size())) )
	{
		for( ListClient::iterator iter = m_listClient.begin() ; iter != m_listClient.end() ; ++iter )
		{
			pMaster = *iter;
			// 글로벌 버전 마스터만 2개인데 서버 구성상 마스터의 WorldID가 게임의 WorldID와 동일하여 마스터가 설치된 RTTAgent를 찾을때
			// WorldID까지 비교할 수 있으나 이외의 버전은 다른 머신에 설치되어 앞서 말한 두 개의 World가 동일하지 않으므로 필드 추가등의
			// 추가적인 조치를 해야하지만 게임쪽도 고쳐야하기 때문에 현실상 힘듦. 그럴땐 그냥 브로드캐스트...ㅡ,.ㅡ;
			// 아무튼 이런 처리도 현재는 글로벌 전용이고 다른 국가에 이런식으로 마스터 서버가 2개 이상 설치되는 경우 게임의 WOrldID와
			// 마스터서버의 WorldID가 다를 수 있으므로 이 때는 이 로직을 바꾸어야 함
			if( ((short)dwWorldID == pMaster->m_worldID) && ( ( pMaster->GetContainType() & SERVERTYPE_MASTER ) == SERVERTYPE_MASTER  ) )
			{
				pMaster->SendMessage(packet);
				IsSend = true;
				break;
			}
		}
	}
	else
	{
		eXtreme::Guard	< eXtreme::Mutex >	lock( m_mutexMasterClient );

		int SendCount = 0;

		for( ListClient::iterator iter = m_listClient.begin() ; iter != m_listClient.end() ; ++iter )
		{
			pMaster = *iter;
			if(  ( pMaster->GetContainType() & SERVERTYPE_MASTER ) == SERVERTYPE_MASTER )
			{
				pMaster->SendMessage(packet);
				IsSend = true;
				SendCount++;
				OutputDebug( "[ManagerClient::SendToMasterClient::MSG_HEADER8&] Send To Master(%s)\n", pMaster->GetIPAddress() );
			}
		}

		if( SendCount < static_cast<int>( m_mapMasterClientIP.size() ) )
		{
            OutputDebug( "[ManagerClient::SendToMasterClient::MSG_HEADER8&] Broadcast Failed\n");
			OutputDebug( "[ManagerClient::SendToMasterClient::MSG_HEADER8&] Master IP List\n");

			for( MapMasterClient::iterator iter = m_mapMasterClientIP.begin() ; iter != m_mapMasterClientIP.end() ; ++iter )
			{
				OutputDebug( "[ManagerClient::SendToMasterClient::MSG_HEADER8&] %d, %s\n", (*iter).first, (*iter).second );
			}
		}
	}

	return IsSend;
}

bool ManagerClient::SendToMasterClient( eXtreme::Stream& stream, DWORD dwWorldID )
{
	RTTAgent* pMaster = NULL;

	bool IsSend = false;

	// 월드가 0 이거나 마스터 서버가 1개면 브로드캐스트, 월드가 1 이상이면서 마스터가 2개 이상이면 해당 월드만...
	if( (0 != dwWorldID) && (1 < static_cast<int>( m_mapMasterClientIP.size())) )
	{
		for( ListClient::iterator iter = m_listClient.begin() ; iter != m_listClient.end() ; ++iter )
		{
			pMaster = *iter;
			// 글로벌 버전 마스터만 2개인데 서버 구성상 마스터의 WorldID가 게임의 WorldID와 동일하여 마스터가 설치된 RTTAgent를 찾을때
			// WorldID까지 비교할 수 있으나 이외의 버전은 다른 머신에 설치되어 앞서 말한 두 개의 World가 동일하지 않으므로 필드 추가등의
			// 추가적인 조치를 해야하지만 게임쪽도 고쳐야하기 때문에 현실상 힘듦. 그럴땐 그냥 브로드캐스트...ㅡ,.ㅡ;
			// 아무튼 이런 처리도 현재는 글로벌 전용이고 다른 국가에 이런식으로 마스터 서버가 2개 이상 설치되는 경우 게임의 WOrldID와
			// 마스터서버의 WorldID가 다를 수 있으므로 이 때는 이 로직을 바꾸어야 함
			if( ((short)dwWorldID == pMaster->m_worldID) && ( ( pMaster->GetContainType() & SERVERTYPE_MASTER ) == SERVERTYPE_MASTER  ) )
			{
				pMaster->SendPacket(stream);
				IsSend = true;
				break;
			}
		}
	}
	else
	{
		eXtreme::Guard	< eXtreme::Mutex >	lock( m_mutexMasterClient );

		int SendCount = 0;

		for( ListClient::iterator iter = m_listClient.begin() ; iter != m_listClient.end() ; ++iter )
		{
			pMaster = *iter;
			if(  ( pMaster->GetContainType() & SERVERTYPE_MASTER ) == SERVERTYPE_MASTER )
			{
				pMaster->SendPacket(stream);
				IsSend = true;
				SendCount++;
				OutputDebug( "[ManagerClient::SendToMasterClient::eXtreme::Stream&] Send To Master(%s)\n", pMaster->GetIPAddress() );
			}
		}

		if( SendCount < static_cast<int>( m_mapMasterClientIP.size() ) )
		{
            OutputDebug( "[ManagerClient::SendToMasterClient::eXtreme::Stream&] Broadcast Failed\n");
			OutputDebug( "[ManagerClient::SendToMasterClient::eXtreme::Stream&] Master IP List\n");

			for( MapMasterClient::iterator iter = m_mapMasterClientIP.begin() ; iter != m_mapMasterClientIP.end() ; ++iter )
			{
				OutputDebug( "[ManagerClient::SendToMasterClient::eXtreme::Stream&] %d, %s\n", (*iter).first, (*iter).second );
			}
		}
	}

	return IsSend;
}

/**
	Database와 연결이 끊긴 경우 모든 소켓을 정리를 해야 한다.
*/
void ManagerClient::DisconnectAll()
{
	ListClient::iterator	iter;
	RTTAgent*				client = NULL;

	while(!m_listClient.empty())
	{
		iter = m_listClient.begin();
		client = *iter;
		
		m_listClient.erase( iter );
		client->SetKill();
	}
}
