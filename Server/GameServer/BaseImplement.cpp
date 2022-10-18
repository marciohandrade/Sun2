#include "StdAfx.h"
#include ".\Player.h"
#include ".\BaseImplement.h"


BaseImplement::BaseImplement( void ) :
m_pPlayer(NULL),
player_(NULL),
receive_protocol_(0)
{
}

BaseImplement::~BaseImplement( void )
{
}

VOID BaseImplement::DoMessageImplement( MSG_BASE* pRecvMessage )
{
    if( IsException( pRecvMessage ) )
        return;

    DWORD dwResult = 0;
    receive_protocol_ = pRecvMessage->m_byProtocol;
    if( OnCheck( pRecvMessage, dwResult ) )
        OnImplement( pRecvMessage );
    else
        OnFailed( dwResult );
}

BOOLEAN BaseImplement::IsException( MSG_BASE* pRecvMessage )
{
    ASSERT( pRecvMessage );
    if( !pRecvMessage )
    {
        SUNLOG( eCRITICAL_LOG, "BaseImplement::IsException() Received Message is NULL" );
        return TRUE;
    }

    Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMessage->m_dwKey );
    ASSERT( pPlayer );
    if( !pPlayer )
    {
        SUNLOG( eCRITICAL_LOG, "BaseImplement::IsException() Can't Find Player [UserKey:%d]", pRecvMessage->m_dwKey );
        return TRUE;
    }

    m_pPlayer = pPlayer;
    player_ = pPlayer;

    return FALSE;
}
