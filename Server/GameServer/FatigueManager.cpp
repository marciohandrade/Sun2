#include "StdAfx.h"
#include ".\fatiguemanager.h"
#include "FatigueOptionParser.h"
#include "Player.h"
#include "GameZone.h"


FatigueManager::FatigueManager(void)
{
	m_pPlayer = NULL;
	m_pCurFatigueType = NULL;
	m_nSecPlayTime = 0;
	m_CurDay = 0;
	m_Init = FALSE;
	m_CurTickCount = 0;
	m_PreTickCount = 0;
	m_bApplyFatigue = FALSE;
}

FatigueManager::~FatigueManager(void)
{

}

//nPlayTime : 초단위의 현재 플레이 시간.
VOID	FatigueManager::SetPlayTime( int nSecPlayTime )
{
	m_nSecPlayTime = nSecPlayTime;
	SetDBPlayTime( m_nSecPlayTime/60 );		//분단위로 저장한다.
}

//nPlayTime : 분단위의 현재 플레이 시간.
VOID	FatigueManager::AddPlayTime( int nSecPlayTime )
{
	m_nSecPlayTime += nSecPlayTime;
	SetDBPlayTime( m_nSecPlayTime/60 );		//분단위로 저장한다.
}

BYTE	FatigueManager::GetCurFatigueTypeNum()
{
	if( !m_pCurFatigueType )
		return 0;

	return m_pCurFatigueType->m_byType;
}

BOOL	FatigueManager::IsMsgTimeExpired()
{
	//현재 메세지를 보내는데, 현재 남은 메세지 타임이 0보다 작거나 같다면..
	//메세지 간격이 0이면, 메세지를 보내지 않는다.
	if( (m_pCurFatigueType->m_wMsgInterval > 0) && (m_nSecRemainMsgTime <= 0) )
		return TRUE;

	return FALSE;
}

BOOL	FatigueManager::CheckAuth()
{
	if( !m_pPlayer )		return FALSE;

#ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
	//중국에서는 18세 이상의 실명인증한 유저는 피로도를 적용시키지 않는다. 
	if( (m_pPlayer->GetAge() >= 18) && (m_pPlayer->GetRealNameAuth() == 1) )	
		return FALSE;
#endif

	return TRUE;
}

// nPlayTime : 현재 플레이어의 플레이 시간(분단위)
void	FatigueManager::Init( Player * pPlayer )
{
	ASSERT( pPlayer );
	m_pPlayer = pPlayer;
	SetInit( FALSE );
	m_nSecRemainMsgTime = 0;
	int nMinPlayTime = 0;

	BASE_PLAYERINFO* pInfo = NULL;
	if( !m_pPlayer || !( pInfo = m_pPlayer->GetCharInfo() ) )
	{
		SUNLOG( eCRITICAL_LOG, "현재 플레이어의 정보가 없습니다." );
		return;
	}

#ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
	nMinPlayTime = m_pPlayer->GetPlayTime();
#else
	nMinPlayTime = pInfo->m_PlayLimitedTime;
#endif

	if( nMinPlayTime < 0 )
	{
		SUNLOG( eCRITICAL_LOG, "현재 플레이어의 플레이 타임이 0보다 작습니다. Time = %d", nMinPlayTime );
		return;
	}

	if( FatigueOptionParser::Instance()->IsUse() )
	{
		m_PlayingTimer.SetTimer( FatigueOptionParser::Instance()->GetCheckCycle() * 1000 * 60 );
		SetApplyFatigue( TRUE );			//피로도 사용.
	}
	else
	{
		SetApplyFatigue( FALSE );			//피로도 사용안함.
		m_PlayingTimer.DisableCheckTime();
		return;
	}

	SetPlayTime( nMinPlayTime * 60 );		// DB에 저장되어 있는 플레이어의 플레이 누적시간를 초단위로 변경하여 저장.
	SetPreTickCount( GetTickCount() );		// 이전 틱카운트를 현재틱카운트로 설정한다.

	// 이구문은 12시에 초기화 될때 나올수 있는 최대치를 체크하는 구문이다. 중국에서는 사용되지 않는다.
#ifndef __CN_OO0510__FATIGUE_MATCHING_UPDATE
	SYSTEMTIME sysTime;				util::TimeSync::GetLocalTime( &sysTime );
	SetCurDay( sysTime.wDay );				//현재 날짜로 세팅한다.
	if( !CheckMaxPlayTime( sysTime ) )
		SetPlayTime(0);
#endif //__CN_OO0510__FATIGUE_MATCHING_UPDATE

	//현재 플레이어의 누적 플레이 시간에 맞는 메세지 간격을 구한다. 그리고 해당 간격을 세팅한다.
	_FATIGUE_TYPE* pFatigueType = FatigueOptionParser::Instance()->GetFatigueTypeFromTime( GetMinutePlayTime() );
	if( !pFatigueType )		
	{
		SUNLOG( eCRITICAL_LOG, "현재 플레이어의 플레이 시간이 피로도타입에 존재하지 않습니다.[%s], Time = %d", m_pPlayer->GetCharName(), GetMinutePlayTime() );
		return;
	}
	else
	{
		m_pCurFatigueType = pFatigueType;

		//현재 초기화 단계 이므로 DB에서 읽어온 값에서 남은 값을 빼준다.( 단위 : 분단위 )
		int nMinMsgRemainTime = 0;
		if( nMinPlayTime == 0 )
		{
			nMinMsgRemainTime = m_pCurFatigueType->m_wMsgInterval;
		}
		else
		{
			// 공식
			// 남은 메세지 시간 = 메세지 간격 - { (현재 누적 플레이 시간 - 현재 피로도 구간의 시작 시간) / 현재 피로도구간의 메세지 간격 }의 나머지
			int nMod = ( nMinPlayTime - m_pCurFatigueType->m_nStartTime ) % m_pCurFatigueType->m_wMsgInterval;
			nMinMsgRemainTime = m_pCurFatigueType->m_wMsgInterval - nMod;
		}

		if( nMinMsgRemainTime <= 0 )
		{
			SUNLOG( eCRITICAL_LOG, "이상한 메세지 간격 이다. [%s], MsgTime = %d", m_pPlayer->GetCharName(), GetMinutePlayTime() );
		}

		m_nSecRemainMsgTime = nMinMsgRemainTime*60; //현재 메세지구간에서 남은 메세지시간 만큼을 설정한다.(단위 : 초)
	}

	SetInit( TRUE );
}

VOID	FatigueManager::Update()
{
	//사용 하지 않거나, 초기화 되지 않은 상태로 여기 들어오면 리턴한다.
	if( !IsApplyFatigue() || !GetInit() )		
		return;	

	// 해당 체크 사이클이 지났으면..
	if( m_PlayingTimer.IsExpired() )
	{
		InitTickCount();

		if( FatigueOptionParser::Instance()->GetInitType() == FATIGUE_INIT_MIDNIGHT )		//자정에 초기화 하는 것이라면.
		{
			SYSTEMTIME sysTime;		
			util::TimeSync::GetLocalTime( &sysTime );

			if( sysTime.wDay != GetCurDay() )		//현재 날짜와 저장된 날짜가 틀리면(하루가 지났다면..)
			{
				SetPlayTime(0);
				SetCurDay( sysTime.wDay );
				SetPreTickCount( GetCurTickCount() );
				SendTimeMessage();
				return;
			}
		}
		else if( FatigueOptionParser::Instance()->GetInitType() == FATIGUE_INIT_OFFLINE )
		{
			// 자정이 아니면, 중국과 같은 오프라인 시간이다. 이것은 DB에서 처리해준다.
		}
		else
		{
			SUNLOG( eCRITICAL_LOG, "이상한 초기화 타입이다. [%s] Type = %d", m_pPlayer->GetCharName(), FatigueOptionParser::Instance()->GetInitType() );
		}

		//피로도를 적용시켜도 되는가?
		if( CanApplyFatigue() )
		{
			// 단위는 초이다. 현재 플레이 시간을 초단위로 TickCount로 직접 계산한다. 따라서 직접 시간을 계산해 준다.
			int nSecDiffTick = (int)(GetDiffTickCount() / 1000); 
			AddPlayTime( nSecDiffTick );
			m_nSecRemainMsgTime -= nSecDiffTick;		//현재 남은 메세지시간을 피로시간 시간만큼 빼준다.
		}

		SetPreTickCount( GetCurTickCount() );
	}

	if( IsMsgTimeExpired() )		//메세지 시간이 지났으면..
	{
		m_nSecRemainMsgTime = m_pCurFatigueType->m_wMsgInterval*60;    //새로운 메세지 간격으로 설정한다.
		SendTimeMessage();
	}

	//현재 피로도구간의 종료 시간보다 현재 플레이 시간이 같거나 크다면.. 피로도 구간을 바꿔야 할 시점이다.
	//현재 피로도 끝시간이 -1이면 그 피로도 구간이 마지막 이므로 더이상 바꾸지 않는다.
	if( (m_pCurFatigueType->m_nEndTime != -1) && (m_pCurFatigueType->m_nEndTime <= GetMinutePlayTime()) )
		ChangeFatigueType();		//구간이 바뀌면 바뀐정보를 세팅한다.
}

BOOL	FatigueManager::ChangeFatigueType()
{
	// 현재 플레이 타임으로 새로운 메세지 타입을 비교한다.
	_FATIGUE_TYPE* pFatigueType = FatigueOptionParser::Instance()->GetFatigueTypeFromTime( GetMinutePlayTime() );
	if( pFatigueType && m_pCurFatigueType )								
	{
		if( pFatigueType->m_byType != m_pCurFatigueType->m_byType )		//현재 다른 피로도타입이면
		{
			m_pCurFatigueType = pFatigueType;
			m_nSecRemainMsgTime = m_pCurFatigueType->m_wMsgInterval*60;    //새로운 메세지 간격으로 설정한다.
		}

		return TRUE;
	}
	else
	{
		SUNLOG( eCRITICAL_LOG, "현재 플레이누적시간에 맞는 피로도 타입이 존재하지 않습니다.[%s] Type=%d", m_pPlayer->GetCharName(), pFatigueType->m_byType );
	}

	return FALSE;
}

VOID	FatigueManager::InitTickCount()
{
	SetCurTickCount( GetTickCount() );
}

BOOL	FatigueManager::CheckMaxPlayTime( SYSTEMTIME& sysTime )
{
	//현재시간에 나올수 있는 최대 플레이 시간을 구한다.(단위 : 초)
	DWORD dwMaxTime = sysTime.wHour*60*60 + sysTime.wMinute*60 + sysTime.wSecond;

	if( dwMaxTime < (DWORD)GetPlayTime() )		//현재 플레이 시간이, 나올수 있는 플레이 시간보다 크다면.. 잘못된거다. 0으로 초기화한다.
	{
		SUNLOG( eFULL_LOG, "플레이누적시간이이상한유저(단위:초)[%s]CurTime=%u<Time=%u", m_pPlayer->GetCharName(), dwMaxTime, GetPlayTime() );
		return FALSE;
	}

	return TRUE;
}

//nPlayTime : 플레이 시간( 분단위 )
BOOL	FatigueManager::SetDBPlayTime( int nPlayTime )
{

#ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
	if( !m_pPlayer )
	{
		SUNLOG( eCRITICAL_LOG, "피로도를 적용할 사용자 정보가 없습니다." );
		return FALSE;
	}

	m_pPlayer->SetPlayTime( nPlayTime );
	// Offline 시간은 변하지 않는다.
#else
	BASE_PLAYERINFO* pInfo = NULL;
	if( !m_pPlayer || !( pInfo = m_pPlayer->GetCharInfo() ) )
	{
		SUNLOG( eCRITICAL_LOG, "피로도를 적용할 기본 사용자 정보가 없습니다." );
		return FALSE;
	}

	pInfo->m_PlayLimitedTime = nPlayTime;
#endif

	return TRUE;
}


//현재 피로시간을 적용할 수 있는가?
BOOL FatigueManager::CanApplyFatigue()
{
	if( !m_pPlayer )		return FALSE;

	FatigueOptionParser* const pFatigueOptionParser = FatigueOptionParser::Instance();

	if( !pFatigueOptionParser->ApplyInSSQField() )
	{
		sPOLICY_CHARACTER& rPolicyment = m_pPlayer->Policyment;
		if( rPolicyment.FIELD & rPolicyment.PCC_IN_SSQ_FIELD_PLAYER )
			return FALSE;
	}

	// 마을에서 피로도가 쌓이지 않게 설정되어 있는데, 현재 마을이면 적용시키지 않는다.
	if( !pFatigueOptionParser->ApplyVillageTime() && m_pPlayer->GetGameZoneState() == ePRS_AT_VILLAGE )
		return FALSE;

	// 로비에서 피로도가 쌓이지 않게 설정되어 있는데, 현재 로비이면 적용시키지 않는다.
	if( !pFatigueOptionParser->ApplyLobbyTime() && m_pPlayer->GetGameZoneState() == ePRS_AT_LOBBY )
		return FALSE;

	// 슈퍼방장은 피로도가 쌓이지 않게 설정되어 있는데, 현재 슈퍼방장이면 적용시키지 않는다
	if( !pFatigueOptionParser->ApplySuperMaster() )
	{
		GameZone *pZone = m_pPlayer->GetGameZonePtr();
		if( pZone && pZone->CanApplySuperMasterBenefit( m_pPlayer ) )
			return FALSE;
	}

	// 죽었을시에 피로도가 쌓이지 않게 설정되어 있는데, 현재 죽었으면 적용시키지 않는다.
	if( !pFatigueOptionParser->ApplyDeadTime() && m_pPlayer->GetHP() == 0 )
		return FALSE;

	return TRUE;
}

// 플레이 시간에 따른 메세지 출력.
void	FatigueManager::SendTimeMessage( )
{
	if( !m_pPlayer )		return;

	MSG_CG_STATUS_FATIGUE_CHANGE_CMD msg;
	msg.m_wPlayingTime = GetMinutePlayTime();
	m_pPlayer->SendPacket( &msg, sizeof(MSG_CG_STATUS_FATIGUE_CHANGE_CMD) );
}

// 피로도에 따른 아이템 드롭률
BYTE	FatigueManager::GetItemDropRatio( )
{
	BYTE byDropRatio = 100;

	if( !IsApplyFatigue() )		return byDropRatio;

	_FATIGUE_TYPE* pFatigueType = GetCurFatigueType();
	if( pFatigueType )
		byDropRatio = pFatigueType->m_byItemDropRatio;

	return byDropRatio;
}

// 피로도에 따른 국가별 경험치 획득률
BYTE	FatigueManager::GetExpRatio( )
{
	BYTE byExpRatio = 100;

	if( !IsApplyFatigue() )			return byExpRatio;

	_FATIGUE_TYPE* pFatigueType = GetCurFatigueType();
	if( pFatigueType )
		byExpRatio = pFatigueType->m_byExpRatio;

	return byExpRatio;
}

// 피로도에 따른 국가별 아이템을 줍을수 있는 비율
BOOL	FatigueManager::GetItemPickUp()
{
	BOOL bItemPickUp = TRUE;

	if( !IsApplyFatigue() )			return bItemPickUp;

	_FATIGUE_TYPE* pFatigueType = GetCurFatigueType();
	if( pFatigueType )
		bItemPickUp = pFatigueType->m_bItemPickUp;

	return bItemPickUp;
}

// 피로도에 따른 국가별 파티원을 맺었을때 공유되는 아이템 얻을 확률
BYTE	FatigueManager::GetShareItemPickupRatio()
{
	BYTE byPickupRatio = 100;

	if( !IsApplyFatigue() )			return byPickupRatio;

	_FATIGUE_TYPE* pFatigueType = GetCurFatigueType();
	if( pFatigueType )
		byPickupRatio = pFatigueType->m_bySharedItemPickupRatio;

	return byPickupRatio;
}

// 피로도에 따른 국가별 파티원을 맺었을때 공유되는 아이템 얻을 확률
BYTE	FatigueManager::GetShareHeimPickupRatio()
{
	BYTE byPickupRatio = 100;

	if( !IsApplyFatigue() )			return byPickupRatio;

	_FATIGUE_TYPE* pFatigueType = GetCurFatigueType();
	if( pFatigueType )
		byPickupRatio = pFatigueType->m_bySharedHeimPickupRatio;

	return byPickupRatio;
}

BOOL	FatigueManager::IsApplyFatigue()
{
	if( !m_bApplyFatigue )		return FALSE;
	if( !CheckAuth() )			return FALSE;

	return TRUE;
}

_FATIGUE_TYPE*	FatigueManager::GetCurFatigueType()
{
    const BYTE fatigueTypeNum = GetCurFatigueTypeNum();
	_FATIGUE_TYPE* pFatigueType = 
        FatigueOptionParser::Instance()->GetFatigueType( fatigueTypeNum );
	
    if( !pFatigueType )
    {
		SUNLOG( eDEV_LOG, "[FatigueManager::GetCurFatigueType] Type = %d, "
                          "해당 시간(%u)에 설정된 피로도정보가 없습니다.", 
                           fatigueTypeNum, GetMinutePlayTime() );
    }
	return pFatigueType;
}

BOOL	FatigueManager::GetItemReward()
{
	BOOL bItemReward = TRUE;

	if( !IsApplyFatigue() )			return bItemReward;

	_FATIGUE_TYPE* pFatigueType = GetCurFatigueType();
	if( pFatigueType )
		bItemReward = pFatigueType->m_bItemReward;

	return bItemReward;

}

BYTE	FatigueManager::GetHeimRewardRatio()
{
	BYTE byHeimRatio = 100;

	if( !IsApplyFatigue() )			return byHeimRatio;

	_FATIGUE_TYPE* pFatigueType = GetCurFatigueType();
	if( pFatigueType )
		byHeimRatio = pFatigueType->m_byHeimRewardRatio;

	return byHeimRatio;
}

BYTE	FatigueManager::GetExpRewardRatio()
{
	BYTE	byExpRatio = 100;

	if( !IsApplyFatigue() )			return byExpRatio;

	_FATIGUE_TYPE* pFatigueType = GetCurFatigueType();
	if( pFatigueType )
		byExpRatio = pFatigueType->m_byExpRewardRatio;

	return byExpRatio;
}

