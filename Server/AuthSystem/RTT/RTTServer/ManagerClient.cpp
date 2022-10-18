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
		
		if( worldID == 0 ||				//  ��ü World�� ������.
										//  �ش� World�� ��ü Channel�� ������.
			( worldID != 0 && channelID == CHANNEL_ALL && client->m_worldID == (short)worldID ) ||
										//  World�� Channel�� ���͸��Ѵ�.
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
	
	bool bBroadCast = false;	// broadcast ����� �˻��Ǿ����� ����

	for(ListClient::iterator iter = m_listClient.begin() ; iter != m_listClient.end() ; iter++ ){
		client = *iter;
		bBroadCast = false;

		if(worldID==0 || (short)worldID == client->m_worldID)							// ��ü world �Ǵ� Ư�� world
		{
			if(channelID==CHANNEL_ALL || (short)channelID==client->m_channelID)			// ��ü channel �Ǵ� Ư�� channel
			{
				if(serverType==0 || (short)serverType==client->m_serverType)			// ��ü servertype �Ǵ� Ư�� servertype
				{
					if(serverID==0 || (short)serverID==client->m_serverID)				// ��ü serverid �Ǵ� Ư�� serverid
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

	// ���尡 0 �̰ų� ������ ������ 1���� ��ε�ĳ��Ʈ, ���尡 1 �̻��̸鼭 �����Ͱ� 2�� �̻��̸� �ش� ���常...
	if( (0 != dwWorldID) && (1 < static_cast<int>( m_mapMasterClientIP.size())) )
	{
		for( ListClient::iterator iter = m_listClient.begin() ; iter != m_listClient.end() ; ++iter )
		{
			pMaster = *iter;
			// �۷ι� ���� �����͸� 2���ε� ���� ������ �������� WorldID�� ������ WorldID�� �����Ͽ� �����Ͱ� ��ġ�� RTTAgent�� ã����
			// WorldID���� ���� �� ������ �̿��� ������ �ٸ� �ӽſ� ��ġ�Ǿ� �ռ� ���� �� ���� World�� �������� �����Ƿ� �ʵ� �߰�����
			// �߰����� ��ġ�� �ؾ������� �����ʵ� ���ľ��ϱ� ������ ���ǻ� ����. �׷��� �׳� ��ε�ĳ��Ʈ...��,.��;
			// �ƹ�ư �̷� ó���� ����� �۷ι� �����̰� �ٸ� ������ �̷������� ������ ������ 2�� �̻� ��ġ�Ǵ� ��� ������ WOrldID��
			// �����ͼ����� WorldID�� �ٸ� �� �����Ƿ� �� ���� �� ������ �ٲپ�� ��
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

	// ���尡 0 �̰ų� ������ ������ 1���� ��ε�ĳ��Ʈ, ���尡 1 �̻��̸鼭 �����Ͱ� 2�� �̻��̸� �ش� ���常...
	if( (0 != dwWorldID) && (1 < static_cast<int>( m_mapMasterClientIP.size())) )
	{
		for( ListClient::iterator iter = m_listClient.begin() ; iter != m_listClient.end() ; ++iter )
		{
			pMaster = *iter;
			// �۷ι� ���� �����͸� 2���ε� ���� ������ �������� WorldID�� ������ WorldID�� �����Ͽ� �����Ͱ� ��ġ�� RTTAgent�� ã����
			// WorldID���� ���� �� ������ �̿��� ������ �ٸ� �ӽſ� ��ġ�Ǿ� �ռ� ���� �� ���� World�� �������� �����Ƿ� �ʵ� �߰�����
			// �߰����� ��ġ�� �ؾ������� �����ʵ� ���ľ��ϱ� ������ ���ǻ� ����. �׷��� �׳� ��ε�ĳ��Ʈ...��,.��;
			// �ƹ�ư �̷� ó���� ����� �۷ι� �����̰� �ٸ� ������ �̷������� ������ ������ 2�� �̻� ��ġ�Ǵ� ��� ������ WOrldID��
			// �����ͼ����� WorldID�� �ٸ� �� �����Ƿ� �� ���� �� ������ �ٲپ�� ��
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
	Database�� ������ ���� ��� ��� ������ ������ �ؾ� �Ѵ�.
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
