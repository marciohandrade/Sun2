#include "StdAfx.h"
#include ".\handler_cg_areaconquest.h"
#include "Player.h"
#include "PlayerManager.h"
#include <PacketStruct_CG.h>
#include <AreaConquestCommonHeader.h>
#include "FatigueManager.h"

//--------------------------------------------------------------------------------------------------
// changes, f100819.1L
const nsPacket::PacketHandlerInfo Handler_CG_AREACONQUEST::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), phase, Handler_CG_AREACONQUEST::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
        HANDLER_NODE_INFO(CG_AREACONQUEST, CG_AREACONQUEST_REWARD_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO_EOR() // end of records
    };
    //
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    nsPacket::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;
};
//--------------------------------------------------------------------------------------------------


Handler_CG_AREACONQUEST_IMPL(CG_AREACONQUEST_REWARD_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_AREACONQUEST_REWARD_SYN * pRecvMsg = (MSG_CG_AREACONQUEST_REWARD_SYN *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;
	if( !pPlayer->GetField() ) return;

	//피로도 때문에 현재 보상을 받지 못하는 거라면...
	if( pPlayer->GetFatigueManager()->GetItemReward() == FALSE )
	{
		MSG_CG_AREACONQUEST_REWARD_NAK msg;
		msg.m_byErrorCode = RC::RC_AC_REWARD_FAIL_FATIGUE;
		pPlayer->SendPacket( &msg, sizeof(msg) );
		return;
	}

	pPlayer->GetACManager()->RewardAchievement(pRecvMsg->index_number_);
}