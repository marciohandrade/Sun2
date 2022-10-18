#include "StdAfx.h"

#ifdef _NA_0_20100416_RELOAD_MAPOBJECT

#include ".\ZoneReloader.h"
#include ".\ObjectFactory.h"
#include ".\GameBattleResult.h"
#include ".\ResrcManager.h"
#include ".\GameZoneManager.h"
#include ".\GameField.h"
#include ".\GameZone.h"
#include ".\Player.h"
#include ".\MapObject.h"
#include ".\Item.h"
#include ".\MapNpc.h"
#include ".\Collection.h"

ZoneReloader::ZoneReloader()
{
}

ZoneReloader::~ZoneReloader()
{
}

void ZoneReloader::Reload()
{
    location_hash_.clear();

    Location location;
    PLAYERKEY object_key;
    GameZone* zone = NULL;
    GameZone* temp_zone = NULL;
    Player* player = NULL;
    GameField* field = NULL;

    GameZoneManager::GAMEZONE_HASH::iterator iter = g_GameZoneManager.m_pZoneHash->begin();
    for ( ; iter != g_GameZoneManager.m_pZoneHash->end() ; ++iter)
    {
        zone = iter->second;

        // 플레이어들의 기존 필드및 위치를 저장해 놓는다.
        GameZone::_PLAYER_HASH::iterator player_iter = zone->m_PlayerHash.begin();
        for ( ; player_iter != zone->m_PlayerHash.end() ; ++player_iter)
        {
            object_key = player_iter->first;
            player = player_iter->second;
            if (obj_util::IsValid(player, __FUNCTION__, __LINE__) == false)
            {
                continue;
            }
            location.zone = zone;
            location.player = player;
            player->GetGameZone(temp_zone, location.field_code, &location.area_id, &location.position);
            location_hash_[object_key] = location;
        }

        _GAMEFIELD_HASH::iterator iter2 = zone->m_GameFieldHash.begin();
        for ( ; iter2 != zone->m_GameFieldHash.end() ; ++iter2 )
        {
            field = iter2->second;

            LeaveNPC(field);

            LeaveMapObject(field);

            LeaveItem(field);

            LeaveCollection(field);
        }

        _GAMEFIELD_HASH::iterator iter3 = zone->m_GameFieldHash.begin();
        for ( ; iter3 != zone->m_GameFieldHash.end() ; ++iter3 )
        {
            field = iter3->second;

            LeavePlayer(field);
        }

        zone->Release();
    }


    g_ResrcMgr.Destroy();

    g_ResrcMgr.Establish();


    iter = g_GameZoneManager.m_pZoneHash->begin();
    for ( ; iter != g_GameZoneManager.m_pZoneHash->end() ; ++iter)
    {
        zone = iter->second;

        eZONE_RULESET& rRuleset = const_cast<eZONE_RULESET&>(zone->Ruleset);
        zone->Init(zone->GetKey(), zone->GetMapCode(), rRuleset.rules);
        zone->OnCreate();
    }



    Location_Hash::iterator iter3 = location_hash_.begin();
    for ( ; iter3 != location_hash_.end() ; ++iter3)
    {
        Location* location = &(iter3->second);

        zone = location->zone;
        player = location->player;

        player->PlanEvent.Init(player);

        zone->Ready(player, location->field_code, location->area_id, &(location->position));
        zone->Join(player, location->field_code, location->area_id, &(location->position));
        player->SetPos(&(location->position));

        //_NA_0_20091118_GAMEROOM_BATTLE_RESULT
        if (zone->m_pBattleResult)
        {
            zone->m_pBattleResult->Enter(player);
        }
    }
}

void ZoneReloader::LeavePlayer(GameField* field)
{
    GameField::_PLAYER_HASH player_hash = field->m_PlayerHash;
    GameField::_PLAYER_HASH::iterator iter = player_hash.begin();
    for ( ; iter != player_hash.end() ; ++iter)
    {
        Player* player = iter->second;
        field->LeaveField(player);
    }
    field->m_PlayerHash.clear();
}

void ZoneReloader::LeaveNPC(GameField* field)
{
    ObjectFactory* const pObjectFactory = ObjectFactory::Instance();

    GameField::_NPC_HASH npc_hash = field->m_NPCHash;
    GameField::_NPC_HASH::iterator iter = npc_hash.begin();
    for ( ; iter != npc_hash.end() ; ++iter)
    {
        NPC* npc = iter->second;
        field->LeaveField(npc);
        pObjectFactory->FreeObject(npc);
    }
    field->m_NPCHash.clear();
}

void ZoneReloader::LeaveMapObject(GameField* field)
{
    ObjectFactory* const pObjectFactory = ObjectFactory::Instance();

    GameField::_MAPOBJECT_HASH::iterator iter = field->m_MapObjectHash.begin();
    for ( ; iter != field->m_MapObjectHash.end() ; ++iter)
    {
        MapObject* map_object = iter->second;
        map_object->OnLeaveField();
        pObjectFactory->FreeObject(map_object);
    }
    field->m_MapObjectHash.clear();
}

void ZoneReloader::LeaveCollection(GameField* field)
{
    ObjectFactory* const pObjectFactory = ObjectFactory::Instance();

    GameField::_COLLECTION_HASH collection_hash = field->m_FieldCollectHash;
    GameField::_COLLECTION_HASH::iterator iter = collection_hash.begin();
    for ( ; iter != collection_hash.end() ; ++iter)
    {
        Collection* collection = iter->second;
        field->LeaveField(collection);
        pObjectFactory->FreeObject(collection);
    }
    field->m_FieldCollectHash.clear();
}

void ZoneReloader::LeaveItem(GameField* field)
{
    ObjectFactory* const pObjectFactory = ObjectFactory::Instance();

    GameField::_ITEM_HASH item_hash = field->m_FieldItemHash;
    GameField::_ITEM_HASH::iterator iter = item_hash.begin();
    for ( ; iter != item_hash.end() ; ++iter)
    {
        Item* item = iter->second;
        field->LeaveField(item);
        pObjectFactory->FreeObject(item);
    }
    field->m_FieldItemHash.clear();
}


#endif //_NA_0_20100416_RELOAD_MAPOBJECT
