#include "StdAfx.h"
#include "ChatRoomManager.h"
#include "UserManager.h"
#include "User.h"
#include "BaseZone.h"
#include <assert.h>

// 채널 채팅 때문
#include <PacketStruct_CW.h>
#include <ResultCode.h>

ChatRoomManager::ChatRoomManager(void)
{
	m_pChatRoomPool = NULL;
}

ChatRoomManager::~ChatRoomManager(void)
{
	assert( m_pChatRoomPool == NULL );
}

VOID ChatRoomManager::Init()
{
	m_pChatRoomPool = new util::CMemoryPoolFactory<CommChatZone>;
	//m_pChatRoomPool->Initialize( 10000, 5000 );
	m_pChatRoomPool->Initialize( 50, 50 );
}

VOID ChatRoomManager::Release()
{	
	CHATROOM_MAP_ITER	it;
	CommChatZone		*pChatRoom;
	CHATROOM_MAP& mapClone = m_mapChatRoom;

	for( it = mapClone.begin(); it != mapClone.end(); ++it )
	{
		pChatRoom = it->second;
		FreeChatRoom( pChatRoom );
	}
	mapClone.clear();

	m_pChatRoomPool->Release();
	SAFE_DELETE(m_pChatRoomPool);
}

VOID ChatRoomManager::Update()
{
}

CommChatZone* ChatRoomManager::AllocChatRoom()
{
	CommChatZone *pChatRoom = (CommChatZone *)m_pChatRoomPool->Alloc();
	pChatRoom->Init();
	return pChatRoom;
}

VOID ChatRoomManager::FreeChatRoom( CommChatZone *pChatRoom )
{
	pChatRoom->Release();
	m_pChatRoomPool->Free( pChatRoom );
}

CommChatZone *ChatRoomManager::AddChatRoom( TCHAR *ptszChatRoomName )
{
	CHATROOM_MAP_ITER it = m_mapChatRoom.find( ptszChatRoomName );

	if( it != m_mapChatRoom.end() )
		return NULL;

	CommChatZone *pChatRoom = AllocChatRoom();
//	pChatRoom->SetChannelID( dwChannelID );
	pChatRoom->SetChatRoomName( ptszChatRoomName );
	m_mapChatRoom.insert( CHATROOM_MAP_PAIR( ptszChatRoomName, pChatRoom ) );
	MessageOut( eFULL_LOG, "Add ChatRoom(%s)", ptszChatRoomName );

	return pChatRoom;
}

VOID ChatRoomManager::RemoveChatRoom( TCHAR *ptszChatRoomName )
{
	CHATROOM_MAP_ITER it = m_mapChatRoom.find( ptszChatRoomName );
	CommChatZone *pChatRoom;
	if( it != m_mapChatRoom.end() )
	{
		pChatRoom = it->second;
		m_mapChatRoom.erase( it );
		FreeChatRoom( pChatRoom );
		MessageOut( eFULL_LOG,   "Remove ChatRoom(%s)", ptszChatRoomName );
	}
}

CommChatZone* ChatRoomManager::GetChatRoom( TCHAR *ptszChatRoomName )
{
	CHATROOM_MAP_ITER it = m_mapChatRoom.find( ptszChatRoomName );
	if( it == m_mapChatRoom.end() )
	{
		return NULL;
	}
	else
	{
		return it->second;
	}
}

// 채팅방 참여
VOID ChatRoomManager::AddUser( User *pUser, BYTE byRoomNameLength, TCHAR* ptszChatRoomName )
{
	MSG_CW_CHAT_CHANNEL_JOIN_ACK	ackMsg;
	MSG_CW_CHAT_CHANNEL_JOIN_BRD	brdMsg;

	// 이미 들어가 있는 채팅방이 있으면 오류
	if( pUser->GetChannelChatRoom() )
	{
		MessageOut( eCRITICAL_LOG, "ChatRoomManager AddUser Error - Already Join ChatRoom[CharGuid:%u]", pUser->GetCharGuid() );

		MSG_CW_CHAT_CHANNEL_JOIN_NAK nakMsg;

		nakMsg.m_byErrorCode = RC::RC_CHAT_ALREADYINROOM;

		pUser->SendPacket( (BYTE*)&nakMsg, sizeof(nakMsg) );
		return;
	}

	// 혹시나 패킷이 잘못되었을 경우를 대비해 최대 길이를 보장한다
	TCHAR ptszRoomName[MAX_ROOMTITLE_LENGTH+1]={0,};

	_tcsncpy( ptszRoomName, ptszChatRoomName, byRoomNameLength );

	// 채팅방 있는가?
	ackMsg.m_bCreated = FALSE;
	CommChatZone *pChatZone = GetChatRoom( ptszRoomName );
	if( !pChatZone )
	{
		// 없으면 새로 개설
		pChatZone = AddChatRoom( ptszRoomName );
		ackMsg.m_bCreated = TRUE;
	}

	// 인원 다 차있으면 못 들어간다
	if( pChatZone->GetNumberOfUsers() >= MAX_CHATZONE_USER )
	{
		MSG_CW_CHAT_CHANNEL_JOIN_NAK	nakMsg;

		nakMsg.m_byErrorCode = RC::RC_CHAT_UNKNOWN;
		pUser->SendPacket( (BYTE*)&nakMsg, sizeof(nakMsg) );

		MessageOut( eFULL_LOG, "ChannelChatZone Join Failed - User Full[%u]", pUser->GetCharGuid() );
		return;
	}

	// 기존 멤버들에게 신규유저 들어온다고 알려주고
	brdMsg.m_byChannelID = static_cast<BYTE>( pUser->GetChannelID() );
	_tcsncpy( brdMsg.m_ptszJoinMember, pUser->GetCharName().c_str(), MAX_CHARNAME_LENGTH );
	brdMsg.m_ptszJoinMember[MAX_CHARNAME_LENGTH] = '\0';

    DWORD number_of_users = pChatZone->GetNumberOfUsers();
    ASSERT(SAFE_NUMERIC_TYPECAST(DWORD, number_of_users, BYTE));
    brdMsg.m_byUserNum = (BYTE)number_of_users + 1; // (NOTE) (WAVERIX) +1이 뭔지...몰겠당.

	pChatZone->Send( (BYTE*)&brdMsg, sizeof(brdMsg) );

	// 들어간다
	if( !pChatZone->AddUser( pUser->GetGUID(), pUser ) )
	{
		MessageOut( eCRITICAL_LOG, "CHAT_CHANNEL_JOIN_SYN Error - AddUser Failed[CharGuid:%u]", pUser->GetCharGuid() );

		MSG_CW_CHAT_CHANNEL_JOIN_NAK nakMsg;

		nakMsg.m_byErrorCode = RC::RC_CHAT_UNKNOWN;

		pUser->SendPacket( (BYTE*)&nakMsg, sizeof(nakMsg) );

		// 들어오지 않았다는 걸 확인시켜준다
		MSG_CW_CHAT_CHANNEL_OUT_BRD	outMsg;

		_tcsncpy( outMsg.m_ptszOutName, brdMsg.m_ptszJoinMember, MAX_CHARNAME_LENGTH );
		outMsg.m_ptszOutName[MAX_CHARNAME_LENGTH] = '\0';
		outMsg.m_byUserNum = (BYTE)number_of_users;

		pChatZone->Send( (BYTE*)&outMsg, sizeof(outMsg) );
		return;
	}

	pUser->JoinChannelChatRoom( pChatZone );

    {
        DWORD number_of_users = pChatZone->GetNumberOfUsers();
        ASSERT(SAFE_NUMERIC_TYPECAST(DWORD, number_of_users, BYTE));
	    ackMsg.m_byUserNum = (BYTE)number_of_users;
    };
	pUser->SendPacket( (BYTE*)&ackMsg, sizeof(ackMsg) );
}

// 채팅방 나감
VOID ChatRoomManager::RemoveUser( User *pUser )
{
	MSG_CW_CHAT_CHANNEL_OUT_ACK	ackMsg;
	MSG_CW_CHAT_CHANNEL_OUT_NAK	nakMsg;
	MSG_CW_CHAT_CHANNEL_OUT_BRD	brdMsg;

	// 채팅방이 있는가?
	CommChatZone* pChatZone = pUser->GetChannelChatRoom();

	if( !pChatZone )
	{
		MessageOut( eFULL_LOG, "CHAT_CHANNEL_OUT Error - No ChatRoom[CharGuid:%u]", pUser->GetCharGuid() );
		nakMsg.m_byErrorCode = RC::RC_CHAT_NOROOM;

		pUser->SendPacket( (BYTE*)&nakMsg, sizeof(nakMsg) );
		return;
	}

	// 퇴장한다
	pChatZone->RemoveUser( pUser->GetGUID() );
	pUser->OutChannelChatRoom();

	pUser->SendPacket( (BYTE*)&ackMsg, sizeof(ackMsg) );

	if( !pChatZone->GetNumberOfUsers() )
	{
		RemoveChatRoom( pChatZone->GetChatRoomName() );
	}
	else
	{
		// 참여자들에게 알려준다
        {
            DWORD number_of_users = pChatZone->GetNumberOfUsers();
            ASSERT(SAFE_NUMERIC_TYPECAST(DWORD, number_of_users, BYTE));
		    brdMsg.m_byUserNum = (BYTE)number_of_users;
        };
		_tcsncpy( brdMsg.m_ptszOutName, pUser->GetCharName().c_str(), MAX_CHARNAME_LENGTH );
		brdMsg.m_ptszOutName[MAX_CHARNAME_LENGTH] = '\0';

		pChatZone->Send( (BYTE*)&brdMsg, sizeof(brdMsg) );
	}
}

VOID ChatRoomManager::DisplayChatRoomInfo()
{
	CHATROOM_MAP_ITER	it;

	if( m_mapChatRoom.empty() )
	{
		MessageOut(eCRITICAL_LOG,   "No Channel ChatRoom." );
		return;
	}

	MessageOut( eCRITICAL_LOG, "ChannelChat Room Number:[%u]", m_mapChatRoom.size() );
}
