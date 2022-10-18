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
    // BattleRecord 업데이트
    m_pBattleRecord->Update();

    // 주변 타겟리스트를 갱신한다.
    if(m_pSearchTimer->IsExpired())
    {
        UpdateTargetList();
    }

    // 새로운 타겟을 찾는다.
    if(m_pCurTarget && m_pAggroTimer->IsExpired())
    {
        // 현재는 최초 타겟을 찾을때는 SearchTarget()을 사용하고 이후에는 BattleRecord가 큰 타겟으로 바꾸는 구조이다.
        // 나중에... 타겟을 찾거나 변경하는 방식을 합쳐야 할 것이다!
        // HP가 약한 타겟을 우선순위로 타겟팅하는 몬스터는 SelectBestTarget()에서도 그대로 속성이 적용되어야 할 것이다.
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

    // 새로운 타겟을 찾은 경우에만 바꾼다.
    if(!pNewTarget || pNewTarget == m_pCurTarget)
        return;

    // 타겟이 공격가능한 상태인지 체크
    if(!pNewTarget->CanBeAttacked())
        return;

    // 적군인지 체크한다.
    if(m_pNPC->IsFriend(pNewTarget) == USER_RELATION_FRIEND)
        return;

    // 타겟변경 확률을 적용한다.
    const AI_TYPE_INFO* pAITypeInfo = m_pBattleRecord->m_pAITypeInfo;
    if(random(0,100) <= pAITypeInfo->m_byTARGET_CHANGE_RATIO)
    {
        SetCurTarget(pNewTarget);
    }
}

void
TargetManager::UpdateTargetList()
{
    //  갱신 목적   - 리스트 갱신이 잘못된 정보가 리스트에서 삭제되지 않고 남는 형태로 불완전하다
    //              - SSQMonster의 갱신 로직은 달라진다. ... 달라질 것 같지는 않다. 다른 곳에서 하면 될 듯.

    GameField* const pField = m_pNPC->GetField();
    if(!pField)
        return;

    StatusManager* const pStatusManager = m_pNPC->GetStatusManager();
    BaseStatus* pStatus = NULL;

    // Return, Help 상태에서는 적들을 등록하지 않는다.
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

    //  기존 리스트 갱신
    //  -   시야 2배 밖, 공격할 수 없는 적들을 BattleRecord에서 삭제한다.
    BattleRecord::Records& battle_records = m_pBattleRecord->GetRecords();
    if(!battle_records.empty())
    {
        const float fSightRange2_2x = (fSightRange*fSightRange) * (2*2); // 시야^2 의 2배

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
    // 고정형 소환수 대신 소환자가 어그로를 쌓는다
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

    // 최초 타겟이면 배틀레코드에 등록!
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

    // 데미지 포인트의 경우 누적데미지를 기준으로 UpdateTotalPoint()를 통해 계산된다.
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
    // 이미 설정되어있는 적이면 다시 설정할 필요 없다.
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

    // 이전에 선택된 타겟이 있다면 타겟슬롯을 Release한다.
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
        // 타겟을 배틀레코드에 등록!
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
        // (BUGFIX) (091216) (WAVERIX) 타깃을 찾을 수 없다고 return처리하면 본래 로직 의도에 어긋날 듯.
        // 하기 사항에 대한 좀 더 검증이 필요하다. change 'return' to 'continue' 그런데... 왜 눈치 못챘지?
        if(!pTarget)
            continue; // 하단에서 clear하므로 수정하지 않는다.__NA000941_080204_TASK_SURVIVAL_SOLOING_QUEST_SYSTEM__

        // NPC를 타겟의 옵져버 리스트에서 제거
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

