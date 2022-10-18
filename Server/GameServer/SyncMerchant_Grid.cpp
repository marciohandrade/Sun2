#include "stdafx.h"
#include "SyncMerchant.h"

VOID SyncMerchant::OnEnterObject(const Sector& IN rSector)
{
    // CHANGES: f110415.6L, at this point in time, the usage case of 'OnEnterObject' only has
    // notification to users so that if the sector has no users,
    // it is no need to process contiuously.
    if (rSector.GetPlayerNum() == 0) {
        return;
    };
    //
    MSG_CG_SYNC_MERCHANT_ENTER_BRD* brd_msg = 
        m_VarMsg2.StartTypePtr(Param2Class<MSG_CG_SYNC_MERCHANT_ENTER_BRD>());

    m_VarMsg2.SetSize(brd_msg->GetSize());

    // RenderInfo 정보 셋팅
    MONSTER_RENDER_INFO* render_msg = reinterpret_cast<MONSTER_RENDER_INFO*>(m_VarMsg2.NextPtr());
    GetNPCRenderInfo(render_msg);
    m_VarMsg2.SetSize(render_msg->GetSize());

    GameField* const game_field = GetField();

    game_field->SendExPacketToSector(rSector.GetSectorIndex(), m_VarMsg2.GetMsgCount(),
                                     m_VarMsg2.GetMsgPPtr(), m_VarMsg2.GetMsgSizePtr());
}
