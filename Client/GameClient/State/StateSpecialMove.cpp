//------------------------------------------------------------------------------
//  StateSepecialMove.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "statemove.h"
#include "statespecialmove.h"
#include "Hero.h"
#include "Player.h"
#include "Map.h"
#include "ObjectManager.h"
#include "Camera.h"
#include "GameFunc.h"
#include "NumericValues.h"
#include "Clone.h"
#include "CloneManager.h"
#include "HeroActionInput.h"
#include "Camera.h"
#include "GlobalFunc.h"
#include "KBMoveCheckManagerInClient.h"
#include "BattleScene.h"
#include "HeroAttributes.h"

#define INT_DUST_TIME 20


//------------------------------------------------------------------------------
/**
*/
StateSpecialMove::StateSpecialMove(void)
{
}

//------------------------------------------------------------------------------
/**
*/
StateSpecialMove::~StateSpecialMove(void)
{
}

//------------------------------------------------------------------------------
/**
*/
BOOL StateSpecialMove::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
{
	return TRUE;	
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateSpecialMove::OnStart(Object* pOwner, DWORD CurTime)
{
	Player*	pPlayer = (Player*)pOwner;
	assert(pPlayer->IsKindOfObject(PLAYER_OBJECT));

	

	if (pPlayer->GetObjectKey() == g_pHero->GetObjectKey())
	{

		if(g_pHero->GetSpecialMoveFlag()==CMS_TUMBLING_BACK)
		{
			vector3 v1,v2;
			WzVector vCameraDir = g_Camera.GetCameraDirection();
			WzVector vDir = g_pHero->GetDirection();

			v1.set(vCameraDir.x,vCameraDir.z,vCameraDir.y);
			v1.norm();
			v2.set(vDir.x,vDir.z,vDir.y);
			v2.norm();

			float dot=v1.dot(v2);
			float degree=acos(dot);
			degree=n_rad2deg(degree);


			if(degree<45||degree>320)
			{
				WzVector CameraPosition = g_Camera.GetCameraPosition();
				g_pHero->SetDirection(CameraPosition);

				g_pHero->SetSpecialMoveFlag(CMS_TUMBLING_FRONT);//바꿔치기
			}
			else
				g_pHero->SetAngle(g_Camera.GetAngle(),TRUE);

		}
		else
			g_pHero->SetAngle(g_Camera.GetAngle(),TRUE);
	}


	if(pPlayer->GetSpecialMoveFlag()==CMS_TUMBLING_LEFT)//옆구르기
	{
		pPlayer->SetAnimation(pPlayer->GetCurrentSpecialLeftStepAnimation());
		
		

	}
	else if(pPlayer->GetSpecialMoveFlag()==CMS_TUMBLING_RIGHT)//옆구르기
	{
		pPlayer->SetAnimation(pPlayer->GetCurrentSpecialRightStepAnimation());

	
	}
	else if(pPlayer->GetSpecialMoveFlag()==CMS_TUMBLING_FRONT)
	{
		pPlayer->SetAnimation(pPlayer->GetCurrentSpecialFrontStepAnimation());
	
	}
	else
	{
		pPlayer->SetAnimation(pPlayer->GetCurrentSpecialBackStepAnimation());
		
	}

	if (pPlayer->GetObjectKey() == g_pHero->GetObjectKey())
	{
#ifdef _NA000000_SPEED_HACK_PREVENTION_SUPPORT
        if ((g_pHero->PacketNak.is_recv_nak == false) && (BattleScene::IsWaitEnterSync() == FALSE))
        {
            ulong next_serial_key = 0;
            if (KBMoveCheckManagerInClient::Instance()->IssueNextSerial(&next_serial_key) == true)
            {
		        MSG_CG_SYNC_FAST_KBMOVE_SYN Packet;
		        //Packet.m_byCategory = CG_SYNC;
		        //Packet.m_byProtocol = CG_SYNC_FAST_KBMOVE_SYN;
		        Packet.m_byCmsMoveState =pPlayer->GetSpecialMoveFlag();
		        Packet.m_wvCurPos = g_pHero->GetPosition();
		        Packet.m_wAngle = (WORD) (g_pHero->GetAngle() * 180.0f / WZ_PI);
		        Packet.m_wTileIndex = g_pHero->GetPathExplorer()->GetTile();

                Packet.issued_serial = next_serial_key;
                Packet.client_test_result = \
                    nsSync::MemoryHackMonitor::Instance()->GetNextClientTestSerial();
                Packet.Encode();
		        g_pHero->SendPacket(&Packet,sizeof(Packet));
            }
            else
            {
                WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_SYSCHAT, "KBMoveCheckManagerInClient::Instance()->IssueNextSerial(&next_serial_key) == false");
            }
        }
#else
		MSG_CG_SYNC_FAST_KBMOVE_SYN Packet;

		Packet.m_byCategory = CG_SYNC;
		Packet.m_byProtocol = CG_SYNC_FAST_KBMOVE_SYN;

		Packet.m_byCmsMoveState =pPlayer->GetSpecialMoveFlag();
		Packet.m_wvCurPos = g_pHero->GetPosition();
		Packet.m_wAngle = (WORD) (g_pHero->GetAngle() * 180.0f / WZ_PI);
		Packet.m_wTileIndex = g_pHero->GetPathExplorer()->GetTile();

		g_pHero->SendPacket(&Packet,sizeof(Packet));
#endif //_NA000000_SPEED_HACK_PREVENTION_SUPPORT

	}

	pPlayer->StopBlendAnimation();

	pPlayer->m_bSpecialMoveMove=TRUE;
	pPlayer->m_dwSpecialMoveCurTime=0;

	

	m_iDustTime=0;
	m_dwCopyTime=INT_DUST_TIME;
	
	return STATECHANGE::SUCCESS;
}

//------------------------------------------------------------------------------
/**
*/
void StateSpecialMove::OnProcessEffect(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Player*	pPlayer = (Player*)pOwner;

	if( m_dwCopyTime < dwTick ) 
	{
		m_dwCopyTime=INT_DUST_TIME;
		

		WzVector vPos = pPlayer->GetVisiblePos();

		// 지형속성에 따라 이펙트를 뿌려준다
		switch(pPlayer->m_dwTerrainAttribute)
		{
		case eTERRAIN_ATTRIBUTE_WOOD:
		case eTERRAIN_ATTRIBUTE_MUD:
		case eTERRAIN_ATTRIBUTE_METAL:
		case eTERRAIN_ATTRIBUTE_STONE:
		case eTERRAIN_ATTRIBUTE_DIRT:
			{
				GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A000"),vPos,pPlayer->GetObjectKey());
			}
			break;

		case eTERRAIN_ATTRIBUTE_WATER:
			{
				GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A002"),vPos,pPlayer->GetObjectKey());
			}
			break;

		case eTERRAIN_ATTRIBUTE_SNOW:
			{
				GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A003"),vPos,pPlayer->GetObjectKey());
			}
			break;
		case eTERRAIN_ATTRIBUTE_DESERT:
			{
				GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A001"),vPos,pPlayer->GetObjectKey());
			}
			break;

		default:
			{
				GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A000"),vPos,pPlayer->GetObjectKey());
			}
			break;
		}



	}
	else
	{
		m_dwCopyTime-=dwTick;
	}

}	

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE StateSpecialMove::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Player*	pPlayer = (Player*)pOwner;
	assert(pPlayer->IsKindOfObject(PLAYER_OBJECT));
	WzEvent event;	
	pPlayer->m_dwSpecialMoveCurTime+=dwTick;

	pPlayer->ProcessInterpolation(dwTick);
	float fRet = pPlayer->ProcessAnimation(dwTick,&event);


	// 이동 중단 점 
	if (event.m_iNumCustom)
	{
		for (int a = 0; a < event.m_iNumCustom; a++)
		{
			switch(event.m_Custom[a].m_iaParam3[0])
			{
				
			case 1://이벤트가 잡히면 이동 중단.
				{
					pPlayer->m_bSpecialMoveMove=FALSE;
				}
				break;
			}
		}
	}


	//에니 데이터 수정될때까지 임시 
	const DWORD dwMoveTime=GetMoveTimeAsState(pPlayer->GetSpecialMoveFlag());

	if(pPlayer->m_dwSpecialMoveCurTime>=dwMoveTime)
	{
		pPlayer->m_bSpecialMoveMove=FALSE;
	}
	
	//기타 연출 
	if(pPlayer->m_bSpecialMoveMove)
		OnProcessEffect( pOwner,CurTime,dwTick);

	if(pPlayer->GetSpecialMoveFlag()==CMS_TUMBLING_LEFT||pPlayer->GetSpecialMoveFlag()==CMS_TUMBLING_RIGHT)
	{
		if (fRet >= 100.0f)
		{
			return STATEPROCESS::END;
		}
	}
	else if(!pPlayer->m_bSpecialMoveMove)
		return STATEPROCESS::END;



	pPlayer->SetVisibleAngleModifer(0.0f);

	WzVector vFoward = pPlayer->GetDirection();

	WzVector vRight,vLeft;
	vRight.x = vFoward.y; // sin(a - 90) = -cos(a) = vFoward.y;
	vRight.y = -vFoward.x; // -cos(a - 90) = -sin(a) = -vFoward.y;
	vRight.z = 0.0f;

	vLeft.x = -vFoward.y; // sin(a + 90) = cos(a) = -vFoward.y;
	vLeft.y = vFoward.x; // -cos(a + 90) = sin(a) = vFoward.x;
	vLeft.z = 0.0f;
	WzVector vDirection;
	SetVector(&vDirection,0.0f,0.0f,0.0f);

	if (pPlayer->GetSpecialMoveFlag()==CMS_TUMBLING_RIGHT)
	{
		vDirection =  vDirection + vRight;		
	}
	else if (pPlayer->GetSpecialMoveFlag()==CMS_TUMBLING_LEFT)
	{
		vDirection =  vDirection + vLeft;		
		
	}
	else if (pPlayer->GetSpecialMoveFlag()==CMS_TUMBLING_FRONT)
	{
		vDirection =  vDirection + vFoward;
	}
	else
	{
		vDirection =  vDirection - vFoward;
	}

#ifdef _NA_000000_20120410_SPEEDHACK
    if (pPlayer == g_pHero)
    {
        g_pHero->move_speed_ = GetBaseMoveSpeedAsState(pPlayer->GetSpecialMoveFlag());
    }
    else
    {
        pPlayer->SetMoveSpeed(GetBaseMoveSpeedAsState(pPlayer->GetSpecialMoveFlag()));	
    }
#else
	pPlayer->SetMoveSpeed(GetBaseMoveSpeedAsState(pPlayer->GetSpecialMoveFlag()));	
#endif //_NA_000000_20120410_SPEEDHACK

#ifdef _NA_000000_20120410_SPEEDHACK
    float fMoveSpeed = 0.0f;
    if (pPlayer == g_pHero)
    {
        float move_speed_modifier = 0.0f;
        if (g_pHero->IsRiding() || g_pHero->m_bCustomMoveSpeed)
        {
            move_speed_modifier =  pPlayer->GetMoveSpeedModifier();
        }
        else
        {
            HeroAttributes* const hero_attr = (HeroAttributes*)(g_pHero->GetPlayerAttribute());
            move_speed_modifier = hero_attr->move_speed_modifier_;
        }
        fMoveSpeed = g_pHero->move_speed_ * move_speed_modifier * (float)dwTick ;
    }
    else
    {
        fMoveSpeed = pPlayer->GetMoveSpeed() * pPlayer->GetMoveSpeedModifier() * (float)dwTick ;
    }
#else
	float fMoveSpeed = pPlayer->GetMoveSpeed() * pPlayer->GetMoveSpeedModifier() * (float)dwTick ;
#endif //_NA_000000_20120410_SPEEDHACK

	VectorNormalize(&vDirection,&vDirection);
	WzVector vCurrent = pPlayer->GetPathExplorer()->GetPos();
	WzVector vOffset = (vDirection * fMoveSpeed);
	WzVector vLongOffset = vOffset * 10.0f;

	if (pPlayer->m_bSpecialMoveMove)
	{
		CPathExplorer* pPathExp = pPlayer->GetPathExplorer();
		WzVector oldPos = pPathExp->GetPos();

		if (pPathExp->GetTile() != -1) 
		{
			SUNPROCESSPROFILE_BEGIN(_T("PathFinding"));
			
			g_pSunTerrain->PathExp_Thrust(pPathExp,&vLongOffset);
			g_pSunTerrain->PathExp_Progress(pPathExp, fMoveSpeed, NULL);

			SUNPROCESSPROFILE_END(_T("PathFinding"));
		}
	}



	return STATEPROCESS::HOLD;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateSpecialMove::OnEnd(Object* pOwner, DWORD CurTime)
{
	Player*	pPlayer = (Player*)pOwner;
	assert(pPlayer->IsKindOfObject(PLAYER_OBJECT));

	pPlayer->SetAttackStandTime(ATTACK_STAND_TIME);


	return STATECHANGE::SUCCESS;
}