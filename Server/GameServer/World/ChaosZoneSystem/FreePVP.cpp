#include "StdAfx.h"

#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
#include <World/ChaosZoneSystem/FreePVP.h>
#include <MapInfoParser.h>
#include <GameInstanceDungeon.h>
#include <PacketStruct_AG_ChaosZoneSystem.h>


void FreePVP::Init(GameInstanceDungeon* const instance_dungeon)
{
    instance_dugeon_ = instance_dungeon;
}

void FreePVP::Release()
{
    
}

void FreePVP::Update()
{
}

bool FreePVP::EnterPlayer(Player* const player)
{
    if (obj_util::IsValid(player, __FUNCTION__, __LINE__) == false)
    {
        return false;
    }
    player_hash_.insert(PLAYER_HASH::value_type(player->GetObjectKey(), player));
    return true;
}

void FreePVP::LeavePlayer(Player* const player)
{
    // Agent서버에서 리스트에서 나간 유저 삭제..
	if (obj_util::IsValid(player, __FUNCTION__, __LINE__) == false)
    {
        return;
    }

    PLAYER_HASH::const_iterator const_itr = player_hash_.find(player->GetObjectKey());
    if (const_itr == player_hash_.end())
    {
        return;
    }

    MSG_AG_FREE_PVP_LEAVE_ROOM_CMD cmd_msg;
    cmd_msg.zone_key = instance_dugeon_->GetKey();
    player->SendPacket(&cmd_msg,sizeof(cmd_msg));

    player_hash_.erase(player->GetObjectKey());
}

void FreePVP::EnterTeam(const ChaosZoneTeam team_key, const BYTE player_count, DWORD* user_key_list)
{

}

void FreePVP::DeadCharacter(Character* const dead_character, Character* const kill_character)
{
}

eUSER_RELATION_TYPE FreePVP::IsFriendPVP(Player* attacker,Player* target)
{
    // 타겟이 자기 자신일 경우..
    if (attacker == target)
    {
        return USER_RELATION_FRIEND;
    }

    // 둘다 파티에 속했있는 경우 같은 파티이면 친구..
    if (attacker->GetPartyState().GetPartyKey() != 0 &&
        target->GetPartyState().GetPartyKey() != 0)
    {
        if (attacker->GetPartyState().GetPartyKey() == 
            target->GetPartyState().GetPartyKey())
        {
            return USER_RELATION_FRIEND;
        }
    }

    // 그외 모두 적..
    return USER_RELATION_ENEMY;
}

void FreePVP::Resurrection( Player* player )
{
    _OnMoveToSafezone(player);
    player->OnResurrection(0, 1.f, 1.f);
}

//--------------------------------------------------------------------------------------------------
RC::eSTATUS_RESULT FreePVP::_OnMoveToSafezone(Player* const player)
{
    GameField* const field = player->GetField();
    if (field == NULL)
    {
        return RC::RC_STATUS_FIELD_IS_NULL;
    }

    const sMAPINFO* const map_info = \
        MapInfoParser::Instance()->FindMapInfo(instance_dugeon_->GetMapCode());
    if (map_info == NULL)
    {
        return RC::RC_STATUS_MAPINFO_IS_NULL;
    }
   
    WzVector dest_pos;
    DWORD area_id = random(0, 1) ? map_info->wStartAreaID : map_info->wStartAreaID2;
    if (field->GetAreaRandomStartingVector(area_id, dest_pos) == false)
    {
        return RC::RC_STATUS_IS_NOT_SAFEZONE;
    }

    // 시작위치로 텔레포트 시킨다.
    if(field->TeleportObject( player, &dest_pos, TRUE ) == false)
    {
        return RC::RC_STATUS_TELEPORT_FAIL;
    }

    return RC::RC_STATUS_SUCCESS;
}
#endif // _NA_006088_20120910_CHAOS_ZONE_FREE_PVP