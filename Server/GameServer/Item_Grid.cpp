#include "stdafx.h"
#include "Item.h"
#include "Sector.h"


VOID Item::OnEnterObject( const Sector & IN rSector )
{
    // CHANGES: f110415.6L, at this point in time, the usage case of 'OnEnterObject' only has
    // notification to users so that if the sector has no users,
    // it is no need to process contiuously.
    if (rSector.GetPlayerNum() == 0) {
        return;
    };
    //
	MSG_CG_SYNC_FIELDITEM_ENTER_BRD BrdMsg;
	BrdMsg.m_dwFromMonsterKey = GetFromMonsterKey();
	GetItemRenderInfo( BrdMsg.m_ItemRenderInfo );

	GameField* const pGameField = GetField();
	pGameField->SendPacketToSector( rSector.GetSectorIndex(), &BrdMsg, sizeof(MSG_CG_SYNC_FIELDITEM_ENTER_BRD) );
}

// �ڽ��� ������ �ֺ�����
VOID Item::OnLeaveObject( const Sector & IN rSector )
{
	MSG_CG_SYNC_FIELDITEM_LEAVE_BRD BrdMsg;
	BrdMsg.m_byCount = 1;
	BrdMsg.m_dwObjectKey[0] = this->GetObjectKey();

	GameField* const pGameField = GetField();
	pGameField->SendPacketToSector( rSector.GetSectorIndex(), &BrdMsg, BrdMsg.GetSize() );
}


VOID Item::OnEnterField( GameField & IN rField, const WzVector & IN rwzVec, int iAngle )
{
	NonCharacter::OnEnterField( rField, rwzVec, iAngle );

#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION\
	/*
		��ȹ ����� �ҳٻ��ΰ� �ð��������������� ��Ƽ�� ������ �������� ���� ��� ��Ƽ�� ������ ����� �����ϵ��� ���ϼż�
		������ ���� ������ �����ϴ� �� ���ٴ� ���� �ð��� ª�� ó���Ͽ� �������� ���ִ� ����� �����Ͽ���. ���� �� ������ �����
		�������� ��������...
	*/
	if((rField.GetFieldCode() == HardCode::MAPCODE_for_CROSSINGSPACEOFTIME) || (rField.GetFieldCode() == HardCode::MAPCODE_for_SONNENSCHEIN))
	{
		m_OwnershipTimer.SetTimer( 100 );
		m_DestroyTimer.SetTimer( _MAX_DESTROY_TIME_DELAY );
	}
	else
	{
		m_OwnershipTimer.SetTimer( _MAX_FREE_OWNERSHIP_TIME_DELAY );
		m_DestroyTimer.SetTimer( _MAX_DESTROY_TIME_DELAY );
	}
#else
	m_OwnershipTimer.SetTimer( _MAX_FREE_OWNERSHIP_TIME_DELAY );
	m_DestroyTimer.SetTimer( _MAX_DESTROY_TIME_DELAY );
#endif //(_NA_008543_20151027_ONE_DAYS_GUILD_MISSION)
}

VOID Item::OnLeaveField()
{
	NonCharacter::OnLeaveField();
	Release();
}
