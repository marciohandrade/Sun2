#include "StdAfx.h"
#include ".\serveroptionparserex.h"

ServerOptionParserEx::ServerOptionParserEx(void)
{
}

ServerOptionParserEx::~ServerOptionParserEx(void)
{
}

VOID	ServerOptionParserEx::Release()
{
	ZeroMemory( &m_ServerOption, sizeof( m_ServerOption ) );
}

BOOL	ServerOptionParserEx::LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload )
{
	switch( ScriptCode )
	{
	case SCRIPT_SERVER_OPTION:
		return _Load( bReload );
	}

	return FALSE;	
}

BOOL			ServerOptionParserEx::_Load( BOOL bReload )
{
    __UNUSED(bReload);
	//GAME_OPTION
	// HP, MP 리젠 주기
	m_ServerOption.m_wPLAYER_HP_REGEN_PERIOD = GetINIDataWORD( "SERVER_OPTION", "HP_REGEN_CYCLE" );								//플레이어 HP리젠주기
	m_ServerOption.m_wPLAYER_MP_REGEN_PERIOD = GetINIDataWORD( "SERVER_OPTION", "MP_REGEN_CYCLE" );								//플레이어 MP리젠주기

    //{__NA_001290_20090525_SHIELD_SYSTEM
    m_ServerOption.m_wPLAYER_SD_REGEN_PERIOD = GetINIDataWORD( "SERVER_OPTION", "SD_REGEN_CYCLE" );								//플레이어 SD리젠주기
    m_ServerOption.player_fp_regen_period    = GetINIDataWORD("SERVER_OPTION", "FP_REGEN_CYCLE");                               //플레이어 FP리젠주기
    m_ServerOption.player_fp_maintain_time    = GetINIDataWORD("SERVER_OPTION", "FP_MAINTAIN_TIME");                               //플레이어 FP리젠주기
    m_ServerOption.m_wPLAYER_FIGHTING_TIME   = GetINIDataWORD( "SERVER_OPTION", "FIGHTING_STUTS_TIME" );						//플레이어 전투 상태 시간
    m_ServerOption.m_wNPC_FIGHTING_TIME      = GetINIDataWORD( "SERVER_OPTION", "NPC_FIGHTING_STUTS_TIME" );		    		//NPC 전투 상태 시간
    //}

	//공격 관련	
	m_ServerOption.m_fETHER_SPLASH_DAMAGE_RATIO = ( GetINIDataInt( "SERVER_OPTION", "ETHERWEAPON_SPLASH_DAMAGE_RATIO" ) / 100.0f );	//에테르웨폰 스플래쉬 데미지 비율 ( 30% )
	m_ServerOption.m_fSTYLE_SPLASH_DAMAGE_RATIO = ( GetINIDataInt( "SERVER_OPTION", "STYLE_SPLASH_DAMAGE_RATIO" ) / 100.f );	//스타일 스플래쉬 데미지 비율 ( 50% )

	//기타
	m_ServerOption.m_wMAX_ROOM_COUNT = GetINIDataWORD( "SERVER_OPTION", "MAX_ROOM_COUNT" );										//최대 방개수 제한
	m_ServerOption.m_fFINDPATH_JUMPLIMIT = GetINIDataFloat( "SERVER_OPTION", "FINDPATH_JUMP_LIMIT" );								//길찾기 JUMP LIMIT
	m_ServerOption.m_wFINDPATH_MAX_DISTANCE = GetINIDataWORD( "SERVER_OPTION", "FINDPATH_MAX_DISTANCE" );						//길찾기 최대 거리
	m_ServerOption.m_wFINDPATH_MAX_NODE_COUNT= GetINIDataWORD( "SERVER_OPTION", "FINDPATH_MAX_NODE_COUNT" );						//길찾기 최대 노드 개수			
	m_ServerOption.m_byGRID_MOVEINFO_DISTANCE = GetINIDataBYTE( "SERVER_OPTION", "GRID_MOVEINFO_DISTANCE" );						//주변 캐릭터 정보 전송시 이동정보를 주는 최소거리	

	//경쟁헌팅 자동매칭 제어
	m_ServerOption.m_byMATCHING_LEVEL_LIMIT = GetINIDataBYTE( "SERVER_OPTION", "AUTO_MATCHING_LEVEL_CONTROL" );					//자동매칭레벨제어
	m_ServerOption.m_byMATCHING_WIN_RATIO = GetINIDataBYTE( "SERVER_OPTION", "AUTO_MATCHING_WIN_CONTROL" );						//자동매칭승률제어
	m_ServerOption.m_byMATCHING_MIN_USER_NUM = GetINIDataBYTE( "SERVER_OPTION", "MATCHING_MIN_USER_COUNT" );						//매칭에 필요한 최소 유저수

	//경쟁헌팅 플레이 시간 
	m_ServerOption.m_dwTIMEATTACK_PLAY_TIME = GetINIDataInt( "SERVER_OPTION", "TIME_ATTACK_PLAY_TIME" );						//타임어택 플레이 시간
	m_ServerOption.m_dwSURVIVAL_PLAY_TIME = GetINIDataInt( "SERVER_OPTION", "SURVIVE_PLAY_TIME" );								//서바이벌 플레이 시간

	//이동동기 관련 거리차 
	m_ServerOption.m_byMOVE_SYNC_WARNING_DISTANCE = GetINIDataBYTE( "SERVER_OPTION", "MOVE_SYNC_WARNING_DISTANCE" );				//서버와 클라간의 플레이어 위치 경고 거리(이동 허용)
	m_ServerOption.m_byMOVE_SYNC_FAIL_DISTANCE = GetINIDataBYTE( "SERVER_OPTION", "MOVE_SYNC_FAIL_DISTANCE" );					//서버와 클라간의 플레이어 위치 실패 거리(이동 실패)
	m_ServerOption.m_byMOVE_SYNC_THRUST_DISTANCE = GetINIDataBYTE( "SERVER_OPTION", "MOVE_SYNC_THRUST_DISTANCE" );					//서버와 클라간의 트러스트(밀리기) 최대거리
	m_ServerOption.m_byMOVE_SYNC_HACKING_DISTANCE = GetINIDataBYTE( "SERVER_OPTION", "MOVE_SYNC_HACKING_DISTANCE" );				//서버와 클라간의 플레이어 위치 해킹 거리(이동 실패 및 해킹으로 간주)
	m_ServerOption.m_wFORCED_WARP_MOVE_DELAY = GetINIDataWORD( "SERVER_OPTION", "MOVE_SYNC_FORCE_WARP_DELAY" );					//서버에서 플레이어 강제 워프시 이동 딜레이

	//기타
	m_ServerOption.m_fPCBANG_BONUS_EXP_RATIO = (float)(GetINIDataInt( "SERVER_OPTION", "PCBANG_BONUS_EXP_RATIO")  / 100.0f);			//PC방 추가 경험치( 10% )
	m_ServerOption.m_byUseItemShopBattle = GetINIDataBYTE( "SERVER_OPTION", "USE_ITEM_SHOP_BATTLE" );							//필드에서 아이템 샵을 쓸수 있게 할 것인가?
	m_ServerOption.m_byPCBangSuperBangMasterAddExp = GetINIDataBYTE( "SERVER_OPTION", "PCBANG_SUPER_BANG_MASTER_ADD_EXP" );	// PC방 슈퍼방 방장 추가 경험치
	m_ServerOption.m_byPCBangSuperBangNormalAddExp = GetINIDataBYTE( "SERVER_OPTION", "PCBANG_SUPER_BANG_NORMAL_ADD_EXP" );	// PC방 슈퍼방 일반인 추가 경험치
	m_ServerOption.m_byItemSuperBangMasterAddExp = GetINIDataBYTE( "SERVER_OPTION", "ITEM_SUPER_BANG_MASTER_ADD_EXP" );		// PC방 슈퍼방 일반인 추가 경험치
	m_ServerOption.m_byItemSuperBangNormalAddExp = GetINIDataBYTE( "SERVER_OPTION", "ITEM_SUPER_BANG_NORMAL_ADD_EXP" );		// PC방 슈퍼방 일반인 추가 경험치
	m_ServerOption.m_byPCBangApplyBenefit = GetINIDataBYTE( "SERVER_OPTION", "PCBANG_APPLY_BENEFIT" );						// PC방 헤택 여부
	m_ServerOption.m_byApplyItemOwnerShipByLevel = GetINIDataBYTE( "SERVER_OPTION", "APPLY_ITEM_OWNERSHIP_BY_LEVEL" );		// 몬스터와의 레벨차에 의한 아이템소유권을 적용할 것인가?
	m_ServerOption.m_byGuildPVPUserCount = GetINIDataBYTE( "SERVER_OPTION", "GUILD_PVP_USER_COUNT" );						// 길드 PVP시 대결 인원(각각)

	// 뷰포트
	m_ServerOption.m_wVIEWPORT_MAX_USER_COUNT = ( GetINIDataWORD( "SERVER_OPTION", "VIEWPORT_MAX_USER_COUNT" ) );				// 주변에 보이는 최대 유저수
	m_ServerOption.m_WVIEWPORT_UPDATE_TIME = ( GetINIDataWORD( "SERVER_OPTION", "VIEWPORT_UPDATE_TIME" ) );					// 주변 유저정보를 갱신하는 주기

	// 카오 상태관련 옵션
	m_ServerOption.m_byInsDungeonUseConOpt = ( GetINIDataBYTE("SERVER_OPTION", "CHAOSTATE_USE_INSTANCEDUNGEON_OPT") );				// 카오케릭의 인스턴스던전 이용 가능 설정 옵션
	m_ServerOption.m_byLoobyUseConOpt = ( GetINIDataBYTE("SERVER_OPTION", "CHAOSTATE_USE_LOBBYROOM_OPT") );							// 카오 상태의 유저가 미션을 이용할 수 있다.

	m_ServerOption.m_bItemPickUpMsgBrd = ( GetINIDataBYTE("SERVER_OPTION", "ITEM_PICKUP_MSG_BRD") );									// 아이템습득 메세지 브로드 여부.
	m_ServerOption.m_bItemPickUpPartyMsgBrd = ( GetINIDataBYTE("SERVER_OPTION", "ITEM_PICKUP_PARTY_MSG_BRD") );						// 아이템습득 메세지 브로드 여부.

	m_ServerOption.m_wABUSE_MIN_CLIENT_LOADING_TIME = ( GetINIDataWORD("SERVER_OPTION", "ABUSE_MIN_CLIENT_LOADING_TIME" ) );									// 아이템습득 메세지 브로드 여부.

    //__NA000000_081110_LIVE_SERVICE_TEST_CLINET_SUPPORT__
	m_ServerOption.m_bApplyTestClientAccept_UserID = FALSE;
	ZeroMemory( m_ServerOption.m_pszTestClientAccept_UserID, sizeof(m_ServerOption.m_pszTestClientAccept_UserID) );

	//#ifdef _KOREA
	//	//const CHAR* const pszUserID = GetINIDataString("SERVER_OPTION", "TEST_CLIENT_ACCEPT_USER_ID" );
	//	//if( m_ServerOption.m_bApplyTestClientAccept_UserID = ( pszUserID && pszUserID[0] ) )
	//	//{
	//m_ServerOption.m_bApplyTestClientAccept_UserID = TRUE; //temp code
	//// 대문자로 변환할 필요 있다.
	//		strncpy(
	//			m_ServerOption.m_pszTestClientAccept_UserID, "CRAZYTOTO",// pszUserID,
	//			_countof(m_ServerOption.m_pszTestClientAccept_UserID)
	//			);
	//		m_ServerOption.m_pszTestClientAccept_UserID[sizeof(m_ServerOption.m_pszTestClientAccept_UserID)-1] = '\0';
	//	//}
	//#endif
    //__NA001283_20090225_COLLECTION_USE
	m_ServerOption.m_DelayCollectionRespawnTime = ( GetINIDataInt("SERVER_OPTION", "DELAY_COLLECTION_RESPAWN_TIME" ) );
    //__NA000000_20090320_DELAY_ITEM_DROP_TIME
	m_ServerOption.m_DelayDropTime = ( GetINIDataInt("SERVER_OPTION", "DELAY_ITEM_DROP_TIME" ) );
#if !defined(_NA_20110610_MOVE_DOMINATION_CHANNEL_INFO)
    //_NA001385_20090924_DOMINATION_ETC
    m_ServerOption.m_byDominationChannel = ( GetINIDataBYTE("SERVER_OPTION", "DOMINATION_CHANNEL" ) );

    /// [2012-6-7 by i4u4me] 기본값을 1채널로 고정 
    if (m_ServerOption.m_byDominationChannel == 0)
    {
        m_ServerOption.m_byDominationChannel = 1;
    }
#endif

#ifdef _NA_007135_20140402_MODIFY_BATTLE_GROUND_RESERVATION_AVAILABLE_IN_BATTLE_GROUND_CHANNEL
    m_ServerOption.m_byBattleGroundChannel = ( GetINIDataInt("SERVER_OPTION", "BATTLE_GROUND_CHANNEL" ) );

    if (m_ServerOption.m_byBattleGroundChannel == 0)    //기본값을 1채널로 고정
    {
        m_ServerOption.m_byBattleGroundChannel = 1;
    }
#endif
#ifdef _NA_0_20100929_LANGUAGE_OF_FILTER
    m_ServerOption.m_byLanguageOfFilter = ( GetINIDataBYTE("SERVER_OPTION", "LANGUAGE_OF_FILTER" ) );
#endif

#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
    // 자유전투실 최대 방 갯수
    m_ServerOption.free_pvp_max_room_count = ( GetINIDataBYTE("SERVER_OPTION", "FREE_PVP_MAX_ROOM_COUNT" ) );
    if (m_ServerOption.free_pvp_max_room_count == 0)
    {
        m_ServerOption.free_pvp_max_room_count = 100;
    }

    // 자유전투실 최대 인원수
    m_ServerOption.free_pvp_max_user_count = ( GetINIDataBYTE("SERVER_OPTION", "FREE_PVP_MAX_USER_COUNT" ) );
    if (m_ServerOption.free_pvp_max_user_count == 0)
    {
        m_ServerOption.free_pvp_max_user_count = 20;
    }
#endif // _NA_006088_20120910_CHAOS_ZONE_FREE_PVP

#ifdef _NA_006538_BATTLE_GROUND_TIME_CONTROL_SYSTEM
    m_ServerOption.battle_ground_time_control_system = (GetINIDataBYTE("SERVER_OPTION", "BATTLE_GROUND_TIME_CONTROL_SYSTEM"));
#endif // _NA_006538_BATTLE_GROUND_TIME_CONTROL_SYSTEM

#ifdef _NA_000000_20130429_DOMINATION_PENALTY
    // 지역점령전 패널티 최소 참여 인원수
    m_ServerOption.domination_penalty.minimum_participation_members = \
        (GetINIDataWORD("DOMINATION_PENALTY", "MINIMUM_PARTICIPATION_MEMBERS"));
    if (m_ServerOption.domination_penalty.minimum_participation_members == 0) {
        m_ServerOption.domination_penalty.minimum_participation_members = 5;
    }

    m_ServerOption.domination_penalty.penalty_days_of_offence_fail = \
        (GetINIDataWORD("DOMINATION_PENALTY", "PENALTY_DAYS_OF_OFFENCE_FAIL"));
    if (m_ServerOption.domination_penalty.penalty_days_of_offence_fail == 0) {
        m_ServerOption.domination_penalty.penalty_days_of_offence_fail = 28; // 4 weeks
    }

    m_ServerOption.domination_penalty.penalty_days_of_defense_fail = \
        (GetINIDataWORD("DOMINATION_PENALTY", "PENALTY_DAYS_OF_DEFENSE_FAIL"));
    if (m_ServerOption.domination_penalty.penalty_days_of_defense_fail == 0) {
        m_ServerOption.domination_penalty.penalty_days_of_defense_fail = 7; // 1 week
    }
#endif //_NA_000000_20130429_DOMINATION_PENALTY

#ifdef _NA_007992_20150127_EVENT_MONSTER_INVASION
    m_ServerOption.monster_invasion_channel = ( GetINIDataBYTE("SERVER_OPTION", "MONSTER_INVASION_CHANNEL"));
    if (m_ServerOption.monster_invasion_channel == 0)    //기본값을 1채널로 고정
    {
        m_ServerOption.monster_invasion_channel = 1;
    }
#endif
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    m_ServerOption.spa_field_channel = ( GetINIDataBYTE("SERVER_OPTION", "SPA_CHANNEL"));
    // 0값이면 채널구분을 하지 않도록 한다
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
    m_ServerOption.sun_ranking_settle_channel = ( GetINIDataBYTE("SERVER_OPTION", "SUNRANKING_SETTLE_CHANNEL"));
    if (m_ServerOption.sun_ranking_settle_channel == 0)    //기본값을 1채널로 고정
    {
        m_ServerOption.sun_ranking_settle_channel = 1;
    }
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM

	return TRUE;
}

