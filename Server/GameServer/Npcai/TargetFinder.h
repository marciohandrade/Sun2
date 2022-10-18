#pragma once

#include <PublicMath.h>
#include "Character.h"
#include "AIParamParser.h"
#include "MovingPlanner.h"

// 화일이름 바꿔야 한다. -_-;

//==================================================================================================
//
//										TargetFinder
//
//==================================================================================================
// @history
//  - ~~~~~
//  - 09/12/13|waverix|add skip tile filter for _NA001415_091123_CHANGE_MOB_AI_IN_SAFETY_AREA_

//─────────────────────────────────────────
//	<TargetFinder>
//	-	특정 상태의 캐릭터를 찾는다.
//_NA_0_20100222_UNIT_TRIGGERS_FRIEND_MONSTER
struct TargetFinderArg
{
    Character* owner_;
    eTARGET_SEARCH_TYPE search_type_;
    eSKILL_TARGET_TYPE target_type_;
    REGENID unitID_;
    DWORD mapobjectID_;
    MONSTERCODE npc_code_; 
};

class TargetFinder
{
public:		
    TargetFinder()
	: m_pOwner( 0 )
	, m_pTarget( 0 )
	, m_eTargetType( SKILL_TARGET_NONE )
    , m_SkipTileAttr(0)
	, m_fOwnerSight( .0f )
	, m_fOwnerSightX2( .0f )
    , m_fBetweenOwnerWithTargetX2( .0f ) {}

	virtual	~TargetFinder() { }
public:
//_NA_0_20100222_UNIT_TRIGGERS_FRIEND_MONSTER
    TargetFinder(const TargetFinderArg& arg) 
        : m_pOwner( 0 )
        , m_pTarget( 0 )
        , m_eTargetType( SKILL_TARGET_NONE )
        , m_SkipTileAttr(0)
        , m_fOwnerSight( .0f )
        , m_fOwnerSightX2( .0f )
        , m_fBetweenOwnerWithTargetX2( .0f ) { SetFinder(arg); }

    virtual void SetFinder(const TargetFinderArg& finder_arg )
    {
        ASSERT(finder_arg.owner_);

        m_pOwner		= finder_arg.owner_;
        m_fOwnerSight	= m_pOwner->GetSightRange();
        m_fOwnerSightX2	= m_fOwnerSight*m_fOwnerSight;
        m_vOwnerPos		= *m_pOwner->GetPosPtr();
        m_eTargetType   = finder_arg.target_type_;

        switch(m_eTargetType)
        {
        case SKILL_TARGET_ENEMY:
        case SKILL_TARGET_REACHABLE_ENEMY:
        case SKILL_TARGET_ENEMY_PLAYER:
        case SKILL_TARGET_ENEMY_CORPSE:
        case SKILL_TARGET_AREA_ENEMY_CORPSE:
            m_SkipTileAttr = PTA_SAFETY_ZONE;
            break;
        default:
            break;
        }
    }
	//	Result
	inline	Character*		GetTarget()	{ return m_pTarget; }

public:
	//	<INTERFACE>
	virtual	bool			operator () ( Character* const pCharacter ) = 0;

protected:	bool			CheckTarget( Character* const pTarget )
			{
				if( !( pTarget && (m_pOwner != m_pTarget) ) )
					return false;

				m_vTargetPos = *pTarget->GetPosPtr();
                WzVector diff = m_vTargetPos;
                ;        diff -= m_vOwnerPos;
                m_fBetweenOwnerWithTargetX2 = Math_Vector2DLength2(&diff);

                WORD skip_check_tile_attr_for_target = 0;
                if (pTarget->IsEqualObjectKind(MAPNPC_OBJECT)) 
                {
                    skip_check_tile_attr_for_target = PTA_NO_WALK;
                }

				if( !nsAI::PlanEventObject::AcceptableTarget(pTarget, m_SkipTileAttr, 
                                                             skip_check_tile_attr_for_target))
                {
					return false;
                }

				if( !m_pOwner->IsCoincidentWithTargetType( pTarget, m_eTargetType ) )
					return false;

				return true;
			}

			bool			IsCheckTargetCasebyAlive( Character* const pTarget )
			{
				if( !CheckTarget( pTarget ) )
					return false;

				if( pTarget->IsDead()
					|| (!pTarget->CanBeAttacked())
					)
					return false;

				return true;
			}

protected:
	Character*			m_pOwner;
	Character*			m_pTarget;

	eSKILL_TARGET_TYPE	m_eTargetType;					// 찾아야할 타겟 타입
    WORD                m_SkipTileAttr;//_NA001415_091123_CHANGE_MOB_AI_IN_SAFETY_AREA_
#pragma warning(push)
#pragma warning(disable : 4201)
	struct {	// SetOwner 호출 이후 계산되는 값
		FLOAT			m_fOwnerSight;					//
		FLOAT			m_fOwnerSightX2;				// (OwnerSight*OwnerSight)
	};
	struct {	// CheckTarget 호출 이후 계산되는 값
		FLOAT			m_fBetweenOwnerWithTargetX2;	// 둘 사이의 거리
		WzVector		m_vOwnerPos;
		WzVector		m_vTargetPos;
	};
#pragma warning(pop)
};

//─────────────────────────────────────────
//	<NearestTargetFinder>
//
class NearestTargetFinder : public TargetFinder
{
public:		NearestTargetFinder()
				: m_TargetDistX2(999999.f)
			{
			}

            //_NA_0_20100222_UNIT_TRIGGERS_FRIEND_MONSTER
            NearestTargetFinder(const TargetFinderArg& arg)
                : TargetFinder(arg),
                m_TargetDistX2(999999.f) {}

public:
	virtual	bool			operator () ( Character* const pCharacter )
	{
		if( !IsCheckTargetCasebyAlive( pCharacter ) )
			return false;

		if( m_fBetweenOwnerWithTargetX2 >= m_TargetDistX2 )
			return false;

		m_pTarget = pCharacter;
		m_TargetDistX2 = m_fBetweenOwnerWithTargetX2;

		return false;	// continue 유지 목적?
	}

protected:
	FLOAT		m_TargetDistX2;
};


//─────────────────────────────────────────
//	<LowHPRatioTargetFinder>
//
class LowHPRatioTargetFinder : public TargetFinder
{
public:		LowHPRatioTargetFinder() : m_LowHPRatio(ULONG_MAX)
			{}

            //_NA_0_20100222_UNIT_TRIGGERS_FRIEND_MONSTER
            LowHPRatioTargetFinder(const TargetFinderArg& arg) 
                : TargetFinder(arg),
                m_LowHPRatio(ULONG_MAX) {}

public:
	virtual	bool			operator () ( Character* const pCharacter )
	{
		if( !IsCheckTargetCasebyAlive( pCharacter ) )
			return false;

		DWORD targetHP = pCharacter->GetMaxHP();
		targetHP = targetHP ? targetHP : 1;
		const DWORD targetHPRatio = (DWORD)( pCharacter->GetHP() * 100 / targetHP );

		if( targetHPRatio >= m_LowHPRatio )
			return false;

		m_pTarget = pCharacter;
		m_LowHPRatio = targetHPRatio;

		// not found --> continue
		return false;	// continue 유지 목적?
	}

private :
	DWORD		m_LowHPRatio;
};

//─────────────────────────────────────────
//	<LowHPTargetFinder>
//
class LowHPTargetFinder : public TargetFinder
{
public:		LowHPTargetFinder() : m_LowHP(ULONG_MAX) {}

            //_NA_0_20100222_UNIT_TRIGGERS_FRIEND_MONSTER
            LowHPTargetFinder(const TargetFinderArg& arg)
                : TargetFinder(arg),
                m_LowHP(ULONG_MAX) {}

public:
	virtual	bool			operator () ( Character* const pCharacter )
	{
		if( !IsCheckTargetCasebyAlive( pCharacter ) )
			return false;

		const DWORD targetHP = pCharacter->GetHP();
		if( targetHP >= m_LowHP )
			return false;

		m_pTarget = pCharacter;
		m_LowHP = targetHP;

		// not found --> continue
		return false;	// continue 유지 목적?
	}

private :
	DWORD		m_LowHP;
};

//─────────────────────────────────────────
//	<LowLevelTargetFinder>
//
class LowLevelTargetFinder : public TargetFinder
{
public:		LowLevelTargetFinder() : m_LowLevel(USHRT_MAX) {}

            //_NA_0_20100222_UNIT_TRIGGERS_FRIEND_MONSTER
            LowLevelTargetFinder(const TargetFinderArg& arg)
                : TargetFinder(arg),
                m_LowLevel(USHRT_MAX) {}

public :
	virtual	bool			operator () ( Character* const pCharacter )
	{
		if( !IsCheckTargetCasebyAlive( pCharacter ) )
			return false;

		const LEVELTYPE targetLevel = pCharacter->GetLevel();
		if( targetLevel >= m_LowLevel )
			return false;

		m_pTarget = pCharacter;
		m_LowLevel = targetLevel;

		// not found --> continue
		return false;	// continue 유지 목적?
	}

private :
	LEVELTYPE		m_LowLevel;
};


//─────────────────────────────────────────
//	<HighMPTargetFinder>
//
class HighMPTargetFinder : public TargetFinder
{
public:		HighMPTargetFinder() : m_HighMP(0) {}

            //_NA_0_20100222_UNIT_TRIGGERS_FRIEND_MONSTER
            HighMPTargetFinder(const TargetFinderArg& arg)
                : TargetFinder(arg),
                m_HighMP(0) {}

public:
	virtual	bool			operator () ( Character* const pCharacter )
	{
		if( !IsCheckTargetCasebyAlive( pCharacter ) )
			return false;

		const DWORD targetMP = pCharacter->GetMP();
		if( targetMP <= m_HighMP )
			return false;

		m_pTarget = pCharacter;
		m_HighMP = targetMP;

		// not found --> continue
		return false;	// continue 유지 목적?
	}

private :
	DWORD		m_HighMP;
};

//─────────────────────────────────────────
//	<HighMPTargetFinder>
//
class DeadTargetFinder : public TargetFinder
{
public:		DeadTargetFinder() {}

            //_NA_0_20100222_UNIT_TRIGGERS_FRIEND_MONSTER
            DeadTargetFinder(const TargetFinderArg& arg) : TargetFinder(arg) {}

public:
	virtual	bool			operator () ( Character* const pCharacter )
	{
		if( !CheckTarget( pCharacter ) )
			return false;

		if( pCharacter->IsAlive() )
			return false;

		m_pTarget = pCharacter;

		return true;	// 가장 죽은...이라는 의미는 없으므로 바로 true
	}
};

//─────────────────────────────────────────
// 해당 regenID(몬스터)

//_NA_0_20100222_UNIT_TRIGGERS_FRIEND_MONSTER

class UnitIDTargetFinder : public TargetFinder
{
public:		
    UnitIDTargetFinder() {}
    UnitIDTargetFinder(const TargetFinderArg& arg) 
        : TargetFinder(arg), 
        unitID_(arg.unitID_) {}

    virtual	bool operator () (Character* const pCharacter)
    {
        if (!IsCheckTargetCasebyAlive(pCharacter))
            return false;

        if (pCharacter->IsEqualObjectKind(NPC_OBJECT) == FALSE)
            return false;

        NPC* const npc = static_cast<NPC*>(pCharacter);
        const REGENID unitID = npc->GetRegenID();

        if (unitID == unitID_)
        {
            m_pTarget = pCharacter;
            return true;
        }
        
        return false;
    }

protected:
    REGENID	unitID_;
};

class MapNpcObjectTargetFinder : public TargetFinder
{
public:		
    MapNpcObjectTargetFinder() {}
    MapNpcObjectTargetFinder(const TargetFinderArg& arg) 
        : TargetFinder(arg),
        mapobjectID_(arg.mapobjectID_) {}

    virtual	bool operator () (Character* const pCharacter)
    {
        if (!IsCheckTargetCasebyAlive(pCharacter))
            return false;

        if (pCharacter->IsEqualObjectKind(MAPNPC_OBJECT) == FALSE)
            return false;

        MapNpc* const npc = static_cast<MapNpc*>(pCharacter);
        const DWORD mapobjectID = npc->GetMapObjectID();
        if (mapobjectID == mapobjectID_)
        {
            m_pTarget = pCharacter;
            return true;
        }

        return false;
    }

protected:
    DWORD mapobjectID_;
};

class NearestMonsterCodeTargetFinder : public TargetFinder
{
public:		
    NearestMonsterCodeTargetFinder() : target_distX2_(999999.f) {}
    NearestMonsterCodeTargetFinder(const TargetFinderArg& arg) 
        : TargetFinder(arg),
        target_distX2_(999999.f),
        monster_code_(arg.npc_code_) {}

    virtual	bool operator () (Character* const pCharacter)
    {
        if (!IsCheckTargetCasebyAlive(pCharacter))
            return false;

        if (pCharacter->IsEqualObjectKind(NPC_OBJECT) == FALSE)
            return false;

        const NPC* const npc = static_cast<NPC*>(pCharacter);
        const BASE_NPCINFO* const base_info = npc->GetBaseInfo();
        const MONSTERCODE monster_code = base_info->m_MonsterCode;

        if (monster_code == monster_code_)
        {
            if( m_fBetweenOwnerWithTargetX2 >= target_distX2_ )
                return false;

            target_distX2_ = m_fBetweenOwnerWithTargetX2;
            m_pTarget = pCharacter;
        }

        return false;
    }

protected:
    FLOAT target_distX2_;
    MONSTERCODE monster_code_;
};





