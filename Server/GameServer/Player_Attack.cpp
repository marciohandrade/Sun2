#include "stdafx.h"

#include <float.h>
#include <PublicMath.h>
#include <ItemFormula.h>
#include <SkillInfoParser.h>

#include "GameZoneCommon.h"
#include "CharacterFormula.h"
#include "Player.h"

#include "StatusManager.h"
#include "ItemManager.h"
#include "PVPInfo.h"

#include "Summoned.h"

#include "ChaoState.h"
#include "DragonTransStatus.h"
#include "WarehouseSlotContainer.h"
#include "StateInfoParser.h"
#include "./PlayerRider.h"
#include "GameZone.h"

//────────────────────────────────────────────────────────────────


BOOL 
Player::CanAttack()
{
	//	(WAVERIX)(080907)(NOTE)
	//	- 이것 어캐 안되나? 그때 그때 이런 식으로 예외처리가 수행되다보니 구조상 또는 유지보수도 힘든
	//	예외들이 아래와 같이 추가되고 있다. 일단, SSQ관련 작업 이전에 로직 처리 순서를 변경해서
	//	디버깅도 더 쉽고, 더 가볍도록 만들기로 한다.

	const BOOL bCanAttack = Character::CanAttack();
	if( !bCanAttack )
		return FALSE;

	//	플레이어의 상태가 기본상태가 아니면( 예:현재 개인상점을 개설한 상태등 ) 공격할 수 없다.
	const ePLAYER_BEHAVE_STATE state_of_behave = GetBehave();
#ifdef _NA_006699_20130506_DONOT_ATTACK_WHILE_BATTLE_GROUND_RESERVATION
    if( state_of_behave != PLAYER_BEHAVE_IDLE_STATE &&
        state_of_behave != PLAYER_BEHAVE_RESERVATION_BATTLE_GROUND_ZONE)
#else
	if( state_of_behave != PLAYER_BEHAVE_IDLE_STATE )
#endif // _NA_006699_20130506_DONOT_ATTACK_WHILE_BATTLE_GROUND_RESERVATION
		return FALSE;

	//	(WAVERIX)(080907)(NOTE)
	//	- 아래와 같은 상황이 쓴웃음이 나오게 하는 내용... 작업 단계가 서로 틀리니 각각 구현해버리는 (그게 더 작업할 때는 편하니까...)
	//	상황에 의한 것이라 생각된다. 좀 더 고민되더라도 앞선 ePLAYER_BEHAVE_STATE와 같은 통합 일괄체계를 갖추는 편이 나을 거라
	//	생각된다. 적어도 작업할 때, 선행작업(분석단계)를 가지고 기존에 있는 기능의 활용방안(또는 확장방안)을 고민해보면 좋을 것이다.
	//	그런 것이 조금 고민스럽더라도 자기 발전에도 이득일 거라 생각된다. 뭐... 작업일정 압박이나 회사니까 또는 잘 돌아가고 있는
	//	것을 건드려서 뭐할려구? 라고 한다면 씁쓸하지만 무시할 수는 없는 부분이고 나 역시 예외일 수는 없으므로 별로 항변하고 싶지 않다.
	//	단지 그런 생각이 만연되는 것이 무서울 뿐이다.

	//	PVP 상태 체크
	const ePVP_USER_STATE state_of_pvp = m_pPVPInfo->GetPVPState();
	if( !( state_of_pvp == ePVP_USER_NONE
		|| state_of_pvp == ePVP_USER_MATCH
		) )
		return FALSE;

	//	(WAVERIX)(080907)(NOTE)
	//	- 상동의 이유로 쓴웃음이 나온다. 그렇다면... 길드 창고 추가된 현 시점에서 길드 창고에 대해서는 검사하지 않아도 되는가
	//	하는 문제가 생긴다. 그게 문제라면 여기에 또 추가할 것인가? 아이러니다...
	//	- 분석적인 문제, pWarehouse 포인터 상태를 검사하는 것은 타당하나 pWarehouse가 null이 되는 상황은
	//	GameServer가 종료될 때 뿐이다. 뭐... 포인터 문제나 메모리 할당 방법을 달리하거나, 하면 별개의 문제가 되기는 하지만, 그런
	//	경우라면, 일괄적으로 해당 플레이어의 Action 허용 여부라는 단순한 평가로 이뤄지는 게 더 간단해 질 것이다.
	//	흔히 하지 않는가? pPlayer->GetField() 검사...

	//	현재 창고가 열려 있는 상태면 공격할 수 없다.
	CWarehouseSlotContainer* const pWareHouse = GetWarehouseContainer();
	if( !pWareHouse || pWareHouse->IsStarted() )	//창고가 열려 있으면.. < (WAVERIX) ㅋㅋ 상태 한 가지가 더 있다. 아래 첨삭했음.
		return FALSE;

	//	(WAVERIX)(080907)(NOTE)
	//		if( !pWareHouse || pWareHouse->IsStarted() )
	//	- 알고 이렇게 했는지 아닌지는 파악할 수 없지만 그 논리 결과에는 재미있는 부분이 있다. 이는 다음과 같이 풀이된다.
	//		if( !pWareHouse || (pWareHouse && pWareHouse->IsStarted()) )
	//		pWareHouse이 0이거나 pWareHouse->IsStarted()이라면... ==> (0이거나 0이 아니고 started라면...)
	//	- 결과적으로 위 논리에서 타당하기는 하지만, CanAttack의 목적과는 다른 부분이 생겨난다.
	//	- 참고로 본 작성자의 경우는 굳이 표현한다면 아래처럼 한다.
	//	if( pWareHouse && pWareHouse->IsStarted() )
	//		위의 상태보다는 조건이 하나 빠져 있다. 이는 pWareHouse가 0이냐 아니냐는 여기서 고려할 대상이 아니기 때문이다. 그런것을
	//		고민하면 프로그램의 복잡도는 예측하기 힘들다. 적어도 pWareHouse를 검사한 단일한 목적(창고가 열려 있으면)에는 부합하게 된다.
	//		창고의 무결성은 창고 관련 처리시에 해결하면 된다.
	//		일단 이와 같은 처리로 다른 곳에 영향을 주었을 가능성도 있고, 창고가 없을 리도 없으니 논리 블록은 그대로 유지한다.
	//
	//	결론적으로 위의 상황을 직설적으로 표현하면 다음과 같다.
	//	"공격은 창고를 가지고 있는 사람만의 특권이다!" ... 아이러니다. 왜?
	return TRUE;
}


//────────────────────────────────────────────────────────────────


BOOL
Player::CanBeAttacked() const
{
	const BOOL bCanBeAttacked = Character::CanBeAttacked();
	if( !bCanBeAttacked )
		return FALSE;

	//	PVP 상태 체크
	const ePVP_USER_STATE state_of_pvp = m_pPVPInfo->GetPVPState();
	if( !( state_of_pvp == ePVP_USER_NONE
		|| state_of_pvp == ePVP_USER_MATCH
		) )
		return FALSE;

	return TRUE;
}


//────────────────────────────────────────────────────────────────

// CHANGES, f100830.4L, replace an arguement block for a skill to a SkillInfo
RC::eBATTLE_RESULT Player::CanNormalAttack(SkillInfo* preparing_skill_info_data)
{
    const SkillInfo& skill_info_data = *preparing_skill_info_data; // safe-control
    //  (WAVERIX)(080907)(NOTE) 너무 복잡해졌으므로 현 시점 적용되는 기능을 추출한다.
    //////////////////////////////////////////////////////////////////////////
    //  <LOGIC CONTROL>
    FlowControl flow;
    FLOWCONTROL_START()
    {
        // 플레이어가 필드에 없으면 리턴
        GameField* const pField = GetField();
        if (!(pField && pField->IsReady())) {
            return RC::RC_BATTLE_PLAYER_NOTEXIST_TO_FIELD;
        }
        //  (WAVERIX)(080907)(NOTE) ... 이 상태 찾기 (특히 eCHAR_STATE_TRANSFORMATION)나중에 개선 방안 강구하자.
        StatusManager* const status_manager = GetStatusManager();

        //  공격 타수 체크(일단 틀려도 넘어가 준다...) ... 그런데 왜?
        StatusField& rStatus = status_manager->Status;
        const bool isCheck_AtkSeq = !(rStatus.IsDragonTransforming());

        if (isCheck_AtkSeq)
        {
            const BYTE last_atkseq = GetLastAttackSeq();
            if (eATTACK_SEQUENCE_SECOND == skill_info_data.attack_sequence_)
            {
                if (last_atkseq != eATTACK_SEQUENCE_FIRST)
                {
                    SECURELOG(SECURELOG_PLAYERINFO(this, \
                        eABUSE_LOGIC_VIOLATION, eABUSE_SUB_LOGIC_VIOLATION_ATTACK_SEQUENCE),
                        _T("["__FUNCTION__"] Attack Logic Violation, received 2nd, except 1st"));
                }
            }
            else if (eATTACK_SEQUENCE_THIRD == skill_info_data.attack_sequence_)
            {
                if (last_atkseq != eATTACK_SEQUENCE_SECOND)
                {
                    SECURELOG(SECURELOG_PLAYERINFO(this, \
                        eABUSE_LOGIC_VIOLATION, eABUSE_SUB_LOGIC_VIOLATION_ATTACK_SEQUENCE),
                        _T("["__FUNCTION__"] Attack Logic Violation, received 3rd, except 2nd"));
                }
            }
        }
        // sequence update는 field check 이전에 해줘야 하는가? 좀더 따져볼 필요는 있다.
        SetLastAttackSeq(skill_info_data.attack_sequence_);

        //  플레이어가 공격할 수 없는 상태이면 리턴 < atk seq는 맞춰줘야 한다.
        if (!CanAttack()) {
            return RC::RC_BATTLE_PLAYER_STATE_WHERE_CANNOT_ATTACK_ENEMY;
        }
        if (!rStatus.IsPassConstraint(this, (SkillScriptInfo*)0)) {
            return RC::RC_BATTLE_PLAYER_STATE_WHERE_CANNOT_ATTACK_ENEMY;
        }
        //  아직 액션 도중인 경우 이 패킷은 해킹으로 간주해도 무방함
        IfFailThenBreak(!IsDoingAction(), RC::RC_BATTLE_ALREADY_DOING_ACTION);

        //  현재 좌표가 유효한지 체크
        IfFailThenBreak(WzVector_FiniteCheck(&skill_info_data.cur_pos_), RC::RC_BATTLE_INVLIDPOS);
        //  목적지 좌표가 유효한지 체크
        IfFailThenBreak(WzVector_FiniteCheck(&skill_info_data.dest_pos_), RC::RC_BATTLE_INVLIDPOS);

        //  (WAVERIX)(080907)(NOTE) 이후 딜레이 처리가 고려되면 이 문제도 좀 더 고민해봐야 한다.
        //  밀리는 거리(어택시작점-어택끝점)가 너무 길면 접속을 끊는다.
        const float fDiffBetween_Cur_With_Dest_Q2 = 3.0f * 3.0f;
        ;{
            WzVector diff = skill_info_data.cur_pos_;
            ;        diff -= skill_info_data.dest_pos_;
            IfFailThenBreak(Math_Vector2DLength2(&diff) <= fDiffBetween_Cur_With_Dest_Q2,
                            RC::RC_BATTLE_THRUST_DIST_OVER);
        };

        // (NOTE) to support a shadow control, __NA_S00015_20080828_NEW_HOPE_SHADOW
        ;{
            // 공격시 사라지는 상태들......
            Player_TransOff();  //_NA001385_20090924_DOMINATION_SHADOW
#if !defined(_NA_005159_20120807_DISABLE_REMOVE_RUSH_STATE)
            //러쉬상태이면 사라진다.
            BaseStatus* status = status_manager->FindStatus(eCHAR_STATE_RUSH);
            if (status) {
                status->StopStatus();
            }
#endif //_NA_005159_20120807_DISABLE_REMOVE_RUSH_STATE
        };
        return RC::RC_BATTLE_SUCCESS;

    }
    FLOWCONTROL_END;

    //////////////////////////////////////////////////////////////////////////
    //  <ERROR CONTROL>
    switch (flow.GetErrorCode())
    {
    case RC::RC_BATTLE_INVLIDPOS:
        {
            const WzVector& cur_pos = skill_info_data.cur_pos_;
            const WzVector& dest_pos = skill_info_data.dest_pos_;
            SECURELOG(SECURELOG_PLAYERINFO(this, \
                eABUSE_LOGIC_VIOLATION, eABUSE_SUB_LOGIC_VIOLATION_INVALID_POSITION),
                _T("["__FUNCTION__"] Invalid Position! ")
                _T("CurPos={%.2f,%.2f,%.2f} DestPos={%.2f,%.2f,%.2f}"),
                cur_pos.x, cur_pos.y, cur_pos.z, dest_pos.x, dest_pos.y, dest_pos.z);
        }
        return RC::RC_BATTLE_INVLIDPOS;
    case RC::RC_BATTLE_THRUST_DIST_OVER:
        {
            SECURELOG(SECURELOG_PLAYERINFO(this, \
                eABUSE_LOGIC_VIOLATION, eABUSE_SUB_LOGIC_VIOLATION_ATTACK_RANGE),
                _T("["__FUNCTION__"] Coordinates Hack, End to End Attck Distance is too long"));
        }
        return RC::RC_BATTLE_THRUST_DIST_OVER;
    case RC::RC_BATTLE_ALREADY_DOING_ACTION:
        {
            SECURELOG(SECURELOG_PLAYERINFO(this, \
                eABUSE_LOGIC_VIOLATION, eABUSE_SUB_LOGIC_VIOLATION_ATTACK_SPEED),
                _T("["__FUNCTION__"] Already Doing Action! TimeInterval [%d] ProgressRatio [%f]"),
                m_ActionTimer.GetIntervalTime() , m_ActionTimer.GetProgressRatio());
        }
        return RC::RC_BATTLE_ALREADY_DOING_ACTION;
    }

    SUNLOG(eCRITICAL_LOG, _T("["__FUNCTION__"] 스킵이 발생하는 상황은 무엇인가?|Error=%d|"),
           flow.GetErrorCode());

    return RC::RC_BATTLE_FAILED;
}


//────────────────────────────────────────────────────────────────

// CHANGES, f100830.4L, replace an arguement block for a skill to a SkillInfo
RC::eBATTLE_RESULT Player::CanStyleAttack(SkillInfo* preparing_skill_info_data)
{
    // for safe-control. if you want modify a SkillInfo, do access to preparing_skill_info_data.
    const SkillInfo& skill_info_data = *preparing_skill_info_data;
    //////////////////////////////////////////////////////////////////////////
    //  <LOGIC CONTROL>
    FlowControl flow;
    FLOWCONTROL_START()
    {
        // 공격 가능한지 기본적인 사항을 체크한다.
        const RC::eBATTLE_RESULT result = CanNormalAttack(preparing_skill_info_data);
        if (result != RC::RC_BATTLE_SUCCESS) {
            return result;
        };
        const SkillInfoParser* const skill_parser = SkillInfoParser::Instance();
        const SLOTCODE style_code = GetSelectedStyle();
        const BASE_STYLEINFO* const style_info = skill_parser->GetStyleInfo(style_code);
        if (style_info == NULL) {
            return RC::RC_BATTLE_BASEINFO_NOTEXIST; // 이거야 말로 abusing이 아닌가?
        }
        StatusManager* const status_manager = GetStatusManager();
        //  (WAVERIX)(080907)(NOTE)
        //  - ... 또 드래곤변신이다... -.-;;;
        //  - 클래스 타입 관련은 특정 위치에서 해결이 가능하다. 그쪽을 다시 한번 살펴보고 개선 방안을 강구하자.

        //  클래스 제한에 걸리는지 체크
        DWORD current_class_type = (DWORD)GetCharType();    // eCHAR_TYPE | eCHAREX_TYPE
        //StatusField& rStatus = pStatusManager->Status;
        if (status_manager->Status.IsDragonTransforming())
        {
            DragonTransStatus* const status = static_cast<DragonTransStatus*>(\
                status_manager->FindStatus(eCHAR_STATE_TRANSFORMATION));
            current_class_type = (DWORD)status->GetCharType();
        }

        IfFailThenBreak((DWORD)style_info->m_byClassDefine == current_class_type,
                        RC::RC_BATTLE_CHAR_CLASS_LIMIT);

        //  무기제한에 걸리는지 체크
        const DWORD weapon_kind_by_script = (DWORD)style_info->m_iWeaponDefine;
        if (weapon_kind_by_script != ULONG_MAX)
        {
            const DWORD weapon_kind = GetWeaponKind();
            IfFailThenBreak(weapon_kind == weapon_kind_by_script, RC::RC_BATTLE_WEAPON_LIMIT);
        }
        ;{
            // CHANGES, f100830.5L, update a skill_info_data.
            preparing_skill_info_data->skill_code_ = style_code;
            preparing_skill_info_data->root_skill_info_ = style_info;
        };
        //
        return RC::RC_BATTLE_SUCCESS;

    }
    FLOWCONTROL_END;

    //////////////////////////////////////////////////////////////////////////
    //  <ERROR CONTROL>
    switch (const RC::eBATTLE_RESULT result = (RC::eBATTLE_RESULT)flow.GetErrorCode())
    {
    case RC::RC_BATTLE_CHAR_CLASS_LIMIT:
        {
            SECURELOG(SECURELOG_PLAYERINFO(this,
                eABUSE_LOGIC_VIOLATION, eABUSE_SUB_LOGIC_VIOLATION_ATTACK_TYPE),
                _T("["__FUNCTION__"] The Type Of Character is Invalid! Style[%d] CharType[%d]"),
                GetSelectedStyle(), GetCharType());  // 드래곤 변신은 고려되지 않은 로그다...
        }
        return result;
    case RC::RC_BATTLE_WEAPON_LIMIT:
        {
            SECURELOG(SECURELOG_PLAYERINFO(this, \
                eABUSE_LOGIC_VIOLATION, eABUSE_SUB_LOGIC_VIOLATION_ATTACK_TYPE),
                _T("["__FUNCTION__"] The Weapon Type is Invalid! Style[%d] Weapon[%d]"),
                GetSelectedStyle(), GetWeaponKind());
        }
        return result;
    }

    return RC::RC_BATTLE_FAILED;
}


//────────────────────────────────────────────────────────────────


VOID
Player::SetAttackDelay( BYTE byAttackSequence, SLOTCODE StyleCode )
{
	BASE_STYLEINFO *pBaseStyleInfo = SkillInfoParser::Instance()->GetStyleInfo( StyleCode );

	// 액션 딜레이 설정
	DWORD dwActionDelay;
	switch( byAttackSequence )
	{
	case eATTACK_SEQUENCE_FIRST:
		dwActionDelay = pBaseStyleInfo->m_wTimeFirst;
		break;
	case eATTACK_SEQUENCE_SECOND:
		dwActionDelay = pBaseStyleInfo->m_wTimeSecond;
		break;
	case eATTACK_SEQUENCE_THIRD:
		dwActionDelay = pBaseStyleInfo->m_wTimeThird;
		break;
	default:

		SECURELOG( SECURELOG_PLAYERINFO(this, eABUSE_LOGIC_VIOLATION, eABUSE_SUB_LOGIC_VIOLATION_ATTACK_SEQUENCE),
			_T("[Player::SetAttackDelay] Invalid attack sequence! Style[%d] byAttackSequence[%d]"), StyleCode, byAttackSequence );
		ForceDisconnect( RC::RC_DISCONNECT_INVALID_ATTACK_SEQUENCE );
		return;
	}

	// 서버측 액션 딜레이를 약간 여유있게 잡는다.
	dwActionDelay = (DWORD)(dwActionDelay * 0.9f);

	if( 0 != GetPhysicalAttackSpeed() )
	{
		SetActionDelay( (DWORD)( dwActionDelay / GetPhysicalAttackSpeed() ) );
	}
	else
	{
		SUNLOG( eCRITICAL_LOG, _T("[Player::SetAttackDelay] GetPhysicalAttackSpeed() = 0!") );
		return;
	}
}


//────────────────────────────────────────────────────────────────


VOID
Player::OnAttack( Character *pTarget, SLOTCODE skillcode, DAMAGETYPE damage )
{
	// 공격자의 무기 내구도 감소
	//밀리공격이면 명중률 적용 하여 내구도 감소
	DWORD dwDecreaseDuraVal = CalcDecreaseItemDuraValue( m_Attr.GetDecreaseItemDuraRatio() );
    bool apply_decrease_durability = false;
    if (skillcode == SKILLCODE_NORMAL_ATTACK) {
        //명중했으니 내구도 감소
        if (damage != 0) {
            apply_decrease_durability = true;
        }
    }
    //마법공격이면 명중률 적용 없이 내구도 감소
    else
    {
        apply_decrease_durability = true;
    };
    // CHANGES: f110405.1L
    if (apply_decrease_durability) {
        if (nsSlot::DurabilityImpl* const impl = this->GetItemManager()->GetDurabilityImpl()) {
            impl->DecreaseDurability(dwDecreaseDuraVal, eEQUIPCONTAINER_WEAPON);
        };
    };

	Character::OnAttack( pTarget, skillcode, damage );

	DWORD skillRecoverPercent = 100;
	DWORD skillRequireMP = 0;

	SkillScriptInfo *pSkillInfo = SkillInfoParser::Instance()->GetSkillInfo( skillcode );
	if( pSkillInfo )
	{
		skillRequireMP = pSkillInfo->m_wMPSpend;
	}

	// 공격할때 회복량
	DWORD recoverMP = CalcMPRecoverByAttack( GetCharType(), m_Attr.GetSPR(), skillRecoverPercent, skillRequireMP );

	OnRecover( 0, recoverMP );

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    if (GetCharType() == eCHAR_WITCHBLADE && damage > 0) // 평타로 피해를 주면 적용
    {
        PLAYERFP recoverFP = CalcFPRecoverByAttack(GetCharType());
        OnRecoverFP(recoverFP);
    }
    
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
}


//────────────────────────────────────────────────────────────────


void
Player::_Damaged(DamageArgs* damage_arg)
{
    Character* pAttacker = damage_arg->actor_;

    // (WAVERIX) (081013) (NOTE) 너무 길고 중복이 많다. 디파인 제거 작업이 끝나면 정리하기로 한다.
    if (Policyment.FIELD & Policyment.PCC_IN_SSQ_FIELD)
    {
        int damage = damage_arg->damage_;
        if (damage) {
            damage = (damage >> 1);
            if (damage == 0) {
                damage = 1;
            }
            damage_arg->damage_ = damage;
        }
        /*
        // (WAVERIX) (090824) (CHANGES) 3항 연산은 2회 중복 조건을 true 계산 방식으로 풀어내면
        // 난해해지는 문제점때문에 다음과 같이 수정.
        if(damage) {
            damage = (damage >> 1) ? damage >> 1
                                   : 1;
        }
        // (NOTE) 앞에서 말한 내용을 다음과 같이 변환하면 좀 더 가독성이 좋아진다.
        // 하지만 이럴 필요 있나? 상수화 목적이 아닌 이상 이렇게 무리하게 하지는 말자.
        // (advanced)                           | (original code)
        //damage = (damage == 0) ?  0           | damage = damage
        //       : (damage >> 1) ?  damage >> 1 |        ? (damage >> 1) ? damage >> 1
        //       :                  1;          |                        : 1
        //                                      |        : 0;
        */
    }

    //             || 이미 죽은 유저에게 데미지를 주어도 소용없다... 
    if (damage_arg->damage_ == 0 || IsDead()) {
        return;
    }

    if (damage_arg->damage_) // ... 이 앞단으로 추가 로직이 삽입되는 경우를 감안해서 제거하지 않는다.
    {
#if !defined(_NA_000000_20121015_DISABLE_REMOVE_HIDE_STATE)
        Player_TransOff();
#endif //_NA_000000_20121015_DISABLE_REMOVE_HIDE_STATE

        // 방어자의 방어구 내구도 감소
        //POSTYPE RandomPos = random(eEQUIPCONTAINER_ARMOR, eEQUIPCONTAINER_SHIRTS); // unused
        // (CHANGES) (091212) (WAVERIX) to support simple debugging facility, \
        // more reduce overhead and code arrangement
        const int iDuraRatio  = m_Attr.GetDecreaseItemDuraRatio();
        const DWORD duraVal = CalcDecreaseItemDuraValue(iDuraRatio);
        // CHANGES: f110404.2L
        if (nsSlot::DurabilityImpl* const impl = this->GetItemManager()->GetDurabilityImpl()) {
            impl->DecreaseDurabilityWithRanges(duraVal,
                eEQUIPCONTAINER_ARMOR, eEQUIPCONTAINER_SHIRTS);
        };
    }

    { // Recover MP
        DWORD recoverMP = CalcMPRecoverByAttacked(GetCharType(), m_Attr.GetSPR(), damage_arg->damage_);
        OnRecover(0, recoverMP);
    };

    // Character Damage Process
    Character::_Damaged(damage_arg); //여기서 실제 데미지 감소가 일어난다.

    if(damage_arg->damage_ > 0 && IsAlive())
    {   
        // (WAVERIX) (090821) (BUG-FIX) HP Recovery기능 관련 버그 수정
        // (해당 회복률은 최종 계산 결과 값이 아닌 비율)만으로 사용해야 할 듯, HP의 값은 Attribute에
        // 변동치를 반영할 수 없으며, (설사 반영하더라도 복잡해지거나, 매번 update수행해야 한다.)
        // 결과적으로 현 시점, value(0) * ratio로 연산되어 항상 0이 나오게 된다.
        // (Addin Note)
        // 초기 구현자와 기능 추가자가 동일하지만, 이 사람은 테스트는 개뿔로 생각하는 사람이라
        // 단정지어야(거짓은 아니니 뭐...) 이후 해당 작업자가 발생시킨 (현재 스킬 관련 버그들
        // 내가 가지고 있는) 버그들을 수정하면서 스트레스 덜 받을 것 같다. 현재 위도 아파오는데... ㅆx!

        // 리커버리량 증가 패시브 스킬이 있으면..
        int reserveHp_Addin = (damage_arg->damage_ >> 1);
        if(reserveHp_Addin) {              //'+Ratio' (ADD-IN)
            int reserveHp_Ratio = m_Attr.GetRatioIncreaseReserveHPRatio();
            if(reserveHp_Ratio)  // = (src * percent) / 100
                reserveHp_Addin = int((reserveHp_Addin * (100 + reserveHp_Ratio)) / 100.0f);
        }

        DWORD reserveHP = GetReserveHP() + reserveHp_Addin;

        //// old code backup
        //int nReserveHPRatio = GetAttr().GetIncreaseReserveHPRatio(); // 리커버리량 증가 패시브 스킬이 있으면..
        //double dbTempReserveHP = (double)damage / 2;
        //double dbCalcReserveHPRatio = (double)(nReserveHPRatio/100.0f)*dbTempReserveHP;
        //
        //DWORD reserveHP = GetReserveHP() + dbTempReserveHP + dbCalcReserveHPRatio;

        {// (WAVERIX) (090821) (CHANGES) 코드 검증 후 수정
            DWORD calcHP = GetHP() + reserveHP;
            const DWORD maxHP = GetMaxHP();
            // (NOTE) curHP + reserveHP > maxHP 상황이 발생하지만, 실제 회복 시점에 잉여 값이 고려된다.
            if(maxHP < calcHP)
                reserveHP = calcHP - maxHP;
        }

        SetReserveHP(reserveHP);
    }

    if(pAttacker)
    {
        if(IsDead()) // 플레이어가 강제공격(PK)을 받아서 죽었을 경우
        {
            Player* pPlayer = pAttacker->GetPlayerOwner();
            if(pPlayer) 
            {
#ifdef _NA_008706_20160229_TEXTLOG_PK 
                pPlayer->KillPlayer(this, FALSE, damage_arg->damage_first_);
#else
                pPlayer->KillPlayer(this);
#endif //_NA_008706_20160229_TEXTLOG_PK
            }
        }
        else if(damage_arg->option_.is_mirror_ == false && GetReflectDamageRatio())
        {
            // f100908.1L
            DamageArgs mirror_damage_args(this, damage_arg->attack_type_);
            ;{
                //mirror_damage_args.option_.is_normal_;
                //mirror_damage_args.option_.is_critical_;
                mirror_damage_args.option_.is_mirror_ = true;
                //mirror_damage_args.target_;
                //mirror_damage_args.limit_hp_;
                mirror_damage_args.attack_regist_kind_ = damage_arg->attack_regist_kind_;

                // 이 부분은 옵션에 대한 처리 이나, 상태의 SDApply 값을 따른다.
                BASE_STATEINFO* pStateInfo = \
                    StateInfoParser::Instance()->GetStateInfo(eCHAR_STATE_REFLECT_DAMAGE);
                if (pStateInfo) {
                    mirror_damage_args.sd_apply_ = (eSD_APPLY)pStateInfo->m_bySDApply;
                }
                mirror_damage_args.damage_ = damage_arg->damage_ * GetReflectDamageRatio();
                //mirror_damage_args.damage_sd_;
            };

            pAttacker->Damaged(&mirror_damage_args); // 데미지 반사 속성 체크
            DWORD dwAttackerReserveHP = pAttacker->GetReserveHP();
            if(dwAttackerReserveHP > 0)
                dwAttackerReserveHP = dwAttackerReserveHP / 10 + mirror_damage_args.damage_ / 2; //기본의 적립HP 10% + Damage의 50%를 적립한다.
            else
                dwAttackerReserveHP = mirror_damage_args.damage_ / 2; //기본의 적립HP 10% + Damage의 50%를 적립한다.

            pAttacker->SetReserveHP(dwAttackerReserveHP);

            //반사 데미지는 내가 공격 받았는데, 반사적으로 나를 공격한 놈에게 데미지를 주는 것이다.
            //따라서 공격 주체가 "내"가 된다.
            ;{
                MSGSUB_SKILL_REFLECT_DAMAGE_BRD msg_brd;
                msg_brd.Clear();

                msg_brd.m_dwAttackerKey = GetObjectKey();
                msg_brd.m_dwTargetKey = pAttacker->GetObjectKey();
                msg_brd.m_wDamage = mirror_damage_args.damage_;
                msg_brd.m_dwTargetHP = pAttacker->GetHP();
                msg_brd.m_dwTargetResrveHP = pAttacker->GetReserveHP();
                msg_brd.m_SkillCode = 0;
                msg_brd.m_wAbilityCode = 0;

                this->SendPacketAroundForSync(&msg_brd);
            };
        }
    }
#if !defined(_NA_005159_20120807_DISABLE_REMOVE_RUSH_STATE)
    ;{  // (NOTE) to support a shadow control, __NA_S00015_20080828_NEW_HOPE_SHADOW
        // 플레이어가 공격 당했을때 사라지는 상태들..
        //러쉬상태이면 사라진다.
        BaseStatus* pStatus = GetStatusManager()->FindStatus(eCHAR_STATE_RUSH);
        if (pStatus) {
            pStatus->StopStatus();
        }
    };
#endif //_NA_005159_20120807_DISABLE_REMOVE_RUSH_STATE
    IVehicleEvent::OnEvent(this, IVehicleEvent::Hit()); // for vehicles 'Hit' event processing

#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    //위치블레이드, 라이징포스의 처리
    ProcessRisingForce();
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
}

//────────────────────────────────────────────────────────────────


VOID
Player::ForcedAttack( Character *pTarget )
{
	if( !pTarget ) return;

	if( !pTarget || !pTarget->GetPlayerOwner() )	return;

	const BOOLEAN bApplyGeneralForcedAtkRule = !( Policyment.FIELD & Policyment.PCC_IN_SSQ_FIELD );
	if( !bApplyGeneralForcedAtkRule )
		return;

 #if defined(_CHINA) //CHINA_PK_RULE
	if( m_ForceAttack || pTarget->GetPlayerOwner()->GetChaoState().IsChaoState() )
 #else
	if( m_ForceAttack )
 #endif
		m_pChaoState->StartBoradoriState( pTarget->GetPlayerOwner() );
}


//────────────────────────────────────────────────────────────────


BOOL
Player::CanResurrection( Character *pTarget )
{
	if( !pTarget )
		return FALSE;

	//파티원이 아니면 자신만 부활
	if( !GetPartyState().IsPartyMember() )
	{
		if( GetObjectKey() == pTarget->GetObjectKey() )
			return TRUE;

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
        GameZone* pZone = GetGameZonePtr();
        if (pZone && pZone->GetZoneType() == eZONETYPE_DOMINATION_FIELD)
        {
            if (pTarget->IsEqualObjectKind(PLAYER_OBJECT)) 
            {
                Player* target_player = static_cast<Player*>(pTarget);
                if (pZone->IsFriendPVP(this, target_player) == USER_RELATION_FRIEND) {
                    return TRUE;
                }
            }
        }
#endif //_NA_006826_20130722_DOMINATION_RENEWAL

        return FALSE;
	}
	//파티원이면 파티원들 부활
	else
	{
		if( GetPartyState().GetPartyKey() != pTarget->GetPartyState().GetPartyKey() )
			return FALSE;
	}
	return TRUE;
}






























