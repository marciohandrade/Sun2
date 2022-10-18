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
/** ī�޶�� ���ΰ��� �⺻������ 0~360(�� ���Ȱ��� �׻������Ѵ�) WZ_PI * 2
	�ѻ����� ���� üũ�ؼ� ��������Ƿ� ���������� üũ�Ѵ�. �����ʹ����� üũ�Ѵ�	
*/
void StateRiderTurn::CheckTurnDirection(Player *pPlayer)
{
	float fDestAngle   = pPlayer->GetAngle();
	float fCameraAngle = g_Camera.GetAngle();

	int iTurnSTate = TARGET_LEFT;
	
	//ī�޶� ���ʿ� �ִ�.
	if ( fCameraAngle > fDestAngle)
	{
		//���ʿ��־ 180�Ѿ���� ������ ī�޶��� ������ �������̿����Ѵ�.
		if( fCameraAngle > fDestAngle+WZ_PI)
		{
			iTurnSTate = TARGET_REVERSE_RIGHT;
		}
	}
	else
	{
		iTurnSTate = TARGET_RIGHT;

		//�����ʿ� ī�޶� ��ġ�ص� 180�� �Ѿ���̶�� �������� ���°� ��������
		if( fDestAngle > fCameraAngle+WZ_PI)
		{
			iTurnSTate = TARGET_REVERSE_LEFT;
		}
	}

	//���۽ÿ��� ������ ����
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
			//���ʰ迭�϶� 
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
	case TARGET_LEFT://ī�޶� ����ũ�� ĳ���� �ݽð�(����)����
		{
			pPlayer->Rotate(fMove);
			if(g_Camera.GetAngle()<=pPlayer->GetAngle())
			{
				return STATEPROCESS::END;
			}
		}
		break;
	case TARGET_RIGHT://���ΰ����� ũ�� ĳ���� �ð�(������)����
		{
			pPlayer->Rotate(-fMove);
			if(g_Camera.GetAngle()>=pPlayer->GetAngle())
			{
				return STATEPROCESS::END;
			}
		}
		break;
	case TARGET_REVERSE_LEFT://���ΰ����� ũ���� �������� ���°� �� ������
		{
			pPlayer->Rotate(fMove);
		}
		break;
	case TARGET_REVERSE_RIGHT://ī�޶� ũ���� ���������� ���°� �� ������
		{
			pPlayer->Rotate(-fMove);
		}
		break;
	}
	
	// ���� ������ ������ �ٷ� �������ּ���~
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


