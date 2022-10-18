#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
#include <PacketStruct_AG.h>
#include <PacketStruct_AW.h>

//////////////////////////////////////////////////////////////////////////
#include <UserSessions/User.h>
#include "Village.h"

#include <AgentParty/AgentPartyManager.h>

_IMPL_POOL(Village)

Village::Village(void)
{
}

Village::~Village(void)
{
}
VOID Village::Create( KEYTYPE key, MAPCODE MapCode, ServerSession * pServerSession )
{
	SetKey( key );
	SetMapCode( MapCode );
	SetLinkedServer( pServerSession );

	On_create();
}

VOID Village::On_create()
{
	MSG_AG_ZONE_VILLAGE_CREATE_CMD msg;
	msg.m_dwKey		= 0;
	msg.m_MapCode	= GetMapCode();
	msg.m_RoomKey	= GetKey();
	SendToLinkedServer( &msg, sizeof(msg) );
}

VOID Village::On_destroy()
{
}

VOID Village::On_readyuser( User * pUser )
{
	pUser->SetZoneState( ePRS_BEFORE_ENTER_VILLAGE );
	pUser->SetZoneKey( GetKey() );
	pUser->SetZoneType( eZONETYPE_VILLAGE );
}

VOID Village::On_enteruser( User * pUser )
{
	ZoneInterface::On_enteruser( pUser );

	pUser->SetZoneState( ePRS_AT_VILLAGE );
	pUser->SetSelectedCharMapCode( GetMapCode() );

	//pUser->SetSelectedCharLastVillageMapCode( GetMapCode() );

	// 방에 들어올때 파티관련 처리를 한다.
	AgentPartyManager::Instance()->EnterZone( pUser, this );
}

VOID Village::On_leaveuser( User * pUser )
{
    if (pUser->GetPartyState().IsPartyMember() == true)
    {
        AgentPartyManager::Instance()->LeaveZone( pUser, this );
    }
}













