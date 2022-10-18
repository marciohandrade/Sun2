#include "SunClientPrecompiledHeader.h"
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
#include "StateBase.h"
#include "SkillState.h"
#include "skillstatespinturnthrust.h"
#include "Character.h"
#include <../../ProgramCommon/SkillInfoParser.h>
#include "Hero.h"
#include "Camera.h"
#include "CloneManager.h"
#include "Clone.h"


SkillStateSpinTurnThrust::SkillStateSpinTurnThrust()
{
    is_move_ = TRUE;
}

SkillStateSpinTurnThrust::~SkillStateSpinTurnThrust()
{

}

STATECHANGE::TYPE SkillStateSpinTurnThrust::OnStart( Object* owner_ptr, DWORD cur_time )
{
    return StateSkill::OnStart( owner_ptr, cur_time );
}

STATEPROCESS::TYPE SkillStateSpinTurnThrust::OnProcess( Object* owner_ptr, DWORD cur_time, DWORD tick )
{
    EnterProcess( owner_ptr, cur_time, tick );

    Character* character_ptr = static_cast<Character*>(owner_ptr);
    Character* target_ptr = static_cast<Character*>(character_ptr->GetTargetObject());

    if (target_ptr == NULL)
        return STATEPROCESS::END;

    // 이벤트체크
    CheckEvent();
    
    // 먼저 이동까지 완료한다.
    WzVector Distance = character_ptr->GetVisiblePos() - target_ptr->GetVisiblePos();
    if (TRUE == is_move_)
    {        
        SkillScriptInfo* pSkillInfo = GetSkillInfo();

        WzVector CurrentPosition = character_ptr->GetVisiblePos();
        WzVector Direction = target_ptr->GetVisiblePos() - CurrentPosition;
        VectorNormalize(&Direction,&Direction);

        WzVector TargetPosition = CurrentPosition + Direction * ((float)pSkillInfo->m_wSkillRange / 10.f);

        character_ptr->Move( CurrentPosition, TargetPosition, CMS_RUN, TRUE );

        const float SPEED_PER_SECOND = 70.0f * SPEED_MULTIPLIER;
        ProcessMove(character_ptr, SPEED_PER_SECOND, tick);
    }

    if (VectorLength(&Distance) < 4.0f)
    {
        // 도착 후에 데미지
        if (GetAnimationRate() >= 80.0f)
        {
            // 데미지 표시
            character_ptr->ProcessAttackResult_Group( character_ptr->GetCurrentAttackSerial() );
        }

        is_move_ = FALSE;
        character_ptr->StopMove();
    }

    // 대상의 위치에 도착
    if ( FALSE == character_ptr->GetPathExplorer()->IsMoving() )
    {
        is_move_ = FALSE;
    }

    return LeaveProcess( owner_ptr, cur_time, tick );
}

void SkillStateSpinTurnThrust::CreateIllusion( Character* character_ptr, DWORD live_time, DWORD disappear_time )
{
    DWORD dwCloneKey = g_CloneManager.CreateClone(character_ptr);

    if ( Clone *pClone = g_CloneManager.GetClone(dwCloneKey) )
    {
        pClone->SetColor(WzColor_RGBA(255,255,255,255));
        pClone->SetLifeTime( live_time );
        pClone->SetDisappear( disappear_time );
    }
}
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE