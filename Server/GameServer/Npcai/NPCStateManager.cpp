#include "stdafx.h"
#include "NPCStateManager.h"

#include <PacketControl/WarMessages_Sync.h>

#include "GameServerEx.h"

#include "MovingPlanner.h"
#include "MoveStateControl.h"
#include "NPCStateFactory.h"
#include "NPC.h"
#include "State.h"

//==================================================================================================
//  @history
//  ~09/12/22
//  09/12/22, waverix, code-arrangement

static NPCStateFactory s_NPCStateFactory;

//==================================================================================================
//  (WAVERIX)(081005)(NOTE)
//  - 다음 '///' 사이의 로직은 임시 로직이며,
//  NPCState 관련 구조 변경이 완료되는 시점에 제거될 것이다.
namespace nsAI {

static const WzVector NullVector = { .0f, .0f, .0f };

//  <Prototype><temporary>
//  각각의 State에 별도의 자료구조를 두지 않도록 하자. 현재는 기존 방식에 묻어가는 형태로...
class EscapeState
{
    enum eTracingMethod { eTM_None, eTM_TileTracing, eTM_DestPosTracing, };
public:
    EscapeState()
        : m_pOwner(0)
        , m_pGameFieldSync(0)
        , m_TimeoutTick(0)
        , m_TracingMethod(eTM_None)
        , m_DestTileNo(-1)
        , m_DestPos(NullVector)
    {
        m_UpdateTimer.Disabled();
    }
    ~EscapeState()
    {
    }

    void Init(NPC* pNPC)
    {
        m_pOwner = pNPC;
        m_pGameFieldSync = pNPC->GetField();
    }

    BOOLEAN OnEnter(const CmdAI::Escape& rCmd);
    BOOLEAN OnUpdate(const DWORD delta_tick);
    BOOLEAN OnExit();

private:
    NPC*       m_pOwner;
    GameField* m_pGameFieldSync;
    ITimeout   m_UpdateTimer;
    DWORD m_TimeoutTick;

    eTracingMethod m_TracingMethod; // 뭐 때문에 피하는지를 명시할 필요 있다.
    WORD m_DestTileNo;
    WzVector m_DestPos;
};

}; // end of namespace 'nsAI'
//
//==================================================================================================
// (CHANGES) (091222) (WAVERIX) code arrangement

NPCStateManager::NPCStateManager(NPC* pOwner)
    : m_pNPC(pOwner)
    , m_pEscapeState(0)
{
    ClearFields();
}

NPCStateManager::~NPCStateManager()
{
    Release();
}

//void
//NPCStateManager::_ReleaseNode(STATE_MAP_PAIR rPair)
//{
//  State* pState = rPair.second;
//  s_NPCStateFactory.Free(pState);
//}

void
NPCStateManager::ClearFields()
{
    m_pCurState = 0;
    //STATE_MAP m_mapStates
    m_eMoveType = eMOVE_ATTITUDE_WANDER;
    m_dwMoveAreaID = 0;

    pos_index_ = 0;

    //ITimerBase m_SpecialTimer
    //ITimeout m_StateChangeCheckTimer;
    m_StateChangeCount = 0;
    m_bStateTransaction = 0;
    m_bRequestHelp = 0;
    if (m_pEscapeState)
    {
        TAllocDelete(nsAI::EscapeState, m_pEscapeState);
        m_pEscapeState = 0;
        // (NOTE) (WAVERIX) >-- 일단 다음이 문제이긴 한데... 임시 코드임.
    }

    FOREACH_CONTAINER(const STATE_MAP::value_type& node, m_mapStates, STATE_MAP)
    {
        State* state = node.second;
        s_NPCStateFactory.Free(state);
    }

    m_mapStates.clear();
}

// _NA_0_20100514_NPC_DEAD_STATE_TIME
void
NPCStateManager::ResetStateObject()
{
    STATE_MAP::iterator it = m_mapStates.begin();

    while(it != m_mapStates.end())
    {
        State* state = it->second;

        if (m_pCurState != state)
        {
            m_mapStates.erase(it++);
            s_NPCStateFactory.Free(state);
        }
        else
        {
            ++it;
        }
    }
}

void
NPCStateManager::Init(DWORD stateID, BYTE byMoveType, DWORD moveAreaID, LPARAM param1)
{
    m_StateChangeCount = 0;
    m_StateChangeCheckTimer.SetNextTimeoutByCurTickBased(1000);
    m_bRequestHelp = false;
    m_eMoveType = (eNPC_MOVE_ATTITUDE)byMoveType;
    m_dwMoveAreaID = moveAreaID;

    const BASE_NPCINFO* pNpcInfo = m_pNPC->GetBaseInfo();
    // 처음 State
    STATE_MAP::const_iterator it = m_mapStates.find(stateID);
    if (it == m_mapStates.end())
    {
    #ifdef _DEBUG
        __debugbreak();
    #endif
        SUNLOG(eCRITICAL_LOG, "Can't find NPC state [NPC:%u][ID:%u]",
               pNpcInfo->m_MonsterCode, stateID);
    }

    m_pCurState = it->second;
    m_pCurState->OnEnter(param1);

    m_SpecialTimer.SetTimer(pNpcInfo->m_wSkillUpdateTime);
}

BOOL
NPCStateManager::Update(DWORD delta_tick)
{
    if (m_SpecialTimer.IsExpired() && m_pNPC->IsAlive())
    {
        SPECIAL_CONDITION* special_condition = m_pNPC->GetDrawLotsSpecialAction();
        if (special_condition)
           m_pNPC->SpecialAction(*special_condition);

        m_pNPC->StatusResist(); // 상태저항 체크
    }

    if (m_pEscapeState)
    {
        BOOLEAN bProgress = m_pEscapeState->OnUpdate(delta_tick);
        if (bProgress != false)
            return true;

        m_pEscapeState->OnExit();
        TAllocDelete(nsAI::EscapeState, m_pEscapeState);
        m_pEscapeState = 0;
        // (NOTE) (WAVERIX) >-- 일단 다음이 문제이긴 한데... 임시 코드임.
    }

    return m_pCurState->OnUpdate(delta_tick);
}

void
NPCStateManager::AddStateObject(DWORD stateID, DWORD linkStateID)
{
    STATE_MAP::const_iterator it = m_mapStates.find(stateID);
    if (it != m_mapStates.end())
    {
        State* pAddState = it->second;

        const NPC* find_state_owner_npc = pAddState->GetOwnerNpc();
        const DWORD find_state_link_stateID = pAddState->GetLinkStateID();

        if (find_state_owner_npc != m_pNPC ||
            find_state_link_stateID != linkStateID)
        {
            const BASE_NPCINFO* manager_npc_info = m_pNPC->GetBaseInfo();
            const BASE_NPCINFO* find_state_owner_npc_info = find_state_owner_npc->GetBaseInfo();

            SUNLOG(eCRITICAL_LOG, "[NPCStateManager::AddStateObject] 이미 존재하는 상태에러"
                                  "manager_npc[%d] find_state_owner_npc[%d]"
                                  "new_link_stateID[%d] cur_link_stateID[%d]",
                                  manager_npc_info->m_MonsterCode, 
                                  find_state_owner_npc_info->m_MonsterCode,
                                  linkStateID, find_state_link_stateID );
        }
    }
    else
    {
        State* pAddState = s_NPCStateFactory.Alloc(linkStateID);

        pAddState->SetNPC(m_pNPC);
        pAddState->SetStateID((ENUM_STATD_ID)stateID);
        pAddState->SetLinkStateID((ENUM_STATD_ID)linkStateID);

        pAddState->Init();

        m_mapStates.insert(std::make_pair(stateID, pAddState));
    }
}

void
NPCStateManager::ChangeState(DWORD stateID, LPARAM param1, LPARAM param2, LPARAM param3)
{
    if (++m_StateChangeCount > 10)
    {
        const BASE_NPCINFO* pNpcInfo = m_pNPC->GetBaseInfo();
        Player* npc_owner = m_pNPC->GetPlayerOwner();

        if (npc_owner == NULL)
        {
            SUNLOG(eCRITICAL_LOG,
                "too many state change try! NPCCode(%d) MoveType[%d] "
                "CurState[%d] CurLinkState[%d] NextState[%d] AIMsgID[%d]",
                pNpcInfo->m_MonsterCode, pNpcInfo->m_byMoveAttitude,
                m_pCurState->GetStateID(), m_pCurState->GetLinkStateID(), stateID, param3);
        }
        else //(npc_owner != NULL)
        {
            SUNLOG(eCRITICAL_LOG,
                "too many state change try! NPC_Owner_Userkey(%d) NPCCode(%d) MoveType[%d] "
                "CurState[%d] CurLinkState[%d] NextState[%d] AIMsgID[%d]",
                npc_owner->GetCharGuid(), pNpcInfo->m_MonsterCode, pNpcInfo->m_byMoveAttitude,
                m_pCurState->GetStateID(), m_pCurState->GetLinkStateID(), stateID, param3);
        }
    }

    if (m_StateChangeCheckTimer.IsExpired())
    {
        m_StateChangeCheckTimer.SetNextTimeoutByCurTickBased(1000);
        m_StateChangeCount = 0;
    }
    //
    WAVERIX_WRAPPER((WAVERIX_LC_CALL3("waverix_log_ai_changes", m_pNPC, stateID, m_pCurState)));
    // 바뀔 상태의 포인터를 찾는다.
    STATE_MAP::const_iterator it = m_mapStates.find(stateID);
    // 바뀔 상태가 정의되어 있지 않은 경우 리턴한다.
    // -or- 바뀔 상태가 현재 상태와 같으면 그냥 리턴한다.
    if (it == m_mapStates.end() || it->second == m_pCurState)
    {
        return;
    }
    else
    {
        StateTransaction trans(m_bStateTransaction);
        // 현재 상태의 마무리 코드
        m_pCurState->OnExit();
        // 상태 포인터 교체
        m_pCurState = it->second;
        // 바뀐 상태의 초기화 코드
        m_pCurState->OnEnter(param1, param2, param3); 
    }
}

const WzVector& NPCStateManager::GetStartPos() const 
{ 
    UnitRegenInfo* const regeninfo = m_pNPC->GetUnitRegenInfo();

    ASSERT(regeninfo);
    return regeninfo->GetStartPos(pos_index_);
}

const WzVector& NPCStateManager::GetDestPos() const 
{
    UnitRegenInfo* const regeninfo = m_pNPC->GetUnitRegenInfo();

    ASSERT(regeninfo);
    return regeninfo->GetDestPos(pos_index_);
}

const eCHAR_MOVE_STATE NPCStateManager::GetWalkState() const 
{
    UnitRegenInfo* const regeninfo = m_pNPC->GetUnitRegenInfo();

    ASSERT(regeninfo);
    return regeninfo->GetWalkState(pos_index_);
}

// num_of_pos 까지 순 방향으로 증가
BOOL NPCStateManager::IncreaseMovePosIndex()
{
    UnitRegenInfo* const regeninfo = m_pNPC->GetUnitRegenInfo();
    ASSERT(regeninfo);

    const WORD num_of_pos = regeninfo->GetNumOfPos();

    if (pos_index_ < num_of_pos-1)
    {
        ++pos_index_;
        return TRUE;
    }

    return FALSE;
}

// num_of_pos 까지 순 방향으로 증가
BOOL NPCStateManager::DecreaseMovePosIndex()
{
    UnitRegenInfo* const regeninfo = m_pNPC->GetUnitRegenInfo();
    ASSERT(regeninfo);

    const WORD num_of_pos = regeninfo->GetNumOfPos();

    if (pos_index_ > 0)
    {
        --pos_index_;
        return TRUE;
    }

    return FALSE;
}

// 위치 목록 중 마지막 위치에 도달 했는지 검사한다.
bool NPCStateManager::IsArrivedLastPosition() const
{
    const UnitRegenInfo* const unit_regen = m_pNPC->GetUnitRegenInfo();
    ASSERT(unit_regen != NULL);
    const WORD num_of_pos = unit_regen->GetNumOfPos();
    return (pos_index_ == num_of_pos-1);
}

//==================================================================================================

void
NPCStateManager::RequestCommand_(const nsAI::CmdAI& rCmd)
{
    using namespace nsAI;
    switch (rCmd.Cmd)
    {
    case CmdAI::eCmd_Escape:
        {
            if (m_pEscapeState) {
                TAllocDelete(nsAI::EscapeState, m_pEscapeState);
            }
            m_pEscapeState = TAllocNew(nsAI::EscapeState);
            m_pEscapeState->Init(m_pNPC);
            m_pEscapeState->OnEnter(static_cast<const CmdAI::Escape&>(rCmd));
        }
        break;
    default:
        ASSERT(!"["__FUNCTION__"] Unexpected Cmd");
        break;
    }
}

//==================================================================================================
// namespace
namespace nsAI {

const DWORD escape_check_timer = 50;

//  대충 만들고 있다. 반드시 다른 위치에 다른 로직으로 재구축하라!!!

BOOLEAN
EscapeState::OnEnter(const CmdAI::Escape& rCmd)
{
    if ((m_pOwner && m_pGameFieldSync) == false)
        return false;

    const DWORD curTick = GetTickCount();
    m_UpdateTimer.SetNextTimeoutByCurTickBased(escape_check_timer);
    m_TimeoutTick = curTick + rCmd.TimeoutInterval;

    BOOLEAN valid_pos = (rCmd.pDestPos != 0);
    BOOLEAN valid_tile = (rCmd.DestTileNo != -1);
    if ((valid_tile & valid_pos) == 0)
    {
        // 현재로선 지원할 시간적 여유 없음. 어차피 재정리할 예정이긴 하지만...
        ASSERT(!"Current Version not implementation");
        return false;
    }

    m_DestTileNo = rCmd.DestTileNo;
    m_DestPos = *rCmd.pDestPos;

/*
    CPathExplorer* const pPathExplorer = m_pOwner->GetPathExplorer();
    const BOOLEAN isMoving = pPathExplorer->IsMoving();
    if (isMoving)
        pPathExplorer->Stop();

    WzVector& diff = (m_DestPos - (*pCurPos));

    const float lenQ2 = 5.f*5.f;

    const float calc_lenQ2 = VectorLength2(&diff);
    BOOLEAN bJump = false;
    if (calc_lenQ2 < lenQ2)
    {
      const DWORD bJumpRaio = !!RandomNumberGenerator::GetRandomNumberRange(0, 1000);
      if (bJumpRaio < 900)
          bJump = true;
    }

    WzVector normal_diff;
    VectorNormalize(&normal_diff, &diff);

    if (!bJump)
    {
      BOOL bTouchedSomething;
      WzVector wvRotatedVector;
      if (m_pOwner->ThrustMoveAndBroadcast(
          &m_DestPos
          , CMS_RUN
          , bTouchedSomething
          , wvRotatedVector
))
      {
          return true;
      }
    }
*/

    const WzVector* pCurPos = m_pOwner->GetPosPtr();
    if (m_pGameFieldSync->FindPath(m_pOwner, &m_DestPos))
    {
        MoveStateControl* const pMoveStateControl = m_pOwner->GetMoveStateControl();
        eCHAR_MOVE_STATE move_state = pMoveStateControl->GetMoveState();
        if (move_state == CMS_WALK) {
            move_state = CMS_RUN;
            pMoveStateControl->SetMoveState(move_state);
        }

        MSGSUB_SYNC_MOVE_BRD msg;
        msg.m_dwObjectKey   = m_pOwner->GetObjectKey();
        msg.m_byState       = move_state;
        msg.m_ForcedMove    = true;
        msg.m_wvCurPos      = *pCurPos;
        msg.m_wvDestPos     = m_DestPos;

        m_pOwner->SendPacketAroundForSync(&msg);
    }

    return true;
}


BOOLEAN
EscapeState::OnUpdate(const DWORD delta_tick)
{
    const BOOLEAN update_continue = true;
    const BOOLEAN update_stop = false;

    if (!m_UpdateTimer.IsExpired())
        return update_continue;

    GameField* pGameField = m_pOwner->GetField();
    if ((pGameField && (pGameField == m_pGameFieldSync)) == false)
        return update_stop;

    if (m_UpdateTimer.GetTimeoutTicks() > m_TimeoutTick)
        return update_stop;

    m_UpdateTimer.SetNextTimeoutByCurTickBased(escape_check_timer);

    CPathExplorer* const pPathExplorer = m_pOwner->GetPathExplorer();
    BOOLEAN isMoving = pPathExplorer->IsMoving();

    //const int iTile = pPathExplorer->GetTile();
    //if ((WORD)iTile == m_DestTileNo)
    //  return update_stop;
    if (isMoving == false)
        return update_stop;

    return update_continue;
}


BOOLEAN
EscapeState::OnExit()
{
    IPlanEventObject& rPlanEvent = m_pOwner->PlanEvent;
    rPlanEvent.RemoveField(IPlanEventObject::AIACT_ESCAPE);
    return false;
}

}; //end of namespace 'nsAI'

//==================================================================================================


