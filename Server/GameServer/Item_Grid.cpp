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

// 자신의 정보를 주변에게
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
		기획 내용상 소넨샤인과 시간교차공간에서는 파티의 아이템 소유권을 없애 어느 파티든 아이템 드랍이 가능하도록 원하셔서
		소유권 관련 로직을 수정하는 것 보다는 소유 시간을 짧게 처리하여 소유권을 없애는 방식을 선택하였다. 추후 더 괜찮은 방법이
		생각나면 변경하자...
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
