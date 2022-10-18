//------------------------------------------------------------------------------
//  StateKeyboardMove.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#ifdef _NA_20120220_JUMP_RENEWAL
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


static float s_fMaxUpper = -40.0f;      // 수직 낙하시의 제한치

static float s_fJumpUpperForce = 11.0f; // 점프 초기 속도 (수직)
static float s_fJumpMoveForce = 11.7f;  // 점프 초기 속도 (수평)

static float s_fDownGravity = 20.0f;    // 수직 속도에 적용할 중력치
static float s_fMoveGravity = 7.0f;     // 수평이동 속도에 적용할 중력치


static float s_fMoveableMinDistance = 0.25f;    // 낙하중 지면에서 수평이동을 가능하게할 최소한의 거리 - 착지모션에 미끄러지듯 이동하지 않기위해
static float s_fJumpEffectHeight = 0.5f;
                                
static float s_fRateForTick1 = 1.0f;    // 시간당 중력치 차감 비율 (수평)
static float s_fRateForTick2 = 1.0f;    // 시간당 중력치 차감 비율 (수직)


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

BOOL StateKeyboardJump::CheckJumpTile(WzVector vSrc, WzVector vDest)
{
	if(!g_pPathExplorer || !g_pMap)
		return FALSE;

	g_pPathExplorer->Stop();

	// 대상 Tile 얻기	

	int iDestTileFocussing = g_pSunTerrain->x_pPathFinder->GetTileToStand( vDest, NULL, -1.f, 100.0f );
	if( iDestTileFocussing < 0 )
	{
		return FALSE;
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
			return FALSE;
	}

	int iSrcTileFocussing = g_pSunTerrain->x_pPathFinder->GetTileToStand( vSrc, NULL, -1.f, 100.0f );
	if( iSrcTileFocussing < 0 )
	{
		return FALSE;
	}

    WORD Src_tile_attribute = g_pMap->GetWorldBase()->x_pPathFinder->GetAttribute(iSrcTileFocussing);
    WORD dest_tile_attribute = g_pMap->GetWorldBase()->x_pPathFinder->GetAttribute(iDestTileFocussing);
#if WZENGINEVER >= 270
    if ((Src_tile_attribute & PTA_JUMP_CONTROL_A) &&
        (dest_tile_attribute & PTA_JUMP_CONTROL_B))
    {
        return FALSE;
    }
    if ((Src_tile_attribute & PTA_JUMP_CONTROL_B) &&
        (dest_tile_attribute & PTA_JUMP_CONTROL_A))
    {
        return FALSE;
    }
#endif //WZENGINEVER >= 270

	if ((Src_tile_attribute & PTA_PLAYER_JUMP_TILE) &&
		(dest_tile_attribute & PTA_PLAYER_JUMP_TILE))
	{
		return TRUE;
	}

	return FALSE;
}

BOOL StateKeyboardJump::TestMoveablePosition( const WzVector& StartPosition, WzVector& TargetPosition )
{
    WzVector vStart = StartPosition;

    g_pPathExplorer->Stop();
    if( g_pSunTerrain->PathExp_Reset(g_pPathExplorer, &vStart, -1) == FALSE )
    {
        return FALSE;
    }


    WzVector vOldPosition = g_pPathExplorer->GetPos();
    WzVector vDirection = TargetPosition - StartPosition;

    g_pSunTerrain->PathExp_Thrust( g_pPathExplorer, &vDirection, PTA_NO_WALK );
    g_pSunTerrain->PathExp_Progress( g_pPathExplorer, 1000000, NULL );

    if( vOldPosition != g_pPathExplorer->GetPos() )
    {
        // 새 위치
        TargetPosition = g_pPathExplorer->GetPos();
        return TRUE;
    }

    return FALSE;
}

//------------------------------------------------------------------------------ 
bool StateKeyboardJump::GetJumpDestPosByDirection(OUT WzVector& dest_pos, WzVector& scr_pos, WzVector& direction, float player_height, float min_dist, float max_dist)
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

                WzVector backup_pos = g_pHero->m_JumpData.m_vJumpTargetPosition;

                g_pHero->m_JumpData.m_vJumpTargetPosition = dest_pos;
                WzVector result;
                if( FindEnablePathPosition( g_pHero->m_JumpData.m_vJumpStartPosition, g_pHero->m_JumpData.m_vJumpTargetPosition, result ) == TRUE )
                {
                    g_pHero->m_vTargetPosition = result;
                    int tile_no = g_pPathExplorer->GetTile();

                    if ((tile_no >= 0) && 
                        ((g_pSunTerrain->x_pPathFinder->GetAttribute(tile_no) & PTA_NO_WALK) == false))
                    {
                        g_pHero->m_JumpData.m_vJumpTargetPosition = dest_pos;
                        return true;
                    }
                }

                g_pHero->m_JumpData.m_vJumpTargetPosition = backup_pos;
            }
        }
    }

    return false;
}

//------------------------------------------------------------------------------ 
BOOL StateKeyboardJump::GetJumpDirection(OUT WzVector& direction, int flag, WzVector& foward)
{
    SetVector( &direction, 0.0f, 0.0f, 0.0f);

    WzVector vRight, vLeft;
    vRight.x =  foward.y; // sin(a - 90) = -cos(a) = vFoward.y;
    vRight.y = -foward.x; // -cos(a - 90) = -sin(a) = -vFoward.y;
    vRight.z =  0.0f;

    vLeft.x = -foward.y; // sin(a + 90) = cos(a) = -vFoward.y;
    vLeft.y =  foward.x; // -cos(a + 90) = sin(a) = vFoward.x;
    vLeft.z =  0.0f;

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
    }

    if( direction.x == 0.0f && direction.y == 0.0f && direction.z == 0.0f )
    {
        return FALSE;
    }

    VectorNormalize( &direction, &direction );

    return TRUE;
}

void CatmulRom( WzVector& Out, const WzVector& P1, const WzVector& P2, const WzVector& P3, const WzVector& P4, float s )
{
    float s2 = s*s;
    float s3 = s2*s;
    Out.x = 0.5f * (( 2.0f * P2.x ) +
        ( -P1.x + P3.x ) * s +
        ( 2.0f * P1.x - 5.0f * P2.x + 4.0f * P3.x - P4.x ) * s2 +
        ( -P1.x + 3.0f * P2.x - 3.0f * P3.x + P4.x ) * s3 );

    Out.y = 0.5f * (( 2.0f * P2.y ) +
        ( -P1.y + P3.y ) * s +
        ( 2.0f * P1.y - 5.0f * P2.y + 4.0f * P3.y - P4.y ) * s2 +
        ( -P1.y + 3.0f * P2.y - 3.0f * P3.y + P4.y ) * s3 );

    Out.z = 0.5f * (( 2.0f * P2.z ) +
        ( -P1.z + P3.z ) * s +
        ( 2.0f * P1.z - 5.0f * P2.z + 4.0f * P3.z - P4.z ) * s2 +
        ( -P1.z + 3.0f * P2.z - 3.0f * P3.z + P4.z ) * s3 );
};

bool StateKeyboardJump::MovePositionOnSimulatedPosition( Player* pPlayer, float fTick, WzVector& Position, const WzVector& Direction, float& fMoveForce, float& fUpperForce )
{
    const std::vector<WzVector>& simulated_position_array = pPlayer->m_JumpData.m_JumpPositionArray;
    //pPlayer->m_fJumpTick += fTick;

    if( simulated_position_array.size() < 2 )
    {
        return false;
    }

    float current_tick = pPlayer->m_JumpData.m_fJumpTick;

    float tick_per_frame = 1.0f / 30.0f;
    size_t frame_index = (size_t)(current_tick / tick_per_frame);
    float rate = (current_tick - (tick_per_frame*(float)frame_index) ) / tick_per_frame;


    WzVector p1,p2,p3,p4;
    if( frame_index > 0 && frame_index < simulated_position_array.size() -2 )
    {
        p1 = simulated_position_array[frame_index-1];
        p2 = simulated_position_array[frame_index];
        p3 = simulated_position_array[frame_index+1];
        p4 = simulated_position_array[frame_index+2];
    }
    else if( frame_index == 0 && frame_index < simulated_position_array.size() -2 )
    {
        //p1 = simulated_position_array[frame_index];
        p2 = simulated_position_array[frame_index];
        p3 = simulated_position_array[frame_index+1];
        p4 = simulated_position_array[frame_index+2];

        WzVector d = p3-p2;
        p1 = p2 - d;
    }
    else if( frame_index > 0 && frame_index < simulated_position_array.size() -1 )
    {
        p1 = simulated_position_array[frame_index-1];
        p2 = simulated_position_array[frame_index];
        p3 = simulated_position_array[frame_index+1];
        //p4 = simulated_position_array[frame_index+1];

        WzVector d = p3-p2;
        p4 = p3 + d;
    }
    else
    {
        return false;
    }

    CatmulRom( Position, p1, p2, p3, p4, rate );

    
    // 1. 수평이동할 위치를 먼저 구한다
    if( fMoveForce > 0.0f )
    {
        float fDelta = fMoveForce * fTick * s_fRateForTick1;
        WzVector vDelta = Direction * fDelta;

        fMoveForce -= s_fMoveGravity * fTick;
        fMoveForce = max( fMoveForce, 0.0f );
    }


    // 3. 수직 이동
    float fDelta = fUpperForce * fTick * s_fRateForTick2;

    fUpperForce -= s_fDownGravity * fTick;   // 1초에 s_fDownGravity 감소
    fUpperForce = max( fUpperForce, s_fMaxUpper );

    return true;

}

void StateKeyboardJump::MovePositionOnJump( const WzVector& Direction, float fTick, WzVector& Position, float& fMoveForce, float& fUpperForce )
{
    // 1. 수평이동할 위치를 먼저 구한다
    if( fMoveForce > 0.0f )
    {
        float fDelta = fMoveForce * fTick;
        WzVector vDelta = Direction * fDelta;

        Position.x += vDelta.x;
        Position.y += vDelta.y;

        fMoveForce -= s_fMoveGravity * fTick;
        fMoveForce = max( fMoveForce, 0.0f );
    }


    // 3. 수직 이동
    float fDelta = fUpperForce * fTick * s_fRateForTick2;
    Position.z += fDelta;

    fUpperForce -= s_fDownGravity * fTick;   // 1초에 s_fDownGravity 감소
    fUpperForce = max( fUpperForce, s_fMaxUpper );

}


BOOL StateKeyboardJump::SimulateJump( const WzVector& StartPosition, const WzVector& Direction, OUT WzVector& ResultPosition, OUT std::vector<WzVector>& SimulatedPositionArray )
{
    // 점프의 목적지, 시작위치, 방향을 설정 (목적지는 수평점프 거리에 피킹한 위치 )
    float fCurrentTick = 0.0f;
    float fMoveForce = s_fJumpMoveForce;
    float fUpperForce = s_fJumpUpperForce;


    WzVector vTestPosition = StartPosition;
    WzVector LastStandablePosition = StartPosition; 
    WzVector vLastPosition;

    // 20프레임단위로 가상으로 점프를 시뮬레이션해서
    // 최종 목적지의 수평상의 위치를 구한다.

    SimulatedPositionArray.clear();
    SimulatedPositionArray.push_back(StartPosition);

    float fMoveTick = 1.0f / 30;    // 30프레임으로 테스트
    while( true )
    {
        vLastPosition = vTestPosition;
        MovePositionOnJump( Direction, fMoveTick, vTestPosition, fMoveForce, fUpperForce );
        fCurrentTick += fMoveTick;

        // 오브젝트에 부딧치면 수평이동은 중지
        if( TestObjectCollision( vLastPosition, vTestPosition ) == TRUE )
        {
            break; 
        }

        // 착지가 가능한 위치인가
        WzVector vTemp = vTestPosition;

        if( fUpperForce < 0.0f )
        {
            vTemp.z = vLastPosition.z;
        }

        if( TestMoveableTerrainPosition( StartPosition, vTemp ) == TRUE )
        {
            
            LastStandablePosition = vTemp;

            // 다운중이고, 지면의 위치와 0.5이내라면 착지로 간주
            float fDistanceToLand = vTestPosition.z - vTemp.z;
            if( fUpperForce < 0.0f && fDistanceToLand < s_fMoveableMinDistance )
            {
                break;
            }
        }

        if( fMoveForce <= 0.0f )
        {
            break;
        }

        SimulatedPositionArray.push_back( vTestPosition );
    }

    ResultPosition = LastStandablePosition;

    return ResultPosition != StartPosition;
}

// 목표 위치가 길찾기엔진상의 유효한 위치인지 확인
BOOL StateKeyboardJump::TestEnablePathPosition( const WzVector& TestPosition )
{
    WzVector vVisible = TestPosition;

    g_pPathExplorer->Stop();
    g_pSunTerrain->PathExp_Reset(g_pPathExplorer, &vVisible, -1);

    if( g_pPathExplorer->GetTile() < 0 )
        return FALSE;

    if( g_pPathExplorer->GetVisiblePos().z - TestPosition.z > 1.0f )
        return FALSE;

    return TRUE;
}

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

// 길찾기 매쉬상의  유효한 위치를 구한다.
BOOL StateKeyboardJump::FindEnablePathPosition( const WzVector& StartPosition, const WzVector& TargetPosition, OUT WzVector& ResultPosition )
{
    WzVector TestPosition;
    float fRate = 1.0f;

    //최대 10회까지만 찾는다
    for(int i=0;i<10;i++)
    {
        TestPosition =  StartPosition * (1.0f - fRate) +  TargetPosition * fRate;

        // 길찾기 매쉬의 속성설정에 양쪽 모두 점프가 가능한경우에는 유효한 위치
        if( CheckJumpTile( StartPosition, TestPosition ) )
        {
            g_pPathExplorer->Stop();
            g_pSunTerrain->PathExp_Reset(g_pPathExplorer, &TestPosition, -1);

            WzVector vPos = g_pPathExplorer->GetVisiblePos();
            if( vPos.z - g_pHero->GetVisiblePos().z <= g_pHero->GetJumpHeight() )
            {
                ResultPosition = vPos;
                return TRUE;
            }
        }

        fRate -= 0.1f;
    }

    g_pPathExplorer->Stop();


    // 길찾기 타일의 속성을 직접 검사후 이동가능한 위치면 유효하도록 적용하지만
    // 특정 씬에서는 너무 멀리 돌아가야하는 위치라면 유효하지 않도록 적용

    // 대상 Tile 얻기	
    int TargetTileIndex = g_pSunTerrain->x_pPathFinder->GetTileToStand( TargetPosition, NULL, -1.0f, 100.0f );
    if( TargetTileIndex < 0 )
    {
        return FALSE;
    }

    if (g_pMap->GetWorldBase()->x_pPathFinder->GetAttribute(TargetTileIndex) & (PTA_NO_WALK))
    {
        return FALSE;
    }


    if(GameFramework::GetCurrentScene())
    {
        SCENE_TYPE type = GameFramework::GetCurrentScene()->GetType();

        switch(type) 
        {
        case SCENE_TYPE_MISSION:
            if( GameFunc::IsMove( StartPosition, TargetPosition, TRUE, 200 ) )
            {
                ResultPosition = TargetPosition;
                return TRUE;
            }
            break;

        case SCENE_TYPE_DOMINATION:
            if( GameFunc::IsMove( StartPosition, TargetPosition, TRUE, 20 ) )
            {
                ResultPosition = TargetPosition;
                return TRUE;
            }
            break;

        case SCENE_TYPE_CHUNTING:
            if( GameFunc::IsMove( StartPosition, TargetPosition, TRUE, 1000 ) )
            {
                ResultPosition = TargetPosition;
                return TRUE;
            }
            break;
        default:

            ResultPosition = TargetPosition;
            return TRUE;
        }
    }

    return FALSE;
}

BOOL StateKeyboardJump::TestObjectCollision( const WzVector& TestPosition )
{
    // 캐릭터의 BV를 써야하지만 일단 대충 구로 하자
    WzBoundingVolume BoundingVolume;
    BoundingVolume.m_eBvType = BVT_SPHERE;
    BoundingVolume.m_BvSphere.m_wvCenter = TestPosition;
    BoundingVolume.m_BvSphere.m_fRadius = 0.3f;
    

    // 나무
    if( g_pMap->IntersectTree( &BoundingVolume ) == TRUE )
    {
        return TRUE;
    }

    // 오브젝트
    if( g_pMap->IntersectMapObject( &BoundingVolume ) == TRUE )
    {
        return TRUE;
    }

    return FALSE;
}

// StartPosition에서 TargetPosition 까지 오브젝트의 BV과 충돌되는가
BOOL StateKeyboardJump::TestObjectCollision( const WzVector& StartPosition, const WzVector& TargetPosition )
{
    if( TestObjectCollision( TargetPosition ) == TRUE )
    {
        return TRUE;
    }

    if( StartPosition == TargetPosition )
    {
        return FALSE;
    }

    WzVector vDistance = TargetPosition - StartPosition;
    float fDistance = VectorLength(&vDistance);

    WzRay ray;
    ray.m_wvDirection = TargetPosition - StartPosition;

    ray.m_wvOrigin = StartPosition;
    ray.m_wvOrigin.z += 0.2f;

    VectorNormalize(&ray.m_wvDirection,&ray.m_wvDirection);

    // 지형
    float fT;
    int Index = g_pSunTerrain->PickTerrainMesh( &ray, &fT, FALSE, TMGO_NO_COLLISION );
    if( Index >= 0 && fT <= fDistance )
    {
        return TRUE;
    }


    int PathTileIndex = g_pSunTerrain->x_pPathFinder->PickPathTile(&ray, &fT, TRUE );
    if( PathTileIndex >= 0 && fT < fDistance )
    {
        // 속성체크
        WORD TileAttrib = g_pSunTerrain->x_pPathFinder->GetAttribute(PathTileIndex);
        if( TileAttrib & PTA_NO_WALK )
        {
            return TRUE;
        }

        if( TileAttrib & PTA_PLAYER_FALLDOWN_TILE )
        {
            return TRUE;
        }
    }


    ray.m_wvOrigin = StartPosition;
    ray.m_wvOrigin.z += 0.8f;

    PathTileIndex = g_pSunTerrain->x_pPathFinder->PickPathTile(&ray, &fT, TRUE );
    if( PathTileIndex >= 0 && fT < fDistance )
    {
        // 속성체크
        WORD TileAttrib = g_pSunTerrain->x_pPathFinder->GetAttribute(PathTileIndex);
        if( TileAttrib & PTA_NO_WALK )
        {
            return TRUE;
        }

        if( TileAttrib & PTA_PLAYER_FALLDOWN_TILE )
        {
            return TRUE;
        }
    }

    return FALSE;
}

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
            pPlayer->m_JumpData.m_bJumpStart=TRUE;
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
	pPlayer->SetFreeMove(TRUE);

	if( pPlayer->IsTransForm() )//변신은 준비 동작 없음 
		pPlayer->m_JumpData.m_bJumpStart = TRUE;
	else
		pPlayer->m_JumpData.m_bJumpStart = FALSE;

	pPlayer->m_JumpData.m_fJumpEndFrame = 0.0f;


    pPlayer->m_JumpData.m_fJumpUpperForce = s_fJumpUpperForce;
    pPlayer->m_JumpData.m_fJumpMoveForce = s_fJumpUpperForce;
    pPlayer->m_JumpData.m_fJumpDistanceToLand = 1.0f;
	pPlayer->m_JumpData.m_bCreateJumpEndEffect = FALSE;
	pPlayer->m_JumpData.m_JumpTime = 0;

	WzVector vDummy;
	vDummy.x = 0.0f;
	vDummy.y = 0.0f;
	vDummy.z = 0.0f;
	pPlayer->SetOldKeyboardDirection( &vDummy );
	pPlayer->SetHeight( 0.0f );
	
	pPlayer->m_fTurnAccelerate = 0.0f;
	pPlayer->StopBlendAnimation();
    
	//m_OffsetHeight = 3.0f;
    pPlayer->set_is_riding_jump(false);
    if (pPlayer->IsRiding())
    {
        pPlayer->set_is_riding_jump(true);
        pPlayer->SetKeyBoardJumpFlag(TRUE);
    }

    // 애니메이션을 점프 시작으로 설정
	pPlayer->SetPlayerAnimation( pPlayer->GetCurrentJumpStartAnimation(), eANI_JUMP_START, FALSE);

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
            if( g_pMap )
            {
                // 이동상태에로 방향을 구함
                WzVector vDirection;
                if ( GetJumpDirection(vDirection, g_pHero->m_JumpData.m_JumpFlag.GetMoveFlag(), g_pHero->m_JumpData.m_CharacterDirection ) == TRUE )
                {
                    // 점프의 목적지, 시작위치, 방향을 설정 (목적지는 수평점프 거리에 피킹한 위치 )
                    WzVector vTargetPosition;
                    SimulateJump( g_pHero->GetVisiblePos(), vDirection, vTargetPosition, g_pHero->m_JumpData.m_JumpPositionArray );



                    // 설정
                    g_pHero->m_JumpData.m_vJumpStartPosition = g_pHero->GetVisiblePos();
                    g_pHero->m_JumpData.m_vJumpTargetPosition = vTargetPosition;
                    g_pHero->m_JumpData.m_vJumpDirection = vDirection;
                }
                else
                {
                    // 제자리 점프
                    g_pHero->m_JumpData.m_vJumpStartPosition = g_pHero->GetVisiblePos();
                    g_pHero->m_JumpData.m_vJumpTargetPosition = g_pHero->GetVisiblePos();
                    g_pHero->m_JumpData.m_vJumpDirection = vDirection;
                    g_pHero->m_JumpData.m_fJumpMoveForce = 0.0f;
                }

#ifdef _DEBUG
                // TargetPosition을 확인하기 위해
                g_pEffectManager->CreateEffect(StrToWzID("A023"), g_pHero->m_JumpData.m_vJumpTargetPosition );
#endif

            }
        }
        SendJumpPacket();
    }
    else
    {
        GetJumpDirection( pPlayer->m_JumpData.m_vJumpDirection, pPlayer->m_JumpData.m_JumpFlag.GetMoveFlag(), pPlayer->GetDirection() );
    }

	//시간 계산 
	WzVector vDistance = pPlayer->m_JumpData.m_vJumpTargetPosition - pPlayer->m_JumpData.m_vJumpStartPosition;
	float fDistance = VectorLength(&vDistance);

	
    //타락한 사원에서 속성 검사 해서 속성이 검출 될 경우 z값을 -10정도 더 내려 준다
	int iTileFocussing = g_pSunTerrain->x_pPathFinder->GetTileToStand(pPlayer->m_JumpData.m_vJumpTargetPosition, NULL, -1.0f, 100.0f);
	if (iTileFocussing >= 0)
	{
		if (g_pSunTerrain->x_pPathFinder->GetAttribute( iTileFocussing ) & PTA_PLAYER_FALLDOWN_TILE)
		{
			pPlayer->m_JumpData.m_vJumpTargetPosition.z -= 50.0f;
		}
	}


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

    // 높이가 높으면 스턴? -_-
	float fH = pPlayer->m_JumpData.m_vJumpStartPosition.z - pPlayer->m_JumpData.m_vJumpTargetPosition.z;
	if( fH > STUN_HEIGHT )
	{
		pPlayer->m_JumpData.m_bJumpStun = TRUE;

        // 높이가 높을 수록 경직시간은 길게한다
		float fTime = 50;
		fTime*=(fH-STUN_HEIGHT);
		pPlayer->m_JumpData.m_JumpStunTime = (int)fTime;
	}
	else
	{
		pPlayer->m_JumpData.m_bJumpStun = FALSE;
		pPlayer->m_JumpData.m_JumpStunTime = 0;
	}

	return STATECHANGE::SUCCESS;
}

BOOL StateKeyboardJump::TestMoveableTerrainPosition( const WzVector& vStartPosition, WzVector& vTargetPosition )
{
    // 현재 위치가 이동가능한 타일인가

    static float s_fTemp = 0.01f;

    WzRay ray;
    ray.m_wvOrigin = vTargetPosition;
    ray.m_wvOrigin.z += s_fTemp;
    ray.m_wvDirection.x = 0.0f;
    ray.m_wvDirection.y = 0.0f;
    ray.m_wvDirection.z = -1.0f;

    float fT;

    int iCurrentTerrainMesh = -1;
    iCurrentTerrainMesh = g_pSunTerrain->PickTerrainMesh( &ray, &fT, TRUE, TMGO_NO_COLLISION );
    if( iCurrentTerrainMesh >= 0 )
    {
        vTargetPosition = ray.m_wvOrigin + ray.m_wvDirection * fT;
    }
    else if( iCurrentTerrainMesh < 0 && SCENE_TYPE_DOMINATION == GameFramework::GetCurScene() )
    {
        int iTile = g_pSunTerrain->PickPathTile(&ray, &fT);
        if( iTile >= 0 )
        {
            vTargetPosition = ray.m_wvOrigin + ray.m_wvDirection * fT;
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        return FALSE;
    }

    // 길찾기 타일 얻기
    int iSrcTileFocussing = g_pSunTerrain->x_pPathFinder->GetTileToStand(vStartPosition, NULL, -1.f, 100.0f );
    int iTileFocussing = g_pSunTerrain->x_pPathFinder->GetTileToStand(vTargetPosition, NULL, -1.0f, 100.0f);
    if (iTileFocussing < 0 || iSrcTileFocussing < 0)
    {
        return FALSE;
    }

    // 속성체크
    WORD TargetTileAttrib = g_pSunTerrain->x_pPathFinder->GetAttribute(iTileFocussing);
    WORD SourceTileAttrib = g_pSunTerrain->x_pPathFinder->GetAttribute(iSrcTileFocussing);

#if WZENGINEVER >= 270
    if ((SourceTileAttrib & PTA_JUMP_CONTROL_A) &&
        (TargetTileAttrib & PTA_JUMP_CONTROL_B))
    {
        return FALSE;
    }
    if ((SourceTileAttrib & PTA_JUMP_CONTROL_B) &&
        (TargetTileAttrib & PTA_JUMP_CONTROL_A))
    {
        return FALSE;
    }
#endif //WZENGINEVER >= 270

    if ((SourceTileAttrib & PTA_PLAYER_JUMP_TILE) &&
        (TargetTileAttrib & PTA_PLAYER_JUMP_TILE))
    {
        return TRUE;
    }

    if( TargetTileAttrib & PTA_NO_WALK )
    {
        return FALSE;
    }

    if( TargetTileAttrib & PTA_PLAYER_FALLDOWN_TILE )
    {
        return FALSE;
    }

    if( SceneBase* current_scene = GameFramework::GetCurrentScene())
    {
        int maxNode = 0;

        switch(current_scene->GetType()) 
        {
        case SCENE_TYPE_MISSION:    maxNode = 200; break;
        case SCENE_TYPE_DOMINATION:    maxNode = 20; break;
        case SCENE_TYPE_CHUNTING:    maxNode = 1000; break;
        }

        if( maxNode )
            return GameFunc::IsMove(vStartPosition, vTargetPosition,TRUE,maxNode);
        else 
            return TRUE;
    }

    return FALSE;
}

BOOL StateKeyboardJump::IsInCource( WzVector P1, WzVector P2, WzVector T )
{
    P1.z = P2.z = T.z;

    P2 = P2 - P1;
    T = T - P1;

    return VectorLength(&T) <= VectorLength( &P2 );
}

BOOL StateKeyboardJump::CalcJumpMoving( Player* pPlayer, const WzVector& CurrentPosition, DWORD dwTick )
{
    // 1. 지형과의 거리를 구한다
    float fDistanceToLand = pPlayer->m_JumpData.m_fJumpDistanceToLand;
    float fLandHeight = CurrentPosition.z - fDistanceToLand;

    if( pPlayer->m_JumpData.m_fJumpUpperForce < 0.0f && fDistanceToLand <= 0.0f )
    {
        return FALSE;
    }

    // 2. 점프중에는 별다른 충돌검사를 하지 않고, TargetPosition까지 그냥 날아간다
    float fMoveTick = (float)dwTick * 0.001f;
    WzVector ResultPosition = CurrentPosition;


    pPlayer->m_JumpData.m_fJumpTick += fMoveTick;
    if( MovePositionOnSimulatedPosition( pPlayer, fMoveTick, ResultPosition, pPlayer->m_JumpData.m_vJumpDirection, pPlayer->m_JumpData.m_fJumpMoveForce, pPlayer->m_JumpData.m_fJumpUpperForce ) == false )
        MovePositionOnJump( pPlayer->m_JumpData.m_vJumpDirection, fMoveTick, ResultPosition, pPlayer->m_JumpData.m_fJumpMoveForce, pPlayer->m_JumpData.m_fJumpUpperForce );


    WzVector vTargetPosition = ResultPosition;

    BOOL bCanMove = TRUE;

    // 4. 이동이 좌표인가
    if( TestObjectCollision( CurrentPosition, vTargetPosition ) == TRUE )
    {
        bCanMove = FALSE;
    }


    WzVector vTemp = vTargetPosition;
    if( pPlayer->m_JumpData.m_fJumpUpperForce < 0.0f )
    {
        vTemp.z = CurrentPosition.z;
    }

    if( TestMoveableTerrainPosition( pPlayer->m_JumpData.m_vJumpStartPosition, vTemp ) == TRUE )
    {
        vTargetPosition = vTemp;
    }
    else
    {
        bCanMove = FALSE;
    }


    if( bCanMove == TRUE )
    {
        // 착지가능한 위치라면, 지형과의 거리와 지형의 높이 갱신
        fDistanceToLand = ResultPosition.z - vTargetPosition.z;
        fLandHeight = vTargetPosition.z;

    }

    if( bCanMove == FALSE )
    {
        // 목표위치에 이미 도달한 경우에는 이동불가능 지역이 있으면 수평이동은 멈춘다
        if( IsInCource( pPlayer->m_JumpData.m_vJumpStartPosition, pPlayer->m_JumpData.m_vJumpTargetPosition, vTargetPosition ) == FALSE )
        {
            // 이전 위치를 사용하고, 더이상 수평이동은 하지않음
            ResultPosition.x = CurrentPosition.x;
            ResultPosition.y = CurrentPosition.y;
            pPlayer->m_JumpData.m_fJumpMoveForce = 0.0f;
        }

        // 공중에 떠있다면 이전 마지막 수치를 그대로 사용
        fDistanceToLand = ResultPosition.z - fLandHeight;
    }


    // 땅속으로 파고들지 않도록
    if( fDistanceToLand < 0.0f )
    {
        pPlayer->m_JumpData.m_fJumpDistanceToLand = 0.0f;
        ResultPosition.z = fLandHeight;
    }
    else
    {
        //ResultPosition.z = vTargetPosition.z;
        pPlayer->m_JumpData.m_fJumpDistanceToLand = fDistanceToLand;
    }

    // 
    if( pPlayer->m_JumpData.m_fJumpUpperForce < 0.0f && pPlayer->m_JumpData.m_fJumpDistanceToLand < s_fMoveableMinDistance )
    {
        ResultPosition.x = CurrentPosition.x;
        ResultPosition.y = CurrentPosition.y;
        pPlayer->m_JumpData.m_fJumpMoveForce = 0.0f;
    }



    // 5. 오브젝트에 적용
    pPlayer->SetVisiblePos( ResultPosition );
    pPlayer->GetBasePart()->SetPosition( ResultPosition );
    pPlayer->SetFreeMove(TRUE);

    pPlayer->GetBasePart()->SetPosition(pPlayer->GetVisiblePos());
    pPlayer->UpdateOctree(TRUE);


    return TRUE;
}

STATEPROCESS::TYPE StateKeyboardJump::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Player* pPlayer = (Player*)pOwner;

	// 회전 관련 액셀 값을 세팅 한다.
	if(pPlayer == g_pHero)
	{
		CalculateTurnAccelerate(dwTick);
	}

	int iMoveFlag = pPlayer->m_JumpData.m_JumpFlag.GetMoveFlag();

	pPlayer->m_JumpData.m_JumpTime += dwTick;

	if(pPlayer->m_JumpData.m_bJumpStart == FALSE)
    {
		pPlayer->m_JumpData.m_JumpTime = 1;
    }

	if(pPlayer->m_JumpData.m_hStunEffect != INVALID_HANDLE_VALUE)
	{
		if( CEffectBody *pBody= g_pEffectManager->GetEffectPtrHandle(pPlayer->m_JumpData.m_hStunEffect) )
		{
			WzVector vPos;
			vPos = pPlayer->GetHeadBonePos();
			vPos.z+=0.1f;

			WzVector vRot;
			vRot.x = 0.0f;
			vRot.y = 0.0f;
			vRot.z = 0.0f;
			WzVector vScale;
			vScale.x=1.0f;
			vScale.y=1.0f;
			vScale.z=1.0f;
			pBody->SetResultMatrix(vPos,vRot,vScale);
		}
	}

    WzVector CurrentPosition = pPlayer->GetVisiblePos();

    // 이동
    CalcJumpMoving( pPlayer, CurrentPosition, dwTick );

    float fDistanceToLand = pPlayer->m_JumpData.m_fJumpDistanceToLand;


    if(pPlayer->m_JumpData.m_bJumpStun )
    {
        if( fDistanceToLand <= s_fJumpEffectHeight )
        {
            if( pPlayer->m_JumpData.m_fJumpUpperForce < 0.0f && pPlayer->m_JumpData.m_bCreateJumpEndEffect == FALSE )
            {
                pPlayer->m_JumpData.m_bCreateJumpEndEffect  = TRUE;
                OnFootStep(pPlayer);

                SendStopPacket();

            }
            pPlayer->m_JumpData.m_fJumpEndFrame = 0;
        }
    }
    else
    {
        if( pPlayer->m_JumpData.m_fJumpUpperForce < 0.0f && fDistanceToLand <= s_fJumpEffectHeight )
        {
            if(!pPlayer->m_JumpData.m_bCreateJumpEndEffect)
            {
                pPlayer->m_JumpData.m_bCreateJumpEndEffect = TRUE;
                OnFootStep(pPlayer);

                SendStopPacket();
            }				
            pPlayer->m_JumpData.m_fJumpEndFrame=0;
        }
    }


    //점프중 라이딩상태 변경(소환.해제)이 일어났을경우 OnProcess를 끝낸다
    if(CheckRidingState(pPlayer))
    {
        return STATEPROCESS::END;
    }

    if(pPlayer->IsPolymorph() )
    {
        return STATEPROCESS::END;
    }


    // 애니메이션 이펙트 처리

    char szAniCode[5];
    ZeroMemory(szAniCode,sizeof(szAniCode));
    WzIDToString(szAniCode,pPlayer->GetCurrentAnimation());
    szAniCode[1]='0';
    WzID wzCurrentAnimationID=StrToWzID(szAniCode);

    ZeroMemory(szAniCode,sizeof(szAniCode));
    WzIDToString(szAniCode,pPlayer->GetCurrentJumpStartAnimation());
    szAniCode[1]='0';
    WzID wzJumpStartAnimationID=StrToWzID(szAniCode);

    ZeroMemory(szAniCode,sizeof(szAniCode));
    WzIDToString(szAniCode,pPlayer->GetCurrentJumpKeepAnimation());
    szAniCode[1]='0';
    WzID wzJumpKeepAnimationID=StrToWzID(szAniCode);

    ZeroMemory(szAniCode,sizeof(szAniCode));
    WzIDToString(szAniCode,pPlayer->GetCurrentJumpEndAnimation());
    szAniCode[1]='0';
    WzID wzJumpEndAnimationID=StrToWzID(szAniCode);


    ZeroMemory(szAniCode,sizeof(szAniCode));
    WzIDToString(szAniCode,pPlayer->GetCurrentJumpLandAnimation());
    szAniCode[1]='0';
    WzID wzJumpLandAnimationID=StrToWzID(szAniCode);


    ZeroMemory(szAniCode,sizeof(szAniCode));
    WzIDToString(szAniCode,pPlayer->GetCurrentJumpStunEndAnimation());
    szAniCode[1]='0';
    WzID wzJumpStunAnimationEndID=StrToWzID(szAniCode);


    ZeroMemory(szAniCode,sizeof(szAniCode));
    WzIDToString(szAniCode,pPlayer->GetCurrentJumpStunAnimation());
    szAniCode[1]='0';
    WzID wzJumpStunAnimationID=StrToWzID(szAniCode);

    ZeroMemory(szAniCode,sizeof(szAniCode));
    WzIDToString(szAniCode,pPlayer->GetCurrentJumpStunKeepAnimation());
    szAniCode[1]='0';
    WzID wzJumpStunKeepAnimationID=StrToWzID(szAniCode);



    WzEvent event;
    float fRet = 0.0f;
	if(wzCurrentAnimationID == wzJumpStartAnimationID)//점프시작
	{
		pPlayer->ProcessInterpolation(dwTick);

		fRet=pPlayer->ProcessAnimation(dwTick,&event);

		// 이벤트가 잡힐때까지 못움직인다.
		if( pPlayer->m_JumpData.m_bJumpStart == FALSE )
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
								pPlayer->m_JumpData.m_bJumpStart = TRUE;
							}
						}
						break;
					}
				}
			}
		}

        // 적절한 타이밍이 지난 후 애니메이션 변경
		if (fRet >= 100.0f)
		{
            pPlayer->SetPlayerAnimation(pPlayer->GetCurrentJumpKeepAnimation(),eANI_JUMP_LOOP,TRUE);
		    pPlayer->ProcessAnimation(0);
		}
	}
	else if(wzCurrentAnimationID==wzJumpKeepAnimationID)
	{
		//pPlayer->SetAnimation(pPlayer->GetCurrentJumpKeepAnimation(),TRUE);
        pPlayer->SetPlayerAnimation(pPlayer->GetCurrentJumpKeepAnimation(),eANI_JUMP_LOOP,TRUE);
		pPlayer->ProcessInterpolation(dwTick);
		pPlayer->ProcessAnimation(dwTick,&event);

        

        // 낙하중 모션블러 적용
        if ( pPlayer->m_JumpData.m_bJumpStun == TRUE && pPlayer->m_JumpData.m_fJumpUpperForce <= s_fMaxUpper )
        {
            SetMotionBlur(pPlayer, true);
        }
    
        static float s_Distance = 2.0f;
        // 떨어지고 있는중이고, 거리가 2 이하인 경우
        if( fDistanceToLand <= s_Distance && pPlayer->m_JumpData.m_fJumpUpperForce < 0.0f )
        {
            //SetMotionBlur(pPlayer, false);

			if(pPlayer->m_JumpData.m_bJumpStun == TRUE && pPlayer->IsDead() == FALSE )
			{
				pPlayer->SetPlayerAnimation(pPlayer->GetCurrentJumpStunAnimation(),eANI_HJUMP_END,FALSE);
			}
			else
			{
				pPlayer->SetPlayerAnimation(pPlayer->GetCurrentJumpEndAnimation(),eANI_JUMP_END,FALSE);
			}
		}
	}
	else if(wzCurrentAnimationID==wzJumpEndAnimationID)
	{
		if(pPlayer->GetVisiblePos().z - pPlayer->m_JumpData.m_fJumpTargetHeight<=0.8f)
		{
			pPlayer->m_JumpData.m_fJumpEndFrame = 0;
		}


		if (pPlayer->m_JumpData.m_fJumpEndFrame )
		{
			pPlayer->SetCurrentFrame(pPlayer->m_JumpData.m_fJumpEndFrame );
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
								pPlayer->m_JumpData.m_fJumpEndFrame = pPlayer->GetCurrentFrame();
							}
						}
						break;
					}
				}
			}
		}

		if( fDistanceToLand <= 0.0f && 
            iMoveFlag &&
			(   pPlayer->CheckMoveFlag(KEYBOARDMOVE_FOWARD)||
			    pPlayer->CheckMoveFlag(KEYBOARDMOVE_BACKWARD)||
			    pPlayer->CheckMoveFlag(KEYBOARDMOVE_RIGHTSTEP)||
			    pPlayer->CheckMoveFlag(KEYBOARDMOVE_LEFTSTEP)||
			    pPlayer->IsDead()
            )
            )
		{
    			
			return STATEPROCESS::END;
		}

        float s_tRet = 100.0f;

		//if (pPlayer->GetVisiblePos().z == pPlayer->m_fJumpTargetHeight )
        if ( fDistanceToLand <= 0.0f && fRet == s_tRet )
		{
			return STATEPROCESS::END;
		}
	}
	else if(wzCurrentAnimationID==wzJumpStunAnimationID)
	{
		pPlayer->ProcessInterpolation(dwTick);

		fRet = pPlayer->ProcessAnimation(dwTick,&event);

		if( fRet == 100.0f )
		{
			pPlayer->SetPlayerAnimation(pPlayer->GetCurrentJumpStunKeepAnimation(),eANI_HJUMP_END_LOOP,FALSE);
			pPlayer->ProcessAnimation(0);
		}

	}
	else if(wzCurrentAnimationID==wzJumpStunKeepAnimationID)
	{
		pPlayer->m_JumpData.m_JumpStunTime -= dwTick;
		pPlayer->m_JumpData.m_JumpStunTime = max(0,pPlayer->m_JumpData.m_JumpStunTime);
		pPlayer->m_JumpData.m_JumpStunTime = min(1000,pPlayer->m_JumpData.m_JumpStunTime);

		pPlayer->ProcessInterpolation(dwTick);

		fRet = pPlayer->ProcessAnimation(dwTick,&event);

		if( fDistanceToLand <= 0.0f && fRet == 100.0f )
		{
			if(pPlayer->m_JumpData.m_hStunEffect != INVALID_HANDLE_VALUE )
				g_pEffectManager->RemoveEffectPtr(pPlayer->m_JumpData.m_hStunEffect);

			//pPlayer->SetAnimation(pPlayer->GetCurrentJumpStunEndAnimation(),FALSE);		
            pPlayer->SetPlayerAnimation(pPlayer->GetCurrentJumpStunEndAnimation(), eANI_HJUMP_GETUP, FALSE);		
		}
	}	
	else if(wzCurrentAnimationID==wzJumpStunAnimationEndID)//Todo;IDLE 상태에서 처리 해야 한다.
	{
		pPlayer->ProcessInterpolation(dwTick);

		fRet = pPlayer->ProcessAnimation(dwTick,&event);

		if( fRet == 100.0f )
        {
			return STATEPROCESS::END;
        }
	}
	else
	{
		pPlayer->ProcessInterpolation(dwTick);

		fRet=pPlayer->ProcessAnimation(dwTick,&event);
	}

	
	return STATEPROCESS::HOLD;
}

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

	pPlayer->m_JumpData.m_bJumpStun = FALSE;
	pPlayer->m_JumpData.m_JumpFlag.Reset();
    pPlayer->SetKeyBoardJumpFlag(FALSE);
	
	//타락한 사원에서 떨어져 죽을땐 하면 안됨
	int iTileFocussing = g_pSunTerrain->x_pPathFinder->GetTileToStand(pPlayer->m_JumpData.m_vJumpTargetPosition, NULL, -1.0f, 100.0f);
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
		g_pEffectManager->RemoveEffectPtr( pPlayer->m_JumpData.m_hStunEffect );
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

	Player *pChr=(Player *)pOwner;
	assert(pChr->IsKindOfObject(PLAYER_OBJECT));

	if(g_pHero==pChr)
	{
		

		pChr->Player::OnFootStep(TRUE);
	}

	if( pChr->m_JumpData.m_bJumpStun )
	{
		WzVector vRet;
		vRet = pPlayer->GetHeadBonePos();
		vRet.z+=0.1f;

	
	}

	HANDLE handle;

	// 지형속성에 따라 이펙트를 뿌려준다
	switch(pChr->GetTerrainAttribute())
	{
	default:
	case eTERRAIN_ATTRIBUTE_DIRT:
		{
			handle=GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A000"),vPos,pChr->GetObjectKey());
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

			handle=GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A000"),vPos2,pChr->GetObjectKey());
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

			handle=GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A000"),vPos3,pChr->GetObjectKey());
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
			handle=GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A042"),vPos,pChr->GetObjectKey());
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


			handle=GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A042"),vPos2,pChr->GetObjectKey());
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


			handle=GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A042"),vPos3,pChr->GetObjectKey());
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
                                                          pChr->GetWaterTerrainPos(), 
                                                          pChr->GetObjectKey());
		}
		break;


	case eTERRAIN_ATTRIBUTE_SNOW:
		{
			handle=GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A003"),vPos,pChr->GetObjectKey());
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

			handle=GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A003"),vPos2,pChr->GetObjectKey());
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


			handle=GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A003"),vPos3,pChr->GetObjectKey());
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
			handle=GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A001"),vPos,pChr->GetObjectKey());
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


			handle=GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A001"),vPos2,pChr->GetObjectKey());
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


			handle=GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A001"),vPos3,pChr->GetObjectKey());

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