//------------------------------------------------------------------------------
//  StateAir.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "stateair.h"
#include "Character.h"

//------------------------------------------------------------------------------
/**
*/
StateAir::StateAir(void)
{
}

//------------------------------------------------------------------------------
/**
*/
StateAir::~StateAir(void)
{
}

//------------------------------------------------------------------------------
/**
*/
BOOL StateAir::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
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
STATECHANGE::TYPE StateAir::OnStart(Object* pOwner, DWORD CurTime)
{
	Character*	pChr = (Character*)pOwner;

	// 점프 시작모션
    switch( pChr->GetWeaponAnimationCode() )
    {
    case '0':	pChr->SetAnimation( "HS01", FALSE); break;
    case '1':	pChr->SetAnimation( "HS11", FALSE); break;
    case '2':	pChr->SetAnimation( "HS21", FALSE); break;
    case '3':	pChr->SetAnimation( "HS31", FALSE); break;
    }
	pChr->m_AirAnimationState = Character::AIR_ANIMATION_STATE_BLOW;
	pChr->m_fHeight = 0.001f;

	return STATECHANGE::SUCCESS;
}

//------------------------------------------------------------------------------
/**
	중력 가속도
	friend 설정때문에 모든 변수에 접근가능 전부 작성후 함수로 교체
*/
STATEPROCESS::TYPE StateAir::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character*	pChr = (Character*)pOwner;
	pChr->ProcessInterpolation(dwTick);
    float fRet = pChr->Character::ProcessAnimation(dwTick);

#ifdef _NA_006688_20130502_1302_SKILL_RENEWAL_AIR_ANIMATION_RENEWAL
	if( pChr->m_AirAnimationState == Character::AIR_ANIMATION_STATE_BLOW )
	{
		pChr->UpdateAir( dwTick );

        if( pChr->m_AirData.m_NoDown == TRUE )
        {
            // 버서커의 더블 어택

            if( pChr->GetHeight() == 0.0f )
            {
                // 바닥에 착지하는 타이밍에 연출

                pChr->SetAngleX( 0.0f );
                pChr->SetHeight( 0.0f );

                // 떨림효과
                WzVector vUp;
                vUp.x = 0.0f;
                vUp.y = 0.0f;
                vUp.z = 1.0f;
                pChr->Vibrate(vUp,0.05f);			

                // 먼지튀기
                pChr->CreateEffectAtHead(StrToWzID("A012"),0.0f);

                // 죽었으면 ㅃㅃ~
                if (pChr->GetHP() <= 0)
                {
                    pChr->SetNextState(STATE::DEATH,g_CurTime);
                    return STATEPROCESS::SUSPEND;
                }

                pChr->m_AirAnimationState = Character::AIR_ANIMATION_STATE_SLASH;
                fRet = 0.0f;
            }
        }
        else
        {
            // 에어블로우
            if( pChr->GetHeight() == 0.0f )
		    {
			    pChr->SetAngleX( 0.0f );
			    pChr->SetHeight( 0.0f );

			    // 떨림효과
			    WzVector vUp;
			    vUp.x = 0.0f;
			    vUp.y = 0.0f;
			    vUp.z = 1.0f;
			    pChr->Vibrate(vUp,0.05f);			
    			
			    // 먼지튀기
			    pChr->CreateEffectAtHead(StrToWzID("A012"),0.0f);


			    // 죽었으면 ㅃㅃ~
			    if (pChr->GetHP() <= 0)
			    {
				    pChr->SetNextState(STATE::DEATH,g_CurTime);
				    return STATEPROCESS::SUSPEND;
			    }

                // 바닥에 패댕겨쳐지는 동작
                switch( pChr->GetWeaponAnimationCode() )
                {
                case '0':	pChr->SetAnimation( "HS04", FALSE); break;
                case '1':	pChr->SetAnimation( "HS14", FALSE); break;
                case '2':	pChr->SetAnimation( "HS24", FALSE); break;
                case '3':	pChr->SetAnimation( "HS34", FALSE); break;
                }
                pChr->m_AirAnimationState = Character::AIR_ANIMATION_STATE_DOWN;
                fRet = 0.0f;
		    }
	    }
    }

    // 버서커의 더블어택 애니메이션은 여기에서 끝낸다
    if( pChr->m_AirAnimationState == Character::AIR_ANIMATION_STATE_SLASH && fRet == 100.0f )
    {
        return STATEPROCESS::END;
    }



    // 에어블로우의 착지와 일어나는 애니메이션 처리
    // 구버전은 F002, F003 애니메이션을 이용해 착지후 정신차리는 연출을 구현했었는데
    // F00X 시리즈는 다운상태의 애니메이션에서도 사용되고 있어서 마음대로 바꾸질 못함..
    // 
    // 그래서 전용 애니메이션을 추가한것이 HS07, HS08임..
    // 문제는 이 두 애니메이션은 플레이어만 가지고 있다는것인데... 
    // 그런 이유로 플레이어들은 HS07, HS08 애니메이션을 사용하기로하고, 
    // 그외의 몬스터등은 이전의 애니메이션인 F002, F003을 그대로 사용하게된것임
    //
    // 플레이어형 몬스터가 존재하기 때문에 단순히 오브젝트 타입만으로는 식별할 수 없어
    // Class 정보를 이용해 식별함. SSQ이 아닌 플레이어형 몬스터는 MONSTER_OBJECT로 되어있기때문에...


    // 다운 애니메이션 착지후 일어나는 동작
    if( pChr->m_AirAnimationState == Character::AIR_ANIMATION_STATE_DOWN && fRet == 100.0f )
    {
        bool is_monster = pChr->GetClass() == eCHAR_TYPE_MAX;

        // 다운상태의 스턴 모션
        if( is_monster )
        {
            switch( pChr->GetWeaponAnimationCode() )
            {
            case '0':	pChr->SetAnimation( "F002", TRUE); break;
            case '1':	pChr->SetAnimation( "F102", TRUE); break;
            case '2':	pChr->SetAnimation( "F202", TRUE); break;
            case '3':	pChr->SetAnimation( "F302", TRUE); break;
            }
        }
        else
        {
            switch( pChr->GetWeaponAnimationCode() )
            {
            case '0':	pChr->SetAnimation( "HS07", TRUE); break;
            case '1':	pChr->SetAnimation( "HS17", TRUE); break;
            case '2':	pChr->SetAnimation( "HS27", TRUE); break;
            case '3':	pChr->SetAnimation( "HS37", TRUE); break;
            }
        }

        pChr->m_AirAnimationState = Character::AIR_ANIMATION_STATE_DOWN_STUN;
    }

#ifdef _NA_007619_20140916_PROCESS_CHANGE_AIRBLOW_DOWN
    // 일어나는 동작 후 상태 종료
    if( pChr->m_AirAnimationState == Character::AIR_ANIMATION_STATE_DOWN_STUN && fRet == 100.0f )
    {
        return STATEPROCESS::END;
    }
#else
    // 다운 애니메이션 일어나는 동작
	if( pChr->m_AirAnimationState == Character::AIR_ANIMATION_STATE_DOWN_STUN && CurTime >= pChr->m_AirData.m_AirTime)
	{
        bool is_monster = pChr->GetClass() == eCHAR_TYPE_MAX;

        // 다운상태에서 일어서는 모션
        if( is_monster )
        {
            switch( pChr->GetWeaponAnimationCode() )
            {
            case '0':	pChr->SetAnimation( "F003", FALSE); break;
            case '1':	pChr->SetAnimation( "F103", FALSE); break;
            case '2':	pChr->SetAnimation( "F203", FALSE); break;
            case '3':	pChr->SetAnimation( "F303", FALSE); break;
            }
        }
        else
        {
		    switch( pChr->GetWeaponAnimationCode() )
		    {
		    case '0':	pChr->SetAnimation( "HS08", FALSE); break;
		    case '1':	pChr->SetAnimation( "HS18", FALSE); break;
		    case '2':	pChr->SetAnimation( "HS28", FALSE); break;
		    case '3':	pChr->SetAnimation( "HS38", FALSE); break;
		    }
        }

		pChr->m_AirAnimationState = Character::AIR_ANIMATION_STATE_GETUP;
        fRet = 0.0f;
	}

    // 일어나는 동작 후 상태 종료
	if( pChr->m_AirAnimationState == Character::AIR_ANIMATION_STATE_GETUP && fRet == 100.0f )
	{
		return STATEPROCESS::END;
	}
#endif // _NA_007619_20140916_PROCESS_CHANGE_AIRBLOW_DOWN

#else // _NA_006688_20130502_1302_SKILL_RENEWAL_AIR_ANIMATION_RENEWAL

    if( pChr->m_AirAnimationState == Character::AIR_ANIMATION_STATE_BLOW )
    {
        pChr->UpdateAir( dwTick );
        if( pChr->GetHeight() == 0.0f )
        {
            pChr->SetAngleX( 0.0f );
            pChr->SetHeight( 0.0f );

            // 떨림효과
            WzVector vUp;
            vUp.x = 0.0f;
            vUp.y = 0.0f;
            vUp.z = 1.0f;
            pChr->Vibrate(vUp,0.05f);			

            // 먼지튀기
            pChr->CreateEffectAtHead(StrToWzID("A012"),0.0f);


            // 죽었으면 ㅃㅃ~
            if (pChr->GetHP() <= 0)
            {
                pChr->SetNextState(STATE::DEATH,g_CurTime);
                return STATEPROCESS::SUSPEND;
            }

            // 다운 동작으로 변경
            switch( pChr->GetWeaponAnimationCode() )
            {
            case '0':	pChr->SetAnimation( "F002", TRUE); break;
            case '1':	pChr->SetAnimation( "F102", TRUE); break;
            case '2':	pChr->SetAnimation( "F202", TRUE); break;
            case '3':	pChr->SetAnimation( "F302", TRUE); break;
            }


            if( pChr->m_AirData.m_NoDown == TRUE )
            {
                // 다운되지 않고 바로 상태종료
                return STATEPROCESS::END;
            }
            else
            {
                pChr->m_AirAnimationState = Character::AIR_ANIMATION_STATE_DOWN;
            }
        }
    }

    if( pChr->m_AirAnimationState == Character::AIR_ANIMATION_STATE_DOWN && CurTime >= pChr->m_AirData.m_AirTime)
    {
        switch( pChr->GetWeaponAnimationCode() )
        {
        case '0':	pChr->SetAnimation( "F003", FALSE); break;
        case '1':	pChr->SetAnimation( "F103", FALSE); break;
        case '2':	pChr->SetAnimation( "F203", FALSE); break;
        case '3':	pChr->SetAnimation( "F303", FALSE); break;
        }
        pChr->m_AirAnimationState = Character::AIR_ANIMATION_STATE_GETUP;
    }

    if( pChr->m_AirAnimationState == Character::AIR_ANIMATION_STATE_GETUP && fRet == 100.0f )
    {
        return STATEPROCESS::END;
    }

#endif // _NA_006688_20130502_1302_SKILL_RENEWAL_AIR_ANIMATION_RENEWAL


	return STATEPROCESS::HOLD;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateAir::OnEnd(Object* pOwner, DWORD CurTime)
{
	Character* pChr = (Character*)pOwner;
	pChr->SetAngleX( 0.0f );
	pChr->SetHeight( 0.0f );

	return STATECHANGE::SUCCESS;
}
