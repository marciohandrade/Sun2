#include "StdAfx.h"
#include ".\guild.h"
#include ".\guildMember.h"
#include <Struct.h>
#include <PacketStruct_DZ.h>
#include <GuildServer.h>
#include <SerializeStream.h>

#include <GuildRankInfoParser.h>
#include <MaxGuildMemberInfoParser.h>

#include <Guild/GuildManager.h>

#ifdef _NA_20100604_BETTER_DOMINATION_MANAGER
    #include "DominationManager.h"
#endif

__IMPL_GUILDPOOL(Guild)
Guild::Guild(void)
{
}

Guild::~Guild(void)
{
}

VOID Guild::InitializeGuild()
{
	m_nMasterChangeTime = 0;
	ZeroMemory( &m_GuildInfo, sizeof(m_GuildInfo) );
	ZeroMemory( m_szGuildNotice, MAX_GUILDNOTICE_LENGTH * sizeof(TCHAR));

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
	memset( &m_ChuntingPVPInfo, 0, sizeof(CHUNTING_PVP_INFO) );
#endif

}

VOID Guild::ReleaseGuild()
{
	m_GuildRelationEntry.RelationRelease();
}

VOID Guild::SetGuildInfo( BASE_GUILD_INFO & IN rInfo )
{
	SerializeBaseGuildInfo( rInfo, SERIALIZE_STORE );
}

GuildMember*		Guild::FindMember( CHARGUID CharGuid )
{
	GUILD_CORPS_MAP_ITER iter;
	for( iter = m_mapGuildCorps.begin(); iter != m_mapGuildCorps.end(); iter++ )
	{
		GuildCorps* pCorps = iter->second;
		if( !pCorps )	continue;

		GuildMember* pMember = pCorps->FindMember( CharGuid );
		if( !pMember )	continue;

		return pMember;
	}

	return NULL;
}

// 이름으로 찾기
GuildMember * Guild::FindMemberByCharName( const TCHAR* ptszCharName )
{
	GUILD_CORPS_MAP_ITER iter;
	for( iter = m_mapGuildCorps.begin(); iter != m_mapGuildCorps.end(); ++iter )
	{
		GuildCorps* pCorps = iter->second;
		if( !pCorps )		continue;

		GuildMember* pMember = pCorps->FindMember( ptszCharName );
		if( pMember )		return pMember;
	}

	return NULL;
}


RC::eGUILD_RESULT		Guild::CanRankUp( OUT GuildRankInfo& RankInfo )
{
	BYTE byGuildMaxLevel = GuildRankInfoParser::Instance()->GetGuildMaxLevel();

	// 현재 길드의 등급이 최대 등급이거나, 그 이상이면 더 이상 랭크업 할 수 없다.
	if( m_GuildInfo.m_GuildGrade >= byGuildMaxLevel )
		return RC::RC_GUILD_MAX_GUILD_RANK;

	GuildRankInfo* pInfo = GuildRankInfoParser::Instance()->FindGuildRankInfo( (eGUILD_GRADE)(m_GuildInfo.m_GuildGrade + 1) );
	if( !pInfo )
		return RC::RC_GUILD_NOT_FOUND_GUILD_INFO_FOR_GRADE;

	//1. UP 계산
	if( m_GuildInfo.m_UserPoint < pInfo->m_UP )
		return RC::RC_GUILD_NOT_ENOUGH_UP;

	//2. GP 계산
	if( m_GuildInfo.m_GuildPoint < pInfo->m_GP )
		return RC::RC_GUILD_NOT_ENOUGH_GP;

	// OUT 반환
	RankInfo = *pInfo;

	return RC::RC_GUILD_SUCCESS;
}

VOID Guild::RankUp()
{
	BYTE prevGrade = (BYTE)GetGuildGrade();

	GuildRankInfo* pInfo = GuildRankInfoParser::Instance()->FindGuildRankInfo( (eGUILD_GRADE)( m_GuildInfo.m_GuildGrade + 1) );
	if( !pInfo )
	{
		SUNLOG( eCRITICAL_LOG, "[Guild::RankUp()] Error!! CurGuildGrade = %d", m_GuildInfo.m_GuildGrade );
		return;
	}
	
	SetGuildGrade( (eGUILD_GRADE)(GetGuildGrade() + 1) );
	SetGuildUP( m_GuildInfo.m_UserPoint - pInfo->m_UP );
	SetGuildGP( m_GuildInfo.m_GuildPoint - pInfo->m_GP );

	GUILDLOG->LogRankUp(this, prevGrade);
}


BOOL Guild::SendDBProxy( MSG_BASE * pMsg, WORD wSize )
{

	return TRUE;
}


//길드 직책
BOOL Guild::SetMemberDuty(CHARGUID CharGuid, eGUILD_DUTY eDuty)
{ 
	GuildMember* pMember = FindMember(CharGuid);
	if(!pMember) { return FALSE; }
	eGUILD_DUTY prevDuty = pMember->GetDuty();
	pMember->SetDuty(eDuty);

	GUILDLOG->LogDutyChange(this, pMember, prevDuty);

	return TRUE;
}

BOOL	Guild::IsExistMemberDuty( eGUILD_DUTY eDuty, BYTE byCorpsNum )
{
	GuildCorps* pCorps = FindCorps( byCorpsNum );
	if( !pCorps )		return FALSE;

	return FALSE;
}


VOID 				Guild::SetGuildNotice( const TCHAR* pszGuildNotice )
{
	strncpy( m_szGuildNotice, pszGuildNotice, MAX_GUILDNOTICE_LENGTH );
	m_szGuildNotice[MAX_GUILDNOTICE_LENGTH] = '\0';
}


VOID 				Guild::SetGuildName( const TCHAR* pszGuildName )
{
    strncpy(m_GuildInfo.m_tszGuildName, pszGuildName, _countof(m_GuildInfo.m_tszGuildName));
    m_GuildInfo.m_tszGuildName[_countof(m_GuildInfo.m_tszGuildName) - 1] = '\0';
}



GuildCorps* Guild::FindCorps( BYTE corps_number ) const
{
	GUILD_CORPS_MAP::const_iterator iter = m_mapGuildCorps.find(corps_number);     
	if (iter != m_mapGuildCorps.end())
	{
		GuildCorps* corps = iter->second;
		if (corps != NULL)
        {
            return corps;
        }
	}
	return NULL;
}

WORD	Guild::GetMemberCount()
{
	WORD wCount = 0;

	for( GUILD_CORPS_MAP_ITER iter = m_mapGuildCorps.begin(); iter != m_mapGuildCorps.end(); ++iter )
	{
		GuildCorps* pCorps = iter->second;
		if( !pCorps )	continue;

		wCount += pCorps->GetMemberCount();
	}

	return wCount;	
}

//DB에서 군단을 생성하고 그 결과 값을 세팅한다.
GuildCorps*	Guild::CreateCorps( BYTE byCorpsNum )
{
	GuildCorps* pCorps = FindCorps( byCorpsNum );
	if( pCorps )
	{
		SUNLOG( eCRITICAL_LOG, "[Guild::CreateCorps] Already Corps Create, GuildGuid = %u, CoprsNum = %d", GetGuildGuid(), byCorpsNum );
		return NULL;
	}

	GuildCorps* pNewCorps = new GuildCorps;
	pNewCorps->SetNumber( byCorpsNum );
	m_mapGuildCorps.insert( std::make_pair( byCorpsNum, pNewCorps ) );
	return pNewCorps;
}

VOID		Guild::DestroyCorps( BYTE byCorpsNum )
{
	GUILD_CORPS_MAP_ITER iter = m_mapGuildCorps.find( byCorpsNum );
	if( iter != m_mapGuildCorps.end() )
	{
		GuildCorps* pCorps = iter->second;
		if( pCorps )		delete pCorps;
		m_mapGuildCorps.erase( iter );
	}
}

//현재 생성 가능한 군단을 생성한다. ( 순차적으로 군단을 생성한다. )
GuildCorps*		Guild::CreateCorpsBySequence()
{
	BYTE byCorpsNum = GetEmptyCorpsNumBySequence();
	if( byCorpsNum == 0 )		
		return NULL;

	return CreateCorps( byCorpsNum );
}

//이 함수는 현재 길드에서 창설할 수 있는 군단의 번호를 리턴한다.
//현재 해당 군단이 존재 하지 않으면 0를 리턴한다.
//이 함수로는 직할 군단의 번호를 얻을 수 없다.
BYTE			Guild::GetEmptyCorpsNumBySequence()
{
	//1. 현재 길드레벨에서 가능한 군단의 개수를 가져온다.
	GuildRankInfo* pInfo = GuildRankInfoParser::Instance()->FindGuildRankInfo( GetGuildGrade() );
	if( !pInfo )		return NULL;

	// 0은 직할 군단이다. 따라서 1부터 시작한다. 군단 최대 개수는 직할군단을 포함하는 개수이다. 따라서 "<" 비교한다.
	for( BYTE i = 1; i < pInfo->m_byCreateCopsCount; i++ )
	{
		GUILD_CORPS_MAP_ITER iter = m_mapGuildCorps.find( i );	

		// i 군단이 존재하면
		if( iter != m_mapGuildCorps.end() )
			continue;

		return i;
	}

	return 0;
}

GuildMember* Guild::FindMaster() const
{
	GuildCorps* corps = FindCorps(0);
	if (corps == NULL)
    {
        return NULL;
    }
	return corps->FindCommander();
}

const TCHAR* Guild::GetMasterCharName()
{
	GuildMember* pGuildMaster = FindMaster();
	if(!pGuildMaster) 
		return NULL;
	
	return pGuildMaster->GetCharName();
}

GuildMember*	Guild::FindMemberByDuty(eGUILD_DUTY eDuty)
{
	GUILD_CORPS_MAP_ITER iter;
	for( iter = m_mapGuildCorps.begin(); iter != m_mapGuildCorps.end(); ++iter )
	{
		GuildCorps* pCorps = iter->second;
		if(pCorps)
		{
			GuildMember* Member = pCorps->FindMemberByDuty(eDuty);
			if(Member)  return Member; 
			else	continue;
		}
	}

	return NULL;
}

VOID	Guild::CreateGuild( GUILDGUID GuidGuid, const TCHAR* tszGuildName, BYTE byCorpsNum, CHARGUID MasterCharGuid, const TCHAR* tszMasterCharName, eGUILD_DUTY MasterDuty,
							BYTE byMasterClass, LEVELTYPE MasterLevel, BYTE byChaoState, INT64 nGuildPanaltyTime, INT64 nDestroyRequestTime, INT64 nMasterChangeTime )
{
	InitializeGuild();

	SetGuildGuid( GuidGuid );
	SetGuildName( tszGuildName );
	SetGuildGrade( eGUILD_GRADE_LEGION );
	SetGuildGP( 0 );	
	SetGuildUP( 0 );
	SetPenaltyTime( nGuildPanaltyTime );
	SetDestroyTime( nDestroyRequestTime );
	SetMasterChangeTime( nMasterChangeTime );
	GuildCorps* pCorps = CreateCorps( byCorpsNum );		//직할 군단 생성
	if( !pCorps )
	{
		SUNLOG( eCRITICAL_LOG, "[Guild::CreateGuild] Corps Create Fail, GuildGuid = %u, MasterCharGuid = %u", GuidGuid, MasterCharGuid );
		return;
	}

	GuildMember* pMaster = pCorps->CreateMember( MasterCharGuid, tszMasterCharName, MasterDuty, byMasterClass, MasterLevel, byChaoState, TRUE );
	if( !pMaster )
	{
		SUNLOG( eCRITICAL_LOG, "[Guild::CreateGuild] Corps Create GuildMaster, GuildGuid = %u, MasterCharGuid = %u", GuidGuid, MasterCharGuid );
		return;
	}
}

VOID		Guild::Destroy()
{
	GUILD_CORPS_MAP_ITER iter;
	for( iter = m_mapGuildCorps.begin(); iter != m_mapGuildCorps.end(); ++iter )
	{
		GuildCorps* pCorps = iter->second;
		if( pCorps )
		{
			pCorps->Destroy();
			delete pCorps;
		}
	}

	m_mapGuildCorps.clear();
}

BYTE		Guild::GetEmptyCommanderInfo( BYTE* pEmptyCommderInfo )
{
	BYTE byCount = 0;

	GUILD_CORPS_MAP_ITER iter;
	for( iter = m_mapGuildCorps.begin(); iter != m_mapGuildCorps.end(); ++iter )
	{
		GuildCorps* pCorps = iter->second;
		if( pCorps && !pCorps->FindCommander() )
		{
			pEmptyCommderInfo[byCount] = pCorps->GetNumber();
			byCount++;		
			if( byCount >= MAX_GUILD_CORPS_NUM )
			{
				SUNLOG( eCRITICAL_LOG, "[Guild::GetEmptyCommanderInfo] Overflow CorpsInfo, GuildGuid = %u", GetGuildGuid() );
				return 0;
			}
		}
	}

	return byCount;
}

//길드 가입
RC::eGUILD_RESULT Guild::JoinGuild( BYTE byCorpsNum, CHARGUID CharGuid, const TCHAR* tszCharName, eGUILD_DUTY Duty, BYTE byClass, LEVELTYPE Level, BYTE byChaoState )
{
	GuildCorps* pCorps = FindCorps(byCorpsNum);
	if( !pCorps )
	{
		SUNLOG( eCRITICAL_LOG, "[Guild::JoinGuild] Corps(%d) Not Exist", byCorpsNum );
		return RC::RC_GUILD_CORPS_NOT_FOUND;
	}

	//현재 가입한 놈은 로그인한 상태 이므로 로그인 상태로 유지한다.
	GuildMember* pMember = pCorps->CreateMember( CharGuid, tszCharName, Duty, byClass, Level, byChaoState, TRUE );
	if( !pMember )		return RC::RC_GUILD_FAILED;
	
	return RC::RC_GUILD_SUCCESS;
}

RC::eGUILD_RESULT			Guild::LeaveGuild( CHARGUID CharGuid )
{
	GuildMember* pMember = FindMember( CharGuid );
	if( !pMember )			return RC::RC_GUILD_NOT_GUILD_MEMBER;

	GuildCorps* pCorps = FindCorps( pMember->GetCorpsNumber() );
	if( !pCorps )			return RC::RC_GUILD_CORPS_NOT_FOUND;

	pCorps->DestroyMember( CharGuid );
	
	return RC::RC_GUILD_SUCCESS;
}

RC::eGUILD_RESULT			Guild::ChangeCorps( CHARGUID CharGuid, BYTE byNewCorpsNum )
{
	GuildMember* pMember = FindMember( CharGuid );
	if( !pMember )
		return RC::RC_GUILD_NOT_GUILD_MEMBER;

	//군단번호가 같다면 성공한것으로 처리한다.
	if( byNewCorpsNum == pMember->GetCorpsNumber() )
		return RC::RC_GUILD_SUCCESS;

	GuildCorps* pOldCorps = FindCorps( pMember->GetCorpsNumber() );
	if( !pOldCorps )
		return RC::RC_GUILD_CORPS_NOT_FOUND;

	GuildCorps* pNewCorps = FindCorps( byNewCorpsNum );
	if( !pNewCorps )
		return RC::RC_GUILD_CORPS_NOT_FOUND;

	pOldCorps->DeleteMember( CharGuid );
	pNewCorps->InsertMember( pMember );
	pMember->SetCorpsNumber( byNewCorpsNum );

	return RC::RC_GUILD_SUCCESS;
}

// 군단별 풀 멤버체크.
BOOL	Guild::IsFullCorpsMemberByDuty( BYTE byCorpsNum, eGUILD_DUTY Duty )
{
	GuildCorps* pCorps = FindCorps( byCorpsNum );
	if( !pCorps )
	{
		SUNLOG( eCRITICAL_LOG, "[Guild::IsFullMemberByCorps] Not Exist Corps, Corps = %d, Duty = %d ", byCorpsNum, Duty );
		return TRUE;
	}

	BYTE byCurCount = pCorps->GetMemberCountByDuty( Duty );
	BYTE byMaxCount = MaxGuildMemberInfoParser::Instance()->GetMaxCorpsMemberByDuty( GetGuildGrade(), byCorpsNum, Duty );
	if( byCurCount >= byMaxCount )
		return TRUE;

	return FALSE;
}

BOOL	Guild::CreateRearCamp( BYTE byCorpsNum )
{
	GuildCorps* pCorps = FindCorps( byCorpsNum );
	if( !pCorps )		return FALSE;

	pCorps->SetRearCamp( TRUE );

	return TRUE;
}

BOOL	Guild::DestroyRearCamp( BYTE byCorpsNum )
{
	GuildCorps* pCorps = FindCorps( byCorpsNum );
	if( !pCorps )		return FALSE;

	pCorps->SetRearCamp( FALSE );
	return TRUE;
}

//길드에서 해당 멤버를 제거 한다.
RC::eGUILD_RESULT			Guild::DestroyMember( CHARGUID CharGuid )
{
	GuildMember* pMember = FindMember( CharGuid );
	if( !pMember )			return RC::RC_GUILD_NOT_GUILD_MEMBER;

	GuildCorps* pCorps = FindCorps( pMember->GetCorpsNumber() );
	if( !pCorps )			return RC::RC_GUILD_CORPS_NOT_FOUND;

	pCorps->DestroyMember( CharGuid );
	return RC::RC_GUILD_SUCCESS;
}

VOID Guild::SerializeBaseGuildInfo( INOUT BASE_GUILD_INFO&  rInfo, eSERIALIZE se )
{
	if( se == SERIALIZE_LOAD )
	{
		rInfo.m_GuildGuid		= GetGuildGuid();
		rInfo.m_GuildGrade		= (BYTE)GetGuildGrade();
		memcpy( rInfo.m_tszGuildName, GetGuildName(), MAX_GUILDNAME_LENGTH*sizeof(TCHAR) );
	}
	else
	{
		SetGuildGuid( rInfo.m_GuildGuid );
		SetGuildGrade( (eGUILD_GRADE)rInfo.m_GuildGrade );
		SetGuildName( rInfo.m_tszGuildName );
	}
}

VOID	Guild::SerializeGameGuildInfo( INOUT GAME_GUILD_INFO&  rInfo, eSERIALIZE se )
{
	SerializeBaseGuildInfo( rInfo, se );

	if( se == SERIALIZE_LOAD )
	{
		AcquireGuildMark(rInfo.m_BackGroundIdx, rInfo.m_PatternIdx);

	#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
		rInfo.m_ChuntingPVPInfo = m_ChuntingPVPInfo;
	#endif
	}
	else
	{
	#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
		m_ChuntingPVPInfo	= rInfo.m_ChuntingPVPInfo;
	#endif
	}
}

VOID	Guild::SerializeGuildInfo( INOUT GUILD_INFO&  rInfo, eSERIALIZE se )
{
	SerializeGameGuildInfo( rInfo, se );

	if( se == SERIALIZE_LOAD )
	{
		rInfo.m_GuildPoint		= GetGuildGP();
		rInfo.m_UserPoint		= GetGuildUP();

		rInfo.m_GuildDestroyWaitFlag = CheckGuildDeleteState();
		rInfo.m_GuildPenaltyTime	= GetPenaltyTime();
		rInfo.m_GuildDestroyTime	= GetDestroyTime();
		AcquireGuildMark(rInfo.m_BackGroundIdx, rInfo.m_PatternIdx);
		rInfo.m_nMasterChangeTime	= GetMasterChangeTime();
#ifdef _NA_20100604_BETTER_DOMINATION_MANAGER
        rInfo.m_DominationMapCount = 
            DominationManager::Instance()->NumberOfDominationAreaByGuild(rInfo.m_GuildGuid);
        rInfo.m_DominationJoinCount = GetDominationJoinCount();
        rInfo.m_DominationDefenseCount = GetDominationDefenseCount();
        rInfo.m_DominationHuntingCount = GetDominationHuntingCount();
#endif
	}
	else
	{
		SetGuildGP( rInfo.m_GuildPoint );
		SetGuildUP( rInfo.m_UserPoint );

		SetDeleteGuildState(rInfo.m_GuildDestroyWaitFlag);
		SetPenaltyTime(rInfo.m_GuildPenaltyTime);
		SetDestroyTime(rInfo.m_GuildDestroyTime);
		RegisterGuildMark(rInfo.m_BackGroundIdx, rInfo.m_PatternIdx);
		SetMasterChangeTime( rInfo.m_nMasterChangeTime );
#ifdef _NA_20100604_BETTER_DOMINATION_MANAGER
        SetDominationJointCount(rInfo.m_DominationJoinCount);
        SetDominationDefenseCount(rInfo.m_DominationDefenseCount);
        SetDominationHuntingCount(rInfo.m_DominationHuntingCount);
#endif
	}
}

VOID	Guild::SerializeCorpsInfo( INOUT GUILD_CORPS_TOTAL_INFO&  rInfo, eSERIALIZE se )
{
	if( se == SERIALIZE_LOAD )
	{
		ZeroMemory( &rInfo, sizeof(rInfo) );

		for( GUILD_CORPS_MAP_ITER iter = m_mapGuildCorps.begin(); iter != m_mapGuildCorps.end(); ++iter )
		{
			GuildCorps* pCorps = iter->second;
			if( !pCorps )	continue;

			BYTE byCorpsNum = pCorps->GetNumber();

			rInfo.m_CorpsInfo[byCorpsNum].m_bCreate = TRUE;
			rInfo.m_CorpsInfo[byCorpsNum].m_byCorpsNum = byCorpsNum;
			rInfo.m_CorpsInfo[byCorpsNum].m_bRearCamp = pCorps->IsExistRearCamp();
		}

	}
	else //STORE
	{
		for( int i = 0 ; i < MAX_GUILD_CORPS_NUM; i++ )
		{
			if( rInfo.m_CorpsInfo[i].m_bCreate )
			{
				GuildCorps* pCorps = FindCorps( i );
				if( pCorps )
				{
					pCorps->SetRearCamp( rInfo.m_CorpsInfo[i].m_bRearCamp );
				}
				else
				{
					//DB에서 날라오는데 현재 군단이 생성되어 있지 않으면...
					GuildCorps* pNewCorps = CreateCorps( rInfo.m_CorpsInfo[i].m_byCorpsNum );
					if( pNewCorps )	pNewCorps->SetRearCamp( rInfo.m_CorpsInfo[i].m_bRearCamp );
				}
			}
		}
	}	

}

VOID Guild::SerializeBaseMemberTotalInfo( INOUT BASE_GUILD_MEMBER_TOTAL_INFO&  rInfo, eSERIALIZE se )
{
	if( se == SERIALIZE_LOAD )
	{
		rInfo.m_byMemberCount = 0;

		for( GUILD_CORPS_MAP_ITER iter = m_mapGuildCorps.begin(); iter != m_mapGuildCorps.end(); ++iter )
		{
			GuildCorps* pCorps = iter->second;
			if( !pCorps )	continue;

			pCorps->SerializeBaseMemberTotalInfo( rInfo, se );
		}

	}
	else
	{
		SUNLOG( eCRITICAL_LOG, "[Guild::SerializeMemberInfo] 미구현" );
	}

}

VOID Guild::SerializeMemberTotalInfo( INOUT GUILD_MEMBER_TOTAL_INFO&  rInfo, eSERIALIZE se )
{
	if( se == SERIALIZE_LOAD )
	{
		rInfo.m_byMemberCount = 0;

		for( GUILD_CORPS_MAP_ITER iter = m_mapGuildCorps.begin(); iter != m_mapGuildCorps.end(); ++iter )
		{
			GuildCorps* pCorps = iter->second;
			if( !pCorps )	continue;

			pCorps->SerializeMemberTotalInfo( rInfo, se );
		}
	}
	else
	{
		//멤버는 반드시 해당 군단이 먼저 창설되어야 한다.
		for( GUILD_CORPS_MAP_ITER iter = m_mapGuildCorps.begin(); iter != m_mapGuildCorps.end(); ++iter )
		{
			GuildCorps* pCorps = iter->second;
			if( !pCorps )	continue;

			pCorps->SerializeMemberTotalInfo( rInfo, se );
		}
	}
}

VOID	Guild::SerializeServerMemberTotalInfo( INOUT SERVER_GUILD_MEMBER_TOTAL_INFO&  rInfo, eSERIALIZE se )
{
	if( se == SERIALIZE_LOAD )
	{
		rInfo.m_byMemberCount = 0;

		for( GUILD_CORPS_MAP_ITER iter = m_mapGuildCorps.begin(); iter != m_mapGuildCorps.end(); ++iter )
		{
			GuildCorps* pCorps = iter->second;
			if( !pCorps )	continue;

			pCorps->SerializeServerMemberTotalInfo( rInfo, se );
		}
	}
	else
	{
		//멤버는 반드시 해당 군단이 먼저 창설되어야 한다.
		for( GUILD_CORPS_MAP_ITER iter = m_mapGuildCorps.begin(); iter != m_mapGuildCorps.end(); ++iter )
		{
			GuildCorps* pCorps = iter->second;
			if( !pCorps )	continue;

			pCorps->SerializeServerMemberTotalInfo( rInfo, se );
		}
	}
}

BOOL		Guild::GetBaseMemberInfo( CHARGUID CharGuid, OUT BASE_GUILD_MEMBER_INFO& info )
{
	GuildMember* pMember = FindMember( CharGuid );
	if( !pMember )		return FALSE;

	pMember->SerializeBaseMemberInfo( info, SERIALIZE_LOAD );

	return TRUE;
}

BOOL		Guild::GetMemberInfo( CHARGUID CharGuid, OUT GUILD_MEMBER_INFO& info )
{
	GuildMember* pMember = FindMember( CharGuid );
	if( !pMember )		return FALSE;

	pMember->SerializeMemberInfo( info, SERIALIZE_LOAD );

	return TRUE;
}

VOID		Guild::GetWorldGuildInfo( WORLD_GUILD_INFO&  rInfo )
{
	SerializeBaseGuildInfo( rInfo, SERIALIZE_LOAD );
	CopyMemory( rInfo.m_szGuildNotice, GetGuildNotice(), MAX_GUILDNOTICE_LENGTH * sizeof(TCHAR));
}

BOOL Guild::IsFullMember()
{
	GuildRankInfo* pGuildInfo = GuildRankInfoParser::Instance()->FindGuildRankInfo( (eGUILD_GRADE)m_GuildInfo.m_GuildGrade );
	if( !pGuildInfo )		return FALSE;
	return ( GetMemberCount() >= pGuildInfo->m_wMaxMember );
}

VOID Guild::ChangeGuildName( const TCHAR* pszGuildName )
{
    util::GUILDNAMECOPY(m_GuildInfo.m_tszGuildName, pszGuildName);
}

int Guild::GetRelationGuid(STLX_VECTOR<GUILDGUID>& vGuildGuid, eGUILD_RELATION RelationType)
{
	int size = m_GuildRelationEntry.GetRelationCount(RelationType);
	vGuildGuid.reserve(size);

	return m_GuildRelationEntry.GetRelationGuid(vGuildGuid,RelationType);
}

