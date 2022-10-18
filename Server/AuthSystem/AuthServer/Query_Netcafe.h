#ifndef __AUTHSERVER_QUERY_NETCAFE_H
#define __AUTHSERVER_QUERY_NETCAFE_H
#pragma once

#include "Query_Base.h"

#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
enum eNetCafeLoginType {
    NETCAFE_ONTIME_LOGIN,
    NETCAFE_GAMECHU_LOGIN,
    NETCAFE_LOGOUT,
};
#endif

//#ifdef _NET_CAFE
class QueryNetCafeLog : public QueryObject
{
public:
    QueryNetCafeLog() : QueryObject(QUERYTYPE_NETCATFE_LOG)
    {
        ClearField();
    }

    QueryNetCafeLog(eQUERYTYPE query_type) : QueryObject(query_type)
    {
        ClearField();
        if (query_type == QUERYTYPE_NETCATFE_LOGIN) {
            m_bLogin = true;
        }
    }

    void ClearField();

    VOID    onResult();
    DB_TYPE GetDBType() { return DB_TYPE_NETCAFE; }

    DWORD   m_NetCafeNum;     // 넷카페 고유번호 - return
    int     m_OtpRegistered; // One-time password registered user (_JP000000_ONETIMEPASSWORD_AUTHFLOW_)
    DWORD   m_dwFrontUserId;  // 유저 고유 식별 번호
    bool    m_bLogin;         // 0 : 로그아웃, 1 : 로그인 SP 호출
    DWORD   m_ReAuthKey;
    TCHAR   m_account[ID_MAX_LEN + 1];
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
public:
    void SetNetCafeLoginType(eNetCafeLoginType token_type) {netcafe_logintype_ = token_type;};
private:
    eNetCafeLoginType netcafe_logintype_;
#endif
};

// (WAVERIX) (090708) (CHANGES) x (BUG-FIX)
class QueryNetCafeLogIn : public QueryNetCafeLog
{
public:
    QueryNetCafeLogIn();
};
//#endif

// _JP000000_ONETIMEPASSWORD_AUTHFLOW_
class QueryNetCafe2ndAuth : public QueryObject
{
public:
    QueryNetCafe2ndAuth();

    virtual DB_TYPE GetDBType() { return DB_TYPE_NETCAFE; }
    virtual void onResult();

    void ClearField();

    DWORD front_user_id_;
    struct {
        char account[ID_MAX_LEN + 1];
    } query_info_;
    struct {
        int auth_ret;
    } result_data_;
};

inline void
QueryNetCafe2ndAuth::ClearField()
{
    front_user_id_ = 0;
    ZeroMemory(&query_info_, sizeof(query_info_));
    ZeroMemory(&result_data_, sizeof(result_data_));
}

#endif //__AUTHSERVER_QUERY_NETCAFE_H
