#pragma once

#ifndef SUN_GAMESERVER_FRIENDMONSTER_H_
#define SUN_GAMESERVER_FRIENDMONSTER_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "Monster.h"

// implemented by _NA_0_20100222_UNIT_TRIGGERS_FRIEND_MONSTER

class FriendMonster : public Monster
{
public:
    FriendMonster();
    virtual	~FriendMonster();

    virtual	VOID OnDead();    
    virtual	eUSER_RELATION_TYPE IsFriend( Character* pTarget );
};

#endif // SUN_GAMESERVER_FRIENDMONSTER_H_