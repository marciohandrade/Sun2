#ifndef     __USER_INTERFACE_MANAGER_LOGIN_H__
#define     __USER_INTERFACE_MANAGER_LOGIN_H__
//------------------------------------------------------------------------------
/**
    @class uiLoginMan

    interface manager for a party

    (C) 2005 ckbang
*/
#include "uiLoginMan_def.h"
#include "PacketStruct_CL.h"
#include "uiBaseMan.h"

class uiLoginBack;
#ifdef _COMMON_CHANNELING_LOGIC_DEFINE
#else
class uiLoginAccount;
#endif
class uiLogin15Year;
class uiLoginServerChannelList;
class uiLoginOneTimePassword;
#ifdef _NA_20100307_BETAKEY_SYSTEM
#include "betakey.h"
class BetaKeyInfo;
#endif//_NA_20100307_BETAKEY_SYSTEM

class uiLoginBackTop;
class uiLoginServerChannelList;

struct uiLogineAsigned
{
	static int is_show_count_Account_;
	static int is_show_coint_ServerList_;
};

class uiLoginMan :
    public uiBaseMan
{
	typedef	std::vector<AuthGroupData>				AuthGroupDataVector;

public:
    uiLoginMan(InterfaceManager *pUIMan);

    //--------------------------------------------------------------------------
    // init/release
    //
public:
    virtual void            OnRelease();
	virtual void			OnInitialize();
    void                    Clear();
	void					HideAllDialogsRelease();

    virtual void            InitPKBooleans();
    virtual void            HandlerPKBooleans();

private:
    BOOL                    _LoadUI();

    //--------------------------------------------------------------------------
    // functions
    //
public:
    static void SetParameterForAuth(wchar_t** command_line_arg)
    {
        parameter_auth_info.SetCommandLineParameter(command_line_arg);
    }
    static bool IsUseParameterAuthInfo() { return parameter_auth_info.is_use_auth_info(); }
    static const wchar_t* GetAuthInfo() { return parameter_auth_info.GetAuthInfo(); }
    static const wchar_t* GetUserInfo() { return parameter_auth_info.GetUserInfo(); }

private:
    static ParameterAuthInfo parameter_auth_info;
public:

#ifdef _NA_20100307_BETAKEY_SYSTEM
    BetaKeyInfo* GetBetaKeyInfo() { return &m_betakey_info; }
    BETAKEY GetBetaKey() { return m_betakey_info.GetBetaKey(); }
    bool CanAbilityBetaKey(UINT32 ability_key){ return m_betakey_info.CanAbilityBetaKey(ability_key); }
    bool CanAbilityContent(UINT32 ability_key){ return m_betakey_info.CanAbilityContent(ability_key); }
#endif//_NA_20100307_BETAKEY_SYSTEM

	void                    HandlerPKB_LOGIN_PROCESS();
    int                     GetAuthServerGroupNum();
    bool                    IsSyncChannelnfos();
    BYTE *                  GetSerialKeys();
    void                    resetTryReconnect();

    DWORD                   GetLoginFrontEncKey();
    DWORD                   GetAuthUserKey();
    void                    SetAuthUserKey(DWORD userKey);

    bool                    IsAuthFinished();
    bool                    IsAuthConnected();

    bool                    IsTryReconnect();
    void                    SetTryReconnect();
    void                    UnsetTryReconnect();

	bool                    execLoginQuery(TCHAR* strID, TCHAR* strPW);
	bool                    autoexecLoginQuery();

    bool                    CheckAuthFlow();

	void					RECV_AUTH_READY_CMD(MSG_BASE * pMsg);
	void					SEND_AUTH_VERIFY_SYN();
	void					RECV_AUTH_VERIFY_ACK(MSG_BASE * pMsg);
	bool					SEND_AUTH_ID_PASSWORD_CHECK_SYN();

#ifdef _APPLY_KOREA_CHANNELING
	bool					SEND_AUTH_TOKENPASS_SYN();
	void					RECV_AUTH_TOKENPASS_ACK(MSG_BASE * pMsg);
#endif//_APPLY_KOREA_CHANNELING

#ifdef _NHN_USA_CHANNELING
	bool					SEND_AUTH_TOKEN_NORTHA_PASS_SYN();
	void					RECV_AUTH_TOKEN_NORTHA_PASS_ACK(MSG_BASE * pMsg);
#endif//_NHN_USA_CHANNELING(북미채널링)

#ifdef _APPLY_JAPAN_GAMECHU_CHANNELING
    bool					SEND_AUTH_TOKEN_JP_GAMECHU_SYN();
    void					RECV_AUTH_TOKEN_JP_GAMECHU_ACK(MSG_BASE * pMsg);
#endif //_APPLY_JAPAN_GAMECHU_CHANNELING

#ifdef _APPLY_RUSSIA_INGAMBA_CHANNELING
    bool					SEND_AUTH_PASS_RU_INGAMBA_SYN();
    void					RECV_AUTH_PASS_RU_INGAMBA_ACK(MSG_BASE * pMsg);
#endif //_APPLY_RUSSIA_INGAMBA_CHANNELING

#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
	bool					SEND_AUTH_REAUTH_PASS_SYN();
	void					RECV_AUTH_REAUTH_PASS_ACK(MSG_BASE * pMsg);
	bool					RECV_REAUTH_TOKENSTEAM(const nsPacket::ReAuthTokenStream& rReauthToken);
#endif //__NA001334_090420_SERVER_SELECT_FACILITY__

#ifdef __CN_269_MATRIX_CARD_AUTH
	void					RECV_AUTH_MATRIX_CARD_SYN(MSG_BASE * pMsg);
#endif //__CN_269_MATRIX_CARD_AUTH

#if defined(__CN_20100909_SECURITY_TOKEN__)
    void					SEND_AUTH_SECURITY_TOKEN_SYN(const char* security_token_code);
    void					RECV_AUTH_SECURITY_TOKEN_ACK(MSG_BASE* msg_base);
#elif defined(_JP000000_ONETIMEPASSWORD_AUTHFLOW_)
    void					RECV_AUTH_OTP_PASS_CMD(MSG_BASE * pMsg);
    void					SEND_AUTH_OTP_PASS_SYN(TCHAR* szOTPInput);
    void					RECV_AUTH_OTP_PASS_ACK(MSG_BASE * pMsg);
#endif //_JP000000_ONETIMEPASSWORD_AUTHFLOW_ __CN_20100909_SECURITY_TOKEN__

	void					RECV_AUTH_ID_PASSWORD_CHECK_ACK(MSG_BASE * pMsg);
    void                    AuthOk();
	void					SEND_AUTH_SERVER_LIST_SYN();
	void					RECV_AUTH_SERVER_LIST_WORLD_ACK(MSG_BASE * pMsg);
	void					RECV_AUTH_SERVER_LIST_CHANNEL_ACK(MSG_BASE * pMsg);
	void					SEND_AUTH_SERVER_SELECT_SYN(AuthSetverData* pServerData);
	void					RECV_AUTH_SERVER_SELECT_ACK(MSG_BASE * pMsg);
	int						GetAuthGroupNum();
	AuthGroupData*			GetAuthGroupByID(BYTE groupID);
	AuthGroupData*			GetAuthGroupByIndex(BYTE groupIndex);
    AuthSetverData*			GetServerDataByServerName(LPCTSTR ServerName );
    
	void					SetCurrentServerData(AuthSetverData* pServerData);
	AuthSetverData*			GetCurrentServerData();
	void					DeleteCurrentServerData();
	bool					isRecvGroupServerData();

	void					ShowOnOffUI(bool onoff);
	bool					GetTransTime();

#ifdef _JBH_ADD_AGE_LIMIT_MARK
	BYTE					GetCurrentServerLimitAge();
#endif 

    TCHAR *                 GetID();
    TCHAR *                 GetPW();

    bool					WorldSort();

#if defined(_DEBUG) || defined(_GAMECLIENT_VER)
	void					SetIDPW4AutoTest(const TCHAR* ptszID, const TCHAR* ptszPW);
#endif

    //--------------------------------------------------------------------------
    // procedure
    //
public:
    virtual void	        NetworkProc( MSG_BASE * pMsg );         // Pure

private:
    //--------------------------------------------------------------------------
    // member vars
    //
private:
    uiLoginBack*            m_pUILoginBack;
#ifdef _COMMON_CHANNELING_LOGIC_DEFINE
#else
	uiLoginAccount*         m_pUILoginAccount;
#endif //_COMMON_CHANNELING_LOGIC_DEFINE

    uiLoginBackTop*           m_pUILoginBackTop;
    uiLoginServerChannelList* m_pUILoginServerList;

	uiLogin15Year*			m_pUILogin15Year;
#if defined(_JP000000_ONETIMEPASSWORD_AUTHFLOW_) || defined(__CN_20100909_SECURITY_TOKEN__)
    uiLoginOneTimePassword* m_pUILoginOneTimePassword;
#endif //_JP000000_ONETIMEPASSWORD_AUTHFLOW_ __CN_20100909_SECURITY_TOKEN__

    DWORD                   m_dwEncKey;         // 인증키
    DWORD                   m_dwUserKey;        // 사용자 키

    bool                    m_bTryReconnect;    // 재접속 시도

#ifdef __CH1305_090303_ACCOUNT_ID_LENGTH_EXTENTION__
	TCHAR                   m_strTryID[ID_MAX_LEN+1];     // 아이디
	TCHAR                   m_strTryPW[PASSWD_MAX_LEN+1]; // 비번
#else
    TCHAR                   m_strTryID[ID_MAX_LEN];     // 아이디
    TCHAR                   m_strTryPW[PASSWD_MAX_LEN]; // 비번
#endif//__CH1305_090303_ACCOUNT_ID_LENGTH_EXTENTION__

    BYTE		            szSerialKey[AUTH_SERIAL_KEY_LEN];	// 접속 시리얼키(인증절차를 통과해서 발급한 키)
    char		            szSvrIP[IP_MAX_LEN];	//게임 서버 아이피
    DWORD		            dwSvrPort;				//게임 서버 포트

    bool                    m_bAuthFinished;            // 인증완료 여부
    bool                    m_bAuthConnected;           // 인증접속 여부
    int						m_TryCount;

	AuthGroupDataVector		m_AuthGroupData;
	AuthSetverData*			m_pCurrentConnectedServerData;
	bool					m_bRecvGroupData;
	bool					m_bRecvServerData;

	bool					m_bServerListInfo;
#ifdef _APPLY_KOREA_CHANNELING
	WLAUTH_INFO				m_Auth_Info;
#endif//_APPLY_KOREA_CHANNELING
#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__	
	nsPacket::ReAuthTokenStream m_ReAuth_Info;
	bool					m_bServerSelect;
#endif //__NA001334_090420_SERVER_SELECT_FACILITY__
#ifdef _NA_20100307_BETAKEY_SYSTEM
    BetaKeyInfo m_betakey_info;    
#endif//_NA_20100307_BETAKEY_SYSTEM
};

//------------------------------------------------------------------------------
/**
*/
inline
BYTE *
uiLoginMan::GetSerialKeys()
{
    return &szSerialKey[0];
}

//------------------------------------------------------------------------------
/**
*/
inline
DWORD
uiLoginMan::GetLoginFrontEncKey()
{
    return m_dwEncKey;
}

//------------------------------------------------------------------------------
/**
*/
inline
DWORD
uiLoginMan::GetAuthUserKey()
{
    return m_dwUserKey;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
uiLoginMan::SetAuthUserKey(DWORD userKey)
{
    m_dwUserKey = userKey;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
uiLoginMan::IsTryReconnect()
{
    return m_bTryReconnect;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
uiLoginMan::SetTryReconnect()
{
    m_bTryReconnect = true;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
uiLoginMan::UnsetTryReconnect()
{
    m_bTryReconnect = false;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
uiLoginMan::IsAuthFinished()
{
    return m_bAuthFinished;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
uiLoginMan::IsAuthConnected()
{
    return m_bAuthConnected;
}

//------------------------------------------------------------------------------
/**
*/
inline
TCHAR *
uiLoginMan::GetID()
{
    return &m_strTryID[0];
}

//------------------------------------------------------------------------------
/**
*/
inline
TCHAR *
uiLoginMan::GetPW()
{
    return &m_strTryPW[0];
}
//------------------------------------------------------------------------------
#endif  //  __USER_INTERFACE_MANAGER_LOGIN_H__
