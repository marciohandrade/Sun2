#include "stdafx.h"
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
#include <PacketStruct_AG.h>

#include "SpaField.h"
#include <UserSessions/User.h>
#include <AgentParty/AgentPartyManager.h>

VOID SpaField::ReadyUser( User * pUser, eSEND_TYPE type, MAPCODE FieldCode /*= 0*/, DWORD dwAreaID /*= 0*/, 
                         WzVector * pwvStartPos /*= NULL*/, bool bFirstJoin /*= false */ )
{
    if (!dwAreaID)
    {
        const sMAPINFO* psMAPINFO = GetMapInfo();

        // 실제 시작 위치는 GameServer에서 결정된다.
        dwAreaID = psMAPINFO->wStartAreaID;
    }
    ZoneInterface::ReadyUser( pUser, type, FieldCode, dwAreaID, pwvStartPos, bFirstJoin );
}

VOID SpaField::Create( KEYTYPE key, MAPCODE MapCode, ServerSession* pServerSession )
{
    SetKey(key);
    SetMapCode(MapCode);
    SetLinkedServer(pServerSession);

    On_create();
}

void SpaField::On_create()
{
    MSG_AG_ZONE_SPAFIELD_CREATE_CMD msg;
    msg.m_dwKey		= 0;
    msg.map_code	= GetMapCode();
    msg.room_key	= GetKey();
    SendToLinkedServer( &msg, sizeof(msg) );
}

void SpaField::On_readyuser( User* user )
{
    user->SetZoneState( ePRS_BEFORE_ENTER_SPA_FIELD );
    user->SetZoneKey( GetKey() );
    user->SetZoneType( eZONETYPE_SPA_FIELD);
}

void SpaField::On_enteruser( User* user )
{
    ZoneInterface::On_enteruser( user );

    user->SetZoneState( ePRS_AT_SPA_FIELD );
    user->SetSelectedCharMapCode( GetMapCode() );

    // 방에 들어올때 파티관련 처리를 한다.
    AgentPartyManager::Instance()->EnterZone( user, this );
}

void SpaField::On_leaveuser( User* user )
{
    if (user->GetPartyState().IsPartyMember() == true)
    {
        AgentPartyManager::Instance()->LeaveZone( user, this );
    }
}

#endif //_NA_008069_20150210_ADD_SPA_SYSTEM

