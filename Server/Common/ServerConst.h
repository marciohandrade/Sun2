


/** ServerConst.h : 서버에서만 사용하는 DEFINE값 정의

*/

#ifndef __SERVER_CONST_H__
#define __SERVER_CONST_H__



// 서버 종류
// 이 정보는 DBTable의 정보와 연구개발쪽과도 동기화가 이루어 져야 한다.
// 수정에 주의!!!!!!!
// Related : Global.h의 GetServerType2String()
enum eSERVER_TYPE
{
	UNKNOWN_SERVER 			= 0,
	AGENT_SERVER			= 1,
	FIELD_SERVER			= 2,
	BATTLE_SERVER			= 3,
	GAME_DBPROXY			= 4,
	WORLD_SERVER			= 5,
	MASTER_SERVER			= 6,
	GUILD_SERVER            = 7,
	ACCOUNT_DBPROXY			= 8,
	TOURNAMENT_SERVER		= 9,
	AUTH_AGENT_SERVER		= 10,	// TODO: DB에 넣기?
	PORTAL_SERVER			= 11,	//14,
	SOLARAUTH_SERVER		= 12,	//15,
	SHOP_SERVER				= 13,	//16,	

	OP_SERVER				= 25,	//11,   // WOPS(Webzen Online Portal Solution) Agent Server
	TEMP_SERVER				= 26,	//12,
	GAME_SERVER				= 27,	//< FIELD_SERVER와 BATTLE_SERVER를 통칭

	AUTH_SERVER				= 28,
	LOGIN_FRONT_SERVER		= 29,
	BILLING_SERVER			= 30,
	CLIENT_SERVER			= 31,
#ifdef _NA002635_GP_DP_PROTOCOL_UINIFICATION
    RANKING_SERVER          = GAME_DBPROXY,
#else
    RANKING_SERVER          = PORTAL_SERVER,
#endif
	MAX_SERVER				= 32,	
};

enum eSERVER_STATE
{
	SERVER_ABNORMAL_DISCONNECTED,
	SERVER_BOOTING,
	SERVER_NORMAL,
};

enum eDEBUGINFO_TYPE
{
	eDEBUGINFO_TYPE_INVALID,		// 이 값이 날라오면 디버그정보를 보내지 않음
	eDEBUGINFO_TYPE_DAMAGE,			// 데미지 디버그정보
	eDEBUGINFO_TYPE_AGGRO,			// 어그로 디버그정보(배틀레코드)
	eDEBUGINFO_TYPE_MONSTERSKILL,	// 몬스터 스킬정보
	eDEBUGINFO_TYPE_MAX,
};

enum eAUTH_LOGOUT_REASON
{
	AUTH_LOGOUT_NORMAL				= 0,			// 정상 로그아웃
	AUTH_LOGOUT_AUTH_REQUEST		= 1,			// 인증서버의 요청으로 강제 로그아웃
	AUTH_LOGOUT_USER_NOT_FOUND      = 2,			// 인증에서 보낸 유저를 찾을 수 없는 경우 게임Agent서버에서 강제 로그아웃
	AUTH_LOGOUT_HACKING_USER        = 3,			// 스핵 등등의 해킹툴 사용 유저에 대한 게임Agent서버에서 강제 로그아웃
	AUTH_LOGOUT_WRONG_AUTHKEY       = 4,			// 인증키가 일치하지 않은 유저의 접속으로 게임Agent서버에서 강제 로그아웃
	AUTH_LOGOUT_WRONG_VERSION       = 5,			// 버전이 일치하지 않아 게임Agent서버에서 강제 로그아웃
	AUTH_LOGOUT_ETC                 = 6,
};


enum eABUSE_TYPE
{
	eABUSE_FORCED_DISCONNECT		= 1,		// 강제종료(강제종료 5회면 블락됨)
	eABUSE_TEMPORARY_BLOCK			= 2,		// 임시블락(Master가 떠 있는 동안 블락)
	eABUSE_WORKPLACE_IP_BLOCK		= 3,		// 작업장 IP BLOCK
	eABUSE_DUPLICATE_ITEM_SERIAL	= 4,		// 아이템 중복시리얼 발생
	eABUSE_CRASH_REPORT				= 5,		// 클라이언트 크래쉬 리포트
	eABUSE_LOGIC_VIOLATION			= 6,		// 로직위반(로직위반 점수가 일정수치를 넘어가면 강제종료됨)
};

enum eABUSE_SUB_TYPE
{
	eABUSE_SUB_FORCED_DISCONNECT									= 0,		// 어뷰징위반 누적으로인한 강제종료
	eABUSE_SUB_TEMPORARY_BLOCK										= 1,		// 강제종료 누적으로인한 임시블락(마스터서버가 떠 있는 동안)
	eABUSE_SUB_WORKPLACE_IP_BLOCK									= 2,		// 작업장 IP 블럭
	eABUSE_SUB_DUPLICATE_ITEM_SERIAL_EXSIT							= 3,		// 아이템 중복 시리얼 존재
	eABUSE_SUB_DUPLICATE_ITEM_SERIAL_GENERATE						= 4,		// 아이템 중복 시리얼 발급
	eABUSE_SUB_CRASH_REPORT											= 5,		// 크래쉬 리포트

	eABUSE_SUB_LOGIC_VIOLATION_ATTACK_SPEED							= 6,		// 공속 위반
	eABUSE_SUB_LOGIC_VIOLATION_ATTACK_SEQUENCE						= 7,		// 공격순서 위반
	eABUSE_SUB_LOGIC_VIOLATION_ATTACK_RANGE							= 8,		// 공격거리 위반
	eABUSE_SUB_LOGIC_VIOLATION_ATTACK_TYPE							= 9,		// 공격타입 위반

	eABUSE_SUB_LOGIC_VIOLATION_INVALID_POSITION						= 10,		// 좌표 이상
	eABUSE_SUB_LOGIC_VIOLATION_NOT_SYNC_POSITION					= 11,		// 서버클라 위치동기화 위반
	eABUSE_SUB_LOGIC_VIOLATION_MOVE_RANGE							= 12,		// 이동거리 위반
	eABUSE_SUB_LOGIC_VIOLATION_MOVE_CRACKED_PACKET					= 13,		// 이동관련 크랙된 패킷
	eABUSE_SUB_LOGIC_VIOLATION_MOVE_SPEED							= 14,		// 이속 위반

	eABUSE_SUB_LOGIC_VIOLATION_SECURITY_QUERY_DISCONNECT			= 15,		// 보안쿼리 위반
	eABUSE_SUB_LOGIC_VIOLATION_HEARTBEAT_DISCONNECT					= 16,		// 하트비트 위반
	eABUSE_SUB_LOGIC_VIOLATION_HACKSHIELD_DISCONNECT				= 17,		// 핵쉴드 위반
	eABUSE_SUB_LOGIC_VIOLATION_UNEXPECTED_PACKET					= 18,		// 예상하지 못한 패킷전송
	eABUSE_SUB_LOGIC_VIOLATION_TEMPUSER_AUTH_DISCONNECT				= 19,		// TempUser 인증
	eABUSE_SUB_LOGIC_VIOLATION_MISSION_REWARD						= 20,		// 미션보상 위반
	eABUSE_SUB_LOGIC_VIOLATION_LOADING_TIME_SHORT					= 21,		// 로딩시간 위반(로딩시간이 너무 짧은경우)

	eABUSE_SUB_LOGIC_VIOLATION_WRONG_SECTOR_WORLDSVR				= 22,		// 월드서버에서 잘못된 섹터 인덱스값에 대한 로그를 찍는경우
	eABUSE_SUB_LOGIC_VIOLATION_USER_NONE_WORLDSVR					= 23,		// 게임서버에는 접속해 있는데 월드서버에는 유저가 없는 경우
	eABUSE_SUB_LOGIC_VIOLATION_UNKNOWN_PROTOCOL						= 24,		// 전혀 엉뚱한 패킷 전송(툴이라면 오래된 버전의 패킷을 전송하는 경우를 들 수 있다.)
	eABUSE_SUB_LOGIC_VIOLATION_ASSERTION_FLOW						= 25,		// 확정 플로우 위반
	eABUSE_SUB_LOGIC_VIOLATION_HACKSHIELD_INVALID_STATUS_DISCONNECT	= 26,		// 핵쉴드 위반 V2 - (툴 가능성도 있지만 핵쉴드 패치 못받고 들어올 수 있는 경우)
	eABUSE_SUB_MAX,
};

//MSG_DG_OTHER_EXIST_CHARNAME_SYN 참조!
enum eCHARNAME_QUERY_ACTION_TYPE
{
	eCHARNAME_ACTION_SHOP_SEND_GIFT,	//< 샵에서 아이템을 선물할때, 캐릭터존재여부 체크
    eCHARNAME_ACTION_DEFAULT,           // Added by __NA000000_SHOP_USERID_50_PASSING_PROC__
};

//__NA_0_20090627_HEIM_ACQUISITION_S2S_NOTIFICATION
// 게임 내 하임 획득 유형
enum eHEIM_ACQUISITION_TYPE
{
    eHEIM_ACQUISITION_PICKUP,           // 줍기
    eHEIM_ACQUISITION_QUEST,           // 보상
    eHEIM_ACQUISITION_MISSION,           // 미션
};

// __KR_001355_20090608_EXP_ACQUISITION_NOTIFICATION
enum eEXP_ACQUISITION_TYPE
{
    eEXP_ACQUISITION_HUNTING,           //사냥
    eEXP_ACQUISITION_QUEST,            //보상
    eEXP_ACQUISITION_MISSION,           //미션
};

//__NA_0_20090627_HEIM_CONSUMPTION_S2S_NOTIFICATION
// 게임 내 하임 소비 유형
enum eHEIM_CONSUMPTION_TYPE
{
    eHEIM_CONSUMPTION_WAYPOINT,         // 웨이포인트 이용
    eHEIM_CONSUMPTION_RANKUP,           // 랭크업 비용
    eHEIM_CONSUMPTION_COMPOSITION,      // 조합 비용
    eHEIM_CONSUMPTION_REPAIR,           // 아이템 수리 비용
    eHEIM_CONSUMPTION_NPC_SHOP,         // NPC 상점에서 아이템 구입 (재구입 제외)
    eHEIM_CONSUMPTION_ENCHANT,          // 인챈팅 비용
};

// __KR_001355_20090805_CHECK_GROUPSYSTEM_MSG
//20090805 현재 그룹 시스템에서 클라이언트로 그룹 관련 정보임을 알리는것으로 쓰임
enum eGAME_CONTENT_TYPE
{
    GAME_CONTENT_NORMAL                   = 1,    //기본 시스템
    GAME_CONTENT_GROUPING                 = 2,    //그룹 시스템
};

// NOTE: f101210.3L, moved from filtered eGM_STRING enumeration
enum eGM_STRING
{
    GMSTR_NONE = 0,
    //----------------------------------------------------------------------------------------------
    GMSTR_TURNON,
    GMSTR_TURNOFF,
    //----------------------------------------------------------------------------------------------
    GMSTR_MAX
};

#endif // __SERVER_CONST_H__
