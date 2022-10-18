//------------------------------------------------------------------------------
//  SkillStateFactory.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"

#include "Character.h"
#include "Statebase.h"
#include "SkillState.h"
#include "SkillStateFactory.h"
#include "SkillInfoParser.h"
#include "SkillStateBerserkMode.h"
#include "skillstatedoubleattack.h"
#include "skillstatetackle.h"
#include "SkillStateWeaponThrowing.h"
#include "SkillStateAssault.h"
#include "SkillStateDragonBead.h"
#include "SkillStateDragonEye.h"
#include "SkillStateIllusionDance.h"
#include "SkillStateManaDrain.h"
#include "SkillStateMachSlash.h"
#include "SkillStateSonicBlade.h"
#include "SkillStateBladeFury.h"
#include "SkillStateViperBlade.h"
#include "SkillStateAirBlow.h"
#include "SkillStateDoubleSpinKick.h"
#include "SkillStateFastShot.h"
#include "SkillStateMoonAssault.h"
#include "SkillStateMultiTarget.h"
#include "SkillStateBloodRain.h"
#include "SkillStateHeavyShot.h"
#include "SkillStateShockwave.h"
#include "SkillStateSniping.h"
#include "SkillStateBlazeArrow.h"
#include "SkillStateSpinningWave.h"
#include "SkillStateOverLimit.h"
#include "SkillStateDefenceMotion.h"
#include "SkillStateWildShot.h"
#include "SkillStateFireBall.h"
#include "SkillStateFireSpear.h"
#include "SkillStateIceDagger.h"
#include "SkillStateIceBlast.h"
#include "SkillStateIceBolt.h"
#include "SkillStateIcePressingWall.h"
#include "SkillStateFrozenArrow.h"
#include "SkillStateFrozenShield.h"
#include "SkillStateWindBolt.h"
#include "SkillStateLighting.h"
#include "SkillStateElectricField.h"
#include "SkillStateWindShield.h"
#include "SkillStateLightingWave.h"
#include "SkillStatePoisonThorn.h"
#include "SkillStatePoisonRain.h"
#include "SkillStatePowerWave.h"
#include "SkillStateCoveredHeal.h"
#include "SkillStateMagicShield.h"
#include "SkillStateTeleport.h"
#include "SkillStateWisdom.h"
#include "SkillStateFireArrow.h"
#include "SkillStateRevival.h"
#include "SkillStateReverse.h"
#include "SkillStateFireFila.h"
#include "SkillStateSonyVine.h"
#include "SkillStateExplozenAttack.h"
#include "SkillStateStarFall.h"
#include "SkillStateChaseShot.h"
#include "SkillStateCircleSprits.h"
#include "SkillStateInAbility.h"
#include "SkillStateSaintAid.h"
#include "SkillStateChargeAttack.h"
#include "SkillStateSpritSlug.h"
#include "SkillStatePurge.h"
#include "SkillStateMonsterSkill.h"
#include "SkillStatePoisonDagger.h"
#include "SkillStateDarkSpark.h"
#include "SkillStateDarkForce.h"
#include "SkillStateDarkStun.h"
#include "SkillDarkBreak.h"
#include "SkillStateHpSuction.h"
//#include "SkillStateMpSuction.h"
#include "SkillStateHide.h"
#include "SkillStateRush.h"
#include "SkillStateDashStun.h"
#include "SkillStateCustom.h"
#include "SkillStateMonsterTackle.h"

//전캐릭터 공용
#include "SkillStateCreateCrystalWarp.h"
#include "SkillStateDestroyCrystalWarp.h"

#include "InterfaceManager.h"

// 미스틱
#include "SkillStateSnakeKick.h"
#include "SkillStateRisingSerpent.h"
#include "SkillStateMystic.h"
#include "SkillStateMysticDance.h"
#include "SkillStateRollingPalm.h"

// 헬로이드
#include "SkillStateDrillAttack.h"
#include "SkillStateFlyingCharging.h"
#include "SkillStateHelroidSpin.h"
#include "SkillStateSlidingAttack.h"
#include "SkillStateGuardianShield.h"

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
// 위치블레이드
#include "SkillStateSpinTurnThrust.h"
#include "SkillStateSpiningHearts.h"
#include "SkillStateDarkPolingGrave.h"
#include "SkillStateBlairPang.h"
#include "SkillStateRutainPoint.h"
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

SkillStateFactory g_SkillStateFactory;

TCHAR *g_szSkillName[] = 
{
	_T( "Covered Heal" ),
};


//------------------------------------------------------------------------------
/**
*/
SkillStateFactory::SkillStateFactory()
{
	
}

//------------------------------------------------------------------------------
/**
*/
SkillStateFactory::~SkillStateFactory()
{
#ifdef _K2P_SKILL_REFACTORYING
	DeleteState();
#endif
}

BOOL SkillStateFactory::InitState()
{


	return TRUE;
}

void SkillStateFactory::DeleteState()
{
	STATE_BASE_HASH_MAP_ITR_TYPE Itr = m_StateBaseMap.begin();
	STATE_BASE_HASH_MAP_ITR_TYPE end = m_StateBaseMap.end();

	for( ; Itr != end; ++Itr )
	{
		delete Itr->second;
	}

	m_StateBaseMap.clear();
}

StateBase *SkillStateFactory::CreateState(int iSkillID,DWORD dwObjectType)
{
	BASE_SKILLINFO *pInfo = SkillInfoParser::Instance()->GetSkillInfo( iSkillID );
	assert(pInfo && "Invalid Skill Code");
	StateSkill *pSkillState = NULL;

#ifdef _K2P_SKILL_REFACTORYING
	if( dwObjectType == MONSTER_OBJECT )
	{
		pInfo->m_SkillClassCode = eSKILL_MONSTER;
	}

	STATE_BASE_HASH_MAP_ITR_TYPE Itr = m_StateBaseMap.find( pInfo->m_SkillClassCode );

	if( Itr != m_StateBaseMap.end() )
	{
		StateBase * pSkillState = Itr->second;
		return pSkillState;
	}
#else
	if(dwObjectType== MONSTER_OBJECT)
	{
		pSkillState = new SkillStateMonsterSkill();
		return pSkillState;
	}
#endif


	switch (pInfo->m_SkillClassCode)
	{
#ifdef _K2P_SKILL_REFACTORYING
	case eSKILL_MONSTER:
		pSkillState = new SkillStateMonsterSkill();
		break;
#endif
	//--------------------------------------------------------------------------------------
    //		버서커 
	//--------------------------------------------------------------------------------------

    case eSKILL_DOUBLE_ATTACK:
		pSkillState = new SkillStateDoubleAttack();        
        break;	

    case eSKILL_TACKLE:
        pSkillState = new SkillStateTackle();
        break;

    case eSKILL_BERSERK_MODE:
        pSkillState = new SkillStateBerserkMode();
        break;
                    	
	case eSKILL_WEAPON_THROWING:	
		pSkillState = new SkillStateWeaponThrowing();
		break;
			
	case eSKILL_DASH:
		pSkillState = new SkillStateDashStun();
		break;

    case eSKILL_MONSTER_TACKLE:
        pSkillState = new SkillStateMonsterTackle();
        break;

	//--------------------------------------------------------------------------------------
    //		드라곤 
	//--------------------------------------------------------------------------------------

	case eSKILL_ASSAULT:
		pSkillState = new SkillStateAssault();
		break;

	case eSKILL_DRAGON_EYE:
		pSkillState = new SkillStateDragonEye();
		break;

	case eSKILL_MACHSLASH:
		pSkillState = new SkillStateMachSlash();
		break;

	case eSKILL_ILLUSION_DANCE:
		pSkillState = new SkillStateIllusionDance();
		break;

	case eSKILL_SONICBLADE:
		pSkillState = new SkillStateSonicBlade();
		break;

	case eSKILL_BLADE_FURY:
		pSkillState = new SkillStateBladeFury();
		break;

	case eSKILL_VIPER_BLADE:
		pSkillState = new SkillStateViperBlade();
		break;

	case eSKILL_EXPLOZEN_ATTACK:
		pSkillState = new SkillStateExplozenAttack();
		break;

	case eSKILL_CHARGE_ATTACK:
		pSkillState = new SkillStateChargeAttack();
		break;

	case eSKILL_SPRIT_SLUG:
		pSkillState = new SkillStateSpritSlug();
		break;
	
	case eSKILL_DRAGON_BEAD:
		pSkillState = new SkillStateDragonBead();
		break;


	//--------------------------------------------------------------------------------------
    //		발키리 
	//--------------------------------------------------------------------------------------

	case eSKILL_FAST_SHOT:
		pSkillState = new SkillStateFastShot();
		break;
	
	case eSKILL_PIERCEARROW:
		pSkillState = new SkillStateMultiTarget();
		break;

	case eSKILL_MANA_DRAIN:
		pSkillState = new SkillStateManaDrain();
		break;

	case eSKILL_AIRBLOW:
		pSkillState = new SkillStateAirBlow();
		break;	
	

	case eSKILL_HEAVY_SHOT:
		pSkillState = new SkillStateHeavyShot();
		break;

	case eSKILL_BLOOD_RAIN:
		pSkillState = new SkillStateBloodRain();
		break;	

	case eSKILL_SNIPING:
		pSkillState = new SkillStateSniping();
		break;

	case eSKILL_SHOCKWAVE:
		pSkillState = new SkillStateShockwave();
		break;

	case eSKILL_MOON_ASSAULT:
		pSkillState = new SkillStateMoonAssault();
		break;

	case eSKILL_DOUBLE_SPINKICK:
		pSkillState = new SkillStateDoubleSpinkick();
		break;

	case eSKILL_ETHER_BLASE:
		pSkillState = new SkillStateBlazeArrow();
		break;


	case eSKILL_SPINING_WAVE:
		pSkillState = new SkillStateSpinningWave();
		break;

	case eSKILL_OVER_LIMIT:
		pSkillState = new SkillStateOverLimit();
		break;

	case eSKILL_DEFENCE_MOTION:
		pSkillState = new SkillStateDefenceMotion();
		break;

	case eSKILL_WILD_SHOT:
		pSkillState = new SkillStateWildShot();
		break;

	case eSKILL_CHASE_SHOT:
		pSkillState = new SkillStateChaseShot();
		break;


	//--------------------------------------------------------------------------------------
    //		엘리멘탈리스트 
	//--------------------------------------------------------------------------------------

	case eSKILL_REVIVAL:
		pSkillState = new SkillStateRevival();
		break;

	case eSKILL_REVERSE:
		pSkillState = new SkillStateReverse();
		break;

	case eSKILL_FIRE_FILA:
		pSkillState = new SkillStateFireFila();
		break;

	case eSKILL_SONY_VINE:
		pSkillState = new SkillStateSonyVine();
		break;

	case eSKILL_FIRE_ARROW:
		pSkillState = new SkillStateFireArrow();
		break;

	case eSKILL_FIRE_BALL:
		pSkillState = new SkillStateFireBall();
		break;

	case eSKILL_FIRE_SPEAR:
		pSkillState = new SkillStateFireSpear();
		break;

	case eSKILL_ICE_DAGGER:
	case eSKILL_ICE_DAGGER2:
		pSkillState = new SkillStateIceDagger();
		break;

	case eSKILL_ICE_BLAST:
		pSkillState = new SkillStateIceBlast();
		break;

	case eSKILL_ICE_BOLT:
		pSkillState = new SkillStateIceBolt();
		break;

    case eSKILL_PRESSING_WALL:
        pSkillState = new SkillStateIcePressingWall();
        break;

	case eSKILL_FROZEN_ARROW:
		pSkillState = new SkillStateFrozenArrow();
		break;

    case eSKILL_FROZON_SHIELD:
        pSkillState = new SkillStateFrozenShield();
        break;

    case eSKILL_WIND_BOLT:
        pSkillState = new SkillStateWindBolt();
        break;

    case eSKILL_LIGHTING:
        pSkillState = new SkillStateLighting();
        break;

    case eSKILL_ELECTRIC_FIELD:
        pSkillState = new SkillStateElectricField();
        break;

    case eSKILL_WIND_SHIELD:
        pSkillState = new SkillStateWindShield();
        break;

    case eSKILL_LIGHTING_WAVE:
        pSkillState = new SkillStateLightingWave();
        break;

    case eSKILL_POISON_THORN:
        pSkillState = new SkillStatePoisonThorn();
        break;

    case eSKILL_POISON_RAIN:
        pSkillState = new SkillStatePoisonRain();
        break;

    case eSKILL_POWER_WAVE:
        pSkillState = new SkillStatePowerWave();
        break;

    case eSKILL_MAGIC_SHIELD:
        pSkillState = new SkillStateMagicShield();
        break;

    case eSKILL_TELEPORT:
	case eSKILL_TELEPORT2:
        pSkillState = new SkillStateTeleport();
        break;

	case eSKILL_STAR_FALL:
		pSkillState = new SkillStateStarFall();
		break;

	case eSKILL_CIRCLE_SPRITS:
		pSkillState = new SkillStateCircleSprits();
		break;

	case eSKILL_INABILITTY:
		pSkillState = new SkillStateInAbility();
		break;

	case eSKILL_SAINT_AID:
		pSkillState = new SkillStateSaintAid();
		break;

	case eSKILL_PURGE:
		pSkillState = new SkillStatePurge();
		break;


	//--------------------------------------------------------------------------------------
	//		섀도우
	//--------------------------------------------------------------------------------------
	case eSKILL_POISON_DAGGER:		
		pSkillState = new SkillStatePoisonDagger();
		break;
	case eSKILL_DARK_SPARK:
		pSkillState = new SkillStateDarkSpark();
		break;
	case eSKILL_DARK_FORCE:
		pSkillState = new SkillStateDarkForce();
		break;
	case eSKILL_DARK_STUN:
		pSkillState = new SkillStateDarkStun();
		break;
	case eSKILL_DARK_BREAK:
		pSkillState = new SkillStateDarkBreak();
		break;
	case eSKILL_HP_SUCTION:
		pSkillState = new SkillStateHpSuction();
		break;
	//case eSKILL_MP_SUCTION:
	//	pSkillState = new SkillStateMpSuction();
	//	break;
	case eSKILL_HIDE:
		pSkillState = new SkillStateHide();
		break;
	case eSKILL_RUSH:
		pSkillState = new SkillStateRush();
		break;


    //--------------------------------------------------------------------------------------
    //		미스틱
    //--------------------------------------------------------------------------------------
    case eSKILL_MYSTIC_907:
        pSkillState = new SkillStateSnakeKick();
        break;
    case eSKILL_MYSTIC_908:
        pSkillState = new SkillStateRollingPalm();
        break;
    case eSKILL_MYSTIC_905:
        pSkillState = new SkillStateRisingSerpent();
        break;
    case eSKILL_MYSTIC_901:
    case eSKILL_MYSTIC_914:
        pSkillState = new SkillStateMystic();
        break;
	case eSKILL_MYSTIC_906:
		pSkillState = new SkillStateMysticSmash();
		break;

	case eSKILL_MYSTIC_921:	// 미스틱 블링크
		pSkillState = new SkillStateMystyBlink();
		break;

	case eSKILL_MYSTIC_916:
		pSkillState = new SkillStateMysticDance();
		break;

	case eSKILL_CREATE_CRYSTAL_WARP:
		pSkillState = new SkillStateCreateCrystalWarp();
		break;

	case eSKILL_DESTROY_CRYSTAL_WARP:
		pSkillState = new SkillStateDestroyCrystalWarp();
		break;

    //--------------------------------------------------------------------------------------
    //		헬로이드
    //--------------------------------------------------------------------------------------
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID

    case eSKILL_HELLROID_1005:  // 드릴어택
        pSkillState = new SkillStateDrillAttack();
        break;
        
    case eSKILL_HELLROID_1008:  // 플라잉차지
        pSkillState = new SkillStateFlyingCharging();
        break;

    case eSKILL_HELLROID_1015:  // 헬로이드스핀
        pSkillState = new SkillStateHellroidSpin();
        break;

    case eSKILL_HELLROID_1026:  // 슬라이딩 어택
        pSkillState = new SkillStateSlidingAttack();
        break;

    case eSKILL_HELLROID_1030:
        pSkillState = new SkillStateGuardianShield();
        break;


#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID

        //--------------------------------------------------------------------------------------
        //		위치블레이드
        //--------------------------------------------------------------------------------------
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

    case eSKILL_WITCHBLADE_1103:  // 스핀턴쓰러스트
        pSkillState = new SkillStateSpinTurnThrust();
        break;

    case eSKILL_WITCHBLADE_1110:  // 스피닝하츠
        pSkillState = new SkillStateSpiningHearts();
        break;

    case eSKILL_WITCHBLADE_1113:  // 다크폴링그레이브
        pSkillState = new SkillStateDarkPolingGrave();
        break;

    case eSKILL_WITCHBLADE_1120:  // 블레어팡
        pSkillState = new SkillStateBlairPang();
        break;

    case eSKILL_WITCHBLADE_1133:  // 루튼포인트
        pSkillState = new SkillStateRutainPoint();
        break;

#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

	//--------------------------------------------------------------------------------------
	//		기본은 연출이 필요없는 플레이어 스킬이다.
	//--------------------------------------------------------------------------------------
	default:
		pSkillState = new SkillStateCustom();
		break;
	}

#ifdef _K2P_SKILL_REFACTORYING
	m_StateBaseMap.insert( STATE_BASE_HASH_MAP_ITR_TYPE::value_type( pInfo->m_SkillClassCode, pSkillState ) );
#endif

	return pSkillState;
}


//------------------------------------------------------------------------------
/**
*/
BOOL SkillStateFactory::CanUseSkill(Character *pCharacter,int iSkillID)
{
	return TRUE;
}

BOOL SkillStateFactory::CreateNonstopSkillEffect(Character *pCharacter,Character *pTarget,int iSkillID)
{
	BASE_SKILLINFO *pInfo = SkillInfoParser::Instance()->GetSkillInfo(iSkillID);
	assert(pInfo && "Invalid Skill Code");

	switch(pInfo->m_SkillClassCode)
	{
		case eSKILL_DRAGON_EYE:
		SkillStateDragonEye::PlayNonstopEffect(pCharacter,pTarget);
		break;

	}

	return TRUE;

}
//------------------------------------------------------------------------------ 
SkillStateFactory::ResultProcessingType SkillStateFactory::GetResultProcessingType(SLOTCODE skill_code)
{
    BASE_SKILLINFO* skill_info = SkillInfoParser::Instance()->GetSkillInfo(skill_code);
    if (skill_info == NULL)
    {
        return SkillStateFactory::kProcessing_PacketRecvImmediate;
    }

    //if (skill_info->m_wFlyingLifeTime <= 0)
    //{
    //    return SkillStateFactory::kProcessing_Projectile;
    //}
    
    switch(skill_info->m_SkillClassCode)
    {
    case eSKILL_POISON_THORN:
    case eSKILL_FIRE_BALL:
    case eSKILL_ICE_BLAST:
    case eSKILL_SHOCKWAVE:
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
    case eSKILL_HELLROID_1017:  // 에테르차징
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
        return SkillStateFactory::kProcessing_Projectile;
    default:
        break;
    }

    return SkillStateFactory::kProcessing_PacketRecvImmediate;
}