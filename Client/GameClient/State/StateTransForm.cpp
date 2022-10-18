
//------------------------------------------------------------------------------
//  StateTransForm.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "StateTransForm.h"
#include "character.h"
#include "HeroActionInput.h"
#include "Hero.h"
#include "Player.h"
#include "InterfaceManager.h"
#include "npc.h"
#include "Monster.h"
#include "ProjectileManagerEx.h"
#include "AppearanceManager.h"
#include "StyleContainer.h"

#include "CloneManager.h"
#include "clone.h"
#include "globalfunc.h"


//------------------------------------------------------------------------------
/**
*/
StateTransForm::StateTransForm(void)
{
}

//------------------------------------------------------------------------------
/**
*/
StateTransForm::~StateTransForm(void)
{
}

//------------------------------------------------------------------------------
/**
*/
BOOL StateTransForm::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
{
	switch(pToTransiteState->GetStateKind())
	{
	case 0:
		break;
	}
	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateTransForm::OnStart(Object* pOwner, DWORD CurTime)
{
	Character*	pChr = (Character*)pOwner;

    if( pChr->IsKindOfObject( PLAYER_OBJECT ) )
    {
        Player* pPlayer = (Player*)pChr;

        //변신이 시작 되는 상황은 여기서 리소스 교체 
        pPlayer->TransForm();

        WzID AnimID = pPlayer->GetCurrentTransFormAnimation();

        pPlayer->SetAnimation(AnimID,FALSE);
    }

    if ( pChr->IsKindOfObject( MONSTER_OBJECT ) )
    {
        Monster* monster_ptr = static_cast<Monster*> (pChr);

        // 변신이 시작 되는 상황은 여기서 리소스 교체
        monster_ptr->TransForm();

        WzID AnimID = monster_ptr->GetCurrentTransFormAnimation();

        monster_ptr->SetAnimation(AnimID, FALSE);
    }
	
	return STATECHANGE::SUCCESS;
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE StateTransForm::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character* pChr = (Character*)pOwner;

    BOOL Is_monster_object_trans = FALSE;
    if (pChr->IsKindOfObject(MONSTER_OBJECT) == TRUE)
    {
        Is_monster_object_trans = pChr->IsTransForm();
    }

    if( pChr->IsKindOfObject( PLAYER_OBJECT ) == FALSE && Is_monster_object_trans == FALSE )
        return STATEPROCESS::END;


    if ( pChr->IsKindOfObject( PLAYER_OBJECT ) )
    {
        Player* pPlayer = (Player*)pChr;

	    WzEvent event;	

	    // 로드 하는 동안 틱이 너무 커져 변신 에니가 생략되지 않도록 한다.
	    dwTick=min(30,dwTick);

	    //	에니메이션 업데이트
	    pPlayer->ProcessInterpolation(dwTick,0.9999f);
	    float fRet= pPlayer->ProcessAnimation( dwTick ,&event);


	    //플레이어 변신 종료 연출 
	    if(pPlayer->IsKindOfObject(PLAYER_OBJECT)&&
		    pPlayer->GetTransformData().GetTransFormProcessType()==TRANSFORM_PROCESS_UNTRANSFORM)
	    {
		    // 타격점 타이밍
		    BOOL bCheckEvent=FALSE;
		    if (event.m_iNumCustom)
		    {
			    for (int a = 0; a < event.m_iNumCustom; a++)
			    {
				    // 
				    switch(event.m_Custom[a].m_iaParam3[0])
				    {
					    // 
				    case 1:
					    {
						    bCheckEvent=TRUE;
					    }
					    break;
				    }
			    }
		    }

		    if(bCheckEvent)
		    {
			    DWORD dwCloneKey = g_CloneManager.CreateClone(pPlayer);
			    Clone *pClone = g_CloneManager.GetClone(dwCloneKey);
			    if (pClone) 
			    {
				    int iR = Red_WzColor(pPlayer->GetColor());
				    int iG = Green_WzColor(pPlayer->GetColor());
				    int iB = Blue_WzColor(pPlayer->GetColor());

				    WzColor color=WzColor_RGBA(iR,iG,iB,255);
				    pClone->SetAlphaBlend(AB_LIGHTING);
				    pClone->SetColor(color);

				    pClone->SetLifeTime(200);
				    pClone->SetDisappear(100);
				    pClone->SetPlayAni(TRUE);

				    pClone->SetRestraintAniTick(TRUE);
			    }

			    pPlayer->UnTransForm();

			    pPlayer->SetAnimation("T001",FALSE);
			    pPlayer->SetCurrentFrame(57.839993f);
			    pPlayer->GetTransformData().SetTransFormProcessType(TRANSFORM_PROCESS_NONE);

			    GlobalFunc::CreateEffectBeUpToOption(StrToWzID("H623"),pPlayer->GetVisiblePos(),pPlayer->GetObjectKey());   
		    }
	    }


	    if( 100.0f ==fRet)
	    {
		    return STATEPROCESS::END;
	    }
    }

    if ( pChr->IsKindOfObject( MONSTER_OBJECT ) )
    {
        Monster* monster_ptr = static_cast<Monster*> (pChr);

        WzEvent event;

        // 로드 하는 동안 틱이 너무 커져 변신 애니가 생략되지 않도록 한다.
        dwTick=min(30, dwTick);

        // 애니메이션 업데이트
        monster_ptr->ProcessInterpolation(dwTick, 0.9999f);
        float fRet = monster_ptr->ProcessAnimation( dwTick, &event);

        // 몬스터는 변신 종료를 하지 않고 그냥 죽기 때문에 연출해 줄 필요가 없다.

        if ( 100.0f == fRet)
        {
            return STATEPROCESS::END;
        }
    }

	return STATEPROCESS::HOLD;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateTransForm::OnEnd(Object* pOwner, DWORD CurTime)
{
	Character*	pChr = (Character*)pOwner;

    if( pChr->IsKindOfObject( PLAYER_OBJECT ) )
    {
        Player* pPlayer = (Player*)pChr;
	    if(pPlayer->GetTransformData().GetTransFormType()==TRANSFORM_PROCESS_NONE)
		    pPlayer->UnTransForm();

	    pPlayer->GetTransformData().SetTransFormProcessType(TRANSFORM_PROCESS_NONE);
    }

    if (pChr->IsKindOfObject( MONSTER_OBJECT ) )
    {
        Monster* monster_ptr = static_cast<Monster*> (pChr);

        monster_ptr->GetTransformData().SetTransFormProcessType(TRANSFORM_PROCESS_NONE);
    }

	return STATECHANGE::SUCCESS;
}

