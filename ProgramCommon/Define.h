#ifndef __DEFINE_H__
#define __DEFINE_H__

#include "Version.h"
#include "Define.Build.h"
#include "DefineForLog.h"

#if defined(__KR) || defined(_KOREA)
	#define GENERAL_SHOUT_LIMIT_LEVEL	1
#elif defined(__TW) || defined(_TAIWAN)
	#define GENERAL_SHOUT_LIMIT_LEVEL	20
#elif defined(__CN) || defined(_CHINA)
	#define GENERAL_SHOUT_LIMIT_LEVEL	15
#elif defined(__JP) || defined(_JAPAN)
	#define GENERAL_SHOUT_LIMIT_LEVEL	1
#endif


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 바이너리 모듈 배포 시에 필요한 부분
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

typedef BYTE				NUMTYPE;			///< 개수
typedef BYTE				DURATYPE;			///< 내구력
typedef BYTE				POSTYPE;			///< 몰라
const POSTYPE				INVALID_POSTYPE_VALUE = 255;

typedef INT64				DBSERIAL;			///< DB 시리얼

typedef DWORD64				DBWLDSERIAL;

typedef WORD				SLOTCODE;			///< 슬롯 코드
typedef DWORD				CODETYPE;			///< 몰라
typedef WORD				KEYTYPE;			///< 각종 키들 타입	<= ZONEKEY랑 동일해야 한다.
typedef WORD				LEVELTYPE;			///< item, character level type
typedef BYTE				SLOTIDX;			///< slot index
typedef int					FIELDID;
typedef ULONGLONG			MONEY;				///< 돈
typedef DWORD				CASH;				///< 캐쉬
typedef WORD				MAPCODE;			///< 맵코드
typedef WORD				FIELDCODE;			///< 필드코드
typedef WORD				ZONEKEY;			///< 존키

typedef INT64				EXPTYPE;			///< 경험치 타입
typedef WORD                PORTALKEY;
const PORTALKEY             INVALID_PORTAL_KEY = 0;

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 패킷용!!! 최적화 용 typedef임 : client로 보내는 패킷에서만 사용바람
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
typedef int DAMAGETYPE; // extended by _NA001605_EP2_DAMAGETYPE_EXTENTION_

typedef short               TILEINDEX;          ///< 패킷용!!! 타일인덱스
typedef WORD				MONSTERCODE;		///< 몬스터코드
typedef	UINT				PLAYERHP;			///< 플레이어HP
typedef WORD				MPTYPE;				///< MP 타입
//__NA_001290_20090525_SHIELD_SYSTEM
typedef WORD				PLAYERSD;			///< 플레이어SD
//_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
typedef FLOAT               PLAYERFP;           ///< 위치블레이드 FP
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#define DAMAGETYPE_MAX  (1000000) // expanded by _NA001605_EP2_DAMAGETYPE_EXTENTION_

//점프 관련 
#define MAX_JUMP_NODE 30000
#define MIN_JUMP_NODE 200
/// [2012-5-31 by i4u4me] 클라 점프 알고리즘 변경으로 최대 첨프거리 변경
#define MAX_JUMP_DISTANCE			(12)			// 최대 점프로 이동할 수 있는 거리
//#define MAX_JUMP_DISTANCE			(15)			// 최대 점프로 이동할 수 있는 거리

#define MAX_ALLPLAYERS_PACKET_SIZE	( 1500 )	// 주변 플레이어 정보 전송시 최대 패킷사이즈

#define MAX_ITEMNAME_LENGTH			( 64 )		// 아이템 이름 길이
#define MAX_SKILLNAME_LENGTH		( 64 )		// 스킬 이름 길이
#define	MAX_MAPNAME_LENGTH			( 64 )		// 맵이름 길이
enum { MAX_ABILITY_NUM = 5 }; // 스킬하나당 최대 어빌러티(스킬효과) 개수
#define MAX_SUMMONED_NUM			( 10 )		// 한번에 소환되는 최대마리수
#define MAX_AREA_ID_LENGTH			( 4 )		// Area 영역 ID 길이

// (NOTE) an unused section since the 0816 version
// removed by __NA_963_LOG_LIB_VERSION_REFACTORING

#define	MAX_STATEINFOSTREAM_SIZE			( 1024 )	// 캐릭터 상태정보
#define MAX_DBSAVE_STATEINFO_NUM			( 40 )		// DB에 저장되는 최대 상태개수

#ifdef _NA_001990_ACSYSTEM_ADD
	//_NA_007422_20140716_MODIFY_MAX_ACHIEVEMENT_COUNT
    #define MAX_ACHIEVEMENT_COUNT				(1000)   //최대 업적 
    #define MAX_ACHIEVEMENT_REWARD				(2)     //업적 보상 개수
    #define MAX_ACHIEVEMENT_OBJECT				(10)    //업적 과재 개수
    #define ACHIEVEMENT_UINT_SIZE				(23)    //업적 하나의 길이
    #define MAX_ACHIEVEMENT_PARENT_CODE         (4)     //선행AC코드 최대갯수
#else
    #define MAX_ACHIEVEMENT_COUNT				(300)   //최대 업적 
    #define MAX_ACHIEVEMENT_REWARD				(2)     //업적 보상 개수
    #define MAX_ACHIEVEMENT_OBJECT				(8)     //업적 과재 개수
    #define ACHIEVEMENT_UINT_SIZE				(19)    //업적 하나의 길이
#endif

#define MAX_SKILL_SLOT_NUM				(100)

#define MAX_VENDOR_SLOT_NUM				(130) //extended by __NA000920_20080306_NPCSHOP_ITEM_SELL_HISTORY (from 125)
#ifdef _NA_0_20101124_VENDOR_SLOT_COUNT_INCREASE
    #define MAX_PERSONAL_VENDOR_SLOT_NUM	(40)
#else
    #define MAX_PERSONAL_VENDOR_SLOT_NUM	(24)
#endif
#define MAX_TOTAL_SLOT_NUM				(MAX_INVENTORY_SLOT_NUM+MAX_EQUIPMENT_SLOT_NUM+MAX_SKILL_SLOT_NUM+MAX_WAREHOUSE_SLOT_NUM+MAX_TEMPINVENTORY_SLOT_NUM)
#define MAX_QUICK_SLOT_NUM				(48)
#define MAX_STYLE_SLOT_NUM				(4)
#define MAX_EVENT_INVENTORY_SLOT_NUM	(5*5)			//< 이벤트 인벤토리 슬롯 개수 한페이지 5*5 = 25

#define MAX_INVENTORY_TAB_NUM			(5)				//< 인벤토리 기본 탭 개수
#define MAX_CHARGE_INVENTORY_TAB_NUM	(5)				//< 인벤토리 기본 탭 개수
#define MAX_INVENTORY_TAB_TOTAL_NUM		(MAX_INVENTORY_TAB_NUM+MAX_CHARGE_INVENTORY_TAB_NUM)
#define MAX_INVENTORY_SLOT_PER_TAB		(15)			//< 인벤토리 탭당 슬롯 개수
enum { MAX_PANDORA_SLOT_NUM	= 8 }; // 판도라 상자 슬롯 개수
#define MAX_CHANGE_ITEMSTAT_SLOT_NUM	(4)				//< 아이템 스텟 변환 슬롯 개수

#define MAX_WAREHOUSE_INVENTORY_PWD_LENGTH	(8)			// 창고 인벤토리 번호 

#define MAX_SSN_STREAM_LENGTH				(10)		// Social Society Number (10자 까지만... 길이 길어도 무시)
#ifdef _KR_0_20120207_CRYPT_SHA256
#define MAX_SSN_ENCODED_STREAM_LENGTH		(65)
#else
#define MAX_SSN_ENCODED_STREAM_LENGTH		(16)
#endif

//게임서버에서만 사용하는 슬롯
#define MAX_TRADE_SLOT_NUM				(20)

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 상점 리스트 관련
#define MAX_ONLOOKER_VENDOR_NUM			(100)			//상점목록 NPC 사용자 POOL의 Init size(늘어날 수 있음)
#define COUNT_SEARCH_CONTAINER_INIT		(10)			//상점목록 검색시 벡터의 Init size(늘어날 수 있음)
#define COUNT_VENDOR_IN_PAGE			(10)			//상점목록 한페이지에 들어갈 리스트 수
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

#define TEMP_DBSERIAL_VALUE				(0)
#define DBSERIAL_MASK_VALUE				(0xFFFFFFFFFFFFFFFF)
#define QUICK_SLOT_NULL_VALUE			(255)
#define INVALID_SLOT_POS				(255)
#define MAX_LEVEL						( 100 )		// 최대 레벨(차후 최대 레벨은 바뀔 수 있다고 함!!)

#define MIN_PENALTY_LEVEL			    ( 11 )      // 최소 패널티 적용 레벨
#ifdef _NA_003966_20111227_ADD_ENCHANT
enum { MAX_ENCHANT_GRADE = 15 };
#else
enum { MAX_ENCHANT_GRADE = 12 };
#endif
enum { MAX_AWAKENING_GRADE = 9, };                  // 최대 각성 레벨 // _NA_008124_20150313_AWAKENING_SYSTEM
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

// 미션 관련
#ifdef _NA_1388_20101207_EP2_INCREASED_MISSION_MAX
    #ifdef _NA_003649_20111115_BATTLE_ZONE_HARDMODE
        #define MAX_MISSION_NUM                 100
    #else
        #define MAX_MISSION_NUM                 50
    #endif
#else
	#define MAX_MISSION_NUM					32
#endif
#define MAX_HIDDEN_MISSION_NUM			16
#define MAX_MAPCODE_NUM					0xFFFF

#define BEGIN_MISSION_MAP				1		//미션 맵코드  시작
#define BEGIN_CHUNTING_MAP				40001	//C헌팅 맵코드 시작
#define BEGIN_PVP_MAP					20001	//PVP 맵코드   시작	

//미션 릴레이 관련.
#define MISSION_RELAY_NOTICE_TIME		21000

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 레벨별 기능 제한 관련
//#define	CREATE_GUILD_LIMIT_LEVEL		(20)
#define LIMIT_LEVEL_WARHOUSE			(6)
#define LIMIT_LEVEL_RANKUP				(7)
#define LIMIT_LEVEL_ENCHANT				(8)
#define LIMIT_LEVEL_ACCESSORY			(15)
#define LIMIT_LEVEL_SACCESSORY			(30)

#ifdef _NA_007408_20140711_MODIFY_CRYSTALLIZE
// 결정화 레벨제한 삭제
#else
#define LIMIT_LEVEL_CRYSTALIZE			(9)
#endif //_NA_007408_20140711_MODIFY_CRYSTALLIZE
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

/// 새로 정의
#define MAX_EXP_POTION_VALUE		( 10000000 ) //경험치 물약으로 올릴 수 있는 최대 경험치
#define DISTANCE_SPACE_GET_ITEM		7.0f	// 스페이스로 먹을 수 있는 아이템은 10 미터이내 있는 아이템

#ifdef _NA_004530_20120315_1202_SKILL_RENEWAL
    enum { MAX_SKILL_LEVEL = 10 }; // 최대 스킬 레벨
    enum { MAX_SKILL_OVER_LEVEL = 20 }; // 최대 스킬 오버 레벨
#else
    #ifdef _NA_0_20100901_SKILLTREE_SYSTEM
        enum { MAX_SKILL_LEVEL = 5 }; // 최대 스킬 레벨
        enum { MAX_SKILL_OVER_LEVEL = 10 }; // 최대 스킬 오버 레벨
    #else
        enum { MAX_SKILL_LEVEL = 10 }; // 최대 스킬 레벨
        enum { MAX_SKILL_OVER_LEVEL = 20 }; // 최대 스킬 오버 레벨
    #endif
#endif

// 이름 길이 정의
#define MAX_CLIENT_CHARNAME_LENGTH	( MAX_CHARNAME_LENGTH + 1 )	 // 클라이언트에 보내는 패킷이나 클라이언트에서 캐릭터 이름을 담기위한 스트링 길이.
#define MAX_CLASSNAME_LENGTH		( 16 )
#define MAX_TITLEID_LENGTH			( 16 )		// 캐릭터이름이외의 특수 칭호
#define MAX_SHOPNAME_LENGTH			( 32 )		// 상점 이름 길이

#define MAX_SMALLDATETIME_SIZE		( 16 )		// "2006-10-12 24:03" ( 분단위 )
#define MAX_MIDDATETIME_SIZE		( 19 )		// "2006-10-12 24:03:00" ( 초단위 )
#define MAX_DATETIME_SIZE			( 23 )		// "2006-10-12 24:03:00.123" ( 밀리초단위 )

#ifdef _INTERNATIONAL_UI
 #if defined (_GLOBAL) || defined (_RUSSIA)
	#define MAX_ROOMTITLE_LENGTH		( 31 )
 #else 
	#define MAX_ROOMTITLE_LENGTH		( 29 )
 #endif//북미는 30자 그외는 28자로제한
#else
    //_NA_20110630_BATTLE_ZONE_RENEWAL
    #define MAX_ROOMTITLE_LENGTH		( 31 )
#endif//_INTERNATIONAL_UI(영문은 30자로제한합니다.)

#define MAX_ROOMPASSWORD_LENGTH		( 7 )		// 방 비밀번호 최대 길이
#define	MAX_VENDOR_TITLE_LENGTH		(80)		//< 개인상점 타이틀 길이
#define	MAX_VENDOR_SEARCH_LENGTH	(20)		//< 검색어 길이
#define VENDOR_ITEMS_PER_PAGE       (12)        // 개인상점 검색결과 페이지당 아이템 개수

#define MAX_CHAT_LEN				(100)
#define MAX_NOTICE_LEN				(1024)
#define MAX_SHOUT_LEN				(255)

// 개수 정의
#define MAX_MONSTER_NUM				( 10000 )	// 게임 전체의 몬스터 수
#define MAX_PLAYER_NUM				( 1000 )	// 게임 전체의 최대 동접 캐릭터 수
#define MAX_FIELDITEM_NUM			( 2000 )	// 게임 전체의 아이템 수
#define MAX_TARGET_NUM				( 32 )		// 범위공격시 최대공격 가능한 캐릭터 수
enum { MAX_FIGHTING_ENERGY_NUM = 5 }; // 최대 충전시킬 수 있는 투기 개수
#define MAX_EFFECT_COUNT			( 10 )		// 스킬시전시 최대 이펙트 개수

#define MAX_PARTYMEMBER_NUM			( 10 )		// 파티원의 최대 개수
#define MAX_FIELD_PARTYMEMBER_NUM	( 10 )		// 필드에서 파티원의 최대 개수

#ifdef _NA_002050_20110216_ADD_GENDER
    #define MAX_CHARACTER_LIST_NUM (10) // 한 서버군에서 캐릭터 생성할 수 있는 최대 개수
    #define MAX_CHARACTER_SLOT_NUM (10) // 한 서버군에서 캐릭터 생성할 수 있는 최대 개수
    // 캐릭터 슬롯 개수 관련 설정 (변경 시 재빌드 필요)
    struct MaxCharListConfig
    {
        enum { kDefaultSlotSize = 6 }; // 기본 캐릭터 슬롯 개수
        enum { kMaxSlotSize = MAX_CHARACTER_LIST_NUM }; // 최대 캐릭터 슬롯 개수
        enum { kUseExtension = 1 }; // 캐릭터 슬롯 확장 사용 여부 (0, 1)
        enum { kSlotSizePerExtension = 1 }; // 확장 시도시 늘어나는 캐릭터 슬롯 개수
    };
#else
    #define MAX_CHARACTER_LIST_NUM (6) // 한 서버군에서 캐릭터 생성할 수 있는 최대 개수
    #define MAX_CHARACTER_SLOT_NUM (6) // 한 서버군에서 캐릭터 생성할 수 있는 최대 개수
#endif // _NA_002050_20110216_ADD_GENDER

#define VILLAGE_SECTOR_SIZE			40			// 마을맵의 섹터 크기(뷰포트 처리를 위한 한개의 섹터 크기(정사각형))
#define FIELD_SECTOR_SIZE			40			// 필드의 섹터 크기
#define ROOM_SECTOR_SIZE			40			// 배틀룸의 섹터 크기
#define DOMINATION_SECTOR_SIZE		40			// 지역점령전의 섹터 크기

#define PLAYER_MOVE_UPDATE_TICK		(500)		// 플레이어의 이동 업데이트 Tick
#define SERVER_UPDATE_TICK			(20)		// 서버의 전체 Update Tick


#define DISTANCE_NPC_MEET			6.0f		// 6 미터이내 npc와 접선 할수 있는 위치로 본다.
#define JUMP_DAMAGE_HEIGHT			8.0f		// 8 미터 부터 점프 데미지 적용
#define JUMP_METER_PER_DAMAGE		60.0f		// 미터당 데미지

#define MAX_IPADDRESS_SIZE					15

#define DEFAULT_ADJUST_LIMIT			(3.0f)							// GetTileToStand의 인자값

// 애니메이션 프레임별 시간
// 엔진에서 DEF_NUM_FRAMES_PER_MS 의 값은 0.03f 
#define DEF_NUM_FRAMES_PER_MS (30)
#define SPEED_MULTIPLIER				(0.001f)	// 캐릭터 스피드 계수
#define MAX_MAP_NUM						(100)		// World.txt의 Row의 개수

//개인상점 수수료
#define TRADE_COMMISSION_RATE			(0.05f)		//< 5%

#if NATION_BUILD_VERSION >= 1002
#define CHAR_DESTROY_LEVEL				(70)		//< 캐릭터 완전 삭제하는 레벨 (70)
#else
#define CHAR_DESTROY_LEVEL				(40)		//< 캐릭터 완전 삭제하는 레벨 (40)
#endif

// GM 명령의 MAX Define 값
#define GM_MAX_CREATE_ITEM_NUM			( 1000)
#define GM_MAX_CREATE_MONEY				( 1000000000)

#define GM_MAX_STAT_UP_VALUE			( 100)
#define GM_MAX_SKILLPOINT_UP_VALUE		( 100)
#define	MAX_RANK_OPTION					( 49)
#define	MAX_ITEM_GRADE					( 1)		//현재는 0은 일반, 1은 디바인아이템이다. 추후 2는 유니크 아이템이 될것이다.

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// PVP관련 시간 딜레이
#define PVP_PRESS_KEY_TIMEOUT_TIME		(30*1000)
#define PVP_PRESS_KEY_LEAVE_TIME		(PVP_PRESS_KEY_TIMEOUT_TIME - (10*1000))	// 20 초 동안 응답없으면 마을로 돌아가는 패킷날림
#define PVP_MATCHLESSS_DELAY_TIME		(10*1000)
#define PVP_REVIVAL_DELAY_TIME			(20*1000)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 경쟁헌팅 관련 시간 딜레이
#define CHUNTING_TIMEATTACK_EXPIRE_TIME				(20*60*1000)	// 타임어택 유지시간
#define CHUNTING_SURVIVAL_EXPIRE_TIME				(20*60*1000)	// 서버이벌 유지시간
#define CHUNTING_INIT_POTION_NUM					(40)			// 초기포션 개수
#define CHUNTING_MAX_MEMBER_NUM						(10)			// 최대 멤버수
#define CHUNTING_RESURRECTION_DELAY_TIME			(5*1000)        // 부활 딜레이
#define CHUNTING_INTERVAL_START_READY				(20*1000)       // 타 파티 입성 후 게임 시작까지 딜레이
#define CHUNTING_TIMEATTACK_EXPIRE_TIME_GUILD_PVP	(10*60*1000)
#define CHUNTING_SURVIVAL_EXPIRE_TIME_GUILD_PVP		(10*60*1000)

#define SKILL_COOLTIME_MARGIN (500) // Server: 클라보다는 쿨타임을 작게 잡아서 쿨타임에 의한 스킬 실패율을 낮춰준다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 길드 관련
#define	MAX_GUILDMARKSTREAM_SIZE		(3072)
#define	MAX_GUILDSTYLESTREAM_SIZE		(8)

#define	MAX_GUILDNOTICE_LENGTH			(768)
#define MAX_GUILD_INTRODUCTION_LENGTH   (768)

#define	CREATE_GUILD_LIMIT_LEVEL		(20)			//<< 길드창설 레벨제한
#define GUILD_UP2GP_RATE				(1000000)		//<< 길드의 UP:GP 변환 비율

#ifdef _NA_007239_20140521_BUG_GUILDRELATION_COUNT
#define MAX_ALLIENCE_GUILD_NUM			(20)			// 동맹 길드 최대수
#define MAX_HOSTILITY_GUILD_NUM			(20)				// 적대 길드 최대수 
#define MAX_ONE_SIDE_HOSTILITY_NUM		(100)			// 일방적 적대선포 길드 최대수
#else // 길드리뉴얼 이전-이후
#define MAX_GUILD_RELATION_NUM			(10)			// 한 길드당 관계 길드 최대수
#define MAX_ALLIENCE_GUILD_NUM			(10)			// 동맹 길드 최대수
#define MAX_HOSTILITY_GUILD_NUM			(5)				// 적대 길드 최대수 
#define MAX_ONE_SIDE_HOSTILITY_NUM		(100)			// 일방적 적대선포 길드 최대수
#endif //_NA_007239_20140521_BUG_GUILDRELATION_COUNT

#define MAX_GUILD_MEMBER_NUM            (200)			//<< 현재 받을 수 있는 최대 길드원 수
#define	MAX_GUILD_CORPS_MEMEBER_NUM     (60)			//<< 길드 군단이 받을 수 있는 최대 길드원 수

#define MAX_GUILD_CORPS_NUM			(8)				// 길드당 최대 군단수(직할군단 포함)

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#define MAX_GUILD_JOIN_REQUEST_NUM      (30)            // 길드 가입 요청 목록 최대수
#define MAX_GUILD_JOIN_REQUEST_TIME_MAX (24*60*60)      // 길드 가입 요청 유지 시간(단위 초)
#define MAX_GUILD_JOIN_REQUEST_PLAYER_NUM (5)           // 플레이어가 신청할 수 있는 최대 가입 요청 수

#define MAX_GUILD_FACILITY              (8)             // 길드 시설 최대수
#define MAX_GUILD_FACILITY_OPTION       (2)             // 길드 시설 옵션 최대수

typedef BYTE FCODE;                                     //길드 시설 종류
typedef int  FLEVEL;                                    //길드 시설 레벨
typedef BYTE ATYPE;                                     //길드 활성화 종류
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 타락한 사원
#define VERSION_AREA_TILE				( 1)            //WAT파일 버전  

typedef INT					UPTYPE;
typedef INT					GPTYPE;
typedef INT					GUILDGUID;
typedef INT					GUILDMARKIDX;		///< 맵코드
typedef	INT					CORPSGUID;			
typedef	INT					CAMPGUID;
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
typedef INT					CHARGUID;
typedef INT					USERGUID;
typedef BYTE				MISSIONNO;			//< MAX_MISSION_NUM를 넘지 않는다.
typedef int					RANKINGKEY;			//< 랭킹 일련 번호 
#if defined (_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)
typedef BYTE                RANKINGID;          //< 맵코드의 중복으로 인한 고유 키값 추가.
typedef BYTE                RANKINGTYPE;         //< 맵코드가 같고 랭킹 타입 다를 수 있기 때문에 추가.
typedef WORD               RANKING;            // 랭킹 순위 표시 타입
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM
typedef WORD				RANKINGNO;			//< 랭킹 종류별 번호 == MAPCODE
typedef	INT					CORPSGUID;			//< 길드 군단 GUID
typedef INT                 GAMECHUUSN;
typedef UINT64              BETAKEY;
#ifdef _NA_0_20100819_AGGRO_RENEWAL
typedef DWORD               BATTLEPOINT;
#else
typedef WORD                BATTLEPOINT;
#endif
#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
typedef INT64               SCORE;              //< 랭킹 점수
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM

/// 항상 타입을 이것을 사용하기로 함!!!!! 자료형(DATATYPE) 통일
typedef WORD				MAPID;				///< server identification
typedef int					SOUND_INDEX;		///< sound index : it's same sound handle
typedef DWORD				PLAYERKEY;

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// WAYPOINT 관련
typedef WORD				WPINDEX;			//< 현재 DB는 int로 되어있음
#ifdef _NA_007942_20150113_MODIFY_RUNE_STONE_WAYPOINT_MAXCOUNT
  #define MAX_CHAR_WAYPOINT_NUM		(15)			// 개인 웨이 포인트 최대 갯수
#else
  #define MAX_CHAR_WAYPOINT_NUM		(10)			// 개인 웨이 포인트 최대 갯수
#endif //_NA_007942_20150113_MODIFY_RUNE_STONE_WAYPOINT_MAXCOUNT
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 채집 관련
#define MAX_COLLECTION_LIST_NUM	(255)			//< 스크립트의 최대 갯수 (채집 가능한 채집물 종류)
#define MAX_FIELD_COLLECTION_NUM (255)			//< 필드에 존재 하는 최대 갯수

#define MAX_COLLECT_DISTANCE_OBJ	(10)		//< 채집물과 채집 가능한 최대 거리		(채집물과 거리차)
#define MAX_COLLECT_DISTANCE_CHAR	(5)			//< 케릭터가 보상 받을수 있는 최대 거리	(시작시와 종료시 거리차)

//{__NA001283_20090303_COLLECTION_MAPOBJECTID
// 맵오브젝트 키는 생성시 20000번 대를 사용 내부적으로 사용되며,
// 트리거에서는 1번부터 시작되는 번호를 사용, 
// 트리거쪽과 연관된 로직에서는 트리거 번호로 변경 필요하다.

// MapObjectKey는 (200000~)값을 사용하며, TriggerObjectKey는 (1~)값을 사용한다.
//하위키(1~)를 상위키(200000~)로 변환
#define Conv_MapObjectKey(Key)	(MAP_OBJECT_KEY>Key?Key+MAP_OBJECT_KEY:Key)

//상위키(200000~)를 하위키(1~)로 변환
#define Conv_TriggerObjectKey(Key)	(MAP_OBJECT_KEY<=Key?Key-MAP_OBJECT_KEY:Key)
//}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//그룹 맺기 시스템 '__KR_001355_20090608_GROUPINGSYSTEM_DATA'
#define MAX_GROUP_MEMBER_LOW_MAX (5)        //개인이 가질수 있는 최대 하위 목록 개수
#define MAX_GROUP_MEMBER_HIGH_MAX (1)       //개인이 가질수 있는 최대 상위 목록 개수
#define MAX_GROUP_MEMBER (MAX_GROUP_MEMBER_LOW_MAX + MAX_GROUP_MEMBER_HIGH_MAX)//그룹총 인원수
#define MAX_DATE_GROUPBONUS (7)             //그룹경험치 보너스를 받기 위해 재접해야할 날수
#define HIGH_MEMBER_LEVEL (80)              //상위 멤버 레벨, 중국 버전에서만 사용
#define LOW_MEMBER_LEVEL (10)               //하위 멤버 레벨, 중국 버전에서만 사용
#if defined (_CHINA)
    #define MAX_GROUP_EXP (10000000)	            //오프라인 최대 누적 경험치
#else
    #define MAX_GROUP_EXP (3000000)	            //오프라인 최대 누적 경험치
#endif

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

typedef DWORD REGENID;
typedef DWORD UNITGROUPID;

// 인증 관련해서 프로토콜에서 사용하는값
const int GAMECHU_USN_MAX       = 10;
const int INFO_MAX_LEN			= 64;

#if defined(__CH1305_090303_ACCOUNT_ID_LENGTH_EXTENTION__)
    const int ID_MAX_LEN        = 50;
#else
    const int ID_MAX_LEN        = 24;
#endif

#ifndef CHECK_ID_LENGTH_BUFFER
    #if defined(__CH1305_090303_ACCOUNT_ID_LENGTH_EXTENTION__) || defined(_ACCOUNT_ID_LENGTH_EXTENTION_)
        #define CHECK_ID_LENGTH_BUFFER(member)  \
            BOOST_STATIC_ASSERT(static_cast<size_t>(sizeof(member)/sizeof(member[0])) == (ID_MAX_LEN + 1) && \
                                ID_MAX_LEN == 50)
    #else
        #define CHECK_ID_LENGTH_BUFFER(member)  \
            BOOST_STATIC_ASSERT(static_cast<size_t>(sizeof(member)/sizeof(member[0])) >= ID_MAX_LEN && \
                                ID_MAX_LEN == 24)
    #endif
#endif


const int PASSWD_MAX_LEN		= 24;
#define MAX_IP_STRING_LEN		15  // NOTE: f110923.1L, moved from serverstruct.h
const int IP_MAX_LEN			= 32;	// NOTE: f110923.1L, replace with MAX_IP_STRING_LEN. 유니코드를 대비해서 Size를 늘림(16->32)
const int MAX_IP_LENGTH         = 16;   // NOTE: f110923.1L, does not use anymore, 샵 서버와 맞추기 위해 추가
const int AUTH_SERIAL_KEY_LEN	= 32;	// 접속 시리얼 키(인증)

const int MAX_WORLD_NAME_LEN	= 32;	// 월드이름 길이
const int MAX_CHANNEL_NAME_LEN  = 32;   // 채널이름 길이

const DWORD HEARTBEAT_TICK      = 20000;
#if USING_GAMEGUARD
const DWORD GAME_GUARD_CHECK_TICK = 5000; // 5초에 한 번씩 게임가드 체크
#endif // USING_GAMEGUARD

#ifdef _KR_0_20120207_CRYPT_SHA256
    enum { kCryptStringSize = 65 };
#else
    enum { kCryptStringSize = 16 };
#endif

//로그파일의 경로 최대 길이
#define		MAX_LOGPATH_LEN			256
//일정 금액 이상 거래시 로그 남김(1억)
#define		LOG_TRADE_OVER_MONEY	100000000
//미션방 생성시 레벨 차이값.
#define		MISSION_LEVEL_JOIN_OFFSET	25

#define		HUNTING_JOIN_LEVEL_OFFSET	15

const BYTE MAX_VALKYRIE_RELOAD_COUNT = 20;	// 발키리 무기 최대 장전개수

#ifdef _NA_000094_20100622_ENHANCED_USER_BLOCK_LIST
    #define MAX_FRIEND_STAT_NUM	200 // 한 패킷에 들어갈 수 있는 최대 친구 숫자
    #define MAX_BLOCK_STAT_NUM 200 // 한 패킷에 들어갈 수 있는 최대 차단 숫자
    #define MAX_FRIENDBLOCK_STAT_NUM 400 // 한 패킷에 들어갈 수 있는 친구/차단 포함 최대 숫자
#else
    #define MAX_FRIEND_STAT_NUM	90 // 한 패킷에 들어갈 수 있는 최대 친구 숫자
    #define MAX_BLOCK_STAT_NUM 90 // 한 패킷에 들어갈 수 있는 최대 차단 숫자
    #define MAX_FRIENDBLOCK_STAT_NUM 190 // 한 패킷에 들어갈 수 있는 친구/차단 포함 최대 숫자
#endif //_NA_000094_20100622_ENHANCED_USER_BLOCK_LIST

#define MAX_REVENGE_STAT_NUM 10 // 한 패킷에 들어갈 수 있는 최대 리벤지 대상 숫자
#define		MAX_FRIENDINVITATE_REWARD_NUM	MAX_FRIEND_STAT_NUM * 4 // 한 패킷에 들어갈 수 있는 친구 보상 목록 정보
//_NA_0_20100527_ADD_CHAR_RELATION_MEMO
#define     MAX_FRIEND_MEMO_LENGHT      114     // 상세 정보창의 메모 사이즈

#define __AUTH_TEST__

/*정리된 디파인*/#define REMOVE_ATTACK_MOVE              // 2006 .2 .18 / 박준태 / 공격이동 제거

// 이벤트 관련 값
#define		_MAX_EVENT_REGITEM			5		// 한 이벤트에 등록할 수 있는 최대 아이템 개수

// NOTE: increased by _KR_0_20091021_FIRENDINVITATION_EXTEND //친구초대 이벤트는 보상 이 4가지 타입이당.
#define		_MAX_EVENT_REWARD			4		// 한 이벤트에서 주는 보상 가지 수 

#define		_MAX_EVENT_ROW_NUM			100		// 현재 수행중인 이벤트 최대 개수

#define		MAX_AKPATTERN_SIZE				1024	// 아머킷 패턴 데이터 최대 크기

#define MAX_MEMO_NUM			200		// 최대 쪽지 개수
#define MAX_MEMO_PAGENUM		20		// 한 페이지(패킷 단위)에 보내지는 최대 쪽지 개수(총 200개면 20*10 = 200이므로 10번 전송함)
#define MAX_MEMO_LENGTH			200		// 실제 길이(BYTE)
#define MAX_MEMO_STRLENGTH		201		// NULL포함
#define MAX_SUBJECT_WLENGTH		10		// 실제 길이(wchar_t)
#define MAX_SUBJECT_LENGTH		20		// 실제 길이(BYTE)
#define MAX_SUBJECT_STRLENGTH	21		// NULL포함
typedef ULONGLONG					MEMOGUID;

// 채널채팅방 최대 입장 인원(define 없었다)
#if defined (_RUSSIA)
#define	MAX_CHATZONE_USER			500
#else
#define	MAX_CHATZONE_USER			255
#endif
// Null 문자를 DWORD 형으로 변형한 값 
#define Null_Value 1819047246

// 인증서버에서 클라이언트 접속종료 요청할 때 이유 플래그 추가-창현씨가 지워서 다시 추가. 패치 나갈 때 define 풀려야 함
#define _AUTH_USERDISCONN_CODE

#define	MAX_VERSION_SIZE			64		// 스크립트나 각종 버전의 사이즈

//크래쉬리포트 관련 값들.
#define	MAX_MODULE_NAME_SIZE		64
#define MAX_BUILD_VERSION_SIZE		8
#define	MAX_OS_NAME_SIZE			64
#define	MAX_PROCESS_NAME_SIZE		100
#define	MAX_PHYSICAL_MEMORY_SIZE	32
#define	MAX_DIRECTX_NAME_SIZE		64
#define	MAX_DESCRIPT_SIZE			64
#define	MAX_DISPLAY_MEMORY_SIZE		32
#define	MAX_DRIVER_VERSION_SIZE		32

#define __NA_515_20070608_JUMP_DAMAGE

#define MAX_SOCKET_OPTION_INDEX			    1024    
#define MAX_ITEM_OPTION_KIND_NUM			128     // 삭제 _NA_000251_20100727_SOCKET_SYSTEM

#define SKILLCODE_INVALID					0
#define SKILLCODE_NORMAL_ATTACK				65500
#define SKILLCODE_FIGHINGENERGY				2101		// 스피릿리차지 스킬코드

#define MAX_GUILD_DELEGATE_RIGHT			6			//__NA_000596_20070821_GUILD_CLASS_SYSTEM


#define INFINITE_ITEM_DAY					10000

#define HEIGHT_OF_STORY						20			// 한 층의 높이

 ///////////////////////////////////////////길드 ///////////////////////////////////////////
#define INVALID_GUILDGUID	(0)
#define MAX_REPURCHASE_ITEM_NUM	(5)                             // 재구매 아이템 목록의 최대 갯수
#define GUILDWAREHOUSE_USING_LEVEL	(1)                         // 길드 창고를 사용할 수 없는 레벨

#ifdef _NA_003923_20120130_GUILD_RENEWAL
    #define GUILDWAREHOUSE_SLOTEX_GUILD_GRADE       (3)         // 확장 슬롯을 사용할 수 없는 길드 등급
    #define GUILDWAREHOUSE_SLOTEXPOS	            (25)        // 길드 레벨 3이상부터 사용가능한 슬롯 위치 번호
    #define GUILDWAREHOUSE_SLOTEXPOS_BY_CASHITEM	(50)	    // 길드 레벨 캐쉬 아이템을 사용함으로써 사용 가능한 슬롯 위치 번호
    #define MAX_GUILDWAREHOUSE_SLOT_PER_TAB         (25)        //  길드 창고 탭당 슬롯 개수
#else
    #define GUILDWAREHOUSE_SLOTEX_GUILD_GRADE       (2)         // 확장 슬롯을 사용할 수 없는 길드 등급
    #define GUILDWAREHOUSE_SLOTEXPOS	            (24)        // 길드 레벨 2이상부터 사용가능한 슬롯 위치 번호
    #define GUILDWAREHOUSE_SLOTEXPOS_BY_CASHITEM	(48)	    // 길드 레벨 캐쉬 아이템을 사용함으로써 사용 가능한 슬롯 위치 번호
    #define MAX_GUILDWAREHOUSE_SLOT_PER_TAB         (24)        //  길드 창고 탭당 슬롯 개수
#endif //_NA_003923_20120130_GUILD_RENEWAL

#define MAX_GUILDWAREHOUSE_TAB_NUM (1)                       // 길드 창고 기본 탭 개수
#define MAX_GRADE_GUILDWAREHOUSE_TAB_NUM	 (1)        // 길드 창고 길드 등급이 3이 됨으로써 사용 가능한 최대 탭 개수
#define MAX_CHARGE_GUILDWAREHOUSE_TAB_NUM	 (1)        // 길드 창고 캐쉬템으로 사용 가능한 최대 탭 개수
#define MAX_GUILDWAREHOUSE_TAB_TOTAL_NUM (3)           // 길드 창고에서 사용가능한 최대 탭 개수

#define MAX_GUILDWAREHOUSE_LOGLIST_NUM	(200)
#define MAX_GUILDWAREHOUSE_LOGPAGE_NUM	(20)

#define GUILD_DESTROY_WAIT_TIME	(3*24*60)

#define GUILDGRADE_AT_GUILDWAREHOUSE_CREATE (2)         //길드 창고를 생성할 수 있는 Guild Grade
#define GUILDWAREHOUSE_TAB_COUNT_AT_GUILD_CREATE (1)    //길드 창고 생성시에 주어지는 창고 탭 개수
#define GRADE_AT_GUILD_CREATE (1)                       //길드 생성시 기본 Guild Grade

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
#define	MAX_GUILDNAME_LENGTH		( 16 )		// 길드명 최대 길이
#endif //_NA_007128_20140326_MONDSCHIEN_DOMINATION
////////////////////////////////////////////지역점령전////////////////////////////////////////
#define MAX_TILE_NODE_CNT_IN_DOMINATION 50   
#define MAX_ATTACK_HEIGHT_IN_DOMINATION 4.5f
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////BIT CONTROL///////////////////////////
#define BIT_RESET(val) (val = 0)
#define BIT_ON(val,bit) (val |= bit)
#define BIT_OFF(val,bit) (val &= (~bit))
#define IS_BIT_ON(val,bit) ((val & bit) == bit)
////////////////////////////////////////////////////////////////////////

#define PACKET_PTR_CONVERT(type, obj, Value)	\
	type* obj = static_cast<type*>(Value);

#define		_MAKESTR(s)		#s
#define		MAKESTR(s)		_MAKESTR(s)

enum { MAX_PET_NAME_LENGTH = 10 }; //__NA_1131_PET_NAME_SETTING

// used by __NA_001359_20090619_HEIM_LOTTO_SYSTEM = {
// 다음 하임 로또 설정 항목들은 클라/서버/DB 협의 없이는 변경 불가
#define LOTTO_MIN_NUMBER            1   // 선택 가능한 최소 숫자
#define LOTTO_MAX_NUMBER            25  // 선택 가능한 최대 숫자
#define LOTTO_NUM_PICKS             4   // 응모/추첨시 몇 개의 숫자를 선택할 것인가
#define LOTTO_NUM_RECENT_DRAWS      2   // 최근 몇 회차의 하임 로또 데이터를 다룰 것인가 ( >= 2)
#define LOTTO_MAX_NOTICE            10  // 최근 몇 개의 당첨 공지 메시지를 보관할 것인가

#if defined (_GLOBAL) || defined (_RUSSIA)
#define LOTTO_MAX_TICKETS           30   // 한 캐릭터가 보유할 수 있는 복권 용지의 최대 갯수
#else
#define LOTTO_MAX_TICKETS           5   // 한 캐릭터가 보유할 수 있는 복권 용지의 최대 갯수
#endif

// 서버측 페이지 처리를 위한 상수
#define LOTTO_NUM_WINNERS_PER_PAGE  200 // 당첨자 목록 선택시 페이지당 레코드 수
// }

#define CHANNEL_DOMINATION_WAR 0

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//지역 점령전 시스템
#define MAX_DOMINATION_ACTIONLIST (10)      // 경매창에 표시될 길드의 리스트 숫자   
#define MAX_DOMINATION_MAP (100)            // 지역 점령전을 시작할 맵의 전체 갯수
#define MAX_DOMINATION_AUCTION (100)         // 한지역 경매에 입찰할 수 있는 길드의 갯수
#define MAX_DOMINATION_DEFENSEWALL_NUM (3)  // 지역점령전 방벽 갯수
#define DOMINATION_DEFFENSWALL_POINT (5)    // 현재 5포인트가 MAX이지만 추후 변경이 가능하다.
// WARNING: to gamelulu, only you need test value (3)!, you must be restore to valid value (20)
#define MAX_DOMINATION_MEMBER (20)          // 점령전에 참여할 수 있는 멤버 최대값
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

#define CONDITION_DELEY             (1000)  // 컨디션 트리거 업데이트 타임

#if defined (_RUSSIA)
    #define GIFT_REQ_USERLEVEL          (20)    //러시아 선물하기 가능 Level
#else
    #define GIFT_REQ_USERLEVEL          (25)    //북미 선물하기 가능 Level
#endif


#ifdef _CHINA  //_NA_001290_20090525_MODIFY_CALC_SD
#define SDINCREASE_RATIO            (3.0f)  // SP 계산용 증가 비율치
#else
//youngmoon #2996 SD 공식 변경
//#define SDINCREASE_RATIO            (8.0f)  
#define SDINCREASE_RATIO            (6.0f)  
#endif

#define STATE_BATTLE_TIME           (5000)  // 배틀 상태 지속 시간
//_NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
#define MAX_SYSTEM_MEMO_LIST        (200)   // 한번에 보낼 수 있는 시스템 메모 갯수
#else
#define MAX_SYSTEM_MEMO_LIST        (100)   // 한번에 보낼 수 있는 시스템 메모 갯수
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR

//_NA_0_20100901_SKILLTREE_SYSTEM
#ifdef _NA_004530_20120315_1202_SKILL_RENEWAL
    enum { CHARACTER_SKILL_LEVEL_RATIO = 1 }; // 레벨 1당 1포인트
    enum { REQUIRE_USED_SKILL_POINT_FOR_NEXT_LINE = 20 };
#else
    enum { CHARACTER_SKILL_LEVEL_RATIO = 3 }; // 캐릭터레벨 3에 스킬레벨 1
    enum { REQUIRE_USED_SKILL_POINT_FOR_NEXT_LINE = 5 }; // 다음 라인의 스킬을 익히기위해 사용된 스킬 포인트 비율
#endif

//_NA_20110630_BATTLE_ZONE_RENEWAL
#define MAX_MISSION_COUNT           (100)   //지정할 수 있는 미션 최대 값
#define MAX_PARTYROOM_LIST          (40)    //배틀존 리스트에 표시되는 파티룸 리스트 최대값
#define MAX_PARTYROOM_SEARCH_LIST   (100)   //검색결과 리스트 최대 값

#define MAX_REQUIRE_ITEM            (10)     //요구 아이템 종류값 (스크립트의 요구 갯수와 일치해야함)
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 캐시 샵 관련
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#define GIFT_MESSAGE_LENGTH 201
#define BUY_STORAGE_ITEM_PER_PAGE 30
#define GIFT_STORAGE_ITEM_PER_PAGE 30
#define SHOP_MAX_ITEM_BUY_COUNT 30
#define SHOP_MAX_ITEM_GIFT_COUNT 30
#define SHOP_MAX_CASH_DETAIL_COUNT 10
#define SHOP_MAX_ITEM_PROPERTY_COUNT 10

// 샵 게임 코드.
#ifdef _KOREA
    #define SHOP_GAMECODE 113 
    #define SHOP_SALESZONE 329
    #define SHOP_SERVERTYPE 0   // 인게임 포인트 유형별 지급 퍼센트 조회(InquireInGamePointValue() 에서 사용). 썬에서는 사용하지 않음.
#elif defined(_CHINA)
    #define SHOP_GAMECODE 862
    #define SHOP_SALESZONE 860
    #define SHOP_SERVERTYPE 0
#elif defined(_JAPAN)
    #define SHOP_GAMECODE 844
    #define SHOP_SALESZONE 847
    #define SHOP_SERVERTYPE 0
#elif defined(_GLOBAL)
    #define SHOP_GAMECODE 503
    #define SHOP_SALESZONE 505
    #define SHOP_SERVERTYPE 687
#elif defined(_RUSSIA)
    #define SHOP_GAMECODE 856
    #define SHOP_SALESZONE 853
    #define SHOP_SERVERTYPE 852
#endif


//_NA_003027_20111013_HONOR_SYSTEM
#define MAX_HONORTITLE_COUNT        (300)
#define MAX_HONOR_POINT_INFO_COUNT  (200)
#define MAX_DAILYQUEST_INFO_COUNT   (300)
#define MAX_DAILYQUEST_PROGRESS_COUNT (24)
#define MAX_GUILD_DAILYQUEST_PROGRESS_COUNT (5)

#ifdef _NA_007407_20140710_CHALLENGE_DAILY_QUEST
#define MAX_CHALLENGE_DAILYQUEST_PROGRESS_COUNT (1000)
#endif //_NA_007407_20140710_CHALLENGE_DAILY_QUEST

//_NA004034_20120102_POINT_WALLET_SYSTEM
#define MAX_POINTWALLET_COUNT       (100)

//_NA_003923_20120130_GUILD_RENEWAL
#define MAX_IN_GAME_GUILD_LOG       (100)
#define MAX_IN_GAME_LOG_PARAM_STR   (20)

// _NA_008283_20150511_NOTIFICATIONS_SYSTEM
#define MAX_NOTIFICATION_COUNT      (50)

// _NA_008404_20150712_MINIGAME_SYSTEM
#define MINIGAME_BINGO_WIDTH        (5)
#define MINIGAME_BINGO_HEIGHT       (5)
#define MINIGAME_BINGO_LENGTH       (MINIGAME_BINGO_WIDTH * MINIGAME_BINGO_HEIGHT)
#define MINIGAME_BINGO_MAX_REWARD_COUNT (12)

#endif // __DEFINE_H__

