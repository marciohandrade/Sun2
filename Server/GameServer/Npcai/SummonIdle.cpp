#include "stdafx.h"
#include "SummonIdle.h"
#include "GameField.h"
#include <Timer.h>
#include "AIParamParser.h"
#include "Summoned.h"
#include "PublicMath.h"
#include "MoveStateControl.h"
#include "TargetManager.h"
#include "ChaoState.h"
#include "Player.h"
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
#include "SummonManager.h"
#include "SummonedNPCs.h"
#endif //_NA_007667_20141001_WITCHBLADE_SKILL

SummonIdle::SummonIdle()
{
}

SummonIdle::~SummonIdle()
{
}

void
SummonIdle::Init()
{
}

void
SummonIdle::SetNPC(NPC* pNpc)
{
    m_pSummoned = DOWNCASTER_SELECT<Summoned*>(pNpc);
    if (m_pSummoned == NULL || pNpc->IsEqualObjectKind(SUMMON_OBJECT) == false) {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]|Unexpected type %08X", pNpc->GetObjectType());
        __debugbreak();
    }

    State::SetNPC(pNpc);
}

void
SummonIdle::OnEnter(LPARAM param1, LPARAM param2, LPARAM param3)
{
    State::OnEnter(param1, param2, param3);

    m_SearchTimer.SetTimer(m_pAITypeInfo->m_wSEARCH_PERIOD);
    m_TrackTimer.SetTimer(m_pAITypeInfo->m_wTRACK_PERIOD);
    m_SpawnTimer.SetTimer(param1);

    m_pSummoned->SetTargetChar(NULL);
    m_pSummoned->SetSkillUseDelegate(false);

    // 소환체가 공격을 하다가 Idle로 돌아오는 시점에서 강제공격 플래그를 끈다.
    Player* const player = m_pSummoned->GetPlayerOwner();
    if (player) {
        player->SetForceAttack(false);
    }
}

void
SummonIdle::OnExit()
{
    State::OnExit();
}

BOOL
SummonIdle::OnUpdate(DWORD dwDeltaTick)
{
    // 최초 스폰시간을 위해서 일정시간 기다려준다.
    if (!m_SpawnTimer.IsExpired(false))
        return true;

    if (obj_util::IsValid(m_pSummoned, __FUNCTION__, __LINE__) == false) {
        return true;
    }

    Character* const m_pSummoner = m_pSummoned->GetSummoner();
    if (m_pSummoner == NULL)
        return true;

    m_vSummonedPos = *m_pSummoned->GetPosPtr();
    m_vSummonerPos = *m_pSummoner->GetPosPtr();

    // (CHANGES) (100206) (WAVERIX) PCC_CONTINUING_NPC_UPDATE 추가에 따른 AttackState유지 상태를
    // 바꿀 필요성이 생겼다.
    GameField* game_field = m_pSummoned->GetField(); //assert not null
    DWORD references = 0;
    {
        SECTOR_IDX sector_index = m_pSummoned->GetSectorIndex();
        Sector* sector = sector_index != INVALID_SECTOR_INDEX ? game_field->GetSector(sector_index)
                                                              : NULL;
        // 'sector == NULL' 발생해서는 안되지만, 경우에 따라
        // (3DEngine에서 미묘한 계산 오차에 따른 사례 존재) 가능성이 존재한다.
        references = sector != NULL ? sector->GetReferenceCount() : 0;
    };

#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    // 고정형 소환수는 추적상태전환, 이동처리 스킵
    if (m_pSummoned->GetSummonType() == eSUMMON_IMMOVABLE) 
    {
        UseConstantSkill();
    }
    else
    {
        // (lst1024)(080923) 적군 플레이어 공격버그를 위해 _NONPK 모드 추가 필요.
        // Summoned에 SearchTarget함수를 추가 하여 적군 플레이어 유저를 찾지 않는 파인더 사용. 
        // 아군도 스킬 사용 범위에 넣을 경우 자기 자신도 아군이므로 SummonIdle상태로 돌아오지 않는다. 
        //
        if (references != 0 && // skip target searching
            m_SearchTimer.IsExpired() && (m_pSummoned->IsOffensive() || m_pSummoned->IsNonPVP()))
        {
            // 주변에 적이 있는지 탐색
            Character* pTargetChar = m_pSummoned->SearchTarget();
            if (pTargetChar)
            {
                m_pSummoned->SetTargetChar(pTargetChar);
                m_pSummoned->ChangeState(STATE_ID_TRACK);
                return true;
            }
        }
    }
#else
    // (lst1024)(080923) 적군 플레이어 공격버그를 위해 _NONPK 모드 추가 필요.
    // Summoned에 SearchTarget함수를 추가 하여 적군 플레이어 유저를 찾지 않는 파인더 사용. 
    // 아군도 스킬 사용 범위에 넣을 경우 자기 자신도 아군이므로 SummonIdle상태로 돌아오지 않는다. 
    //
    if (references != 0 && // skip target searching
        m_SearchTimer.IsExpired() && (m_pSummoned->IsOffensive() || m_pSummoned->IsNonPVP()))
    {
        // 주변에 적이 있는지 탐색
        Character* pTargetChar = m_pSummoned->SearchTarget();
        if (pTargetChar)
        {
            m_pSummoned->SetTargetChar(pTargetChar);
            m_pSummoned->ChangeState(STATE_ID_TRACK);
            return true;
        }
    }
#endif //_NA_007667_20141001_WITCHBLADE_SKILL

    // 소환자와의 거리가 일정범위 내이면 이동을 하지 않는다.
    static const float SummonFollowDistance = AIParamParser::AICommonInfo.m_fSUMMON_FOLLOW_DISTANCE *
                                              AIParamParser::AICommonInfo.m_fSUMMON_FOLLOW_DISTANCE;
    float fDistance2 = Math_Vector2DLength2(&(m_vSummonedPos - m_vSummonerPos));
    if (fDistance2 <= SummonFollowDistance)
        return true;

    if (m_TrackTimer.IsExpired() == false)
        return true;

    // 소환체가 아직 목표점에 도달하지 않았더라도, 탐색주기가 다 되었고 소환자가 움직이고 있다면 다시 목표점을 잡는다.
    if (references != 0 && m_pSummoned->IsMoving())
    {
        //  if (!m_TrackTimer.IsExpired()) return;

        // 소환자가 멈춰버리면 목표점을 다시 찾는다.
        if (m_pSummoner->IsMoving() == false)
        {
            FollowSummoner(m_pSummoner);
        }
        else
        {
            m_vSummonerDestPos = m_pSummoned->GetPathExplorer()->GetTargetPos();

            // 소환자가 소환체가 뛰려는 방향의 부채꼴을 벗어난 각에 위치하는가?
            BOOLEAN bOutOfAngle =
                !Math_IsPositionInFanshapedArea(&m_vSummonerPos, &m_vSummonedPos,
                                                &(m_vSummonerDestPos - m_vSummonedPos),
                                                AIParamParser::AICommonInfo.m_wTRACK_INNER_ANGLE);

            // 소환자가 소환체가 뛰려는 목적지에서 멀리있는가?
            BOOLEAN bOutOfRange = \
                Math_VectorLength2(m_vSummonerDestPos, m_vSummonerPos) > SummonFollowDistance;

            if (bOutOfAngle && bOutOfRange)
            {
                FollowSummoner(m_pSummoner);
                // printf("Moving Track!! DiffTick[%d]\n", GetTickCount()-m_dwPrevTick);
                // m_dwPrevTick = GetTickCount();
            }
        }
    }
    else
    {
        FollowSummoner(m_pSummoner);
        //  printf("Stop Track!! DiffTick[%d]\n", GetTickCount()-m_dwPrevTick);
        //  m_dwPrevTick = GetTickCount();
    }
    return true;
}

void
SummonIdle::DecideMovement()
{
    // 소환자가 이동하고 있는 경우에만 이동예상 목표점(TargetPos)을 이용한다.
    Character* m_pSummoner = m_pSummoned->GetSummoner();
    if (m_pSummoner && m_pSummoner->IsMoving())
    {
        m_vSummonerDestPos = m_pSummoner->GetPathExplorer()->GetTargetPos();
    }
    else
    {
        m_vSummonerDestPos = m_vSummonerPos;
    }

    // 목적지 좌표가 유효한 좌표인지 체크
    GameField* pField = m_pSummoned->GetField();
    if (pField)
    {
        if (!pField->IsMovableCoordinates(&m_vSummonerDestPos)) 
        {
            SUNLOG(eMIDDLE_LOG, "[DecideMovement] Is Not MovableCoordinates!");
            m_vSummonerDestPos = m_vSummonerPos;
        }
    }

    // 방향각 결정(소환체와 소환자의 최단거리 방향으로 결정)
    WzVector wvNormal;
    VectorNormalize(&wvNormal, &(m_vSummonedPos - m_vSummonerDestPos));

    float max_value = static_cast<float>(AIParamParser::AICommonInfo.m_fSUMMON_FOLLOW_DISTANCE);
    float min_value = max_value * 0.5f;

    float fDistance = (float)dRandom(min_value, max_value);
    m_vMovement = wvNormal * fDistance;
}

void
SummonIdle::FollowSummoner(Character* m_pSummoner)
{
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    // 고정형 소환수는 움직이지 않는다
    if (m_pSummoned->GetSummonType() == eSUMMON_IMMOVABLE) 
    {
        // 고정형 소환수는 일정거리를 벗어나면 소멸된다.
        if (m_pSummoner->GetDist2(m_pSummoned) > AIParamParser::AICommonInfo.m_fSUMMON_MAX_FOLLOW_DISTANCE *
                                                 AIParamParser::AICommonInfo.m_fSUMMON_MAX_FOLLOW_DISTANCE)
        {
            SummonManager* summon_manager = SummonManager::Instance();
            if (summon_manager != NULL) 
            {
                SummonedNPCs* summoned_list = summon_manager->FindSummonNPCs(m_pSummoner->GetObjectKey());
                if (summoned_list != NULL)
                {
                    summoned_list->Destroy(m_pSummoned->GetObjectKey());
                }
            }
        }
            return;
    }
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
    // 소환자에대한 상대 좌표를 구한다.
    DecideMovement();

    WzVector vDestPos = m_vSummonerDestPos + m_vMovement;

    eCHAR_MOVE_STATE eMoveState = m_pSummoner->GetMoveStateControl()->GetMoveState();
    if (eMoveState != CMS_WALK)
    {
        eMoveState = CMS_RUN;
    }

    // 일정거리를 벗어나면 텔레포트한다.
    if (m_pSummoner->GetDist2(m_pSummoned) > AIParamParser::AICommonInfo.m_fSUMMON_MAX_FOLLOW_DISTANCE *
                                            AIParamParser::AICommonInfo.m_fSUMMON_MAX_FOLLOW_DISTANCE)
    {
        Teleport(m_vSummonerPos);
        return;
    }

    // 길찾기
    if (m_pSummoned->MoveAndBroadcast(&vDestPos, eMoveState) == false)
    {
        vDestPos = m_vSummonerPos;
        if (m_pSummoned->MoveAndBroadcast(&m_vSummonerPos, eMoveState) == false)
        {
            //  SUNLOG(eFULL_LOG, "[FollowSummoner] MoveAndBroadcast Fail!");
        }
    }

    // 리턴상태를 방지하기 위해서 목적지 좌표를 리젠좌표로 셋팅한다.
    m_pSummoned->SetRegenPos(vDestPos);

    m_TrackTimer.SetTimer(m_pAITypeInfo->m_wTRACK_PERIOD);
}

void
SummonIdle::Teleport(WzVector &vDestPos)
{
    GameField* pField = m_pSummoned->GetField();
    if (pField == NULL)
        return;

    pField->TeleportObject(m_pSummoned, &vDestPos);
}


void
SummonIdle::OnMsgAttacked(const AI_MSG* pMsg)
{
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    // 고정형 소환수는 공격상태로 안넘어간다
    if (m_pSummoned->GetSummonType() == eSUMMON_IMMOVABLE) {
        return;
    }
#endif //_NA_007667_20141001_WITCHBLADE_SKILL

    State::OnMsgAttacked(pMsg);

    // 뒤따름 상태이면 반응하지 않는다.
    if (m_pSummoned->IsFollowing())
    {
        return;
    }

    const AI_MSG_ATTACKED* pAttackMsg = static_cast<const AI_MSG_ATTACKED*>(pMsg);

    GameField* pField = m_pSummoned->GetField();
    if (pField == NULL)
        return;

    Character* pAttacker = pField->FindCharacter(pAttackMsg->dwAttackerKey);
    if (pAttacker == NULL)
        return;

    // 현재 선택된 타겟이 없고 상대가 적군이면 공격 상태로 전이
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    State::SetTarget(pMsg, STATE_ID_TRACK);
#else
    if (m_pSummoned->GetTargetChar() == NULL) {
        if (m_pSummoned->IsFriend(pAttacker) == USER_RELATION_ENEMY) {
            m_pSummoned->SetTargetChar(pAttacker);
            m_pSummoned->ChangeState(STATE_ID_TRACK, 0, 0, pAttackMsg->dwMsgID);
        }
    }
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
}

#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
void SummonIdle::UseConstantSkill()
{
    m_pSummoned->SelectSkill();
    SLOTCODE skill_code = m_pSummoned->GetSelectedSkill();
    m_pSummoned->UseSkill(skill_code);
}
#endif //_NA_007667_20141001_WITCHBLADE_SKILL