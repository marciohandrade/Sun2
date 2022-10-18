#include "stdafx.h"
#include "Summoned.h"
#include "GameField.h"
#include "SummonIdle.h"
#include "TrackState.h"
#include "AttackState.h"
#include "DeadState.h"
#include "ThrustState.h"
#include "JumpState.h"
#include "FallApartState.h"
#include "RetreatState.h"
#include "FlyingState.h"
#include "KnockDownState.h"
#include "RunAwayState.h"
#include "ChaosState.h"
#include "StatusManager.h"
#include "SkillInfoParser.h"
#include "SCSkillSlotContainer.h"
#include "Player.h"
#include "SCSkillSlot.h"
#include "ObjectFactory.h"
#include "NPCStateManager.h"
#include "TargetManager.h"
#include "ChaoState.h"
#include "PVPInfo.h"
#include <Attribute.h>

#include "Skill.h"
#include "SkillSlotManager.h"

#include "StatusManager.h"
#include "IntensifySummon.h"

#ifdef _NA_004791_20120430_1202_SUMMON_STATE
#include "SummonedNPCs.h"
#endif // _NA_004791_20120430_1202_SUMMON_STATE
#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
#include "GameZone.h"
#include "GameInstanceDungeon.h"
#endif // _NA_006088_20120910_CHAOS_ZONE_FREE_PVP

Summoned::Summoned() 
    : summon_skill_code_(0)
{
#if !defined(_NA_000000_20140224_SUMMONER_REFACTORING)
    m_pSummoner = NULL;
#endif
	SetObjectType( SUMMON_OBJECT );
}

Summoned::~Summoned()
{
}

BOOL Summoned::Init()
{	
	m_eCommandState = eSUMMON_COMMAND_DELEGATE_ATTACK_IGNORE_PLAYER;
	m_bSkillUseDelegate = FALSE;

	return NPC::Init();
}

void Summoned::InitSummoned(WORD npc_code, SLOTCODE skill_code, Character* host_char, DWORD expire_time)
{
    this->Init();
#ifdef _NA_000000_20140224_SUMMONER_REFACTORING
    SetSummonerKey( host_char->GetObjectKey() );
#else
    this->SetSummoner(host_char);
#endif //_NA_000000_20140224_SUMMONER_REFACTORING
    this->SetExpireTime(expire_time);
    NPC::SetBaseInfo(npc_code, 0, 0);

#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    const SkillInfoParser* skill_parser = SkillInfoParser::Instance();
    const SkillScriptInfo* skill_info = skill_parser->GetSkillInfo(skill_code);

    if (skill_info->GetAbilityInfo(eABILITY_SUMMON) != NULL) {
        ApplyPlayerStatusToSummoned(skill_code, host_char);
        m_SummonType = eSUMMON_NORMAL;
    }
    else if (skill_info->GetAbilityInfo(eABILITY_SUMMON_IMMOVABLE) != NULL) {
        //본인이 가진 기본스킬을 등록시켜준다
        ApplyPlayerStatusToSummoned(skill_code, host_char);
        NPC::AddDefaultSkill();
        m_SummonType = eSUMMON_IMMOVABLE;
        m_eCommandState = eSUMMON_COMMAND_SKILL_ATTACK;
    }
#else
    ApplyPlayerStatusToSummoned(skill_code);
#endif //_NA_007667_20141001_WITCHBLADE_SKILL

    Attributes& attributes = this->GetAttr();
    ;{
        attributes.UpdateEx();
        DWORD max_hp = this->GetMaxHP();
        this->SetHP(max_hp);
        DWORD max_mp = this->GetMaxMP();
        this->SetMP(max_mp);
    };

    ApplySummonerPassiveSkill();
    ApplyAdrenaline();
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
#else
    m_SummonType = eSUMMON_NORMAL;
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
    summon_skill_code_ = skill_code;
    //
}

void	Summoned::InitSummonedDeadMonster( WORD NPCCode, SLOTCODE SKillCode, Character* pHost )
{
	this->Init();
#ifdef _NA_000000_20140224_SUMMONER_REFACTORING
    SetSummonerKey( pHost->GetObjectKey() );
#else
	this->SetSummoner( pHost );
#endif //_NA_000000_20140224_SUMMONER_REFACTORING
	m_pExpireTimer->Stop();

	NPC::SetBaseInfo( NPCCode, 0, 0 );
#ifdef _NA_007123_20140324_SHADOW_SKILL_RENEWAL
    // NPC스킬을 추가해준다.
    NPC::AddDefaultSkill();
#endif //_NA_007123_2014_0324_SHADOW_SKILL_RENEWAL

	m_SummonType = eSUMMON_DEAD_MONSTER;
	m_eCommandState = eSUMMON_COMMAND_DELEGATE_ATTACK;
}

VOID Summoned::Release()
{
#if !defined(_NA_000000_20140224_SUMMONER_REFACTORING)
	m_pSummoner = NULL;
#endif //_NA_000000_20140224_SUMMONER_REFACTORING

	NPC::Release();
}

BOOL Summoned::Update( DWORD dwDeltaTick )
{
	// 시간이 다되면 소멸된다.
	if( m_pExpireTimer->IsExpired() && IsAlive() )
	{
		SetHP(0);
		OnDead();

		// 삭제한다.
		return FALSE;
	}
#ifdef _NA_004791_20120430_1202_SUMMON_STATE
    Character* const target_character = GetTargetChar();
    if (target_character && (target_character->CanBeAttacked() || UseResurrectionSkill()) == false)
    {
        SummonedNPCs* summoned_npc = SummonManager::Instance()->FindSummonNPCs(GetSummonerKey());
        if (summoned_npc != NULL && m_eCommandState != summoned_npc->GetCommandState())
        {
            SetCommandState( summoned_npc->GetCommandState(), target_character->GetObjectKey() );
        }
    }
#endif  // _NA_004791_20120430_1202_SUMMON_STATE
#ifdef _NA_005005_20120622_SUMMON_PVP_RULE_SYNC_OWNER_PVP_RULE
    // 발키리 소환수 PVP룰을 주인과 같이 룰로 적용..
    Player* const summon_owner = GetPlayerOwner();
    if (obj_util::IsValid(summon_owner, __FUNCTION__, __LINE__))
    {
        if (target_character && summon_owner->ChkAttackPvP(target_character) != RC::RC_BATTLE_SUCCESS)
        {
            // 주인이 PVP를 할수 없는 상황이 면 소환수가 가진 PVP Target을 잃어 버리게 한다..
            SetTargetChar(NULL);
        }
    }
#endif  // _NA_005005_20120622_SUMMON_PVP_RULE_SYNC_OWNER_PVP_RULE

	// 큐에 쌓인 AI 메세지 처리
	//AIMsgProgress();

	//m_pTargetManager->Update();
	//BOOL rt = m_pNPCStatusManager->Update( dwDeltaTick );
	//if( !rt ) return FALSE;

	return NPC::Update( dwDeltaTick );
}


VOID Summoned::SetInitialState( DWORD dwStateID, BYTE byMoveType, DWORD dwMoveAreaID, LPARAM param1 )
{
	m_pNPCStatusManager->Release();

	m_pNPCStatusManager->AddStateObject( STATE_ID_TRACK, STATE_ID_TRACK );
	m_pNPCStatusManager->AddStateObject( STATE_ID_ATTACK, STATE_ID_ATTACK );
	m_pNPCStatusManager->AddStateObject( STATE_ID_DEAD, STATE_ID_DEAD );
	m_pNPCStatusManager->AddStateObject( STATE_ID_THRUST, STATE_ID_THRUST );
	m_pNPCStatusManager->AddStateObject( STATE_ID_FLYING, STATE_ID_FLYING );
	m_pNPCStatusManager->AddStateObject( STATE_ID_KNOCKDOWN, STATE_ID_KNOCKDOWN );
	m_pNPCStatusManager->AddStateObject( STATE_ID_JUMP, STATE_ID_JUMP );
	m_pNPCStatusManager->AddStateObject( STATE_ID_FALL_APART, STATE_ID_FALL_APART );
	m_pNPCStatusManager->AddStateObject( STATE_ID_RETREAT, STATE_ID_RETREAT );
	m_pNPCStatusManager->AddStateObject( STATE_ID_RUNAWAY, STATE_ID_RUNAWAY );
	m_pNPCStatusManager->AddStateObject( STATE_ID_CHAOS, STATE_ID_CHAOS );

	m_pNPCStatusManager->AddStateObject( STATE_ID_IDLE, STATE_ID_SUMMON_IDLE );
	m_pNPCStatusManager->AddStateObject( STATE_ID_WANDER, STATE_ID_SUMMON_IDLE );
	m_pNPCStatusManager->AddStateObject( STATE_ID_RETURN, STATE_ID_SUMMON_IDLE );
	m_pNPCStatusManager->AddStateObject( STATE_ID_BLIND, STATE_ID_BLIND );

    m_pNPCStatusManager->AddStateObject( STATE_ID_DESTROY_OBJECT, STATE_ID_DESTROY_OBJECT );
    m_pNPCStatusManager->AddStateObject( STATE_ID_SPECIAL_DEAD_ACTION, STATE_ID_DESTROY_OBJECT );

	// 초기상태 설정
	m_pNPCStatusManager->Init( dwStateID, m_pBaseInfo->m_byMoveAttitude, m_pBaseInfo->m_dwMoveAreaID, m_pBaseInfo->m_dwSpawnTime );

	GetStatusManager()->AllocStatus( eCHAR_STATE_ETC_DISABLE_VISION, m_pBaseInfo->m_dwSpawnTime, 0 );
}


VOID Summoned::OnDead()
{
    Player* const pPlayer = GetPlayerOwner();
    if (pPlayer) {
		pPlayer->SetForceAttack( false );
    }

	NPC::OnDead();

	//위의 NPC::OnDead()에서 DeadState가 생성되었다.
	if( GetSummonType() == eSUMMON_DEAD_MONSTER )
	{
		State* pState = GetStateManager()->GetCurState();
		if( !pState )	return;

		if( pState->GetStateID() != STATE_ID_DEAD )	return;

		SetDeadType( eCHAR_DEAD_SUMMON );
        static_cast<DeadState*>(pState)->SetNextTimeoutInterval(0);
	}
}

VOID Summoned::AddSkill( SLOTCODE SkillCode )
{
	SkillScriptInfo *pNewSkill = SkillInfoParser::Instance()->GetSkillInfo( SkillCode );
	if( !pNewSkill )	return;

	// 사용가능 스킬 추가
	for( int i = 0; i < BASE_NPCINFO::MAX_SKILL_NUM; ++i )
	{
		if( !m_pBaseInfo->m_wSkillCode[i] )		continue;
		
		SkillScriptInfo *pBaseSkill = SkillInfoParser::Instance()->GetSkillInfo( m_pBaseInfo->m_wSkillCode[i] );
		if( !pBaseSkill )	continue;

		// 같은 종류의 스킬이 있는지 체크 후 추가한다.
		if( pBaseSkill->m_SkillClassCode != pNewSkill->m_SkillClassCode )	continue;
	
		m_SkillSelector.Add( SkillCode,	m_pBaseInfo->m_bySkillRate[i] );

		// 쿨타임 추가
		SetSkillCoolTime( SkillCode, 0 );
	}
}

BOOL Summoned::DelSkill( SLOTCODE SkillCode )
{
	FreeSkillCoolTime( SkillCode );
	return m_SkillSelector.Remove( SkillCode );
}

#if !defined(_NA_000000_20140224_SUMMONER_REFACTORING)
VOID Summoned::SetSummoner( Character *pSummoner )
{
	m_pSummoner = pSummoner;
	SetSummonerKey( pSummoner->GetObjectKey() );
}
#endif //_NA_000000_20140224_SUMMONER_REFACTORING

VOID Summoned::AddDefaultSkill()
{
	m_SkillSelector.Release();

    Player* pPlayer = GetPlayerOwner();

    if (pPlayer == NULL) {
        return;
    }

	// 플레이어가 가지고 있는 스킬들 중 소환체관련 스킬을 소환체에 추가한다.
    // 발키리의 소환체에 해당되는 로직
	m_SkillSelector.Release();
	for( int i = 0; i < BASE_NPCINFO::MAX_SKILL_NUM; ++i )
	{
		if( !m_pBaseInfo->m_wSkillCode[i] )		continue;

		SkillScriptInfo *pBaseSkill = SkillInfoParser::Instance()->GetSkillInfo( m_pBaseInfo->m_wSkillCode[i] );
		if( !pBaseSkill )	continue;

		SCSkillSlotContainer *pContainer = pPlayer->GetSkillSlotManager()->GetSkillSlotContainer();
		SCSkillSlot *pCurSkillSlot = pContainer->GetSkillSlotByClass( pBaseSkill->m_SkillClassCode );
		if( !pCurSkillSlot )	continue;

		m_SkillSelector.Add( pCurSkillSlot->GetCode(),	m_pBaseInfo->m_bySkillRate[i] );

		// 쿨타임 추가
		SetSkillCoolTime( pCurSkillSlot->GetCode(), 0 );
	}
}

VOID Summoned::SetExpireTime( DWORD dwInterval )
{
	m_pExpireTimer->SetTimer( dwInterval );

	if( !dwInterval )
		m_pExpireTimer->Stop();
}

DWORD Summoned::GetRemainedTime() const
{
    return m_pExpireTimer->GetRemaindTime();
}

SLOTCODE Summoned::GetSummonSkillCode() const
{
    return summon_skill_code_;
}

VOID Summoned::SetCommandState( eSUMMON_COMMAND eCommand, DWORD dwTargetKey )
{
	switch(eCommand)
	{
	case eSUMMON_COMMAND_ATTACK:			// 공격지시
		{
			AI_MSG_FORCE_ATTACK AIAttackMsg;
			AIAttackMsg.dwTargetKey = dwTargetKey;
			SendAIMessage( &AIAttackMsg, sizeof(AIAttackMsg) );
#ifdef _NA_004791_20120430_1202_SUMMON_STATE
            m_eCommandState = eCommand;
#endif // _NA_004791_20120430_1202_SUMMON_STATE

		}
		break;
	case eSUMMON_COMMAND_FOLLOW:			// 뒤따름
		{
			AI_MSG_COMMAND_FOLLOW AIFollowMsg;
			SendAIMessage( &AIFollowMsg, sizeof(AIFollowMsg) );
			m_eCommandState = eCommand;
		}
		break;
	case eSUMMON_COMMAND_DELEGATE_ATTACK:
	case eSUMMON_COMMAND_DELEGATE_DEFENSE:
		m_eCommandState = eCommand;
		break;
	case eSUMMON_COMMAND_DELEGATE_ATTACK_IGNORE_PLAYER:
		{
			AI_MSG_COMMAND_NONPVP AINONPVPMsg;
			SendAIMessage( &AINONPVPMsg, sizeof(AINONPVPMsg) );
			m_eCommandState = eCommand;
		}
		break;
	case eSUMMON_COMMAND_SKILL_ATTACK:
		SetSkillUseDelegate( TRUE );
		break;
	default:
		SUNLOG( eCRITICAL_LOG, "[Summoned::SetCommandState] Invalid Command[%d] ObjKey[%d]", eCommand, GetObjectKey() );
	}
}

BOOL Summoned::CheckClassDefine( SLOTCODE SkillCode, BOOL bUseActiveSkill )
{
	SkillScriptInfo *pBaseSkillInfo = (SkillScriptInfo *)SkillInfoParser::Instance()->GetSkillInfo( SkillCode );
	if( !pBaseSkillInfo )
		return FALSE;

#ifdef _NA_007123_20140324_SHADOW_SKILL_RENEWAL
    if (GetSummonType() == eSUMMON_DEAD_MONSTER) {
        return TRUE;
    }
#endif //_NA_007123_2014_0324_SHADOW_SKILL_RENEWAL
	if( pBaseSkillInfo->m_bySkillUserType != eSKILL_USER_SUMMONED )
		return FALSE;

	// 스킬을 사용할 수 있는 캐릭터인지 체크한다.
	if( m_pBaseInfo->m_wClass != pBaseSkillInfo->m_dwClassDefine )
		return FALSE;

	return TRUE;
}

VOID Summoned::GetSummonedRenderInfo( SUMMONED_RENDER_INFO * OUT pRenderInfo )
{
	pRenderInfo->m_byCurCommand = m_eCommandState;
#ifdef _NA_000000_20140224_SUMMONER_REFACTORING
    pRenderInfo->m_dwSummonerObjKey = GetSummonerKey();
#else
	if( m_pSummoner )	pRenderInfo->m_dwSummonerObjKey = m_pSummoner->GetObjectKey();
	else				pRenderInfo->m_dwSummonerObjKey = 0;
#endif //_NA_000000_20140224_SUMMONER_REFACTORING
	
	pRenderInfo->m_dwObjectKey		= GetObjectKey();
	pRenderInfo->m_dwMonsterCode	= GetBaseInfo()->m_MonsterCode;
	GetPos( &pRenderInfo->m_wvPos );
	pRenderInfo->m_dwMaxHP			= GetMaxHP();
	pRenderInfo->m_dwHP				= GetHP();
	pRenderInfo->m_wMoveSpeedRatio	= (WORD)m_Attr.GetMoveSpeedRatio();
	pRenderInfo->m_wAttSpeedRatio	= m_Attr.GetAttSpeedRatio();

#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    pRenderInfo->m_bySummonType = GetSummonType();
#else
    // 섀도우면.. 
    pRenderInfo->m_bySummonType = eSUMMON_NORMAL;

    Player* const pSummoner = GetPlayerOwner();
    if (pSummoner)
    {
        if( eCHAR_SHADOW == pSummoner->GetCharType() )
            pRenderInfo->m_bySummonType = eSUMMON_DEAD_MONSTER;
    }
#endif //_NA_007667_20141001_WITCHBLADE_SKILL

	// STATE_INFO를 채운다.
	GetStatusManager()->SerialzieStatusInfo( pRenderInfo->m_StateTotalInfo );
}


// _NA_0_20091118_GAMEROOM_BATTLE_RESULT
VOID Summoned::OnRecover( INT recoverHP, INT recoverMP, INT recoverSD, eRECOVER_TYPE Type, Character* pAttacker )
{	
	DWORD oldHP = GetHP();
	DWORD oldMP = GetMP();

#ifdef _NA_006605_VALKYRIES_SUMMONS_BATTLE_RESULT_APPLY
    if (recoverHP > 0)
    {
        DWORD max_hp = GetMaxHP();
        if((oldHP + recoverHP) > max_hp)     
        {
            recoverHP = max_hp - oldHP;
            SetReserveHP(0);
        }
    }
#endif // _NA_006605_VALKYRIES_SUMMONS_BATTLE_RESULT_APPLY

    // 실제 회복을 시킨다.
// _NA_0_20091118_GAMEROOM_BATTLE_RESULT
    Character::OnRecover( recoverHP, recoverMP, recoverSD, Type, pAttacker );

	// HP, MP 변화
	if( oldHP != GetHP() || oldMP != GetMP() )
	{
		MSG_CG_STATUS_RECOVER_ATTR_BRD HPMPMsg;
		HPMPMsg.m_dwObjectKey	= GetObjectKey();
		HPMPMsg.m_dwTargetHP	= GetHP();
		HPMPMsg.m_dwTargetMP	= GetMP();
		SendPacketAround( &HPMPMsg, sizeof(HPMPMsg), FALSE );
	}
}

VOID Summoned::ForcedAttack( Character *pTarget )
{
    if (obj_util::IsValid(pTarget, __FUNCTION__, __LINE__) == false) {
        return;
    }

	const BOOLEAN bApplyGeneralForcedAtkRule = !( Policyment.FIELD & Policyment.PCC_IN_SSQ_FIELD );
	if( !bApplyGeneralForcedAtkRule )
		return;

	if( IsNonPVP() )
		return;

	// 타겟이 플레이어인지 체크
	if(pTarget->IsEqualObjectKind(PLAYER_OBJECT) == 0)
		return;

	// 소환자가 플레이어인지 체크
	Player *pSummonerPlayer = GetPlayerOwner();
    if (pSummonerPlayer == NULL) {
        return;
    }
	Player *pTargetPlayer = (Player*)pTarget;

	//(lst1024)(080905)(2389)소환수의 버프 스킬 시전시 보라돌이 되는 현상 수정
	if( pTargetPlayer == pSummonerPlayer ) return;

	// 강제공격을 했으므로 보라돌이 상태로 만든다.
	if( pSummonerPlayer->IsForceAttack() )
		pSummonerPlayer->GetChaoState().StartBoradoriState( pTargetPlayer );
}


void
Summoned::_Damaged(DamageArgs* damage_arg)
{
    Character* pAttacker = damage_arg->actor_;

    NPC::_Damaged(damage_arg);

    if( pAttacker && IsDead() )          // 소환체가 강제공격(PK)을 받아서 죽었을 경우
    {
        Player* pPlayer       = GetPlayerOwner();
        Player* pAttackPlayer = pAttacker->GetPlayerOwner();
        if( pPlayer && pAttackPlayer )
            pAttackPlayer->KillPlayer(pPlayer,TRUE);
    }
}

eUSER_RELATION_TYPE
Summoned::IsFriend(Character* pTarget)
{
    if (obj_util::IsValid(pTarget, __FUNCTION__, __LINE__) == false) {
        return USER_RELATION_NEUTRALIST;
    }
    
    //// (WAVERIX) (090729) (CHANGES) GameField 검사 추가: for SSQ
    //GameField* pGameField = pTarget->GetField();
    //GameZone* pGameZone = 0;
    //if(pGameField == 0 || !(pGameZone = pGameField->GetGameZone()))
    //    return USER_RELATION_NEUTRALIST;

	// 본인인지 체크한다.
	if(GetObjectKey() == pTarget->GetObjectKey())
        return USER_RELATION_FRIEND; 
	if(this == pTarget)
        return USER_RELATION_FRIEND;

    if (FALSE == pTarget->IsEqualObjectKind(PLAYER_OBJECT) &&
        FALSE == pTarget->IsEqualObjectKind(SUMMON_OBJECT) &&
        FALSE == pTarget->IsEqualObjectKind(CRYSTALWARP_OBJECT))
    {
        return NPC::IsFriend(pTarget);  // 타겟이 플레이어, 소환체가 아니라면 일반적인 적대관계 적용
    }

    // (WAVERIX) (090729) (BUG-FIX) WOPS:4444 타락한 사원에서 발키리 서먼 소환수가 PC를 공격하지 않는 현상
    if(Policyment.FIELD & Policyment.PCC_IN_SSQ_FIELD)
        return NPC::IsFriend(pTarget);

    // 플레이어의 소환수일 경우 소환자의 친구관계를 따른다.
    Player* const summon_owner = GetPlayerOwner();
    if (summon_owner)
    {
        eUSER_RELATION_TYPE releation_type = summon_owner->IsFriend(pTarget);
        // 단, 소환수의 상태(모드)가 "몬스터만공격" 일때는 적대관계가 성립되더라도 중립으로 설정
        // 적대 관계가 성립되면 힐이나, 이로운 버프가 적용되기 때문에..
        if(IsNonPVP() && pTarget->IsEqualObjectKind(PLAYER_OBJECT) )
        {
            if (releation_type != USER_RELATION_FRIEND)
            {
                return USER_RELATION_NEUTRALIST;
            }
        }
        return releation_type;
    }

	return NPC::IsFriend(pTarget);
}

Player* Summoned::GetPlayerOwner()
{
#ifdef _NA_000000_20140224_SUMMONER_REFACTORING
    Character* const owner = GetSummoner();
    if (owner && owner->IsEqualObjectKind(PLAYER_OBJECT)) {
    	return static_cast<Player*>(owner);
    }
#else
    if (obj_util::IsValid(m_pSummoner, __FUNCTION__, __LINE__) == false) {
        return NULL;
    }

    if (m_pSummoner && m_pSummoner->IsEqualObjectKind(PLAYER_OBJECT)) {
		return static_cast<Player*>(m_pSummoner);
    }
#endif //_NA_000000_20140224_SUMMONER_REFACTORING

    return NULL;
}

BOOL Summoned::CanResurrection( Character *pTarget )
{
	if( !pTarget )
		return FALSE;

	Player *pSummoner = GetPlayerOwner();
	if( !pSummoner )
		return TRUE;

#ifdef __NA_0_20080421_REBIRTH_NO_PARTY
	//적군은 부활 안됨
	if( pSummoner->IsFriend(pTarget) == USER_RELATION_ENEMY )
		return FALSE;
#else

		if( !pSummoner->GetPartyState().IsPartyMember() ) 
			return FALSE;

	if( pSummoner->GetPartyState().GetPartyKey() != pTarget->GetPartyState().GetPartyKey() )
		return FALSE;
#endif
	return TRUE;
}

void Summoned::ApplySummonerPassiveSkill(const bool is_apply)
{
    if (is_apply == false)
    {
        return;
    }
    
    const Player* const summoner = GetPlayerOwner();
    if (summoner == NULL)
    {
        return;
    }
    const Skill* const passive_skill = 
        summoner->GetSkillSlotManager()->FindPassiveSkillfromSkillClass(eSKILL_SUMMON_MASTERY);
    if (passive_skill == NULL)
    {
        return;
    }
    const SkillScriptInfo* const skill_info = passive_skill->GetSkillBaseInfo();
    if (skill_info == NULL)
    {
        return;
    }
    const BASE_ABILITYINFO* const ability_info = 
        skill_info->GetAbilityInfo(eABILITY_SUMMONMASTERY);
    if (ability_info == NULL)
    {
        return;
    }
    
    // HP 증가
    const int add_val_1 = ability_info->m_iOption1;
    if (add_val_1 != 0)
    {
        const int max_hp = m_Attr.GetMaxHP() + add_val_1;
        SetAttr(eATTR_MAX_HP, eATTR_KIND_BASE, max_hp);
        SetHP(GetMaxHP());
    }

    // 공격 성공률, 크리티컬 확률 증가
    const int add_val_2 = ability_info->m_iOption2 / 10; // 1000분율을 100분율 값으로 변환
    if (add_val_2 != 0)
    {
        const int attack_success_ratio = m_Attr.GetBasePhysicalAttackRate() + add_val_2;
        SetAttr(eATTR_PHYSICAL_ATTACK_SUCCESS_RATIO, eATTR_KIND_BASE, attack_success_ratio);
        
        const int critical_ratio = m_Attr.GetBaseCriticalRatioChange() + add_val_2;
        SetAttr(eATTR_CRITICAL_RATIO_CHANGE, eATTR_KIND_BASE, critical_ratio);
    }

    // 공격력 증가
    const int add_val_3 = ability_info->m_iParam[0]; 
    if (add_val_3 != 0)
    {
        const int melee_min_attack = m_Attr.GetBaseMeleeMinAttPower() + add_val_3;
        SetAttr(eATTR_BASE_MELEE_MIN_ATTACK_POWER, eATTR_KIND_BASE, melee_min_attack);

        const int melee_max_attack = m_Attr.GetBaseMeleeMaxAttPower() + add_val_3;
        SetAttr(eATTR_BASE_MELEE_MAX_ATTACK_POWER, eATTR_KIND_BASE, melee_max_attack);

        const int range_min_attack = m_Attr.GetBaseRangeMinAttPower() + add_val_3;
        SetAttr(eATTR_BASE_RANGE_MIN_ATTACK_POWER, eATTR_KIND_BASE, range_min_attack);

        const int range_max_attack = m_Attr.GetBaseRangeMaxAttPower() + add_val_3;
        SetAttr(eATTR_BASE_RANGE_MAX_ATTACK_POWER, eATTR_KIND_BASE, range_max_attack);

#if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
        const int magic_min_attack = m_Attr.GetBaseMagicMinAttPower() + add_val_3;
        SetAttr(eATTR_BASE_MAGICAL_MIN_ATTACK_POWER, eATTR_KIND_BASE, magic_min_attack);

        const int magic_max_attack = m_Attr.GetBaseMagicMaxAttPower() + add_val_3;
        SetAttr(eATTR_BASE_MAGICAL_MAX_ATTACK_POWER, eATTR_KIND_BASE, magic_max_attack);
#endif
    }

    // 방어력 증가
    const int add_val_4 = ability_info->m_iParam[1];
    if (add_val_4 != 0)
    {
        const int melee_defense = m_Attr.GetBaseMeleeDefPower() + add_val_4;
        SetAttr(eATTR_BASE_MELEE_DEFENSE_POWER, eATTR_KIND_BASE, melee_defense);

        const int range_defense = m_Attr.GetBaseRangeDefPower() + add_val_4;
        SetAttr(eATTR_BASE_RANGE_DEFENSE_POWER, eATTR_KIND_BASE, range_defense);

#if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
        const int magic_defense = m_Attr.GetBaseMagicDefPower() + add_val_4;
        SetAttr(eATTR_BASE_MAGICAL_DEFENSE_POWER, eATTR_KIND_BASE, magic_defense);
#endif
    }

    // 공격 속도, 이동 속도 증가
    const int add_val_5 = ability_info->m_iParam[2] / 10;
    if (add_val_5 != 0)
    {
        const int attack_speed_ratio = m_Attr.GetBaseAttSpeedRatio() + add_val_5;
        SetAttr(eATTR_ATTACK_SPEED, eATTR_KIND_SKILL_RATIO, attack_speed_ratio);
        
        const int move_speed_ratio = m_Attr.GetBaseMoveSpeedRatio() + add_val_5;
        SetAttr(eATTR_MOVE_SPEED, eATTR_KIND_SKILL_RATIO, move_speed_ratio);
    }
    
    // 회피율 증가
    const int add_val_6 = ability_info->m_iParam[3] / 10;
    if (add_val_6 != 0)
    {
        const int phys_avoid_rate = m_Attr.GetBasePhysicalAvoidRate() + add_val_6;
        SetAttr(eATTR_PHYSICAL_ATTACK_BLOCK_RATIO, eATTR_KIND_BASE, phys_avoid_rate);
    }
}

void Summoned::ApplyAdrenaline()
{
    // CHANGES: f110622.4L, when the Adrenaline status is activating, it is just fail
    // to call the ApplyAdrenaline in InitSummoned
    // because at that point in time, the summoned can't find in its list
    Player* const summoner = GetPlayerOwner();
    if (summoner == NULL) 
    {
        //SUNLOG(eCRITICAL_LOG,
        //    _T("|["__FUNCTION__"]|Msg=invalid request, summoner=0x%p"),
        //    m_pSummoner);
        return;
    };

    if (IntensifySummon* const intensification_status = static_cast<IntensifySummon*>(\
            summoner->GetStatusManager()->FindStatus(eCHAR_STATE_INTENSIFY_SUMMON)))
    {
        intensification_status->ApplyAttributes(this);
    };
}

RC::eSKILL_RESULT Summoned::CanUseSkill( SLOTCODE SkillCode, DWORD TargetKey, BOOL bCoolTimerReset )
{
#ifdef _NA_006592_VALKYRIES_SUMMONS_SKILL_COOLTIME_BUG
    RC::eSKILL_RESULT rcResult = CanUseSkill( SkillCode, bCoolTimerReset );
#else
	RC::eSKILL_RESULT rcResult = NPC::CanUseSkill( SkillCode, bCoolTimerReset );
#endif // _NA_006592_VALKYRIES_SUMMONS_SKILL_COOLTIME_BUG
	if( rcResult != RC::RC_SKILL_SUCCESS )
		return rcResult;

	return CanUseSkilltoTarget(SkillCode,TargetKey);
}

RC::eSKILL_RESULT Summoned::CanUseSkilltoTarget( SLOTCODE SkillCode, DWORD TargetKey )
{
	GameField* pField = GetField();
	if( pField == NULL ) return RC::RC_SKILL_INVALID_STATE;

	Player* pCharacter = static_cast<Player*>( pField->FindObject(PLAYER_OBJECT, TargetKey));
	if( pCharacter )
	{
		SkillScriptInfo *pBaseSkillInfo = SkillInfoParser::Instance()->GetSkillInfo( SkillCode );
		if( pBaseSkillInfo->m_byTarget == SKILL_TARGET_ENEMY || 
			pBaseSkillInfo->m_byTarget == SKILL_TARGET_REACHABLE_ENEMY || 
			pBaseSkillInfo->m_byTarget == SKILL_TARGET_ENEMY_PLAYER )
		{
			if( IsFriend( pCharacter) == USER_RELATION_FRIEND )
				return RC::RC_SKILL_STATE_WHERE_CANNOT_ATTACK_ENEMY;
		}
	}

	return RC::RC_SKILL_SUCCESS;
}

#ifdef _NA_006592_VALKYRIES_SUMMONS_SKILL_COOLTIME_BUG
RC::eSKILL_RESULT Summoned::CanUseSkill( SLOTCODE SkillCode, BOOL bCoolTimerReset )
{
    RC::eSKILL_RESULT rcResult = Character::CanUseSkill( SkillCode, bCoolTimerReset );
    if( rcResult != RC::RC_SKILL_SUCCESS )	return rcResult;

    SkillScriptInfo *pBaseSkillInfo = SkillInfoParser::Instance()->GetSkillInfo( SkillCode );

    // 스킬을 사용할 수 있는 캐릭터인지 체크한다.
    if( !CheckClassDefine( SkillCode, TRUE ) )
    {
        return RC::RC_SKILL_CHAR_CLASS_LIMIT;
    }

    // 쿨타임 처리
    if( bCoolTimerReset && 
        FALSE == CoolTimerReset(SkillCode) )
        return RC::RC_SKILL_COOLTIME_ERROR;

    return RC::RC_SKILL_SUCCESS;
}

bool Summoned::CoolTimerReset(SLOTCODE skill_code)
{
    COOL_TIME_TABLE::const_iterator found = skill_cool_time_table_.find(skill_code);
    if (found == skill_cool_time_table_.end()) {
        return false; //RC::RC_SKILL_COOLTIME_ERROR
    };
    util::Timer* timer = found->second;
    // 스킬 쿨타임 증감 적용
    const int base_cool_time = timer->GetBaseIntervalTime();
    int calculate_cool_time = base_cool_time;
    if(calculate_cool_time)
    {
#ifdef _NA_000000_20140224_SUMMONER_REFACTORING
        Character* const owner = GetSummoner();
        if (owner)
        {
            const Attributes& attributes = owner->GetAttr();
            int bonus_cool_time = attributes.GetItemBonusSkillCoolTime(); // CHANGES: f110603.2L
            float bonus_cool_time_ratio = attributes.GetRatioBonusSkillCoolTime() / 100.f;
            bonus_cool_time_ratio = max(-1.0f, bonus_cool_time_ratio);

            calculate_cool_time = calculate_cool_time + bonus_cool_time;
            calculate_cool_time = max(0, calculate_cool_time);
            calculate_cool_time = (int)(calculate_cool_time * (1 + bonus_cool_time_ratio));

            int bonus_time = calculate_cool_time - base_cool_time;
            timer->SetBonusIntervalTime(bonus_time);
        }
#else
        const Attributes& attributes = m_pSummoner->GetAttr();
        int bonus_cool_time = attributes.GetItemBonusSkillCoolTime(); // CHANGES: f110603.2L
        float bonus_cool_time_ratio = attributes.GetRatioBonusSkillCoolTime() / 100.f;
        bonus_cool_time_ratio = max(-1.0f, bonus_cool_time_ratio);

        calculate_cool_time = calculate_cool_time + bonus_cool_time;
        calculate_cool_time = max(0, calculate_cool_time);
        calculate_cool_time = (int)(calculate_cool_time * (1 + bonus_cool_time_ratio));

        int bonus_time = calculate_cool_time - base_cool_time;
        timer->SetBonusIntervalTime(bonus_time);
#endif //_NA_000000_20140224_SUMMONER_REFACTORING
    }

    timer->Reset();

    return true;
}

Character* Summoned::GetSummoner()
{
#ifdef _NA_000000_20140224_SUMMONER_REFACTORING
    // 소환수 Init부분에선 소환수의 필드가 아직 잡혀있지 않은 상태.
    // 필드가 없을경우에 플레이어 매니저에서 한번 더 찾아보도록 한다.
    GameField* const game_field = GetField();
    if (game_field == NULL) {
        Player* player = PlayerManager::Instance()->FindPlayerByObjKey(GetSummonerKey());

        if (player != NULL) {
            return player;
        }
    }
    else // if (game_field != NULL)
    {
        Character* const owner = game_field->FindCharacter(GetSummonerKey());
        if (obj_util::IsValid(owner, __FUNCTION__, __LINE__) == false) {
            return NULL;
        }

        return owner;
    }

    return NULL;
#else
    if (obj_util::IsValid(m_pSummoner, __FUNCTION__, __LINE__) == false) {
        return NULL;
    }

    return m_pSummoner;
#endif //_NA_000000_20140224_SUMMONER_REFACTORING
}

void Summoned::ApplyPlayerStatusToSummoned( SLOTCODE skill_code, Character* host_char )
{
    const SkillInfoParser* skill_parser = SkillInfoParser::Instance();
    const SkillScriptInfo* skill_info = skill_parser->GetSkillInfo(skill_code);
    const BASE_ABILITYINFO* ability_info
        = skill_info ?  skill_info->GetAbilityInfo(eABILITY_SUMMON_CHANGE_STATUS)
        :               NULL;
    if (ability_info != NULL)
    {   
        //어빌리티 eABILITY_SUMMON_CHANGE_STATUS(221)
        AcquireBasicSummonerStatus(ability_info, host_char);
    }
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    
    ability_info = skill_info ?  skill_info->GetAbilityInfo(eABILITY_SUMMON_CHANGE_STATUS_EXTEND)
                                 : NULL;
    if (ability_info != NULL)
    {
        //어빌리티 eABILITY_SUMMON_CHANGE_STATUS_EXTEND(357)
        AcquireExtendSummonerStatus(ability_info, host_char);
    }
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
    
}

#endif // _NA_006592_VALKYRIES_SUMMONS_SKILL_COOLTIME_BUG

void Summoned::AcquireBasicSummonerStatus( const BASE_ABILITYINFO* ability_info, Character* host_char )
{
    const Attributes::Operation oper = { 0, };
    Attributes& attributes = this->GetAttr();
    Attributes& owner_attributes = host_char->GetAttr();
#ifdef _NA_007264_20140529_BUG_CALCULATE_SUMMONED_ATTRIBUTES
    ;{
        Attributes::OperationRecord records[2] =
        {
            { oper.eOper_Add, eATTR_KIND_BASE,
            eATTR_MAX_HP, host_char->GetMaxHP() },
            { oper.eOper_Add, eATTR_KIND_BASE,
            eATTR_MAX_MP, host_char->GetMaxMP() },
        };
        // 체력(HP)
        if (Attributes::OperationRecord* option = &records[0]) {
            option->attr_value = (option->attr_value * ability_info->m_iOption1 / 1000);
        }
        // 마력(MP)
        if (Attributes::OperationRecord* option = &records[1]) {
            option->attr_value = (option->attr_value * ability_info->m_iOption2 / 1000);
        }
        attributes.GroupBatchOperate(records, _countof(records));
    };
    ;{
        Attributes::OperationRecord records[] =
        {
            { oper.eOper_Add, eATTR_KIND_BASE,
            eATTR_BASE_MELEE_MIN_ATTACK_POWER, owner_attributes.GetBaseMeleeMinAttPower() },
            { oper.eOper_Add, eATTR_KIND_BASE,
            eATTR_BASE_MELEE_MAX_ATTACK_POWER, owner_attributes.GetBaseMeleeMaxAttPower() },

            { oper.eOper_Add, eATTR_KIND_BASE,
            eATTR_BASE_RANGE_MIN_ATTACK_POWER, owner_attributes.GetBaseRangeMinAttPower() },
            { oper.eOper_Add, eATTR_KIND_BASE,
            eATTR_BASE_RANGE_MAX_ATTACK_POWER, owner_attributes.GetBaseRangeMaxAttPower() },
#if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
            { oper.eOper_Add, eATTR_BASE_MAGICAL_MIN_ATTACK_POWER,
            eATTR_KIND_BASE, attributes.GetBaseMagicMinAttPower() },
            { oper.eOper_Add, eATTR_BASE_MAGICAL_MAX_ATTACK_POWER,
            eATTR_KIND_BASE, attributes.GetBaseMagicMaxAttPower() },
#endif
        };

        int param = ability_info->m_iParam[0];
        // 최대/최소 근거리/원거리 공격력
        for (Attributes::OperationRecord* it = records; it != &records[_countof(records)]; ++it) {
            it->attr_value = (it->attr_value * param / 1000);
        }
        attributes.GroupBatchOperate(records, _countof(records));
    };
    ;{
        Attributes::OperationRecord records[] =
        {
            { oper.eOper_Add, eATTR_KIND_BASE,
            eATTR_BASE_MELEE_DEFENSE_POWER, owner_attributes.GetBaseMeleeDefPower() },
            { oper.eOper_Add, eATTR_KIND_BASE,
            eATTR_BASE_RANGE_DEFENSE_POWER, owner_attributes.GetBaseRangeDefPower() },
#if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
            { oper.eOper_Add, eATTR_BASE_MAGICAL_DEFENSE_POWER,
            eATTR_KIND_BASE, attributes.GetBaseMagicDefPower() },
#endif
        };

        int param = ability_info->m_iParam[1];
        // 근거리/원거리 벙어력
        for (Attributes::OperationRecord* it = records; it != &records[_countof(records)]; ++it) {
            it->attr_value = (it->attr_value * param / 1000);
        }
        attributes.GroupBatchOperate(records, _countof(records));
    };
#endif //_NA_007264_20140529_BUG_CALCULATE_SUMMONED_ATTRIBUTES
}

#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
void Summoned::AcquireExtendSummonerStatus( const BASE_ABILITYINFO* ability_info, Character* host_char )
{
    const Attributes::Operation oper = { 0, };
    Attributes& attributes = this->GetAttr();
    Attributes& owner_attributes = host_char->GetAttr();

    ;{
        Attributes::OperationRecord records[1] =
        {
            //스킬공격력
            { oper.eOper_Add, eATTR_KIND_BASE,
            eATTR_ADD_SKILL_ATTACK_POWER, owner_attributes.GetSkillAttackPower() },
        };

        int param = ability_info->m_iOption1;
        records[0].attr_value = (records[0].attr_value * param / 1000);

        attributes.GroupBatchOperate(records, _countof(records));
    };

    ;{
        Attributes::OperationRecord records[] =
        {
            //원소공격력
            { oper.eOper_Add, eATTR_KIND_BASE,
            eATTR_MAGICAL_WATER_ATTACK_POWER, owner_attributes.GetMagicalAttackPower(eATTACK_TYPE_WATER) },
            { oper.eOper_Add, eATTR_KIND_BASE,
            eATTR_MAGICAL_FIRE_ATTACK_POWER, owner_attributes.GetMagicalAttackPower(eATTACK_TYPE_FIRE) },
            { oper.eOper_Add, eATTR_KIND_BASE,
            eATTR_MAGICAL_WIND_ATTACK_POWER, owner_attributes.GetMagicalAttackPower(eATTACK_TYPE_WIND) },
            { oper.eOper_Add, eATTR_KIND_BASE,
            eATTR_MAGICAL_EARTH_ATTACK_POWER, owner_attributes.GetMagicalAttackPower(eATTACK_TYPE_EARTH) },
            { oper.eOper_Add, eATTR_KIND_BASE,
            eATTR_MAGICAL_DARKNESS_ATTACK_POWER, owner_attributes.GetMagicalAttackPower(eATTACK_TYPE_DARKNESS) },
        };

        int param = ability_info->m_iOption2;
        
        for (Attributes::OperationRecord* it = records; it != &records[_countof(records)]; ++it) {
            it->attr_value = (it->attr_value * param / 1000);
        }
        attributes.GroupBatchOperate(records, _countof(records));
    };
    ;{
        Attributes::OperationRecord records[1] =
        {
            //크리티컬 확률
            { oper.eOper_Add, eATTR_KIND_BASE,
            eATTR_CRITICAL_RATIO_CHANGE, owner_attributes.GetCriticalRatioChange() },
        };

        int param = ability_info->m_iParam[0];
        records[0].attr_value = (records[0].attr_value * param / 1000);

        attributes.GroupBatchOperate(records, _countof(records));
    };
    ;{
        Attributes::OperationRecord records[1] =
        {
            //크리티컬 데미지
            { oper.eOper_Add, eATTR_KIND_BASE,
            eATTR_CRITICAL_DAMAGE_CHANGE, owner_attributes.GetCriticalDamageBonus() },
        };

        int param = ability_info->m_iParam[1];
        records[0].attr_value = (records[0].attr_value * param / 1000);

        attributes.GroupBatchOperate(records, _countof(records));
    };
}
#endif //_NA_007667_20141001_WITCHBLADE_SKILL