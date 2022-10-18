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
#endif//__CH1305_090303_ACCOUNT_ID_LENGTH_EXTENTION__(�߱�12�ڸ����� 50�ڷ� �ø�)

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
    eAUTH_SERVER_STAT_NORMAL_LEVEL_1,       // ��Ȱ
    eAUTH_SERVER_STAT_NORMAL_LEVEL_2,       // ��Ȱ
    eAUTH_SERVER_STAT_NORMAL_LEVEL_3,       // ��Ȱ
    eAUTH_SERVER_STAT_BUSY_LEVEL_4,         // ����
    eAUTH_SERVER_STAT_BUSY_LEVEL_5,         // ����
    eAUTH_SERVER_STAT_BUSY_LEVEL_6,         // ����
    eAUTH_SERVER_STAT_VERY_BUSY_LEVEL_7,    // ȥ��
    eAUTH_SERVER_STAT_VERY_BUSY_LEVEL_8,    // ȥ��
    eAUTH_SERVER_STAT_VERY_BUSY_LEVEL_9,    // MAX
};

const int c_AUTH_RESULT_IDS[] =
{
    0,  // 0. AUTHRESULT_OK					85168	�α��ο� �����Ͽ����ϴ�.
	1,  // 1. AUTHRESULT_INVALIDACCOUNT		85169	�α��� ������ ��Ȯ���� �ʽ��ϴ�. ���̵� �Ǵ� ��й�ȣ�� ��Ȯ�� �Է��� �ּ���.
	2,  // 2. AUTHRESULT_SYSTEMERROR		85170	���������κ��� ������ �����ϴ�. ��� �� �ٽ� �õ��� �ֽñ� �ٶ��ϴ�.
    3,  // 3. AUTHRESULT_NOTEXISTACCOUNT	85171	�������� �ʴ� �����Դϴ�.
    4,  // 4. AUTHRESULT_INVALIDPASSWORD	85172	�߸��� ��ȣ
    5,  // 5. AUTHRESULT_NOTALLOWEDACCOUNT	85173	����Ͻô� ������ ���� ������ �Ұ����մϴ�.\n���� Ȩ������(http://www.sunonline.co.kr) - ���������ͷ� ������ �ֽñ� �ٶ��ϴ�.
    6,  // 6. AUTHRESULT_PRECONNECTREMAIN	85174	���� ���� ���� �����Դϴ�. ��� �� �ٽ� ������ �õ��� �ֽñ� �ٶ��ϴ�.
    7,  // 7. AUTHRESULT_RESTRICTADULT		85175	�ش� ����� �� 18�� �̻� �̿� ������ �����Դϴ�.\n�� 18�� �̸� ���� ���, �̺���� ���带 �̿��� �ֽñ� �ٶ��ϴ�.
    2,  // 8. AUTHRESULT_SYSTEMERROR		85170	���������κ��� ������ �����ϴ�. ��� �� �ٽ� �õ��� �ֽñ� �ٶ��ϴ�.
    22,  // 9. AUTHRESULT_SYSTEMERROR		85190	���ð��� ����Ǿ� ������ �� �����ϴ�..
    8,  // 10. AUTHRESULT_PROCESSLOGIN		85176	�̹� �α��εǾ� �ִ� �����Դϴ�. ��� �� �ٽ� ������ �õ��� �ֽñ� �ٶ��ϴ�.
    9,  // 11. AUTHRESULT_ALREADYLOGIN		85177	�̹� ���� ���� �����Դϴ�. ��� �� �ٽ� ������ �õ��� �ֽñ� �ٶ��ϴ�.
	110,// 12. AUTHRESUKT_ALREADYLOGIN		85278   5ȸ �α��� ���з� ���� 30�а� �����Ҽ� �����ϴ�.
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
    �������� �׷쳻�� ä������
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

        // command_line_arg[0]�� �ڽ��� ���� ��� �� ���ϸ�
        // �������� ����
        auth_info_ = command_line_arg[1];
        // �������� ����
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
    /// ��������
    std::wstring auth_info_;
    /// ��������
    std::wstring user_info_;
};

#endif //__UILOGINMAN_DEFINE_H__
