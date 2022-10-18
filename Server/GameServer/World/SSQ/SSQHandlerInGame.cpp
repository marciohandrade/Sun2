#include "stdafx.h"

#include <PacketStruct_DG.h>

#include <SSQ/SSQInfoStruct.h>
#include <SSQ/SSQPacketProcS2S.h>

#include "SSQHandlerInGame.h"

#include "Player.h"
#include "ItemManager.h"

BOOST_STATIC_ASSERT(SSQInfo::MI_SUCCESS_POINT_BY_GRADE == SSQHandlerInGame::eOrder_None);

//==================================================================================================
//
SSQHandlerInGame::SSQHandlerInGame()
    : m_Order(eOrder_None)
    , m_ObserverCoordinateIndex(nsSSQ::Observer::Info::eCoord_Slots)
    , m_BonusExpRatio(0)
{
}


SSQHandlerInGame::~SSQHandlerInGame()
{
}


BOOLEAN
SSQHandlerInGame::IsExistRewardInfo() const
{
    SSQ_RESERVED_REWARD_INFO info;
    return GetRewardInfo(info);
}


BOOLEAN
SSQHandlerInGame::SetInfo_Empty()
{
    ZeroMemory(&m_RewardInfo, sizeof(m_RewardInfo));
    m_RewardInfo.m_SharedInfo.EventID = EVENT_MULTIUSE_EMPTYVALUE_FOR_SSQ;
    return TRUE;
}

BOOLEAN
SSQHandlerInGame::SetInfo_WinnerReward(const DWORD point, const DWORD exp,
                                       const BYTE mobkills, const BYTE playerkills,
                                       const SLOTCODE reward_itemcode)
{
    ZeroMemory(&m_RewardInfo, sizeof(m_RewardInfo));
    SSQ_RESERVED_REWARD_INFO::WinnerReward& rReward = m_RewardInfo.m_WinnerReward;
    rReward.EventID         = eEVENT_MULTIUSE_SSQ_WINNER_REWARD;
    rReward.Point           = point;
    rReward.Exp             = exp;
    rReward.MonsterKillNum  = mobkills;
    rReward.PlayerKillNum   = playerkills;
    rReward.ItemCode        = reward_itemcode;
    rReward.Status          = FALSE;
    return TRUE;
}

BOOLEAN
SSQHandlerInGame::SetInfo_QuestFailed(const DWORD point, const DWORD exp,
                                      const BYTE mobkills, const BYTE playerkills)
{
    ZeroMemory(&m_RewardInfo, sizeof(m_RewardInfo));
    SSQ_RESERVED_REWARD_INFO::QuestFail& rReward = m_RewardInfo.m_QuestFail;
    rReward.EventID         = eEVENT_MULTIUSE_SSQ_QUEST_FAIL_INFO;
    rReward.Point           = point;
    rReward.Exp             = exp;
    rReward.MonsterKillNum  = mobkills;
    rReward.PlayerKillNum   = playerkills;
    rReward.Status          = FALSE;
    return TRUE;
}

BOOLEAN
SSQHandlerInGame::SetInfo_Refund(const WORD itemCode, const DWORD heim)
{
    ASSERT(itemCode);
    ZeroMemory(&m_RewardInfo, sizeof(m_RewardInfo));
    SSQ_RESERVED_REWARD_INFO::AdmissionFeeRefund& rRefund = m_RewardInfo.m_AdmissionFeeRefund;
    rRefund.EventID     = eEVENT_MULTIUSE_SSQ_REFUND;
    rRefund.ItemCode    = itemCode;
    rRefund.Money       = heim;
    rRefund.Status      = FALSE;
    return TRUE;
}


BOOLEAN
SSQHandlerInGame::UpdateInfo_PlayPoint(const DWORD point, const DWORD exp,
                                       const BYTE mobkills, const BYTE playerkills)
{
    const eEVENT_MODE_MULTIUSE mode = (eEVENT_MODE_MULTIUSE)(m_RewardInfo.m_SharedInfo.EventID);
    // 퀘스트 실패 및 성공 중인 정보여야만 한다.
    switch(mode)
    {
    case eEVENT_MULTIUSE_SSQ_QUEST_FAIL_INFO:
        {
            SSQ_RESERVED_REWARD_INFO::QuestFail& rUpdate = m_RewardInfo.m_QuestFail;
            if(point)         rUpdate.Point           = point;
            if(exp)           rUpdate.Exp             = exp;
            if(mobkills)      rUpdate.MonsterKillNum  = mobkills;
            if(playerkills)   rUpdate.PlayerKillNum   = playerkills;
        }
        return TRUE;
    case eEVENT_MULTIUSE_SSQ_WINNER_REWARD:
        {
            SSQ_RESERVED_REWARD_INFO::WinnerReward& rUpdate = m_RewardInfo.m_WinnerReward;
            if(point)         rUpdate.Point           = point;
            if(exp)           rUpdate.Exp             = exp;
            if(mobkills)      rUpdate.MonsterKillNum  = mobkills;
            if(playerkills)   rUpdate.PlayerKillNum   = playerkills;
        }
        return TRUE;
    }
    ASSERT(!"Unexpected Status [" __FUNCTION__ "]");
    return FALSE;
}


BOOLEAN
SSQHandlerInGame::UpdateInfo_ChangeToWinner()
{
    const eEVENT_MODE_MULTIUSE mode = (eEVENT_MODE_MULTIUSE)(m_RewardInfo.m_SharedInfo.EventID);
    if(FlowControl::FCAssert(mode == eEVENT_MULTIUSE_SSQ_QUEST_FAIL_INFO))
    {
        SSQ_RESERVED_REWARD_INFO::QuestFail fail_info = m_RewardInfo.m_QuestFail;
        SSQ_RESERVED_REWARD_INFO::WinnerReward& rUpdate = m_RewardInfo.m_WinnerReward;
        rUpdate.EventID         = eEVENT_MULTIUSE_SSQ_WINNER_REWARD;
        //
        rUpdate.Point           = fail_info.Point;
        rUpdate.Exp             = fail_info.Exp;
        rUpdate.MonsterKillNum  = fail_info.MonsterKillNum;
        rUpdate.PlayerKillNum   = fail_info.PlayerKillNum;
        rUpdate.ItemCode        = 0;
        return TRUE;
    }
    return FALSE;
}


BOOLEAN
SSQHandlerInGame::GetInfo_QuestFail(SSQ_RESERVED_REWARD_INFO::QuestFail& rInfo)
{
    const eEVENT_MODE_MULTIUSE mode = (eEVENT_MODE_MULTIUSE)(m_RewardInfo.m_SharedInfo.EventID);
    if(FlowControl::FCAssert(mode == eEVENT_MULTIUSE_SSQ_QUEST_FAIL_INFO))
    {
        rInfo = m_RewardInfo.m_QuestFail;
        return TRUE;
    }
    return FALSE;
}


BOOLEAN
SSQHandlerInGame::GetInfo_QuestWinner(SSQ_RESERVED_REWARD_INFO::WinnerReward& rInfo)
{
    const eEVENT_MODE_MULTIUSE mode = (eEVENT_MODE_MULTIUSE)(m_RewardInfo.m_SharedInfo.EventID);
    if(FlowControl::FCAssert(mode == eEVENT_MULTIUSE_SSQ_WINNER_REWARD))
    {
        rInfo = m_RewardInfo.m_WinnerReward;
        return TRUE;
    }
    return FALSE;
}


BOOLEAN
SSQHandlerInGame::Send_RewardProcess(Player* const pPlayer, const BOOLEAN bShowUI)
{
    const DWORD NEED_LEAVE_TRANSACTION = 10;
    //  Work to do
    //  1. check db transaction
    //  2. Player inventory control
    //  3. update db info
    //////////////////////////////////////////////////////////////////////////
    //  <LOGIC CONTROL>
    FlowControl flow;
    FLOWCONTROL_START()
    {
        const BOOLEAN isTransaction = IsTransaction();
        if(isTransaction)
            return FALSE;

        MSG_CG_ZONE_EX_SSQ_REWARD_OR_REFUND_INFO_CMD msgCMD;
        if(!GetRewardInfo(msgCMD.m_RewardInfo))
            return FALSE;

        RC::eITEM_RESULT result = RC::RC_ITEM_SUCCESS;

        const eEVENT_MODE_MULTIUSE reward_type =
            eEVENT_MODE_MULTIUSE(msgCMD.m_RewardInfo.m_SharedInfo.EventID);
        switch(reward_type)
        {
        case eEVENT_MULTIUSE_SSQ_REFUND:
            {   //  환불 처리
                SSQ_RESERVED_REWARD_INFO::AdmissionFeeRefund& rInfo =
                    msgCMD.m_RewardInfo.m_AdmissionFeeRefund;
                SLOTCODE itemCode = rInfo.ItemCode;
                POSTYPE itemCount = 1;

                ItemManager* pItemManager = pPlayer->GetItemManager();
                result = pItemManager->ObtainAllKindOfItem(1, &itemCode, &itemCount,
                                                           &msgCMD.m_Refund.TotalInfo);
                msgCMD.m_Refund.Result  = result;

                if(result == RC::RC_ITEM_SUCCESS)
                {
                    pPlayer->PlusMoney(rInfo.Money);
                    msgCMD.m_Refund.ChangedTotalHeim = pPlayer->GetMoney();
                    pPlayer->SendPacket(&msgCMD, msgCMD.GetSize());

                    GAMELOG->LogMoney(MONEY_REFUND_SSQ_ADMISSION_FEE, pPlayer, rInfo.Money);
                    GAMELOG->LogMultiItem(ITEM_SSQ_ENTRANCE_ITEM_REFUND, pPlayer,
                                          itemCount, &itemCode, &itemCount);
                    SUNLOG(eCRITICAL_LOG,
                           _T("|Msg=ssq refund process success|")
                           _T("|CharGuid=%d|CharName=%s|ItemCode=%d|Money=%d|"),
                           pPlayer->GetCharGuid(), pPlayer->GetCharName(), itemCode, rInfo.Money);
                }
                else
                {
                    SUNLOG(eCRITICAL_LOG,
                           _T("|Msg=ssq refund process failed|")
                           _T("|CharGuid=%d|CharName=%s|ItemCode=%d|Money=%d|"),
                           pPlayer->GetCharGuid(), pPlayer->GetCharName(), itemCode, rInfo.Money);
                }
            }
            break;
        case eEVENT_MULTIUSE_SSQ_QUEST_FAIL_INFO:
            {   //  정보 알림 형태 처리
                pPlayer->SendPacket(&msgCMD, msgCMD.GetSize());
            }
            break;
        case eEVENT_MULTIUSE_SSQ_WINNER_REWARD:
            {
                SSQ_RESERVED_REWARD_INFO::WinnerReward& rInfo = msgCMD.m_RewardInfo.m_WinnerReward;

                if(bShowUI)
                    pPlayer->SendPacket(&msgCMD, msgCMD.GetSize());

                // 아직 보상 받지 못한 아이템이 존재한다.
                BOOLEAN exist_item = !!msgCMD.m_RewardInfo.m_WinnerReward.ItemCode;

                if(exist_item)
                {
                    SLOTCODE itemCode = rInfo.ItemCode;
                    POSTYPE itemCount = 1;

                    ItemManager* pItemManager = pPlayer->GetItemManager();

                    MSG_CG_ZONE_EX_SSQ_WINNER_REWARD_CMD msgReward;
                    result = pItemManager->ObtainAllKindOfItem(1, &itemCode, &itemCount,
                                                               &msgReward.m_TotalInfo);
                    msgReward.m_Result  = result;
                    pPlayer->SendPacket(&msgReward, msgReward.GetSize());
                }
            }
            break;
        }

        if(result == RC::RC_ITEM_SUCCESS)
        {
            MSG_DG_EVENT_SSQ_REWARD_UPDATE_SYN msgSYN;
            msgSYN.m_dwKey = pPlayer->GetUserKey();
            msgSYN.m_CharGuid = pPlayer->GetCharGuid();

            msgSYN.m_RewardInfo = msgCMD.m_RewardInfo;
            msgSYN.m_RewardInfo.m_SharedInfo.Status = TRUE;

            g_pGameServer->SendToServer(GAME_DBPROXY, &msgSYN, msgSYN.GetSize());

            Rewarded();

            //BeginTransaction();
        }

        return TRUE;
    }
    FLOWCONTROL_END;

    // send to ack : fail result...?


    return FALSE;
}


//BOOLEAN
//SSQHandlerInGame::Recv_DBRewardUpdated(Player* const pPlayer)
//{
//  if(!IsDBTransaction())
//      return FALSE; //...
//
//  SSQ_RESERVED_REWARD_INFO info;
//  if(!GetRewardInfo(info))
//      return FALSE;
//
//  SLOTCODE itemCode = 0;
//  POSTYPE itemCount = 0;
//  MONEY heim = 0;
//  BYTE* pStatus = 0;
//
//  const eEVENT_MODE_MULTIUSE reward_type = (eEVENT_MODE_MULTIUSE)info.m_SSQRewardType;
//  switch(reward_type)
//  {
//  case eEVENT_MULTIUSE_SSQ_WINNER_REWARD:
//      {
//          SSQ_RESERVED_REWARD_INFO::WinnerReward& rInfo = info.m_WinnerReward;
//          itemCode = rInfo.ItemCode;
//          itemCount = rInfo.Count;
//          pStatus = &rInfo.Status;
//      }
//      break;
//  case eEVENT_MULTIUSE_SSQ_REFUND:
//      {
//          SSQ_RESERVED_REWARD_INFO::AdmissionFeeRefund& rInfo = info.m_AdmissionFeeRefund;
//          itemCode = rInfo.ItemCode;
//          itemCount = 1;
//          heim = rInfo.Money;
//          pStatus = &rInfo.Status;
//      }
//      break;
//  }
//
//  if(!pStatus)
//      return FALSE;   // ... critical
//
//  Rewarded(reward_type);
//
//  EndDBTransaction();
//  return TRUE;
//}


BOOLEAN
SSQHandlerInGame::ConfigEvent_OnDead(Player* const pPlayer, const DWORD leave_delay)
{
    BOOLEAN isDead     = !!(ControlStatus.FIELD & ControlStatus.CHECKED_DEAD);
    BOOLEAN need_event =  !(ControlStatus.FIELD & ControlStatus.CHECKED_DEAD_TIMER);

    //  isDead      need_event
    //      0           0       (none)
    //      0           1       (x)
    //      1           0       (none)
    //      1           1       (reg event)

    if(isDead & need_event)
    {
        m_DeadTimer.SetTimer(leave_delay, true, false);
        ControlStatus.SetField(ControlStatus.FIELD | ControlStatus.CHECKED_DEAD_TIMER);
        return TRUE;
    }
    return TRUE;
//  ASSERT(!"Unexpected Status");
//  return FALSE;
}


BOOLEAN
SSQHandlerInGame::QueryEvent_NeedLeaveProcess(Player* const pPlayer)
{
    BOOLEAN working_deadProcess = !!(ControlStatus.FIELD & ControlStatus.CHECKED_DEAD_TIMER);

    if(working_deadProcess)
    {
        if(!(ControlStatus.FIELD & ControlStatus.CHECKED_DEAD_TIMER_EXPIRED))
        {
            if(m_DeadTimer.IsExpired())
            {
                ControlStatus.AddField(ControlStatus.CHECKED_DEAD_TIMER_EXPIRED);
                return TRUE;
            }
        }
        //return !!(ControlStatus.FIELD & ControlStatus.CHECKED_DEAD_TIMER_EXPIRED);
    }

    return FALSE;
}


//==================================================================================================


ISSQHandlerInGame::~ISSQHandlerInGame()
{
    Release();
}


VOID
ISSQHandlerInGame::Init()
{
    if (m_pHandler == NULL) {
        m_pHandler = TAllocNew(SSQHandlerInGame);
    };
}


VOID
ISSQHandlerInGame::Release()
{
    if (m_pHandler) {
        TAllocDelete(SSQHandlerInGame, m_pHandler);
    };
    m_pHandler = 0;
    m_ItemCode = 0;
}


