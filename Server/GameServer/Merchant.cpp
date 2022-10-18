#include "stdafx.h"
#include "Merchant.h"

Merchant::Merchant()
{
	SetObjectType( MERCHANT_OBJECT );
}

Merchant::~Merchant()
{
}

BOOL Merchant::Init()
{
	return NPC::Init();
}

VOID Merchant::Release()
{
	NPC::Release();
}

BOOL Merchant::Update( DWORD dwDeltaTick )
{
	return NPC::Update( dwDeltaTick );
}

void
Merchant::_Damaged(DamageArgs* damage_arg)
{
    return NPC::_Damaged(damage_arg);
}

VOID Merchant::OnDead()
{
	NPC::OnDead();
}