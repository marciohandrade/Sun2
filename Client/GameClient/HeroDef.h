#pragma once

#include "PlayerDef.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct _HERO_INFO
{
	_HERO_INFO() : m_pItemStream(NULL), m_wStreamLength(0)
	{
	}

	~_HERO_INFO() 
	{
	}

	DWORD					m_dwKey;
	PLAYER_MAKE_TYPE		m_MakeType;
	BASE_PLAYERINFO			m_CharInfo;
	EQUIP_ITEM_INFO			m_EquiptItemInfo;
	INVENTORY_TOTAL_INFO	m_InventoryTotalInfo;
	SKILL_TOTAL_INFO		m_SkillTotalInfo;
	QUICK_TOTAL_INFO		m_QuickTotalInfo;
	BYTE					m_byGMGrade;
	BYTE                    m_bHideHelmet;
#ifdef _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
    BYTE                    m_bHideWing;
#endif//_NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
#ifdef _NA_006683_20130502_EQUIPMENTS_VISIBLE_COSTUME
    BYTE                    m_bHideCostume;
#endif
	BYTE					m_byPCBang;
	BYTE					m_byBlockPCBang;
	BOOL                    m_bObserverMode;
	WORD					m_wStreamLength;
	BYTE*					m_pItemStream;  //포인터 공유는 되지 않으나 구조체 대입을  할경우 위험하다!
	GUILD_RENDER_INFO		m_GuildRenderInfo;
	ETHER_INFO				m_EthreInfo;
	INT						m_iMaxSD;
	INT						m_iSD;

#ifdef _PCBANG_POLICY_CHANGE_POST_EXPIRED
	BYTE					m_bPCBangStatus;
#endif //_PCBANG_POLICY_CHANGE_POST_EXPIRED

    INT                     m_FightEnergy;  // 분노카운트
    DWORD                   m_OldAttackStyle; //이전 스타일 값(드나 용변신 후 맵 이동 시 되돌려줄 값)
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE
	CLIENT_PLAYER_RANKING_GRADE_INFO client_player_ranking_grade_info_;
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE

#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    SLOTCODE m_spa_item_code;
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM

} HERO_INFO;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	탈것정보 구조체(hero)
typedef struct _RIDER_INFO
{
	POSTYPE	m_RiderItemPos;
	POSTYPE	m_RiderChangePos;		
	BOOL	m_bChangeRider;	
	BOOL	m_bSendSummon;
    BOOL    m_bCancelSummon;
}RIDER_INFO;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum eTRIGGER_STATE
{
	TRIGGER_STATE_NONE				= 0,
	TRIGGER_STATE_ENTER_AREA		= 1,
	TRIGGER_STATE_LEAVE_AREA		= 2,
	TRIGGER_STATE_ENTER_POTAL_AREA	= 3,
	TRIGGER_STATE_LEAVE_POTAL_AREA	= 4,
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum eSHORTCUT_AREA_STATE
{
	SHORTCUT_AREA_STATE_NONE		= 0,
	SHORTCUT_AREA_STATE_ENTER_AREA	= 1,
	SHORTCUT_AREA_STATE_LEAVE_AREA	= 2,
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum PACKET_STATUS
{ 
	PACKET_STATUS_NONE					= 0, 
	PACKET_STATUS_PICK_ITEM				= 1,        //캐릭터 생성 패킷
	PACKET_STATUS_PICK_MONEY			= (2 << 1), //캐릭터 삭제 패킷 
	PACKET_STATUS_CAST_CANCEL           = (2 << 2),
	PACKET_STATUS__END_FORMATION        = (2 << 3), 
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum eHERO_TURN_STATE
{
	eHERO_NOT_TURN		= 0,
	eHERO_LEFT_TURN		= 1,
	eHERO_RIGHT_TURN	= 2,
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct GET_ITEM
{
	DWORD	dwObjectKey;
	float	fDistance;
};
typedef		std::vector<GET_ITEM>		GET_ITEM_vector;
typedef		GET_ITEM_vector::iterator	GET_ITEM_vector_Itr;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define HERO_OBJECT_KEY			0xFFFFFFFF

#define CRYSTAL_WARP_OBJ_MAX_CNT	2

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct TAB_TARGET_MONSTER
{
	DWORD	dwObjectKey;
	float	fAngle;
	float	fDistance;
	WORD	wTargetCount;
	WORD	wTargetCheck;
};

inline bool operator == (const TAB_TARGET_MONSTER& x, const TAB_TARGET_MONSTER& y)
{
	return ( x.fDistance == y.fDistance );
}

inline bool operator < (const TAB_TARGET_MONSTER& x, const TAB_TARGET_MONSTER& y)
{
	return ( x.fDistance < y.fDistance );
}

inline bool operator > (const TAB_TARGET_MONSTER& x, const TAB_TARGET_MONSTER& y)
{
	return ( x.fDistance > y.fDistance );
}

typedef		std::vector<TAB_TARGET_MONSTER>			TAB_TARGET_MONSTER_vector;
typedef		TAB_TARGET_MONSTER_vector::iterator		TAB_TARGET_MONSTER_vector_Itr;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

