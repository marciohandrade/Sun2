#include "stdafx.h"
#include <PacketStruct_AG.h>

#include <UserSessions/User.h>
#include <UserSessions/UserManager.h>

#include <ServerSessions/ServerSessionManager.h>
#include <ServerSessions/BattleServerSession.h>
#include <ServerSessions/FieldServerSession.h>

#include <Zone/DungeonSSQ.h>
#include <Zone/ZoneManager.h>
#include <Party/PartyManager.h>

#include <Zone/WarControlManager.h>
#include "ISSQHandler.h"
#include "SSQControlInfos.hxx"
#include "SSQUpdateCtrl.hxx"
#include "SSQPacketProcess.hxx"
#include "SSQControlManagerImpl.hxx"

//==================================================================================================
//  <ISSQHandler>
//
ISSQHandler::ISSQHandler()
    : m_eState(ISSQHandler::USR_NONE_STATE)
{
    m_Timer.Disabled();
    ZeroMemory(&m_FLD_USR_INFO, sizeof(m_FLD_USR_INFO));
    ClearDupInfo();
}

ISSQHandler::~ISSQHandler()
{
}

//template< ISSQHandler::eSTATE state >
//BOOLEAN
//ISSQHandler::ChangeStatus(const State<state>& state_info)
//{
//  return FALSE;
//}

BOOLEAN
ISSQHandler::ChangeStatus(const State<MONSTER_STATE>& state_info)
{
    m_eState = state_info.value;
    m_Timer.Disabled();
    m_FLD_USR_INFO.m_pUser = 0;
    m_FLD_USR_INFO.m_IsPlayer = state_info.INFO.m_IsPlayer;
    m_FLD_USR_INFO.m_ObjectKey = state_info.INFO.m_ObjectKey;
    m_FLD_USR_INFO.m_MsgRouter = state_info.INFO.m_MsgRouter;
    return TRUE;
}

BOOLEAN
ISSQHandler::ChangeStatus(const State<USR_REGISTERED_SSQ_INDEXER>& state_info)
{
    m_eState = state_info.value;
    m_Timer.SetNextTimeoutByCurTickBased(state_info.timeout);

    m_FLD_USR_INFO = state_info.INFO;

    //_NA_20110630_BATTLE_ZONE_RENEWAL

    WAVERIX_DEBUG_CODE(
        SUNLOG(eCRITICAL_LOG, "[USR_REGISTERED_SSQ_INDEXER] - timeout %u", state_info.timeout);
    );
    return TRUE;
}

BOOLEAN
ISSQHandler::ChangeStatus(const State<USR_PORTAL_OBJECT_CREATED>& state_info)
{
    m_eState = state_info.value;
    m_Timer.SetNextTimeoutByCurTickBased(state_info.timeout);

    m_Dup.FLD_PORTAL_OBJECT = state_info.INFO;

    WAVERIX_DEBUG_CODE(
        SUNLOG(eCRITICAL_LOG, "[USR_PORTAL_OBJECT_CREATED] - timeout %u", state_info.timeout);
    );
    return TRUE;
}

BOOLEAN
ISSQHandler::ChangeStatus(const State<USR_PORTAL_TRANSACTION>& state_info)
{
    m_eState = state_info.value;
    m_Timer.SetNextTimeoutByCurTickBased(state_info.timeout);

    m_Dup.FLD_PORTAL_TRANS = state_info.INFO;

    WAVERIX_DEBUG_CODE(
        SUNLOG(eCRITICAL_LOG, "[USR_PORTAL_TRANSACTION] - timeout %u", state_info.timeout);
    );
    return TRUE;
}

BOOLEAN
ISSQHandler::ChangeStatus(const State<USR_ENTERING_SSQ_FIELD>& state_info)
{
    ASSERT(USR_PORTAL_TRANSACTION == m_eState);

    User* const pUser = m_FLD_USR_INFO.m_pUser;
    //_NA_20110630_BATTLE_ZONE_RENEWAL

    m_eState = state_info.value;
    m_Timer.SetNextTimeoutByCurTickBased(state_info.timeout);

    m_Dup.FLD_SSQ_FIELD_ENTTRANS = state_info.INFO;

    WAVERIX_DEBUG_CODE(
        SUNLOG(eCRITICAL_LOG, "[USR_ENTERING_SSQ_FIELD] - timeout %u", state_info.timeout);
    );
    return TRUE;
}

BOOLEAN
ISSQHandler::ChangeStatus(const State<USR_ENTERED_SSQ_FIELD>& state_info)
{
    //////////////////////////////////////////////////////////////////////////
    //  새로운 상태를 위한 정보 설정
    m_eState = state_info.value;
    m_Timer.Disabled();

    m_Dup.FLD_SSQ_FIELD = state_info.INFO;

    WAVERIX_DEBUG_CODE(
        SUNLOG(eCRITICAL_LOG, "[USR_ENTERED_SSQ_FIELD] - timeout X");
    );
    return TRUE;
}


BOOLEAN
ISSQHandler::ChangeStatus(const State<USR_LEAVED_SSQ_FIELD>& state_info)
{
    //WAVERIX_SSQ_PORTAL_TRANSACTION_MODIFY
    //////////////////////////////////////////////////////////////////////////
    //  새로운 상태를 위한 정보 설정
    m_eState = state_info.value;
    m_Timer.Instance();

    m_Dup.FLD_SSQ_FIELD_LEAVED = state_info.INFO;

    WAVERIX_DEBUG_CODE(
        SUNLOG(eCRITICAL_LOG, "[USR_LEAVED_SSQ_FIELD] - timeout X");
    );
    return TRUE;
}


BOOLEAN
ISSQHandler::ChangeStatus(const State<USR_ENTERED_ON_DEAD>& state_info)
{
    m_eState = state_info.value;
    m_Timer.SetNextTimeoutByCurTickBased(state_info.timeout);

    m_Dup.FLD_ON_DEAD = state_info.INFO;

    WAVERIX_DEBUG_CODE(
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] - timeout %u", state_info.timeout);
    );
    return TRUE;
}

BOOLEAN
ISSQHandler::ChangeStatus(const State<USR_PORTAL_FAILED>& state_info)
{
    switch(m_eState)
    {
    case USR_NONE_STATE:
    case MONSTER_STATE:
        break;
    case USR_REGISTERED_SSQ_INDEXER:
    case USR_PORTAL_OBJECT_CREATED:
    case USR_PORTAL_TRANSACTION:
        {
            User* const pUser = m_FLD_USR_INFO.m_pUser;
            const POSTYPE posOfItem = pUser->GetLockItemPosForInstanceDungeon();
            if(INVALID_POSTYPE_VALUE != posOfItem)
            {
                MSG_AG_ITEM_UNLOCK_CMD msg;
                msg.m_ItemIndex = SI_INVENTORY;
                msg.m_ItemPos   = m_FLD_USR_INFO.m_PosOfItem;
                // (f100528.2L) prevent a locked state
                // after an instance dungeon moving transaction failed
                msg.post_action_ = msg.eAction_MoveFailed;
                pUser->SendToLinkedServer(&msg, sizeof(msg));
                pUser->SetLockItemPosForInstanceDungeon(INVALID_POSTYPE_VALUE);
            }

            switch(m_eState)
            {
            case USR_PORTAL_OBJECT_CREATED:
            case USR_PORTAL_TRANSACTION:
                {
                    WAR_MSG_SINGLE_ASSEMBLER_SSQ_INFO_BRD<
                        MSGSUB_SSQ_PROTOCOL_NOTICE_1TO1_PORTAL_CLOSED > assembler;
                    assembler.Completion();
                    assembler.SendPacket(pUser);
                }
                break;
            }
        }
        break;
    case USR_ENTERING_SSQ_FIELD:
    case USR_ENTERED_SSQ_FIELD:
    case USR_CLEAR_INFO:
    case USR_LEAVED_SSQ_FIELD:
        break;
    };
    // (WAVERIX) (090421) (BUG-FIX) ChangeState인데, 상태 변경이 안되고 있음.
    m_eState = state_info.value;

    return TRUE;
}


BOOLEAN
ISSQHandler::IsPreparedReqTransaction() 
{
    if(USR_REGISTERED_SSQ_INDEXER == m_eState)
    {
        return !m_Timer.IsExpired();
    }
    return FALSE;
}

BOOLEAN
ISSQHandler::IsWarpObjectAlived()
{
    if(USR_PORTAL_OBJECT_CREATED == m_eState)
    {
        return !m_Timer.IsExpired();
    }
    return FALSE;
}

BOOLEAN
ISSQHandler::IsWarpTransaction()
{
    if(USR_PORTAL_TRANSACTION == m_eState)
    {
        return !m_Timer.IsExpired();
    }
    return FALSE;
}


BOOLEAN
ISSQHandler::Update()
{
    switch(m_eState)
    {
    case USR_NONE_STATE:
        return UPDATE_FAIL;
    case MONSTER_STATE:
        return UPDATE_SUCCESS;
    case USR_REGISTERED_SSQ_INDEXER:
        {
            const BOOLEAN isClosed = !!m_Timer.IsExpired(); //!IsPreparedReqTransaction();
            if(isClosed)
            {
                // Serializing SSQInfo to client
                WAVERIX_DEBUG_CODE(
                    SUNLOG(eCRITICAL_LOG, "USR_REGISTERED_SSQ_INDEXER - closed\n");
                );
            }

            return isClosed ? UPDATE_FAIL : UPDATE_SUCCESS;
        }
        break;
    case USR_PORTAL_OBJECT_CREATED:
        {
            const BOOLEAN isClosed = !!m_Timer.IsExpired();    // !IsWarpObjectAlived();
            if(isClosed)
            {
                // Serializing SSQInfo to client
                WAVERIX_DEBUG_CODE(
                    SUNLOG(eCRITICAL_LOG, "USR_PORTAL_OBJECT_CREATED - closed\n");
                );
            }

            return isClosed ? UPDATE_FAIL : UPDATE_SUCCESS;
        };
        break;
    case USR_PORTAL_TRANSACTION:
        {
            const BOOLEAN isClosed = !!m_Timer.IsExpired();    // !IsWarpTransaction();
            if(isClosed)
            {
                // Serializing SSQInfo to client
                WAVERIX_DEBUG_CODE(
                    SUNLOG(eCRITICAL_LOG, "USR_PORTAL_TRANSACTION - closed\n");
                );
            }

            return isClosed ? UPDATE_FAIL : UPDATE_SUCCESS;
        };
        break;
    case USR_ENTERING_SSQ_FIELD:
        return UPDATE_SUCCESS;  // 시간 제어는 좀더 고민해보자.
    case USR_ENTERED_SSQ_FIELD:
        return UPDATE_SUCCESS;  // 다른 제어가 들어갈 때까지 일단 성공으로 반환하자.
    case USR_PORTAL_FAILED:
    case USR_LEAVED_SSQ_FIELD:
        return UPDATE_FAIL;
    case USR_CLEAR_INFO:
        break;
    default:
        ASSERT(!"Invalid Tag");
        break;
    };

    return UPDATE_FAIL;
}


VOID
ISSQHandler::Release() // Update실패시 이 루틴을 먼저 호출해 준다.
{
    ChangeStatus(State<USR_PORTAL_FAILED>());
}

