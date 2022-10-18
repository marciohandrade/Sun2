#include "stdafx.h"
#include "MoveStateControl.h"
#include "NumericValues.h"
#include "Player.h"
#include "PlayerRider.h"
#include "./Status/StatusManager.h"

#include "SkillSystem/CSSyncTuner.h"

void
MoveStateControl::Init(Character *pOwner, eCHAR_MOVE_STATE eMoveState)
{
	m_pOwner = pOwner;
	SetMoveState(eMoveState);
}

void
MoveStateControl::SetMoveState(eCHAR_MOVE_STATE eMoveState)
{
	m_eMoveState = eMoveState;

	if(m_pOwner->IsEqualObjectKind(PLAYER_OBJECT))
	{
		m_fBaseMoveSpeed = GetBaseMoveSpeedAsState(m_eMoveState);
	}
	else if(m_pOwner->IsEqualObjectKind(NPC_OBJECT))
	{
		NPC* pNPC = static_cast<NPC*>(m_pOwner);
        const BASE_NPCINFO* npc_info = pNPC->GetBaseInfo();

		switch(m_eMoveState)
		{
		case CMS_WALK:
			m_fBaseMoveSpeed = npc_info->m_fWalkSpeed * SPEED_MULTIPLIER;
			break;
		case CMS_RUN:
			m_fBaseMoveSpeed = npc_info->m_fRunSpeed * SPEED_MULTIPLIER;
			break;
		default:
			m_fBaseMoveSpeed = GetBaseMoveSpeedAsState(m_eMoveState);
		}
	}
#ifdef _NA000000_SPEED_HACK_PREVENTION_SUPPORT
    // must be settled at last position
    if (m_pOwner->IsEqualObjectKind(PLAYER_OBJECT)) {
        static_cast<Player*>(m_pOwner)->GetSyncTuner()->OnChangeAttribute();
    };
#endif
}

float
MoveStateControl::GetMoveSpeed()
{
    // (NOTE) ����� ���� ��ġ ������ �̷�����. ��Ȯ�� ������ �� �� ū ������ ��ٰ� ������.
    //                                              (Dex.|Item|Skill)
    // (Base Formula) MoveSpeed = BaseMoveSpeed * Owner's MoveSpeedRatio / 100.0
    //                if(GM) then MoveSpeed = MoveSpeed * AddMoveSpeedRatio
    // (Riding Formula) MoveSpeed = 
    // CHANGES: f110802.3L, synchronize gm speed with client
    const bool is_player = m_pOwner->IsEqualObjectKind(PLAYER_OBJECT);
    float move_speed = m_fBaseMoveSpeed;
    if (is_player == false)
    {
        float move_speed_ratio = static_cast<float>(m_pOwner->GetMoveSpeedRatio());
        move_speed = move_speed * move_speed_ratio / 100.0f;
    }
    else if (Player* player = static_cast<Player*>(m_pOwner))
    {
        // (THINKING) : �������� CMS_RUN�� ���°� �ƴ� ���¿��� Riding�̶�� �� �������� �͵� �����.
        // ������ ���� �̻����� Ŭ���̾�Ʈ�� ���� ������ ���� �߻��� �� ���� �� ���� �ϴ� ����.
        const bool is_rider_summoned = player->GetStatusManager()->Status.IsRidingRider();
        float add_move_speed_ratio = player->GetAddMoveSpeedRatio();
        bool applied_gm_speed = (add_move_speed_ratio > 1.0f);
        // �ϱ� ���뿡�� ������ ��Ȯ�� �ľ�����.
        float move_speed_ratio = 100.0f;
        // Riding ���¿����� GM �ӵ� ���� ��� ������ �ʴ´�.
        if (is_rider_summoned) {
            PlayerAttributes& player_attrs = static_cast<PlayerAttributes&>(player->GetAttr());
            move_speed_ratio = static_cast<float>(player_attrs.GetRiderSpeedRatio());
        }
        else if (applied_gm_speed == false) {
            move_speed_ratio = static_cast<float>(player->GetMoveSpeedRatio());
        };

        move_speed = move_speed * move_speed_ratio;
        if (applied_gm_speed == false) {
            move_speed = move_speed / 100.0f;
        }
        else {
            move_speed = move_speed * add_move_speed_ratio / 100.0f;
        };
    };

    return move_speed;
}

// CHANGES: f110805.2L
float MoveStateControl::GetPlayerSpeedInMoving()
{
    if (m_pOwner == NULL || m_pOwner->IsEqualObjectKind(PLAYER_OBJECT) == false) {
        return 0.0f;
    };

    if (m_eMoveState == CMS_RUN) {
        return this->GetMoveSpeed();
    };
    const eCHAR_MOVE_STATE backup_moving_state = m_eMoveState;
    const float running_speed = ns_formula::GetBaseMoveSpeedAsState(CMS_RUN);
    const float current_speed = ns_formula::GetBaseMoveSpeedAsState(backup_moving_state);
    if (running_speed <= current_speed) {
        return this->GetMoveSpeed();
    };

    float base_moving_speed = m_fBaseMoveSpeed;
    //
    m_eMoveState = CMS_RUN;
    m_fBaseMoveSpeed = ns_formula::GetBaseMoveSpeedAsState(CMS_RUN);

    float calculated_running_speed = this->GetMoveSpeed();
    m_eMoveState = backup_moving_state;
    m_fBaseMoveSpeed = base_moving_speed;

    return calculated_running_speed;
}


#ifdef _NA_0_20110803_PLAYER_MOVEMENT_SYNC
float MoveStateControl::GetLimitMaxSpeed()
{
    eCHAR_MOVE_STATE move_state = m_eMoveState;
    SetMoveState(CMS_RUN);
    float speed = GetMoveSpeed();
    SetMoveState(move_state);
    return speed;
}
#endif
