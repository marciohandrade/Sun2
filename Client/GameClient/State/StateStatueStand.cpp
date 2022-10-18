#include "SunClientPrecompiledHeader.h"
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
#include "StateBase.h"
#include "StateStatueStand.h"
#include "Character.h"
#include "StatuePlayer.h"

StateStatueStand::StateStatueStand(void)
{
}

StateStatueStand::~StateStatueStand(void)
{
}

BOOL StateStatueStand::CanTransite(Object* owner_ptr, DWORD cur_time, StateBase* to_transite_state_ptr)
{
	switch ( to_transite_state_ptr->GetStateKind() )
	{
	case 0:
		break;
	}
	return TRUE;
}

STATECHANGE::TYPE StateStatueStand::OnStart(Object* owner_ptr, DWORD cur_time)
{
	Character* character_ptr = static_cast<Character*>(owner_ptr);

	if ( character_ptr->GetObjectType() == STATUE_PLAYER_OBJECT )
	{
		character_ptr->SetAnimation(static_cast<StatuePlayer*>(character_ptr)->get_stand_animation_id_(), TRUE);
	}
	else
	{
		character_ptr->SetAnimation("N205", TRUE);
	}

	return STATECHANGE::SUCCESS;
}

STATEPROCESS::TYPE StateStatueStand::OnProcess(Object* owner_ptr, DWORD cur_time, DWORD tick)
{
	Character* character_ptr = static_cast<Character*>(owner_ptr);

	// �ִϸ��̼� ������Ʈ
	character_ptr->ProcessInterpolation(tick);
	float ret = character_ptr->ProcessAnimation(tick);

	if (ret >= 0.3f)
	{
		character_ptr->SetCurrentFrame(0.0f);
	}

	if (ret >= 100.0f)
	{
		if ( character_ptr->GetObjectType() == STATUE_PLAYER_OBJECT )
		{
			character_ptr->SetAnimation(static_cast<StatuePlayer*>(character_ptr)->get_stand_animation_id_(), TRUE);
		}
		else
		{
			character_ptr->SetAnimation("N205", TRUE);
		}
	}

	// ��κ��� State���� ���������� ó���ϴ� ���º�ȭ�� ó���Ѵ�.
	if (character_ptr->CommonStateProcess())
	{
		return STATEPROCESS::SUSPEND;
	}

	// ���� ������ ������ �ٷ� �������ּ���~
	if (character_ptr->GetActionQueueCount())
	{
		character_ptr->ProcessNextAction();
		return STATEPROCESS::SUSPEND;
	}

	return STATEPROCESS::SUSPEND;
}

STATECHANGE::TYPE StateStatueStand::OnEnd(Object* owner_ptr, DWORD cur_time)
{
	Character* character_ptr = static_cast<Character*>(owner_ptr);

	character_ptr->SetSitState( FALSE );
	//character_ptr->SetStandUp( FALSE );

	return STATECHANGE::SUCCESS;
}
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
