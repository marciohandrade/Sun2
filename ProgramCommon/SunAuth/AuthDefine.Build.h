#ifndef __AUTH_DEFINE_BUILD_H__
#define __AUTH_DEFINE_BUILD_H__

//==================================================================================================
//
#include "AuthCountryCode.h" // ���� �ڵ�
//
//==================================================================================================
//
#define _AUTH_NATION_CODE_GENERIC   (0)
#define _AUTH_NATION_CODE_KOREA     1000000
#define _AUTH_NATION_CODE_JAPAN     2000000
#define _AUTH_NATION_CODE_USA       3000000
#define _AUTH_NATION_CODE_TAIWAN    4000000
#define _AUTH_NATION_CODE_CHINA     5000000
// 600000 - ���� ENGLAND��� �̸����� �����...(North America)
#define _AUTH_NATION_CODE_GLOBAL    7000000 // (090826) (ADD-IN)
#define _AUTH_NATION_CODE_RUSSIA    8000000 // (20110110) (ADD-IN)
//
//==================================================================================================
// ������ ������ ����(������ ������� �ش� �������� ���� �������� ����)
#if defined(_AUTH_KOREA)
    #define _AUTH_NATION_CODE       _AUTH_NATION_CODE_KOREA
    #define _NATION_BUILD_VERSION   905
    #define _BILLING_ONLYPCBANG     // PC�游 ���� ����/�α׾ƿ� üũ
    #define _BILLING_WAITLOGOUT     // ����� �α׾ƿ� ��û���� �� Ans �� ������ ������ ����Ų��
    #define _BILLING_DISCONNECT     // ���� ����Ǿ��� �� �����ִ� ����
    #define _RESTRICT_ADULT         // ���� û�ҳ��̿�Ұ� �������� ���Ͽ� 18 �̸��� ���� �Ұ�
    #define _CHANNELING_AUTH_FLOW   // __KR000000_090128_CHANNELING__
    #define _CHANNELING_AUTH_FLOW_HANGAME_USER_ // ä�θ� + �Ѱ��� ���� ����

    #define _PCBANG_POLICY_CHANGE_POST_EXPIRED // PC�� ��å ���濡 ����, ����PC���� ���� ���� ���ο� ���� ���� ó��
    #define _PCBANG_POLICY_CHANGE_POST_EXPIRED2 // PC�� ���� ���� �� �� ������ ����
    #define __NA_20100201_INTERGRATEDBILLING_   //2010.02.01 ������ / ���պ��� ����
    //#define __PCBANG_IP_UPDATE                  //���պ������� �Ǿ��� ��ϵ� ������ ����� ����Ǿ����� �ٷ� ó���� �ش�.
    #define _NA000000_CONFIGURATION_TO_ACCEPT_MULTI_AGENT_ON_SAME_IP_ADDRESS // ������ / ���������� ���� IP���� �����Ǵ� Agent�� �����ϱ� ���� ó��
#elif defined(_AUTH_CHINA)
    #define _AUTH_NATION_CODE       _AUTH_NATION_CODE_CHINA
    #define _NATION_BUILD_VERSION   816
    #define _GM_IPCHECK_DISCONN     // GM�� �系 IP �ƴ� ������ �����ϸ� ���������
    //#define _SECURE_AUTH            // ���� ���� �߰�
    //#define _NEW_MATRIX             // �߱� ��Ʈ���� ī�� ���� �Է�
    #define _TIRED_CHECK            // �߱� �Ƿε� üũ �ý���
    #define _AUTH_MATRIX_FAIL_COUNT // FailCount �߱��������̽�
    #define _ACCOUNT_ID_LENGTH_EXTENTION_   // �߱� ���� ID���� 50�ڸ����� Ȯ��
    #define __NA000000_090430_ADDIN_DROP_CODE__ // 2009.04.30 / ������ / ���� ���� ���� ����
	#define __CN_20100909_SECURITY_TOKEN__		//��ū ����
    #define _CN_ZY_AUTH_PROCESS // �߱� ZY ����
    //#define _CN_ZY_AUTH_PROCESS_DBAUTH // �߱� ZY ���� - DB���� ���� ����
#elif defined(_AUTH_TAIWAN)
    #define _AUTH_NATION_CODE       _AUTH_NATION_CODE_TAIWAN
    #define _NATION_BUILD_VERSION   815

#elif defined(_AUTH_JAPAN)
    #define _AUTH_NATION_CODE       _AUTH_NATION_CODE_JAPAN
    #define _NATION_BUILD_VERSION   905
    #define _NET_CAFE       // �Ϻ� �� ī�� ���� DB����
        #undef _NA_005011_20120621_MULTICLIENTS	  // 13.10.08 ��ƼŬ���̾�Ʈ ����
    //#define _JP000000_ONETIMEPASSWORD_AUTHFLOW_ // |10.01.18|������|One-Time Password Authentication flow �߰� �۾�|
    #define _JP_0_20100723_GAMECHU_AUTHFLOW_    // ������ ������ ���� ���� �۾�
    #define _JP_0_20101123_PMS                   //�Ϻ� ���ӿ��� ������ ���� �ý����� PMS ����
#elif defined(_AUTH_USA)
    #define _AUTH_NATION_CODE       _AUTH_NATION_CODE_USA
    #define _NATION_BUILD_VERSION   902
    #define __NA000000_NORTH_AMERICA_HANGAME_CHANNELING__  // 09.06.29 / ������ / �Ϲ� ä�θ�
    #define __NA000000_SHOP_USERID_50_PASSING_PROC__ // 09.07.28 / ������ / �������� UserID(50)����

#elif defined(_AUTH_GLOBAL)
    #define _AUTH_NATION_CODE       _AUTH_NATION_CODE_GLOBAL
    #define _NATION_BUILD_VERSION   902
    #define __NA000000_GLOBAL_AUTH_PROCESS__    // 09.08.26 / ������ / �۷ι� ���� ���� ���μ���
    #define _NA000000_091005_EXTENDED_LOG_ // 2009.10.05 / ������ / ���� ���� �α� ��ȭ|����
    #define __NA000000_SHOP_USERID_50_PASSING_PROC__ // 09.07.28 / ������ / �������� UserID(50)����

	#define _PCBANG_POLICY_CHANGE_POST_EXPIRED  // PC�� ��å ���濡 ����, ����PC���� ���� ���� ���ο� ���� ���� ó��
	#define _PCBANG_POLICY_CHANGE_POST_EXPIRED2 // PC�� ���� ���� �� �� ������ ����
	#define __NA_20100201_INTERGRATEDBILLING_   //2010.02.01 ������ / ���պ��� ����
	#define __20100916_BILLING_GODLCHANNEL__    //��� ä�� ����

#elif defined(_AUTH_RUSSIA)
    #define _AUTH_NATION_CODE       _AUTH_NATION_CODE_RUSSIA
    #define _NATION_BUILD_VERSION   902
    #define _NA000000_091005_EXTENDED_LOG_              // 2009.10.05 / ������ / ���� ���� �α� ��ȭ|����
    #define __NA000000_SHOP_USERID_50_PASSING_PROC__    // 2009.07.28 / ������ / �������� UserID(50)����
    #define _RU_INGAMBA_AUTH_PROCESS                    // 2011.03.14 / ������ / ���þ� Ingamba ���� ���μ���
    #define _PCBANG_POLICY_CHANGE_POST_EXPIRED          // PC�� ��å ���濡 ����, ����PC���� ���� ���� ���ο� ���� ���� ó��
    #define _NA_20120215_RUSSIA_PREMIUM_SERVICE         // 2012.02.09 / ������ / �����̾� ����
#endif

// ���� �������� �����ε�
#if( _NATION_BUILD_VERSION >= 10000 )

    //#define _SHAPE_AUTH           // ���� ���� �ý���
    //#define _PAAS_ALERT           // �߱� PAAS �ý���
    //#define _NEW_BILLING_SYSTEM   // �ű� ���պ��� ����

#endif

// �ֽ� ����� ����
#if (_NATION_BUILD_VERSION >= 905)
    #define __NA000000_SHOP_USERID_50_PASSING_PROC__ // 09.07.28 / ������ / �������� UserID(50)����
#endif

#if (_NATION_BUILD_VERSION >= 903)
    #define _NA000000_091005_EXTENDED_LOG_ // 2009.10.05 / ������ / ���� ���� �α� ��ȭ|����
#endif

#if (_NATION_BUILD_VERSION >= 902)
    #define __NA001334_090420_SERVER_SELECT_FACILITY__    // 2009.04.17 / ������ / ���� ���� ���
#endif


#ifdef __NA_20100201_INTERGRATEDBILLING_
	#ifndef _PCBANG_POLICY_CHANGE_POST_EXPIRED2
	#error "Impossible Build!! - Reason: Violation Dependency Chain"
	#endif
#endif

#ifdef __20100916_BILLING_GODLCHANNEL__
	#ifndef __NA_20100201_INTERGRATEDBILLING_
	// ���ä���� ���պ����� ���� ���� �Ѵ�.
	#error "Impossible Build!! - Reason: Violation Dependency Chain"
	#endif
	#ifdef _BILLING_ONLYPCBANG
	// ��� ä���� ������ �Ǿ���ó�� �̿��ϴ� ���̹Ƿ� �Ǿ��游�� �������� ���� �ȵȴ�.
	#error "Impossible Build!! - Reason: Violation Dependency Chain"
	#endif
#endif

//���þ� �����̾� ���񽺴� ���þ� ���� ���μ��� ������� �����ؾ� �Ѵ�.
#if defined (_NA_20120215_RUSSIA_PREMIUM_SERVICE)
    #if !defined (_RU_INGAMBA_AUTH_PROCESS) || !defined (_PCBANG_POLICY_CHANGE_POST_EXPIRED)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
    #endif
#endif

#if defined(_CHANNELING_AUTH_FLOW_HANGAME_USER_) && \
    !defined(_CHANNELING_AUTH_FLOW)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(__PCBANG_IP_UPDATE) && \
    !defined(__NA_20100201_INTERGRATEDBILLING_)
#error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif



//#define __NA_20100201_INTERGRATEDBILLING_   //2010.02.01 ������ / ���պ��� ����

#endif //__AUTH_DEFINE_BUILD_H__

