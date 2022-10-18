#include "stdafx.h"
#include "MapNpc.h"
#include "ChangeMonsterAbility.h"


VOID MapNpc::OnEnterObject( const Sector& IN rSector )
{
    // CHANGES: f110415.6L, at this point in time, the usage case of 'OnEnterObject' only has
    // notification to users so that if the sector has no users,
    // it is no need to process contiuously.
    if (rSector.GetPlayerNum() == 0) {
        return;
    };
    //
	MSG_CG_SYNC_MAPNPC_ENTER_BRD* pBrdMsg = m_VarMsg2.StartTypePtr( Param2Class<MSG_CG_SYNC_MAPNPC_ENTER_BRD>() );
	pBrdMsg->m_dwMapNpcID = Conv_TriggerObjectKey(GetMapObjectID());
	m_VarMsg2.SetSize(pBrdMsg->GetSize());

	// RenderInfo 정보 셋팅
	MONSTER_RENDER_INFO *pRenderMsg = (MONSTER_RENDER_INFO*)m_VarMsg2.NextPtr();
	GetNPCRenderInfo( pRenderMsg );
	m_VarMsg2.SetSize(pRenderMsg->GetSize());

	GameField* const pGameField = GetField();
	pGameField->SendExPacketToSector( rSector.GetSectorIndex(), m_VarMsg2.GetMsgCount(), m_VarMsg2.GetMsgPPtr(), m_VarMsg2.GetMsgSizePtr() );
}

VOID MapNpc::OnEnterField( GameField & IN rField, const WzVector & IN rwzVec, int iAngle )
{
    // 방타입과 방인원에 따른 몬스터 능력치 변화
    g_ChangeMonsterAbility.UpdateMonster( rField.GetGameZone(), this );

    NPC::OnEnterField( rField, rwzVec, iAngle );
}

VOID MapNpc::OnLeaveField()
{
    NPC::OnLeaveField();
}
