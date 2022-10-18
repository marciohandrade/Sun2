#include "WorldServerPch.hxx"
#include "./Handler_CW.h"

#include <ProhibitionWordParser.h>

#include "./PacketHandler.h"
#include "WorldServer.h"

//#include "Sessions/ServerSessionInWorld.h"
//#include "Channel/WorldChannelManager.h"
//#include "Channel/WorldChannel.h"
#include "Objects/WorldUserManager.h"
#include "Objects/WorldUser.h"
//#include "Services/ChattingService.h"
//#include "Services/Lotto/Lotto.h"
//#include "Services/Group/GroupManager.h"
//#include "Services/Guild/WorldGuildManager.h"
#include "Services/Memo/WorldSystemMemo.h"
#include "Services/Friends/FriendMap.h"

//==================================================================================================

namespace ns_world { namespace handlers {
;

}}; //end of namespace

//==================================================================================================
//==================================================================================================
//==================================================================================================

#undef HANDLER_NODE_INFO
#define HANDLER_NODE_INFO(_category_, _protocol_, _phase_) \
    void ns_world::handlers::Handler_CW::On##_protocol_(\
        ns_world::ServerSessionInWorld* const server_session, \
        const MSG_BASE* const msg_base, const uint16_t msg_size)

#if SUN_CODE_SAMPLE // to support visual assistx intelli-sense
ns_world::ServerSessionInWorld* server_session;
MSG_BASE* msg_base; uint16_t msg_size;
#endif

//==================================================================================================
//==================================================================================================
//==================================================================================================

// 쪽지 관련 프로토콜 처리
HANDLER_NODE_INFO(CW_MEMO, CW_MEMO_SEND_SYN, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));
    ns_world::WorldUser* const user = FIND_USER_FROM_AGENT_MSG(msg_base);
    if (user == NULL) {
        return;
    };
    const MSG_CW_MEMO_SEND_SYN* recv_msg = static_cast<const MSG_CW_MEMO_SEND_SYN*>(msg_base);
    //
    if (CheckEnabledChattingState(user, server_session) == false) {
        return;
    };

    ns_world::ServerSessionInWorld* const dbproxy = \
        ns_world::WorldServer::Instance()->GetGameDBProxy();
    if (dbproxy == NULL)
    {
        MSG_CW_MEMO_SEND_NAK msg_nak;
        msg_nak.m_byErrorCode = RC::RC_MEMO_FAILED;
        user->SendPacket(&msg_nak, sizeof(msg_nak));

        SUNLOG(eMIDDLE_LOG, _T("|["__FUNCTION__"]|Msg=sending memo is fail ")
               _T("because the worldserver is disconnected with dbproxy|"));
        return;
    };

    if (ns_world::WorldSystemMemo::IsMemoTransaction(user))
    {
        MSG_CW_MEMO_SEND_NAK msg_nak;
        msg_nak.m_byErrorCode = RC::RC_MEMO_DOING_TRANSACTION;
        user->SendPacket(&msg_nak, sizeof(msg_nak));

        SUNLOG(eMIDDLE_LOG, _T("|["__FUNCTION__"]|Msg=the memo task is already processing|")
               _T("UserGuid=%d, CharName='%s'|"),
               user->GetUserGuid(), user->GetCharName());
        return;
    };
    //----------------------------------------------------------------------------------------------
    // CHANGES: f110826.1L, fixed buffer overrun problem. modified DG message
    MSG_DW_MEMO_SEND_SYN msg_update_memo;
    ;{  // setup message 1st step without memo string
        msg_update_memo.m_dwKey = recv_msg->m_dwKey;
        msg_update_memo.m_SendGuid = user->GetCharGuid();
        _tcsncpy(msg_update_memo.m_ptszRecvName, recv_msg->m_ptszRecvName,
                 _countof(msg_update_memo.m_ptszRecvName));
        msg_update_memo.m_ptszRecvName[_countof(msg_update_memo.m_ptszRecvName) - 1] = _T('\0');
        msg_update_memo.m_ptszMemo[0] = _T('\0');
        //_NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION = {
        msg_update_memo.m_typeSystemMemo = NORMAL_MEMO;
        msg_update_memo.m_recvItemCode = 0;
        msg_update_memo.m_amountCost = 0;
        msg_update_memo.m_recvGuid = 0;
        //}
    };
    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    ;{
        ns_world::WorldUser* target_user = \
            user_manager->FindUserByCharName(msg_update_memo.m_ptszRecvName);
        if (target_user && ns_world::FriendMap::FindBlockChar(target_user, user->GetCharGuid()))
        {
            MSG_CW_MEMO_SEND_NAK msg_nak;
            msg_nak.m_byErrorCode = RC::RC_MEMO_FAILED;
            user->SendPacket(&msg_nak, sizeof(msg_nak));

            SUNLOG(eMIDDLE_LOG, _T("|["__FUNCTION__"]|Msg=sending memo is fail because of blocking|")
                   _T("UserGuid=%d, CharName='%s', target='%s'|"),
                   user->GetUserGuid(), user->GetCharName(), msg_update_memo.m_ptszRecvName);
            return;
        };
    };
    ;{  // setup message 2nd step.
        _tcsncpy(msg_update_memo.m_ptszMemo, recv_msg->m_ptszMemo,
                 _countof(msg_update_memo.m_ptszMemo));
        msg_update_memo.m_ptszMemo[_countof(msg_update_memo.m_ptszMemo) - 1] = _T('\0');
    };

    ProhibitionWordParser* const banned_word_filter = ProhibitionWordParser::Instance();
    if (banned_word_filter->CheckFilter(msg_update_memo.m_ptszMemo) == false)
    {
        MSG_CW_CHAT_NORMAL_NAK msg_nak;
        msg_nak.m_wErrorCode = RC::RC_CHAT_USE_BAN_WORD;
        user->SendPacket(&msg_nak, sizeof(msg_nak));

        SUNLOG(eMIDDLE_LOG, _T("|["__FUNCTION__"]|Msg=sending memo is fail ")
               _T("because of using of banned word|")
               _T("UserGuid=%d, CharName='%s', target='%s'|"),
               user->GetUserGuid(), user->GetCharName(), msg_update_memo.m_ptszRecvName);
        return;
    };
    //20070131_메모 특수문자 파싱하고 다음문자 붙여주기
#ifdef _KOREA
    banned_word_filter->CheckIncludeSymbol(msg_update_memo.m_ptszMemo);
#endif
    dbproxy->SendPacket(&msg_update_memo, sizeof(msg_update_memo));

    ns_world::WorldSystemMemo::StartMemoTransaction(user);

    SUNLOG(eFULL_LOG, _T("Memo Send (%s[%u]->%s)"),
           user->GetCharName(), user->GetCharGuid(), msg_update_memo.m_ptszRecvName);
}

HANDLER_NODE_INFO(CW_MEMO, CW_MEMO_LIST_REQ, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));
    ns_world::WorldUser* const user = FIND_USER_FROM_AGENT_MSG(msg_base);
    if (user == NULL) {
        return;
    };
    const MSG_CW_MEMO_LIST_REQ* recv_msg = static_cast<const MSG_CW_MEMO_LIST_REQ*>(msg_base);
    __TOUCHED(recv_msg);
    //
    if (CheckEnabledChattingState(user, server_session) == false) {
        return;
    };
    ns_world::ServerSessionInWorld* const dbproxy = \
        ns_world::WorldServer::Instance()->GetGameDBProxy();
    if (dbproxy == NULL)
    {
        MSG_CW_MEMO_LIST_NAK msg_nak;
        msg_nak.m_byErrorCode = RC::RC_MEMO_FAILED;
        user->SendPacket(&msg_nak, sizeof(msg_nak));

        SUNLOG(eMIDDLE_LOG, _T("|["__FUNCTION__"]|Msg=memo list request is fail ")
               _T("because the worldserver is disconnected with dbproxy|"));
        return;
    };

    if (ns_world::WorldSystemMemo::IsMemoTransaction(user))
    {
        MSG_CW_MEMO_LIST_NAK msg_nak;
        msg_nak.m_byErrorCode = RC::RC_MEMO_DOING_TRANSACTION;
        user->SendPacket(&msg_nak, sizeof(msg_nak));

        SUNLOG(eMIDDLE_LOG, _T("|["__FUNCTION__"]|Msg=the memo task is already processing|")
               _T("UserGuid=%d, CharName='%s'|"),
               user->GetUserGuid(), user->GetCharName());
        return;
    };

    // 시간 체크하고 바로 전송, 이미 길드 멤버 받았으면 응답 없다
    if (ns_world::WorldSystemMemo::EnabledMemoListRequest(user) == false)
    {
        MSG_CW_MEMO_LIST_NAK msg_nak;
        msg_nak.m_byErrorCode = RC::RC_MEMO_WAIT_INFO;
        user->SendPacket(&msg_nak, sizeof(msg_nak));

        SUNLOG(eMIDDLE_LOG, _T("|["__FUNCTION__"]|Msg=memo list request is fail ")
               _T("because of unsatisfied time check|")
               _T("UserGuid=%d, CharName='%s'|"),
               user->GetUserGuid(), user->GetCharName());
        return;
    };

    MSG_DW_MEMO_LIST_REQ msg_syn;
    msg_syn.m_RecvGuid = user->GetCharGuid();
    dbproxy->SendPacket(&msg_syn, sizeof(msg_syn));

    ns_world::WorldSystemMemo::StartMemoTransaction(user);

    SUNLOG(eFULL_LOG, _T("Memo List Req To DB(%s[%u])"),
           user->GetCharName(), msg_syn.m_RecvGuid);
}

HANDLER_NODE_INFO(CW_MEMO, CW_MEMO_VIEW_REQ, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));
    ns_world::WorldUser* const user = FIND_USER_FROM_AGENT_MSG(msg_base);
    if (user == NULL) {
        return;
    };
    const MSG_CW_MEMO_VIEW_REQ* recv_msg = static_cast<const MSG_CW_MEMO_VIEW_REQ*>(msg_base);
    //
    if (CheckEnabledChattingState(user, server_session) == false) {
        return;
    };
    ns_world::ServerSessionInWorld* const dbproxy = \
        ns_world::WorldServer::Instance()->GetGameDBProxy();
    if (dbproxy == NULL)
    {
        MSG_CW_MEMO_VIEW_NAK msg_nak;
        msg_nak.m_byErrorCode = RC::RC_MEMO_FAILED;
        user->SendPacket(&msg_nak, sizeof(msg_nak));

        SUNLOG(eMIDDLE_LOG, _T("|["__FUNCTION__"]|Msg=memo view request is fail ")
               _T("because the worldserver is disconnected with dbproxy|"));
        return;
    };

    if (ns_world::WorldSystemMemo::IsMemoTransaction(user))
    {
        MSG_CW_MEMO_VIEW_NAK msg_nak;
        msg_nak.m_byErrorCode = RC::RC_MEMO_DOING_TRANSACTION;
        user->SendPacket(&msg_nak, sizeof(msg_nak));

        SUNLOG(eMIDDLE_LOG, _T("|["__FUNCTION__"]|Msg=the memo task is already processing|")
               _T("UserGuid=%d, CharName='%s'|"),
               user->GetUserGuid(), user->GetCharName());
        return;
    };

    MSG_DW_MEMO_VIEW_REQ msg_syn;
    msg_syn.m_MemoGuid = recv_msg->m_MemoGuid;
    msg_syn.m_RecvGuid = user->GetCharGuid();

    dbproxy->SendPacket(&msg_syn, sizeof(msg_syn));

    ns_world::WorldSystemMemo::StartMemoTransaction(user);

    SUNLOG(eFULL_LOG, _T("Memo View Req To DB(%s[%u][Msg:%u])"),
           user->GetCharName(), msg_syn.m_RecvGuid, msg_syn.m_MemoGuid);
}

HANDLER_NODE_INFO(CW_MEMO, CW_MEMO_DELETE_SYN, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));
    ns_world::WorldUser* const user = FIND_USER_FROM_AGENT_MSG(msg_base);
    if (user == NULL) {
        return;
    };
    const MSG_CW_MEMO_DELETE_SYN* recv_msg = static_cast<const MSG_CW_MEMO_DELETE_SYN*>(msg_base);
    //
    if (CheckEnabledChattingState(user, server_session) == false) {
        return;
    };
    ns_world::ServerSessionInWorld* const dbproxy = \
        ns_world::WorldServer::Instance()->GetGameDBProxy();
    if (dbproxy == NULL)
    {
        MSG_CW_MEMO_DELETE_NAK msg_nak;
        msg_nak.m_byErrorCode = RC::RC_MEMO_FAILED;
        user->SendPacket(&msg_nak, sizeof(msg_nak));

        SUNLOG(eMIDDLE_LOG, _T("|["__FUNCTION__"]|Msg=memo deletion request is fail ")
               _T("because the worldserver is disconnected with dbproxy|"));
        return;
    };

    if (ns_world::WorldSystemMemo::IsMemoTransaction(user))
    {
        MSG_CW_MEMO_DELETE_NAK msg_nak;
        msg_nak.m_byErrorCode = RC::RC_MEMO_DOING_TRANSACTION;
        user->SendPacket(&msg_nak, sizeof(msg_nak));

        SUNLOG(eMIDDLE_LOG, _T("|["__FUNCTION__"]|Msg=the memo task is already processing|")
               _T("UserGuid=%d, CharName='%s'|"),
               user->GetUserGuid(), user->GetCharName());
        return;
    };

    MSG_DW_MEMO_DELETE_SYN  msg_syn;
    msg_syn.m_MemoGuid = recv_msg->m_MemoGuid;
    msg_syn.m_RecvGuid = user->GetCharGuid();
    msg_syn.m_MemoType = recv_msg->m_MemoType;
    dbproxy->SendPacket(&msg_syn, sizeof(msg_syn));
    ns_world::WorldSystemMemo::StartMemoTransaction(user);

    SUNLOG(eFULL_LOG, _T("Memo Delete Req To DB(%s[%u][Msg:%u])"),
           user->GetCharName(), msg_syn.m_RecvGuid, msg_syn.m_MemoGuid);
}
