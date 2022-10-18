#include "StdAfx.h"
#include ".\gameguildmanager.h"
#include ".\GameGuild.h"
#include ".\Player.h"
#include <PacketStruct_GZ.h>
#include "ChaoState.h"
#include "GameGuildMember.h"
#include <GuildRankInfoParser.h>
#include "GameZone.h"
#include <ProhibitionWordParser.h>
#include "GameGuildExtension.h"
#include "GuildInfoParser.h"

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
#include "SunRanking/SunRankingManager.h"
#include "SunRanking/SunRankingGradeManager.h"
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM

#ifdef _NA_008334_20150608_SONNENSCHEIN
#include "PlayerManager.h"
#endif //_NA_008334_20150608_SONNENSCHEIN

GameGuildManager g_GameGuildManager;
GameGuildManager::GameGuildManager(void)
{
	m_GuildHash.Initialize(MAX_GUILD_POOL_NUM);
}

GameGuildManager::~GameGuildManager(void)
{
	Release();
}

VOID GameGuildManager::Release()
{
	for( GUILD_HASH::iterator it = m_GuildHash.begin() ; it != m_GuildHash.end() ; ++it )
	{
		(*it)->Destroy();
		GameGuild::FREE( (*it) );
	}

	m_GuildHash.RemoveAll();
}

BOOL GameGuildManager::ValidPlayer( Player * pPlayer )
{
	if( !pPlayer )		return FALSE;

	//1. 플레이어가 죽은 상태에서 할 수 있는가?

	return TRUE;
}


RC::eGUILD_RESULT GameGuildManager::WithdrawMember( Player * pPlayer )
{
	GUILDGUID GuildGuid = pPlayer->GetCharInfo()->m_GuildGuid;
	if( 0 == GuildGuid )
		return RC::RC_GUILD_CANNOT_WITHDRAW_STATE;

	GameGuild * pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return RC::RC_GUILD_INVALID_STATE;

	MSG_GZ_GUILD_WITHDRAW_SYN msgSYN;
	msgSYN.m_GuildGuid = pGuild->GetGuildGuid();
	msgSYN.m_CharGuid	= pPlayer->GetCharGuid();
	if( FALSE == pPlayer->SendToGuildServer( &msgSYN, sizeof(msgSYN) ) )
	{
		return RC::RC_GUILD_DISCONNECTED_GUILD_SERVER;
	}

	return RC::RC_GUILD_SUCCESS;
}

// 길드장이 강퇴
RC::eGUILD_RESULT GameGuildManager::KickMember( Player * pMaster, TCHAR *ptszCharName )
{
	GUILDGUID GuildGuid = pMaster->GetCharInfo()->m_GuildGuid;
	if( 0 == GuildGuid )
		return RC::RC_GUILD_CANNOT_KICK_STATE;

	GameGuild * pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return RC::RC_GUILD_INVALID_STATE;

	// 길드서버에 요청
	MSG_GZ_GUILD_KICKPLAYER_SYN msg;
	msg.m_GuildGuid = pGuild->GetGuildGuid();
	memset( msg.m_ptszCharName, 0, sizeof(TCHAR) * (MAX_CHARNAME_LENGTH+1) );
	_tcsncpy( msg.m_ptszCharName, ptszCharName, MAX_CHARNAME_LENGTH );
	if( FALSE == pMaster->SendToGuildServer( &msg, sizeof(msg) ) )
	{
		return RC::RC_GUILD_DISCONNECTED_GUILD_SERVER;
	}

	return RC::RC_GUILD_SUCCESS;
}



VOID GameGuildManager::DisplayerGuildInfo( BOOL bLog )
{
	DWORD dwNum = 0;
	for( GUILD_HASH::iterator it = m_GuildHash.begin() ; it != m_GuildHash.end() ; ++it )
	{
		dwNum += (*it)->GetMemberNum();
	}

	if( bLog )
		SUNLOG( eCRITICAL_LOG, "Guild : %u, GuildMember : %u\n", m_GuildHash.GetDataNum(), dwNum );
	else
		DISPMSG( "Guild : %u, GuildMember : %u\n", m_GuildHash.GetDataNum(), dwNum );
}

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
VOID GameGuildManager::SetChuntingPVPInfo( GUILDGUID GuildGuid, CHUNTING_PVP_INFO & IN PVPInfo )
{
	GameGuild * pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return;

	pGuild->SetChuntingPVPInfo( PVPInfo );
}
#endif

//길드 직책
VOID GameGuildManager::ChangeMemberDuty(GUILDGUID GuildGuid, CHARGUID TargetCharGuid, eGUILD_DUTY eDuty)
{
	//현재 직책을 가진 캐릭터가 존재할 경우 그 멤버의 직책은 eGUILD_DUTY_NONE으로 바꿔준다.
	GameGuild *pGuild = FindGuild(GuildGuid);
	if(!pGuild)
	{
		SUNLOG( eCRITICAL_LOG, "[GameGuildManager::ChangeMemberDuty] Error! Guild Not Exist, GuildGuid = %d", GuildGuid );
		return;
	}

	//해당 게임서버에 직책변경하는 멤버가 없을수도 있다.
	pGuild->SetMemberDuty( TargetCharGuid, eDuty );
}


VOID		GameGuildManager::ChangeMaster( GUILDGUID GuildGuid, CHARGUID OldMasterCharGuid, CHARGUID NewMasterCharGuid )
{
	GameGuild *pGuild = FindGuild(GuildGuid);
	if(!pGuild)
	{
		SUNLOG( eCRITICAL_LOG, "[GameGuildManager::ChangeMaster] Error! Guild Not Exist, GuildGuid = %d", GuildGuid );
		return;
	}

	//현재 새로운 마스터 유저가 접속해 있다면 해당 유저의 직책을 바꾼다.
	GameGuildMember* pOldMaster = pGuild->FindMember( OldMasterCharGuid );
	if( pOldMaster )
		pOldMaster->SetDuty( eGUILD_DUTY_REGULAR_SOLDIER );
	
	//현재 새로운 마스터 유저가 접속해 있다면 해당 유저의 직책을 바꾼다.
	GameGuildMember* pNewMaster = pGuild->FindMember( NewMasterCharGuid );
	if( pNewMaster )
		pNewMaster->SetDuty( eGUILD_DUTY_MASTER );
}

RC::eGUILD_RESULT	GameGuildManager::CanChangeDuty( GUILDGUID GuildGuid, CHARGUID OperCharGuid, CHARGUID TargetCharGuid, eGUILD_DUTY eDuty )
{
	GameGuild* pGuild = FindGuild( GuildGuid );
	if( !pGuild )
		return RC::RC_GUILD_NOT_EXIST_GUILD;

	// 길드 창고 사용중인 유저는 제명 시키지 못함
	Player* pGuildWarehouseUsedPlayer = pGuild->GetGuildWareHouse()->GetUsedPlayer();
	if(pGuildWarehouseUsedPlayer &&  0 != TargetCharGuid &&
		pGuildWarehouseUsedPlayer->GetCharGuid() == TargetCharGuid)
	{
		return RC::RC_GUILD_CANNOT_CHANGE_DUTY_BY_WAREHOUSE_USEING;
	}

	//직책 범위 검사
	if( eDuty <= eGUILD_DUTY_NONE || eDuty >= eGUILD_DUTY_MAX )
		return RC::RC_GUILD_CANNOT_CHANGE_DUTY_BECAUSE_INVALID_DUTY;

	return RC::RC_GUILD_SUCCESS;
}


VOID GameGuildManager::AddGuildName(std::string strGuildName, GameGuild *pGuild)
{
	// 대문자 변환
	transform(strGuildName.begin(), strGuildName.end(), strGuildName.begin(), toupper);

	GUILD_NAME_MAP::const_iterator itr = m_GuildNameMap.find(strGuildName);
    // NOTE: itr unused...
//#ifdef _DEBUG
//	assert(itr == m_GuildNameMap.end());
//#else
//	if(itr != m_GuildNameMap.end())
//	{
//		m_GuildNameMap.erase(itr);
//		MessageOut( eFULL_LOG,  "GameGuildManager::AddGuildName - Existed Guild Name(%s)  ", strGuildName.c_str() );
//	}
//#endif
	m_GuildNameMap.insert(GUILD_NAME_MAP::value_type(strGuildName, pGuild));
}

VOID GameGuildManager::RemoveGuildName(std::string strGuildName)
{
	// 대문자 변환
	transform(strGuildName.begin(), strGuildName.end(), strGuildName.begin(), toupper);

	GUILD_NAME_MAP::iterator itr = m_GuildNameMap.find(strGuildName);
	if(itr != m_GuildNameMap.end())
	{	
		m_GuildNameMap.erase(itr);
	}
}

GameGuild *GameGuildManager::FindGuildName(std::string strGuildName)
{	
	// 대문자 변환
	transform(strGuildName.begin(), strGuildName.end(), strGuildName.begin(), toupper);

	GUILD_NAME_MAP::const_iterator itr = m_GuildNameMap.find(strGuildName);

	if(itr != m_GuildNameMap.end())
	{
		return itr->second;
	}

	return FALSE;
}

// NPC를 통해서 하는 길드 관련 행동에 대해 유저체크.
RC::eGUILD_RESULT	GameGuildManager::ValidPlayerWithNPC( Player* pPlayer )
{
	if( !pPlayer )				return RC::RC_GUILD_INVALID_STATE;

	if( !pPlayer->GetField() || !pPlayer->GetField()->GetGameZone() ) 
		return RC::RC_GUILD_INVALID_STATE;

	// 길드 NPC에게 가서 기능을 수행함으로마을에 있어야 한다.
	if( pPlayer->GetField()->GetGameZone()->GetZoneType() != eZONETYPE_VILLAGE )
		return RC::RC_GUILD_INVALID_STATE;

	return RC::RC_GUILD_SUCCESS;
}

//군단장변경
VOID		GameGuildManager::ChangeCommander( GUILDGUID GuildGuid, CHARGUID TargetCharGuid,  BYTE byCorpsNum )
{
	//게임서버에서는 군단이 존재 하지 않는다. 
	//단지 길드 멤버의 군단번호가 바뀌고, 직책이 바뀌게 된다.
	GameGuild* pGuild = FindGuild(GuildGuid);
	if( !pGuild )	return;

	GameGuildMember* pMember = pGuild->FindMember( TargetCharGuid );
	if( !pMember )	return;

	pMember->SetDuty( eGUILD_DUTY_COMMANDER );
	pMember->SetCorpsNum( byCorpsNum );
}

//군단이동.
VOID		GameGuildManager::ChangeCorps( GUILDGUID GuildGuid,  CHARGUID TargetCharGuid, BYTE byCorpsNum )
{
	GameGuild* pGuild = FindGuild(GuildGuid);
	if( !pGuild )	return;

	GameGuildMember* pMember = pGuild->FindMember( TargetCharGuid );
	if( !pMember )	return;

	pMember->SetCorpsNum( byCorpsNum );
}

//훈련캠프 창설
VOID		GameGuildManager::CreateRearCamp( GUILDGUID GuildGuid, BYTE byCorpsNum )
{
	GameGuild* pGuild = FindGuild(GuildGuid);
	if( !pGuild )	return;

	pGuild->CreateRearCamp( byCorpsNum );
}

//훈련캠프 해산.
VOID		GameGuildManager::DestroyRearCamp( GUILDGUID GuildGuid, BYTE byCorpsNum )
{
	GameGuild* pGuild = FindGuild(GuildGuid);
	if( !pGuild )	return;

	pGuild->DestroyRearCamp( byCorpsNum );
}

#ifdef _NA_003923_20120130_GUILD_RENEWAL
RC::eGUILD_RESULT	
GameGuildManager::CanCreateGuild(Player* player, OUT MONEY& heim)
{
    RC::eGUILD_RESULT rc = ValidPlayerWithNPC(player);
    if (rc != RC::RC_GUILD_SUCCESS)
    {
        return rc;
    }

    GUILDGUID guild_guid = player->GetCharInfo()->m_GuildGuid;
    if (guild_guid != 0)
    {
        return RC::RC_GUILD_CANNOT_CREAT_GUILD_STATE;
    }

    if (player->GetChaoState().IsChaoState())
    {
        return RC::RC_GUILD_UNABLE_FUNCTION_FOR_CHAOSTATE;
    }

    GuildInfoParser* const guild_info_parser = GuildInfoParser::Instance();
    const GuildInfoData& parser_data = guild_info_parser->GetGuildInfo();

    if (player->GetLevel() < parser_data.guild_create_level_)
    {
        return RC::RC_GUILD_CANNOT_CREAT_LEVEL_STATE;
    }

    OUT heim = parser_data.guild_create_heim_;
    if (player->GetMoney() < heim)
    {
        return RC::RC_GUILD_CANNOT_RANKUP_MONEY_STATE;
    }

    return RC::RC_GUILD_SUCCESS;
}
#else//_NA_003923_20120130_GUILD_RENEWAL
//길드 생성
// OUT 인자가 있는것은 현재 길드 창고가 없어서 플레이어의 돈을 빼내는 구조 이기 때문이다.
RC::eGUILD_RESULT	GameGuildManager::CanCreateGuild( Player* pPlayer, OUT MONEY& heim )
{
	heim = 0;

	RC::eGUILD_RESULT rc = ValidPlayerWithNPC( pPlayer );
	if( rc != RC::RC_GUILD_SUCCESS )
		return rc;

	GUILDGUID GuildGuid = pPlayer->GetCharInfo()->m_GuildGuid;
	if( 0 != GuildGuid )	//길드가 없어여 창설 할 수 있다.
		return RC::RC_GUILD_CANNOT_CREAT_GUILD_STATE;

    //최초 생성 길드 정보를 가지고 온다.
    GuildRankInfo* pGuildInfo = GuildRankInfoParser::Instance()->FindGuildRankInfo(GRADE_AT_GUILD_CREATE);
    if( !pGuildInfo )
        return RC::RC_GUILD_NOT_FOUND_GUILD_INFO_FOR_GRADE;

    //카오상태이면 길드 생성을 할 수 없다.
    if( pPlayer->GetChaoState().IsChaoState() )
        return RC::RC_GUILD_UNABLE_FUNCTION_FOR_CHAOSTATE;

    //길드마스터 레벨 체크 
    if( pPlayer->GetLevel() < CREATE_GUILD_LIMIT_LEVEL )
        return RC::RC_GUILD_CANNOT_CREAT_LEVEL_STATE;

    //하임체크
    if( pPlayer->GetMoney() < pGuildInfo->m_dwHeim )
        return RC::RC_GUILD_CANNOT_RANKUP_MONEY_STATE;

    heim = pGuildInfo->m_dwHeim;

    return RC::RC_GUILD_SUCCESS;
}
#endif//_NA_003923_20120130_GUILD_RENEWAL

RC::eGUILD_RESULT	GameGuildManager::CreateGuild( Player* pPlayer, const GAME_GUILD_INFO& rInfo, const BASE_GUILD_MEMBER_INFO& rMemberInfo )
{
	RC::eGUILD_RESULT rc = ValidPlayerWithNPC( pPlayer );
	if( rc != RC::RC_GUILD_SUCCESS )
		return rc;

	GameGuild* pGuild = NULL;
	pGuild = g_GameGuildManager.FindGuild( rInfo.m_GuildGuid );
	if( pGuild )
	{
		// 기존의 길드를 모두 삭제한다.
		RemoveGuild( pGuild->GetGuildGuid() );
		pGuild->Destroy();
		GameGuild::FREE( pGuild );
		pGuild = NULL;
		SUNLOG( eCRITICAL_LOG, "[GameGuildManager::CreateGuild] Guild Exist, GuildGuid = %u, And Destroy ", rInfo.m_GuildGuid ); 
	}

	//1. Player 객체에 해당 정보를 세팅한다.
	pPlayer->GetCharInfo()->m_GuildGuid		= rInfo.m_GuildGuid;
	pPlayer->GetCharInfo()->m_eGuildDuty		= eGUILD_DUTY_MASTER;
	strncpy( pPlayer->GetCharInfo()->m_tszGuildName, rInfo.m_tszGuildName, MAX_GUILDNAME_LENGTH );
	pPlayer->GetCharInfo()->m_tszGuildName[MAX_GUILDNAME_LENGTH] = '\0';

	//2. Guild를 생성한다.
	pGuild = GameGuild::ALLOC();
	pGuild->Create( rInfo, rMemberInfo );
	AddGuild( pGuild, rInfo.m_GuildGuid );
	AddGuildName(pGuild->GetGuildName(), pGuild); //이거 바꿔보자..이건 아닌듯..

	return RC::RC_GUILD_SUCCESS;
}

RC::eGUILD_RESULT	GameGuildManager::CanDestroyGuild( Player* pPlayer )
{
	RC::eGUILD_RESULT rc = ValidPlayerWithNPC( pPlayer );
	if( rc != RC::RC_GUILD_SUCCESS )
		return rc;

	GUILDGUID GuildGuid = pPlayer->GetCharInfo()->m_GuildGuid;
	if( 0 == GuildGuid )
		return RC::RC_GUILD_NOT_GUILD_MEMBER;

	return RC::RC_GUILD_SUCCESS;
}

VOID				GameGuildManager::DestroyGuild( GUILDGUID GuildGuid )
{
	GameGuild* pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return;

	RemoveGuild( GuildGuid );
	RemoveGuildName(pGuild->GetGuildName());
	pGuild->Destroy();
	GameGuild::FREE( pGuild );
	pGuild = NULL;
}

// 1. 길드가입의 권한이 있는 길드원
// 2. 길드에 가입할 유저가 다른 길드에 가입되어 있지 않아야 한다 
RC::eGUILD_RESULT GameGuildManager::CanJoinGuild( Player* request_player, Player* target_player )
{
	if (request_player == NULL)
    {
        return RC::RC_GUILD_INVALID_STATE;
    }

	if (request_player->GetField() == NULL || 
        request_player->GetField()->GetGameZone() == NULL)
    {
		return RC::RC_GUILD_INVALID_STATE;
    }

	if (target_player == NULL)
    {
        return RC::RC_GUILD_INVALID_STATE;
    }

	if (target_player->GetField() == NULL || 
        target_player->GetField()->GetGameZone() == NULL)
    {
		return RC::RC_GUILD_INVALID_STATE;
    }

	GUILDGUID guild_guid = request_player->GetCharInfo()->m_GuildGuid;
	if (guild_guid == 0)
    {
		return RC::RC_GUILD_NOT_GUILD_MEMBER;
    }

	if (target_player->GetCharInfo()->m_GuildGuid != 0)
    {
		return RC::RC_GUILD_ALREADY_GUILD_MEMBER;
    }

	GameGuild* guild = FindGuild(guild_guid);
	if (guild == NULL) 
    {
        return RC::RC_GUILD_NOT_EXIST_GUILD;
    }

	if (target_player->GetBehave() != PLAYER_BEHAVE_IDLE_STATE)
    {
		return RC::RC_GUILD_INVALID_PLAYER_STATE;
    }

	return RC::RC_GUILD_SUCCESS;
}

RC::eGUILD_RESULT GameGuildManager::JoinGuild( GUILDGUID GuildGuid, const BASE_GUILD_MEMBER_INFO& rMemberInfo )
{
	GameGuild* pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return RC::RC_GUILD_NOT_EXIST_GUILD;

	pGuild->Join(rMemberInfo);

	return RC::RC_GUILD_SUCCESS;
}

VOID	GameGuildManager::DestroyMember( const GUILDGUID GuildGuid, const CHARGUID CharGuid )
{
	GameGuild* pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return;

	pGuild->DestroyMember( CharGuid );
}

// 길드 랭크업
RC::eGUILD_RESULT GameGuildManager::CanRankUpGuild( const GUILDGUID GuildGuid, const MONEY Money, OUT MONEY& UseHeim )
{
	UseHeim = 0;
		
	GameGuild* pGuild = g_GameGuildManager.FindGuild( GuildGuid );
	if( !pGuild )			return RC::RC_GUILD_INVALID_STATE;

	const GuildRankInfo* pGuildInfo = GuildRankInfoParser::Instance()->FindGuildRankInfo( pGuild->GetGuildGrade() + 1);
	if( !pGuildInfo )
		return RC::RC_GUILD_NOT_FOUND_GUILD_INFO_FOR_GRADE;

#ifndef _NA_003923_20120130_GUILD_RENEWAL
    //1. 하임체크는 현재 게임서버에서만 가능하다.
    if( Money < pGuildInfo->m_dwHeim )
        return RC::RC_GUILD_CANNOT_RANKUP_MONEY_STATE;

    UseHeim = pGuildInfo->m_dwHeim;
#endif

    UseHeim = 0;

	return RC::RC_GUILD_SUCCESS;
}

VOID	GameGuildManager::RankUpGuild( const GUILDGUID GuildGuid, BYTE byGrade )
{
	GameGuild* pGuild = g_GameGuildManager.FindGuild( GuildGuid );
	if( !pGuild )			return;

	pGuild->SetGuildGrade( byGrade );
}

//캐릭터가 로그인 했을 때..
BOOL	GameGuildManager::LoginChar( const GAME_GUILD_INFO& rInfo, const BASE_GUILD_MEMBER_INFO& rMemberInfo )
{
	GameGuild* pGuild = g_GameGuildManager.FindGuild( rInfo.m_GuildGuid );
    BOOL is_result = FALSE;
	//길드가 존재하지 않으면 길드 / 맴버를 생성한다.
	if( !pGuild )
	{
		pGuild = GameGuild::ALLOC();
		pGuild->Create( rInfo, rMemberInfo );
		AddGuild( pGuild, pGuild->GetGuildGuid() );
		AddGuildName(pGuild->GetGuildName(), pGuild); //이거 바꿔보자..이건 아닌듯..

		is_result = TRUE;
	}
    else {
		pGuild->AddMember( rMemberInfo );
    }

    Player* player = PlayerManager::Instance()->FindPlayerByCharKey(rMemberInfo.m_CharGuid);
    if (player != NULL) 
    {
        //CharInfo쪽에서 하면 서버켜진 후, 길드의 첫 접속자는 길드정보가 없어서 버프를 못받는다
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        g_GameGuildManager.ApplyGuildFacilityPassive(player, true);
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

        //소넨샤인 버프 적용
        //로그인 후, 길드가 셋팅되는 이곳에서 동작시킨다
#ifdef _NA_008334_20150608_SONNENSCHEIN
        pGuild->ApplyGuildRewardOption(player);
#endif //_NA_008334_20150608_SONNENSCHEIN
    }

	return is_result;
}

//캐릭터가 해당 게임서버에서 나갈때.
VOID GameGuildManager::Logout( GUILDGUID GuildGuid, CHARGUID CharGuid )
{
	GameGuild * pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return;

	pGuild->DestroyMember( CharGuid );

}

//캐릭터가 해당 게임서버로 들어왔을 때.
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#else
VOID	GameGuildManager::EnterServer( const GAME_GUILD_INFO& rInfo, const BASE_GUILD_MEMBER_INFO& rMemberInfo )
{
    GameGuild* pGuild = g_GameGuildManager.FindGuild( rInfo.m_GuildGuid );
    //길드가 존재하지 않으면 길드 / 맴버를 생성한다.
    if( !pGuild )
    {
        pGuild = GameGuild::ALLOC();
        pGuild->Create( rInfo, rMemberInfo );
        AddGuild( pGuild, pGuild->GetGuildGuid() );
        AddGuildName(pGuild->GetGuildName(), pGuild); //이거 바꿔보자..이건 아닌듯..
    }
    else
        pGuild->AddMember( rMemberInfo );
}
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION

//캐릭터가 해당 게임서버에서 나갈때.
VOID GameGuildManager::LeaveServer( GUILDGUID GuildGuid, CHARGUID CharGuid )
{
	GameGuild* pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return;

	pGuild->DestroyMember( CharGuid );
}

VOID	GameGuildManager::SendPacket( const GUILDGUID GuildGuid, MSG_BASE_FORWARD* pMsg, int nSize, CHARGUID ExceptionCharGuid  )
{
	GameGuild* pGuild = g_GameGuildManager.FindGuild( GuildGuid );
	if( !pGuild )			return;

	pGuild->SendPacketAll( pMsg, nSize, ExceptionCharGuid  );
}

VOID GameGuildManager::LeaveGuild( GUILDGUID GuildGuid, CHARGUID CharGuid )
{
#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
    if (SunRankingGradeManager::Instance()->IsSettleChannel())
    {
        SunRankingManager::Instance()->SetGuildGuid(CharGuid, 0);
    }
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM

	GameGuild * pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return;

	pGuild->DestroyMember( CharGuid );
}

CGuildWarehouseSlotContainer* GameGuildManager::GetGuildWarehouse(GUILDGUID GuildGuid)
{
	GameGuild * pGuild = FindGuild( GuildGuid );
	if( !pGuild ) return NULL;

	return pGuild->GetGuildWareHouse();
}

RC::eGUILD_RESULT GameGuildManager::CanChangeGuildName( GUILDGUID GuildGuid, CHARGUID OperCharGuid, const TCHAR* pszGuildName )
{
	GameGuild* pGuild = FindGuild( GuildGuid );
	if( NULL == pGuild )
		return RC::RC_GUILD_NOT_EXIST_GUILD;

	GameGuildMember* pMember = pGuild->FindMember( OperCharGuid );
	if( !pMember )
		return RC::RC_GUILD_NOT_GUILD_MEMBER;

	if( 0 == memcmp( pGuild->GetGuildName(),pszGuildName,MAX_GUILDNAME_LENGTH+1) )
		return RC::RC_GUILD_NAME_CHANGE_CANNOT_DB_SAME_GUILDNAME;

    util::StrArrayForGuildName guild_name;
    _tcsncpy(guild_name, pszGuildName, _countof(guild_name));
    guild_name[_countof(guild_name) - 1] = _T('\0');
    if (!ProhibitionWordParser::Instance()->CheckFilter(guild_name)) {
        return RC::RC_GUILD_NAME_BAN_WORD;
    };

	if( eGUILD_DUTY_MASTER != pMember->GetDuty() )
		return RC::RC_GUILD_NAME_CHANGE_CANNOT_GRADE;

	return RC::RC_GUILD_SUCCESS;
}

VOID GameGuildManager::ChangeGuildName( GUILDGUID guid, const TCHAR* pszGuildName )
{
	// 현재 게임 서버의 길드명을 바꾼다.
	GameGuild* pGuild = FindGuild(guid);
	if( !pGuild )
		return;

	RemoveGuildName( pGuild->GetGuildName() );
	pGuild->ChangeGuildName( pszGuildName );
	AddGuildName( pGuild->GetGuildName(), pGuild );

	// 길드 관계에 있는 길드가 가진 길드 관계정보를 바꾼다.
	STLX_VECTOR<GUILDGUID> vGuildGuid;
	if( 0 == pGuild->GetRelationGuid(vGuildGuid) )
		return;

	STLX_VECTOR<GUILDGUID>::iterator it = vGuildGuid.begin();
	for( ; it != vGuildGuid.end() ; ++it )
	{
		GameGuild* pGuild = FindGuild( *it );
		if( pGuild )
			pGuild->GetRelationEntry()->ChangeGuildName( guid, pszGuildName );
	}
}

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
VOID GameGuildManager::ApplyGuildFacilityPassive( Player* player, const bool is_add )
{
    if (player == NULL)
    {
        return;
    }
    
    GameGuild* game_guild = FindGuild(player->GetGuildGuid());
    if (game_guild == NULL)
    {
        return;
    }

    GuildFacilities*const facilities = game_guild->GetGuildFacilities();
    if (facilities == NULL)
    {
        return;
    }

    const GUILD_FACILITY_MAP* facility_map = facilities->GetGuildFacilityMap();
    if (facility_map == NULL)
    {
        return;
    }

    game_guild->ApplyGuildFacilityPassive(player, is_add, facility_map);
}

RC::eGUILD_RESULT GameGuildManager::ApplyGuildFacilityActive( Player* player, FCODE facility_code )
{
    if (player == NULL)
    {
        return RC::RC_GUILD_NOT_EXIST_PLAYER;
    }

    GameGuild* game_guild = FindGuild(player->GetGuildGuid());
    if (game_guild == NULL)
    {
        return RC::RC_GUILD_NOT_EXIST_GUILD;
    }
    
    return game_guild->ApplyGuildFacilityActive(player, facility_code);
}

VOID GameGuildManager::SendGuildFacilityTotalInfo( Player* player, GUILDGUID guild_guid )
{
    if (player == NULL)
    {
        return;
    }

    GameGuild* game_guild = FindGuild(guild_guid);
    if (game_guild != NULL)
    {
        GuildFacilities*const facilities = game_guild->GetGuildFacilities();
        //GetLoaded가 false이면 길드 정보가 DB에서 로드된 길드시설이 없는 상태이다.
        if (facilities != NULL && facilities->GetLoaded())
        {
            MSG_CG_GUILD_FACILITY_TOTALINFO_ACK total_msg;
            facilities->GetGuildFacilityTotalInfo(total_msg.guild_facility_total_info);
            player->SendPacket(&total_msg, sizeof(total_msg));
        }
        else
        {
            SUNLOG(eCRITICAL_LOG,
                "["__FUNCTION__"] 게임서버에 길드가 생성되었는데 길드 시설 정보가 로드 되지않았다(%d)", 
                guild_guid);
        }
    }
}

VOID GameGuildManager::ApplyGuildFacilityPassiveAllmember( GUILDGUID guild_guid, const bool is_add )
{
    GameGuild* game_guild = FindGuild(guild_guid);
    if (game_guild != NULL)
    {
        GuildFacilities*const facilities = game_guild->GetGuildFacilities();
        if (facilities != NULL && facilities->GetLoaded())
        {
            game_guild->ApplyGuildFacilityPassiveAllMember(is_add, facilities->GetGuildFacilityMap());
        }
        else
        {
            SUNLOG(eCRITICAL_LOG,
                "["__FUNCTION__"] 게임서버에 길드가 생성되었는데 길드 시설 정보가 로드 되지않았다(%d)", 
                guild_guid);
        }

    }
}

#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

#ifdef _NA_008334_20150608_SONNENSCHEIN
VOID GameGuildManager::InitializeAllSonnenScheinPhase()
{
    GUILD_HASH::iterator itr = m_GuildHash.begin();
    for (itr; itr != m_GuildHash.end(); ++itr)
    {
        GameGuild* game_guild = *itr;

        game_guild->InitializeSonnenScheinRewardPhase();
    }
}

BOOL GameGuildManager::SetGuildSonnenScheinPhase( GUILDGUID guid, BYTE phase )
{
    GameGuild* game_guild = FindGuild(guid);
    if (game_guild == NULL) {
        return FALSE;
    }

    game_guild->ApplyGuildRewardOptionForAllMembers(FALSE);
    game_guild->SetSonnenScheinRewardPhase(phase);
    game_guild->ApplyGuildRewardOptionForAllMembers(TRUE);
    return TRUE;
}

#endif //_NA_008334_20150608_SONNENSCHEIN