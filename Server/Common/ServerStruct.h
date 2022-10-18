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
//   MAX_WORLD_NAME_LEN = define.h에도 정의되어 있으며, 32로 되어 있음.
//   아래 전처리기는 사용하지 않으므로 제거
//#define MAX_WORLD_NAME_LEN		16

// (WAVERIX) (090728) (ADD-IN) 북미를 필두로 샵서버에 UserID(unified:50)를 보내는 로직이 추가된다.
// 각 버전에 맞춰 작업할 필요가 존재하게 되었다.
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
        // (NOTE) (WAVERIX) 잘못된 처리를 수행한다. 정말 반성해야 한다. 지금 와서 바꿀 수도 없다.
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
	TCHAR							m_tszCharName[MAX_CHARNAME_LENGTH+1];		//< Agent에서 바로 사용하기 위해
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

	// 길드 정보 : 2005/12/28(taiyo)
	GUILDGUID						m_GuildGuid;
	eGUILD_DUTY						m_eGuildDuty;
	TCHAR							m_tszGuildNicName[MAX_CHARNAME_LENGTH+1];

    //__NA001084_PET_IMAGE_DRAW_IN_CHARACTER_SELECT_SCREEN
	ITEMSTREAMEX					m_PetSlotStream;
	BOOL							m_bSummoned;
	// 가변 길이 장착창 정보
	_EQUIP_TOTAL_INFO				m_EquipItemInfo;

	BYTE							m_DelChk;
	TCHAR							m_tszDelDate[MAX_SMALLDATETIME_SIZE+1];
    // f100929.4L, removed an unused section
    //STATE_DETAIL_TOTAL_INFO			m_TotalStateInfo;
#ifdef _NA_002050_20110216_ADD_GENDER
    BYTE gender; // 성별
#endif
#ifdef _NA_003027_20111013_HONOR_SYSTEM
	WORD							honor_title;
#endif //_NA_003027_20111013_HONOR_SYSTEM
#ifdef _NA_004035_20120227_COSTUME_ITEM
    bool is_equipped_costume; // 코스튬 활성화 여부
    POSTYPE costume_item_pos; // 코스튬 인벤토리 위치
    SLOTCODE costume_item_code; // 코스튬 아이템 번호

#ifdef _NA_008610_20160104_ADD_DECORATECOSTUME
    bool is_equipped_deco_costume; // 데코 코스튬 활성화 여부
    POSTYPE deco_costume_item_pos; // 데코 코스튬 인벤토리 위치
    SLOTCODE deco_costume_item_code; // 데코 코스튬 아이템 번호
#endif //_NA_008610_20160104_ADD_DECORATECOSTUME
#endif  // _NA_004035_20120227_COSTUME_ITEM
    //----------------------------------------------------------------------------------------------
    void Output(ArchiveOfStream& archive);
    void Input(ArchiveOfStream& archive);
};

typedef struct tagLOGOUT_AUTH_DATA
{
	DWORD			dwAuthUserID;					// AuthServer에서 발급한 식별자
#ifdef __CH1305_090303_ACCOUNT_ID_LENGTH_EXTENTION__
    char			szAccountID[ID_MAX_LEN + 1];	// 계정 아이디
#else
	char			szAccountID[ID_MAX_LEN];		// 계정 아이디
#endif
	char			szPCName[MAX_CHARNAME_LENGTH];	// 캐릭터 이름 =>클베이후 삭제 예정->DBA요청에 의한 임시!!
	WORD			wSvrCode;						// 서버 코드
	WORD			wPCLevel;						// 캐릭터 레벨 =>클베이후 삭제 예정->DBA요청에 의한 임시!!
	BYTE			byPCClass;						// 캐릭터 클래스 =>클베이후 삭제 예정->DBA요청에 의한 임시!!
	BYTE			byLogoutType;					// 로그아웃 타입
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
	BYTE	byServiceMode;						//서비스 모드 관련 0:콘솔모드, 1:서비스 모드.......
	BYTE	byAgeGrade;							//0 : 18세미만,, 1 : 18세이상 ...
	BYTE	byServerStatus;						//0 : 개발용 서버, 1: 서비스(라이브서버) 서버

	// IOCP 설정
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
	char	pszLogPath[MAX_LOGPATH_LEN];		//로그파일 경로 저장 변수
	BYTE	byLogOption;

	//썬로그관련 정보
	BYTE	bySunLogOption;
	BYTE	bySunLogfileLevel;
	char	pszSunLogfilePath[MAX_LOGPATH_LEN];
	DWORD	dwServerInfoCycle;
}; // 서버 환경 변수


struct SERVER_COMMON_OPTION
{
	// HP, MP 리젠 주기 관련
	WORD		m_wPLAYER_HP_REGEN_PERIOD;			// 플레이어 HP 리젠 주기
    WORD		m_wPLAYER_MP_REGEN_PERIOD;			// 플레이어 MP 리젠 주기
    //{__NA_001290_20090525_SHIELD_SYSTEM
    WORD		m_wPLAYER_SD_REGEN_PERIOD;			// 플레이어 SD 리젠 주기
    WORD        player_fp_regen_period;             // 플레이어 FP 리젠 주기
    WORD        player_fp_maintain_time;            // 플레이어 FP 유지시간
    WORD		m_wPLAYER_FIGHTING_TIME;			// 플레이어 전투 상태 시간
    WORD		m_wNPC_FIGHTING_TIME;			    // NPC 전투 상태 시간
    //}
	// 공격 관련
	float		m_fETHER_SPLASH_DAMAGE_RATIO;		// 발키리 에테르 웨폰 스플래쉬 데미지 비율
	float		m_fSTYLE_SPLASH_DAMAGE_RATIO;		// 일반 스타일 스플래쉬 데미지 비율

	// 기타
	WORD		m_wMAX_ROOM_COUNT;					// 배틀서버 하나당 최대 방개수 제한
	float		m_fFINDPATH_JUMPLIMIT;				// 길찾기 JumpLimit
	WORD		m_wFINDPATH_MAX_DISTANCE;			// 길찾기 최대 거리
	WORD		m_wFINDPATH_MAX_NODE_COUNT;			// 길찾기 최대 노드개수
	BYTE		m_byGRID_MOVEINFO_DISTANCE;			// 주변 캐릭터 정보 전송시 이동정보를 주는 최소거리

	// 경쟁헌팅 자동매칭 제어
	BYTE		m_byMATCHING_LEVEL_LIMIT;			// 레벨 제어
	BYTE		m_byMATCHING_WIN_RATIO;				// 승률 제어
	BYTE		m_byMATCHING_MIN_USER_NUM;			// 매칭에 필요한 최소 유저수

	// 경쟁헌팅 플레이 시간
	DWORD		m_dwTIMEATTACK_PLAY_TIME;			// 타임어택 플레이 타임
	DWORD		m_dwSURVIVAL_PLAY_TIME;				// 서바이벌 플레이 타임

	// 이동동기 관련 거리차
	BYTE		m_byMOVE_SYNC_WARNING_DISTANCE;		// 서버와 클라간의 플레이어 위치 경고 거리(이동 허용)
	BYTE		m_byMOVE_SYNC_FAIL_DISTANCE;		// 서버와 클라간의 플레이어 위치 실패 거리(이동 실패)
	BYTE		m_byMOVE_SYNC_THRUST_DISTANCE;		// 서버와 클라간의 트러스트(밀리기) 최대 거리
	BYTE		m_byMOVE_SYNC_HACKING_DISTANCE;		// 서버와 클라간의 플레이어 위치 해킹 거리(이동 실패 및 해킹으로 간주)
	WORD		m_wFORCED_WARP_MOVE_DELAY;			// 서버에서 플레이어 강제 워프시 이동 딜레이

	// PC방 경험치 추가
	float		m_fPCBANG_BONUS_EXP_RATIO;			// PC방 경험치 추가 비율

	BYTE		m_byUseItemShopBattle;				// 배틀서버에서 아이템 샵을 쓸수 있게 할 것인가?
	BYTE		m_byPCBangSuperBangMasterAddExp;	// PC방 슈퍼방 방장 추가 경험치
	BYTE		m_byPCBangSuperBangNormalAddExp;	// PC방 슈퍼방 일반인 추가 경험치
	BYTE		m_byItemSuperBangMasterAddExp;		// 아이템 슈퍼방 방장 추가 경험치
	BYTE		m_byItemSuperBangNormalAddExp;		// 아이템 슈퍼방 일반인 추가 경험치

	BYTE		m_byPCBangApplyBenefit;				// PC방 혜택 적용 유무
	BYTE		m_byApplyItemOwnerShipByLevel;		// 몬스터와의 레벨차에 의한 아이템소유권을 적용할 것인가?
	BYTE		m_byGuildPVPUserCount;				// 길드 PVP시 인원수 

	WORD		m_wVIEWPORT_MAX_USER_COUNT;			// 주변에 보이는 최대 유저수
	WORD		m_WVIEWPORT_UPDATE_TIME;			// 주변 유저정보를 갱신하는 주기

	BOOL		m_byInsDungeonUseConOpt;			// 카오케릭의 인스턴스던전 이용 가능 설정 옵션
	BOOL		m_byLoobyUseConOpt;					// 카오케릭의 로비 이용 가능 설정 옵션

	BOOL		m_bItemPickUpMsgBrd;				// 아이템습득 메세지 주위에게 브로드 여부.
	BOOL		m_bItemPickUpPartyMsgBrd;			// 아이템습득 메세지 파티원들에게 브로드 여부.
	WORD		m_wABUSE_MIN_CLIENT_LOADING_TIME;	// 정상 클라라면 맵로딩에 이정도의 시간은 걸린다
    //__NA000000_081110_LIVE_SERVICE_TEST_CLINET_SUPPORT__ - unused section
	BOOLEAN		m_bApplyTestClientAccept_UserID;
	CHAR		m_pszTestClientAccept_UserID[ID_MAX_LEN+1];	// 라이브에서 특정 유저에 대한 C/S보안 체계 완화 처리 목적
    //__NA001283_20090225_COLLECTION_USE
	DWORD		m_DelayCollectionRespawnTime;
    DWORD		m_DelayDropTime;
#if !defined(_NA_20110610_MOVE_DOMINATION_CHANNEL_INFO)
    BYTE		m_byDominationChannel; //_NA001385_20090924_DOMINATION_ETC
#endif
#ifdef _NA_007135_20140402_MODIFY_BATTLE_GROUND_RESERVATION_AVAILABLE_IN_BATTLE_GROUND_CHANNEL
    int         m_byBattleGroundChannel;  //전장 예약 가능 채널 설정
#endif
#ifdef _NA_0_20100929_LANGUAGE_OF_FILTER
    BYTE        m_byLanguageOfFilter;
#endif
#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
    BYTE        free_pvp_max_room_count; // 자유전투실 최대 방 갯수 제한.
    BYTE        free_pvp_max_user_count; // 자유전투실 최대 유저 제한.
#endif // _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
#ifdef _NA_006538_BATTLE_GROUND_TIME_CONTROL_SYSTEM
    BYTE        battle_ground_time_control_system; // 전장 시간제어 시스템 활성화 여부
#endif // _NA_006538_BATTLE_GROUND_TIME_CONTROL_SYSTEM
#ifdef _NA_007992_20150127_EVENT_MONSTER_INVASION
    BYTE        monster_invasion_channel;   //몬스터침공 채널 설정
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
        WORD minimum_participation_members; // 점령전 패널티 참여 최소 인원수 
        WORD penalty_days_of_offence_fail; //공성 실패시 입찰 불가 패널티 기간(일)
        WORD penalty_days_of_defense_fail; // 수성 실패시 입찰 불가 패널티 기간(일)
    } domination_penalty;
};

struct SERVER_POOL_OPTION
{
	BYTE		m_byIndex;							// 인덱스
	char		m_szServerName[16];					// 서버구분
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

	// 슬롯 풀
	WORD		m_wItemSlotPool;
	WORD		m_wSkillSlotPool;
	WORD		m_wQuickSlotPool;
	WORD		m_wQuickStyleSlotPool;
	WORD		m_wTradeSlotPool;
	WORD		m_wVendorSlotPool;
	WORD		m_wEventSlotPool;

	// 파서 풀
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
	// Battle Record 관련
	BYTE		m_byDAMAGE_POINT_REDUCE_RATIO;		// 데미지 포인트 삭감률
	WORD		m_wMIN_DAMAGE_POINT;				// 최소데미지 포인트(최소데미지 포인트 이하이면 포인트가 삭감될 때 0으로 초기화된다.)

	// 소환 관련
	float		m_fSUMMON_FOLLOW_DISTANCE;			// 소환체가 소환자를 따라댕기는 거리
	float		m_fSUMMON_MAX_FOLLOW_DISTANCE;		// 소환체가 텔레포트하는 거리

	// 상태 주기 시간
	WORD		m_wKNOCKDOWN_TIME;					// 다운 유지 시간
	WORD		m_wTHRUST_TIME;						// 밀리기 유지 시간
	// 거리 관련
	float		m_fMIN_MOVE_DISTANCE;				// 한번에 이동하는 최소거리(Wander, Return, RunAway ...)
	float		m_fMAX_MOVE_DISTANCE;
	float		m_fMIN_MOVABLE_DISTANCE;			// 이수치 이하이면 이동을 안한다.
	float		m_fGROUP_FOLLOWER_MIN_RADIUS;		// 그룹몬스터 멤버들이 리더로부터 따라댕기는 최소범위
	float		m_fGROUP_FOLLOWER_MAX_RADIUS;
#if !defined(_NA_003649_20111115_BATTLE_ZONE_HARDMODE)
	float		m_fMOVE_HEIGHT_LIMIT;				// 몬스터 이동 높이 제한(일정 높이 이상 못 올라온다)
#endif //_NA_003649_20111115_BATTLE_ZONE_HARDMODE
	float		m_fSIGHT_HEIGHT_LIMIT;				// 시야 높이 제한(일정 높이 차이가 나면 몬스터가 알아차리지 못한다)
	float		m_fATTACK_RANGE_HEIGHT_RATIO;		// 사거리 높이적용 비율

	// 점프 상태
	float		m_fJUMP_MIN_HEIGHT;					// 점프가능한 최소 높이
	float		m_fJUMP_MAX_HEIGHT;
	float		m_fJUMP_MIN_DISTANCE;
	float		m_fJUMP_MAX_DISTANCE;
	float		m_fJUMP_ATTACK_MIN_DISTANCE;
	float		m_fJUMP_ATTACK_MAX_DISTANCE;
	float		m_fJUMP_ATTACK_RANGE_RATIO;			// 점프목적지 계산시 공격사거리 만큼 파고드는 비율

	// FallApart 상태
	float		m_fFALLAPART_MIN_ATTACK_RANGE_LIMIT;// 최소 공격사거리, 이거리보다 가까우면 몬스터는 한발 물러난다.

	// Retreat 상태
	float		m_fRETREAT_MIN_ATTACKRANGE_LIMIT;	// 공격사거리가 이수치 이상이어야 후퇴공격을 한다.
	float		m_fRETREAT_MIN_DISTANCE_LIMIT_RATIO;// 현재거리가 사거리*Ratio 이하가 되면 후퇴를 한다.

	// Help 상태
	float		m_fHELP_SIGHTRANGE_RATIO;			// 헬프상태일 때의 시야범위 배수

	// 기타
	float		m_fRANGE_TOLERANCE;					// 사거리 공차
	WORD		m_wSEARCH_ROTATE_ANGLE;				// 탐색 회전각
	BYTE		m_byMAX_OBSERVERS_PER_PLAYER;		// 플레이어당 몬스터 수
	WORD		m_wGROUP_AIMESSAGE_MIN_DELAY;		// 그룹명령 최소 딜레이
	WORD		m_wGROUP_AIMESSAGE_MAX_DELAY;
	WORD		m_wTRACK_INNER_ANGLE;				// 추적 내각

	// 끝을 표시하는 파라미터
	WORD		m_wEND_ROW_KEY;						// 마지막 열
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

	// Battle Record 관련
	WORD		m_wSEARCH_TARGET_TIME;				// 주변 타겟리스트를 갱신하는 주기
	WORD		m_wAGGRO_TIME;						// 어그로 주기
	WORD		m_wBATTLE_RECORD_UPDATE_TIME;		// BattleRecord 업데이트 주기
	BYTE		m_byPOINT_INIT_RANDOM_RATIO;		// 포인트를 초기화시키는 랜덤 확률(거리, 데미지포인트 초기화)
	BYTE		m_byTARGET_CHANGE_RATIO;			// 타겟을 변경시킬지를 결정하는 최종 확률

#ifdef _NA_0_20100819_AGGRO_RENEWAL
	AGGRO_DISTANCE_INFO		m_Distance_Info[AGGRO_CHANGE_RATIO_MAX];// 클래스 기본 포인트
#endif
	// 클래스별 기본 포인트
	WORD		m_wCLASS_BASE_POINT[eCHAR_TYPE_MAX];// 클래스 기본 포인트

	// 배틀 포인트 파라미터
	WORD		m_wFIRST_ATTACK_POINT;				// 선공 포인트
	WORD		m_wNEAR_DISTANCE_POINT_INC;			// 최소 거리 포인트 증감
	WORD		m_wNEAR_DISTANCE_POINT_MAX;			// 최소 거리 포인트 최대
	WORD		m_wLOW_LEVEL_POINT_INC;				// 최소 레벨 포인트 증감
	WORD		m_wLOW_LEVEL_POINT_MAX;				// 최소 레벨 포인트 최대
	WORD		m_wLOW_HP_POINT_INC;				// 최소 체력비율 포인트 증감
	WORD		m_wLOW_HP_POINT_MAX;				// 최소 체력비율 포인트 최대
	WORD		m_wDAMAGE_POINT_INC;				// 데미지 포인트 증감
	WORD		m_wDAMAGE_POINT_MAX;				// 데미지 포인트 최대

	// 상태 주기 시간
	WORD		m_wSEARCH_PERIOD;					// 탐색 주기
	WORD		m_wRETREAT_PERIOD;					// 후퇴 주기
	WORD		m_wTRACK_PERIOD;					// 추적 주기
	WORD		m_wIDLE_MIN_TIME;					// IDLE 최소 시간
	WORD		m_wIDLE_MAX_TIME;					// IDLE 최대 시간
	WORD		m_wRUNAWAY_TIME;					// 도망 유지 시간

	// 거리 관련
	float		m_fREGEN_LOCATION_LIMIT;			// 몬스터가 최대로 벗어날 수 있는 반경
	float		m_fWANDER_RADIUS_FROM_REGENAREA;	// 리젠된 영역으로부터 방황하는 최대범위
#ifdef _NA_003649_20111115_BATTLE_ZONE_HARDMODE
    // AIType별로 설정할수 있게 AICommonInfo.txt에서 AITypeInfo.txt로 이동함 - arycoat.2012.01.27
    float		m_fMOVE_HEIGHT_LIMIT;				// 몬스터 이동 높이 제한(일정 높이 이상 못 올라온다)
#endif //_NA_003649_20111115_BATTLE_ZONE_HARDMODE

	// Help 상태
	float		m_fHELP_REQUEST_HP_PERCENT;			// 체력이 50프로 이하일 때 헬프를 요청한다.

	// 기타
	WORD		m_wNPC_HPMP_REGEN_PERIOD;			// NPC HPMP 리젠 주기
    //__NA_001290_20090525_SHIELD_SYSTEM
    WORD		m_wNPC_SD_REGEN_PERIOD;			    // NPC SD 리젠 주기

	// 럭키몬스터
	DWORD		m_dwLUCKY_MONSTER_RUNAWAY_TIME_MIN;	// 최소 도망시간
	DWORD		m_dwLUCKY_MONSTER_RUNAWAY_TIME_MAX;	// 최대 도망시간
	DWORD		m_dwLUCKY_MONSTER_REGEN_TIME;		// 도망후 리젠시간
};

// RTT에 보내는 배틀존 관련 정보
struct STRUCT_BATTLEZONE_INFO
{
	BYTE			byWorldID;
	BYTE			byChannelID;
	DWORD			dwZoneCount[eZONETYPE_MAX];				// 각 존 개수
//	DWORD			dwZoneUserCount[eZONETYPE_MAX];			// 각 존 총 동접(일반 유저)
//	DWORD			dwZonePCRoomUserCount[eZONETYPE_MAX];	// 각 존 총 동접(PC방)
/*
	DWORD			dwVillageUserCount;			// 마을존 총 동접
	DWORD			dwLobbyZoneCount;			// 로비방 개수
	DWORD			dwLobbyUserCount;			// 로비방 총 동접
	DWORD			dwHuntingZoneCount;			// 헌팅방 개수
	DWORD			dwHuntingUserCount;			// 헌팅방 총 동접
	DWORD			dwMissionZoneCount;			// 미션방 개수
	DWORD			dwMissionUserCount;			// 미션방 총 동접
	DWORD			dwQuestZoneCount;			// 퀘스트방 개수
	DWORD			dwQuestUserCount;			// 퀘스트방 총 동접
	DWORD			dwPVPZoneCount;				// PVP방 개수
	DWORD			dwPVPUserCount;				// PVP방 총 동접
	DWORD			dwEventZoneCount;			// 이벤트방 개수
	DWORD			dwEventUserCount;			// 이벤트방 총 동접
	DWORD			dwGuildZoneCount;			// 길드방 개수
	DWORD			dwGuildUserCount;			// 길드방 총 동접
	DWORD			dwSiegeZoneCount;			// 공성방 개수
	DWORD			dwSiegeUserCount;			// 공성방 총 동접

	DWORD			dwVillagePCRoomUserCount;	// 마을존 총 동접(PC방)
	DWORD			dwLobbyPCRoomUserCount;		// 로비방 총 동접(PC방)
	DWORD			dwHuntingPCRoomUserCount;	// 헌팅방 총 동접(PC방)
	DWORD			dwMissionPCRoomUserCount;	// 미션방 총 동접(PC방)
	DWORD			dwQuestPCRoomUserCount;		// 퀘스트방 총 동접(PC방)
	DWORD			dwPVPPCRoomUserCount;		// PVP방 총 동접(PC방)
	DWORD			dwEventPCRoomUserCount;		// 이벤트방 총 동접(PC방)
	DWORD			dwGuildPCRoomUserCount;		// 길드방 총 동접(PC방)
	DWORD			dwSiegePCRoomUserCount;		// 공성방 총 동접(PC방)
*/
};

// RTT 서버 통화량 정보
struct SERVER_MONEY_INFO
{
	BYTE	byWorldID;
	BYTE	byChannelID;
	BYTE	byServerType;
	BYTE	byServerID;

	ULONGLONG	ui64TotalGetMoney;		// 일정 시간 동안 생겨난 돈 총계
	ULONGLONG	ui64TotalLoseMoney;		// 일정 시간 동안 없어진 돈 총계
	ULONGLONG	ui64TotalUserMoney;		// 현재 동접 유저들이 갖고 있는 평균 금액
};

// CHANGES: f110725.2L, moved to (World)ChannelManager in WorldServer
// struct SERVER_LINKEDZONE_INFO

//아머타입별 데이지 정보
struct ARMOR_DAMAGE_INFO
{
	BYTE			byWeaponType;
	BYTE			byArmorType;
	DAMAGETYPE		Damage;						// 데미지 비율
};

// 금지 기능 관련 정보
struct STRUCT_RESTRICT
{
	DWORD	dwFunc;
	BOOL	bAllow;
};

// 금지 기능 관련 정보 - 확장판
struct STRUCT_RESTRICT_MASTER
{
	DWORD			dwFunc;						// 금지할 기능 인덱스(eFUNCTIONALRESTRICT_TYPE 참조)
	BOOL			bAllow;						// 허용이냐?(FALSE: 금지 TRUE: 허용)
	TCHAR			ptszDesc[64];				// 어떤 기능 제한인지에 대한 설명
};

// 메모 관련 정보
typedef struct
{
	MEMOGUID	m_MemoGuid;									// 메모 GUID(DB에 저장된 유니크한 값)
	BYTE		m_byRead;									// 메모 읽었으면 1, 아니면 0
	TCHAR		m_ptszSenderName[MAX_CHARNAME_LENGTH+1];	// 보낸 사람 이름
	TCHAR		m_ptszSubject[MAX_SUBJECT_STRLENGTH];		// 메모 요약(25자). 메모 목록 받는 즉시 조립하여 넣어둔다
	TCHAR		m_ptszMemo[MAX_MEMO_LENGTH + 1];				// 실제 메모 내용
} STRUCT_MEMO_INFO, *LP_STRUCT_MEMO_INFO;

//==================================================================================================

//{__NA001254_20090109_RTTG_FUNC_ADD
// 멀티 메모 관련 정보
typedef struct tag_MMEMO_INFOS
{
    enum{ MAX_MESSAGE_LENGTH = MAX_MEMO_LENGTH, MAX_USER_INFO = 10, };

    typedef struct
    {
        char m_szRecvCharName[MAX_CHARNAME_LENGTH + 1];    // 쪽지를 받을 캐릭터명
    } USER_INFO, *LPUSER_INFO;

    tag_MMEMO_INFOS& operator=(const tag_MMEMO_INFOS& rh);
    bool ChkRecvCount() const;
    const TCHAR* GetRecvName(int index) const;

    int       m_nCount;                               // 쪽지 정보 갯수
    USER_INFO m_arrUserInfoList[MAX_USER_INFO];           // 쪽지 정보들   (패킷용량 손해..지만... 라이브 개발과 맞춰진거라 그냥 둡니다.) 
    TCHAR     m_OperCharName[MAX_CHARNAME_LENGTH + 1];  // 쪽지를 보내는 케릭터명
    TCHAR     m_szMessage[MAX_MESSAGE_LENGTH + 1];      // 쪽지 내용    

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

    DWORD   m_dwResult;                                             // 0 : 전송 성공, 1 : 전송 실패
    TCHAR   m_szRecvCharName[MAX_CHARNAME_LENGTH + 1];                  // 메시지를 받은 캐릭터명
    TCHAR   m_szResultDescription[MAX_RESULT_DESCRIPTION_LENGTH + 1];   // 쪽지 전송 결과 Description

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
    TCHAR    m_tszCreateDate[21]; // 보낸 날짜
    MMEMO_ERR_RESULT m_ErrResult; // 쪽지 전송 결과 정보들
} MMEMO_RESULT, *LPMMEMO_RESULT;

//}__NA001254_20090109_RTTG_FUNC_ADD

//==================================================================================================

// D->W 패킷에 들어갈 정보

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
	static const DWORD WAREHOUSE_INITIALIZE				=  1;	// 초기화된 상황여부
	static const DWORD WAREHOUSE_QUERY_TRANSACTION		=  2;	// 창고PWD DB Query중
	static const DWORD WAREHOUSE_SET_PWD				=  3;	// 창고 비번 설정 여부
	static const DWORD WAREHOUSE_PWD_TRANSACTION		=  4;
	static const DWORD WAREHOUSE_USABLE					=  5;	// 비번 설정시 락 해제 상태 여부
	static const DWORD MASK_WAREHOUSE_FLAGS				= 0x000000FF;

	static const DWORD INVENTORY_QUERY_TRANSACTION		= 10;	// 인벤PWD DB Query중
	static const DWORD INVENTORY_SET_PWD				= 11;	// 인벤 비번 설정 여부
	static const DWORD INVENTORY_PWD_TRANSACTION		= 12;
	static const DWORD INVENTORY_USABLE					= 13;	// 비번 설정시 락 해제 상태 여부
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
















