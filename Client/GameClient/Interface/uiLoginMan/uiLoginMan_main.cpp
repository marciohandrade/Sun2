//------------------------------------------------------------------------------
//  uiLoginMan_main.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiLoginMan.h"

#include <PacketStruct_CG.h>

#include "uiLoginBack/uiLoginBack.h"
#include "uiLoginBack/uiLoginBackTop.h"
#include "uiLoginAccount/uiLoginAccount.h"
#include "uiLoginServerList/uiLoginServerList.h"
#include "uiLoginChannelList/uiLoginChannelList.h"
#include "uiLoginServerChannelList/uiLoginServerChannelList.h"

#include "Version.h"
#include "LoginScene.h"
#include "LoadScene.h"
#include "../../ProgramCommon/PacketCrypt.h"
#pragma  comment(lib, "../../Lib/"VC_DIR_REDIRECT"/CryptLib.lib")

#include "interfacemanager.h"
#include "globalfunc.h"
#include "uiSystemMan/uiSystemMan.h"

#if defined(_DEBUG) || defined(_GAMECLIENT_VER) 
#include "AutoTestManager.h"
#endif

#ifdef __CN_269_MATRIX_CARD_AUTH
	#include "MatrixCardAuth.h"
#endif
#if defined(_JP000000_ONETIMEPASSWORD_AUTHFLOW_) || defined(__CN_20100909_SECURITY_TOKEN__)
#include "uiLoginOneTimePassword/uiLoginOneTimePassword.h"
#endif //_JP000000_ONETIMEPASSWORD_AUTHFLOW_ __CN_20100909_SECURITY_TOKEN__

#ifdef _NHN_USA_CHANNELING
	#include <HanAuth/HanReportForClient.h>	//지표모듈
	#include <HanAuth/HanAuthForClient.h>	//인증모듈
#endif//_NHN_USA_CHANNELING(북미채널링)

#include "uiAssistMan/uiAssistMan.h"

ParameterAuthInfo uiLoginMan::parameter_auth_info;

int uiLogineAsigned::is_show_count_Account_ = 0;
int uiLogineAsigned::is_show_coint_ServerList_ = 0;
//------------------------------------------------------------------------------
/**
*/
uiLoginMan::uiLoginMan(InterfaceManager *pUIMan) :
    uiBaseMan(pUIMan)
{
    m_pUILoginBack = NULL;
#ifdef _COMMON_CHANNELING_LOGIC_DEFINE
    // no operation
#else
	m_pUILoginAccount = NULL;
#endif //_COMMON_CHANNELING_LOGIC_DEFINE
    m_pUILoginServerList = NULL;

    m_pUILoginBackTop = NULL;

#if defined(_JP000000_ONETIMEPASSWORD_AUTHFLOW_) || defined(__CN_20100909_SECURITY_TOKEN__)
    m_pUILoginOneTimePassword = NULL;
#endif //_JP000000_ONETIMEPASSWORD_AUTHFLOW_ __CN_20100909_SECURITY_TOKEN__

    m_bAuthFinished = false;
    m_bTryReconnect = false;
    m_TryCount = 0;    
	
	m_pCurrentConnectedServerData = NULL;
	m_bRecvGroupData	= false;
	m_bRecvServerData	= false;

    m_strTryID[0] = '\0';
    m_strTryPW[0] = '\0';
#ifdef _APPLY_KOREA_CHANNELING
	ZeroMemory(&m_Auth_Info, sizeof(m_Auth_Info));

#endif//_APPLY_KOREA_CHANNELING

#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
	ZeroMemory(&m_ReAuth_Info, sizeof(m_ReAuth_Info));
	m_bServerSelect = false;
#endif //__NA001334_090420_SERVER_SELECT_FACILITY__

#ifdef _NA_20100307_BETAKEY_SYSTEM
    m_betakey_info = static_cast<UINT64>(0);
#endif//_NA_20100307_BETAKEY_SYSTEM

#ifdef _COMMON_CHANNELING_LOGIC_DEFINE
	// no operation
	m_bServerListInfo = true;
#else
	m_bServerListInfo = false;
#endif
}

void
uiLoginMan::OnInitialize()
{
    _LoadUI();
}

//------------------------------------------------------------------------------
/**
*/
void
uiLoginMan::OnRelease()
{
	DeleteCurrentServerData();
}

void uiLoginMan::HideAllDialogsRelease()
{
#ifdef _COMMON_CHANNELING_LOGIC_DEFINE
#else
	if(m_pUILoginAccount != NULL)
	{
		m_pUILoginAccount->ShowInterface(FALSE);
	}
#endif

	if(m_pUILoginServerList != NULL )
	{
		m_pUILoginServerList->ShowInterface(FALSE);
	}

}

//------------------------------------------------------------------------------
/**
*/
void
uiLoginMan::Clear()
{
    InitPKBooleans();

#ifdef _COMMON_CHANNELING_LOGIC_DEFINE
    // no operation
	m_bServerListInfo = true;
#else
	m_bServerListInfo = false;
	m_pUILoginAccount->ClearAll();
#endif //_COMMON_CHANNELING_LOGIC_DEFINE
#if defined(_JP000000_ONETIMEPASSWORD_AUTHFLOW_) || defined(__CN_20100909_SECURITY_TOKEN__)
    m_pUILoginOneTimePassword->SetOTPUserStamp(uiLoginOneTimePassword::NoneStamp);
    m_pUILoginOneTimePassword->ShowInterface(FALSE);
#endif //_JP000000_ONETIMEPASSWORD_AUTHFLOW

    //------------------------------------------------------------------------------ 
	//pk안내 관련 팝업 창 끄기	
	uiAssistMan * assist_man = GET_DIALOG_MANAGER(uiAssistMan, UIMAN_ASSIST);
    if (assist_man != NULL)
    {
        assist_man->ShowAskPkPopupDialog(FALSE, 0, 0);
        assist_man->ShowAskPkPopupDialog(FALSE, NULL);
    }

    //------------------------------------------------------------------------------ 
	m_AuthGroupData.clear();
	m_bRecvGroupData	= false;
	m_bRecvServerData	= false;
}

//------------------------------------------------------------------------------
/**
*/
void
_fn_uiLoginMan_toolong_id()
{
	uiLoginAccount* loginAccount = GET_CAST_DIALOG( uiLoginAccount, IM_LOGIN_MANAGER::LOGIN_ACCOUNT );

    assert (loginAccount);
    loginAccount->ShowInterface(FALSE);
    loginAccount->ShowInterface(TRUE);
    loginAccount->SetFocusID(false);
    loginAccount->ConnectBtnOn();
}

//------------------------------------------------------------------------------
/**
*/
bool
uiLoginMan::execLoginQuery(TCHAR* strID, TCHAR* strPW)
{
    TCHAR	szMessage[MAX_MESSAGE_LENGTH];

    Sprintf (m_strTryID, _T("%s"), strID);
    Sprintf (m_strTryPW, _T("%s"), strPW);

    int length = (int)lstrlen(strID);
	
    if( length > MAX_EDIT_IDLEN )
    {
		//	아이디는 10글자 이내로 제한됩니다 !!
		g_InterfaceManager.GetInterfaceString( eST_ID_LENGTH_IS_10, szMessage, INTERFACE_STRING_LENGTH );

		uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
		if(pSystemMan)
			pSystemMan->Sys_VerifyLock(szMessage, _fn_uiLoginMan_toolong_id);

        return false;
    }

    // toupper account id
    for( int i = 0 ; i < length ; ++i )
    {
        char ch = m_strTryID[i];
        m_strTryID[i] = toupper(ch);
    }

    if( GENERALPARAM->IsNet() )
    {
        // net
		return SEND_AUTH_ID_PASSWORD_CHECK_SYN();
    }
    else
    {
        // single mode		
        g_LoadScene.SetNextSceneAfterLoaded(SCENE_TYPE_CHARSELECT, GENERALPARAM->GetCharSelectMapID(), 0 );
        GameFramework::ChangeScene( SCENE_TYPE_LOAD );
    }

    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool
uiLoginMan::autoexecLoginQuery()
{
    if( GENERALPARAM->IsNet() )
    {
        // net
		return this->SEND_AUTH_ID_PASSWORD_CHECK_SYN();
    }
    else
    {
        // single mode
        g_LoadScene.SetNextSceneAfterLoaded(SCENE_TYPE_CHARSELECT, GENERALPARAM->GetCharSelectMapID(), 0 );
        GameFramework::ChangeScene( SCENE_TYPE_LOAD );
    }

    return true;
}

//------------------------------------------------------------------------------
/**
*/
void _fn_uiLoginMan_Register(bool bYes)
{
	if (bYes)
	{
		uiLoginAccount* loginAccount = GET_CAST_DIALOG( uiLoginAccount, IM_LOGIN_MANAGER::LOGIN_ACCOUNT );
		assert (loginAccount);
		loginAccount->ShowInterface(FALSE);
		loginAccount->ShowInterface(TRUE);
		loginAccount->ClearAll();
		loginAccount->SetFocusID();
		loginAccount->ConnectBtnOn();
	}
	else
	{
		TCHAR szREGISTER_URL[512];
		// 마이폴더 홈페이지에 접속하렴
		SHELLEXECUTEINFO ShExecInfo;

		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		ShExecInfo.fMask = NULL;
		ShExecInfo.hwnd = g_hWndMain;
		ShExecInfo.lpVerb = NULL;
		g_InterfaceManager.GetInterfaceString(989, szREGISTER_URL, 512);
		ShExecInfo.lpFile = szREGISTER_URL;
		ShExecInfo.lpParameters = NULL;
		ShExecInfo.lpDirectory = NULL;
		ShExecInfo.nShow = SW_MAXIMIZE;
		ShExecInfo.hInstApp = NULL;

		ShellExecuteEx(&ShExecInfo);

		uiLoginAccount* loginAccount = GET_CAST_DIALOG( uiLoginAccount, IM_LOGIN_MANAGER::LOGIN_ACCOUNT );
		assert (loginAccount);
		loginAccount->ShowInterface(FALSE);
		loginAccount->ShowInterface(TRUE);
		loginAccount->ClearAll();
		loginAccount->SetFocusID();
		loginAccount->ConnectBtnOn();
	}
}

//------------------------------------------------------------------------------
/**
*/
void
_fn_uiLoginMan_wrong_id()
{
	uiLoginAccount* loginAccount = GET_CAST_DIALOG( uiLoginAccount, IM_LOGIN_MANAGER::LOGIN_ACCOUNT );
    assert (loginAccount);
    loginAccount->ShowInterface(FALSE);
    loginAccount->ShowInterface(TRUE);
    loginAccount->ClearAll();
    loginAccount->SetFocusID();
    loginAccount->ConnectBtnOn();
}

//------------------------------------------------------------------------------
/**
*/
void _fnuiLoginMan_notAllowed_id(bool bYes)
{
	if (bYes)
	{
		TCHAR szREGISTER_URL[512];
		// 마이폴더 홈페이지에 접속하렴
		SHELLEXECUTEINFO ShExecInfo;

		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		ShExecInfo.fMask = NULL;
		ShExecInfo.hwnd = g_hWndMain;
		ShExecInfo.lpVerb = NULL;
		g_InterfaceManager.GetInterfaceString(990, szREGISTER_URL, 512);
		ShExecInfo.lpFile = szREGISTER_URL;
		ShExecInfo.lpParameters = NULL;
		ShExecInfo.lpDirectory = NULL;
		ShExecInfo.nShow = SW_MAXIMIZE;
		ShExecInfo.hInstApp = NULL;

		ShellExecuteEx(&ShExecInfo);
	}
	else
	{
		_fn_uiLoginMan_wrong_id();
	}
}

#ifdef _CHINA
void _fnuiLoginMan_ExecuteCNLuancher()
{
    /*
    TCHAR szFilePath[1024];
    memset(szFilePath,0,sizeof(TCHAR)*1024);
    GetModuleFileName(NULL,szFilePath,sizeof(TCHAR)*1024);
    
    TCHAR* file = strstr(szFilePath,"Sungame.exe");
    TCHAR Path[1024];
    memset(Path,0,sizeof(TCHAR)*1024);

    int lensize = strlen(szFilePath) - strlen(file);
    strncpy(Path,szFilePath,sizeof(TCHAR)*lensize);
    Path[lensize+1] = '\0';

    
    TCHAR szCommand[MAX_PATH] = {0};
    _stprintf(szCommand, _T("%s\\%s"), Path,"SUN2.exe");

    STARTUPINFO StartupInfo = {0};
    StartupInfo.cb = sizeof(STARTUPINFO);
    PROCESS_INFORMATION ProcessInfo;

    BOOL bSucced = CreateProcess(NULL, szCommand, NULL, NULL, FALSE, 0, NULL, NULL,
        &StartupInfo, &ProcessInfo);

    CloseHandle( ProcessInfo.hProcess );
    CloseHandle( ProcessInfo.hThread );	
    */
    //런처 실행은 임시 보류
    GlobalFunc::LogOutGame(false);
}
#endif

#ifdef _APPLY_KOREA_CHANNELING 
void _fnuiLoginMan_ExecuteWebstarter()
{
	TCHAR szGameStartPage_URL[512];
	SHELLEXECUTEINFO ShExecInfo;
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = NULL;
	ShExecInfo.hwnd = g_hWndMain;
	ShExecInfo.lpVerb = NULL;
	g_InterfaceManager.GetInterfaceString( 85229, szGameStartPage_URL, 512);
	ShExecInfo.lpFile = szGameStartPage_URL;
	ShExecInfo.lpParameters = NULL;
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_MAXIMIZE;
	ShExecInfo.hInstApp = NULL;

	ShellExecuteEx(&ShExecInfo);

	GlobalFunc::LogOutGame();
}
#endif

#ifdef _JBH_ADD_AGE_LIMIT_MARK
BYTE uiLoginMan::GetCurrentServerLimitAge()
{
	if( m_pCurrentConnectedServerData )
	{
		AuthGroupData * pAuthGroupData = GetAuthGroupByID(m_pCurrentConnectedServerData->m_GroupID);
		if( pAuthGroupData )
		{
			return pAuthGroupData->GetAuthGroupAge();	
		}
	}
	return 0;
}
#endif 
//////////////////////////////////////////////////////////////////////////
bool _sort_auth_group_total_status( AuthGroupData & authGroup1, AuthGroupData & authGroup2 )
{
	return (authGroup1.GetTotalStatus() < authGroup2.GetTotalStatus());
}

bool _sort_auth_group_global_style(AuthGroupData & authGroup1, AuthGroupData & authGroup2)
{
    // 3, 1, 2 순서로 강제 정렬
    return (authGroup1.GetAuthGroupID()%3 < authGroup2.GetAuthGroupID()%3);
}

void uiLoginMan::RECV_AUTH_READY_CMD(MSG_BASE * pMsg)
{
	Clear();	// 상태 데이터 초기화

	MSG_FC_AUTH_READY_CMD * pRecvPacket = (MSG_FC_AUTH_READY_CMD*)pMsg;
	m_dwEncKey = pRecvPacket->m_dwEncKey;

	// 유저 Auth 키 초기화
	m_dwUserKey = 0;
	GENERALPARAM->SetUserID(0);

	m_bAuthConnected = true;

}
void uiLoginMan::SEND_AUTH_VERIFY_SYN()
{
    if (!g_pNetwork)
    {
        return;
    }

	if (SafePKBool(ePKBOOL_LOGIN_C2S_ASK_VERIFY))
	{
		MSG_CF_AUTH_VERIFY_SYN  sync;

		sync.m_byHighVersion = C2S_HIGH_VERSION_NO;
		sync.m_byMiddleVersion = C2S_MIDDLE_VERSION_NO;
		sync.m_byLowVersion = C2S_LOW_VERSION_NO;

		strncpy(sync.m_szLocalIP, g_pNetwork->GetLocalAddress(CI_AUTHSERVERIDX), IP_MAX_LEN);

		GlobalFunc::SendPacket(CI_AUTHSERVERIDX, &sync, sizeof (sync));

		SetPKBool(ePKBOOL_LOGIN_C2S_ASK_VERIFY, kTimeOutLogin);
	}
}
void uiLoginMan::RECV_AUTH_VERIFY_ACK(MSG_BASE * pMsg)
{
	UnsetPKBool(ePKBOOL_LOGIN_C2S_ASK_VERIFY);

	MSG_FC_AUTH_VERIFY_ACK * pRecvPacket = (MSG_FC_AUTH_VERIFY_ACK *)pMsg;

	TCHAR   szMessage[INTERFACE_STRING_LENGTH] = {0,};

	BYTE    byResult = pRecvPacket->m_byResult;

	if (byResult == 0x01)
	{
		//	최신 버전이 아닙니다.
		g_InterfaceManager.GetInterfaceString( eST_INVALID_CLIENT_VERSION, szMessage, INTERFACE_STRING_LENGTH );

		uiSystemMan * pSystemMan = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
		if(pSystemMan)
			pSystemMan->Sys_VerifyLock(szMessage, TRUE);

		return;
	}
 
#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
    //=============================================
    //      서버선택기능 구문
    //=============================================
    if (m_bServerSelect)
    {
        SEND_AUTH_REAUTH_PASS_SYN();
        return;
    }
#endif //__NA001334_090420_SERVER_SELECT_FACILITY__

#ifdef _COMMON_CHANNELING_LOGIC_DEFINE
    // 비정상 인증플로 및 재시도제한
    if (CheckAuthFlow() == true)
    {
    #if defined (_APPLY_KOREA_CHANNELING)
        // 국내,글로벌 채널링 처리
        SEND_AUTH_TOKENPASS_SYN();
    #elif defined (_NHN_USA_CHANNELING)
        // 북미채널링 처리
        SEND_AUTH_TOKEN_NORTHA_PASS_SYN();
    #elif defined (_APPLY_JAPAN_GAMECHU_CHANNELING)
        SEND_AUTH_TOKEN_JP_GAMECHU_SYN();
    #endif
    }	

#else //_COMMON_CHANNELING_LOGIC_DEFINE

    if (uiLoginMan::IsUseParameterAuthInfo() == true)
    {
        if (CheckAuthFlow() == true)
        {
    #if defined(_APPLY_JAPAN_GAMECHU_CHANNELING) && defined(_JAPAN_GAMECHU_SUPPORT_DOUBLE_AUTH_FLOW_)
            SEND_AUTH_TOKEN_JP_GAMECHU_SYN();
    #elif defined(_APPLY_RUSSIA_INGAMBA_CHANNELING)
            SEND_AUTH_PASS_RU_INGAMBA_SYN();
    #endif
        }
        return;
    }
#if defined(_APPLY_RUSSIA_INGAMBA_CHANNELING)
    #if defined(_SUNGAME_DUMP_VER) || defined(_SUNGAME_VER)
    else
    {
        uiSystemMan * pSystemMan = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
        if (pSystemMan)
        {
            pSystemMan->Sys_VerifyLock(g_InterfaceManager.GetInterfaceString(70278), TRUE);
        }

        return;
    }
    #endif //defined(_SUNGAME_DUMP_VER) || defined(_SUNGAME_VER)
#endif //defined(_APPLY_RUSSIA_INGAMBA_CHANNELING)
    
    
	// 5002	계정 명과 비밀번호를 입력하세요
		m_pUILoginServerList->ShowInterface(FALSE);

	#ifdef __CN_269_MATRIX_CARD_AUTH

        MatrixCardAuth * pMatrixCardAuth = ( MatrixCardAuth *) g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_MATRIX_CARD_AUTH);
		if( pMatrixCardAuth )
		{
			pMatrixCardAuth->ShowInterface(FALSE);
		}
	#endif // __CN_269_MATRIX_CARD_AUTH

    	if(m_pUILoginAccount->IsVisible())
		{
			m_pUILoginAccount->ShowInterface(FALSE);
			m_pUILoginAccount->ShowInterface(TRUE);
		}
		else
		{
			m_pUILoginAccount->ShowInterface(TRUE);
		}
#endif //_COMMON_CHANNELING_LOGIC_DEFINE
}
bool uiLoginMan::SEND_AUTH_ID_PASSWORD_CHECK_SYN()
{
#if defined(_APPLY_RUSSIA_INGAMBA_CHANNELING)
    #if defined(_SUNGAME_DUMP_VER) || defined(_SUNGAME_VER)
        GlobalFunc::LogOutGame();
        return false;
    #endif //defined(_SUNGAME_DUMP_VER) || defined(_SUNGAME_VER)
#endif //defined(_APPLY_RUSSIA_INGAMBA_CHANNELING)
	if (this->SafePKBool(ePKBOOL_LOGIN_C2S_ASK_AUTH))
	{
		MSG_CF_AUTH_ID_PASSWORD_CHECK_SYN	sync;

		sync.m_dwAuthUserID = 0;
		Sprintf (sync.m_szID, "%s", m_strTryID);
		Sprintf (sync.m_szPasswd, "%s", m_strTryPW);

		// 비밀번호 암호화!!
		Crypt::PacketEncode( (unsigned char *)sync.m_szPasswd,
			PASSWD_MAX_LEN,
			(unsigned char *)&sync.m_szPasswd,
			this->m_dwEncKey );

		GlobalFunc::SendPacket(CI_AUTHSERVERIDX, &sync, sizeof (sync));

		this->SetPKBool(ePKBOOL_LOGIN_C2S_ASK_AUTH, kTimeOutLogin);
	}
	else
		return false;

	return true;
}
//////////////////////////////////////////////////////////////////////////
#ifdef __CN_269_MATRIX_CARD_AUTH
void uiLoginMan::RECV_AUTH_MATRIX_CARD_SYN(MSG_BASE * pMsg)
{
	this->UnsetPKBool(ePKBOOL_LOGIN_C2S_ASK_AUTH);
    MatrixCardAuth * pMatrixCardAuth = ( MatrixCardAuth *) g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_MATRIX_CARD_AUTH);
	if( !pMatrixCardAuth )
	{
		assert(_T("MatrixCardAuth 가 없습니다.") );
		return;
	}

	MSG_FC_AUTH_MATRIX_CARD_SYN * pRecvPacket = (MSG_FC_AUTH_MATRIX_CARD_SYN *) pMsg;

	if( m_pUILoginAccount )
	{
		m_pUILoginAccount->ShowInterface(FALSE);
	}

	pMatrixCardAuth->ShowInterface(TRUE);
	SC_INFO * pSCInfo = (SC_INFO * )pRecvPacket->m_SecureInfo.byData;
	pMatrixCardAuth->InitializeMatrixCode(pSCInfo);	

	// 추가 보안 인증 성공
	if( pRecvPacket->m_byResult == 0 )
	{
		// 다시 시도
		//보안 인증 셋팅 및 창 띄우기

		TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};
		// 5041 인증 실패 : 등록된 카드 번호와 일치하지 않습니다.
		g_InterfaceManager.GetInterfaceString(5041 , szMessage , INTERFACE_STRING_LENGTH );
		uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
		if(pSystemMan)
			pSystemMan->Sys_VerifyLock(szMessage);
	}
}
#endif // __CN_269_MATRIX_CARD_AUTH

//------------------------------------------------------------------------------ 
#if defined(__CN_20100909_SECURITY_TOKEN__)

//------------------------------------------------------------------------------ 
void uiLoginMan::SEND_AUTH_SECURITY_TOKEN_SYN(const char* security_token_code)
{
    MSG_CF_AUTH_SECURITY_TOKEN_SYN sync;

    nsPacket::CSAuthFlow::MakeOtpStream(security_token_code, sync);
    GlobalFunc::SendPacket(CI_AUTHSERVERIDX, &sync, sizeof(sync));

    m_pUILoginOneTimePassword->ShowInterface(FALSE);

    SetPKBool(ePKBOOL_LOGIN_C2S_ASK_AUTH, kTimeOutLogin);
}
//------------------------------------------------------------------------------ 
void uiLoginMan::RECV_AUTH_SECURITY_TOKEN_ACK(MSG_BASE* msg_base)
{
    SUN_ASSERT(m_pUILoginOneTimePassword);
    UnsetPKBool(ePKBOOL_LOGIN_C2S_ASK_AUTH);

    MSG_CF_AUTH_SECURITY_TOKEN_ACK* packet = (MSG_CF_AUTH_SECURITY_TOKEN_ACK*)msg_base;

#ifdef _DEV_VER
    //! 개발버전은 첫번째는 실패하고 2번째는 성공하게 셋팅
    static bool auth_ok = false;
    if (auth_ok == false)
    {
        auth_ok = true;
    }
    else
    {
        packet->result_ = MSG_CF_AUTH_SECURITY_TOKEN_ACK::eResult_Success;
    }
#endif //_DEV_VER

    if (packet->result_ == MSG_CF_AUTH_SECURITY_TOKEN_ACK::eResult_Success)
    {
        m_pUILoginOneTimePassword->SetOTPUserStamp(uiLoginOneTimePassword::UseNPassOneTimePassword);
        AuthOk();
    }
    else
    {
        uiSystemMan* pUIMan = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
        if (pUIMan)
        {
            // 5041 인증실패 : 등록된 카드 번호를 다시 입력하시고 순서대로 연속 입력 하세요.
            TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0, };
            g_InterfaceManager.GetInterfaceString(5041, szMessage);

            pUIMan->Sys_Confirm(szMessage, GlobalFunc::RetrySecurityToken, NULL, NULL, 100 /*확인*/, 3 /*게임종료*/);

        }
    }
}

#elif defined(_JP000000_ONETIMEPASSWORD_AUTHFLOW_)

void uiLoginMan::RECV_AUTH_OTP_PASS_CMD(MSG_BASE * pMsg)
{
    UnsetPKBool(ePKBOOL_LOGIN_C2S_ASK_AUTH);

    MSG_CF_AUTH_OTP_PASS_CMD* pRecvPacket = (MSG_CF_AUTH_OTP_PASS_CMD*)pMsg;
    SUN_ASSERT(m_pUILoginOneTimePassword);

    switch (pRecvPacket->command)
    {
    case MSG_CF_AUTH_OTP_PASS_CMD::eCommand_None:
        {
            SUN_ASSERT_MSG(NULL, "MSG_CF_AUTH_OTP_PASS_CMD::eCommand Value(eCommand_None)");
            m_pUILoginOneTimePassword->SetOTPUserStamp(uiLoginOneTimePassword::NoneStamp);
            resetTryReconnect();
        }
        break;
    case MSG_CF_AUTH_OTP_PASS_CMD::eCommand_AuthSuccess_HasNotOneTimePassword:
        {
            // OTP 미사용유저 - 특별한 처리를 하지 않고 AUTH_ID_PASSWORD_CHECK_ACK 패킷을 기다린다.
            m_pUILoginOneTimePassword->SetOTPUserStamp(uiLoginOneTimePassword::UseNotOneTimePassword);
        }        
        break;
    case MSG_CF_AUTH_OTP_PASS_CMD::eCommand_Require_OneTimePassword:
        {
            // OTP 사용유저 - OTP입력UI를 띄워준다.
            m_pUILoginOneTimePassword->SetOTPUserStamp(uiLoginOneTimePassword::UseOneTimePassword);
            m_pUILoginAccount->ShowInterface(FALSE);
            m_pUILoginOneTimePassword->ShowInterface(TRUE);
        }        
        break;    
    case MSG_CF_AUTH_OTP_PASS_CMD::eCommand_AuthSuccess_NoOtpEvent:
        {
            // OTP 서비스 미지원 - 테스트서버같은 경우 OTP플로우는 패스
            m_pUILoginOneTimePassword->SetOTPUserStamp(uiLoginOneTimePassword::Auth_NonOneTimePasswordFlow);
        }        
        break;        
    default:
        {
            SUN_ASSERT_MSG(NULL, "Undefined MSG_CF_AUTH_OTP_PASS_CMD::eCommand Value");
            uiSystemMan* pUIMan = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
            if (pUIMan)
            {
                // 5013	올바른 인증절차를 거치지 않아 로그아웃 됩니다.
                TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0, };
                g_InterfaceManager.GetInterfaceString(5013, szMessage);
                // 종료
                pUIMan->Sys_VerifyLock(szMessage , TRUE);
            }            
        }        
        break;
    }
}

void uiLoginMan::SEND_AUTH_OTP_PASS_SYN(TCHAR* szOTPInput)
{
    // 입력한 OTP 보내기
    MSG_CF_AUTH_OTP_PASS_SYN sync;
    nsPacket::OneTimePassword Opt_info;
    strncpy(Opt_info.value, szOTPInput, nsPacket::OneTimePassword::kLength);
    nsPacket::CSAuthFlow::MakeOtpStream(m_strTryID, Opt_info, &sync);
    GlobalFunc::SendPacket(CI_AUTHSERVERIDX, &sync, sizeof (sync));

    m_pUILoginOneTimePassword->ShowInterface(FALSE);

    SetPKBool(ePKBOOL_LOGIN_C2S_ASK_AUTH, kTimeOutLogin);
}

void uiLoginMan::RECV_AUTH_OTP_PASS_ACK(MSG_BASE * pMsg)
{
    UnsetPKBool(ePKBOOL_LOGIN_C2S_ASK_AUTH);    

    // OTP입력에 대한 결과응답 수신처리
    MSG_CF_AUTH_OTP_PASS_ACK* pRecvPacket = (MSG_CF_AUTH_OTP_PASS_ACK*)pMsg;
    switch(pRecvPacket->result)
    {
    case MSG_CF_AUTH_OTP_PASS_ACK::eResult_AuthSuccess:
        // OTP인증통과 - 곧이어 AUTH_ID_PASSWORD_CHECK_ACK 이 온다. 기다림
        m_pUILoginOneTimePassword->SetOTPUserStamp(uiLoginOneTimePassword::UseNPassOneTimePassword);
        break;
    case MSG_CF_AUTH_OTP_PASS_ACK::eResult_OtpExpired:
        {
            // OTP시간만료 - FC_AUTH_READY_CMD 패킷 기다림 *인증재접속을 위해 서버/클라 어디서 끊을지 결정필요
            m_pUILoginOneTimePassword->SetOTPUserStamp(uiLoginOneTimePassword::UseNExpiredOneTimePassword);
            // 만료시에 대한 메시지 출력 후 인증재접속
            uiSystemMan* pUIMan = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
            if (pUIMan)
            {
                TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0, };
                g_InterfaceManager.GetInterfaceString(5639, szMessage);
                pUIMan->Sys_VerifyLock(szMessage, GlobalFunc::RetryOneTimePassword);
            }
        }        
        break;
    case MSG_CF_AUTH_OTP_PASS_ACK::eResult_Retry:
        {
            // OTP 틀림 - 알림창 띄운 후 재입력
            m_pUILoginOneTimePassword->SetOTPUserStamp(uiLoginOneTimePassword::UseNRetryOneTimePassword);
            uiSystemMan* pUIMan = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
            if (pUIMan)
            {
                TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0, };
                g_InterfaceManager.GetInterfaceString(5636, szMessage);
                pUIMan->Sys_VerifyLock(szMessage, GlobalFunc::RetryOneTimePassword);
            }
        }        
        break;
    case MSG_CF_AUTH_OTP_PASS_ACK::eResult_Fail:
    default:
        {
            uiSystemMan* pUIMan = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
            if (pUIMan)
            {
                // 5013	올바른 인증절차를 거치지 않아 로그아웃 됩니다.
                TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0, };
                g_InterfaceManager.GetInterfaceString(5013, szMessage);
                // 종료
                pUIMan->Sys_VerifyLock(szMessage , TRUE);
            }  
        }
        break;
    }
}
#endif //_JP000000_ONETIMEPASSWORD_AUTHFLOW_ __CN_20100909_SECURITY_TOKEN__

//////////////////////////////////////////////////////////////////////////
void uiLoginMan::RECV_AUTH_ID_PASSWORD_CHECK_ACK(MSG_BASE * pMsg)
{
	UnsetPKBool(ePKBOOL_LOGIN_C2S_ASK_AUTH);
	MSG_FC_AUTH_ID_PASSWORD_CHECK_ACK *pRecvPacket = (MSG_FC_AUTH_ID_PASSWORD_CHECK_ACK *)pMsg;
    
    m_TryCount = KLoginConnected;

    // OK (0) ; ERROR (Code)
	BYTE bOk = !(pRecvPacket->m_byResult);
	if (bOk == FALSE)
	{
        g_LoginScene.SetLoginProgressState(0);  // 계정정보를 다시 입력

#ifdef _NA_20100307_BETAKEY_SYSTEM
        m_betakey_info = static_cast<UINT64>(0);
#endif//_NA_20100307_BETAKEY_SYSTEM
		int iResult = (pRecvPacket->m_byResult < c_iMAX_AUTH_RESULT_IDS) ? pRecvPacket->m_byResult : 2;
		int iErrorCode = eST_AUTHRESULT_OK + c_AUTH_RESULT_IDS[iResult];
		int iFailCount = pRecvPacket->m_byFailCount;

		TCHAR szMessage[INTERFACE_STRING_LENGTH];
		g_InterfaceManager.GetInterfaceString(iErrorCode, szMessage, INTERFACE_STRING_LENGTH);

		uiSystemMan* pSystemMan = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
		if(pSystemMan)
		{
#ifdef _COMMON_CHANNELING_LOGIC_DEFINE
			if (iErrorCode != eST_AUTHRESULT_SYSTEMERROR)
			{
				pSystemMan->Sys_VerifyLock(szMessage, TRUE);
			}
#else //_COMMON_CHANNELING_LOGIC_DEFINE
    #if defined(_JAPAN_GAMECHU_SUPPORT_DOUBLE_AUTH_FLOW_) || defined(_APPLY_RUSSIA_INGAMBA_CHANNELING)
            if (uiLoginMan::IsUseParameterAuthInfo() == true)
            {
                if (iErrorCode != eST_AUTHRESULT_SYSTEMERROR)
                {
                    pSystemMan->Sys_VerifyLock(szMessage, TRUE);
                    return;
                }                    
            }
    #endif //_JAPAN_GAMECHU_SUPPORT_DOUBLE_AUTH_FLOW_

            DWORD messagebox_key = 0;
			if (iErrorCode == eST_AUTHRESULT_NOTALLOWEDACCOUNT)
			{
				messagebox_key = pSystemMan->Sys_Confirm(szMessage, _fnuiLoginMan_notAllowed_id, NULL, NULL, 100, 5, true);
				m_pUILoginAccount->ClearAll();
			}
			else
			{
				messagebox_key = pSystemMan->Sys_Confirm(szMessage, _fn_uiLoginMan_Register, NULL, NULL, 100, 285, true);	
				m_pUILoginAccount->ClearAll();
			}

            SolarDialog* solar_dialog = g_InterfaceManager.GetDialog(messagebox_key);
            if (solar_dialog != NULL)
            {
                float x = solar_dialog->GetDialogWZ()->GetDlgX();
                float y = solar_dialog->GetDialogWZ()->GetDlgY() + 50.0f;
                solar_dialog->MoveWindowWZ(x, y);
            }

#endif //_COMMON_CHANNELING_LOGIC_DEFINE
		}
		return;
	}

    // 로그인 성공


#ifdef _NA_20100307_BETAKEY_SYSTEM
    m_betakey_info = pRecvPacket->m_betakey_;
#endif//_NA_20100307_BETAKEY_SYSTEM

#if defined(__CN_20100909_SECURITY_TOKEN__)

    #ifdef _DEV_VER
        // 개발 버전에는 토큰 보이게
        pRecvPacket->m_Is_Token_Use = true;
    #endif //_DEV_VER

    if (pRecvPacket->m_Is_Token_Use)
    {
        // OTP 사용유저 - OTP입력UI를 띄워준다.
        m_pUILoginOneTimePassword->SetOTPUserStamp(uiLoginOneTimePassword::UseOneTimePassword);
        m_pUILoginAccount->ShowInterface(FALSE);
        m_pUILoginOneTimePassword->ShowInterface(TRUE);
        return;
    }

#elif defined(_JP000000_ONETIMEPASSWORD_AUTHFLOW_)

    DWORD otp_stamp = m_pUILoginOneTimePassword->GetOTPUserStamp();

    #ifdef _JAPAN_GAMECHU_SUPPORT_DOUBLE_AUTH_FLOW_
    if (uiLoginMan::IsUseParameterAuthInfo() == true)
    {
        // OPT 패스 처리
        otp_stamp = uiLoginOneTimePassword::Auth_NonOneTimePasswordFlow;
    }
    #endif

    // 서버선택 or 테스트서버 같이 OTP서비스를 지원하지 않을 경우
    if ((uiLoginOneTimePassword::ReAuth_NonOneTimePasswordFlow == otp_stamp)
        || (uiLoginOneTimePassword::Auth_NonOneTimePasswordFlow == otp_stamp))
    {
        // OTP플로우 무시
    }
    // OTP 미사용 유저의 경우 사용권유 팝업띄우기
    else if (uiLoginOneTimePassword::UseNotOneTimePassword == otp_stamp)
    {
        uiSystemMan* pUIMan = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
        if (pUIMan)
        {
            TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0, };
            g_InterfaceManager.GetInterfaceString(5637, szMessage);
            pUIMan->Sys_Confirm(szMessage, GlobalFunc::EncourageOTP);
        }
        return;
    }    
    else if (uiLoginOneTimePassword::UseNPassOneTimePassword != otp_stamp)
    {   
        // OTP 사용자가 OPT정상인증을 통하지 않은 상태로 오면 안됨
        uiSystemMan* pUIMan = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
        if (pUIMan)
        {
            // 5013	올바른 인증절차를 거치지 않아 로그아웃 됩니다.
            TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0, };
            g_InterfaceManager.GetInterfaceString(5013, szMessage);
            // 종료
            pUIMan->Sys_VerifyLock(szMessage , TRUE);
        }
    }
#endif //_JP000000_ONETIMEPASSWORD_AUTHFLOW_ __CN_20100909_SECURITY_TOKEN__

    AuthOk();
}

//------------------------------------------------------------------------------
void uiLoginMan::AuthOk()
{
#if defined(__NA000000_070913_HACKSHIELD_INSTALLATION__) && defined(_APPLY_HACKSHIELD_MONITOR)
    #ifdef _APPLY_KOREA_CHANNELING
        _AhnHS_SetUserId(m_Auth_Info.id);
    #else
        _AhnHS_SetUserId(m_strTryID);
    #endif
#endif //defined(__NA000000_070913_HACKSHIELD_INSTALLATION__) || defined(_APPLY_HACKSHIELD_MONITOR)

#ifdef _YMS_USING_FROST_LIBRARY_
    #ifdef _APPLY_KOREA_CHANNELING
        sun_network_api::sun_set_username(m_Auth_Info.id);
    #else
        sun_network_api::sun_set_username(m_strTryID);
    #endif
#endif //_YMS_USING_FROST_LIBRARY_

    SEND_AUTH_SERVER_LIST_SYN();

#ifdef _DEV_VER//_DH_LOGIN_LOG_ID_AND_CHANNEL
    TCHAR szTitle[MAX_PATH];
    TCHAR temp_string[MAX_PATH];
    GetWindowText(g_hWndMain, temp_string, sizeof(szTitle));
    sprintf(szTitle, "%s#%s", strtok(temp_string, "#"), GetID());
    //_tcsncat(szTitle, GetID(), sizeof(szTitle));
    ::SetWindowText(g_hWndMain, szTitle);
#endif//

    // 인증 통과 --> 서버 목록 요청
    this->m_bAuthFinished = true;
    this->m_bTryReconnect = false;
}

//------------------------------------------------------------------------------ 
// 월드 및 서버 
void uiLoginMan::SEND_AUTH_SERVER_LIST_SYN()
{
	if (SafePKBool(ePKBOOL_LOGIN_C2S_ASK_SVRLIST)
		&& SafePKBool(ePKBOOL_LOGIN_S2C_SERVER_NAME_LIST))
	{
        // 계정 입력창 내리기
#ifdef _COMMON_CHANNELING_LOGIC_DEFINE
        // no operation
#else //_COMMON_CHANNELING_LOGIC_DEFINE
        m_pUILoginAccount->ShowInterface(FALSE);
#endif //_COMMON_CHANNELING_LOGIC_DEFINE
   
		m_bServerListInfo = true;

		MSG_CF_AUTH_SERVER_LIST_SYN sync;
		GlobalFunc::SendPacket(CI_AUTHSERVERIDX, &sync, sizeof (sync));
		SetPKBool(ePKBOOL_LOGIN_C2S_ASK_SVRLIST, kTimeOutLogin);
		SetPKBool(ePKBOOL_LOGIN_S2C_SERVER_NAME_LIST, kTimeOutLogin);

		// 서버 선택창 띄우기
		m_pUILoginServerList->ShowInterface(TRUE);
	}
}
void uiLoginMan::RECV_AUTH_SERVER_LIST_WORLD_ACK(MSG_BASE * pMsg)
{
#ifdef _DH_GLOBAL_VERSION_TEST
    const char* server_name1 = "TestWorld"; // 글로벌 내부 개발 1101, DMZ 테스트 1101
    const char* server_name2 = "Test - Asia"; // 라이브 테스트 1101
    const char* server_name3 = "Test(Int'l)"; // 라이브 테스트 1101
#endif//_DH_GLOBAL_VERSION_TEST
	UnsetPKBool(ePKBOOL_LOGIN_S2C_SERVER_NAME_LIST);

	MSG_FC_AUTH_SERVER_LIST_WORLD_ACK * pRecvPacket = (MSG_FC_AUTH_SERVER_LIST_WORLD_ACK*)pMsg;

	int nGroupCount = (int)(pRecvPacket->m_Count);
	m_AuthGroupData.resize(nGroupCount);
	for (int i = 0;i < nGroupCount;++i)
	{
#ifdef _DH_GLOBAL_VERSION_TEST
#if (NATION_BUILD_VERSION == 1101)
        if (strncmp((pRecvPacket->m_GroupDataList[i]).m_szGroupName, server_name1, strlen(server_name1)) &&
            strncmp((pRecvPacket->m_GroupDataList[i]).m_szGroupName, server_name2, strlen(server_name2)) &&
            strncmp((pRecvPacket->m_GroupDataList[i]).m_szGroupName, server_name3, strlen(server_name3)))
#else
        if (strncmp((pRecvPacket->m_GroupDataList[i]).m_szGroupName, server_name1, strlen(server_name1)) == 0 ||
            strncmp((pRecvPacket->m_GroupDataList[i]).m_szGroupName, server_name2, strlen(server_name2)) == 0 ||
            strncmp((pRecvPacket->m_GroupDataList[i]).m_szGroupName, server_name3, strlen(server_name3)) == 0)
#endif//NATION_BUILD_VERSION
        {
            continue;
        }
#endif//_DH__DH_GLOBAL_VERSION_TEST
		m_AuthGroupData[i].SetGroupData( &(pRecvPacket->m_GroupDataList[i]) );
	}

    m_pUILoginServerList->UpdateServerList();

    m_bRecvGroupData = true;

}

void uiLoginMan::RECV_AUTH_SERVER_LIST_CHANNEL_ACK(MSG_BASE * pMsg)
{
	UnsetPKBool(ePKBOOL_LOGIN_C2S_ASK_SVRLIST);

	MSG_FC_AUTH_SERVER_LIST_CHANNEL_ACK * pRecvPacket = (MSG_FC_AUTH_SERVER_LIST_CHANNEL_ACK*)pMsg;

	int nServerCount = (int)(pRecvPacket->m_Count);
	for (int i = 0;i < nServerCount;++i)
	{
		AuthGroupData* authGroupData = GetAuthGroupByID(pRecvPacket->m_ServerDataList[i].m_GroupID);
#ifdef _DH_GLOBAL_VERSION_TEST		
        assert(authGroupData);
#endif//_DH_GLOBAL_VERSION_TEST
		if (authGroupData)
		{
			authGroupData->AddServerData( &(pRecvPacket->m_ServerDataList[i]) );
		}		
	}

	m_bRecvServerData = true;
#if defined(_GOLBAL)
    sort( m_AuthGroupData.begin(), m_AuthGroupData.end(), _sort_auth_group_global_style );
#else
	sort( m_AuthGroupData.begin(), m_AuthGroupData.end(), _sort_auth_group_total_status );
#endif//_GOLBAL

    m_pUILoginServerList->UpdateServerList();
}

void uiLoginMan::SEND_AUTH_SERVER_SELECT_SYN(AuthSetverData* pServerData)
{
    if (pServerData == NULL)
    {
        return;
    }

	//혼잡도 체크
	if (pServerData->m_CrowdGrade == eAUTH_SERVER_STAT_VERY_BUSY_LEVEL_9)
	{
		TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
		// 채널 혼잡으로 인해 접속이 가능하지 않습니다.\n 잠시 후에 시도하시거나 다른 채널을 선택해 주시기 바랍니다. : 5017
		g_InterfaceManager.GetInterfaceString(eST_CHANNEL_IS_FULL, szMessage, INTERFACE_STRING_LENGTH );

		uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
		if(pSystemMan)
			pSystemMan->Sys_VerifyLock(szMessage);

		return;
	}

	// 서버 선택
	if (this->SafePKBool(ePKBOOL_LOGIN_C2S_ASK_SVRSELECT))
	{
#ifdef _NHN_USA_CHANNELING
	GlobalFunc::SendHanReportPacket(300);//서버선택(300)
	GlobalFunc::SendHanReportPacket(304);//게임서버접속시작(304)
#endif//_NHN_USA_CHANNELING
		MSG_CF_AUTH_SERVER_SELECT_SYN	sync;
		sync.m_GroupID		= pServerData->m_GroupID;
		sync.m_ChannelID	= pServerData->m_ChannelID;

		GlobalFunc::SendPacket(CI_AUTHSERVERIDX, &sync, sizeof (sync));

		this->SetPKBool(ePKBOOL_LOGIN_C2S_ASK_SVRSELECT, kTimeOutLogin);
	}

	SetCurrentServerData(pServerData);	// 선택된 서버(채널)정보 저장
}
void uiLoginMan::RECV_AUTH_SERVER_SELECT_ACK(MSG_BASE * pMsg)
{
	this->UnsetPKBool(ePKBOOL_LOGIN_C2S_ASK_SVRSELECT);

	MSG_FC_AUTH_SERVER_SELECT_ACK* pRecvPacket = (MSG_FC_AUTH_SERVER_SELECT_ACK *)pMsg;

	m_dwUserKey = pRecvPacket->m_dwAuthUserID;
	memcpy((BYTE*)szSerialKey, pRecvPacket->m_serialKey, 32);
	memcpy((BYTE*)szSvrIP, pRecvPacket->m_szSvrIP, IP_MAX_LEN);
	dwSvrPort = pRecvPacket->m_dwSvrPort;

	GENERALPARAM->SetUserID(m_dwUserKey);

	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0, };

	if (pRecvPacket->m_byResult != 0)
	{
		int iResult = (pRecvPacket->m_byResult < c_iMAX_AUTH_RESULT_IDS) ? pRecvPacket->m_byResult : 2;
		int iErrorCode = eST_AUTHRESULT_OK + c_AUTH_RESULT_IDS[iResult];

		TCHAR szMessage[INTERFACE_STRING_LENGTH];
		g_InterfaceManager.GetInterfaceString(iErrorCode, szMessage, INTERFACE_STRING_LENGTH);

		uiSystemMan* pSystemMan = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
		if(pSystemMan)
        {
			pSystemMan->Sys_VerifyLock(szMessage, _VerifyCallBack_TryReconnect);
        }

		return;
	}

	// 게임서버에 연결
	int iConnectRet = FALSE;

    if (g_pNetwork)
    {
        iConnectRet = g_pNetwork->Connect(CI_GAMESERVERIDX, szSvrIP, (unsigned short)dwSvrPort);
    }

	if (FALSE == iConnectRet)
	{
		//	게임 서버 연결 실패
		g_InterfaceManager.GetInterfaceString( eST_GAME_SERVER_CONNECT_FAIL, szMessage, INTERFACE_STRING_LENGTH );

		uiSystemMan* pSystemMan = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
		if(pSystemMan)
        {
			pSystemMan->Sys_VerifyLock(szMessage, _VerifyCallBack_TryReconnect);
        }

		return;
	}

    if (g_pNetwork)
    {
	    g_pNetwork->SetEnableSendHeartBeat(TRUE);
    }

	// 인증서버 접속해제
	g_LoginScene.Disconnect();
	g_LoginScene.SetConnectedToAuthServer();

	this->m_pUILoginServerList->ShowInterface(FALSE);
#ifdef _COMMON_CHANNELING_LOGIC_DEFINE
    // no operation
#else //_COMMON_CHANNELING_LOGIC_DEFINE
	this->m_pUILoginAccount->ShowInterface(FALSE);
#endif //_COMMON_CHANNELING_LOGIC_DEFINE

	// 게임서버에 접속이 된 상태.
	g_LoadScene.ChangeImage(TRUE);
	g_LoadScene.SetNextSceneAfterLoaded(SCENE_TYPE_CHARSELECT, GENERALPARAM->GetCharSelectMapID(), 0 );

	SCENE_TYPE changeSceneType = SCENE_TYPE_LOAD;
	DWORD changeWaitingSec = 0;

	g_LoginScene.ChangeSceneReady(changeWaitingSec, changeSceneType);

	g_isChangeServer = false;

#ifdef _NHN_USA_CHANNELING
	GlobalFunc::SendHanReportPacket(305);//게임서버접속완료(305)
#endif//_NHN_USA_CHANNELING(북미 채널링:지표)

#ifdef _KIKI_WEB_AUTH	
#ifdef _KIKI_UI_WEB_DIALOG

	uiWebDialogMan* webMan = GET_DIALOG_MANAGER(uiWebDialogMan, UIMAN_WEB);
	if (webMan)
	{
		webMan->RequestLogin(GetID(), pRecvPacket->m_szGLogKey);	// 웹 인증 요청
	}

#endif
#endif
}
int	uiLoginMan::GetAuthGroupNum()
{
	return (int)m_AuthGroupData.size();
}
AuthGroupData* uiLoginMan::GetAuthGroupByID(BYTE groupID)
{
	for (unsigned int i = 0;i < m_AuthGroupData.size();++i)
	{
		if ( groupID == m_AuthGroupData[i].GetAuthGroupID() )
		{
			return &(m_AuthGroupData[i]);
		}
	}
	return NULL;
}

AuthSetverData* uiLoginMan::GetServerDataByServerName(LPCTSTR ServerName )
{
    for( size_t i=0;i!=m_AuthGroupData.size();++i)
    {
        AuthGroupData* pGroupData = &(m_AuthGroupData[i]);
        if( AuthSetverData* pServerData = pGroupData->GetAuthServerDatabyName( ServerName ) )
        {
            return pServerData;
        }
    }
    return NULL;
}

AuthGroupData* uiLoginMan::GetAuthGroupByIndex(BYTE groupIndex)
{
	if (groupIndex >= m_AuthGroupData.size() || groupIndex < 0)
		return NULL;

	return &(m_AuthGroupData[groupIndex]);
}
void uiLoginMan::SetCurrentServerData(AuthSetverData* pServerData)
{
	if (!pServerData)
		return;

	DeleteCurrentServerData();
	m_pCurrentConnectedServerData = new AuthSetverData;
	m_pCurrentConnectedServerData->m_GroupName		= pServerData->m_GroupName;
	m_pCurrentConnectedServerData->m_ServerName		= pServerData->m_ServerName;
	m_pCurrentConnectedServerData->m_GroupID		= pServerData->m_GroupID;
	m_pCurrentConnectedServerData->m_ChannelID		= pServerData->m_ChannelID;
	m_pCurrentConnectedServerData->m_ChannelIndex	= pServerData->m_ChannelIndex;
	m_pCurrentConnectedServerData->m_CrowdGrade		= pServerData->m_CrowdGrade;
	m_pCurrentConnectedServerData->m_ConnectStatus	= pServerData->m_ConnectStatus;
}
AuthSetverData* uiLoginMan::GetCurrentServerData()
{
	return m_pCurrentConnectedServerData;
}
void uiLoginMan::DeleteCurrentServerData()
{
	if (m_pCurrentConnectedServerData)
	{
		delete m_pCurrentConnectedServerData;
		m_pCurrentConnectedServerData = NULL;
	}
}
bool uiLoginMan::isRecvGroupServerData()
{
	return (m_bRecvGroupData && m_bRecvServerData);
}

#ifdef _APPLY_KOREA_CHANNELING
bool uiLoginMan::SEND_AUTH_TOKENPASS_SYN()
{
	if (this->SafePKBool(ePKBOOL_LOGIN_C2S_ASK_AUTH))
	{
        int commandline_arg_count = 0;
        wchar_t** command_line_arg_array = NULL;
        command_line_arg_array = ::CommandLineToArgvW(GetCommandLineW(), &commandline_arg_count);

		char string[INTERFACE_STRING_LENGTH] = "SUN";

#ifdef _NA_005039_20120703_ENABLE_MULTI_CLIENT_COUNT
        if (commandline_arg_count > 1)
        {
            GlobalFunc::ChangeMultiEditInput(command_line_arg_array[1], string);
        }
#endif //_NA_005011_20120621_MULTICLIENTS

        // 인증키를 가져오기
        RMSM ShareMemManager(CreateShareMem());
        ShareMemManager.get()->ReadFromShareMemory(string, &m_Auth_Info);

        // 웹스타터를 이용하지 않은 비정상실행
        if (m_Auth_Info.pMapHandle == NULL)
        {
            TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0, };
            g_InterfaceManager.GetInterfaceString( 85228, szMessage, INTERFACE_STRING_LENGTH-1);

            uiSystemMan * pSystemMan = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
            if(pSystemMan)
                pSystemMan->Sys_VerifyLock(szMessage, _fnuiLoginMan_ExecuteWebstarter);

            return false;
        }

#ifdef _APPLY_GLOBAL_CHANNELING
		//- 글로벌 채널링 인증패킷 생성
		MSG_CF_AUTH_TOKEN_GLOBAL_PASS_SYN sync;
		WORD msg_size = nsPacket::CSAuthFlow::MakeStream((char*)m_Auth_Info.id, (char*)m_Auth_Info.key, "", &sync.info_stream);
#else
		//- 한게임 채널링 인증패킷 생성	
		MSG_CF_AUTH_TOKEN_PASS_SYN sync;
		WORD msg_size = nsPacket::CSAuthFlow::MakeStream(m_Auth_Info, &sync.info_stream);
#endif //_APPLY_GLOBAL_CHANNELING	

		if (msg_size)
		{
			Crypt::PacketEncode((BYTE*)&sync.info_stream, 
                                msg_size, 
                                (BYTE*)&sync.info_stream, 
                                m_dwEncKey);
            GlobalFunc::SendPacket(CI_AUTHSERVERIDX, &sync, sizeof(sync));
            this->SetPKBool(ePKBOOL_LOGIN_C2S_ASK_AUTH, kTimeOutLogin);
            ++m_TryCount;
		}
		else
        {
            return false;
        }	
	}
	else
    {
		return false;
    }    
    
	return true;
}
void uiLoginMan::RECV_AUTH_TOKENPASS_ACK(MSG_BASE * pMsg)
{
	strncpy(m_strTryID, m_Auth_Info.id, ID_MAX_LEN+1 );

	RECV_AUTH_ID_PASSWORD_CHECK_ACK(pMsg);
}
#endif//_APPLY_KOREA_CHANNELING

#ifdef _NHN_USA_CHANNELING
//------------------------------------------------------------------------------
/**  북미 인증 SYN
*/
bool uiLoginMan::SEND_AUTH_TOKEN_NORTHA_PASS_SYN()
{
	if (this->SafePKBool(ePKBOOL_LOGIN_C2S_ASK_AUTH))
	{
		//게임스트링없으면 리턴
		if (g_szGameString == NULL) 
        {
            return false;
        }

		//param1:Member ID얻기 
		char szMemberId[21] = {0x00}; //21 is Max
		int nRet = HanAuthGetId(g_szGameString, szMemberId, sizeof(szMemberId));
		memcpy(&m_strTryID, szMemberId,sizeof(szMemberId));

		//param2:AuthString얻기
		char szAuthString[SIZE_AUTHSTRING] = {0x00};
		nRet = GetAuthString(szAuthString, sizeof(szAuthString));
		
		//패킷생성해서 서버로 보내기
        MSG_CF_AUTH_TOKEN_NORTHA_PASS_SYN sync;
		WORD msg_size = nsPacket::CSAuthFlow::MakeStream(szMemberId, szAuthString, &sync);
		if (msg_size)
		{
			GlobalFunc::SendPacket(CI_AUTHSERVERIDX, &sync, sizeof(sync));
			++m_TryCount;
			this->SetPKBool(ePKBOOL_LOGIN_C2S_ASK_AUTH, kTimeOutLogin);
		}
		else
        {
			return false;
        }
	}
	else
    {
		return false;
    }

	return true;
}
//------------------------------------------------------------------------------
/**  북미 인증 ACK
*/
void uiLoginMan::RECV_AUTH_TOKEN_NORTHA_PASS_ACK(MSG_BASE * pMsg)
{
	RECV_AUTH_ID_PASSWORD_CHECK_ACK(pMsg);
}
#endif//_NHN_USA_CHANNELING(북미채널링)

#ifdef _APPLY_JAPAN_GAMECHU_CHANNELING
bool uiLoginMan::SEND_AUTH_TOKEN_JP_GAMECHU_SYN()
{
    if (this->SafePKBool(ePKBOOL_LOGIN_C2S_ASK_AUTH))
    {
        // 게임츄 파라매터가 없으면 리턴
        if (uiLoginMan::IsUseParameterAuthInfo() != true) 
        {
            return false;
        }

        wchar_t auth_info[GameChuInfoStream::MAX_FIRST_PARAMETER] = {L'\0', };
        wchar_t user_info[GameChuInfoStream::MAX_SECOND_PARAMETER] = {L'\0', };
        wcsncpy(auth_info, uiLoginMan::GetAuthInfo(), GameChuInfoStream::MAX_FIRST_PARAMETER);
        auth_info[_countof(auth_info) - 1] = L'\0';
        wcsncpy(user_info, uiLoginMan::GetUserInfo(), GameChuInfoStream::MAX_SECOND_PARAMETER);
        user_info[_countof(user_info) - 1] = L'\0';

        //패킷생성해서 서버로 보내기
        MSG_CF_AUTH_TOKEN_JP_GAMECHU_SYN sync;
        WORD msg_size = nsPacket::CSAuthFlow::MakeStream(auth_info, user_info, sync);
        if (msg_size)
        {
            GlobalFunc::SendPacket(CI_AUTHSERVERIDX, &sync, sizeof(sync));
            ++m_TryCount;
            this->SetPKBool(ePKBOOL_LOGIN_C2S_ASK_AUTH, kTimeOutLogin);
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    return true;
}

void uiLoginMan::RECV_AUTH_TOKEN_JP_GAMECHU_ACK(MSG_BASE * pMsg)
{
#ifdef _JAPAN_GAMECHU_SUPPORT_DOUBLE_AUTH_FLOW_
    if (uiLoginMan::IsUseParameterAuthInfo() == true)
#endif
    {
        MSG_FC_AUTH_ID_PASSWORD_CHECK_ACK* pRecvPacket = (MSG_FC_AUTH_ID_PASSWORD_CHECK_ACK*)(pMsg);
        strncpy(m_strTryID, pRecvPacket->m_szID, ID_MAX_LEN+1);
        m_strTryID[_countof(m_strTryID) - 1] = '\0';
    }
    RECV_AUTH_ID_PASSWORD_CHECK_ACK(pMsg);
}
#endif //_APPLY_JAPAN_GAMECHU_CHANNELING

#ifdef _APPLY_RUSSIA_INGAMBA_CHANNELING
bool uiLoginMan::SEND_AUTH_PASS_RU_INGAMBA_SYN()
{
    if (this->SafePKBool(ePKBOOL_LOGIN_C2S_ASK_AUTH))
    {
        if (uiLoginMan::IsUseParameterAuthInfo() != true) 
        {
            return false;
        }

        wchar_t id_info[IngambaAuthInfo::MAX_ID_LEN] = {L'\0', };
        wchar_t password_info[IngambaAuthInfo::MAX_PWD_LEN] = {L'\0', };
        wcsncpy(id_info, uiLoginMan::GetAuthInfo(), IngambaAuthInfo::MAX_ID_LEN);
        id_info[_countof(id_info) - 1] = L'\0';
        wcsncpy(password_info, uiLoginMan::GetUserInfo(), IngambaAuthInfo::MAX_PWD_LEN);
        password_info[_countof(password_info) - 1] = L'\0';

        //패킷생성해서 서버로 보내기
        MSG_CF_AUTH_PASS_RU_INGAMBA_SYN sync;
        IngambaAuthInfo auth_info;
        WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, id_info, -1, &auth_info.user_id_[0], IngambaAuthInfo::MAX_ID_LEN, NULL, NULL);
        WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, password_info, -1, &auth_info.user_password_[0], IngambaAuthInfo::MAX_PWD_LEN, NULL, NULL);
        auth_info.user_id_[_countof(auth_info.user_id_) - 1] = '\0';
        auth_info.user_password_[_countof(auth_info.user_password_) - 1] = '\0';

         strncpy(m_strTryID, auth_info.user_id_, ID_MAX_LEN + 1);
         m_strTryID[_countof(m_strTryID) - 1] = '\0';

        WORD msg_size = nsPacket::CSAuthFlow::MakeStream(&auth_info, &sync);
        if (msg_size)
        {
            GlobalFunc::SendPacket(CI_AUTHSERVERIDX, &sync, sizeof(sync));
            ++m_TryCount;
            this->SetPKBool(ePKBOOL_LOGIN_C2S_ASK_AUTH, kTimeOutLogin);
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    return true;
}
void uiLoginMan::RECV_AUTH_PASS_RU_INGAMBA_ACK(MSG_BASE * pMsg)
{
    if (uiLoginMan::IsUseParameterAuthInfo() == true)
    {
        MSG_FC_AUTH_ID_PASSWORD_CHECK_ACK* pRecvPacket = (MSG_FC_AUTH_ID_PASSWORD_CHECK_ACK*)(pMsg);
    }
    RECV_AUTH_ID_PASSWORD_CHECK_ACK(pMsg);
}
#endif //_APPLY_RUSSIA_INGAMBA_CHANNELING

#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
bool uiLoginMan::SEND_AUTH_REAUTH_PASS_SYN()
{
	if (this->SafePKBool(ePKBOOL_LOGIN_C2S_ASK_AUTH))
	{
#ifdef _NHN_USA_CHANNELING
		MSG_CF_AUTH_TOKEN_NORTHA_PASS_SYN  sync;
		//step1:인증스트링갱신
		HanAuthRefresh(m_strTryID);
	
		//step2:인증스트링얻기
		char szAuthString[SIZE_AUTHSTRING] = {0x00};
		GetAuthString(szAuthString, sizeof(szAuthString));
		//패킷생성해서 서버로 보내기
		WORD msg_size = nsPacket::CSAuthFlow::MakeStream(m_strTryID, szAuthString, &sync);
#else
		MSG_CF_AUTH_REAUTH_PASS_SYN sync;
		WORD msg_size = nsPacket::CSAuthFlow::MakeStream(m_strTryID, m_ReAuth_Info, &sync);
#endif//_NHN_USA_CHANNELING
		
		if (msg_size)
		{
			GlobalFunc::SendPacket(CI_AUTHSERVERIDX, &sync, sizeof(sync));
			++m_TryCount;
			this->SetPKBool(ePKBOOL_LOGIN_C2S_ASK_AUTH, kTimeOutLogin);
			ZeroMemory(&m_ReAuth_Info, sizeof(m_ReAuth_Info));
		}
		else
        {
			return false;
        }
	}
	else
    {
		return false;
    }

	return true;
}

void uiLoginMan::RECV_AUTH_REAUTH_PASS_ACK(MSG_BASE * pMsg)
{
#ifdef _JP000000_ONETIMEPASSWORD_AUTHFLOW_
    // OTP - 서버선택 처리
    m_pUILoginOneTimePassword->SetOTPUserStamp(uiLoginOneTimePassword::ReAuth_NonOneTimePasswordFlow);    
#endif //_JP000000_ONETIMEPASSWORD_AUTHFLOW_
    m_bServerSelect = false;
	RECV_AUTH_ID_PASSWORD_CHECK_ACK(pMsg);
}

bool uiLoginMan::RECV_REAUTH_TOKENSTEAM(const nsPacket::ReAuthTokenStream& rReauthToken)
{
	if (NULL == rReauthToken.stream[0])
    {
		return false;
    }

	m_TryCount = 0;
	memcpy(&m_ReAuth_Info, &rReauthToken, sizeof(rReauthToken) );

    m_bServerSelect = true;

	return true;
}
#endif //__NA001334_090420_SERVER_SELECT_FACILITY__

bool uiLoginMan::CheckAuthFlow()
{
    bool check_auth_flow = true;
    // 인증키 라이프타임 및 재사용 여부에 따라 위치 변경(서버선택 관련, 익스플로러 최소화/최대화 )
    if (m_TryCount == KLoginConnected)
    {
        TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0, };
        g_InterfaceManager.GetInterfaceString(70000, szMessage, INTERFACE_STRING_LENGTH);

        uiSystemMan * pSystemMan = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
        if (pSystemMan)
        {
            pSystemMan->Sys_VerifyLock(szMessage, TRUE, 1, 30);
        }
        check_auth_flow = false;
    }
    // 인증 재시도 횟수 제한
    else if (m_TryCount >= KTryMaxCount)
    {
        TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0, };
        g_InterfaceManager.GetInterfaceString(70276, szMessage, INTERFACE_STRING_LENGTH);

        uiSystemMan * pSystemMan = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
        if (pSystemMan)
        {
            pSystemMan->Sys_VerifyLock(szMessage, TRUE);
        }
        check_auth_flow = false;
    }

    return check_auth_flow;
}

#if defined(_DEBUG) || defined(_GAMECLIENT_VER)
void uiLoginMan::SetIDPW4AutoTest(const TCHAR* ptszID, const TCHAR* ptszPW)
{
	Sprintf (m_strTryID, _T("%s"), ptszID);
	Sprintf (m_strTryPW, _T("%s"), ptszPW);
}
#endif


void uiLoginMan::ShowOnOffUI( bool onoff )
{
#ifdef _COMMON_CHANNELING_LOGIC_DEFINE
#elif defined(_APPLY_RUSSIA_INGAMBA_CHANNELING) && (defined(_SUNGAME_DUMP_VER) || defined(_SUNGAME_VER))
#else
	if(m_pUILoginAccount != NULL && m_bServerListInfo == false)
	{
		if(m_pUILoginAccount->IsVisible() && onoff == false)
		{
			m_pUILoginAccount->hideCheckonShow(true);
		}

		if(!m_pUILoginAccount->IsVisible() && onoff == true)
		{
			m_pUILoginAccount->hideCheckonShow(false);
			m_pUILoginAccount->ShowInterface(TRUE);
		}
	}
#endif

	if(m_pUILoginServerList != NULL && m_bServerListInfo == true)
	{
		if(m_pUILoginServerList->IsVisible() && onoff == false)
		{
			m_pUILoginServerList->hideCheckonShow(true);
		}

		if(!m_pUILoginServerList->IsVisible() && onoff == true)
		{
			m_pUILoginServerList->hideCheckonShow(false);
			m_pUILoginServerList->ShowInterface(TRUE);
		}
	}
	
}


bool uiLoginMan::GetTransTime()
{
#ifdef _COMMON_CHANNELING_LOGIC_DEFINE
#else
	if(m_pUILoginAccount != NULL && m_bServerListInfo == false)
	{
		if(m_pUILoginAccount->IsVisible())
		{
			if(m_pUILoginAccount->GetTransTime() >= 1.0f)
			{
				return true;
			}
		}
	}
#endif

	if(m_pUILoginServerList != NULL && m_bServerListInfo == true)
	{
		if(m_pUILoginServerList->IsVisible())
		{
			if(m_pUILoginServerList->GetTransTime() >=  1.0f)
			{
				return true;
			}
		}
	}

	return false;
}


//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
