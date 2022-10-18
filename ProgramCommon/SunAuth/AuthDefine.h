#ifndef __AUTH_DEFINE_H__
#define __AUTH_DEFINE_H__

#include <SunAuth/AuthDefine.Build.h>


typedef WORD	AGENTID;

#define GUARD(name, obj)		eXtreme::Guard	< eXtreme::Mutex >		name( obj )


#define	MAX_GROUP_NAME_LEN			32
#define	MAX_SERVER_NAME_LEN			32
#define	MAX_GROUP					32		// SUN 게임 월드(그룹)의 최대 개수
#define	MAX_SVR						254		// SUN 게임 에이전트 최대 개수 = 유저가 선택할 수 있는최대 채널 개수
	
#define MAX_IP_NAME_LENGTH			15
#define IP_MAX_LEN					32

#define GLOGKEY_LEN					9		//  8 + Null terminated(1)
#define INFO_MAX_LEN				64
#ifdef _ACCOUNT_ID_LENGTH_EXTENTION_
    #define ID_MAX_LEN				50  // 나중에 제거하도록 하자... SUN쪽 ID_MAX_LEN충돌 문제
#else
    #define ID_MAX_LEN				24
#endif

//#if defined(CHECK_ID_LENGTH_BUFFER)
//#pragma push_macro("CHECK_ID_LENGTH_BUFFER")
//#endif

#ifndef CHECK_ID_LENGTH_BUFFER
    #if defined(_ACCOUNT_ID_LENGTH_EXTENTION_)
        #define CHECK_ID_LENGTH_BUFFER(member)  \
            BOOST_STATIC_ASSERT(static_cast<size_t>(sizeof(member)/sizeof(member[0])) == (ID_MAX_LEN + 1) && \
                                ID_MAX_LEN == 50)
    #else
        #define CHECK_ID_LENGTH_BUFFER(member)  \
            BOOST_STATIC_ASSERT(static_cast<size_t>(sizeof(member)/sizeof(member[0])) >= ID_MAX_LEN && \
                                ID_MAX_LEN == 24)
    #endif
#endif
//#pragma pop_macro("CHECK_ID_LENGTH_BUFFER")

#define PASSWD_MAX_LEN				24
#define PC_MAX_LEN					16
#define MAX_AUTHRESULT_MESSAGE_LEN	64

#define TOKEN_BASE64_KEY_MAX		1024 //중국 인증 키 최대값

#define MAX_DBCRYPT_LENGTH			16

#define MAX_BETA_KEY                6
//#ifdef  _SHAPE_AUTH	// 형상 인증
//	#define SHAPE_DATA_MAX_LEN		4096
//#endif

#ifdef _SECURE_AUTH

	#define MAX_AUTH				64		// 패킷 구조체 내부 버퍼 최대값(현재 Matrix 사용하는 값은 48byte지만 혹시나 해서 64byte 잡았음)
	#define MAX_MATRIX				16		// 인증받을 최대 좌표 개수
	#define MAX_MATRIX_KEY			81
	#define MAX_MATRIX_SEED			26
	#define MAX_MATRIX_BASE			1000

#endif
 

enum PACKET_RESULT_TYPE
{
	PACKET_RESULT_SUCCESS,
	PACKET_RESULT_FAIL,
};

enum AUTHRESULT_TYPE
{ 
	AUTHRESULT_OK,						// 인증 성공
	AUTHRESULT_INVALIDACCOUNT,			// id 혹은 password 오류
	AUTHRESULT_SYSTEMERROR,				// 시스템 장애
	AUTHRESULT_NOTEXISTACCOUNT,			// 존재하지 않는 계정(현재 사용되지 않음)
	AUTHRESULT_INVALIDPASSWORD,			// 잘못된 암호(현재 사용되지 않음)
	AUTHRESULT_NOTALLOWEDACCOUNT,		// 접속이 허용되지 않은 계정
	AUTHRESULT_PRECONNECTREMAIN,		// 이전 접속정보가 남아있음. 잠시후 다시 시도
	AUTHRESULT_RESTRICTADULT,			// 성인서버에 접속할 수 없음
	AUTHRESULT_RESTRICTCROWD,			// 서버 혼잡도가 만땅이라 접속할 수 없음
	AUTHRESULT_BILLINGEXPIRED,			// 빌링 결제 시간 만료되었음
	AUTHRESULT_PROCESSLOGIN,			// 이미 인증중인 계정이 존재
	AUTHRESULT_ALREADYLOGIN,			// 이미 접속중. 잠시후 다시 시도.
	AUTHRESULT_ACCOUNT_BLOCK,			// 인증 연속 오류로 인한 계정 블록 상태
    AUTHRESULT_GAMECHU_INVALIDUSN,      // 게임츄 인증 모듈 실패
    AUTHRESULT_INGAMBA_INVALID,         // 러시아 인감바 모듈 인증 실패
	AUTHRESULT_MAX,
};

enum ACCOUNTSTATUS_TYPE
{
	ACCOUNTSTATUS_NONE,					// 인증받지 않은 계정
	ACCOUNTSTATUS_AUTH,					// 인증 완료
	ACCOUNTSTATUS_TRYPLAY,				// 게임에 접속 시도 중
	ACCOUNTSTATUS_PLAY,					// 게임에 접속
	ACCOUNTSTATUS_WAITLOGOUT,			// 로그아웃 대기중(GameAgent에 Logout 보냈음)
	ACCOUNTSTATUS_OVERTIME,				// 로그아웃 대기 한계 시간 지났음(빌링 차감 주기 넘어가면 체크됨)
	ACCOUNTSTATUS_MAX,
};

enum USERSTATUS_TYPE
{
	USERSTATUS_LOGOUT,					// 접속되지 않은 계정
	USERSTATUS_TRYLOGIN,				// 게임에 접속 시도중(id/pw 체크완료 ~ 게임접속 시도)
	USERSTATUS_PLAY,					// 게임에 접속 중
};

enum eIPTYPE
{
	eIPTYPE_COUNTRY				= 1,	// 허용
	eIPTYPE_EXCEPT,						// 예외(금지상태라도 허용해줌)
	eIPTYPE_OFFICE,						// 사내
	eIPTYPE_INTERCEPT,					// 금지
};

enum eSERIALIZE_TYPE
{
	eSERIALIZE_LOAD,
	eSERIALIZE_SAVE,
};

#ifdef _SECURE_AUTH
	enum eSECURE_AUTH_CODE
	{
		eSC_NONE,						// 보안 인증 사용 안함
		eSC_SCARD,						// 보안 카드(현재는 이것만 사용)
		eSC_MAX,
	};
#endif

enum USER_STS
{
	USER_STS_NORMAL				= 0,	// 일반 유저
	USER_STS_1ST_BETA_TESTER	= 1,	// 1차베타 테스터
	USER_STS_BLOCK				= 2,	// 블록 유저
	USER_STS_2ND_BETA_TESTER	= 3,	// 2차베타 테스터
	USER_STS_INNER				= 4,	// 내부 계정
	USER_STS_FOREIGN			= 5,	// 해외 계정
	USER_STS_GM					= 100,	// 100이상 GM 계정
	USER_STS_MAX				= 256,
};

enum RTT_SERVER_TYPE
{
	RTT_RTTAGENT				= 1,
	RTT_AUTHSERVER				= 2,
	RTT_MASTER					= 4,
};

// 강제 종료 이유 코드
enum RTT_DROP_REASON
{
	eDRC_OVERLAPPEDCONN			= 1,	// 중복 접속
	eDRC_NOTAUTHUSER			= 2,	// 인증되지 않은 유저
	eDRC_WOPS_GM				= 100,	// GM 팅
	eDRC_BILLING_EXPIRE			= 200,	// 빌링 만료
    eDRC_WEB                    = 300,  // 웹을 통한 접종 지원 __NA000000_090430_ADDIN_DROP_CODE__
	eDRC_MAX
};

// 사용자 차감 유형
enum BILLING_DEDUCTION_TYPE
{
	eDEDUCTIONTYPE_NONE,
	ePERSONAL_FIXED_AMOUNT,
	ePERSONAL_FIXED_QUANTITY,
	ePCROOM_FIXED_AMOUNT,
	ePCROOM_FIXED_QUANTITY,
	eDEDUCTIONTYPE_MAX,
};

enum VERSION_TYPE
{
	GAME_VERSION_PROTOCOL		= 1,
	GAME_VERSION_LAUNCHER		= 2,
	GAME_VERSION_MAJOR			= 3,
	GAME_VERSION_MINOR			= 4,
	GAME_VERSION_MAX,
};

enum DB_TYPE
{
	DB_TYPE_AUTH,
	DB_TYPE_LOG,
	DB_TYPE_RTT,
	DB_TYPE_NETCAFE,
	DB_TYPE_MAX,
};

enum eAUTH_TYPE
{
	ACCOUNT_AUTH,
	SHAPE_AUTH,
	MATRIX_CARD_AUTH,
	AUTH_FAIL_TYPE_MAX
};


//_CHANNELING_AUTH_FLOW_HANGAME_USER_
enum eAUTH_USER_TYPE
{
    AUTH_USER_DEFAULT,  // 기존 유저
    AUTH_USER_HANGAME,  // 한게임 유저
    AUTH_USER_NEOWIZ,   // 피망 유저
};

#endif //__AUTH_DEFINE_H__


