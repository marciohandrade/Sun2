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
// �̰��� ���� ������ �����ϴ� ���� ���� �� �ϴ�.
// ���� �� ���⿡ �־�� �ϴ����� ServerFrame �ʿ��� ��� �� ��� ����� ������ ����.
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

// ��ũ��
RC::eGUILD_RESULT	GuildManager::CanRankUp( GUILDGUID GuildGuid, CHARGUID OperCharGuid, OUT GuildRankInfo& RankInfo )
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild )		return RC::RC_GUILD_NOT_EXIST_GUILD;

    // ����ϴ� ���� ��å���� ������ �ִ��� üũ..
    if( !CheckRight(GuildGuid,OperCharGuid,eGUILD_RIGHTS_RANKUP) )
        return RC::RC_GUILD_HAVE_NOT_RIGHT;

	// ��� ���� ��� ���¸� ��ũ���� �� ����.
	if(eGUILD_DELCHK_DESTROY_WAIT == pGuild->CheckGuildDeleteState())
		return RC::RC_GUILD_DELCHK_DESTROY_WAIT;;

	return pGuild->CanRankUp(  RankInfo );
}


VOID GuildManager::RankUpGuild( GUILDGUID GuildGuid )
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild )
	{
		SUNLOG( eCRITICAL_LOG, "[GuildManager::RankUpGuild] �Ͼ���� �ȵǴ� ���� �Ͼ��. GuildGuid = %u", GuildGuid );
		return;
	}

	pGuild->RankUp();
}

VOID		GuildManager::SetGuildTransaction( GUILDGUID GuildGuid, eGUILD_TRANSACTION tran )
{
	Guild *pGuild = FindGuild(GuildGuid);
	if(!pGuild)
	{
		SUNLOG( eCRITICAL_LOG, "[GuildManager::SetGuildTransaction] Caution!!! Ʈ����� �Ŵµ� GuildGuid�� ����.. Guid = %d", GuildGuid );
		return;
	}
}

BOOL		GuildManager::IsGuildTransaction( GUILDGUID GuildGuid, eGUILD_TRANSACTION trans )
{
	Guild *pGuild = FindGuild(GuildGuid);

	//��带 ã�� ���ϸ� � �۾��� �ϸ� �ȵȴ�. ���� ���� Ʈ����� ���̶�� �����Ѵ�.
	if(!pGuild)	return TRUE;

	return TRUE;
}

// ���� ���� ó��
VOID GuildManager::OnChangeNoticeGuild( BOOL bSuccess, GUILDGUID GuildGuid, TCHAR * OUT ptszChangedNotice )
{
	Guild *pGuild = FindGuild( GuildGuid );
	ASSERT(pGuild);
}


// #���� ī������ ����#
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

	// ��� �·�
	float fWinGuildWinRatio = 1 / ( 1.0f + pow( 10.0f, (pLosePVPInfo->m_CHuntingPVPPoint-pWinPVPInfo->m_CHuntingPVPPoint) / 400.0f ) );
	float fLoseGuildWinRatio = 1 / ( 1.0f + pow( 10.0f, (pWinPVPInfo->m_CHuntingPVPPoint-pLosePVPInfo->m_CHuntingPVPPoint) / 400.0f ) );

	// ���� K ����
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

	// ������Ʈ�� �����Ѵ�.
	if( DisGuildGuid )
	{
		// Dis�� ��� �й����� ��캸�� 2�踦 ������Ų��.
		pLosePVPInfo->m_CHuntingPVPPoint -= (int)( LoseGuildK * fLoseGuildWinRatio * 2 );
		pLosePVPInfo->m_CHuntingPVPPoint = max( 0, pLosePVPInfo->m_CHuntingPVPPoint );

		// ������ �����Ѵ�.
		pWinPVPInfo->m_CHuntingTotCount++;
		pLosePVPInfo->m_CHuntingTotCount++;
		pLosePVPInfo->m_CHuntingDisCount++;
	}
	else
	{
		pWinPVPInfo->m_CHuntingPVPPoint += (int)( WinGuildK * ( 1 - fWinGuildWinRatio ) );
		pLosePVPInfo->m_CHuntingPVPPoint -= (int)( LoseGuildK * fLoseGuildWinRatio );
		pLosePVPInfo->m_CHuntingPVPPoint = max( 0, pLosePVPInfo->m_CHuntingPVPPoint );

		// ������ �����Ѵ�.
		pWinPVPInfo->m_CHuntingTotCount++;
		pWinPVPInfo->m_CHuntingVCount++;
		pLosePVPInfo->m_CHuntingTotCount++;
		pLosePVPInfo->m_CHuntingLCount++;
	}

	return RC::RC_GUILD_SUCCESS;
}
#endif

//��� ��å ����
	BOOL		GuildManager::ChangeMemberDuty( GUILDGUID GuildGuid, CHARGUID TargetCharGuid, eGUILD_DUTY eDuty)
	{
		//���⿡ ������ ���� DB�� �ش� ������ ���ο� ��å���� ����� ���̴�. ���� ������ �����̾�� �Ѵ�.
		//DB���� �� ���� ���� ���ϸ� --;; ��å ����.
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
		//0. GUID üũ
		if( OperGuid == TargetGuid )
			return RC::RC_GUILD_CANNOT_COMMAND_MYSELF;

		//1. ����ϴ� ��� �������� ���� ���� ��� ���� üũ.
		Guild* pGuild = FindGuild(GuildGuid);
		if(!pGuild)	return RC::RC_GUILD_INVALID_STATE;

		GuildMember* pOper = pGuild->FindMember( OperGuid );
		if( !pOper )	return RC::RC_GUILD_NOT_GUILD_MEMBER;	

		GuildMember* pTarget = pGuild->FindMember( TargetGuid );
		if( !pTarget )	return RC::RC_GUILD_NOT_GUILD_MEMBER;	

		//2. ����ϴ� ���� ��å���� ������ �ִ��� üũ..
		if( !CheckRight( GuildGuid, OperGuid, eGUILD_RIGHTS_CHANGE_DUTY ) )
			return RC::RC_GUILD_HAVE_NOT_RIGHT;

		//3. ��å �ٲٷ��� ���� ���� ��å üũ
		if( ChangeDuty == pTarget->GetDuty() )
			return RC::RC_GUILD_CANNOT_CHANGE_DUTY_BECAUSE_SAME_DUTY;

		//9. ������å�� ����� ������å�� ����� ��å�� ������ �� ����.( ���ڰ� ���� ���� ���� ��å �̴�. )
		if( pOper->GetDuty() >= pTarget->GetDuty() )
			return RC::RC_GUILD_HAVE_NOT_RIGHT;

		//10. ������ ���� ��å���� ���� �� �� ����.
		if( pOper->GetDuty() >= ChangeDuty )
			return RC::RC_GUILD_HAVE_NOT_RIGHT;

		//4. �����ͳ� ������ ������ �� �Լ��� üũ ���� �ʴ´�.
		if( ChangeDuty == eGUILD_DUTY_MASTER || ChangeDuty == eGUILD_DUTY_COMMANDER )
			return RC::RC_GUILD_CHANGE_MEMBER_DUTY_FAILED;

		//5. �θ����Ϳ� ����� �Ѹ� �����ؾ� �Ѵ�. �̹� ���� �ϴµ� �����Ϸ��� ���� �ȵȴ�.
		// ���� ����� ���� ��忡 �־�߸� �Ѵ�.
		if( ChangeDuty == eGUILD_DUTY_SUBMASTER || ChangeDuty == eGUILD_DUTY_STRATEGIST )
		{
			if( pGuild->IsExistMemberDuty( ChangeDuty, 0 ) )
				return RC::RC_GUILD_ALREADY_EXIST_DUTY;

			if( pTarget->GetCorpsNumber() != 0 )
				return RC::RC_GUILD_CHANGE_MEMBER_DUTY_FAILED;
		}

		//�������̰ų� �Ʒô����� ���� ����� ������ ���.
		if( pOper->GetDuty() == eGUILD_DUTY_COMMANDER || pOper->GetDuty() == eGUILD_DUTY_CAMP_LEADER )
		{
			BYTE byOperCorpsNum = pOper->GetCorpsNumber();

			// ����� ������ ����� ���� ������ or �Ʒô��� ���̴�.���� �������� ���ܰ� ����� ������ ���ƾ� �Ѵ�.
			if( byOperCorpsNum != pTarget->GetCorpsNumber() )
				return RC::RC_GUILD_CORPS_NOT_FOUND;

			// ���ұ����� �������̳�, �Ʒô����� �������� �ʴ´�.
			if( byOperCorpsNum == 0 )		
				return RC::RC_GUILD_INVALID_DUTY_IN_BASE_CORPS;

            // ���� �����ϴ��� üũ.
            GuildCorps* pOperCorps = pGuild->FindCorps( byOperCorpsNum );
            if( !pOperCorps )		return RC::RC_GUILD_CORPS_NOT_FOUND;

            // ��å�� �ٲٷ��� ����� ���ܳ��� ���� �ؾ� �Ѵ�.
            if( !pOperCorps->FindMember( pTarget->GetCharGuid() ) )
                return RC::RC_GUILD_NOT_GUILD_MEMBER;	
        }

        // ��� �����ʹ� ����ķ�� ��� �Ʒô��� ��å ���� ������ ���� ���� [8/5/2009 lst1024] 
        // �Ʒô��� ���� üũ ��ġ �̵�
        
        if( ChangeDuty == eGUILD_DUTY_CAMP_LEADER || ChangeDuty == eGUILD_DUTY_CAMP_SOLDIER )
        {
            BYTE byOperCorpsNum = pTarget->GetCorpsNumber();

            // ���ұ����� ����ķ���� ����.
            if( byOperCorpsNum == 0 )		
                return RC::RC_GUILD_NOT_EXIST_REAR_CAMP;

            // ���� �����ϴ��� üũ.
            GuildCorps* pOperCorps = pGuild->FindCorps( byOperCorpsNum );
            if( !pOperCorps )		
                return RC::RC_GUILD_CORPS_NOT_FOUND;

            // �Ʒô����̳� �Ʒú����� �ٲٷ��� ���� ����ķ���� Ȱ��ȭ �Ǿ� �־�� �Ѵ�.
            if( !pOperCorps->IsExistRearCamp() )
                return RC::RC_GUILD_NOT_EXIST_REAR_CAMP;

            // �Ʒú��̳� �Ʒô������� �ٲܶ��� ���� ��å�� �������̿��� �Ѵ�.
            if( pTarget->GetDuty() != eGUILD_DUTY_REGULAR_SOLDIER )
                return RC::RC_GUILD_NOT_SOLDIER;
        }

        // �Ʒú� ���� üũ... 16���� ���� ũ�� �Ʒú����� �������� ���Ѵ�.
        if( ChangeDuty == eGUILD_DUTY_CAMP_SOLDIER )
        {
            if( pTarget->GetLevel() >= 16 )
                return RC::RC_GUILD_CANT_CHANGE_CAMP_SOLDIER_BY_LEVEL;
        }

		// �Ʒú� ������ �������̳� �Ʒô��常 �� �� �ִ�.
		if( ChangeDuty == eGUILD_DUTY_CAMP_SOLDIER )
		{
			if( pOper->GetDuty() != eGUILD_DUTY_COMMANDER && pOper->GetDuty() != eGUILD_DUTY_CAMP_LEADER )
				return RC::RC_GUILD_HAVE_NOT_RIGHT; 
		}

		//�ο� üũ �Ѵ�.
		if( pGuild->IsFullCorpsMemberByDuty( pTarget->GetCorpsNumber(), ChangeDuty ) )
			return RC::RC_GUILD_CANNOT_CHANGE_DUTY_BECAUSE_FULL_MEMEBER;

		return RC::RC_GUILD_SUCCESS;
	}

	//�ش� ������ �ش� ������ ������ �� �ִ��� üũ�Ѵ�.
	BOOL		GuildManager::CheckRight( GUILDGUID GuildGuid, CHARGUID CharGuid, eGUILD_RIGHTS Right )
	{
		//���� ����üũ ���μ�����
		//	1) ��� ��޿� �ش� ������ �ִ���( ��ũ��Ʈ�� )
		//	2) ������ ����Ϸ��� ������ ��å�� �ش� ������ �ִ��� üũ.
		//	- ������ ��å�� ���� ��� ��޿� ���ԵǾ� �ִ��� üũ.
		//	- �������� ���ӵ� ������ �ִ��� üũ.( ���ӵ� ������ ��ũ��Ʈ�ʹ� ������ �����ؾ� �Ѵ�. ������ + ��)
		//	���� ���⼭ ������ �ִٰ� �ǴܵǸ�, �Ʒ��� üũ�� ���� �ʴ´�.
		//	- ������ ��å�� ���� ������ �ִ��� üũ.

		Guild *pGuild = FindGuild(GuildGuid);
		if(!pGuild)	return FALSE;

		GuildMember* pMember = pGuild->FindMember( CharGuid );
		if( !pMember )	return FALSE;
		
		//1. ���� ��ũ��Ʈ ���� ��� ��޺� ������ üũ�Ѵ�.
		if( !GuildRightInfoParser::Instance()->CheckGradeRight( Right, pGuild->GetGuildGrade() ) )
			return FALSE;

		//2. ���� ������ ��å�� ���� ����޿� ��ϵǾ� �ִ� ������ üũ�Ѵ�.
		GuildRankInfo* pGuildRankInfo = GuildRankInfoParser::Instance()->FindGuildRankInfo( pGuild->GetGuildGrade() );
		if( !pGuildRankInfo )
			return FALSE;

		GUILD_DUTY_USE_INFO* pDutyUseInfo = pGuildRankInfo->Find( pMember->GetDuty() );
		if( !pDutyUseInfo || !pDutyUseInfo->m_bUse )
			return FALSE;

		// ���� �����Ϳ� ���翡�Ը� ���ӵ� ������ �����Ѵ�. 
		if( pMember->GetDuty() == eGUILD_DUTY_SUBMASTER || pMember->GetDuty() == eGUILD_DUTY_STRATEGIST )
		{
			//3. �������� ���ӹ��� ������ �ִ��� üũ.
			if( pMember->CheckDelegateRight( Right ) )
				return TRUE;
		}

		//3. ���ӹ��� ������ ������ ��ũ��Ʈ �󿡼� ��å�� ������ �ִ��� üũ�Ѵ�.
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
		// ���������� �θ����ͳ�, ���翡�Ը� �� �� �ִ�.
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

	//1. ������ �����ʹ� ������ �ǰ�, ���ο� �������� ��å�� �����ͷ� �����Ѵ�
    const eGUILD_DUTY prev_duty_of_new_master = pNewMaster->GetDuty();
	pOldMaster->SetDuty( eGUILD_DUTY_REGULAR_SOLDIER );
	pNewMaster->SetDuty( eGUILD_DUTY_MASTER );

	//2. ���ӵ� ������ �Ѵ� �ʱ�ȭ ��Ű��.
	pOldMaster->InitDelegateRight();
	pNewMaster->InitDelegateRight();

	//�ð��� �����ؾ� ���� ������?
	pGuild->SetMasterChangeTime( nMasterChangeTime );

    // ��帶���� ���濡 ���� ��� ��å ���� �α� �߰�
    GUILDLOG->LogDutyChange(pGuild, pOldMaster, eGUILD_DUTY_MASTER);
    GUILDLOG->LogDutyChange(pGuild, pNewMaster, prev_duty_of_new_master);

	return TRUE;
}

RC::eGUILD_RESULT	GuildManager::CanChangeMaster( GUILDGUID GuildGuid, CHARGUID OldMasterCharGuid , TCHAR* pszNewMasterName )
{
	//1. ��尡 �ִ��� üũ
	Guild* pGuild = g_GuildManager.FindGuild( GuildGuid );
	if( !pGuild )
		return RC::RC_GUILD_NOT_EXIST_GUILD;

	// �Ѵ� ��� ������� üũ
	GuildMember* pMaster = pGuild->FindMember( OldMasterCharGuid );
	if( !pMaster )			return RC::RC_GUILD_NOT_GUILD_MEMBER;

	GuildMember* pNewMaster = pGuild->FindMemberByCharName( pszNewMasterName );
	if( !pNewMaster )		return RC::RC_GUILD_NOT_GUILD_MEMBER;

	// ��å �� ( �����Ͱ� �ƴϸ� �ȵȴ�. )
	if( pMaster->GetDuty() != eGUILD_DUTY_MASTER || pMaster->GetCorpsNumber() != 0 )
		return RC::RC_GUILD_HAVE_NOT_RIGHT;

	// �̹� ������ �ε� �����Ͱ� �Ǹ� �ȵȴ�
	if( pNewMaster->GetDuty() == eGUILD_DUTY_MASTER )
		return RC::RC_GUILD_HAVE_NOT_RIGHT;

	// ���ұ�尡 �ƴϸ� �ȵȴ�.
	if( pNewMaster->GetCorpsNumber() != 0 )
		return RC::RC_GUILD_NOT_BASE_CORPS;

	return RC::RC_GUILD_SUCCESS;			 

}

RC::eGUILD_RESULT	GuildManager::CanRightDelegate( GUILDGUID GuildGuid, CHARGUID OperCharGuid, TCHAR* pszTargetCharName, const GUILD_RIGHT_USE_INFO* pRightInfo )
{
	//1. ��尡 �ִ��� üũ
	Guild* pGuild = g_GuildManager.FindGuild( GuildGuid );
	if( !pGuild )
		return RC::RC_GUILD_NOT_EXIST_GUILD;

	//2. Ÿ�ϴ���� �ִ��� Ȯ���Ѵ�.
	GuildMember* pTargetMember = pGuild->FindMemberByCharName( pszTargetCharName );
	if( !pTargetMember )
		return RC::RC_GUILD_NOT_GUILD_MEMBER;

	//3. ����Ʈ ��û�ڰ� ����Ʈ�� ��û�ص� �Ǵ� ������� üũ�Ѵ�.
	if( !g_GuildManager.CheckRight( GuildGuid, OperCharGuid, eGUILD_RIGHTS_DELEGATION_RIGHT ) )
		return RC::RC_GUILD_HAVE_NOT_RIGHT;

	//4. �������Ӵ���� ���Ӵ���� �Ǵ��� üũ.
	RC::eGUILD_RESULT rc = g_GuildManager.IsDelegateChar( GuildGuid, pTargetMember->GetCharGuid() );	
	if( rc != RC::RC_GUILD_SUCCESS )
		return rc;

	//4. ���� �����Ϸ��� ���ѵ��� ��Ȯ�� ���ӱ������� üũ.
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

// ��� ���� ����
RC::eGUILD_RESULT GuildManager::CreateCorps( GUILDGUID GuildGuid, BYTE byCorpsNum, CHARGUID TargetCharGuid )
{
	Guild* pGuild = FindGuild(GuildGuid);
	if(!pGuild)	return RC::RC_GUILD_NOT_EXIST_GUILD;

	GuildCorps* pCorps = pGuild->CreateCorps( byCorpsNum );
	if( !pCorps ) return RC::RC_GUILD_CREATE_CORPS_FAILED;

	//������ �Ӹ�
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
	// ����üũ..
	//1. ��尡 �ִ��� üũ
	Guild* pGuild = g_GuildManager.FindGuild( GuildGuid );
	if( !pGuild )
		return RC::RC_GUILD_NOT_EXIST_GUILD;

	//2. ���ܻ��� ��û�ڰ� �ش����� ��û�ص� �Ǵ� ������� üũ�Ѵ�.
	if( !g_GuildManager.CheckRight( GuildGuid, OperCharGuid, eGUILD_RIGHTS_CREATE_CORPS ) )
		return RC::RC_GUILD_HAVE_NOT_RIGHT;

	//3. ���� ���� â�� ������ üũ�Ͽ� â�� �������� Ȯ���Ѵ�.
	GuildRankInfo* pRankInfo = GuildRankInfoParser::Instance()->FindGuildRankInfo( pGuild->GetGuildGrade() );
	if( !pRankInfo )
		return RC::RC_GUILD_NOT_FOUND_GUILD_INFO_FOR_GRADE;

	byCorpsNum = pGuild->GetEmptyCorpsNumBySequence();
	if( byCorpsNum == 0 )
		return RC::RC_GUILD_CORPS_NUM_OVERFLOW;

	// �������� �� ����� ���� ��� ������� Ȯ���Ѵ�.
	GuildMember* pTargetMember = pGuild->FindMemberByCharName( pszTargetCharName );
	if( !pTargetMember )
		return RC::RC_GUILD_NOT_GUILD_MEMBER;

	// ������ �� ����� ���� �������� üũ
	if( pTargetMember->GetCorpsNumber() != 0 )
		return RC::RC_GUILD_NOT_BASE_CORPS;

	// ������ �� ����� ���������� üũ.
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

	//1. ���� �����̸� �ȵȴ�.
	if( byCorpsNum == 0 ) 
		return RC::RC_GUILD_INVALID_FUNCTION_IN_BASE_CORPS;

	//2. ����üũ.
	if( !CheckRight( GuildGuid, OperCharGuid, eGUILD_RIGHTS_DISMISS_CORPS ) )
		return RC::RC_GUILD_HAVE_NOT_RIGHT;

	GuildCorps* pCorps = pGuild->FindCorps( byCorpsNum );
	if( !pCorps )			return RC::RC_GUILD_CORPS_NOT_FOUND;

	// ���� �ػ� ����
	// 1. ���ܳ��� �ο��� ����� �Ѵ�.
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

	//�ش� ����� �������� �ƴϸ� �ɼ� ����.
	if( pMember->GetDuty() != eGUILD_DUTY_REGULAR_SOLDIER )
		return RC::RC_GUILD_MEMBER_NOT_SOLDIER;

	//�ش� ����� ���ұ�� ���� üũ�Ѵ�.
	if( pMember->GetCorpsNumber() != 0 )  //���ұ�尡 �ƴϸ�..
		return RC::RC_GUILD_MEMBER_NOT_HEAD_CORPS;

	return  RC::RC_GUILD_SUCCESS;
}

RC::eGUILD_RESULT	GuildManager::CanCreateGuild( CHARGUID MasterCharGuid, LEVELTYPE MasterLV  )
{
	// ������ ��� �̸��� ������尡 �����ϴ� ����, DB���� üũ �ϵ��� �Ѵ�.

	//1. ���� ��忡 ���� �Ǿ� �ִ��� üũ
	//GuildMember* pMember = FindMember( MasterCharGuid );
	//if( pMember )
	//	return RC::RC_GUILD_ALREADY_GUILD_MEMBER;

	//2. �����̹Ƿ� Legion�� ������ ������ �´�.
	GuildRankInfo* pRankInfo = GuildRankInfoParser::Instance()->FindGuildRankInfo( eGUILD_GRADE_LEGION );
	if( !pRankInfo )
		return RC::RC_GUILD_NOT_FOUND_GUILD_INFO_FOR_GRADE;

	//3. ��帶���� ���� üũ 
	if( MasterLV < CREATE_GUILD_LIMIT_LEVEL )
		return RC::RC_GUILD_CANNOT_CREAT_LEVEL_STATE;

	//4. ����/ī������ üũ �ؾ� �ϴµ�... �ش� ������ ���⼭ �˼� ����. ���Ӽ������� ���� üũ �ϰ� �´�.
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

	//��� ��ü�� �����͸��� �� �� �ִ�.
	GuildMember* pMaster = pGuild->FindMaster();
	if( !pMaster )	return	RC::RC_GUILD_CURRENT_NONE_MASTER;

	if( pMaster->GetCharGuid() != OperCharGuid )
		return RC::RC_GUILD_HAVE_NOT_RIGHT;

	// ��� ���� ��� ���¸� Destroy�� �� ����.
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

	//���� ������ �������� ����.
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

	//���� ������ �������� ������ ������ ���� ���Ѵ�.
	GuildCorps* pCorps = pGuild->FindCorps( byCorpsNum );
	if( !pCorps )		//������ �������� �ʴ´�.
		return RC::RC_GUILD_CORPS_NOT_FOUND;

	if( pCorps->FindCommander() )		//�̹� �������� �����ϸ�.. 
		return RC::RC_GUILD_ALREADY_EXIST_COMMANDER;

	// ���� ������ ��Ű���� ���� ��å�� �������̸� �ȵȴ�.
	if( pTargetMember->GetDuty() == eGUILD_DUTY_COMMANDER )
		return RC::RC_GUILD_CANT_CHANGE_COMMANDER_BECAUSE_COMMANDER;

	//�������� 1���̹Ƿ� �ο��� üũ ���Ѵ�.

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

	//1. ������ �ٲ۴�.
	RC::eGUILD_RESULT rt =pGuild->ChangeCorps( CommanderGuid, byCorpsNum );
	if( rt != RC::RC_GUILD_SUCCESS )
		return rt;

	//2. ��å�� �ٲپ� �ְ�
	pCommander->SetDuty( eGUILD_DUTY_COMMANDER );

	return RC::RC_GUILD_SUCCESS;
}

// ���� ����
RC::eGUILD_RESULT  	GuildManager::CanChangeCorps( GUILDGUID GuildGuid, CHARGUID OperCharGuid, const TCHAR* tszMasterCharName, BYTE byCorpsNum )
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return RC::RC_GUILD_NOT_EXIST_GUILD;

	GuildMember* pOperMember = pGuild->FindMember( OperCharGuid );
	if( !pOperMember )			return RC::RC_GUILD_NOT_GUILD_MEMBER;

	GuildMember* pTargetMember = pGuild->FindMemberByCharName( tszMasterCharName );
	if( !pTargetMember )			return RC::RC_GUILD_NOT_GUILD_MEMBER;

	// �ű���� ������ ��å üũ ( �ݵ�� ������ �̾�� �Ѵ�. )
	if( pTargetMember->GetDuty() != eGUILD_DUTY_REGULAR_SOLDIER )
		return RC::RC_GUILD_NOT_SOLDIER;

	if( !CheckRight( GuildGuid, OperCharGuid, eGUILD_RIGHTS_CHANGE_CORPS ) )
		return RC::RC_GUILD_HAVE_NOT_RIGHT;

	//�����Ϸ��� ���ܰ� ���� Ÿ�������� ���� ��ȣ�� ������ �ȵȴ�. )
	if( pTargetMember->GetCorpsNumber() == byCorpsNum )
		return RC::RC_GUILD_CANNOT_CHANGE_CORPS_BECAUSE_SAME_CORPS;

	//���� Ÿ�������� �ű���� ������ �����ؾ� �Ѵ�.
	if( !pGuild->FindCorps( pTargetMember->GetCorpsNumber() ) )
		return RC::RC_GUILD_CORPS_NOT_FOUND;

	GuildCorps* pTargetCorps = pGuild->FindCorps( byCorpsNum );
	if( !pTargetCorps )			return RC::RC_GUILD_CORPS_NOT_FOUND;

	//�ο� üũ �Ѵ�.
	if( pGuild->IsFullCorpsMemberByDuty( byCorpsNum, pTargetMember->GetDuty() ) )
		return RC::RC_GUILD_CORPS_FULL_MEMBER;

	return RC::RC_GUILD_SUCCESS;
}

//���ܰ� ����
RC::eGUILD_RESULT	GuildManager::ChangeCorps( GUILDGUID GuildGuid, CHARGUID TargetCharGuid, BYTE byCorpsNum )
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return RC::RC_GUILD_NOT_EXIST_GUILD;

	return pGuild->ChangeCorps( TargetCharGuid, byCorpsNum );
}

//���� ���� ķ�� ����
RC::eGUILD_RESULT  	GuildManager::CanCreateRearCamp( GUILDGUID GuildGuid, CHARGUID OperCharGuid, const TCHAR* pszTargetCharName, OUT BYTE& byCorpsNum )
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return RC::RC_GUILD_NOT_EXIST_GUILD;

	//��� Ȯ��
	GuildMember* pOperMember = pGuild->FindMember( OperCharGuid );
	if( !pOperMember )			return RC::RC_GUILD_NOT_GUILD_MEMBER;

	//1. ����üũ.
	if( !CheckRight( GuildGuid, OperCharGuid, eGUILD_RIGHTS_CREATE_CAMP ) )
		return RC::RC_GUILD_HAVE_NOT_RIGHT;

	//2. �������� Ȯ��
	GuildCorps* pCorps = pGuild->FindCorps( pOperMember->GetCorpsNumber() );
	if( !pCorps )			return RC::RC_GUILD_CORPS_NOT_FOUND;

	//3. �Ʒô��� �� ���� ���� �������� Ȯ��.
	GuildMember* pTargetMember = pCorps->FindMember( pszTargetCharName );
	if( !pTargetMember )		return RC::RC_GUILD_NOT_CORPS_MEMBER;

	//4. ��å�� ���������� Ȯ��..
	if( pOperMember->GetDuty() != eGUILD_DUTY_COMMANDER )
		return RC::RC_GUILD_HAVE_NOT_RIGHT;

	//5. Ÿ�ٸ���� ���������� Ȯ��.
	if( pTargetMember->GetDuty() != eGUILD_DUTY_REGULAR_SOLDIER )
		return RC::RC_GUILD_HAVE_NOT_RIGHT;

	//6. ���� ����ķ���� �ִ��� üũ.
	if( pCorps->IsExistRearCamp() )
		return RC::RC_GUILD_ALREADY_EXIST_REAR_CAMP;

	//���ܹ�ȣ ����..
	byCorpsNum = pOperMember->GetCorpsNumber();

	return RC::RC_GUILD_SUCCESS;
}

RC::eGUILD_RESULT  	GuildManager::CreateRearCamp( GUILDGUID GuildGuid, BYTE byCorpsNum, CHARGUID TargetCharGuid )
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return RC::RC_GUILD_NOT_EXIST_GUILD;

	if( !pGuild->CreateRearCamp( byCorpsNum ) )
		return RC::RC_GUILD_FAILED;

	//1. ����� ���ܺ���
	RC::eGUILD_RESULT rc = ChangeCorps( GuildGuid, TargetCharGuid, byCorpsNum );
	if( rc != RC::RC_GUILD_SUCCESS )
		return rc;

	//2. ����� ��å����( �Ʒô��� )
	GuildMember* pMember = FindGuildMember( GuildGuid, TargetCharGuid );
	if( !pMember )	return RC::RC_GUILD_NOT_GUILD_MEMBER;

	pMember->SetDuty( eGUILD_DUTY_CAMP_LEADER );

	GUILDLOG->LogCampCreation(pGuild, pMember, byCorpsNum);

	return RC::RC_GUILD_SUCCESS;
}

// ���� �ػ��� �����常�� �� �� �����Ƿ� ���ܹ�ȣ�� �ʿ� ������ ������,
// ���߿� �渶�� �Ǵ� �������� �� �� �������� �𸥴�. 
// �׷��� ���ܹ�ȣ�� ����.
RC::eGUILD_RESULT  	GuildManager::CanDestroyRearCamp( GUILDGUID GuildGuid, CHARGUID OperCharGuid, BYTE byCorpsNum )
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return RC::RC_GUILD_NOT_EXIST_GUILD;

	GuildMember* pOperMember = pGuild->FindMember( OperCharGuid );
	if( !pOperMember )			return RC::RC_GUILD_NOT_GUILD_MEMBER;

	//1. ���� �����̸� �ȵȴ�.
	if( byCorpsNum == 0 ) 
		return RC::RC_GUILD_INVALID_FUNCTION_IN_BASE_CORPS;

	//2. ����üũ.
	if( !CheckRight( GuildGuid, OperCharGuid, eGUILD_RIGHTS_DISMISS_CAMP ) )
		return RC::RC_GUILD_HAVE_NOT_RIGHT;

	// �Ʒ�ķ�� �ػ� ����
	// 1. �Ʒô����̶� �Ʒú��� ����� �Ѵ�.
	GuildCorps* pCorps = pGuild->FindCorps( byCorpsNum );
	if( !pCorps )			return RC::RC_GUILD_CORPS_NOT_FOUND;

	// ���� �Ʒ�ķ���� �ִ��� üũ.
	if( pCorps->IsExistRearCamp() == FALSE )
		return RC::RC_GUILD_NOT_EXIST_REAR_CAMP;

	// �Ʒô����� ������ �ȵȴ�.
	if( pCorps->GetMemberCountByDuty( eGUILD_DUTY_CAMP_LEADER ) > 0 )
		return RC::RC_GUILD_EXIST_MEMBER;

	// �Ʒú��� ������ �ȵȴ�.
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

//����ʴ밡 �����Ѱ�?
RC::eGUILD_RESULT	GuildManager::CanInviteGuild( const GUILDGUID GuildGuid, const CHARGUID OperCharGuid, const CHARGUID TargetCharGuid, const LEVELTYPE TargetLevel )
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return RC::RC_GUILD_NOT_EXIST_GUILD;

	// ����üũ .
	if( !CheckRight( GuildGuid, OperCharGuid, eGUILD_RIGHTS_JOIN ) )
		return RC::RC_GUILD_HAVE_NOT_RIGHT;

	// ��� ���� ��� ���¸� Join�� �� ����.
	if(eGUILD_DELCHK_DESTROY_WAIT == pGuild->CheckGuildDeleteState())
		return RC::RC_GUILD_DELCHK_DESTROY_WAIT;

	//��尡 ���� �г�Ƽ�� ������ �ް� �ִ� ��������
	if(!GuildSmallContentsEntry::GuildPenalty::CheckGuildPenaltyTime(pGuild))
		return RC::RC_GUILD_WITHDRAW_PENALTY_TIME;

	// TODO : ���� �г�Ƽ�� üũ..����� DB������ üũ�ϰ� ����

	// �ش� ����� �ٸ� �����̸� �������� ���Ѵ�. 
	// �̰� ó�� ��� ���� ���ΰ�? DB���� ������.. ���⼭�� �ش� ��忡 ���������� üũ�Ѵ�.
	// �� ���� ���Ӽ������� �ش� ĳ���Ϳ� ���� ��� üũ�� �ϰ� ���� �´�.

	GuildMember* pOperMember = pGuild->FindMember( OperCharGuid );
	if( !pOperMember )	return RC::RC_GUILD_NOT_GUILD_MEMBER;

	GuildMember* pTargetMember = pGuild->FindMember( TargetCharGuid );
	if( pTargetMember )	return RC::RC_GUILD_ALREADY_GUILD_MEMBER;

	//���Խ�Ų���� �Ʒô����̰�,
	eGUILD_DUTY Duty = eGUILD_DUTY_REGULAR_SOLDIER;
	if( pOperMember->GetDuty() == eGUILD_DUTY_CAMP_LEADER )
	{
		if( TargetLevel >= 16 )
			return RC::RC_GUILD_CANNOT_JOIN_CAMP_SOLDERY_BY_LEVEL;

		Duty = eGUILD_DUTY_CAMP_SOLDIER;
	}

	//3. ���ܰ���( ���Խ�Ű���� ���� �������̳�, �Ʒô����̸� ���� ���ܿ� �����ϴ°Ŵ�. )
	BYTE byCorpsNum = 0;
	if( pOperMember->GetDuty() == eGUILD_DUTY_CAMP_LEADER || pOperMember->GetDuty() == eGUILD_DUTY_COMMANDER )
		byCorpsNum = pOperMember->GetCorpsNumber();

	//�ο� üũ �Ѵ�.
	if( pGuild->IsFullCorpsMemberByDuty( byCorpsNum, Duty ) )
		return RC::RC_GUILD_CORPS_FULL_MEMBER;

	return RC::RC_GUILD_SUCCESS;
}


RC::eGUILD_RESULT	GuildManager::CanJoinGuild( const GUILDGUID& GuildGuid, const CHARGUID& OperCharGuid, const CHARGUID& TargetCharGuid, const LEVELTYPE& TargetLevel, OUT eGUILD_DUTY& Duty, OUT BYTE& byCorpsNum )
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return RC::RC_GUILD_NOT_EXIST_GUILD;

	// ����üũ .
	if( !CheckRight( GuildGuid, OperCharGuid, eGUILD_RIGHTS_JOIN ) )
		return RC::RC_GUILD_HAVE_NOT_RIGHT;

	// ��� ���� ��� ���¸� Join�� �� ����.
	if(eGUILD_DELCHK_DESTROY_WAIT == pGuild->CheckGuildDeleteState())
		return RC::RC_GUILD_DELCHK_DESTROY_WAIT;

	if(!GuildSmallContentsEntry::GuildPenalty::CheckGuildPenaltyTime(pGuild))
		return RC::RC_GUILD_KICK_PENALTY_TIME;

	// �ش� ����� �ٸ� �����̸� �������� ���Ѵ�. 
	// �̰� ó�� ��� ���� ���ΰ�? DB���� ������.. ���⼭�� �ش� ��忡 ���������� üũ�Ѵ�.
	// �� ���� ���Ӽ������� �ش� ĳ���Ϳ� ���� ��� üũ�� �ϰ� ���� �´�.
	GuildMember* pTargetMember = pGuild->FindMember( TargetCharGuid );
	if( pTargetMember )	return RC::RC_GUILD_ALREADY_GUILD_MEMBER;

	//2. Ÿ�ٸ���� ��å�� �����Ѵ�.
	GuildMember* pOperMember = pGuild->FindMember( OperCharGuid );
	if( !pOperMember )	return RC::RC_GUILD_NOT_GUILD_MEMBER;

	//���Խ�Ų���� �Ʒô����̰�,
	Duty = eGUILD_DUTY_REGULAR_SOLDIER;
	if( pOperMember->GetDuty() == eGUILD_DUTY_CAMP_LEADER )
	{
		if( TargetLevel >= 16 )
			return RC::RC_GUILD_CANNOT_JOIN_CAMP_SOLDERY_BY_LEVEL;

		Duty = eGUILD_DUTY_CAMP_SOLDIER;
	}

	//3. ���ܰ���( ���Խ�Ű���� ���� �������̳�, �Ʒô����̸� ���� ���ܿ� �����ϴ°Ŵ�. )
	byCorpsNum = 0;
	if( pOperMember->GetDuty() == eGUILD_DUTY_CAMP_LEADER || pOperMember->GetDuty() == eGUILD_DUTY_COMMANDER )
		byCorpsNum = pOperMember->GetCorpsNumber();

	//�ο� üũ �Ѵ�.
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

// #���Ż��#
RC::eGUILD_RESULT	GuildManager::CanLeaveGuild( GUILDGUID GuildGuid, CHARGUID CharGuid )
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return RC::RC_GUILD_NOT_EXIST_GUILD;
	
	// ��� ���� ��� ���¸� Ż���� �� ����.
	if(eGUILD_DELCHK_DESTROY_WAIT == pGuild->CheckGuildDeleteState())
		return RC::RC_GUILD_DELCHK_DESTROY_WAIT;

	//1. �ش� ����� �����̳�?
	GuildMember* pMember = pGuild->FindMember( CharGuid );
	if( !pMember )	return RC::RC_GUILD_NOT_GUILD_MEMBER;

	//2. �ش� ����� �����͸� ����� ���� Ż�� �����ϴ�.
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

	//1. �ش� ����� �����̳�?
	GuildMember* pMember = pGuild->FindMember( CharGuid );
	if( !pMember )	return RC::RC_GUILD_NOT_GUILD_MEMBER;

	GUILDLOG->LogMemberLeave(pGuild, pMember);

	//DestroyMember�� �ٸ� ���ϵ� �ִµ� ������ ��������??
	pGuild->DestroyMember( CharGuid );
	return RC::RC_GUILD_SUCCESS;
}

RC::eGUILD_RESULT	GuildManager::CanKickMember( GUILDGUID GuildGuid, CHARGUID OperCharGuid, TCHAR* ptszCharName )
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return RC::RC_GUILD_NOT_EXIST_GUILD;		

	// ��� ���� ��� ���¸� Kick �� �� ����.
	if(eGUILD_DELCHK_DESTROY_WAIT == pGuild->CheckGuildDeleteState())
		return RC::RC_GUILD_DELCHK_DESTROY_WAIT;

	// ����üũ .
	if( !CheckRight( GuildGuid, OperCharGuid, eGUILD_RIGHTS_KICK ) )
		return RC::RC_GUILD_HAVE_NOT_RIGHT;

	GuildMember* pOperMember = pGuild->FindMember( OperCharGuid );
	if( !pOperMember )	return RC::RC_GUILD_NOT_GUILD_MEMBER;

	GuildMember* pTargetMember = pGuild->FindMemberByCharName( ptszCharName );
	if( !pTargetMember )	return RC::RC_GUILD_NOT_GUILD_MEMBER;

	// �ڱ� �ڽ��� Ż���ų �� ����.
	if( pOperMember->GetCharGuid() == pTargetMember->GetCharGuid() )
		return RC::RC_GUILD_HAVE_NOT_RIGHT;

	//����ں��� ������å�� ������ �� ����.
	if( pOperMember->GetDuty() > pTargetMember->GetDuty() )
		return RC::RC_GUILD_HAVE_NOT_RIGHT;

	if( pTargetMember->GetDuty() == eGUILD_DUTY_MASTER )
		return RC::RC_GUILD_CANNOT_LEAVE_MASTER;

	return RC::RC_GUILD_SUCCESS;
}

// #��尭��#
RC::eGUILD_RESULT	GuildManager::KickMember( GUILDGUID GuildGuid, CHARGUID OperCharGuid, CHARGUID TargetCharGuid )
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return RC::RC_GUILD_NOT_EXIST_GUILD;

    GuildMember* pOperMember = pGuild->FindMember( OperCharGuid );
    if ( !pOperMember ) { return RC::RC_GUILD_NOT_GUILD_MEMBER; }

	GuildMember* pTargetMember = pGuild->FindMember( TargetCharGuid );
    if( !pTargetMember ) { return RC::RC_GUILD_NOT_GUILD_MEMBER; }

	GUILDLOG->LogMemberKickOut(pGuild, pOperMember, pTargetMember);
	
	//DestroyMember�� �ٸ� ���ϵ� �ִµ� ������ ��������??
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

	// ��尡 �����ϸ� ��� ������� Ȯ���Ѵ�.
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

	// ��尡 �����ϸ� ��� ������� Ȯ���Ѵ�.
	GuildMember* pMember = pGuild->FindMember( CharGuid );
	if( !pMember )	return RC::RC_GUILD_NOT_GUILD_MEMBER;

	if( pMember->IsLogIn() == FALSE )
	{
		SUNLOG( eCRITICAL_LOG, "[GuildManager::CanLogin] �̹� �α��� ���� ������ �α��� ���� �����̵� �Ϸ��� �Ѵ�. GuildGuid = %u, CharGuid = %u", GuildGuid, CharGuid );
	}

	return RC::RC_GUILD_SUCCESS;
}


RC::eGUILD_RESULT	GuildManager::Login( GUILDGUID GuildGuid, CHARGUID CharGuid )
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return RC::RC_GUILD_NOT_EXIST_GUILD;

	// ��尡 �����ϸ� ��� ������� Ȯ���Ѵ�.
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

	// ��尡 �����ϸ� ��� ������� Ȯ���Ѵ�.
	GuildMember* pMember = pGuild->FindMember( CharGuid );
	if( !pMember )	return RC::RC_GUILD_NOT_GUILD_MEMBER;

	pMember->SetLogin( FALSE ); 
	pMember->SetTotalInfoReqCnt( 0 );	

	return RC::RC_GUILD_SUCCESS;
}

// UP/GP ��ȯ
// NowUP : ���� ����� UP
// ResultUP : ��ȯ�� UP
// ResultGP : ��ȯ�� GP
RC::eGUILD_RESULT	GuildManager::CanConvertUP2GP( GUILDGUID GuildGuid, CHARGUID OperGuid, GPTYPE ConverGP )
{
	Guild* pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return RC::RC_GUILD_NOT_EXIST_GUILD;

	// ��尡 �����ϸ� ��� ������� Ȯ���Ѵ�.
	GuildMember* pMember = pGuild->FindMember( OperGuid );
	if( !pMember )	return RC::RC_GUILD_NOT_GUILD_MEMBER;

	// ������ �ִ��� üũ�Ѵ�.
	// ����üũ .
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

	// ��尡 �����ϸ� ��� ������� Ȯ���Ѵ�.
	GuildMember* pMember = pGuild->FindMember( CharGuid );
	if( !pMember )	return RC::RC_GUILD_NOT_GUILD_MEMBER;

	if( !CheckRight( GuildGuid, CharGuid, eGUILD_RIGHTS_DONATEUP ) )
		return RC::RC_GUILD_HAVE_NOT_RIGHT;

	// ĳ���Ͱ� ������ UP�� ���Ӽ������� �Ǻ��ϰ� �´�.

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

	//���� ���� Ƚ���� 3ȸ �̻��̸�..
	if( pMember->GetTotalInfoReqCnt() > 3 )
	{
		SUNLOG( eCRITICAL_LOG, "[GuildManager::CanRequestTotalInfo] 3ȸ �̻� ���TOTAL���� ��û��. GuildGuid = %u, CharGuid = %u, CharName = %s",
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
	{// ��� �����͸� ���� ����. (��ũ��Ʈ�� ���ϱ����)
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
	// ���� ��� ������ ������ �ٲ۴�.
	// ��� ������ ���°�� �����ϰ� ������ �ѹ��� ���� ���� �ʾҴٸ�, ��� ������ ����.
	// �󽺸� ���� ���� ��� ���� ���� ���Ҽ� �ִ�.
	Guild* pGuild = FindGuild(guid);
	if( !pGuild )
		return;

	RemoveGuildName( pGuild->GetGuildName() );
	pGuild->ChangeGuildName( pszGuildName );
	AddGuildName( pGuild->GetGuildName(), pGuild->GetGuildGuid() );


	// ��� ���迡 �ִ� ��尡 ���� ��� ���������� �ٲ۴�.
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

