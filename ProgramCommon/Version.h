/************************************************************************/
/* Version.h : C/S Version ����
/************************************************************************/

#ifndef	__VERSION_H__
#define	__VERSION_H__

//#include "../include/Global.Define.h"
#include "CountryCode.h" // ���� �ڵ�

// Deployment.h �Ǵ� �ܺο��� NATION_BUILD_VERSION�� �����Ǿ� ���� �ʴٸ� 
// ������ ������ ���� ������ ���ȴ�.

#if defined( _KOREA )
  #ifndef NATION_BUILD_VERSION
	#define NATION_BUILD_VERSION		1601		// �������(����, Ŭ�� ����)
  #endif
    #define XIGNCODE_VERSION            111215
	#define	HACKSHIELD_VERSION			5050601

#elif defined( _JAPAN )
  #ifndef NATION_BUILD_VERSION
	#define NATION_BUILD_VERSION		1302		// �������(����, Ŭ�� ����)
  #endif
	#define XIGNCODE_VERSION            111215
	#define	HACKSHIELD_VERSION			5040601

#elif defined( _AMERICA )

#elif defined( _TAIWAN )
  #ifndef NATION_BUILD_VERSION
	#define NATION_BUILD_VERSION		904			// �������(����, Ŭ�� ����)
  #endif
	#define	HACKSHIELD_VERSION			1055

#elif defined( _CHINA )
  #ifndef NATION_BUILD_VERSION
	#define NATION_BUILD_VERSION		1204		// �������(����, Ŭ�� ����)
  #endif
	#define	HACKSHIELD_VERSION			5050401
    #define XIGNCODE_VERSION            130203

#elif defined( _USA )
  #ifndef NATION_BUILD_VERSION
	#define NATION_BUILD_VERSION		1202		// �������(����, Ŭ�� ����)
  #endif
	#define	HACKSHIELD_VERSION			1055

#elif defined( _GLOBAL )
  #ifndef NATION_BUILD_VERSION
    #define NATION_BUILD_VERSION		1301		// �������(����, Ŭ�� ����)
  #endif
    #define	HACKSHIELD_VERSION			5050601
    #define XIGNCODE_VERSION            111215

#elif defined( _RUSSIA )
#ifndef NATION_BUILD_VERSION
#define NATION_BUILD_VERSION		    1203		// �������(����, Ŭ�� ����)
#endif
#define	HACKSHIELD_VERSION			    5040401


#else //
	#error "Not Defined Preprocessor -Nation Definition-"
#endif //

//
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// ī�װ��� ���������� ��� ��ȣ ���� �߻��⸦ ���� ��ġ �Է�
// ������ ����� �Է��ϼ���.
// <WARNING>
//   1) �ݵ�� 1�� �ڸ��� Ȧ�� ���� �Ǿ�� �մϴ�, (1, 3, 5, 7, 9) �߿� �ϳ��� �Է��ϼ���.
//   2) ���(+)���� �Է��� �ּ���.
//   3) �Է� ��ġ ���� (�ּ� 1001 ~ �ִ� 99999999 )
//
// �Ʒ��� ����
//   #define CATEGORY_ODD_NUMBER		(159371)
//   #define PROTOCOL_ODD_NUMBER		(310025)

//////////////////////////////////////////////////////////////////////////
// nProtect GameGuard ����� ���ǻ���
// GameGuard�� ���̺귯�� �������� �̸��� �����ؾ� �Ѵ�.
// ����, ����� �߱��� ��� SUN1->1204, SUN3->1304(���̻�)
// ���� �ÿ� ���������� SUN1���� SUN3���� �����ϵ��� �Ͽ��� �� ���̴�.
// #if defined(__APPLY_GAMEGUARD)//     
//     #if defined(SUN1)
//         #define _CHINA_GAMEGUARD_NAME "SUN1CN"
//     #elif defined(SUN3)
//         #define _CHINA_GAMEGUARD_NAME "SUN3CN"
//     #else
//         �ƿ� ���尡 �������� ����.
//     #endif // SUN1
// #endif // __APPLY_GAMEGUARD
///////////////////////////////////////////////////////////////////////////

#if defined( _KOREA )
        #if NATION_BUILD_VERSION >= 1601
		#define CATEGORY_ODD_NUMBER         (51375) // 160121
		#define PROTOCOL_ODD_NUMBER         (13547)
		#define __APPLY_XIGNCODE    // �����ڵ� ����
        #elif NATION_BUILD_VERSION >= 1504
		#define CATEGORY_ODD_NUMBER         (87953) // 130805
		#define PROTOCOL_ODD_NUMBER         (94215)
		#define __APPLY_XIGNCODE    // �����ڵ� ����
	#elif NATION_BUILD_VERSION >= 1503
		#define CATEGORY_ODD_NUMBER         (61251) // 130805
		#define PROTOCOL_ODD_NUMBER         (84573)
		#define __APPLY_XIGNCODE    // �����ڵ� ����
	#elif NATION_BUILD_VERSION >= 1502
		#define CATEGORY_ODD_NUMBER         (56319) // 130805
		#define PROTOCOL_ODD_NUMBER         (74121)
		#define __APPLY_XIGNCODE    // �����ڵ� ����
	#elif NATION_BUILD_VERSION >= 1501
		#define CATEGORY_ODD_NUMBER         (36757) // 130805
		#define PROTOCOL_ODD_NUMBER         (51379)
		#define __APPLY_XIGNCODE    // �����ڵ� ����
	#elif NATION_BUILD_VERSION >= 1404
		#define CATEGORY_ODD_NUMBER         (368725) // 130805
		#define PROTOCOL_ODD_NUMBER         (435127)
		#define __APPLY_XIGNCODE    // �����ڵ� ����
	#elif NATION_BUILD_VERSION >= 1403
		#define CATEGORY_ODD_NUMBER         (218973) // 130805
		#define PROTOCOL_ODD_NUMBER         (348205)
		#define __APPLY_XIGNCODE    // �����ڵ� ����
	#elif NATION_BUILD_VERSION >= 1303
        #define CATEGORY_ODD_NUMBER         (218971) // 130805
        #define PROTOCOL_ODD_NUMBER         (348203)
        #define __APPLY_XIGNCODE    // �����ڵ� ����
	#elif NATION_BUILD_VERSION >= 1302
        #define CATEGORY_ODD_NUMBER         (673247) // 130125
        #define PROTOCOL_ODD_NUMBER         (452249)
        #define __APPLY_XIGNCODE    // �����ڵ� ����
	#elif NATION_BUILD_VERSION >= 1301
        #define CATEGORY_ODD_NUMBER         (346243) // 130125
        #define PROTOCOL_ODD_NUMBER         (622135)
        #define __APPLY_XIGNCODE    // �����ڵ� ����
	#elif NATION_BUILD_VERSION >= 1204
        #define CATEGORY_ODD_NUMBER         (346241) // 120502
        #define PROTOCOL_ODD_NUMBER         (622133)
        #define __APPLY_XIGNCODE    // �����ڵ� ����
    #elif NATION_BUILD_VERSION == 1203
        #define CATEGORY_ODD_NUMBER         (304659) // 120502
        #define PROTOCOL_ODD_NUMBER         (342091)
        #define __APPLY_XIGNCODE    // �����ڵ� ����
    #elif NATION_BUILD_VERSION == 1202
        #define CATEGORY_ODD_NUMBER         (264817) // 120502
        #define PROTOCOL_ODD_NUMBER         (492349)
        #define __APPLY_XIGNCODE    // �����ڵ� ����
    #elif NATION_BUILD_VERSION == 1201
        #define CATEGORY_ODD_NUMBER         (519425) // 120109
        #define PROTOCOL_ODD_NUMBER         (987821)
        //#define __APPLY_HACKSHIELD // �ٽ��� ����
        #define __APPLY_XIGNCODE    // �����ڵ� ����
    #elif NATION_BUILD_VERSION == 1103
        #define CATEGORY_ODD_NUMBER         ( 56823) // 110524
        #define PROTOCOL_ODD_NUMBER         (167459)
        //#define __APPLY_HACKSHIELD // �ٽ��� ����
        #define __APPLY_XIGNCODE    // �����ڵ� ����
    #elif NATION_BUILD_VERSION == 1102
        #define CATEGORY_ODD_NUMBER         ( 93187) // 110524
        #define PROTOCOL_ODD_NUMBER         (204653)
        #define __APPLY_HACKSHIELD // �ٽ��� ����
    #elif NATION_BUILD_VERSION == 1101
        #define CATEGORY_ODD_NUMBER         (287631) // 101213
        #define PROTOCOL_ODD_NUMBER         (120945)
        #define __APPLY_HACKSHIELD // �ٽ��� ����
    #elif NATION_BUILD_VERSION == 1006
        #define CATEGORY_ODD_NUMBER         (212635) // 101105
        #define PROTOCOL_ODD_NUMBER         ( 58473)
        #define __APPLY_HACKSHIELD // �ٽ��� ����
    #elif NATION_BUILD_VERSION == 1005
        #define CATEGORY_ODD_NUMBER         ( 72989) // 101001
        #define PROTOCOL_ODD_NUMBER         (351765)
    #elif NATION_BUILD_VERSION == 1004
        #define CATEGORY_ODD_NUMBER         (198473) // 100726
        #define PROTOCOL_ODD_NUMBER         ( 68537)
    #elif NATION_BUILD_VERSION == 1002
        #define CATEGORY_ODD_NUMBER         ( 28391) // 100325
        #define PROTOCOL_ODD_NUMBER         (162745)
    #elif NATION_BUILD_VERSION == 1001
        #define CATEGORY_ODD_NUMBER         (149567) // 091127
        #define PROTOCOL_ODD_NUMBER         ( 51391)
    #elif NATION_BUILD_VERSION == 905
        #define CATEGORY_ODD_NUMBER         (19567) // 091019
        #define PROTOCOL_ODD_NUMBER         (251091)
    #elif NATION_BUILD_VERSION == 904
        #define CATEGORY_ODD_NUMBER         (199567) // 090717
        #define PROTOCOL_ODD_NUMBER         (65191)
    #elif NATION_BUILD_VERSION == 903
        #define CATEGORY_ODD_NUMBER         (45551) // 090305
        #define PROTOCOL_ODD_NUMBER         (119967)
    #else
        #error "Unsupported BuildVersion"
    #endif
    #ifdef _SERVER
        #define NATION_BUILD_NAME_OF_COUNTRY    ""
    #else
        #define NATION_BUILD_NAME_OF_COUNTRY    ""
    #endif
#elif defined( _JAPAN )
    #if NATION_BUILD_VERSION >= 1302
        #define CATEGORY_ODD_NUMBER         (3102283)   // 130620
        #define PROTOCOL_ODD_NUMBER         (1245729)
        #define __APPLY_XIGNCODE    // �����ڵ� ����
    #elif NATION_BUILD_VERSION >= 1301
        #define CATEGORY_ODD_NUMBER         (202289)    // 130307
        #define PROTOCOL_ODD_NUMBER         (482825)
        #define __APPLY_XIGNCODE    // �����ڵ� ����
    #elif NATION_BUILD_VERSION == 1204
        #define CATEGORY_ODD_NUMBER         (452731)    // 120927
        #define PROTOCOL_ODD_NUMBER         (236343)
        #define __APPLY_XIGNCODE    // �����ڵ� ����
    #elif NATION_BUILD_VERSION >= 1203
        #define CATEGORY_ODD_NUMBER         (156829)    // 120927
        #define PROTOCOL_ODD_NUMBER         (267455)
        #define __APPLY_XIGNCODE    // �����ڵ� ����
    #elif NATION_BUILD_VERSION >= 1202
        #define CATEGORY_ODD_NUMBER         (156827)    // 110524
        #define PROTOCOL_ODD_NUMBER         (267453)
    	//#define __APPLY_HACKSHIELD // �ٽ��� ����
        #define __APPLY_XIGNCODE    // �����ڵ� ����
    #elif NATION_BUILD_VERSION >= 1201
        #define CATEGORY_ODD_NUMBER         (156825)    // 110524
        #define PROTOCOL_ODD_NUMBER         (267451)
        //#define __APPLY_HACKSHIELD // �ٽ��� ����
        #define __APPLY_XIGNCODE    // �����ڵ� ����
    #elif NATION_BUILD_VERSION >= 1103
        #define CATEGORY_ODD_NUMBER         (156825)    // 110524
        #define PROTOCOL_ODD_NUMBER         (267451)
        #define __APPLY_HACKSHIELD // �ٽ��� ����
    #elif NATION_BUILD_VERSION >= 1102
        #define CATEGORY_ODD_NUMBER         ( 93187)    // 110524
        #define PROTOCOL_ODD_NUMBER         (204653)
        #define __APPLY_HACKSHIELD // �ٽ��� ����
    #elif NATION_BUILD_VERSION >= 1101
        #define CATEGORY_ODD_NUMBER         (135895)    // 100910
        #define PROTOCOL_ODD_NUMBER         (586017)
        #define __APPLY_HACKSHIELD // �ٽ��� ����
    #elif NATION_BUILD_VERSION >= 1006
        #define CATEGORY_ODD_NUMBER         ( 39717)    // 100910
        #define PROTOCOL_ODD_NUMBER         (732463)
        #define __APPLY_HACKSHIELD // �ٽ��� ����
    #elif NATION_BUILD_VERSION >= 1005
        #define CATEGORY_ODD_NUMBER         ( 76517)    // 100910
        #define PROTOCOL_ODD_NUMBER         (342433)
        #define __APPLY_HACKSHIELD // �ٽ��� ����
    #elif NATION_BUILD_VERSION >= 1004
        #define CATEGORY_ODD_NUMBER         ( 36519)    // 100910
        #define PROTOCOL_ODD_NUMBER         (782065)
        #define __APPLY_HACKSHIELD // �ٽ��� ����
    #elif NATION_BUILD_VERSION == 1002
        #define CATEGORY_ODD_NUMBER         ( 18815)    // 100528
        #define PROTOCOL_ODD_NUMBER         (145171)
    #elif NATION_BUILD_VERSION == 1001
        #define CATEGORY_ODD_NUMBER         (188215)    // 100311
        #define PROTOCOL_ODD_NUMBER         ( 45171)
    #elif NATION_BUILD_VERSION == 905
        #define CATEGORY_ODD_NUMBER         (348117)    // 100108
        #define PROTOCOL_ODD_NUMBER         ( 75423)
    #elif NATION_BUILD_VERSION == 903
        #define CATEGORY_ODD_NUMBER         ( 78419)    // 090826
        #define PROTOCOL_ODD_NUMBER         ( 90403)
    #elif NATION_BUILD_VERSION == 902
        #define CATEGORY_ODD_NUMBER         (414419)    // 090619
        #define PROTOCOL_ODD_NUMBER         ( 86407)
    #else
        #error "Unsupported BuildVersion"
    #endif
    #ifdef _SERVER
        #define NATION_BUILD_NAME_OF_COUNTRY    "Japan"
    #else
        #define NATION_BUILD_NAME_OF_COUNTRY    "JapanClient"
    #endif
#elif defined( _AMERICA )

#elif defined( _TAIWAN )
    #if NATION_BUILD_VERSION >= 905
        #define CATEGORY_ODD_NUMBER         (718143)    // 100311
        #define PROTOCOL_ODD_NUMBER         ( 91221)
        #define __APPLY_HACKSHIELD // �ٽ��� ����
    #elif NATION_BUILD_VERSION == 904
        #define CATEGORY_ODD_NUMBER         ( 82197)    // 100312
        #define PROTOCOL_ODD_NUMBER         (102235)
        #define __APPLY_HACKSHIELD // �ٽ��� ����
    #elif NATION_BUILD_VERSION == 903
        #define CATEGORY_ODD_NUMBER         (4119)      // 091102
        #define PROTOCOL_ODD_NUMBER         (121193)
    #elif NATION_BUILD_VERSION == 902
        #define CATEGORY_ODD_NUMBER         (417717)    // 090605
        #define PROTOCOL_ODD_NUMBER         (902901)
    #else
        #error "Unsupported BuildVersion"
    #endif
    #ifdef _SERVER
        #define NATION_BUILD_NAME_OF_COUNTRY    "Taiwan"
    #else
        #define NATION_BUILD_NAME_OF_COUNTRY    "TaiwanClient"
    #endif
#elif defined( _CHINA )
    #if NATION_BUILD_VERSION >= 1204
        #define CATEGORY_ODD_NUMBER         (561531)  // 121211
        #define PROTOCOL_ODD_NUMBER         (900379)
        #define __APPLY_XIGNCODE
    #elif NATION_BUILD_VERSION >= 1203
        #define CATEGORY_ODD_NUMBER         (550329)  // 121004
        #define PROTOCOL_ODD_NUMBER         (998925)
        #define __APPLY_XIGNCODE
    #elif NATION_BUILD_VERSION >= 1202
        #define CATEGORY_ODD_NUMBER         (540427)  // 120705
        #define PROTOCOL_ODD_NUMBER         (998823)
        #define __APPLY_XIGNCODE
    #elif NATION_BUILD_VERSION >= 1201
        #define CATEGORY_ODD_NUMBER         (539425) // 120109
        #define PROTOCOL_ODD_NUMBER         (997821)
        #define __APPLY_XIGNCODE    // �����ڵ� ����
    #elif NATION_BUILD_VERSION >= 1103
        #define CATEGORY_ODD_NUMBER         (42613)  // 111222
        #define PROTOCOL_ODD_NUMBER         (62859)
        //#define __APPLY_HACKSHIELD // �ٽ��� ����
        #define __APPLY_XIGNCODE
    #elif NATION_BUILD_VERSION >= 1102
        #define CATEGORY_ODD_NUMBER         (41611)  // 101130
        #define PROTOCOL_ODD_NUMBER         (61853)
        //#define __APPLY_HACKSHIELD // �ٽ��� ����
        #define __APPLY_XIGNCODE
    #elif NATION_BUILD_VERSION >= 1101
        #define CATEGORY_ODD_NUMBER         (51315)  // 101130
        #define PROTOCOL_ODD_NUMBER         (63457)
        //#define __APPLY_HACKSHIELD // �ٽ��� ����
        #define __APPLY_XIGNCODE
    #elif NATION_BUILD_VERSION >= 1006
        #define CATEGORY_ODD_NUMBER         (62311)  // 101130
        #define PROTOCOL_ODD_NUMBER         (56453)
        #define __APPLY_HACKSHIELD // �ٽ��� ����
    #elif NATION_BUILD_VERSION >= 1005
        #define CATEGORY_ODD_NUMBER         (67323)  // 101027
        #define PROTOCOL_ODD_NUMBER         (55479)
        #define __APPLY_HACKSHIELD // �ٽ��� ����
    #elif NATION_BUILD_VERSION >= 1004
        #define CATEGORY_ODD_NUMBER         (62321)  // 100914
        #define PROTOCOL_ODD_NUMBER         (55673)
    #elif NATION_BUILD_VERSION == 1003
        #define CATEGORY_ODD_NUMBER         (66021)  // 100810
        #define PROTOCOL_ODD_NUMBER         (51679)
    #elif NATION_BUILD_VERSION == 1001
        #define CATEGORY_ODD_NUMBER         (74023)  // 100317
        #define PROTOCOL_ODD_NUMBER         (11273)
    #elif NATION_BUILD_VERSION == 904
        #define CATEGORY_ODD_NUMBER         (719423)  // 091106
        #define PROTOCOL_ODD_NUMBER         (12777)
    #else
        #error "Unsupported BuildVersion"
    #endif
    #ifdef _SERVER
        #define NATION_BUILD_NAME_OF_COUNTRY    "China"
    #else
        #define NATION_BUILD_NAME_OF_COUNTRY    "ChinaClient"
    #endif
#elif defined( _USA )
    #if NATION_BUILD_VERSION >= 1002
        #define CATEGORY_ODD_NUMBER           (369571)    // 100625
        #define PROTOCOL_ODD_NUMBER           (546793)
    #elif NATION_BUILD_VERSION == 904
        #define CATEGORY_ODD_NUMBER           (11211)    // 100201
        #define PROTOCOL_ODD_NUMBER           (87463)
    #elif NATION_BUILD_VERSION == 902
        #define CATEGORY_ODD_NUMBER           (932217)    // 090616
        #define PROTOCOL_ODD_NUMBER           (317921)
    #elif NATION_BUILD_VERSION == 815
        #define CATEGORY_ODD_NUMBER           (114407)    // 080812
        #define PROTOCOL_ODD_NUMBER           (369369)
    #else
        #error "Unsupported BuildVersion"
    #endif
    #ifdef _SERVER
        #define NATION_BUILD_NAME_OF_COUNTRY    "England"
    #else
        #define NATION_BUILD_NAME_OF_COUNTRY    "EnglandClient"
    #endif
#elif defined( _GLOBAL )
    #if NATION_BUILD_VERSION >= 1301
        #define CATEGORY_ODD_NUMBER           (514311)    // 130423
        #define PROTOCOL_ODD_NUMBER           (246789)
        #define __APPLY_XIGNCODE    // �����ڵ� ����
    #elif NATION_BUILD_VERSION >= 1204
        #define CATEGORY_ODD_NUMBER           (412279)    // 121227
        #define PROTOCOL_ODD_NUMBER           (262333)
        #define __APPLY_XIGNCODE    // �����ڵ� ����
    #elif NATION_BUILD_VERSION >= 1203
        #define CATEGORY_ODD_NUMBER           (312035)    // 121008
        #define PROTOCOL_ODD_NUMBER           (652035)
        #define __APPLY_XIGNCODE    // �����ڵ� ����
    #elif NATION_BUILD_VERSION >= 1202
        #define CATEGORY_ODD_NUMBER           (340025)    // 120423
        #define PROTOCOL_ODD_NUMBER           (650933)
        #define __APPLY_XIGNCODE    // �����ڵ� ����
	#elif NATION_BUILD_VERSION >= 1201
        #define CATEGORY_ODD_NUMBER           (220425)    // 120423
        #define PROTOCOL_ODD_NUMBER           (120423)
        //#define __APPLY_HACKSHIELD // �ٽ��� ����
        #define __APPLY_XIGNCODE    // �����ڵ� ����
    #elif NATION_BUILD_VERSION >= 1103
        #define CATEGORY_ODD_NUMBER           (211123)    // 111121
        #define PROTOCOL_ODD_NUMBER           (111121)
        //#define __APPLY_HACKSHIELD // �ٽ��� ����
        #define __APPLY_XIGNCODE    // �����ڵ� ����
    #elif NATION_BUILD_VERSION >= 1102
        #define CATEGORY_ODD_NUMBER           (210627)    // 110627
        #define PROTOCOL_ODD_NUMBER           (110625)
        #define __APPLY_HACKSHIELD // �ٽ��� ����
    #elif NATION_BUILD_VERSION >= 1101
        #define CATEGORY_ODD_NUMBER           (210329)    // 110328
        #define PROTOCOL_ODD_NUMBER           (110327)
        #define __APPLY_HACKSHIELD // �ٽ��� ����
    #elif NATION_BUILD_VERSION >= 1006
        #define CATEGORY_ODD_NUMBER           (210223)    // 110224
        #define PROTOCOL_ODD_NUMBER           (110225)
        #define __APPLY_HACKSHIELD // �ٽ��� ����
    #elif NATION_BUILD_VERSION >= 1004
        #define CATEGORY_ODD_NUMBER           (145693)    // 101111
        #define PROTOCOL_ODD_NUMBER           (698541)
        #define __APPLY_HACKSHIELD // �ٽ��� ����
    #elif NATION_BUILD_VERSION >= 1002
        #define CATEGORY_ODD_NUMBER           (956487)    // 100805
        #define PROTOCOL_ODD_NUMBER           (186491)
        #define __APPLY_HACKSHIELD // �ٽ��� ����
    #elif NATION_BUILD_VERSION == 904
        #define CATEGORY_ODD_NUMBER           (711937)    // 100303
        #define PROTOCOL_ODD_NUMBER           (295529)
    #elif NATION_BUILD_VERSION == 902
        #define CATEGORY_ODD_NUMBER           (169371)    // 091109
        #define PROTOCOL_ODD_NUMBER           (892217)
    #elif NATION_BUILD_VERSION == 815
        #define CATEGORY_ODD_NUMBER           (114407)    // 080812
        #define PROTOCOL_ODD_NUMBER           (369369)
    #else
        #error "Unsupported BuildVersion"
    #endif
    #ifdef _SERVER
        #define NATION_BUILD_NAME_OF_COUNTRY	"Global"
    #else
        #define NATION_BUILD_NAME_OF_COUNTRY	"GlobalClient"
    #endif
#elif defined( _RUSSIA )
    #if NATION_BUILD_VERSION >= 1203
        #define CATEGORY_ODD_NUMBER           (546539)
        #define PROTOCOL_ODD_NUMBER           (263439)     // 130109
        #define __APPLY_FROST
    #elif NATION_BUILD_VERSION >= 1202
        #define CATEGORY_ODD_NUMBER           (345537)
        #define PROTOCOL_ODD_NUMBER           (565433)     // 120813
        #define __APPLY_FROST
    #elif NATION_BUILD_VERSION >= 1103
        #define CATEGORY_ODD_NUMBER           (345335)
        #define PROTOCOL_ODD_NUMBER           (563431)     // 120112
        #define __APPLY_FROST
    #elif NATION_BUILD_VERSION >= 1102
        #define CATEGORY_ODD_NUMBER           (122229)
        #define PROTOCOL_ODD_NUMBER           (233455)     // 110919
        #define __APPLY_FROST
    #elif NATION_BUILD_VERSION >= 1101
        #define CATEGORY_ODD_NUMBER           (177663)
        #define PROTOCOL_ODD_NUMBER           (286541)     // 110329
        #define __APPLY_FROST
    #elif NATION_BUILD_VERSION >= 1004
        #define CATEGORY_ODD_NUMBER           (102013)
        #define PROTOCOL_ODD_NUMBER           (234451)
        #define __APPLY_FROST
    #else
        #error "Unsupported BuildVersion"
    #endif
#ifdef _SERVER
#define NATION_BUILD_NAME_OF_COUNTRY	"Russia"
#else
#define NATION_BUILD_NAME_OF_COUNTRY	"RussiaClient"
#endif
#else //
    #error "Not Defined Preprocessor -Nation Definition-"

#endif //

#if defined(__APPLY_GAMEGUARD)
#if defined(SUN1)
#define _CUSTOM_GAMEGUARD_NAME _T("SUN1CN")
#elif defined(SUN3)
#define _CUSTOM_GAMEGUARD_NAME _T("SUN3CN")
#else
//         �ƿ� ���尡 �������� ����.
#endif // SUN1
#endif // __APPLY_GAMEGUARD

//==================================================================================================
// Compiler version control
#if _MSC_VER >= 1600 // vs2010
    #define VC_DIR_REDIRECT     "v100"
#elif _MSC_VER >= 1500 // vs2008
    #define VC_DIR_REDIRECT     "v90"
#elif _MSC_VER >= 1310 // vs2003
    #define VC_DIR_REDIRECT
#else
    #pragma message("not tested on a low version complier");
    #define VC_DIR_REDIRECT
#endif
//==================================================================================================

// C/S Version History
//------------------------------------------------------------------------------------------------------------------
// 2007-11-06(ȭ) Ver 3.2.4  : 0.8.1.1 ������ ���ҽ� ������ ���� ���� : ����Ʈ ������
// 2007-01-31(��) Ver 3.2.1  : 0.8.0.8 ������ ���ҽ� ������ ���� ���� : ������ž
// 2007-01-31(��) Ver 3.2.0  : 0.8.0.7 ������ ���ҽ� ������ ���� ���� : ��Ű��/����/�����̼��߰�/����������߰�
// 2007-01-11(��) Ver 3.1.5  : 
// 2006-12-22(��) Ver 3.1.4  : ĳ�þ����� ����Ʈ 2�� �߰�
// 2006-12-19(ȭ) Ver 3.1.3  : PC�� ����ɼ� ����
// 2006-12-15(��) Ver 3.1.2  : ��ȹ��ũ��Ʈ ����, ���� ����ȭ ���÷α� ����
// 2006-12-14(��) Ver 3.1.1  : [SUN ������ ���ȭ ���۹���] ������ ���ȭ ���� ����, ���λ��� ����Ʈ �����
// 2006-12-12(ȭ) Ver 2.9.1  : ���ȭ 2�� ������ ����ȭ ���� 2�� ������ ����(1�� ���� �⺻���� ���� ��������)
// 2006-12-10(��) Ver 2.8.15 : ���ȭ 2�� ������ ����ȭ ���� 1�� ������ ����
// 2006-11-29(��) Ver 2.8.14 : ī������Ʈ �߰�, ä�� ���� 1��, AC�ý��� Ȯ��
// 2006-11-14(ȭ) Ver 2.8.13 : ���λ��� ������ ���� ����
// 2006-10-30(��) Ver 2.8.12 : �űԹ̼� �߰�, ���ӳ� ���� ���ȭ 1��
// 2006-10-10(ȭ) Ver 2.8.11 : �̼Ƿ�ŷ üũ ��ȭ, 300% ����ġ �̺�Ʈ
// 2006-09-29(��) Ver 2.8.10 : �̼Ƿ�ŷ ���� ���� ����ȭ
// 2006-09-06(��) Ver 2.8.9  : �̼Ƿ�ŷ
// 2006-08-23(��) Ver 2.8.8  : �� �̵��� �����������, �ʵ� �����丵, wmo�и��۾� �Ϸ�
// 2006-08-16(��) Ver 2.8.7  : �������� PC�� �̺�Ʈ, ģ�������� ó��
// 2006-08-08(ȭ) Ver 2.8.6  : PK ������
// 2006-08-01(ȭ) Ver 2.8.5  : ��������Ʈ �۾�
// 2006-07-25(ȭ) Ver 2.8.4  : �������� ����ȭ �� ���¡ ����ó��, ä��ä�� ���
// 2006-07-19(��) Ver 2.8.3  : �̺�Ʈ ���� DB����, Ʈ���� ������Ʈ ���� ������� ����, �������� ���� �۾�
// 2006-07-11(ȭ) Ver 2.8.2  : ģ������ ����Ʈ �ִ밪 ���� ����
// 2006-06-29(��) Ver 2.8.1  : ���� ���� �۾�
// 2006-06-22(��) Ver 2.7.2  : ��ƼMPâ ���� ����, ��ų ���� ����, ������ ó��
// 2006-06-13(ȭ) Ver 2.7.1  : �������� �ڵ���Ī ���, ������ ���� ���, ��� �� ģ�� ������ �ǽð� �������
// 2006-06-01(��) Ver 2.6.3  : �̺�Ʈ ������ ���� ����, ������ ������� ���� ����, ���ä�� ���� ����
// 2006-05-29(��) Ver 2.6.2  : ���º�Ÿ �� ù ��������, ���ӷα� ���� �� ��� �߰�, FSM���� �߻��ϴ� �� ����
// 2006-05-23(ȭ) Ver 2.6.1  : [���º�Ÿ ���� ����]
// 2006-05-22(��) Ver 2.5.6  : [StressTest ������ ����]������ ó�� �� �������, ������ ������
// 2006-05-19(��) Ver 2.5.5  : ������ ���� ��Ŷ �߰�, ��Ƽ ����ġ ���� ����, ���� ���� ���Ȱ ���� ����
// 2006-05-15(��) Ver 2.5.4  : �������� ������ġ �ٸ��� ó��, �ʵ� ����� ������ġ ��Ȱ, ��Ÿ ���� ����
// 2006-05-10(��) Ver 2.5.3  : ���� ��ȯ ���� ����, GM��ɾ� ���� ����
// 2006-05-08(��) Ver 2.5.2  : [StressTest ���� ����] ����Ʈ ���� ����, �ִ뷹���̻� ������ ���� ����
// 2006-05-07(��) Ver 2.5.1  : ��ũŸ�� �̺�Ʈ, �������� �߰� ���, GM��� �������� �޾ƿ�
// 2006-05-05(��) Ver 2.4.3  : [E3 ����] PVP ����ȭ ���� ����, �̼� �������ġ ���� ���� 
// 2006-05-04(��) Ver 2.4.2  : â������ ���� ����, �������� ���� ��û ���� �ݿ�����
// 2006-05-03(��) Ver 2.4.1  : �̼Ƿ�ŷ ���� DBó��, ������ ����ȭ ���� ����
// 2006-05-02(ȭ) Ver 2.3.8  : ���� ���� ����(������,���,�ʵ��̵�,�̼� ���)
// 2006-04-29(��) Ver 2.3.7  : ���������� Ʈ�������� �и� �۾�, �������� Death Match Mode
// 2006-04-26(��) Ver 2.3.6  : Ŭ���̾�Ʈ�� �ְ�޴� �⺻ ��Ŷ(MSG_FORWARD)���� DWORD�� ����
// 2006-04-24(��) Ver 2.3.5  : �ֱ����� ��Ŷ�ø��� ���� ���
// 2006-04-21(��) Ver 2.3.4  : ��ĪDBó�� �߰�, PVP ��Ƽ ���� ����, ���� ����Ʈ
// 2006-04-19(��) Ver 2.3.3  : AC�ý��� �߰�
// 2006-04-18(ȭ) Ver 2.3.2  : �̼Ǹ� �׽�Ʈ �Ϸ� ����
// 2006-04-17(��) Ver 2.3.1  : ����ø��� �������� �ٲٱ� ��� �߰�
// 2006-04-14(��) Ver 2.2.5  : ��� �ʵ� ����ġ �и�, ������DBũ�� ����
// 2006-04-11(ȭ) Ver 2.2.4  : ���� ������Ʈ ���� ���� ����, �������ÿ� ���� �������� 1�� �Ϸ�, ������ ��� ���� ���� 
// 2006-04-10(��) Ver 2.2.3  : ���� ������/���� ������ �� ����, ���� ���� �̵� �������� ����
// 2006-04-07(��) Ver 2.2.2  : ���� ������/���� ������ ����, �������� 1��,���� ������ ��ġ ����, ����Ʈ ����ȭ ����
// 2006-04-04(ȭ) Ver 2.2.1  : ����ä��, ����Ʈ 1�� �Ϸ�, ��Ƽ�� ��ó�� ���� �� ��ȯ, �ʵ� ��ɸ� ó������
// 2006-03-31(��) Ver 2.1.7  : ĳ���� ���� ���, ��Ƽ��Ƽ�� ���� ����ġ �� ������ �й�, ������ ������ ��� ����
// 2006-03-21(ȭ) Ver 2.1.6  : ������ Cycle �ϼ�, ��Ŷ��ȣȭ ����, ��Ƽ��Ƽ ���, �̿��� ���� �ý��� �߰� ����
// 2006-03-09(��) Ver 2.1.5  : ��Ƽ��Ƽ �������� �߰�, ���̺귯��ȭ �۾�, SUN ��Ŷ ��ȣȭ ��� �߰� ����
// 2006-02-21(ȭ) Ver 2.1.4  : �⺻ �Ķ���� ����, �Ӽ� ������ ����, ���� ��Ŷ ����ȭ ����
// 2006-02-15(��) Ver 2.1.3  : ������ ���� ��Ŷ ����ȭ �۾�
// 2006-01-25(��) Ver 2.1.2  : [PreOpen ������ ����]�ִ� Join�ο� ���� ����, �ִ� �氹�� ���� ���, ���λ��� ���� ���� ����
// 2006-01-24(ȭ) Ver 2.1.1  : [PreOpen ���۹���]RetreatState ����ϰ� ����Ǵ� ���� ����, ���������� ���� ���� 
// 2006-01-23(��) Ver 1.5.29 : ĳ�� ����� HP/MP���� �ȵǴ� ���׼���,�����Ƽ �ε��� ���� ����,���������� Join���� ��������
// 2006-01-23(��) Ver 1.5.28 : ĳ���� ���� �ȵǴ� ���� ����, PVP������ DB��� �κ� �߰�, ���� ��Ÿ��� ���ݹ��� ��������
// 2006-01-22(��) Ver 1.5.27 : ��Ƽ�� �������� ���� ����, ���� ���� ��Ŷ ���� ���� ���� 
// 2006-01-20(��) Ver 1.5.26 : AI BattleRecode ���� ����, �ŷ��� ���� ����, Ʈ���� ���� ����, �̺�Ʈ ������ ó�� ����
// 2006-01-19(��) Ver 1.5.25 : �����̵� ���� ����, Ʈ���� ���� ����, ���弭�� ������� ���� ó�� ����
// 2006-01-18(��) Ver 1.5.24 : �������� ���ӷα׸�⿡�� �״� ���� �ּ� ó��, �κ���� �������� ��������
// 2006-01-16(��) Ver 1.5.23 : ĳ������ ������� ��ε�ĳ����, �������� ũ��Ƽ�� ��Ŷ, ������ �� �⺻���ĵ� ��������
// 2006-01-13(��) Ver 1.5.22 : ��ų�����Ƽ �������� ����, �����̵��� ���� ���� ��ġ ����, GM��Ʈ�� ó�� ����
// 2006-01-11(��) Ver 1.5.21 : ��ų�ڵ� ü�� ����, ������ ���� ��ȹ ������ �ݿ�, �̼� ���� ��ũ��Ʈ ó�� ����
// 2006-01-05(��) Ver 1.5.20 : ��� �ɼ�ȭ ���, ĳ�������� ���� ���� ��Ŷ �߰�, ���ڷ��� ���� �߰� ����  
// 2006-01-04(��) Ver 1.5.19 : ������ �̵�, ĳ���� �Ӹ�/��/Ű Customize ����
// 2006-01-02(��) Ver 1.5.18 : ��� ���� �� ����, PC�� ����ġ ���ó��, ��Ŷ 3�� ����ȭ ����
// 2005-12-23(��) Ver 1.5.17 : ģ�� �������� ����, ���DB, ��ȯü ���� ���� ����
// 2005-12-16(��) Ver 1.5.16 : ��ȯü ��ų, ������ ���ù�, ������ �ø��� �ߺ� ó�� ����
// 2005-12-07(��) Ver 1.5.15 : PVP�����, ����ä�� ����Ʈó��, �ɱ�/����, ���� ũ��Ƽ��, �ǽð� ��� ó�� ����
// 2005-11-22(ȭ) Ver 1.5.14 : ĳ���� �̸��, GM Ŭ���̾�Ʈ���� ���� ������ ����
// 2005-11-17(��) Ver 1.5.13 : �⺻ PVP����, ���� �̼�/���� ó�� ����
// 2005-11-13(��) Ver 1.5.12 : [Gstar ������ ����] ���� ��Ȱ��ų ���� ����, ��ŷ� ���� ���� ����
// 2005-11-11(��) Ver 1.5.11 : ������ ����� �� �ȵǴ� ���� ����, �κ񿡼� ��Ƽ�� ���� ���� ����
// 2005-11-10(��) Ver 1.5.10 : GM �Ӹ����� ���, ����ξ����� �� ���� ����, Ʈ���� ���� ���� ���� ����
// 2005-11-09(��) Ver 1.5.9  : [GStar ���� Version] �߰����� ���� ó��, ������ �Ǹ� ���� ���� ����
// 2005-11-08(ȭ) Ver 1.5.8  : ���� ���� �߰� ������ ���, ���� ������ġ ����, ĳ���� Stat ���� Minus(-)�� ó�� ���� 
// 2005-11-07(��) Ver 1.5.7  : �⺻����/���ݰ��� ����, ȸ����ų ���� ���� ����
// 2005-11-02(��) Ver 1.5.6  : ���� ��ų(���,�ٿ�), ��Ű�� ���÷��� ������ ó��, ���弭�� ������ ó�� ����
// 2005-10-30(��) Ver 1.5.5  : [Ŭ���� ��Ÿ 2�� ������ ����] ��Ƽ ����ġ �߰� ���� ���� ����
// 2005-10-29(��) Ver 1.5.4  : ���������� �̻��� ���� ���� ���� �߰�
// 2005-10-28(��) Ver 1.5.3  : SUNLog ���� �Ÿ��°� ����, ��Ƽ �߸� üũ�ε� ���� ����, ����� ������ Ȯ�� ���� ����
// 2005-10-27(��) Ver 1.5.2  : ä�δ� ���ȣ ���Ҵ� ���ϴ� ���� ����, ��ø ���� �����÷ο� ����, �ӼӸ� ó�� ����
// 2005-10-25(ȭ) Ver 1.5.1  : [Ŭ���� ��Ÿ 2�� ���۹���]
// 2005-10-24(��) Ver 1.4.16 : ĳ���� ���� ��Ŷ ����, ���� ��Ȳ ���� ����, DB ServerInfo����
// 2005-10-23(��) Ver 1.4.15 : ��Ʋ������ �� ���� ���� ����, �������� �ɾ �ڵ� ���� 
// 2005-10-22(��) Ver 1.4.14 : ����ó��, ������ ��ų ���� ����, ���� ���� ���ҹ��� ����, ���ӷα�
// 2005-10-18(ȭ) Ver 1.4.12 : ���ο� �������� �߰� �� ����(��Ƽ Ÿ����, �ٷΰ��� ��)
// 2005-10-14(��) Ver 1.4.11 : ���ο� �������� �߰� �� ����, Ŭ�� 1�� ���� ����Ʈ ��������
// 2005-10-01(��) Ver 1.4.9  : Ŭ���̾�Ʈ ��ġ�� �ϱ� ���� ���� �ø�(�������� ������ ���ҽ� ��ġ ������ �и� �ʿ���)
// 2005-09-30(��) Ver 1.4.8  : â�� ��� ����, Ŭ���� ��Ÿ ��°���� �߰ߵ� ���׵� ����
// 2005-09-29(��) Ver 1.4.7  : Ŭ���� ��Ÿ 1�� ��°���� �߰ߵ� ���׵� ����
// 2005-09-28(��) Ver 1.4.6  : [Ŭ���� ��Ÿ 1�� ���� ���� ] ù���� �߰ߵ� ���׵� ����
// 2005-09-25(��) Ver 1.4.5  : ��Ŷ ����ȭ �۾����� ��Ŷ Size����, ������ �������� ����, ���� ��ų ����, ���ҽ� ����
// 2005-09-24(��) Ver 1.4.4  : ��Ŷ ����ȭ �۾����� ��Ŷ ����, ���ҽ� ����
// 2005-09-23(��) Ver 1.4.3  : �� ���� ���濡 ���� ��Ƽ���� �������� ����
// 2005-09-22(��) Ver 1.4.2  : ĳ���� �������� �������� ����
// 2005-09-21(��) Ver 1.4.1  : ����� ���� ����, �α���/���� �������� ����, �� �����, ��ų ���� ����
// 2005-09-17(��) Ver 1.3.5  : �� �����(ȭ��� �߰�), ���� ��ų 1�� ����, ���� ���� ����, ��ų ���� ����
// 2005-09-11(��) Ver 1.3.4  : ��þƮ ���� ����, ��ų/��Ÿ�� �ļ� ����, ��Ƽ���� ����ġ ���� ����
// 2005-09-08(��) Ver 1.3.3  : ��ų���� ����, ��Ÿ�� ������ ���� ����, �̼� ���� ����
// 2005-09-07(��) Ver 1.3.2  : ���� ���Ž� ���� ����, ������ ������ ���� ����, �̼Ǹ� Ʈ���� ����, ��ų ���� ����
// 2005-09-06(ȭ) Ver 1.3.1  : �� ����� �� �������� ����(������)
// 2005-09-05(��) Ver 1.2.1  : �� ����� ����, �ʹ����� �ٸ� ��� �������� �ʵ������� ������� ����
// 2005-09-02(��) Ver 1.1.1  : ���� üũ ����
//------------------------------------------------------------------------------------------------------------------


#endif // __VERSION_H__

