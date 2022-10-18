#include "stdafx.h"
#include "NPCAttrForUpdate.h"
#include "NPC.h"


// 공속, 이속, 최대 HP, MP 변화시 클라에 알려주기 위해 필요한 부분이다.
VOID NPCAttrForUpdate::UpdateEx()
{
	int iAttSpeedRatio = m_pNPC->GetAttSpeedRatio();
	int iMoveSpeedRatio = m_pNPC->GetMoveSpeedRatio();
	DWORD dwMaxHP = m_pNPC->GetMaxHP();
	DWORD dwMaxMP = m_pNPC->GetMaxMP();

	NPCAttributes::Update();

	if( iAttSpeedRatio != m_pNPC->GetAttSpeedRatio() )
	{
		m_pNPC->SendAttrChange( eATTR_ATTACK_SPEED, m_pNPC->GetAttSpeedRatio() );
	}

	if( iMoveSpeedRatio != m_pNPC->GetMoveSpeedRatio() )
	{
		m_pNPC->SendAttrChange( eATTR_MOVE_SPEED, m_pNPC->GetMoveSpeedRatio() );
	}

	if( dwMaxHP != m_pNPC->GetMaxHP() )
	{
		m_pNPC->SendAttrChange( eATTR_MAX_HP, m_pNPC->GetMaxHP() );
	}

	if( dwMaxMP != m_pNPC->GetMaxMP() )
	{
		m_pNPC->SendAttrChange( eATTR_MAX_MP, m_pNPC->GetMaxMP() );
	}
}


VOID NPCAttrForUpdate::SetOwner( SCNPC* pNPC )
{
	NPCAttributes::SetOwner( pNPC );
	m_pNPC = (NPC*)pNPC;
}




































































