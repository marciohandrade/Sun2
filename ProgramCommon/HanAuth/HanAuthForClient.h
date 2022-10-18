/**
* @brief HanAuthForClient.h
* This file declares Publishing Authentication Client Module api
* 
* Copyright (c) 2008 NHN Corporation. All Rights Reserved.
*
* @author		: nhn
* @date			: 2008-08-08
*/

#pragma once

#ifdef HANAUTHFORCLIENT_EXPORTS
	#define HANAUTH_API __declspec(dllexport)
#else
	#define HANAUTH_API __declspec(dllimport)
	#ifdef _DEBUG
		#pragma comment(lib, "../../lib/HanAuthForClientD.lib") 
	#else
		#pragma comment(lib, "../../lib/HanAuthForClient.lib") 
	#endif
#endif

// pre define value
//////////////////////////////////////////////////////////////////////////
#define SIZE_AUTHSTRING			2048
#define SIZE_GAMESTRING			4096

#define SERVICE_NATION			0x000000FF
#define SERVICE_KOR				0x00000001
#define SERVICE_USA				0x00000002
#define SERVICE_JPN				0x00000003
#define SERVICE_CHN				0x00000004

#define SERVICE_TYPE			0x00000F00
#define SERVICE_ALPHA			0x00000100
#define SERVICE_REAL			0x00000200
#define SERVICE_BETA			0x00000300

#define SERVICE_SITE			0x0000F000
#define SERVICE_HANGAME			0x00001000
#define SERVICE_ASOBLOG			0x00002000
#define SERVICE_IJJI			0x00003000
#define SERVICE_GAMETAP			0x00004000

#define REFRESH_MAXFAIL			3			// The number of consecutive failures after which clients will stop to refresh
#define USA_REFRESH_RET_VARS	4			// The number of variables in the string returned after cookie refresh in USA
//////////////////////////////////////////////////////////////////////////


// ACM api global return code
//////////////////////////////////////////////////////////////////////////
#define HAN_AUTHCLI_OK								0
#define HAN_AUTHCLI_ARGUMENT_INVALID				-1
#define HAN_AUTHCLI_INITED_ALREADY					-2
#define HAN_AUTHCLI_INITED_NOT						-3
#define HAN_AUTHCLI_INITED_FAIL						-4
#define HAN_AUTHCLI_AUTHHTTP_INITFAIL				-5
#define HAN_AUTHCLI_AUTHHTTP_CONNFAIL				-6
#define HAN_AUTHCLI_REFRESHHTTP_INITFAIL			-7
#define HAN_AUTHCLI_REFRESHHTTP_CONNFAIL			-8
#define HAN_AUTHCLI_NOT_IMPLEMENTED					-9
#define HAN_AUTHCLI_AUTHHTTP_OPENREQFAIL			-10
#define HAN_AUTHCLI_AUTHHTTP_SENDREQFAIL			-11
#define HAN_AUTHCLI_COOKIE_SETFAIL					-13
#define HAN_AUTHCLI_GAMESTRING_IDINVALID			-14
#define HAN_AUTHCLI_GAMESTRING_USNINVALID			-15
#define HAN_AUTHCLI_GAMESTRING_GAMEIDINVALID		-16
#define HAN_AUTHCLI_GAMESTRING_SERVICECODEINVALID	-17
#define HAN_AUTHCLI_CREATEINTANCEFAIL				-18
#define HAN_AUTHCLI_DLL_UNEXPECTED					-100

#define HAN_AUTHCLI_INVALID_BILLINGNO				-101

#define HAN_AUTHCLI_PARAM_INVALID					1
#define HAN_AUTHCLI_IPADDR_INVALID					2
#define HAN_AUTHCLI_MEMBERID_INVALID				3
#define HAN_AUTHCLI_PASSWORD_INCORRECT				4
#define HAN_AUTHCLI_PASSWORD_MISMATCHOVER			5
#define HAN_AUTHCLI_MEMBERID_NOTEXIST				6
#define HAN_AUTHCLI_SYSTEM_ERROR					7
#define HAN_AUTHCLI_COOKIE_SETERROR					8
//////////////////////////////////////////////////////////////////////////

#if defined(__cplusplus)
	extern "C"{
#endif

/**
* @fn		HanAuthInit()
* @brief	initialize module
* 
* @param	szGameId			Unique ID of a game
*			nServiceCode		Service code
*/
HANAUTH_API int __stdcall HanAuthInit(IN char* szGameId,
									  IN int nServiceCode);
/**
* @fn		HanAuthInitGameString()
* @brief	initialize module
* 
* @param	szGameString		GameString
*/
HANAUTH_API int __stdcall HanAuthInitGameString(IN char* szGameString);

/**
* @fn		HanAuthForClientGameString()
* @brief	A method that receives GameString and saves it within the ACM
* 
* @param	szGameString		GameString
*/
HANAUTH_API int __stdcall HanAuthForClientGameString(IN char* szGameString);

/**
* @fn		HanAuthRefresh()
* @brief	Refreshes AuthString saved within the Authentication Module
* 
* @param	szMemberId			User ID
*/
HANAUTH_API int __stdcall HanAuthRefresh(IN char* szMemberId);

/**
* @fn		GetAuthString()
* @brief	Obtains AuthString saved within the Authentication Module
* 
* @param	szAuthString		Buffer for AuthString to be received
*			nSize				Size of buffer for AuthString to be received		
*/
HANAUTH_API int __stdcall GetAuthString(OUT char* szAuthString, 
										IN int nSize);

/**
* @fn		UpdateGameString()
* @brief	Updates GameString using the recently refreshed AuthString.
*			it is called before PURPLE Launcher delivers GameString to the Game Client.
* 
* @param	szOldGameString		GameString before update
*			szNewGameString		Buffer where GameString to be updated is saved
*			nSize				Size of GameString buffer
*/
HANAUTH_API int __stdcall UpdateGameString(IN char* szOldGameString, 
										   OUT char* szNewGameString,
										   IN int nSize);

/**
* @fn		HanAuthGetId()
* @brief	Extracts user ID from GameString
* 
* @param	szGameString		Buffer for AuthString to be received
*			szMemberId			Size of buffer for AuthString to be received
*			nsize_id			Size of buffer for user ID to be received (Maximum size is 21 bytes.)
*/
HANAUTH_API int __stdcall HanAuthGetId(IN char* szGameString,
									   OUT char* szMemberId,
									   IN int nsize_id);

/**
* @fn		HanAuthGetNationCode()
* @brief	Extracts the nation code from GameString; the nation code comprises the followings:
*			?	SERVICE_KOR: Korea 
*			?	SERVICE_USA: United States
*			?	SERVICE_JPN: Japan
*			?	SERVICE_CHN: China
* 
* @param	szGameString		GameString
*			pNationCode			Buffer for nation code to be returned
*/
HANAUTH_API int __stdcall HanAuthGetNationCode(IN char* szGameString,
											   OUT int* pNationCode);

/**
* @fn		HanAuthGetServiceTypeCode()
* @brief	Extracts the service type code from GameString;
*			the service type is a parameter that distinguishes whether it is the test environment or the live environment.
*			The service type code is as follows:
*			?	SERVICE_ALPHA: Alpha test environment
*			?	SERVICE_REAL: Official (live) service environment
* 
* @param	szGameString		GameString
*			pServiceTypeCode	Buffer for service type code to be returned (test/live)
*/
HANAUTH_API int __stdcall HanAuthGetServiceTypeCode(IN char* szGameString,
													OUT int* pServiceTypeCode);

/**
* @fn		HanAuthGetSiteCode()
* @brief	Extracts the site service code from GameString;
*			it is a code to correspond to multiple sites within a country and it comprises as follows:
*			?	SERVICE_HANGAME : Hangame
*			?	SERVICE_ASOBLOG : Adult site
* 
* @param	szGameString		GameString
*			pSiteCode			Buffer for site service code to be returned
*/
HANAUTH_API int __stdcall HanAuthGetSiteCode(IN char* szGameString,
											 OUT int* pSiteCode);

/**
* @fn		HanAuthGetValue()
* @brief	Extracts the value from GameString; the value is the corresponding variable specified by user.
* 
* @param	szGameString		GameString
*			szValueName			Name of variable that user wants to receive
*			pBuffer				Buffer for the variable value to be queried
*			nSize				Size of buffer for variable value to be queried
*/
HANAUTH_API int __stdcall HanAuthGetValue(IN char* szGameString,
										  IN char* szValueName,
										  OUT char* pBuffer,
										  IN int nSize);

/**
* @fn		HanAuthGetUSN()
* @brief	Extracts the user billing number from GameString; this method is used only in the U.S
* 
* @param	szGameString		GameString
*/
HANAUTH_API int __stdcall HanAuthGetUSN(IN char* szGameString);

/**
* @fn		HanAuthGetBillUserNo()
* @brief	Extracts the user billing number from GameString; this method is used only in Japan.
* 
* @param	szGameString		GameString
*			szUserNo			Buffer for user billing number to be received
*			nSize				Size of buffer for user billing number to be received
*/
HANAUTH_API int __stdcall HanAuthGetBillUserNo(IN char* szGameString,
											   OUT char* szUserNo,
											   IN int nSize);

/**
* @fn		HanAuthMiniExplorer()
* @brief	A method that minimizes the size of Internet Explorer (IE) or restores the minimized IE;
*			in restoring the size of IE to the original size,
*			the web page address (static page URL) of the current IE is returned to the main page URL.
* 
* @param	bShow				Specifies the value for the size of IE
*								?	FALSE: Minimizes all the IE windows
*								?	TRUE: Restores the minimized IE windows 
*/
HANAUTH_API int __stdcall HanAuthMiniExplorer(bool bShow);

/**
* @fn		HanAuthMarketingTag()
* @brief	no use method
* 
* @param	szGameString		GameString
*/
HANAUTH_API int __stdcall HanAuthMarketingTag(IN char* szGameString);

/**
* @fn		HanForceAuthRefresh()
* @brief	Refreshes AuthString saved within the Authentication Module immediately
* 
* @param	szMemberId			User ID
*/
HANAUTH_API int __stdcall HanForceAuthRefresh(IN char* szMemberId);

/**
* @fn		HanAuthForClientDirect()
* @brief	Generates and saves AuthString when the game is run directly on the Client
* 
* @param	szMemberId			User ID
*			szPassword			User password
*
* @return	(only Korea)
*			0		OK
*			-102	한게임플러스요금미납으로 로그인이 불가능합니다.
*			-1		존재하지 않는 아이디이거나 잘못된 아이디입니다.
*			1  		회원님의 아이디, 비밀번호를 다시 한번 확인해 주세요. 
*			5		입력하신 한게임아이디로 등록되어 있는 주민등록번호와 이름이 일치하지 않습니다.
*			33		회원님의 비밀번호가 3회 이상 다르게 입력되었습니다. 
*
*			11		귀하는 불량이용자로 판명되어 아이디가 삭제되었습니다.
*			12		귀하는 불량이용자로 판명되어 아이디가 일시정지되었습니다.
*			13		귀하는 불량이용자로 판명되어 아이디가 90일간 이용정지후 삭제처리 될 예정입니다.
*
*			14		지금 접속하신 아이디는 해지신청 처리중인 아이디입니다.
*			15		지금 접속하신 아이디는 해지/삭제된 아이디입니다.
*
*			16		지금 접속하신 아이디는 등록 처리중인 아이디입니다.	
*					본인임을 확인 할 수 있는 주민등록증, 운전면허증, 학생증 사본을 \n
*					팩스[ 02-6008-5487 ]로 보내주시면 확인후 정식등록 처리가 됩니다.
*
*			17		부모님 동의 메일이 아직 도착하지 않거나 부모님의 동의를 받지 못했습니다.\n
*					지금 접속하신 아이디는 14세 미만 아이디입니다.\n
*					14세 미만 어린이의 경우 부모님의 이메일 주소로 확인이 보내지고\n
*					부모님이 동의를 한 경우에만 로그인이 가능합니다.
*			18		지금 접속하신 아이디는 정상적인 아이디가 아닙니다.
*			19		14세 미만 가입자 - 부모 동의 미확인상태입니다.\n
*					한게임메인에서 로긴하시기 바랍니다.
*			20		14세 미만 가입자 - 부모 동의 확인상태입니다.\n
*					한게임메인에서 로긴하시기 바랍니다.
*
*/										   
HANAUTH_API int __stdcall HanAuthForClientDirect(IN char* szMemberId, 
												 IN char* szPassword);

/**
* @fn		HanAuthRegisterWebPageURL()
* @brief	Registers the main page URL and static page URL of publishing games;
*			uses the URL information registered here to execute the address transition of the HanAuthMiniExplorer() method.
*			If this method is called several times for the same game, the last called URL is set.
* 
* @param	szMainPageURL		Address of the Game Web Page (main page URL)
*			szStaticPageURL		Address of the Game Web Page (static page URL)
*/
HANAUTH_API int __stdcall HanAuthRegisterWebPageURL(IN const char* szMainPageURL, 
													IN const char* szStaticPageURL);

#if defined(__cplusplus)
	}
#endif
