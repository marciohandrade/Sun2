#include "stdafx.h"
#include "TargetManager.h"

// Commons
#include "PublicMath.h"
#include "AIParamParser.h"

#include "GameServerEx.h"
#include "MovingPlanner.h"
#include "BaseStatus.h"
#include "StatusManager.h"
#include "BattleRecord.h"

#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
#include "Summoned.h"
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
//==================================================================================================

TargetManager::TargetManager(NPC* pNPC)
    : m_pNPC(pNPC)
    , m_pCurTarget(NULL)
{
    m_pBattleRecord = new BattleRecord(pNPC);
}

TargetManager::~TargetManager()
{
    SAFE_DELETE(m_pBattleRecord);
}

void
TargetManager::Init()
{
    m_byTrackSlot = TRACK_SLOT_NONE;
    m_pCurTarget = NULL;
    m_pBattleRecord->Init();

    const AI_TYPE_INFO* pAITypeInfo = m_pBattleRecord->m_pAITypeInfo;
    m_pSearchTimer->SetTimer(pAITypeInfo->m_wSEARCH_TARGET_TIME);
    m_pAggroTimer->SetTimer(pAITypeInfo->m_wAGGRO_TIME);
}

//==================================================================================================


void
TargetManager::Update()
{
    // BattleRecord ������Ʈ
    m_pBattleRecord->Update();

    // �ֺ� Ÿ�ٸ���Ʈ�� �����Ѵ�.
    if(m_pSearchTimer->IsExpired())
    {
        UpdateTargetList();
    }

    // ���ο� Ÿ���� ã�´�.
    if(m_pCurTarget && m_pAggroTimer->IsExpired())
    {
        // ����� ���� Ÿ���� ã������ SearchTarget()�� ����ϰ� ���Ŀ��� BattleRecord�� ū Ÿ������ �ٲٴ� �����̴�.
        // ���߿�... Ÿ���� ã�ų� �����ϴ� ����� ���ľ� �� ���̴�!
        // HP�� ���� Ÿ���� �켱������ Ÿ�����ϴ� ���ʹ� SelectBestTarget()������ �״�� �Ӽ��� ����Ǿ�� �� ���̴�.
        if(m_pNPC->IsDelegate())      
            SelectBestTarget();

#ifdef _NA_0_20100819_AGGRO_RENEWAL	
        if (IsEmptyAggroDisplayUser() == false) {
		    NotifyAggroValue();
        }
#endif
    }
}

void
TargetManager::SelectBestTarget()
{
    Character* pNewTarget = m_pBattleRecord->SelectBestTarget();

    // ���ο� Ÿ���� ã�� ��쿡�� �ٲ۴�.
    if(!pNewTarget || pNewTarget == m_pCurTarget)
        return;

    // Ÿ���� ���ݰ����� �������� üũ
    if(!pNewTarget->CanBeAttacked())
        return;

    // �������� üũ�Ѵ�.
    if(m_pNPC->IsFriend(pNewTarget) == USER_RELATION_FRIEND)
        return;

    // Ÿ�ٺ��� Ȯ���� �����Ѵ�.
    const AI_TYPE_INFO* pAITypeInfo = m_pBattleRecord->m_pAITypeInfo;
    if(random(0,100) <= pAITypeInfo->m_byTARGET_CHANGE_RATIO)
    {
        SetCurTarget(pNewTarget);
    }
}

void
TargetManager::UpdateTargetList()
{
    //  ���� ����   - ����Ʈ ������ �߸��� ������ ����Ʈ���� �������� �ʰ� ���� ���·� �ҿ����ϴ�
    //              - SSQMonster�� ���� ������ �޶�����. ... �޶��� �� ������ �ʴ�. �ٸ� ������ �ϸ� �� ��.

    GameField* const pField = m_pNPC->GetField();
    if(!pField)
        return;

    StatusManager* const pStatusManager = m_pNPC->GetStatusManager();
    BaseStatus* pStatus = NULL;

    // Return, Help ���¿����� ������ ������� �ʴ´�.
    pStatus = pStatusManager->FindStatus(eCHAR_STATE_ETC_RETURNING);
    if(pStatus)
        return;
    pStatus = pStatusManager->FindStatus(eCHAR_STATE_ETC_HELP);
    if(pStatus)
        return;

    //_NA_0_20100222_UNIT_TRIGGERS_FRIEND_MONSTER
    pStatus = pStatusManager->FindStatus(eCHAR_STATE_ETC_SKIP_UPDATE_TARGET_LIST);
    if(pStatus)
        return;

    if (m_pNPC->GetSearchType() == eRARGET_SEARCH_NOT_SEARCH_AND_TARGET_CHNAGE)
        return;

    const WzVector& wvNPCPos = *m_pNPC->GetPosPtr();
    const float fSightRange = m_pNPC->GetSightRange();

    //  ���� ����Ʈ ����
    //  -   �þ� 2�� ��, ������ �� ���� ������ BattleRecord���� �����Ѵ�.
    BattleRecord::Records& battle_records = m_pBattleRecord->GetRecords();
    if(!battle_records.empty())
    {
        const float fSightRange2_2x = (fSightRange*fSightRange) * (2*2); // �þ�^2 �� 2��

        BattleRecord::Records::iterator it = battle_records.begin(), itend = battle_records.end();
        while(it != itend)
        {
            BattleRecord::Node& rRecord = it->second;
            Character* const pTarget = pField->FindCharacter(rRecord.GetObjectKey());
            if(!pTarget)
            {
                it = battle_records.erase(it);
                continue;
            }

			/*SUNLOG(eCRITICAL_LOG, "|Total=%u|Class=%u|is1st=%u|Distance=%u|LV=%u|HP=%u|Damage=%u|Etc=%u|",
				rRecord.GetBattlePoint(BATTLE_POINT_TOTAL), rRecord.GetBattlePoint(BATTLE_POINT_CLASS),
				rRecord.GetBattlePoint(BATTLE_POINT_FIRST), rRecord.GetBattlePoint(BATTLE_POINT_DISTANCE),
				rRecord.GetBattlePoint(BATTLE_POINT_LEVEL), rRecord.GetBattlePoint(BATTLE_POINT_HP),
				rRecord.GetBattlePoint(BATTLE_POINT_DAMAGE), rRecord.GetBattlePoint(BATTLE_POINT_ETC));*/

            if(!pTarget->CanBeAttacked())
            {
                it = battle_records.erase(it);
                continue;
            }

            const WzVector& wvTargetPos = *pTarget->GetPosPtr();
            const BOOLEAN outRange = (Math_VectorLength2(wvNPCPos, wvTargetPos) > fSightRange2_2x);
            if(outRange)
            {
                it = battle_records.erase(it);
                continue;
            }
            
            WORD skip_check_tile_attr_for_target = 0;
            if (pTarget->IsEqualObjectKind(MAPNPC_OBJECT)) 
            {
                skip_check_tile_attr_for_target = PTA_NO_WALK;
            }

            if (!nsAI::PlanEventObject::AcceptableTarget(pTarget, PTA_SAFETY_ZONE, 
                                                         skip_check_tile_attr_for_target))
            {
                it = battle_records.erase(it);
                continue;
            }

            ++it;
        }
    }
    //_NA000000_110210_ENHANCED_TARGET_SEARCH_ALGORITHM
    ns_object::SearchOption::NpcAI::result_type search_result;
    ns_object::SearchOption::NpcAI search_option(m_pNPC);
    if (pField->FindTargetsForNpcAI(search_option, &search_result) == false) {
        return;
    };
    ;{
        Character** it = search_result.targets,
                 ** end = &search_result.targets[search_result.number_of_selected_chars];
        for ( ; it != end; ++it)
        {
            Character* character = *it;
            AddBattlePoint(character, BATTLE_POINT_DAMAGE, 0);
        }
    };
}

void
TargetManager::AddBattlePoint(Character* pAttacker, eBATTLE_POINT_TYPE ePointType, int iBattlePoint)
{
    if(!pAttacker)                        
        return;

    if(m_pNPC->IsFriend(pAttacker) != USER_RELATION_ENEMY)
        return;

#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    // ������ ��ȯ�� ��� ��ȯ�ڰ� ��׷θ� �״´�
    if (pAttacker->IsEqualObjectKind(SUMMON_OBJECT) == true && 
        pAttacker->GetSummonType() == eSUMMON_IMMOVABLE)
    {
        GameField* field = m_pNPC->GetField();
        if (field != NULL) 
        {
            DWORD summoner_key = pAttacker->GetSummonerKey();
            Character* summoner = field->FindCharacter(summoner_key);
            if (summoner != NULL) {
                pAttacker = summoner;
            }
        }
    }
#endif //_NA_007667_20141001_WITCHBLADE_SKILL

    BattleRecord::Node* pRecord = m_pBattleRecord->Get(pAttacker->GetObjectKey());

    // ���� Ÿ���̸� ��Ʋ���ڵ忡 ���!
    if(pRecord == NULL)
    {
        pRecord = m_pBattleRecord->Add(pAttacker);
        /*
#ifdef _NA_20100322_AGGRO_DISPLAY        
        if(SUMMON_OBJECT != m_pNPC->GetObjectType() &&
            NULL != m_pNPC->GetTargetChar())
        {
            m_pBattleRecord->SetFirstVictim(pRecord);
        }        
#endif    
        */
    }

    // ������ ����Ʈ�� ��� ������������ �������� UpdateTotalPoint()�� ���� ���ȴ�.
    if(ePointType == BATTLE_POINT_DAMAGE)
    {
        pRecord->PlusDamage(iBattlePoint);

        if(iBattlePoint)
            m_pBattleRecord->SetFirstAttacker(pRecord);
    }
    else if(ePointType == BATTLE_POINT_ETC)
    {
        iBattlePoint += pRecord->GetBattlePoint(BATTLE_POINT_ETC);
        pRecord->SetBattlePoint(BATTLE_POINT_ETC, iBattlePoint);
    }
    else
    {
        pRecord->PlusDamage(iBattlePoint);
        SUNLOG(eFULL_LOG, "[AddBattlePoint] Invalid ePointType[%d]", ePointType);
    }
}

void
TargetManager::SetCurTarget(Character* pTarget)
{
    // �̹� �����Ǿ��ִ� ���̸� �ٽ� ������ �ʿ� ����.
    if(m_pCurTarget == pTarget)
    {
        return;
    }

//#ifdef _NA_004126_20120111_CHANGE_GAME_SERVER_OBJECT_FACTORY
    if (pTarget != NULL && pTarget->IsDeleted())
    {
        pTarget = NULL;
    }
//#endif

    // ������ ���õ� Ÿ���� �ִٸ� Ÿ�ٽ����� Release�Ѵ�.
    if (m_pCurTarget != NULL)
    {
//#ifdef _NA_004126_20120111_CHANGE_GAME_SERVER_OBJECT_FACTORY
        if (m_pCurTarget->IsDeleted() == false)
        {
            m_pCurTarget->FreeEnemySlot(m_byTrackSlot);
            m_pCurTarget->GetObservers().Remove(m_pNPC->GetObjectKey());
        }
//#else
//        m_pCurTarget->FreeEnemySlot(m_byTrackSlot);
//        m_pCurTarget->GetObservers().Remove(m_pNPC->GetObjectKey());
//#endif
        const DWORD object_key = m_pCurTarget->GetObjectKey();
        m_pBattleRecord->Remove(object_key);
    }

    m_pCurTarget = pTarget;
    m_byTrackSlot = TRACK_SLOT_NONE;

    if (m_pCurTarget != NULL)
    {
        // Ÿ���� ��Ʋ���ڵ忡 ���!
        AddBattlePoint(pTarget, BATTLE_POINT_DAMAGE, 0);
        m_pCurTarget->GetObservers().Add(m_pNPC);
    }
}

void
TargetManager::RemoveTarget(DWORD leaveEnemyKey)
{
    if(m_pCurTarget && m_pCurTarget->GetObjectKey() == leaveEnemyKey)
    {
        SetCurTarget(NULL);
    }
}

void
TargetManager::RemoveBattleRecord(DWORD ObjKey)
{
    m_pBattleRecord->Remove(ObjKey);
}

void
TargetManager::ClearBattleRecord()
{
    GameField* pField = m_pNPC->GetField();
    if(!pField)
        return;

    const DWORD npc_object_key = m_pNPC->GetObjectKey();

    BattleRecord::Records records = m_pBattleRecord->GetRecords();
    BattleRecord::Records::const_iterator it = records.begin(),
                                          itend = records.end();
    for( ; it != itend; ++it)
    {
        const BattleRecord::Node* pRecord = &it->second;

        Character* pTarget = pField->FindCharacter(pRecord->GetObjectKey());
        // (BUGFIX) (091216) (WAVERIX) Ÿ���� ã�� �� ���ٰ� returnó���ϸ� ���� ���� �ǵ��� ��߳� ��.
        // �ϱ� ���׿� ���� �� �� ������ �ʿ��ϴ�. change 'return' to 'continue' �׷���... �� ��ġ ��ë��?
        if(!pTarget)
            continue; // �ϴܿ��� clear�ϹǷ� �������� �ʴ´�.__NA000941_080204_TASK_SURVIVAL_SOLOING_QUEST_SYSTEM__

        // NPC�� Ÿ���� ������ ����Ʈ���� ����
        pTarget->GetObservers().Remove(npc_object_key);
    }

    SetCurTarget(NULL);
    m_pBattleRecord->Clear();
}

#ifdef _NA_0_20100819_AGGRO_RENEWAL
void 
TargetManager::AddAggroDisplayUser(const DWORD object_key)
{
    aggro_moitor_users_.clear();
	aggro_moitor_users_.push_back(object_key);
}

void 
TargetManager::NotifyAggroValue()
{
	MSG_CG_AGGRO_VALUES_CMD msg;

	msg.aggro_player_count_ = 0;

	BattleRecord::Records records = m_pBattleRecord->GetRecords();
	BattleRecord::Records::const_iterator it = records.begin(),
		itend = records.end();

	for( ; it != itend; ++it )
	{
		const BattleRecord::Node* pRecord = &it->second;

		Player *target_player = PlayerManager::Instance()->FindPlayerByObjKey( pRecord->GetObjectKey() );

		if(target_player)
		{
			_tcsncpy(msg.aggor_info_[msg.aggro_player_count_].character_name_,  
				     target_player->GetCharName(), 
					 _countof(msg.aggor_info_[msg.aggro_player_count_].character_name_));

			msg.aggor_info_[msg.aggro_player_count_].character_name_[_countof(msg.aggor_info_[msg.aggro_player_count_].character_name_) - 1] = '\0';						
			msg.aggor_info_[msg.aggro_player_count_++].aggro_value_ = pRecord->GetCalcTotalPoint();
		}
        // CHANGES: f101206.1L, added a bound checker
        if (msg.aggro_player_count_ >= _countof(msg.aggor_info_)) {
            break;
        }
	}

	if(msg.aggro_player_count_ <= 0)
		return;

    AGGRO_MONITOR_USER_ITER iter = aggro_moitor_users_.begin(),
        iterend = aggro_moitor_users_.end();
	for(; iter != iterend ; iter++)
	{
		Player *player = PlayerManager::Instance()->FindPlayerByObjKey( (*iter) );

		if( player )
		{
			player->SendPacket(&msg,msg.GetSize());
		}
	}
}
#endif

