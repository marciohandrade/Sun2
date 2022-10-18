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
	// HP, MP ���� �ֱ�
	m_ServerOption.m_wPLAYER_HP_REGEN_PERIOD = GetINIDataWORD( "SERVER_OPTION", "HP_REGEN_CYCLE" );								//�÷��̾� HP�����ֱ�
	m_ServerOption.m_wPLAYER_MP_REGEN_PERIOD = GetINIDataWORD( "SERVER_OPTION", "MP_REGEN_CYCLE" );								//�÷��̾� MP�����ֱ�

    //{__NA_001290_20090525_SHIELD_SYSTEM
    m_ServerOption.m_wPLAYER_SD_REGEN_PERIOD = GetINIDataWORD( "SERVER_OPTION", "SD_REGEN_CYCLE" );								//�÷��̾� SD�����ֱ�
    m_ServerOption.player_fp_regen_period    = GetINIDataWORD("SERVER_OPTION", "FP_REGEN_CYCLE");                               //�÷��̾� FP�����ֱ�
    m_ServerOption.player_fp_maintain_time    = GetINIDataWORD("SERVER_OPTION", "FP_MAINTAIN_TIME");                               //�÷��̾� FP�����ֱ�
    m_ServerOption.m_wPLAYER_FIGHTING_TIME   = GetINIDataWORD( "SERVER_OPTION", "FIGHTING_STUTS_TIME" );						//�÷��̾� ���� ���� �ð�
    m_ServerOption.m_wNPC_FIGHTING_TIME      = GetINIDataWORD( "SERVER_OPTION", "NPC_FIGHTING_STUTS_TIME" );		    		//NPC ���� ���� �ð�
    //}

	//���� ����	
	m_ServerOption.m_fETHER_SPLASH_DAMAGE_RATIO = ( GetINIDataInt( "SERVER_OPTION", "ETHERWEAPON_SPLASH_DAMAGE_RATIO" ) / 100.0f );	//���׸����� ���÷��� ������ ���� ( 30% )
	m_ServerOption.m_fSTYLE_SPLASH_DAMAGE_RATIO = ( GetINIDataInt( "SERVER_OPTION", "STYLE_SPLASH_DAMAGE_RATIO" ) / 100.f );	//��Ÿ�� ���÷��� ������ ���� ( 50% )

	//��Ÿ
	m_ServerOption.m_wMAX_ROOM_COUNT = GetINIDataWORD( "SERVER_OPTION", "MAX_ROOM_COUNT" );										//�ִ� �氳�� ����
	m_ServerOption.m_fFINDPATH_JUMPLIMIT = GetINIDataFloat( "SERVER_OPTION", "FINDPATH_JUMP_LIMIT" );								//��ã�� JUMP LIMIT
	m_ServerOption.m_wFINDPATH_MAX_DISTANCE = GetINIDataWORD( "SERVER_OPTION", "FINDPATH_MAX_DISTANCE" );						//��ã�� �ִ� �Ÿ�
	m_ServerOption.m_wFINDPATH_MAX_NODE_COUNT= GetINIDataWORD( "SERVER_OPTION", "FINDPATH_MAX_NODE_COUNT" );						//��ã�� �ִ� ��� ����			
	m_ServerOption.m_byGRID_MOVEINFO_DISTANCE = GetINIDataBYTE( "SERVER_OPTION", "GRID_MOVEINFO_DISTANCE" );						//�ֺ� ĳ���� ���� ���۽� �̵������� �ִ� �ּҰŸ�	

	//�������� �ڵ���Ī ����
	m_ServerOption.m_byMATCHING_LEVEL_LIMIT = GetINIDataBYTE( "SERVER_OPTION", "AUTO_MATCHING_LEVEL_CONTROL" );					//�ڵ���Ī��������
	m_ServerOption.m_byMATCHING_WIN_RATIO = GetINIDataBYTE( "SERVER_OPTION", "AUTO_MATCHING_WIN_CONTROL" );						//�ڵ���Ī�·�����
	m_ServerOption.m_byMATCHING_MIN_USER_NUM = GetINIDataBYTE( "SERVER_OPTION", "MATCHING_MIN_USER_COUNT" );						//��Ī�� �ʿ��� �ּ� ������

	//�������� �÷��� �ð� 
	m_ServerOption.m_dwTIMEATTACK_PLAY_TIME = GetINIDataInt( "SERVER_OPTION", "TIME_ATTACK_PLAY_TIME" );						//Ÿ�Ӿ��� �÷��� �ð�
	m_ServerOption.m_dwSURVIVAL_PLAY_TIME = GetINIDataInt( "SERVER_OPTION", "SURVIVE_PLAY_TIME" );								//�����̹� �÷��� �ð�

	//�̵����� ���� �Ÿ��� 
	m_ServerOption.m_byMOVE_SYNC_WARNING_DISTANCE = GetINIDataBYTE( "SERVER_OPTION", "MOVE_SYNC_WARNING_DISTANCE" );				//������ Ŭ���� �÷��̾� ��ġ ��� �Ÿ�(�̵� ���)
	m_ServerOption.m_byMOVE_SYNC_FAIL_DISTANCE = GetINIDataBYTE( "SERVER_OPTION", "MOVE_SYNC_FAIL_DISTANCE" );					//������ Ŭ���� �÷��̾� ��ġ ���� �Ÿ�(�̵� ����)
	m_ServerOption.m_byMOVE_SYNC_THRUST_DISTANCE = GetINIDataBYTE( "SERVER_OPTION", "MOVE_SYNC_THRUST_DISTANCE" );					//������ Ŭ���� Ʈ����Ʈ(�и���) �ִ�Ÿ�
	m_ServerOption.m_byMOVE_SYNC_HACKING_DISTANCE = GetINIDataBYTE( "SERVER_OPTION", "MOVE_SYNC_HACKING_DISTANCE" );				//������ Ŭ���� �÷��̾� ��ġ ��ŷ �Ÿ�(�̵� ���� �� ��ŷ���� ����)
	m_ServerOption.m_wFORCED_WARP_MOVE_DELAY = GetINIDataWORD( "SERVER_OPTION", "MOVE_SYNC_FORCE_WARP_DELAY" );					//�������� �÷��̾� ���� ������ �̵� ������

	//��Ÿ
	m_ServerOption.m_fPCBANG_BONUS_EXP_RATIO = (float)(GetINIDataInt( "SERVER_OPTION", "PCBANG_BONUS_EXP_RATIO")  / 100.0f);			//PC�� �߰� ����ġ( 10% )
	m_ServerOption.m_byUseItemShopBattle = GetINIDataBYTE( "SERVER_OPTION", "USE_ITEM_SHOP_BATTLE" );							//�ʵ忡�� ������ ���� ���� �ְ� �� ���ΰ�?
	m_ServerOption.m_byPCBangSuperBangMasterAddExp = GetINIDataBYTE( "SERVER_OPTION", "PCBANG_SUPER_BANG_MASTER_ADD_EXP" );	// PC�� ���۹� ���� �߰� ����ġ
	m_ServerOption.m_byPCBangSuperBangNormalAddExp = GetINIDataBYTE( "SERVER_OPTION", "PCBANG_SUPER_BANG_NORMAL_ADD_EXP" );	// PC�� ���۹� �Ϲ��� �߰� ����ġ
	m_ServerOption.m_byItemSuperBangMasterAddExp = GetINIDataBYTE( "SERVER_OPTION", "ITEM_SUPER_BANG_MASTER_ADD_EXP" );		// PC�� ���۹� �Ϲ��� �߰� ����ġ
	m_ServerOption.m_byItemSuperBangNormalAddExp = GetINIDataBYTE( "SERVER_OPTION", "ITEM_SUPER_BANG_NORMAL_ADD_EXP" );		// PC�� ���۹� �Ϲ��� �߰� ����ġ
	m_ServerOption.m_byPCBangApplyBenefit = GetINIDataBYTE( "SERVER_OPTION", "PCBANG_APPLY_BENEFIT" );						// PC�� ���� ����
	m_ServerOption.m_byApplyItemOwnerShipByLevel = GetINIDataBYTE( "SERVER_OPTION", "APPLY_ITEM_OWNERSHIP_BY_LEVEL" );		// ���Ϳ��� �������� ���� �����ۼ������� ������ ���ΰ�?
	m_ServerOption.m_byGuildPVPUserCount = GetINIDataBYTE( "SERVER_OPTION", "GUILD_PVP_USER_COUNT" );						// ��� PVP�� ��� �ο�(����)

	// ����Ʈ
	m_ServerOption.m_wVIEWPORT_MAX_USER_COUNT = ( GetINIDataWORD( "SERVER_OPTION", "VIEWPORT_MAX_USER_COUNT" ) );				// �ֺ��� ���̴� �ִ� ������
	m_ServerOption.m_WVIEWPORT_UPDATE_TIME = ( GetINIDataWORD( "SERVER_OPTION", "VIEWPORT_UPDATE_TIME" ) );					// �ֺ� ���������� �����ϴ� �ֱ�

	// ī�� ���°��� �ɼ�
	m_ServerOption.m_byInsDungeonUseConOpt = ( GetINIDataBYTE("SERVER_OPTION", "CHAOSTATE_USE_INSTANCEDUNGEON_OPT") );				// ī���ɸ��� �ν��Ͻ����� �̿� ���� ���� �ɼ�
	m_ServerOption.m_byLoobyUseConOpt = ( GetINIDataBYTE("SERVER_OPTION", "CHAOSTATE_USE_LOBBYROOM_OPT") );							// ī�� ������ ������ �̼��� �̿��� �� �ִ�.

	m_ServerOption.m_bItemPickUpMsgBrd = ( GetINIDataBYTE("SERVER_OPTION", "ITEM_PICKUP_MSG_BRD") );									// �����۽��� �޼��� ��ε� ����.
	m_ServerOption.m_bItemPickUpPartyMsgBrd = ( GetINIDataBYTE("SERVER_OPTION", "ITEM_PICKUP_PARTY_MSG_BRD") );						// �����۽��� �޼��� ��ε� ����.

	m_ServerOption.m_wABUSE_MIN_CLIENT_LOADING_TIME = ( GetINIDataWORD("SERVER_OPTION", "ABUSE_MIN_CLIENT_LOADING_TIME" ) );									// �����۽��� �޼��� ��ε� ����.

    //__NA000000_081110_LIVE_SERVICE_TEST_CLINET_SUPPORT__
	m_ServerOption.m_bApplyTestClientAccept_UserID = FALSE;
	ZeroMemory( m_ServerOption.m_pszTestClientAccept_UserID, sizeof(m_ServerOption.m_pszTestClientAccept_UserID) );

	//#ifdef _KOREA
	//	//const CHAR* const pszUserID = GetINIDataString("SERVER_OPTION", "TEST_CLIENT_ACCEPT_USER_ID" );
	//	//if( m_ServerOption.m_bApplyTestClientAccept_UserID = ( pszUserID && pszUserID[0] ) )
	//	//{
	//m_ServerOption.m_bApplyTestClientAccept_UserID = TRUE; //temp code
	//// �빮�ڷ� ��ȯ�� �ʿ� �ִ�.
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

    /// [2012-6-7 by i4u4me] �⺻���� 1ä�η� ���� 
    if (m_ServerOption.m_byDominationChannel == 0)
    {
        m_ServerOption.m_byDominationChannel = 1;
    }
#endif

#ifdef _NA_007135_20140402_MODIFY_BATTLE_GROUND_RESERVATION_AVAILABLE_IN_BATTLE_GROUND_CHANNEL
    m_ServerOption.m_byBattleGroundChannel = ( GetINIDataInt("SERVER_OPTION", "BATTLE_GROUND_CHANNEL" ) );

    if (m_ServerOption.m_byBattleGroundChannel == 0)    //�⺻���� 1ä�η� ����
    {
        m_ServerOption.m_byBattleGroundChannel = 1;
    }
#endif
#ifdef _NA_0_20100929_LANGUAGE_OF_FILTER
    m_ServerOption.m_byLanguageOfFilter = ( GetINIDataBYTE("SERVER_OPTION", "LANGUAGE_OF_FILTER" ) );
#endif

#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
    // ���������� �ִ� �� ����
    m_ServerOption.free_pvp_max_room_count = ( GetINIDataBYTE("SERVER_OPTION", "FREE_PVP_MAX_ROOM_COUNT" ) );
    if (m_ServerOption.free_pvp_max_room_count == 0)
    {
        m_ServerOption.free_pvp_max_room_count = 100;
    }

    // ���������� �ִ� �ο���
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
    // ���������� �г�Ƽ �ּ� ���� �ο���
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
    if (m_ServerOption.monster_invasion_channel == 0)    //�⺻���� 1ä�η� ����
    {
        m_ServerOption.monster_invasion_channel = 1;
    }
#endif
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    m_ServerOption.spa_field_channel = ( GetINIDataBYTE("SERVER_OPTION", "SPA_CHANNEL"));
    // 0���̸� ä�α����� ���� �ʵ��� �Ѵ�
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
    m_ServerOption.sun_ranking_settle_channel = ( GetINIDataBYTE("SERVER_OPTION", "SUNRANKING_SETTLE_CHANNEL"));
    if (m_ServerOption.sun_ranking_settle_channel == 0)    //�⺻���� 1ä�η� ����
    {
        m_ServerOption.sun_ranking_settle_channel = 1;
    }
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM

	return TRUE;
}

