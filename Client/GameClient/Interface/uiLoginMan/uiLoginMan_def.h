#ifndef     __UILOGINMAN_DEFINE_H__
#define     __UILOGINMAN_DEFINE_H__

#include <boost/shared_ptr.hpp>
#include <SunAuth/AuthDefine.h>
#include <SunAuth/AuthStruct.h>
#include <SunAuth/AuthProtocol_CF.h>
#include "PacketControl/PacketControl_AuthFlow.h"
 #ifdef _APPLY_KOREA_CHANNELING
	#include "../../include/ISharemem.h"
	#pragma comment( lib, "../../lib/ShareMemory.lib")
 #endif//_APPLY_KOREA_CHANNELING

#ifdef _APPLY_JAPAN_GAMECHU_CHANNELING
#include "../Utility/GeneralFunction.h"
#endif //_APPLY_JAPAN_GAMECHU_CHANNELING

const int KTryMaxCount = 1;
const int KLoginConnected = -1;

struct	IM_LOGIN_MANAGER
{
	static const DWORD LOGIN_BACK;
	static const DWORD LOGIN_ACCOUNT;		
	static const DWORD LOGIN_SERVERLIST;
	static const DWORD LOGIN_CHANNELLIST;	
	static const DWORD LOGIN_15YEAR;
#if defined(_JP000000_ONETIMEPASSWORD_AUTHFLOW_) || defined(__CN_20100909_SECURITY_TOKEN__)
    static const DWORD LOGIN_ONETIMEPASSWORD;
#endif //_JP000000_ONETIMEPASSWORD_AUTHFLOW_ __CN_20100909_SECURITY_TOKEN__    
};

const char  c_strDlgFname_LoginBack[]           = ("Data\\Interface\\1_Login_Window.iwz");
#ifdef _COMMON_CHANNELING_LOGIC_DEFINE
#else
const char  c_strDlgFname_LoginAccount[]        = ("Data\\Interface\\1_1_Login_Window.iwz");
#endif //_COMMON_CHANNELING_LOGIC_DEFINE
const char  c_strDlgFname_LoginServerList[]     = ("Data\\Interface\\2_Server_List.iwz");
const char  c_strDlgFname_LoginChannelList[]    = ("Data\\Interface\\2_2_Server_List_Botton.iwz");
const char  c_strDlgFname_Login15year[]			= ("Data\\Interface\\1_3_Login_15_year.iwz");
#if defined(_JP000000_ONETIMEPASSWORD_AUTHFLOW_) || defined(__CN_20100909_SECURITY_TOKEN__)
const char  c_strDlgFname_LoginOneTimePassword[]= ("Data\\Interface\\1_9_Login_phone.iwz");
#endif //_JP000000_ONETIMEPASSWORD_AUTHFLOW_ __CN_20100909_SECURITY_TOKEN__

#if defined (_CHINA)
	#define MAX_EDIT_PASSLEN	12
#ifdef __CH1305_090303_ACCOUNT_ID_LENGTH_EXTENTION__
	#define MAX_EDIT_IDLEN		50
#else
	#define MAX_EDIT_IDLEN		12
#endif//__CH1305_090303_ACCOUNT_ID_LENGTH_EXTENTION__(중국12자리에서 50자로 늘림)

#elif	defined ( _JAPAN )
	#define MAX_EDIT_PASSLEN	14
	#define MAX_EDIT_IDLEN		18
#else
	#define MAX_EDIT_PASSLEN	12
	#define MAX_EDIT_IDLEN		10
#endif

enum
{
    eAUTH_SERVER_STAT_NONE = 0,
    eAUTH_SERVER_STAT_NORMAL_LEVEL_1,       // 원활
    eAUTH_SERVER_STAT_NORMAL_LEVEL_2,       // 원활
    eAUTH_SERVER_STAT_NORMAL_LEVEL_3,       // 원활
    eAUTH_SERVER_STAT_BUSY_LEVEL_4,         // 보통
    eAUTH_SERVER_STAT_BUSY_LEVEL_5,         // 보통
    eAUTH_SERVER_STAT_BUSY_LEVEL_6,         // 보통
    eAUTH_SERVER_STAT_VERY_BUSY_LEVEL_7,    // 혼잡
    eAUTH_SERVER_STAT_VERY_BUSY_LEVEL_8,    // 혼잡
    eAUTH_SERVER_STAT_VERY_BUSY_LEVEL_9,    // MAX
};

const int c_AUTH_RESULT_IDS[] =
{
    0,  // 0. AUTHRESULT_OK					85168	로그인에 성공하였습니다.
	1,  // 1. AUTHRESULT_INVALIDACCOUNT		85169	로그인 정보가 정확하지 않습니다. 아이디 또는 비밀번호를 정확히 입력해 주세요.
	2,  // 2. AUTHRESULT_SYSTEMERROR		85170	인증서버로부터 응답이 없습니다. 잠시 후 다시 시도해 주시기 바랍니다.
    3,  // 3. AUTHRESULT_NOTEXISTACCOUNT	85171	존재하지 않는 계정입니다.
    4,  // 4. AUTHRESULT_INVALIDPASSWORD	85172	잘못된 암호
    5,  // 5. AUTHRESULT_NOTALLOWEDACCOUNT	85173	사용하시는 계정은 게임 접속이 불가능합니다.\n공식 홈페이지(http://www.sunonline.co.kr) - 고객지원센터로 문의해 주시기 바랍니다.
    6,  // 6. AUTHRESULT_PRECONNECTREMAIN	85174	현재 접속 중인 계정입니다. 잠시 후 다시 접속을 시도해 주시기 바랍니다.
    7,  // 7. AUTHRESULT_RESTRICTADULT		85175	해당 월드는 만 18세 이상 이용 가능한 월드입니다.\n만 18세 미만 고객의 경우, 이블아이 월드를 이용해 주시기 바랍니다.
    2,  // 8. AUTHRESULT_SYSTEMERROR		85170	인증서버로부터 응답이 없습니다. 잠시 후 다시 시도해 주시기 바랍니다.
    22,  // 9. AUTHRESULT_SYSTEMERROR		85190	사용시간이 만료되어 접속할 수 없습니다..
    8,  // 10. AUTHRESULT_PROCESSLOGIN		85176	이미 로그인되어 있는 계정입니다. 잠시 후 다시 접속을 시도해 주시기 바랍니다.
    9,  // 11. AUTHRESULT_ALREADYLOGIN		85177	이미 접속 중인 계정입니다. 잠시 후 다시 접속을 시도해 주시기 바랍니다.
	110,// 12. AUTHRESUKT_ALREADYLOGIN		85278   5회 로그인 실패로 인해 30분간 접속할수 없습니다.
};

const int   c_iMAX_AUTH_RESULT_IDS = 13;

//
//  Packet I/O Boolean
//
enum
{
    ePKBOOL_LOGIN_BASE = 0,
    ePKBOOL_LOGIN_C2S_ASK_VERIFY,
    ePKBOOL_LOGIN_C2S_ASK_AUTH,
    ePKBOOL_LOGIN_C2S_ASK_SVRLIST,
    ePKBOOL_LOGIN_S2C_SERVER_NAME_LIST,
    ePKBOOL_LOGIN_C2S_ASK_SVRSELECT,
    ePKBOOL_LOGIN_MAX,
};

const int kTimeOutLogin = 10;

void _VerifyCallBack_TryReconnect(void);

//------------------------------------------------------------------------------
/**
    인증서버 그룹내에 채널정보
*/
struct AuthSetverData
{
	std::string				m_GroupName;
	std::string				m_ServerName;
	BYTE					m_GroupID;
	BYTE					m_ChannelID;
	BYTE					m_ChannelIndex;
	BYTE					m_CrowdGrade;
	BYTE					m_ConnectStatus;
};

typedef	boost::shared_ptr<AuthSetverData>		AuthSetverData_Ptr;

class AuthGroupData
{
private:
	std::string						m_GroupName;
	BYTE							m_GroupID;
	BYTE							m_LimitedAge;
	int								m_nTotalStatus;
	std::vector<AuthSetverData_Ptr> m_ServerData;

public:
	AuthGroupData()
		:m_GroupID(0),m_LimitedAge(0)
	{

	}
	~AuthGroupData()
	{
		m_GroupID		= 0;
		m_LimitedAge	= 0;
		m_nTotalStatus	= 0;
		m_GroupName.clear();
		m_ServerData.clear();
	}
	void SetGroupData(GROUP_DATA* pGroupData)
	{
		m_GroupName = std::string(pGroupData->m_szGroupName);
		m_GroupID = pGroupData->m_GroupID;
		m_LimitedAge = pGroupData->m_LimitedAge;
		m_nTotalStatus = 0;
	}
	void AddServerData(SERVER_DATA* pServerData)
	{
		if (pServerData->m_CrowdGrade == eAUTH_SERVER_STAT_NONE)
			return;

		if (pServerData->m_GroupID == m_GroupID)
		{
			AuthSetverData_Ptr NewServer_Ptr = AuthSetverData_Ptr(new AuthSetverData);

			NewServer_Ptr->m_GroupName		= m_GroupName;
			NewServer_Ptr->m_ServerName		= std::string(pServerData->m_szChannelName);
			NewServer_Ptr->m_GroupID		= pServerData->m_GroupID;
			NewServer_Ptr->m_ChannelID		= pServerData->m_ChannelID;
			NewServer_Ptr->m_ChannelIndex	= (BYTE)(m_ServerData.size()+1);
			NewServer_Ptr->m_CrowdGrade		= pServerData->m_CrowdGrade;
			NewServer_Ptr->m_ConnectStatus	= pServerData->m_ConnectStatus;

			m_ServerData.push_back(NewServer_Ptr);

			m_nTotalStatus += (int)(pServerData->m_CrowdGrade);
		}		
	}
	const TCHAR* GetAuthGroupName()
	{
		return m_GroupName.c_str();
	}
	int GetAuthGroupID()
	{
		return (int)m_GroupID;
	}
	int GetAuthGroupAge()
	{
		return (int)m_LimitedAge;
	}
	int GetAuthServerDataCount()
	{
		return (int)m_ServerData.size();
	}
	AuthSetverData* GetAuthServerDatabyID(BYTE authSetverID)
	{
		for (size_t i = 0;i < m_ServerData.size();++i)
		{
			if ( authSetverID == m_ServerData[i]->m_ChannelID )
			{
				return m_ServerData[i].get();
			}
		}
		return NULL;
	}

    AuthSetverData* GetAuthServerDatabyName( LPCTSTR authSetverName )
    {
        for (size_t i = 0;i < m_ServerData.size();++i)
        {
            TCHAR temp[1024];
            strcpy(temp, m_ServerData[i]->m_ServerName.c_str());
            if ( strcmp(m_ServerData[i]->m_ServerName.c_str(), authSetverName ) == 0 )
            {
                return m_ServerData[i].get();
            }
        }
        return NULL;
    }

	AuthSetverData* GetAuthServerDatabyIndex(BYTE authSetverIndex)
	{
		if (authSetverIndex > m_ServerData.size() || authSetverIndex < 0)
		{
			return NULL;
		}
		return m_ServerData[authSetverIndex].get();
	}
	int GetTotalStatus()
	{
		if ( m_LimitedAge < 18 )
		{
			return m_nTotalStatus + 1000;
		}
		return m_nTotalStatus;
	}
};

class ParameterAuthInfo
{
public:
    ParameterAuthInfo() { ClearInfo(); }
    ~ParameterAuthInfo() {}

    void SetCommandLineParameter(wchar_t** command_line_arg)
    {
        ClearInfo();

        // command_line_arg[0]은 자신의 실행 경로 및 파일명
        // 인증정보 세팅
        auth_info_ = command_line_arg[1];
        // 유저정보 세팅
        user_info_ = command_line_arg[2];

        if ((auth_info_.empty() != true) && 
            (user_info_.empty() != true))
        {
            is_use_auth_info_ = true;
        }        
    }

    const wchar_t* GetAuthInfo() { return auth_info_.c_str(); }
    const wchar_t* GetUserInfo() { return user_info_.c_str(); }
    bool is_use_auth_info() { return is_use_auth_info_; }

private:
    void ClearInfo()
    {
        auth_info_.clear();
        user_info_.clear();
        is_use_auth_info_ = false;
    }

private:
    bool is_use_auth_info_;
    /// 인증정보
    std::wstring auth_info_;
    /// 유저정보
    std::wstring user_info_;
};

#endif //__UILOGINMAN_DEFINE_H__
