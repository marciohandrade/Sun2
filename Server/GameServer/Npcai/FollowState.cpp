#include "stdafx.h"
#include "FollowState.h"

#include "AIParamParser.h"
#include "FieldInfo.h"
#include "GameField.h"
#include "Monster.h"
#include "../MovingPlanner.h"
#include "PublicMath.h"
#include "Skill.h"
#include "TargetManager.h"

// implemented by _NA_0_20100222_UNIT_TRIGGERS_PATHLIST

FollowState::FollowState()
{
}

FollowState::~FollowState()
{
}

void FollowState::Init()
{
}

void FollowState::SetNPC(NPC* pNpc)
{
    monster_ = DOWNCASTER_SELECT<Monster*>(pNpc);
    if (monster_ == NULL || pNpc->IsEqualObjectKind(MONSTER_OBJECT) == false) {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]|Unexpected type %08X", pNpc->GetObjectType());
        __debugbreak();
    }

    State::SetNPC(pNpc);
}

void FollowState::OnEnter(LPARAM param1, LPARAM param2, LPARAM param3)
{
    State::OnEnter(param1, param2, param3); 

    search_timer_.SetTimer(search_timer_.eStaticTimer_Search, 0);
    track_timer_.SetTimer(track_timer_.eStaticTimer_Track, m_pAITypeInfo->m_wTRACK_PERIOD);
    
    follow_char_ = NULL;
}

void FollowState::OnExit()
{
    const WzVector& wvCurPos = *monster_->GetPosPtr();
    monster_->SetRegenPos(wvCurPos);

    State::OnExit();
}

BOOL FollowState::OnUpdate(DWORD dwDeltaTick)
{
    __UNUSED(dwDeltaTick);

    if (search_timer_.IsExpired())
    {
        Character* target_char = monster_->SearchTarget();
        if (target_char)
        {
            monster_->SetTargetChar(target_char);
            monster_->ChangeState(STATE_ID_TRACK);
            return true;
        }

        follow_char_ = monster_->SearchTargetFriend();
    }

    if (follow_char_ == NULL)
        return true;

    WzVector follow_pos = *follow_char_->GetPosPtr();
    const float sight_range = monster_->GetSightRange();
    float distance_sq = monster_->GetDist2(follow_pos);

    if(distance_sq < sight_range)
    { 
        return true;
    }

    if (monster_->IsMoving())
    {
        if (track_timer_.IsExpired())
        {
            WzVector monster_pos = *monster_->GetPosPtr();
            WzVector monster_dest_pos = monster_->GetPathExplorer()->GetTargetPos();

            // 플레이어가 NPC가 뛰려는 방향의 부채꼴을 벗어난 각에 위치하는가?
            BOOLEAN is_out_angle = !Math_IsPositionInFanshapedArea(&follow_pos, &monster_pos,
                &(monster_dest_pos - monster_pos), AIParamParser::AICommonInfo.m_wTRACK_INNER_ANGLE);

            if (is_out_angle)
                DoTrack();
        }
    }
    else
    {
        DoTrack();
    }

    return true;
}

void FollowState::DoTrack()
{
    WzVector cur_pos    = *monster_->GetPosPtr();
    WzVector follow_pos = *follow_char_->GetPosPtr();
    WzVector track_pos  = follow_pos;

    // 타겟이 이동중이라면 이동 방향 감안해서 추적점 보정
    if (follow_char_->IsMoving())
    {
        const WzVector& follow_char_dest_pos = follow_char_->GetPathExplorer()->GetTargetPos();
        // 타겟 플레이어가 NPC의 반대쪽으로 이동하려는 경우에만 보정
        if (!Math_IsPositionInFanshapedArea(&cur_pos, &follow_pos,
            &(follow_char_dest_pos - follow_pos),
            180))
        {
            WzVector normal_pos;
            VectorNormalize(&normal_pos, &(follow_char_dest_pos - follow_pos));
            normal_pos = normal_pos * 2.0f;
            track_pos = track_pos + normal_pos;
        }
    }

    eCHAR_MOVE_STATE move_state = GetMoveState(track_pos, cur_pos);

    if (monster_->MoveAndBroadcast(&track_pos, move_state) == false)
    {
        move_state = GetMoveState(follow_pos, cur_pos);

        if(monster_->MoveAndBroadcast(&follow_pos, move_state) == false)
        {
            follow_pos = FindRandomPos();
            move_state = GetMoveState(follow_pos, cur_pos);

            monster_->MoveAndBroadcast(&follow_pos, move_state);
        }
    }

    track_timer_.SetTimer(track_timer_.eStaticTimer_Track, m_pAITypeInfo->m_wTRACK_PERIOD);
}

eCHAR_MOVE_STATE FollowState::GetMoveState(const WzVector& follow_pos, const WzVector& cur_pos)
{
    float distance_current = Math_VectorLength2(follow_pos, cur_pos);
    if (distance_current > (kDistanceRunFollow * kDistanceRunFollow))
        return CMS_RUN;

    return CMS_WALK;
}

WzVector FollowState::FindRandomPos()
{
    WzVector follow_pos = *follow_char_->GetPosPtr();
    const WzVector& cur_pos = *monster_->GetPosPtr();

    float move_distance = (float)dRandom(AIParamParser::AICommonInfo.m_fMIN_MOVE_DISTANCE,
        AIParamParser::AICommonInfo.m_fMAX_MOVE_DISTANCE);

    WzVector normal_pos;
    VectorNormalize(&normal_pos, &(follow_pos - cur_pos));
    normal_pos.z = 0;

    // 이동해야할 방향의 45도 안팍으로 랜덤한 위치를 얻어온다.
    int rotate_angle = random(0, 90);
    if (rotate_angle > 45)
        rotate_angle = 405 - rotate_angle;

    Math_ZRotateVector(&normal_pos, follow_pos, rotate_angle);
    follow_pos = cur_pos + follow_pos * move_distance;

    return follow_pos;
}

void FollowState::OnMsgForceAttack(const AI_MSG* pMsg)
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
