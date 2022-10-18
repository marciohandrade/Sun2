#ifndef __SERVER_STRUCT_H__
#define __SERVER_STRUCT_H__

#include <Define.h>
#include <Struct.h>
#include <StructInPacket.h>
#include <ServerStructInPacket.h>
//#include <ArchiveOfStream.h>

//==================================================================================================
// @history
//  ~11.05.21
//  11.05.21, waverix, remove pack directive

//==================================================================================================
// (WAVERIX)(WARNING)(070525)
//   MAX_WORLD_NAME_LEN = define.h���� ���ǵǾ� ������, 32�� �Ǿ� ����.
//   �Ʒ� ��ó����� ������� �����Ƿ� ����
//#define MAX_WORLD_NAME_LEN		16

// (WAVERIX) (090728) (ADD-IN) �Ϲ̸� �ʵη� �������� UserID(unified:50)�� ������ ������ �߰��ȴ�.
// �� ������ ���� �۾��� �ʿ䰡 �����ϰ� �Ǿ���.
// (reference) __NA000000_SHOP_USERID_50_PASSING_PROC__
#define ID_MAX_LEN_FOR_SHOP_USER_ID     (50)

#define PARSER_END_ROW_KEY		7777

#ifdef _NA_0_20100819_AGGRO_RENEWAL
#define AGGRO_CHANGE_RATIO_MAX	2 
#endif

#pragma warning(push)
#pragma warning(disable : 4201)
typedef struct tagSERVER_KEY
{
	union
	{
		struct
		{
			BYTE m_WorldID;
			BYTE m_ChannelID;
			BYTE m_ServerType;
			BYTE m_ServerID;
		};
		DWORD m_dwKey;
	};
	
	tagSERVER_KEY( const DWORD key = 0 )	{ m_dwKey = key; }
	tagSERVER_KEY( BYTE worldID, BYTE channelID, BYTE serverType, BYTE serverID )	{ Set(worldID, channelID, serverType, serverID); }
	operator DWORD()						{ return m_dwKey; }
	DWORD Set( BYTE worldID, BYTE channelID, BYTE serverType, BYTE serverID )
	{
		m_WorldID		= worldID;
		m_ChannelID		= channelID;
		m_ServerType	= serverType;
		m_ServerID		= serverID;

		return m_dwKey;
	}
	DWORD GetOldServerKey()
	{
        // (NOTE) (WAVERIX) �߸��� ó���� �����Ѵ�. ���� �ݼ��ؾ� �Ѵ�. ���� �ͼ� �ٲ� ���� ����.
		return (((m_dwKey)&0x000000FF << 24) | ((m_dwKey)&0x0000FF00 << 8) | ((m_dwKey)&0x00FF0000 >> 8) | ((m_dwKey)&0xFF000000 >> 24));
		//or
		//return ((m_ServerID << 24) | (m_ServerType << 16) | (m_ChannelID >> 8) | (m_WorldID));
	}

    BYTE GetWorldID() const { return m_WorldID; }
    BYTE GetChannelID() const { return m_ChannelID; }
    BYTE GetServerType() const { return m_ServerType; }
    BYTE GetServerID() const { return m_ServerID; }
} SERVER_KEY, *LPSERVER_KEY;
#pragma warning(pop)

typedef struct tagSERVER_INFO
{
	SERVER_KEY		ServerKey;
	DWORD			dwServerGUID;
	char			szIP[MAX_IP_STRING_LEN+1];
	WORD			wPort;
	char			szInnerIP[MAX_IP_STRING_LEN+1];
	WORD			wInnerPort;
} SERVER_INFO, *LPSERVER_INFO;

typedef struct tagGLOBAL_SERVER_INFO_STRUCT
{
	CHAR			szMasterIP[MAX_IP_STRING_LEN+1];
	SERVER_KEY		skServerKey;
	CHAR			szPublicIP[MAX_IP_STRING_LEN+1];
	WORD			wPublicPort;
	CHAR			szNatIP[MAX_IP_STRING_LEN+1];
	CHAR			szInnerIP[MAX_IP_STRING_LEN+1];
	WORD			wInnerPort;
} GLOBAL_SERVER_INFO_STRUCT, *LPGLOBAL_SERVER_INFO_STRUCT;

struct SERVER_CHARACTER_PART
{
	CHARGUID						m_CharGuid;
	BYTE							m_bySlot;
	TCHAR							m_tszCharName[MAX_CHARNAME_LENGTH+1];		//< Agent���� �ٷ� ����ϱ� ����
	BYTE							m_byHeight;
	BYTE							m_byFace;
	BYTE							m_byHair;
	BYTE							m_byClass;
	LEVELTYPE						m_LV;
	BYTE							m_CharState;
	INT								m_iRegion;
	SHORT							m_sX;
	SHORT							m_sY;
	SHORT							m_sZ;

	BYTE							m_ChangeOfClassStep;

	// ��� ���� : 2005/12/28(taiyo)
	GUILDGUID						m_GuildGuid;
	eGUILD_DUTY						m_eGuildDuty;
	TCHAR							m_tszGuildNicName[MAX_CHARNAME_LENGTH+1];

    //__NA001084_PET_IMAGE_DRAW_IN_CHARACTER_SELECT_SCREEN
	ITEMSTREAMEX					m_PetSlotStream;
	BOOL							m_bSummoned;
	// ���� ���� ����â ����
	_EQUIP_TOTAL_INFO				m_EquipItemInfo;

	BYTE							m_DelChk;
	TCHAR							m_tszDelDate[MAX_SMALLDATETIME_SIZE+1];
    // f100929.4L, removed an unused section
    //STATE_DETAIL_TOTAL_INFO			m_TotalStateInfo;
#ifdef _NA_002050_20110216_ADD_GENDER
    BYTE gender; // ����
#endif
#ifdef _NA_003027_20111013_HONOR_SYSTEM
	WORD							honor_title;
#endif //_NA_003027_20111013_HONOR_SYSTEM
#ifdef _NA_004035_20120227_COSTUME_ITEM
    bool is_equipped_costume; // �ڽ�Ƭ Ȱ��ȭ ����
    POSTYPE costume_item_pos; // �ڽ�Ƭ �κ��丮 ��ġ
    SLOTCODE costume_item_code; // �ڽ�Ƭ ������ ��ȣ

#ifdef _NA_008610_20160104_ADD_DECORATECOSTUME
    bool is_equipped_deco_costume; // ���� �ڽ�Ƭ Ȱ��ȭ ����
    POSTYPE deco_costume_item_pos; // ���� �ڽ�Ƭ �κ��丮 ��ġ
    SLOTCODE deco_costume_item_code; // ���� �ڽ�Ƭ ������ ��ȣ
#endif //_NA_008610_20160104_ADD_DECORATECOSTUME
#endif  // _NA_004035_20120227_COSTUME_ITEM
    //----------------------------------------------------------------------------------------------
    void Output(ArchiveOfStream& archive);
    void Input(ArchiveOfStream& archive);
};

typedef struct tagLOGOUT_AUTH_DATA
{
	DWORD			dwAuthUserID;					// AuthServer���� �߱��� �ĺ���
#ifdef __CH1305_090303_ACCOUNT_ID_LENGTH_EXTENTION__
    char			szAccountID[ID_MAX_LEN + 1];	// ���� ���̵�
#else
	char			szAccountID[ID_MAX_LEN];		// ���� ���̵�
#endif
	char			szPCName[MAX_CHARNAME_LENGTH];	// ĳ���� �̸� =>Ŭ������ ���� ����->DBA��û�� ���� �ӽ�!!
	WORD			wSvrCode;						// ���� �ڵ�
	WORD			wPCLevel;						// ĳ���� ���� =>Ŭ������ ���� ����->DBA��û�� ���� �ӽ�!!
	BYTE			byPCClass;						// ĳ���� Ŭ���� =>Ŭ������ ���� ����->DBA��û�� ���� �ӽ�!!
	BYTE			byLogoutType;					// �α׾ƿ� Ÿ��
	TCHAR			pszLoginTime[MAX_TIMEDATA_SIZE];
} LOGOUT_AUTH_DATA, *LPLOGOUT_AUTH_DATA;

struct SUNLOG_INFO
{
	BYTE	bySunLogOption;
	WORD	wSunLogfileLevel;
	char	pszSunLogfilePath[MAX_LOGPATH_LEN];
};

struct INI_IOHANDLER_DESC
{
	char	szIP[MAX_IPADDRESS_SIZE];
	WORD	wPort;
	DWORD	dwMaxAcceptSession;
	DWORD	dwMaxConnectSession;
	DWORD	dwSendBufferSize;
	DWORD	dwRecvBufferSize;
	DWORD	dwTimeOut;
	DWORD	dwNumberOfAcceptThreads;
	DWORD	dwNumberOfIoThreads;
	DWORD	dwNumberOfConnectThreads;
	DWORD	dwMaxPacketSize;
};

struct SERVER_ENV
{
	//Common Description
	BOOL	bGetServerInfoFromFile;
	BOOL	bGMListApply;
	BYTE	byFieldServerType;
	BYTE	byBattleServerType;
	BOOL	bVillageRenderInfo;
	BYTE	byServiceMode;						//���� ��� ���� 0:�ָܼ��, 1:���� ���.......
	BYTE	byAgeGrade;							//0 : 18���̸�,, 1 : 18���̻� ...
	BYTE	byServerStatus;						//0 : ���߿� ����, 1: ����(���̺꼭��) ����

	// IOCP ����
	INI_IOHANDLER_DESC ClientIoHandler;
	INI_IOHANDLER_DESC ServerIoHandler;

	char	pszMasterServerIP[ MAX_IPADDRESS_SIZE ];
	WORD	wMasterServerPort;

	char	pszAccountDBProxyServerIP[ MAX_IPADDRESS_SIZE ];
	WORD	wAccountDBProxyServerPort;

	char	pszGameDBProxyServerIP[ MAX_IPADDRESS_SIZE ];
	WORD	wGameDBProxyServerPort;

	char	pszAgentServerOutSideIP[ MAX_IPADDRESS_SIZE ];
	WORD	wAgentServerOutSidePort;
	char	pszAgentServerInSideIP[ MAX_IPADDRESS_SIZE ];
	WORD	wAgentServerInSidePort;

	char	pszPortalServerIP[ MAX_IPADDRESS_SIZE ];
	WORD	wPortalServerPort;

	BOOL	bEnableAuthAgentServer;
	WORD	wAgentServerID;
	char	pszAuthAgentServerIP[ MAX_IPADDRESS_SIZE ];
	WORD	wAuthAgentServerPort;

	char	pszWorldServerIP[ MAX_IPADDRESS_SIZE ];
	WORD	wWorldServerPort;
	char	pszWorldServerClientIP[ MAX_IPADDRESS_SIZE ];
	WORD	wWorldServerClientPort;

	char	pszGuildServerIP[ MAX_IPADDRESS_SIZE ];
	WORD	wGuildServerPort;

	char	pszSQLServerIP[ MAX_IPADDRESS_SIZE ];

	//WOPS_LOG
	char	pszLogPath[MAX_LOGPATH_LEN];		//�α����� ��� ���� ����
	BYTE	byLogOption;

	//��αװ��� ����
	BYTE	bySunLogOption;
	BYTE	bySunLogfileLevel;
	char	pszSunLogfilePath[MAX_LOGPATH_LEN];
	DWORD	dwServerInfoCycle;
}; // ���� ȯ�� ����


struct SERVER_COMMON_OPTION
{
	// HP, MP ���� �ֱ� ����
	WORD		m_wPLAYER_HP_REGEN_PERIOD;			// �÷��̾� HP ���� �ֱ�
    WORD		m_wPLAYER_MP_REGEN_PERIOD;			// �÷��̾� MP ���� �ֱ�
    //{__NA_001290_20090525_SHIELD_SYSTEM
    WORD		m_wPLAYER_SD_REGEN_PERIOD;			// �÷��̾� SD ���� �ֱ�
    WORD        player_fp_regen_period;             // �÷��̾� FP ���� �ֱ�
    WORD        player_fp_maintain_time;            // �÷��̾� FP �����ð�
    WORD		m_wPLAYER_FIGHTING_TIME;			// �÷��̾� ���� ���� �ð�
    WORD		m_wNPC_FIGHTING_TIME;			    // NPC ���� ���� �ð�
    //}
	// ���� ����
	float		m_fETHER_SPLASH_DAMAGE_RATIO;		// ��Ű�� ���׸� ���� ���÷��� ������ ����
	float		m_fSTYLE_SPLASH_DAMAGE_RATIO;		// �Ϲ� ��Ÿ�� ���÷��� ������ ����

	// ��Ÿ
	WORD		m_wMAX_ROOM_COUNT;					// ��Ʋ���� �ϳ��� �ִ� �氳�� ����
	float		m_fFINDPATH_JUMPLIMIT;				// ��ã�� JumpLimit
	WORD		m_wFINDPATH_MAX_DISTANCE;			// ��ã�� �ִ� �Ÿ�
	WORD		m_wFINDPATH_MAX_NODE_COUNT;			// ��ã�� �ִ� ��尳��
	BYTE		m_byGRID_MOVEINFO_DISTANCE;			// �ֺ� ĳ���� ���� ���۽� �̵������� �ִ� �ּҰŸ�

	// �������� �ڵ���Ī ����
	BYTE		m_byMATCHING_LEVEL_LIMIT;			// ���� ����
	BYTE		m_byMATCHING_WIN_RATIO;				// �·� ����
	BYTE		m_byMATCHING_MIN_USER_NUM;			// ��Ī�� �ʿ��� �ּ� ������

	// �������� �÷��� �ð�
	DWORD		m_dwTIMEATTACK_PLAY_TIME;			// Ÿ�Ӿ��� �÷��� Ÿ��
	DWORD		m_dwSURVIVAL_PLAY_TIME;				// �����̹� �÷��� Ÿ��

	// �̵����� ���� �Ÿ���
	BYTE		m_byMOVE_SYNC_WARNING_DISTANCE;		// ������ Ŭ���� �÷��̾� ��ġ ��� �Ÿ�(�̵� ���)
	BYTE		m_byMOVE_SYNC_FAIL_DISTANCE;		// ������ Ŭ���� �÷��̾� ��ġ ���� �Ÿ�(�̵� ����)
	BYTE		m_byMOVE_SYNC_THRUST_DISTANCE;		// ������ Ŭ���� Ʈ����Ʈ(�и���) �ִ� �Ÿ�
	BYTE		m_byMOVE_SYNC_HACKING_DISTANCE;		// ������ Ŭ���� �÷��̾� ��ġ ��ŷ �Ÿ�(�̵� ���� �� ��ŷ���� ����)
	WORD		m_wFORCED_WARP_MOVE_DELAY;			// �������� �÷��̾� ���� ������ �̵� ������

	// PC�� ����ġ �߰�
	float		m_fPCBANG_BONUS_EXP_RATIO;			// PC�� ����ġ �߰� ����

	BYTE		m_byUseItemShopBattle;				// ��Ʋ�������� ������ ���� ���� �ְ� �� ���ΰ�?
	BYTE		m_byPCBangSuperBangMasterAddExp;	// PC�� ���۹� ���� �߰� ����ġ
	BYTE		m_byPCBangSuperBangNormalAddExp;	// PC�� ���۹� �Ϲ��� �߰� ����ġ
	BYTE		m_byItemSuperBangMasterAddExp;		// ������ ���۹� ���� �߰� ����ġ
	BYTE		m_byItemSuperBangNormalAddExp;		// ������ ���۹� �Ϲ��� �߰� ����ġ

	BYTE		m_byPCBangApplyBenefit;				// PC�� ���� ���� ����
	BYTE		m_byApplyItemOwnerShipByLevel;		// ���Ϳ��� �������� ���� �����ۼ������� ������ ���ΰ�?
	BYTE		m_byGuildPVPUserCount;				// ��� PVP�� �ο��� 

	WORD		m_wVIEWPORT_MAX_USER_COUNT;			// �ֺ��� ���̴� �ִ� ������
	WORD		m_WVIEWPORT_UPDATE_TIME;			// �ֺ� ���������� �����ϴ� �ֱ�

	BOOL		m_byInsDungeonUseConOpt;			// ī���ɸ��� �ν��Ͻ����� �̿� ���� ���� �ɼ�
	BOOL		m_byLoobyUseConOpt;					// ī���ɸ��� �κ� �̿� ���� ���� �ɼ�

	BOOL		m_bItemPickUpMsgBrd;				// �����۽��� �޼��� �������� ��ε� ����.
	BOOL		m_bItemPickUpPartyMsgBrd;			// �����۽��� �޼��� ��Ƽ���鿡�� ��ε� ����.
	WORD		m_wABUSE_MIN_CLIENT_LOADING_TIME;	// ���� Ŭ���� �ʷε��� �������� �ð��� �ɸ���
    //__NA000000_081110_LIVE_SERVICE_TEST_CLINET_SUPPORT__ - unused section
	BOOLEAN		m_bApplyTestClientAccept_UserID;
	CHAR		m_pszTestClientAccept_UserID[ID_MAX_LEN+1];	// ���̺꿡�� Ư�� ������ ���� C/S���� ü�� ��ȭ ó�� ����
    //__NA001283_20090225_COLLECTION_USE
	DWORD		m_DelayCollectionRespawnTime;
    DWORD		m_DelayDropTime;
#if !defined(_NA_20110610_MOVE_DOMINATION_CHANNEL_INFO)
    BYTE		m_byDominationChannel; //_NA001385_20090924_DOMINATION_ETC
#endif
#ifdef _NA_007135_20140402_MODIFY_BATTLE_GROUND_RESERVATION_AVAILABLE_IN_BATTLE_GROUND_CHANNEL
    int         m_byBattleGroundChannel;  //���� ���� ���� ä�� ����
#endif
#ifdef _NA_0_20100929_LANGUAGE_OF_FILTER
    BYTE        m_byLanguageOfFilter;
#endif
#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
    BYTE        free_pvp_max_room_count; // ���������� �ִ� �� ���� ����.
    BYTE        free_pvp_max_user_count; // ���������� �ִ� ���� ����.
#endif // _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
#ifdef _NA_006538_BATTLE_GROUND_TIME_CONTROL_SYSTEM
    BYTE        battle_ground_time_control_system; // ���� �ð����� �ý��� Ȱ��ȭ ����
#endif // _NA_006538_BATTLE_GROUND_TIME_CONTROL_SYSTEM
#ifdef _NA_007992_20150127_EVENT_MONSTER_INVASION
    BYTE        monster_invasion_channel;   //����ħ�� ä�� ����
#endif //_NA_007992_20150127_EVENT_MONSTER_INVASION
#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
    BYTE        sun_ranking_settle_channel;
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    BYTE        spa_field_channel;
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
    
    //_NA_000000_20130429_DOMINATION_PENALTY
    struct 
    {
        WORD minimum_participation_members; // ������ �г�Ƽ ���� �ּ� �ο��� 
        WORD penalty_days_of_offence_fail; //���� ���н� ���� �Ұ� �г�Ƽ �Ⱓ(��)
        WORD penalty_days_of_defense_fail; // ���� ���н� ���� �Ұ� �г�Ƽ �Ⱓ(��)
    } domination_penalty;
};

struct SERVER_POOL_OPTION
{
	BYTE		m_byIndex;							// �ε���
	char		m_szServerName[16];					// ��������
	WORD		m_wPlayerPool;
	WORD		m_wNPCPool;
	WORD		m_wItemPool;
	WORD		m_wMapObjectPool;
	WORD		m_wMapNPCPool;
	WORD		m_wMerchantPool;
	WORD		m_wPlayerSkillPool;
	WORD		m_wNPCSkillPool;
	WORD		m_wAbilityPool;
	WORD		m_wPartyPool;
	WORD		m_wSummonPool;
	WORD		m_wStatusPool;
	WORD		m_wCoolTimerCharPool;
	WORD		m_wMapPool;	
	WORD		m_wSectorPool;
	WORD		m_wTradePool;
	WORD		m_wObjGroupPool;

	// ���� Ǯ
	WORD		m_wItemSlotPool;
	WORD		m_wSkillSlotPool;
	WORD		m_wQuickSlotPool;
	WORD		m_wQuickStyleSlotPool;
	WORD		m_wTradeSlotPool;
	WORD		m_wVendorSlotPool;
	WORD		m_wEventSlotPool;

	// �ļ� Ǯ
	WORD		m_wItemInfo;
	WORD		m_wNPCInfo;
	WORD		m_wMapInfo;
	WORD		m_wShopInfo;
	WORD		m_wSkillInfo;
	WORD		m_wRegenInfo;
	WORD		m_wGroupInfo;
	WORD		m_wStateInfo;
	WORD		m_wItemCompInfo;
	WORD		m_wItemCompResultInfo;
	WORD		m_wMissionRewardInfo;
	WORD		m_wFormulaRatioInfo;
	WORD		m_wStyleQuickInfo;
	WORD		m_wAITypeInfo;
};

struct AI_COMMON_INFO
{
	// Battle Record ����
	BYTE		m_byDAMAGE_POINT_REDUCE_RATIO;		// ������ ����Ʈ �谨��
	WORD		m_wMIN_DAMAGE_POINT;				// �ּҵ����� ����Ʈ(�ּҵ����� ����Ʈ �����̸� ����Ʈ�� �谨�� �� 0���� �ʱ�ȭ�ȴ�.)

	// ��ȯ ����
	float		m_fSUMMON_FOLLOW_DISTANCE;			// ��ȯü�� ��ȯ�ڸ� ������� �Ÿ�
	float		m_fSUMMON_MAX_FOLLOW_DISTANCE;		// ��ȯü�� �ڷ���Ʈ�ϴ� �Ÿ�

	// ���� �ֱ� �ð�
	WORD		m_wKNOCKDOWN_TIME;					// �ٿ� ���� �ð�
	WORD		m_wTHRUST_TIME;						// �и��� ���� �ð�
	// �Ÿ� ����
	float		m_fMIN_MOVE_DISTANCE;				// �ѹ��� �̵��ϴ� �ּҰŸ�(Wander, Return, RunAway ...)
	float		m_fMAX_MOVE_DISTANCE;
	float		m_fMIN_MOVABLE_DISTANCE;			// �̼�ġ �����̸� �̵��� ���Ѵ�.
	float		m_fGROUP_FOLLOWER_MIN_RADIUS;		// �׷���� ������� �����κ��� ������� �ּҹ���
	float		m_fGROUP_FOLLOWER_MAX_RADIUS;
#if !defined(_NA_003649_20111115_BATTLE_ZONE_HARDMODE)
	float		m_fMOVE_HEIGHT_LIMIT;				// ���� �̵� ���� ����(���� ���� �̻� �� �ö�´�)
#endif //_NA_003649_20111115_BATTLE_ZONE_HARDMODE
	float		m_fSIGHT_HEIGHT_LIMIT;				// �þ� ���� ����(���� ���� ���̰� ���� ���Ͱ� �˾������� ���Ѵ�)
	float		m_fATTACK_RANGE_HEIGHT_RATIO;		// ��Ÿ� �������� ����

	// ���� ����
	float		m_fJUMP_MIN_HEIGHT;					// ���������� �ּ� ����
	float		m_fJUMP_MAX_HEIGHT;
	float		m_fJUMP_MIN_DISTANCE;
	float		m_fJUMP_MAX_DISTANCE;
	float		m_fJUMP_ATTACK_MIN_DISTANCE;
	float		m_fJUMP_ATTACK_MAX_DISTANCE;
	float		m_fJUMP_ATTACK_RANGE_RATIO;			// ���������� ���� ���ݻ�Ÿ� ��ŭ �İ��� ����

	// FallApart ����
	float		m_fFALLAPART_MIN_ATTACK_RANGE_LIMIT;// �ּ� ���ݻ�Ÿ�, �̰Ÿ����� ������ ���ʹ� �ѹ� ��������.

	// Retreat ����
	float		m_fRETREAT_MIN_ATTACKRANGE_LIMIT;	// ���ݻ�Ÿ��� �̼�ġ �̻��̾�� ��������� �Ѵ�.
	float		m_fRETREAT_MIN_DISTANCE_LIMIT_RATIO;// ����Ÿ��� ��Ÿ�*Ratio ���ϰ� �Ǹ� ���� �Ѵ�.

	// Help ����
	float		m_fHELP_SIGHTRANGE_RATIO;			// ���������� ���� �þ߹��� ���

	// ��Ÿ
	float		m_fRANGE_TOLERANCE;					// ��Ÿ� ����
	WORD		m_wSEARCH_ROTATE_ANGLE;				// Ž�� ȸ����
	BYTE		m_byMAX_OBSERVERS_PER_PLAYER;		// �÷��̾�� ���� ��
	WORD		m_wGROUP_AIMESSAGE_MIN_DELAY;		// �׷��� �ּ� ������
	WORD		m_wGROUP_AIMESSAGE_MAX_DELAY;
	WORD		m_wTRACK_INNER_ANGLE;				// ���� ����

	// ���� ǥ���ϴ� �Ķ����
	WORD		m_wEND_ROW_KEY;						// ������ ��
};

#ifdef _NA_0_20100819_AGGRO_RENEWAL
struct AGGRO_DISTANCE_INFO
{
	WORD	change_point_;
	WORD	change_range_;
};
#endif
struct AI_TYPE_INFO
{
	WORD		m_wCode;							// AIType Code

	// Battle Record ����
	WORD		m_wSEARCH_TARGET_TIME;				// �ֺ� Ÿ�ٸ���Ʈ�� �����ϴ� �ֱ�
	WORD		m_wAGGRO_TIME;						// ��׷� �ֱ�
	WORD		m_wBATTLE_RECORD_UPDATE_TIME;		// BattleRecord ������Ʈ �ֱ�
	BYTE		m_byPOINT_INIT_RANDOM_RATIO;		// ����Ʈ�� �ʱ�ȭ��Ű�� ���� Ȯ��(�Ÿ�, ����������Ʈ �ʱ�ȭ)
	BYTE		m_byTARGET_CHANGE_RATIO;			// Ÿ���� �����ų���� �����ϴ� ���� Ȯ��

#ifdef _NA_0_20100819_AGGRO_RENEWAL
	AGGRO_DISTANCE_INFO		m_Distance_Info[AGGRO_CHANGE_RATIO_MAX];// Ŭ���� �⺻ ����Ʈ
#endif
	// Ŭ������ �⺻ ����Ʈ
	WORD		m_wCLASS_BASE_POINT[eCHAR_TYPE_MAX];// Ŭ���� �⺻ ����Ʈ

	// ��Ʋ ����Ʈ �Ķ����
	WORD		m_wFIRST_ATTACK_POINT;				// ���� ����Ʈ
	WORD		m_wNEAR_DISTANCE_POINT_INC;			// �ּ� �Ÿ� ����Ʈ ����
	WORD		m_wNEAR_DISTANCE_POINT_MAX;			// �ּ� �Ÿ� ����Ʈ �ִ�
	WORD		m_wLOW_LEVEL_POINT_INC;				// �ּ� ���� ����Ʈ ����
	WORD		m_wLOW_LEVEL_POINT_MAX;				// �ּ� ���� ����Ʈ �ִ�
	WORD		m_wLOW_HP_POINT_INC;				// �ּ� ü�º��� ����Ʈ ����
	WORD		m_wLOW_HP_POINT_MAX;				// �ּ� ü�º��� ����Ʈ �ִ�
	WORD		m_wDAMAGE_POINT_INC;				// ������ ����Ʈ ����
	WORD		m_wDAMAGE_POINT_MAX;				// ������ ����Ʈ �ִ�

	// ���� �ֱ� �ð�
	WORD		m_wSEARCH_PERIOD;					// Ž�� �ֱ�
	WORD		m_wRETREAT_PERIOD;					// ���� �ֱ�
	WORD		m_wTRACK_PERIOD;					// ���� �ֱ�
	WORD		m_wIDLE_MIN_TIME;					// IDLE �ּ� �ð�
	WORD		m_wIDLE_MAX_TIME;					// IDLE �ִ� �ð�
	WORD		m_wRUNAWAY_TIME;					// ���� ���� �ð�

	// �Ÿ� ����
	float		m_fREGEN_LOCATION_LIMIT;			// ���Ͱ� �ִ�� ��� �� �ִ� �ݰ�
	float		m_fWANDER_RADIUS_FROM_REGENAREA;	// ������ �������κ��� ��Ȳ�ϴ� �ִ����
#ifdef _NA_003649_20111115_BATTLE_ZONE_HARDMODE
    // AIType���� �����Ҽ� �ְ� AICommonInfo.txt���� AITypeInfo.txt�� �̵��� - arycoat.2012.01.27
    float		m_fMOVE_HEIGHT_LIMIT;				// ���� �̵� ���� ����(���� ���� �̻� �� �ö�´�)
#endif //_NA_003649_20111115_BATTLE_ZONE_HARDMODE

	// Help ����
	float		m_fHELP_REQUEST_HP_PERCENT;			// ü���� 50���� ������ �� ������ ��û�Ѵ�.

	// ��Ÿ
	WORD		m_wNPC_HPMP_REGEN_PERIOD;			// NPC HPMP ���� �ֱ�
    //__NA_001290_20090525_SHIELD_SYSTEM
    WORD		m_wNPC_SD_REGEN_PERIOD;			    // NPC SD ���� �ֱ�

	// ��Ű����
	DWORD		m_dwLUCKY_MONSTER_RUNAWAY_TIME_MIN;	// �ּ� �����ð�
	DWORD		m_dwLUCKY_MONSTER_RUNAWAY_TIME_MAX;	// �ִ� �����ð�
	DWORD		m_dwLUCKY_MONSTER_REGEN_TIME;		// ������ �����ð�
};

// RTT�� ������ ��Ʋ�� ���� ����
struct STRUCT_BATTLEZONE_INFO
{
	BYTE			byWorldID;
	BYTE			byChannelID;
	DWORD			dwZoneCount[eZONETYPE_MAX];				// �� �� ����
//	DWORD			dwZoneUserCount[eZONETYPE_MAX];			// �� �� �� ����(�Ϲ� ����)
//	DWORD			dwZonePCRoomUserCount[eZONETYPE_MAX];	// �� �� �� ����(PC��)
/*
	DWORD			dwVillageUserCount;			// ������ �� ����
	DWORD			dwLobbyZoneCount;			// �κ�� ����
	DWORD			dwLobbyUserCount;			// �κ�� �� ����
	DWORD			dwHuntingZoneCount;			// ���ù� ����
	DWORD			dwHuntingUserCount;			// ���ù� �� ����
	DWORD			dwMissionZoneCount;			// �̼ǹ� ����
	DWORD			dwMissionUserCount;			// �̼ǹ� �� ����
	DWORD			dwQuestZoneCount;			// ����Ʈ�� ����
	DWORD			dwQuestUserCount;			// ����Ʈ�� �� ����
	DWORD			dwPVPZoneCount;				// PVP�� ����
	DWORD			dwPVPUserCount;				// PVP�� �� ����
	DWORD			dwEventZoneCount;			// �̺�Ʈ�� ����
	DWORD			dwEventUserCount;			// �̺�Ʈ�� �� ����
	DWORD			dwGuildZoneCount;			// ���� ����
	DWORD			dwGuildUserCount;			// ���� �� ����
	DWORD			dwSiegeZoneCount;			// ������ ����
	DWORD			dwSiegeUserCount;			// ������ �� ����

	DWORD			dwVillagePCRoomUserCount;	// ������ �� ����(PC��)
	DWORD			dwLobbyPCRoomUserCount;		// �κ�� �� ����(PC��)
	DWORD			dwHuntingPCRoomUserCount;	// ���ù� �� ����(PC��)
	DWORD			dwMissionPCRoomUserCount;	// �̼ǹ� �� ����(PC��)
	DWORD			dwQuestPCRoomUserCount;		// ����Ʈ�� �� ����(PC��)
	DWORD			dwPVPPCRoomUserCount;		// PVP�� �� ����(PC��)
	DWORD			dwEventPCRoomUserCount;		// �̺�Ʈ�� �� ����(PC��)
	DWORD			dwGuildPCRoomUserCount;		// ���� �� ����(PC��)
	DWORD			dwSiegePCRoomUserCount;		// ������ �� ����(PC��)
*/
};

// RTT ���� ��ȭ�� ����
struct SERVER_MONEY_INFO
{
	BYTE	byWorldID;
	BYTE	byChannelID;
	BYTE	byServerType;
	BYTE	byServerID;

	ULONGLONG	ui64TotalGetMoney;		// ���� �ð� ���� ���ܳ� �� �Ѱ�
	ULONGLONG	ui64TotalLoseMoney;		// ���� �ð� ���� ������ �� �Ѱ�
	ULONGLONG	ui64TotalUserMoney;		// ���� ���� �������� ���� �ִ� ��� �ݾ�
};

// CHANGES: f110725.2L, moved to (World)ChannelManager in WorldServer
// struct SERVER_LINKEDZONE_INFO

//�Ƹ�Ÿ�Ժ� ������ ����
struct ARMOR_DAMAGE_INFO
{
	BYTE			byWeaponType;
	BYTE			byArmorType;
	DAMAGETYPE		Damage;						// ������ ����
};

// ���� ��� ���� ����
struct STRUCT_RESTRICT
{
	DWORD	dwFunc;
	BOOL	bAllow;
};

// ���� ��� ���� ���� - Ȯ����
struct STRUCT_RESTRICT_MASTER
{
	DWORD			dwFunc;						// ������ ��� �ε���(eFUNCTIONALRESTRICT_TYPE ����)
	BOOL			bAllow;						// ����̳�?(FALSE: ���� TRUE: ���)
	TCHAR			ptszDesc[64];				// � ��� ���������� ���� ����
};

// �޸� ���� ����
typedef struct
{
	MEMOGUID	m_MemoGuid;									// �޸� GUID(DB�� ����� ����ũ�� ��)
	BYTE		m_byRead;									// �޸� �о����� 1, �ƴϸ� 0
	TCHAR		m_ptszSenderName[MAX_CHARNAME_LENGTH+1];	// ���� ��� �̸�
	TCHAR		m_ptszSubject[MAX_SUBJECT_STRLENGTH];		// �޸� ���(25��). �޸� ��� �޴� ��� �����Ͽ� �־�д�
	TCHAR		m_ptszMemo[MAX_MEMO_LENGTH + 1];				// ���� �޸� ����
} STRUCT_MEMO_INFO, *LP_STRUCT_MEMO_INFO;

//==================================================================================================

//{__NA001254_20090109_RTTG_FUNC_ADD
// ��Ƽ �޸� ���� ����
typedef struct tag_MMEMO_INFOS
{
    enum{ MAX_MESSAGE_LENGTH = MAX_MEMO_LENGTH, MAX_USER_INFO = 10, };

    typedef struct
    {
        char m_szRecvCharName[MAX_CHARNAME_LENGTH + 1];    // ������ ���� ĳ���͸�
    } USER_INFO, *LPUSER_INFO;

    tag_MMEMO_INFOS& operator=(const tag_MMEMO_INFOS& rh);
    bool ChkRecvCount() const;
    const TCHAR* GetRecvName(int index) const;

    int       m_nCount;                               // ���� ���� ����
    USER_INFO m_arrUserInfoList[MAX_USER_INFO];           // ���� ������   (��Ŷ�뷮 ����..����... ���̺� ���߰� �������Ŷ� �׳� �Ӵϴ�.) 
    TCHAR     m_OperCharName[MAX_CHARNAME_LENGTH + 1];  // ������ ������ �ɸ��͸�
    TCHAR     m_szMessage[MAX_MESSAGE_LENGTH + 1];      // ���� ����    

} MMEMO_INFOS, *LPMMEMO_INFO;

inline bool tag_MMEMO_INFOS::ChkRecvCount() const {
    return (m_nCount <= MAX_USER_INFO) && (m_nCount > 0);
}

inline const TCHAR* tag_MMEMO_INFOS::GetRecvName(int index) const {
    return index < m_nCount ? m_arrUserInfoList[index].m_szRecvCharName : NULL;
}

//==================================================================================================

typedef struct tag_MMEMO_ERR_RESULT
{
    enum { MAX_RESULT_INFO = 10, MAX_RESULT_DESCRIPTION_LENGTH = 200, };

    tag_MMEMO_ERR_RESULT();
    tag_MMEMO_ERR_RESULT& operator=(const tag_MMEMO_ERR_RESULT& rh);
    void SetCharName(const TCHAR* receiver_charname);
    void SetResult(DWORD result);

    DWORD   m_dwResult;                                             // 0 : ���� ����, 1 : ���� ����
    TCHAR   m_szRecvCharName[MAX_CHARNAME_LENGTH + 1];                  // �޽����� ���� ĳ���͸�
    TCHAR   m_szResultDescription[MAX_RESULT_DESCRIPTION_LENGTH + 1];   // ���� ���� ��� Description

} MMEMO_ERR_RESULT, *LPMMEMO_ERR_RESULT;


inline tag_MMEMO_ERR_RESULT::tag_MMEMO_ERR_RESULT()
    : m_dwResult(0)
{
    ZeroMemory(this, sizeof(*this));
}

inline void tag_MMEMO_ERR_RESULT::SetCharName(const TCHAR* receiver_charname)
{
    _tcsncpy(m_szRecvCharName, receiver_charname, _countof(m_szRecvCharName));
    m_szRecvCharName[_countof(m_szRecvCharName) - 1] = '\0';
}

//==================================================================================================

typedef struct tag_MMEMO_RESULT
{
    tag_MMEMO_RESULT() {
        ZeroMemory(this, sizeof(*this));
    }

    bool Set(const TCHAR* receiver_charname, DWORD db_result_error, CHARGUID receiver_charguid,
             MEMOGUID memo_guid, const TCHAR* created_date, const TCHAR* subject);

    MEMOGUID m_MemoGuid;
    CHARGUID m_RecvGuid;
    TCHAR    m_ptszSubject[MAX_SUBJECT_STRLENGTH];
    TCHAR    m_tszCreateDate[21]; // ���� ��¥
    MMEMO_ERR_RESULT m_ErrResult; // ���� ���� ��� ������
} MMEMO_RESULT, *LPMMEMO_RESULT;

//}__NA001254_20090109_RTTG_FUNC_ADD

//==================================================================================================

// D->W ��Ŷ�� �� ����

typedef struct
{
	TCHAR		m_SendName[MAX_CHARNAME_LENGTH+1];
	BYTE		m_byRead;
	TCHAR		m_ptszMemo[MAX_MEMO_LENGTH + 1];
} STRUCT_MEMO_PACKETINFO, *LP_STRUCT_MEMO_PACKETINFO;


struct MAP_PARTIAL_LOADING
{
	MAPCODE		m_MapKind;
	TCHAR		m_ptszMapName[MAX_MAPNAME_LENGTH];
	TCHAR		m_ptszZoneType[MAX_MAPNAME_LENGTH];
	BOOL		m_bLoading;
};

struct eWARE_INVEN_FILTER_FLAG_INFO
{
	static const DWORD RESERVED1						=  0;
	static const DWORD WAREHOUSE_INITIALIZE				=  1;	// �ʱ�ȭ�� ��Ȳ����
	static const DWORD WAREHOUSE_QUERY_TRANSACTION		=  2;	// â��PWD DB Query��
	static const DWORD WAREHOUSE_SET_PWD				=  3;	// â�� ��� ���� ����
	static const DWORD WAREHOUSE_PWD_TRANSACTION		=  4;
	static const DWORD WAREHOUSE_USABLE					=  5;	// ��� ������ �� ���� ���� ����
	static const DWORD MASK_WAREHOUSE_FLAGS				= 0x000000FF;

	static const DWORD INVENTORY_QUERY_TRANSACTION		= 10;	// �κ�PWD DB Query��
	static const DWORD INVENTORY_SET_PWD				= 11;	// �κ� ��� ���� ����
	static const DWORD INVENTORY_PWD_TRANSACTION		= 12;
	static const DWORD INVENTORY_USABLE					= 13;	// ��� ������ �� ���� ���� ����
	static const DWORD MASK_INVENTORY_FLAGS				= 0x0000FF00;
	//
	static const DWORD MAX_INDEX						= 31;
};

typedef util::BitSetOperation<eWARE_INVEN_FILTER_FLAG_INFO::MAX_INDEX>	WAREINVEN_FILTER_BITFLAGS;

struct ToggleItemInfos
{
    nsSlot::TOGGLE_ITEMS toggle_item_info_[kToggleItemMax+1];

    nsSlot::TOGGLE_ITEMS* begin() { return &toggle_item_info_[0]; };
    nsSlot::TOGGLE_ITEMS* end() { return &toggle_item_info_[kToggleItemMax]; };

    void init() 
    {
        for (nsSlot::TOGGLE_ITEMS* it = begin(); it != end(); ++it) {
            it->clear();
        }
    }
};

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
struct BattleGroundRecordInfos
{
    enum { kRecordMaxCount = 5 };
    BattleGroundRecord battle_ground_record[kRecordMaxCount+1];
    BattleGroundRecord* begin() { return &battle_ground_record[0]; };
    BattleGroundRecord* end() { return &battle_ground_record[kRecordMaxCount]; };
};
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
struct GoldRushRecordInfos
{
    enum { kRecordMaxCount = 5 };
    GoldRushRecord goldrush_record[kRecordMaxCount+1];
    GoldRushRecord* begin() { return &goldrush_record[0]; };
    GoldRushRecord* end() { return &goldrush_record[kRecordMaxCount]; };
};
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH

#ifdef _NA_007086_20140318_MONSTERBOOK
struct MonsterBook
{
    enum { MonsterBookMaxCount = 1000 };
    
    DWORD monster_code_count;
    DWORD monster_code[MonsterBookMaxCount];

    void clear()
    {
        memset(&monster_code, 0, sizeof(monster_code));
        monster_code_count = 0;
    }

    void push_back(DWORD _monster_code) {
        monster_code[monster_code_count++] = _monster_code;
    }

    DWORD find(DWORD _monster_code) {
        for (int i = 0; i < monster_code_count; i++) {
            if (monster_code[i] == _monster_code)
                return monster_code[i]; //
        }

        return 0;
    }
};
#endif //_NA_007086_20140318_MONSTERBOOK

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
struct SunRankingPlayerInfo
{
    SunRankingPlayerInfo()
    {
        init();
    }
    void init()
    {
        for (eSUN_RANKING_TYPE type = eSUN_RANKING_TYPE_START; type < eSUN_RANKING_TYPE_END; ++type)
        {
            score[type] = 0;
        }

        memset(char_name, 0, MAX_CHARNAME_LENGTH+1);
        class_code = eCHAR_NONE;
        char_guid = 0;
        guild_guid = 0;
    }

    SCORE score[eSUN_RANKING_TYPE_END];

    char       char_name[MAX_CHARNAME_LENGTH+1];
    eCHAR_TYPE class_code;
    GUILDGUID  guild_guid;
    CHARGUID   char_guid;
};
struct SunRankingGradeInfo
{
    SunRankingGradeInfo()
    {
        init();
    }
    void init()
    {
        char_guid = 0;
        ranking_type = 0;
        grade = eSUN_RANKING_GRADE_NONE;
        score = 0;
    }
    CHARGUID  char_guid;
    int       ranking_type;
    SCORE     score;
    int       grade;
};

struct LoadControl
{
    LoadControl()
    {
        init(0);
    }

    void init(int total_load_count = 0)
    {
        m_total_load_count = total_load_count;
        m_loaded_count = 0;
    }

    void SetLoaded( int total_msg_count )
    {
        if (m_total_load_count == 0)
        {
            m_total_load_count = total_msg_count;
        }

        ++m_loaded_count;
    }

    bool IsLoaded()
    {
        if (m_total_load_count == 0)
        {
            return false;
        }

        if (m_total_load_count == m_loaded_count)
        {
            return true;
        }

        return false;
    }

    int m_total_load_count;
    int m_loaded_count;
};
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM

#endif // __SERVER_STRUCT_H__
















