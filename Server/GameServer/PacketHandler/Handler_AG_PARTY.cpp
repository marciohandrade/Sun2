#include "StdAfx.h"
#include "Handler_AG_PARTY.h"
#include <PacketStruct_AG.h>
#include "Player.h"
#include <ServerSession.h>

#include "GameParty/NewGamePartyManager.h"

#include "GameGuildManager.h"
#include "GameGuildExtension.h"

//_NA_20110630_BATTLE_ZONE_RENEWAL
#include "BattlezoneMissionTreeParser.h"
#include "GameParty/NewGameParty.h"
#include "ItemInfoParser.h"
#include "MapInfoParser.h"

Handler_AG_PARTY::Handler_AG_PARTY(void)
{
}

Handler_AG_PARTY::~Handler_AG_PARTY(void)
{
}

//--------------------------------------------------------------------------------------------------
// changes, f100819.1L
const nsPacket::PacketHandlerInfo Handler_AG_PARTY::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), phase, Handler_AG_PARTY::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
	    HANDLER_NODE_INFO(AG_PARTY, AG_PARTY_CREATE_CMD, PHASE_SERVICE),
	    HANDLER_NODE_INFO(AG_PARTY, AG_PARTY_DESTROY_CMD, PHASE_SERVICE),
	    HANDLER_NODE_INFO(AG_PARTY, AG_PARTY_JOIN_CMD, PHASE_SERVICE),
	    HANDLER_NODE_INFO(AG_PARTY, AG_PARTY_LEAVE_CMD, PHASE_SERVICE),
	    HANDLER_NODE_INFO(AG_PARTY, AG_PARTY_CHANGE_MASTER_CMD, PHASE_SERVICE),
	    HANDLER_NODE_INFO(AG_PARTY, AG_PARTY_ITEM_DISTRIBUTION_CHANGE_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(AG_PARTY, AG_PARTY_JOIN_SYN, PHASE_SERVICE),
        //_NA_20110630_BATTLE_ZONE_RENEWAL
        HANDLER_NODE_INFO(AG_PARTY, AG_PARTY_CAN_CHANGE_OPTION_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(AG_PARTY, AG_PARTY_CAN_CREATE_ROOM_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(AG_PARTY, AG_PARTY_READY_STATE_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(AG_PARTY, AG_PARTY_NOTIFY_PARTY_MEMBER_INFO_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(AG_PARTY, AG_PARTY_PARTYROOM_ITEM_DELETE_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(AG_PARTY, AG_PARTY_RESET_READY_STATE_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(AG_PARTY, AG_PARTY_CAN_JOIN_ROOM_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(AG_PARTY, AG_PARTY_SEND_MEMBER_RENDER_INFO_CMD, PHASE_SERVICE),
    
    #ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
        HANDLER_NODE_INFO(AG_PARTY, AG_CHAOS_ZONE_PACKET, PHASE_SERVICE),
    #endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
        HANDLER_NODE_INFO_EOR() // end of records
    };
    //
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    nsPacket::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;
};
//--------------------------------------------------------------------------------------------------

Handler_AG_PARTY_IMPL( AG_PARTY_JOIN_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	PACKET_PTR_CONVERT(MSG_AG_PARTY_JOIN_SYN, recv_msg, pMsg);


    RC::ePARTY_RESULT party_result = 
        NewGamePartyManager::Instance()->CanRequestParty(recv_msg->m_dwReqMemberKey, 
                                                         recv_msg->m_dwRecvMemberKey);

    if (party_result != RC::RC_PARTY_SUCCESS)
    {
        MSG_AG_PARTY_JOIN_NAK nak_msg;
        {
            //_NA_0_20100616_PARTY_JOIN_HOSTILITY_RELATION_BUG
            nak_msg.m_dwFlag = recv_msg->m_dwFlag;
            nak_msg.m_dwRecvMemberKey = recv_msg->m_dwRecvMemberKey;
            nak_msg.m_dwErrorCode = party_result;
            nak_msg.m_dwReqMemberKey = recv_msg->m_dwReqMemberKey;
        }
        pServerSession->SendPacket(&nak_msg, sizeof(nak_msg));
        return;
    }

	MSG_AG_PARTY_JOIN_ACK ack_msg;
    {
        //_NA_0_20100616_PARTY_JOIN_HOSTILITY_RELATION_BUG
        ack_msg.m_dwFlag = recv_msg->m_dwFlag;
        ack_msg.m_dwReqMemberKey = recv_msg->m_dwReqMemberKey;
        ack_msg.m_dwRecvMemberKey = recv_msg->m_dwRecvMemberKey;
    }
    pServerSession->SendPacket(&ack_msg, sizeof(ack_msg));
}

// 파티 생성
Handler_AG_PARTY_IMPL( AG_PARTY_CREATE_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_PARTY_CREATE_CMD* pRecvMsg = (MSG_AG_PARTY_CREATE_CMD*)pMsg;

    NewGamePartyManager* const party_manager = NewGamePartyManager::Instance();

    //_NA_20110630_BATTLE_ZONE_RENEWAL
    party_manager->CreateGameParty(pRecvMsg->m_wPartykey, 
                                   pRecvMsg->m_MasterKey, 
                                   (eITEM_DISTRIBUTION_TYPE)pRecvMsg->m_byItemDistributionType, 
                                   pRecvMsg->m_PartyType);   
}

// 파티 소멸
Handler_AG_PARTY_IMPL( AG_PARTY_DESTROY_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_PARTY_DESTROY_CMD* recv_msg = static_cast<MSG_AG_PARTY_DESTROY_CMD*>(pMsg);
	NewGamePartyManager::Instance()->DestroyGameParty(recv_msg->m_wPartykey);
}

// 파티 가입
// (lst1024)(090715) 파티 가입시 게임서버에 길드 관계 적대가 있는지 알아보아야함.
// 패킷 흐름 변경 필요. (AG_PARTY_JOIN_SYN에서의 체크사항 필요)
Handler_AG_PARTY_IMPL( AG_PARTY_JOIN_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_PARTY_JOIN_CMD* recv_msg = static_cast<MSG_AG_PARTY_JOIN_CMD*>(pMsg);
	NewGamePartyManager::Instance()->JoinGameParty(recv_msg->m_wPartykey,
                                                   recv_msg->m_MemberInfo,
                                                   recv_msg->m_bMaster );
}

// 파티 탈퇴
Handler_AG_PARTY_IMPL( AG_PARTY_LEAVE_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_PARTY_LEAVE_CMD* recv_msg = static_cast<MSG_AG_PARTY_LEAVE_CMD*>(pMsg);
	NewGamePartyManager::Instance()->LeaveGameParty(recv_msg->m_wPartykey, recv_msg->m_dwMemberKey);
}

// 파티장 변경
Handler_AG_PARTY_IMPL( AG_PARTY_CHANGE_MASTER_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_PARTY_CHANGE_MASTER_CMD* pRecvMsg = (MSG_AG_PARTY_CHANGE_MASTER_CMD*)pMsg;

	DWORD MemberKey = pRecvMsg->m_dwMemberKey;

	NewGamePartyManager::Instance()->ChangeGameMaster( pRecvMsg->m_wPartykey, MemberKey );
}

Handler_AG_PARTY_IMPL( AG_PARTY_ITEM_DISTRIBUTION_CHANGE_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AG_PARTY_ITEM_DISTRIBUTION_CHANGE_CMD* pRecvMsg = (MSG_AG_PARTY_ITEM_DISTRIBUTION_CHANGE_CMD*)pMsg;

	NewGamePartyManager::Instance()->ChangeItemDistributionType( pRecvMsg->m_wPartykey, (eITEM_DISTRIBUTION_TYPE)pRecvMsg->m_byItemDistributionType );
}


//_NA_20110630_BATTLE_ZONE_RENEWAL

Handler_AG_PARTY_IMPL( AG_PARTY_CAN_JOIN_ROOM_SYN )
{
    const MSG_AG_PARTY_CAN_JOIN_ROOM_SYN* recv_msg = 
        static_cast<MSG_AG_PARTY_CAN_JOIN_ROOM_SYN*>(pMsg);

    NewGamePartyManager* const party_manager = NewGamePartyManager::Instance();
    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(recv_msg->m_dwKey);
    if (player == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found player");
        return;
    }

    MSG_AG_PARTY_CAN_JOIN_ROOM_ACK ack_msg;
    ack_msg.result_code_ = party_manager->CheckJoinRoomTerms(player, 0, 0, 
                                                             recv_msg->party_room_type_, 
                                                             recv_msg->map_code_);
    ack_msg.join_party_key_ = recv_msg->join_party_key_;
    ack_msg.request_user_key_ = player->GetObjectKey();

    pServerSession->SendPacket(&ack_msg, sizeof(ack_msg));
}


Handler_AG_PARTY_IMPL( AG_PARTY_RESET_READY_STATE_CMD )
{
    const MSG_AG_PARTY_RESET_READY_STATE_CMD* recv_msg = 
        static_cast<MSG_AG_PARTY_RESET_READY_STATE_CMD*>(pMsg);

    NewGamePartyManager* party_manager = NewGamePartyManager::Instance();
    Player* player = PlayerManager::Instance()->FindPlayerByObjKey(recv_msg->master_key_);
    if (player == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found player");
        return;
    }

    party_manager->ResetMemberReadyState(recv_msg->party_key_);
}

Handler_AG_PARTY_IMPL( AG_PARTY_PARTYROOM_ITEM_DELETE_CMD )
{
    const MSG_AG_PARTY_PARTYROOM_ITEM_DELETE_CMD* recv_msg = 
        static_cast<MSG_AG_PARTY_PARTYROOM_ITEM_DELETE_CMD*>(pMsg);

    NewGamePartyManager* party_manager = NewGamePartyManager::Instance();
    Player* player = PlayerManager::Instance()->FindPlayerByObjKey(recv_msg->member_key_);
    if (player == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found player");
        return;
    }

#ifdef _NA_007734_20141027_ADD_ROOMKIND_SPECIALMISSION
    PartyState& party_state = player->GetPartyState();
    const BaseParty* const party = party_manager->FindParty(party_state.GetPartyKey());

    if (party != NULL && recv_msg->party_zone_type_ == PartyOfSpecialMission) 
    {
        PlayerManager* player_manager = PlayerManager::Instance();
        const BaseParty::MEMBER_MAP& member_map = party->GetMemberList();
        BaseParty::MEMBER_MAP::const_iterator itr = member_map.begin();
        for( ; itr != member_map.end(); ++itr)
        {
            Player* party_member = player_manager->FindPlayerByObjKey(itr->first);
            if (party_member != NULL) {
                party_manager->RemoveMissionItem(party_member, recv_msg->require_item_, 1);
            }
        }
    }
    else
    {
        party_manager->RemoveMissionItem(player, recv_msg->require_item_, 1);
    }
#else
    party_manager->RemoveMissionItem(player, recv_msg->require_item_, 1);
#endif //_NA_007734_20141027_ADD_ROOMKIND_SPECIALMISSION
}

Handler_AG_PARTY_IMPL( AG_PARTY_SEND_MEMBER_RENDER_INFO_CMD )
{
    const MSG_AG_PARTY_SEND_MEMBER_RENDER_INFO_CMD* recv_msg = 
        static_cast<MSG_AG_PARTY_SEND_MEMBER_RENDER_INFO_CMD*>(pMsg);

    NewGamePartyManager* party_manager = NewGamePartyManager::Instance();
    Player* request_player = 
        PlayerManager::Instance()->FindPlayerByObjKey(recv_msg->request_user_key_);
    if (request_player == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found request player");
        return;
    }

    Player* target_player = 
        PlayerManager::Instance()->FindPlayerByObjKey(recv_msg->target_user_key_);
    if (target_player == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found target player");
        return;
    }

    WORD party_key = recv_msg->request_party_key_;
    const NewGameParty* party = static_cast<const NewGameParty*>(party_manager->FindParty(party_key));
    if (party == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found party : %d", party_key);
        return;
    }

    party->SendPlayerRenderInfo(request_player, target_player);
}

Handler_AG_PARTY_IMPL( AG_PARTY_NOTIFY_PARTY_MEMBER_INFO_CMD )
{
    const MSG_AG_PARTY_NOTIFY_PARTY_MEMBER_INFO_CMD* recv_msg = 
        static_cast<MSG_AG_PARTY_NOTIFY_PARTY_MEMBER_INFO_CMD*>(pMsg);

    NewGamePartyManager* party_manager = NewGamePartyManager::Instance();
    Player* player = PlayerManager::Instance()->FindPlayerByObjKey(recv_msg->member_key_);
    if (player == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found player");
        return;
    }

    WORD party_key = recv_msg->party_key_;
    const NewGameParty* party = 
        static_cast<const NewGameParty*>(party_manager->FindParty(party_key));

    if (party == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found party : %d", party_key);
        return;
    }

    party->NotifyMembersInfoToPlayer(player);
}

Handler_AG_PARTY_IMPL( AG_PARTY_READY_STATE_SYN )
{
    const MSG_AG_PARTY_READY_STATE_SYN* recv_msg = 
        static_cast<MSG_AG_PARTY_READY_STATE_SYN*>(pMsg);

    NewGamePartyManager* party_manager = NewGamePartyManager::Instance();
    Player* player = PlayerManager::Instance()->FindPlayerByObjKey(recv_msg->member_key_);
    if (player == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found player");
        return;
    }

    RC::ePARTY_RESULT result_code = party_manager->SetMemberReadyState(player,
                                                                       recv_msg->party_key_,
                                                                       recv_msg->ready_state_);

    MSG_AG_PARTY_READY_STATE_ACK ack_msg;
    ack_msg.result_code_ = result_code;
    ack_msg.member_key_ = recv_msg->member_key_;
    ack_msg.ready_state_ = recv_msg->ready_state_;
    pServerSession->SendPacket(&ack_msg, sizeof(ack_msg));
}

Handler_AG_PARTY_IMPL( AG_PARTY_CAN_CHANGE_OPTION_SYN )
{
    const MSG_AG_PARTY_CAN_CHANGE_OPTION_SYN* recv_msg = 
        static_cast<MSG_AG_PARTY_CAN_CHANGE_OPTION_SYN *>(pMsg);

    NewGamePartyManager* const party_manager = NewGamePartyManager::Instance();
    Player* master_player = PlayerManager::Instance()->FindPlayerByObjKey(recv_msg->master_key_);
    if (master_player == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found player");
        return;
    }

    const WORD party_key = recv_msg->party_key_;
    const MAPCODE map_code = recv_msg->party_room_info_.map_code_;
    const PartyZoneType party_room_type = recv_msg->party_room_info_.party_room_type_;
    const SLOTCODE item_code = recv_msg->require_item_;

    MSG_AG_PARTY_CAN_CHANGE_OPTION_ACK ack_msg;
    ack_msg.Clear();

    RC::ePARTY_RESULT result_code = party_manager->CheckRoomTerms(master_player, 
                                                                  item_code, 
                                                                  1,
                                                                  party_room_type,
                                                                  map_code);
    if (result_code == RC::RC_PARTY_SUCCESS)
    {
        MapInfoParser* const map_info_parser = MapInfoParser::Instance();
        nsPartyRoomMission::EnterSynArg arg = { 0, };
        arg.map_info_ = map_info_parser->FindMapInfo(map_code);
        if (arg.map_info_ == NULL)
        {
            SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found map parser info : %d", map_code);
            return;
        }

        static CheckMemberConditionToChangeOptionOperator oper;
        oper.Init(eGAMEPARTY_OPERATOR_CHANGE_OPTION_CONDITION_PARTYMEMBER_INFO, 0, &ack_msg, &arg);
        ack_msg.number_of_partymember_ = 
            party_manager->GetMemberConditionForChangeOption(oper, party_key);
        if (ack_msg.number_of_partymember_ == 0 || 
            ack_msg.number_of_partymember_ > _countof(ack_msg.member_state_))
        {
            SUNLOG(eCRITICAL_LOG, __FUNCTION__" : An exception data of member count : %d", 
                   ack_msg.number_of_partymember_);
            return;
        }       
    }
    ack_msg.result_code_ = result_code;
    ack_msg.master_key_ = recv_msg->master_key_;
    ack_msg.party_room_info_ = recv_msg->party_room_info_;

    pServerSession->SendPacket(&ack_msg, sizeof(ack_msg));
}

Handler_AG_PARTY_IMPL( AG_PARTY_CAN_CREATE_ROOM_SYN )
{
    const MSG_AG_PARTY_CAN_CREATE_ROOM_SYN* recv_msg = 
        static_cast<MSG_AG_PARTY_CAN_CREATE_ROOM_SYN *>(pMsg);

    NewGamePartyManager* const party_manager = NewGamePartyManager::Instance();
    Player* master_player = PlayerManager::Instance()->FindPlayerByObjKey(recv_msg->master_key_);
    if (master_player == NULL)
    {
        SUNLOG(eCRITICAL_LOG, 
               __FUNCTION__" : Not found player : Key : %d",
               recv_msg->master_key_);
        return;
    }

    const RC::ePARTY_RESULT result_code = 
        party_manager->CheckRoomTerms(master_player, 
                                      recv_msg->require_item_, 
                                      1, 
                                      recv_msg->party_room_info_.party_room_type_, 
                                      recv_msg->party_room_info_.map_code_);

    MSG_AG_PARTY_CAN_CREATE_ROOM_ACK ack_msg;
    ack_msg.result_code_ = result_code;
    ack_msg.master_key_ = recv_msg->master_key_;
    ack_msg.party_room_info_ = recv_msg->party_room_info_;

    pServerSession->SendPacket(&ack_msg, sizeof(ack_msg));
}

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include <../ProgramCommon/ChaosZoneSystem/PacketStruct_ChaosZoneSystem.h>
#include <World/ChaosZoneSystem/GameChaosZoneManaager.h>
Handler_AG_PARTY_IMPL( AG_CHAOS_ZONE_PACKET )
{
    __TOUCHED((pServerSession, pMsg, wSize));
    GameChaosZoneManager::Instance()->ParseAGPacket(pServerSession, pMsg, wSize);
}
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND