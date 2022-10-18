#pragma once
#ifndef __IODBC_H__
#define __IODBC_H__

//==================================================================================================
/// ODBC를 사용하기 위해 ODBC API를 Wrapping한 Class
/**
    @author
        Kim Min Wook < taiyo@webzen.co.kr > 
    @since
        2004. 10. 20
    @remarks
        - ODBC를 구현한 클래스
        - 다른 Library와 달리 ODBC 설정만 해준다면 모든 DB Server에서 사용할 수 있다.
    @note
        - Init(), Connect(), Execute(), Release()순으로 사용한다.
*/
//==================================================================================================

#include "IDBCInterface.h"
#include <sql.h>
#include <sqlext.h>

class QueryResult;

//==================================================================================================

class IODBC :   public IDBCInterface
{
public:
    IODBC();
    virtual ~IODBC();

    virtual void Release();
    virtual bool Init(int index,
                      const char* ip_address, const char* database_name,
                      const char* user_name, const char* user_password,
                      fnCallBackMessage fnptr_error_message,
                      fnCallBackMessageEx fnptr_error_message_ex);
    virtual bool Connect(DWORD dwLoginTimeout);
    virtual void Disconnect();
    virtual bool Execute(Query* query);
    virtual bool ExecuteLargeData(Query* query);

    bool SQLOK(SQLRETURN ret);
private:
    bool SQLError();
    void SQLError(SQLSMALLINT handle_type, SQLHANDLE handle, QueryResult* query);
    SQLHANDLE m_hENV;
    SQLHDBC   m_hDBC; 
    SQLHSTMT  m_hSTMT;
};

//==================================================================================================

inline bool IODBC::SQLOK(SQLRETURN ret) 
{ 
    switch (ret)
    {
    case SQL_SUCCESS:
        return true;
    case SQL_SUCCESS_WITH_INFO:
        SQLError();
        return true;
    case SQL_NEED_DATA:
        return true;
    }
    SQLError();
    return false;
}

#endif // __IODBC_H__