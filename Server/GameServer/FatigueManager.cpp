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

//nPlayTime : �ʴ����� ���� �÷��� �ð�.
VOID	FatigueManager::SetPlayTime( int nSecPlayTime )
{
	m_nSecPlayTime = nSecPlayTime;
	SetDBPlayTime( m_nSecPlayTime/60 );		//�д����� �����Ѵ�.
}

//nPlayTime : �д����� ���� �÷��� �ð�.
VOID	FatigueManager::AddPlayTime( int nSecPlayTime )
{
	m_nSecPlayTime += nSecPlayTime;
	SetDBPlayTime( m_nSecPlayTime/60 );		//�д����� �����Ѵ�.
}

BYTE	FatigueManager::GetCurFatigueTypeNum()
{
	if( !m_pCurFatigueType )
		return 0;

	return m_pCurFatigueType->m_byType;
}

BOOL	FatigueManager::IsMsgTimeExpired()
{
	//���� �޼����� �����µ�, ���� ���� �޼��� Ÿ���� 0���� �۰ų� ���ٸ�..
	//�޼��� ������ 0�̸�, �޼����� ������ �ʴ´�.
	if( (m_pCurFatigueType->m_wMsgInterval > 0) && (m_nSecRemainMsgTime <= 0) )
		return TRUE;

	return FALSE;
}

BOOL	FatigueManager::CheckAuth()
{
	if( !m_pPlayer )		return FALSE;

#ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
	//�߱������� 18�� �̻��� �Ǹ������� ������ �Ƿε��� �����Ű�� �ʴ´�. 
	if( (m_pPlayer->GetAge() >= 18) && (m_pPlayer->GetRealNameAuth() == 1) )	
		return FALSE;
#endif

	return TRUE;
}

// nPlayTime : ���� �÷��̾��� �÷��� �ð�(�д���)
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
		SUNLOG( eCRITICAL_LOG, "���� �÷��̾��� ������ �����ϴ�." );
		return;
	}

#ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
	nMinPlayTime = m_pPlayer->GetPlayTime();
#else
	nMinPlayTime = pInfo->m_PlayLimitedTime;
#endif

	if( nMinPlayTime < 0 )
	{
		SUNLOG( eCRITICAL_LOG, "���� �÷��̾��� �÷��� Ÿ���� 0���� �۽��ϴ�. Time = %d", nMinPlayTime );
		return;
	}

	if( FatigueOptionParser::Instance()->IsUse() )
	{
		m_PlayingTimer.SetTimer( FatigueOptionParser::Instance()->GetCheckCycle() * 1000 * 60 );
		SetApplyFatigue( TRUE );			//�Ƿε� ���.
	}
	else
	{
		SetApplyFatigue( FALSE );			//�Ƿε� ������.
		m_PlayingTimer.DisableCheckTime();
		return;
	}

	SetPlayTime( nMinPlayTime * 60 );		// DB�� ����Ǿ� �ִ� �÷��̾��� �÷��� �����ð��� �ʴ����� �����Ͽ� ����.
	SetPreTickCount( GetTickCount() );		// ���� ƽī��Ʈ�� ����ƽī��Ʈ�� �����Ѵ�.

	// �̱����� 12�ÿ� �ʱ�ȭ �ɶ� ���ü� �ִ� �ִ�ġ�� üũ�ϴ� �����̴�. �߱������� ������ �ʴ´�.
#ifndef __CN_OO0510__FATIGUE_MATCHING_UPDATE
	SYSTEMTIME sysTime;				util::TimeSync::GetLocalTime( &sysTime );
	SetCurDay( sysTime.wDay );				//���� ��¥�� �����Ѵ�.
	if( !CheckMaxPlayTime( sysTime ) )
		SetPlayTime(0);
#endif //__CN_OO0510__FATIGUE_MATCHING_UPDATE

	//���� �÷��̾��� ���� �÷��� �ð��� �´� �޼��� ������ ���Ѵ�. �׸��� �ش� ������ �����Ѵ�.
	_FATIGUE_TYPE* pFatigueType = FatigueOptionParser::Instance()->GetFatigueTypeFromTime( GetMinutePlayTime() );
	if( !pFatigueType )		
	{
		SUNLOG( eCRITICAL_LOG, "���� �÷��̾��� �÷��� �ð��� �Ƿε�Ÿ�Կ� �������� �ʽ��ϴ�.[%s], Time = %d", m_pPlayer->GetCharName(), GetMinutePlayTime() );
		return;
	}
	else
	{
		m_pCurFatigueType = pFatigueType;

		//���� �ʱ�ȭ �ܰ� �̹Ƿ� DB���� �о�� ������ ���� ���� ���ش�.( ���� : �д��� )
		int nMinMsgRemainTime = 0;
		if( nMinPlayTime == 0 )
		{
			nMinMsgRemainTime = m_pCurFatigueType->m_wMsgInterval;
		}
		else
		{
			// ����
			// ���� �޼��� �ð� = �޼��� ���� - { (���� ���� �÷��� �ð� - ���� �Ƿε� ������ ���� �ð�) / ���� �Ƿε������� �޼��� ���� }�� ������
			int nMod = ( nMinPlayTime - m_pCurFatigueType->m_nStartTime ) % m_pCurFatigueType->m_wMsgInterval;
			nMinMsgRemainTime = m_pCurFatigueType->m_wMsgInterval - nMod;
		}

		if( nMinMsgRemainTime <= 0 )
		{
			SUNLOG( eCRITICAL_LOG, "�̻��� �޼��� ���� �̴�. [%s], MsgTime = %d", m_pPlayer->GetCharName(), GetMinutePlayTime() );
		}

		m_nSecRemainMsgTime = nMinMsgRemainTime*60; //���� �޼����������� ���� �޼����ð� ��ŭ�� �����Ѵ�.(���� : ��)
	}

	SetInit( TRUE );
}

VOID	FatigueManager::Update()
{
	//��� ���� �ʰų�, �ʱ�ȭ ���� ���� ���·� ���� ������ �����Ѵ�.
	if( !IsApplyFatigue() || !GetInit() )		
		return;	

	// �ش� üũ ����Ŭ�� ��������..
	if( m_PlayingTimer.IsExpired() )
	{
		InitTickCount();

		if( FatigueOptionParser::Instance()->GetInitType() == FATIGUE_INIT_MIDNIGHT )		//������ �ʱ�ȭ �ϴ� ���̶��.
		{
			SYSTEMTIME sysTime;		
			util::TimeSync::GetLocalTime( &sysTime );

			if( sysTime.wDay != GetCurDay() )		//���� ��¥�� ����� ��¥�� Ʋ����(�Ϸ簡 �����ٸ�..)
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
			// ������ �ƴϸ�, �߱��� ���� �������� �ð��̴�. �̰��� DB���� ó�����ش�.
		}
		else
		{
			SUNLOG( eCRITICAL_LOG, "�̻��� �ʱ�ȭ Ÿ���̴�. [%s] Type = %d", m_pPlayer->GetCharName(), FatigueOptionParser::Instance()->GetInitType() );
		}

		//�Ƿε��� ������ѵ� �Ǵ°�?
		if( CanApplyFatigue() )
		{
			// ������ ���̴�. ���� �÷��� �ð��� �ʴ����� TickCount�� ���� ����Ѵ�. ���� ���� �ð��� ����� �ش�.
			int nSecDiffTick = (int)(GetDiffTickCount() / 1000); 
			AddPlayTime( nSecDiffTick );
			m_nSecRemainMsgTime -= nSecDiffTick;		//���� ���� �޼����ð��� �Ƿνð� �ð���ŭ ���ش�.
		}

		SetPreTickCount( GetCurTickCount() );
	}

	if( IsMsgTimeExpired() )		//�޼��� �ð��� ��������..
	{
		m_nSecRemainMsgTime = m_pCurFatigueType->m_wMsgInterval*60;    //���ο� �޼��� �������� �����Ѵ�.
		SendTimeMessage();
	}

	//���� �Ƿε������� ���� �ð����� ���� �÷��� �ð��� ���ų� ũ�ٸ�.. �Ƿε� ������ �ٲ�� �� �����̴�.
	//���� �Ƿε� ���ð��� -1�̸� �� �Ƿε� ������ ������ �̹Ƿ� ���̻� �ٲ��� �ʴ´�.
	if( (m_pCurFatigueType->m_nEndTime != -1) && (m_pCurFatigueType->m_nEndTime <= GetMinutePlayTime()) )
		ChangeFatigueType();		//������ �ٲ�� �ٲ������� �����Ѵ�.
}

BOOL	FatigueManager::ChangeFatigueType()
{
	// ���� �÷��� Ÿ������ ���ο� �޼��� Ÿ���� ���Ѵ�.
	_FATIGUE_TYPE* pFatigueType = FatigueOptionParser::Instance()->GetFatigueTypeFromTime( GetMinutePlayTime() );
	if( pFatigueType && m_pCurFatigueType )								
	{
		if( pFatigueType->m_byType != m_pCurFatigueType->m_byType )		//���� �ٸ� �Ƿε�Ÿ���̸�
		{
			m_pCurFatigueType = pFatigueType;
			m_nSecRemainMsgTime = m_pCurFatigueType->m_wMsgInterval*60;    //���ο� �޼��� �������� �����Ѵ�.
		}

		return TRUE;
	}
	else
	{
		SUNLOG( eCRITICAL_LOG, "���� �÷��̴����ð��� �´� �Ƿε� Ÿ���� �������� �ʽ��ϴ�.[%s] Type=%d", m_pPlayer->GetCharName(), pFatigueType->m_byType );
	}

	return FALSE;
}

VOID	FatigueManager::InitTickCount()
{
	SetCurTickCount( GetTickCount() );
}

BOOL	FatigueManager::CheckMaxPlayTime( SYSTEMTIME& sysTime )
{
	//����ð��� ���ü� �ִ� �ִ� �÷��� �ð��� ���Ѵ�.(���� : ��)
	DWORD dwMaxTime = sysTime.wHour*60*60 + sysTime.wMinute*60 + sysTime.wSecond;

	if( dwMaxTime < (DWORD)GetPlayTime() )		//���� �÷��� �ð���, ���ü� �ִ� �÷��� �ð����� ũ�ٸ�.. �߸��ȰŴ�. 0���� �ʱ�ȭ�Ѵ�.
	{
		SUNLOG( eFULL_LOG, "�÷��̴����ð����̻�������(����:��)[%s]CurTime=%u<Time=%u", m_pPlayer->GetCharName(), dwMaxTime, GetPlayTime() );
		return FALSE;
	}

	return TRUE;
}

//nPlayTime : �÷��� �ð�( �д��� )
BOOL	FatigueManager::SetDBPlayTime( int nPlayTime )
{

#ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
	if( !m_pPlayer )
	{
		SUNLOG( eCRITICAL_LOG, "�Ƿε��� ������ ����� ������ �����ϴ�." );
		return FALSE;
	}

	m_pPlayer->SetPlayTime( nPlayTime );
	// Offline �ð��� ������ �ʴ´�.
#else
	BASE_PLAYERINFO* pInfo = NULL;
	if( !m_pPlayer || !( pInfo = m_pPlayer->GetCharInfo() ) )
	{
		SUNLOG( eCRITICAL_LOG, "�Ƿε��� ������ �⺻ ����� ������ �����ϴ�." );
		return FALSE;
	}

	pInfo->m_PlayLimitedTime = nPlayTime;
#endif

	return TRUE;
}


//���� �Ƿνð��� ������ �� �ִ°�?
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

	// �������� �Ƿε��� ������ �ʰ� �����Ǿ� �ִµ�, ���� �����̸� �����Ű�� �ʴ´�.
	if( !pFatigueOptionParser->ApplyVillageTime() && m_pPlayer->GetGameZoneState() == ePRS_AT_VILLAGE )
		return FALSE;

	// �κ񿡼� �Ƿε��� ������ �ʰ� �����Ǿ� �ִµ�, ���� �κ��̸� �����Ű�� �ʴ´�.
	if( !pFatigueOptionParser->ApplyLobbyTime() && m_pPlayer->GetGameZoneState() == ePRS_AT_LOBBY )
		return FALSE;

	// ���۹����� �Ƿε��� ������ �ʰ� �����Ǿ� �ִµ�, ���� ���۹����̸� �����Ű�� �ʴ´�
	if( !pFatigueOptionParser->ApplySuperMaster() )
	{
		GameZone *pZone = m_pPlayer->GetGameZonePtr();
		if( pZone && pZone->CanApplySuperMasterBenefit( m_pPlayer ) )
			return FALSE;
	}

	// �׾����ÿ� �Ƿε��� ������ �ʰ� �����Ǿ� �ִµ�, ���� �׾����� �����Ű�� �ʴ´�.
	if( !pFatigueOptionParser->ApplyDeadTime() && m_pPlayer->GetHP() == 0 )
		return FALSE;

	return TRUE;
}

// �÷��� �ð��� ���� �޼��� ���.
void	FatigueManager::SendTimeMessage( )
{
	if( !m_pPlayer )		return;

	MSG_CG_STATUS_FATIGUE_CHANGE_CMD msg;
	msg.m_wPlayingTime = GetMinutePlayTime();
	m_pPlayer->SendPacket( &msg, sizeof(MSG_CG_STATUS_FATIGUE_CHANGE_CMD) );
}

// �Ƿε��� ���� ������ ��ӷ�
BYTE	FatigueManager::GetItemDropRatio( )
{
	BYTE byDropRatio = 100;

	if( !IsApplyFatigue() )		return byDropRatio;

	_FATIGUE_TYPE* pFatigueType = GetCurFatigueType();
	if( pFatigueType )
		byDropRatio = pFatigueType->m_byItemDropRatio;

	return byDropRatio;
}

// �Ƿε��� ���� ������ ����ġ ȹ���
BYTE	FatigueManager::GetExpRatio( )
{
	BYTE byExpRatio = 100;

	if( !IsApplyFatigue() )			return byExpRatio;

	_FATIGUE_TYPE* pFatigueType = GetCurFatigueType();
	if( pFatigueType )
		byExpRatio = pFatigueType->m_byExpRatio;

	return byExpRatio;
}

// �Ƿε��� ���� ������ �������� ������ �ִ� ����
BOOL	FatigueManager::GetItemPickUp()
{
	BOOL bItemPickUp = TRUE;

	if( !IsApplyFatigue() )			return bItemPickUp;

	_FATIGUE_TYPE* pFatigueType = GetCurFatigueType();
	if( pFatigueType )
		bItemPickUp = pFatigueType->m_bItemPickUp;

	return bItemPickUp;
}

// �Ƿε��� ���� ������ ��Ƽ���� �ξ����� �����Ǵ� ������ ���� Ȯ��
BYTE	FatigueManager::GetShareItemPickupRatio()
{
	BYTE byPickupRatio = 100;

	if( !IsApplyFatigue() )			return byPickupRatio;

	_FATIGUE_TYPE* pFatigueType = GetCurFatigueType();
	if( pFatigueType )
		byPickupRatio = pFatigueType->m_bySharedItemPickupRatio;

	return byPickupRatio;
}

// �Ƿε��� ���� ������ ��Ƽ���� �ξ����� �����Ǵ� ������ ���� Ȯ��
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
                          "�ش� �ð�(%u)�� ������ �Ƿε������� �����ϴ�.", 
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

