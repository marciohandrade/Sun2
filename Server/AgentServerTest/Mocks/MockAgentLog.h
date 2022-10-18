#ifndef SUN_SERVER_AGENTSERVERTEST_MOCKS_MOCKAGENTLOG_H_
#define SUN_SERVER_AGENTSERVERTEST_MOCKS_MOCKAGENTLOG_H_

#pragma once

#include <gmock/gmock.h>

// Note for workarounds: expectations should be set on *Mocker() method.

class MockAgentLog : public AgentLog
{
public:
    virtual ~MockAgentLog() {}


};

#endif // SUN_SERVER_AGENTSERVERTEST_MOCKS_MOCKAGENTLOG_H_
