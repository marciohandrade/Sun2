#include "stdafx.h"
#include "Ability.h"
#include "SummonStatus.h"
#include "ApplySplashDamage.h"


VOID SummonStatus::Start()
{
    Character* const owner = GetOwner();
    if (owner == NULL)
    {
        return;
    }
    const Character* const attacker = GetAttacker();
    if (attacker == NULL)
    {
        return;
    }

	// ������ 1m ������ ������ �ڴ�.
	if (owner->GetField() != NULL)
	{
		WzVector cur_pos;
		owner->GetPos(&cur_pos);	
        
        WzVector attacker_pos;
        attacker->GetPos(&attacker_pos);

		const float summon_length = attacker->GetBodyRadius() + 1.5f;
		const WzVector diff = cur_pos - attacker_pos;
        const float dist_length = VectorLength(&diff);

		// ��ȯ���Ѿ��� �������� �ָ� ������ ��ȯ��Ų��.
		if (summon_length < dist_length)
		{
            WzVector normal;
			VectorNormalize(&normal, &diff);	
            normal.z = 0;
			
            WzVector dest_pos = attacker_pos + normal * summon_length;
			if (owner->GetField()->IsMovableCoordinates(&dest_pos) == false)
            {
                dest_pos = attacker_pos;
            }
				
			owner->GetField()->TeleportObject(owner, &dest_pos, TRUE);
		}
	}

	// �̵����̶�� �����.
	owner->StopMoving();
	owner->SetActionDelay(0);
}

VOID SummonStatus::End()
{
	AbilityStatus::End();

    Character* const owner = GetOwner();
    if (owner == NULL)
    {
        return;
    }
    Character* const attacker = GetAttacker();
    if (attacker == NULL)
    {
        return;
    }

	// HP�� ������ŭ ��´�.
	ChangeHP(false, VALUE_TYPE_PERCENT_PER_MAX, m_iOption1, 0);

	// �ֺ��� ���÷��� �������� �ش�.
	const DAMAGETYPE damage = max(0, m_iOption2);
	const float radius = GetAbilityValueType() / 10.f;
	const int count_limit = max(0, GetAbilityValue());
	WzVector cur_pos;	
    owner->GetPos(&cur_pos);

    nsSkill::ApplySplashDamage( 
        owner->GetField(), 
        attacker, 
        GetSkillCode(), 
        eATTACK_TYPE_MELEE, 
		damage, 
        count_limit, 
        owner->GetSectorIndex(), 
        cur_pos, 
        radius, 
        eSD_APPLY_NOT,
        NULL
    );
}