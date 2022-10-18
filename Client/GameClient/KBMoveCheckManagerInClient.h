#pragma once

#include "Singleton.h"
#include "KBMoveCheckManager.h"
#include "PacketStruct_CG.h"

//------------------------------------------------------------------------------ 
class KBMoveCheckManagerInClient : public nsSync::KBMoveCheckManager, public Singleton<KBMoveCheckManagerInClient>
{
public:
    static const int kBadCountMax = 10;
public:
    KBMoveCheckManagerInClient();
    virtual ~KBMoveCheckManagerInClient();

    //! 클라에서 MSG_CG_CONNECTION_CLIENT_TICK_TEST_SYN -> 서버  20초 후에
    //! 서버에서 MSG_CG_CONNECTION_CLIENT_TICK_TEST_ACK -> 클라
    //! 맵로딩 등 최대 2분을 기준으로 3분동안 연속적으로 클라이언트 TICK와 서버 TICK 가 오차범위를 벗어나면 스피드핵 사용간준
    //! bad_count >= 10 스피드핵 사용.
    //! 오차범위는 20 * 1.5배 => 35초 차이가나면 bad_count
    virtual bool ReceiveReplyFromServer(const MSG_CG_CONNECTION_CLIENT_TICK_TEST_ACK& reply_msg);

    int bad_count() const { return bad_count_; }

private:
    int bad_count_;
};