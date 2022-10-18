#include "StdAfx.h"

//������������������������������������������������������������������������������
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

//������������������������������������������������������������������������������
//
FLOAT			MovementChecker::sfSYNC_HACK_DISTANCE2 = .0f;
FLOAT			MovementChecker::sfSYNC_FAIL_DISTANCE2 = .0f;
FLOAT			MovementChecker::sfSYNC_FAIL_DISTANCE_ACCEPT2 = .0f;
FLOAT			MovementChecker::sfSYNC_WARN_DISTANCE2 = .0f;
DWORD			MovementChecker::sdwFRAME_DELTA_TICK = 0;
DWORD			MovementChecker::sdwFRAME_POLE_TICK = 0;
WzVector		MovementChecker::sWzVectorZero = { .0f, .0f, .0f };

//������������������������������������������������������������������������������
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
	// ������ Ŭ���� �÷��̾� ���� ��ġ��
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

//������������������������������������������������������������������������������
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
		// �� ���� Server�� ���� ������Ʈ �ȵǾ� �ְų� �ұԸ� �̵����� �ν��ϴ� ���
		// ���� ó�� ������ ���� ����+���� �Ÿ��� �˻����� �ʴ´�.
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

				if( fMaxDistRadius < fLen2Client )	// Ŭ���̵� ũ��...
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

//������������������������������������������������������������������������������
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

	//	���°� Ʋ������ ��쿡 �־� ���� ���� ��Ȳ�� ���� ���·� ������ ���
	//	���� �߻��� �� �ִ�.
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
									WzVector& OUT wvDirection		// ���, �̵� ���� ���� ����
									)
{
	const FLOAT cfMaxDistance = 10.0f;
	FLOAT fMovSpd = m_pPlayer->GetMoveStateControl()->GetMoveSpeed();

	FLOAT fDistance = (fMovSpd == .0f) ? cfMaxDistance : min( cfMaxDistance, (DELAY_DEFAULT_KBMOVE_SYN)*fMovSpd );
	wvDirection *= fDistance;

	// ���� ������ ��ǥ�� ��ȿ���� üũ
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
			{	// �ܹ��⿡�� ������ �ٲٰų� �� ��... �̶��� Timeout�� ������ �� �ֵ��� ����.
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
			// <WARNING> ���� Ʈ����� ���� ó���� �Ǿ�� �Ѵ�.
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
// �̵���Ŷ �������(CG_SYNC_MOVE_SYN, CG_SYNC_TARGET_MOVE_SYN)
RC::eSYNC_RESULT
MovementChecker::MoveCommon(
							BOOL				bIsTargetMove,
							eCHAR_MOVE_STATE	eState,		// CMS_RUN, ... �̵� ����
							WzVector&			wvCurPosR,	// ���� ��ǥ
							WzVector&			wvDestPosR,	// �̵��� ��ǥ
							INT					iNumRoute,	// PathRoute ����
							TILEINDEX*			piRoute,	// PathRoute �迭
							DWORD& OUT			rdwErrorLevel
							)
{
	static BOOL __bInit = PreCalcScript();

	// �÷��̾ �ִ� �ʵ� �����Ͱ� ��ȿ���� �˻�
	GameField* pField = m_pPlayer->GetField();
	if( !pField )
		return RC::RC_SYNC_FIELD_IS_NULL;

	// �÷��̾ ������� ������ ����
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
	// ���� ������ SUCTION ��ų ������̿����� STOP ��Ų��.
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

		// �̵��������� üũ (�ӽ�...�׳� return �ϸ� �ȵȴ�.) -- ���� �̰�?
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

		//wvCurPosR;	// �߲� �ڷ��� ����
		//wvDestPosR;	// �߲� �ڷ��� ����

		// ������ Ŭ���� �÷��̾� ���� ��ġ��

		// ���� ��ǥ ��ȿ�� �˻�
		bProcess = IsFiniteVector( wvCurPosR );
		if( !bProcess )
		{
			eRET = RC::RC_SYNC_CUR_POSITION_IS_INVALID;
			continue;
		}

		WzVector wvPosOfPlayer;
		m_pPlayer->GetPos( &wvPosOfPlayer );
		FLOAT fCSdistance2 = 0;

		const BOOL scbPROCESS = TRUE;	// �� ���� �����Ű�� �� ��.
		const FLOAT fPathEnginePrecision2 = (0.3f)*(0.3f);

		// ��ũ��Ʈ�� �ִ� ���Ѱ� boundary check
		if( scbPROCESS )
		{
			fCSdistance2 = Math_Vector2DLength2( &(wvPosOfPlayer - wvCurPosR) );

			// ��ŷ���� ����!!!

			// ��Ŷ�� �ִ� �̵� ���� ��ġ�� ���� ������ ���� ��ġ�� �߶׸°� �ٸ�
			bProcess = bProcess && (sfSYNC_HACK_DISTANCE2 > fCSdistance2);
			bProcess = bProcess && (sfSYNC_FAIL_DISTANCE2 > fCSdistance2);
			if( !bProcess )
			{
				eRET = RC::RC_SYNC_CUR_POSITION_IS_INVALID;
				continue;
			}

			// Path Engine ���� �� ������ ����...
			bProcess = bProcess && (sfSYNC_WARN_DISTANCE2 > fCSdistance2);
			if( !bProcess )
			{
				bProcess = TRUE;	// �Ÿ� ���� ���� �߻��ϱ� ������ ����ϴ� ��ġ
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// 3DEngine - Path���� ó��
		// PATHENGINE�� ����ϴ� ���, PathRoute ������ ����.
		// <THINKING>
		//	������ �ƴ� POS�� Ŭ�� ������ POS... � ���� ����ؾ� �ϴ°�?
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
			// �÷��̾� ��ġ�� Ŭ�� ������ ���� ���������� �ű��.
			bProcess = m_pPlayer->SetPath( &wvPosOfPlayer, &wvDestPosR, NULL, 0 );
#else
			// Ÿ���ε��� ��ȯ�۾�(short�迭 -> int�迭)
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

		// ���� ���� - �̰��� ��ġ��?
		rStatusManager.Move();
		m_pPlayer->SetMoveState( eState );

//////		// �̵��� üũ�� ���� ����
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

	// �÷��̾ �ִ� �ʵ� �����Ͱ� ��ȿ���� �˻�
	GameField* pField = m_pPlayer->GetField();
	if( !pField )
		return RC::RC_SYNC_FIELD_IS_NULL;

	// �÷��̾ ������� ������ ����
	if( ! m_pPlayer->IsAlive() )
		return RC::RC_SYNC_PLAYER_IS_DEAD;

	BOOL bProcess = TRUE;
	RC::eSYNC_RESULT eRET = RC::RC_SYNC_SUCCESS;

	StatusManager& rStatusManager = *m_pPlayer->GetStatusManager();

	while( bProcess )
	{
		// �̵��������� üũ (�ӽ�...�׳� return �ϸ� �ȵȴ�.) -- ���� �̰�?
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

		// ������ Ŭ���� �÷��̾� ���� ��ġ�� ���� �������� �˻�
		// �����ϴٸ� ���� �˻��� �ʿ� ����... ����? �ϴ� ���Ĵ� ��ǥ ���� ���̹Ƿ� ������ �̹� ������ �Ǿ��� ���̴�.
		// �̰��� TURN �迭�� ���� �͵� ���ǹ��� ���� ���ſ�
		WzVector wvPosOfPlayer;
		m_pPlayer->GetPos( &wvPosOfPlayer );

		if( wvPosOfPlayer == wvCurPosR )
		{
			bNeedMoveCalc = FALSE;
			return RC::RC_SYNC_SUCCESS;
		}

		// ���� ��ǥ ��ȿ�� �˻�
		bProcess = IsFiniteVector( wvCurPosR );
		if( !bProcess )
		{
			eRET = RC::RC_SYNC_CUR_POSITION_IS_INVALID;
			continue;
		}

		FLOAT fCSdistance2 = 0;

		const BOOL scbPROCESS = TRUE;	// �� ���� �����Ű�� �� ��.
		const FLOAT fPathEnginePrecision2 = (0.3f)*(0.3f);

		//////////////////////////////////////////////////////////////////////////
		// 3DEngine - Path���� ó��
		// PATHENGINE�� ����ϴ� ���, PathRoute ������ ����.
		// <THINKING>
		//	������ �ƴ� POS�� Ŭ�� ������ POS... � ���� ����ؾ� �ϴ°�?
		fCSdistance2 = Math_Vector2DLength2( &(wvPosOfPlayer - wvCurPosR) );
		BOOL bSamePosition = !(fCSdistance2 > fPathEnginePrecision2);
		if( !bSamePosition )
		{
			// ��ũ��Ʈ�� �ִ� ���Ѱ� boundary check

			// ��ŷ���� ����!!!

			// ��Ŷ�� �ִ� �̵� ���� ��ġ�� ���� ������ ���� ��ġ�� �߶׸°� �ٸ�
			bProcess = bProcess && (sfSYNC_HACK_DISTANCE2 > fCSdistance2);
			if( !bProcess )
			{
				eRET = RC::RC_SYNC_CUR_POSITION_IS_INVALID;
				continue;
			}

			// ����ϱ� ���� ����
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

			// �Ÿ� ���� ���� �߻��ϱ� ������ ����ϴ� ��ġ
			// �̴� Ű���� �̵��� ������ȯ���� ���� ���� �߻��� �� �ִ� ������.
			// ������ �н��������� ������ �߻��Ѵ�. ��...
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

		// -------- ���� �ҽ��� �����ϴ� ��ƾ -------- //
		// ����
		//	Ű���� �̵��� ���� �̵� ����� �� ������ �� �������� �����Ѵ�.
		//	���� �ڵ��� �������� Ŭ�� �����ִ� �ڵ�� �ٷ� �÷��̾� ��ġ�� �����Կ� �־���.
		//	{	m_pPlayer->SetPos( &wvCurPosition );	}
		// ���� ���
		//	���� ����� �������� �ʾҴ�. �̵� ó���� ���ؼ��� Ŭ��� �������� ������ ���� ���̸�,
		//	�ᱹ Ŭ���̾�Ʈ �����͸� ������� �ŷ��ؾ� �ϴ� �����̴�.
		//  ����, �ӽù������γ��� ���� �ֱ� ���� ���ŵǴ� ��� ����ϵ��� �Ѵ�.
		//	��, ������ 5����...-.-;;; ũ��. (�н����� ����� 0.3����...)

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

		rStatusManager.Move();	// ���� ���� - �̰��� ��ġ��? - ���ſʹ� �ٸ��� ���¿� ���� ����� ó���ȴ�. �̵� �����ÿ� ó���ϵ��� ����.

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

	WzVector& wvDirFB = wvTemp[DIR_FB];		// ������ ����
	WzVector& wvDirLR = wvTemp[DIR_LR];		// �¿� ����

	if( !bFastMove )
	{
		// ������ ������ ����
		if( KEYBOARDMOVE_FOWARD&byMoveState )
		{	// ����
			wvDirFB = wvDirection;
		}
		else if( KEYBOARDMOVE_BACKWARD&byMoveState )
		{	// ����
			wvDirFB = -wvDirection;
		}

		// �°԰���, ��԰��� ������ ����
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
		{	// ���� �����
			wvDirFB = wvDirection;
		}
	}

	// ������ ���Ϳ� �¿� ���͸� ���� ���͸� ��ֶ����� �� ���� ���� ����
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
	BOOL rbNeedMoveCalc = TRUE;	// �̵� ��� �ʿ� ���� ���

	RC::eSYNC_RESULT rcResult = KBMoveCommonCHECK( wvCurPosR, byMoveState, rbNeedMoveCalc, rdwErrorLevel );
	//printf( "NEED CALC %u\n", rbNeedMoveCalc );
	while( RC::RC_SYNC_SUCCESS == rcResult )
	{
		BOOL bProcess = TRUE;

        BOOL bContinue = TRUE;
		while (bContinue)
		{
			// �������̳� �԰����� ���� ��쿡�� �׳� ����!
			// (WAVERIX)(NOTICE)
			// �ٸ� ���� ���� LEFT_TURN, RIGHT_TURN�� ��� ���� ��� �Ǵ� BRD ó���� �ʿ� ����.
            if( !(KEYBOARDMOVE_ISMOVING&byMoveState) )
			{
				// �������̳� �԰����� ���� ��쿡�� �׳� ����!
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

			// ���� ������ ���Ѵ�.
			WzVector wvDestPosR;
			KBMoveCommonDIR( wvDestPosR, pRecvMsg->m_wAngle, FALSE, byMoveState );

			// 0.5�� ���� �̵��� �� �ִ� �ִ� �Ÿ��� ���Ѵ�.(Ŭ�󿡼� 0.5�� �������� �̵���Ŷ�� ������)
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
				// �̵��� üũ�� ���� ����
				if( !m_pPlayer->GetMovementChecker()->AddMovement( wvCurPosR ) )
				{
					SECURELOG( SECURELOG_PLAYERINFO(m_pPlayer, eABUSE_LOGIC_VIOLATION, eABUSE_SUB_LOGIC_VIOLATION_MOVE_SPEED),
						_T("[OnCG_SYNC_KBMOVE_SYN] AddMovement Fail!") );
				}
			}

			// ���� ����
			//	- ���� ���¿� ���� ����(���� ��)�� ������ �˻��ؼ� BRD���θ� �����ϸ� ���?
			// �ٸ� �÷��̾�鿡�� ��ε� ĳ����
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
    GameField* pField = m_pPlayer->GetField();	// �����Ѵٴ� �ǹ̴� Field �����Ѵٴ� �ǹ�
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
			// �̵����°� Ư���̵�(�����, ������) ���°� �´��� üũ
			BYTE byCMS_MoveState = pRecvMsg->m_byCmsMoveState;

			bProcess = !( pRecvMsg->m_byCmsMoveState < CMS_TUMBLING_FRONT || pRecvMsg->m_byCmsMoveState > CMS_MAX );
			if( !bProcess )
			{
				rcResult = RC::RC_SYNC_INVALID_MOVESATE;
				bContinue = FALSE;
				continue;
			}

			// ���� ������ ���Ѵ�.
			WzVector wvFinalDir;
			KBMoveCommonDIR( wvFinalDir, pRecvMsg->m_wAngle, TRUE, byCMS_MoveState );

			// �ӵ� ����
			m_pPlayer->SetMoveState( (eCHAR_MOVE_STATE)byCMS_MoveState );
			float fMoveSpeed = m_pPlayer->GetMoveStateControl()->GetMoveSpeed();

			// ������ ���� �������� �̵��Ÿ� ���ϱ�
			float fMoveDistance = GetMoveTimeAsState( pRecvMsg->m_byCmsMoveState ) * fMoveSpeed;
			wvFinalDir = wvFinalDir * fMoveDistance;

			// ���� ������ ��ǥ�� ��ȿ���� üũ
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

			// �̵��� üũ�� ���� ����
			if( !m_pPlayer->GetMovementChecker()->AddMovement( pRecvMsg->m_wvCurPos ) )
			{
				SUNLOG( eCRITICAL_LOG, "[OnCG_SYNC_FAST_KBMOVE_SYN] AddMovement Fail!" );
			}

			// �ٸ� �÷��̾�鿡�� ��ε� ĳ����
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
	// �÷��̾ �ִ� �ʵ� �����Ͱ� ��ȿ���� �˻�
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

	// ���� ��ǥ�� ��ȿ���� üũ
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

		// �̵��� üũ�� ���� ����
		if( !m_pPlayer->GetMovementChecker()->AddMovement( pRecvMsg->wvCurPos ) )
		{
			SUNLOG( eCRITICAL_LOG, "[OnCG_SYNC_STOP_SYN] AddMovement Fail!" );
		}

		// ���� �ϴ� ���� ��ε带 ������ ���� NAK ó���� �׽�Ʈ�غ���
		// ������ �� ���� ���̴�. �Ϲ�������... �̰� ������ ���� �����͸� Ŭ�����ͷ� �����Ű�� ���� �� ������
		m_pPlayer->SetPos( &wvChangePos );

		// �ٸ� �÷��̾�鿡�� ��ε� ĳ����
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
		// ��� ����� Player ���� ��ġ
		WzVector wvPosOfPlayer;
		m_pPlayer->GetPos( &wvPosOfPlayer );

		WzVector wvPosDest = pRecvMsg->m_wvDestPos;
        // CHANGES: f110321.2L
        FiltersForMovableCoordinates coord_check_filter = { 0, };
        // ��ȿ�� ��ġ�ΰ� üũ
        bProcess = pField->IsMovableCoordinates(&wvPosDest, &coord_check_filter);
		if( !bProcess )
		{
			eRET = RC::RC_SYNC_DEST_POSITION_IS_INVALID;
			continue;
		}

		// ������ �� �� �ִ� �ִ� �̵��Ÿ� üũ
		const DWORD cdwMaxDistance = (DWORD)(MAX_JUMP_DISTANCE * MAX_JUMP_DISTANCE);
		DWORD dwCSdistance2 = (DWORD)Math_Vector2DLength2( &(wvPosOfPlayer - wvPosDest) );
		// + : �Ʒ���, - : ����
		FLOAT fHeight = wvPosOfPlayer.z - wvPosDest.z;

		// �Ÿ��� ���Ѱ� �̻��̰ų� 4���ͺ��� ���� �ٸ� ���� ó��
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
		m_pPlayer->SetMoveState( CMS_RUN );			// �������� �߰� �ʿ�!
        
        eRET = pField->JumpObject(m_pPlayer, &wvPosDest);
		if (eRET != RC::RC_SYNC_SUCCESS)
        {
            // (CHANGE)(20100412)(hukim) 
            // ���� ������ ������ ���� �� �� �ִµ��� ���� ���ο� ������� ���� �ڵ�� ��� �����Ѵ�.
            // ���� ��ġ�� ���ŵ��� �ʾҴµ� Ŭ���̾�Ʈ���� ��ε�ĳ���� �����ν� ���� ��ġ���� 
            // �߻��� ���ɼ��� �ִ�. ���� ���� ���θ� �˻��ϵ��� �����Ѵ�.
            // ���� ������ ��ó�� continue ó���ϸ� ���� ������ ���� ���ɼ��� �ִ�.
            // �ϴ��� Ư���� �������� ���� ������ �����ϰ� ��ü���� ���� �����Ŀ� �����Ѵ�.

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

		// JUMP_DAMAGE_HEIGHT(8)���� �ʰ� ���̺��� 1m�� 60������
		fHeight = (JUMP_DAMAGE_HEIGHT < fHeight) ? fHeight-JUMP_DAMAGE_HEIGHT : .0f;
		// �������� ���ų� �پ���� ���� 8���� �̸��϶�
		if( fHeight != .0f )
		{
			DAMAGETYPE dtDamage = (DAMAGETYPE)(fHeight * JUMP_METER_PER_DAMAGE);
			DWORD ResultDamage = m_pPlayer->DecreaseHP( dtDamage );
			SUNLOG( eDEV_LOG, "[MovementChecker::OnRecvCG_SYNC_JUMP_SYN] Jump Damage = %d", dtDamage );

			// ������ �׾����� ���� ���� & �ٱ� �� ��ġ ����
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

		// �̵��� üũ�� ���� ����
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
		msg.m_byState		= CMS_RUN;	//ĳ������ ���� ������ �ٱ⸸ ����!!(���Ϳ� ���� ���� ��Ŷ�̹Ƿ� ���� �־���)
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
		msg.m_wPlayerKey	= (WORD)m_pPlayer->GetObjectKey();//�÷��̾�Ű�� ����� WORD������ ���� �ʱ⶧��!!(��Ŷ ����ȭ�� ���� �ٿ��� ����)
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
		case RC::RC_SYNC_PLAYER_IS_DEAD:			// �÷��̾ �׾ �̵�����
		case RC::RC_SYNC_FIELD_IS_NULL:				// �÷��̾��� �ʵ尪�� NULL�̴�.
		case RC::RC_SYNC_PLAYER_IS_UNMOVABLE_STATE:	// �÷��̾ �̵��� �� ���� �����̴�.
		case RC::RC_SYNC_FAILED:
		case RC::RC_SYNC_SETPATH_ERROR:
		case RC::RC_SYNC_ANIMATION_DELAY_PROCESSING:
			break;
		//	Abusing Notification List
		case RC::RC_SYNC_ROUTE_SIZE_EXCESS:			// ��� ���� �ִ밪 �ʰ�
			pAbusingMsg = _T("[MOVE] Cracked Packet Route Size Excess! %f");
			eTYPE = eABUSE_SUB_LOGIC_VIOLATION_MOVE_CRACKED_PACKET;
			break;
		case RC::RC_SYNC_INVALID_MOVESATE:			// �̵����� �̻�(�ȱ�: �޸���)
			pAbusingMsg = _T("[MOVE] Cracked Packet Invalid MoveState! %f");
			eTYPE = eABUSE_SUB_LOGIC_VIOLATION_MOVE_CRACKED_PACKET;
			break;
		case RC::RC_SYNC_CUR_POSITION_IS_INVALID:	// Ŭ�� ������ ������ǥ�� ��ȿ���� �ʴ�.
		case RC::RC_SYNC_DEST_POSITION_IS_INVALID:	// ������ ��ǥ�� ��ȿ���� �ʴ�.
			pAbusingMsg = _T("[MOVE] Irregular Coordinates! %f");
			eTYPE = eABUSE_SUB_LOGIC_VIOLATION_INVALID_POSITION;
			break;
		case RC::RC_SYNC_JUMP_LENGTH_TOO_LONG:		// �����Ÿ��� �ʹ� �ִ�
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

    // Ŭ���̾�Ʈ�� ��ǥ�� ��ȿ�� ��ǥ���� �����Ѵ�.
    // ��ǥ�� Ʋ�� ��� ���� ĳ������ ��ġ�� ���� �����Ѵ�.
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

    // ĳ������ ���� ���°� �̵� �������� �����Ѵ�. 
    RC::eSYNC_RESULT result = check_state_enable_move();
    if (result != RC::RC_SYNC_SUCCESS)
    {
        send_move_fail(result);
        return;
    }

    // Ŭ���̾�Ʈ�� ��ǥ�� ��ȿ�� ��ǥ���� �����Ѵ�.
    if (!is_same_position(recv_message->wvCurPos))
    {
        send_move_fail(RC::RC_SYNC_CUR_POSITION_IS_INVALID);
        return;
    }

    // ���� �̵� ����� ������.
    result = do_run(recv_message->m_wAngle, recv_message->m_byMoveState);
    if (result != RC::RC_SYNC_SUCCESS)
    {
        send_move_fail(result);
        return;
    }

    // (NOTE) to support a shadow control, __NA_S00015_20080828_NEW_HOPE_SHADOW
    // ���� ������ SUCTION ��ų ������̿����� STOP ��Ų��.
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

    // ĳ������ ���� ���°� �̵� �������� �����Ѵ�. 
    RC::eSYNC_RESULT result = check_state_enable_move();
    if (result != RC::RC_SYNC_SUCCESS)
    {
        send_move_fail(result);
        return;
    }

    // Ŭ���̾�Ʈ�� ��ǥ�� ��ȿ�� ��ǥ���� �����Ѵ�.
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

    // �÷��̾ ������Ų��.
    m_pPlayer->StopMoving();
    m_pPlayer->SetMoveState(CMS_STOP);

    switch(result)
    {
    case RC::RC_SYNC_SUCCESS:
    case RC::RC_SYNC_PLAYER_IS_DEAD:                    // �÷��̾ �׾ �̵�����
    case RC::RC_SYNC_FIELD_IS_NULL:                     // �÷��̾��� �ʵ尪�� NULL�̴�.
    case RC::RC_SYNC_PLAYER_IS_UNMOVABLE_STATE:         // �÷��̾ �̵��� �� ���� �����̴�.
    case RC::RC_SYNC_SETPATH_ERROR:
    case RC::RC_SYNC_ROUTE_SIZE_EXCESS:                 // ��� ���� �ִ밪 �ʰ�
    case RC::RC_SYNC_INVALID_MOVESATE:                  // �̵����� �̻�(�ȱ�: �޸���)
    case RC::RC_SYNC_CUR_POSITION_IS_INVALID:           // Ŭ�� ������ ������ǥ�� ��ȿ���� �ʴ�.
    case RC::RC_SYNC_DEST_POSITION_IS_INVALID:          // ������ ��ǥ�� ��ȿ���� �ʴ�.
    case RC::RC_SYNC_JUMP_LENGTH_TOO_LONG:              // �����Ÿ��� �ʹ� �ִ�
    case RC::RC_SYNC_CUR_POSITION_IS_INVALID_NO_HACK:   // Ŭ�� ������ ������ǥ�� ��ȿ���� �ʴ�. ���� ��輱���� Ŭ�� ���� Ʋ�� ��ǥ�� ���� �� �ִ�.
        {
            // �ֺ��� ������ �˸���.
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


    // �������̳� �԰����� ���� ��쿡�� �׳� ����!
    // �ٸ� ���� ���� LEFT_TURN, RIGHT_TURN�� ��� ���� ��� �Ǵ� BRD ó���� �ʿ� ����.
    if (!(KEYBOARDMOVE_ISMOVING & move_state))
    {
        // �������̳� �԰����� ���� ��쿡�� �׳� ����!
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

    // ���� ������ ���Ѵ�.
    WzVector direction;
    KBMoveCommonDIR(direction, angle, FALSE, move_state);


    eCHAR_MOVE_STATE character_move_state =
        KEYBOARDMOVE_FOWARD&move_state ? CMS_RUN
        : KEYBOARDMOVE_BACKWARD&move_state ? CMS_BACKSTEP
        : CMS_SIDESTEP;

    m_pPlayer->SetMoveState(character_move_state);


    // 0.1������ ��ġ�� ��ǥ ��ġ�� �ϸ� ������ �̵����� ���� ��
    // FindPathThrust() �� �̿��� ��ġ�� ������Ų��.
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

    // �̵� �Ұ����Ҷ� ����(Delay) �ִϸ��̼� ���ΰ�?
    if (!m_pPlayer->CanMove())
    {
        StatusManager* status_manager = m_pPlayer->GetStatusManager();
        if (status_manager->AnimationDelay.NeedSkipProcessByAnimationDelay((BaseStatus*)NULL))
            return RC::RC_SYNC_ANIMATION_DELAY_PROCESSING;
        return RC::RC_SYNC_PLAYER_IS_UNMOVABLE_STATE;
    }

    // TODO : �̵� ���ɿ��� ���� ���� �߰�.


    return RC::RC_SYNC_SUCCESS;
}

bool MovementChecker::is_same_position(WzVector& client_position)
{
    // ��ǥ���� ��ȿ �Ѱ� ����.
    if (!IsFiniteVector(client_position))
        return false;

    WzVector server_position;
    m_pPlayer->GetPos(&server_position);
    if (client_position != server_position)
    {
        MoveStateControl* move_state_control = m_pPlayer->GetMoveStateControl();
        // ���� ĳ���Ͱ� ���� �ִ� �ִ� �ӵ��� 10%������ �ӵ��� 0.4���� �̵� �Ÿ��� �������� ����Ѵ�.
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

    // Ŭ���̾�Ʈ�� ȸ�� �ӵ��� 2rad/s(= 114.59��/s)
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

    // 0.1������ ��ġ�� ��ǥ ��ġ�� �ϸ� ������ �̵����� ���� ��
    // FindPathThrust() �� �̿��� ��ġ�� ������Ų��.
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



