#pragma once
#ifndef AGENTSERVER_DUMMY_PROTOCOL_HANDLER_H
#define AGENTSERVER_DUMMY_PROTOCOL_HANDLER_H
#if USING_DUMMY_CLIENT

namespace test_unit {
;

class DummyUser;

struct DummyUserProtocolHandler
{
    static void ActionStep(int configuration_step, DummyUser* dummy_user);
    static void OnRecv(DummyUser* dummy_user, MSG_BASE* msg_base, uint16_t msg_size);
};

}; //end of namespace

#endif //USING_DUMMY_CLIENT
#endif //AGENTSERVER_DUMMY_PROTOCOL_HANDLER_H