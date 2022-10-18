#include "SunClientPrecompiledHeader.h"
#include "SkillFacade.h"
#include "SkillInfoParser.h"

namespace SkillFacade
{
    void InstantResultData::ParserInstantResult( MSGSUB_SKILL_ACTION_INSTANT_RESULT_BRD* pRecvPacket, BYTE* pPacket_Additions )
    {
        // Delay와 다른것은 CurPos, DestPos가 있는것 뿐. 아쉽게도 상속구조가 아님 ㅋ
        m_SkillActionInstantResult = pRecvPacket->result;

        _ParserCommonResult( pPacket_Additions );
    }

    void DelayResultData::ParserDelayResult( MSGSUB_SKILL_ACTION_DELAY_RESULT_BRD* pRecvPacket, BYTE* pPacket_Additions )
    {
        m_SkillActionDelayResult = pRecvPacket->result;

        _ParserCommonResult( pPacket_Additions );
    }

    void ResultData::_ParserCommonResult( BYTE* pMessage )
    {
        SkillScriptInfo* pSkillScriptInfo = SkillInfoParser::Instance()->GetSkillInfo(GetSkillCode());
        if( pSkillScriptInfo == NULL )
        {
            return;
        }
        m_pSkillScriptInfo = pSkillScriptInfo;


        for (int i = 0; i < GetTargetResultCount(); i++)
        {
            SKILL_RESULT_BASE* pSkillResult = (SKILL_RESULT_BASE *)pMessage;

            TargetResult TargetResult;
            TargetResult.m_pResult = pSkillResult;
            TargetResult.m_AttackSerial = GetSkillActionBase()->m_dwClientSerial;
            TargetResult.m_TimeSerial = 0;
            TargetResult.m_ResultCodeDataArray.clear();

            pMessage += sizeof(SKILL_RESULT_BASE);

            for( size_t j=0;j!=pSkillResult->m_byAbilityNum;++j)
            {
                // 어빌리티 타입에 따라 SKILL_RESULT_CODE를 상속받은 형태로 들어가있어서
                // SKILL_RESULT_CODE를 먼저 꺼내고 타입을 식별하고 각각의 케이스에 맞게 Offset 시켜준다.
                SKILL_RESULT_CODE* pResultCode = (SKILL_RESULT_CODE*)pMessage;

                BASE_ABILITYINFO* pAbilityInfo = pSkillScriptInfo->GetAbilityInfoByIndex((BYTE)pResultCode->m_wAbilityIndex);
                if( pAbilityInfo == NULL )
                    break;

                TargetResult::ResultCodeData CodeData;
                CodeData.m_pResultCode = pResultCode;
                CodeData.m_pAbillityInfo = pAbilityInfo;
                TargetResult.m_ResultCodeDataArray.push_back( CodeData );


                switch( pAbilityInfo->m_wAbilityID )
                {
                case eABILITY_MONSTER_TRANSFORMATION:
                    pMessage += sizeof(SKILL_RESULT_MONSTER_TRANSFORMATION);
                    break;
                case eABILITY_DAMAGE:
                    pMessage += sizeof(SKILL_RESULT_DAMAGE);
                    break;
                case eABILITY_EXHAUST_MP:
                case eABILITY_EXHAUST_HP:
                    pMessage += sizeof(SKILL_RESULT_EXHAUST);
                    break;
                case eABILITY_STUN_STATUS:
                    pMessage += sizeof(SKILL_RESULT_STUN);
                    break;
                case eABILITY_RESURRECTION:
                    pMessage += sizeof(SKILL_RESULT_RESURRECTION);
                    break;
                case eABILITY_SELF_DESTRUCTION:
                case eABILITY_KNOCKBACK:
				case eABILITY_PULLING:
                case eABILITY_KNOCKBACK2:
				case eABILITY_TELEPORT:
                    pMessage += sizeof(SKILL_RESULT_POSITION);
                    break;
                case eABILITY_FIGHTING_ENERGY_NUM_INCREASE:
                case eABILITY_BONUS_DAMAGE_PER_FIGHTING_ENERGY:
                    pMessage += sizeof(SKILL_RESULT_FIGHTING_ENERGY );
                    break;
                case eABILITY_QUICKSTEP:
                    pMessage += sizeof(SKILL_RESULT_POSITION);
                    break;
                default:
                    pMessage += sizeof(SKILL_RESULT_CODE);
                    break;
                }
            }
            m_TargetResult.push_back( TargetResult );
        }

        if( pMessage != NULL )
        {
            for( int i=0;i<GetEffectResultCount();++i)
            {
                SKILL_RESULT_EFFECT* pEffectResult = (SKILL_RESULT_EFFECT*)pMessage;
                pMessage += sizeof(SKILL_RESULT_EFFECT);

                EffectResult EffectResult;
                EffectResult.m_pResult = pEffectResult;
                m_EffectResult.push_back( EffectResult );
            }
        }
    }

}