#pragma once
#ifndef __DBC_INTERFACE_H__
#define __DBC_INTERFACE_H__

//==================================================================================================
/// Database 모듈에서 SQL 관련 Library를 공통적으로 사용하기위해 제공하는 인터페이스 추상 클래스
/**
    @author
        Kim Min Wook < taiyo@webzen.co.kr > 
    @since
        2004. 10. 20
    @remarks
        - 인스턴스를 가지지 않는 인터페이스 클래스
    @note
        - 라이브러리를 구현 할때 IDBCInterface를 상속 받는다.
*/
//==================================================================================================

class QueryResult;

//==================================================================================================

class IDBCInterface
{
public:
    enum
    {
        DB_CONNECT_TIMEOUT = 3,
        MAX_IP_ADDRESS_LENGTH = 15,
        MAX_NAME_LENGTH = 64,
        MSG_BUF_SIZE = 128,
    };
    //
    IDBCInterface();
    virtual ~IDBCInterface();

    virtual void Release() {}
    virtual bool Init(int index,
                      const char* ip_address, const char* database_name,
                      const char* user_name, const char* user_password,
                      fnCallBackMessage fnptr_error_message,
                      fnCallBackMessageEx fnptr_error_message_ex);
    virtual bool Connect(DWORD dwLoginTimeout) {
        __UNUSED(dwLoginTimeout);
        return false;
    }
    virtual void Disconnect() {
    }
    virtual bool Execute(Query* query) {
        __UNUSED(query);
        return false;
    }
    virtual bool ExecuteLargeData(Query* query) {
        __UNUSED(query);
        return false;
    }

protected:
    struct IDBCData {
        int index_;
        fnCallBackMessage   fnptr_error_message_;
        fnCallBackMessageEx fnptr_error_message_ex_;
        //
        char ip_address_[MAX_IP_ADDRESS_LENGTH];
        char database_name_[MAX_NAME_LENGTH];
        char user_name_[MAX_NAME_LENGTH];
        char user_password_[MAX_NAME_LENGTH];
        char m_pszDeviceName[MAX_NAME_LENGTH];
    };
    const IDBCData& DBCDataRef;
private:
    IDBCData dbc_data_;
    //
    IDBCInterface(const IDBCInterface& rhs);
    void operator=(const IDBCInterface& rhs);
};

//==================================================================================================

#endif // __DBC_INTERFACE_H__