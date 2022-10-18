#include "SunClientPrecompiledHeader.h"
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
#include "StateBase.h"
#include "SkillState.h"
#include "SkillStateDarkPolingGrave.h"
#include "Character.h"
#include "Hero.h"
#include "Camera.h"

SkillStateDarkPolingGrave::SkillStateDarkPolingGrave(void)
{
}

SkillStateDarkPolingGrave::~SkillStateDarkPolingGrave(void)
{
}

STATECHANGE::TYPE SkillStateDarkPolingGrave::OnStart( Object* owner_ptr, DWORD cur_time )
{
    Character* character_ptr = static_cast<Character*>(owner_ptr);

    WzVector target_pos = character_ptr->GetCurrentAction()->SKILL.vTargePos;
    character_ptr->SetDirection( target_pos );

    // 목표위치로 이동
    character_ptr->StopMove();

    return StateSkill::OnStart( owner_ptr, cur_time );
}

STATEPROCESS::TYPE SkillStateDarkPolingGrave::OnProcess( Object* owner_ptr, DWORD cur_time, DWORD tick )
{
    Character* character_ptr = static_cast<Character*>(owner_ptr);
    WzVector target_pos = character_ptr->GetCurrentAction()->SKILL.vTargePos;

    EnterProcess( owner_ptr, cur_time, tick );

    // 이벤트체크
    CheckEvent();

    // 이동
    if (GetAnimationRate() >= 30.0f)
    {
        static float s_MoveSpeed = 40.0f;
        float fMoveSpeed = s_MoveSpeed * (float)tick * SPEED_MULTIPLIER;

        character_ptr->Move( character_ptr->GetVisiblePos(), target_pos , CMS_WALK, TRUE, -1, -1 );
        g_pSunTerrain->PathExp_Progress(character_ptr->GetPathExplorer(), fMoveSpeed, NULL);        
    }

    if (CheckNotProcessedHitEvent(1))
    {
        ++m_iHitProcessed;
        if (character_ptr->GetObjectKey() == g_pHero->GetObjectKey())
        {
            if (character_ptr->GetObjectKey()==g_pHero->GetObjectKey())
            {
                g_Camera.SetVibeType(Camera::VIBE_TYPE_HORISONTAL);
                g_Camera.StartVibration(150, 0.08f);
            }
        }
    }

    return LeaveProcess( owner_ptr, cur_time, tick );
}
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE