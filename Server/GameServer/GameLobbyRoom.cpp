#include "StdAfx.h"
#include ".\gamelobbyroom.h"
#include "Player.h"
#include "ResrcManager.h"
#include "World/HighLoadBalancer.h"

__IMPL_CUSTOMPOOL_PTR(GameLobbyRoom)
GameLobbyRoom::GameLobbyRoom(void)
{
}

GameLobbyRoom::~GameLobbyRoom(void)
{
}

FIELDCODE tmpCode = 501;
WORD wmoCode = 501;
void GameLobbyRoom::OnCreate()
{
	ASSERT(m_pMAPINFO);
	FieldInfo * pFieldInfo = NULL;
	pFieldInfo = g_ResrcMgr.FindFieldInfo(tmpCode, wmoCode);

	GameField * pGameField = GameField::ALLOC();

	pGameField->Init( this, pFieldInfo, NULL, NULL, NULL, false, false, false );

	m_GameFieldHash.insert( _GAMEFIELD_HASH::value_type( pGameField->GetFieldCode(), pGameField ) );
}

VOID GameLobbyRoom::Ready( Player * pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector * pwvStartPos )
{
	pPlayer->SetGameZone( ePRS_BEFORE_ENTER_LOBBY, this, tmpCode, 0, NULL );
}

void GameLobbyRoom::SetZone( Object *pObject, MAPCODE FieldCode, WzID AreaID, WzVector * pwvStartPos )
{
	pObject->SetGameZone( ePRS_AT_LOBBY, this, tmpCode, 0, NULL );
}

BOOL GameLobbyRoom::Join( Player * pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector * pwvStartPos )
{
	if( !( pPlayer->GetGameZoneState() & ePRS_BEFORE_ENTER_LOBBY ) )
	{
		SUNLOG( eCRITICAL_LOG, "[SusunLove] GameLobbyRoom::Join eRoomState[%d] ZoneType[%d]", pPlayer->GetGameZoneState(), GetZoneType() );
		return FALSE;
	}

	SetZone( pPlayer, tmpCode, 0, pwvStartPos );

	if( !GameRoom::Join( pPlayer, tmpCode, 0, NULL ) )
	{
		pPlayer->SetGameZone( ePRS_NOT_AT_ZONE, NULL, 0, NULL );
		return FALSE;
	}

	return TRUE;
}
VOID GameLobbyRoom::Leave( Player * pPlayer )
{
	GameRoom::Leave( pPlayer );
}

// (f100623.1L) change an interface to support a multi-checksum checker
bool GameLobbyRoom::CheckMapVersion(FIELDCODE field_code,
                                    EngineFileChecksum* stored_checksums,
                                    const EngineFileChecksum& inputed_checksums) const
{
    return true;
}
/*
BOOL GameLobbyRoom::CheckMapVersion( MAPCODE FieldCode, DWORD checksum, DWORD * OUT pCurCheckSum )
{
	return TRUE;
}
*/

VOID GameLobbyRoom::Update( DWORD dwDeltaTick )
{
    // CHANGES: f110601.7L, it did fix an update problem that caused packet pendings.
    // the changes is f110531.4L set up the packet processing policy is
    // the high-load balanced network process basically. but GameLobbyRoom don't call GameRoom::Update
    // that call HighLoadBalancer::Update in GameField::Update.
    if (this->m_GameFieldHash.empty()) {
        return;
    };
    GameField* const game_field = this->m_GameFieldHash.begin()->second;
    if (game_field == NULL || game_field->IsReady() == false) {
        return;
    };
	HighLoadBalancer::Instance()->Update(game_field, true);
}