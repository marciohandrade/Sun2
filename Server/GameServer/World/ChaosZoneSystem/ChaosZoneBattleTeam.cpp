#include <stdafx.h>

//_NA_007065_20140217_BATTLEGROUND_REFACTORYING
#include "World/ChaosZoneSystem/ChaosZoneBattleTeam.h"

void ChaosZoneBattleTeam::SendPacketToAll( MSG_BASE_FORWARD* msg, WORD msg_size )
{
    PLAYER_HASH::const_iterator const_itr = player_hash_map_.begin();
    for (; const_itr != player_hash_map_.end(); ++const_itr)
    {
        Player* const player = const_itr->second;
        if (player == NULL || player->IsDeleted())
            continue;
        player->SendPacket(msg, msg_size);
    }
}

void ChaosZoneBattleTeam::RemoveMember( const DWORD char_guid )
{
    Player* const member = FindMember(char_guid);
    if (member == NULL || member->IsDeleted())
    {
        return;
    }

    player_hash_map_.erase(char_guid);
}

Player* ChaosZoneBattleTeam::FindMember( const DWORD char_guid )
{
    PLAYER_HASH_ITR itr = player_hash_map_.find(char_guid);
    return (itr != player_hash_map_.end() ? itr->second : NULL);
}

void ChaosZoneBattleTeam::SetReadyPlayer( Player* const player )
{
    const DWORD user_key = player->GetUserKey();
    ready_user_keys_.insert(user_key);
}

void ChaosZoneBattleTeam::SetReadyPlayer( const BYTE player_count, DWORD* user_key_list )
{
    ready_user_keys_.clear();
    ready_player_count_ = player_count;
    for (int i = 0; i < (int)ready_player_count_; ++i) {
        ready_user_keys_.insert(user_key_list[i]);
    }
}

bool ChaosZoneBattleTeam::IsReadyPlayer( const DWORD user_key )
{
    STLX_SET<DWORD>::const_iterator const_itr = ready_user_keys_.find(user_key);
    if (const_itr != ready_user_keys_.end())
    {
        return true;
    }
    return false;
}

bool ChaosZoneBattleTeam::IsAllPlayerEntered()
{
    if (ready_user_keys_.size() == 0)
    {
        return true;
    }
    return false;
}