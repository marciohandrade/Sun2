#include "stdafx.h"
#include "ManagerWeb.h"

ManagerWeb::ManagerWeb()
{
}

ManagerWeb::~ManagerWeb()
{
}

bool ManagerWeb::AddClient( Web* client )
{
	m_listClient.push_back( client );

#ifdef _DEBUG
	OutputDebug("[ManagerWeb::AddClient] %s(%d) \n", client->GetIPAddress(), m_listClient.size());
#endif
	return true;
}

void ManagerWeb::RemoveClient( Web* client )
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
	OutputDebug("[ManagerWeb::RemoveClient] %s(%d) \n", client->GetIPAddress(), m_listClient.size());
#endif
}


/**
	Database와 연결이 끊긴 경우 모든 소켓을 정리를 해야 한다.
*/
void ManagerWeb::DisconnectAll()
{
	ListClient::iterator	iter;
	Web*				client = NULL;

	while(!m_listClient.empty())
	{
		iter = m_listClient.begin();
		client = *iter;

		m_listClient.erase( iter );
		client->SetKill();		
	}
}