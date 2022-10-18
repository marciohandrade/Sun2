#include "stdafx.h"
#include "FriendMonster.h"
#include "GameZone.h"

// implemented by _NA_0_20100222_UNIT_TRIGGERS_FRIEND_MONSTER

FriendMonster::FriendMonster()
{
    SetObjectType(FRIEND_MONSTER_OBJECT);
}

FriendMonster::~FriendMonster()
{
}

VOID FriendMonster::OnDead()
{
    GameField* const game_field = GetField();
    if (!game_field)
        return;

    GameZone* const game_zone = GetGameZonePtr();

    // 존으로 이벤트 전달
    if (game_zone)
        game_zone->OnKillMonster(this);

    // for Trigger & Quest & ItemDeleteEvent
    m_LinkedAction.OnDead(GetField(), this, NULL, m_dwKillerObjectKey);

    NPC::OnDead();
}

eUSER_RELATION_TYPE FriendMonster::IsFriend(Character* pTarget)
{
    if (obj_util::IsValid(pTarget, __FUNCTION__, __LINE__) == false) {
        return USER_RELATION_NEUTRALIST;
    }

    if (this == pTarget)	
        return USER_RELATION_FRIEND;

    if (GetObjectKey() == pTarget->GetObjectKey())
        return USER_RELATION_FRIEND;

    if (pTarget->IsEqualObjectKind(NPC_OBJECT))
    {
        NPC* const npc = static_cast<NPC*>(pTarget);
        const BYTE grade = npc->GetGrade();
        // CHANGES: f110525.9L, add iff relations about eNPC_MOVE_SYNC_MERCHANT
        if (grade == eNPC_FRIEND_MAPOBJECT_NPC ||
            grade == eNPC_FRIEND_MONSTER ||
            grade == eNPC_MOVE_SYNC_MERCHANT)
        {
            return USER_RELATION_FRIEND;
        }
    }
    
    if (pTarget->IsEqualObjectKind(PLAYER_OBJECT)) {
        return USER_RELATION_FRIEND;
    }
    // CHANGES: f110608.2L, added iff relation who is the owner of summoned npc.
    else if (Player* summoner = pTarget->GetPlayerOwner()) {
        return USER_RELATION_FRIEND;
    }

    return USER_RELATION_ENEMY;
}
