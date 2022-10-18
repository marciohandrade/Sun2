#include "StdAfx.h"

#include <Struct.h>
#include <GuildServer.h>
#include "GuildManager.h"
#include "GuildMember.h"
#include "Guild.h"
#include "PacketStruct_GZ.h"

#include <GuildRightInfoParser.h>
#include <MaxGuildMemberInfoParser.h>

#include "GuildSmallContentEntry.h"

GuildManager g_GuildManager;
GuildManager::GuildManager(void)
{
	m_GuildHash.Initialize( _MAX_GUILD_POOL_NUM );
	m_GuildNameMap.clear();
}

GuildManager::~GuildManager(void)
{
}

// <070114, waverix>
// 이것은 관리 구조를 변경하는 쪽이 나을 듯 하다.
// 굳이 또 여기에 있어야 하는지나 ServerFrame 쪽에서 얻는 것 등등 방법을 생각해 보자.
#include <Sessions/DBProxyServerSession.h>

BOOL GuildManager::IsConnectedDBProxy()
{
	DBProxyServerSession* pDBPServer = GuildServer::GetMainFrame()->ConnectSessions.SessionDBProxy();
	if( pDBPServer && pDBPServer->IsConnected() )
		return TRUE;
	return FALSE;
}


RC::eGUILD_RESULT GuildManager::ChangeGuildNotice( DWORD dwKey, GUILDGUID GuildGuid, TCHAR *ptszNotice )
{
	if( !IsConnectedDBProxy() ) return  RC::RC_GUILD_CANNOT_CHANGEPOSITION_STATE;
	Guild * pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return RC::RC_GUILD_FAILED;

	return RC::RC_GUILD_SUCCESS;
}

// 랭크업
RC::eGUILD_RESULT	GuildManager::CanRankUp( GUILDGUID GuildGuid, CHARGUID OperCharGuid, OUT GuildRankInfo& RankInfo )
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild )		return RC::RC_GUILD_NOT_EXIST_GUILD;

    // 명령하는 놈이 직책변경 권한이 있는지 체크..
    if( !CheckRight(GuildGuid,OperCharGuid,eGUILD_RIGHTS_RANKUP) )
        return RC::RC_GUILD_HAVE_NOT_RIGHT;

	// 길드 삭제 대기 상태면 랭크업할 수 없다.
	if(eGUILD_DELCHK_DESTROY_WAIT == pGuild->CheckGuildDeleteState())
		return RC::RC_GUILD_DELCHK_DESTROY_WAIT;;

	return pGuild->CanRankUp(  RankInfo );
}


VOID GuildManager::RankUpGuild( GUILDGUID GuildGuid )
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild )
	{
		SUNLOG( eCRITICAL_LOG, "[GuildManager::RankUpGuild] 일어나서는 안되는 것이 일어났다. GuildGuid = %u", GuildGuid );
		return;
	}

	pGuild->RankUp();
}

VOID		GuildManager::SetGuildTransaction( GUILDGUID GuildGuid, eGUILD_TRANSACTION tran )
{
	Guild *pGuild = FindGuild(GuildGuid);
	if(!pGuild)
	{
		SUNLOG( eCRITICAL_LOG, "[GuildManager::SetGuildTransaction] Caution!!! 트랜잭션 거는데 GuildGuid가 없다.. Guid = %d", GuildGuid );
		return;
	}
}

BOOL		GuildManager::IsGuildTransaction( GUILDGUID GuildGuid, eGUILD_TRANSACTION trans )
{
	Guild *pGuild = FindGuild(GuildGuid);

	//길드를 찾지 못하면 어떤 작업도 하면 안된다. 따라서 현재 트랜잭션 중이라고 리턴한다.
	if(!pGuild)	return TRUE;

	return TRUE;
}

// 공지 변경 처리
VOID GuildManager::OnChangeNoticeGuild( BOOL bSuccess, GUILDGUID GuildGuid, TCHAR * OUT ptszChangedNotice )
{
	Guild *pGuild = FindGuild( GuildGuid );
	ASSERT(pGuild);
}


// #길드원 카오상태 변경#
RC::eGUILD_RESULT GuildManager::ChangeChaoState( GUILDGUID GuildGuid, CHARGUID CharGuid, BYTE byChaoState )
{
	Guild *pGuild = FindGuild( GuildGuid );
	if( !pGuild )
	{
		return RC::RC_GUILD_NOT_EXIST_GUILD;
	}

	GuildMember *pMember = pGuild->FindMember( CharGuid );

	if( !pMember )
	{
		return RC::RC_GUILD_FAILED;
	}

	pMember->SetChaoState( byChaoState );

	return RC::RC_GUILD_SUCCESS;
}


VOID GuildManager::DisplayGuildInfo()
{
}

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
RC::eGUILD_RESULT GuildManager::ChuntingPVPResult(GUILDGUID WinGuildGuid, GUILDGUID LoseGuildGuid, GUILDGUID DisGuildGuid)
{
	Guild *pWinGuild = FindGuild( WinGuildGuid );
	if( !pWinGuild )	return RC::RC_GUILD_NOT_EXIST_GUILD;
	CHUNTING_PVP_INFO *pWinPVPInfo = pWinGuild->GetChuntingPVPInfo();

	Guild *pLoseGuild = NULL;
	if( DisGuildGuid )	pLoseGuild = FindGuild( DisGuildGuid );
	else				pLoseGuild = FindGuild( LoseGuildGuid );
	if( !pLoseGuild )	return RC::RC_GUILD_NOT_EXIST_GUILD;
	CHUNTING_PVP_INFO *pLosePVPInfo = pLoseGuild->GetChuntingPVPInfo();

	// 길드 승률
	float fWinGuildWinRatio = 1 / ( 1.0f + pow( 10.0f, (pLosePVPInfo->m_CHuntingPVPPoint-pWinPVPInfo->m_CHuntingPVPPoint) / 400.0f ) );
	float fLoseGuildWinRatio = 1 / ( 1.0f + pow( 10.0f, (pWinPVPInfo->m_CHuntingPVPPoint-pLosePVPInfo->m_CHuntingPVPPoint) / 400.0f ) );

	// 변수 K 결정
	int WinGuildK = 0, LoseGuildK = 0;
	int PVPPointDiff = pWinPVPInfo->m_CHuntingPVPPoint-pLosePVPInfo->m_CHuntingPVPPoint;
	if( PVPPointDiff >= 400 )							WinGuildK = 0;
	else if( pWinPVPInfo->m_CHuntingPVPPoint >= 2400 )	WinGuildK = 20;
	else if( pWinPVPInfo->m_CHuntingTotCount < 30 )		WinGuildK = 50;
	else												WinGuildK = 30;

	if( PVPPointDiff >= 400 )							LoseGuildK = 0;
	else if( pLosePVPInfo->m_CHuntingPVPPoint >= 2400 )	LoseGuildK = 20;
	else if( pLosePVPInfo->m_CHuntingTotCount < 30 )	LoseGuildK = 50;
	else												LoseGuildK = 30;

	// 평가포인트를 적용한다.
	if( DisGuildGuid )
	{
		// Dis의 경우 패배했을 경우보다 2배를 차감시킨다.
		pLosePVPInfo->m_CHuntingPVPPoint -= (int)( LoseGuildK * fLoseGuildWinRatio * 2 );
		pLosePVPInfo->m_CHuntingPVPPoint = max( 0, pLosePVPInfo->m_CHuntingPVPPoint );

		// 전적을 갱신한다.
		pWinPVPInfo->m_CHuntingTotCount++;
		pLosePVPInfo->m_CHuntingTotCount++;
		pLosePVPInfo->m_CHuntingDisCount++;
	}
	else
	{
		pWinPVPInfo->m_CHuntingPVPPoint += (int)( WinGuildK * ( 1 - fWinGuildWinRatio ) );
		pLosePVPInfo->m_CHuntingPVPPoint -= (int)( LoseGuildK * fLoseGuildWinRatio );
		pLosePVPInfo->m_CHuntingPVPPoint = max( 0, pLosePVPInfo->m_CHuntingPVPPoint );

		// 전적을 갱신한다.
		pWinPVPInfo->m_CHuntingTotCount++;
		pWinPVPInfo->m_CHuntingVCount++;
		pLosePVPInfo->m_CHuntingTotCount++;
		pLosePVPInfo->m_CHuntingLCount++;
	}

	return RC::RC_GUILD_SUCCESS;
}
#endif

//길드 직책 변경
	BOOL		GuildManager::ChangeMemberDuty( GUILDGUID GuildGuid, CHARGUID TargetCharGuid, eGUILD_DUTY eDuty)
	{
		//여기에 들어오면 벌써 DB에 해당 유저는 새로운 직책으로 변경된 후이다. 따라서 무조건 성공이어야 한다.
		//DB에서 온 값을 믿지 못하면 --;; 대책 없다.
		Guild* pGuild = FindGuild(GuildGuid);
		if( !pGuild )
		{
			SUNLOG( eCRITICAL_LOG, "[GuildManager::ChangeMemberDuty] Caution!, Guild Not Found = %d", GuildGuid );
			return FALSE;
		}
		
		return pGuild->SetMemberDuty( TargetCharGuid, eDuty );
	}

	GuildMember*	GuildManager::FindGuildMemberByName( GUILDGUID GuildGuid, TCHAR* pszMemberName )
	{
		Guild *pGuild = FindGuild(GuildGuid);
		if(!pGuild)	return NULL;

		return pGuild->FindMemberByCharName( pszMemberName );
	}

	GuildMember*	GuildManager::FindGuildMember( GUILDGUID GuildGuid, CHARGUID CharGuid )
	{
		Guild *pGuild = FindGuild(GuildGuid);
		if(!pGuild)	return NULL;

		return pGuild->FindMember( CharGuid );
	}

	RC::eGUILD_RESULT	GuildManager::CanChangeDuty( GUILDGUID GuildGuid, CHARGUID OperGuid, CHARGUID TargetGuid, eGUILD_DUTY ChangeDuty )
	{
		//0. GUID 체크
		if( OperGuid == TargetGuid )
			return RC::RC_GUILD_CANNOT_COMMAND_MYSELF;

		//1. 명령하는 놈과 받을려는 놈이 같은 길드 인지 체크.
		Guild* pGuild = FindGuild(GuildGuid);
		if(!pGuild)	return RC::RC_GUILD_INVALID_STATE;

		GuildMember* pOper = pGuild->FindMember( OperGuid );
		if( !pOper )	return RC::RC_GUILD_NOT_GUILD_MEMBER;	

		GuildMember* pTarget = pGuild->FindMember( TargetGuid );
		if( !pTarget )	return RC::RC_GUILD_NOT_GUILD_MEMBER;	

		//2. 명령하는 놈이 직책변경 권한이 있는지 체크..
		if( !CheckRight( GuildGuid, OperGuid, eGUILD_RIGHTS_CHANGE_DUTY ) )
			return RC::RC_GUILD_HAVE_NOT_RIGHT;

		//3. 직책 바꾸려는 놈의 현재 직책 체크
		if( ChangeDuty == pTarget->GetDuty() )
			return RC::RC_GUILD_CANNOT_CHANGE_DUTY_BECAUSE_SAME_DUTY;

		//9. 하위직책의 멤버가 상위직책의 멤버의 직책을 변경할 수 없다.( 숫자가 작을 수록 상위 직책 이다. )
		if( pOper->GetDuty() >= pTarget->GetDuty() )
			return RC::RC_GUILD_HAVE_NOT_RIGHT;

		//10. 나보다 상위 직책으로 변경 할 수 없다.
		if( pOper->GetDuty() >= ChangeDuty )
			return RC::RC_GUILD_HAVE_NOT_RIGHT;

		//4. 마스터나 군단장 변경은 이 함수로 체크 하지 않는다.
		if( ChangeDuty == eGUILD_DUTY_MASTER || ChangeDuty == eGUILD_DUTY_COMMANDER )
			return RC::RC_GUILD_CHANGE_MEMBER_DUTY_FAILED;

		//5. 부마스터와 군사는 한명만 존재해야 한다. 이미 존재 하는데 변경하려는 경우는 안된다.
		// 또한 대상이 직할 길드에 있어야만 한다.
		if( ChangeDuty == eGUILD_DUTY_SUBMASTER || ChangeDuty == eGUILD_DUTY_STRATEGIST )
		{
			if( pGuild->IsExistMemberDuty( ChangeDuty, 0 ) )
				return RC::RC_GUILD_ALREADY_EXIST_DUTY;

			if( pTarget->GetCorpsNumber() != 0 )
				return RC::RC_GUILD_CHANGE_MEMBER_DUTY_FAILED;
		}

		//군단장이거나 훈련대장이 변경 명령을 내리는 경우.
		if( pOper->GetDuty() == eGUILD_DUTY_COMMANDER || pOper->GetDuty() == eGUILD_DUTY_CAMP_LEADER )
		{
			BYTE byOperCorpsNum = pOper->GetCorpsNumber();

			// 명령을 내리는 사람은 현재 군단장 or 훈련대장 뿐이다.따라서 군단장의 군단과 대상의 군단이 같아야 한다.
			if( byOperCorpsNum != pTarget->GetCorpsNumber() )
				return RC::RC_GUILD_CORPS_NOT_FOUND;

			// 직할군단은 군장장이나, 훈련대장이 존재하지 않는다.
			if( byOperCorpsNum == 0 )		
				return RC::RC_GUILD_INVALID_DUTY_IN_BASE_CORPS;

            // 군단 존재하는지 체크.
            GuildCorps* pOperCorps = pGuild->FindCorps( byOperCorpsNum );
            if( !pOperCorps )		return RC::RC_GUILD_CORPS_NOT_FOUND;

            // 직책을 바꾸려는 대상이 군단내에 존재 해야 한다.
            if( !pOperCorps->FindMember( pTarget->GetCharGuid() ) )
                return RC::RC_GUILD_NOT_GUILD_MEMBER;	
        }

        // 길드 마스터는 육성캠프 없어도 훈련대장 직책 변경 가능한 버그 수정 [8/5/2009 lst1024] 
        // 훈련대장 관련 체크 위치 이동
        
        if( ChangeDuty == eGUILD_DUTY_CAMP_LEADER || ChangeDuty == eGUILD_DUTY_CAMP_SOLDIER )
        {
            BYTE byOperCorpsNum = pTarget->GetCorpsNumber();

            // 직할군단은 육성캠프가 없다.
            if( byOperCorpsNum == 0 )		
                return RC::RC_GUILD_NOT_EXIST_REAR_CAMP;

            // 군단 존재하는지 체크.
            GuildCorps* pOperCorps = pGuild->FindCorps( byOperCorpsNum );
            if( !pOperCorps )		
                return RC::RC_GUILD_CORPS_NOT_FOUND;

            // 훈련대장이나 훈련병으로 바꾸려면 현재 육성캠프가 활성화 되어 있어야 한다.
            if( !pOperCorps->IsExistRearCamp() )
                return RC::RC_GUILD_NOT_EXIST_REAR_CAMP;

            // 훈련병이나 훈련대장으로 바꿀때는 현재 직책이 정예병이여야 한다.
            if( pTarget->GetDuty() != eGUILD_DUTY_REGULAR_SOLDIER )
                return RC::RC_GUILD_NOT_SOLDIER;
        }

        // 훈련병 레벨 체크... 16레벨 보다 크면 훈련병으로 변경하지 못한다.
        if( ChangeDuty == eGUILD_DUTY_CAMP_SOLDIER )
        {
            if( pTarget->GetLevel() >= 16 )
                return RC::RC_GUILD_CANT_CHANGE_CAMP_SOLDIER_BY_LEVEL;
        }

		// 훈련병 변경은 군단장이나 훈련대장만 할 수 있다.
		if( ChangeDuty == eGUILD_DUTY_CAMP_SOLDIER )
		{
			if( pOper->GetDuty() != eGUILD_DUTY_COMMANDER && pOper->GetDuty() != eGUILD_DUTY_CAMP_LEADER )
				return RC::RC_GUILD_HAVE_NOT_RIGHT; 
		}

		//인원 체크 한다.
		if( pGuild->IsFullCorpsMemberByDuty( pTarget->GetCorpsNumber(), ChangeDuty ) )
			return RC::RC_GUILD_CANNOT_CHANGE_DUTY_BECAUSE_FULL_MEMEBER;

		return RC::RC_GUILD_SUCCESS;
	}

	//해당 유저가 해당 권한을 실행할 수 있는지 체크한다.
	BOOL		GuildManager::CheckRight( GUILDGUID GuildGuid, CHARGUID CharGuid, eGUILD_RIGHTS Right )
	{
		//따라서 권한체크 프로세서는
		//	1) 길드 등급에 해당 권한이 있는지( 스크립트상 )
		//	2) 권한을 사용하려는 유저의 직책에 해당 권한이 있는지 체크.
		//	- 유저의 직책이 현재 길드 등급에 포함되어 있는지 체크.
		//	- 유저에게 위임된 권한이 있는지 체크.( 위임된 권한은 스크립트와는 별개로 생각해야 한다. 무조건 + 다)
		//	따라서 여기서 권한이 있다고 판단되면, 아래의 체크는 하지 않는다.
		//	- 유저의 직책에 현재 권한이 있는지 체크.

		Guild *pGuild = FindGuild(GuildGuid);
		if(!pGuild)	return FALSE;

		GuildMember* pMember = pGuild->FindMember( CharGuid );
		if( !pMember )	return FALSE;
		
		//1. 먼저 스크립트 상의 길드 등급별 권한을 체크한다.
		if( !GuildRightInfoParser::Instance()->CheckGradeRight( Right, pGuild->GetGuildGrade() ) )
			return FALSE;

		//2. 현재 유저의 직책이 현재 길드등급에 등록되어 있는 것인지 체크한다.
		GuildRankInfo* pGuildRankInfo = GuildRankInfoParser::Instance()->FindGuildRankInfo( pGuild->GetGuildGrade() );
		if( !pGuildRankInfo )
			return FALSE;

		GUILD_DUTY_USE_INFO* pDutyUseInfo = pGuildRankInfo->Find( pMember->GetDuty() );
		if( !pDutyUseInfo || !pDutyUseInfo->m_bUse )
			return FALSE;

		// 서브 마스터와 군사에게만 위임된 권한이 존재한다. 
		if( pMember->GetDuty() == eGUILD_DUTY_SUBMASTER || pMember->GetDuty() == eGUILD_DUTY_STRATEGIST )
		{
			//3. 유저에게 위임받은 권한이 있는지 체크.
			if( pMember->CheckDelegateRight( Right ) )
				return TRUE;
		}

		//3. 위임받은 권한이 없으면 스크립트 상에서 직책별 권한이 있는지 체크한다.
		if( !GuildRightInfoParser::Instance()->CheckDutyRight( Right, pMember->GetDuty() ) )
			return FALSE;

		return TRUE;
	}

	VOID		GuildManager::GetCharName( GUILDGUID GuildGuid, CHARGUID CharGuid, OUT TCHAR* pszCharName, IN BYTE byBufSize )
	{
		if( byBufSize == 0 )	return;
		pszCharName[0] = '\0';

		Guild *pGuild = FindGuild(GuildGuid);
		if(!pGuild)		return;

		GuildMember* pMember = pGuild->FindMember( CharGuid );
		if(!pMember)	return;

		strncpy( pszCharName, pMember->GetCharName(), byBufSize );
	}

	RC::eGUILD_RESULT	GuildManager::IsDelegateChar( GUILDGUID GuildGuid, CHARGUID TargetGuid )
	{
		Guild *pGuild = FindGuild(GuildGuid);
		if(!pGuild)
		{
			SUNLOG( eCRITICAL_LOG, "[GuildManager::IsDelegateChar] Caution!!! Guild Not Found Guid = %d", GuildGuid );
			return RC::RC_GUILD_NOT_EXIST_GUILD;
		}

		GuildMember* pMember = pGuild->FindMember( TargetGuid );
		if(!pMember)
		{
			SUNLOG( eCRITICAL_LOG, "[GuildManager::IsDelegateChar] Caution!!! Guild Not Found Guid = %d", GuildGuid );
			return RC::RC_GUILD_NOT_GUILD_MEMBER;
		}

		if( !CheckDelegatedRight( pMember->GetDuty() ) )
			return RC::RC_GUILD_HAVE_NOT_RIGHT_TARGET_USER;

		return RC::RC_GUILD_SUCCESS;
	}

	RC::eGUILD_RESULT	GuildManager::IsDelegateChar( GUILDGUID GuildGuid, TCHAR* pszTargetCharName )
	{
		Guild *pGuild = FindGuild(GuildGuid);
		if(!pGuild)
		{
			SUNLOG( eCRITICAL_LOG, "[GuildManager::IsDelegateChar] Caution!!! Guild Not Found Guid = %d", GuildGuid );
			return RC::RC_GUILD_NOT_EXIST_GUILD;
		}

		GuildMember* pMember = pGuild->FindMemberByCharName( pszTargetCharName );
		if(!pMember)
		{
			SUNLOG( eCRITICAL_LOG, "[GuildManager::IsDelegateChar] Caution!!! Guild Not Found Guid = %d", GuildGuid );
			return RC::RC_GUILD_NOT_GUILD_MEMBER;
		}

		if( !CheckDelegatedRight( pMember->GetDuty() ) )
			return RC::RC_GUILD_HAVE_NOT_RIGHT_TARGET_USER;

		return RC::RC_GUILD_SUCCESS;
	}

	BOOL				GuildManager::CheckDelegatedRight( const eGUILD_DUTY& Duty )
	{
		// 권한위임은 부마스터나, 군사에게만 할 수 있다.
		if( Duty == eGUILD_DUTY_SUBMASTER || Duty == eGUILD_DUTY_STRATEGIST )
			return TRUE;

		return FALSE;
	}

	VOID				GuildManager::GetDelegateRight( GUILDGUID GuildGuid, TCHAR* pszTargetCharName, OUT GUILD_RIGHT_USE_INFO* pDelegateRight )
	{
		Guild *pGuild = FindGuild(GuildGuid);
		if(!pGuild)
		{
			SUNLOG( eCRITICAL_LOG, "[GuildManager::IsDelegateChar] Caution!!! Guild Not Found Guid = %d", GuildGuid );
			return;
		}

		GuildMember* pMember = pGuild->FindMemberByCharName( pszTargetCharName );
		if(!pMember)
		{
			SUNLOG( eCRITICAL_LOG, "[GuildManager::IsDelegateChar] Caution!!! Guild Not Found Guid = %d", GuildGuid );
			return;
		}

		pMember->GetDelegateRight( pDelegateRight );
	}

	BOOL GuildManager::IsDelegateRight( const GUILD_RIGHT_USE_INFO* pRightInfo )
	{
		for( int i = 0; i < MAX_GUILD_DELEGATE_RIGHT; i++ )
		{
			switch( pRightInfo[i].m_Right )
			{
			case eGUILD_RIGHTS_GUILD_MARK:
				break;
			case eGUILD_RIGHTS_RANKUP:
				break;
			case eGUILD_RIGHTS_RELATION:
				break;
			case eGUILD_RIGHTS_SEIGE_WARFARE :
				break;
			case eGUILD_RIGHTS_UP2GP:
				break;
			case eGUILD_RIGHTS_USE_UPGP:
				break;
			default:
				return FALSE;
				break;
			}
		}

		return TRUE;
	}


	VOID	GuildManager::ChangeDelegateRight( GUILDGUID GuildGuid, CHARGUID TargetGuid, GUILD_RIGHT_USE_INFO* pDelegateRight )
	{
		Guild *pGuild = FindGuild(GuildGuid);
		if(!pGuild)
		{
			SUNLOG( eCRITICAL_LOG, "[GuildManager:CanDelegateRight] Caution!!! Guild Not Found Guid = %d", GuildGuid );
			return;
		}

		GuildMember* pTargetMember = pGuild->FindMember( TargetGuid );
		if( !pTargetMember )
		{
			SUNLOG( eCRITICAL_LOG, "[GuildManager:CanDelegateRight] Caution!!! Guild Not Found GuidMeber = %d", TargetGuid );
			return;
		}

		if( !pDelegateRight )
			return;

		pTargetMember->SetDelegateRight( pDelegateRight );

		GUILDLOG->LogDelegation(pGuild, pTargetMember, pDelegateRight->m_Right);
	}

BOOL		GuildManager::ChangeMaster( GUILDGUID GuildGuid, CHARGUID OldMasterCharGuid, CHARGUID NewMasterCharGuid, INT64 nMasterChangeTime )
{
	Guild *pGuild = FindGuild(GuildGuid);
	if(!pGuild)
	{
		SUNLOG( eCRITICAL_LOG, "[GuildManager:ChangeMaster] Caution!!! Guild Not Found Guid = %d", GuildGuid );
		return FALSE;
	}

	GuildMember* pOldMaster = pGuild->FindMember( OldMasterCharGuid );
	if( !pOldMaster )
	{
		SUNLOG( eCRITICAL_LOG, "[GuildManager:ChangeMaster] Caution!!! Guild Not Found GuidMeber = %d", OldMasterCharGuid );
		return FALSE;
	}

	GuildMember* pNewMaster = pGuild->FindMember( NewMasterCharGuid );
	if( !pNewMaster )
	{
		SUNLOG( eCRITICAL_LOG, "[GuildManager:ChangeMaster] Caution!!! Guild Not Found GuidMeber = %d", NewMasterCharGuid );
		return FALSE;
	}

	//1. 기존의 마스터는 정예로 되고, 새로운 마스터의 직책을 마스터로 변경한다
    const eGUILD_DUTY prev_duty_of_new_master = pNewMaster->GetDuty();
	pOldMaster->SetDuty( eGUILD_DUTY_REGULAR_SOLDIER );
	pNewMaster->SetDuty( eGUILD_DUTY_MASTER );

	//2. 위임된 권한은 둘다 초기화 시키자.
	pOldMaster->InitDelegateRight();
	pNewMaster->InitDelegateRight();

	//시간을 세팅해야 되지 않을까?
	pGuild->SetMasterChangeTime( nMasterChangeTime );

    // 길드마스터 변경에 의한 길드 직책 변경 로그 추가
    GUILDLOG->LogDutyChange(pGuild, pOldMaster, eGUILD_DUTY_MASTER);
    GUILDLOG->LogDutyChange(pGuild, pNewMaster, prev_duty_of_new_master);

	return TRUE;
}

RC::eGUILD_RESULT	GuildManager::CanChangeMaster( GUILDGUID GuildGuid, CHARGUID OldMasterCharGuid , TCHAR* pszNewMasterName )
{
	//1. 길드가 있는지 체크
	Guild* pGuild = g_GuildManager.FindGuild( GuildGuid );
	if( !pGuild )
		return RC::RC_GUILD_NOT_EXIST_GUILD;

	// 둘다 길드 멤버인지 체크
	GuildMember* pMaster = pGuild->FindMember( OldMasterCharGuid );
	if( !pMaster )			return RC::RC_GUILD_NOT_GUILD_MEMBER;

	GuildMember* pNewMaster = pGuild->FindMemberByCharName( pszNewMasterName );
	if( !pNewMaster )		return RC::RC_GUILD_NOT_GUILD_MEMBER;

	// 직책 비교 ( 마스터가 아니면 안된다. )
	if( pMaster->GetDuty() != eGUILD_DUTY_MASTER || pMaster->GetCorpsNumber() != 0 )
		return RC::RC_GUILD_HAVE_NOT_RIGHT;

	// 이미 마스터 인데 마스터가 되면 안된다
	if( pNewMaster->GetDuty() == eGUILD_DUTY_MASTER )
		return RC::RC_GUILD_HAVE_NOT_RIGHT;

	// 직할길드가 아니면 안된다.
	if( pNewMaster->GetCorpsNumber() != 0 )
		return RC::RC_GUILD_NOT_BASE_CORPS;

	return RC::RC_GUILD_SUCCESS;			 

}

RC::eGUILD_RESULT	GuildManager::CanRightDelegate( GUILDGUID GuildGuid, CHARGUID OperCharGuid, TCHAR* pszTargetCharName, const GUILD_RIGHT_USE_INFO* pRightInfo )
{
	//1. 길드가 있는지 체크
	Guild* pGuild = g_GuildManager.FindGuild( GuildGuid );
	if( !pGuild )
		return RC::RC_GUILD_NOT_EXIST_GUILD;

	//2. 타켓대상이 있는지 확인한다.
	GuildMember* pTargetMember = pGuild->FindMemberByCharName( pszTargetCharName );
	if( !pTargetMember )
		return RC::RC_GUILD_NOT_GUILD_MEMBER;

	//3. 리스트 요청자가 리스트를 요청해도 되는 사람인지 체크한다.
	if( !g_GuildManager.CheckRight( GuildGuid, OperCharGuid, eGUILD_RIGHTS_DELEGATION_RIGHT ) )
		return RC::RC_GUILD_HAVE_NOT_RIGHT;

	//4. 권한위임대상이 위임대상이 되는지 체크.
	RC::eGUILD_RESULT rc = g_GuildManager.IsDelegateChar( GuildGuid, pTargetMember->GetCharGuid() );	
	if( rc != RC::RC_GUILD_SUCCESS )
		return rc;

	//4. 현재 위임하려는 권한들이 정확한 위임권한인지 체크.
	if( !g_GuildManager.IsDelegateRight( pRightInfo ) )
		return RC::RC_GUILD_INVALID_DELEGATE_RIGHT;

	return RC::RC_GUILD_SUCCESS;
}

CHARGUID GuildManager::FindRightMember(GUILDGUID GuildGuid, eGUILD_DUTY eDUTY)
{
	Guild* pGuild = g_GuildManager.FindGuild( GuildGuid );
	if(pGuild)
	{
		GuildMember* Member = pGuild->FindMemberByDuty(eDUTY);
		if(Member && Member->IsLogIn()) return Member->GetCharGuid();
	}

	return 0;
}

// 길드 군단 생성
RC::eGUILD_RESULT GuildManager::CreateCorps( GUILDGUID GuildGuid, BYTE byCorpsNum, CHARGUID TargetCharGuid )
{
	Guild* pGuild = FindGuild(GuildGuid);
	if(!pGuild)	return RC::RC_GUILD_NOT_EXIST_GUILD;

	GuildCorps* pCorps = pGuild->CreateCorps( byCorpsNum );
	if( !pCorps ) return RC::RC_GUILD_CREATE_CORPS_FAILED;

	//군단장 임명
	RC::eGUILD_RESULT rc = g_GuildManager.ChangeCommander( GuildGuid, byCorpsNum, TargetCharGuid );
	if( rc != RC::RC_GUILD_SUCCESS )
		return rc;

	if(GuildMember* pCommander = g_GuildManager.FindGuildMember(GuildGuid, TargetCharGuid))
	{
		GUILDLOG->LogCorpsCreation(pGuild, pCommander, byCorpsNum);
	}

	return RC::RC_GUILD_SUCCESS;
}


RC::eGUILD_RESULT	GuildManager::CanCreateCorps( GUILDGUID GuildGuid, CHARGUID OperCharGuid, const TCHAR* pszTargetCharName, OUT BYTE& byCorpsNum )
{
	// 권한체크..
	//1. 길드가 있는지 체크
	Guild* pGuild = g_GuildManager.FindGuild( GuildGuid );
	if( !pGuild )
		return RC::RC_GUILD_NOT_EXIST_GUILD;

	//2. 군단생성 요청자가 해당명령을 요청해도 되는 사람인지 체크한다.
	if( !g_GuildManager.CheckRight( GuildGuid, OperCharGuid, eGUILD_RIGHTS_CREATE_CORPS ) )
		return RC::RC_GUILD_HAVE_NOT_RIGHT;

	//3. 현재 군단 창설 개수를 체크하여 창설 가능한지 확인한다.
	GuildRankInfo* pRankInfo = GuildRankInfoParser::Instance()->FindGuildRankInfo( pGuild->GetGuildGrade() );
	if( !pRankInfo )
		return RC::RC_GUILD_NOT_FOUND_GUILD_INFO_FOR_GRADE;

	byCorpsNum = pGuild->GetEmptyCorpsNumBySequence();
	if( byCorpsNum == 0 )
		return RC::RC_GUILD_CORPS_NUM_OVERFLOW;

	// 군단장이 될 사람이 현재 길드 멤버인지 확인한다.
	GuildMember* pTargetMember = pGuild->FindMemberByCharName( pszTargetCharName );
	if( !pTargetMember )
		return RC::RC_GUILD_NOT_GUILD_MEMBER;

	// 군단장 될 사람이 직할 군단인지 체크
	if( pTargetMember->GetCorpsNumber() != 0 )
		return RC::RC_GUILD_NOT_BASE_CORPS;

	// 군단장 될 사람이 정예병인지 체크.
	if( pTargetMember->GetDuty() != eGUILD_DUTY_REGULAR_SOLDIER )
		return RC::RC_GUILD_NOT_SOLDIER;


	return RC::RC_GUILD_SUCCESS;
}

RC::eGUILD_RESULT  	GuildManager::CanDestoryCorps( GUILDGUID GuildGuid, CHARGUID OperCharGuid, BYTE byCorpsNum )
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return RC::RC_GUILD_NOT_EXIST_GUILD;

	GuildMember* pOperMember = pGuild->FindMember( OperCharGuid );
	if( !pOperMember )			return RC::RC_GUILD_NOT_GUILD_MEMBER;

	//1. 직할 군단이면 안된다.
	if( byCorpsNum == 0 ) 
		return RC::RC_GUILD_INVALID_FUNCTION_IN_BASE_CORPS;

	//2. 권한체크.
	if( !CheckRight( GuildGuid, OperCharGuid, eGUILD_RIGHTS_DISMISS_CORPS ) )
		return RC::RC_GUILD_HAVE_NOT_RIGHT;

	GuildCorps* pCorps = pGuild->FindCorps( byCorpsNum );
	if( !pCorps )			return RC::RC_GUILD_CORPS_NOT_FOUND;

	// 군단 해산 조건
	// 1. 군단내에 인원이 없어야 한다.
	if( pCorps->GetMemberCount() > 0 )
		return RC::RC_GUILD_EXIST_MEMBER;

	return RC::RC_GUILD_SUCCESS;
}

RC::eGUILD_RESULT GuildManager::DestoryCorps( GUILDGUID GuildGuid, BYTE byCorpsNum, CHARGUID operCharGuid )
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return RC::RC_GUILD_NOT_EXIST_GUILD;

	pGuild->DestroyCorps( byCorpsNum );

    GuildMember* pMember = pGuild->FindMember(operCharGuid);
    if (pMember)
    {
    	GUILDLOG->LogCorpsBreakUp(pGuild, pMember, byCorpsNum);
    }

	return RC::RC_GUILD_SUCCESS;
}

RC::eGUILD_RESULT	GuildManager::CanBeCommander(  GUILDGUID GuildGuid, CHARGUID CommanderCharGuid )
{
	Guild* pGuild = g_GuildManager.FindGuild( GuildGuid );
	if( !pGuild )		return RC::RC_GUILD_NOT_EXIST_GUILD;

	GuildMember* pMember = pGuild->FindMember( CommanderCharGuid );
	if( !pMember )		return RC::RC_GUILD_NOT_GUILD_MEMBER;

	//해당 멤버가 정예군이 아니면 될수 없다.
	if( pMember->GetDuty() != eGUILD_DUTY_REGULAR_SOLDIER )
		return RC::RC_GUILD_MEMBER_NOT_SOLDIER;

	//해당 멤버가 직할길드 인지 체크한다.
	if( pMember->GetCorpsNumber() != 0 )  //직할길드가 아니면..
		return RC::RC_GUILD_MEMBER_NOT_HEAD_CORPS;

	return  RC::RC_GUILD_SUCCESS;
}

RC::eGUILD_RESULT	GuildManager::CanCreateGuild( CHARGUID MasterCharGuid, LEVELTYPE MasterLV  )
{
	// 기존의 길드 이름과 같은길드가 존재하는 지도, DB에서 체크 하도록 한다.

	//1. 기존 길드에 가입 되어 있는지 체크
	//GuildMember* pMember = FindMember( MasterCharGuid );
	//if( pMember )
	//	return RC::RC_GUILD_ALREADY_GUILD_MEMBER;

	//2. 생성이므로 Legion의 정보를 가지고 온다.
	GuildRankInfo* pRankInfo = GuildRankInfoParser::Instance()->FindGuildRankInfo( eGUILD_GRADE_LEGION );
	if( !pRankInfo )
		return RC::RC_GUILD_NOT_FOUND_GUILD_INFO_FOR_GRADE;

	//3. 길드마스터 레벨 체크 
	if( MasterLV < CREATE_GUILD_LIMIT_LEVEL )
		return RC::RC_GUILD_CANNOT_CREAT_LEVEL_STATE;

	//4. 하임/카오상태 체크 해야 하는데... 해당 정보를 여기서 알수 없다. 게임서버에서 하임 체크 하고 온다.
	return RC::RC_GUILD_SUCCESS;
}

RC::eGUILD_RESULT		GuildManager::CreateGuild( GUILDGUID GuildGuid, const TCHAR* tszGuildName, BYTE byCorpsNum, CHARGUID MasterCharGuid, 
	const TCHAR* tszMasterCharName, eGUILD_DUTY MasterDuty, BYTE byMasterClass, LEVELTYPE MasterLevel, BYTE byMasterChaoState,
	INT64 nGuildPanaltyTime, INT64 nDestroyRequestTime, INT64 nMasterChangeTime )
{
	if( FindGuild( GuildGuid ) )
	{
		SUNLOG( eCRITICAL_LOG, "[GuildManager::CreateGuild] Error! GuildGuid = %u", GuildGuid );
		return RC::RC_GUILD_ALREADY_EXIST_GUILD;
	}

	Guild* pGuild = Guild::ALLOC();
	pGuild->InitializeGuild();
	pGuild->CreateGuild( GuildGuid, tszGuildName, byCorpsNum, MasterCharGuid, tszMasterCharName, MasterDuty, byMasterClass, MasterLevel,
		byMasterChaoState, nGuildPanaltyTime, nDestroyRequestTime, nMasterChangeTime );
	AddGuild( pGuild, pGuild->GetGuildGuid() );
	AddGuildName(pGuild->GetGuildName(), pGuild->GetGuildGuid());

	GUILDLOG->LogGuildCreation(pGuild);

	return RC::RC_GUILD_SUCCESS;
}

GuildMember*		GuildManager::FindMaster( GUILDGUID GuildGuid )
{
	Guild * pGuild = FindGuild( GuildGuid );
	if( !pGuild )		return NULL;

	return pGuild->FindMaster();
}

RC::eGUILD_RESULT  	GuildManager::CanDestroyGuild( GUILDGUID GuildGuid, CHARGUID OperCharGuid )
{
	Guild * pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return RC::RC_GUILD_NOT_EXIST_GUILD;

	//길드 해체는 마스터만이 할 수 있다.
	GuildMember* pMaster = pGuild->FindMaster();
	if( !pMaster )	return	RC::RC_GUILD_CURRENT_NONE_MASTER;

	if( pMaster->GetCharGuid() != OperCharGuid )
		return RC::RC_GUILD_HAVE_NOT_RIGHT;

	// 길드 삭제 대기 상태면 Destroy할 수 없다.
	if(eGUILD_DELCHK_DESTROY_WAIT == pGuild->CheckGuildDeleteState())
		return RC::RC_GUILD_DELCHK_DESTROY_WAIT;

	return RC::RC_GUILD_SUCCESS;
}

VOID GuildManager::DestroyGuild( GUILDGUID GuildGuid )
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return;

	GUILDLOG->LogGuildBreakUpCompleted(pGuild);

	pGuild->Destroy();

	RemoveGuild( GuildGuid );
	RemoveGuildName(pGuild->GetGuildName());
	m_GuildPenaltyMgr.erase(GuildGuid);
	Guild::FREE( pGuild );
}

BYTE	GuildManager::GetEmptyCommanderInfo( GUILDGUID GuildGuid, OUT BYTE* pEmptyCommanderInfo )
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return 0;

	return pGuild->GetEmptyCommanderInfo( pEmptyCommanderInfo );

}

RC::eGUILD_RESULT  GuildManager::CanChangeCommander( GUILDGUID GuildGuid, CHARGUID OperCharGuid, const TCHAR* tszMasterCharName, BYTE byCorpsNum )
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return RC::RC_GUILD_NOT_EXIST_GUILD;

	//직할 군단은 군단장이 없다.
	if( byCorpsNum == 0 ) 
		return RC::RC_GUILD_INVALID_DUTY_IN_BASE_CORPS;

	GuildMember* pOperMember = pGuild->FindMember( OperCharGuid );
	if( !pOperMember )
		return RC::RC_GUILD_NOT_GUILD_MEMBER;

	if( !CheckRight( GuildGuid, OperCharGuid, eGUILD_RIGHTS_CHANGE_DUTY ) )
		return RC::RC_GUILD_HAVE_NOT_RIGHT;

	GuildMember* pTargetMember = pGuild->FindMemberByCharName( tszMasterCharName );
	if( !pTargetMember )
		return RC::RC_GUILD_NOT_GUILD_MEMBER;

	//현재 군단의 군단장이 있으면 변경을 하지 못한다.
	GuildCorps* pCorps = pGuild->FindCorps( byCorpsNum );
	if( !pCorps )		//군단이 존재하지 않는다.
		return RC::RC_GUILD_CORPS_NOT_FOUND;

	if( pCorps->FindCommander() )		//이미 군단장이 존재하면.. 
		return RC::RC_GUILD_ALREADY_EXIST_COMMANDER;

	// 현재 군단장 시키려는 자의 직책이 군단장이면 안된다.
	if( pTargetMember->GetDuty() == eGUILD_DUTY_COMMANDER )
		return RC::RC_GUILD_CANT_CHANGE_COMMANDER_BECAUSE_COMMANDER;

	//군단장은 1명이므로 인원수 체크 안한다.

	return RC::RC_GUILD_SUCCESS;
}

RC::eGUILD_RESULT	GuildManager::ChangeCommander( GUILDGUID GuildGuid, BYTE byCorpsNum, CHARGUID CommanderGuid)
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild )			 return RC::RC_GUILD_NOT_EXIST_GUILD;

	GuildCorps* pCorps = pGuild->FindCorps( byCorpsNum );
	if( !pCorps )			return RC::RC_GUILD_CORPS_NOT_FOUND;

	GuildMember* pCommander = pGuild->FindMember( CommanderGuid );
	if( !pCommander )		return RC::RC_GUILD_NOT_GUILD_MEMBER;

	//1. 군단을 바꾼다.
	RC::eGUILD_RESULT rt =pGuild->ChangeCorps( CommanderGuid, byCorpsNum );
	if( rt != RC::RC_GUILD_SUCCESS )
		return rt;

	//2. 직책를 바꾸어 주고
	pCommander->SetDuty( eGUILD_DUTY_COMMANDER );

	return RC::RC_GUILD_SUCCESS;
}

// 군단 변경
RC::eGUILD_RESULT  	GuildManager::CanChangeCorps( GUILDGUID GuildGuid, CHARGUID OperCharGuid, const TCHAR* tszMasterCharName, BYTE byCorpsNum )
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return RC::RC_GUILD_NOT_EXIST_GUILD;

	GuildMember* pOperMember = pGuild->FindMember( OperCharGuid );
	if( !pOperMember )			return RC::RC_GUILD_NOT_GUILD_MEMBER;

	GuildMember* pTargetMember = pGuild->FindMemberByCharName( tszMasterCharName );
	if( !pTargetMember )			return RC::RC_GUILD_NOT_GUILD_MEMBER;

	// 옮기려는 유저의 직책 체크 ( 반드시 정예병 이어야 한다. )
	if( pTargetMember->GetDuty() != eGUILD_DUTY_REGULAR_SOLDIER )
		return RC::RC_GUILD_NOT_SOLDIER;

	if( !CheckRight( GuildGuid, OperCharGuid, eGUILD_RIGHTS_CHANGE_CORPS ) )
		return RC::RC_GUILD_HAVE_NOT_RIGHT;

	//변경하려는 군단과 현재 타겟유저의 군단 번호가 같으면 안된다. )
	if( pTargetMember->GetCorpsNumber() == byCorpsNum )
		return RC::RC_GUILD_CANNOT_CHANGE_CORPS_BECAUSE_SAME_CORPS;

	//현재 타겟유저와 옮길려는 군단이 존재해야 한다.
	if( !pGuild->FindCorps( pTargetMember->GetCorpsNumber() ) )
		return RC::RC_GUILD_CORPS_NOT_FOUND;

	GuildCorps* pTargetCorps = pGuild->FindCorps( byCorpsNum );
	if( !pTargetCorps )			return RC::RC_GUILD_CORPS_NOT_FOUND;

	//인원 체크 한다.
	if( pGuild->IsFullCorpsMemberByDuty( byCorpsNum, pTargetMember->GetDuty() ) )
		return RC::RC_GUILD_CORPS_FULL_MEMBER;

	return RC::RC_GUILD_SUCCESS;
}

//군단간 변경
RC::eGUILD_RESULT	GuildManager::ChangeCorps( GUILDGUID GuildGuid, CHARGUID TargetCharGuid, BYTE byCorpsNum )
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return RC::RC_GUILD_NOT_EXIST_GUILD;

	return pGuild->ChangeCorps( TargetCharGuid, byCorpsNum );
}

//군단 육성 캠프 생성
RC::eGUILD_RESULT  	GuildManager::CanCreateRearCamp( GUILDGUID GuildGuid, CHARGUID OperCharGuid, const TCHAR* pszTargetCharName, OUT BYTE& byCorpsNum )
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return RC::RC_GUILD_NOT_EXIST_GUILD;

	//멤버 확인
	GuildMember* pOperMember = pGuild->FindMember( OperCharGuid );
	if( !pOperMember )			return RC::RC_GUILD_NOT_GUILD_MEMBER;

	//1. 권한체크.
	if( !CheckRight( GuildGuid, OperCharGuid, eGUILD_RIGHTS_CREATE_CAMP ) )
		return RC::RC_GUILD_HAVE_NOT_RIGHT;

	//2. 군단존재 확인
	GuildCorps* pCorps = pGuild->FindCorps( pOperMember->GetCorpsNumber() );
	if( !pCorps )			return RC::RC_GUILD_CORPS_NOT_FOUND;

	//3. 훈련대장 될 놈이 같은 군단인지 확인.
	GuildMember* pTargetMember = pCorps->FindMember( pszTargetCharName );
	if( !pTargetMember )		return RC::RC_GUILD_NOT_CORPS_MEMBER;

	//4. 직책이 군단장인지 확인..
	if( pOperMember->GetDuty() != eGUILD_DUTY_COMMANDER )
		return RC::RC_GUILD_HAVE_NOT_RIGHT;

	//5. 타겟멤버가 정예병인지 확인.
	if( pTargetMember->GetDuty() != eGUILD_DUTY_REGULAR_SOLDIER )
		return RC::RC_GUILD_HAVE_NOT_RIGHT;

	//6. 현재 육성캠프가 있는지 체크.
	if( pCorps->IsExistRearCamp() )
		return RC::RC_GUILD_ALREADY_EXIST_REAR_CAMP;

	//군단번호 리턴..
	byCorpsNum = pOperMember->GetCorpsNumber();

	return RC::RC_GUILD_SUCCESS;
}

RC::eGUILD_RESULT  	GuildManager::CreateRearCamp( GUILDGUID GuildGuid, BYTE byCorpsNum, CHARGUID TargetCharGuid )
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return RC::RC_GUILD_NOT_EXIST_GUILD;

	if( !pGuild->CreateRearCamp( byCorpsNum ) )
		return RC::RC_GUILD_FAILED;

	//1. 멤버의 군단변경
	RC::eGUILD_RESULT rc = ChangeCorps( GuildGuid, TargetCharGuid, byCorpsNum );
	if( rc != RC::RC_GUILD_SUCCESS )
		return rc;

	//2. 멤버의 직책변경( 훈련대장 )
	GuildMember* pMember = FindGuildMember( GuildGuid, TargetCharGuid );
	if( !pMember )	return RC::RC_GUILD_NOT_GUILD_MEMBER;

	pMember->SetDuty( eGUILD_DUTY_CAMP_LEADER );

	GUILDLOG->LogCampCreation(pGuild, pMember, byCorpsNum);

	return RC::RC_GUILD_SUCCESS;
}

// 원래 해산은 군단장만이 할 수 있으므로 군단번호가 필요 없을거 같지만,
// 나중에 길마나 또는 누군가가 할 수 있을지도 모른다. 
// 그래서 군단번호를 받자.
RC::eGUILD_RESULT  	GuildManager::CanDestroyRearCamp( GUILDGUID GuildGuid, CHARGUID OperCharGuid, BYTE byCorpsNum )
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return RC::RC_GUILD_NOT_EXIST_GUILD;

	GuildMember* pOperMember = pGuild->FindMember( OperCharGuid );
	if( !pOperMember )			return RC::RC_GUILD_NOT_GUILD_MEMBER;

	//1. 직할 군단이면 안된다.
	if( byCorpsNum == 0 ) 
		return RC::RC_GUILD_INVALID_FUNCTION_IN_BASE_CORPS;

	//2. 권한체크.
	if( !CheckRight( GuildGuid, OperCharGuid, eGUILD_RIGHTS_DISMISS_CAMP ) )
		return RC::RC_GUILD_HAVE_NOT_RIGHT;

	// 훈련캠프 해산 조건
	// 1. 훈련대장이랑 훈련병이 없어야 한다.
	GuildCorps* pCorps = pGuild->FindCorps( byCorpsNum );
	if( !pCorps )			return RC::RC_GUILD_CORPS_NOT_FOUND;

	// 현재 훈련캠프가 있는지 체크.
	if( pCorps->IsExistRearCamp() == FALSE )
		return RC::RC_GUILD_NOT_EXIST_REAR_CAMP;

	// 훈련대장이 있으면 안된다.
	if( pCorps->GetMemberCountByDuty( eGUILD_DUTY_CAMP_LEADER ) > 0 )
		return RC::RC_GUILD_EXIST_MEMBER;

	// 훈련병이 있으면 안된다.
	if( pCorps->GetMemberCountByDuty( eGUILD_DUTY_CAMP_SOLDIER ) > 0 )
		return RC::RC_GUILD_EXIST_MEMBER;

	return RC::RC_GUILD_SUCCESS;
}

RC::eGUILD_RESULT  	GuildManager::DestroyRearCamp( GUILDGUID GuildGuid, BYTE byCorpsNum, CHARGUID operCharGuid )
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return RC::RC_GUILD_NOT_EXIST_GUILD;

	if( !pGuild->DestroyRearCamp( byCorpsNum ) )
		return RC::RC_GUILD_FAILED;

    GuildMember* pMember = pGuild->FindMember(operCharGuid);
    if (pMember)
    {
    	GUILDLOG->LogCampBreakUp(pGuild, pMember, byCorpsNum);
    }

	return RC::RC_GUILD_SUCCESS;
}

//길드초대가 가능한가?
RC::eGUILD_RESULT	GuildManager::CanInviteGuild( const GUILDGUID GuildGuid, const CHARGUID OperCharGuid, const CHARGUID TargetCharGuid, const LEVELTYPE TargetLevel )
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return RC::RC_GUILD_NOT_EXIST_GUILD;

	// 권한체크 .
	if( !CheckRight( GuildGuid, OperCharGuid, eGUILD_RIGHTS_JOIN ) )
		return RC::RC_GUILD_HAVE_NOT_RIGHT;

	// 길드 삭제 대기 상태면 Join할 수 없다.
	if(eGUILD_DELCHK_DESTROY_WAIT == pGuild->CheckGuildDeleteState())
		return RC::RC_GUILD_DELCHK_DESTROY_WAIT;

	//길드가 현재 패널티에 적용을 받고 있는 상태인지
	if(!GuildSmallContentsEntry::GuildPenalty::CheckGuildPenaltyTime(pGuild))
		return RC::RC_GUILD_WITHDRAW_PENALTY_TIME;

	// TODO : 군단 패널티도 체크..현재는 DB에서만 체크하고 있음

	// 해당 멤버가 다른 길드원이면 가입하지 못한다. 
	// 이거 처리 어디서 해줄 것인가? DB에서 해주자.. 여기서는 해당 길드에 길드원인지만 체크한다.
	// 그 전에 게임서버에서 해당 캐릭터에 대한 길드 체크를 하고 여기 온다.

	GuildMember* pOperMember = pGuild->FindMember( OperCharGuid );
	if( !pOperMember )	return RC::RC_GUILD_NOT_GUILD_MEMBER;

	GuildMember* pTargetMember = pGuild->FindMember( TargetCharGuid );
	if( pTargetMember )	return RC::RC_GUILD_ALREADY_GUILD_MEMBER;

	//가입시킨놈이 훈련대장이고,
	eGUILD_DUTY Duty = eGUILD_DUTY_REGULAR_SOLDIER;
	if( pOperMember->GetDuty() == eGUILD_DUTY_CAMP_LEADER )
	{
		if( TargetLevel >= 16 )
			return RC::RC_GUILD_CANNOT_JOIN_CAMP_SOLDERY_BY_LEVEL;

		Duty = eGUILD_DUTY_CAMP_SOLDIER;
	}

	//3. 군단결정( 가입시키려는 놈이 군단장이나, 훈련대장이면 같은 군단에 가입하는거다. )
	BYTE byCorpsNum = 0;
	if( pOperMember->GetDuty() == eGUILD_DUTY_CAMP_LEADER || pOperMember->GetDuty() == eGUILD_DUTY_COMMANDER )
		byCorpsNum = pOperMember->GetCorpsNumber();

	//인원 체크 한다.
	if( pGuild->IsFullCorpsMemberByDuty( byCorpsNum, Duty ) )
		return RC::RC_GUILD_CORPS_FULL_MEMBER;

	return RC::RC_GUILD_SUCCESS;
}


RC::eGUILD_RESULT	GuildManager::CanJoinGuild( const GUILDGUID& GuildGuid, const CHARGUID& OperCharGuid, const CHARGUID& TargetCharGuid, const LEVELTYPE& TargetLevel, OUT eGUILD_DUTY& Duty, OUT BYTE& byCorpsNum )
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return RC::RC_GUILD_NOT_EXIST_GUILD;

	// 권한체크 .
	if( !CheckRight( GuildGuid, OperCharGuid, eGUILD_RIGHTS_JOIN ) )
		return RC::RC_GUILD_HAVE_NOT_RIGHT;

	// 길드 삭제 대기 상태면 Join할 수 없다.
	if(eGUILD_DELCHK_DESTROY_WAIT == pGuild->CheckGuildDeleteState())
		return RC::RC_GUILD_DELCHK_DESTROY_WAIT;

	if(!GuildSmallContentsEntry::GuildPenalty::CheckGuildPenaltyTime(pGuild))
		return RC::RC_GUILD_KICK_PENALTY_TIME;

	// 해당 멤버가 다른 길드원이면 가입하지 못한다. 
	// 이거 처리 어디서 해줄 것인가? DB에서 해주자.. 여기서는 해당 길드에 길드원인지만 체크한다.
	// 그 전에 게임서버에서 해당 캐릭터에 대한 길드 체크를 하고 여기 온다.
	GuildMember* pTargetMember = pGuild->FindMember( TargetCharGuid );
	if( pTargetMember )	return RC::RC_GUILD_ALREADY_GUILD_MEMBER;

	//2. 타겟멤버의 직책을 결정한다.
	GuildMember* pOperMember = pGuild->FindMember( OperCharGuid );
	if( !pOperMember )	return RC::RC_GUILD_NOT_GUILD_MEMBER;

	//가입시킨놈이 훈련대장이고,
	Duty = eGUILD_DUTY_REGULAR_SOLDIER;
	if( pOperMember->GetDuty() == eGUILD_DUTY_CAMP_LEADER )
	{
		if( TargetLevel >= 16 )
			return RC::RC_GUILD_CANNOT_JOIN_CAMP_SOLDERY_BY_LEVEL;

		Duty = eGUILD_DUTY_CAMP_SOLDIER;
	}

	//3. 군단결정( 가입시키려는 놈이 군단장이나, 훈련대장이면 같은 군단에 가입하는거다. )
	byCorpsNum = 0;
	if( pOperMember->GetDuty() == eGUILD_DUTY_CAMP_LEADER || pOperMember->GetDuty() == eGUILD_DUTY_COMMANDER )
		byCorpsNum = pOperMember->GetCorpsNumber();

	//인원 체크 한다.
	if( pGuild->IsFullCorpsMemberByDuty( byCorpsNum, Duty ) )
		return RC::RC_GUILD_CORPS_FULL_MEMBER;

	return RC::RC_GUILD_SUCCESS;
}

RC::eGUILD_RESULT	GuildManager::JoinGuild( GUILDGUID GuildGuid, BYTE byCorpsNum, CHARGUID CharGuid, 
	const TCHAR* tszCharName, eGUILD_DUTY Duty, BYTE byClass, LEVELTYPE Level, BYTE byChaoState )
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return RC::RC_GUILD_NOT_EXIST_GUILD;
	
	RC::eGUILD_RESULT result = pGuild->JoinGuild( byCorpsNum, CharGuid, tszCharName, Duty, byClass, Level, byChaoState );
	if(RC::RC_GUILD_SUCCESS == result)
	{
		GUILDLOG->LogMemberJoin(pGuild, CharGuid, tszCharName);
	}

	return result;
}

// #길드탈퇴#
RC::eGUILD_RESULT	GuildManager::CanLeaveGuild( GUILDGUID GuildGuid, CHARGUID CharGuid )
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return RC::RC_GUILD_NOT_EXIST_GUILD;
	
	// 길드 삭제 대기 상태면 탈퇴할 수 없다.
	if(eGUILD_DELCHK_DESTROY_WAIT == pGuild->CheckGuildDeleteState())
		return RC::RC_GUILD_DELCHK_DESTROY_WAIT;

	//1. 해당 길드의 길드원이냐?
	GuildMember* pMember = pGuild->FindMember( CharGuid );
	if( !pMember )	return RC::RC_GUILD_NOT_GUILD_MEMBER;

	//2. 해당 길드의 마스터만 빼고는 전부 탈퇴 가능하다.
	GuildMember* pMaster = pGuild->FindMaster();
	if( !pMaster )	return RC::RC_GUILD_CURRENT_NONE_MASTER;

	if( pMember->GetCharGuid() == pMaster->GetCharGuid() )
		return RC::RC_GUILD_CANNOT_LEAVE_MASTER;
		
	return RC::RC_GUILD_SUCCESS;
}

RC::eGUILD_RESULT	GuildManager::LeaveGuild( GUILDGUID GuildGuid, CHARGUID CharGuid )
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return RC::RC_GUILD_NOT_EXIST_GUILD;

	//1. 해당 길드의 길드원이냐?
	GuildMember* pMember = pGuild->FindMember( CharGuid );
	if( !pMember )	return RC::RC_GUILD_NOT_GUILD_MEMBER;

	GUILDLOG->LogMemberLeave(pGuild, pMember);

	//DestroyMember는 다른 리턴도 있는데 무조건 성공리턴??
	pGuild->DestroyMember( CharGuid );
	return RC::RC_GUILD_SUCCESS;
}

RC::eGUILD_RESULT	GuildManager::CanKickMember( GUILDGUID GuildGuid, CHARGUID OperCharGuid, TCHAR* ptszCharName )
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return RC::RC_GUILD_NOT_EXIST_GUILD;		

	// 길드 삭제 대기 상태면 Kick 할 수 없다.
	if(eGUILD_DELCHK_DESTROY_WAIT == pGuild->CheckGuildDeleteState())
		return RC::RC_GUILD_DELCHK_DESTROY_WAIT;

	// 권한체크 .
	if( !CheckRight( GuildGuid, OperCharGuid, eGUILD_RIGHTS_KICK ) )
		return RC::RC_GUILD_HAVE_NOT_RIGHT;

	GuildMember* pOperMember = pGuild->FindMember( OperCharGuid );
	if( !pOperMember )	return RC::RC_GUILD_NOT_GUILD_MEMBER;

	GuildMember* pTargetMember = pGuild->FindMemberByCharName( ptszCharName );
	if( !pTargetMember )	return RC::RC_GUILD_NOT_GUILD_MEMBER;

	// 자기 자신은 탈퇴시킬 수 없다.
	if( pOperMember->GetCharGuid() == pTargetMember->GetCharGuid() )
		return RC::RC_GUILD_HAVE_NOT_RIGHT;

	//명령자보다 상위직책을 제명할 수 없다.
	if( pOperMember->GetDuty() > pTargetMember->GetDuty() )
		return RC::RC_GUILD_HAVE_NOT_RIGHT;

	if( pTargetMember->GetDuty() == eGUILD_DUTY_MASTER )
		return RC::RC_GUILD_CANNOT_LEAVE_MASTER;

	return RC::RC_GUILD_SUCCESS;
}

// #길드강퇴#
RC::eGUILD_RESULT	GuildManager::KickMember( GUILDGUID GuildGuid, CHARGUID OperCharGuid, CHARGUID TargetCharGuid )
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return RC::RC_GUILD_NOT_EXIST_GUILD;

    GuildMember* pOperMember = pGuild->FindMember( OperCharGuid );
    if ( !pOperMember ) { return RC::RC_GUILD_NOT_GUILD_MEMBER; }

	GuildMember* pTargetMember = pGuild->FindMember( TargetCharGuid );
    if( !pTargetMember ) { return RC::RC_GUILD_NOT_GUILD_MEMBER; }

	GUILDLOG->LogMemberKickOut(pGuild, pOperMember, pTargetMember);
	
	//DestroyMember는 다른 리턴도 있는데 무조건 성공리턴??
	pGuild->DestroyMember( TargetCharGuid );
	return RC::RC_GUILD_SUCCESS;
}

//Serialize
BOOL		GuildManager::SerializeBaseGuildInfo( GUILDGUID GuildGuid, INOUT BASE_GUILD_INFO& info, eSERIALIZE se )
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return FALSE;

	pGuild->SerializeBaseGuildInfo( info, se );
	return TRUE;
}

BOOL		GuildManager::SerializeGameGuildInfo( GUILDGUID GuildGuid, INOUT GAME_GUILD_INFO& info, eSERIALIZE se )
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return FALSE;

	pGuild->SerializeGameGuildInfo( info, se );
	return TRUE;
}

BOOL		GuildManager::SerializeGuildInfo( GUILDGUID GuildGuid, INOUT GUILD_INFO& info, eSERIALIZE se )
{
	if( se == SERIALIZE_LOAD )
	{
		Guild* pGuild = FindGuild( GuildGuid );
		if( !pGuild ) 				return FALSE;

		pGuild->SerializeGuildInfo( info, se );
	}
	else
	{
		Guild* pGuild = FindGuild( GuildGuid );
		if( !pGuild )
		{
			Guild* pNewGuild = Guild::ALLOC();
			pNewGuild->InitializeGuild();
			pNewGuild->SerializeGuildInfo( info, se );
			AddGuild( pNewGuild, pNewGuild->GetGuildGuid() );
			AddGuildName(pNewGuild->GetGuildName(), pNewGuild->GetGuildGuid());
		}
		else
		{
			pGuild->SerializeGuildInfo( info, se );
		}
	}

	return TRUE;
}

BOOL		GuildManager::SerializeCorpsInfo( GUILDGUID GuildGuid, INOUT GUILD_CORPS_TOTAL_INFO& info, eSERIALIZE se )
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return FALSE;

	pGuild->SerializeCorpsInfo( info, se );
	return TRUE;
}


BOOL		GuildManager::SerializeBaseMemberTotalInfo( GUILDGUID GuildGuid, INOUT INOUT BASE_GUILD_MEMBER_TOTAL_INFO& info, eSERIALIZE se )
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return FALSE;

	pGuild->SerializeBaseMemberTotalInfo( info, se );
	return TRUE;
}

BOOL		GuildManager::SerializeMemberTotalInfo( GUILDGUID GuildGuid, INOUT INOUT GUILD_MEMBER_TOTAL_INFO& info, eSERIALIZE se )
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return FALSE;

	pGuild->SerializeMemberTotalInfo( info, se );
	return TRUE;
}

BOOL		GuildManager::SerializeServerMemberTotalInfo( GUILDGUID GuildGuid, INOUT SERVER_GUILD_MEMBER_TOTAL_INFO& info, eSERIALIZE se )
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return FALSE;

	pGuild->SerializeServerMemberTotalInfo( info, se );
	return TRUE;
}

BOOL		GuildManager::GetGuildBaseMemberInfo( GUILDGUID GuildGuid, CHARGUID CharGuid, OUT BASE_GUILD_MEMBER_INFO& info )
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return FALSE;

	return pGuild->GetBaseMemberInfo( CharGuid, info );
}

BOOL		GuildManager::GetGuildMemberInfo( GUILDGUID GuildGuid, CHARGUID CharGuid, OUT GUILD_MEMBER_INFO& info )
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return FALSE;

	return pGuild->GetMemberInfo( CharGuid, info );
}


RC::eGUILD_RESULT	GuildManager::CanLogin( GUILDGUID GuildGuid, CHARGUID CharGuid )
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return RC::RC_GUILD_NOT_EXIST_GUILD;

	// 길드가 존재하면 길드 멤버인지 확인한다.
	GuildMember* pMember = pGuild->FindMember( CharGuid );
	if( !pMember )	return RC::RC_GUILD_NOT_GUILD_MEMBER;

	if( pMember->IsLogIn() == TRUE )
		return RC::RC_GUILD_ALREADY_LOGIN_MEMBER;

	return RC::RC_GUILD_SUCCESS;
}

RC::eGUILD_RESULT	GuildManager::CanMoveServer( GUILDGUID GuildGuid, CHARGUID CharGuid )
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return RC::RC_GUILD_NOT_EXIST_GUILD;

	// 길드가 존재하면 길드 멤버인지 확인한다.
	GuildMember* pMember = pGuild->FindMember( CharGuid );
	if( !pMember )	return RC::RC_GUILD_NOT_GUILD_MEMBER;

	if( pMember->IsLogIn() == FALSE )
	{
		SUNLOG( eCRITICAL_LOG, "[GuildManager::CanLogin] 이미 로그인 안한 유저가 로그인 없이 서버이동 하려고 한다. GuildGuid = %u, CharGuid = %u", GuildGuid, CharGuid );
	}

	return RC::RC_GUILD_SUCCESS;
}


RC::eGUILD_RESULT	GuildManager::Login( GUILDGUID GuildGuid, CHARGUID CharGuid )
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return RC::RC_GUILD_NOT_EXIST_GUILD;

	// 길드가 존재하면 길드 멤버인지 확인한다.
	GuildMember* pMember = pGuild->FindMember( CharGuid );
	if( !pMember )	return RC::RC_GUILD_NOT_GUILD_MEMBER;

	pMember->SetLogin( TRUE );
	pMember->SetTotalInfoReqCnt( 0 );	

	return RC::RC_GUILD_SUCCESS;
}

RC::eGUILD_RESULT GuildManager::Logout( GUILDGUID GuildGuid, CHARGUID CharGuid )
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return RC::RC_GUILD_NOT_EXIST_GUILD;

	// 길드가 존재하면 길드 멤버인지 확인한다.
	GuildMember* pMember = pGuild->FindMember( CharGuid );
	if( !pMember )	return RC::RC_GUILD_NOT_GUILD_MEMBER;

	pMember->SetLogin( FALSE ); 
	pMember->SetTotalInfoReqCnt( 0 );	

	return RC::RC_GUILD_SUCCESS;
}

// UP/GP 변환
// NowUP : 현재 길드의 UP
// ResultUP : 변환후 UP
// ResultGP : 변환후 GP
RC::eGUILD_RESULT	GuildManager::CanConvertUP2GP( GUILDGUID GuildGuid, CHARGUID OperGuid, GPTYPE ConverGP )
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return RC::RC_GUILD_NOT_EXIST_GUILD;

	// 길드가 존재하면 길드 멤버인지 확인한다.
	GuildMember* pMember = pGuild->FindMember( OperGuid );
	if( !pMember )	return RC::RC_GUILD_NOT_GUILD_MEMBER;

	// 권한이 있는지 체크한다.
	// 권한체크 .
	if( !CheckRight( GuildGuid, OperGuid, eGUILD_RIGHTS_UP2GP ) )
		return RC::RC_GUILD_HAVE_NOT_RIGHT;

	if( ConverGP*GUILD_UP2GP_RATE > pGuild->GetGuildUP() )
		return RC::RC_GUILD_NOT_ENOUGH_UP;

	return RC::RC_GUILD_SUCCESS;
}

BOOL	GuildManager::SetPoint( GUILDGUID GuildGuid, GPTYPE gp, UPTYPE up )
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return FALSE;

	pGuild->SetGuildGP( gp );
	pGuild->SetGuildUP( up );

	return TRUE;
}

RC::eGUILD_RESULT	GuildManager::CanDonateUP( GUILDGUID GuildGuid, CHARGUID CharGuid  )
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return RC::RC_GUILD_NOT_EXIST_GUILD;

	// 길드가 존재하면 길드 멤버인지 확인한다.
	GuildMember* pMember = pGuild->FindMember( CharGuid );
	if( !pMember )	return RC::RC_GUILD_NOT_GUILD_MEMBER;

	if( !CheckRight( GuildGuid, CharGuid, eGUILD_RIGHTS_DONATEUP ) )
		return RC::RC_GUILD_HAVE_NOT_RIGHT;

	// 캐릭터가 소유한 UP는 게임서버에서 판별하고 온다.

	return RC::RC_GUILD_SUCCESS;
}

RC::eGUILD_RESULT	GuildManager::DonateUP( GUILDGUID GuildGuid, UPTYPE TotalUP, CHARGUID CharGuid, UPTYPE DonateUP )
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return RC::RC_GUILD_NOT_EXIST_GUILD;

    GuildMember* pMember = pGuild->FindMember( CharGuid );
    if( !pMember )	return RC::RC_GUILD_NOT_GUILD_MEMBER;

    GUILDLOG->LogUPDonation(pGuild, pMember, DonateUP, TotalUP);

	pGuild->SetGuildUP( TotalUP );

	return RC::RC_GUILD_SUCCESS;
}

RC::eGUILD_RESULT	GuildManager::LevelUpMember( GUILDGUID GuildGuid, CHARGUID CharGuid, LEVELTYPE Level )
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return RC::RC_GUILD_NOT_EXIST_GUILD;

	GuildMember* pMember = pGuild->FindMember( CharGuid );
	if( !pMember )	return RC::RC_GUILD_NOT_GUILD_MEMBER;

	if( pMember->GetLevel() >= Level )
		return RC::RC_GUILD_CANNOT_LEVELUP;

	pMember->SetLevel( Level );

	if( Level >= 31 && pMember->GetDuty() == eGUILD_DUTY_CAMP_SOLDIER )
		return RC::RC_GUILD_KICK_GUILD_BY_LEVELUP;

	return RC::RC_GUILD_SUCCESS;
}

BOOL				GuildManager::GetWorldGuildInfo( GUILDGUID GuildGuid, OUT WORLD_GUILD_INFO& info )
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return FALSE;

	pGuild->GetWorldGuildInfo( info );
	return TRUE;
}

RC::eGUILD_RESULT	GuildManager::CanRequestTotalInfo( GUILDGUID GuildGuid, CHARGUID CharGuid )
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return RC::RC_GUILD_NOT_EXIST_GUILD;

	GuildMember* pMember = pGuild->FindMember( CharGuid );
	if( !pMember )	return RC::RC_GUILD_NOT_GUILD_MEMBER;

	//현재 보낸 횟수가 3회 이상이면..
	if( pMember->GetTotalInfoReqCnt() > 3 )
	{
		SUNLOG( eCRITICAL_LOG, "[GuildManager::CanRequestTotalInfo] 3회 이상 길드TOTAL정보 요청함. GuildGuid = %u, CharGuid = %u, CharName = %s",
			GuildGuid, CharGuid, pMember->GetCharName() );
		return RC::RC_GUILD_OVER_TOTALINFO_REQ_CNT;
	}

	pMember->SetTotalInfoReqCnt( pMember->GetTotalInfoReqCnt() + 1 );

	return RC::RC_GUILD_SUCCESS;
}

VOID	 GuildManager::SetGuildNotice( GUILDGUID GuildGuid, const TCHAR* pszNotice )
{
	Guild* pGuild = g_GuildManager.FindGuild( GuildGuid );
	if( !pGuild )	return;

	pGuild->SetGuildNotice( pszNotice );

}

VOID GuildManager::AddGuildName(std::string name, GUILDGUID idx) 
{
	m_GuildNameMap[name] = idx;
}

VOID GuildManager::RemoveGuildName(std::string guild_name) 
{
	GUILD_NAME_HASH::iterator it = m_GuildNameMap.find(guild_name);
	if (it != m_GuildNameMap.end()) 
		m_GuildNameMap.erase(it);
}
	
Guild* GuildManager::FindGuildName(std::string guild_name) 
{
	GUILDGUID guild_index(0);
	GUILD_NAME_HASH::const_iterator it = m_GuildNameMap.find(guild_name);
	if (it != m_GuildNameMap.end())  
	{
		guild_index  =  it->second;
	}
	
	return FindGuild(guild_index );
}

GUILDGUID GuildManager::GetGuildGuidByGuildName(std::string guild_name)
{
	GUILDGUID guild_index(0);
	GUILD_NAME_HASH::const_iterator it = m_GuildNameMap.find(guild_name);
	if (it != m_GuildNameMap.end())  
	{
		guild_index  =  it->second;
	}

	return guild_index ;
}

CHARGUID GuildManager::GetGuildMasterByGuildName(std::string guild_name)
{
	GUILDGUID guild_index(0);
	GUILD_NAME_HASH::const_iterator it = m_GuildNameMap.find(guild_name);
	if (it != m_GuildNameMap.end())  
		guild_index  =  it->second;
	
	CHARGUID guild_master_index(0);
	Guild* pGuild = FindGuild(guild_index);
	if(pGuild)
	{
		GuildMember* pMember = pGuild->FindMaster();
		if(pMember && pMember->IsLogIn())
		{
			guild_master_index = pMember->GetCharGuid();
		}
	}
	
	return guild_master_index;	
}

RC::eGUILD_RESULT GuildManager::CanChangeGuildName( GUILDGUID GuildGuid, CHARGUID OperCharGuid )
{
	Guild *pGuild = FindGuild(GuildGuid);
	if(!pGuild)	
		return RC::RC_GUILD_NOT_EXIST_GUILD;

	if(GuildSmallContentsEntry::GuildPenalty::IsGuildDestroyWaitState(GuildGuid)) 
		return RC::RC_GUILD_DELCHK_DESTROY_WAIT;

	if( OperCharGuid != 0 ) //INVALID_CHARGUID
	{// 길드 마스터만 변경 가능. (스크립트로 안하기로함)
		GuildMember* pMember = pGuild->FindMember( OperCharGuid );
		if( !pMember )
			return RC::RC_GUILD_NOT_GUILD_MEMBER;

		if( eGUILD_DUTY_MASTER != pMember->GetDuty() )
			return RC::RC_GUILD_NAME_CHANGE_CANNOT_GRADE;
	}

	return RC::RC_GUILD_SUCCESS;
}

VOID GuildManager::ChangeGuildName( GUILDGUID guid, const TCHAR* pszGuildName )
{
	// 현재 길드 서버의 길드명을 바꾼다.
	// 길드 정보가 없는경우 일주일간 길드원이 한번도 접속 하지 않았다면, 길드 정보는 없다.
	// 웁스를 통해 들어올 경우 길드원 접속 안할수 있다.
	Guild* pGuild = FindGuild(guid);
	if( !pGuild )
		return;

	RemoveGuildName( pGuild->GetGuildName() );
	pGuild->ChangeGuildName( pszGuildName );
	AddGuildName( pGuild->GetGuildName(), pGuild->GetGuildGuid() );


	// 길드 관계에 있는 길드가 가진 길드 관계정보를 바꾼다.
	STLX_VECTOR<GUILDGUID> vGuildGuid;
	if( 0 == pGuild->GetRelationGuid(vGuildGuid) )
		return;

	STLX_VECTOR<GUILDGUID>::iterator it = vGuildGuid.begin();
	for( ; it != vGuildGuid.end() ; ++it )
	{
		Guild* pGuild = FindGuild( *it );
		if( pGuild )
			pGuild->GetRelationEntry()->ChangeGuildName( guid, pszGuildName );
	}
}

