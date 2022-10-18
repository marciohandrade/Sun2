//------------------------------------------------------------------------------
//  SkillStateBloodRain.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"

#include "statebase.h"
#include "SkillState.h"
#include "SkillStateSpritSlug.h"
#include "Character.h"
#include "Clone.h"
#include "CloneManager.h"

#ifdef _NA_006221_20121031_SPRITSLUG_SKILL_MODIFY
#include "Hero.h"
#include "GlobalFunc.h"
#include "Monster.h"
#include "SkillInfoParser.h"

STATECHANGE::TYPE SkillStateSpritSlug::OnStart(Object* pOwner, DWORD CurTime)
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
            int use_hp = ability_info->m_iParam[1]; // �Ҹ�HP

            int need_hp = g_pHero->GetMaxHP() * std::abs(use_hp) / 1000;

            need_hp = g_pHero->GetHP() - need_hp;

            if (need_hp <= 0)
            {
                //7019	%s ��ų�� ����ϱ⿣ ü���� �ʹ� �����ϴ�.
                g_InterfaceManager.GetInterfaceStringFormat(szMessage,
                    INTERFACE_STRING_LENGTH,
                    7019,
                    skill_name);
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
                g_pHero->SendStopMovePacket();
                return STATECHANGE::FAILED;
            }
        }
        

        Object* pObj = g_pHero->GetTargetObject();
        if(pObj)
        {
            if(pObj->IsKindOfObject(MONSTER_OBJECT))
            {
                Monster* pMon = (Monster*)pObj;

                //Ư������ üũ
                switch(pMon->GetMonsterInfo()->m_byGrade)
                {
                case eNPC_BOSS:
                case eNPC_MIDDLEBOSS:
                case eNPC_LUCKY_MONSTER:
                    {
                        //7020	�ش� ���Ϳ��� %s ��ų�� ����Ҽ� �����ϴ�.
                        g_InterfaceManager.GetInterfaceStringFormat(szMessage,
                            INTERFACE_STRING_LENGTH,
                            7020,
                            skill_name);
                        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
                        g_pHero->SendStopMovePacket();
                        return STATECHANGE::FAILED;
                    }
                }

                //������ üũ
                int iLev = pMon->GetDisplayLevel() - g_pHero->GetLevel();
                if(10 <= iLev )	
                {
                    //7021	10���� �̻� ���� ��뿡�� ��ũ�극��ũ ��ų ����� �Ұ��� �մϴ�.
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

                //������ üũ
                int iLev = pTargetPlayer->GetLevel() - g_pHero->GetLevel();

#ifdef _DH_CHANGE_SHADOW_SKILL_DARK_BREAK
                if(1 <= iLev)	
#else
                if(10 <= iLev)	
#endif//_DH_CHANGE_SHADOW_SKILL_DARK_BREAK
                {
                    //7021	10���� �̻� ���� ��뿡�� ��ũ�극��ũ ��ų ����� �Ұ��� �մϴ�.
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
    }

    return StateSkill::OnStart(pOwner, CurTime );
}
#endif //_NA_006221_20121031_SPRITSLUG_SKILL_MODIFY

BOOL SkillStateSpritSlug::OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick )
{
	if( nEventIndex == 1 )
	{
		Character* pCharacter = (Character*)pOwner;
		Character* pTarget = (Character *)pCharacter->GetTargetObject();


		for( int i = 0; i<3 ; i++ )
		{
			DWORD dwCloneID = g_CloneManager.CreateClone(pCharacter);
			if( Clone* pClone = g_CloneManager.GetClone(dwCloneID) )
			{
				pClone->SetLifeTime(1500);
				pClone->SetColor(WzColor_RGBA(255,30,30,220));
				pClone->SetAlphaBlend(AB_LIGHTING);
				pClone->SetPlayAni(TRUE);

				pClone->SetAppear(i *300);
				WzVector wvNewPos = pCharacter->GetVisiblePos() + (pCharacter->GetDirection() ) * (float)(0.5*i);

				pClone->SetMoveSpeed(50.0f * SPEED_MULTIPLIER);
				pClone->Move(pCharacter->GetVisiblePos(), wvNewPos );
				if( i== 0 )
					pClone->SetAnimation(StrToWzID("S016"));
				else if( i==1 )
					pClone->SetAnimation(StrToWzID("S017"));
				else if( i==2) 
					pClone->SetAnimation(StrToWzID("S018"));
			}
		}
	}

	return TRUE;
}

