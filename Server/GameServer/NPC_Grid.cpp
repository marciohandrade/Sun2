#include "stdafx.h"
#include "NPC.h"
#include "Sector.h"
#include "UnitGroupManager.h"
#include <PacketControl/WarMessages_Sync.h> //_NA_0_20100817_HELLON_LAST_TRIGGER

VarMsg2 NPC::m_VarMsg2;


// NPC 리스트에 추가
VOID NPC::OnEnterField( GameField & IN rField, const WzVector & IN rwzVec, int iAngle )
{
	m_iAngle = iAngle;


	// 임시: 미션에만 해당, 새로 몬스터를 찍게 되면 변경 예정
	REGENID id = 0;
	if( !GetUnitRegenInfo() )
		id = GetObjectKey();
	else
		id = GetRegenID();

	ASSERT( NULL == m_pUnitListOfGroup );
	if( m_pUnitGroupInfo && 0 != m_pUnitGroupInfo->GetID() )
		m_pUnitListOfGroup = rField.GetUnitGroupMgr()->Join( m_pUnitGroupInfo->GetID(), id, this );

	Character::OnEnterField( rField, rwzVec, iAngle );
}

VOID NPC::OnLeaveField()
{
    GameField* game_field = GetField();
	SASSERT(game_field != NULL, "GetField() == NULL"); 

	// 임시: 미션에만 해당, 새로 몬스터를 찍게 되면 변경 예정
    // 어떤식으로 변경할 생각?
    // .. 새로 몬스터를 찍게되면이라는 말이 아마도 모든 몬스터는 툴에 의한 리젠 정보를 가진다.
    // 라는 정의를 의미 한다면, 이는 불가능하다. npc info를 직접 접근하여 생성하는 사례는 사라지지
    // 않을 것이다. 
    // 의도 분석 필요함. [4/12/2010 lst1024]

	REGENID id = 0;
	if( !GetUnitRegenInfo() )
		id = GetObjectKey();
	else
		id = GetRegenID();

	if( m_pUnitGroupInfo && 0 != m_pUnitGroupInfo->GetID() )
		GetField()->GetUnitGroupMgr()->Leave( m_pUnitGroupInfo->GetID(), id );
	m_pUnitListOfGroup = NULL;
    // CHANGES: f110311.4L add a slot type for a trigger area linker to support a multi-area link
    // NOTE: a calling of 'Disconnect' exist in a NPC::Release method
    if (TriggerManagerEx* trigger_manager = (game_field ? game_field->GetTriggerManager() : NULL)) {
        trigger_links_.Disconnect(trigger_manager);
    };
	Character::OnLeaveField();

	Release();
}

VOID NPC::OnEnterObject( const Sector& IN rSector )
{
    // CHANGES: f110415.6L, at this point in time, the usage case of 'OnEnterObject' only has
    // notification to users so that if the sector has no users,
    // it is no need to process contiuously.
    if (rSector.GetPlayerNum() == 0) {
        return;
    };
	// 생성시 방향각을 가지는 몬스터
	if( m_iAngle >= 0 )
	{
		MSG_CG_SYNC_MONSTER_DIR_ENTER_BRD* pBrdMsg = m_VarMsg2.StartTypePtr(Param2Class<MSG_CG_SYNC_MONSTER_DIR_ENTER_BRD>());
		pBrdMsg->wAngle = (WORD)m_iAngle;
		m_VarMsg2.SetSize(pBrdMsg->GetSize());
	}
	else
	{
		MSG_CG_SYNC_MONSTER_ENTER_BRD* pBrdMsg = m_VarMsg2.StartTypePtr(Param2Class<MSG_CG_SYNC_MONSTER_ENTER_BRD>());
		m_VarMsg2.SetSize(pBrdMsg->GetSize());
	}

	// RenderInfo 정보 셋팅
	MONSTER_RENDER_INFO* pRenderMsg = (MONSTER_RENDER_INFO*)m_VarMsg2.NextPtr();
	GetNPCRenderInfo( pRenderMsg );
	m_VarMsg2.SetSize(pRenderMsg->GetSize());

	GameField* const pGameField = GetField();
	pGameField->SendExPacketToSector( rSector.GetSectorIndex(), m_VarMsg2.GetMsgCount(), m_VarMsg2.GetMsgPPtr(), m_VarMsg2.GetMsgSizePtr() );
	// 자신의 이동정보를 보낸다.
	this->SendMoveInfo( NULL, rSector );
}

// 자신의 정보를 주변에게
VOID NPC::OnLeaveObject( const Sector & IN rSector )
{
    // Sector에 있는 Player들에게 제거 패킷을 보냄
    if (IsAlive())
    {
        MSG_CG_SYNC_MONSTER_LEAVE_BRD msgBRD;
        msgBRD.m_byCount = 1;
        msgBRD.m_dwObjectKey[0] = this->GetObjectKey();

        GameField* const pGameField = GetField();
        pGameField->SendPacketToSector( rSector.GetSectorIndex(), &msgBRD, msgBRD.GetSize() );
    }
    else
    {
        MSG_CG_SYNC_OBJECT_DISAPPEAR_DEAD_BRD msgBRD;
        msgBRD.m_dwObjectKey = this->GetObjectKey();

        GameField* const pGameField = GetField();
        pGameField->SendPacketToSector( rSector.GetSectorIndex(), &msgBRD, sizeof(msgBRD) );
    }
}

void NPC::SendMoveInfo(Player* pEnterPlayer, const Sector& IN rSector)
{
    Character::SendMoveInfo(pEnterPlayer, rSector);

    // added by _NA_0_20100817_HELLON_LAST_TRIGGER
    REGENID regen_id = GetRegenID();
    if (regen_id)
    {
        GameField* pField = GetField();
        if (!pField)
            return;

        MSGSUB_SYNC_REGENID_BRD msg;
        msg.m_dwObjectKey	  = GetObjectKey();
        msg.m_dwRegenID		  = regen_id;
        msg.m_dwAnimationCode = GetTriggerAnimationCode();

        pEnterPlayer
            ?	pEnterPlayer->SendPacketForSync(&msg)
            :	pField->SendPacketAroundForSync(this, &msg)
            ;
    }
}

