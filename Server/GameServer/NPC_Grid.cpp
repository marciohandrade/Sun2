#include "stdafx.h"
#include "NPC.h"
#include "Sector.h"
#include "UnitGroupManager.h"
#include <PacketControl/WarMessages_Sync.h> //_NA_0_20100817_HELLON_LAST_TRIGGER

VarMsg2 NPC::m_VarMsg2;


// NPC ����Ʈ�� �߰�
VOID NPC::OnEnterField( GameField & IN rField, const WzVector & IN rwzVec, int iAngle )
{
	m_iAngle = iAngle;


	// �ӽ�: �̼ǿ��� �ش�, ���� ���͸� ��� �Ǹ� ���� ����
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

	// �ӽ�: �̼ǿ��� �ش�, ���� ���͸� ��� �Ǹ� ���� ����
    // ������� ������ ����?
    // .. ���� ���͸� ��ԵǸ��̶�� ���� �Ƹ��� ��� ���ʹ� ���� ���� ���� ������ ������.
    // ��� ���Ǹ� �ǹ� �Ѵٸ�, �̴� �Ұ����ϴ�. npc info�� ���� �����Ͽ� �����ϴ� ��ʴ� �������
    // ���� ���̴�. 
    // �ǵ� �м� �ʿ���. [4/12/2010 lst1024]

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
	// ������ ���Ⱒ�� ������ ����
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

	// RenderInfo ���� ����
	MONSTER_RENDER_INFO* pRenderMsg = (MONSTER_RENDER_INFO*)m_VarMsg2.NextPtr();
	GetNPCRenderInfo( pRenderMsg );
	m_VarMsg2.SetSize(pRenderMsg->GetSize());

	GameField* const pGameField = GetField();
	pGameField->SendExPacketToSector( rSector.GetSectorIndex(), m_VarMsg2.GetMsgCount(), m_VarMsg2.GetMsgPPtr(), m_VarMsg2.GetMsgSizePtr() );
	// �ڽ��� �̵������� ������.
	this->SendMoveInfo( NULL, rSector );
}

// �ڽ��� ������ �ֺ�����
VOID NPC::OnLeaveObject( const Sector & IN rSector )
{
    // Sector�� �ִ� Player�鿡�� ���� ��Ŷ�� ����
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

