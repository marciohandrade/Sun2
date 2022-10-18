#include "SunClientPrecompiledHeader.h"

#include "statebase.h"
#include "SkillState.h"
#include "Hero.h"
#include "Character.h"
#include "Monster.h"
#include "Clone.h"
#include "CloneManager.h"
#include "GlobalFunc.h"
#include "SkillDarkBreak.h"
#include "SkillEffectFunc.h"
#include "InterfaceManager.h"
#include "SkillInfoParser.h"

SkillStateDarkBreak::SkillStateDarkBreak()
:m_iIllusionCount( 0 )
,m_bStartMove( FALSE )
{
}

SkillStateDarkBreak::~SkillStateDarkBreak(void)
{
}

STATECHANGE::TYPE SkillStateDarkBreak::OnStart(Object* pOwner, DWORD CurTime)
{
	if(NULL == g_pHero)
		return STATECHANGE::FAILED;

	TCHAR szMessage[INTERFACE_STRING_LENGTH + 1] = {0,};

	Character*	pCharacter = (Character*)pOwner;

	if (g_pHero->GetObjectKey() == pCharacter->GetObjectKey())
	{
        SkillScriptInfo* skill_info = GetSkillInfo();
        if (skill_info == NULL)
        {
            return STATECHANGE::FAILED;
        }
        TCHAR skill_name[INTERFACE_STRING_LENGTH] = {0, };
        g_InterfaceManager.GetInterfaceString(skill_info->m_dwSkillNcode, skill_name, INTERFACE_STRING_LENGTH);

        BASE_ABILITYINFO* ability_info = skill_info->GetAbilityInfoByIndex(0);
        if (ability_info)
        {
            int use_hp = ability_info->m_iParam[1]; // 소모HP

            int need_hp = std::abs(use_hp);

            need_hp = g_pHero->GetHP() - need_hp;
            if (need_hp <= 0)
            {
                //7019	%s 스킬을 사용하기엔 체력이 너무 적습니다.
                g_InterfaceManager.GetInterfaceStringFormat(szMessage,
                    INTERFACE_STRING_LENGTH,
                    7019,
                    skill_name);
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
                g_pHero->SendStopMovePacket();
                return STATECHANGE::FAILED;
            }
        }
#ifdef _NA_006749_20130604_NOT_LIMIT_DARKBREAK
#else

        Object* pObj = g_pHero->GetTargetObject();
        if(pObj)
        {
            if(pObj->IsKindOfObject(MONSTER_OBJECT))
            {
                Monster* pMon = (Monster*)pObj;

                //특수몬스터 체크
                switch(pMon->GetMonsterInfo()->m_byGrade)
                {
                case eNPC_BOSS:
                case eNPC_MIDDLEBOSS:
                case eNPC_LUCKY_MONSTER:
                    {
                        //7020	해당 몬스터에겐 %s 스킬을 사용할수 없습니다.
                        g_InterfaceManager.GetInterfaceStringFormat(szMessage,
                            INTERFACE_STRING_LENGTH,
                            7020,
                            skill_name);
                        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
                        g_pHero->SendStopMovePacket();
                        return STATECHANGE::FAILED;
                    }
                }

                //레벨차 체크
                int iLev = pMon->GetDisplayLevel() - g_pHero->GetLevel();
                if(10 <= iLev )	
                {
                    //7021	10레벨 이상 강한 상대에겐 %s 스킬 사용이 불가능 합니다.
                    g_InterfaceManager.GetInterfaceStringFormat(szMessage,
                        INTERFACE_STRING_LENGTH,
                        7021,
                        skill_name);
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
                    g_pHero->SendStopMovePacket();
                    return STATECHANGE::FAILED;
                }
            }
            else if(pObj->IsKindOfObject(PLAYER_OBJECT))
            {
                Player* pTargetPlayer = (Player*)pObj;

                //레벨차 체크
                int iLev = pTargetPlayer->GetLevel() - g_pHero->GetLevel();

#ifdef _DH_CHANGE_SHADOW_SKILL_DARK_BREAK
                if(1 <= iLev)	
#else
                if(10 <= iLev)	
#endif//_DH_CHANGE_SHADOW_SKILL_DARK_BREAK
                {
					//7021	10레벨 이상 강한 상대에겐 %s 스킬 사용이 불가능 합니다.
                    g_InterfaceManager.GetInterfaceStringFormat(szMessage,
                        INTERFACE_STRING_LENGTH,
                        7021,
                        skill_name);
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
					g_pHero->SendStopMovePacket();
					return STATECHANGE::FAILED;
				}
			}
		}
#endif // _NA_006749_20130604_NOT_LIMIT_DARKBREAK
	}


	m_iIllusionCount = 0;


	Character* pChr = (Character*)pOwner;

	WzVector StartPosition = pChr->GetVisiblePos();
	WzVector TargetPosition = GetTargetNearPosition( StartPosition, (Character*)pChr->GetTargetObject(), 1.5f );

	pChr->Move( StartPosition, TargetPosition, CMS_RUN, FALSE, -1, -1 );

	return StateSkill::OnStart(pOwner, CurTime );
}

STATEPROCESS::TYPE SkillStateDarkBreak::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character*	pCharacter = (Character*)pOwner;

	EnterProcess(pOwner,CurTime,dwTick);

	float fAnimationRate = GetAnimationRate();

	if( fAnimationRate >= 20.0f && FALSE == m_bStartMove)
	{
		// 이동시작
		m_bStartMove = TRUE;
	}

	WzVector wvPos, wvDir, wvLen, wvVal;
	wvPos = pCharacter->GetPosition();
	wvLen = pCharacter->GetTarGetPosition() - wvPos;
	wvDir = pCharacter->GetDirection() * 0.5f;
	wvVal = wvPos + wvDir;

	if (0 == m_iIllusionCount && fAnimationRate >= 20.0f)
	{

		CreateIllusion( pCharacter, 900, 800 );
		wvVal = wvLen * 0.3f;
		CreateIllusion( pCharacter, 1000, 900 , &wvVal);
		m_iIllusionCount++;
	}
	else if (1 == m_iIllusionCount && fAnimationRate >= 20.0f)
	{
		CreateIllusion( pCharacter, 1100, 1000 );
		wvVal = wvLen * 0.6f;
		CreateIllusion( pCharacter, 1200, 1100 , &wvVal);
		m_iIllusionCount++;
	}
	else if (2 == m_iIllusionCount && fAnimationRate >= 20.0f)
	{
		CreateIllusion( pCharacter, 1300, 1200 );
		m_iIllusionCount++;
	}

	// 이벤트체크
	CheckEvent();

	if( m_bStartMove == TRUE )
	{
		// 무지하게 빨리 지나가면서
		const float SPEED_PER_SECOND = 100.0f * SPEED_MULTIPLIER;
		ProcessMove(pCharacter,SPEED_PER_SECOND,dwTick);
	}

	// 친다
	ProcessHit(pCharacter,1);

	return LeaveProcess(pOwner,CurTime,dwTick);
}


void SkillStateDarkBreak::CreateIllusion( Character *pCharacter, DWORD LiveTime, DWORD DisappearTime, WzVector* pwvPos/* = NULL*/)
{
	DWORD dwCloneKey = g_CloneManager.CreateClone(pCharacter);

	if (Clone* pClone = g_CloneManager.GetClone(dwCloneKey) )
	{
		if(NULL != pwvPos)
		{
			pClone->SetCurPos(*pwvPos);
			pClone->SetPosition(*pwvPos);
		}

		pClone->SetAlphaBlend(AB_ALPHA);
		pClone->SetAnimation(StrToWzID("S507"));
		pClone->SetColor(WzColor_RGBA(0,0,0,255));		
		pClone->SetLifeTime( LiveTime );	
		pClone->SetDisappear( DisappearTime );
	}
}
