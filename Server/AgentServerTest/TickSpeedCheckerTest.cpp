// TickSpeedChecker 테스트 케이스

#include "stdafx.h"

#ifdef _NA_20100802_SPEED_HACK_DETECTION

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "TimeCheck/TickSpeedChecker.h"

#include "Mocks/MockUser.h"

using namespace ::testing;

class TickSpeedCheckerTest : public ::testing::Test
{
protected:
    // per-test-case set-up
    static void SetUpTestCase() {}
    // per-test-case tear-down
    static void TearDownTestCase() {}

    // per-test set-up
    virtual void SetUp() {}
    // per-test tear-down
    virtual void TearDown() {}
};

TEST(TickSpeedCheckerTest, EffectiveForcedDisconnection)
{
    TickSpeedChecker t;

    MockUser user;
    t.Init(&user);

    TickSpeedChecker::GetSettings().UseForcedDisconnection(true);
    
    {
        InSequence s;

        struct IsExpectedPacket {
            bool operator()(const MSG_BASE* p) const {
                if (p->m_byCategory != CG_CONNECTION || 
                    p->m_byProtocol != CG_CONNECTION_DISCONNECT_CMD) {
                    return false;
                }
                const MSG_CG_CONNECTION_DISCONNECT_CMD* msg = \
                    static_cast<const MSG_CG_CONNECTION_DISCONNECT_CMD*>(p);
                if (msg->m_dwErrorCode != RC::RC_CONNECTION_REQUESTFROMGAMESERVER) {
                    return false;
                }
                return true;
            }
        };

        EXPECT_CALL(user, SendPacketMocker(Truly(IsExpectedPacket()), _, _))
            .Times(1);
        EXPECT_CALL(user, DisconnectUserMocker(TRUE))
            .Times(1);
    }

    t.DisconnectUser();
}

TEST(TickSpeedCheckerTest, IneffectiveForcedDisconnection)
{
    TickSpeedChecker t;

    MockUser user;
    t.Init(&user);

    TickSpeedChecker::GetSettings().UseForcedDisconnection(false);

    EXPECT_CALL(user, SendPacketMocker(_, _, _))
        .Times(0);
    EXPECT_CALL(user, DisconnectUserMocker(_))
        .Times(0);
    
    t.DisconnectUser();
}

#endif // _NA_20100802_SPEED_HACK_DETECTION