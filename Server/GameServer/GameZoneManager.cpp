#include "StdAfx.h"
#include ".\gamezonemanager.h"
#include ".\GameVillage.h"
#include ".\GameHuntingRoom.h"
#include ".\GameLobbyRoom.h"
#include ".\GameMissionRoom.h"
#include ".\GamePVPRoom.h"
#include ".\GameCHuntingRoom.h"
#include ".\GameHuntingField.h"
#include ".\GameInstanceDungeon.h"
#include ".\GameDominationField.h"
#include "Map.h"
#include <functional_custom.h>
#include "Player.h"

//_NA_0_20091118_GAMEROOM_BATTLE_RESULT
#include "GameBattleResult.h"
#ifdef _NA_001990_ACSYSTEM_ADD
#include "AchievementManager.h"
#endif //_NA_001990_ACSYSTEM_ADD

#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
#include "GameSpaField.h"
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
#include "GameZone.h"

#ifdef _NA_008334_20150608_SONNENSCHEIN
#include "GameGuild.h"
#endif //_NA_008334_20150608_SONNENSCHEIN

namespace
{
	__inline GameZone *	Alloc( eZONETYPE zoneType )
	{
		GameZone * pZone = NULL;
		switch( zoneType )
		{
		case eZONETYPE_VILLAGE:
			pZone = GameVillage::ALLOC(); break;
		case eZONETYPE_LOBBY:
			pZone = GameLobbyRoom::ALLOC(); break;
		case eZONETYPE_HUNTING:
			pZone = GameHuntingRoom::ALLOC(); break;
		case eZONETYPE_MISSION:
			pZone = GameMissionRoom::ALLOC(); break;
		case eZONETYPE_PVP:
			pZone = GamePVPRoom::ALLOC(); break;
		case eZONETYPE_CHUNTING:
			pZone = GameCHuntingRoom::ALLOC(); break;
		case eZONETYPE_FIELD:
			pZone = GameHuntingField::ALLOC(); break;
        case eZONETYPE_DOMINATION_FIELD:
            pZone = GameDominationField::ALLOC(); break;
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
        case eZONETYPE_SPA_FIELD:
            pZone = GameSpaField::ALLOC(); break;
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
		case eZONETYPE_INSTANCE:
			pZone = GameInstanceDungeon::ALLOC(); break;
		}   

		return pZone;
	}
	__inline void Free( GameZone * pZone )
	{
		ASSERT( pZone );
		switch( pZone->GetZoneType() )
		{
		case eZONETYPE_VILLAGE:
			GameVillage::FREE((GameVillage*)pZone); return ;
		case eZONETYPE_LOBBY:
			GameLobbyRoom::FREE((GameLobbyRoom*)pZone ); return ;
		case eZONETYPE_HUNTING:
			GameHuntingRoom::FREE((GameHuntingRoom*)pZone); return ;
		case eZONETYPE_MISSION:
			GameMissionRoom::FREE((GameMissionRoom*)pZone); return ;
		case eZONETYPE_PVP:
			GamePVPRoom::FREE((GamePVPRoom*)pZone); return ;
		case eZONETYPE_CHUNTING:
			GameCHuntingRoom::FREE((GameCHuntingRoom*)pZone); return;
		case eZONETYPE_FIELD:
			GameHuntingField::FREE((GameHuntingField*)pZone); return ;
        case eZONETYPE_DOMINATION_FIELD:
            GameDominationField::FREE((GameDominationField*)pZone); return ;
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
        case eZONETYPE_SPA_FIELD:
            GameSpaField::FREE((GameSpaField*)pZone); return;
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
		case eZONETYPE_INSTANCE:
			GameInstanceDungeon::FREE((GameInstanceDungeon*)pZone); return ;
		}
	}
}

class GameZoneManager::ZonePool
{
public:
	util::CMemoryPoolFactory<GameVillage>			m_pool1;
	util::CMemoryPoolFactory<GameLobbyRoom>		m_pool2;
	util::CMemoryPoolFactory<GameHuntingRoom>		m_pool3;
	util::CMemoryPoolFactory<GameMissionRoom>		m_pool4;
	util::CMemoryPoolFactory<GamePVPRoom>			m_pool5;
	util::CMemoryPoolFactory<GameCHuntingRoom>	m_pool6;
	util::CMemoryPoolFactory<GameHuntingField>	m_pool7;
	util::CMemoryPoolFactory<GameInstanceDungeon>	m_pool8;
    util::CMemoryPoolFactory<GameDominationField>	m_pool9;
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    util::CMemoryPoolFactory<GameSpaField>      m_pool10;
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM

	explicit ZonePool(){}
    void SetPool(DWORD poolSize)
    {
        __UNUSED(poolSize);
        m_pool1.Initialize( 0,  5, "GameVillage");
        m_pool2.Initialize( 5,  5, "GameLobbyRoom");
        m_pool3.Initialize( 0,  1, "GameHuntingRoom"); // not used
        m_pool4.Initialize( 0, 20, "GameMissionRoom");
        m_pool5.Initialize( 0,  5, "GamePVPRoom");
        m_pool6.Initialize( 0,  5, "GameCHuntingRoom");
        m_pool7.Initialize(40,  5, "GameHuntingField");
        m_pool8.Initialize( 0, 10, "GameInstanceDungeon");
        m_pool9.Initialize( 0,  5, "GameDominationField");
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
        m_pool10.Initialize(0, 1, "GameSpaField");
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
    }
	~ZonePool(){}
};

GameZoneManager::ZonePool GameZoneManager::s_pool;
void GameZoneManager::ReservedZonePool( DWORD poolSize )
{
	s_pool.SetPool(poolSize);

	GameVillage::SetPool(&s_pool.m_pool1);
	GameLobbyRoom::SetPool(&s_pool.m_pool2);
	GameHuntingRoom::SetPool(&s_pool.m_pool3);
	GameMissionRoom::SetPool(&s_pool.m_pool4);
	GamePVPRoom::SetPool(&s_pool.m_pool5);
	GameCHuntingRoom::SetPool(&s_pool.m_pool6);
	GameHuntingField::SetPool(&s_pool.m_pool7);
	GameInstanceDungeon::SetPool(&s_pool.m_pool8);
    GameDominationField::SetPool(&s_pool.m_pool9);
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    GameSpaField::SetPool(&s_pool.m_pool10);
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
}


//---------------------------------------------------------------------------------------
// 구현부
//---------------------------------------------------------------------------------------

GameZoneManager g_GameZoneManager;
GameZoneManager::GameZoneManager(void)
{
	m_pZoneHash = new GAMEZONE_HASH;
}

GameZoneManager::~GameZoneManager(void)
{
	Release();
	SAFE_DELETE(m_pZoneHash);
}

VOID GameZoneManager::Release()
{
	DestroyZoneAll();
}
VOID GameZoneManager::DestroyZoneAll()
{
	if( m_pZoneHash )
	{	
		GAMEZONE_HASH::iterator it = m_pZoneHash->begin(), end = m_pZoneHash->end();
		while( it != end )
		{
			_DestroyZone(it++);
		}
		m_pZoneHash->clear();
	}
}

VOID GameZoneManager::_AddZone( GameZone * pZone )
{
	GAMEZONE_HASH::iterator it = m_pZoneHash->find( pZone->GetKey() );
	if( it == m_pZoneHash->end() )
	{
		m_pZoneHash->insert( GAMEZONE_PAIR(pZone->GetKey(), pZone) );
		return;
	}
	ASSERT( !"이미 같은 키의 존이 존재한다:_AddZone()" && pZone->GetKey() );
}
VOID GameZoneManager::_RemoveZone( ZONEKEY Key )
{
	GAMEZONE_HASH::iterator it = m_pZoneHash->find( Key );
	if( it != m_pZoneHash->end() )
	{
		m_pZoneHash->erase( it );
		return;
	}
	ASSERT( !"특정 키의 존이 존재하지 않는다:_RemoveZone()" && Key );
}

GameZone * GameZoneManager::GetZone( ZONEKEY Key )
{
	GAMEZONE_HASH::iterator it = m_pZoneHash->find( Key );
	if( it != m_pZoneHash->end() )
	{
		return (it->second);
	}

	ASSERT( !"특정 키의 존이 존재하지 않는다:GetZone()" && Key );
	return NULL;
}

VOID GameZoneManager::DestroyZone( ZONEKEY Key )
{
	GAMEZONE_HASH::iterator it = m_pZoneHash->find(Key);
	_DestroyZone( it );
}

VOID GameZoneManager::_DestroyZone(  GAMEZONE_HASH::iterator it )
{
	if( it == m_pZoneHash->end() )
	{
		SUNLOG( eCRITICAL_LOG, "[GameZoneManager::DestroyZone] Zone is NULL");
		return;
	}

	GameZone *pZone = it->second;

	ZONEPOOL_HASH::iterator it2 = m_ZonePoolHash.find(pZone);
	if( it2 == m_ZonePoolHash.end() )
	{
		SUNLOG( eCRITICAL_LOG, "[SusunLove] GameZoneManager::_DestroyZone ZoneType[%d]", pZone->GetZoneType() );
		return;
	}
	else
	{
		m_ZonePoolHash.erase(it2);
	}

	pZone->Release();
	m_pZoneHash->erase(it);

	Free( pZone );
}

void GameZoneManager::Update(DWORD delta_tick)
{
    GAMEZONE_HASH::const_iterator it = m_pZoneHash->begin(), end = m_pZoneHash->end();
    for (; it != end; ++it)
    {
        GameZone* game_zone = it->second;
        game_zone->Update(delta_tick);
    }
}

VOID GameZoneManager::ReadyPlayer( ZONEKEY Key, Player * pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector * pwvStartPos )
{
	GameZone * pZone = GetZone( Key );
	if(!pZone) return;
	pZone->Ready( pPlayer, FieldCode, AreaID, pwvStartPos );
}

BOOL GameZoneManager::JoinPlayer( ZONEKEY Key, Player * pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector * pwvStartPos )
{
	GameZone * pZone = GetZone( Key );
	if(!pZone) return FALSE;

	//	(WAVERIX)(080604)(BUG-FIX) - 처리 순서 변경( 후처리->전처리 )
	//	죽어서 존에 들어오는 경우 살려준다.
	if( pPlayer->GetHP() == 0 )
		pPlayer->SetHP( 1 );

	if( !pZone->Join( pPlayer, FieldCode, AreaID, pwvStartPos ) )
		return FALSE;

    // CHANGES: f110103.2L
    if (ns_object::PlayerZoneTransaction* transaction_info = pPlayer->GetZoneTransactionInfo()) {
        transaction_info->EndTransaction();
    }

    //_NA_0_20091118_GAMEROOM_BATTLE_RESULT
    GameBattleResult* battle_result = pZone->GetGameBattleResult();
    if (battle_result)
    {
        battle_result->Enter(pPlayer);

#ifdef _NA_001990_ACSYSTEM_ADD
        if( AchievementManager* pACManager = pPlayer->GetACManager() )
        {
            pACManager->ProcessBattleEnter(pZone->GetMapCode());
        }
#endif //_NA_001990_ACSYSTEM_ADD
    }

    //_NA_003457_20111013_EXCEPT_DOMINATION_REWARD
    if (pZone->GetZoneType() == eZONETYPE_INSTANCE)
    {
        GameInstanceDungeon* const pIndun = static_cast<GameInstanceDungeon*>(pZone);
        if(pIndun->GetTypeOfIndun() == INDUN_KIND_SS_QUEST)
        {
#ifdef _NA_008334_20150608_SONNENSCHEIN
            GameGuild* player_guild = pPlayer->GetGuild();
            if (player_guild != NULL) {
                player_guild->ApplyGuildRewardOption(pPlayer, FALSE);   
            }
#else
            GameDominationManager::Instance()->ApplyAllRewardOption(pPlayer, FALSE);
#endif //_NA_008334_20150608_SONNENSCHEIN

            GameDominationManager::Instance()->SendAreaInfoToPlayer(pPlayer, FALSE);
        }
    }

    return TRUE;
}

VOID GameZoneManager::WarpPlayer( ZONEKEY Key, Player * pPlayer, MAPCODE FieldCode, WzID AreaID, WzVector * pwvStartPos )
{
	GameZone * pZone = GetZone( Key );
	if(!pZone) return;

	pZone->MoveField( pPlayer, FieldCode, AreaID, pwvStartPos );
}

VOID GameZoneManager::LeavePlayer( ZONEKEY Key, Player * pPlayer )
{
	GameZone * pZone = GetZone( Key );
	if(!pZone) return;

    //_NA_0_20091118_GAMEROOM_BATTLE_RESULT
    GameBattleResult* battle_result = pZone->GetGameBattleResult();
    if (battle_result)
    {
        battle_result->Leave(pPlayer, false);
    }

    //_NA_003457_20111013_EXCEPT_DOMINATION_REWARD
    if (pZone->GetZoneType() == eZONETYPE_INSTANCE)
    {
        GameInstanceDungeon* const pIndun = static_cast<GameInstanceDungeon*>(pZone);
        if(pIndun->GetTypeOfIndun() == INDUN_KIND_SS_QUEST)
        {
#ifdef _NA_008334_20150608_SONNENSCHEIN
            GameGuild* player_guild = pPlayer->GetGuild();
            if (player_guild != NULL) {
                player_guild->ApplyGuildRewardOption(pPlayer);
            }
#else
            GameDominationManager::Instance()->ApplyAllRewardOption(pPlayer, TRUE);
#endif //_NA_008334_20150608_SONNENSCHEIN

            GameDominationManager::Instance()->SendAreaInfoToPlayer(pPlayer, TRUE);
        }
    }

	pZone->Leave( pPlayer );
}

// (f100623.1L) change an interface to support a multi-checksum checker
bool GameZoneManager::FieldVersionCheck(ZONEKEY zone_key, FIELDCODE field_code,
                                        EngineFileChecksum* stored_checksums,
                                        const EngineFileChecksum& inputed_checksums) const
{
    GameZone* game_zone = const_cast<GameZoneManager*>(this)->GetZone(zone_key);
    if (game_zone == NULL) {
        return  false;
    }
    return game_zone->CheckMapVersion(field_code, stored_checksums, inputed_checksums);
}

/*
BOOL GameZoneManager::FieldVersionCheck( ZONEKEY Key, MAPCODE FieldCode, DWORD checksum, DWORD * OUT pCurCheckSum )
{
	GameZone * pZone = GetZone( Key );
	if(!pZone) return  FALSE;
	return pZone->CheckMapVersion( FieldCode, checksum, pCurCheckSum );
}
*/
GameZone*
GameZoneManager::CreateZone(eZONETYPE zoneType, ZONEKEY zoneKey, MAPCODE mapCode,
                            eZONE_RULESET::eRules rules)
{
    GameZone* pZone = Alloc(zoneType);
    if(NULL == pZone)
        return NULL;
    pZone->Init(zoneKey, mapCode, rules); //< Init() 안에서 로드된 리소스를 연결한다.
    _AddZone(pZone);

    m_ZonePoolHash[pZone] = pZone;

    return pZone;
}

GameZone * GameZoneManager::GetFieldVillageZone( MAPCODE MapCode )
{
	GAMEZONE_HASH::iterator it = m_pZoneHash->begin(), end = m_pZoneHash->end();
	for( ; it != end ; ++it )
	{
		GameZone *pZone = it->second;

		if( pZone->GetMapCode() != MapCode )	continue;
	
		if( pZone->GetZoneType() == eZONETYPE_VILLAGE ||
            pZone->GetZoneType() == eZONETYPE_DOMINATION_FIELD ||
            pZone->GetZoneType() == eZONETYPE_FIELD ||
            pZone->GetZoneType() == eZONETYPE_SPA_FIELD)    //_NA_008069_20150210_ADD_SPA_SYSTEM
            return pZone;
	}

	return NULL;
}

VOID GameZoneManager::Display( BOOL bLog )
{
	DWORD village = 0;
	DWORD lobby = 0;
	DWORD mission = 0;
	DWORD hunting = 0;
	DWORD pvp = 0;
	DWORD competition = 0;
	DWORD MonsterNum = 0;
	DWORD DeadMonsterNum = 0;
	DWORD tmpNum = 0;
    DWORD field = 0;
    DWORD domination = 0;
	DWORD Instance = 0;
    DWORD battle_ground = 0;    //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    DWORD goldrush = 0;         //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
    DWORD spa_field=0;          //_NA_008069_20150210_ADD_SPA_SYSTEM

	GameZone * pZone = NULL;
	GAMEZONE_HASH::iterator it = m_pZoneHash->begin(), end = m_pZoneHash->end();
	for( ; it != end ; ++it )
	{
		pZone = it->second;
		switch(pZone->GetZoneType())
		{
		case eZONETYPE_VILLAGE:
			++village;	break;
		case eZONETYPE_LOBBY:
			++lobby;	break;
		case eZONETYPE_MISSION:
			++mission;	break;
		case eZONETYPE_HUNTING:
			++hunting;	break;			
		case eZONETYPE_PVP:
			++pvp;	break;
		case eZONETYPE_CHUNTING:
			++competition;	break;
		case eZONETYPE_FIELD:
			++field;	break;
        case eZONETYPE_DOMINATION_FIELD:
            ++domination;	break;
        case eZONETYPE_SPA_FIELD:   //_NA_008069_20150210_ADD_SPA_SYSTEM
            ++spa_field;    break;
		case eZONETYPE_INSTANCE:
            {
                GameInstanceDungeon* const instance_dungeon = \
                    static_cast<GameInstanceDungeon*>(pZone);
                if (instance_dungeon->GetChaosZoneManager())
                {
                    ChaosZoneType chaos_zone_type = \
                        instance_dungeon->GetChaosZoneManager()->GetChaosZoneType();
                    switch(chaos_zone_type)
                    {
                    case kBattleGround:             //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
                        ++battle_ground;
                        break;
                    case eCHAOSZONE_TYPE_GOLDRUSH:  //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
                        ++goldrush;
                        break;
                    default:
                        ++Instance;
                        break;
                    }

                }
                else
                {
                    ++Instance;	
                }
            }
            break;
		}

		MonsterNum += pZone->GetTotalMonsterNum(tmpNum);
		DeadMonsterNum += tmpNum;
    }

    //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
    //_NA_008069_20150210_ADD_SPA_SYSTEM
    if( bLog )
    {
        SUNLOG( eCRITICAL_LOG, "Village(%u+%u+%u) Lobby(%u) Mission(%u) Hunting(%u) PVP(%u) \
                                Competition(%u) Instance(%u) BattleGround(%u) GoldRush(%u) SpaField(%u) Monster(%u,%u)", 
                village, field, domination, lobby, mission, hunting, pvp, competition, Instance, 
                battle_ground, goldrush, spa_field, MonsterNum, DeadMonsterNum );
    }
    else
    {
        DISPMSG( "Village(%u+%u+%u) Lobby(%u) Mission(%u) Hunting(%u) PVP(%u) Competition(%u) \
                  Instance(%u) BattleGround(%u) GoldRush(%u) SpaField(%u) Monster(%u,%u)\n", 
                  village, field, domination, lobby, mission, hunting, pvp, competition, Instance, 
                  battle_ground, goldrush, spa_field, MonsterNum, DeadMonsterNum );
	}
}

VOID GameZoneManager::DisplayPoolInfo()
{ 
	//GameVillage::DisplayerPoolInfo();
	//DISPMSG(  " VillagePool\n" );

	//GameLobbyRoom::DisplayerPoolInfo();
	//DISPMSG(  " LobbyPool\n" );

	//GameMissionRoom::DisplayerPoolInfo();
	//DISPMSG(  " MissionPool\n" );

	//GameHuntingRoom::DisplayerPoolInfo();
	//DISPMSG(  " HuntingPool\n" );

	//GamePVPRoom::DisplayerPoolInfo();
	//DISPMSG(  " PVPPool\n" );

	//GameCHuntingRoom::DisplayerPoolInfo();
	//DISPMSG(  " CHuntingPool\n" );

	//GameHuntingField::DisplayerPoolInfo();
	//DISPMSG(  " HuntingField\n" );
}





