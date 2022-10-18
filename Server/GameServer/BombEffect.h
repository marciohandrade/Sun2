#pragma once

#include "BaseEffect.h"


class BombEffect : public BaseEffect
{
public:
	BombEffect() {}
	~BombEffect() {}

	virtual VOID		End();
	virtual VOID		SetSkillPercentDamage( float fSkillPercentDamage ) { m_fSkillPercentDamage = fSkillPercentDamage; }

private:
	float				m_fSkillPercentDamage;
};


struct BASE_ABILITYINFO;

struct SelfDestructionDamage_info_t {
	DAMAGETYPE	m_Damage;		// Damage or DamageRatio
	BYTE		m_DamageOpt;	//SelfDestructionDamageEffect::eDamageOpt
};

class SelfDestructionDamageEffect : public BaseEffect
{
public:		SelfDestructionDamageEffect() : IntegrityChecker(m_eStep) {}
			~SelfDestructionDamageEffect() {}
			enum eDamageOpt { eDamageOpt_None = 0, eDamageOpt_Value = 1, eDamageOpt_Ratio = 2, };

			VOID		SetInformation( Character* const pAttacker, GameField* const pGameField, const SelfDestructionDamage_info_t* const pDamaged_char_info );
	virtual	VOID		Start();
	virtual	VOID	End();
	virtual BOOL		Update( DWORD curTick );

	inline	BOOLEAN		IsPassIntegrity() const { return &IntegrityChecker == &m_eStep; }
			VOID		CmdExecute();

private:
	enum eStep { eStep_Init, eStep_Execute, eStep_Executed, eStep_Release };
	const	eStep&						IntegrityChecker;
private:	GameField*					m_pGameFieldForIntegrity;
			eStep						m_eStep;
			static const DWORD			TIMEOUT_LIFE_TIME	= 10*1000;
			ITimerBase					m_ExecutableLifetime;
			SelfDestructionDamage_info_t	m_DamagedCharInfo;
    //
    __DISABLE_COPY(SelfDestructionDamageEffect);
};

