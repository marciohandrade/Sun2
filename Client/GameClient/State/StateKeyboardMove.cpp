//------------------------------------------------------------------------------
//  StateKeyboardMove.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "statemove.h"
#include "statekeyboardmove.h"
#include "Hero.h"
#include "Player.h"
#include "Map.h"
#include "ObjectManager.h"
#include "Camera.h"
#include "GameFunc.h"
#include "NumericValues.h"
#include "GlobalFunc.h"
#include "InterfaceManager.h"
#include "HeroActionInput.h"
#include "StateKeyboardJump.h"
#include "Rider.h"
#include "KBMoveCheckManagerInClient.h"
#include "BattleScene.h"
#include "HeroAttributes.h"

//------------------------------------------------------------------------------
/**
*/
StateKeyboardMove::StateKeyboardMove(void)
{
}

//------------------------------------------------------------------------------
/**
*/
StateKeyboardMove::~StateKeyboardMove(void)
{
}

//------------------------------------------------------------------------------
/**
*/
BOOL StateKeyboardMove::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
{
	return TRUE;	
}




//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateKeyboardMove::OnStart(Object* pOwner, DWORD CurTime)
{
	Player*	pPlayer = (Player*)pOwner;

	
	if (pPlayer == g_pHero)
	{
		SetHeroKeyboardMoveValue();//자기 자신의 값 초기화 
	}
	
	SetKeyboardMoveValue(pPlayer);//플레이어 공통 속성 초기화 

    g_pHero->KBMove.is_first_kbmove = true;

	return STATECHANGE::SUCCESS;
}


//------------------------------------------------------------------------------
/**
자기 자신의 값 이동 속성, 패킷 주기,방향각등을 초기화 한다.
*/
void StateKeyboardMove::SetHeroKeyboardMoveValue()
{
	// Moveflag가 바뀌었는지 체크하는 부분이 무조건 성공하게 하도록해서 시작시 패킷을 보내도록 유도한다.
	g_pHero->SetOldMoveFlag(0xffffffff);
	g_pHero->SetKeyboardMovePacketSendDuration(0);
	g_pHero->SetKeyboardMovePacketSendTime(0);

	//카메라를 따라 회전할 경우
	if(g_HeroInput.GetCameraBaseMove()) 
		g_pHero->SetAngle(g_Camera.GetAngle());

#ifdef PERF_AUTO_MOVE_TEST
	g_HeroInput.SetAutoMove(FALSE);
#endif 

}

//------------------------------------------------------------------------------
/**
플레이어 공통 속성,이속,회전 액셀값등 
*/
void  StateKeyboardMove::SetKeyboardMoveValue(Player* pPlayer)
{
	// 맨처음 길찾기 체크가 무조건 성공하게 하도록해서 PathThrust를 하도록 유도한다.
	WzVector vDummy;
	vDummy.x = 0.0f;
	vDummy.y = 0.0f;
	vDummy.z = 0.0f;
	pPlayer->SetOldKeyboardDirection(&vDummy); 
	pPlayer->SetKeyboardMoveTime(0);
	

#ifdef _NA_000000_20120410_SPEEDHACK
    if (pPlayer == g_pHero)
    {
        g_pHero->move_speed_ = GetBaseMoveSpeedAsState(CMS_RUN);
    }
    else
    {
        pPlayer->SetMoveSpeed( GetBaseMoveSpeedAsState(CMS_RUN));
    }
#else
    pPlayer->SetMoveSpeed( GetBaseMoveSpeedAsState(CMS_RUN));
#endif //_NA_000000_20120410_SPEEDHACK

	
	pPlayer->m_fTurnAccelerate = 0.0f;

}

//------------------------------------------------------------------------------
/**
	액션 큐에 녹백,띄우기등의 동작이 있는지 검사 한다.
*/
BOOL StateKeyboardMove::CheckActionQueue(Player* pPlayer)
{
	if (pPlayer->CommonStateProcess())
	{
		return TRUE;
	}

	return FALSE;
}

//------------------------------------------------------------------------------
/**
*/
BOOL StateKeyboardMove::SendKeyboardMovePacket(Player* pPlayer,DWORD dwTick)
{
	if (pPlayer != g_pHero)
	{		
		return FALSE;
	}

    BOOL rt = FALSE;
    bool is_need_distance_check = false;

	g_pHero->AddKeyboardMovePacketSendTime(dwTick); 

	if (g_pHero->GetMoveFlag() != g_pHero->GetOldMoveFlag())
	{
		rt= TRUE;

        if (g_pHero->KBMove.is_first_kbmove == false)
        {
            is_need_distance_check = true;
        }
	}
	else if (g_pHero->GetKeyboardMovePacketSendTime() >= g_pHero->GetKeyboardMovePacketSendDuration())
	{
		rt= TRUE;
	}
    

#ifdef _NA000000_SPEED_HACK_PREVENTION_SUPPORT
    if ((rt == FALSE) && (g_pHero->KBMove.last_sent_move_flag != g_pHero->GetMoveFlag()))
    {
        rt = TRUE;
        if (g_pHero->KBMove.is_first_kbmove == false)
        {
            is_need_distance_check = true;
        }
    }

    //! 20도  20/180*3.14 = 0.34f
    float ang_gab = abs(g_pHero->KBMove.ang - g_pHero->GetAngle()); 
    if ((rt == FALSE) && (ang_gab > 0.34f)) 
    {
        rt= TRUE;
        is_need_distance_check = true;
    }
    if ((rt == TRUE) && (is_need_distance_check == true))
    {
        float fMoveSpeed = pPlayer->GetMoveSpeed() * pPlayer->GetMoveSpeedModifier();

        float dist_gab = (fMoveSpeed / 0.01f) * (fMoveSpeed / 0.01f) * 10.0f; 


        if (g_pHero->KBMove.last_sent_move_flag != g_pHero->GetMoveFlag())
        {
            dist_gab = min(2, dist_gab);
        }
        else
        {
            dist_gab = min(6.0f, dist_gab);
            //if (ang_gab < WZ_PI / 4.0f) dist_gab = min(10, dist_gab);
            //else if (ang_gab < WZ_PI / 2.0f) dist_gab = min(7, dist_gab);
            //else if (ang_gab < WZ_PI / 1.5f) dist_gab = min(5, dist_gab);
            //else dist_gab = min(3, dist_gab);
        }

        WzVector gab = g_pHero->KBMove.pos - g_pHero->GetPosition(); 
        float gab_len = VectorLength(&gab);
        if (gab_len < dist_gab)
        {
            g_pHero->KBMove.debug_move_speed = fMoveSpeed;
            g_pHero->KBMove.debug_kbmove_skip_count += 1;
            rt = FALSE;
        }
        g_pHero->KBMove.debug_kbmove_gab = gab_len;
    }
    if (rt)
    {
        DWORD tick_gab = clock_function::GetTickCount() - g_pHero->KBMove.sent_tick;
        if (tick_gab > 100)
        {
            return SEND_CG_SYNC_KBMOVE_SYN();
        }
    }
#else
	if(rt)
	{
		MSG_CG_SYNC_KBMOVE_SYN Packet;
		Packet.m_byCategory = CG_SYNC;
		Packet.m_byProtocol = CG_SYNC_KBMOVE_SYN;
		Packet.m_byMoveState = (BYTE)g_pHero->GetMoveFlag();
		Packet.wvCurPos = g_pHero->GetPosition();
		Packet.m_wAngle = (WORD) (g_pHero->GetAngle() * 180.0f / WZ_PI);
		Packet.m_wTileIndex = g_pHero->GetPathExplorer()->GetTile();

		Packet.Encode();

		g_pHero->SendPacket(&Packet,sizeof(Packet));
		g_pHero->SetOldMoveFlag(g_pHero->GetMoveFlag());
		g_pHero->SetKeyboardMovePacketSendDuration(500);
		g_pHero->SetKeyboardMovePacketSendTime(0);
		WzVector vDummy;
		vDummy.x = 0.0f;
		vDummy.y = 0.0f;
		vDummy.z = 0.0f;

		g_pHero->SetOldKeyboardDirection(&vDummy); 
	}
#endif //_NA000000_SPEED_HACK_PREVENTION_SUPPORT

	return rt;
}

//------------------------------------------------------------------------------
/**
*/
void StateKeyboardMove::CalculateTurnAccelerate(Player* pPlayer,DWORD dwTick)
{
	const  float l_fSpeedAccelerator = 0.002f;
	float  fTurnSpeed = 0.0f;
	float  fMaxTurnSpeed = 0.04f;

	if (pPlayer == g_pHero)
	{
		int oldturnflag = g_pHero->GetOldMoveFlag() & (KEYBOARDMOVE_LEFTTURN | KEYBOARDMOVE_RIGHTTURN);
		int newturnflag = g_pHero->GetMoveFlag() & (KEYBOARDMOVE_LEFTTURN | KEYBOARDMOVE_RIGHTTURN);

		if (oldturnflag == newturnflag && newturnflag)
		{
			if (newturnflag & KEYBOARDMOVE_LEFTTURN)
			{
				pPlayer->m_fTurnAccelerate += l_fSpeedAccelerator;
			}
			else 
			{
				pPlayer->m_fTurnAccelerate -= l_fSpeedAccelerator;
			}
		}
		else
		{
			pPlayer->m_fTurnAccelerate *= 0.7f;
			if (fabs(pPlayer->m_fTurnAccelerate) <= (l_fSpeedAccelerator * 0.1f))
			{
				pPlayer->m_fTurnAccelerate = 0.0f;
			}
		}

		if (pPlayer->GetMoveFlag() & (KEYBOARDMOVE_LEFTTURN | KEYBOARDMOVE_RIGHTTURN))
		{
			if (g_pHero->GetMoveFlag() & KEYBOARDMOVE_LEFTTURN)
			{
				pPlayer->m_fTurnAccelerate = l_fSpeedAccelerator;
			}
			else 
			{
				pPlayer->m_fTurnAccelerate = -l_fSpeedAccelerator;
			}
		}
		else
		{
			pPlayer->m_fTurnAccelerate = 0.0f;
		}		
	}
	else
	{
		if (pPlayer->GetMoveFlag() & (KEYBOARDMOVE_LEFTTURN | KEYBOARDMOVE_RIGHTTURN))
		{
			if (pPlayer->GetMoveFlag() & KEYBOARDMOVE_LEFTTURN)
			{
				pPlayer->m_fTurnAccelerate = l_fSpeedAccelerator;
			}
			else 
			{
				pPlayer->m_fTurnAccelerate = -l_fSpeedAccelerator;
			}
		}
		else
		{
			pPlayer->m_fTurnAccelerate = 0.0f;
		}
	}

	fTurnSpeed += pPlayer->m_fTurnAccelerate;

	if (fTurnSpeed >= fMaxTurnSpeed)
	{
		fTurnSpeed = fMaxTurnSpeed;
	}
	else if (fTurnSpeed <= -fMaxTurnSpeed)
	{
		fTurnSpeed = -fMaxTurnSpeed;
	}


	if (fabs(fTurnSpeed) > 0.0f) 
	{
		pPlayer->Rotate(fTurnSpeed * dwTick);		
	}

}

//------------------------------------------------------------------------------
/**
*/
void StateKeyboardMove::CheckHeroCameraRotateOption()
{
	if( g_Camera.IsRotate() && g_Camera.RotateMouseState() == Camera::CAMERA_ROTATE_LMOUSE )
	{
		g_pHero->SetDirection( g_Camera.GetCameraTo(), g_Camera.GetCameraFrom(), FALSE );
	}

}

//------------------------------------------------------------------------------
/**
*/
//BOOL StateKeyboardMove::CalculateValue(Player* pPlayer,DWORD dwTick,WzID& wzGetMoveAnimation,float&  fGetAnimMultiplier,WzVector& vGetDirection)
BOOL    StateKeyboardMove::CalculateValue(Player* pPlayer, DWORD dwTick, 
                                           OUT WzID& wzGetMoveAnimation, 
                                           OUT WzVector& vGetDirection,
                                           OUT ePLAYER_ANI& eAni)
{

	WzVector vPos = pPlayer->GetPosition();
	WzVector vFoward = pPlayer->GetDirection();
	float    fAngle = pPlayer->GetAngle();
	float    fAngleRight = fAngle - WZ_PI / 2.0f;
	float    fAngleLeft = fAngle + WZ_PI / 2.0f;
	WzVector vRight,vLeft;

	// 오른쪽이 마이너스다.
	vRight.x = vFoward.y;  // sin(a - 90) = -cos(a) = vFoward.y;
	vRight.y = -vFoward.x; // -cos(a - 90) = -sin(a) = -vFoward.y;
	vRight.z = 0.0f;
	vLeft.x  = -vFoward.y; // sin(a + 90) = cos(a) = -vFoward.y;
	vLeft.y  = vFoward.x;  // -cos(a + 90) = sin(a) = vFoward.x;
	vLeft.z  = 0.0f;

	float fSideStepSpeed = GetBaseMoveSpeedAsState(CMS_SIDESTEP);
	pPlayer->SetVisibleAngleModifer(0.0f);


	BOOL bUseOneAnimation = FALSE;
	if( pPlayer->IsPolymorph() == TRUE )
	{
		bUseOneAnimation = TRUE;
	}

	BOOL bMoved = FALSE;

	if (pPlayer->GetMoveFlag() & KEYBOARDMOVE_FOWARD) 
	{		
		vGetDirection =  vGetDirection + vFoward;
		bMoved = TRUE;
		wzGetMoveAnimation = pPlayer->GetCurrentMoveAnimation();

#ifdef _NA_000000_20120410_SPEEDHACK
        if (pPlayer == g_pHero)
        {
            g_pHero->move_speed_ = GetBaseMoveSpeedAsState(CMS_RUN);
        }
        else
        {
            pPlayer->SetMoveSpeed( GetBaseMoveSpeedAsState(CMS_RUN));
        }
#else
        pPlayer->SetMoveSpeed( GetBaseMoveSpeedAsState(CMS_RUN));
#endif //_NA_000000_20120410_SPEEDHACK

		
    	eAni = eANI_RUN;
	}
	else if (pPlayer->GetMoveFlag() & KEYBOARDMOVE_BACKWARD)
	{
		vGetDirection =  vGetDirection - vFoward;
		bMoved = TRUE;
		wzGetMoveAnimation = pPlayer->GetCurrentBackStepAnimation();		

#ifdef _NA_000000_20120410_SPEEDHACK
        if (pPlayer == g_pHero)
        {
            g_pHero->move_speed_ = GetBaseMoveSpeedAsState(CMS_BACKSTEP);
        }
        else
        {
            pPlayer->SetMoveSpeed( GetBaseMoveSpeedAsState(CMS_BACKSTEP));
        }
#else
        pPlayer->SetMoveSpeed( GetBaseMoveSpeedAsState(CMS_BACKSTEP));
#endif //_NA_000000_20120410_SPEEDHACK

		
		eAni = eANI_B_MOVE;

		if( bUseOneAnimation == TRUE )
		{
			pPlayer->SetVisibleAngleModifer( WZ_PI );
		}
	}

	if (pPlayer->GetMoveFlag() & KEYBOARDMOVE_RIGHTSTEP)
	{
		vGetDirection =  vGetDirection + vRight;		

		// -45도
		if (pPlayer->GetMoveFlag() & KEYBOARDMOVE_FOWARD)
		{			
			pPlayer->LockOnPosition(FALSE,vPos);
			pPlayer->SetVisibleAngleModifer(-(WZ_PI / 4.0f));

#ifdef _NA_000000_20120410_SPEEDHACK
            if (pPlayer == g_pHero)
            {
                g_pHero->move_speed_ = GetBaseMoveSpeedAsState(CMS_RUN);
            }
            else
            {
                pPlayer->SetMoveSpeed( GetBaseMoveSpeedAsState(CMS_RUN));
            }
#else
            pPlayer->SetMoveSpeed(GetBaseMoveSpeedAsState(CMS_RUN) );
#endif //_NA_000000_20120410_SPEEDHACK

		
			eAni = eANI_RUN;
		}	
		else if (pPlayer->GetMoveFlag() & KEYBOARDMOVE_BACKWARD)
		{
			pPlayer->LockOnPosition(FALSE,vPos);
			pPlayer->SetVisibleAngleModifer((WZ_PI / 4.0f));
			wzGetMoveAnimation = pPlayer->GetCurrentBackStepAnimation();

#ifdef _NA_000000_20120410_SPEEDHACK
            if (pPlayer == g_pHero)
            {
                g_pHero->move_speed_ = GetBaseMoveSpeedAsState(CMS_BACKSTEP);
            }
            else
            {
                pPlayer->SetMoveSpeed( GetBaseMoveSpeedAsState(CMS_BACKSTEP));
            }
#else
            pPlayer->SetMoveSpeed( GetBaseMoveSpeedAsState(CMS_BACKSTEP) );
#endif //_NA_000000_20120410_SPEEDHACK

			eAni = eANI_B_MOVE;

			if( bUseOneAnimation == TRUE )
			{
				pPlayer->SetVisibleAngleModifer( WZ_PI * 1.25f );
			}

		}
		else
		{
			pPlayer->LockOnPosition(FALSE,vPos);
			wzGetMoveAnimation = pPlayer->GetCurrentRightStepAnimation();

#ifdef _NA_000000_20120410_SPEEDHACK
            if (pPlayer == g_pHero)
            {
                g_pHero->move_speed_ = GetBaseMoveSpeedAsState(CMS_SIDESTEP);
            }
            else
            {
                pPlayer->SetMoveSpeed( GetBaseMoveSpeedAsState(CMS_SIDESTEP));
            }
#else
            pPlayer->SetMoveSpeed( GetBaseMoveSpeedAsState(CMS_SIDESTEP));
#endif //_NA_000000_20120410_SPEEDHACK

			eAni = eANI_R_MOVE;

			if( bUseOneAnimation == TRUE )
			{
				pPlayer->SetVisibleAngleModifer( WZ_PI*1.5f);
			}

		}
		bMoved = TRUE;
	}
	else if (pPlayer->GetMoveFlag() & KEYBOARDMOVE_LEFTSTEP)
	{
		vGetDirection =  vGetDirection + vLeft;		
		if (pPlayer->GetMoveFlag() & KEYBOARDMOVE_FOWARD)
		{
			pPlayer->LockOnPosition(FALSE,vPos);
			pPlayer->SetVisibleAngleModifer((WZ_PI / 4.0f));		

#ifdef _NA_000000_20120410_SPEEDHACK
            if (pPlayer == g_pHero)
            {
                g_pHero->move_speed_ = GetBaseMoveSpeedAsState(CMS_RUN);
            }
            else
            {
                pPlayer->SetMoveSpeed( GetBaseMoveSpeedAsState(CMS_RUN));
            }
#else
            pPlayer->SetMoveSpeed( GetBaseMoveSpeedAsState(CMS_RUN));
#endif //_NA_000000_20120410_S

			
			eAni = eANI_RUN;
		}
		else if (pPlayer->GetMoveFlag() & KEYBOARDMOVE_BACKWARD)
		{
			pPlayer->LockOnPosition(FALSE,vPos);
			pPlayer->SetVisibleAngleModifer(-(WZ_PI / 4.0f));
			wzGetMoveAnimation = pPlayer->GetCurrentBackStepAnimation();		

#ifdef _NA_000000_20120410_SPEEDHACK
            if (pPlayer == g_pHero)
            {
                g_pHero->move_speed_ = GetBaseMoveSpeedAsState(CMS_BACKSTEP);
            }
            else
            {
                pPlayer->SetMoveSpeed(GetBaseMoveSpeedAsState(CMS_BACKSTEP));
            }
#else
            pPlayer->SetMoveSpeed( GetBaseMoveSpeedAsState(CMS_BACKSTEP));
#endif //_NA_000000_20120410_S

			
			eAni = eANI_B_MOVE;

			if( bUseOneAnimation == TRUE )
			{
				pPlayer->SetVisibleAngleModifer( WZ_PI*0.75f);
			}
		}		
		// 90도
		else
		{
			pPlayer->LockOnPosition(FALSE,vPos);
			wzGetMoveAnimation = pPlayer->GetCurrentLeftStepAnimation();

#ifdef _NA_000000_20120410_SPEEDHACK
            if (pPlayer == g_pHero)
            {
                g_pHero->move_speed_ = GetBaseMoveSpeedAsState(CMS_SIDESTEP);
            }
            else
            {
                pPlayer->SetMoveSpeed(GetBaseMoveSpeedAsState(CMS_SIDESTEP));
            }
#else
            pPlayer->SetMoveSpeed( GetBaseMoveSpeedAsState(CMS_SIDESTEP));
#endif //_NA_000000_20120410_S

			eAni = eANI_L_MOVE;

			if( bUseOneAnimation == TRUE )
			{
				pPlayer->SetVisibleAngleModifer( WZ_PI*0.5f);
			}
		}		
		bMoved = TRUE;
	}
	else
	{
		pPlayer->LockOnPosition(FALSE,vPos);
	}

	if (!bMoved)
	{
		if (pPlayer->GetMoveFlag() & KEYBOARDMOVE_LEFTTURN)
		{
			wzGetMoveAnimation = pPlayer->GetCurrentLeftTurnAnimation();
			eAni = eANI_L_ROTATE;
		}
		else if (pPlayer->GetMoveFlag() & KEYBOARDMOVE_RIGHTTURN)
		{
			wzGetMoveAnimation = pPlayer->GetCurrentRightTurnAnimation();
			eAni = eANI_R_ROTATE;
		}
	}


    //! 날기 상태(idle/move) 있을대 다른 모션 체크
    wzGetMoveAnimation = pPlayer->GetCurrentSpreadOtherAnimation(wzGetMoveAnimation, eAni);

	return bMoved;
}

//------------------------------------------------------------------------------
/**
*/
BOOL StateKeyboardMove::ThrustCanBeMove(Player* pPlayer,DWORD dwTick,WzVector oldPos,int iOldTile)
{
	WzVector newPos = oldPos;
	CPathExplorer* pPathExp = pPlayer->GetPathExplorer();

#ifdef _NA_000000_20120410_SPEEDHACK
    float fMoveSpeed = 0.0f;
    if (pPlayer == g_pHero)
    {
        fMoveSpeed = g_pHero->move_speed_ * (float)dwTick;
    }
    else
    {
        fMoveSpeed = pPlayer->GetMoveSpeed() * pPlayer->GetMoveSpeedModifier() * (float)dwTick ;
    }
#else
    float fMoveSpeed = pPlayer->GetMoveSpeed() * pPlayer->GetMoveSpeedModifier() * (float)dwTick ;
#endif //_NA_000000_20120410_SPEEDHACK

	

	WzVector vCurrent ;
	WzVector vOffset;
	WzVector vLongOffset; 
	
	WzVector vPreLongOffset; 
	WzVector vPreOldPos;

	if (!(pPlayer->GetMoveFlag() & KEYBOARDMOVE_RIGHTSTEP))
	{
		float fSideAngle = WZ_PI / 6;
		float fAngleAlpha = WZ_PI / 18;

		// 왼쪽 : 오른쪽 이동이 아니어야한다.

		// 90도 까지 사용
		while (fSideAngle < WZ_PI / 2)
		{
			float fAngle = pPlayer->GetAngle() + fSideAngle;
			ClampAngle(&fAngle);
			WzVector vDirection;
			vDirection.x = sin(fAngle);
			vDirection.y = -cos(fAngle);
			vDirection.z = 0.0f;
			VectorNormalize(&vDirection,&vDirection);
			vCurrent = pPathExp->GetPos();
			vOffset = (vDirection * fMoveSpeed);
			vLongOffset = vOffset * 10.0f;
			vPreOldPos=pPathExp->GetPos();
			vPreLongOffset=vLongOffset;
			g_pSunTerrain->PathExp_Thrust(pPathExp,&vLongOffset,PTA_NO_WALK);

			BOOL rt = g_pSunTerrain->PathExp_Progress(pPathExp, fMoveSpeed, NULL);

			newPos = pPathExp->GetPos();

			if (oldPos != newPos&&rt)
			{
				pPlayer->SetOldKeyboardDirection(&vDirection);
				break;
			}

			fSideAngle += fAngleAlpha;
		}
	}

	// 오른쪽 : 왼쪽 이동이 아니어야한다.
	if (!(pPlayer->GetMoveFlag() & KEYBOARDMOVE_LEFTSTEP) && oldPos == newPos)
	{
		float fSideAngle = WZ_PI / 6;
		float fAngleAlpha = WZ_PI / 18;

		// 90도 까지 사용
		while (fSideAngle < WZ_PI / 2)
		{
			float fAngle = pPlayer->GetAngle() - fSideAngle;
			ClampAngle(&fAngle);

			WzVector vDirection;
			vDirection.x = sin(fAngle);
			vDirection.y = -cos(fAngle);
			vDirection.z = 0.0f;

			VectorNormalize(&vDirection,&vDirection);
			vCurrent = pPathExp->GetPos();
			vOffset = (vDirection * fMoveSpeed);
			vLongOffset = vOffset * 10.0f;
			vPreLongOffset=vLongOffset;
			vPreOldPos=pPathExp->GetPos();
			g_pSunTerrain->PathExp_Thrust(pPathExp,&vLongOffset,PTA_NO_WALK);

			BOOL rt = g_pSunTerrain->PathExp_Progress(pPathExp, fMoveSpeed, NULL);

			newPos = pPathExp->GetPos();

			if (oldPos != newPos&&rt)
			{
				pPlayer->SetOldKeyboardDirection(&vDirection);
				break;
			}

			fSideAngle += fAngleAlpha;
		}
	}

	BOOL bMoved = FALSE;
	WzID wzFinalMoveAnimation;
	WzVector vDirection;
	//float fAnimMultiplier;
    ePLAYER_ANI eAni = eANI_RUN;
	SetVector(&vDirection,0.0f,0.0f,0.0f);

	//이동 속도,에니메이션,방향 백터등을 얻어 온다.
	bMoved=CalculateValue(pPlayer,dwTick,wzFinalMoveAnimation, vDirection, eAni);

	WzVector oldPos2=pPathExp->GetPos();

#ifdef _NA_000000_20120410_SPEEDHACK
    if (pPlayer == g_pHero)
    {
        fMoveSpeed = g_pHero->move_speed_ * (float)dwTick ;
    }
    else
    {
        fMoveSpeed = pPlayer->GetMoveSpeed() * pPlayer->GetMoveSpeedModifier() * (float)dwTick ;
    }
#else
    fMoveSpeed = pPlayer->GetMoveSpeed() * pPlayer->GetMoveSpeedModifier() * (float)dwTick ;
#endif //_NA_000000_20120410_SPEEDHACK

	
	vLongOffset = vDirection*10.0f;

	g_pSunTerrain->PathExp_Thrust(pPathExp,&vLongOffset,PTA_NO_WALK);
	
	BOOL rt = g_pSunTerrain->PathExp_Progress(pPathExp, 1000000, NULL);

	newPos = pPathExp->GetPos();


	if (oldPos2 != newPos&&rt)
	{
		vDirection=newPos-oldPos;
		float fLen=VectorLength(&vDirection);

		if(fLen<3.0f)
		{
			VectorNormalize(&vDirection,&vDirection);


			if(g_pSunTerrain->PathExp_Reset(pPathExp,&oldPos,iOldTile))
			{
				vLongOffset = vDirection * fLen;
				g_pSunTerrain->PathExp_Thrust(pPathExp,&vLongOffset,PTA_NO_WALK);

				g_pSunTerrain->PathExp_Progress(pPathExp, fMoveSpeed, NULL);

			}
			
		}
		else
		{

			if(g_pSunTerrain->PathExp_Reset(pPathExp,&oldPos,iOldTile))
			{
				g_pSunTerrain->PathExp_Thrust(pPathExp,&vPreLongOffset,PTA_NO_WALK);
		
				g_pSunTerrain->PathExp_Progress(pPathExp, fMoveSpeed, NULL);

			}
			
		}
		
	}
	
	return FALSE;
}


//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE StateKeyboardMove::OnProcess(Object* pOwner, DWORD CurTime, DWORD dwTick)
{
	Player* pPlayer = (Player*)pOwner;

	float  fAnimMultiplier = 1.0f;
	CPathExplorer* pPathExp = pPlayer->GetPathExplorer();
	WzVector oldPos = pPathExp->GetPos();
	int      iOldTile = pPathExp->GetTile();
	//
	pPlayer->AddKeyboardMoveTime(dwTick);

	int iDestTileFocussing = g_pSunTerrain->x_pPathFinder->GetTileToStand( pPlayer->GetPosition(), NULL, -1.0f, 100.0f );
	if( iDestTileFocussing >= 0 )
	{
		WORD Attribute = g_pMap->GetWorldBase()->x_pPathFinder->GetAttribute( iDestTileFocussing );

		if (Attribute & (PTA_PLAYER_FALLDOWN_TILE))
		{
			pPlayer->SetNextState(STATE::FALL_DOWN, g_CurTime);		
			return STATEPROCESS::SUSPEND;
		}
	}
#ifdef _NA000000_SPEED_HACK_PREVENTION_SUPPORT
    if (pPlayer == g_pHero)
    {
        if (g_pHero->PacketNak.is_recv_nak == true)
        {
            g_pHero->ResetMoveFlag();
            //return STATEPROCESS::SUSPEND;
        }
    }
#endif //_NA000000_SPEED_HACK_PREVENTION_SUPPORT

	if(pPlayer->IsMoving())
	{
		if (pPathExp->GetTile() != -1) 
		{
#ifdef _NA_000000_20120410_SPEEDHACK
            float fMoveSpeed = 0.0f;
            if (pPlayer == g_pHero)
            {
#ifdef _NA_000000_20131022_RECIEVE_SPEED_FROM_SERVER
                float move_speed_modifier = pPlayer->GetMoveSpeedModifier();
#else //_NA_000000_20131022_RECIEVE_SPEED_FROM_SERVER
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
#endif //_NA_000000_20131022_RECIEVE_SPEED_FROM_SERVER
                fMoveSpeed = g_pHero->move_speed_ * move_speed_modifier * (float)dwTick;
            }
            else
            {
                fMoveSpeed = pPlayer->GetMoveSpeed() * pPlayer->GetMoveSpeedModifier() * (float)dwTick;
            }
#else //_NA_000000_20120410_SPEEDHACK
            float fMoveSpeed = pPlayer->GetMoveSpeed() * pPlayer->GetMoveSpeedModifier() * (float)dwTick;
#endif //_NA_000000_20120410_SPEEDHACK

			g_pSunTerrain->PathExp_Progress(pPathExp, fMoveSpeed, NULL);

		}	
	}
		
	if(!(pPlayer->GetMoveFlag()&KEYBOARDMOVE_ISMOVING))
	{
		pPlayer->StopMove();
	}
	else if(pPlayer==g_pHero)
	{
		WzVector newPos = pPathExp->GetPos();
		WzVector vDummy;
		vDummy.x = 0.0f;
		vDummy.y = 0.0f;
		vDummy.z = 0.0f;
		// 자기 자신이 못가는 곳으로 갈려고 할땐 민다.
		if(newPos==oldPos&&vDummy!=pPlayer->GetOldKeyboardDirection())
		{
			ThrustCanBeMove(pPlayer,dwTick,oldPos,iOldTile);
		}
	}

	// 액션 큐에 해야 할 행동이 있을 경우 키보드 이동을 멈춘다.
	if(CheckActionQueue(pPlayer))
	{
		return STATEPROCESS::SUSPEND;
	}

	// 주기 체크 및 회전 엑셀 검사 
	if (pPlayer == g_pHero)
	{
		CheckHeroCameraRotateOption();
	}

	// 회전 관련 액셀 값을 세팅 한다.
	CalculateTurnAccelerate(pPlayer,dwTick);

	BOOL bMoved = FALSE;
	WzID wzFinalMoveAnimation = pPlayer->GetCurrentMoveAnimation();
	WzVector vDirection;
	SetVector(&vDirection,0.0f,0.0f,0.0f);

	//이동 속도,에니메이션,방향 백터등을 얻어 온다.
    ePLAYER_ANI eAni = eANI_RUN;
	bMoved=CalculateValue(pPlayer,dwTick,wzFinalMoveAnimation, vDirection, eAni);
	

#ifdef _NA_000000_20120410_SPEEDHACK
    float fMoveSpeed = 0.0f;
    if (pPlayer == g_pHero)
    {
#ifdef _NA_000000_20131022_RECIEVE_SPEED_FROM_SERVER
        float move_speed_modifier = pPlayer->GetMoveSpeedModifier();
#else //_NA_000000_20131022_RECIEVE_SPEED_FROM_SERVER
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
#endif //_NA_000000_20131022_RECIEVE_SPEED_FROM_SERVER
        fMoveSpeed = g_pHero->move_speed_ * move_speed_modifier;
    }
    else
    {
        fMoveSpeed = pPlayer->GetMoveSpeed() * pPlayer->GetMoveSpeedModifier();
    }
#else
	float fMoveSpeed = pPlayer->GetMoveSpeed() * pPlayer->GetMoveSpeedModifier();
#endif //_NA_000000_20120410_SPEEDHACK

	pPlayer->SetPlayerAnimation(wzFinalMoveAnimation, eAni, TRUE);
	VectorNormalize(&vDirection,&vDirection);



	SendKeyboardMovePacket(pPlayer, dwTick);

	pPlayer->ProcessInterpolation(dwTick);

#ifdef _NA_000000_20120410_SPEEDHACK
    if (pPlayer == g_pHero)
    {
#ifdef _NA_000000_20131022_RECIEVE_SPEED_FROM_SERVER
        float move_speed_modifier = pPlayer->GetMoveSpeedModifier();
#else //_NA_000000_20131022_RECIEVE_SPEED_FROM_SERVER
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
#endif //_NA_000000_20131022_RECIEVE_SPEED_FROM_SERVER

        pPlayer->ProcessAnimation(DWORD( (float)dwTick * fAnimMultiplier * move_speed_modifier));

    }
    else
    {
        pPlayer->ProcessAnimation(DWORD( (float)dwTick * fAnimMultiplier * pPlayer->GetMoveSpeedModifier()));
    }
#else
    pPlayer->ProcessAnimation(DWORD( (float)dwTick * fAnimMultiplier * pPlayer->GetMoveSpeedModifier()));
#endif //_NA_000000_20120410_SPEEDHACK

	

	if (bMoved) 
	{
        // 앞으로 갈수 있는 위치가 있으면 자동점프
        float fDistance = 10.0f;
        if(fMoveSpeed)
        {
            fDistance = min(10.0f,500.0f*fMoveSpeed);
        }

        WzVector vLongOffset = vDirection * fDistance;

		CPathExplorer* pExp = pPlayer->GetPathExplorer();
        bool is_check_wing_jump = false;
		if( pExp->GetTile()!=-1 && vDirection != pPlayer->GetOldKeyboardDirection() ) 
		{
			ENUM_FINDPATH_RESULT path_result = g_pSunTerrain->PathExp_Thrust( pPlayer->GetPathExplorer(), &vLongOffset, PTA_NO_WALK );
            if (path_result == EFR_FAILURE)
            {
                if (((pPlayer == g_pHero) && g_pHero->is_spread_wings()) || 
                    ((pPlayer == g_pHero) && g_pHero->IsRiding() && g_pHero->rider() && g_pHero->rider()->is_spread()))
                {
                    is_check_wing_jump = true;
                }
            }
			pPlayer->SetOldKeyboardDirection( &vDirection );
		}

        if (is_check_wing_jump == false && (pPlayer == g_pHero) && g_pHero->is_spread_wings())
        {
            //! 조금 앞에 못가는 타일인지 체크
            WzVector cur_pos = pPlayer->GetPathExplorer()->GetPos() + (vLongOffset * 0.2f);
            if (GameFunc::IsPossibleStand(cur_pos, 3, PTA_NO_WALK) == false)
            {
                is_check_wing_jump = true;
            }
        }
        if (is_check_wing_jump)
        {
#ifdef _NA_20120220_JUMP_RENEWAL
            WzVector direction;
            if (StateKeyboardJump::GetJumpDirection(direction, g_pHero->GetMoveFlag(), g_pHero->GetDirection()) == TRUE)
            {
                // 점프의 목적지, 시작위치, 방향을 설정 (목적지는 수평점프 거리에 피킹한 위치 )
                WzVector vTargetPosition;
                if( StateKeyboardJump::SimulateJump( g_pHero->GetVisiblePos(), direction, vTargetPosition, g_pHero->m_JumpData.m_JumpPositionArray ) == TRUE )
                {
                    // 설정
                    g_pHero->m_JumpData.m_vJumpStartPosition = g_pHero->GetVisiblePos();
                    g_pHero->m_JumpData.m_vJumpTargetPosition = vTargetPosition;
                    g_pHero->m_JumpData.m_vJumpDirection = direction;

                    g_pHero->SetPosition(g_pPathExplorer->GetPos());
					g_pHero->m_JumpData.m_JumpFlag.SetMoveFlag( g_pHero->GetMoveFlag() );

                    g_pHero->SetNextState(STATE::KEYBOARDJUMP, g_CurTime);
                    return STATEPROCESS::SUSPEND;	
                }
            }
#else
            WzVector direction;
            if (StateKeyboardJump::GetJumpDirection(direction, g_pHero->GetMoveFlag(), g_pHero->GetDirection()) == true)
            {
                WzVector dist_pos;
                bool is_res = StateKeyboardJump::GetJumpDestPosByDirection(dist_pos, 
                    g_pHero->GetVisiblePos(), 
                    direction,
                    3, 
                    3.0f, 
                    g_pHero->GetJumpDistance());

                if (is_res) 
                {
                    if (GameFunc::IsIntersect(g_pHero->GetVisiblePos(), dist_pos) == false)
                    {
                        //네비게이션 메쉬가 위로 많이 뜰경우 리셋이 실패 할수 있다.
                        if(g_pSunTerrain->PathExp_Reset(g_pPathExplorer, &dist_pos, -1) == TRUE)
                        {
                            //일정 거리앞에 갈수 있는 타일이 있는지 체크 한다.
                            g_pHero->m_JumpData.m_JumpStartPosition = g_pHero->GetVisiblePos();
                            g_pHero->SetPosition(g_pPathExplorer->GetPos());
							g_pHero->m_JumpData.m_JumpFlag.SetMoveFlag( g_pHero->GetMoveFlag() );
                            //g_pHero->set_wing_jump(true);

                            g_pHero->SetNextState(STATE::KEYBOARDJUMP, g_CurTime);
                            return STATEPROCESS::SUSPEND;	
                        }
                    }
                }
            }
#endif
        }
	}
	
	// 다음 동작이 있으면 바로 수행해주세요~
	if (pPlayer != g_pHero) 
	{
		//목적지 까지 갔지만 다음 동작이 안올 경우(랙) 
		if(!pPlayer->IsMoving()&&pPlayer->GetKeyboardMoveTime()>=3000)
		{
			pPlayer->SetMoveFlag(pPlayer->GetMoveFlag()&(~KEYBOARDMOVE_ISMOVING));
		}

		if (pPlayer->GetActionQueueCount())
		{
			if (pPlayer->ProcessNextAction())
			{
				return STATEPROCESS::SUSPEND;		
			}			
		}
	}

	
	if (!pPlayer->GetMoveFlag() && pPlayer->m_fTurnAccelerate == 0.0f)
	{
		pPlayer->SetNextState(STATE::IDLE,FALSE);
	}
	
	return STATEPROCESS::SUSPEND;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateKeyboardMove::OnEnd(Object* pOwner, DWORD CurTime)
{
#ifdef _NA000000_SPEED_HACK_PREVENTION_SUPPORT
    if (pOwner == g_pHero && 
        g_pHero->KBMove.last_sent_move_flag != 0 && 
        g_pHero->KBMove.last_sent_move_flag != g_pHero->GetMoveFlag())
    {
        SEND_CG_SYNC_KBMOVE_SYN();
    }
#endif //_NA000000_SPEED_HACK_PREVENTION_SUPPORT

	WzVector vDummy;
	vDummy.x = 0.0f;
	vDummy.y = 0.0f;
	vDummy.z = 0.0f;
	Player* pPlayer = (Player*)pOwner;
	pPlayer->StopMove();
	pPlayer->LockOnPosition(FALSE, vDummy);
	pPlayer->SetVisibleAngleModifer(0.0f);

	return STATECHANGE::SUCCESS;
}
//------------------------------------------------------------------------------ 
bool StateKeyboardMove::SEND_CG_SYNC_KBMOVE_SYN()
{
#ifdef _NA000000_SPEED_HACK_PREVENTION_SUPPORT
    if ((g_pHero->PacketNak.is_recv_nak == false) && (BattleScene::IsWaitEnterSync() == FALSE))
    {
        ulong next_serial_key = 0;
        if (KBMoveCheckManagerInClient::Instance()->IssueNextSerial(&next_serial_key) == true)
        {
            MSG_CG_SYNC_KBMOVE_SYN syn;
            syn.m_byMoveState = (BYTE)g_pHero->GetMoveFlag();
            syn.wvCurPos = g_pHero->GetPosition();
            syn.m_wAngle = (WORD) (g_pHero->GetAngle() * 180.0f / WZ_PI);
            syn.m_wTileIndex = g_pHero->GetPathExplorer()->GetTile();
            syn.issued_serial = next_serial_key;
            syn.client_test_result = nsSync::MemoryHackMonitor::Instance()->GetNextClientTestSerial();
            syn.Encode();

            g_pHero->SendPacket(&syn,sizeof(syn));
            g_pHero->SetOldMoveFlag(g_pHero->GetMoveFlag());
            g_pHero->SetKeyboardMovePacketSendDuration(500);
            g_pHero->SetKeyboardMovePacketSendTime(0);
            WzVector vDummy;
            vDummy.x = 0.0f;
            vDummy.y = 0.0f;
            vDummy.z = 0.0f;

            g_pHero->SetOldKeyboardDirection(&vDummy); 

            g_pHero->KBMove.pos = g_pHero->GetPosition();
            g_pHero->KBMove.ang = g_pHero->GetAngle();
            g_pHero->KBMove.sent_tick = clock_function::GetTickCount();
            g_pHero->KBMove.is_first_kbmove = false;
            g_pHero->KBMove.last_sent_move_flag = g_pHero->GetMoveFlag();
            g_pHero->KBMove.debug_kbmove_skip_count = 0;

            return true;
        }
    }
#endif //_NA000000_SPEED_HACK_PREVENTION_SUPPORT

    return false;
}
