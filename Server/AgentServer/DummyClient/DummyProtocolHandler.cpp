#include "stdafx.h"
#include "./DummyProtocolHandler.h"
#if USING_DUMMY_CLIENT

#include "./DummyUser.h"
#include "./DummyClientSession.h"

namespace test_unit {
;

void DummyUserProtocolHandler::OnRecv(DummyUser* dummy_user, MSG_BASE* msg_base, uint16_t msg_size)
{
    // protocol handling
}

void DummyUserProtocolHandler::ActionStep(int configuration_step, DummyUser* dummy_user)
{
}

}; //end of namespace

#endif //USING_DUMMY_CLIENT