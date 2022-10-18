#include "SunClientPrecompiledHeader.h"

#include "statebase.h"
#include "StateTriggerAnimationControl.h"
#include "character.h"

//------------------------------------------------------------------------------
/**
*/
StateTriggerAnimationControl::StateTriggerAnimationControl(void)
{
}

//------------------------------------------------------------------------------
/**
*/
StateTriggerAnimationControl::~StateTriggerAnimationControl(void)
{
}

//------------------------------------------------------------------------------
/**
*/
BOOL StateTriggerAnimationControl::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
{
    return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateTriggerAnimationControl::OnStart(Object* pOwner, DWORD CurTime)
{
	Character* character = static_cast<Character*>(pOwner);
    const PLAYER_ACTION *action = character->GetCurrentAction();
    if (action->ActionID == ACTION_TRIGGER_ANIMATION_CONTROL)
    {
        BOOL is_loop = action->TRIGGERANIMATIONCONTROL.start_animation_loop == 0 ? TRUE : FALSE;
        character->SetAnimation(action->TRIGGERANIMATIONCONTROL.start_animation_id, is_loop);
    }
    else
    {
        return STATECHANGE::FAILED;
    }

	return STATECHANGE::SUCCESS;
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE StateTriggerAnimationControl::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
    Character* character = static_cast<Character*>(pOwner);
    const PLAYER_ACTION* action = character->GetCurrentAction();
    
    if (action->ActionID != ACTION_TRIGGER_ANIMATION_CONTROL)
    {
        character->DoNextAction();
        return STATEPROCESS::SUSPEND;
    }

	//	���ϸ��̼� ������Ʈ
	character->ProcessInterpolation(dwTick);
	float animation_frame_rate = character->ProcessAnimation(dwTick);

    // �ִϸ��̼� ����� ������ üũ(�Լ��� �ۼ�����)
    WzID current_animation_id = character->GetCurrentAnimation();
    
    if (animation_frame_rate >= 100.0f)
    {        
        if ((action->TRIGGERANIMATIONCONTROL.start_animation_id == current_animation_id) && 
            (action->TRIGGERANIMATIONCONTROL.next_animation_id != 0) &&
            (action->TRIGGERANIMATIONCONTROL.start_animation_loop != 0))
        {
            // ���� �ִϸ��̼����� ����
            BOOL is_loop = action->TRIGGERANIMATIONCONTROL.next_animation_loop == 0 ? TRUE : FALSE;
            character->SetAnimation(action->TRIGGERANIMATIONCONTROL.next_animation_id, is_loop);
        }
        else
        {
            character->DoNextAction();
            return STATEPROCESS::END;
        }

    }

    const int antion_count = character->GetActionQueueCount();
    if (antion_count > 0)
    {
        bool next_action = false;
        if ((action->TRIGGERANIMATIONCONTROL.start_animation_id == current_animation_id) &&
            (action->TRIGGERANIMATIONCONTROL.start_animation_loop == 0))
        {
            next_action = true;
        }
        if ((action->TRIGGERANIMATIONCONTROL.next_animation_id == current_animation_id) &&
            (action->TRIGGERANIMATIONCONTROL.next_animation_loop == 0))
        {
            next_action = true;
        }
        if (next_action == true)
        {
            character->DoNextAction();
            return STATEPROCESS::SUSPEND;
        }
    }

	// ��κ��� State���� ���������� ó���ϴ� ���º�ȭ�� ó���Ѵ�.
	if (character->CommonStateProcess())
	{
		return STATEPROCESS::SUSPEND;
	}

	return STATEPROCESS::SUSPEND;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateTriggerAnimationControl::OnEnd(Object* pOwner, DWORD CurTime)
{
	return STATECHANGE::SUCCESS;
}

