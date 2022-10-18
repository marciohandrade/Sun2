#include "StdAfx.h"
#include ".\gamepvproom.h"
#include ".\Player.h"
#include ".\Map.h"
#include ".\GameField.h"
#include ".\PVPManager.h"
#include "PVPInfo.h"

//_NA_0_20091118_GAMEROOM_BATTLE_RESULT
#include "PVPBattleResult.h"

__IMPL_CUSTOMPOOL_PTR(GamePVPRoom)
GamePVPRoom::GamePVPRoom(void)
{
	m_pPVPManager = new PVPManager;

    //_NA_0_20091118_GAMEROOM_BATTLE_RESULT
    m_pBattleResult = new PVPBattleResult;
}

GamePVPRoom::~GamePVPRoom(void)
{
    // _NA_0_20091118_GAMEROOM_BATTLE_RESULT
    SAFE_DELETE(m_pBattleResult);
    SAFE_DELETE(m_pPVPManager);
}

void 
GamePVPRoom::OnCreate()
{
	ChangeHandler( eZONETYPE_PVP );
	GameRoom::OnCreate();

    // (CHANGES) (f100409.2L) an argument of the GamePvPRoom is \
    // added by _NA_0_20091118_GAMEROOM_BATTLE_RESULT
    m_pPVPManager->Init( &m_AdditionalPVPInfo, this );

    m_pBattleResult->Init(true);
}

void 
GamePVPRoom::OnDestroy()
{
    //_NA_0_20091118_GAMEROOM_BATTLE_RESULT
    m_pBattleResult->Release();

	GameRoom::OnDestroy();
	m_pPVPManager->Release();
}

void 
GamePVPRoom::Ready(Player* player, MAPCODE field_code, WzID area_id, WzVector* start_pos)
{
	player->SetGameZone(ePRS_BEFORE_ENTER_PVP, this, field_code, area_id, start_pos);
}

void 
GamePVPRoom::SetZone(Object *pObject, MAPCODE field_code, WzID area_id, WzVector* start_pos)
{
	pObject->SetGameZone(ePRS_AT_PVP, this, field_code, area_id, start_pos);
}

BOOL 
GamePVPRoom::Join(Player* player, MAPCODE field_code, WzID area_id, WzVector* start_pos)
{
	if (!( player->GetGameZoneState() & ePRS_BEFORE_ENTER_PVP))
	{
		SUNLOG(
            eCRITICAL_LOG, 
            __FUNCTION__" : Can not join room : player name : %s, room state : %d, ZoneType : %d", 
            player->GetCharName(),
            player->GetGameZoneState(), 
            GetZoneType() 
            );
		return FALSE;
	}

	SetZone(player, field_code, area_id, start_pos);

	if (!GameRoom::Join(player, field_code, area_id, start_pos))
	{
		player->SetGameZone( ePRS_NOT_AT_ZONE, NULL, 0 );
		return FALSE;
	}

	m_pPVPManager->EnterPlayer( player );

	return TRUE;
}

// 필드간 이동
BOOL 
GamePVPRoom::MoveField(Player* player, MAPCODE field_code, WzID area_id, WzVector* start_pos)
{
	SetZone(player, field_code, area_id, start_pos);

	if (!GameRoom::MoveField(player, field_code, area_id, start_pos))
	{
		player->SetGameZone(ePRS_NOT_AT_ZONE, NULL, 0);
		return FALSE;
	}

	m_pPVPManager->EnterPlayer( player );

	return TRUE;
}


void
GamePVPRoom::Leave( Player* player )
{
	m_pPVPManager->LeavePlayer( player );

	GameRoom::Leave( player );
}
//
//MAPCODE GamePVPRoom::GetEntryFieldCode()
//{
//	ASSERT( GetMap() );
//	ASSERT( GetMap()->GetMapInfo() );
//	return GetMap()->GetMapInfo()->GetMapInfo()->FCode[0];
//}

void
GamePVPRoom::Update( DWORD dwDeltaTick )
{
	GameRoom::Update( dwDeltaTick );

	m_pPVPManager->Update( dwDeltaTick );
}

eUSER_RELATION_TYPE	
GamePVPRoom::IsFriendPVP( Player*pAttacker, Player*pTarget )	
{ 
	if( ePVP_USER_MATCH == pAttacker->GetPVPInfo().GetPVPState() && 
		pAttacker->GetPartyState() == pTarget->GetPartyState() )
	{
		return USER_RELATION_FRIEND;
	}

	return USER_RELATION_ENEMY; 
}

eZONETYPE 
GamePVPRoom::GetZoneType() 
{ 
    return eZONETYPE_PVP;	
}

void 
GamePVPRoom::ChangeMaster(DWORD dwNextMasterUserKey ) 
{ 
    SetMasterUserKey( dwNextMasterUserKey ); 
}

const ADDITIONAL_PVPINFO* 
GamePVPRoom::GetAdditionalPVPInfo()
{ 
    return &m_AdditionalPVPInfo; 
}

BOOL 
GamePVPRoom::IsApplyPenalty()
{ 
    return FALSE; 
}

//_NA_0_20091118_GAMEROOM_BATTLE_RESULT
GameBattleResult* 
GamePVPRoom::GetBattleResult() 
{ 
    return m_pBattleResult; 
}
