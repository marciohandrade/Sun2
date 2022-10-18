#include "stdafx.h"
#include "SyncMerchant.h"

#include "GameZone.h"

// implemented by _NA_0_20100222_UNIT_TRIGGERS_REGEN

SyncMerchant::SyncMerchant()
{
    SetObjectType(SYNC_MERCHANT_OBJECT);
}

SyncMerchant::~SyncMerchant()
{
}

VOID SyncMerchant::OnDead()
{
    NPC::OnDead();
}

eUSER_RELATION_TYPE SyncMerchant::IsFriend( Character* pTarget )
{
    return USER_RELATION_NEUTRALIST;
}

VOID SyncMerchant::OnEnterField( GameField & IN rField, const WzVector & IN rwzVec, int iAngle )
{
    SUNLOG( eDEV_LOG, "iAngle = %d", iAngle );
    Monster::OnEnterField( rField, rwzVec, iAngle );
}

