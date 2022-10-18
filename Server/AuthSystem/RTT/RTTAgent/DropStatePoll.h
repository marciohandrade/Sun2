#pragma once

#pragma warning(push)
#pragma warning(disable:4702)

#include <list>

#pragma warning(pop)

class DropStateInfo;
class DropStatePoll
{
	Singleton( DropStatePoll );

public:
			void			Destroy();

			bool			IsProcessing( wchar_t* userID );
			void			Add( wchar_t* userID, DWORD key );
			DWORD			Remove( wchar_t* userID );

private:
	typedef	std::list	< DropStateInfo* >		ListStateInfo;

			ListStateInfo	m_listState;
};