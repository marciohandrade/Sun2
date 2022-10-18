#include "stdafx.h"
#include "BattleRecord.h"
// Commons
#include "AIParamParser.h"

// GameServers
#include "NPC.h"
#include "Player.h"
#include "GameField.h"

#ifdef _NA_20100322_AGGRO_DISPLAY
#include "Summoned.h"
#endif

//==================================================================================================
//
//AIParamParser*
//BattleRecord::ms_pAIParamParser = NULL;
//#define AIParamParser_Instance()    ms_pAIParamParser

//==================================================================================================
//
//                                      BATTLE_RECORD
//
//==================================================================================================
//

void
BattleRecord::Node::Update()
{
    // TotalPoint도 주기적으로 감소 시킨다
    AI_COMMON_INFO& stAICommonInfo = AIParamParser::Instance()->GetCommonInfo();
    static float fCalcDAMAGE_POINT_REDUCE_RATIO = (1 - (float)stAICommonInfo.m_byDAMAGE_POINT_REDUCE_RATIO / 1000.0f);

    DWORD dwTotalPoint = (DWORD)(m_wBattlePoint[BATTLE_POINT_TOTAL] * fCalcDAMAGE_POINT_REDUCE_RATIO);
    SetBattlePoint(BATTLE_POINT_TOTAL, dwTotalPoint);
}

void
BattleRecord::Node::SetBattlePoint(eBATTLE_POINT_TYPE ePointType, int iPoint)
{
    if(iPoint == 0)
    {
        return;
    }

    if(ePointType == BATTLE_POINT_FIRST)
    {
        m_bFirstAttack = true;
    }

    // 전체 포인트를 설정할 경우에는 하위 포인트들을 모두 변경시켜야 한다.
    if(ePointType == BATTLE_POINT_TOTAL)
    {
        float fIncRatio = 0.01f;
        if(m_wBattlePoint[BATTLE_POINT_TOTAL]) {
            fIncRatio += (float)iPoint / m_wBattlePoint[BATTLE_POINT_TOTAL];
        } else {
            fIncRatio = 0;
        }

        for(int i = BATTLE_POINT_DISTANCE; i <= BATTLE_POINT_ETC; ++i)
        {
            m_wBattlePoint[i] = (WORD)(m_wBattlePoint[i] * fIncRatio);
        }
    }
    else
    {
        m_wBattlePoint[ePointType] = max(0, iPoint);
    }
    // 전체 포인트 재계산
    m_wBattlePoint[BATTLE_POINT_TOTAL] = 0;

#ifdef _NA_0_20100819_AGGRO_RENEWAL //새로운 어그로 시스템에서 캐릭터별 어그로는 전체 어그로의 비율로 사용된다.
    for(int i=BATTLE_POINT_CLASS + 1 ; i<=BATTLE_POINT_ETC ; ++i)
#else
	for(int i=BATTLE_POINT_CLASS ; i<=BATTLE_POINT_ETC ; ++i)
#endif
    {
        m_wBattlePoint[BATTLE_POINT_TOTAL] += m_wBattlePoint[i];
    }
}


//==================================================================================================
//
//                                      BattleRecord
//
//==================================================================================================
BattleRecord::BattleRecord(NPC* pNPC)
    : m_LastUpdateTick(0)
    , m_pNPC(pNPC)
{
}

BattleRecord::~BattleRecord()
{
}

void
BattleRecord::Init()
{   
    m_Records.clear();
    m_LastUpdateTick = 0;
    m_bFirstAttacked = false;

    const BASE_NPCINFO* pNpcInfo = m_pNPC->GetBaseInfo();
    m_pAITypeInfo = AIParamParser::Instance()->GetTypeInfo(pNpcInfo->m_wAICode);
    assert(m_pAITypeInfo != 0);
}

BattleRecord::Node*
BattleRecord::Add(Character* pAttacker)
{
    const DWORD object_key = pAttacker->GetObjectKey();
    BattleRecord::Node* pRecord = Get(object_key);

    if(pRecord == NULL)
    {
        pRecord = &m_Records[object_key];
        pRecord->SetObjectKey(object_key);
#ifdef _NA_0_20100819_AGGRO_RENEWAL
		pRecord->SetAIPaserInfo(const_cast<AI_TYPE_INFO*>(m_pAITypeInfo));
#endif
    }

	sPOLICY_CHARACTER& rPolicyment = pAttacker->Policyment;
	if(rPolicyment.FIELD & rPolicyment.PCC_BATTLE_RECORD_PRIORITY)
	{
		if(pAttacker->IsEqualObjectKind(PLAYER_OBJECT))
		{
			Player* pPlayer = (Player*)pAttacker;

			// 플레이어 클래스를 체크한 후 포인트를 준다.
			int iPoint = m_pAITypeInfo->m_wCLASS_BASE_POINT[pPlayer->GetCharType()];
			pRecord->SetBattlePoint(BATTLE_POINT_CLASS, iPoint);
		}
#ifdef _NA_20100322_AGGRO_DISPLAY
        else if(pAttacker->IsEqualObjectKind(SUMMON_OBJECT))
        {
            Summoned* const target_summoned = DOWNCASTER_SELECT<Summoned*>(pAttacker);
            Player* const owner_player = target_summoned->GetPlayerOwner();
            if(owner_player)
            {
                int iPoint = m_pAITypeInfo->m_wCLASS_BASE_POINT[owner_player->GetCharType()];
                pRecord->SetBattlePoint(BATTLE_POINT_CLASS, iPoint);                
            }
        }    
#endif
	};    

    return pRecord;
}

BOOL
BattleRecord::Remove(DWORD object_key)
{
    if(object_key)
    {
        Records::iterator iRecord = m_Records.find(object_key);

        if(iRecord != m_Records.end())
        {
        #ifdef _DEBUG
            BattleRecord::Node* pRecord = &iRecord->second; // to support debug
        #endif
            m_Records.erase(iRecord);
            return true;
        }
    }

    return false;       
}


void
BattleRecord::Update()
{
    DWORD dwCurTick = GetTickCount();
    if((dwCurTick - m_LastUpdateTick) < m_pAITypeInfo->m_wBATTLE_RECORD_UPDATE_TIME)
        return;

    //  (WAVERIX)(080320)   - 지독한 루프다. 장비 성능에 감사를...
    UpdateTotalPoint(true);

    m_LastUpdateTick = dwCurTick;
}   

void
BattleRecord::SetBattlePoint(DWORD object_key, eBATTLE_POINT_TYPE ePointType, int iPoint)
{
    BattleRecord::Node* pRecord = Get(object_key);
    if(!pRecord)
        return;

    pRecord->SetBattlePoint(ePointType, iPoint);
}

BATTLEPOINT
BattleRecord::GetBattlePoint(DWORD object_key, eBATTLE_POINT_TYPE ePointType) const
{
    const BattleRecord::Node* pRecord = Get(object_key);
    if(!pRecord)
        return 0;

    return pRecord->GetBattlePoint(ePointType);
}

#ifdef _NA_0_20100819_AGGRO_RENEWAL
BATTLEPOINT
BattleRecord::GetCalcTotalPoint(DWORD object_key) const
{
	const BattleRecord::Node* pRecord = Get(object_key);
	if(!pRecord)
		return 0;

	return pRecord->GetCalcTotalPoint();
}

BATTLEPOINT
BattleRecord::Node::GetCalcTotalPoint() const {
	float character_rate = (float)m_wBattlePoint[BATTLE_POINT_CLASS] / 100.0f;
#ifdef _NA_20100322_AGGRO_DISPLAY
	float distance_rate  = 100.0f; // 거래가 멀어지면 백분율로 100 어그로 수치를 갖는다.
#else
    float distance_rate  = m_pAITypeInfo->m_Distance_Info[AGGRO_CHANGE_RATIO_MAX].change_point_;
#endif

	for(int i = 0 ; i < AGGRO_CHANGE_RATIO_MAX - 1; ++i)
	{		
		if(GetTargetDistance() < m_pAITypeInfo->m_Distance_Info[i].change_range_)
		{
			distance_rate = m_pAITypeInfo->m_Distance_Info[i].change_point_;
			break;
		}		
	}

	distance_rate /= 100.0f;
	
	return (BATTLEPOINT)(m_wBattlePoint[BATTLE_POINT_TOTAL] * character_rate * distance_rate);
}
#endif

void
BattleRecord::SetFirstAttacker(BattleRecord::Node* pRecord)
{
    if(!m_bFirstAttacked)
    {
        m_bFirstAttacked = true;
        pRecord->SetBattlePoint(BATTLE_POINT_FIRST, m_pAITypeInfo->m_wFIRST_ATTACK_POINT);
    }
}

#ifdef _NA_20100322_AGGRO_DISPLAY
void
BattleRecord::SetFirstVictim(BattleRecord::Node* pRecord)
{ 
    pRecord->SetBattlePoint(BATTLE_POINT_ETC, m_pAITypeInfo->m_wFIRST_ATTACK_POINT);
}
#endif

//---------------------------------------------------------------------------------------------
//  각종 항목에 대한 포인트를 매긴다.
//  - (WAVERIX)(080320) - 문제점 목록
//  -   1) 초기 계산 값들에 캐릭터가 없는 경우가 배제되어 있다.
//  -   2) 지독한 루프가 수행된다.
//  -   3) begin이 잘못된 경우는?
//---------------------------------------------------------------------------------------------
void
BattleRecord::UpdateTotalPoint(const BOOLEAN need_battlerecord_update)
{
    GameField* pField = m_pNPC->GetField();
    if(!pField)
        return;

    if(m_Records.empty())
        return;

	Records::iterator it;
	Records::iterator itend;

#ifndef _NA_0_20100819_AGGRO_RENEWAL
    // 준비 작업
    DWORD dwTotalDamage = 0;
    it = m_Records.begin();

    itend = m_Records.end();
    for(it = m_Records.begin(); it != itend ; ++it)
    {
        BattleRecord::Node& record = it->second;
        if(need_battlerecord_update)
            record.Update();
        // 플레이어들의 총 누적 데미지를 구해둔다.       
        dwTotalDamage += record.GetPointDamage();
    }
#endif

    //---------------------------------------------------------------------------------------------
    // 적 리스트를 순회하면서 각 항목에 해당하는 포인트를 받을 캐릭터를 찾는다.
    //---------------------------------------------------------------------------------------------

    // 초기값 설정
    float fShortestDistance = 1000.0f;
    LEVELTYPE lvLowestLevel = Generic::GetMaxLevel(NULL);
    //  float fLowestHPRate = 1.0f;
    DWORD dwLowestHP = 0;

    it = m_Records.begin();
    DWORD dwShortestDistanceCharKey = 0;
    DWORD dwLowestLevelCharKey = 0;
    DWORD dwLowestHPCharKey = 0;

    Character* pTarget = NULL;
    DWORD battle_point = 0;
    itend = m_Records.end();
    while(it != itend)
    {
        pTarget = pField->FindCharacter(it->first);
        if(!pTarget)
        {
            it = m_Records.erase(it);
            continue;
        }

        BattleRecord::Node& record = it->second;

        // 이 캐릭과의 거리가 더 가까운지 검사
        float fDist = m_pNPC->GetDist(pTarget);
        if(fDist < fShortestDistance)
        {
            dwShortestDistanceCharKey = it->first;
            fShortestDistance = fDist;
        }

#ifdef _NA_0_20100819_AGGRO_RENEWAL
		record.SetTargetDistance(fDist);
#endif

        // 더 낮은 레벨을 가졌는지 검사
        if(pTarget->GetLevel() < lvLowestLevel)
        {
            dwLowestLevelCharKey = it->first;
            lvLowestLevel = pTarget->GetLevel();
        }

        /*
        // HP 비율이 더 낮은지 검사
        if((float)pTarget->GetHP() / pTarget->GetMaxHP() < fLowestHPRate)
        {
        dwLowestHPCharKey = it->first;
        fLowestHPRate = (float)pTarget->GetHP() / pTarget->GetMaxHP();
        }
        */

        // HP는 상대적인 비율로 계산하지 않고 절대적인 HP양의 차이로 판단!!
        if(0 == dwLowestHP || (dwLowestHP >  pTarget->GetHP()))
        {
            dwLowestHPCharKey = it->first;
            dwLowestHP = pTarget->GetHP();
        }
#ifdef _NA_0_20100819_AGGRO_RENEWAL		
		record.SetBattlePoint(BATTLE_POINT_DAMAGE, 
						record.GetBattlePoint(BATTLE_POINT_DAMAGE) + record.GetPointDamage());
#else
		// 총 누적데미지중 자신이 먹인 누적데미지 비율만큼 포인트를 준다.
		if(dwTotalDamage)
		{
			battle_point = record.GetBattlePoint(BATTLE_POINT_DAMAGE) + (WORD)(m_pAITypeInfo->m_wDAMAGE_POINT_INC * (float)record.GetPointDamage() / dwTotalDamage);
			battle_point = min(battle_point, m_pAITypeInfo->m_wDAMAGE_POINT_MAX);
			record.SetBattlePoint(BATTLE_POINT_DAMAGE, battle_point);
		}
#endif
        // 한번 계산된 데미지는 다시 반영되지 않는다.
        record.SetPointDamage(0);

        ++it;
    }

    if(dwShortestDistanceCharKey)
    {
        BattleRecord::Node* pRecord = Get(dwShortestDistanceCharKey);
        if(pRecord)
        {
            // 최소거리 포인트
            battle_point = pRecord->GetBattlePoint(BATTLE_POINT_DISTANCE) + m_pAITypeInfo->m_wNEAR_DISTANCE_POINT_INC;
            battle_point = min(battle_point, m_pAITypeInfo->m_wNEAR_DISTANCE_POINT_MAX);
            pRecord->SetBattlePoint(BATTLE_POINT_DISTANCE, battle_point);
        }
    }

    if(dwLowestLevelCharKey)
    {
        BattleRecord::Node* pRecord = Get(dwLowestLevelCharKey);
        if(pRecord)
        {
            // 최소거리 포인트
            battle_point = pRecord->GetBattlePoint(BATTLE_POINT_LEVEL) + m_pAITypeInfo->m_wLOW_LEVEL_POINT_INC;
            battle_point = min(battle_point, m_pAITypeInfo->m_wLOW_LEVEL_POINT_MAX);
            pRecord->SetBattlePoint(BATTLE_POINT_LEVEL, battle_point);
        }
    }

    if(dwLowestHPCharKey)
    {
        BattleRecord::Node* pRecord = Get(dwLowestHPCharKey);
        if(pRecord)
        {
            // 최소거리 포인트
            battle_point = pRecord->GetBattlePoint(BATTLE_POINT_HP) + m_pAITypeInfo->m_wLOW_HP_POINT_INC;
            battle_point = min(battle_point, m_pAITypeInfo->m_wLOW_HP_POINT_MAX);
            pRecord->SetBattlePoint(BATTLE_POINT_HP, battle_point);
        }
    }
}

// 적 리스트에서 TotalPoint가 가장 높은 넘을 리턴한다.
Character*
BattleRecord::SelectBestTarget()
{
    // 레코드가 하나도 없으면 리턴
    if(m_Records.empty())
        return NULL;

    GameField* pField = m_pNPC->GetField();
    if(!pField)
        return NULL;

    BattleRecord::Node* pHighestRecord = NULL;
    DWORD dwHighestPoint = 0;

    Records::iterator it = m_Records.begin(),
                      itend = m_Records.end();
    for( ; it != itend; ++it)
    {
        BattleRecord::Node &record = it->second;
#ifdef _NA_0_20100819_AGGRO_RENEWAL		
		if(record.GetCalcTotalPoint() > dwHighestPoint)
		{
			pHighestRecord = &record;
			dwHighestPoint = record.GetCalcTotalPoint();
		}
#else
		if(record.GetBattlePoint(BATTLE_POINT_TOTAL) > dwHighestPoint)
		{
			pHighestRecord = &record;
			dwHighestPoint = record.GetBattlePoint(BATTLE_POINT_TOTAL);
		}
#endif        
    }

    if(!pHighestRecord)
        return NULL;

    //랜덤성 강화를 위한 예외처리 루틴
	//0 -> 1 로 변경
    if(random(1, 100) <= m_pAITypeInfo->m_byPOINT_INIT_RANDOM_RATIO)
    {
        // 거리포인트와 데미지포인트를 잊어먹는다.
        pHighestRecord->SetBattlePoint(BATTLE_POINT_DISTANCE, 0);
        pHighestRecord->SetBattlePoint(BATTLE_POINT_DAMAGE, 0);
        // BattlePoint를 계산할 때 Damage에 따라 계산하므로 BattlePoint를 초기화하였으면 당연히 Damage도 초기화해야 함!!
        pHighestRecord->SetPointDamage(0);
    }

    return pField->FindCharacter(pHighestRecord->GetObjectKey());
}

void
BattleRecord::Clear(DWORD dwExceptKey)
{
    if(dwExceptKey)
    {
        Records::iterator it = m_Records.begin(),
                          itend = m_Records.end();
        while(it != itend)
        {
            BattleRecord::Node& record = it->second;
            if(record.GetObjectKey() == dwExceptKey)
            {
                ++it;
                continue;
            }
            it = m_Records.erase(it);
        }
    }
    else
    {
        m_Records.clear();
    }
}

#ifdef _NA_20100322_AGGRO_DISPLAY
void BattleRecord::SendAggroDisPlayInfo(Player* const player)
{
    GameField* const game_field = m_pNPC->GetField();
    if (game_field == NULL) {
        return;
    };

    static AggroDisplayList aggro_display_list_s;
    AggroDisplayList& aggro_display_list = aggro_display_list_s;
    aggro_display_list.resize(0);

    Records::iterator it = m_Records.begin();
    Records::const_iterator end = m_Records.end();
    for (; it != end;) 
    {
        Character* pTarget = game_field->FindCharacter(it->first);
        if(!pTarget)
        {
            it = m_Records.erase(it);
        }
        else
        {
            aggro_display_list.push_back(it);
            ++it;
        }
    }

    std::sort(aggro_display_list.begin(), aggro_display_list.end(), compare_node_value());

    const BYTE aggro_display_max = aggro_display_list.size() > MSG_CG_ARRGO_DISPLAY_CMD::MAX_AGGRO_DISPLAY
                                    ?   MSG_CG_ARRGO_DISPLAY_CMD::MAX_AGGRO_DISPLAY
                                    :   static_cast<BYTE>(aggro_display_list.size());

    MSG_CG_ARRGO_DISPLAY_CMD msg;
    msg.aggro_player_count_ = 0;

    for (BYTE i = 0 ; i < aggro_display_max; ++i)
    {
        Records::iterator source_it = aggro_display_list[i];
        Character* target_char = game_field->FindCharacter(source_it->first);
        if (target_char)
        {
            AggroValueInfo* const dest_it = &msg.aggor_info_[msg.aggro_player_count_];            
            strncpy(dest_it->character_name_, target_char->GetCharName(), 
                _countof(dest_it->character_name_));
            dest_it->character_name_[_countof(dest_it->character_name_) - 1] = '\0';
            
            dest_it->aggro_value_ = source_it->second.GetCalcTotalPoint();  
            dest_it->class_type_  = static_cast<BYTE>(eCHAR_NONE);
            dest_it->name_code_   = 0;

            if(target_char->IsEqualObjectKind(PLAYER_OBJECT))
            {
                Player* const target_player = static_cast<Player*>(target_char);                    
                dest_it->class_type_  = static_cast<BYTE>(target_player->GetCharType());
                ++msg.aggro_player_count_;
            }
            else if(target_char->IsEqualObjectKind(SUMMON_OBJECT))
            {
                Summoned* const target_summoned = DOWNCASTER_SELECT<Summoned*>(target_char);
                const BASE_NPCINFO* const npc_info = target_summoned->GetBaseInfo();

                dest_it->name_code_ = npc_info->m_NCode;

                Player* const owner_player = target_summoned->GetPlayerOwner();
                if(owner_player)
                {
                    dest_it->class_type_  = static_cast<BYTE>(owner_player->GetCharType());
                }
                ++msg.aggro_player_count_;
            }
            else
            {
                // 여기 올 타입은 없지
            }            
        }                
    }
    player->SendPacket(&msg, msg.GetSize());  
}
#endif



