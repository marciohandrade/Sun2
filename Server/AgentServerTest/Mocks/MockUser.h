#ifndef SUN_SERVER_AGENTSERVERTEST_MOCKS_MOCKUSER_H_
#define SUN_SERVER_AGENTSERVERTEST_MOCKS_MOCKUSER_H_

#pragma once

#include <gmock/gmock.h>

// Note for workarounds: expectations should be set on *Mocker() method.

class MockUser : public User
{
public:
    virtual ~MockUser() {}

    // Workaround for default arguments
    // Expectations should be set on SendPacketMocker(), instead of SendPacket()
    MOCK_METHOD3(SendPacketMocker, BOOL(MSG_BASE* pMsg, WORD wSize, const BOOLEAN bUsePacking));
    virtual BOOL SendPacket(MSG_BASE* pMsg, WORD wSize, const BOOLEAN bUsePacking = TRUE) {
        return SendPacketMocker(pMsg, wSize, bUsePacking);
    }

    // Mocking superclass UserSession ------------------------------------------

    // Workaround for default arguments
    MOCK_METHOD1(DisconnectUserMocker, VOID(BOOL bGracefulDisconnect));
    virtual VOID DisconnectUser(BOOL bGracefulDisconnect = TRUE) {
        return DisconnectUserMocker(bGracefulDisconnect);
    }
};

#endif // SUN_SERVER_AGENTSERVERTEST_MOCKS_MOCKUSER_H_
