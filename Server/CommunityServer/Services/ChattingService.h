#pragma once
#ifndef WORLDSERVER_CHATTING_SERVICE_H
#define WORLDSERVER_CHATTING_SERVICE_H

namespace util {
;
struct ServerTimeInfoPerFrame;
};

//==================================================================================================

namespace ns_world {
;

class ChattingService
{
public:
    static void EnableService(const util::ServerTimeInfoPerFrame* server_frame_timer);
    static bool EnabledService();
    static void Update();
    //------------------------------------------------------------------------------------------
    // chatting block service
    static bool BlockedChatting(ns_world::WorldUser* user);
    // chatting block service
    // NOTE: f110822.1L, if the m_dwRemainTime is 0, the user will enables chatting.
    //                                                        minute based
    static void BlockChatting(ns_world::WorldUser* user, ulong blocking_timeout);
    //------------------------------------------------------------------------------------------
    // whispering service
    static bool EnabledWhisperingIntercepted(ns_world::WorldUser* user);
    // whispering service
    static void EnableWhisperingIntercepted(ns_world::WorldUser* user, bool turned_on);
    //------------------------------------------------------------------------------------------
    // calling per 1minute. this routing is called by WorldUserManager.
    static void UpdateDiscountingBlockedState(
        ns_world::WorldUser* user, ns_world::ServerSessionInWorld* dbproxy);
private:
    static const util::ServerTimeInfoPerFrame* server_frame_timer_;
    //
    __DISABLE_COPY(ChattingService);
}; //end of class declaration

}; //end of namespace

//==================================================================================================

inline void ns_world::ChattingService::EnableService(
    const util::ServerTimeInfoPerFrame* server_frame_timer)
{
    server_frame_timer_ = server_frame_timer;
}

inline bool ns_world::ChattingService::EnabledService()
{
    return (server_frame_timer_ != NULL);
}

//==================================================================================================

#endif //WORLDSERVER_CHATTING_SERVICE_H