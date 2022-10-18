#include "SunClientPrecompiledHeader.h"
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
#include "ToggleSkill.h"
#include "SkillInfoParser.h"
#include "Hero.h"
#include "SkillMain.h"


ToggleSkill::ToggleSkill(void)
{
    if ( toggle_state_array_.empty() )
    {
        // 헬로이드 계열 Aura스킬
        toggle_state_array_.push_back( eCHAR_STATE_SLOW_AURORA          );
        toggle_state_array_.push_back( eCHAR_STATE_WEAKNESS_AURORA      );
        toggle_state_array_.push_back( eCHAR_STATE_MISCHANCE_AURORA     );
        toggle_state_array_.push_back( eCHAR_STATE_DECLINE_AURORA       );
        toggle_state_array_.push_back( eCHAR_STATE_RECOVERY_AURORA      );
        toggle_state_array_.push_back( eCHAR_STATE_BOOST_AURORA         );
        toggle_state_array_.push_back( eCHAR_STATE_IGNORE_AURORA        );
        toggle_state_array_.push_back( eCHAR_STATE_CONCENTRATION_AURORA );
        toggle_state_array_.push_back( eCHAR_STATE_VICE_SWORD           );
        toggle_state_array_.push_back( eCHAR_STATE_EVADE                );
	}
	if ( toggle_skill_code_array_.empty() )
	{
        toggle_skill_code_array_.push_back( eSKILL_HELLROID_1004 );
        toggle_skill_code_array_.push_back( eSKILL_HELLROID_1006 );
        toggle_skill_code_array_.push_back( eSKILL_HELLROID_1011 );
        toggle_skill_code_array_.push_back( eSKILL_HELLROID_1013 );
        toggle_skill_code_array_.push_back( eSKILL_HELLROID_1020 );
        toggle_skill_code_array_.push_back( eSKILL_HELLROID_1022 );
        toggle_skill_code_array_.push_back( eSKILL_HELLROID_1027 );
        toggle_skill_code_array_.push_back( eSKILL_HELLROID_1029 );

        // 위치블레이드 계열 Toggle스킬
        toggle_skill_code_array_.push_back( eSKILL_WITCHBLADE_1111 ); // 바이스소드
        toggle_skill_code_array_.push_back( eSKILL_WITCHBLADE_1112 ); // 이베이드
        toggle_skill_code_array_.push_back( eSKILL_RISINGFORCE     ); // 라이징포스
    }

#ifdef _NA_008252_20150427_AURASKILL_PACKET_PENDING
    send_packet_pending_ = FALSE;
#endif //_NA_008252_20150427_AURASKILL_PACKET_PENDING
}

ToggleSkill::~ToggleSkill(void)
{
    toggle_state_array_.clear();
    toggle_skill_code_array_.clear();
}

eCHAR_STATE_TYPE ToggleSkill::FindToggleSkillState( SLOTCODE skill_class_code )
{
    for( size_t i=0;i < toggle_skill_code_array_.size();++i)
    {
        if( toggle_skill_code_array_[i] == skill_class_code )
        {
			if ( i >= toggle_state_array_.size() )
			{
				continue;
			}

            if( SKILL_EFFECT* skill_effect_ptr = g_pHero->FindSkillEffect( toggle_state_array_[i] ) )
            {
                return toggle_state_array_[i];
            }
        }
    }

    return eCHAR_STATE_INVALID;
}

void ToggleSkill::GetToggleSkillCodeArray( std::vector<SLOTCODE>& skill_code_array )
{
    for( size_t i=0;i!=toggle_skill_code_array_.size();++i)
    {
        if( SkillData* skill_data_ptr = xGetSkillData()->GetSkillDataBySkillClassCode( toggle_skill_code_array_[i] ) )
        {
            if( skill_data_ptr->IsActive() == true )
            {
                if( SkillScriptInfo* skill_info_ptr = skill_data_ptr->GetCurSkillInfo() )
                {
                    skill_code_array.push_back( skill_info_ptr->m_SkillCode );
                }
            }
        }
    }
}
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE