#include "SunClientPrecompiledHeader.h"
#include "AuraSkill.h"
//
#include "SkillInfoParser.h"
//#include "Player.h"
//#include "SkillInventoryStoreParser.h"
//#include "BattleScene.h"

#include "Hero.h"
#include "SkillMain.h"

AuraSkill::AuraSkill()
{
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
    if( m_AuraStateArray.empty() )
    {
        m_AuraStateArray.push_back( eCHAR_STATE_SLOW_AURORA          );
        m_AuraStateArray.push_back( eCHAR_STATE_WEAKNESS_AURORA      );
        m_AuraStateArray.push_back( eCHAR_STATE_MISCHANCE_AURORA     );
        m_AuraStateArray.push_back( eCHAR_STATE_DECLINE_AURORA       );
        m_AuraStateArray.push_back( eCHAR_STATE_RECOVERY_AURORA      );
        m_AuraStateArray.push_back( eCHAR_STATE_BOOST_AURORA         );
        m_AuraStateArray.push_back( eCHAR_STATE_IGNORE_AURORA        );
        m_AuraStateArray.push_back( eCHAR_STATE_CONCENTRATION_AURORA );
        m_AuraSkillCodeArray.push_back( eSKILL_HELLROID_1004 );
        m_AuraSkillCodeArray.push_back( eSKILL_HELLROID_1006 );
        m_AuraSkillCodeArray.push_back( eSKILL_HELLROID_1011 );
        m_AuraSkillCodeArray.push_back( eSKILL_HELLROID_1013 );
        m_AuraSkillCodeArray.push_back( eSKILL_HELLROID_1020 );
        m_AuraSkillCodeArray.push_back( eSKILL_HELLROID_1022 );
        m_AuraSkillCodeArray.push_back( eSKILL_HELLROID_1027 );
        m_AuraSkillCodeArray.push_back( eSKILL_HELLROID_1029 );
    }
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
}


eCHAR_STATE_TYPE AuraSkill::FindAuraSkillState( SLOTCODE SkillClassCode )
{
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
    for( size_t i=0;i!=m_AuraSkillCodeArray.size();++i)
    {
        if( m_AuraSkillCodeArray[i] == SkillClassCode )
        {
            if( SKILL_EFFECT* pSkillEffect = g_pHero->FindSkillEffect( m_AuraStateArray[i] ) )
            {
                return m_AuraStateArray[i];
            }
        }
    }

#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
    return eCHAR_STATE_INVALID;
}

void AuraSkill::GetAuraSkillCodeArray( std::vector<SLOTCODE>& Array )
{
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
    for( size_t i=0;i!=m_AuraSkillCodeArray.size();++i)
    {
        if( SkillData* pSkillData = xGetSkillData()->GetSkillDataBySkillClassCode( m_AuraSkillCodeArray[i] ) )
        {
            if( pSkillData->IsActive() == true )
            {
                if( SkillScriptInfo* pSkillInfo = pSkillData->GetCurSkillInfo() )
                {
                    Array.push_back( pSkillInfo->m_SkillCode );
                }
            }
        }
    }
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
}
