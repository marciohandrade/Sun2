#include "stdafx.h"
#include <Timer.h>
#include "PatrolState.h"
#include "GameField.h"
#include <3DEngine/3DTerrain/PathExplorer.h>
#include "AIParamParser.h"
#include "Monster.h"
#include "PublicMath.h"
#include "NPCStateManager.h"
#include "MovingPlanner.h"

PatrolState::PatrolState()
{
}

PatrolState::~PatrolState()
{
}

void
PatrolState::Init()
{   
}

void
PatrolState::SetNPC(NPC* pNpc)
{
    m_pMonster = DOWNCASTER_SELECT<Monster*>(pNpc);
    if (m_pMonster == NULL || pNpc->IsEqualObjectKind(MONSTER_OBJECT) == false) {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]|Unexpected type %08X", pNpc->GetObjectType());
        __debugbreak();
    }

    State::SetNPC(pNpc);
}

void
PatrolState::OnEnter(LPARAM param1, LPARAM param2, LPARAM param3)
{
    State::OnEnter(param1, param2, param3);

    m_SearchTimer.SetTimer(m_SearchTimer.eStaticTimer_Search, 0);
    m_PatrolTimer.SetTimer(m_PatrolTimer.eStaticTimer_Track, m_pAITypeInfo->m_wTRACK_PERIOD);

    // 현재 타일 위치랑 가장 가까운 패트롤 좌표를 찾아서 이동한다.
    m_bRandomPatrol = false;
    m_bIncreaseIndex = true;
    m_bFirst = true;
}

void
PatrolState::OnExit()
{
    State::OnExit();
}

BOOL
PatrolState::OnUpdate(DWORD dwDeltaTick)
{
    __UNUSED(dwDeltaTick);

    if (m_SearchTimer.IsExpired())
    {
        // 주변에 적이 있는지 탐색
        Character* pTargetChar = m_pMonster->SearchTarget();
        if (pTargetChar)
        {
            m_pMonster->SetTargetChar(pTargetChar);
            m_pMonster->ChangeState(STATE_ID_TRACK);
        }
    }

    // 목적지에 도달한 상태인가
    if (m_PatrolTimer.IsExpired() && m_pMonster->IsMoving() == false)
    {
        DoPatrol();
    }
    return true;
}

// 목적지 선택에 성공하면 true를 리턴
BOOL
PatrolState::DoPatrol()
{
    BOOL bFindPos = false;

    // 이전 길찾기에서 조금씩 나눠서 가던 중인지
    if (m_bRandomPatrol)
    {
        // 나눠서 가던 길을 한번에 갈 수 있는지 시도해 본다.
        bFindPos = true;
        FindNextRandomPos(true);
    }
    else
    {
        // 다음 패트롤 좌표를 찾는다.
        bFindPos = FindNextPos();
    }

    if (bFindPos == false)
        return false;

    // 이동가능 여부를 체크후 패킷을 날린다. 
    if (m_pMonster->MoveAndBroadcast(&m_vDestPos, CMS_WALK))
    {
        m_bRandomPatrol = false;
    }
    else
    {
        // 패트롤 지점 방향의 랜덤 좌표로 이동을 시도해 본다.
        FindNextRandomPos(false);
        if (m_pMonster->MoveAndBroadcast(&m_vDestPos, CMS_WALK))
        {
            m_bRandomPatrol = true;
        }
    }

    m_PatrolTimer.SetTimer(m_PatrolTimer.eStaticTimer_Track, m_pAITypeInfo->m_wTRACK_PERIOD);

    return true;
}

void
PatrolState::FindNextRandomPos(BOOL direct)
{
    GameField* pField = m_pMonster->GetField();
    if (pField == NULL)
        return;

    NPCStateManager* state_manager = m_pMonster->GetStateManager();
    if (m_pMonster->GetRegenID() != 0)  //맵툴로 찍은 몬스터 라면.
    {
        if (m_bIncreaseIndex)
            m_vDestPos = state_manager->GetDestPos();
        else
            m_vDestPos = state_manager->GetStartPos();
    }
    else
    {
        // 이동해야할 패트롤 위치
        WzSpecialArea* pBaseAreaInfo = \
            pField->GetFieldInfo()->GetAreaInfo(state_manager->GetMoveAreaID());
        if (pBaseAreaInfo == NULL)
        {
            SUNLOG(eFULL_LOG,
                   "[PatrolState::FindNextRandomPos] MonsterCode[%d] MoveAreaInfo doesn't exist!",
                   m_pMonster->GetBaseInfo()->m_MonsterCode);
            return;
        }

        if (pBaseAreaInfo ->m_wbvRegion.m_eBvType != BVT_PATH_TILE_INDEX)
        {
            SUNLOG(eCRITICAL_LOG,
                   "[PatrolState::FindNextRandomPos] MonsterCode[%d] AreaType is Invalid!!",
                   m_pMonster->GetBaseInfo()->m_MonsterCode);
            return;
        }

        DWORD* pdwTileIndex = pBaseAreaInfo->m_wbvRegion.m_BvPathTileIndex.m_pdwData;
        nsAI::MovingPlanner* const pMovingPlanner = pField->GetMovingPlanner();
        m_vDestPos = pMovingPlanner->GetRandomPosInTile(pdwTileIndex[m_iIndexOfTileIndex]);
    }

    if (direct == false)
    {
        // 현재위치
        const WzVector& wvCurPos = *m_pMonster->GetPosPtr();

        // 이동할 거리
        float MOVE_DISTANCE = (float)dRandom(AIParamParser::AICommonInfo.m_fMIN_MOVE_DISTANCE,
                                             AIParamParser::AICommonInfo.m_fMAX_MOVE_DISTANCE);

        WzVector wvNormal;
        VectorNormalize(&wvNormal, &(m_vDestPos - wvCurPos));
        wvNormal.z = 0;

        // 이동해야할 방향의 45도 안팍으로 랜덤한 위치를 얻어온다.
        int iRotateAngle = random(0, 90);
        if (iRotateAngle > 45)
            iRotateAngle = 405 - iRotateAngle;

        Math_ZRotateVector(&wvNormal, m_vDestPos, iRotateAngle);

        m_vDestPos = wvCurPos + m_vDestPos * MOVE_DISTANCE;
    }

    // 리턴상태를 방지하기 위해서 목적지 좌표를 리젠좌표로 셋팅한다.
    m_pMonster->SetRegenPos(m_vDestPos);
}

BOOL
PatrolState::FindNextPos()
{
    GameField* pField = m_pMonster->GetField();
    if (pField == NULL)
        return false;

    //const WzVector& wvCurPos = *m_pMonster->GetPosPtr();
    //WzVector wvTempPos;

    NPCStateManager* state_manager = m_pMonster->GetStateManager();

    if (m_pMonster->GetRegenID() != 0)  //맵툴로 찍은 몬스터 라면.
    {
        if (m_bFirst)
        {
            // (CHANGES) (WAVERIX) fault! fix up invalid calculation which is dropped base position
            const WzVector& cur_pos = *m_pMonster->GetPosPtr();
            float dist_with_start = VectorLength2(&(cur_pos - state_manager->GetStartPos()));
            float dist_with_dest = VectorLength2(&(cur_pos - state_manager->GetDestPos()));
            if (dist_with_start > dist_with_dest) {
                m_bIncreaseIndex = false;
            }
            else {
                m_bIncreaseIndex = true;
            }
        }

        if (m_bIncreaseIndex) {
            m_vDestPos = state_manager->GetDestPos();
        }
        else {
            m_vDestPos = state_manager->GetStartPos();
        }

        m_bIncreaseIndex = !m_bIncreaseIndex;

        // 리턴상태를 방지하기 위해서 목적지 좌표를 리젠좌표로 셋팅한다.
        m_pMonster->SetRegenPos(m_vDestPos);

        return true;
    }

    // 몬스터가 패트롤해야할 영역 정보를 얻는다.
    WzSpecialArea* pBaseAreaInfo = pField->GetFieldInfo()->GetAreaInfo(state_manager->GetMoveAreaID());
    if (pBaseAreaInfo == NULL)
    {
        //  SUNLOG(eFULL_LOG, "[PatrolState::FindNextPos] MonsterCode[%d] MoveAreaInfo doesn't exist!", m_pMonster->GetBaseInfo()->m_MonsterCode);
        return false;
    }

    if (pBaseAreaInfo ->m_wbvRegion.m_eBvType != BVT_PATH_TILE_INDEX)
    {
        //  SUNLOG(eCRITICAL_LOG, "[PatrolState::FindNextPos] MonsterCode[%d] AreaType is Invalid!!", m_pMonster->GetBaseInfo()->m_MonsterCode);
        return false;
    }

    //영역 정보
    DWORD dwAreaTileCnt = pBaseAreaInfo->m_wbvRegion.m_BvPathTileIndex.m_nCount;
    if (dwAreaTileCnt == 0)
    {
        SUNLOG(eFULL_LOG, "[PatrolState::FindNextPos] MonsterCode[%d] dwAreaTileCnt is Zero!",
               m_pMonster->GetBaseInfo()->m_MonsterCode);
        return false;
    }

    DWORD* pdwTileIndex = pBaseAreaInfo->m_wbvRegion.m_BvPathTileIndex.m_pdwData;
    if (pdwTileIndex == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "[PatrolState::FindNextPos] MonsterCode[%d] pdwTileIndex is NULL!",
               m_pMonster->GetBaseInfo()->m_MonsterCode);
        return false;
    }

    nsAI::MovingPlanner* const pMovingPlanner = pField->GetMovingPlanner();
    // 처음으로 목적지 좌표를 찾는거라면
    if (m_bFirst)
    {
        // 현재 타일과 가장 가까운 타일인덱스를 구한다.
        m_iIndexOfTileIndex = 0;
        int iCurTileIndex = m_pMonster->GetPathExplorer()->GetTile();

        float fMinDistance = 0.f, fCurDistance = 0.f;
        m_bFirst = false;

        const WzVector& wvCurPos = *m_pMonster->GetPosPtr();
        WzVector wvTempPos;
        // (CHANGES) (091228) (WAVERIX) changes tile selector
        for (DWORD index = 0; index < dwAreaTileCnt; ++index)
        {
        #if WAVERIX_CODE_RENEWAL
            wvTempPos = pMovingPlanner->GetRandomPosInTile(pdwTileIndex[index]);
        #endif
            // 현재 서 있는 타일과 같으면 거기서부터 시작한다.
            if (iCurTileIndex == pdwTileIndex[index])
            {
                m_iIndexOfTileIndex = index;
                m_vDestPos = wvTempPos;
                break;
            }

            // (BUGFIX) (091223) (WAVERIX) remove invalid usage that is use uninitialized 'wvTempPos'
        #if WAVERIX_CODE_RENEWAL
            fCurDistance = VectorLength2(&(wvTempPos - wvCurPos));
        #endif
        #if WAVERIX_CODE_BACKUP
            m_vDestPos = pMovingPlanner->GetRandomPosInTile(pdwTileIndex[index]);
            fCurDistance = VectorLength(&(wvTempPos - wvCurPos));
        #endif

            if (index == 0 || fCurDistance < fMinDistance)
            {
                fMinDistance = fCurDistance;
                m_iIndexOfTileIndex = index;
                m_vDestPos = wvTempPos;
            }
        }

        if (fMinDistance >= 30)
        {
            //SUNLOG(eCRITICAL_LOG, "[FindNextPos::First] MonsterCode[%d] fMinDistance[%f]",
            //       m_pMonster->GetBaseInfo()->m_MonsterCode, fMinDistance);
        }

        // current state : setup 'm_vDestPos'
    }
    else
    {
        // 끝까지 갔으면 다시 돌아온다.
        if ((m_iIndexOfTileIndex + 1) == dwAreaTileCnt)
            m_bIncreaseIndex = false;
        else if (m_iIndexOfTileIndex == 0)
            m_bIncreaseIndex = true;

        if (m_bIncreaseIndex)
        {
            ++m_iIndexOfTileIndex;
            m_iIndexOfTileIndex = min((int)(dwAreaTileCnt - 1), m_iIndexOfTileIndex);
        }
        else
        {
            --m_iIndexOfTileIndex;
            m_iIndexOfTileIndex = max(0, m_iIndexOfTileIndex);
        }
        // 선택된 타일 안에서 랜덤 좌표를 얻어온다.
        m_vDestPos = pMovingPlanner->GetRandomPosInTile(pdwTileIndex[m_iIndexOfTileIndex]);
    }

    // 리턴상태를 방지하기 위해서 목적지 좌표를 리젠좌표로 셋팅한다.
    m_pMonster->SetRegenPos(m_vDestPos);

    return true;
}

void
PatrolState::OnMsgAttacked(const AI_MSG* pMsg)
{
    State::OnMsgAttacked(pMsg);

    const AI_MSG_ATTACKED* pAttackMsg = static_cast<const AI_MSG_ATTACKED*>(pMsg);

    GameField* pField = m_pMonster->GetField();
    if (pField == NULL)
        return;

    Character* pAttacker = pField->FindCharacter(pAttackMsg->dwAttackerKey);
    if (pAttacker == NULL)
        return;

    // 현재 선택된 타겟이 없고 상대가 적군이면 공격 상태로 전이
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    State::SetTarget(pMsg, STATE_ID_TRACK);
#else
    if (m_pMonster->GetTargetChar() == NULL &&
        m_pMonster->IsFriend(pAttacker) == USER_RELATION_ENEMY)
    {
        m_pMonster->SetTargetChar(pAttacker);
        m_pMonster->ChangeState(STATE_ID_TRACK, 0, 0, pAttackMsg->dwMsgID);
    }
#endif //_NA_007667_20141001_WITCHBLADE_SKILL

}

