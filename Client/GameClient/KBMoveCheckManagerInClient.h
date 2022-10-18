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

    //! Ŭ�󿡼� MSG_CG_CONNECTION_CLIENT_TICK_TEST_SYN -> ����  20�� �Ŀ�
    //! �������� MSG_CG_CONNECTION_CLIENT_TICK_TEST_ACK -> Ŭ��
    //! �ʷε� �� �ִ� 2���� �������� 3�е��� ���������� Ŭ���̾�Ʈ TICK�� ���� TICK �� ���������� ����� ���ǵ��� ��밣��
    //! bad_count >= 10 ���ǵ��� ���.
    //! ���������� 20 * 1.5�� => 35�� ���̰����� bad_count
    virtual bool ReceiveReplyFromServer(const MSG_CG_CONNECTION_CLIENT_TICK_TEST_ACK& reply_msg);

    int bad_count() const { return bad_count_; }

private:
    int bad_count_;
};