#include "StdAfx.h"
#include "Handler_CG_SKILL.h"
#include "PlayerManager.h"
#include <Struct.h>
#include <PacketStruct_CG.h>
#include <PacketControl/WarMessages_Sync.h>

#include "GameField.h"
#include "Player.h"
#include "Monster.h"
#include "SkillInfoParser.h"
#include "Skill.h"
#include "PublicMath.h"
#include "float.h"
#include "ChaoState.h"
#include "SkillManager.h"
#include <SkillSystem/PerkManager.h> //_NA002217_100728_EP2_PERK_SYSTEM_

//_NA_0_20110131_SKILLTREE_SYSTEM_RENEWAL
#include "SKillSlotManager.h"

//--------------------------------------------------------------------------------------------------
// changes, f100819.1L
const nsPacket::PacketHandlerInfo Handler_CG_SKILL::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), phase, Handler_CG_SKILL::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
        HANDLER_NODE_INFO(CG_SKILL, CG_SKILL_ACTION_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_SKILL, CG_SKILL_SELECT_SKILLPOINT_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_SKILL, CG_SKILL_CASTING_CANCEL_SYN, PHASE_SERVICE),
        //_NA002217_100728_EP2_PERK_SYSTEM_
        HANDLER_NODE_INFO(CG_SKILL, CG_SKILL_PERK_CONTROL_SYN, PHASE_SERVICE),
#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM
        HANDLER_NODE_INFO(CG_SKILL, CG_SKILL_PERK_GROUP_CONTROL_SYN, PHASE_SERVICE),
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM

        HANDLER_NODE_INFO_EOR() // end of records
    };
    //
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    nsPacket::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;
};
//--------------------------------------------------------------------------------------------------

//==================================================================================================
// <OnCG_SKILL_ACTION_SYN>
void Handler_CG_SKILL::OnCG_SKILL_ACTION_SYN(ServerSession* pServerSession,
                                             MSG_BASE* pMsg, WORD wSize)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    const DWORD ERRCODE_HACKED = 1000000;
    const BOOLEAN packet_integrity = (sizeof(MSG_CG_SKILL_ACTION_SYN) == wSize);
    const MSG_CG_SKILL_ACTION_SYN* recv_msg = static_cast<MSG_CG_SKILL_ACTION_SYN*>(pMsg);
    const DWORD user_key = pMsg->m_dwKey;
    Player* const player = PlayerManager::Instance()->FindPlayerByUserKey(user_key);
    if (player == NULL) {
        return;
    }
    //
    GameField* const game_field = player->GetField();
    if (game_field == NULL)
    {
        SUNLOG(eCRITICAL_LOG,
               _T("|["__FUNCTION__"]|Msg=a player(UserGuid=%d) is not stand on a game field|"),
                user_key);
        return;
    }

    //////////////////////////////////////////////////////////////////////////
    //  <LOGIC CONTROL>
    FlowControl flow;
    FLOWCONTROL_START()
    {
        IfFailThenBreak(packet_integrity, ERRCODE_HACKED);

        BYTE decode_stream[sizeof(MSG_CG_SKILL_ACTION_SYN) << 1];
        CopyMemory(decode_stream, pMsg, wSize);

        recv_msg = reinterpret_cast<const MSG_CG_SKILL_ACTION_SYN*>(decode_stream);
        const_cast<MSG_CG_SKILL_ACTION_SYN*>(recv_msg)->Decode();
        // NOTE, can use a full data of a recv_msg since at this line.
        //
        SkillInfo skill_info_data(player, *recv_msg); // editable data structure

        const RC::eSKILL_RESULT rc = player->UseSkill(&skill_info_data, recv_msg->m_byAttackPropensity);
        IfFailThenBreak(rc == RC::RC_SKILL_SUCCESS, rc);

        return /*RETURN TRUE VALUE*/;
    }
    FLOWCONTROL_END;

    //////////////////////////////////////////////////////////////////////////
    //  <ERROR CONTROL>
    const DWORD error_code = flow.GetErrorCode();
    switch (error_code)
    {
    case ERRCODE_HACKED:
        SUNLOG(eDEV_LOG,
               _T("[RNDPROTOCOL_REPORT][SIZE] MSG_CG_SKILL_ACTION_SYN %u!=%u"),
               sizeof(MSG_CG_SKILL_ACTION_SYN), wSize);
        return;
    }

    //{__NA_003197_20090114_CHARACTER_POS_SYNC_BUG_FIX
    // 스킬 사용시 클라가 모션을 취하고 서버에 스킬 사용을 알린다.
    // 서버에서 스킬 사용에 실폐하더라도 클라에서는 멈추어서 모션을 취한후 다시 이동한다.
    // 이때 스킬 사용 클라와 서버 and 근처 다른 클라간에 위치 동기화가 틀려진다. 
    // PVP시 발생

    player->StopMoving();
    player->SetMoveState(CMS_STOP);
    ;{
        MSGSUB_SYNC_STOP_BRD msg;
        msg.m_dwObjectKey = player->GetObjectKey();
        msg.m_wvCurPos = *player->GetPosPtr();
        player->SendPacketAroundForSync(&msg);
        //}__NA_003197_20090114_CHARACTER_POS_SYNC_BUG_FIX
    };
    ;{
        MSG_CG_SKILL_ACTION_NAK msg;
        assert(SAFE_NUMERIC_TYPECAST(DWORD, error_code, BYTE));
        msg.m_byErrorCode = (BYTE)error_code;
        player->SendPacket(&msg, sizeof(msg));
    };
    return;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━


VOID Handler_CG_SKILL::OnCG_SKILL_SELECT_SKILLPOINT_SYN( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_SKILL_SELECT_SKILLPOINT_SYN * pRecvMsg = (MSG_CG_SKILL_SELECT_SKILLPOINT_SYN *)pMsg;

	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	if( !pPlayer )		return ;

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_SKILL::OnCG_SKILL_SELECT_SKILLPOINT_SYN] Invalid Packet" );
		return;
	}

    //_NA_0_20110131_SKILLTREE_SYSTEM_RENEWAL
    SkillSlotManager* skill_manager = pPlayer->GetSkillSlotManager();
    MSG_CG_SKILL_SELECT_SKILLPOINT_ACK ack_message;
    RC::eSKILL_RESULT result = skill_manager->LevelUpSkill(ack_message.skill_total_info, pRecvMsg->skill_code_array, pRecvMsg->skill_count);
    if (result == RC::RC_SKILL_SUCCESS)
    {
        ack_message.remain_skill_point = pPlayer->GetCharInfo()->m_iRemainSkill;
        pPlayer->SendPacket(&ack_message, ack_message.GetSize());
    }
    else
    {
        MSG_CG_SKILL_SELECT_SKILLPOINT_NAK nak_message;
        nak_message.error_code = result;
        pPlayer->SendPacket(&nak_message, sizeof(nak_message));
    }
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━


VOID Handler_CG_SKILL::OnCG_SKILL_CASTING_CANCEL_SYN( ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_CG_SKILL_CASTING_CANCEL_SYN* pRecvMsg = (MSG_CG_SKILL_CASTING_CANCEL_SYN*)pMsg;

//	MSG_CG_SKILL_CASTING_CANCEL_NAK NakMsg;	// (lst1024)(왜 있을까?)

	Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pRecvMsg->m_dwKey );
	if( !pPlayer ) return ;	//< 플레이어 세션이 끊어졌을 때

	if( !pPlayer->GetField() )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CG_SKILL::OnCG_SKILL_CASTING_CANCEL_SYN] Invalid Packet" );
		return;
	}

	// 취소한다.
	pPlayer->SetSkillCancel( pRecvMsg->m_SkillCode, TRUE );
	
}

//==================================================================================================

#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM
void Handler_CG_SKILL::OnCG_SKILL_PERK_GROUP_CONTROL_SYN(ServerSession* pServerSession,
                                             MSG_BASE* pMsg, WORD wSize)
{
    const MSG_CG_SKILL_PERK_GROUP_CONTROL_SYN* recv_msg =
        static_cast<MSG_CG_SKILL_PERK_GROUP_CONTROL_SYN*>(pMsg);
    
    const DWORD user_key = recv_msg->m_dwKey;
    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(user_key);
    if (!FlowControl::FCAssert(player != NULL)) {
        return;
    }
    //
    GameField* const game_field = player->GetField();
    if (game_field == NULL) {
        return;
    }
    
    //
    nsSkill::PerkManager* perk_manager = player->GetPerkManager();

	MSG_CG_SKILL_PERK_GROUP_CONTROL_CMD msg_cmd;

	// 그룹교체를 사용 할 수 있는 상태인지 검사.
	msg_cmd.isCanProcessState = perk_manager->CanProcessState();
	if (msg_cmd.isCanProcessState == false)
	{
		// 교체 할 수 없는 상태이면 그냥 Packet 전송
		player->SendPacket(&msg_cmd, sizeof(msg_cmd));
		return;
	}

    for (int i = 0; i < 4; ++i)
    {
        const PerkControl& req_info = recv_msg->header[i];
        nsSkill::PerkManager::eRequestResult result = \
            perk_manager->ProcessRequestedMessage(req_info, &msg_cmd.header[i], &msg_cmd.data[i]);
    }    

    player->SendPacket(&msg_cmd, sizeof(msg_cmd));
}
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM

//_NA002217_100728_EP2_PERK_SYSTEM_
#ifdef __WAVERIX_TEST_LOGIC_TASK_MACRO__
extern void __WxTest_Receive_Perk_Control(const MSG_CG_SKILL_PERK_CONTROL_CMD* msg_cmd);
#define WAVERIX_TEST_PERK_CONTROL(msg) __WxTest_Receive_Perk_Control(msg)
#else
#define WAVERIX_TEST_PERK_CONTROL(msg)
#endif

Handler_CG_SKILL_IMPL(CG_SKILL_PERK_CONTROL_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
#if !defined(_NA002217_100728_EP2_PERK_SYSTEM_)
    ASSERT(!"unsupported version");
#endif

#ifdef _NA002217_100728_EP2_PERK_SYSTEM_
    const MSG_CG_SKILL_PERK_CONTROL_SYN* recv_msg =
        static_cast<MSG_CG_SKILL_PERK_CONTROL_SYN*>(pMsg);
    const DWORD user_key = recv_msg->m_dwKey;
    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(user_key);
    if (!FlowControl::FCAssert(player != NULL)) {
        return;
    }
    //
    GameField* const game_field = player->GetField();
    if (game_field == NULL) {
        return;
    }
    //
    nsSkill::PerkManager* perk_manager = player->GetPerkManager();
    const PerkControl& req_info = recv_msg->header;
    switch (req_info.request)
    {
    case req_info.eReq_Register:
    case req_info.eReq_Plants:
    case req_info.eReq_Uproot:
    case req_info.eReq_Unregister:
        break;
    case req_info.eReq_Delete:
    case req_info.eReq_Status:
    default:
        SUNLOG(eCRITICAL_LOG,
               _T("|["__FUNCTION__"]|Msg=unauthrization request|UserGuid=%u|Requester=%s|"),
               user_key, player->GetCharName());
        return;
    }
    //
    MSG_CG_SKILL_PERK_CONTROL_CMD msg_cmd;
    nsSkill::PerkManager::eRequestResult result = \
        perk_manager->ProcessRequestedMessage(req_info, &msg_cmd.header, &msg_cmd.data);
    if (result == perk_manager->eReqResult_ReplyResult)
    {
        player->SendPacket(&msg_cmd, msg_cmd.GetSize());
        WAVERIX_TEST_PERK_CONTROL(&msg_cmd);
    }
#endif
}
