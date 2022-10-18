#pragma once
#ifndef __DBC_INTERFACE_H__
#define __DBC_INTERFACE_H__

//==================================================================================================
/// Database ��⿡�� SQL ���� Library�� ���������� ����ϱ����� �����ϴ� �������̽� �߻� Ŭ����
/**
    @author
        Kim Min Wook < taiyo@webzen.co.kr > 
    @since
        2004. 10. 20
    @remarks
        - �ν��Ͻ��� ������ �ʴ� �������̽� Ŭ����
    @note
        - ���̺귯���� ���� �Ҷ� IDBCInterface�� ��� �޴´�.
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