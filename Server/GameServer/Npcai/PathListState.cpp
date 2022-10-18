#include "stdafx.h"
#include <Timer.h>
#include "AIParamParser.h"
#include "GameField.h"
#include "Monster.h"
#include "MovingPlanner.h"
#include "NPCStateManager.h"
#include "PathListState.h"
#include "PublicMath.h"

// implemented by _NA_0_20100222_UNIT_TRIGGERS_PATHLIST

PathListState::PathListState() : is_arrived_last_position_(false)
{
}

PathListState::~PathListState()
{
}

void PathListState::Init()
{
    is_arrived_last_position_ = false;
}

void PathListState::SetNPC(NPC* pNpc)
{
    monster_ = DOWNCASTER_SELECT<Monster*>(pNpc);
    if (monster_ == NULL || pNpc->IsEqualObjectKind(MONSTER_OBJECT) == false) {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]|Unexpected type %08X", pNpc->GetObjectType());
        __debugbreak();
    }

    State::SetNPC(pNpc);
}

void PathListState::OnEnter(LPARAM param1, LPARAM param2, LPARAM param3)
{
    State::OnEnter(param1, param2, param3);

    search_timer_.SetTimer(search_timer_.eStaticTimer_Search, 0);
    track_timer_.SetTimer(track_timer_.eStaticTimer_Track, m_pAITypeInfo->m_wTRACK_PERIOD);

    is_arrive_ = false;
    is_success_track_ = true;
    is_pos_index_back_ = false; 
    is_arrived_last_position_ = false;
}

void PathListState::OnExit()
{
    if (is_arrive_ == false)
    {
        // 상태 회복시 현재 위치에서 시작 하기 위해.
        // 정/역 방향은 저장하지 않는다.
        const WzVector& wvCurPos = *monster_->GetPosPtr();
        monster_->SetRegenPos(wvCurPos);
    }

    State::OnExit();
}

BOOL PathListState::OnUpdate(DWORD dwDeltaTick)
{
    __UNUSED(dwDeltaTick);

    if (search_timer_.IsExpired())
    {
        Character* pTargetChar = monster_->SearchTarget();
        if (pTargetChar)
        {
            monster_->SetTargetChar(pTargetChar);
            monster_->ChangeState(STATE_ID_TRACK);
            return true;
        }
    }

    if (track_timer_.IsExpired() && monster_->IsMoving() == false)
    {
        // 위치를 변경한 후
        ChangeMovePosIndex();
        
        // 최종 목적지에 도착했다면
        if (is_arrived_last_position_)
        {
            // 다시 역방향으로 되돌아갈지 검사한다.
            enum MovingType
            {
                kPatrol = 1, 
                kNoPatrol = 2
            };
            const UnitRegenInfo* const unit_regen_info = monster_->GetUnitRegenInfo();
            const int moving_type = unit_regen_info->GetAIType();
            if (moving_type == kNoPatrol)
            {
                monster_->ChangeState(STATE_ID_IDLE);
                return true;
            }
        }
        
        DoTrackPath();

        track_timer_.SetTimer(track_timer_.eStaticTimer_Track, m_pAITypeInfo->m_wTRACK_PERIOD);
    }
    return true;
}

bool PathListState::DoTrackPath()
{
    WzVector dest_pos;

    if (is_success_track_)
        dest_pos = GetDestPos();
    else
        dest_pos = FindNextRandomPos();
 
    if (dest_pos.x == 0 &&
        dest_pos.y == 0 && 
        dest_pos.z == 0)
    {
        BASE_NPCINFO* const npc_info = monster_->GetBaseInfo();

        SUNLOG(eCRITICAL_LOG, "[PathListState::FindNextRandomPos] "
            "MonsterCode[%d] DestPos is Invalid!!",
            npc_info->m_MonsterCode);
        return false;
    }


    const NPCStateManager* const state_manager = monster_->GetStateManager();
    const eCHAR_MOVE_STATE move_state = state_manager->GetWalkState();

    if (monster_->MoveAndBroadcast(&dest_pos, move_state))
        is_success_track_ = true;
    else
        is_success_track_ = false;

    return is_success_track_;
}

// num_of_pos 까지 순 방향으로 증가
// 0 까지 역 방향으로 감소
void PathListState::ChangeMovePosIndex()
{
    NPCStateManager* const state_manager = monster_->GetStateManager();
    const WzVector& cur_pos  = *monster_->GetPosPtr();
    const WzVector& dest_pos = GetDestPos();

    const float distance_current = Math_VectorLength2(cur_pos, dest_pos);
    if (distance_current < (kAroundDestPos * kAroundDestPos))
    {
        is_arrived_last_position_ = state_manager->IsArrivedLastPosition();
        //UnitRegenInfo* const regeninfo = monster_->GetUnitRegenInfo();
        //ASSERT(regeninfo);        
        //const WORD num_of_pos = regeninfo->GetNumOfPos();

        if (is_pos_index_back_)
        {
            if (state_manager->DecreaseMovePosIndex() == FALSE)
                is_pos_index_back_ = !is_pos_index_back_;
        }
        else
        {
            if (state_manager->IncreaseMovePosIndex() == FALSE)
                is_pos_index_back_ = !is_pos_index_back_;
        }
    }
}

const WzVector& PathListState::GetDestPos()
{
    NPCStateManager* const state_manager = monster_->GetStateManager();

    if (is_pos_index_back_)
        return state_manager->GetStartPos();
    else
        return state_manager->GetDestPos();
}

WzVector PathListState::FindNextRandomPos()
{
    WzVector dest_pos = GetDestPos();
    const WzVector& cur_pos = *monster_->GetPosPtr();

    float move_distance = (float)dRandom(AIParamParser::AICommonInfo.m_fMIN_MOVE_DISTANCE,
                                         AIParamParser::AICommonInfo.m_fMAX_MOVE_DISTANCE);

    WzVector normal_pos;
    VectorNormalize(&normal_pos, &(dest_pos - cur_pos));
    normal_pos.z = 0;

    // 이동해야할 방향의 45도 안팍으로 랜덤한 위치를 얻어온다.
    int rotate_angle = random(0, 90);
    if (rotate_angle > 45)
        rotate_angle = 405 - rotate_angle;

    Math_ZRotateVector(&normal_pos, dest_pos, rotate_angle);
    dest_pos = cur_pos + dest_pos * move_distance;

    return dest_pos;
}

void PathListState::OnMsgAttacked(const AI_MSG* pMsg)
{
    State::OnMsgAttacked(pMsg);

    const AI_MSG_ATTACKED* attack_msg = static_cast<const AI_MSG_ATTACKED*>(pMsg);

    GameField* field = monster_->GetField();
    if (field == NULL)
        return;

    Character* attacker = field->FindCharacter(attack_msg->dwAttackerKey);
    if (attacker == NULL)
        return;

    // 현재 선택된 타겟이 없고 상대가 적군이면 공격 상태로 전이
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    State::SetTarget(pMsg, STATE_ID_TRACK);
#else
    if (monster_->GetTargetChar() == NULL &&
        monster_->IsFriend(attacker) == USER_RELATION_ENEMY)
    {
        monster_->SetTargetChar(attacker);
        monster_->ChangeState(STATE_ID_TRACK, 0, 0, attack_msg->dwMsgID);
    }
#endif //_NA_007667_20141001_WITCHBLADE_SKILL

}
