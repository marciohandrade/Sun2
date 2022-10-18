#include "stdafx.h"
#include ".\handler_cg_charinfo.h"
#include "Player.h"

#ifdef _NA_008404_20150712_MINIGAME_SYSTEM
#include "MiniGame.h"
#endif // _NA_008404_20150712_MINIGAME_SYSTEM

Handler_CG_CHARINFO::Handler_CG_CHARINFO()
{
}

Handler_CG_CHARINFO::~Handler_CG_CHARINFO()
{
}

//--------------------------------------------------------------------------------------------------
// changes, f100819.1L
const nsPacket::PacketHandlerInfo Handler_CG_CHARINFO::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), phase, Handler_CG_CHARINFO::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
        HANDLER_NODE_INFO(CG_CHARINFO, CG_CHARINFO_SELECT_INFO_SYN, PHASE_SERVICE),
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
        HANDLER_NODE_INFO(CG_CHARINFO, CG_CHARINFO_GET_ATTENDANCE_REWARD_PACKET, PHASE_SERVICE),
#endif // _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
#ifdef _NA_008404_20150712_MINIGAME_SYSTEM
        HANDLER_NODE_INFO(CG_CHARINFO, CG_CHARINFO_GET_MINIGAME_BINGO_REWARD_PACKET, PHASE_SERVICE),
#endif // _NA_008404_20150712_MINIGAME_SYSTEM

        HANDLER_NODE_INFO_EOR() // end of records
    };
    //
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    nsPacket::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;
};
//--------------------------------------------------------------------------------------------------


Handler_CG_CHARINFO_IMPL(CG_CHARINFO_SELECT_INFO_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_CHARINFO_SELECT_INFO_SYN* pRecvMsg = (MSG_CG_CHARINFO_SELECT_INFO_SYN*)pMsg;

	BOOL bIsValidSize = (BOOL)(sizeof(MSG_CG_CHARINFO_SELECT_INFO_SYN) == wSize);
	if (!bIsValidSize)
	{
		SUNLOG( eDEV_LOG, _T("[RNDPROTOCOL_REPORT][SIZE] MSG_CG_CHARINFO_SELECT_INFO_SYN") );
		return;
	}

	// 플레이어의 접속이 끊어졌으면 리턴
	Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pRecvMsg->m_dwKey);
    GameField* pField = pPlayer ? pPlayer->GetField() : NULL;
    if (pPlayer == NULL || pField == NULL) {
        return;
    };

    const DWORD dwObjKey = pRecvMsg->m_dwObjectKey;
	Character* const pSelectChar = pField->FindCharacter(dwObjKey);
	if (!pSelectChar)		
        return;

	MSG_CG_CHARINFO_SELECT_INFO_ACK AckMsg;

    AckMsg.m_dwObjectKey = dwObjKey;
    AckMsg.m_LV = pSelectChar->GetLevel();
	AckMsg.m_HP = (PLAYERHP)pSelectChar->GetHP();
    AckMsg.m_MaxHP = (PLAYERHP)pSelectChar->GetMaxHP();
#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
    AckMsg.shield = static_cast<PLAYERSD>(pSelectChar->GetSD());
    AckMsg.max_shield = static_cast<PLAYERSD>(pSelectChar->GetMaxSD());
#endif // _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO

	pPlayer->SendPacket(&AckMsg, sizeof(AckMsg));
}


#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
Handler_CG_CHARINFO_IMPL(CG_CHARINFO_GET_ATTENDANCE_REWARD_PACKET)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_CG_CHARINFO_GET_ATTENDANCE_REWARD_PACKET* pRecvMsg = (MSG_CG_CHARINFO_GET_ATTENDANCE_REWARD_PACKET*)pMsg;

    Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pRecvMsg->m_dwKey);
    GameField* pField = pPlayer ? pPlayer->GetField() : NULL;
    if (pPlayer == NULL || pField == NULL) {
        return;
    };

    AttendanceManager& attendance_manager = pPlayer->GetExtraInfoManager().GetAttendanceManager();
    
    attendance_manager.ProcessReward(pPlayer);
    
    MSG_CG_CHARINFO_ATTENDANCE_INFO_PACKET info_packet;
    memcpy(&info_packet.attendance_infos, &attendance_manager.GetAttendanceInfos(), 
        sizeof(info_packet.attendance_infos));
    pPlayer->SendPacket(&info_packet, sizeof(info_packet));
}
#endif // _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM

#ifdef _NA_008404_20150712_MINIGAME_SYSTEM
Handler_CG_CHARINFO_IMPL(CG_CHARINFO_GET_MINIGAME_BINGO_REWARD_PACKET)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_CG_CHARINFO_GET_MINIGAME_BINGO_REWARD_PACKET* pRecvMsg =
        (MSG_CG_CHARINFO_GET_MINIGAME_BINGO_REWARD_PACKET*)pMsg;

    Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pRecvMsg->m_dwKey);
    GameField* pField = pPlayer ? pPlayer->GetField() : NULL;
    if (pPlayer == NULL || pField == NULL) {
        return;
    };

    MiniGameBingoRewardParam bingo_param(eMINIGAME_TYPE_BINGO, pPlayer);
    RC::eMINIGAME_RESULT minigame_result =
        pPlayer->GetExtraInfoManager().GetMiniGameManager().GetReward(bingo_param);
    if (minigame_result != RC::RC_MINIGAME_REWARD_SUCCESS)
    {
        MSG_CG_CHARINFO_MINIGAME_NAK nak_packet;
        nak_packet.result_code = minigame_result;
        pPlayer->SendPacket(&nak_packet, sizeof(nak_packet));
        return;
    }

    MSG_CG_CHARINFO_MINIGAME_BINGO_INFO_PACKET info_packet;
    memcpy(&info_packet.minigame_bingo_infos,
        &(pPlayer->GetExtraInfoManager().GetMiniGameManager().GetMiniGameBingoInfos()), 
        sizeof(info_packet.minigame_bingo_infos));
    pPlayer->SendPacket(&info_packet, sizeof(info_packet));
}
#endif // _NA_008404_20150712_MINIGAME_SYSTEM
