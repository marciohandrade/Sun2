#include "stdafx.h"
#include "Summoned.h"
#include "Player.h"
#include "ChaoState.h"

VOID Summoned::OnLeaveField()
{
	Player *pSummoner = GetPlayerOwner();
	if( pSummoner )
		pSummoner->SetForceAttack( false );

	NPC::OnLeaveField();
}

VOID Summoned::OnEnterObject( const Sector& IN rSector )
{
    // CHANGES: f110415.6L, at this point in time, the usage case of 'OnEnterObject' only has
    // notification to users so that if the sector has no users,
    // it is no need to process contiuously.
    if (rSector.GetPlayerNum() == 0) {
        return;
    };
    //
	// ��ȯü ����
	MSG_CG_SYNC_SUMMONED_ENTER_BRD *pBrdMsg = m_VarMsg2.StartTypePtr(Param2Class<MSG_CG_SYNC_SUMMONED_ENTER_BRD>());
	m_VarMsg2.SetSize(pBrdMsg->GetSize());

	// RenderInfo ���� ����
	SUMMONED_RENDER_INFO *pRenderMsg = (SUMMONED_RENDER_INFO*)m_VarMsg2.NextPtr();
	GetSummonedRenderInfo( pRenderMsg );
	m_VarMsg2.SetSize(pRenderMsg->GetSize());

	GameField* const pGameField = GetField();
	pGameField->SendExPacketToSector(
		rSector.GetSectorIndex()
		, m_VarMsg2.GetMsgCount(), m_VarMsg2.GetMsgPPtr(), m_VarMsg2.GetMsgSizePtr()
		);

	// ��ȯü�� ���� ���Ϳ� ��ȯ�ڰ� ������ ��ȯü�� ������ ������.
	// HP�� �ٸ� ĳ���Ϳ��Ե� �������Ƿ�, ���� ���� �������� ��������, MP�� ��ȯ�ڿ��Ը� �ʿ��ϹǷ�, ���⼭ ������.
#ifdef _NA_000000_20140224_SUMMONER_REFACTORING
    Character* m_pSummoner = GetSummoner();
#endif //_NA_000000_20140224_SUMMONER_REFACTORING
	if( m_pSummoner )
	{
        if (rSector.IsExistInSector(m_pSummoner))
		{
			MSG_CG_SUMMON_CURRENT_MP_CMD msg;
			msg.m_dwSummonedObjKey = GetObjectKey();
			msg.m_dwMaxMP = GetMaxMP();
			msg.m_dwMP = GetMP();
			m_pSummoner->SendPacket( &msg, sizeof(MSG_CG_SUMMON_CURRENT_MP_CMD) );
		}
	}

	// �ڽ��� �̵������� ������.
	this->SendMoveInfo( NULL, rSector );
}

