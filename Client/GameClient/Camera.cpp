//------------------------------------------------------------------------------
//  Camera.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "Camera.h"
#include "Hero.h"
#include "Gamefunc.h"
#include "GlobalFunc.h"
#include "InterfaceManager.h"
#include "Map.h"

#define	CAMERA_PITCH_RANGE_MIN			-0.50f
#define	CAMERA_PITCH_RANGE_MAX			1.50f
#define	CAMERA_ROT_STEP_MAX				0.012f

#define	CAMERA_ROT_SLOWDOWN_LEVEL		0.25f
#define	CAMERA_ROT_SLOWDOWN				0.30f
#define	CAMERA_ROT_MIN					( CAMERA_YEW_RANGE_MAX * 0.01f )

#define	CAMERA_POS_SPRING_MINIMUM		0.4f

#define	CAMERA_POS_BASIC_MINIMUM		0.6f


#define	CAMERA_POS_MAXIMUM				1.2f


#ifdef MAP_EDIT_MODE
	#define CAMERA_DISTANCE_RANGE_MAX       ((GENERALPARAM->GetMaxCameraDistance())*(15.0f))
#else
	#define CAMERA_DISTANCE_RANGE_MAX       (GENERALPARAM->GetMaxCameraDistance())
#endif 

#define INTERPOLATE_TIME 1000

Camera	g_Camera;

//------------------------------------------------------------------------------
/**
*/
Camera::Camera( void )
: m_CameraHeight(0.f)
{
	m_bStartMove = FALSE;
	m_fAngle = sqrt( WZ_PI );
	m_vibeType=VIBE_TYPE_ALL;
	m_eRatio = SR_4_3;

	m_bIsRotate = FALSE;
	m_bIsRotateX = FALSE;
	m_bIsRotateZ = FALSE;
	m_fPrevAngleYaw = 0.0f;
	m_fPrevAnglePitch = 0.0f;

	m_iPreYaw = 0;
	m_iPrePitch = 0;
	m_iAngleCheckX = 0;
	m_iAngleCheckZ = 0;
	m_bStartRotate = FALSE;
	m_bStartRotateX = FALSE;
	m_bStartRotateZ = FALSE;
	m_bForceSpringCamera=FALSE;

	m_iTurnState = CAMERA_NOT_TURN;

	m_fMoveSpeed = CAMERA_DEFAULT_MOVE_SPEED;

	m_fRotateLevelX = 0.0f;
	m_fRotateSlowDownX = 0.0f;
	m_fRotateLevelZ = 0.0f;
	m_fRotateSlowDownZ = 0.0f;
	m_fDistanceLevel = 0.0f;
	m_fDistanceSlowDown = 0.0f;
	m_bSpring=FALSE;
	m_fRotationSpeed =CAMERA_DEFAULT_ROTATE_SPEED;
	m_iRotationLevel = 3;
	SetRotationRange(1,5);
	m_bRidingTurn=FALSE;
    m_pPathExplorer = NULL;
}

//------------------------------------------------------------------------------
/**
*/
Camera::~Camera( void )
{
	Destroy();
}

//------------------------------------------------------------------------------
/**
*/
BOOL Camera::Create( float fMin, float fMax ,float fFOV)
{
	m_fAngleYawDirection = 1.0f;
	m_fTargetDirectionAngle = 0.0f;
	SetVector( &m_CameraTargetDirection, 0.0f, -1.0f, 0.0f );


	SetVector( &m_wvX, 1.0f, 0.0f, 0.0f );
	SetVector( &m_wvY, 0.0f, 1.0f, 0.0f );
	SetVector( &m_wvZ, 0.0f, 0.0f, 1.0f );

	m_fUp = 1.3f; // default 
	if (GENERALPARAM->GetScreenShotMode())
	{
		m_fDistance = CAMERA_DISTANCE;
	}
	else
	{
		m_fDistance = GENERALPARAM->GetCameraDistance();
	}

	m_fAngleYaw = 0.0f;
	m_fAngleYawStep = 0.0f;
	m_fAnglePitch = 0.5f;

	SetVector( &m_CameraPosition, 0.0f, 0.0f, 0.0f );
	SetVector( &m_CameraDirection, 0.0f, 1.0f, -1.0f );
	SetVector( &m_CameraFrom, 0.0f, 0.0f, 0.0f );
	SetVector( &m_CameraTo, 0.0f, 0.0f, 0.0f );



	if( g_pSunRenderer )
	{
		SetProjection( fMin, fMax ,fFOV);
	}

	//	길찾기 기능 생성
	if (m_pPathExplorer)
	{
		Func_ContactTerrain(CT_DELETE_PATH_EXPLORER,m_pPathExplorer);

		m_pPathExplorer = NULL;		
	}

	m_pPathExplorer = ( CPathExplorer* )Func_ContactTerrain( CT_NEW_PATH_EXPLORER, 0 );
	m_fTurnAccelerate = 0.0f;

	m_status=0;


	m_springConst=0.709996f;
	m_dampConst=6.8f;
	m_springLen=0.1f;

	m_springConst=GENERALPARAM->GetSpringConst();
	m_dampConst=GENERALPARAM->GetDampConst();
	m_springLen=GENERALPARAM-> GetSpringLen();

	SetVector(&m_targetOldPos,0.0f,0.0f,0.0f);
	SetVector(&m_chasePos,0.0f,0.0f,0.0f);
	
	m_fMoveSpeed = CAMERA_DEFAULT_MOVE_SPEED;
	
	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
void Camera::Destroy( void )
{
	//	길찾기 기능 파괴
	if( m_pPathExplorer )
	{
        Func_ContactTerrain( CT_DELETE_PATH_EXPLORER, m_pPathExplorer );
		m_pPathExplorer = NULL;
	}
}

//------------------------------------------------------------------------------
/**
*/
void Camera::SetProjection( float fMin, float fMax ,float fFOV)
{
	m_fFOV = fFOV;
	m_fNearClip = fMin;
	m_fFarClip  = fMax;

	RestoreProjection();

}

//------------------------------------------------------------------------------
/**
기본은 카메라의 위치를 설정한다.
바이브레이션이 있는경우 좌표를 수정해준다.
*/
void Camera::SetCameraPosition( WzVector* WzTarget )
{
	m_CameraPosition = *WzTarget;
	m_CameraPosition.z += CameraHeight();


	float fMoveOffset = m_vibePower;

	if( m_vibeType == VIBE_TYPE_VERTICAL )
	{	
		m_CameraPosition.z += fMoveOffset;
	}
	else if( m_vibeType==VIBE_TYPE_HORISONTAL )
	{	
		m_CameraPosition.x += fMoveOffset;
		m_CameraPosition.y += fMoveOffset;
	}	
	else
	{
		// VIBE_TYPE_NONE 일때에도 처리되겠지만 아마도 m_vibePower가 0일 것이다.. 

		int r = rand()%3;

		if ( r == 0 )
			m_CameraPosition.z += fMoveOffset;
		else if ( r == 1 )
			m_CameraPosition.x += fMoveOffset;
		else
			m_CameraPosition.y += fMoveOffset;
	}
}

//------------------------------------------------------------------------------
/**
spring damp function

@param  currPos     current position
@param  trgPos      target position
@param  prevTrgPos  previous target position
@param  deltaTime   change in time
@param  springConst hook's constant
@param  dampConst   damping constant
@param  springLen   spring length
*/
vector3
Camera::SpringDamp( vector3 currPos,    // Current Position
				   const vector3& trgPos,     // Target Position
				   const vector3& prevTrgPos, // Previous Target Position
				   float deltaTime,    // Change in Time
				   float springConst,  // Hooke's Constant
				   float dampConst,    // Damp Constant
				   float springLen)
{
	static vector3 disp;           /// displacement
	static vector3 velocity;       /// velocity   
	static float forceMag;         /// force magnitude

	deltaTime = min(deltaTime,1.0f); //틱이 너무 크면 안된다
	/**
	Calculate Spring Force
	*/
	disp = currPos - trgPos;
	velocity = (prevTrgPos - trgPos) * deltaTime;
	forceMag = springConst * (springLen - disp.len()) + 
		dampConst * ((disp % velocity) / disp.len());

	/**
	Apply Spring Force
	*/

	disp.norm();
	disp *= forceMag * deltaTime;
	currPos += disp;
	return currPos;
}

void Camera::SetBasicCamera( WzVector* wvPos, BOOL bHeightCheck,DWORD dwTick)
{
	// 카메라의 위치를 설정
	if( wvPos != NULL )
	{
		if ( 0.f == m_CameraPosition.x && 0.f == m_CameraPosition.y)
		{
			SetCameraPosition( wvPos );
		}
		else
		{
			float fOldPlayerZ = m_CameraPosition.z - CameraHeight();
			float fDistance = wvPos->z - fOldPlayerZ;
			if ( -0.01f >= fDistance || 0.01f <= fDistance)
			{
				wvPos->z = fOldPlayerZ + (fDistance/2);
				SetCameraPosition( wvPos );
			}
			else
			{
				SetCameraPosition( wvPos );
			}
		}
	}

	// 카메라의 데이터를 갱신
	WzMatrix	CameraMatrix, CameraMatrix1, CameraMatrix2;
	MatrixIdentity( &CameraMatrix1 );
	MatrixRotationZ( &CameraMatrix1, m_fAngleYaw );

	MatrixIdentity( &CameraMatrix2 );
	MatrixRotationX( &CameraMatrix2, m_fAnglePitch );

	MatrixMultiply( &CameraMatrix, &CameraMatrix2, &CameraMatrix1 );
	VectorTransformNormal( &m_CameraDirection, &m_wvY, &CameraMatrix );

	m_CameraTo = m_CameraPosition;		
	m_CameraFrom = m_CameraTo + ( m_CameraDirection * m_fDistance );


	WzVector wvNomal;
	SetVector(&wvNomal,0.0f,0.0f,0.0f);

	// 지형 메시에 가려지는 경우 보정
	if ( g_pSunTerrain )
	{
		float fT;
		WzRay ray;

		ray.m_wvOrigin = m_CameraPosition;
		ray.m_wvDirection = m_CameraFrom - m_CameraTo;

		VectorNormalize( &ray.m_wvDirection, &ray.m_wvDirection);
	
		int iTerrainMeshFocussing = g_pSunTerrain->PickTerrainMesh( &ray, &fT ,TRUE,TMGO_HAZE, &wvNomal);
	
		if( iTerrainMeshFocussing >= 0 )
		{			
			if (fT < m_fDistance)
			{
				m_CameraFrom = ray.m_wvOrigin + ray.m_wvDirection * (fT - 0.01f);

				if(m_CameraFrom!=m_CameraTo)
				{
					WzVector temp=m_CameraFrom-m_CameraTo;
					float fLen=VectorLength(&temp);

					if(fLen>=1.0f)
					{
					
						m_CameraFrom+=wvNomal*0.5f;

					}
				}
			}			
		}
	}

	// 천장에 의해 가려지는 경우 보정 
	if( bHeightCheck && g_pMap && wvNomal.z >= 0.0f )
	{
		//리셋이 실패하는 경우가 종종 있고 실패할경우 카메라가 튄다.-지완
        WzVector temp = m_CameraFrom;
		if(g_pSunTerrain->PathExp_Reset(m_pPathExplorer, &temp,-1))
		{
            m_CameraFrom = temp;
			WzVector wvCameraPos = m_pPathExplorer->GetVisiblePos();

			float fAddHeight=CAMERA_POS_SPRING_MINIMUM;

			if(wvCameraPos!=m_CameraTo)
			{
				WzVector tempv;
				tempv=wvCameraPos-m_CameraTo;
				float fdis=VectorLength(&tempv);
				fAddHeight=CAMERA_POS_MAXIMUM;

				float fRate=fdis/CAMERA_DISTANCE_RANGE_MAX;
				fAddHeight*=fRate;

				fAddHeight=min(fAddHeight,CAMERA_POS_MAXIMUM);
				fAddHeight=max(fAddHeight,CAMERA_POS_BASIC_MINIMUM);
			}	

			wvCameraPos.z+=fAddHeight;;

			if( wvCameraPos.z > m_CameraFrom.z )
			{
				m_CameraFrom = wvCameraPos;
			}
		}
	}
    
	// 바운딩 영역에 가려지는 경우 보정
    if( g_pMap && g_pMap->IsLoaded())
    {
		float fT;
		WzRay ray;

        //변경 ray재설정
        ray.m_wvOrigin = m_CameraPosition;
        ray.m_wvDirection = m_CameraFrom - m_CameraTo;
        VectorNormalize(&ray.m_wvDirection,&ray.m_wvDirection);

        if(g_pMap->GetCollisionBV(&ray,m_fDistance,&fT))
        {
            //노말방향의로 밀기는 안하겠음(계산이 많이들어감)
            m_CameraFrom = ray.m_wvOrigin + ray.m_wvDirection * (fT - 0.01f);
        }
    }

	m_targetOldPos = m_CameraFrom;
}

void Camera::InitSpringCamera(BOOL bSpring)
{
	m_bSpring=bSpring;


	WzMatrix	CameraMatrix, CameraMatrix1, CameraMatrix2;

	MatrixIdentity( &CameraMatrix1 );
	MatrixRotationZ( &CameraMatrix1, m_fAngleYaw );

	MatrixIdentity( &CameraMatrix2 );
	MatrixRotationX( &CameraMatrix2, m_fAnglePitch );

	MatrixMultiply( &CameraMatrix, &CameraMatrix2, &CameraMatrix1 );
	VectorTransformNormal( &m_CameraDirection, &m_wvY, &CameraMatrix );

	m_CameraFrom = m_CameraTo + ( m_CameraDirection * m_fDistance );
	m_targetOldPos = m_CameraFrom;

}

//------------------------------------------------------------------------------
/**
	스프링 카메라 기능 추가  2006.12.13 김지완 
*/
void Camera::SetSpringCamera( WzVector* wvPos, BOOL bHeightCheck ,DWORD dwTick)
{
	//Todo:움직일 때만 체크 


	if( wvPos != NULL )
	{
		SetCameraPosition( wvPos);
	}

	m_targetOldPos = m_chasePos;

	//	카메라 원본 

	WzMatrix	CameraMatrix, CameraMatrix1, CameraMatrix2;

	MatrixIdentity( &CameraMatrix1 );
	MatrixRotationZ( &CameraMatrix1, m_fAngleYaw );

	MatrixIdentity( &CameraMatrix2 );
	MatrixRotationX( &CameraMatrix2, m_fAnglePitch );

	MatrixMultiply( &CameraMatrix, &CameraMatrix2, &CameraMatrix1 );
	VectorTransformNormal( &m_CameraDirection, &m_wvY, &CameraMatrix );

	m_CameraTo=m_CameraPosition;	
	m_chasePos = m_CameraTo + ( m_CameraDirection * m_fDistance );

	if(m_CameraFrom==m_chasePos)
		return;

	WzVector vFirstFrom=m_chasePos;
	WzVector temp=m_CameraDirection;
	temp.z=0.0f;

	WzVector up;
	SetVector(&up,0.0f,0.0f,1.0f);
	WzVector wvRight;

	VectorCrossProduct(&wvRight,&temp,&up);
	VectorNormalize(&wvRight,&wvRight);

	float fSideDistance = 1.5f;

	wvRight *= fSideDistance;

	WzVector wvLeft = wvRight * -1.0f;
	WzVector wvPick1, wvPick2;
	SetVector(&wvPick1,0.0f,0.0f,0.0f);
	SetVector(&wvPick2,0.0f,0.0f,0.0f);


	WzRay ray;
	if (g_pSunTerrain)
	{
		//가운데 체크 

		float fT=10000.0f;	
		ray.m_wvOrigin = m_CameraTo;	
		ray.m_wvDirection = m_chasePos- m_CameraTo;		
		VectorNormalize(&ray.m_wvDirection,&ray.m_wvDirection);
		float fT1=10000.0f;
		WzVector wvNomal;
		SetVector(&wvNomal,0.0f,0.0f,0.0f);

		int iTerrainMeshFocussing = g_pSunTerrain->PickTerrainMesh( &ray, &fT ,TRUE,TMGO_HAZE,&wvNomal);

		if( iTerrainMeshFocussing >= 0 )
		{		
			DWORD dwTerrainAttribute=0;

			if (fT < m_fDistance)
			{	
				fT=fT - 0.01f;
				fT=max(fT,0.0f);
				m_chasePos= ray.m_wvOrigin + ray.m_wvDirection *fT;
				wvPick1=m_chasePos;

				if(m_chasePos!=m_CameraTo)
				{
					WzVector temp=m_chasePos-m_CameraTo;
					float fLen=VectorLength(&temp);

					if(fLen>=1.0f)
					{
						wvNomal.x*=2.0f;
						wvNomal.y*=2.0f;
						wvNomal.z*=0.5f;
						m_chasePos+=(wvNomal);			
					}
				}
			}
		}

        //바운딩관련 처리를 여기서한다.
        if(g_pMap && g_pMap->IsLoaded())
        {
            float fT;
            //변경 ray재설정
            ray.m_wvOrigin = m_CameraTo;	
            ray.m_wvDirection = m_chasePos- m_CameraTo;		
            VectorNormalize(&ray.m_wvDirection,&ray.m_wvDirection);
            if(g_pMap->GetCollisionBV(&ray,m_fDistance,&fT))
            {
                //노말방향의로 밀기는 안하겠음(계산이 많이들어감)
                fT=fT - 0.01f;
                fT=max(fT,0.0f);
                m_chasePos= ray.m_wvOrigin + ray.m_wvDirection *fT;
                wvPick1=m_chasePos;
            }
        }

		float fTempLen = m_fDistance-1.0f;
		fTempLen = max(fTempLen,1.0f);
		vFirstFrom = m_CameraTo + (m_CameraDirection *  fTempLen);
		WzVector wvTempTo = m_CameraTo;
		wvTempTo.z += 0.5f;

		ray.m_wvOrigin = vFirstFrom;	
		ray.m_wvDirection = wvTempTo- vFirstFrom;		
		VectorNormalize(&ray.m_wvDirection,&ray.m_wvDirection);

		iTerrainMeshFocussing = g_pSunTerrain->PickTerrainMesh( &ray, &fT ,TRUE,0);

		if( iTerrainMeshFocussing >= 0 )
		{		
			if (fT <= m_fDistance)
			{	
				wvPick2= ray.m_wvOrigin + ray.m_wvDirection *fT;
			}
		}

		if( bHeightCheck && m_pPathExplorer)
		{
			float fAddHeight=CAMERA_POS_SPRING_MINIMUM;

			if(m_chasePos!=m_CameraTo)
			{
				WzVector tempv;
				tempv=m_chasePos-m_CameraTo;
				float fdis=VectorLength(&tempv);
				fAddHeight=CAMERA_POS_MAXIMUM;

				float fRate=fdis/CAMERA_DISTANCE_RANGE_MAX;
				fAddHeight*=fRate;

				fAddHeight=min(fAddHeight,CAMERA_POS_MAXIMUM);
				fAddHeight=max(fAddHeight,CAMERA_POS_SPRING_MINIMUM);
			}	

			g_pSunTerrain->PathExp_Reset( m_pPathExplorer, &m_chasePos, -1);

			WzVector wvCameraPos = m_pPathExplorer->GetPos();

			wvCameraPos.z +=fAddHeight;

			if( wvCameraPos.z > m_chasePos.z )
			{
				m_chasePos = wvCameraPos;
			}
		}
	}

	// 워프면

	if(!m_bSpring)
	{
		WarpFrom();
	}
	else if(m_CameraFrom!=m_chasePos)	
	{
		vector3 from,chasePos,targetOldPos;

		from.set(m_CameraFrom.x,m_CameraFrom.z,m_CameraFrom.y);
		chasePos.set(m_chasePos.x,m_chasePos.z,m_chasePos.y);
		targetOldPos.set(m_targetOldPos.x,m_targetOldPos.z,m_targetOldPos.y);


		float fTick=(float)dwTick/1000.0f;
		from = SpringDamp(from, chasePos, targetOldPos, 5.0f * fTick, m_springConst, m_dampConst, m_springLen);

		SetVector(&m_CameraFrom,from.x,from.z,from.y);
	}
	
}

void Camera::WarpFrom()
{
	m_bSpring=TRUE;
	m_targetOldPos=m_CameraFrom=m_chasePos;
}

//------------------------------------------------------------------------------
/**
카메라 충돌 루틴 수정 2006.12.6 김지완 
*/
void Camera::SetCamera( WzVector* wvPos, BOOL bHeightCheck,DWORD dwTick)
{
	const OptionParam& tempOption = GAMEOPTION->GetGameOption();

    if(tempOption.GetOptionValue( nOptionType::USE_SPRING_CAMERA ) || m_bForceSpringCamera )
		SetSpringCamera(wvPos,bHeightCheck,dwTick);
	else
		SetBasicCamera(wvPos,bHeightCheck,dwTick);

}

//------------------------------------------------------------------------------
/**
*/
void Camera::SetTargetDirection( WzVector *wvDir )
{
	m_CameraDirection.x = wvDir->x;
	m_CameraDirection.y = wvDir->y;
	m_CameraDirection.z = 0.0f;
}

//------------------------------------------------------------------------------
/**
*/
void Camera::StartMove( void )
{
	m_bStartMove = TRUE;
}

//------------------------------------------------------------------------------
/**
*/
void Camera::InterpolateAngle( void )
{
	if( m_fTargetDirectionAngle != m_fAngleYaw )
	{
		float fCount = m_fAngleYaw - m_fTargetDirectionAngle;
		if( fCount < 0.0f )
		{
			fCount += CAMERA_YEW_RANGE_MAX;
		}
		if( fCount == 0.0f || fCount < 0.01f )
		{
			m_fAngleYaw = m_fTargetDirectionAngle;
			m_fAngleYawStep = 0.0f;
			return;
		}

		float	fDir;
		float	fPercent;

		//	점점 가속도 증가
		if( fCount < CAMERA_YEW_RANGE_CENTER )
		{
			//	감소하면서 도달
			fDir = -1.0f;
			fPercent = ( fCount / CAMERA_YEW_RANGE_CENTER );
		}
		else
		{
			//	증가하면서 도달
			fDir = 1.0f;
			fPercent = ( (CAMERA_YEW_RANGE_MAX - fCount) / CAMERA_YEW_RANGE_CENTER );
		}

		float fAngleStep = m_fAngle * fPercent;
		m_fAngleYawStep = (fAngleStep * fAngleStep);
		if( m_fAngleYawStep > CAMERA_ROT_STEP_MAX )
		{
			m_fAngleYawStep = CAMERA_ROT_STEP_MAX;
		}

		//	엥글 조절
		m_fAngleYaw += m_fAngleYawStep * fDir;
		if( m_fAngleYaw < 0.0f )
		{
			m_fAngleYaw += CAMERA_YEW_RANGE_MAX;
		}
		else if( m_fAngleYaw >= CAMERA_YEW_RANGE_MAX )
		{
			m_fAngleYaw -= CAMERA_YEW_RANGE_MAX;
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void Camera::KeyboardModeInterpolateAngle(float fDestViewAngle)
{

	if(g_pHero && g_pHero->GetMoveFlag() & (KEYBOARDMOVE_LEFTTURN | KEYBOARDMOVE_RIGHTTURN) )
	{

		const float fTurnSpeed = WZ_PI / 6.0f; // 6도

		ClampAngle(&fDestViewAngle);

		if (fDestViewAngle != m_fAngleYaw) 
		{

			// 각도를 더하는게 빠른지 빼는게 빠른지 판단한다.
			float fLeftDiff; // 이쪽이 더하기
			float fRightDiff; // 빼기

			if ( fDestViewAngle > m_fAngleYaw)
			{
				fLeftDiff = fDestViewAngle - m_fAngleYaw;
			}
			else
			{
				fLeftDiff = fDestViewAngle - m_fAngleYaw + WZ_PI * 2.0f;
			}

			// 반대쪽 회전각도와의 합은 360도
			fRightDiff = WZ_PI * 2.0f - fLeftDiff;

			if ( fLeftDiff <= fRightDiff )
			{
				if (fLeftDiff > fTurnSpeed)
				{
					m_fAngleYaw += fTurnSpeed;
					ClampAngle(&m_fAngleYaw);
				}
				else
				{
					m_fAngleYaw = fDestViewAngle;
				}
			}
			else
			{
				if (fRightDiff > fTurnSpeed)
				{
					m_fAngleYaw -= fTurnSpeed;
					ClampAngle(&m_fAngleYaw);
				}
				else
				{
					m_fAngleYaw = fDestViewAngle;
				}
			}
		}

	}

}

//------------------------------------------------------------------------------
/**
*/
BOOL Camera::Process( DWORD dwTick )
{
	// 진동 모드 
	if(BIT_CHECK(m_status,CAMERA_STATUS_VIBE))
	{
		if(m_twSwitch < (BOOL)dwTick)
		{
			m_twSwitch=(BOOL)INIT_SWITCH;

			static float fWave  = -1.0f;
			m_vibePower*=fWave;
		}
		else
		{
			m_twSwitch-=dwTick;
		}

		if(m_twVibe<=0.0f)
		{
			EndVibration();
		}
		else
		{
			m_twVibe-=dwTick;
		}

		UpdateCamera();
	}

	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
void Camera::StartVibration(float time,float fPower)
{
	BIT_ADD(m_status,CAMERA_STATUS_VIBE);
	m_twVibe=time;

	m_vibePower=fPower;   
	m_twSwitch=(BOOL)INIT_SWITCH;
}

//------------------------------------------------------------------------------
/**
*/
void Camera::EndVibration()
{
	BIT_REMOVE(m_status,CAMERA_STATUS_VIBE);
	m_twVibe=0;
	m_vibePower=0.0f;
	m_vibeType=VIBE_TYPE_NONE;

	UpdateCamera();
}


#ifdef	CAMERA_SMOOTH_DELETE
//------------------------------------------------------------------------------
/**
*/
void Camera::Rotate( int iAngleX, int iAngleZ, int iState )
{
	const int	c_iAngleCheck = 8;
	const int	c_iTotalAngleCheckX = 12;
	const int	c_iTotalAngleCheckZ = 12;
	float c_fRotate = GetRotationSpeed();

	m_iRotateState = iState;
	m_fAngleYawStep = 0.0f;


	if( iAngleX == 0 )
	{
		m_iTurnState = CAMERA_NOT_TURN;
	}
	else if( iAngleX > 0 )
	{
		m_iTurnState = CAMERA_RIGHT_TURN;
	}
	else if( iAngleX < 0 )
	{
		m_iTurnState = CAMERA_LEFT_TURN;
	}

	if( m_iPreYaw != iAngleX )
	{
		if( m_bStartRotateX == FALSE )
		{
			if( abs(iAngleX) > c_iAngleCheck )
			{
				m_iAngleCheckX = 0;
				m_bStartRotateX = TRUE;
			}
			else
			{
				m_iAngleCheckX += iAngleX;
				if( abs(m_iAngleCheckX) > c_iTotalAngleCheckX )
				{
					m_iAngleCheckX = 0;
					m_bStartRotateX = TRUE;
				}
			}
		}

		if( m_bStartRotateX )
		{
			m_iPreYaw = iAngleX;

			m_fAngleYaw -= (float)(iAngleX)  * c_fRotate;
			if( m_fAngleYaw < CAMERA_YEW_RANGE_MIN )
			{
				m_fAngleYaw += CAMERA_YEW_RANGE_MAX;
			}
			else if( m_fAngleYaw > CAMERA_YEW_RANGE_MAX )
			{
				m_fAngleYaw -= CAMERA_YEW_RANGE_MAX;
			}


			//	카메라 회전 체크
			if( m_fPrevAngleYaw != m_fAngleYaw )
			{
				m_fPrevAngleYaw = m_fAngleYaw;
				m_bIsRotateX = TRUE;
			}
		}
	}



	if( m_iPrePitch != iAngleZ )
	{
		if( m_bStartRotateZ == FALSE )
		{
			if( abs(iAngleZ) > c_iAngleCheck )
			{
				m_iAngleCheckZ = 0;
				m_bStartRotateZ = TRUE;
			}
			else
			{
				m_iAngleCheckZ += iAngleZ;
				if( abs(m_iAngleCheckZ) > c_iTotalAngleCheckZ )
				{
					m_iAngleCheckZ = 0;
					m_bStartRotateZ = TRUE;
				}
			}
		}

		if( m_bStartRotateZ )
		{
			m_iPrePitch = iAngleZ;

			m_fAnglePitch += (float)(iAngleZ)  * c_fRotate;
			if( m_fAnglePitch < CAMERA_PITCH_RANGE_MIN )
			{
				m_fAnglePitch = CAMERA_PITCH_RANGE_MIN;
			}
			else if( m_fAnglePitch > CAMERA_PITCH_RANGE_MAX )
			{
				m_fAnglePitch = CAMERA_PITCH_RANGE_MAX;
			}
		}

		//	카메라 회전 체크
		if( m_fPrevAnglePitch != m_fAnglePitch )
		{
			m_fPrevAnglePitch = m_fAnglePitch;
			m_bIsRotateZ = TRUE;
		}

	}


	//	최종 결과
	m_bIsRotate = m_bIsRotateX | m_bIsRotateZ;
	m_bStartRotate = m_bStartRotateX | m_bStartRotateZ;
}




#else

//------------------------------------------------------------------------------
/**
*/
void Camera::Rotate( int iAngleX, int iAngleZ, int iState )
{
	const int	c_iAngleCheckX = 16 * 2;
	const int	c_iAngleCheckZ = 8 * 2;
	const int	c_iTotalAngleCheckX = 18 * 2;
	const int	c_iTotalAngleCheckZ = 12 * 2;

	float c_fRotate = GetRotationSpeed();

	m_iRotateState = iState;
	m_fAngleYawStep = 0.0f;


	m_fRotateLevelX += (float)iAngleX * CAMERA_ROT_SLOWDOWN_LEVEL;
	m_fRotateSlowDownX = m_fRotateLevelX * CAMERA_ROT_SLOWDOWN;

	m_fRotateLevelZ += (float)iAngleZ * CAMERA_ROT_SLOWDOWN_LEVEL;
	m_fRotateSlowDownZ = m_fRotateLevelZ * CAMERA_ROT_SLOWDOWN;


	if( iAngleX == 0 )
	{
		m_iTurnState = CAMERA_NOT_TURN;
	}
	else if( iAngleX > 0 )
	{
		m_iTurnState = CAMERA_RIGHT_TURN;
	}
	else if( iAngleX < 0 )
	{
		m_iTurnState = CAMERA_LEFT_TURN;
	}




	if( m_iPreYaw != iAngleX )
	{
		if( m_bStartRotateX == FALSE )
		{
			if( abs(iAngleX) > c_iAngleCheckX )
			{
				m_iAngleCheckX = 0;
				m_bStartRotateX = TRUE;
			}
			else
			{
				m_iAngleCheckX += iAngleX;
				if( abs(m_iAngleCheckX) > c_iTotalAngleCheckX )
				{
					m_iAngleCheckX = 0;
					m_bStartRotateX = TRUE;
				}
			}
		}

		if( m_bStartRotateX )
		{
			if(m_iRotateState==Camera::CAMERA_ROTATE_LMOUSE && g_pHero->IsRiding())
			{
				m_bRidingTurn = TRUE;
			}

			m_iPreYaw = (int)m_fRotateLevelX;

			m_fAngleYaw -= (float)(m_fRotateLevelX)  * c_fRotate;
			if( m_fAngleYaw < CAMERA_YEW_RANGE_MIN )
			{
				m_fAngleYaw += CAMERA_YEW_RANGE_MAX;
			}
			else if( m_fAngleYaw > CAMERA_YEW_RANGE_MAX )
			{
				m_fAngleYaw -= CAMERA_YEW_RANGE_MAX;
			}


			//	카메라 회전 체크
			if( m_fPrevAngleYaw != m_fAngleYaw )
			{
				m_fPrevAngleYaw = m_fAngleYaw;
				m_bIsRotateX = TRUE;
			}
		}
	}



	if( m_iPrePitch != iAngleZ )
	{
		if( m_bStartRotateZ == FALSE )
		{
			if( abs(iAngleZ) > c_iAngleCheckZ )
			{
				m_iAngleCheckZ = 0;
				m_bStartRotateZ = TRUE;
			}
			else
			{
				m_iAngleCheckZ += iAngleZ;
				if( abs(m_iAngleCheckZ) > c_iTotalAngleCheckZ )
				{
					m_iAngleCheckZ = 0;
					m_bStartRotateZ = TRUE;
				}
			}
		}

		if( m_bStartRotateZ )
		{
			m_iPrePitch = (int)m_fRotateLevelZ;

			m_fAnglePitch += (float)(m_fRotateLevelZ)  * c_fRotate;
			if( m_fAnglePitch < CAMERA_PITCH_RANGE_MIN )
			{
				m_fAnglePitch = CAMERA_PITCH_RANGE_MIN;
			}
			else if( m_fAnglePitch > CAMERA_PITCH_RANGE_MAX )
			{
				m_fAnglePitch = CAMERA_PITCH_RANGE_MAX;
			}

			//	카메라 회전 체크
			if( m_fPrevAnglePitch != m_fAnglePitch )
			{
				m_fPrevAnglePitch = m_fAnglePitch;
				m_bIsRotateZ = TRUE;
			}
		}

	}


	//	최종 결과
	m_bIsRotate = m_bIsRotateX | m_bIsRotateZ;
	m_bStartRotate = m_bStartRotateX | m_bStartRotateZ;


	m_fRotateLevelX -= m_fRotateSlowDownX;
	if( (float)abs(m_fRotateLevelX) < CAMERA_ROT_MIN )
	{
		m_fRotateLevelX = 0.0f;
		m_fRotateSlowDownX = 0.0f;
	}
	else
	{
		m_fAngleYaw -= (float)(m_fRotateLevelX)  * c_fRotate;
		if( m_fAngleYaw < CAMERA_YEW_RANGE_MIN )
		{
			m_fAngleYaw += CAMERA_YEW_RANGE_MAX;
		}
		else if( m_fAngleYaw > CAMERA_YEW_RANGE_MAX )
		{
			m_fAngleYaw -= CAMERA_YEW_RANGE_MAX;
		}

		//각이 좀 심하게 벌어지면 자동회전시킨다.
		if(m_bRidingTurn)
		{
			AutoTurnState();
		}
	}


	m_fRotateLevelZ -= m_fRotateSlowDownZ;
	if( (float)abs(m_fRotateLevelZ) < CAMERA_ROT_MIN )
	{
		m_fRotateLevelZ = 0.0f;
		m_fRotateSlowDownZ = 0.0f;
	}
	else
	{
		m_fAnglePitch += (float)(m_fRotateLevelZ)  * c_fRotate;
		if( m_fAnglePitch < CAMERA_PITCH_RANGE_MIN )
		{
			m_fAnglePitch = CAMERA_PITCH_RANGE_MIN;
		}
		else if( m_fAnglePitch > CAMERA_PITCH_RANGE_MAX )
		{
			m_fAnglePitch = CAMERA_PITCH_RANGE_MAX;
		}
	}

}
#endif

//------------------------------------------------------------------------------
/**
*/
void Camera::SetRotateState( BOOL bState )
{
	m_bIsRotate = bState; 
	m_bIsRotateX = bState; 
	m_bIsRotateZ = bState; 

	if(bState==FALSE)
	{
		CheckTurnState();
		m_bRidingTurn = FALSE;
	}
}
//------------------------------------------------------------------------------
/** 라이더 자동회전
*/
void Camera::AutoTurnState()
{

	float fDestAngle   = g_pHero->GetAngle();
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
	float fAngleGap = 0.f;
	switch(iTurnSTate)
	{
	case TARGET_LEFT:			fAngleGap = fabs(m_fAngleYaw-fDestAngle);break;
	case TARGET_RIGHT:			fAngleGap = fabs(fDestAngle-m_fAngleYaw);break;	
	case TARGET_REVERSE_LEFT:	fAngleGap = fabs((WZ_PI*2.f)-fDestAngle+m_fAngleYaw);break;
	case TARGET_REVERSE_RIGHT:	fAngleGap = fabs((WZ_PI*2.f)-m_fAngleYaw+fDestAngle);break;
	}
	
	float fAngle = n_rad2deg(fAngleGap);
    //40'도이상벌어질경우 자동의로 돌려버린다.    
	if(fAngle>40.f)
	{
		CheckTurnState();
		m_bRidingTurn= FALSE;
	}
}
//------------------------------------------------------------------------------
/** 라이더 자동회전
*/
void Camera::CheckTurnState()
{
	if(!g_pHero )
	{
		return;
	}

	//우회전이 시작되는부분이다.
	if( m_bRidingTurn && g_pHero->IsRiding() && !g_pHero->IsMove() && !g_pHero->IsJumping())
	{
		if(g_pHero->GetCurState()!=STATE::RIDER_TURN && g_pHero->GetNextState()==STATE::NONE)
		{
			g_pHero->SetNextState(STATE::RIDER_TURN,g_CurTime);
		}
	}
}
//------------------------------------------------------------------------------
/**
*/
void Camera::SetYaw(float fYaw)
{
	m_fAngleYaw = fYaw;
	if( m_fAngleYaw < CAMERA_YEW_RANGE_MIN )
	{
		m_fAngleYaw += CAMERA_YEW_RANGE_MAX;
	}
	else if( m_fAngleYaw > CAMERA_YEW_RANGE_MAX )
	{
		m_fAngleYaw -= CAMERA_YEW_RANGE_MAX;
	}
}

//------------------------------------------------------------------------------
/**
*/
float Camera::GetYaw()
{
	return m_fAngleYaw;

}

//------------------------------------------------------------------------------
/**
*/
void Camera::SetPitch(float fPitch)
{
	m_fAnglePitch = fPitch;
	if( m_fAnglePitch < CAMERA_PITCH_RANGE_MIN )
	{
		m_fAnglePitch = CAMERA_PITCH_RANGE_MIN;
	}
	else if( m_fAnglePitch > CAMERA_PITCH_RANGE_MAX )
	{
		m_fAnglePitch = CAMERA_PITCH_RANGE_MAX;
	}
}

//------------------------------------------------------------------------------
/**
*/
void Camera::RotateAroundFocus(float fAngle)
{
	SetYaw(m_fAngleYaw + fAngle);
}

//------------------------------------------------------------------------------
/**
*/
float Camera::GetFarClip()
{
	return m_fFarClip;

}

//------------------------------------------------------------------------------
/**
*/
float Camera::GetNearClip()
{
	return m_fNearClip;

}

float Camera::GetFOV()
{
	return m_fFOV;

}
void Camera::RestoreProjection()
{
	g_pSunRenderer->SetProjectionMatrix( m_fFOV, m_fNearClip, m_fFarClip ,m_eRatio);

	float fFade[2] = { max(0.0f,m_fFarClip - 30.0f),m_fFarClip };
}

WzRay Camera::GenerateCameraRay()
{
	WzRay ray;

	ray.m_wvOrigin = m_CameraPosition;
	ray.m_wvDirection = m_CameraFrom - m_CameraTo;
	VectorNormalize(&ray.m_wvDirection,&ray.m_wvDirection);

	return ray;
}

//------------------------------------------------------------------------------
/**
*/
void Camera::UpdateCamera()
{
	if(g_pSunRenderer)
	{
		g_pSunRenderer->SetCameraMatrix( &m_CameraFrom, &m_CameraTo, &m_wvZ );

		//const St_LightInformation* pstLightInfo;
		//pstLightInfo = g_pSunRenderer->x_pManagerLight->GetLightInfo( 0 );
		//WzVector wvLightDir = pstLightInfo->m_wvDirection;

		//St_Camera* pCamera = g_pSunRenderer->GetCameraInfo();
		//float fDot = fabsf( VectorDotProduct( &pCamera->m_wvDir, &wvLightDir ) );

		//float fMin = 0.000001f;
		//float fMax = 0.0005f;
		//float fTemp = fMax - fMin;

		//fTemp = ( 1.0f - fDot ) * fTemp;
		//fTemp = fMin + fTemp;

	}
}

void Camera::SetAspectRatio(ENUM_SCREEN_RATIO eRatio)
{
	m_eRatio = eRatio;
	RestoreProjection();
}

#ifdef _KST_CREATECHARACTER_SPRINGCAMERA_

void Camera::InitSpringCameraPosition( const WzVector& From, const WzVector& To )
{
	SetVector(&m_wvZ, 0.0, 0.0, 1.0f);

	m_targetOldPos = m_chasePos;

	m_CameraTo = To;
	m_chasePos = From;

	m_targetOldPos = m_CameraFrom = m_chasePos;

	UpdateCamera();
}

void Camera::SetSpringCameraPosition( const WzVector& From, const WzVector& To, DWORD dwTick )
{
	SetVector(&m_wvZ, 0.0, 0.0, 1.0f);

	m_targetOldPos = m_chasePos;

	m_chasePos = From;
	m_CameraTo = To;

	if( m_chasePos != m_CameraFrom )
	{
		vector3 from,chasePos,targetOldPos;

		from.set(m_CameraFrom.x,m_CameraFrom.z,m_CameraFrom.y);
		chasePos.set(m_chasePos.x,m_chasePos.z,m_chasePos.y);
		targetOldPos.set(m_targetOldPos.x,m_targetOldPos.z,m_targetOldPos.y);


		float fTick=(float)dwTick/1000.0f;
		from = SpringDamp(from, chasePos, targetOldPos, 5.0f * fTick, m_springConst, m_dampConst, m_springLen);

		SetVector(&m_CameraFrom,from.x,from.z,from.y);
	}
}

#else // _KST_CREATECHARACTER_SPRINGCAMERA_

void Camera::AttachToBone(const WzVector& From, const WzVector& To )
{
	SetVector(&m_wvZ, 0.0, 0.0, 1.0f);

	m_CameraFrom = From;
	m_CameraTo = To;

	UpdateCamera();
}

#endif // _KST_CREATECHARACTER_SPRINGCAMERA_

float Camera::GetCameraDistance()
{
	return m_fDistance;
}


void Camera::FreeCameraMoveFrontBack( int iDist )
{
	m_CameraFrom += m_CameraDirection;
}

float  Camera::GetCurrentCameraFocusRange()
{
	WzVector vDiff = m_CameraTo - m_CameraFrom;

	return VectorLength(&vDiff);
}

//------------------------------------------------------------------------------
/**
*/
void Camera::MoveFrontBack( int iDist )
{

	float c_fRotate = GetRotationSpeed();

	m_fDistanceLevel += (float)iDist * CAMERA_ROT_SLOWDOWN_LEVEL;
	m_fDistanceSlowDown = m_fDistanceLevel * CAMERA_ROT_SLOWDOWN;

	m_fDistance -= m_fDistanceLevel * c_fRotate;

	float fRate = 1.0f;

	CGameOption* pOption = NULL;

	pOption = CGameOption::Instance();

	if (pOption)
	{
		const OptionParam& option = pOption->GetGameOption();
        if (option.GetOptionValue( nOptionType::USE_EXTENSION_CAMERA_DISTANCE ) )
        {
			fRate = 1.5f;
        }
	}

	if (g_bObserverMode)
	{
		fRate = 1.5f;
	}


#ifdef PERF_AUTO_MOVE_TEST
	fRate = 5.0f;
#endif 

	if (m_fDistance < CAMERA_DISTANCE_RANGE_MIN)
	{
		m_fDistance = CAMERA_DISTANCE_RANGE_MIN;
	}
	else if (m_fDistance > (CAMERA_DISTANCE_RANGE_MAX * fRate))
	{
		m_fDistance = CAMERA_DISTANCE_RANGE_MAX * fRate;
	}

	m_fDistanceLevel -= m_fDistanceSlowDown;
	if ((float)abs(m_fDistanceLevel) < CAMERA_ROT_MIN)
	{
		m_fDistanceLevel = 0.0f;
		m_fDistanceSlowDown = 0.0f;
	}
	else
	{
		m_fDistance -= m_fDistanceLevel * c_fRotate;
		if (m_fDistance < CAMERA_DISTANCE_RANGE_MIN)
		{
			m_fDistance = CAMERA_DISTANCE_RANGE_MIN;
		}
		else if (m_fDistance > (CAMERA_DISTANCE_RANGE_MAX * fRate))
		{
			m_fDistance = CAMERA_DISTANCE_RANGE_MAX * fRate;
		}
	}
}



void Camera::SetRotationSpeed()
{

	switch(m_iRotationLevel)
	{
    case 0:
        m_fRotationSpeed = CAMERA_DEFAULT_ROTATE_SPEED * 0.4f;
        break;
	case 1:
		m_fRotationSpeed = CAMERA_DEFAULT_ROTATE_SPEED * 0.6f;
		break;
	case 2:
		m_fRotationSpeed = CAMERA_DEFAULT_ROTATE_SPEED * 0.8f;
		break;
	case 3:
		m_fRotationSpeed = CAMERA_DEFAULT_ROTATE_SPEED * 1.0f;
		break;
	case 4:
		m_fRotationSpeed = CAMERA_DEFAULT_ROTATE_SPEED * 1.2f;
		break;
	case 5:
		m_fRotationSpeed = CAMERA_DEFAULT_ROTATE_SPEED * 1.4f;
		break;
	default:
		m_fRotationSpeed = CAMERA_DEFAULT_ROTATE_SPEED;
		break;
	}
}


float Camera::GetRotationSpeed()
{
	return m_fRotationSpeed;
}


void Camera::SetRotationLevel(int iLevel)
{
	m_iRotationLevel = iLevel;
	SetRotationSpeed();
}

int Camera::GetRotationLevel()
{
	return m_iRotationLevel;
}

void Camera::SetRotationRange(int min, int max)
{
	m_RotationScrollRange.iMin = min;
	m_RotationScrollRange.iMax = max;
}

sScope Camera::GetRotationRange()
{
	return m_RotationScrollRange;
}

int	Camera::GetRotationRangeMin()
{
	return m_RotationScrollRange.iMin;
}

int Camera::GetRotationRangeMax()
{
	return m_RotationScrollRange.iMax;
}



