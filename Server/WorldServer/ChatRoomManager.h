#pragma once

#include <Singleton.h>
#include <string>
#include <map>
#include <MemoryPoolFactory.h>

class User;
class CommChatZone;

typedef STLX_MAP<std::string, CommChatZone*>			CHATROOM_MAP;
typedef CHATROOM_MAP::iterator					CHATROOM_MAP_ITER;
typedef std::pair<std::string, CommChatZone*>	CHATROOM_MAP_PAIR;

class ChatRoomManager : public util::Singleton<ChatRoomManager>
{
public:
	ChatRoomManager(void);
	~ChatRoomManager(void);

	VOID							Init();
	VOID							Release();
	VOID							Update();

	CommChatZone*					AllocChatRoom();
	VOID							FreeChatRoom( CommChatZone *pChatRoom );
	CommChatZone*					AddChatRoom( TCHAR *ptszChatRoomName );
	VOID							RemoveChatRoom( TCHAR *ptszChatRoomName );
	CommChatZone*					GetChatRoom( TCHAR *ptszChatRoomName );

	VOID							AddUser( User *pUser, BYTE byRoomNameLength, TCHAR* ptszChatRoomName );
	VOID							RemoveUser( User *pUser );

	VOID							DisplayChatRoomInfo();

private:
	CHATROOM_MAP						m_mapChatRoom;
	util::CMemoryPoolFactory<CommChatZone>	*m_pChatRoomPool;
};
