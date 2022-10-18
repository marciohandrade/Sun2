#include "stdafx.h"

#include "PacketStruct_AW.h"
#include "PacketStruct_CW.h"
#include "PacketStruct_DW.h"
#include "ServerSession.h"
#include "ServerSessionManager.h"
#include "User.h"
#include "UserManager.h"
#include "WorldServer.h"

#include "Lotto.h"

#include "LottoState.h"

// implemented by __NA_001359_20090619_HEIM_LOTTO_SYSTEM

#include "WorldSystemMemo.h"

LottoState::LottoState(BYTE status)
  : lotto_(Lotto::GetInstance())
  , status_(status)
{
}

void LottoState::OnPlayerJoin(CHARGUID charGuid)
{
    SUNLOG(eFULL_LOG, "[Lotto] OnJoin(%d)", charGuid);

    // ĳ���Ͱ� ���� �� ���� ���� �ε��� �����Ѵ�
    RequestEntryLoad(charGuid, (BYTE)0, true);

    // Ŭ���̾�Ʈ���� ������ �ֱ� ���� ��÷ �ȳ� ���� ��� �ε��� �����Ѵ�
    RequestNoticeList(charGuid);
}

void LottoState::OnPlayerLeave(CHARGUID charGuid)
{
    SUNLOG(eFULL_LOG, "[Lotto] OnLeave(%d)", charGuid);

    // ĳ���Ͱ� ���� �� ĳ�̵� ���� ������ �����
    
    LottoEntry* entry = lotto_.FindEntry(charGuid);
    if (entry != NULL)
    {
        if (entry->payment != entry->paymentOrg)
        {
            // ��ȿ �Һ� ���� �׼��� ������ ������ ����
            RequestPaymentUpdate(charGuid, entry->payment);
        }

        lotto_.ReleaseEntry(charGuid);

#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
        // ĳ�� ���������� ������� ���� ���� ��ȯ Ʈ������� ������̸� ����
        TicketActivationXactions& xactions = lotto_.GetTicketActivationXactions();
        if (xactions.Find(charGuid) != NULL) {
            xactions.Remove(charGuid);
        }
#endif // _NA_20101011_HEIM_LOTTO_CASH_TICKET
    }
}

void LottoState::OnInfoRequest(CHARGUID charGuid, MSG_BASE* packet)
{
    MSG_CW_LOTTO_INFO_SYN* pRecvMsg = static_cast<MSG_CW_LOTTO_INFO_SYN*>(packet);

    MSG_CW_LOTTO_INFO_NAK sendMsg;
    sendMsg.iCharGuid = charGuid;
    sendMsg.bReason = (BYTE)MSG_CW_LOTTO_INFO_NAK::NOT_READY;

    SendToClient(charGuid, (BYTE*)&sendMsg, (WORD)sizeof(MSG_CW_LOTTO_INFO_NAK));
}

void LottoState::OnEntryRequest(CHARGUID charGuid, MSG_BASE* packet)
{
    MSG_CW_LOTTO_ENTRY_SYN* pRecvMsg = static_cast<MSG_CW_LOTTO_ENTRY_SYN*>(packet);

    MSG_CW_LOTTO_ENTRY_NAK sendMsg;
    sendMsg.iCharGuid = charGuid;
    sendMsg.bReason = (BYTE)MSG_CW_LOTTO_ENTRY_NAK::NOT_READY;

    SendToClient(charGuid, (BYTE*)&sendMsg, (WORD)sizeof(MSG_CW_LOTTO_ENTRY_NAK));
}

void LottoState::OnNoticeRequest(CHARGUID charGuid, MSG_BASE* packet)
{
    RequestNoticeList(charGuid);
}

void LottoState::OnNoticeConfirm(CHARGUID charGuid, MSG_BASE* packet)
{
    MSG_CW_LOTTO_NOTICE_CONFIRM* pRecvMsg = static_cast<MSG_CW_LOTTO_NOTICE_CONFIRM*>(packet);

    MSG_DW_LOTTO_NOTICE_DEL_SYN sendMsg;
    sendMsg.iCharGuid = charGuid;
    sendMsg.i64EntryIdx = pRecvMsg->i64Index;
    SendToDBProxy((BYTE*)&sendMsg, (WORD)sizeof(MSG_DW_LOTTO_NOTICE_DEL_SYN));
}

void LottoState::OnEntryLoad(MSG_BASE* packet)
{
    MSG_DW_LOTTO_LOAD_ENTRY_ACK* pRecvMsg = static_cast<MSG_DW_LOTTO_LOAD_ENTRY_ACK*>(packet);

    LottoEntry* entry = lotto_.FindEntry(pRecvMsg->iCharGuid);
    
    // ĳ�̵� ���� ������ ������
    if (entry == NULL)
    {
        // ���ο� ���� ������ ������ ä���
        entry = lotto_.NewEntry(pRecvMsg->iCharGuid);
        ASSERT(entry != NULL);
        if (entry == NULL) {
            SUNLOG(eCRITICAL_LOG, "[Lotto] ERROR : Failed to create a new entry cache for character [%d]", pRecvMsg->iCharGuid);
            return;
        }

        entry->payment             = pRecvMsg->dwEffectivePayment;
        entry->numRemainingTickets = pRecvMsg->bNumRemainingTickets;
        entry->numUsedTickets      = pRecvMsg->bNumUsedTickets;

        // �ε��� ������ ��ȿ �Һ� ���� �׼� ���
        entry->paymentOrg = entry->payment;

        for (int i = 0; i < pRecvMsg->wNumEntries; ++i)
        {
            LottoTicket ticket;

            ticket.drawNum = pRecvMsg->entries[i].wDrawNum;
            ticket.status = pRecvMsg->entries[i].bStatus;
            ticket.pickedNum1 = pRecvMsg->entries[i].bPickedNum[0];
            ticket.pickedNum2 = pRecvMsg->entries[i].bPickedNum[1];
            ticket.pickedNum3 = pRecvMsg->entries[i].bPickedNum[2];
            ticket.pickedNum4 = pRecvMsg->entries[i].bPickedNum[3];

            entry->tickets.push_back(ticket);
        }
    }

    // �����ε��� �ƴ϶� Ŭ���̾�Ʈ ��û�� ���� ���̸� ���� �ζ� ���� ���� ����
    if (pRecvMsg->bInPreLoading == 0) {
        SendInfoResponse(pRecvMsg->iCharGuid, pRecvMsg->bFlag);
    }
}

void LottoState::OnReward(MSG_BASE* packet)
{
    MSG_DW_LOTTO_REWARD_ACK* pRecvMsg = static_cast<MSG_DW_LOTTO_REWARD_ACK*>(packet);

    if (pRecvMsg->iResult != 0) {
        // DB���� ���� ó�� ����
        SUNLOG(eCRITICAL_LOG, "[Lotto] ERROR rewarding [%d] entryId=%I64d", pRecvMsg->iCharGuid, pRecvMsg->i64Idx);
        return;
    }

    TCHAR toCharName[MAX_CHARNAME_LENGTH + 1];

    User* user = UserManager::Instance()->FindUserByCharGuid(pRecvMsg->iCharGuid);
    if (user != NULL) {
        _tcsncpy(toCharName, user->GetCharName().c_str(), MAX_CHARNAME_LENGTH);
    }
    else {
        _tcsncpy(toCharName, pRecvMsg->szCharName, MAX_CHARNAME_LENGTH);
    }
    toCharName[MAX_CHARNAME_LENGTH] = '\0';

    // ��÷ �ȳ� �ý��� ���� ����
    MSG_DW_MEMO_SEND_SYN sendMsg;
    sendMsg.m_dwKey = 0;    // �ý������κ����� ����
    sendMsg.m_SendGuid = 0; // �ý������κ����� ����
    //_NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION = {
    sendMsg.m_typeSystemMemo = NORMAL_MEMO;
    sendMsg.m_recvItemCode = 0;
    sendMsg.m_amountCost = 0;
    sendMsg.m_recvGuid = 0;
    //}
    _tcsncpy( sendMsg.m_ptszRecvName, toCharName, MAX_CHARNAME_LENGTH );
    sendMsg.m_ptszRecvName[MAX_CHARNAME_LENGTH] = '\0';
    // �ý��� �޸� ����: "SYSMEMO_LOTTO_WIN | EntryIndex | DrawNum | Rank | Prize"
    _sntprintf(sendMsg.m_ptszMemo, MAX_MEMO_LENGTH, "%d|%I64d|%d|%d|%I64d", \
        SYSMEMO_LOTTO_WIN, pRecvMsg->i64Idx, pRecvMsg->wDrawNum, pRecvMsg->bStatus, pRecvMsg->i64Prize);
    sendMsg.m_ptszMemo[MAX_MEMO_LENGTH] = '\0';
    SendToDBProxy((BYTE*)&sendMsg, (WORD)sizeof(MSG_DW_MEMO_SEND_SYN));

    SUNLOG(eFULL_LOG, "[Lotto] Sent lotto memo to [%d] entryId=%I64d", pRecvMsg->iCharGuid, pRecvMsg->i64Idx);
}

void LottoState::OnNoticeList(MSG_BASE* packet)
{
    MSG_DW_LOTTO_NOTICE_LIST_ACK* pRecvMsg = static_cast<MSG_DW_LOTTO_NOTICE_LIST_ACK*>(packet);

    MSG_CW_LOTTO_NOTICE_LIST_ACK sendMsg;
    sendMsg.bNumNoticeRecords = (BYTE)pRecvMsg->iNumRecords;
    for (int i = 0; i < pRecvMsg->iNumRecords; ++i) {
        sendMsg.notices[i].i64Index = pRecvMsg->notices[i].i64Index;
        sendMsg.notices[i].wDrawNum = pRecvMsg->notices[i].wDrawNum;
        sendMsg.notices[i].bRank    = pRecvMsg->notices[i].bRank;
        sendMsg.notices[i].i64Prize = pRecvMsg->notices[i].i64Prize;
    }
    SendToClient(pRecvMsg->iCharGuid, (BYTE*)&sendMsg, (WORD)sendMsg.GetSize());
}

void LottoState::OnNoticeDelete(MSG_BASE* packet)
{
    MSG_DW_LOTTO_NOTICE_DEL_ACK* pRecvMsg = static_cast<MSG_DW_LOTTO_NOTICE_DEL_ACK*>(packet);

    if (pRecvMsg->iResult != 0 || pRecvMsg->iCharGuid1 != pRecvMsg->iCharGuid2)
    {
        SUNLOG(eCRITICAL_LOG, "[Lotto] Illegal reward attempt : Char [%d]", pRecvMsg->iCharGuid1);
        return;
    }

    CHARGUID charGuid = pRecvMsg->iCharGuid2;

    // ������ ��� ����
    MSG_AW_USER_LOTTO_REWARD_SYN syn;
    syn.m_CharGuid = charGuid;
    syn.m_Prize    = (MONEY)pRecvMsg->i64Prize;
    SendToAgentServer(charGuid, (BYTE*)&syn, (WORD)sizeof(syn));
}

void LottoState::OnActualReward(MSG_BASE* packet)
{
    MSG_AW_USER_LOTTO_REWARD_ACK* pRecvMsg = static_cast<MSG_AW_USER_LOTTO_REWARD_ACK*>(packet);

    if (pRecvMsg->m_iResult != MSG_AW_USER_LOTTO_REWARD_ACK::RESULT_OK)
    {
        // ��÷ �ȳ� ������ Ȯ���� ����ڰ� �¶����� �ƴ� ���� �������� ���:
        // ���������� ����ڿ��� ������ �� �� ���� �α׸� ����� �����Ƿ�
        // ���������� ����ڿ� ���� ������ ������� �ʴ´�.
        // ��� �ȳ� ������ ��� �������� �ʰ� Ȯ�� �ܰ踦 �߰�, �����ϴ� ���� 
        // ó���� ������ ���̴�.
    }

    // ���� �ζ� ��� ������ ���������� �Ϸ��
}

#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
// ĳ�� �������� ����� �ζ� ���� ���� Ȱ��ȭ
void LottoState::OnTicketActivation(MSG_BASE* packet)
{
    // ����Ʈ�� ���� ó���ϸ�, Ȱ��ȭ�� ���� ���� Ŭ�������� �������̵� �ʿ�

    MSG_AW_ITEM_USE_HEIM_LOTTO_TICKET_SYN* msg = \
        static_cast<MSG_AW_ITEM_USE_HEIM_LOTTO_TICKET_SYN*>(packet);

    MSG_AW_ITEM_USE_HEIM_LOTTO_TICKET_ACK ack;
    ack.result_code = static_cast<WORD>(RC::RC_ITEM_HEIM_LOTTO_UNAVAILABLE);
    ack.char_guid = msg->char_guid;
    ack.index     = msg->index;
    ack.pos       = msg->pos;
    SendToAgentServer(msg->char_guid, (BYTE*)&ack, (WORD)sizeof(ack));
}
#endif // _NA_20101011_HEIM_LOTTO_CASH_TICKET

void LottoState::BroadcastLottoStart(int drawNum)
{
    MSG_CW_LOTTO_START_BRD sendMsg;

    sendMsg.wDrawNum = (WORD)drawNum;

    BroadcastToClients((BYTE*)&sendMsg, (WORD)sizeof(MSG_CW_LOTTO_START_BRD));
}

void LottoState::BroadcastCountdown(int secondsTo)
{
    MSG_CW_LOTTO_COUNTDOWN_BRD sendMsg;

    sendMsg.wDrawNum = lotto_.GetCurrentDrawNum();
    sendMsg.wSeconds = (WORD)secondsTo;

    BroadcastToClients((BYTE*)&sendMsg, (WORD)sizeof(MSG_CW_LOTTO_COUNTDOWN_BRD));
}

void LottoState::BroadcastLottoFinish()
{
    LottoResult* result = lotto_.GetResult(0);
    if (NULL == result) {
        SUNLOG(eFULL_LOG, "[Lotto] Finish broadcasting request while no current draw activated");
        return;
    }

    MSG_CW_LOTTO_FINISH_BRD sendMsg;

    sendMsg.wDrawNum = lotto_.GetCurrentDrawNum();
    for (int i = 0; i < LottoConfig::NUM_PICKS; ++i) {
        sendMsg.bWinningNum[i] = result->winningNum[i];
    }

    BroadcastToClients((BYTE*)&sendMsg, (WORD)sizeof(MSG_CW_LOTTO_FINISH_BRD));
}

void LottoState::SendInfoResponse(CHARGUID charGuid, BYTE flag)
{
    LottoEntry* entry = lotto_.FindEntry(charGuid);
    if (entry == NULL) {
        SUNLOG(eCRITICAL_LOG, "[Lotto] ERROR : Entry not found for character [%d]", charGuid);
        return;
    }

    // ���� ȸ���� NumWinners �Ӽ��� ���� ���� ���� ���� ȸ���� �ִ��� �Ǵ�
    bool hasCurrentDraw = ((lotto_.GetResult(0) != NULL) && (lotto_.GetResult(0)->numWinners < 0));
    int numPrevResults = max(0, lotto_.numResults_ - 1);

    MSG_CW_LOTTO_INFO_ACK sendMsg;

    sendMsg.bFlag = flag;           // Ŭ���̾�Ʈ ��û�� ���Ե� �÷��׸� �״�� ����
    sendMsg.bStatus = status_;      // ���� �ζ� Ȱ��ȭ ����
    sendMsg.iCharGuid = charGuid;
    sendMsg.wCurrentDrawNum = (WORD)lotto_.GetCurrentDrawNum();
    /* ��÷ ���� ���� �������� ���� ���� ȸ�� ��ȣ�� ���� �ʿ䰡 �ִٸ�
    if (!hasCurrentDraw) {
        ++sendMsg.wCurrentDrawNum;
    }
    */
    sendMsg.i64TotalAccum = lotto_.GetCurrentTotalAccum();
    
    // ���� ��÷ ���
    sendMsg.bNumTickets = (BYTE)entry->numRemainingTickets;
    int i = 0;
    for (int j = (hasCurrentDraw ? 1 : 0); i < numPrevResults; ++i, ++j) {
        LottoResult* result = lotto_.GetResult(j);
        if (result == NULL) { break; }
        sendMsg.results[i].wDrawNum = (WORD)result->drawNum;
        sendMsg.results[i].bNumWinners = (BYTE)max(result->numWinners, 0);
        sendMsg.results[i].i64Jackpot = result->jackpot;
        for (int k = 0; k < LottoConfig::NUM_PICKS; ++k) {
            sendMsg.results[i].bWinningNum[k] = result->winningNum[k];
        }
    }
    sendMsg.bNumResultRecords = (BYTE)i;

    // ���� ���� ����
    sendMsg.bNumEntryRecords = (BYTE)entry->tickets.size();
    for (int i = 0; i < sendMsg.bNumEntryRecords; ++i) {
        sendMsg.entries[i].wDrawNum = (WORD)entry->tickets[i].drawNum;
        sendMsg.entries[i].bPickedNum[0] = entry->tickets[i].pickedNum1;
        sendMsg.entries[i].bPickedNum[1] = entry->tickets[i].pickedNum2;
        sendMsg.entries[i].bPickedNum[2] = entry->tickets[i].pickedNum3;
        sendMsg.entries[i].bPickedNum[3] = entry->tickets[i].pickedNum4;
    }

    SendToClient(charGuid, (BYTE*)&sendMsg, (WORD)sendMsg.GetSize());
}

void LottoState::NotifyTicketAcquisition(CHARGUID charGuid, int numRemainingTickets)
{
    MSG_CW_LOTTO_TICKET_NTF sendMsg;
    sendMsg.iCharGuid = charGuid;
    sendMsg.bNumTickets = (BYTE)numRemainingTickets;
    SendToClient(charGuid, (BYTE*)&sendMsg, (WORD)sizeof(MSG_CW_LOTTO_TICKET_NTF));
}

void LottoState::RequestEntryLoad(CHARGUID charGuid, BYTE flag, bool inPreLoading)
{
    MSG_DW_LOTTO_LOAD_ENTRY_SYN sendMsg;

    sendMsg.bFlag = flag;
    sendMsg.iCharGuid = charGuid;
    sendMsg.bNumRecent = (BYTE)LottoConfig::NUM_RECENT_DRAWS;
    sendMsg.bInPreLoading = (BYTE)(inPreLoading ? 1 : 0);

    SendToDBProxy((BYTE*)&sendMsg, (WORD)sizeof(MSG_DW_LOTTO_LOAD_ENTRY_SYN));
}

void LottoState::RequestLoad()
{
    MSG_DW_LOTTO_LOAD_SYN sendMsg;

    sendMsg.bNumRecent = LottoConfig::NUM_RECENT_DRAWS;

    SendToDBProxy((BYTE*)&sendMsg, (WORD)sizeof(MSG_DW_LOTTO_LOAD_SYN));
}

void LottoState::RequestWinnerList(int page)
{
    MSG_DW_LOTTO_WINNER_LIST_SYN sendMsg;

    sendMsg.wDrawNum = lotto_.lastDrawNum_; // (JAYKANG) (090828) (BUG-FIX)
    sendMsg.iPage = page;

    SendToDBProxy((BYTE*)&sendMsg, (WORD)sizeof(MSG_DW_LOTTO_WINNER_LIST_SYN));
}

void LottoState::RequestTicketAdd(CHARGUID charGuid, int numRemainingTickets)
{
    MSG_DW_LOTTO_ADD_TICKET_SYN sendMsg;

    sendMsg.iCharGuid = charGuid;
    sendMsg.bNumRemainingTickets = (BYTE)numRemainingTickets;
    sendMsg.bMaxTickets = (BYTE)LottoConfig::MAX_TICKETS;

    SendToDBProxy((BYTE*)&sendMsg, (WORD)sizeof(MSG_DW_LOTTO_ADD_TICKET_SYN));
}

void LottoState::RequestEntrySave(CHARGUID charGuid, BYTE isAuto, BYTE status, BYTE* pickedNum)
{
    MSG_DW_LOTTO_SAVE_ENTRY_SYN sendMsg;

    User* user = UserManager::Instance()->FindUserByCharGuid(charGuid);
    sendMsg.m_dwKey = ( user != NULL ? user->GetGUID() : 0 );
    sendMsg.iCharGuid = charGuid;
    sendMsg.wDrawNum = lotto_.GetCurrentDrawNum();
    sendMsg.bIsAuto = isAuto;
    sendMsg.bStatus = status;
    for (int i = 0; i < LottoConfig::NUM_PICKS; ++i) {
        sendMsg.bPickedNum[i] = pickedNum[i];
    }

    SendToDBProxy((BYTE*)&sendMsg, (WORD)sizeof(MSG_DW_LOTTO_SAVE_ENTRY_SYN));
}

void LottoState::RequestRollOver(WORD currentDrawNum, INT8 numWinners, 
                                   INT64 jackpot, INT64 prize2, INT64 prize3, 
                                   BYTE* winningNum)
{
    MSG_DW_LOTTO_ROLLOVER_SYN sendMsg;

    sendMsg.wCurrentDrawNum = currentDrawNum;
    sendMsg.iNumWinners     = numWinners;
    sendMsg.i64Jackpot      = jackpot;
    sendMsg.i64Prize2       = prize2;
    sendMsg.i64Prize3       = prize3;
    sendMsg.bWinningNum[0]  = winningNum[0];
    sendMsg.bWinningNum[1]  = winningNum[1];
    sendMsg.bWinningNum[2]  = winningNum[2];
    sendMsg.bWinningNum[3]  = winningNum[3];

    SendToDBProxy((BYTE*)&sendMsg, (WORD)sizeof(MSG_DW_LOTTO_ROLLOVER_SYN));
}

void LottoState::RequestAccumulate(WORD currentDrawNum, INT64 totalAccum)
{
    MSG_DW_LOTTO_ACCUM_NTF sendMsg;

    sendMsg.wCurrentDrawNum = currentDrawNum;
    sendMsg.i64TotalAccum   = totalAccum;

    SendToDBProxy((BYTE*)&sendMsg, (WORD)sizeof(MSG_DW_LOTTO_ACCUM_NTF));
}

void LottoState::RequestReward(LottoWinner& winner)
{
    ASSERT(lotto_.lastDrawNum_ != 0);
    ASSERT(0 < winner.status && winner.status <= lotto_.config_.GetNumPrizes());

    if (0 == lotto_.lastDrawNum_) {
        return;
    }

    User* user = UserManager::Instance()->FindUserByCharGuid(winner.charGuid);

    // changed by _NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION
    WorldSystemMemo system_memo;
    SystemMemoInfoOfLotto lotto_memo_info;
    {
        //-- ���� Guid�� �̸��� ������� ������ ������.
        if (user == NULL)
        {
            lotto_memo_info.recv_user_guid_ = winner.charGuid;
        }
        else
        {
            _tcsncpy(lotto_memo_info.recv_user_name_, user->GetCharName().c_str(), 
                     _countof(lotto_memo_info.recv_user_name_));
            lotto_memo_info.recv_user_name_[_countof(lotto_memo_info.recv_user_name_) - 1] = '\0';
        }
        lotto_memo_info.draw_num_ = lotto_.lastDrawNum_;
        lotto_memo_info.i64Idx_ = winner.idx;
        lotto_memo_info.status_ = winner.status;
        lotto_memo_info.amount_cost_ = lotto_.prizes_[winner.status - 1];
        lotto_memo_info.system_memo_type_ = SYSMEMO_LOTTO_WIN_NEW;
        lotto_memo_info.recv_item_code_ = 0;
    }
    system_memo.SendSystemMemo(&lotto_memo_info, 1,SYSMEMO_LOTTO_WIN_NEW);
    if(winner.status == 1 || winner.status == 2)
    {
        //1, 2� ���� �α׸� ��´�.
        GAMELOG->LogSettleHeimLotto(MONEY_HEIM_LOTTO_SETTLE_RESULT, user, 
            winner.charGuid, lotto_.lastDrawNum_, winner.status, lotto_.prizes_[winner.status - 1]);
    }
}

void LottoState::RequestNoticeList(CHARGUID charGuid)
{
    MSG_DW_LOTTO_NOTICE_LIST_SYN sendMsg;
    sendMsg.iCharGuid = charGuid;
    sendMsg.bMaxCount = LottoConfig::MAX_NOTICE;
    SendToDBProxy((BYTE*)&sendMsg, (WORD)sizeof(MSG_DW_LOTTO_NOTICE_LIST_SYN));
}

void LottoState::RequestPaymentUpdate(CHARGUID charGuid, DWORD effectivePayment)
{
    MSG_DW_LOTTO_PAYMENT_UPDATE_NTF sendMsg;
    sendMsg.iCharGuid = charGuid;
    sendMsg.dwEffectivePayment = effectivePayment;
    SendToDBProxy((BYTE*)&sendMsg, (WORD)sizeof(MSG_DW_LOTTO_PAYMENT_UPDATE_NTF));
}

bool LottoState::SendToClient(CHARGUID charGuid, BYTE* msg, WORD wSize)
{
    User* user = UserManager::Instance()->FindUserByCharGuid(charGuid);
    if (NULL == user) {
        SUNLOG(eFULL_LOG, "[Lotto] ERROR : No user found for character [%d]", charGuid);
        return false;
    }
    user->SendPacket(msg, wSize);
    return true;
}

void LottoState::BroadcastToClients(BYTE* msg, WORD wSize)
{
    UserManager::Instance()->SendToAll(msg, wSize);
}

bool LottoState::SendToDBProxy(BYTE* msg, WORD wSize)
{
    ServerSession *pGameDBProxy = ServerSessionManager::Instance()->GetGameDBProxy();
    if (NULL == pGameDBProxy) {
        SUNLOG(eCRITICAL_LOG, "[Lotto] ERROR : No GameDBProxy");
        return false;
    }
    pGameDBProxy->Send(msg, wSize);
    return true;
}

bool LottoState::SendToAgentServer(CHARGUID charGuid, BYTE* msg, WORD wSize)
{
    User* user = UserManager::Instance()->FindUserByCharGuid(charGuid);
    if (NULL == user) {
        SUNLOG(eFULL_LOG, "[Lotto] ERROR : No user found for character [%d]", charGuid);
        return false;
    }
    ServerSession *pAgentServer = \
        ServerSessionManager::Instance()->FindAgentServer(user->GetChannelID());
    if (NULL == pAgentServer) {
        SUNLOG(eCRITICAL_LOG, "[Lotto] ERROR : No AgentServer Found");
        return false;
    }
    pAgentServer->Send(msg, wSize);
    return true;
}
