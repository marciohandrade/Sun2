#include "stdafx.h"
#include "CrystalWarp.h"

VOID CrystalWarp::OnEnterObject( const Sector& IN rSector )
{
    // CHANGES: f110415.6L, at this point in time, the usage case of 'OnEnterObject' only has
    // notification to users so that if the sector has no users,
    // it is no need to process contiuously.
    if (rSector.GetPlayerNum() == 0) {
        return;
    };
    // 소환체 정보
    MSG_CG_SYNC_SUMMONED_ENTER_BRD *pBrdMsg = m_VarMsg2.StartTypePtr(Param2Class<MSG_CG_SYNC_SUMMONED_ENTER_BRD>());
    m_VarMsg2.SetSize(pBrdMsg->GetSize());

    // RenderInfo 정보 셋팅
    SUMMONED_RENDER_INFO *pRenderMsg = (SUMMONED_RENDER_INFO*)m_VarMsg2.NextPtr();
    GetSummonedRenderInfo( pRenderMsg );
    m_VarMsg2.SetSize(pRenderMsg->GetSize());

    GameField* const pGameField = GetField();
    pGameField->SendExPacketToSector( rSector.GetSectorIndex() ,m_VarMsg2.GetMsgCount(),
                                      m_VarMsg2.GetMsgPPtr(), m_VarMsg2.GetMsgSizePtr() );
}

VOID CrystalWarp::OnLeaveField()
{
	NPC::OnLeaveField();
}

