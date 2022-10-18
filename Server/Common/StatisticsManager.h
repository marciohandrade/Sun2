#pragma once

//=======================================================================================================================
/// ItemManager
/**
	@author
			Kim Min Wook < taiyo@webzen.co.kr >	
	@since
			2006. 6. 12~
	@description
			- 생긴돈
			- 사라진돈
			- 필미스톤 생성/소멸
			- 오버미스톤 생성/소멸
			- 메가미스톤 생성/소멸
			- 기가미스톤 생성/소멸
			- 인첸횟수
			- 랭크횟수
			- 창고 돈
			- 잭팟 터진 횟수/금액 합계

			- 생성 : 줍기, 보상(이벤트,미션,퀘스트,경쟁헌팅) 획득, 개인상점획득, NPC상점획득, 개인거래획득, 조합해서 생성
			- 소멸 : 버리기, 개인상점팔기, NPC상점팔기, 개인거래팔기, 인첸트, 조합하여 소멸
	@history
			
			
*/
//=======================================================================================================================
//enum eSTATISTICS_CATEGORY
//{
//	eSTATISTICS_MONEY,
//	eSTATISTICS_ITEM,
//	eSTATISTICS_FUNCTION,
//};

enum eSTATISTICS_TYPE
{
	// 현재 유통되는 통화량(상거래)
	eSTATISTIC_MONEY_CIRCULATION,
	// 생긴돈 : 돈줍기,보상받기,
	eSTATISTIC_MONEY_PLUS,
	// 사라진돈 : 돈버림, 하임소비
	eSTATISTIC_MONEY_MINUS,

	eSTATISTIC_MONEY_WAREHOUSE_PLUS,
	eSTATISTIC_MONEY_WAREHOUSE_MINUS,

	eSTATISTIC_JACKPOT_COUNT,
	eSTATISTIC_JACKPOT_MONEY,

	eSTATISTIC_FEELMISTONE_CREATE,
	eSTATISTIC_FEELMISTONE_DESTROY,
	eSTATISTIC_OVERMISTONE_CREATE,
	eSTATISTIC_OVERMISTONE_DESTROY,
	eSTATISTIC_STRENGTHMISTONE_CREATE,
	eSTATISTIC_STRENGTHMISTONE_DESTROY,
	eSTATISTIC_MEGAMISTONE_CREATE,
	eSTATISTIC_MEGAMISTONE_DESTROY,

	eSTATISTIC_ENCHANT_COUNT,
	eSTATISTIC_RANKUP_COUNT,

	//eSTATISTIC_COMPOSE_COUNT,
	// 결정화
	eSTATISTIC_CRYSTALIZE_COUNT,

	// 조합관련
	eSTATISTIC_COMPOSE_RING_LOW,
	eSTATISTIC_COMPOSE_RING_HIGH,
	eSTATISTIC_COMPOSE_NECKLACE_LOW,
	eSTATISTIC_COMPOSE_NECKLACE_HIGH,
	eSTATISTIC_COMPOSE_SOUL_SHEILD_LOW,
	eSTATISTIC_COMPOSE_SOUL_SHEILD_HIGH,
	eSTATISTIC_COMPOSE_SOUL_ARMLET_LOW,
	eSTATISTIC_COMPOSE_SOUL_ARMLET_HIGH,
	eSTATISTIC_COMPOSE_SOUL_WING_LOW,
	eSTATISTIC_COMPOSE_SOUL_WING_HIGH,
	eSTATISTIC_COMPOSE_SOUL_EYE_LOW,
	eSTATISTIC_COMPOSE_SOUL_EYE_HIGH,

	eSTATISTIC_COMPOSE_EXTRASTONE,
	eSTATISTIC_COMPOSE_FEELMISTONE,
	eSTATISTIC_COMPOSE_OVERMISTONE,
	eSTATISTIC_COMPOSE_STRENGTHMISTONE,
	eSTATISTIC_COMPOSE_MEGAMISTONE,
	eSTATISTIC_COMPOSE_GIGAMISTONE,

	eSTATISTIC_COMPOSE_PROTECTION_CRYSTAL,
	eSTATISTIC_COMPOSE_SHINING_CRYSTAL,
	eSTATISTIC_COMPOSE_UNIQUE_CRYSTAL,
	eSTATISTIC_COMPOSE_GARNET_CRYSTAL,
	eSTATISTIC_COMPOSE_NOBLESS_CRYSTAL,

	eSTATISTIC_MAX,
};

class StatisticsManager
{
public:
	StatisticsManager(void);
	~StatisticsManager(void);

	VOID Init()
	{
		memset( m_StatisticType, 0, sizeof(m_StatisticType) );
	}

	inline VOID ClearStatisticType( eSTATISTICS_TYPE type ) 
	{ 
		if( type < eSTATISTIC_MAX ) 
			m_StatisticType[type] = 0; 
	}
	inline VOID PlusStatisticType( eSTATISTICS_TYPE type, ULONGLONG value ) 
	{ 
		if( type < eSTATISTIC_MAX &&
			m_StatisticType[type] < ULLONG_MAX - value ) 
			m_StatisticType[type] += value; 
	}
	inline VOID MinusStatisticType( eSTATISTICS_TYPE type, ULONGLONG value ) 
	{ 
		if( type < eSTATISTIC_MAX && 
			m_StatisticType[type] >= value ) 
			m_StatisticType[type] -= value; 
	}
	inline ULONGLONG GetStatisticType( eSTATISTICS_TYPE type ) { if(type<eSTATISTIC_MAX) return m_StatisticType[type]; else return 0;	}

private:
	ULONGLONG	m_StatisticType[eSTATISTIC_MAX];
};

extern StatisticsManager g_StatisticsManager;