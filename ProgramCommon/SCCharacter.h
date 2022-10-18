#pragma once
#ifndef PROGRAMCOMMON_SCCHARACTER_H
#define PROGRAMCOMMON_SCCHARACTER_H

//==================================================================================================
//  @history
//  100827, waverix - r100827.4L, a server object is not derived this class anymore
//                    because of a server and client logic is definitely different system
//                    that don't have shared process using this.

//==================================================================================================

#include <Const.h>
#include <Attribute.h>

// CLIENT
#if !defined(_SERVER)
//
#include <SolarHashTable.h>
#include <Timer.h>


class SCCharacter
{
public:		SCCharacter();
			~SCCharacter();

			// Skill Cool Timer
			VOID                InitializeHashTable( DWORD dwTimerMaxNum );
			VOID				SetCoolTime( SLOTCODE SkillCode, WORD wCoolTimeMagin = 0 );
			VOID				FreeCoolTime( SLOTCODE SkillCode );
			VOID				SetAllCoolTime( BOOL bPoolUse, WORD wCoolTimeMagin = 0 );
			VOID				FreeAllCoolTime( BOOL bPoolUse );
			VOID				InitializeCooltime();

	inline	util::Timer *		GetCoolTimer( SLOTCODE SkillCode ) { return m_pSkillCoolTimeHash->GetData( SkillCode ); };

protected:	// 스킬 쿨타임 타이머 해쉬테이블(각 스킬별로 타이머를 가지고 있다)
			util::SolarHashTable<util::Timer *>*	m_pSkillCoolTimeHash;
};

#endif //!defined(_SERVER)

//==================================================================================================

// SERVER
#ifdef _SERVER

class Attributes;
class Player;


struct ICharacterParameter
{
    virtual Attributes& GetAttr() = 0;
    virtual const Attributes& GetAttr() const = 0;
    //_NA_001231_20081216_ADD_OPTION_KIND 
    virtual int GetResistBadStatusRatio(WORD state_id) = 0;

    virtual Player* GetPlayerOwner() = 0;

    virtual LEVELTYPE GetLevel() const = 0;
    virtual LEVELTYPE GetDisplayLevel() const = 0;
    virtual float GetAttackRange() const = 0;
    virtual float GetNormalRangeBonus() const = 0;
    virtual float GetSkillRangeBonus() const = 0;
    virtual float GetSightRange() const = 0;
    virtual eARMOR_TYPE  GetArmorType() const = 0;              // EP2: unused
    virtual eMELEE_TYPE  GetMeleeType() const = 0;              // EP2: unused
    virtual eATTACK_TYPE GetWeaponBaseAttType() const = 0;      // 무기 기본 공격타입(근거리, 원거리, 마법)
    virtual eATTACK_TYPE GetWeaponMagicalAttType() const = 0;   // EP2: same GetWeaponBaseAttType
    virtual DWORD GetHP() const = 0;
    virtual DWORD GetMP() const = 0;
    virtual DWORD GetSD() const = 0;
    virtual float GetHPRatio() const = 0;
    virtual float GetMPRatio() const = 0;
    virtual float GetSDRatio() const = 0;
    virtual DWORD GetMaxHP() const = 0;
    virtual DWORD GetMaxMP() const = 0;
    virtual DWORD GetMaxSD() const = 0;
    virtual int   GetRegenHP() const = 0;
    virtual int   GetRegenMP() const = 0;
    virtual int   GetRegenSD() const = 0;
    virtual const TCHAR* GetCharName() const = 0;

    virtual BOOL  IsAlive() const = 0;
    virtual BOOL  IsDead() const = 0;
    virtual DWORD GetPhysicalAvoidValue() const = 0;
    virtual float GetPhysicalAttackSpeed() const = 0;
    virtual EXPTYPE GetExp() const = 0;
    virtual short GetExperty1() const = 0;
    virtual short GetExperty2() const = 0;
    virtual int   GetMoveSpeedRatio() const = 0;
    virtual int   GetAttSpeedRatio() const = 0;
    virtual float GetBodyRadius() const = 0;
    virtual int   GetMPSpendIncValue() const = 0;
    virtual float GetMPSpendIncRatio() const = 0;

    virtual void  SetAttr(eATTR_TYPE eAttrType, eATTR_KIND eAttrKind, int iValue) = 0;
    virtual void  SetHP(DWORD hp) = 0;
    virtual void  SetMP(DWORD mp) = 0;
    virtual void  SetSD(DWORD sd) = 0;
    //
    virtual DWORD IncreaseHP(DWORD increment) = 0;
    virtual DWORD DecreaseHP(DWORD decrement, int fLimitHP = 0) = 0;
    virtual DWORD IncreaseMP(DWORD increment) = 0;
    virtual DWORD DecreaseMP(DWORD decrement) = 0;
    virtual DWORD IncreaseSD(DWORD increment) = 0;
    virtual DWORD DecreaseSD(DWORD decrement) = 0;
};

#endif //_SERVER

#endif //PROGRAMCOMMON_SCCHARACTER_H