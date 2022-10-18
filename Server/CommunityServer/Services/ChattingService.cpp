#include "WorldServerPch.hxx"
#include "./ChattingService.h"

#include <ServerFrame.h>

#include "Objects/WorldUserManager.h"
#include "Objects/WorldUser.h"
#include "Sessions/ServerSessionInWorld.h"

//==================================================================================================

const util::ServerTimeInfoPerFrame* ns_world::ChattingService::server_frame_timer_ = NULL;

//==================================================================================================

// chatting block service
bool ns_world::ChattingService::BlockedChatting(ns_world::WorldUser* user)
{
    if (server_frame_timer_ == NULL) {
        return true;
    };
    const ns_world::ChattingMode& chatting_mode = user->GetChattingMode();
    if (chatting_mode.chatting_blocked == false) {
        return false;
    };

    return true;
}

// chatting block service
// NOTE: f110822.1L, if the m_dwRemainTime is 0, the user will enables chatting.
//                                                                           minute based
void ns_world::ChattingService::BlockChatting(ns_world::WorldUser* user, ulong blocking_timeout)
{
    if (server_frame_timer_ == NULL) {
        assert(!"unexpected state");
        return;
    };
    const ns_world::ChattingMode& chatting_mode = user->GetChattingMode();
    if (blocking_timeout == 0 && chatting_mode.chatting_blocked == false) {
        return;
    };
    ns_world::ChattingMode chatting_mode_config = chatting_mode;
    if (blocking_timeout == 0)
    {
        //chatting_mode_config.chatting_blocked = false;
        chatting_mode_config.chatting_blocking_time = 0;
    }
    else
    {
        chatting_mode_config.chatting_blocked = true;
        blocking_timeout *= 60; // changes the minutes to the seconds
        chatting_mode_config.chatting_blocking_time = \
            server_frame_timer_->aging_counter_ + blocking_timeout;
    };
    //
    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    user_manager->ChangeChattingMode(user, chatting_mode_config);
}

// whispering service
bool ns_world::ChattingService::EnabledWhisperingIntercepted(ns_world::WorldUser* user)
{
    if (server_frame_timer_ == NULL) {
        return false;
    };
    const ns_world::ChattingMode& chatting_mode = user->GetChattingMode();
    if (chatting_mode.whispering_intercepted == false) {
        return false;
    };
    return true;
}

// whispering service
void ns_world::ChattingService::EnableWhisperingIntercepted(
    ns_world::WorldUser* user, bool turned_on)
{
    if (server_frame_timer_ == NULL) {
        return;
    };
    ns_world::ChattingMode chatting_mode_config = user->GetChattingMode();
    chatting_mode_config.whispering_intercepted = turned_on;
    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    user_manager->ChangeChattingMode(user, chatting_mode_config);
}

// calling per 1minute. this routing is called by WorldUserManager.
void ns_world::ChattingService::UpdateDiscountingBlockedState(
    ns_world::WorldUser* user, ns_world::ServerSessionInWorld* dbproxy)
{
    assert(server_frame_timer_ != NULL);
    const ns_world::ChattingMode& chatting_mode = user->GetChattingMode();
    if (chatting_mode.chatting_blocked == false) {
        assert(!"need prechecked");
        return;
    };

    const ulong aging_counter = server_frame_timer_->aging_counter_;
    ulong remains_seconds = 0;
    if (chatting_mode.chatting_blocking_time > aging_counter) {
        remains_seconds = chatting_mode.chatting_blocking_time - aging_counter;
    };
    //
    if (remains_seconds == 0)
    {
        ns_world::ChattingMode chatting_mode_config = chatting_mode;
        chatting_mode_config.chatting_blocked = false;
        chatting_mode_config.chatting_blocking_time = 0;
        ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
        user_manager->ChangeChattingMode(user, chatting_mode_config);
    };
    // NOTE: last 1 minutes update control
    if (remains_seconds != 0 && remains_seconds < 60) {
        return;
    };
    //
    MSG_DW_GM_CHATBLOCK_UPDATE_SYN msg_syn;
    msg_syn.m_dwKey = user->GetCharGuid();
    msg_syn.m_dwRemainTime = (remains_seconds / 60);
    dbproxy->SendPacket(&msg_syn, sizeof(msg_syn));
}

//==================================================================================================
