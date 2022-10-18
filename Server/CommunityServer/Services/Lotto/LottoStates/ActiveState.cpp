#include "WorldServerPch.hxx"
#include "./ActiveState.h"

#include "PacketStruct_AW.h"
#include "PacketStruct_DW.h"

#include "Services/Lotto/Lotto.h"

//==================================================================================================

// implemented by __NA_001359_20090619_HEIM_LOTTO_SYSTEM

void LottoActiveState::OnEnter()
{
    SUNLOG(eFULL_LOG, "[Lotto] Entering ACTIVE_STATE");

    totalAccum_ = 0;

    accumNotiTimer_.SetTimer(10 * 1000); // 10��
    accumNotiTimer_.EnableCheckTime();

    // ���� ��÷ ���� ���
    lotto_.config_.ScheduleNextDraw(schedule_);
    if (schedule_.IsValid()) {
        tickTimer_.SetTimer(0); // ���� 1ȸ�� ������ Expired ���·�
        tickTimer_.EnableCheckTime();
        reminderIndex_ = 0;
    }
    else {
        SUNLOG(eCRITICAL_LOG, "[Lotto] ERROR scheduling the next draw");
    }

#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
    // ���۽� ���� Ʈ����� ���� �ʱ�ȭ
    lotto_.GetTicketActivationXactions().Clear();
#endif

    // ���� �ζ� ���� ����
    int currentDrawNum = lotto_.GetCurrentDrawNum();
    if (currentDrawNum != 0) {
        BroadcastLottoStart(currentDrawNum);
    }

    // ���� ������ ����
    BroadcastTotalAccum(lotto_.GetCurrentTotalAccum(), false);
}

void LottoActiveState::OnExit()
{
    SUNLOG(eFULL_LOG, "[Lotto] Exiting ACTIVE_STATE");

    // �� ȸ�� ���� ������ ����
    BroadcastTotalAccum(lotto_.GetCurrentTotalAccum());

    if (schedule_.IsValid()) {
        tickTimer_.DisableCheckTime();
    }

    accumNotiTimer_.DisableCheckTime();
}

void LottoActiveState::OnTick()
{
    if (schedule_.IsValid()) {
        if (tickTimer_.IsExpired())
        {
            SYSTEMTIME st;
            util::TimeSync::GetLocalTime(&st);

            // ���� ��÷ �������� ���� �ð�(��)�� ����Ѵ�
            int remainingSeconds = schedule_.SecondsTo(&st);

            DWORD timerInterval = tickTimer_.GetBaseIntervalTime();
            if (timerInterval == 0 || timerInterval > 1000) {
                SUNLOG(eFULL_LOG, "[Lotto] Next draw on %s : %d seconds left", \
                    schedule_.ToString(), remainingSeconds);
            }

            // ���� ��÷���� ���� �ð��� ���� ������
            if (remainingSeconds > 0)
            {
                const ReminderList& reminders = lotto_.config_.GetReminders();
                // ���� ī��Ʈ�ٿ� ���� ���� �ð�(��)���� ���� �������� ī��Ʈ�ٿ� ����
                if ( reminderIndex_ < reminders.size() && remainingSeconds <= reminders[reminderIndex_] )
                {
                    // 10% �̻� ���� ������ ������ �ʴ´�
                    if ( remainingSeconds > (reminders[reminderIndex_] - (int)(reminders[reminderIndex_] * 0.1f)) )
                    {
                        SUNLOG(eFULL_LOG, "[Lotto] Countdown #%d : %d second(s) remaining", \
                            (reminderIndex_ + 1), reminders[reminderIndex_]);
                        
                        BroadcastCountdown(reminders[reminderIndex_]);
                    }
                    else {
                        SUNLOG(eFULL_LOG, "[Lotto] Skipping obsolete countdown #%d : %d second(s)", \
                            (reminderIndex_ + 1), reminders[reminderIndex_]);
                    }
                    ++reminderIndex_;
                }
            }
            // ��÷ �ð��� �Ǿ�����
            else {
                SUNLOG(eFULL_LOG, "[Lotto] Starting scheduled draw");

                lotto_.TransitState(Lotto::DRAWING_STATE);
            }

            // ��÷ �ð� üũ Ÿ�̸� ����
            if (timerInterval > 1000) {
                tickTimer_.Reset();
            }
            else if (timerInterval > 0) {
                if (st.wSecond > 0) {
                    tickTimer_.Reset();
                }
                else {
                    tickTimer_.SetTimer(60 * 1000); // üũ �ֱ⸦ 1������
                }
            }
            else {
                tickTimer_.SetTimer(1000); // ó���� 1�� ��������
            }
        }
    }

    if (accumNotiTimer_.IsExpired())
    {
        INT64 totalAccum = lotto_.GetCurrentTotalAccum();

        // �� �����׿� ������ ������ 
        if (totalAccum != totalAccum_)
        {
            BroadcastTotalAccum(totalAccum);

            totalAccum_ = totalAccum; // ���� ������ ���� ���� �� ���
        }
        
        // ������ ���� Ÿ�̸� ����
        accumNotiTimer_.Reset();
    }
}

void LottoActiveState::OnInfoRequest(CHARGUID charGuid, const MSG_CW_LOTTO_INFO_SYN* packet)
{
    LottoEntry* entry = lotto_.FindEntry(charGuid);
    if (entry == NULL) {
        SUNLOG(eMIDDLE_LOG, "[Lotto] WARNING : Entry not found for character [%d]", charGuid);
        RequestEntryLoad(charGuid, packet->bFlag);
        return;
    }
    SendInfoResponse(charGuid, packet->bFlag);
}

void LottoActiveState::OnEntryRequest(CHARGUID charGuid, const MSG_CW_LOTTO_ENTRY_SYN* packet)
{
    int drawNum       = packet->wDrawNum;
    bool isAuto       = (packet->bAuto != 0);
    BYTE pickedNum[_countof(packet->bPickedNum)];
    BOOST_STATIC_ASSERT(_countof(pickedNum) == LOTTO_NUM_PICKS &&
                        sizeof(pickedNum[0]) == sizeof(packet->bPickedNum[0]));
    CopyMemory(pickedNum, packet->bPickedNum, sizeof(pickedNum));

    lotto_.Sort(pickedNum); // ���� ��ȣ ����

    int errorCode = MSG_CW_LOTTO_ENTRY_NAK::UNKNOWN;
    bool result = true;
    if (!isAuto) { 
        result = Lotto::AreValidPicks(pickedNum); // ���� ��ȣ ���� �� �ߺ� �˻�
    }

#ifndef _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
    // ���ǿ��� ���� ���� ����� ������� ���� ���� ���⼭ �ٷ� ���� ó��
    int status;
    if (result) {
        result = lotto_.ProcessTicket(charGuid, drawNum, isAuto, pickedNum, &status, &errorCode);
        if (result) {
            RequestEntrySave(charGuid, packet->bAuto, (BYTE)status, pickedNum);
        }
    }
#else
    // ���ǿ��� ���� ���� ����� ����� ����
    // ���� ���� Ƚ�� ����
    LottoEntry* entry = lotto_.FindEntry(charGuid);
    if (entry != NULL)
    {
        if (entry->numUsedTickets >= LottoConfig::MAX_TICKETS)
        {
            errorCode = MSG_CW_LOTTO_ENTRY_NAK::OUT_OF_TICKET;
            result = false;
        }
    }
    // �ܾ� ������ ���� ���� ������ ����
    if (result)
    {
        MSG_AW_USER_LOTTO_TICKET_PURCHASE_SYN syn;
        syn.iCharGuid = charGuid;
        syn.wDrawNum = (WORD)drawNum;
        syn.bAuto = (isAuto ? 1 : 0);
        for (int i = 0; i < LOTTO_NUM_PICKS; ++i) {
            syn.bPickedNum[i] = pickedNum[i];
        }
        syn.i64TicketPrice = lotto_.config_.GetTicketPrice();
        result = SendToAgentServer(charGuid, (BYTE*)&syn, (WORD)sizeof(syn));
    }
#endif // _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE

    if (!result)
    {
        MSG_CW_LOTTO_ENTRY_NAK nak;
        nak.iCharGuid = charGuid;
        nak.bReason = (BYTE)errorCode;
        SendToClient(charGuid, &nak, (WORD)sizeof(nak));
    }
}

#ifdef _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
void LottoActiveState::OnTicketPurchase(MSG_BASE* packet)
{
    MSG_AW_USER_LOTTO_TICKET_PURCHASE_ACK* packet = \
        static_cast<MSG_AW_USER_LOTTO_TICKET_PURCHASE_ACK*>(packet);

    int result_code = packet->iResult;
    CHARGUID charGuid = packet->iCharGuid;
    int drawNum = packet->wDrawNum;
    bool isAuto = (packet->bAuto != 0);
    BYTE* pickedNum = packet->bPickedNum;

    int error_code = MSG_CW_LOTTO_ENTRY_NAK::UNKNOWN;
    bool result = false;

    if (result_code == MSG_AW_USER_LOTTO_TICKET_PURCHASE_ACK::RESULT_OK)
    {
        LottoEntry* entry = lotto_.FindEntry(charGuid);
        if (entry != NULL)
        {
            // ���� ���� �ӽ� ����
            entry->temp.drawNum = drawNum;
            entry->temp.status = (isAuto ? 1 : 0);
            entry->temp.pickedNum1 = pickedNum[0];
            entry->temp.pickedNum2 = pickedNum[1];
            entry->temp.pickedNum3 = pickedNum[2];
            entry->temp.pickedNum4 = pickedNum[3];
            // ���ǿ��� ȹ��
            RequestTicketAdd(charGuid, 1);
            result = true;
        }
    }
    else
    {
        switch (result_code)
        {
        case MSG_AW_USER_LOTTO_TICKET_PURCHASE_ACK::ERROR_NOT_ENOUGH_MONEY:
            error_code = MSG_CW_LOTTO_ENTRY_NAK::NOT_ENOUGH_MONEY;
            break;
        default:
            break;
        }
    }

    if (!result)
    {
        MSG_CW_LOTTO_ENTRY_NAK nak;
        nak.iCharGuid = charGuid;
        nak.bReason = (BYTE)error_code;
        SendToClient(charGuid, (BYTE*)&nak, (WORD)sizeof(nak));
    }
}
#endif // _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE

#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
// ĳ�� �������� ����� �ζ� ���� ���� Ȱ��ȭ
void LottoActiveState::OnTicketActivation(MSG_BASE* packet)
{
    MSG_AW_ITEM_USE_HEIM_LOTTO_TICKET_SYN* msg = \
        static_cast<MSG_AW_ITEM_USE_HEIM_LOTTO_TICKET_SYN*>(packet);

    LottoEntry* entry = lotto_.FindEntry(msg->char_guid);
    if (entry == NULL)
    {
        MSG_AW_ITEM_USE_HEIM_LOTTO_TICKET_ACK ack;
        ack.result_code = static_cast<WORD>(RC::RC_ITEM_HEIM_LOTTO_UNAVAILABLE);
        ack.char_guid   = msg->char_guid;
        ack.index       = msg->index;
        ack.pos         = msg->pos;
        SendToAgentServer(msg->char_guid, (BYTE*)&ack, (WORD)sizeof(ack));
        return;
    }

    // �� ȸ���� ����� ���� ���� ��� ������ ���� ���� ���� �ִ밪�� ���������� ����
    int num_total_tickets = entry->numRemainingTickets + entry->numUsedTickets;
    if  (num_total_tickets >= LottoConfig::MAX_TICKETS)
    {
        MSG_AW_ITEM_USE_HEIM_LOTTO_TICKET_ACK ack;
        ack.result_code = static_cast<WORD>(RC::RC_ITEM_HEIM_LOTTO_TICKET_MAX_REACHED);
        ack.char_guid   = msg->char_guid;
        ack.index       = msg->index;
        ack.pos         = msg->pos;
        SendToAgentServer(msg->char_guid, (BYTE*)&ack, (WORD)sizeof(ack));
        return;
    }

    // Ʈ����� ���� ����
    TicketActivationXactions& xactions = lotto_.GetTicketActivationXactions();
    xactions.Add(msg->char_guid, msg->index, msg->pos);

    RequestTicketAdd(msg->char_guid, entry->numRemainingTickets + 1);
}
#endif // _NA_20101011_HEIM_LOTTO_CASH_TICKET

void LottoActiveState::OnEntrySave(const MSG_DW_LOTTO_SAVE_ENTRY_ACK* packet)
{
    CHARGUID charGuid = packet->iCharGuid;
    int drawNum = packet->wDrawNum;
    int status = packet->bStatus;
    const BYTE* pickedNum = packet->bPickedNum;
    
    if (packet->iResult != 0) {
        SUNLOG(eCRITICAL_LOG, "[Lotto] ERROR : Failed to save an entry of character [%d]", charGuid);
        MSG_CW_LOTTO_ENTRY_NAK nak;
        nak.iCharGuid = charGuid;
        nak.bReason = (BYTE)MSG_CW_LOTTO_ENTRY_NAK::UNKNOWN;
        SendToClient(charGuid, &nak, (WORD)sizeof(MSG_CW_LOTTO_ENTRY_NAK));
        return;
    }

    lotto_.EnterTicket(charGuid, drawNum, status, pickedNum);
    LottoEntry* entry = lotto_.FindEntry(charGuid);
    if (entry == NULL) {
        SUNLOG(eMIDDLE_LOG, "[Lotto] WARNING : Entry not found for character [%d]", charGuid);
        return;
    }

    MSG_CW_LOTTO_ENTRY_ACK ack;
    ack.iCharGuid = charGuid;
    ack.wDrawNum = (WORD)drawNum;
    ack.bNumTickets = (BYTE)entry->numRemainingTickets;
    ack.bAuto = packet->bIsAuto;
    for (int i = 0; i < LottoConfig::NUM_PICKS; ++i) {
        ack.bPickedNum[i] = pickedNum[i];
    }
#ifdef _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
    ack.i64TicketPrice = lotto_.config_.GetTicketPrice();
#endif
    SendToClient(charGuid, &ack, (WORD)sizeof(ack));
}

void LottoActiveState::OnTicketAcquisition(const MSG_DW_LOTTO_ADD_TICKET_ACK* packet)
{
    CHARGUID charGuid = packet->iCharGuid;

    if (packet->iResult != 0) {
        SUNLOG(eCRITICAL_LOG, "[Lotto] ERROR : Failed to give ticket(s) to character [%d]", charGuid);
        return;
    }

    LottoEntry* entry = lotto_.FindEntry(charGuid);
    if (entry == NULL) {
        SUNLOG(eCRITICAL_LOG, "[Lotto] ERROR : Entry not found for character [%d]", charGuid);
        return;
    }

    int numRemainingTickets = packet->bNumRemainingTickets;
    // DB ó���� �Ϸ�Ǹ� ĳ������ ������ ���� ���� ������ �����ϰ�
    entry->numRemainingTickets = (WORD)numRemainingTickets;

#ifndef _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
    // Ŭ���̾�Ʈ�� �˷��ش�
    NotifyTicketAcquisition(charGuid, numRemainingTickets);
  #ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
    // Agent ������ ĳ�� ������ ��뿡 ���� ���� ����
    TicketActivationXactions& xactions = lotto_.GetTicketActivationXactions();
    const TicketActivationXactionInfo* xaction_info = xactions.Find(charGuid);
    MSG_AW_ITEM_USE_HEIM_LOTTO_TICKET_ACK ack;
    if (xaction_info != NULL)
    {
        ack.result_code = RC::RC_ITEM_SUCCESS;
        ack.index       = xaction_info->index;
        ack.pos         = xaction_info->pos;
    }
    else
    {
        ack.result_code = RC::RC_ITEM_INVALIDSTATEOFPLAYER;
    }
    ack.char_guid = charGuid;
    SendToAgentServer(charGuid, (BYTE*)&ack, (WORD)sizeof(ack));
    // Ʈ����� ���� ����
    xactions.Remove(charGuid);
  #endif // _NA_20101011_HEIM_LOTTO_CASH_TICKET
#else
    // �ӽ� ����� ���� ���� ��������
    int draw_num = entry->temp.drawNum;
    bool is_auto = (entry->temp.status != 0);
    BYTE picked_num[4];
    picked_num[0] = entry->temp.pickedNum1;
    picked_num[1] = entry->temp.pickedNum2;
    picked_num[2] = entry->temp.pickedNum3;
    picked_num[3] = entry->temp.pickedNum4;

    // ���� ó��
    int status, error_code;
    bool result = lotto_.ProcessTicket(charGuid, draw_num, is_auto, picked_num, &status, &error_code);
    if (result) {
        RequestEntrySave(charGuid, (is_auto ? 1 : 0), (BYTE)status, picked_num);
    }
    if (!result)
    {
        MSG_CW_LOTTO_ENTRY_NAK nak;
        nak.iCharGuid = charGuid;
        nak.bReason = (BYTE)error_code;
        SendToClient(charGuid, (BYTE*)&nak, (WORD)sizeof(nak));
    }
#endif
}

void LottoActiveState::OnHeimConsumption(const MSG_AW_USER_STATUS_HEIM_CONSUMPTION_NTF* packet)
{
    CHARGUID charGuid = packet->m_CharGuid;
    eHEIM_CONSUMPTION_TYPE type = (eHEIM_CONSUMPTION_TYPE)packet->m_byConsumptionType;
    INT64 amount = (INT64)packet->m_Amount;
    int percentage = -1;
    //_NA_0_20091119_HEIM_LOTTO_ACCUM_CTRL_BY_SHOP
    percentage = packet->m_LottoAccumRatio;

    int numRemainingTickets = \
        lotto_.OnHeimConsumption(charGuid, type, amount, percentage);

    if (numRemainingTickets >= 0) {
        RequestTicketAdd(charGuid, numRemainingTickets);
    }
}

void LottoActiveState::OnTotalAccumChange(INT64 delta)
{
    INT64 totalAccum = lotto_.AddCurrentTotalAccum(delta);

    // �� �����׿� ������ ������ 
    if (totalAccum != totalAccum_)
    {
        BroadcastTotalAccum(totalAccum);

        totalAccum_ = totalAccum; // ���� ������ ���� ���� �� ���
    }

    // ������ ���� Ÿ�̸� ����
    accumNotiTimer_.Reset();
}

void LottoActiveState::BroadcastTotalAccum(INT64 totalAccum, bool dbWrite)
{
    SUNLOG(eFULL_LOG, "[Lotto] Notifying Total Accumulation Update : Draw # = %d, Total = %I64d", 
        lotto_.GetCurrentDrawNum(), totalAccum);

    if (dbWrite) {
        // DB �� ������ ������Ʈ ��û
        RequestAccumulate((WORD)lotto_.GetCurrentDrawNum(), totalAccum);
    }

    // Ŭ���̾�Ʈ�鿡�� ����
    MSG_CW_LOTTO_ACCUM_BRD sendMsg;
    sendMsg.wDrawNum = (WORD)lotto_.GetCurrentDrawNum();
    sendMsg.i64TotalAccum = totalAccum;
    BroadcastToClients(&sendMsg, (WORD)sizeof(MSG_CW_LOTTO_ACCUM_BRD));
}

