//=============================================================================================================================
/// TriggerLastedState class
/**
*/
//=============================================================================================================================

#include "stdafx.h"
#include <PacketStruct_CG.h>

#include "TriggerLastedState.h"
#include "GameZone.h"
#include "Player.h"

// <트리거 최종 상태 정리>
TriggerLastedState*
TriggerLastedState::Create()
{
    return TAllocNew(TriggerLastedState);
}


VOID
TriggerLastedState::Destroy( TriggerLastedState* const ptr )
{
    TAllocDelete(TriggerLastedState, ptr);
}


TriggerLastedState::TriggerLastedState()
{
    m_LatestList.reserve(10);
}

TriggerLastedState::~TriggerLastedState()
{
}

VOID	TriggerLastedState::Clear()
{
	m_LatestList.resize(0);
}

VOID	TriggerLastedState::Change( DWORD dwTriggerID )
{
	if( !dwTriggerID )
		return;

    // (WAVERIX) (090220) (BUG-FIX) 임의의 키를 정상적으로 삭제하지 못하는 문제 수정
    OBJECT_STATES::iterator it = std::find(m_LatestList.begin(), m_LatestList.end(), dwTriggerID);
    if(it != m_LatestList.end())
        m_LatestList.erase(it);
	m_LatestList.push_back(dwTriggerID);
}

BOOL	TriggerLastedState::SendInfo( Player* pPlayer )
{
	MSG_CG_TRIGGER_DONE_ACTION_INFO_CMD msgCMD;
	DWORD dwCounts = 0;
	DWORD* pOffset = msgCMD.m_dwDoneTriggerID;

	FOREACH_CONTAINER( const OBJECT_STATES::value_type& rNode, m_LatestList, OBJECT_STATES )
	{
		*pOffset = rNode;
		++dwCounts;
		++pOffset;

		if( MSG_CG_TRIGGER_DONE_ACTION_INFO_CMD::MAX_DONE_TRIGGER_ID_NUM == dwCounts )
			break;
	}

	if( dwCounts )
	{
		msgCMD.m_byCount = (BYTE)dwCounts;
		return pPlayer->SendPacket( &msgCMD, msgCMD.GetSize() );
	}
	return TRUE;
}



