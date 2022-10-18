#pragma once

#include "NPC.h"
#include <Timer.h>


class Summoned : public NPC
{
public:
	Summoned();
	virtual ~Summoned();

protected:
	virtual BOOL				Init();

public:
			void				InitSummoned( WORD NPCCode, SLOTCODE SKillCode, Character * pHost, DWORD ExpireTime );
			void				InitSummonedDeadMonster( WORD NPCCode, SLOTCODE SKillCode, Character* pHost );

            void                ApplyPlayerStatusToSummoned(SLOTCODE skill_code, Character* host_char);
            void                AcquireBasicSummonerStatus(const BASE_ABILITYINFO* ability_info, Character* host_char);
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
            void                AcquireExtendSummonerStatus(const BASE_ABILITYINFO* ability_info, Character* host_char);          
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
            eSUMMON_TYPE		GetSummonType() const { return m_SummonType; }

	virtual BOOL				Update( DWORD dwDeltaTick );
	virtual VOID				Reuse()		{ Release(); }

	virtual VOID				OnLeaveField();

	virtual VOID				OnDead();
	virtual VOID				GetSummonedRenderInfo( SUMMONED_RENDER_INFO * OUT pRenderInfo );

	VOID						SetExpireTime( DWORD dwInterval );
    DWORD GetRemainedTime() const;
    SLOTCODE GetSummonSkillCode() const;

#if !defined(_NA_000000_20140224_SUMMONER_REFACTORING)
	virtual VOID				SetSummoner( Character *pSummoner );
#endif //_NA_000000_20140224_SUMMONER_REFACTORING
	Character*					GetSummoner();
	virtual Player *			GetPlayerOwner();

	eSUMMON_COMMAND				GetCommandState() const { return m_eCommandState; }
	VOID						SetCommandState( eSUMMON_COMMAND eCommand, DWORD dwTargetKey );

	VOID						SetSkillUseDelegate( BOOL bSkillUse )	{ m_bSkillUseDelegate = bSkillUse; }
	BOOL						GetSkillUseDelegate()	{ return m_bSkillUseDelegate; }

	RC::eSKILL_RESULT			CanUseSkilltoTarget( SLOTCODE SkillCode, DWORD TargetKey );
	RC::eSKILL_RESULT			CanUseSkill( SLOTCODE SkillCode, DWORD dwTargetKey = 0, BOOL bCoolTimerReset = TRUE );

	BOOL						IsNonPVP(){ return m_eCommandState == eSUMMON_COMMAND_DELEGATE_ATTACK_IGNORE_PLAYER; }
	virtual BOOL				IsDelegate()	{ return IsOffensive() || IsDefensive() || IsNonPVP(); }
	BOOL						IsOffensive()	{ return m_eCommandState == eSUMMON_COMMAND_DELEGATE_ATTACK; }
	BOOL						IsDefensive()	{ return m_eCommandState == eSUMMON_COMMAND_DELEGATE_DEFENSE; }
	BOOL						IsFollowing()	{ return m_eCommandState == eSUMMON_COMMAND_FOLLOW; }

	// 스킬관련
	VOID						AddSkill( SLOTCODE SkillCode );
	BOOL						DelSkill( SLOTCODE SkillCode );

	virtual BOOL				CheckClassDefine( SLOTCODE SkillCode, BOOL bUseActiveSkill );
	virtual BOOL				CanResurrection( Character *pTarget );

// _NA_0_20091118_GAMEROOM_BATTLE_RESULT
    virtual	VOID				OnRecover( INT recoverHP, INT recoverMP, INT recoverSD = 0, eRECOVER_TYPE Type = eRECOVER_TYPE_NORMAL, Character* pAttacker = NULL );

	virtual VOID				OnEnterObject( const Sector & IN rSector );
#ifdef _NA_006592_VALKYRIES_SUMMONS_SKILL_COOLTIME_BUG
    RC::eSKILL_RESULT CanUseSkill(SLOTCODE SkillCode, BOOL bCoolTimerReset);
    bool CoolTimerReset(SLOTCODE skill_code);
#endif // _NA_006592_VALKYRIES_SUMMONS_SKILL_COOLTIME_BUG
protected:
    virtual void _Damaged(DamageArgs* damage_arg);

public:
    virtual VOID				ForcedAttack( Character *pTarget );
	virtual eUSER_RELATION_TYPE	IsFriend( Character *pTarget );

protected:
	virtual VOID				Release();

private:
	virtual VOID				SetInitialState( DWORD dwStateID, BYTE byMoveType=0, DWORD dwMoveAreaID=0, LPARAM param1=0 );
	virtual VOID				AddDefaultSkill();
	void ApplySummonerPassiveSkill(const bool is_apply = true);
			VOID				ApplyAdrenaline();

	eSUMMON_COMMAND				m_eCommandState;		// 위임 상태
#if !defined(_NA_000000_20140224_SUMMONER_REFACTORING)
	Character*				m_pSummoner;			// 소환자
#endif //_NA_000000_20140224_SUMMONER_REFACTORING
    util::ITimerBase m_pExpireTimer[1];		// 소환체 소멸타이머

	BOOL						m_bSkillUseDelegate;	// 스킬 사용 위임상태
	eSUMMON_TYPE				m_SummonType;			// 소환타입.
    SLOTCODE summon_skill_code_; // 소환에 사용된 스킬코드
};