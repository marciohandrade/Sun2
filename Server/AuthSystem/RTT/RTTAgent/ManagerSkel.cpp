#include "stdafx.h"
#include "ManagerSkel.h"

ManagerSkel::ManagerSkel()
{
}

ManagerSkel::~ManagerSkel()
{
}

bool ManagerSkel::AddClient( Skel* client )
{
	m_listClient.push_back( client );

	return true;
}

void ManagerSkel::RemoveClient( Skel* client )
{
	for(ListClient::iterator iter = m_listClient.begin() ; iter != m_listClient.end() ; iter++ )
	{
		if( *iter == client )
		{
			m_listClient.erase( iter );
			break;
		}
	}
}

void ManagerSkel::Clear()
{
	m_listClient.clear();
}

void ManagerSkel::MakeClientList( Skel* list[], DWORD bufferSize, DWORD* listCount )
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

Skel* ManagerSkel::FindClient( DWORD type )
{
	Skel*					client = NULL;;

	for(ListClient::iterator iter = m_listClient.begin() ; iter != m_listClient.end() ; iter++ )
	{
		client = *iter;

		if( ( client->GetType() & type ) == type )
			return client;
	}

	return NULL;
}

void ManagerSkel::Broadcast( char* buffer, int size )
{
	Skel*					client = NULL;;

	for(ListClient::iterator iter = m_listClient.begin() ; iter != m_listClient.end() ; iter++ )
	{
		client = *iter;
		client->SendPacket( buffer, size );
	}
}

void ManagerSkel::DisconnectAll()
{
	ListClient::iterator	iter;
	Skel*					client = NULL;

	while(!m_listClient.empty())
	{
		iter = m_listClient.begin();
		client = *iter;
		
		m_listClient.erase( iter );
		client->SetKill();
	}
}