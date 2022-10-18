//------------------------------------------------------------------------------
//  StateSit.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"

#include "statebase.h"
#include "StateRiderTurn.h"
#include "RidingInfoParser.h"
#include "player.h"
#include "Camera.h"

//------------------------------------------------------------------------------
/**
*/
StateRiderTurn::StateRiderTurn(void)
{
	m_nTurnState = -1;
}

//------------------------------------------------------------------------------
/**
*/
StateRiderTurn::~StateRiderTurn(void)
{
}

//------------------------------------------------------------------------------
/**
*/
BOOL StateRiderTurn::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
{
	switch(pToTransiteState->GetStateKind())
	{
	case 0:
		break;
	}
	return TRUE;
}
//------------------------------------------------------------------------------
/** 카메라와 주인공은 기본적으로 0~360(의 라디안값을 항상유지한다) WZ_PI * 2
	둘사이의 각을 체크해서 어느방향의로 돌릴것인지 체크한다. 빠른쪽방향을 체크한다	
*/
void StateRiderTurn::CheckTurnDirection(Player *pPlayer)
{
	float fDestAngle   = pPlayer->GetAngle();
	float fCameraAngle = g_Camera.GetAngle();

	int iTurnSTate = TARGET_LEFT;
	
	//카메라가 왼쪽에 있다.
	if ( fCameraAngle > fDestAngle)
	{
		//왼쪽에있어도 180넘어선곳에 있으면 카메라의 방향은 오른쪽이여야한다.
		if( fCameraAngle > fDestAngle+WZ_PI)
		{
			iTurnSTate = TARGET_REVERSE_RIGHT;
		}
	}
	else
	{
		iTurnSTate = TARGET_RIGHT;

		//오른쪽에 카메라가 위치해도 180을 넘어선곳이라면 왼쪽으로 도는게 더빠르다
		if( fDestAngle > fCameraAngle+WZ_PI)
		{
			iTurnSTate = TARGET_REVERSE_LEFT;
		}
	}

	//시작시에는 무조건 셋팅
	if(m_nTurnState==-1)
	{
		if(iTurnSTate==TARGET_LEFT || iTurnSTate==TARGET_REVERSE_LEFT)
		{
			pPlayer->SetPlayerAnimation(pPlayer->GetCurrentLeftTurnAnimation(),eANI_L_ROTATE,TRUE);
			pPlayer->ProcessAnimation(0);
		}
		else
		{
			pPlayer->SetPlayerAnimation(pPlayer->GetCurrentRightTurnAnimation(),eANI_R_ROTATE,TRUE);
			pPlayer->ProcessAnimation(0);
		}
	}
	else
	{
		if(m_nTurnState != iTurnSTate)
		{
			//왼쪽계열일때 
			if(m_nTurnState==TARGET_LEFT ||m_nTurnState==TARGET_REVERSE_LEFT)
			{
				if(iTurnSTate==TARGET_RIGHT ||iTurnSTate==TARGET_REVERSE_RIGHT)
				{
					pPlayer->SetPlayerAnimation(pPlayer->GetCurrentRightTurnAnimation(),eANI_R_ROTATE,TRUE);
					pPlayer->ProcessAnimation(0);
				}
			}
			else
			{
				if(iTurnSTate==TARGET_LEFT ||iTurnSTate==TARGET_REVERSE_LEFT)
				{
					pPlayer->SetPlayerAnimation(pPlayer->GetCurrentLeftTurnAnimation(),eANI_L_ROTATE,TRUE);
					pPlayer->ProcessAnimation(0);
				}
			}
		}
	}

	m_nTurnState = iTurnSTate;
	
}
//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateRiderTurn::OnStart(Object* pOwner, DWORD CurTime)
{
	Player*	pPlayer = (Player*)pOwner;
	assert(pPlayer->IsKindOfObject(PLAYER_OBJECT));
	m_nTurnState = -1;
	CheckTurnDirection(pPlayer);
    static float fSpeed = 0.0014f;
    float fPlusSpeed=0.f;

    const  sRiderInfo* pRiderInfo = RidingInfoParser::Instance()->GetInfo(pPlayer->GetRiderIdx());

    if(pRiderInfo)
    {
        fPlusSpeed = (float)(pRiderInfo->set_speed_ - 160) /100000.f;
        if(fPlusSpeed<0)
        {
            fPlusSpeed = 0;
        }
    }
    m_fTurnSpeed = fSpeed + fPlusSpeed;
	return STATECHANGE::SUCCESS;
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE StateRiderTurn::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Player*	pPlayer = (Player*)pOwner;
	assert(pPlayer->IsKindOfObject(PLAYER_OBJECT));

	pPlayer->ProcessInterpolation(dwTick);
	pPlayer->ProcessAnimation(dwTick);

	float fMove = m_fTurnSpeed * dwTick;

	CheckTurnDirection(pPlayer);

	switch(m_nTurnState)
	{
	case TARGET_LEFT://카메라가 각이크다 캐릭이 반시계(왼쪽)돈다
		{
			pPlayer->Rotate(fMove);
			if(g_Camera.GetAngle()<=pPlayer->GetAngle())
			{
				return STATEPROCESS::END;
			}
		}
		break;
	case TARGET_RIGHT://주인공각이 크다 캐릭이 시계(오른쪽)돈다
		{
			pPlayer->Rotate(-fMove);
			if(g_Camera.GetAngle()>=pPlayer->GetAngle())
			{
				return STATEPROCESS::END;
			}
		}
		break;
	case TARGET_REVERSE_LEFT://주인공각이 크지만 왼쪽으로 도는게 더 빠르다
		{
			pPlayer->Rotate(fMove);
		}
		break;
	case TARGET_REVERSE_RIGHT://카메라 크지만 오른쪽으로 도는게 더 빠르다
		{
			pPlayer->Rotate(-fMove);
		}
		break;
	}
	
	// 다음 동작이 있으면 바로 수행해주세요~
	if (pPlayer->GetActionQueueCount())
	{
		pPlayer->ProcessNextAction();
		return STATEPROCESS::SUSPEND;
	}

	return STATEPROCESS::SUSPEND;
}
//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateRiderTurn::OnEnd(Object* pOwner, DWORD CurTime)
{
	m_nTurnState = -1;
	return STATECHANGE::SUCCESS;
}


