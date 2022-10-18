//------------------------------------------------------------------------------
//  StateKeyboardMove.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#ifdef _NA_20120220_JUMP_RENEWAL
#else
#include "statebase.h"
#include "statemove.h"
#include "./statekeyboardjump.h"
#include "Hero.h"
#include "Player.h"
#include "Map.h"
#include "ObjectManager.h"
#include "Camera.h"
#include "GameFunc.h"
#include "NumericValues.h"
#include "GlobalFunc.h"
#include "InterfaceManager.h"
#include "GameFramework.h"
#include "SceneBase.h"
#include "RidingInfoParser.h"
#include "Rider.h"
#include "KBMoveCheckManagerInClient.h"
#include "BattleScene.h"

#define HEIGHT 2.8F
#define AVERAGE_DISTANCE 6.3f;
#define MAX_JUMP_TIME 1000
#define STUN_HEIGHT 8.0F


//------------------------------------------------------------------------------
/**
*/
StateKeyboardJump::StateKeyboardJump(void)
{
 
}

//------------------------------------------------------------------------------
/**
*/
StateKeyboardJump::~StateKeyboardJump(void)
{
}

//------------------------------------------------------------------------------
/**
*/
BOOL StateKeyboardJump::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
{
	return TRUE;	
}

int StateKeyboardJump::CheckJumpTile(WzVector vSrc,WzVector vDest)
{
	if(!g_pPathExplorer || !g_pMap)
		return 0;

	g_pPathExplorer->Stop();

	// 대상 Tile 얻기	

	int iDestTileFocussing = g_pSunTerrain->x_pPathFinder->GetTileToStand( vDest, NULL, -1.f, 100.0f );
	if( iDestTileFocussing < 0 )
	{
		return 0;
	}

	if(SCENE_TYPE_DOMINATION == GameFramework::GetCurScene()) 
	{
		WzRay ray;
		ray.m_wvOrigin = vDest;
		ray.m_wvOrigin.z += 1.0f;
		ray.m_wvDirection.x = 0.0f;
		ray.m_wvDirection.y = 0.0f;
		ray.m_wvDirection.z = -1.0f;
		int iTile = g_pSunTerrain->PickPathTile(&ray);
		if(0 > iTile)
			return 0;
	}

	int iSrcTileFocussing = g_pSunTerrain->x_pPathFinder->GetTileToStand( vSrc, NULL, -1.f, 100.0f );
	if( iSrcTileFocussing < 0 )
	{
		return 0;
	}

    WORD Src_tile_attribute = g_pMap->GetWorldBase()->x_pPathFinder->GetAttribute(iSrcTileFocussing);
    WORD dest_tile_attribute = g_pMap->GetWorldBase()->x_pPathFinder->GetAttribute(iDestTileFocussing);
#if WZENGINEVER >= 270
    if ((Src_tile_attribute & PTA_JUMP_CONTROL_A) &&
        (dest_tile_attribute & PTA_JUMP_CONTROL_B))
    {
        return 2;
    }
    if ((Src_tile_attribute & PTA_JUMP_CONTROL_B) &&
        (dest_tile_attribute & PTA_JUMP_CONTROL_A))
    {
        return 2;
    }
#endif //WZENGINEVER >= 270

	if ((Src_tile_attribute & PTA_PLAYER_JUMP_TILE) &&
		(dest_tile_attribute & PTA_PLAYER_JUMP_TILE))
	{
		return 1;
	}

	if ((Src_tile_attribute & PTA_NO_WALK) || 
        (dest_tile_attribute & PTA_NO_WALK))
	{
		return 0;
	}

	return 0;
}

//------------------------------------------------------------------------------
/**
*/
BOOL StateKeyboardJump::FindDest()
{
	WzVector vSrc=g_pHero->m_JumpData.m_JumpStartPosition;

	g_pPathExplorer->Stop();

	if(!g_pSunTerrain->PathExp_Reset(g_pPathExplorer, &vSrc, -1))
	{
		g_pHero->m_JumpData.m_JumpTargetPosition = g_pHero->m_JumpData.m_JumpStartPosition;
		return FALSE;
	}

	WzVector oldPos=g_pPathExplorer->GetPos();

	WzVector vLongOffset = g_pHero->m_JumpData.m_JumpTargetPosition - vSrc;

	g_pSunTerrain->PathExp_Thrust(g_pPathExplorer,&vLongOffset,PTA_NO_WALK);
	
	g_pSunTerrain->PathExp_Progress(g_pPathExplorer, 1000000, NULL);


	WzVector newPos = g_pPathExplorer->GetPos();

	if (oldPos != newPos)
	{
		g_pHero->m_JumpData.m_JumpTargetPosition = g_pPathExplorer->GetVisiblePos();
		return	TRUE;
	}

	g_pHero->m_JumpData.m_JumpTargetPosition = g_pHero->m_JumpData.m_JumpStartPosition;
	return FALSE;
}
//------------------------------------------------------------------------------ 
bool StateKeyboardJump::GetJumpDestPosByDirection(OUT WzVector& dest_pos, 
                                                  WzVector& scr_pos, 
                                                  WzVector& direction, 
                                                  float player_height, 
                                                  float min_dist, 
                                                  float max_dist)
{
    if (max_dist < min_dist)
    {
        max_dist = min_dist;
    }

    //최대 10회까지만 찾는다
    for (int i = 0; i < 5; ++i)
    //for (int i = 4; i >= 0; --i)
    {
        float gab = min_dist + ((max_dist - min_dist) / 5.0f * i);

        WzRay ray;
        ray.m_wvOrigin = scr_pos + (direction * gab);
        ray.m_wvOrigin.z += player_height;
        ray.m_wvDirection.x =  0.0f;
        ray.m_wvDirection.y =  0.0f;
        ray.m_wvDirection.z = -1.0f;

        bool is_find = true;
        float fT;
        if (g_pSunTerrain->PickTerrainMesh(&ray, &fT ,TRUE, TMGO_NO_COLLISION) < 0)
        {
            ray.m_wvOrigin.z += player_height;
            ray.m_wvDirection.x = 0.0f;
            ray.m_wvDirection.y = 0.0f;
            ray.m_wvDirection.z = 1.0f;

            if(g_pSunTerrain->PickTerrainMesh(&ray, &fT , FALSE,TMGO_NO_COLLISION) < 0)
            {
                is_find = false;
            }
        }

        if (is_find)
        {
            dest_pos = ray.m_wvOrigin + ray.m_wvDirection * fT;

            if (abs(dest_pos.z - scr_pos.z) < CGeneralGameParam::Instance()->wings_down_jump_height())
            {
                g_pPathExplorer->Stop();

                g_pSunTerrain->PathExp_Reset(g_pPathExplorer, &dest_pos, -1);

                WzVector backup_pos = g_pHero->m_JumpData.m_JumpTargetPosition;

                g_pHero->m_JumpData.m_JumpTargetPosition = dest_pos;
                if (CheckDest() == TRUE)
                {
                    int tile_no = g_pPathExplorer->GetTile();

                    if ((tile_no >= 0) && 
                        ((g_pSunTerrain->x_pPathFinder->GetAttribute(tile_no) & PTA_NO_WALK) == false))
                    {
                        dest_pos = g_pHero->m_JumpData.m_JumpTargetPosition;
                        return true;
                    }
                }

                g_pHero->m_JumpData.m_JumpTargetPosition = backup_pos;
            }
        }
    }

    return false;
}
//------------------------------------------------------------------------------ 
bool StateKeyboardJump::GetJumpDirection(OUT WzVector& direction, int flag, WzVector& foward)
{
    SetVector(&direction,0.0f,0.0f,0.0f);

    WzVector vRight,vLeft;
    vRight.x =  foward.y; // sin(a - 90) = -cos(a) = vFoward.y;
    vRight.y = -foward.x; // -cos(a - 90) = -sin(a) = -vFoward.y;
    vRight.z =  0.0f;

    vLeft.x = -foward.y; // sin(a + 90) = cos(a) = -vFoward.y;
    vLeft.y =  foward.x; // -cos(a + 90) = sin(a) = vFoward.x;
    vLeft.z =  0.0f;

    bool is_result = true;

    if (flag & KEYBOARDMOVE_RIGHTSTEP)
    {
        direction =  direction + vRight;		

        // -45도
        if (flag & KEYBOARDMOVE_FOWARD)
        {
            direction+=foward;
        }	
        else if (flag & KEYBOARDMOVE_BACKWARD)
        {
            direction-=foward;
        }
        else
        {

        }
    }
    else if (flag & KEYBOARDMOVE_LEFTSTEP)
    {
        direction =  direction + vLeft;		
        // 45도
        if (flag & KEYBOARDMOVE_FOWARD)
        {
            direction+=foward;		
        }
        else if (flag & KEYBOARDMOVE_BACKWARD)
        {
            direction-=foward;			
        }	
        else
        {			
        }
    }
    else if (flag & KEYBOARDMOVE_FOWARD)
    {
        direction =  direction + foward;
    }
    else if (flag & KEYBOARDMOVE_BACKWARD)
    {
        direction =  direction - foward;
    }
    else if (flag & KEYBOARDMOVE_LEFTTURN)
    {
    }
    else if (flag & KEYBOARDMOVE_RIGHTTURN)
    {

    }
    else
    {
        return false;
    }

    return true;
}
//------------------------------------------------------------------------------
/**
*/
BOOL StateKeyboardJump::CalCulateJump()
{
	if ( !g_pMap )
	{
		return FALSE;
	}

	////Thrust 이동의 경우 못가는데서 점프하는 경우가 있다. 
	//g_pHero->SetOldJumpFlag(0xffffffff);
	//g_pHero->SetKeyboardMovePacketSendDuration(0);
	//g_pHero->SetKeyboardMovePacketSendTime(0);


	////목적지 계산 
	//g_pHero->vFoward = g_pHero->GetDirection();

 //   g_pHero->SetVisibleAngleModifer(0.0f);

	int iMoveFlag = g_pHero->m_JumpData.m_JumpFlag.GetMoveFlag();


    WzVector vDirection;
    if (GetJumpDirection(vDirection, iMoveFlag, g_pHero->m_JumpData.m_CharacterDirection) == false)
    {
        g_pHero->m_JumpData.m_JumpStartPosition = g_pHero->GetVisiblePos();
        g_pHero->m_JumpData.m_JumpTargetPosition = g_pHero->GetVisiblePos();
        return TRUE;
    }

	VectorNormalize(&vDirection,&vDirection);
	WzVector vCurrent = g_pHero->GetVisiblePos();

    WzVector vLongOffset = vDirection * g_pHero->GetJumpDistance();

	g_pHero->m_JumpData.m_JumpTargetPosition = vCurrent + vLongOffset;

	g_pHero->m_JumpData.m_JumpStartPosition = g_pHero->GetVisiblePos();

	WzVector vDis = g_pHero->m_JumpData.m_JumpTargetPosition - g_pHero->m_JumpData.m_JumpStartPosition;
	float fDis = VectorLength(&vDis);

	WzVector vTest;
	WzRay ray;
	ray.m_wvOrigin = g_pHero->m_JumpData.m_JumpTargetPosition;
	ray.m_wvOrigin.z += g_pHero->GetJumpHeight();
	ray.m_wvDirection.x = 0.0f;
	ray.m_wvDirection.y = 0.0f;
	ray.m_wvDirection.z = -1.0f;

	float		fT;
	BOOL        bFind=TRUE;
	int iTerrainMeshFocussing = g_pSunTerrain->PickTerrainMesh( &ray, &fT ,TRUE,TMGO_NO_COLLISION);
	if( iTerrainMeshFocussing < 0 )
	{
		ray.m_wvOrigin = g_pHero->m_JumpData.m_JumpTargetPosition;
		ray.m_wvOrigin.z += g_pHero->GetJumpHeight();
		ray.m_wvDirection.x = 0.0f;
		ray.m_wvDirection.y = 0.0f;
		ray.m_wvDirection.z = +1.0f;

		if(iTerrainMeshFocussing = g_pSunTerrain->PickTerrainMesh( &ray, &fT ,FALSE,TMGO_NO_COLLISION)>=0)
		{
			vTest = ray.m_wvOrigin + ray.m_wvDirection * fT;

		}
		else 
		{
			bFind=FALSE;
		}

	}
	else
	{
		vTest = ray.m_wvOrigin + ray.m_wvDirection * fT;
	}		

	if(!bFind)
	{
		if(FindStand())
			bFind=TRUE;
	}


	if(bFind)
	{
		WzVector vVisible=vTest;

		g_pPathExplorer->Stop();

		g_pSunTerrain->PathExp_Reset(g_pPathExplorer, &vVisible, -1);

		if(g_pPathExplorer->GetTile()<0||fabs(g_pPathExplorer->GetVisiblePos().z-vTest.z)>1.0f)
			bFind=FALSE;
	}

	BOOL bIsMove=FALSE;
	//타일로 바꾸자.
	if ( bFind)//딛을수 있는 곳인가
	{		
		g_pHero->m_JumpData.m_JumpTargetPosition = vTest;

		if(CheckDest())
		{
			if(!IsIntersect())
			{
				bIsMove=TRUE;
			}
		}
	}

	if(!bIsMove)
	{
		FindDest();
	}


	g_pPathExplorer->Stop();
	
	//네비게이션 메쉬가 위로 많이 뜰경우 리셋이 실패 할수 있다.
	if(g_pSunTerrain->PathExp_Reset(g_pPathExplorer, &g_pHero->m_JumpData.m_JumpTargetPosition, -1))
	{
		g_pHero->SetPosition(g_pPathExplorer->GetPos());
	}
	else
	{
		g_pHero->m_JumpData.m_JumpStartPosition = g_pHero->GetVisiblePos();
		g_pHero->m_JumpData.m_JumpTargetPosition = g_pHero->GetVisiblePos();
	}

	return TRUE;
}



//------------------------------------------------------------------------------
/**
*/
BOOL StateKeyboardJump::SendJumpPacket()
{
	//Todo;hero에 포지션 세팅 할것 
    if (g_pHero)
    {
#ifdef _NA000000_SPEED_HACK_PREVENTION_SUPPORT
        if ((g_pHero->PacketNak.is_recv_nak == false) && (BattleScene::IsWaitEnterSync() == FALSE))
        {
            ulong next_serial_key = 0;
            if (KBMoveCheckManagerInClient::Instance()->IssueNextSerial(&next_serial_key) == true)
            {
                MSG_CG_SYNC_JUMP_SYN Packet;
		        Packet.m_wvDestPos = g_pHero->GetPosition();
                Packet.m_iDir       = g_pHero->m_JumpData.m_JumpFlag.GetMoveFlag();
                Packet.issued_serial = next_serial_key;
                Packet.client_test_result = \
                    nsSync::MemoryHackMonitor::Instance()->GetNextClientTestSerial();
                g_pHero->SendPacket(&Packet,sizeof(Packet));
            }
            else
            {
                WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_SYSCHAT, "KBMoveCheckManagerInClient::Instance()->IssueNextSerial(&next_serial_key) == false");
            }
        }
#else
        MSG_CG_SYNC_JUMP_SYN Packet;

        Packet.m_byCategory = CG_SYNC;
        Packet.m_byProtocol = CG_SYNC_JUMP_SYN;	
		//서버로 네비게이션 메쉬 좌표를 보낸다.
		Packet.m_wvDestPos = g_pHero->GetPosition();
        Packet.m_iDir       = g_pHero->m_JumpData.m_JumpFlag.GetMoveFlag();
        g_pHero->SendPacket(&Packet,sizeof(Packet));
#endif //_NA000000_SPEED_HACK_PREVENTION_SUPPORT
    }

	return TRUE;
}


//------------------------------------------------------------------------------
/**
*/
BOOL StateKeyboardJump::SendStopPacket()
{
	//Todo;hero에 포지션 세팅 할것 
	if (g_pHero)
	{
#ifdef _NA000000_SPEED_HACK_PREVENTION_SUPPORT
        if ((g_pHero->PacketNak.is_recv_nak == false) && (BattleScene::IsWaitEnterSync() == FALSE))
        {
            ulong next_serial_key = 0;
            if (KBMoveCheckManagerInClient::Instance()->IssueNextSerial(&next_serial_key) == true)
            {
                MSG_CG_SYNC_JUMP_STOP_SYN Packet;
                Packet.issued_serial = next_serial_key;
                Packet.client_test_result = \
                    nsSync::MemoryHackMonitor::Instance()->GetNextClientTestSerial();
                Packet.m_wvDestPos = g_pHero->GetPosition();
                g_pHero->SendPacket(&Packet,sizeof(Packet));
            }
        }
#else
	    MSG_CG_SYNC_JUMP_STOP_SYN Packet;

	    Packet.m_byCategory = CG_SYNC;
	    Packet.m_byProtocol = CG_SYNC_JUMP_STOP_SYN;	

	    Packet.m_wvDestPos = g_pHero->GetPosition();
	    g_pHero->SendPacket(&Packet,sizeof(Packet));
#endif //_NA000000_SPEED_HACK_PREVENTION_SUPPORT
	}

	return TRUE;
}

BOOL StateKeyboardJump::FindStand()
{
	WzVector vTemp;
	float fRate=1.0f;

	//최대 10회까지만 찾는다
	for(int i=0;i<10;i++)
	{
		vTemp =  g_pHero->m_JumpData.m_JumpStartPosition* (1.0f - fRate) +  g_pHero->m_JumpData.m_JumpTargetPosition* (fRate);

		WzRay ray;
		ray.m_wvOrigin = vTemp;
		ray.m_wvOrigin.z += g_pHero->GetJumpHeight();
		ray.m_wvDirection.x = 0.0f;
		ray.m_wvDirection.y = 0.0f;
		ray.m_wvDirection.z = -1.0f;

		float		fT;
		BOOL        bFind=TRUE;
		int iTerrainMeshFocussing = g_pSunTerrain->PickTerrainMesh( &ray, &fT ,TRUE,TMGO_NO_COLLISION);
		if( iTerrainMeshFocussing < 0 )
		{
			ray.m_wvOrigin = vTemp;
			ray.m_wvOrigin.z += g_pHero->GetJumpHeight();
			ray.m_wvDirection.x = 0.0f;
			ray.m_wvDirection.y = 0.0f;
			ray.m_wvDirection.z = +1.0f;

			if(iTerrainMeshFocussing = g_pSunTerrain->PickTerrainMesh( &ray, &fT ,FALSE,TMGO_NO_COLLISION)>=0)
			{
				vTemp = ray.m_wvOrigin + ray.m_wvDirection * fT;
				g_pPathExplorer->Stop();

				g_pSunTerrain->PathExp_Reset(g_pPathExplorer, &vTemp, -1);
			
				if(g_pPathExplorer->GetTile()>=0)
					return TRUE;
			}
		}
		else
		{
			vTemp = ray.m_wvOrigin + ray.m_wvDirection * fT;
			g_pPathExplorer->Stop();

			g_pSunTerrain->PathExp_Reset(g_pPathExplorer, &vTemp, -1);

			if(g_pPathExplorer->GetTile()>=0)
				return TRUE;
		}		

		fRate-=0.1f;
	}

	return FALSE;
}

BOOL StateKeyboardJump::CheckDest()
{
	WzVector vTemp;
	float fRate=1.0f;

    BOOL CanWalk = TRUE;
    BOOL CanJump = TRUE;

	//최대 10회까지만 찾는다
	for(int i=0;i<10; i++, fRate-=0.1f)
	{
		vTemp =  g_pHero->m_JumpData.m_JumpStartPosition* (1.0f - fRate) +  g_pHero->m_JumpData.m_JumpTargetPosition* (fRate);

        int CheckJumpResult = CheckJumpTile(g_pHero->m_JumpData.m_JumpStartPosition,vTemp);

        if( CheckJumpResult == 1 )  // 점프가 가능하도록 강제로 설정해둔 타일
        {
            g_pPathExplorer->Stop();

            g_pSunTerrain->PathExp_Reset(g_pPathExplorer, &vTemp, -1);
            WzVector vPos=g_pPathExplorer->GetVisiblePos();
            float height_different = (vPos.z - g_pHero->GetVisiblePos().z);

            if (height_different <= g_pHero->GetJumpHeight())
            {
                if ((g_pHero->is_spread_wings() == true) &&
                    (abs(height_different) >= CGeneralGameParam::Instance()->wings_down_jump_height()))
                {
                    // 날고 있을 때의 높이제한
                    continue;
                }

                g_pHero->m_JumpData.m_JumpTargetPosition = vPos;
                return TRUE;
            }
		}
        else if( CheckJumpResult == 0 ) // 타일이 없거나 갈수 없는 영역을 지난다
        {
            CanWalk = FALSE;
        }
        else if( CheckJumpResult == 2 ) // 점프를 하지 못하도록 강제로 설정해둔 위치
        {
            // 그냥 무조건 점프 불가
            CanJump = FALSE;

            //if( CanWalk == FALSE )
            //{
            //    // 직선 경로중 이동불가 지역이 먼저 포함되어있을떄
            //    CanJump = FALSE;
            //}
        }
	}

    if( CanJump == FALSE )
    {
        return FALSE;
    }

	g_pPathExplorer->Stop();

	// 대상 Tile 얻기	
	int iDestTileFocussing = g_pSunTerrain->x_pPathFinder->GetTileToStand( g_pHero->m_JumpData.m_JumpTargetPosition, NULL, -1.0f, 100.0f );
	if( iDestTileFocussing < 0 )
	{
		return FALSE;
	}

	if (g_pMap->GetWorldBase()->x_pPathFinder->GetAttribute(iDestTileFocussing) & (PTA_NO_WALK))
	{
		return FALSE;
	}


	if(GameFramework::GetCurrentScene())
	{

		SCENE_TYPE type=GameFramework::GetCurrentScene()->GetType();
		int maxNode=0;

		switch(type) 
		{
		case SCENE_TYPE_MISSION:
			{
				maxNode=200;
			}
			break;

		case SCENE_TYPE_DOMINATION:
			{
				maxNode=20;
			}
			break;

		case SCENE_TYPE_CHUNTING:
			{
				maxNode=1000;
			}
			break;
		}


		if( maxNode )
			return GameFunc::IsMove(g_pHero->m_JumpData.m_JumpStartPosition,g_pHero->m_JumpData.m_JumpTargetPosition,TRUE,maxNode);
		else 
			return TRUE;
	}

	return FALSE;
}




BOOL StateKeyboardJump::IsIntersect()
{

	WzRay ray;
	WzVector vDistance = g_pHero->m_JumpData.m_JumpTargetPosition - g_pHero->m_JumpData.m_JumpStartPosition;
	float fDistance = VectorLength(&vDistance);
	ray.m_wvOrigin = g_pHero->m_JumpData.m_JumpStartPosition;
	ray.m_wvOrigin.z+=0.2f;

	ray.m_wvDirection = g_pHero->m_JumpData.m_JumpTargetPosition - ray.m_wvOrigin;
	VectorNormalize(&ray.m_wvDirection,&ray.m_wvDirection);

	BOOL bRt=g_pMap->IntersectTree_Ray_Bv(&ray,fDistance);

	if(bRt)
	{
		return bRt;
	}

	WzVector vObjPosition;

	bRt=g_pMap->IntersectMapObject_Ray_Bv(&ray,fDistance,HEIGHT,vObjPosition);

	if(bRt)
	{
		return bRt;
	}


	float fT;

	WzVector tempDest = g_pHero->m_JumpData.m_JumpTargetPosition;
	WzVector tempSrc=g_pHero->m_JumpData.m_JumpStartPosition;

	tempSrc.z+=2.5f;
	tempDest.z=tempSrc.z;

	vDistance = tempDest -tempSrc;
	fDistance = VectorLength(&vDistance);
	ray.m_wvOrigin = tempSrc;
	ray.m_wvDirection = tempDest - ray.m_wvOrigin;
	VectorNormalize(&ray.m_wvDirection,&ray.m_wvDirection);
	//Todo:뒷면 지형 체크 옵션 바꾸기 
	int iTerrainMeshFocussing = g_pSunTerrain->PickTerrainMesh( &ray, &fT, FALSE,TMGO_NO_COLLISION);
	if( iTerrainMeshFocussing >= 0 &&fT <= fDistance)//지형과 충돌
	{
		return TRUE;
	}

	return FALSE;
}

//------------------------------------------------------------------------------
/** 점프중 라이딩상태변했는지체크
*/
BOOL StateKeyboardJump::CheckRidingState(Player *pPlayer)
{
    if (pPlayer && 
        pPlayer->is_riding_jump())    
    {       
        if (pPlayer->IsRiding() == FALSE)
        {
            //상태전이시켜준다:라이딩상태에서 점프중 라이딩이 풀렸을때
            pPlayer->set_is_riding_jump(false);
            pPlayer->SetAnimation(pPlayer->GetCurrentJumpKeepAnimation(),TRUE);
            pPlayer->m_JumpData.m_bJumpStart = TRUE;
        }
    }
    else
    {
        
       if (pPlayer->IsRiding())
       {
            return TRUE;
       }
    }
    return FALSE;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateKeyboardJump::OnStart(Object* pOwner, DWORD CurTime)
{
#ifdef _DEBUG	
	Character*	pChr = (Character*)pOwner;

	// 키보드 이동은 플레이어만의 특권!
	assert(pChr->IsKindOfObject(PLAYER_OBJECT));

#endif

	Player*	pPlayer = (Player*)pOwner;

#ifdef _NA_000000_20120410_SPEEDHACK
    if (pPlayer == g_pHero)
    {
        g_pHero->move_speed_ = 7.0f * SPEED_MULTIPLIER;
    }
    else
    {
        pPlayer->SetMoveSpeed( 7.0f * SPEED_MULTIPLIER);
    }
#else
    pPlayer->SetMoveSpeed( 7.0f * SPEED_MULTIPLIER);
#endif //_NA_000000_20120410_SPEEDHACK


	pPlayer->m_JumpData.m_bJumpStun = FALSE;
	pPlayer->m_JumpData.m_hStunEffect = INVALID_HANDLE_VALUE;
	pPlayer->m_JumpData.m_bJumpEndEffect = FALSE;
	pPlayer->m_JumpData.m_fJumpEndFrame = 0.0f;
	pPlayer->m_JumpData.m_fGravity = g_fGravity;
	pPlayer->m_JumpData.m_fUpperForce = g_fUpperForce;
	pPlayer->m_JumpData.m_JumpTime = 0;
	pPlayer->m_JumpData.m_bCreateJumpEndEffect = FALSE;

	pPlayer->SetHeight(0.0f);

	WzVector vDummy;
	vDummy.x = 0.0f;
	vDummy.y = 0.0f;
	vDummy.z = 0.0f;
	pPlayer->SetOldKeyboardDirection(&vDummy);

	pPlayer->m_fTurnAccelerate = 0.0f;
	pPlayer->StopBlendAnimation();

	if(pPlayer->IsTransForm())//변신은 준비 동작 없음 
		pPlayer->m_JumpData.m_bJumpStart = TRUE;
	else
		pPlayer->m_JumpData.m_bJumpStart = FALSE;

	//pPlayer->m_JumpData.m_bJumpStun=FALSE;
	//pPlayer->m_JumpData.m_hStunEffect=INVALID_HANDLE_VALUE;
	//pPlayer->bJumpEndEff=FALSE;
	pPlayer->SetFreeMove(TRUE);

	//if(pPlayer->IsTransForm())//변신은 준비 동작 없음 
	//	pPlayer->m_JumpData.m_bJumpStart=TRUE;
	//else
	//	pPlayer->m_JumpData.m_bJumpStart=FALSE;

	//pPlayer->m_JumpData.m_fJumpEndFrame=0.0f;

	//pPlayer->m_JumpData.m_fGravity = g_fGravity;
	//pPlayer->m_JumpData.m_fUpperForce = g_fUpperForce;
	//pPlayer->m_JumpData.m_bCreateJumpEndEffect = FALSE;

	//pPlayer->m_JumpData.m_JumpTime=0;
	//WzVector vDummy;
	//vDummy.x = 0.0f;
	//vDummy.y = 0.0f;
	//vDummy.z = 0.0f;
	//pPlayer->SetOldKeyboardDirection(&vDummy);
	//pPlayer->SetHeight(0.0f);	
	//
	//pPlayer->m_fTurnAccelerate = 0.0f;
	//pPlayer->StopBlendAnimation();
    
	//m_OffsetHeight = 3.0f;

    pPlayer->set_is_riding_jump(false);

    if (pPlayer->IsRiding())
    {
        pPlayer->set_is_riding_jump(true);
        pPlayer->SetKeyBoardJumpFlag(TRUE);
    }
	pPlayer->SetPlayerAnimation(pPlayer->GetCurrentJumpStartAnimation(),eANI_JUMP_START,FALSE);


    if( pPlayer == g_pHero)
    {	
        //Thrust 이동의 경우 못가는데서 점프하는 경우가 있다. 
        g_pHero->SetKeyboardMovePacketSendDuration(0);
        g_pHero->SetKeyboardMovePacketSendTime(0);
        g_pHero->SetVisibleAngleModifer(0.0f);
        g_pHero->m_JumpData.m_CharacterDirection = g_pHero->GetDirection();

        if ((g_pHero->is_spread_wings() == true) ||
            ((g_pHero->rider() != NULL) && (g_pHero->rider()->is_spread() == true)))
        {
            //! 위치가 계산되어 있다.
        }
        else
        {
            CalCulateJump();
        }
        SendJumpPacket();
    }

	//시간 계산 
	WzVector vDistance = pPlayer->m_JumpData.m_JumpTargetPosition -pPlayer->m_JumpData.m_JumpStartPosition;
	float fDistance = VectorLength(&vDistance);

	pPlayer->m_JumpData.m_SrcZ=pPlayer->m_JumpData.m_JumpStartPosition.z;
	
	
//타락한 사원에서 속성 검사 해서 속성이 검출 될 경우 z값을 -10정도 더 내려 준다
	int iTileFocussing = g_pSunTerrain->x_pPathFinder->GetTileToStand(pPlayer->m_JumpData.m_JumpTargetPosition, NULL, -1.0f, 100.0f);
	if (iTileFocussing >= 0)
	{
		if (g_pSunTerrain->x_pPathFinder->GetAttribute( iTileFocussing ) & PTA_PLAYER_FALLDOWN_TILE)
		{
			pPlayer->m_JumpData.m_JumpTargetPosition.z -= 50.0f;
		}
	}

//---------------------------------------------------------------------------------

    pPlayer->CalcJumpTime(fDistance);

	int iMoveFlag = pPlayer->m_JumpData.m_JumpFlag.GetMoveFlag();

	if (iMoveFlag & KEYBOARDMOVE_RIGHTSTEP)
	{
		// -45도
		if (iMoveFlag & KEYBOARDMOVE_FOWARD)
		{
			pPlayer->SetVisibleAngleModifer(-(WZ_PI / 4.0f));
		}	

		if (iMoveFlag & KEYBOARDMOVE_BACKWARD)
		{
			pPlayer->SetVisibleAngleModifer((WZ_PI / 4.0f));
		}	
	}
	else if (iMoveFlag & KEYBOARDMOVE_LEFTSTEP)
	{
		// 45도
		if (iMoveFlag & KEYBOARDMOVE_FOWARD)
		{
			pPlayer->SetVisibleAngleModifer((WZ_PI / 4.0f));		
		}

		if (iMoveFlag & KEYBOARDMOVE_BACKWARD)
		{	
			pPlayer->SetVisibleAngleModifer(-(WZ_PI / 4.0f));
		}	
	}



	float fH=pPlayer->m_JumpData.m_JumpStartPosition.z-pPlayer->m_JumpData.m_JumpTargetPosition.z;
	if(fH>STUN_HEIGHT)
	{
		pPlayer->m_JumpData.m_bJumpStun = TRUE;

		float fTime=50;
		fTime *= (fH-STUN_HEIGHT);
		pPlayer->m_JumpData.m_JumpStunTime = (int)fTime;
	}
	else
	{
		pPlayer->m_JumpData.m_bJumpStun = FALSE;
		pPlayer->m_JumpData.m_JumpStunTime = 0;
	}

	return STATECHANGE::SUCCESS;
}


//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE StateKeyboardJump::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Player* pPlayer = (Player*)pOwner;

	// 회전 관련 액셀 값을 세팅 한다.
	if(pPlayer==g_pHero)
	{
		CalculateTurnAccelerate(dwTick);
	}

	pPlayer->m_JumpData.m_JumpTime += dwTick;

	if( pPlayer->m_JumpData.m_bJumpStart == FALSE )
		pPlayer->m_JumpData.m_JumpTime = 1;

	if( pPlayer->m_JumpData.m_hStunEffect != INVALID_HANDLE_VALUE )
	{
		if( CEffectBody* pBody= g_pEffectManager->GetEffectPtrHandle(pPlayer->m_JumpData.m_hStunEffect ) )
		{
			WzVector vPos;
			vPos = pPlayer->GetHeadBonePos();
			vPos.z += 0.1f;

			WzVector vRot;
			vRot.x = 0.0f;
			vRot.y = 0.0f;
			vRot.z = 0.0f;

			WzVector vScale;
			vScale.x = 1.0f;
			vScale.y = 1.0f;
			vScale.z = 1.0f;
			pBody->SetResultMatrix(vPos,vRot,vScale);
		}
	}

	
	WzEvent event;	

	float fRet=0.0f;

	char szAniCode[5];
	ZeroMemory(szAniCode,sizeof(szAniCode));
	WzIDToString(szAniCode,pPlayer->GetCurrentAnimation());
	szAniCode[1]='0';
	WzID wzCurID=StrToWzID(szAniCode);

	ZeroMemory(szAniCode,sizeof(szAniCode));
	WzIDToString(szAniCode,pPlayer->GetCurrentJumpStartAnimation());
	szAniCode[1]='0';
	WzID wzStartID=StrToWzID(szAniCode);
	
	ZeroMemory(szAniCode,sizeof(szAniCode));
	WzIDToString(szAniCode,pPlayer->GetCurrentJumpKeepAnimation());
	szAniCode[1]='0';
	WzID wzKeepID=StrToWzID(szAniCode);
	
	ZeroMemory(szAniCode,sizeof(szAniCode));
	WzIDToString(szAniCode,pPlayer->GetCurrentJumpEndAnimation());
	szAniCode[1]='0';
	WzID wzEndID=StrToWzID(szAniCode);


	ZeroMemory(szAniCode,sizeof(szAniCode));
	WzIDToString(szAniCode,pPlayer->GetCurrentJumpLandAnimation());
	szAniCode[1]='0';
	WzID wzLandID=StrToWzID(szAniCode);


	ZeroMemory(szAniCode,sizeof(szAniCode));
	WzIDToString(szAniCode,pPlayer->GetCurrentJumpStunEndAnimation());
	szAniCode[1]='0';
	WzID wzStunEndID=StrToWzID(szAniCode);


	ZeroMemory(szAniCode,sizeof(szAniCode));
	WzIDToString(szAniCode,pPlayer->GetCurrentJumpStunAnimation());
	szAniCode[1]='0';
	WzID wzStunID=StrToWzID(szAniCode);

	ZeroMemory(szAniCode,sizeof(szAniCode));
	WzIDToString(szAniCode,pPlayer->GetCurrentJumpStunKeepAnimation());
	szAniCode[1]='0';
	WzID wzStunKeepID=StrToWzID(szAniCode);

    float fRate;
	if (pPlayer->m_JumpData.m_dwHorizonMoveTime == 0)
    {
        fRate = 1.0f;
    }
    else
    {
        fRate = (float)pPlayer->m_JumpData.m_JumpTime / (float) pPlayer->m_JumpData.m_dwHorizonMoveTime;
        if (fRate >= 1.0f)
        {
            fRate = 1.0f;
        }

        if (fRate <= 0.0f)
        {
            fRate = 0.0f;
        }
    }
    
    //수평 이동
    WzVector vPos1 = pPlayer->m_JumpData.m_JumpStartPosition * (1.0f - 0.35f) + pPlayer->m_JumpData.m_JumpTargetPosition* (0.35f);
    WzVector vPos2 = pPlayer->m_JumpData.m_JumpStartPosition * (1.0f - 0.65f) + pPlayer->m_JumpData.m_JumpTargetPosition* (0.65f);
    vPos1.z+=pPlayer->GetJumpHeight();
    vPos2.z=vPos1.z;

    WzVector vCurPos=pPlayer->m_JumpData.m_JumpStartPosition*((1.0f - fRate)*(1.0f - fRate)*(1.0f - fRate)) +
        vPos1*fRate*3*((1.0f - fRate)*(1.0f - fRate))+
        vPos2*3*fRate*fRate*(1.0f - fRate)+
        +pPlayer->m_JumpData.m_JumpTargetPosition * (fRate*fRate*fRate);

    vCurPos.z=pPlayer->GetVisiblePos().z;
    pPlayer->SetVisiblePos(vCurPos);
    pPlayer->GetBasePart()->SetPosition(vCurPos);
    pPlayer->SetFreeMove(TRUE);



	WzVector vTest=pPlayer->GetVisiblePos();
	WzVector visibleTest=vTest;
	BOOL bEnd=FALSE;
	BOOL bIsMove=FALSE;
	
	WzRay ray;
	ray.m_wvOrigin = vTest;
	ray.m_wvOrigin.z += 1.0f;
	ray.m_wvDirection.x = 0.0f;
	ray.m_wvDirection.y = 0.0f;
	ray.m_wvDirection.z = -1.0f;
	bEnd=FALSE;

	float		fT;
	BOOL        bFind=TRUE;
	int iTerrainMeshFocussing = g_pSunTerrain->PickTerrainMesh( &ray, &fT,TRUE,TMGO_NO_COLLISION );
	if( iTerrainMeshFocussing < 0 )
	{

		if(SCENE_TYPE_DOMINATION == GameFramework::GetCurScene())
		{
			int iTile = g_pSunTerrain->PickPathTile(&ray, &fT);
			if(0 > iTile)
				bFind=FALSE;
			else
				vTest = ray.m_wvOrigin + ray.m_wvDirection * fT;
		}
		else
		{
			ray.m_wvOrigin = vTest;
			ray.m_wvOrigin.z += 1.0f;
			ray.m_wvDirection.x = 0.0f;
			ray.m_wvDirection.y = 0.0f;
			ray.m_wvDirection.z = +1.0f;

			if(iTerrainMeshFocussing = g_pSunTerrain->PickTerrainMesh( &ray, &fT,FALSE,TMGO_NO_COLLISION )>=0)
			{
				vTest = ray.m_wvOrigin + ray.m_wvDirection * fT;

			}
			else 
			{
				bFind=FALSE;
			}
		}

	}
	else
	{
		vTest = ray.m_wvOrigin + ray.m_wvDirection * fT;
	}		

	if (bFind)
	{
		// 대상 Tile 얻기	
		int iTileFocussing = g_pSunTerrain->x_pPathFinder->GetTileToStand(vTest, NULL, -1.0f, 100.0f);
		if (iTileFocussing < 0)
		{
			bFind = FALSE;
		}


		if (g_pMap->GetWorldBase()->x_pPathFinder->GetAttribute(iTileFocussing) & (PTA_NO_WALK))
		{
			bFind = FALSE;
		}

		if (g_pSunTerrain->x_pPathFinder->GetAttribute( iTileFocussing ) & PTA_PLAYER_FALLDOWN_TILE)
		{
			vTest.z = pPlayer->m_JumpData.m_JumpTargetPosition.z;
		}
	}

	float fTest = visibleTest.z-vTest.z;
	fRate = (float)pPlayer->m_JumpData.m_JumpTime / (float) pPlayer->m_JumpData.m_dwVerticalMoveTime;		
	fRate = min(1.0f,fRate);
	if (bFind && (fTest <= 2.0f) && (fRate >= 0.65f))
	{	
		bEnd = TRUE;
	}
	else if (visibleTest.z == pPlayer->m_JumpData.m_JumpTargetPosition.z)
	{
		float fRate;
		
		fRate = (float)pPlayer->m_JumpData.m_JumpTime / (float) pPlayer->m_JumpData.m_dwHorizonMoveTime;		
		if (fRate >= 1.0f)
        {
			bEnd=TRUE;
        }
	}

	WzVector vVisiblePos = pPlayer->GetVisiblePos();
	
	BOOL bLandFrame = FALSE;
    float vertical_move_rate = 0.f;

    //수직 이동 
	if (pPlayer->m_JumpData.m_dwVerticalMoveTime)
	{
		float fRate = (float)pPlayer->m_JumpData.m_JumpTime / (float) pPlayer->m_JumpData.m_dwVerticalMoveTime;
        
        vertical_move_rate = fRate;
		
        if (fRate <= 0.0f)                                                              
		{
			fRate = 0.0f;
		}

		if (fRate >= 1.0f)
		{
			fRate = 1.0f;

			pPlayer->m_JumpData.m_fUpperForce += pPlayer->m_JumpData.m_fGravity * dwTick * SPEED_MULTIPLIER;
			pPlayer->m_JumpData.m_fUpperForce = min(pPlayer->m_JumpData.m_fUpperForce, g_fMaxForce );

			float fDelta = (pPlayer->m_JumpData.m_fUpperForce * (float)dwTick);

			vVisiblePos.z -= fDelta; 		

			if(pPlayer->m_JumpData.m_bJumpStun)
			{
				if(vVisiblePos.z-pPlayer->m_JumpData.m_JumpTargetPosition.z<=0.25f)
				{

					if(vVisiblePos.z<=pPlayer->m_JumpData.m_JumpTargetPosition.z)
						vVisiblePos.z=pPlayer->m_JumpData.m_JumpTargetPosition.z;

					if(!pPlayer->m_JumpData.m_bCreateJumpEndEffect)
					{
						pPlayer->m_JumpData.m_bCreateJumpEndEffect=TRUE;
						pPlayer->SetVisiblePos(vVisiblePos);
						pPlayer->GetBasePart()->SetPosition(vVisiblePos);
						OnFootStep(pPlayer);
					}				
					pPlayer->m_JumpData.m_fJumpEndFrame=0;
				}
			}
			else
			{
				if(vVisiblePos.z-pPlayer->m_JumpData.m_JumpTargetPosition.z<=1.0f)
				{

					if(vVisiblePos.z<=pPlayer->m_JumpData.m_JumpTargetPosition.z)
						vVisiblePos.z=pPlayer->m_JumpData.m_JumpTargetPosition.z;


					if(!pPlayer->m_JumpData.m_bCreateJumpEndEffect)
					{
						pPlayer->m_JumpData.m_bCreateJumpEndEffect=TRUE;
						pPlayer->SetVisiblePos(vVisiblePos);
						pPlayer->GetBasePart()->SetPosition(vVisiblePos);
						OnFootStep(pPlayer);
					}				
					pPlayer->m_JumpData.m_fJumpEndFrame=0;
				}
			}
		}
		else 
		{

			float fPos1Z = pPlayer->m_JumpData.m_SrcZ * (1.0f - 0.35f) + pPlayer->m_JumpData.m_SrcZ* (0.35f);
			float fPosZ2 = pPlayer->m_JumpData.m_SrcZ * (1.0f - 0.65f) + pPlayer->m_JumpData.m_SrcZ* (0.65f);

			fPos1Z+=pPlayer->GetJumpHeight();
			fPosZ2+=pPlayer->GetJumpHeight();

			float vCurPosZ=pPlayer->m_JumpData.m_SrcZ*((1.0f - fRate)*(1.0f - fRate)*(1.0f - fRate)) +
				fPos1Z*fRate*3*((1.0f - fRate)*(1.0f - fRate))+
				fPosZ2*3*fRate*fRate*(1.0f - fRate)+
				+pPlayer->m_JumpData.m_SrcZ * (fRate*fRate*fRate);

			vVisiblePos.z=vCurPosZ;	

			if(vVisiblePos.z<=vTest.z)
			{					
				vVisiblePos.z=vTest.z;

				g_pPathExplorer->Stop();
				
				g_pSunTerrain->PathExp_Reset(g_pPathExplorer, &vVisiblePos, -1);
				WzVector vPos=g_pPathExplorer->GetVisiblePos();
				float fHeight=fabs((vPos.z-vTest.z));

				if(bFind&&fHeight<=5.0f&&fRate>=0.5f)
				{
					
					bEnd=TRUE;
                    pPlayer->m_JumpData.m_JumpTargetPosition = vVisiblePos;
					pPlayer->m_JumpData.m_dwHorizonMoveTime=0;
					
					if(!pPlayer->m_JumpData.m_bCreateJumpEndEffect)
					{
						pPlayer->m_JumpData.m_bCreateJumpEndEffect=TRUE;
						pPlayer->SetVisiblePos(vVisiblePos);
						pPlayer->GetBasePart()->SetPosition(vVisiblePos);
						pPlayer->SetPosition(vVisiblePos);

						if(g_pHero==pPlayer)
							SendStopPacket();
					}				

					pPlayer->m_JumpData.m_fJumpEndFrame=0;
				}
			}
		}

		pPlayer->SetVisiblePos(vVisiblePos);
		pPlayer->GetBasePart()->SetPosition(vVisiblePos);
		pPlayer->SetFreeMove(TRUE);
	}
	pPlayer->GetBasePart()->SetPosition(pPlayer->GetVisiblePos());
	pPlayer->UpdateOctree(TRUE);

   //점프중 라이딩상태 변경(소환.해제)이 일어났을경우 OnProcess를 끝낸다
   if(CheckRidingState(pPlayer))
   {
      return STATEPROCESS::END;
   }

   if( pPlayer->IsPolymorph() == TRUE )
   {
	   pPlayer->SetFreeMove(FALSE);
	   pPlayer->SetHeight(0.0f);

	   return STATEPROCESS::END;
   }

   if (g_pHero->GetNextState() == STATE::TRANSFORM)
   {
        if (g_pHero->GetTransformData().GetTransFormType() == eTRANSFORM_TYPE_NONE)
        {
            // 점프 중 용변신 해제가 왔을 때
            pPlayer->SetFreeMove(FALSE);
            pPlayer->SetHeight(0.0f);

            return STATEPROCESS::END;
        }
   }


	if(wzCurID==wzStartID)//점프시작
	{
		pPlayer->ProcessInterpolation(dwTick);

		fRet=pPlayer->ProcessAnimation(dwTick,&event);

		// 이벤트가 잡힐때까지 못움직인다.
		if(pPlayer->m_JumpData.m_bJumpStart==FALSE)
		{
			if (event.m_iNumCustom)
			{
				for (int a = 0; a < event.m_iNumCustom; a++)
				{
					// 타격점에 오른쪽치기인지 왼쪽치기인지 결정한다.
					switch(event.m_Custom[a].m_iaParam3[0])
					{
						// 1번 event는 공격 타이밍
					case 1:
						{
							if (event.m_Custom[a].m_iaParam3[1] == 1) 
							{
								pPlayer->m_JumpData.m_bJumpStart=TRUE;
							}
						}
						break;
					}
				}
			}
		}

		if (fRet >= 100.0f)
		{
           pPlayer->SetPlayerAnimation(pPlayer->GetCurrentJumpKeepAnimation(),eANI_JUMP_LOOP,TRUE);
		   pPlayer->ProcessAnimation(0);
		}
	}
	else if(wzCurID==wzKeepID)
	{
		//pPlayer->SetAnimation(pPlayer->GetCurrentJumpKeepAnimation(),TRUE);
        pPlayer->SetPlayerAnimation(pPlayer->GetCurrentJumpKeepAnimation(),eANI_JUMP_LOOP,TRUE);
		pPlayer->ProcessInterpolation(dwTick);
		pPlayer->ProcessAnimation(dwTick,&event);

        
        // 낙하중 모션블러 적용
        if ((pPlayer->m_JumpData.m_bJumpStun ==  TRUE) &&
           (vertical_move_rate > 1.15f))
        {
            SetMotionBlur(pPlayer, true);
        }
	
		if(bEnd)
		{
			if(pPlayer->m_JumpData.m_bJumpStun&&!pPlayer->IsDead())
			{
				pPlayer->SetPlayerAnimation(pPlayer->GetCurrentJumpStunAnimation(),eANI_HJUMP_END,FALSE);

                // 낙하 스턴은 바로 모션블러꺼줌
                SetMotionBlur(pPlayer, false);
			}
			else
			{
				pPlayer->SetPlayerAnimation(pPlayer->GetCurrentJumpEndAnimation(),eANI_JUMP_END,FALSE);
			}
		}
	}
	else if(wzCurID==wzEndID)
	{
		if(pPlayer->GetVisiblePos().z-pPlayer->m_JumpData.m_JumpTargetPosition.z<=0.8f)
		{
			pPlayer->m_JumpData.m_fJumpEndFrame=0;
		}


		if (pPlayer->m_JumpData.m_fJumpEndFrame)
		{
			pPlayer->SetCurrentFrame(pPlayer->m_JumpData.m_fJumpEndFrame);
			pPlayer->ProcessInterpolation(dwTick);

			pPlayer->ProcessAnimation(0,&event);
		}
		else
		{
			pPlayer->ProcessInterpolation(dwTick);

			fRet=pPlayer->ProcessAnimation(dwTick,&event);

			if (event.m_iNumCustom)
			{
				for (int a = 0; a < event.m_iNumCustom; a++)
				{
					switch(event.m_Custom[a].m_iaParam3[0])
					{

					case 1:
						{
							if (event.m_Custom[a].m_iaParam3[1] == 1) 
							{
								pPlayer->m_JumpData.m_fJumpEndFrame=pPlayer->GetCurrentFrame();
							}
						}
						break;
					}
				}
			}
		}

		int iMoveFlag = pPlayer->m_JumpData.m_JumpFlag.GetMoveFlag();
		if( pPlayer->GetVisiblePos() == pPlayer->m_JumpData.m_JumpTargetPosition && 
			iMoveFlag &&
			(pPlayer->CheckMoveFlag(KEYBOARDMOVE_FOWARD) ||
			pPlayer->CheckMoveFlag(KEYBOARDMOVE_BACKWARD) ||
			pPlayer->CheckMoveFlag(KEYBOARDMOVE_RIGHTSTEP) ||
			pPlayer->CheckMoveFlag(KEYBOARDMOVE_LEFTSTEP) ||
            pPlayer->IsDead() ) )
        {
            return STATEPROCESS::END;
        }

        if ((pPlayer->GetVisiblePos() == pPlayer->m_JumpData.m_JumpTargetPosition) &&
            (fRet == 100.0f))
        {
            return STATEPROCESS::END;
        }
	}
	else if(wzCurID==wzStunID)
	{
		pPlayer->ProcessInterpolation(dwTick);

		fRet=pPlayer->ProcessAnimation(dwTick,&event);

		if(fRet==100.0f)
		{			
			pPlayer->SetPlayerAnimation(pPlayer->GetCurrentJumpStunKeepAnimation(),eANI_HJUMP_END_LOOP,FALSE);
			pPlayer->ProcessAnimation(0);
		}

	}
	else if(wzCurID==wzStunKeepID)
	{
		pPlayer->m_JumpData.m_JumpStunTime-=dwTick;
		pPlayer->m_JumpData.m_JumpStunTime=max(0,pPlayer->m_JumpData.m_JumpStunTime);
		pPlayer->m_JumpData.m_JumpStunTime=min(1000,pPlayer->m_JumpData.m_JumpStunTime);

		pPlayer->ProcessInterpolation(dwTick);

		fRet=pPlayer->ProcessAnimation(dwTick,&event);

		if(pPlayer->GetVisiblePos().z==pPlayer->m_JumpData.m_JumpTargetPosition.z&&fRet==100.0f)
		{
			if(pPlayer->m_JumpData.m_hStunEffect!=INVALID_HANDLE_VALUE)
				g_pEffectManager->RemoveEffectPtr(pPlayer->m_JumpData.m_hStunEffect);

			//pPlayer->SetAnimation(pPlayer->GetCurrentJumpStunEndAnimation(),FALSE);		
            pPlayer->SetPlayerAnimation(pPlayer->GetCurrentJumpStunEndAnimation(), eANI_HJUMP_GETUP, FALSE);		
		}
	}	
	else if(wzCurID==wzStunEndID)//Todo;IDLE 상태에서 처리 해야 한다.
	{
		pPlayer->ProcessInterpolation(dwTick);

		fRet=pPlayer->ProcessAnimation(dwTick,&event);

		if(fRet==100.0f)
			return STATEPROCESS::END;
	}
	else
	{
		pPlayer->ProcessInterpolation(dwTick);

		fRet=pPlayer->ProcessAnimation(dwTick,&event);
	}
	
	return STATEPROCESS::HOLD;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateKeyboardJump::OnEnd(Object* pOwner, DWORD CurTime)
{
	WzVector vDummy;
	vDummy.x = 0.0f;
	vDummy.y = 0.0f;
	vDummy.z = 0.0f;
	Player* pPlayer = (Player*)pOwner;

	g_pPathExplorer->Stop();

	//Todo:잘못된 타일일때 오류 처리 할것.
	if(g_pSunTerrain->PathExp_Reset(g_pPathExplorer, &pPlayer->GetVisiblePos(), -1))
	{
		WzVector vPos=g_pPathExplorer->GetPos();
		pPlayer->SetPosition(vPos);
		pPlayer->SetVisiblePos(g_pPathExplorer->GetVisiblePos());
		pPlayer->GetBasePart()->SetPosition(g_pPathExplorer->GetVisiblePos());
	}

	pPlayer->m_JumpData.m_bJumpStun=FALSE;
	pPlayer->m_JumpData.m_JumpFlag.Reset();
    pPlayer->SetKeyBoardJumpFlag(FALSE);
	
	//타락한 사원에서 떨어져 죽을땐 하면 안됨
	int iTileFocussing = g_pSunTerrain->x_pPathFinder->GetTileToStand(pPlayer->m_JumpData.m_JumpTargetPosition, NULL, -1.0f, 100.0f);
	if (iTileFocussing >= 0)
	{
		if (!(g_pSunTerrain->x_pPathFinder->GetAttribute(iTileFocussing) & PTA_PLAYER_FALLDOWN_TILE))
		{
			pPlayer->SetFreeMove(FALSE);
			pPlayer->SetHeight(0.0f);
		}
	}

#ifdef _NA_000000_20120410_SPEEDHACK
    if (pPlayer == g_pHero)
    {
        g_pHero->move_speed_ = GetBaseMoveSpeedAsState(CMS_RUN);
    }
    else
    {
        pPlayer->SetMoveSpeed(GetBaseMoveSpeedAsState(CMS_RUN));
    }
#else
    pPlayer->SetMoveSpeed(GetBaseMoveSpeedAsState(CMS_RUN));
#endif //_NA_000000_20120410_SPEEDHACK


	if (pPlayer->m_JumpData.m_hStunEffect != INVALID_HANDLE_VALUE)
    {
		g_pEffectManager->RemoveEffectPtr(pPlayer->m_JumpData.m_hStunEffect);
    }

    SetMotionBlur(pOwner, false);

	return STATECHANGE::SUCCESS;
}

void StateKeyboardJump::SetMotionBlur(Object* object, bool flag)
{
    if (object != g_pHero)
    {
        return;
    }

    if (flag)
    {
        g_pSunRenderer->x_pManagerPostEffect->TurnOnPostEffect( PET_MOTION_BLUR );
    }
    else
    {
        g_pSunRenderer->x_pManagerPostEffect->TurnOffPostEffect( PET_MOTION_BLUR );
    }
}

void StateKeyboardJump::OnFootStep(Object* pOwner)
{
	Player* pPlayer = (Player*)pOwner;
        
	if(pPlayer->IsObserverMode())
	{
		return;
	}
    pPlayer->SetKeyBoardJumpFlag(FALSE);
	pPlayer->SetPosition(pPlayer->GetVisiblePos());

	CEffectBody *pBody=NULL;

	WzVector vPos = pPlayer->GetVisiblePos();
	WzVector vPos2;
	WzVector vPos3;
	vPos3=vPos2=vPos;

	float fRand=float( rand()%3);
	fRand/=10.0f;
	
	float fRand2=float( rand()%4);
	if(fRand2==0)
	{
		vPos3.x+=fRand;
		vPos3.y+=fRand;

		vPos2.x+=fRand;
		vPos2.y+=fRand;
	}
	else if(fRand2==1)
	{
		vPos3.x-=fRand;
		vPos3.y-=fRand;

		vPos2.x-=fRand;
		vPos2.y-=fRand;

	}
	else if(fRand2==2)
	{
		vPos3.x-=fRand;
		vPos3.y-=fRand;

		vPos2.x+=fRand;
		vPos2.y+=fRand;

	}
	else 
	{
		vPos3.x+=fRand;
		vPos3.y+=fRand;

		vPos2.x-=fRand;
		vPos2.y-=fRand;

	}

	if(g_pHero==pPlayer)
	{
		pPlayer->Player::OnFootStep(TRUE);
	}

	if(pPlayer->m_JumpData.m_bJumpStun)
	{
		WzVector vRet;
		vRet = pPlayer->GetHeadBonePos();
		vRet.z+=0.1f;

	
	}

	HANDLE handle;

	// 지형속성에 따라 이펙트를 뿌려준다
	switch(pPlayer->GetTerrainAttribute())
	{
	default:
	case eTERRAIN_ATTRIBUTE_DIRT:
		{
			handle=GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A000"),vPos,pPlayer->GetObjectKey());
			pBody = g_pEffectManager->GetEffectPtrHandle(handle);
			if(pBody)
			{
	
				WzVector vRot;
				vRot.x = 0.0f;
				vRot.y = 0.0f;
				vRot.z = 0.0f;
				WzVector vScale;
				vScale.x = float( rand()%20);
				vScale.y = float( rand()%20);
			    vScale.z = float( rand()%20);

				vScale.x/=10.0f;
				vScale.y/=10.0f;
			    vScale.z/=10.0f; 
				
				vScale.x=max(vScale.x,1.0f);
				vScale.y=max(vScale.y,1.0f);
				vScale.z=max(vScale.y,1.0f);

				pBody->SetResultMatrix(vPos,vRot,vScale);
			}

			handle=GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A000"),vPos2,pPlayer->GetObjectKey());
			CEffectBody *pBody = g_pEffectManager->GetEffectPtrHandle(handle);
			if(pBody)
			{

				WzVector vRot;
				vRot.x = 0.0f;
				vRot.y = 0.0f;
				vRot.z = 0.0f;
				WzVector vScale;
				vScale.x = float( rand()%20);
				vScale.y = float( rand()%20);
				vScale.z = float( rand()%20);

				vScale.x/=10.0f;
				vScale.y/=10.0f;
				vScale.z/=10.0f; 

				vScale.x=max(vScale.x,1.0f);
				vScale.y=max(vScale.y,1.0f);
				vScale.z=max(vScale.y,1.0f);

				pBody->SetResultMatrix(vPos2,vRot,vScale);
			}

			handle=GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A000"),vPos3,pPlayer->GetObjectKey());
			pBody = g_pEffectManager->GetEffectPtrHandle(handle);
			if(pBody)
			{

				WzVector vRot;
				vRot.x = 0.0f;
				vRot.y = 0.0f;
				vRot.z = 0.0f;
				WzVector vScale;
				vScale.x = float( rand()%20);
				vScale.y = float( rand()%20);
				vScale.z = float( rand()%20);

				vScale.x/=10.0f;
				vScale.y/=10.0f;
				vScale.z/=10.0f; 

				vScale.x=max(vScale.x,1.0f);
				vScale.y=max(vScale.y,1.0f);
				vScale.z=max(vScale.y,1.0f);

				pBody->SetResultMatrix(vPos3,vRot,vScale);
			}
		}
		break;


	case eTERRAIN_ATTRIBUTE_GRASS:
		{
			handle=GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A042"),vPos,pPlayer->GetObjectKey());
			pBody = g_pEffectManager->GetEffectPtrHandle(handle);
			if(pBody)
			{

				WzVector vRot;
				vRot.x = 0.0f;
				vRot.y = 0.0f;
				vRot.z = 0.0f;
				WzVector vScale;
				vScale.x = float( rand()%20);
				vScale.y = float( rand()%20);
				vScale.z = float( rand()%20);

				vScale.x/=10.0f;
				vScale.y/=10.0f;
				vScale.z/=10.0f; 

				vScale.x=max(vScale.x,1.0f);
				vScale.y=max(vScale.y,1.0f);
				vScale.z=max(vScale.y,1.0f);
				pBody->SetResultMatrix(vPos,vRot,vScale);
			}


			handle=GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A042"),vPos2,pPlayer->GetObjectKey());
			pBody = g_pEffectManager->GetEffectPtrHandle(handle);
			if(pBody)
			{

				WzVector vRot;
				vRot.x = 0.0f;
				vRot.y = 0.0f;
				vRot.z = 0.0f;
				WzVector vScale;
				vScale.x = float( rand()%20);
				vScale.y = float( rand()%20);
				vScale.z = float( rand()%20);

				vScale.x/=10.0f;
				vScale.y/=10.0f;
				vScale.z/=10.0f; 

				vScale.x=max(vScale.x,1.0f);
				vScale.y=max(vScale.y,1.0f);
				vScale.z=max(vScale.y,1.0f);
				pBody->SetResultMatrix(vPos2,vRot,vScale);
			}


			handle=GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A042"),vPos3,pPlayer->GetObjectKey());
			pBody = g_pEffectManager->GetEffectPtrHandle(handle);
			if(pBody)
			{

				WzVector vRot;
				vRot.x = 0.0f;
				vRot.y = 0.0f;
				vRot.z = 0.0f;
				WzVector vScale;
				vScale.x = float( rand()%20);
				vScale.y = float( rand()%20);
				vScale.z = float( rand()%20);

				vScale.x/=10.0f;
				vScale.y/=10.0f;
				vScale.z/=10.0f; 

				vScale.x=max(vScale.x,1.0f);
				vScale.y=max(vScale.y,1.0f);
				vScale.z=max(vScale.y,1.0f);
				pBody->SetResultMatrix(vPos3,vRot,vScale);
			}


		}
		break;


	case eTERRAIN_ATTRIBUTE_WATER:
		{
			vPos.z+=0.5f;
			vPos2.z+=0.5f;
			vPos3.z+=0.5f;

            handle = GlobalFunc::CreateEffectBeUpToOption(StrToWzID("I695"), 
                                                          pPlayer->GetWaterTerrainPos(), 
                                                          pPlayer->GetObjectKey());
		}
		break;


	case eTERRAIN_ATTRIBUTE_SNOW:
		{
			handle=GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A003"),vPos,pPlayer->GetObjectKey());
			pBody = g_pEffectManager->GetEffectPtrHandle(handle);
			if(pBody)
			{

				WzVector vRot;
				vRot.x = 0.0f;
				vRot.y = 0.0f;
				vRot.z = 0.0f;
				WzVector vScale;
				vScale.x = float( rand()%20);
				vScale.y = float( rand()%20);
				vScale.z = float( rand()%20);

				vScale.x/=10.0f;
				vScale.y/=10.0f;
				vScale.z/=10.0f; 

				vScale.x=max(vScale.x,1.0f);
				vScale.y=max(vScale.y,1.0f);
				vScale.z=max(vScale.y,1.0f);

				pBody->SetResultMatrix(vPos,vRot,vScale);
			}

			handle=GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A003"),vPos2,pPlayer->GetObjectKey());
			pBody = g_pEffectManager->GetEffectPtrHandle(handle);
			if(pBody)
			{

				WzVector vRot;
				vRot.x = 0.0f;
				vRot.y = 0.0f;
				vRot.z = 0.0f;
				WzVector vScale;
				vScale.x = float( rand()%20);
				vScale.y = float( rand()%20);
				vScale.z = float( rand()%20);

				vScale.x/=10.0f;
				vScale.y/=10.0f;
				vScale.z/=10.0f; 

				vScale.x=max(vScale.x,1.0f);
				vScale.y=max(vScale.y,1.0f);
				vScale.z=max(vScale.y,1.0f);
				pBody->SetResultMatrix(vPos2,vRot,vScale);
			}


			handle=GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A003"),vPos3,pPlayer->GetObjectKey());
			pBody = g_pEffectManager->GetEffectPtrHandle(handle);
			if(pBody)
			{

				WzVector vRot;
				vRot.x = 0.0f;
				vRot.y = 0.0f;
				vRot.z = 0.0f;
				WzVector vScale;
				vScale.x = float( rand()%20);
				vScale.y = float( rand()%20);
				vScale.z = float( rand()%20);

				vScale.x/=10.0f;
				vScale.y/=10.0f;
				vScale.z/=10.0f; 

				vScale.x=max(vScale.x,1.0f);
				vScale.y=max(vScale.y,1.0f);
				vScale.z=max(vScale.y,1.0f);
				pBody->SetResultMatrix(vPos3,vRot,vScale);
			}

		}
		break;
	case eTERRAIN_ATTRIBUTE_DESERT:
		{
			handle=GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A001"),vPos,pPlayer->GetObjectKey());
			pBody = g_pEffectManager->GetEffectPtrHandle(handle);
			if(pBody)
			{

				WzVector vRot;
				vRot.x = 0.0f;
				vRot.y = 0.0f;
				vRot.z = 0.0f;
				WzVector vScale;
				vScale.x = float( rand()%20);
				vScale.y = float( rand()%20);
				vScale.z = float( rand()%20);

				vScale.x/=10.0f;
				vScale.y/=10.0f;
				vScale.z/=10.0f; 

				vScale.x=max(vScale.x,1.0f);
				vScale.y=max(vScale.y,1.0f);
				vScale.z=max(vScale.y,1.0f);
				pBody->SetResultMatrix(vPos,vRot,vScale);
			}


			handle=GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A001"),vPos2,pPlayer->GetObjectKey());
			pBody = g_pEffectManager->GetEffectPtrHandle(handle);
			if(pBody)
			{

				WzVector vRot;
				vRot.x = 0.0f;
				vRot.y = 0.0f;
				vRot.z = 0.0f;
				WzVector vScale;
				vScale.x = float( rand()%20);
				vScale.y = float( rand()%20);
				vScale.z = float( rand()%20);

				vScale.x/=10.0f;
				vScale.y/=10.0f;
				vScale.z/=10.0f; 

				vScale.x=max(vScale.x,1.0f);
				vScale.y=max(vScale.y,1.0f);
				vScale.z=max(vScale.y,1.0f);
				pBody->SetResultMatrix(vPos2,vRot,vScale);
			}


			handle=GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A001"),vPos3,pPlayer->GetObjectKey());

			pBody = g_pEffectManager->GetEffectPtrHandle(handle);
			if(pBody)
			{

				WzVector vRot;
				vRot.x = 0.0f;
				vRot.y = 0.0f;
				vRot.z = 0.0f;
				WzVector vScale;
				vScale.x = float( rand()%20);
				vScale.y = float( rand()%20);
				vScale.z = float( rand()%20);

				vScale.x/=10.0f;
				vScale.y/=10.0f;
				vScale.z/=10.0f; 

				vScale.x=max(vScale.x,1.0f);
				vScale.y=max(vScale.y,1.0f);
				vScale.z=max(vScale.y,1.0f);
				pBody->SetResultMatrix(vPos3,vRot,vScale);
			}


		}
		break;

	}
}


//------------------------------------------------------------------------------
/**
*/
void StateKeyboardJump::CalculateTurnAccelerate(DWORD dwTick)
{
	const  float l_fSpeedAccelerator = 0.002f;
	float  fTurnSpeed = 0.0f;
	float  fMaxTurnSpeed = 0.04f;


	int oldturnflag = g_pHero->GetOldMoveFlag() & (KEYBOARDMOVE_LEFTTURN | KEYBOARDMOVE_RIGHTTURN);
	int newturnflag = g_pHero->GetMoveFlag() & (KEYBOARDMOVE_LEFTTURN | KEYBOARDMOVE_RIGHTTURN);

	if (oldturnflag == newturnflag && newturnflag)
	{
		if (newturnflag & KEYBOARDMOVE_LEFTTURN)
		{
			g_pHero->m_fTurnAccelerate += l_fSpeedAccelerator;
		}
		else 
		{
			g_pHero->m_fTurnAccelerate -= l_fSpeedAccelerator;
		}
	}
	else
	{
		g_pHero->m_fTurnAccelerate *= 0.7f;
		if (fabs(g_pHero->m_fTurnAccelerate) <= (l_fSpeedAccelerator * 0.1f))
		{
			g_pHero->m_fTurnAccelerate = 0.0f;
		}
	}

	if (g_pHero->GetMoveFlag() & (KEYBOARDMOVE_LEFTTURN | KEYBOARDMOVE_RIGHTTURN))
	{
		if (g_pHero->GetMoveFlag() & KEYBOARDMOVE_LEFTTURN)
		{
			g_pHero->m_fTurnAccelerate = l_fSpeedAccelerator;
		}
		else 
		{
			g_pHero->m_fTurnAccelerate = -l_fSpeedAccelerator;
		}
	}
	else
	{
		g_pHero->m_fTurnAccelerate = 0.0f;
	}		

	fTurnSpeed += g_pHero->m_fTurnAccelerate;

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
		g_pHero->Rotate(fTurnSpeed * dwTick,TRUE);		
	}
}
#endif

