#include "stdafx.h"
#include ".\IODBC.h"
#include ".\QueryResult.h"

//==================================================================================================

IODBC::IODBC()
    : m_hENV(NULL),
      m_hDBC(NULL),
      m_hSTMT(SQL_NULL_HSTMT)
{
}

IODBC::~IODBC()
{
    Release();
}

//==================================================================================================

void IODBC::Release()
{
    Disconnect();
    if (m_hDBC) {
        SQLFreeHandle(SQL_HANDLE_DBC, m_hDBC);
    }
    if (m_hENV) {
        SQLFreeHandle(SQL_HANDLE_ENV, m_hENV);
    }

    m_hDBC = NULL;
    m_hENV = NULL;
}

void IODBC::SQLError(SQLSMALLINT handle_type, SQLHANDLE handle,
                     QueryResult* query)
{
    SHORT sErrorMsgLen = SQL_MAX_MESSAGE_LENGTH;
    SQLRETURN ret = 0;

    ret = SQLGetDiagRec(handle_type, handle, 1, 
                        (SQLCHAR*)query->GetSqlStatePtr(),
                        (SQLINTEGER*)query->GetNativeErrorPtr(),
                        (SQLCHAR*)query->GetErrorMsgPtr(),
                        QueryResult::MAX_SQL_MESSAGE_LENGTH,
                        &sErrorMsgLen);
    if (ret == SQL_SUCCESS)
    {
        DBCDataRef.fnptr_error_message_ex_(static_cast<WORD>(DBCDataRef.index_), query);
        return;
    }
    // fail handling
    switch (ret)
    {
    case SQL_NO_DATA:
        DBCDataRef.fnptr_error_message_(\
            "SQLError() failed SQL_NO_DATA : RecNumber was greater than "
            "the number of diagnostic records that existed for the handle specified in Handle");
        break;
    case SQL_SUCCESS_WITH_INFO:
        DBCDataRef.fnptr_error_message_(\
            "SQLError() failed SQL_SUCCESS_WITH_INFO : The *MessageText buffer was too small");
        break;
    case SQL_INVALID_HANDLE:
        DBCDataRef.fnptr_error_message_("SQLError() failed SQL_INVALID_HANDLE");
        break;
    case SQL_ERROR:
        DBCDataRef.fnptr_error_message_(\
            "SQLError() failed : RecNumber was negative or 0 or BufferLength was less than zero"); 
        break;
    default:
        DBCDataRef.fnptr_error_message_("SQLError() Unknown failed");
        break;
    }
}

bool IODBC::SQLError()
{
    UCHAR szSqlState[MSG_BUF_SIZE];
    UCHAR szErrorMsg[MSG_BUF_SIZE];

    SQLINTEGER fNativeError = 0;
    SWORD cbErrorMsg = MSG_BUF_SIZE;
    RETCODE rc = 0;

    rc = ::SQLError(m_hENV, m_hDBC, m_hSTMT,
                    szSqlState, &fNativeError, szErrorMsg, MSG_BUF_SIZE, &cbErrorMsg);

    if (rc != SQL_ERROR || rc != SQL_NO_DATA)
    {
        //ASSERT(fNativeError != 0x1645);
        ///< ignore change database to master context message
        if (fNativeError != 0x1645)
        {
            char Buffer[255];
            // (WARNING) problem point
            wsprintf(Buffer, "Error Message: %s SqlState: %s, fNativeError: %d \n",
                     szErrorMsg, szSqlState, fNativeError);
            DBCDataRef.fnptr_error_message_(Buffer);
        }
    }
    else
    {
        switch (rc)
        {
        case SQL_NO_DATA:
            DBCDataRef.fnptr_error_message_(\
                "SQLError() failed : return value is SQL_NO_DATA, SQL_NO_DATA_FOUND");
            break;
        default:
            DBCDataRef.fnptr_error_message_("SQLError() failed : return value is SQL_ERROR");
            break;
        }
    }

    return false;
}

bool IODBC::Init(int index,
                 const char* ip_address, const char* database_name,
                 const char* user_name, const char* user_password,
                 fnCallBackMessage fnptr_error_message,
                 fnCallBackMessageEx fnptr_error_message_ex)
{
    IDBCInterface::Init(index, ip_address, database_name,
                        user_name, user_password,
                        fnptr_error_message, fnptr_error_message_ex);

    if (!SQLOK(SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hENV)))
    {
        return false;
    }
    if (!SQLOK(SQLSetEnvAttr(m_hENV, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0)))
    {
        return false;
    }
    if (!SQLOK(SQLAllocHandle(SQL_HANDLE_DBC, m_hENV, &m_hDBC)))
    {
        return false;
    }

    return true;
}

void IODBC::Disconnect()
{
    SQLHSTMT tmpHandle = m_hSTMT;
    InterlockedExchange((LONG volatile*)&m_hSTMT, SQL_NULL_HSTMT);

    if (tmpHandle) {
        SQLFreeHandle(SQL_HANDLE_STMT, tmpHandle);
    }
    if (m_hDBC) {
        SQLDisconnect(m_hDBC);
    }
}

bool IODBC::Connect(DWORD dwLoginTimeout)
{
    ASSERT(m_hDBC);
    //if (!SQLOK(SQLSetConnectAttr(m_hDBC, SQL_ATTR_LOGIN_TIMEOUT, (void*)DB_CONNECT_TIMEOUT, 0)))
    if (!SQLOK(SQLSetConnectAttr(m_hDBC, SQL_ATTR_LOGIN_TIMEOUT, (void*)dwLoginTimeout, 0)))
    {
        return false;
    }

    if (!SQLOK(SQLConnect(m_hDBC, (SQLCHAR *)DBCDataRef.database_name_, SQL_NTS,
                                  (SQLCHAR *)DBCDataRef.user_name_, SQL_NTS,
                                  (SQLCHAR *)DBCDataRef.user_password_, SQL_NTS)))
    {
        return false;
    }

    if (!SQLOK(SQLAllocHandle(SQL_HANDLE_STMT, m_hDBC, &m_hSTMT)))
    {
        return false;
    }

    return true;
}
/*
// removed method
bool IODBC::Execute_GetDataAtExec(Query* query)
{
    char buf[200];
    SQLCHAR binaryPtr[100];
    SQLINTEGER lenBin;
    SQLRETURN ret = 0;
    char* p;
    int nGet;

    ret = SQLExecDirect(m_hSTMT, (SQLCHAR *)query->GetQuery(), SQL_NTS);
    while ((ret = SQLFetch(m_hSTMT)) != SQL_NO_DATA)
    {
        p = buf;
        while ((ret = SQLGetData(m_hSTMT, 1,
            SQL_C_BINARY, binaryPtr, sizeof(binaryPtr), &lenbin)) !=
            SQL_NO_DATA)
        {
            if (lenbin == SQL_NULL_DATA)
            {
                break;
            }
            if (ret == SQL_SUCCESS)
            {
                nGet = lenbin;
            }
            eles
                nGet = 100;
            memcpy(p, binaryPtr, nGet);
            p += nGet;
        }
    }
}
*/

bool IODBC::ExecuteLargeData(Query* query)
{
    SQLRETURN ret = 0;
    query->SetResultColNum(0);
    query->SetResultRowNum(0);
    query->SetError(false);

    LONG lSize = 0;
    SHORT sSqlType = 0;
    SHORT sCType = 0;
    SHORT ioType = 0;
    SQLLEN blobSize = 0;
    BYTE* ptmpPtr;
    const ULONG number_of_parameters = query->GetParameterNum();
    for (ULONG index = 0; index < number_of_parameters; ++index) //!
    {   
        query->GetParameterInfo(index, ioType, ptmpPtr, lSize, sSqlType, sCType);    //!
        if (ioType == SQL_PARAM_INPUT)
        {
            blobSize = SQL_LEN_DATA_AT_EXEC(lSize);
            SQLBindParameter(m_hSTMT, (SQLUSMALLINT)(index + 1),
                             ioType/*SQL_PARAM_INPUT*/, sSqlType,
                             sCType, lSize, 0,
                             (SQLPOINTER)(index + 1), 0, &blobSize);
        }
        else
        {
            blobSize = 0;
            SQLBindParameter(m_hSTMT, (SQLUSMALLINT)(index + 1),
                             ioType/*SQL_PARAM_INPUT*/, sSqlType,
                             sCType, 0, 0,
                             (SQLPOINTER)ptmpPtr, 0, &blobSize);
        }

    };
    //
    SQLCHAR* sql_query_string = (SQLCHAR*)query->GetQuery();
    ret = SQLExecDirect(m_hSTMT, sql_query_string, SQL_NTS);
    if (ret == SQL_NEED_DATA)
    {
        ULONG ParamIndex;
        BYTE* pPutPtr;
        //ULONG ulParamSize = 0;
        LONG lPutSize = 0;
        LONG lRemainSize = 0;

        ret = SQLParamData(m_hSTMT, (SQLPOINTER*)&ParamIndex);
        while (ret == SQL_NEED_DATA)
        {
            query->GetParameterInfo(ParamIndex - 1, ioType, pPutPtr, lRemainSize, sSqlType, sCType);
            // CHANGES: f101026.1L, change logic about putting size
            // to support variable null-terminated string.
            if (sCType == SQL_VARCHAR)
            {
                long str_len = static_cast<long>(_tcslen(reinterpret_cast<char*>(pPutPtr)));
                if (lRemainSize > str_len) {
                    lRemainSize = str_len;
                }
                if (lRemainSize == 0) {
                    SQLPutData(m_hSTMT, (PTR)pPutPtr, lRemainSize);
                }
            }

            for (; lRemainSize > 0 ; lRemainSize -= lPutSize)
            {
                lPutSize = min(600, lRemainSize);
                SQLPutData(m_hSTMT, (PTR)pPutPtr, lPutSize);
                pPutPtr += lPutSize;
            }

            ret = SQLParamData(m_hSTMT, (SQLPOINTER*)&ParamIndex);
        }

        if (ret != SQL_SUCCESS_WITH_INFO && ret != SQL_SUCCESS)
        {
            SQLError(SQL_HANDLE_STMT, m_hSTMT, query);
            goto CLEANUPFAIL;
        }
    }
    else if (ret != SQL_SUCCESS_WITH_INFO && ret != SQL_SUCCESS)
    {
        SQLError(SQL_HANDLE_STMT, m_hSTMT, query);
        goto CLEANUPFAIL;
    }
    //
    SQLNumResultCols(m_hSTMT, (SQLSMALLINT *)query->GetResultColNumPtr());
    SQLRowCount(m_hSTMT, query->GetAffectNumPtr());

    if (query->GetResultColNum() == 0)
    {
        /// update, insert, delete 한 경우
        goto CLEANUPMIDOK;
    }
    else if (query->GetResultColNum() > query->GetMaxColNum())
    {
        /// 쿼리버퍼사이즈에 맞춤
        query->SetResultColNum(query->GetMaxColNum());
    }

    //ASSERT(query->GetMaxRowNum() != 0);
    //ASSERT(query->GetMaxColNum() != 0);

    ULONG ulRow = 0;
    ULONG* pPtr = NULL;
    LONG csize = 0;
    SHORT sType = 0;
    LONG lGetLen = 0;
    while ((ret = SQLFetch(m_hSTMT)) == SQL_SUCCESS ||
           ret == SQL_SUCCESS_WITH_INFO)
    {
        if (ulRow == query->GetMaxRowNum()) {
            break;
        }
        const ULONG number_of_result_cols = query->GetResultColNum();
        for (ULONG i = 0; i < number_of_result_cols; ++i)
        {
            query->GetColumnInfo(ulRow, i, pPtr, csize, sType);
            while ((ret = SQLGetData(m_hSTMT,
                                     static_cast<SQLUSMALLINT>(i + 1),
                                     sType, pPtr, csize, &lGetLen)) != SQL_NO_DATA)
            {
                if (lGetLen == SQL_NULL_DATA) {
                    break;
                };
                if (csize < lGetLen) {
                    break;
                }
                // SQL_SUCCESS_WITH_INFO or SQL_SUCCESS
                pPtr += lGetLen;
            }
        }
        ++ulRow;
    }

    query->SetResultRowNum(ulRow);

CLEANUPMIDOK:
    SQLCloseCursor(m_hSTMT);
    SQLFreeStmt(m_hSTMT, SQL_UNBIND);
    SQLFreeStmt(m_hSTMT, SQL_RESET_PARAMS);
    return true;

    //CLEANUPMIDOK:
    //  SQLCloseCursor(m_hSTMT);
    //  return true;

CLEANUPFAIL:
    // 초기값 셋팅
    query->SetAffectNum(0);
    query->SetResultRowNum(0);
    query->SetResultColNum(0);
    query->SetError(true);

    SQLCloseCursor(m_hSTMT);
    SQLFreeStmt(m_hSTMT, SQL_UNBIND);
    return false;
}

bool IODBC::Execute(Query* query)
{
    query->SetResultColNum(0);
    query->SetResultRowNum(0);
    query->SetError(false);

    if (!SQLOK(SQLExecDirect(m_hSTMT, (SQLCHAR *)query->GetQuery(), SQL_NTS)))
    {
        goto CLEANUPFAIL;
    }

    if (!SQLOK(SQLNumResultCols(m_hSTMT, (SQLSMALLINT *)query->GetResultColNumPtr())))
    {
        goto CLEANUPFAIL;
    }
    //
    SQLRowCount(m_hSTMT, query->GetAffectNumPtr());
    //
    if (query->GetResultColNum() == 0)
    {
        //SQLRowCount(m_hSTMT, query->GetAffectNumPtr());
        /// update, insert, delete 한 경우
        goto CLEANUPMIDOK;
    }
    else if (query->GetResultColNum() > query->GetMaxColNum())
    {
        /// 쿼리버퍼사이즈에 맞춤
        query->SetResultColNum(query->GetMaxColNum());
    }

    ASSERT(query->GetMaxRowNum() != 0);
    ASSERT(query->GetMaxColNum() != 0);

    SQLRETURN ret = 0;
    ULONG ulRow = 0;
    ULONG* pPtr = NULL;
    LONG csize = 0;
    SHORT sType = 0; 
    ULONG* pColLenPtr = NULL;
    do 
    {
        if (ulRow == query->GetMaxRowNum()) {
            break;
        }
        const ULONG number_of_result_cols = query->GetResultColNum();
        for (ULONG i = 0; i < number_of_result_cols; ++i)
        {
            pColLenPtr = query->GetColumnLengthPtr(i);
            query->GetColumnInfo(ulRow, i, pPtr, csize, sType);   
            SQLBindCol(m_hSTMT, (SQLUSMALLINT)i+1, sType, pPtr, csize, (SQLLEN *)pColLenPtr);
        }

    } while (((ret = SQLFetch(m_hSTMT)) == SQL_SUCCESS ||
             ret == SQL_SUCCESS_WITH_INFO) &&
             ++ulRow);

    if (ret != SQL_SUCCESS &&
        ret != SQL_SUCCESS_WITH_INFO &&
        ret != SQL_NO_DATA)
    {
        SQLError(SQL_HANDLE_STMT, m_hSTMT, query);
    }
    //
    query->SetResultRowNum(ulRow);
    //
    SQLCloseCursor(m_hSTMT);
    SQLFreeStmt(m_hSTMT, SQL_UNBIND);
    SQLFreeStmt(m_hSTMT, SQL_RESET_PARAMS);

    return true;

CLEANUPMIDOK:
    SQLCloseCursor(m_hSTMT);
    return true;

CLEANUPFAIL:
    // 초기값 셋팅
    query->SetAffectNum(0);
    query->SetResultRowNum(0);
    query->SetResultColNum(0);
    query->SetError(true);

    SQLCloseCursor(m_hSTMT);
    SQLFreeStmt(m_hSTMT, SQL_UNBIND);
    return false;
}
