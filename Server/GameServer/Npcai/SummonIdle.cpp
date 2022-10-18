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

    // ��ȯü�� ������ �ϴٰ� Idle�� ���ƿ��� �������� �������� �÷��׸� ����.
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
    // ���� �����ð��� ���ؼ� �����ð� ��ٷ��ش�.
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

    // (CHANGES) (100206) (WAVERIX) PCC_CONTINUING_NPC_UPDATE �߰��� ���� AttackState���� ���¸�
    // �ٲ� �ʿ伺�� �����.
    GameField* game_field = m_pSummoned->GetField(); //assert not null
    DWORD references = 0;
    {
        SECTOR_IDX sector_index = m_pSummoned->GetSectorIndex();
        Sector* sector = sector_index != INVALID_SECTOR_INDEX ? game_field->GetSector(sector_index)
                                                              : NULL;
        // 'sector == NULL' �߻��ؼ��� �ȵ�����, ��쿡 ����
        // (3DEngine���� �̹��� ��� ������ ���� ��� ����) ���ɼ��� �����Ѵ�.
        references = sector != NULL ? sector->GetReferenceCount() : 0;
    };

#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    // ������ ��ȯ���� ����������ȯ, �̵�ó�� ��ŵ
    if (m_pSummoned->GetSummonType() == eSUMMON_IMMOVABLE) 
    {
        UseConstantSkill();
    }
    else
    {
        // (lst1024)(080923) ���� �÷��̾� ���ݹ��׸� ���� _NONPK ��� �߰� �ʿ�.
        // Summoned�� SearchTarget�Լ��� �߰� �Ͽ� ���� �÷��̾� ������ ã�� �ʴ� ���δ� ���. 
        // �Ʊ��� ��ų ��� ������ ���� ��� �ڱ� �ڽŵ� �Ʊ��̹Ƿ� SummonIdle���·� ���ƿ��� �ʴ´�. 
        //
        if (references != 0 && // skip target searching
            m_SearchTimer.IsExpired() && (m_pSummoned->IsOffensive() || m_pSummoned->IsNonPVP()))
        {
            // �ֺ��� ���� �ִ��� Ž��
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
    // (lst1024)(080923) ���� �÷��̾� ���ݹ��׸� ���� _NONPK ��� �߰� �ʿ�.
    // Summoned�� SearchTarget�Լ��� �߰� �Ͽ� ���� �÷��̾� ������ ã�� �ʴ� ���δ� ���. 
    // �Ʊ��� ��ų ��� ������ ���� ��� �ڱ� �ڽŵ� �Ʊ��̹Ƿ� SummonIdle���·� ���ƿ��� �ʴ´�. 
    //
    if (references != 0 && // skip target searching
        m_SearchTimer.IsExpired() && (m_pSummoned->IsOffensive() || m_pSummoned->IsNonPVP()))
    {
        // �ֺ��� ���� �ִ��� Ž��
        Character* pTargetChar = m_pSummoned->SearchTarget();
        if (pTargetChar)
        {
            m_pSummoned->SetTargetChar(pTargetChar);
            m_pSummoned->ChangeState(STATE_ID_TRACK);
            return true;
        }
    }
#endif //_NA_007667_20141001_WITCHBLADE_SKILL

    // ��ȯ�ڿ��� �Ÿ��� �������� ���̸� �̵��� ���� �ʴ´�.
    static const float SummonFollowDistance = AIParamParser::AICommonInfo.m_fSUMMON_FOLLOW_DISTANCE *
                                              AIParamParser::AICommonInfo.m_fSUMMON_FOLLOW_DISTANCE;
    float fDistance2 = Math_Vector2DLength2(&(m_vSummonedPos - m_vSummonerPos));
    if (fDistance2 <= SummonFollowDistance)
        return true;

    if (m_TrackTimer.IsExpired() == false)
        return true;

    // ��ȯü�� ���� ��ǥ���� �������� �ʾҴ���, Ž���ֱⰡ �� �Ǿ��� ��ȯ�ڰ� �����̰� �ִٸ� �ٽ� ��ǥ���� ��´�.
    if (references != 0 && m_pSummoned->IsMoving())
    {
        //  if (!m_TrackTimer.IsExpired()) return;

        // ��ȯ�ڰ� ��������� ��ǥ���� �ٽ� ã�´�.
        if (m_pSummoner->IsMoving() == false)
        {
            FollowSummoner(m_pSummoner);
        }
        else
        {
            m_vSummonerDestPos = m_pSummoned->GetPathExplorer()->GetTargetPos();

            // ��ȯ�ڰ� ��ȯü�� �ٷ��� ������ ��ä���� ��� ���� ��ġ�ϴ°�?
            BOOLEAN bOutOfAngle =
                !Math_IsPositionInFanshapedArea(&m_vSummonerPos, &m_vSummonedPos,
                                                &(m_vSummonerDestPos - m_vSummonedPos),
                                                AIParamParser::AICommonInfo.m_wTRACK_INNER_ANGLE);

            // ��ȯ�ڰ� ��ȯü�� �ٷ��� ���������� �ָ��ִ°�?
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
    // ��ȯ�ڰ� �̵��ϰ� �ִ� ��쿡�� �̵����� ��ǥ��(TargetPos)�� �̿��Ѵ�.
    Character* m_pSummoner = m_pSummoned->GetSummoner();
    if (m_pSummoner && m_pSummoner->IsMoving())
    {
        m_vSummonerDestPos = m_pSummoner->GetPathExplorer()->GetTargetPos();
    }
    else
    {
        m_vSummonerDestPos = m_vSummonerPos;
    }

    // ������ ��ǥ�� ��ȿ�� ��ǥ���� üũ
    GameField* pField = m_pSummoned->GetField();
    if (pField)
    {
        if (!pField->IsMovableCoordinates(&m_vSummonerDestPos)) 
        {
            SUNLOG(eMIDDLE_LOG, "[DecideMovement] Is Not MovableCoordinates!");
            m_vSummonerDestPos = m_vSummonerPos;
        }
    }

    // ���Ⱒ ����(��ȯü�� ��ȯ���� �ִܰŸ� �������� ����)
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
    // ������ ��ȯ���� �������� �ʴ´�
    if (m_pSummoned->GetSummonType() == eSUMMON_IMMOVABLE) 
    {
        // ������ ��ȯ���� �����Ÿ��� ����� �Ҹ�ȴ�.
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
    // ��ȯ�ڿ����� ��� ��ǥ�� ���Ѵ�.
    DecideMovement();

    WzVector vDestPos = m_vSummonerDestPos + m_vMovement;

    eCHAR_MOVE_STATE eMoveState = m_pSummoner->GetMoveStateControl()->GetMoveState();
    if (eMoveState != CMS_WALK)
    {
        eMoveState = CMS_RUN;
    }

    // �����Ÿ��� ����� �ڷ���Ʈ�Ѵ�.
    if (m_pSummoner->GetDist2(m_pSummoned) > AIParamParser::AICommonInfo.m_fSUMMON_MAX_FOLLOW_DISTANCE *
                                            AIParamParser::AICommonInfo.m_fSUMMON_MAX_FOLLOW_DISTANCE)
    {
        Teleport(m_vSummonerPos);
        return;
    }

    // ��ã��
    if (m_pSummoned->MoveAndBroadcast(&vDestPos, eMoveState) == false)
    {
        vDestPos = m_vSummonerPos;
        if (m_pSummoned->MoveAndBroadcast(&m_vSummonerPos, eMoveState) == false)
        {
            //  SUNLOG(eFULL_LOG, "[FollowSummoner] MoveAndBroadcast Fail!");
        }
    }

    // ���ϻ��¸� �����ϱ� ���ؼ� ������ ��ǥ�� ������ǥ�� �����Ѵ�.
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
    // ������ ��ȯ���� ���ݻ��·� �ȳѾ��
    if (m_pSummoned->GetSummonType() == eSUMMON_IMMOVABLE) {
        return;
    }
#endif //_NA_007667_20141001_WITCHBLADE_SKILL

    State::OnMsgAttacked(pMsg);

    // �ڵ��� �����̸� �������� �ʴ´�.
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

    // ���� ���õ� Ÿ���� ���� ��밡 �����̸� ���� ���·� ����
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