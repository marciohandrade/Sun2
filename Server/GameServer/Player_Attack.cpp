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

//��������������������������������������������������������������������������������������������������������������������������������


BOOL 
Player::CanAttack()
{
	//	(WAVERIX)(080907)(NOTE)
	//	- �̰� ��ĳ �ȵǳ�? �׶� �׶� �̷� ������ ����ó���� ����Ǵٺ��� ������ �Ǵ� ���������� ����
	//	���ܵ��� �Ʒ��� ���� �߰��ǰ� �ִ�. �ϴ�, SSQ���� �۾� ������ ���� ó�� ������ �����ؼ�
	//	����뵵 �� ����, �� �������� ������ �Ѵ�.

	const BOOL bCanAttack = Character::CanAttack();
	if( !bCanAttack )
		return FALSE;

	//	�÷��̾��� ���°� �⺻���°� �ƴϸ�( ��:���� ���λ����� ������ ���µ� ) ������ �� ����.
	const ePLAYER_BEHAVE_STATE state_of_behave = GetBehave();
#ifdef _NA_006699_20130506_DONOT_ATTACK_WHILE_BATTLE_GROUND_RESERVATION
    if( state_of_behave != PLAYER_BEHAVE_IDLE_STATE &&
        state_of_behave != PLAYER_BEHAVE_RESERVATION_BATTLE_GROUND_ZONE)
#else
	if( state_of_behave != PLAYER_BEHAVE_IDLE_STATE )
#endif // _NA_006699_20130506_DONOT_ATTACK_WHILE_BATTLE_GROUND_RESERVATION
		return FALSE;

	//	(WAVERIX)(080907)(NOTE)
	//	- �Ʒ��� ���� ��Ȳ�� �������� ������ �ϴ� ����... �۾� �ܰ谡 ���� Ʋ���� ���� �����ع����� (�װ� �� �۾��� ���� ���ϴϱ�...)
	//	��Ȳ�� ���� ���̶� �����ȴ�. �� �� ��εǴ��� �ռ� ePLAYER_BEHAVE_STATE�� ���� ���� �ϰ�ü�踦 ���ߴ� ���� ���� �Ŷ�
	//	�����ȴ�. ��� �۾��� ��, �����۾�(�м��ܰ�)�� ������ ������ �ִ� ����� Ȱ����(�Ǵ� Ȯ����)�� ����غ��� ���� ���̴�.
	//	�׷� ���� ���� ��ν������� �ڱ� �������� �̵��� �Ŷ� �����ȴ�. ��... �۾����� �й��̳� ȸ��ϱ� �Ǵ� �� ���ư��� �ִ�
	//	���� �ǵ���� ���ҷ���? ��� �Ѵٸ� ���������� ������ ���� ���� �κ��̰� �� ���� ������ ���� �����Ƿ� ���� �׺��ϰ� ���� �ʴ�.
	//	���� �׷� ������ �����Ǵ� ���� ������ ���̴�.

	//	PVP ���� üũ
	const ePVP_USER_STATE state_of_pvp = m_pPVPInfo->GetPVPState();
	if( !( state_of_pvp == ePVP_USER_NONE
		|| state_of_pvp == ePVP_USER_MATCH
		) )
		return FALSE;

	//	(WAVERIX)(080907)(NOTE)
	//	- ���� ������ �������� ���´�. �׷��ٸ�... ��� â�� �߰��� �� �������� ��� â�� ���ؼ��� �˻����� �ʾƵ� �Ǵ°�
	//	�ϴ� ������ �����. �װ� ������� ���⿡ �� �߰��� ���ΰ�? ���̷��ϴ�...
	//	- �м����� ����, pWarehouse ������ ���¸� �˻��ϴ� ���� Ÿ���ϳ� pWarehouse�� null�� �Ǵ� ��Ȳ��
	//	GameServer�� ����� �� ���̴�. ��... ������ ������ �޸� �Ҵ� ����� �޸��ϰų�, �ϸ� ������ ������ �Ǳ�� ������, �׷�
	//	�����, �ϰ������� �ش� �÷��̾��� Action ��� ���ζ�� �ܼ��� �򰡷� �̷����� �� �� ������ �� ���̴�.
	//	���� ���� �ʴ°�? pPlayer->GetField() �˻�...

	//	���� â�� ���� �ִ� ���¸� ������ �� ����.
	CWarehouseSlotContainer* const pWareHouse = GetWarehouseContainer();
	if( !pWareHouse || pWareHouse->IsStarted() )	//â�� ���� ������.. < (WAVERIX) ���� ���� �� ������ �� �ִ�. �Ʒ� ÷������.
		return FALSE;

	//	(WAVERIX)(080907)(NOTE)
	//		if( !pWareHouse || pWareHouse->IsStarted() )
	//	- �˰� �̷��� �ߴ��� �ƴ����� �ľ��� �� ������ �� �� ������� ����ִ� �κ��� �ִ�. �̴� ������ ���� Ǯ�̵ȴ�.
	//		if( !pWareHouse || (pWareHouse && pWareHouse->IsStarted()) )
	//		pWareHouse�� 0�̰ų� pWareHouse->IsStarted()�̶��... ==> (0�̰ų� 0�� �ƴϰ� started���...)
	//	- ��������� �� ������ Ÿ���ϱ�� ������, CanAttack�� �������� �ٸ� �κ��� ���ܳ���.
	//	- ����� �� �ۼ����� ���� ���� ǥ���Ѵٸ� �Ʒ�ó�� �Ѵ�.
	//	if( pWareHouse && pWareHouse->IsStarted() )
	//		���� ���º��ٴ� ������ �ϳ� ���� �ִ�. �̴� pWareHouse�� 0�̳� �ƴϳĴ� ���⼭ ����� ����� �ƴϱ� �����̴�. �׷�����
	//		����ϸ� ���α׷��� ���⵵�� �����ϱ� �����. ��� pWareHouse�� �˻��� ������ ����(â�� ���� ������)���� �����ϰ� �ȴ�.
	//		â���� ���Ἲ�� â�� ���� ó���ÿ� �ذ��ϸ� �ȴ�.
	//		�ϴ� �̿� ���� ó���� �ٸ� ���� ������ �־��� ���ɼ��� �ְ�, â�� ���� ���� ������ �� ����� �״�� �����Ѵ�.
	//
	//	��������� ���� ��Ȳ�� ���������� ǥ���ϸ� ������ ����.
	//	"������ â�� ������ �ִ� ������� Ư���̴�!" ... ���̷��ϴ�. ��?
	return TRUE;
}


//��������������������������������������������������������������������������������������������������������������������������������


BOOL
Player::CanBeAttacked() const
{
	const BOOL bCanBeAttacked = Character::CanBeAttacked();
	if( !bCanBeAttacked )
		return FALSE;

	//	PVP ���� üũ
	const ePVP_USER_STATE state_of_pvp = m_pPVPInfo->GetPVPState();
	if( !( state_of_pvp == ePVP_USER_NONE
		|| state_of_pvp == ePVP_USER_MATCH
		) )
		return FALSE;

	return TRUE;
}


//��������������������������������������������������������������������������������������������������������������������������������

// CHANGES, f100830.4L, replace an arguement block for a skill to a SkillInfo
RC::eBATTLE_RESULT Player::CanNormalAttack(SkillInfo* preparing_skill_info_data)
{
    const SkillInfo& skill_info_data = *preparing_skill_info_data; // safe-control
    //  (WAVERIX)(080907)(NOTE) �ʹ� �����������Ƿ� �� ���� ����Ǵ� ����� �����Ѵ�.
    //////////////////////////////////////////////////////////////////////////
    //  <LOGIC CONTROL>
    FlowControl flow;
    FLOWCONTROL_START()
    {
        // �÷��̾ �ʵ忡 ������ ����
        GameField* const pField = GetField();
        if (!(pField && pField->IsReady())) {
            return RC::RC_BATTLE_PLAYER_NOTEXIST_TO_FIELD;
        }
        //  (WAVERIX)(080907)(NOTE) ... �� ���� ã�� (Ư�� eCHAR_STATE_TRANSFORMATION)���߿� ���� ��� ��������.
        StatusManager* const status_manager = GetStatusManager();

        //  ���� Ÿ�� üũ(�ϴ� Ʋ���� �Ѿ �ش�...) ... �׷��� ��?
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
        // sequence update�� field check ������ ����� �ϴ°�? ���� ������ �ʿ�� �ִ�.
        SetLastAttackSeq(skill_info_data.attack_sequence_);

        //  �÷��̾ ������ �� ���� �����̸� ���� < atk seq�� ������� �Ѵ�.
        if (!CanAttack()) {
            return RC::RC_BATTLE_PLAYER_STATE_WHERE_CANNOT_ATTACK_ENEMY;
        }
        if (!rStatus.IsPassConstraint(this, (SkillScriptInfo*)0)) {
            return RC::RC_BATTLE_PLAYER_STATE_WHERE_CANNOT_ATTACK_ENEMY;
        }
        //  ���� �׼� ������ ��� �� ��Ŷ�� ��ŷ���� �����ص� ������
        IfFailThenBreak(!IsDoingAction(), RC::RC_BATTLE_ALREADY_DOING_ACTION);

        //  ���� ��ǥ�� ��ȿ���� üũ
        IfFailThenBreak(WzVector_FiniteCheck(&skill_info_data.cur_pos_), RC::RC_BATTLE_INVLIDPOS);
        //  ������ ��ǥ�� ��ȿ���� üũ
        IfFailThenBreak(WzVector_FiniteCheck(&skill_info_data.dest_pos_), RC::RC_BATTLE_INVLIDPOS);

        //  (WAVERIX)(080907)(NOTE) ���� ������ ó���� ����Ǹ� �� ������ �� �� ����غ��� �Ѵ�.
        //  �и��� �Ÿ�(���ý�����-���ó���)�� �ʹ� ��� ������ ���´�.
        const float fDiffBetween_Cur_With_Dest_Q2 = 3.0f * 3.0f;
        ;{
            WzVector diff = skill_info_data.cur_pos_;
            ;        diff -= skill_info_data.dest_pos_;
            IfFailThenBreak(Math_Vector2DLength2(&diff) <= fDiffBetween_Cur_With_Dest_Q2,
                            RC::RC_BATTLE_THRUST_DIST_OVER);
        };

        // (NOTE) to support a shadow control, __NA_S00015_20080828_NEW_HOPE_SHADOW
        ;{
            // ���ݽ� ������� ���µ�......
            Player_TransOff();  //_NA001385_20090924_DOMINATION_SHADOW
#if !defined(_NA_005159_20120807_DISABLE_REMOVE_RUSH_STATE)
            //���������̸� �������.
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

    SUNLOG(eCRITICAL_LOG, _T("["__FUNCTION__"] ��ŵ�� �߻��ϴ� ��Ȳ�� �����ΰ�?|Error=%d|"),
           flow.GetErrorCode());

    return RC::RC_BATTLE_FAILED;
}


//��������������������������������������������������������������������������������������������������������������������������������

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
        // ���� �������� �⺻���� ������ üũ�Ѵ�.
        const RC::eBATTLE_RESULT result = CanNormalAttack(preparing_skill_info_data);
        if (result != RC::RC_BATTLE_SUCCESS) {
            return result;
        };
        const SkillInfoParser* const skill_parser = SkillInfoParser::Instance();
        const SLOTCODE style_code = GetSelectedStyle();
        const BASE_STYLEINFO* const style_info = skill_parser->GetStyleInfo(style_code);
        if (style_info == NULL) {
            return RC::RC_BATTLE_BASEINFO_NOTEXIST; // �̰ž� ���� abusing�� �ƴѰ�?
        }
        StatusManager* const status_manager = GetStatusManager();
        //  (WAVERIX)(080907)(NOTE)
        //  - ... �� �巡�ﺯ���̴�... -.-;;;
        //  - Ŭ���� Ÿ�� ������ Ư�� ��ġ���� �ذ��� �����ϴ�. ������ �ٽ� �ѹ� ���캸�� ���� ����� ��������.

        //  Ŭ���� ���ѿ� �ɸ����� üũ
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

        //  �������ѿ� �ɸ����� üũ
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
                GetSelectedStyle(), GetCharType());  // �巡�� ������ ������� ���� �α״�...
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


//��������������������������������������������������������������������������������������������������������������������������������


VOID
Player::SetAttackDelay( BYTE byAttackSequence, SLOTCODE StyleCode )
{
	BASE_STYLEINFO *pBaseStyleInfo = SkillInfoParser::Instance()->GetStyleInfo( StyleCode );

	// �׼� ������ ����
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

	// ������ �׼� �����̸� �ణ �����ְ� ��´�.
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


//��������������������������������������������������������������������������������������������������������������������������������


VOID
Player::OnAttack( Character *pTarget, SLOTCODE skillcode, DAMAGETYPE damage )
{
	// �������� ���� ������ ����
	//�и������̸� ���߷� ���� �Ͽ� ������ ����
	DWORD dwDecreaseDuraVal = CalcDecreaseItemDuraValue( m_Attr.GetDecreaseItemDuraRatio() );
    bool apply_decrease_durability = false;
    if (skillcode == SKILLCODE_NORMAL_ATTACK) {
        //���������� ������ ����
        if (damage != 0) {
            apply_decrease_durability = true;
        }
    }
    //���������̸� ���߷� ���� ���� ������ ����
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

	// �����Ҷ� ȸ����
	DWORD recoverMP = CalcMPRecoverByAttack( GetCharType(), m_Attr.GetSPR(), skillRecoverPercent, skillRequireMP );

	OnRecover( 0, recoverMP );

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    if (GetCharType() == eCHAR_WITCHBLADE && damage > 0) // ��Ÿ�� ���ظ� �ָ� ����
    {
        PLAYERFP recoverFP = CalcFPRecoverByAttack(GetCharType());
        OnRecoverFP(recoverFP);
    }
    
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
}


//��������������������������������������������������������������������������������������������������������������������������������


void
Player::_Damaged(DamageArgs* damage_arg)
{
    Character* pAttacker = damage_arg->actor_;

    // (WAVERIX) (081013) (NOTE) �ʹ� ��� �ߺ��� ����. ������ ���� �۾��� ������ �����ϱ�� �Ѵ�.
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
        // (WAVERIX) (090824) (CHANGES) 3�� ������ 2ȸ �ߺ� ������ true ��� ������� Ǯ���
        // ���������� ������������ ������ ���� ����.
        if(damage) {
            damage = (damage >> 1) ? damage >> 1
                                   : 1;
        }
        // (NOTE) �տ��� ���� ������ ������ ���� ��ȯ�ϸ� �� �� �������� ��������.
        // ������ �̷� �ʿ� �ֳ�? ���ȭ ������ �ƴ� �̻� �̷��� �����ϰ� ������ ����.
        // (advanced)                           | (original code)
        //damage = (damage == 0) ?  0           | damage = damage
        //       : (damage >> 1) ?  damage >> 1 |        ? (damage >> 1) ? damage >> 1
        //       :                  1;          |                        : 1
        //                                      |        : 0;
        */
    }

    //             || �̹� ���� �������� �������� �־ �ҿ����... 
    if (damage_arg->damage_ == 0 || IsDead()) {
        return;
    }

    if (damage_arg->damage_) // ... �� �մ����� �߰� ������ ���ԵǴ� ��츦 �����ؼ� �������� �ʴ´�.
    {
#if !defined(_NA_000000_20121015_DISABLE_REMOVE_HIDE_STATE)
        Player_TransOff();
#endif //_NA_000000_20121015_DISABLE_REMOVE_HIDE_STATE

        // ������� �� ������ ����
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
    Character::_Damaged(damage_arg); //���⼭ ���� ������ ���Ұ� �Ͼ��.

    if(damage_arg->damage_ > 0 && IsAlive())
    {   
        // (WAVERIX) (090821) (BUG-FIX) HP Recovery��� ���� ���� ����
        // (�ش� ȸ������ ���� ��� ��� ���� �ƴ� ����)������ ����ؾ� �� ��, HP�� ���� Attribute��
        // ����ġ�� �ݿ��� �� ������, (���� �ݿ��ϴ��� ���������ų�, �Ź� update�����ؾ� �Ѵ�.)
        // ��������� �� ����, value(0) * ratio�� ����Ǿ� �׻� 0�� ������ �ȴ�.
        // (Addin Note)
        // �ʱ� �����ڿ� ��� �߰��ڰ� ����������, �� ����� �׽�Ʈ�� ���Է� �����ϴ� ����̶�
        // ���������(������ �ƴϴ� ��...) ���� �ش� �۾��ڰ� �߻���Ų (���� ��ų ���� ���׵�
        // ���� ������ �ִ�) ���׵��� �����ϸ鼭 ��Ʈ���� �� ���� �� ����. ���� ���� ���Ŀ��µ�... ��x!

        // ��Ŀ������ ���� �нú� ��ų�� ������..
        int reserveHp_Addin = (damage_arg->damage_ >> 1);
        if(reserveHp_Addin) {              //'+Ratio' (ADD-IN)
            int reserveHp_Ratio = m_Attr.GetRatioIncreaseReserveHPRatio();
            if(reserveHp_Ratio)  // = (src * percent) / 100
                reserveHp_Addin = int((reserveHp_Addin * (100 + reserveHp_Ratio)) / 100.0f);
        }

        DWORD reserveHP = GetReserveHP() + reserveHp_Addin;

        //// old code backup
        //int nReserveHPRatio = GetAttr().GetIncreaseReserveHPRatio(); // ��Ŀ������ ���� �нú� ��ų�� ������..
        //double dbTempReserveHP = (double)damage / 2;
        //double dbCalcReserveHPRatio = (double)(nReserveHPRatio/100.0f)*dbTempReserveHP;
        //
        //DWORD reserveHP = GetReserveHP() + dbTempReserveHP + dbCalcReserveHPRatio;

        {// (WAVERIX) (090821) (CHANGES) �ڵ� ���� �� ����
            DWORD calcHP = GetHP() + reserveHP;
            const DWORD maxHP = GetMaxHP();
            // (NOTE) curHP + reserveHP > maxHP ��Ȳ�� �߻�������, ���� ȸ�� ������ �׿� ���� ����ȴ�.
            if(maxHP < calcHP)
                reserveHP = calcHP - maxHP;
        }

        SetReserveHP(reserveHP);
    }

    if(pAttacker)
    {
        if(IsDead()) // �÷��̾ ��������(PK)�� �޾Ƽ� �׾��� ���
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

                // �� �κ��� �ɼǿ� ���� ó�� �̳�, ������ SDApply ���� ������.
                BASE_STATEINFO* pStateInfo = \
                    StateInfoParser::Instance()->GetStateInfo(eCHAR_STATE_REFLECT_DAMAGE);
                if (pStateInfo) {
                    mirror_damage_args.sd_apply_ = (eSD_APPLY)pStateInfo->m_bySDApply;
                }
                mirror_damage_args.damage_ = damage_arg->damage_ * GetReflectDamageRatio();
                //mirror_damage_args.damage_sd_;
            };

            pAttacker->Damaged(&mirror_damage_args); // ������ �ݻ� �Ӽ� üũ
            DWORD dwAttackerReserveHP = pAttacker->GetReserveHP();
            if(dwAttackerReserveHP > 0)
                dwAttackerReserveHP = dwAttackerReserveHP / 10 + mirror_damage_args.damage_ / 2; //�⺻�� ����HP 10% + Damage�� 50%�� �����Ѵ�.
            else
                dwAttackerReserveHP = mirror_damage_args.damage_ / 2; //�⺻�� ����HP 10% + Damage�� 50%�� �����Ѵ�.

            pAttacker->SetReserveHP(dwAttackerReserveHP);

            //�ݻ� �������� ���� ���� �޾Ҵµ�, �ݻ������� ���� ������ �𿡰� �������� �ִ� ���̴�.
            //���� ���� ��ü�� "��"�� �ȴ�.
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
        // �÷��̾ ���� �������� ������� ���µ�..
        //���������̸� �������.
        BaseStatus* pStatus = GetStatusManager()->FindStatus(eCHAR_STATE_RUSH);
        if (pStatus) {
            pStatus->StopStatus();
        }
    };
#endif //_NA_005159_20120807_DISABLE_REMOVE_RUSH_STATE
    IVehicleEvent::OnEvent(this, IVehicleEvent::Hit()); // for vehicles 'Hit' event processing

#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    //��ġ���̵�, ����¡������ ó��
    ProcessRisingForce();
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
}

//��������������������������������������������������������������������������������������������������������������������������������


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


//��������������������������������������������������������������������������������������������������������������������������������


BOOL
Player::CanResurrection( Character *pTarget )
{
	if( !pTarget )
		return FALSE;

	//��Ƽ���� �ƴϸ� �ڽŸ� ��Ȱ
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
	//��Ƽ���̸� ��Ƽ���� ��Ȱ
	else
	{
		if( GetPartyState().GetPartyKey() != pTarget->GetPartyState().GetPartyKey() )
			return FALSE;
	}
	return TRUE;
}






























