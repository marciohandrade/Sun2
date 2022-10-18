//------------------------------------------------------------------------------
//  SkillStateDoubleAttack.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "skillstatedoubleattack.h"
#include "HeroACtionInput.h"
#include "Character.h"
#include "Hero.h"
#include "SceneBase.h"
#include "GameFramework.h"
#include "ObjectManager.h"
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
#include "GlobalFunc.h"
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL


STATECHANGE::TYPE SkillStateDoubleAttack::OnStart(Object* pOwner, DWORD CurTime)
{
	// �̱� ��� �ϰ�� 

	//if( GENERALPARAM->IsNet()==FALSE)
	//{
	//	Character* pCharacter = (Character*)pOwner;

	//	GameFramework::GetCurrentScene()->AddBattleTimeSerial();

	//	DWORD dwClientSerial=pCharacter->GetCurrentAttackSerial();

	//	ATTACK_RESULT attackresult;
	//	//Todo:���ʹ� ���ýø����� �������� ���� ����.

	//	pCharacter->SetLatestAttack(GameFramework::GetCurrentScene()->GetBattleTimeSerial());//������ ������ Ÿ�ӽø���


	//	if(pCharacter->GetSCObject().GetObjectType() == MONSTER_OBJECT)
	//	{
	//		dwClientSerial = pCharacter->GetNextAttackSerial();
	//	}


	//	ZeroMemory(&attackresult,sizeof(attackresult));
	//	attackresult.dwAttackSerial = dwClientSerial;
	//	attackresult.dwTimeSerial   = GameFramework::GetCurrentScene()->GetBattleTimeSerial();      


	//	attackresult.TargetInfo.wDamage=50;
	//	attackresult.TargetInfo.dwTargetHP=80;
	//	attackresult.TargetInfo.dwTargetID=g_pHero->GetObjectKey();

	//	attackresult.dwGroupId=1;   //���� �ϳ��� �Ҳ���.


	//	int hp=attackresult.TargetInfo.dwTargetHP;

	//	DAMAGETYPE wTotalDamage=attackresult.TargetInfo.wDamage;
	//	DAMAGETYPE firstDamage = (wTotalDamage / 2);
	//	DAMAGETYPE secondDamage = wTotalDamage - firstDamage;			

	//	attackresult.TargetInfo.wDamage= firstDamage;
	//	attackresult.TargetInfo.dwTargetHP     = hp + secondDamage;	

	//	//1Ÿ
	//	attackresult.TargetInfo.dwAdditionalEffect |= ATTACK_ADDITIONAL_EFFECT_AIR;
	//	attackresult.TargetInfo.dwAirTime     = g_CurTime + 3000;
	//	attackresult.TargetInfo.dwGetupTime   = g_CurTime + 4000;


	//	pCharacter->AddAttackResult(&attackresult);// �����ڰ� ��ü�� �Ǿ� ������ ����� ó�� �Ѵ�.


	//	Character *pTarget = (Character *)g_ObjectManager.GetObject(attackresult.TargetInfo.dwTargetID);
	//	//��ų ����߿� ���� 
	//	if(pTarget&&!pTarget->CannotAir())
	//	{
	//		PLAYER_ACTION action;
	//		action.ActionID = ACTION_TOAIR;
	//		action.dwTimeSerial =GameFramework::GetCurrentScene()->GetBattleTimeSerial();
	//		action.TOAIR.fDownForceLimit = -40.0f;
	//		action.TOAIR.fGravity = 70.0f;
	//		action.TOAIR.fUpperForce = 50.0;
	//		action.TOAIR.bForceDown=FALSE;


	//		action.TOAIR.fHeightLimit = 3.4f;

	//		action.TOAIR.dwAirTime   = attackresult.TargetInfo.dwAirTime;
	//		action.TOAIR.dwGetupTime = attackresult.TargetInfo.dwGetupTime;			
	//		pTarget->PutAction(&action);
	//	}

	//	attackresult.dwGroupId=2;
	//	attackresult.TargetInfo.wDamage       = secondDamage;
	//	attackresult.TargetInfo.dwTargetHP     = hp;				
	//	attackresult.TargetInfo.dwAdditionalEffect |= ATTACK_ADDITIONAL_EFFECT_TOGROUND;
	//	pCharacter->AddAttackResult(&attackresult);

	//	pCharacter->AddNextAttackSerial();

	//}	

	return StateSkill::OnStart(pOwner,CurTime);
}

BOOL SkillStateDoubleAttack::OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick )
{
    if( Character* pCharacter = static_cast<Character*>(pOwner) )
    {
        if( nEventIndex == 1 )
        {
			if( Character* pTarget = (Character*)pCharacter->GetTargetObject() )
			{
                static DWORD duration = 2000;
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
                if (GlobalFunc::IsDominationScene() == FALSE)
                {
                    pTarget->SetAir( duration );
                }
                else if (GlobalFunc::IsDominationScene() == TRUE 
                    && pTarget->GetCurState() != STATE::COLLECT)
                {
                    pTarget->SetAir( duration );
                }
#else
				pTarget->SetAir(duration);
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL
			}
        }
#ifdef _NA_006688_20130502_1302_SKILL_RENEWAL_AIR_ANIMATION_RENEWAL
        else if( nEventIndex == 2 )
        {
            if( Character* pTarget = (Character*)pCharacter->GetTargetObject() )
            {
                // ���� ���°� AIR������ ��츸 �ٿ� �������� ����
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
                if (STATE::AIR == pTarget->GetCurState())
                {
                    // �ٿ� �������� ����
                    switch( pTarget->GetWeaponAnimationCode() )
                    {
                    case '0':	pTarget->SetAnimation( "HS03", FALSE, TRUE ); break;
                    case '1':	pTarget->SetAnimation( "HS13", FALSE, TRUE ); break;
                    case '2':	pTarget->SetAnimation( "HS23", FALSE, TRUE ); break;
                    case '3':	pTarget->SetAnimation( "HS33", FALSE, TRUE ); break;
                    }
                }
#else
                // �ٿ� �������� ����
                switch( pTarget->GetWeaponAnimationCode() )
                {
                case '0':	pTarget->SetAnimation( "HS03", FALSE, TRUE ); break;
                case '1':	pTarget->SetAnimation( "HS13", FALSE, TRUE ); break;
                case '2':	pTarget->SetAnimation( "HS23", FALSE, TRUE ); break;
                case '3':	pTarget->SetAnimation( "HS33", FALSE, TRUE ); break;
                }
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL
            }
        }
#endif //_NA_006688_20130502_1302_SKILL_RENEWAL_AIR_ANIMATION_RENEWAL
    }

    return TRUE;
}

//------------------------------------------------------------------------------ 
STATECHANGE::TYPE SkillStateDoubleAttack::OnEnd(Object* pOwner, DWORD CurTime)
{
    Character* character = static_cast<Character*>(pOwner);
    if (character != NULL)
    {
        character->ProcessAttackResult(character->GetCurrentAttackSerial(), TRUE); 
    }

    return StateSkill::OnEnd(pOwner, CurTime);
}
