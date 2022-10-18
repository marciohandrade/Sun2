#include "stdafx.h"
#include "DropStatePoll.h"

class DropStateInfo
{
public:
			wchar_t		userID[ID_MAX_LEN + 1];
			DWORD		key;
};

DropStatePoll::DropStatePoll()
{
}

DropStatePoll::~DropStatePoll()
{
}

void DropStatePoll::Destroy()
{
	ListStateInfo::iterator		iter;
	DropStateInfo*				info = NULL;

	while( m_listState.size() > 0 )
	{
		info = m_listState.front();
		m_listState.pop_front();

		delete info;
	}
}

bool DropStatePoll::IsProcessing( wchar_t* userID )
{
	DropStateInfo*				info = NULL;

	for(ListStateInfo::iterator iter = m_listState.begin() ; iter != m_listState.end() ; iter++ )
	{
		info = *iter;

		if( wcsncmp( info->userID, userID, ID_MAX_LEN ) == 0 )
			return true;
	}

	return false;
}

void DropStatePoll::Add( wchar_t* userID, DWORD key )
{
	DropStateInfo*	info = new DropStateInfo;

	info->key = key;
	wcsncpy( info->userID, userID, ID_MAX_LEN );
    info->userID[ID_MAX_LEN] = NULL;

	m_listState.push_back( info );
}

DWORD DropStatePoll::Remove( wchar_t* userID )
{
	DropStateInfo*				info = NULL;
	DWORD						key = 0;

	for(ListStateInfo::iterator iter = m_listState.begin() ; iter != m_listState.end() ; iter++ )
	{
		info = *iter;

		if( wcsncmp( info->userID, userID, ID_MAX_LEN ) == 0 )
		{
			key = info->key;

			delete info;
			m_listState.erase( iter );

			break;
		}
	}

	return key;
}