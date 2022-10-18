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

    // ���� Ÿ�� ��ġ�� ���� ����� ��Ʈ�� ��ǥ�� ã�Ƽ� �̵��Ѵ�.
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
        // �ֺ��� ���� �ִ��� Ž��
        Character* pTargetChar = m_pMonster->SearchTarget();
        if (pTargetChar)
        {
            m_pMonster->SetTargetChar(pTargetChar);
            m_pMonster->ChangeState(STATE_ID_TRACK);
        }
    }

    // �������� ������ �����ΰ�
    if (m_PatrolTimer.IsExpired() && m_pMonster->IsMoving() == false)
    {
        DoPatrol();
    }
    return true;
}

// ������ ���ÿ� �����ϸ� true�� ����
BOOL
PatrolState::DoPatrol()
{
    BOOL bFindPos = false;

    // ���� ��ã�⿡�� ���ݾ� ������ ���� ������
    if (m_bRandomPatrol)
    {
        // ������ ���� ���� �ѹ��� �� �� �ִ��� �õ��� ����.
        bFindPos = true;
        FindNextRandomPos(true);
    }
    else
    {
        // ���� ��Ʈ�� ��ǥ�� ã�´�.
        bFindPos = FindNextPos();
    }

    if (bFindPos == false)
        return false;

    // �̵����� ���θ� üũ�� ��Ŷ�� ������. 
    if (m_pMonster->MoveAndBroadcast(&m_vDestPos, CMS_WALK))
    {
        m_bRandomPatrol = false;
    }
    else
    {
        // ��Ʈ�� ���� ������ ���� ��ǥ�� �̵��� �õ��� ����.
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
    if (m_pMonster->GetRegenID() != 0)  //������ ���� ���� ���.
    {
        if (m_bIncreaseIndex)
            m_vDestPos = state_manager->GetDestPos();
        else
            m_vDestPos = state_manager->GetStartPos();
    }
    else
    {
        // �̵��ؾ��� ��Ʈ�� ��ġ
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
        // ������ġ
        const WzVector& wvCurPos = *m_pMonster->GetPosPtr();

        // �̵��� �Ÿ�
        float MOVE_DISTANCE = (float)dRandom(AIParamParser::AICommonInfo.m_fMIN_MOVE_DISTANCE,
                                             AIParamParser::AICommonInfo.m_fMAX_MOVE_DISTANCE);

        WzVector wvNormal;
        VectorNormalize(&wvNormal, &(m_vDestPos - wvCurPos));
        wvNormal.z = 0;

        // �̵��ؾ��� ������ 45�� �������� ������ ��ġ�� ���´�.
        int iRotateAngle = random(0, 90);
        if (iRotateAngle > 45)
            iRotateAngle = 405 - iRotateAngle;

        Math_ZRotateVector(&wvNormal, m_vDestPos, iRotateAngle);

        m_vDestPos = wvCurPos + m_vDestPos * MOVE_DISTANCE;
    }

    // ���ϻ��¸� �����ϱ� ���ؼ� ������ ��ǥ�� ������ǥ�� �����Ѵ�.
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

    if (m_pMonster->GetRegenID() != 0)  //������ ���� ���� ���.
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

        // ���ϻ��¸� �����ϱ� ���ؼ� ������ ��ǥ�� ������ǥ�� �����Ѵ�.
        m_pMonster->SetRegenPos(m_vDestPos);

        return true;
    }

    // ���Ͱ� ��Ʈ���ؾ��� ���� ������ ��´�.
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

    //���� ����
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
    // ó������ ������ ��ǥ�� ã�°Ŷ��
    if (m_bFirst)
    {
        // ���� Ÿ�ϰ� ���� ����� Ÿ���ε����� ���Ѵ�.
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
            // ���� �� �ִ� Ÿ�ϰ� ������ �ű⼭���� �����Ѵ�.
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
        // ������ ������ �ٽ� ���ƿ´�.
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
        // ���õ� Ÿ�� �ȿ��� ���� ��ǥ�� ���´�.
        m_vDestPos = pMovingPlanner->GetRandomPosInTile(pdwTileIndex[m_iIndexOfTileIndex]);
    }

    // ���ϻ��¸� �����ϱ� ���ؼ� ������ ��ǥ�� ������ǥ�� �����Ѵ�.
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

    // ���� ���õ� Ÿ���� ���� ��밡 �����̸� ���� ���·� ����
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

