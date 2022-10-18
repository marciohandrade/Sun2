#ifndef __AUTH_DEFINE_H__
#define __AUTH_DEFINE_H__

#include <SunAuth/AuthDefine.Build.h>


typedef WORD	AGENTID;

#define GUARD(name, obj)		eXtreme::Guard	< eXtreme::Mutex >		name( obj )


#define	MAX_GROUP_NAME_LEN			32
#define	MAX_SERVER_NAME_LEN			32
#define	MAX_GROUP					32		// SUN ���� ����(�׷�)�� �ִ� ����
#define	MAX_SVR						254		// SUN ���� ������Ʈ �ִ� ���� = ������ ������ �� �ִ��ִ� ä�� ����
	
#define MAX_IP_NAME_LENGTH			15
#define IP_MAX_LEN					32

#define GLOGKEY_LEN					9		//  8 + Null terminated(1)
#define INFO_MAX_LEN				64
#ifdef _ACCOUNT_ID_LENGTH_EXTENTION_
    #define ID_MAX_LEN				50  // ���߿� �����ϵ��� ����... SUN�� ID_MAX_LEN�浹 ����
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

#define TOKEN_BASE64_KEY_MAX		1024 //�߱� ���� Ű �ִ밪

#define MAX_DBCRYPT_LENGTH			16

#define MAX_BETA_KEY                6
//#ifdef  _SHAPE_AUTH	// ���� ����
//	#define SHAPE_DATA_MAX_LEN		4096
//#endif

#ifdef _SECURE_AUTH

	#define MAX_AUTH				64		// ��Ŷ ����ü ���� ���� �ִ밪(���� Matrix ����ϴ� ���� 48byte���� Ȥ�ó� �ؼ� 64byte �����)
	#define MAX_MATRIX				16		// �������� �ִ� ��ǥ ����
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
	AUTHRESULT_OK,						// ���� ����
	AUTHRESULT_INVALIDACCOUNT,			// id Ȥ�� password ����
	AUTHRESULT_SYSTEMERROR,				// �ý��� ���
	AUTHRESULT_NOTEXISTACCOUNT,			// �������� �ʴ� ����(���� ������ ����)
	AUTHRESULT_INVALIDPASSWORD,			// �߸��� ��ȣ(���� ������ ����)
	AUTHRESULT_NOTALLOWEDACCOUNT,		// ������ ������ ���� ����
	AUTHRESULT_PRECONNECTREMAIN,		// ���� ���������� ��������. ����� �ٽ� �õ�
	AUTHRESULT_RESTRICTADULT,			// ���μ����� ������ �� ����
	AUTHRESULT_RESTRICTCROWD,			// ���� ȥ�⵵�� �����̶� ������ �� ����
	AUTHRESULT_BILLINGEXPIRED,			// ���� ���� �ð� ����Ǿ���
	AUTHRESULT_PROCESSLOGIN,			// �̹� �������� ������ ����
	AUTHRESULT_ALREADYLOGIN,			// �̹� ������. ����� �ٽ� �õ�.
	AUTHRESULT_ACCOUNT_BLOCK,			// ���� ���� ������ ���� ���� ��� ����
    AUTHRESULT_GAMECHU_INVALIDUSN,      // ������ ���� ��� ����
    AUTHRESULT_INGAMBA_INVALID,         // ���þ� �ΰ��� ��� ���� ����
	AUTHRESULT_MAX,
};

enum ACCOUNTSTATUS_TYPE
{
	ACCOUNTSTATUS_NONE,					// �������� ���� ����
	ACCOUNTSTATUS_AUTH,					// ���� �Ϸ�
	ACCOUNTSTATUS_TRYPLAY,				// ���ӿ� ���� �õ� ��
	ACCOUNTSTATUS_PLAY,					// ���ӿ� ����
	ACCOUNTSTATUS_WAITLOGOUT,			// �α׾ƿ� �����(GameAgent�� Logout ������)
	ACCOUNTSTATUS_OVERTIME,				// �α׾ƿ� ��� �Ѱ� �ð� ������(���� ���� �ֱ� �Ѿ�� üũ��)
	ACCOUNTSTATUS_MAX,
};

enum USERSTATUS_TYPE
{
	USERSTATUS_LOGOUT,					// ���ӵ��� ���� ����
	USERSTATUS_TRYLOGIN,				// ���ӿ� ���� �õ���(id/pw üũ�Ϸ� ~ �������� �õ�)
	USERSTATUS_PLAY,					// ���ӿ� ���� ��
};

enum eIPTYPE
{
	eIPTYPE_COUNTRY				= 1,	// ���
	eIPTYPE_EXCEPT,						// ����(�������¶� �������)
	eIPTYPE_OFFICE,						// �系
	eIPTYPE_INTERCEPT,					// ����
};

enum eSERIALIZE_TYPE
{
	eSERIALIZE_LOAD,
	eSERIALIZE_SAVE,
};

#ifdef _SECURE_AUTH
	enum eSECURE_AUTH_CODE
	{
		eSC_NONE,						// ���� ���� ��� ����
		eSC_SCARD,						// ���� ī��(����� �̰͸� ���)
		eSC_MAX,
	};
#endif

enum USER_STS
{
	USER_STS_NORMAL				= 0,	// �Ϲ� ����
	USER_STS_1ST_BETA_TESTER	= 1,	// 1����Ÿ �׽���
	USER_STS_BLOCK				= 2,	// ��� ����
	USER_STS_2ND_BETA_TESTER	= 3,	// 2����Ÿ �׽���
	USER_STS_INNER				= 4,	// ���� ����
	USER_STS_FOREIGN			= 5,	// �ؿ� ����
	USER_STS_GM					= 100,	// 100�̻� GM ����
	USER_STS_MAX				= 256,
};

enum RTT_SERVER_TYPE
{
	RTT_RTTAGENT				= 1,
	RTT_AUTHSERVER				= 2,
	RTT_MASTER					= 4,
};

// ���� ���� ���� �ڵ�
enum RTT_DROP_REASON
{
	eDRC_OVERLAPPEDCONN			= 1,	// �ߺ� ����
	eDRC_NOTAUTHUSER			= 2,	// �������� ���� ����
	eDRC_WOPS_GM				= 100,	// GM ��
	eDRC_BILLING_EXPIRE			= 200,	// ���� ����
    eDRC_WEB                    = 300,  // ���� ���� ���� ���� __NA000000_090430_ADDIN_DROP_CODE__
	eDRC_MAX
};

// ����� ���� ����
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
    AUTH_USER_DEFAULT,  // ���� ����
    AUTH_USER_HANGAME,  // �Ѱ��� ����
    AUTH_USER_NEOWIZ,   // �Ǹ� ����
};

#endif //__AUTH_DEFINE_H__


