#pragma  once

#include "RecordSet.h"

namespace eXtreme {

class DBC;

typedef void (*CallbackDBCError)(DBC*, HRESULT, _bstr_t, _bstr_t, _bstr_t);

class DBC
{
    enum { _CommandTimeout = 120 };
public:
    DBC();
    virtual ~DBC();

    bool  Open(_bstr_t connStr, long timeout = 5);
    bool  Open(_bstr_t dbName, _bstr_t initDB, _bstr_t account, _bstr_t pw, long timeout = 5);
    void  Close();
    DWORD GetState();

    bool  Query(RecordSet* bind, _bstr_t queryStr);
    bool  CallStoredProcedure(RecordSet* bind, _bstr_t spName, _bstr_t queryStr);


    CallbackDBCError m_callbackError;
    _bstr_t m_lastQuery;

private:
    _ConnectionPtr m_connection;
};

}; //end of namespace 'eXtreme'