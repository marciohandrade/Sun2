#pragma once

#include "Query_Base.h"
#ifdef _NA_20100307_BETAKEY_SYSTEM
#include <BetaKey.h>
#endif

//////////////////////////////////////////////////////////////////////////
//  ������û
class QueryAuth : public QueryObject
{
    enum eQueryResultCode {
        eQueryResult_Undefined  = -1,
        eQueryResult_Success    = 0,
        eQueryResult_Fail       = 1, // can't find user
        //// for NorthA, Already Exist, but not matched 'UserKey' & 'UserID'
        //eQueryResult_FailKeyAndId = 2,
    };
public:
    QueryAuth();

    VOID onResult();
    virtual DB_TYPE GetDBType() { return DB_TYPE_AUTH; }

    inline eAuthToken GetTokenType() const { return m_TokenType; }
    void Configuration(eAuthToken token_type);
    eAuthToken m_TokenType;
    nsPacket::ReAuthInfoBase::ReAuthKeyType m_ReAuthKey; //__NA001334_090420_SERVER_SELECT_FACILITY__
    DWORD m_front_user_id; // login front�� ����Ǿ� �ִ� ���� Ŀ�ؼ� ���� ID
    int m_DbResult; // Result ó�� �ʿ��� ��� ó��
    DB_LOGIN_USER m_Record;

    TCHAR   m_account[ID_MAX_LEN + 1];
    TCHAR   m_password[PASSWD_MAX_LEN];
    TCHAR   m_clientIp[IP_MAX_LEN];
    TCHAR   m_localIp[IP_MAX_LEN];
    DB_LOGIN_USER::AuthToken m_AuthToken;   //_CHANNELING_AUTH_FLOW
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
    GAMECHUUSN gamechu_usn_;    
#endif

private:
    //#ifdef _CHANNELING_AUTH_FLOW
    BOOLEAN ValidClientAuthInfo();
    //#else
    //  BOOLEAN CheckPassword(LPTSTR key, LPTSTR password, LPTSTR recvPasswd);
    //#endif
    BOOLEAN CheckPassword();
    BOOLEAN ReauthTokenAuthentication();

    AccountInfo* CheckOverlapLogin(DWORD userGuid, LPTSTR account,
        DWORD login_front_id, DWORD front_user_id, BYTE& byResult);
};

//////////////////////////////////////////////////////////////////////////
class QueryUserStsChange : public QueryObject
{
public:
    QueryUserStsChange() : QueryObject(QUERYTYPE_USERSTS)
    {
    }

    DB_TYPE GetDBType() { return DB_TYPE_AUTH; }
};

//////////////////////////////////////////////////////////////////////////
class QueryRecoverLocation : public QueryObject
{
public:
    QueryRecoverLocation() : QueryObject(QUERYTYPE_RECOVERSTS)
    {
        m_recordSet.RegisterVariable(&m_Record.UserGuid, "UserGuid");
        m_recordSet.RegisterVariable(m_Record.UserID, "UserID");
        m_recordSet.RegisterVariable(&m_Record.ServerGuid, "ServerGuid");
        m_recordSet.RegisterVariable(&m_Record.ConnectSts, "ConnectSts");
        m_recordSet.RegisterVariable(m_Record.IpPublicAddr, "IpPublicAddr");
        m_recordSet.RegisterVariable(m_Record.IpLocalAddr, "IpLocalAddr");
        m_recordSet.RegisterVariable(m_Record.AuthUserGuid, "AuthUserGuid");
        m_recordSet.RegisterVariable(&m_Record.CompGuid, "CompGuid");
        m_recordSet.RegisterVariable(&m_Record.AuthKey, "AuthKey");
        m_recordSet.RegisterVariable(&m_Record.ConnectTime, "ConnectTime");
    }

    VOID    onResult();
    DB_TYPE GetDBType() { return DB_TYPE_AUTH; }

    DB_RECOVER_LOCATION m_Record;
};

//////////////////////////////////////////////////////////////////////////
class QueryAuthFailCount : public QueryObject
{
public:
    QueryAuthFailCount() : QueryObject(QUERYTYPE_INCRESEAUTHFAILCOUNT)
    {
        m_btAuthFailCount = 0;
        m_btFlag = m_btAuthType = 0;
        CHECK_ID_LENGTH_BUFFER(m_account);
        ZeroMemory(m_account, sizeof(m_account));
        ZeroMemory(m_clientIP, sizeof(m_clientIP));
    }

    VOID    onResult();
    DB_TYPE GetDBType() { return DB_TYPE_AUTH; }

    DWORD   m_btAuthFailCount; // ������ ���� Ƚ���� ����
    BYTE    m_btFlag;          // ���� �������� ���������� ���� �÷���
    BYTE    m_btAuthType;      // ���� Ÿ��(���н� � ����Ÿ������ �����Ѵ�)
    DWORD   m_front_user_id;   // ���� ��� �α��� ����Ʈ�� Ű
    TCHAR   m_account[ID_MAX_LEN + 1];
    TCHAR   m_clientIP[IP_MAX_LEN];
};


//////////////////////////////////////////////////////////////////////////


class QueryCheckPassword : public QueryObject
{
public:
    QueryCheckPassword();

    VOID onResult();
    virtual DB_TYPE GetDBType() { return DB_TYPE_AUTH; }

    eAuthToken m_TokenType;
    nsPacket::ReAuthInfoBase::ReAuthKeyType m_ReAuthKey; //__NA001334_090420_SERVER_SELECT_FACILITY__
    DWORD m_front_user_id; // login front�� ����Ǿ� �ִ� ���� Ŀ�ؼ� ���� ID
    int m_DbResult; // Result ó�� �ʿ��� ��� ó��

    TCHAR   m_account[ID_MAX_LEN + 1];
    TCHAR   m_password[PASSWD_MAX_LEN + 1];
    TCHAR   m_clientIp[IP_MAX_LEN];
    TCHAR   m_localIp[IP_MAX_LEN];
};

