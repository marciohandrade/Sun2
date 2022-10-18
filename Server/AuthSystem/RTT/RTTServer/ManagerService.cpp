#include "stdafx.h"
#include "ManagerService.h"

ManagerService::ManagerService()
{
}

ManagerService::~ManagerService()
{
}

bool ManagerService::AddClient( RTTService* client )
{
	m_listClient.push_back( client );

#ifdef _DEBUG
	OutputDebug("[ManagerService::AddClient] %s(%d) \n", client->GetIPAddress(), m_listClient.size());
#endif
	return true;
}

void ManagerService::RemoveClient( RTTService* client )
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
	OutputDebug("[ManagerService::RemoveClient] %s(%d) \n", client->GetIPAddress(), m_listClient.size());
#endif
}

RTTService* ManagerService::FindClient( char* ipAddress )
{
	RTTService*				client = NULL;

	for(ListClient::iterator iter = m_listClient.begin() ; iter != m_listClient.end() ; iter++ )
	{
		client = *iter;

		if( strncmp( ipAddress, client->GetIPAddress(), 20 ) == 0 )
			return client;

		if( !(client->IsConnected()) )
		{
			OutputDebug( "[ManagerService::FindClient] disconnected RTTService(%s)\n", client->GetIPAddress() );
		}
	}

	return NULL;
}

void ManagerService::BroadcastRTTAStart()
{
	RTTService*				client = NULL;

	for(ListClient::iterator iter = m_listClient.begin() ; iter != m_listClient.end() ; iter++ )
	{
		client = *iter;
		client->SendRTTAStart();
	}
}

void ManagerService::BroadcastRTTAStop()
{
	RTTService*				client = NULL;

	for(ListClient::iterator iter = m_listClient.begin() ; iter != m_listClient.end() ; iter++ )
	{
		client = *iter;
		client->SendRTTAStop();
	}
}

void ManagerService::Broadcast( char* buffer, int size )
{
	RTTService*				client = NULL;

	for(ListClient::iterator iter = m_listClient.begin() ; iter != m_listClient.end() ; iter++ )
	{
		client = *iter;
		client->SendPacket( buffer, size );
	}
}

void ManagerService::Broadcast( DWORD worldID, DWORD channelID, char* buffer, int size )
{
	RTTService*				client = NULL;

	for(ListClient::iterator iter = m_listClient.begin() ; iter != m_listClient.end() ; iter++ )
	{
		client = *iter;

		if( worldID == 0 ||				//  ��ü World�� ������.
										//  �ش� World�� ��ü Channel�� ������.
			( worldID != 0 && channelID == CHANNEL_ALL && client->m_worldID == (short)worldID ) ||
										//  World�� Channel�� ���͸��Ѵ�.
			( worldID != 0 && channelID != CHANNEL_ALL && client->m_worldID == (short)worldID && client->m_channelID == (short)channelID ) )
		{
			OutputDebug( "[RTTService Broadcast info] %d, %d, %d, %d, %s\n",
				client->m_worldID, client->m_channelID, client->m_serverType, client->m_serverID,
				client->GetIPAddress() );
			client->SendPacket( buffer, size );
		}
	}
}

void ManagerService::Broadcast( DWORD worldID, char* buffer, int size )
{
	RTTService*				pRTTService = NULL;

	for(ListClient::iterator iter = m_listClient.begin() ; iter != m_listClient.end() ; iter++ )
	{
		pRTTService = *iter;
		if(worldID == 0 || (short)worldID == pRTTService->m_worldID){	// ��ü ���� �̰ų� �ش� ���忡�� Broadcast
			pRTTService->SendPacket( buffer, size );

			OutputDebug( "[RTTService Broadcast info] %d, %d, %d, %d, %s\n",
				pRTTService->m_worldID, 
				pRTTService->m_channelID, 
				pRTTService->m_serverType, 
				pRTTService->m_serverID,
				pRTTService->GetIPAddress() );
		}
	}
}

/**
	Database�� ������ ���� ��� ��� ������ ������ �ؾ� �Ѵ�.
*/
void ManagerService::DisconnectAll()
{
	ListClient::iterator	iter;
	RTTService*				client = NULL;

	while(!m_listClient.empty())
	{
		iter = m_listClient.begin();
		client = *iter;

		m_listClient.erase( iter );
		client->SetKill();		
	}
}