#include "StdAfx.h"

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
//
#include <float.h>

#include <Struct.h>
#include <ServerOptionParser.h>
#include <Global.h>
#include <PublicMath.h>
#include <NumericValues.h>
#include <PacketControl/WarMessages_Sync.h>

#include "Map.h"
#include "GameField.h"
#include "Player.h"
#include "PlayerManager.h"
#include "MovementChecker.h"
#include "MoveStateControl.h"
#include "GameZone.h"
#include "GameZoneManager.h"
#include "StatusManager.h"
#include <ServerOptionParserEx.h>
#include "MovingPlanner.h"

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
//
FLOAT			MovementChecker::sfSYNC_HACK_DISTANCE2 = .0f;
FLOAT			MovementChecker::sfSYNC_FAIL_DISTANCE2 = .0f;
FLOAT			MovementChecker::sfSYNC_FAIL_DISTANCE_ACCEPT2 = .0f;
FLOAT			MovementChecker::sfSYNC_WARN_DISTANCE2 = .0f;
DWORD			MovementChecker::sdwFRAME_DELTA_TICK = 0;
DWORD			MovementChecker::sdwFRAME_POLE_TICK = 0;
WzVector		MovementChecker::sWzVectorZero = { .0f, .0f, .0f };

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
//
MovementChecker::MovementChecker( Player* pPlayer )
{
	m_pPlayer = pPlayer;
	Reset();
}

MovementChecker::~MovementChecker()
{
}

BOOL		MovementChecker::PreCalcScript()
{
	// 서버와 클라간의 플레이어 현재 위치차
	SERVER_COMMON_OPTION& stServerOptionInfo = ServerOptionParserEx::Instance()->GetServerOption();

	sfSYNC_HACK_DISTANCE2 = (FLOAT)(stServerOptionInfo.m_byMOVE_SYNC_HACKING_DISTANCE)*(stServerOptionInfo.m_byMOVE_SYNC_HACKING_DISTANCE);
	sfSYNC_FAIL_DISTANCE2 = (FLOAT)(stServerOptionInfo.m_byMOVE_SYNC_FAIL_DISTANCE)*(stServerOptionInfo.m_byMOVE_SYNC_FAIL_DISTANCE);
	BYTE byFailAccept = stServerOptionInfo.m_byMOVE_SYNC_FAIL_DISTANCE<<1;
	sfSYNC_FAIL_DISTANCE_ACCEPT2 = (FLOAT)(byFailAccept*byFailAccept);
	sfSYNC_WARN_DISTANCE2 = (FLOAT)(stServerOptionInfo.m_byMOVE_SYNC_WARNING_DISTANCE)*(stServerOptionInfo.m_byMOVE_SYNC_WARNING_DISTANCE);

	return TRUE;
}

VOID		MovementChecker::Reset()
{
	m_bFirst = TRUE;

	INTERNAL.Reset();
	MOVEKEYBOARD.Reset();
	MOVEKEYBOARD_ANGLE = 0;
	MOVEKEYBOARD_STATE = 0;
	MOVEKEYBOARDJ.Reset();
	MOVEMOUSE.Reset();

	PATH_EXP.LatestAcceptTick = 0;
	SetVector( &PATH_EXP.LatestAcceptPosition, .0f, .0f, .0f );
}

VOID		MovementChecker::Update(  DWORD dwDeltaTick )
{
	sdwFRAME_DELTA_TICK = dwDeltaTick;
	sdwFRAME_POLE_TICK = GetTickCount();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
//
DWORD	MovementChecker::sMSG_TIMER_INFO::Increase()
{
	return ++ReceivedCount;
}

VOID	MovementChecker::sMSG_TIMER_INFO::Reset()
{
	AcceptTimer.Instance();
	ReceivedCount = 0;
}

BOOL	MovementChecker::sPATH_INFO::SetupLatestState( MovementChecker* pChecker, FLOAT fDistOfCS, BYTE byMoveState, WzVector& wvCurPos, WzVector& wvPos )
{
	if( MovementChecker::sWzVectorZero == LatestAcceptPosition )
	{
		LatestAcceptPosition = wvCurPos;
		LatestAcceptTick = GetTickCount();
		fAccumulatedDistance = .0f;
		//return TRUE;
	}
	else
	{
		Player* pPlayer = pChecker->m_pPlayer;
		DWORD dwTicks = GetTickCount()-LatestAcceptTick;
		LatestAcceptTick = GetTickCount();

		BOOL bAcceptClientPos = TRUE;
		const BOOL bFaultTolerence = pChecker->IsFaultToleranceDifference( byMoveState );
		const FLOAT fFixValue = bFaultTolerence ? sfSYNC_WARN_DISTANCE2 : 7.0f ;

		WzVector& wvDIRnDIST_SERVER = (wvCurPos-LatestAcceptPosition);
		FLOAT fLen2Server = Math_Vector2DLength2( &wvDIRnDIST_SERVER );
		FLOAT fDifference = 0.0f;
		// 이 경우는 Server는 아직 업데이트 안되어 있거나 소규모 이동으로 인식하는 경우
		// 서버 처리 지연에 대한 보정+적은 거리는 검사하지 않는다.
		if( fLen2Server > (4.4f) )
		{
			WzVector wvDIRnDIST_CLIENT = (wvPos-LatestAcceptPosition);
			FLOAT fLen2Client = Math_Vector2DLength2( &wvDIRnDIST_CLIENT );

			fDifference = fLen2Client-fLen2Server;
			if( fDifference > .0f )
			{
				FLOAT fMovSpd = pPlayer->GetMoveStateControl()->GetMoveSpeed();
				DWORD dwFixTicks = dwTicks;// - (dwTicks>>4);// dwTicks>>2 + dwTicks>>3; // 87%
				FLOAT fMaxDistRadius = ((FLOAT)dwFixTicks)*fMovSpd;
				fMaxDistRadius = fMaxDistRadius * fMaxDistRadius;
				fMaxDistRadius += fFixValue;

				if( fMaxDistRadius < fLen2Client )	// 클라이동 크다...
				{
					fAccumulatedDistance += sqrt(fDifference);
					//printf( "S:%f  C:%f  S:%f  M:%f  D:%f\n",
					//	sqrt(fLen2Server), sqrt(fLen2Client),
					//	fAccumulatedDistance, sqrt(fMaxDistRadius),
					//	sqrt(fDifference) );

					bAcceptClientPos = FALSE;
				}
			}
		}
		//LatestAcceptPosition = wvCurPos;

		if( bAcceptClientPos )
		{
			if( dwTicks < DELAY_MAX_PERMIT_LASTED_STATE+1 )
				pPlayer->SetPos( &wvPos );
			LatestAcceptPosition = wvPos;
			return TRUE;
		}

		LatestAcceptPosition = wvCurPos;
	}
	return FALSE;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
//
BOOL	MovementChecker::IsFiniteVector( const WzVector& rVector )
{
	if( _finite(rVector.x) && _finite(rVector.y) && _finite(rVector.z) )
		return TRUE;
	return FALSE;
}

BOOL
MovementChecker::IsFaultToleranceDifference( BYTE byMoveState )
{
	const BYTE cbyCheck1 = KEYBOARDMOVE_LEFTTURN|KEYBOARDMOVE_RIGHTTURN;
	const BYTE cbyCheck2 = KEYBOARDMOVE_LEFTSTEP|KEYBOARDMOVE_RIGHTSTEP;

	//	상태가 틀려지는 경우에 있어 위와 같은 상황이 이전 상태로 존재할 경우
	//	오차 발생할 수 있다.
	BYTE byCheckState = byMoveState;
	if( MOVEKEYBOARD_STATE != byMoveState )
		byCheckState = KEYBOARDMOVE_FOWARD;

	if(
		((KEYBOARDMOVE_FOWARD & byCheckState) && (cbyCheck1 & byCheckState))
		||
		((cbyCheck1 & byCheckState) && (cbyCheck2 & byCheckState))
		)
		return TRUE;

	return FALSE;
}

BOOL	MovementChecker::KBMoveCommonDIST(
									WzVector& OUT wvDirection		// 결과, 이동 방향 벡터 설정
									)
{
	const FLOAT cfMaxDistance = 10.0f;
	FLOAT fMovSpd = m_pPlayer->GetMoveStateControl()->GetMoveSpeed();

	FLOAT fDistance = (fMovSpd == .0f) ? cfMaxDistance : min( cfMaxDistance, (DELAY_DEFAULT_KBMOVE_SYN)*fMovSpd );
	wvDirection *= fDistance;

	// 최종 목적지 좌표가 유효한지 체크
	return IsFiniteVector( wvDirection );
}


MovementChecker::eMOVE_CTRL
MovementChecker::CanStartAction( const eMOVE_TYPE mtTYPE, const WORD wAngle, const BYTE byMoveState, const BOOL bSpecial )
{
	ITimeout& rTIMEOUT_I = INTERNAL.AcceptTimer;
	ITimeout& rTIMEOUT_K = MOVEKEYBOARD.AcceptTimer;
	ITimeout& rTIMEOUT_J = MOVEKEYBOARDJ.AcceptTimer;
	ITimeout& rTIMEOUT_M = MOVEMOUSE.AcceptTimer;

	eMOVE_CTRL eCTRL = this->CANTACCEPTABLE;

	DWORD dwMASK = TIMEOUT_BIT_PASS;

	switch( mtTYPE )
	{
	case this->KEYBOARD_MOVE:
		{
			//printf( "TICK %u\tMOVSTATE %u\n", GetTickCount(), byMoveState );
			MOVEKEYBOARD.Increase();
			BOOL bNONMOVE = !(KEYBOARDMOVE_ISMOVING&byMoveState);
			if( bNONMOVE )
			{
				dwMASK = INSTANCE_BIT_KJM;
				eCTRL = this->ACCEPT_STOP;
				break;
			}

			if( (MOVEKEYBOARD_STATE != byMoveState) || (MOVEKEYBOARD_ANGLE != wAngle) )
			{	// 단방향에서 방향을 바꾸거나 할 때... 이때는 Timeout을 무시할 수 있도록 하자.
				dwMASK = INSTANCE_BIT_KJM;
				eCTRL = this->ACCEPT_ALL;
				//MOVEKEYBOARD_STATE = byMoveState;
				MOVEKEYBOARD_ANGLE = wAngle;
				break;
			}
			else if( rTIMEOUT_K.IsExpired() ) // || ((KEYBOARDMOVE_FOWARD&byMoveState) && ((KEYBOARDMOVE_LEFTTURN|KEYBOARDMOVE_RIGHTTURN)&byMoveState)) )
			{
				dwMASK = TIMEOUT_BIT_K;
				eCTRL = this->ACCEPT_ALL;
				break;
			}

			dwMASK = TIMEOUT_BIT_PASS;
			eCTRL = this->CANTACCEPTABLE;
		}
		break;
	case this->KEYBOARD_JUMP:
		{
			MOVEKEYBOARDJ.Increase();
			if( rTIMEOUT_J.IsExpired() )
			{
				dwMASK = TIMEOUT_BIT_J;
				eCTRL = this->ACCEPT_ALL;
				break;
			}

			dwMASK = TIMEOUT_BIT_PASS;
			eCTRL = this->CANTACCEPTABLE;
		}
		break;
	case this->KEYBOARD_JUMP_STOP:
		{
			MOVEKEYBOARDJ.Increase();
			// <WARNING> 점프 트랜잭션 내에 처리가 되어야 한다.
			dwMASK = TIMEOUT_BIT_J;
			eCTRL = this->ACCEPT_ALL;
		}
		break;
	case this->MOUSE_MOVE:
		{
			MOVEMOUSE.Increase();
			if( bSpecial || rTIMEOUT_M.IsExpired() )
			{
				dwMASK = TIMEOUT_BIT_M;
				eCTRL = this->ACCEPT_ALL;
				break;
			}

			dwMASK = TIMEOUT_BIT_PASS;
			eCTRL = this->CANTACCEPTABLE;
		}
		break;
	case this->MOVE_STOP:
		{
			MOVEMOUSE.Increase();
			dwMASK = INSTANCE_BIT_KJM;
			eCTRL = this->ACCEPT_ALL;
		}
		break;
	default:
		{
			dwMASK = TIMEOUT_BIT_KJM;
			eCTRL = this->CANTACCEPTABLE;
		}
		break;
	};

	//printf( "MASK %08X  eCTRL %u\n", dwMASK, eCTRL );
	if( TIMEOUT_BIT_PASS == dwMASK )
		return eCTRL;

	if( INSTANCE_BIT_K & dwMASK )
		MOVEKEYBOARD.Reset();
	if( INSTANCE_BIT_J & dwMASK )
		MOVEKEYBOARDJ.Reset();
	if( INSTANCE_BIT_M & dwMASK )
		MOVEMOUSE.Reset();

	if( TIMEOUT_BIT_I & dwMASK )
		rTIMEOUT_I.SetNextTimeoutByCurTickBased(INTERVAL_LASTED_ACCEPT_SYN);

	if( TIMEOUT_BIT_K & dwMASK )
	{
		MOVEKEYBOARD.Reset();
		rTIMEOUT_K.SetNextTimeoutByCurTickBased(DELAY_MIN_MOVE_TRANSACTION);
	}
	if( TIMEOUT_BIT_J & dwMASK )
	{
		MOVEKEYBOARDJ.Reset();
		rTIMEOUT_J.SetNextTimeoutByCurTickBased(DELAY_MIN_JUMP_TRANSACTION);
	}
	if( TIMEOUT_BIT_M & dwMASK )
	{
		MOVEMOUSE.Reset();
		rTIMEOUT_M.SetNextTimeoutByCurTickBased(DELAY_MIN_MOVE_TRANSACTION);
	}

	return eCTRL;
}


BOOL
MovementChecker::AddMovement( WzVector& wvCurPos/*, WzVector* pwvPosOfPlayer*/ )
{
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
// 이동패킷 공통로직(CG_SYNC_MOVE_SYN, CG_SYNC_TARGET_MOVE_SYN)
RC::eSYNC_RESULT
MovementChecker::MoveCommon(
							BOOL				bIsTargetMove,
							eCHAR_MOVE_STATE	eState,		// CMS_RUN, ... 이동 상태
							WzVector&			wvCurPosR,	// 현재 좌표
							WzVector&			wvDestPosR,	// 이동할 좌표
							INT					iNumRoute,	// PathRoute 개수
							TILEINDEX*			piRoute,	// PathRoute 배열
							DWORD& OUT			rdwErrorLevel
							)
{
	static BOOL __bInit = PreCalcScript();

	// 플레이어가 있는 필드 포인터가 유효한지 검사
	GameField* pField = m_pPlayer->GetField();
	if( !pField )
		return RC::RC_SYNC_FIELD_IS_NULL;

	// 플레이어가 살아있지 않으면 리턴
	if( ! m_pPlayer->IsAlive() )
		return RC::RC_SYNC_PLAYER_IS_DEAD;

    //removed by __NA000000_080324_KEYBOARD_MOVE_FILTER_REMOVE__
	//eMOVE_CTRL eCTRL = CanStartAction( this->MOUSE_MOVE, 0, (BYTE)eState, bIsTargetMove );
	//if( this->CANTACCEPTABLE == eCTRL )
	//	return RC::RC_SYNC_FAILED;

	BOOL bProcess = TRUE;
	RC::eSYNC_RESULT eRET = RC::RC_SYNC_SUCCESS;

	StatusManager& rStatusManager = *m_pPlayer->GetStatusManager();

    ;{  // (NOTE) to support a shadow control, __NA_S00015_20080828_NEW_HOPE_SHADOW
	// 현재 유저가 SUCTION 스킬 사용중이였으면 STOP 시킨다.
	BaseStatus* pStatus = NULL;
	pStatus = rStatusManager.FindStatus( eCHAR_STATE_HP_SUCTION );
	if( pStatus )		pStatus->StopStatus();

	pStatus = rStatusManager.FindStatus( eCHAR_STATE_MP_SUCTION );
	if( pStatus )		pStatus->StopStatus();
    };

	while( bProcess )
	{
#if !defined(_DEV_TAIYO_PATHENGINE_CODE)

		bProcess = bProcess && !((DWORD)iNumRoute > _MAX_PATHROUTE_SIZE);
		bProcess = bProcess && iNumRoute ;

		if( !bProcess )
		{
			eRET = RC::RC_SYNC_ROUTE_SIZE_EXCESS;
			continue;
		}

#endif //< _DEV_TAIYO_PATHENGINE_CODE

		// 이동가능한지 체크 (임시...그냥 return 하면 안된다.) -- 뭐지 이건?
		bProcess = bProcess && m_pPlayer->CanMove();
		if( !bProcess )
		{
			if( rStatusManager.AnimationDelay.NeedSkipProcessByAnimationDelay( (BaseStatus*)NULL ) )
			{
				eRET = RC::RC_SYNC_ANIMATION_DELAY_PROCESSING;
				continue;
			}
			eRET = RC::RC_SYNC_PLAYER_IS_UNMOVABLE_STATE;
			continue;
		}

		bProcess = bProcess && ( CMS_RUN == eState || CMS_WALK == eState );
		if( !bProcess )
		{
			eRET = RC::RC_SYNC_INVALID_MOVESATE;
			continue;
		}

		//wvCurPosR;	// 삐꾸 자료형 보정
		//wvDestPosR;	// 삐꾸 자료형 보정

		// 서버와 클라간의 플레이어 현재 위치차

		// 현재 좌표 유효성 검사
		bProcess = IsFiniteVector( wvCurPosR );
		if( !bProcess )
		{
			eRET = RC::RC_SYNC_CUR_POSITION_IS_INVALID;
			continue;
		}

		WzVector wvPosOfPlayer;
		m_pPlayer->GetPos( &wvPosOfPlayer );
		FLOAT fCSdistance2 = 0;

		const BOOL scbPROCESS = TRUE;	// 이 값은 변경시키지 말 것.
		const FLOAT fPathEnginePrecision2 = (0.3f)*(0.3f);

		// 스크립트에 있는 제한값 boundary check
		if( scbPROCESS )
		{
			fCSdistance2 = Math_Vector2DLength2( &(wvPosOfPlayer - wvCurPosR) );

			// 해킹으로 간주!!!

			// 패킷에 있는 이동 시작 위치가 현재 서버가 가진 위치와 쌩뚱맞게 다름
			bProcess = bProcess && (sfSYNC_HACK_DISTANCE2 > fCSdistance2);
			bProcess = bProcess && (sfSYNC_FAIL_DISTANCE2 > fCSdistance2);
			if( !bProcess )
			{
				eRET = RC::RC_SYNC_CUR_POSITION_IS_INVALID;
				continue;
			}

			// Path Engine 사용시 이 오차는 과연...
			bProcess = bProcess && (sfSYNC_WARN_DISTANCE2 > fCSdistance2);
			if( !bProcess )
			{
				bProcess = TRUE;	// 거리 오차 문제 발생하긴 했지만 허용하는 수치
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// 3DEngine - Path관련 처리
		// PATHENGINE을 사용하는 경우, PathRoute 정보는 없다.
		// <THINKING>
		//	서버가 아닌 POS와 클라가 보내준 POS... 어떤 것을 사용해야 하는가?
		BOOL bSamePosition = !(fCSdistance2 > fPathEnginePrecision2);
		if( pField->IsWarCastle() )
		{
#ifdef _DEV_TAIYO_PATHENGINE_CODE
			bProcess = bProcess && bSamePosition;
			bProcess = bProcess && pField->FindPath( m_pPlayer, &wvPosOfPlayer );
			if( !bProcess )
			{
				eRET = RC::RC_SYNC_SETPATH_ERROR;
				continue;
			}
#endif //
		}

		if( scbPROCESS )
		{
#ifdef _DEV_TAIYO_PATHENGINE_CODE
			// 플레이어 위치를 클라가 보내준 어택 시작점으로 옮긴다.
			bProcess = m_pPlayer->SetPath( &wvPosOfPlayer, &wvDestPosR, NULL, 0 );
#else
			// 타일인덱스 변환작업(short배열 -> int배열)
			int iRoutes[_MAX_PATHROUTE_SIZE]; // no init
            // use only = { 'CG_SYNC_MOVE_SYN', 'CG_SYNC_TARGET_MOVE_SYN' }
            BOOST_STATIC_ASSERT(\
                _countof(iRoutes) == \
                _countof(reinterpret_cast<MSG_CG_SYNC_MOVE_SYN*>(0)->m_iRoute) &&
                // -- and --
                _countof(iRoutes) == \
                _countof(reinterpret_cast<MSG_CG_SYNC_TARGET_MOVE_SYN*>(0)->m_iRoute));

            { // copy conversion block
                // pre-checked : assert(iNumRoute <= _MAX_PATHROUTE_SIZE)
                const TILEINDEX* src = &piRoute[0];
                int* dest = &iRoutes[0];
                for (int loop = iNumRoute; loop; --loop) {
                    *dest++ = *src++;
                }
            };
			//copy<TILEINDEX*, INT*>( &piRoute[0], &piRoute[iNumRoute], iRoutes );
			bProcess = m_pPlayer->SetPath( &wvPosOfPlayer, &wvDestPosR, iRoutes, iNumRoute );
#endif //< _DEV_TAIYO_PATHENGINE_CODE

			if( !bProcess )
			{
				eRET = RC::RC_SYNC_SETPATH_ERROR;
				continue;
			}
		}

		// 상태 설정 - 이것의 위치는?
		rStatusManager.Move();
		m_pPlayer->SetMoveState( eState );

//////		// 이동량 체크를 위해 누적
//////		if( !m_pPlayer->GetMovementChecker()->AddMovement( wvPosOfPlayer ) )
//////		{
//////			SECURELOG( CSunLog::cCRITICAL_LOG, CSunLog::cCATE_CONNECTION,
//////				SECURELOG_GAME_SERVER_PLAYER_INFO_ARG(m_pPlayer, sVIOLATION::MOVE_POSITION),
//////				_T("[MoveCommon] AddMovement Fail!") );
//////		}

		wvCurPosR = wvPosOfPlayer;

		return RC::RC_SYNC_SUCCESS;
	}

	//printf( "move invalid state %u\n", eRET );
	m_pPlayer->GetPos( &wvCurPosR );

	return eRET;
}

RC::eSYNC_RESULT
MovementChecker::KBMoveCommonCHECK( WzVector& wvCurPosR, BYTE IN byMoveState, BOOL& OUT bNeedMoveCalc, DWORD& OUT rdwErrorLevel )
{
	static BOOL __bInit = PreCalcScript();

	// 플레이어가 있는 필드 포인터가 유효한지 검사
	GameField* pField = m_pPlayer->GetField();
	if( !pField )
		return RC::RC_SYNC_FIELD_IS_NULL;

	// 플레이어가 살아있지 않으면 리턴
	if( ! m_pPlayer->IsAlive() )
		return RC::RC_SYNC_PLAYER_IS_DEAD;

	BOOL bProcess = TRUE;
	RC::eSYNC_RESULT eRET = RC::RC_SYNC_SUCCESS;

	StatusManager& rStatusManager = *m_pPlayer->GetStatusManager();

	while( bProcess )
	{
		// 이동가능한지 체크 (임시...그냥 return 하면 안된다.) -- 뭐지 이건?
		bProcess = bProcess && m_pPlayer->CanMove();
		if( !bProcess )
		{
			if( rStatusManager.AnimationDelay.NeedSkipProcessByAnimationDelay( (BaseStatus*)NULL ) )
			{
				eRET = RC::RC_SYNC_ANIMATION_DELAY_PROCESSING;
				continue;
			}
			eRET = RC::RC_SYNC_PLAYER_IS_UNMOVABLE_STATE;
			continue;
		}

		// 서버와 클라간의 플레이어 현재 위치차 먼저 동일한지 검사
		// 동일하다면 굳이 검사할 필요 없다... 정말? 일단 이후는 좌표 검증 쪽이므로 이전에 이미 검증이 되었을 것이다.
		// 이것은 TURN 계열과 같은 것들 무의미한 연산 제거용
		WzVector wvPosOfPlayer;
		m_pPlayer->GetPos( &wvPosOfPlayer );

		if( wvPosOfPlayer == wvCurPosR )
		{
			bNeedMoveCalc = FALSE;
			return RC::RC_SYNC_SUCCESS;
		}

		// 현재 좌표 유효성 검사
		bProcess = IsFiniteVector( wvCurPosR );
		if( !bProcess )
		{
			eRET = RC::RC_SYNC_CUR_POSITION_IS_INVALID;
			continue;
		}

		FLOAT fCSdistance2 = 0;

		const BOOL scbPROCESS = TRUE;	// 이 값은 변경시키지 말 것.
		const FLOAT fPathEnginePrecision2 = (0.3f)*(0.3f);

		//////////////////////////////////////////////////////////////////////////
		// 3DEngine - Path관련 처리
		// PATHENGINE을 사용하는 경우, PathRoute 정보는 없다.
		// <THINKING>
		//	서버가 아닌 POS와 클라가 보내준 POS... 어떤 것을 사용해야 하는가?
		fCSdistance2 = Math_Vector2DLength2( &(wvPosOfPlayer - wvCurPosR) );
		BOOL bSamePosition = !(fCSdistance2 > fPathEnginePrecision2);
		if( !bSamePosition )
		{
			// 스크립트에 있는 제한값 boundary check

			// 해킹으로 간주!!!

			// 패킷에 있는 이동 시작 위치가 현재 서버가 가진 위치와 쌩뚱맞게 다름
			bProcess = bProcess && (sfSYNC_HACK_DISTANCE2 > fCSdistance2);
			if( !bProcess )
			{
				eRET = RC::RC_SYNC_CUR_POSITION_IS_INVALID;
				continue;
			}

			// 계산하기 힘든 형태
			bProcess = bProcess && (sfSYNC_FAIL_DISTANCE2 > fCSdistance2);
			if( !bProcess )
			{
				if( sfSYNC_FAIL_DISTANCE_ACCEPT2 > fCSdistance2 )
				{
					if( IsFaultToleranceDifference( byMoveState ) )
						bProcess = TRUE;
				}

				if( !bProcess )
				{
					eRET = RC::RC_SYNC_CUR_POSITION_IS_INVALID;
					continue;
				}
			}

			// 거리 오차 문제 발생하긴 했지만 허용하는 수치
			// 이는 키보드 이동시 방향전환같은 문제 땜시 발생할 수 있는 오차다.
			// 하지만 패스엔진에서 문제가 발생한다. 음...
			bProcess = bProcess && (sfSYNC_WARN_DISTANCE2 > fCSdistance2);
			if( !bProcess )
			{
				
				//if( IsFaultToleranceDifference( byMoveState ) )
					bProcess = TRUE;
				//else
				//{
				//	eRET = RC::RC_SYNC_CUR_POSITION_IS_INVALID;
				//	continue;
				//}
			}
		}
		MOVEKEYBOARD_STATE = byMoveState;

		if( pField->IsWarCastle() )
		{
#ifdef _DEV_TAIYO_PATHENGINE_CODE
			bProcess = bProcess && bSamePosition;
			bProcess = bProcess && pField->FindPath( m_pPlayer, &wvPosOfPlayer );
			if( !bProcess )
			{
				eRET = RC::RC_SYNC_SETPATH_ERROR;
				continue;
			}
#endif //
		}

		// -------- 원본 소스에 존재하는 루틴 -------- //
		// 기존
		//	키보드 이동에 대한 이동 계산을 할 것인지 말 것인지를 결정한다.
		//	기존 코드의 문제점은 클라가 보내주는 코드로 바로 플레이어 위치를 설정함에 있었다.
		//	{	m_pPlayer->SetPos( &wvCurPosition );	}
		// 개선 방안
		//	아직 충분히 개선되지 않았다. 이동 처리에 대해서는 클라와 서버간의 오차가 심한 편이며,
		//	결국 클라이언트 데이터를 어느정도 신뢰해야 하는 형편이다.
		//  따라서, 임시방편으로나마 적정 주기 내에 수신되는 경우 허용하도록 한다.
		//	즉, 오차율 5미터...-.-;;; 크다. (패스엔진 적용시 0.3미터...)

		bNeedMoveCalc = bSamePosition ? FALSE : TRUE ;
		if( bNeedMoveCalc )
			m_pPlayer->SetPos( &wvCurPosR );
            //removed by __NA000000_080324_KEYBOARD_MOVE_FILTER_REMOVE__
			//if( !PATH_EXP.SetupLatestState( this, fCSdistance2, byMoveState, wvPosOfPlayer, wvCurPosR ) )
			//{
			//	bProcess = FALSE;
			//	eRET = RC::RC_SYNC_FAILED;
			//	continue;
			//}
		//if( !bProcess )
		//{
		//	eRET = RC::RC_SYNC_DEST_POSITION_IS_INVALID;
		//	continue;
		//}

		//printf( "Distance %.1f,%.1f,%.1f <-> %.1f,%.1f,%.1f\n",
		//	wvPosOfPlayer.x, wvPosOfPlayer.y, wvPosOfPlayer.z,
		//	wvCurPosR.x, wvCurPosR.y, wvCurPosR.z );

		rStatusManager.Move();	// 상태 설정 - 이것의 위치는? - 과거와는 다르게 상태에 따른 계산이 처리된다. 이동 성공시에 처리하도록 하자.

		return RC::RC_SYNC_SUCCESS;
	}

	//printf( "---------------------------------------------move invalid state %u\n", eRET );
	bNeedMoveCalc = FALSE;

	return eRET;
}

VOID
MovementChecker::KBMoveCommonDIR( WzVector &wvDirection, WORD wAngle, BOOL bFastMove, BYTE byMoveState )
{
	wAngle = (wAngle) % 360;

	wvDirection.x = g_SinTable.FSIN(wAngle);
	wvDirection.y = -g_SinTable.FCOS(wAngle);
	wvDirection.z = 0.0f;

	const DWORD DIR_FB = 0;
	const DWORD DIR_LR = 1;
	WzVector wvTemp[2];

	SetVector( &wvTemp[0], .0f, .0f, .0f );
	SetVector( &wvTemp[1], .0f, .0f, .0f );

	WzVector& wvDirFB = wvTemp[DIR_FB];		// 전후진 벡터
	WzVector& wvDirLR = wvTemp[DIR_LR];		// 좌우 벡터

	if( !bFastMove )
	{
		// 전후진 벡터중 선택
		if( KEYBOARDMOVE_FOWARD&byMoveState )
		{	// 전진
			wvDirFB = wvDirection;
		}
		else if( KEYBOARDMOVE_BACKWARD&byMoveState )
		{	// 후진
			wvDirFB = -wvDirection;
		}

		// 좌게걸음, 우게걸음 벡터중 선택
		if( KEYBOARDMOVE_RIGHTSTEP&byMoveState )
		{
			wvDirLR.x = wvDirection.y;		// sin(a - 90) = -cos(a) = wvDir.y;
			wvDirLR.y = -wvDirection.x;		// -cos(a - 90) = -sin(a) = -wvDir.x;
		}
		else if( KEYBOARDMOVE_LEFTSTEP&byMoveState )
		{
			wvDirLR.x = -wvDirection.y;		// sin(a + 90) = cos(a)  = -wvDir.y;
			wvDirLR.y = wvDirection.x;		// -cos(a + 90) = sin(a) = wvDir.x;
		}
	}
	else
	{	// FAST MOVING
		if( CMS_TUMBLING_RIGHT==byMoveState )
		{
			wvDirLR.x = wvDirection.y;
			wvDirLR.y = -wvDirection.x;
		}
		else if( CMS_TUMBLING_LEFT==byMoveState )
		{
			wvDirLR.x = -wvDirection.y;
			wvDirLR.y = wvDirection.x;
		}
		else if( CMS_TUMBLING_BACK==byMoveState )
		{
			wvDirFB = -wvDirection;
		}
		else
		{	// 전진 숏대시
			wvDirFB = wvDirection;
		}
	}

	// 전후진 벡터와 좌우 벡터를 더한 벡터를 노멀라이즈 한 것이 최종 방향
	WzVector& wvAdded = wvDirFB + wvDirLR;
	VectorNormalize( &wvDirection, &wvAdded );
}


RC::eSYNC_RESULT	MovementChecker::OnRecvCG_SYNC_KBMOVE_SYN( MSG_CG_SYNC_KBMOVE_SYN* pRecvMsg, DWORD& OUT rdwErrorLevel )
{
    GameField* pField = m_pPlayer->GetField();
    if (pField == NULL) {
        return RC::RC_SYNC_FIELD_IS_NULL;
    };
#ifdef _NA000000_SPEED_HACK_PREVENTION_SUPPORT
    this->sync_tuner_.CheckClientAbusedData(pRecvMsg->client_test_result);
    if (this->sync_tuner_.EnabledMoving(pRecvMsg->issued_serial) == false) {
        return RC::RC_SYNC_PLAYER_IS_UNMOVABLE_STATE;
    };
#endif
	BYTE byMoveState = pRecvMsg->m_byMoveState;

    //removed by __NA000000_080324_KEYBOARD_MOVE_FILTER_REMOVE__
	//eMOVE_CTRL eCTRL = CanStartAction( this->KEYBOARD_MOVE, pRecvMsg->m_wAngle, byMoveState );
	//if( this->CANTACCEPTABLE == eCTRL )
	//	return RC::RC_SYNC_FAILED;

	WzVector wvCurPosR = pRecvMsg->wvCurPos;
	BOOL rbNeedMoveCalc = TRUE;	// 이동 계산 필요 여부 결과

	RC::eSYNC_RESULT rcResult = KBMoveCommonCHECK( wvCurPosR, byMoveState, rbNeedMoveCalc, rdwErrorLevel );
	//printf( "NEED CALC %u\n", rbNeedMoveCalc );
	while( RC::RC_SYNC_SUCCESS == rcResult )
	{
		BOOL bProcess = TRUE;

        BOOL bContinue = TRUE;
		while (bContinue)
		{
			// 전후진이나 게걸음이 없는 경우에는 그냥 정지!
			// (WAVERIX)(NOTICE)
			// 다른 조합 없이 LEFT_TURN, RIGHT_TURN의 경우 굳이 계산 또는 BRD 처리할 필요 없다.
            if( !(KEYBOARDMOVE_ISMOVING&byMoveState) )
			{
				// 전후진이나 게걸음이 없는 경우에는 그냥 정지!
				BOOL bIsMoving = m_pPlayer->IsMoving();
				m_pPlayer->StopMoving();

				if( !byMoveState )
				{
					MSGSUB_SYNC_KBMOVE_BRD msg;
					msg.m_wPlayerKey	= (WORD)m_pPlayer->GetObjectKey();
					msg.m_wvCurPos		= wvCurPosR;
					msg.m_wAngle		= pRecvMsg->m_wAngle;
					msg.m_byMoveState	= byMoveState;
					m_pPlayer->SendPacketAroundForSync( &msg );
					return RC::RC_SYNC_SUCCESS;
				}
#ifndef _NA_008333_20150615_BROADCAST_LEFT_RIGHT_TURN
                if( !bIsMoving && ((KEYBOARDMOVE_LEFTTURN == byMoveState) || (KEYBOARDMOVE_RIGHTTURN == byMoveState)) )
                 	return RC::RC_SYNC_SUCCESS;
#endif // _NA_008333_20150615_BROADCAST_LEFT_RIGHT_TURN
			}

			// 최종 방향을 구한다.
			WzVector wvDestPosR;
			KBMoveCommonDIR( wvDestPosR, pRecvMsg->m_wAngle, FALSE, byMoveState );

			// 0.5초 동안 이동할 수 있는 최대 거리를 구한다.(클라에서 0.5초 간격으로 이동패킷을 보낸다)
			bProcess = KBMoveCommonDIST( wvDestPosR );
			if( !bProcess )
			{
				rcResult = RC::RC_SYNC_DEST_POSITION_IS_INVALID;
				bContinue = FALSE;
				continue;
			}

			eCHAR_MOVE_STATE cmsMOVSTATE =
				KEYBOARDMOVE_FOWARD&byMoveState ? CMS_RUN
				: KEYBOARDMOVE_BACKWARD&byMoveState ? CMS_BACKSTEP
				: CMS_SIDESTEP;

			m_pPlayer->SetMoveState( cmsMOVSTATE );

			//if( rbNeedMoveCalc )
			{
				bProcess = bProcess && pField->FindPathThrust( m_pPlayer, &wvDestPosR );
				if( !bProcess )
				{
					bProcess = pField->IsMovableCoordinates( &wvCurPosR );
					if( !bProcess )
					{
						rcResult = RC::RC_SYNC_CUR_POSITION_IS_INVALID_NO_HACK;
						bContinue = FALSE;
						continue;
					}
					m_pPlayer->SetPos( &wvCurPosR );
				}
			}
			//printf( "Distance (%.1f,%.1f,%.1f) %.1f,%.1f,%.1f <-> %.1f,%.1f,%.1f\n",
			//	   pRecvMsg->wvCurPos.x, pRecvMsg->wvCurPos.y, pRecvMsg->wvCurPos.z,
			//	   wvCurPosR.x, wvCurPosR.y, wvCurPosR.z,
			//	   wvDestPosR.x, wvDestPosR.y, wvDestPosR.z );

			break;
		}

		if( bProcess )
		{
			if( rbNeedMoveCalc )
			{
				// 이동량 체크를 위해 누적
				if( !m_pPlayer->GetMovementChecker()->AddMovement( wvCurPosR ) )
				{
					SECURELOG( SECURELOG_PLAYERINFO(m_pPlayer, eABUSE_LOGIC_VIOLATION, eABUSE_SUB_LOGIC_VIOLATION_MOVE_SPEED),
						_T("[OnCG_SYNC_KBMOVE_SYN] AddMovement Fail!") );
				}
			}

			// 개선 사항
			//	- 여러 상태에 따른 정보(방향 등)를 서버가 검사해서 BRD여부를 결정하면 어떨까?
			// 다른 플레이어들에게 브로드 캐스팅
			{
				MSGSUB_SYNC_KBMOVE_BRD msg;
				msg.m_wPlayerKey	= (WORD)m_pPlayer->GetObjectKey();
				msg.m_wvCurPos		= wvCurPosR;
				msg.m_wAngle		= pRecvMsg->m_wAngle;
				msg.m_byMoveState	= byMoveState;
				m_pPlayer->SendPacketAroundForSync( &msg );
			};
		}
		else if( RC::RC_SYNC_SUCCESS != rcResult )
		{
			bContinue = FALSE;
			continue;
		}

		//printf( "End\n" );
		return rcResult;
	}

	//printf( "Distance (%f,%f,%f) %f,%f,%f\n",
	//	pRecvMsg->wvCurPos.x, pRecvMsg->wvCurPos.y, pRecvMsg->wvCurPos.z,
	//	wvCurPosR.x, wvCurPosR.y, wvCurPosR.z
	//	);

	return rcResult;
}

RC::eSYNC_RESULT	MovementChecker::OnRecvCG_SYNC_FAST_KBMOVE_SYN( MSG_CG_SYNC_FAST_KBMOVE_SYN* pRecvMsg, DWORD& OUT rdwErrorLevel )
{
    GameField* pField = m_pPlayer->GetField();	// 성공한다는 의미는 Field 존재한다는 의미
    if (pField == NULL) {
        return RC::RC_SYNC_FIELD_IS_NULL;
    };
#ifdef _NA000000_SPEED_HACK_PREVENTION_SUPPORT
    this->sync_tuner_.CheckClientAbusedData(pRecvMsg->client_test_result);
    if (this->sync_tuner_.EnabledMoving(pRecvMsg->issued_serial) == false) {
        return RC::RC_SYNC_PLAYER_IS_UNMOVABLE_STATE;
    };
#endif
    //removed by __NA000000_080324_KEYBOARD_MOVE_FILTER_REMOVE__
	//eMOVE_CTRL eCTRL = CanStartAction( this->KEYBOARD_MOVE, 0, KEYBOARDMOVE_ISMOVING, TRUE );
	//if( this->CANTACCEPTABLE == eCTRL )
	//	return RC::RC_SYNC_FAILED;

	WzVector wvCurPosR = pRecvMsg->m_wvCurPos;
	BOOL bNeedMoveCalc = TRUE;
	RC::eSYNC_RESULT rcResult = KBMoveCommonCHECK( wvCurPosR, 0, bNeedMoveCalc, rdwErrorLevel );

	//printf( "NEED CALC %u\n", rbNeedMoveCalc );
	while( RC::RC_SYNC_SUCCESS == rcResult )
	{
		BOOL bProcess = TRUE;

		for( BOOL bContinue = TRUE ; bContinue ; )
		{
			// 이동상태가 특수이동(숏대시, 구르기) 상태가 맞는지 체크
			BYTE byCMS_MoveState = pRecvMsg->m_byCmsMoveState;

			bProcess = !( pRecvMsg->m_byCmsMoveState < CMS_TUMBLING_FRONT || pRecvMsg->m_byCmsMoveState > CMS_MAX );
			if( !bProcess )
			{
				rcResult = RC::RC_SYNC_INVALID_MOVESATE;
				bContinue = FALSE;
				continue;
			}

			// 최종 방향을 구한다.
			WzVector wvFinalDir;
			KBMoveCommonDIR( wvFinalDir, pRecvMsg->m_wAngle, TRUE, byCMS_MoveState );

			// 속도 결정
			m_pPlayer->SetMoveState( (eCHAR_MOVE_STATE)byCMS_MoveState );
			float fMoveSpeed = m_pPlayer->GetMoveStateControl()->GetMoveSpeed();

			// 구해진 최종 방향으로 이동거리 구하기
			float fMoveDistance = GetMoveTimeAsState( pRecvMsg->m_byCmsMoveState ) * fMoveSpeed;
			wvFinalDir = wvFinalDir * fMoveDistance;

			// 최종 목적지 좌표가 유효한지 체크
			bProcess = IsFiniteVector( wvFinalDir );
			if( !bProcess )
			{
				rcResult = RC::RC_SYNC_DEST_POSITION_IS_INVALID;
				bContinue = FALSE;
				continue;
			}

			bProcess = pField->FindPathThrust( m_pPlayer, &wvFinalDir );
			if( !bProcess )
			{
				rcResult = RC::RC_SYNC_SETPATH_ERROR;
				bContinue = FALSE;
				continue;
			}

			// 이동량 체크를 위해 누적
			if( !m_pPlayer->GetMovementChecker()->AddMovement( pRecvMsg->m_wvCurPos ) )
			{
				SUNLOG( eCRITICAL_LOG, "[OnCG_SYNC_FAST_KBMOVE_SYN] AddMovement Fail!" );
			}

			// 다른 플레이어들에게 브로드 캐스팅
			{
				MSGSUB_SYNC_FAST_KBMOVE_BRD msg;
				msg.m_wPlayerKey		= (WORD)m_pPlayer->GetObjectKey();
				msg.m_wvCurPos			= pRecvMsg->m_wvCurPos;
				msg.m_wAngle			= pRecvMsg->m_wAngle;
				msg.m_byCmsMoveState	= pRecvMsg->m_byCmsMoveState;
				m_pPlayer->SendPacketAroundForSync( &msg );
			};
			return rcResult;
		}
	}

	return rcResult;
}

RC::eSYNC_RESULT	MovementChecker::OnRecvCG_SYNC_STOP_SYN( MSG_CG_SYNC_STOP_SYN* pRecvMsg, DWORD& OUT rdwErrorLevel )
{
	// 플레이어가 있는 필드 포인터가 유효한지 검사
	GameField* pField = m_pPlayer->GetField();
	if( !pField )
		return RC::RC_SYNC_FIELD_IS_NULL;
#ifdef _NA000000_SPEED_HACK_PREVENTION_SUPPORT
    this->sync_tuner_.CheckClientAbusedData(pRecvMsg->client_test_result);
    if (this->sync_tuner_.EnabledStopping(pRecvMsg->issued_stop_serial) == false) {
        return RC::RC_SYNC_PLAYER_IS_UNMOVABLE_STATE;
    };
#endif
	WzVector wvPlayerPos;
	WzVector wvChangePos = pRecvMsg->wvCurPos;

	// 현재 좌표가 유효한지 체크
	BOOL bProcess = TRUE;
	RC::eSYNC_RESULT eRET = RC::RC_SYNC_SUCCESS;

	while( bProcess )
	{
		bProcess = IsFiniteVector( wvChangePos );
		if( !bProcess )
		{
			eRET = RC::RC_SYNC_CUR_POSITION_IS_INVALID;
			continue;
		}

		m_pPlayer->GetPos( &wvPlayerPos );

		FLOAT fCSDistance2 = Math_Vector2DLength2( &(wvPlayerPos - wvChangePos) );
		bProcess = ( sfSYNC_FAIL_DISTANCE2 > fCSDistance2 );
		if( !bProcess )
		{
			eRET = RC::RC_SYNC_CUR_POSITION_IS_INVALID;
			continue;
		}

		m_pPlayer->StopMoving();
		m_pPlayer->SetMoveState( CMS_STOP );

		// 이동량 체크를 위해 누적
		if( !m_pPlayer->GetMovementChecker()->AddMovement( pRecvMsg->wvCurPos ) )
		{
			SUNLOG( eCRITICAL_LOG, "[OnCG_SYNC_STOP_SYN] AddMovement Fail!" );
		}

		// 이후 일단 정지 브로드를 날리지 말고 NAK 처리로 테스트해보자
		// 서버가 더 빠를 것이다. 일반적으로... 이걸 감안해 서버 데이터를 클라데이터로 변경시키는 것이 더 안정적
		m_pPlayer->SetPos( &wvChangePos );

		// 다른 플레이어들에게 브로드 캐스팅
		{
			MSGSUB_SYNC_STOP_BRD msg;
			msg.m_dwObjectKey	= m_pPlayer->GetObjectKey();
			msg.m_wvCurPos	= wvChangePos;
			m_pPlayer->SendPacketAroundForSync( &msg );
		};

		//SUNLOG( eCRITICAL_LOG, "[CG_SYNC_STOP_SYN] Stop! (%5.2f %5.2f %5.2f)", pRecvMsg->wvCurPos.x, pRecvMsg->wvCurPos.y, pRecvMsg->wvCurPos.z );
		return eRET;
	}

	// ERROR HANDLING
	return eRET;
}


RC::eSYNC_RESULT	MovementChecker::OnRecvCG_SYNC_JUMP_SYN( MSG_CG_SYNC_JUMP_SYN* pRecvMsg, DWORD& OUT rdwErrorLevel, eJUMP_TYPE jumpType )
{
	//////////////////////////////////////////////////////////////////////////
	// <ERROR>
	BOOL bProcess = TRUE;
	RC::eSYNC_RESULT eRET = RC::RC_SYNC_SUCCESS;

	GameField* pField = m_pPlayer->GetField();

	bProcess = bProcess && (pField != NULL);
    if(bProcess == false) {
        return RC::RC_SYNC_FIELD_IS_NULL;
    };
#ifdef _NA000000_SPEED_HACK_PREVENTION_SUPPORT
    this->sync_tuner_.CheckClientAbusedData(pRecvMsg->client_test_result);
    if (this->sync_tuner_.EnabledMoving(pRecvMsg->issued_serial) == false) {
        return RC::RC_SYNC_PLAYER_IS_UNMOVABLE_STATE;
    };
#endif
    // CHANGES: f110808.1L, added the disabled movable state filter.
    if (m_pPlayer->CanMove() == false) {
        return RC::RC_SYNC_PLAYER_IS_UNMOVABLE_STATE;
    };
    //
	while( bProcess )
	{
		// 계속 사용할 Player 현재 위치
		WzVector wvPosOfPlayer;
		m_pPlayer->GetPos( &wvPosOfPlayer );

		WzVector wvPosDest = pRecvMsg->m_wvDestPos;
        // CHANGES: f110321.2L
        FiltersForMovableCoordinates coord_check_filter = { 0, };
        // 유효한 위치인가 체크
        bProcess = pField->IsMovableCoordinates(&wvPosDest, &coord_check_filter);
		if( !bProcess )
		{
			eRET = RC::RC_SYNC_DEST_POSITION_IS_INVALID;
			continue;
		}

		// 점프로 갈 수 있는 최대 이동거리 체크
		const DWORD cdwMaxDistance = (DWORD)(MAX_JUMP_DISTANCE * MAX_JUMP_DISTANCE);
		DWORD dwCSdistance2 = (DWORD)Math_Vector2DLength2( &(wvPosOfPlayer - wvPosDest) );
		// + : 아래로, - : 위로
		FLOAT fHeight = wvPosOfPlayer.z - wvPosDest.z;

		// 거리가 제한값 이상이거나 4미터보다 높이 뛰면 에러 처리
		bProcess = ( cdwMaxDistance > dwCSdistance2 ) && (fHeight > -4.0f);
		if( !bProcess )
		{
			eRET = RC::RC_SYNC_JUMP_LENGTH_TOO_LONG;
			continue;
		}

		const INT iTilePrecision = pField->GetMovingPlanner()->GetTileToStand( wvPosDest, NULL, -1.0f, 1.0f );
        bProcess = (coord_check_filter.result_tile_no == iTilePrecision);
		if( !bProcess )
		{
			eRET = RC::RC_SYNC_DEST_POSITION_IS_INVALID;
			continue;
		}

		//printf( "PROCED1\n" );

		//////////////////////////////////////////////////////////////////////////
		// <PROCESSING>
		m_pPlayer->SetMoveState( CMS_RUN );			// 점프상태 추가 필요!
        
        eRET = pField->JumpObject(m_pPlayer, &wvPosDest);
		if (eRET != RC::RC_SYNC_SUCCESS)
        {
            // (CHANGE)(20100412)(hukim) 
            // 점프 목적지 선정이 실패 할 수 있는데도 성공 여부에 상관없이 이전 코드는 계속 진행한다.
            // 서버 위치는 갱신되지 않았는데 클라이언트에게 브로드캐스팅 함으로써 서로 위치차가 
            // 발생할 가능성이 있다. 점프 성공 여부를 검사하도록 수정한다.
            // 현재 구조상 위처럼 continue 처리하면 무한 루프에 빠질 가능성이 있다.
            // 일단은 특별히 문제없는 이전 구조를 유지하고 전체적인 구조 검토후에 수정한다.

            //continue;
        }
    #ifdef _NA000000_SPEED_HACK_PREVENTION_SUPPORT
        this->sync_tuner_.OnJump(wvPosDest);
    #endif
		// <BROADCAST>
		if( JUMP_NORMAL == jumpType )
		{
			MSGSUB_SYNC_JUMP_BRD msg;
			msg.m_PlayerKey	= m_pPlayer->GetObjectKey();
			msg.m_wvDestPos	= wvPosDest;
			msg.m_iDir		= pRecvMsg->m_iDir;
			pField->SendPacketAroundForSync( m_pPlayer, &msg );
		}

		// JUMP_DAMAGE_HEIGHT(8)미터 초과 높이부터 1m당 60데미지
		fHeight = (JUMP_DAMAGE_HEIGHT < fHeight) ? fHeight-JUMP_DAMAGE_HEIGHT : .0f;
		// 목적지가 높거나 뛰어내리는 곳이 8미터 미만일때
		if( fHeight != .0f )
		{
			DAMAGETYPE dtDamage = (DAMAGETYPE)(fHeight * JUMP_METER_PER_DAMAGE);
			DWORD ResultDamage = m_pPlayer->DecreaseHP( dtDamage );
			SUNLOG( eDEV_LOG, "[MovementChecker::OnRecvCG_SYNC_JUMP_SYN] Jump Damage = %d", dtDamage );

			// 떨어져 죽었으면 낙사 저장 & 뛰기 전 위치 저장
			if( m_pPlayer->IsDead() )
			{
				m_pPlayer->SetFallDead(wvPosOfPlayer);
			}
			else
			{
				DWORD dwReserveHP = m_pPlayer->GetReserveHP() + ( ResultDamage / 2 );
				if( m_pPlayer->GetMaxHP() < ( m_pPlayer->GetHP() + dwReserveHP ) )
				{
					m_pPlayer->SetReserveHP(  ( m_pPlayer->GetHP() + dwReserveHP ) - m_pPlayer->GetMaxHP() );
				}
				else
				{
					m_pPlayer->SetReserveHP( dwReserveHP );
				}
			}
            IVehicleEvent::OnEvent(m_pPlayer, IVehicleEvent::Hit());
		}

		// 이동량 체크를 위해 누적
		if( !m_pPlayer->GetMovementChecker()->AddMovement( wvPosOfPlayer ) )
		{
			SUNLOG( eCRITICAL_LOG, "[CG_SYNC_JUMP_SYN] AddMovement Fail!" );
		}

		//printf( "PROCED-D\n" );

		return eRET;
	};

//ERROR_PROCEDURE:
	return eRET;
}

RC::eSYNC_RESULT	MovementChecker::OnRecvCG_SYNC_JUMP_STOP_SYN( MSG_CG_SYNC_JUMP_STOP_SYN* pRecvMsg, DWORD& OUT rdwErrorLevel )
{
    MSG_CG_SYNC_JUMP_SYN msg_syn;
    msg_syn.m_wvDestPos = pRecvMsg->m_wvDestPos;
    msg_syn.m_iDir = 0;
#ifdef _NA000000_SPEED_HACK_PREVENTION_SUPPORT
    msg_syn.issued_serial = pRecvMsg->issued_serial;
    msg_syn.client_test_result = pRecvMsg->client_test_result;
#endif //
    return OnRecvCG_SYNC_JUMP_SYN(&msg_syn, rdwErrorLevel, JUMP_STOP);
}


RC::eSYNC_RESULT	MovementChecker::OnRecvCG_SYNC_MOVE_SYN( MSG_CG_SYNC_MOVE_SYN* pRecvMsg, DWORD& OUT rdwErrorLevel )
{
	WzVector wvCurPosR = pRecvMsg->wvCurPos;
	WzVector wvDestPosR = pRecvMsg->wvDestPos;

	RC::eSYNC_RESULT rcResult = MoveCommon( FALSE, CMS_RUN,
										wvCurPosR, wvDestPosR, 
										pRecvMsg->m_byNumRoute, pRecvMsg->m_iRoute, rdwErrorLevel );

	if( RC::RC_SYNC_SUCCESS == rcResult )
	{
		MSGSUB_SYNC_MOVE_BRD msg;
		msg.m_dwObjectKey	= m_pPlayer->GetObjectKey();
		msg.m_byState		= CMS_RUN;	//캐릭터의 경우는 무조건 뛰기만 존재!!(몬스터와 같이 쓰는 패킷이므로 값은 넣어줌)
		msg.m_ForcedMove	= FALSE;
		msg.m_wvCurPos		= wvCurPosR;
		msg.m_wvDestPos		= wvDestPosR;

		m_pPlayer->SendPacketAroundForSync( &msg );
	}

	return rcResult;
}

RC::eSYNC_RESULT	MovementChecker::OnRecvCG_SYNC_TARGET_MOVE_SYN( MSG_CG_SYNC_TARGET_MOVE_SYN* pRecvMsg, DWORD& OUT rdwErrorLevel )
{
	WzVector wvCurPosR = pRecvMsg->wvCurPos;
	WzVector wvDestPosR = pRecvMsg->wvDestPos;

	RC::eSYNC_RESULT rcResult = MoveCommon(	TRUE, CMS_RUN,
										wvCurPosR, wvDestPosR, 
										pRecvMsg->m_byNumRoute, pRecvMsg->m_iRoute, rdwErrorLevel );

	if( RC::RC_SYNC_SUCCESS == rcResult )
	{
		MSGSUB_SYNC_TARGET_MOVE_BRD msg;
		msg.m_dwTargetKey	= pRecvMsg->m_dwTargetKey;
		msg.m_wPlayerKey	= (WORD)m_pPlayer->GetObjectKey();//플레이어키가 현재는 WORD범위를 넘지 않기때문!!(패킷 최적화를 위해 줄여서 보냄)
		msg.m_wvCurPos		= wvCurPosR;
		msg.m_wvDestPos		= wvDestPosR;

		m_pPlayer->SendPacketAroundForSync( &msg );
	}

	return rcResult;
}



VOID				MovementChecker::AbusingNotify( RC::eSYNC_RESULT eRET, DWORD IN dwErrorLevel, FLOAT fDIST )
{

	TCHAR* pAbusingMsg = NULL;
	eABUSE_SUB_TYPE eTYPE;
	switch( eRET )
	{
		//	Skip Code List
		case RC::RC_SYNC_SUCCESS:
		case RC::RC_SYNC_PLAYER_IS_DEAD:			// 플레이어가 죽어서 이동실패
		case RC::RC_SYNC_FIELD_IS_NULL:				// 플레이어의 필드값이 NULL이다.
		case RC::RC_SYNC_PLAYER_IS_UNMOVABLE_STATE:	// 플레이어가 이동할 수 없는 상태이다.
		case RC::RC_SYNC_FAILED:
		case RC::RC_SYNC_SETPATH_ERROR:
		case RC::RC_SYNC_ANIMATION_DELAY_PROCESSING:
			break;
		//	Abusing Notification List
		case RC::RC_SYNC_ROUTE_SIZE_EXCESS:			// 경로 개수 최대값 초과
			pAbusingMsg = _T("[MOVE] Cracked Packet Route Size Excess! %f");
			eTYPE = eABUSE_SUB_LOGIC_VIOLATION_MOVE_CRACKED_PACKET;
			break;
		case RC::RC_SYNC_INVALID_MOVESATE:			// 이동상태 이상(걷기: 달리기)
			pAbusingMsg = _T("[MOVE] Cracked Packet Invalid MoveState! %f");
			eTYPE = eABUSE_SUB_LOGIC_VIOLATION_MOVE_CRACKED_PACKET;
			break;
		case RC::RC_SYNC_CUR_POSITION_IS_INVALID:	// 클라가 보내준 현재좌표가 유효하지 않다.
		case RC::RC_SYNC_DEST_POSITION_IS_INVALID:	// 목적지 좌표가 유효하지 않다.
			pAbusingMsg = _T("[MOVE] Irregular Coordinates! %f");
			eTYPE = eABUSE_SUB_LOGIC_VIOLATION_INVALID_POSITION;
			break;
		case RC::RC_SYNC_JUMP_LENGTH_TOO_LONG:		// 점프거리가 너무 멀다
			pAbusingMsg = _T("[MOVE] JUMP is Too Long! %f");
			eTYPE = eABUSE_SUB_LOGIC_VIOLATION_MOVE_RANGE;
			break;
		default:
			break;
	}

	if( pAbusingMsg )
	{
		SECURELOG( SECURELOG_PLAYERINFO(m_pPlayer, eABUSE_LOGIC_VIOLATION, eTYPE),
			pAbusingMsg, fDIST );
	}
}





//====================================================================================================
//
//
//
//
//
//====================================================================================================

#ifdef _NA_0_20110803_PLAYER_MOVEMENT_SYNC
void MovementChecker::Move_Stop(MSG_CG_SYNC_STOP_SYN* recv_message)
{
    GameField* field = m_pPlayer->GetField();
    ASSERT(field);
    if (field == NULL)
    {
        send_move_fail(RC::RC_SYNC_FIELD_IS_NULL);
        return;
    }

    // 클라이언트의 좌표가 유효한 좌표인지 검증한다.
    // 좌표가 틀린 경우 보낸 캐릭터의 위치를 강제 조절한다.
    if (!is_same_position(recv_message->wvCurPos))
    {
        MSG_CG_SYNC_MOVE_NAK nak_message;
        nak_message.m_wvCurPos = *m_pPlayer->GetPosPtr();
        nak_message.m_byErrorCode = RC::RC_SYNC_CUR_POSITION_IS_INVALID;
        m_pPlayer->SendPacket(&nak_message, sizeof(nak_message));
    }

    m_pPlayer->StopMoving();
    m_pPlayer->SetMoveState(CMS_STOP);

    MSGSUB_SYNC_STOP_BRD brd_message;
    brd_message.m_dwObjectKey = m_pPlayer->GetObjectKey();
    brd_message.m_wvCurPos = *m_pPlayer->GetPosPtr();
    m_pPlayer->SendPacketAroundForSync(&brd_message);
}

void MovementChecker::KeyBoard_Move(MSG_CG_SYNC_KBMOVE_SYN* recv_message)
{
    GameField* field = m_pPlayer->GetField();
    ASSERT(field);
    if (field == NULL)
    {
        send_move_fail(RC::RC_SYNC_FIELD_IS_NULL);
        return;
    }

    // 캐릭터의 현재 상태가 이동 가능한지 검증한다. 
    RC::eSYNC_RESULT result = check_state_enable_move();
    if (result != RC::RC_SYNC_SUCCESS)
    {
        send_move_fail(result);
        return;
    }

    // 클라이언트의 좌표가 유효한 좌표인지 검증한다.
    if (!is_same_position(recv_message->wvCurPos))
    {
        send_move_fail(RC::RC_SYNC_CUR_POSITION_IS_INVALID);
        return;
    }

    // 실제 이동 명령을 내린다.
    result = do_run(recv_message->m_wAngle, recv_message->m_byMoveState);
    if (result != RC::RC_SYNC_SUCCESS)
    {
        send_move_fail(result);
        return;
    }

    // (NOTE) to support a shadow control, __NA_S00015_20080828_NEW_HOPE_SHADOW
    // 현재 유저가 SUCTION 스킬 사용중이였으면 STOP 시킨다.
    BaseStatus* pStatus = NULL;
    pStatus = m_pPlayer->GetStatusManager()->FindStatus(eCHAR_STATE_HP_SUCTION);
    if(pStatus)     
        pStatus->StopStatus();

    pStatus = m_pPlayer->GetStatusManager()->FindStatus(eCHAR_STATE_MP_SUCTION);
    if(pStatus)     
        pStatus->StopStatus();
}

void MovementChecker::KeyBoard_Move(MSG_CG_SYNC_FAST_KBMOVE_SYN* recv_message)
{
    GameField* field = m_pPlayer->GetField();
    ASSERT(field);
    if (field == NULL)
    {
        send_move_fail(RC::RC_SYNC_FIELD_IS_NULL);
        return;
    }

    // 캐릭터의 현재 상태가 이동 가능한지 검증한다. 
    RC::eSYNC_RESULT result = check_state_enable_move();
    if (result != RC::RC_SYNC_SUCCESS)
    {
        send_move_fail(result);
        return;
    }

    // 클라이언트의 좌표가 유효한 좌표인지 검증한다.
    if (!is_same_position(recv_message->m_wvCurPos))
    {
        send_move_fail(RC::RC_SYNC_CUR_POSITION_IS_INVALID);
        return;
    }


}




void MovementChecker::send_move_fail(RC::eSYNC_RESULT result)
{
    MSG_CG_SYNC_MOVE_NAK nak_message;
    nak_message.m_wvCurPos = *m_pPlayer->GetPosPtr();
    nak_message.m_byErrorCode = result;
    m_pPlayer->SendPacket(&nak_message, sizeof(nak_message));

    // 플레이어를 정지시킨다.
    m_pPlayer->StopMoving();
    m_pPlayer->SetMoveState(CMS_STOP);

    switch(result)
    {
    case RC::RC_SYNC_SUCCESS:
    case RC::RC_SYNC_PLAYER_IS_DEAD:                    // 플레이어가 죽어서 이동실패
    case RC::RC_SYNC_FIELD_IS_NULL:                     // 플레이어의 필드값이 NULL이다.
    case RC::RC_SYNC_PLAYER_IS_UNMOVABLE_STATE:         // 플레이어가 이동할 수 없는 상태이다.
    case RC::RC_SYNC_SETPATH_ERROR:
    case RC::RC_SYNC_ROUTE_SIZE_EXCESS:                 // 경로 개수 최대값 초과
    case RC::RC_SYNC_INVALID_MOVESATE:                  // 이동상태 이상(걷기: 달리기)
    case RC::RC_SYNC_CUR_POSITION_IS_INVALID:           // 클라가 보내준 현재좌표가 유효하지 않다.
    case RC::RC_SYNC_DEST_POSITION_IS_INVALID:          // 목적지 좌표가 유효하지 않다.
    case RC::RC_SYNC_JUMP_LENGTH_TOO_LONG:              // 점프거리가 너무 멀다
    case RC::RC_SYNC_CUR_POSITION_IS_INVALID_NO_HACK:   // 클라가 보내준 현재좌표가 유효하지 않다. 맵의 경계선에서 클라가 조금 틀린 좌표를 보낼 수 있다.
        {
            // 주변에 정지를 알린다.
            MSGSUB_SYNC_STOP_BRD stop_message;
            stop_message.m_dwObjectKey = m_pPlayer->GetObjectKey();
            stop_message.m_wvCurPos = *m_pPlayer->GetPosPtr();
            m_pPlayer->SendPacketAroundForSync(&stop_message);
        }
        break;
    case RC::RC_SYNC_ANIMATION_DELAY_PROCESSING:
        break;
    case RC::RC_SYNC_FAILED:
        break;
    default:
        break;
    }
}


RC::eSYNC_RESULT MovementChecker::do_fast_run(ushort angle, uchar fast_state)
{
    return RC::RC_SYNC_SUCCESS;
}


RC::eSYNC_RESULT MovementChecker::do_run(ushort angle, uchar move_state)
{
    WzVector server_position;
    m_pPlayer->GetPos(&server_position);

    MOVEKEYBOARD_STATE = move_state;
    StatusManager* status_manager = m_pPlayer->GetStatusManager();
    status_manager->Move();


    // 전후진이나 게걸음이 없는 경우에는 그냥 정지!
    // 다른 조합 없이 LEFT_TURN, RIGHT_TURN의 경우 굳이 계산 또는 BRD 처리할 필요 없다.
    if (!(KEYBOARDMOVE_ISMOVING & move_state))
    {
        // 전후진이나 게걸음이 없는 경우에는 그냥 정지!
        bool is_moving = m_pPlayer->IsMoving();
        m_pPlayer->StopMoving();

        if (!move_state)
        {
            MSGSUB_SYNC_KBMOVE_BRD brd_message;
            brd_message.m_wPlayerKey	= (WORD)m_pPlayer->GetObjectKey();
            brd_message.m_wvCurPos		= server_position;
            brd_message.m_wAngle		= angle;
            brd_message.m_byMoveState	= move_state;
            m_pPlayer->SendPacketAroundForSync(&brd_message);
            return RC::RC_SYNC_SUCCESS;
        }

        if (!is_moving && ((KEYBOARDMOVE_LEFTTURN == move_state) || (KEYBOARDMOVE_RIGHTTURN == move_state)))
            return RC::RC_SYNC_SUCCESS;
    }

    // 최종 방향을 구한다.
    WzVector direction;
    KBMoveCommonDIR(direction, angle, FALSE, move_state);


    eCHAR_MOVE_STATE character_move_state =
        KEYBOARDMOVE_FOWARD&move_state ? CMS_RUN
        : KEYBOARDMOVE_BACKWARD&move_state ? CMS_BACKSTEP
        : CMS_SIDESTEP;

    m_pPlayer->SetMoveState(character_move_state);


    // 0.1초후의 위치를 목표 위치로 하며 증가될 이동량을 구한 후
    // FindPathThrust() 를 이용해 위치를 증가시킨다.
    const uint move_time = 100;
    float move_speed = m_pPlayer->GetMoveStateControl()->GetMoveSpeed();

    WzVector destination = direction * (move_speed * move_time);
    if (!IsFiniteVector(destination))
        return RC::RC_SYNC_DEST_POSITION_IS_INVALID;

   GameField* field = m_pPlayer->GetField();
   if (field->FindPathThrust(m_pPlayer, &destination))
   {
       MSGSUB_SYNC_KBMOVE_BRD brd_message;
       brd_message.m_wPlayerKey	= (WORD)m_pPlayer->GetObjectKey();
       brd_message.m_wvCurPos	= server_position;
       brd_message.m_wAngle		= angle;
       brd_message.m_byMoveState= move_state;
       m_pPlayer->SendPacketAroundForSync(&brd_message);
   }
   else
   {
//        if (field->IsMovableCoordinates(&server_position))
//            m_pPlayer->SetPos(&server_position);
       return RC::RC_SYNC_SUCCESS;
    }

    apply_angle(angle);

    return RC::RC_SYNC_SUCCESS;
}

RC::eSYNC_RESULT MovementChecker::check_state_enable_move()
{
    if (m_pPlayer->IsAlive() == FALSE)
        return RC::RC_SYNC_PLAYER_IS_DEAD;

    // 이동 불가능할때 지연(Delay) 애니메이션 중인가?
    if (!m_pPlayer->CanMove())
    {
        StatusManager* status_manager = m_pPlayer->GetStatusManager();
        if (status_manager->AnimationDelay.NeedSkipProcessByAnimationDelay((BaseStatus*)NULL))
            return RC::RC_SYNC_ANIMATION_DELAY_PROCESSING;
        return RC::RC_SYNC_PLAYER_IS_UNMOVABLE_STATE;
    }

    // TODO : 이동 가능여부 상태 검증 추가.


    return RC::RC_SYNC_SUCCESS;
}

bool MovementChecker::is_same_position(WzVector& client_position)
{
    // 좌표값의 유효 한계 검증.
    if (!IsFiniteVector(client_position))
        return false;

    WzVector server_position;
    m_pPlayer->GetPos(&server_position);
    if (client_position != server_position)
    {
        MoveStateControl* move_state_control = m_pPlayer->GetMoveStateControl();
        // 현재 캐릭터가 낼수 있는 최대 속도의 10%증가된 속도로 0.4초의 이동 거리의 오차까지 허용한다.
        float move_speed = move_state_control->GetMoveSpeed();
        float limit_max_speed = move_state_control->GetLimitMaxSpeed();
        limit_max_speed = max(limit_max_speed, move_speed);
        float allowed_max_length = ((limit_max_speed + (limit_max_speed * 0.1f)) * 400.0f);
        float length = Math_Vector3DLength(&(server_position - client_position));
        if (length > allowed_max_length)
            return false;
    }
    return true;
}

void MovementChecker::apply_angle(ushort angle)
{
    angle_ = (angle) % 360;
}

bool MovementChecker::Update_Moving_Turn(uint delta_tick)
{
    if (!m_pPlayer->IsMoving())
        return false;

    if (!(KEYBOARDMOVE_ISMOVING & MOVEKEYBOARD_STATE))
        return false;

    // 클라이언트의 회전 속도는 2rad/s(= 114.59도/s)
    float turn_speed = 114.59f;
    float move_speed = m_pPlayer->GetMoveStateControl()->GetMoveSpeed();
    if (move_speed == 0.0f)
        return false;

    if (MOVEKEYBOARD_STATE & KEYBOARDMOVE_LEFTTURN)
    {
        turn_speed *= 1.0f;
    }
    else if (MOVEKEYBOARD_STATE & KEYBOARDMOVE_RIGHTTURN)
    {
        turn_speed *= -1.0f;
    }
    else
    {
        turn_speed = 0.0f;
    }

    angle_ += turn_speed * ((float)delta_tick/1000.0f);
    short total_angle = ((short)(angle_)) % 360;
    if (total_angle < 0)
        total_angle += 360;

    WzVector direction;
    KBMoveCommonDIR(direction, total_angle, FALSE, MOVEKEYBOARD_STATE);

    // 0.1초후의 위치를 목표 위치로 하며 증가될 이동량을 구한 후
    // FindPathThrust() 를 이용해 위치를 증가시킨다.
    WzVector server_position;
    m_pPlayer->GetPos(&server_position);

    const uint move_time = 100;
    direction = direction * (move_speed * move_time);
    if (!m_pPlayer->GetField()->FindPathThrust(m_pPlayer, &direction))
    {
//         if (m_pPlayer->GetField()->IsMovableCoordinates(&server_position))
//             m_pPlayer->SetPos(&server_position);
    }
    return true;
}


#endif //_NA_0_20110803_PLAYER_MOVEMENT_SYNC



