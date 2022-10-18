#include "stdafx.h"
#include "TotemNpc.h"



VOID TotemNpc::OnEnterObject( const Sector& IN rSector )
{
    // CHANGES: f110415.6L, at this point in time, the usage case of 'OnEnterObject' only has
    // notification to users so that if the sector has no users,
    // it is no need to process contiuously.
    if (rSector.GetPlayerNum() == 0) {
        return;
    };
    //
#ifndef __NA000000_070928_PACKET_UNICAST_UPDATE__

	MSG_CG_SYNC_TOTEMNPC_ENTER_BRD* pBrdMsg = m_VarMsg2.StartTypePtr( Param2Class<MSG_CG_SYNC_TOTEMNPC_ENTER_BRD>() );
	m_VarMsg2.SetSize(pBrdMsg->GetSize());

	// RenderInfo 정보 셋팅
	TOTEMNPC_RENDER_INFO *pRenderMsg = (TOTEMNPC_RENDER_INFO*)m_VarMsg2.NextPtr();
	GetTotemNPCRenderInfo( pRenderMsg );
	m_VarMsg2.SetSize(pRenderMsg->GetSize());

	GameField* const pGameField = GetField();
	pGameField->SendExPacketToSector(
		rSector.GetSectorIndex()
		, m_VarMsg2.GetMsgCount(), m_VarMsg2.GetMsgPPtr(), m_VarMsg2.GetMsgSizePtr()
		);

#else //

	VARMSG_ASSEMBLER0 vmsg;

	MSG_CG_SYNC_TOTEMNPC_ENTER_BRD& rBRD = vmsg.InstanceOf<MSG_CG_SYNC_TOTEMNPC_ENTER_BRD>();
	vmsg.AddOffset( rBRD.GetSize() );

	// RenderInfo 정보 셋팅
	TOTEMNPC_RENDER_INFO& rRENDER_INFO = vmsg.TypeOf<TOTEMNPC_RENDER_INFO>();
	GetTotemNPCRenderInfo( &rRENDER_INFO );
	vmsg.AddOffset( rRENDER_INFO.GetSize() );

	rSector.SendPacketAll( vmsg.GetFirstPacketPtr(), vmsg.GetTotalSize() );

#endif //
}

