#include "StdAfx.h"
#include ".\UnitRegenInfo.h"
#include "FormulaRatioParser.h"
#include "NPCInfoParser.h"

UnitRegenInfo::UnitRegenInfo(void)
//,	m_LeaderRegenID(0)
//:	m_NPCCode(0)
:	m_StartPos(NULL)
,	m_DestPos(NULL)
,	m_NumOfPos(0)
,	m_GroupID(0)
,	regen_type_(0)      //_NA_0_20100222_UNIT_TRIGGERS_REGEN
,   walk_state_(NULL)   //_NA_0_20100222_UNIT_TRIGGERS_PATHLIST
{
}

UnitRegenInfo::~UnitRegenInfo(void)
{
	Release();
}

void UnitRegenInfo::Release()
{
	SAFE_DELETE_ARRAY( m_StartPos );
	SAFE_DELETE_ARRAY( m_DestPos );
    SAFE_DELETE_ARRAY(walk_state_);         //_NA_0_20100222_UNIT_TRIGGERS_PATHLIST

	m_NumOfPos = 0;
	m_GroupID = 0;
    regen_type_ = 0;            //_NA_0_20100222_UNIT_TRIGGERS_REGEN

    regen_monster_selector_.Release();
}

void UnitRegenInfo::Init( BYTE NumOfNPC, WORD NumOfPos )
{
    ASSERT( NumOfNPC <= NUM_REGENUNITTYPE );
	ASSERT( !m_StartPos && !m_DestPos );
	
	m_NumOfPos		= NumOfPos;
	m_StartPos		= new WzVector[NumOfPos];
    m_DestPos		= new WzVector[NumOfPos];
    walk_state_		= new eCHAR_MOVE_STATE[NumOfPos];   //_NA_0_20100222_UNIT_TRIGGERS_PATHLIST

    regen_type_ = 0;    //_NA_0_20100222_UNIT_TRIGGERS_REGEN
	m_GroupID = 0;
}

void UnitRegenInfo::SetRegenTime( DWORD RegenTime )
{
	m_RegenTime = RegenTime;

	BASE_FORMULA_RATIOINFO *pFormulaRatio = FormulaRatioParser::Instance()->GetFormulaRatio( RATIO_TYPE_REGEN_TIME );
	if( !pFormulaRatio )	return;

	if( pFormulaRatio->m_fRatio <= 0 || pFormulaRatio->m_fRatio == 1 )
		return;

    const int count = regen_monster_selector_.GetCount();
    if (count <= 0)
	{
		SUNLOG( eFULL_LOG, "[UnitRegenInfo::SetRegenTime] Call SetRegenTime() After call Init() first of all!" );
		return;
	}

	// 럭키몬스터나 보스급 몬스터는 리젠속도 조절에서 제외한다.
	BOOL bRatioApply = TRUE;
    for (int i = 0; i < count; ++i)
    {
		BASE_NPCINFO *pBaseNPCInfo = NPCInfoParser::Instance()->GetNPCInfo( m_pNPCCodes[i] );
		if( !pBaseNPCInfo )	continue;

		if( pBaseNPCInfo->m_byGrade == eNPC_MIDDLEBOSS || pBaseNPCInfo->m_byGrade == eNPC_BOSS || pBaseNPCInfo->m_byGrade == eNPC_LUCKY_MONSTER )	
		{
			bRatioApply = FALSE;
			break;
		}
	}

	if( bRatioApply )
	{
		m_RegenTime = max( 1000, (DWORD)( m_RegenTime * pFormulaRatio->m_fRatio ) );
	}
}

// 가진 npc 코드중 랜덤하게 하나를 리턴한다.
int UnitRegenInfo::GetRandomNPCCode()
{
    // changed by _NA_0_20100706_RANDOM_REGEN
    const DWORD total_ratio = regen_monster_selector_.GetTotalRatio();
    DWORD ratio = random(0, total_ratio-1);

    MONSTERCODE code = 0;
    BOOL is_success = regen_monster_selector_.Select(ratio, code);
    ASSERT(is_success);

    return code;
}

void UnitRegenInfo::SetStartPos(WORD idx, WzVector * pos)
{
	ASSERT( idx < m_NumOfPos );
	m_StartPos[idx] = *pos;
}
void UnitRegenInfo::SetDestPos(WORD idx, WzVector * pos)
{
	ASSERT( idx < m_NumOfPos );
	m_DestPos[idx] = *pos;
}

DWORD UnitRegenInfo::GetNpcCodeArray(MONSTERCODE* OUT npc_code_array, int size)
{
    const int map_size = regen_monster_selector_.GetCount();
    ASSERT(map_size <= size);

    RatioSelector<MONSTERCODE>::ACTIONS& action_data = regen_monster_selector_.GetActionData();
    RatioSelector<MONSTERCODE>::ACTIONS::iterator itr = action_data.begin();
    for (int i = 0;itr != action_data.end(); ++itr, ++i)
    {
        npc_code_array[i] = itr->first;
    }

    return map_size;
}

WORD UnitRegenInfo::GetRandomPosIndex()
{
    if( m_NumOfPos == 1 || 
        GetUnitType() == eMOVE_ATTITUDE_PATHLIST)
    {
        return 0;
    }

    return static_cast<WORD>(dRandom(0, m_NumOfPos));    
}

void UnitRegenInfo::SetWalkState(WORD idx, eCHAR_MOVE_STATE walk_state)
{
    ASSERT( idx < m_NumOfPos );
    walk_state_[idx] = walk_state;
}

///////////////////////////////////////////////////////////
//  _NA_0_20100706_RANDOM_REGEN 삭제
BOOL UnitRegenInfo::IsRandomRegen()
{
    if( m_RegenType == 0 )	
    {
        // 몬스터 일반 리젠
        m_SelectedNPC = 0;
        return TRUE;
    }
    else
    {
        ASSERT( 0 != m_NumOfNPC );
        // 랜덤 몬스터 리젠
        BYTE i = 0;
        int seed = random(0, 99);
        for( i = 0 ; i < m_NumOfNPC ; ++i )
        {
            if( seed < m_pSelectRates[i])
            {
                m_SelectedNPC = i;
                return TRUE;
            }
        }

    }

    return FALSE;
}

//  _NA_0_20100706_RANDOM_REGEN 삭제
// 데이터를 맴버에 셋팅한후 처음에 한번 호출하여 계산해 두어야 한다.
void UnitRegenInfo::CalcRatio()
{
    // 확률의 급수
    for( int i = 1 ; i < m_NumOfNPC ; ++i )
    {
        m_pSelectRates[i] += m_pSelectRates[i-1];
    }
}

//  _NA_0_20100706_RANDOM_REGEN 삭제
void UnitRegenInfo::SetNPCCodes(BYTE idx, int value)
{
    ASSERT( idx < m_NumOfNPC );
    m_pNPCCodes[idx] = value;
}

//  _NA_0_20100706_RANDOM_REGEN 삭제
void UnitRegenInfo::SetSelectRates(BYTE idx, int value)
{
    ASSERT( idx < m_NumOfNPC );
    m_pSelectRates[idx] = value;
}
///////////////////////////////////////////////////////////
