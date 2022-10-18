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
    // TotalPoint�� �ֱ������� ���� ��Ų��
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

    // ��ü ����Ʈ�� ������ ��쿡�� ���� ����Ʈ���� ��� ������Ѿ� �Ѵ�.
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
    // ��ü ����Ʈ ����
    m_wBattlePoint[BATTLE_POINT_TOTAL] = 0;

#ifdef _NA_0_20100819_AGGRO_RENEWAL //���ο� ��׷� �ý��ۿ��� ĳ���ͺ� ��׷δ� ��ü ��׷��� ������ ���ȴ�.
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

			// �÷��̾� Ŭ������ üũ�� �� ����Ʈ�� �ش�.
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

    //  (WAVERIX)(080320)   - ������ ������. ��� ���ɿ� ���縦...
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
	float distance_rate  = 100.0f; // �ŷ��� �־����� ������� 100 ��׷� ��ġ�� ���´�.
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
//  ���� �׸� ���� ����Ʈ�� �ű��.
//  - (WAVERIX)(080320) - ������ ���
//  -   1) �ʱ� ��� ���鿡 ĳ���Ͱ� ���� ��찡 �����Ǿ� �ִ�.
//  -   2) ������ ������ ����ȴ�.
//  -   3) begin�� �߸��� ����?
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
    // �غ� �۾�
    DWORD dwTotalDamage = 0;
    it = m_Records.begin();

    itend = m_Records.end();
    for(it = m_Records.begin(); it != itend ; ++it)
    {
        BattleRecord::Node& record = it->second;
        if(need_battlerecord_update)
            record.Update();
        // �÷��̾���� �� ���� �������� ���صд�.       
        dwTotalDamage += record.GetPointDamage();
    }
#endif

    //---------------------------------------------------------------------------------------------
    // �� ����Ʈ�� ��ȸ�ϸ鼭 �� �׸� �ش��ϴ� ����Ʈ�� ���� ĳ���͸� ã�´�.
    //---------------------------------------------------------------------------------------------

    // �ʱⰪ ����
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

        // �� ĳ������ �Ÿ��� �� ������� �˻�
        float fDist = m_pNPC->GetDist(pTarget);
        if(fDist < fShortestDistance)
        {
            dwShortestDistanceCharKey = it->first;
            fShortestDistance = fDist;
        }

#ifdef _NA_0_20100819_AGGRO_RENEWAL
		record.SetTargetDistance(fDist);
#endif

        // �� ���� ������ �������� �˻�
        if(pTarget->GetLevel() < lvLowestLevel)
        {
            dwLowestLevelCharKey = it->first;
            lvLowestLevel = pTarget->GetLevel();
        }

        /*
        // HP ������ �� ������ �˻�
        if((float)pTarget->GetHP() / pTarget->GetMaxHP() < fLowestHPRate)
        {
        dwLowestHPCharKey = it->first;
        fLowestHPRate = (float)pTarget->GetHP() / pTarget->GetMaxHP();
        }
        */

        // HP�� ������� ������ ������� �ʰ� �������� HP���� ���̷� �Ǵ�!!
        if(0 == dwLowestHP || (dwLowestHP >  pTarget->GetHP()))
        {
            dwLowestHPCharKey = it->first;
            dwLowestHP = pTarget->GetHP();
        }
#ifdef _NA_0_20100819_AGGRO_RENEWAL		
		record.SetBattlePoint(BATTLE_POINT_DAMAGE, 
						record.GetBattlePoint(BATTLE_POINT_DAMAGE) + record.GetPointDamage());
#else
		// �� ������������ �ڽ��� ���� ���������� ������ŭ ����Ʈ�� �ش�.
		if(dwTotalDamage)
		{
			battle_point = record.GetBattlePoint(BATTLE_POINT_DAMAGE) + (WORD)(m_pAITypeInfo->m_wDAMAGE_POINT_INC * (float)record.GetPointDamage() / dwTotalDamage);
			battle_point = min(battle_point, m_pAITypeInfo->m_wDAMAGE_POINT_MAX);
			record.SetBattlePoint(BATTLE_POINT_DAMAGE, battle_point);
		}
#endif
        // �ѹ� ���� �������� �ٽ� �ݿ����� �ʴ´�.
        record.SetPointDamage(0);

        ++it;
    }

    if(dwShortestDistanceCharKey)
    {
        BattleRecord::Node* pRecord = Get(dwShortestDistanceCharKey);
        if(pRecord)
        {
            // �ּҰŸ� ����Ʈ
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
            // �ּҰŸ� ����Ʈ
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
            // �ּҰŸ� ����Ʈ
            battle_point = pRecord->GetBattlePoint(BATTLE_POINT_HP) + m_pAITypeInfo->m_wLOW_HP_POINT_INC;
            battle_point = min(battle_point, m_pAITypeInfo->m_wLOW_HP_POINT_MAX);
            pRecord->SetBattlePoint(BATTLE_POINT_HP, battle_point);
        }
    }
}

// �� ����Ʈ���� TotalPoint�� ���� ���� ���� �����Ѵ�.
Character*
BattleRecord::SelectBestTarget()
{
    // ���ڵ尡 �ϳ��� ������ ����
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

    //������ ��ȭ�� ���� ����ó�� ��ƾ
	//0 -> 1 �� ����
    if(random(1, 100) <= m_pAITypeInfo->m_byPOINT_INIT_RANDOM_RATIO)
    {
        // �Ÿ�����Ʈ�� ����������Ʈ�� �ؾ�Դ´�.
        pHighestRecord->SetBattlePoint(BATTLE_POINT_DISTANCE, 0);
        pHighestRecord->SetBattlePoint(BATTLE_POINT_DAMAGE, 0);
        // BattlePoint�� ����� �� Damage�� ���� ����ϹǷ� BattlePoint�� �ʱ�ȭ�Ͽ����� �翬�� Damage�� �ʱ�ȭ�ؾ� ��!!
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
                // ���� �� Ÿ���� ����
            }            
        }                
    }
    player->SendPacket(&msg, msg.GetSize());  
}
#endif



