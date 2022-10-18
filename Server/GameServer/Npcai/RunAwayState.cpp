#include "stdafx.h"
#include "RunAwayState.h"

#include <Timer.h>
#include <SinTable.h>
#include <AIParamParser.h>
#include <PacketControl/WarMessages_Sync.h>


#include "GameField.h"
#include "Player.h"


RunAwayState::RunAwayState()
{
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    m_dwTargetKey = 0;
    m_abilityID = eABILITY_INVALID;
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
}

RunAwayState::~RunAwayState()
{
}

void
RunAwayState::Init()
{
    __noop;
}

void
RunAwayState::OnEnter(LPARAM param1, LPARAM param2, LPARAM param3)
{
    State::OnEnter(param1, param2, param3);

    m_vTargetPos = *m_pNPC->GetPosPtr();
    m_bTouchedSomething = false;

    // 도망가야할 적을 찾아서 타겟과 위치를 셋팅한다.
    DWORD target_key = static_cast<DWORD>(param1);
    Player* pTarget = PlayerManager::Instance()->FindPlayerByObjKey(target_key);
    if (pTarget)
    {
        m_pNPC->SetTargetChar(pTarget);
        m_vTargetPos = *pTarget->GetPosPtr();
    }

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE 
    DWORD delay = m_pAITypeInfo->m_wRUNAWAY_TIME;
    const AI_MSG_RUNAWAY* pRecvMsg = reinterpret_cast<const AI_MSG_RUNAWAY*>(param2);
    if (param1 != NULL && pRecvMsg != NULL)
    {
        delay = pRecvMsg->dwRunAwayTime;
        m_abilityID = pRecvMsg->abilityID;
    }
    m_dwTargetKey = target_key;
#else  // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    // 도망가야할 시간을 셋팅한다.
    DWORD delay = param1 ? static_cast<DWORD>(param2) : m_pAITypeInfo->m_wRUNAWAY_TIME;
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    m_RunAwayTimer.SetTimer(delay);
    //__NA_S00015_20080828_NEW_HOPE_SHADOW
    m_wStateID = static_cast<WORD>(param3); // TODO : need check boundary
}

void
RunAwayState::OnExit()
{
    State::OnExit();
}

BOOL
RunAwayState::OnUpdate(DWORD dwDeltaTick)
{
    __UNUSED(dwDeltaTick);

    if (!m_pNPC->IsMoving())
    {
        RunAway();
    }

    if (m_RunAwayTimer.IsExpired())
    {
        if (m_pNPC->GetTargetChar())
        {
            m_pNPC->ChangeState(STATE_ID_HELP);
        }
        else
        {
            m_pNPC->SetTargetChar(NULL);
            m_pNPC->ChangeState(STATE_ID_IDLE);
        }
    }
    return true;
}

void
RunAwayState::RunAway()
{
    // 이동할 거리를 구한다.
    float MOVE_DISTANCE = (float)dRandom(AIParamParser::AICommonInfo.m_fMIN_MOVE_DISTANCE,
                                         AIParamParser::AICommonInfo.m_fMAX_MOVE_DISTANCE);

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    if (m_abilityID == eABILITY_CHARMED) // 매혹
    {
        Player* pTarget = PlayerManager::Instance()->FindPlayerByObjKey(m_dwTargetKey); // 타겟 위치 갱신
        if (pTarget)
        {
            m_pNPC->SetTargetChar(pTarget);
            m_vTargetPos = *pTarget->GetPosPtr();
        }
        MOVE_DISTANCE = 1;   // 1을 이동할때마다 타겟의 위치를 갱신하도록 한다
        MOVE_DISTANCE *= -1; // 방향 설정
    }
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

    const WzVector& wvCurPos = *m_pNPC->GetPosPtr();

    GameField* pField = m_pNPC->GetField();
    if (pField == NULL)
        return;

    WzVector wvMovement;
    // 이전 길찾기에서 무언가에 닿은 경우 60도 회전시키고, 그렇지 않다면 랜덤한 방향으로 포인팅
    if (m_bTouchedSomething)
    {
        m_bTouchedSomething = false;
        wvMovement = m_wvRotatedVector; 
    }
    else
    {
        VectorNormalize(&wvMovement, &(wvCurPos - m_vTargetPos));
        wvMovement.x *= MOVE_DISTANCE;
        wvMovement.y *= MOVE_DISTANCE;
    }
    wvMovement.z = 2.f;

    WzVector& wvDestPos = wvCurPos + wvMovement;
    if (!pField->FindPath(m_pNPC, &wvDestPos, (eCHAR_STATE_TYPE)m_wStateID))
    {
        // 장애물에 닿았다는 표시를 하고 지금 가려는 방향에서 60도 회전시킨 벡터를 구해둔다.
        m_bTouchedSomething = true;
        const WzVector& wvDistanceWantToMove = wvDestPos - wvCurPos;
        m_wvRotatedVector.x =  wvDistanceWantToMove.x *
                               g_SinTable.FCOS(AIParamParser::AICommonInfo.m_wSEARCH_ROTATE_ANGLE) +
                               wvDistanceWantToMove.y *
                               g_SinTable.FSIN(AIParamParser::AICommonInfo.m_wSEARCH_ROTATE_ANGLE);
        m_wvRotatedVector.y =  -wvDistanceWantToMove.x *
                               g_SinTable.FSIN(AIParamParser::AICommonInfo.m_wSEARCH_ROTATE_ANGLE) +
                               wvDistanceWantToMove.y *
                               g_SinTable.FCOS(AIParamParser::AICommonInfo.m_wSEARCH_ROTATE_ANGLE); 
        m_wvRotatedVector.z = 0.0f;

        return;
    }

    m_pNPC->SetMoveState(CMS_RUN);

    {
        MSGSUB_SYNC_MOVE_THRUST_BRD msg;
        msg.m_dwObjectKey = m_pNPC->GetObjectKey();
        msg.m_byState     = CMS_RUN;
        msg.m_wTileIndex  = m_pNPC->GetPathExplorer()->GetTile();
        msg.m_byState     = CMS_RUN; //캐릭터의 경우는 무조건 뛰기만 존재!!(몬스터와 같이 쓰는 패킷이므로 값은 넣어줌)
        msg.m_wvCurPos    = wvCurPos;
        msg.m_wvDestPos   = wvDestPos;

        m_pNPC->SendPacketAroundForSync(&msg);
    };
}

// 도망상태가 끝나면 Follow(Idle)상태로 복귀한다.
void
RunAwayState::OnMsgCommandFollow(const AI_MSG* pMsg)
{
    m_pNPC->SetTargetChar(NULL);
}

