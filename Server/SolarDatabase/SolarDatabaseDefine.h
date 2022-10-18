#ifndef __SOLAR_DATABAES_DEFINE_H__
#define __SOLAR_DATABAES_DEFINE_H__

//==================================================================================================
//
#include <sqlext.h>

//==================================================================================================
#define MAX_COL_NUM     255


enum eDBCInterfaceType
{
    DBCInterfaceType_ODBC   = 0x01,
    DBCInterfaceType_OLEDB  = 0x02,
    DBCInterfaceType_SQLLIB = 0x03,
};

//==================================================================================================

typedef unsigned char   UCHAR;
typedef long int        SDWORD;
//
class QueryResult;
typedef QueryResult     Query;
//
typedef void (*fnCallBackMessage)(char* szMessage);
typedef void (*fnCallBackMessageEx)(WORD dwDBIndex, QueryResult* pData);
typedef void (*fnCallBackQueryResult)(DWORD dwIndex, QueryResult* pData);

//==================================================================================================

struct SolarDatabaseDesc
{
    eDBCInterfaceType       database_module_type;
    DWORD                   query_pool_size;
    WORD                    query_processor_num; ///<  1개이상일 경우 쿼리의 동기화는 확신 할 수 없다.
    char*                   database_ip;
    char*                   database_name;
    char*                   user_name;
    char*                   user_password;
    DWORD                   login_timeout; //< 로그인 타입아웃 타임, if value is equal to 0, there is no timeout
    fnCallBackQueryResult   fnptr_query_result;
    fnCallBackMessage       fnptr_error_message;
    fnCallBackMessageEx     fnptr_error_message_ex;
};


#ifdef SOLARDATABASE_EXPORTS
#define SOLARDATABASE_API __declspec(dllexport)
#else
#define SOLARDATABASE_API __declspec(dllimport)
#endif


//=================================================================================================
/**
    @remarks
        사용자가 사용하는 QueryResult custom class의 멤버를 바인딩하기 위한 DEFINE
*/
//=================================================================================================
#define _INIT_PARAM_DATA() \
    virtual inline eQUERYTYPE GetQueryType() { return eLargeQuery; }

#define _BEGIN_PARAM_DATA(_tagType, _tagPtr, _nParam) \
    virtual inline ULONG GetParameterNum() { return _nParam; } \
    virtual inline eQUERYTYPE GetQueryType() { return eLargeQuery; } \
    virtual inline VOID GetParameterInfo(ULONG ulParamIndex, SHORT& ioType, BYTE*& pParamPtr, \
                                         LONG& lSize, SHORT& sqlType, SHORT& cType) \
    { \
        _tagType* pTagPtr = _tagPtr; \
        switch (ulParamIndex) \
        {

#define _BINDING_PARAM_PTR(_paramIndex, _paramPtr, _ioType) \
        case _paramIndex: \
            { \
                pParamPtr = (BYTE *)(pTagPtr->_paramPtr); \
                lSize = sizeof(pTagPtr->_paramPtr); \
                sqlType = _GET_DB_TYPE(pTagPtr->_paramPtr); \
                cType = static_cast<short>(_GetODBC_C_Type(pTagPtr->_paramPtr)); \
            } \
            ioType = static_cast<short>(_ioType); \
            return;

#define _BINDING_PARAM_PTR_WITH_SIZE(_paramIndex, _paramPtr, _paramSize, _ioType) \
        case _paramIndex: \
            { \
                pParamPtr = (BYTE *)(pTagPtr->_paramPtr); \
                lSize = _paramSize; \
                sqlType = _GET_DB_TYPE(pTagPtr->_paramPtr); \
                cType = static_cast<short>(_GetODBC_C_Type(pTagPtr->_paramPtr)); \
            } \
            ioType = static_cast<short>(_ioType); \
            return;

#define _BINDING_PARAM(_paramIndex, _param, _ioType) \
        case _paramIndex: \
            { \
                pParamPtr = (BYTE *)&(pTagPtr->_param); \
                lSize = sizeof(pTagPtr->_param); \
                sqlType = _GET_DB_TYPE(pTagPtr->_param); \
                cType = static_cast<short>(_GetODBC_C_Type(pTagPtr->_param)); \
                ioType = static_cast<short>(_ioType); \
            } \
            return;

#define _END_PARAM_DATA() \
        } \
        return; \
    }


#define _INIT_BINDING_DATA() \
    virtual inline ULONG    GetMaxRowNum() const { return 0; } \
    virtual inline ULONG    GetMaxColNum() const { return 0; } \
    virtual inline BYTE*    GetBuffer(ULONG ulRow) { ulRow; return NULL; }

#define _BEGIN_BINDING_DATA(_tagType, col_arr, col_len_arr, nrow, ncol) \
    typedef _tagType    TagType;    \
    virtual inline ULONG    GetMaxRowNum() const { return nrow; } \
    virtual inline ULONG    GetMaxColNum() const { return ncol; } \
    virtual inline BYTE*    GetBuffer(ULONG ulRow) { return (BYTE *)&col_arr[ulRow]; }  \
    virtual inline ULONG*   GetColumnLengthPtr(ULONG col) { \
        return &col_len_arr[col]; \
    } \
    virtual inline VOID     GetColumnInfo(ULONG row, ULONG col, ULONG*& pColPtr, \
                                          LONG& lSize, SHORT& sColType) \
    { \
        TagType* pTag = col_arr; \
        switch (col) \
        {

#ifdef _USING_OLEDB
    #define _BINDING_COLUMN_PTR(col_num, col_data_ptr) \
        case col_num: \
            { \
                *pColPtr =  (size_t)&(((TagType *)0)->col_data_ptr); \
                lSize = sizeof(pTag[row].col_data_ptr); \
                sColType = _GET_DB_TYPE(pTag[row].col_data_ptr); \
            } \
            return;

    #define _BINDING_COLUMN(col_num, col_data) \
        case col_num: \
            { \
                *pColPtr =  (size_t)&(((TagType *)0)->col_data); \
                lSize = sizeof(pTag[row].col_data); \
                sColType = _GET_DB_TYPE(pTag[row].col_data); \
            } \
            return;

#else //if !defined(_USING_OLEDB)
    #define _BINDING_COLUMN_PTR(col_num, col_data_ptr) \
        case col_num: \
            { \
                pColPtr = (ULONG *)(pTag[row].col_data_ptr); \
                lSize = sizeof(pTag[row].col_data_ptr); \
                sColType = _GET_DB_TYPE(pTag[row].col_data_ptr); \
            } \
            return;

    #define _BINDING_COLUMN(col_num, col_data) \
        case col_num: \
            { \
                pColPtr = (ULONG *)&(pTag[row].col_data); \
                lSize = sizeof(pTag[row].col_data); \
                sColType = _GET_DB_TYPE(pTag[row].col_data); \
            } \
            return;
#endif

#define _END_BINDING_DATA() \
        } \
        return; \
    }

//==================================================================================================

#define _TINYBIND      1
#define _SMALLBIND     2
#define _INTBIND       3
#define _CHARBIND      4
#define _BITBIND       6
#define _FLT8BIND      9
#define _NTBSTRINGBIND 11
#define _FLT4BIND      14

#if !defined(_LIB_IGNORE)

  #if defined(_USING_ODBC)
    #define _GET_DB_TYPE(ctype) _GetODBCType(ctype)
  #elif defined(_USING_OLEDB)
    #define _GET_DB_TYPE(ctype) _GetOLEDBType(ctype)
  #elif defined(_USING_SQLLIB)
    #define _GET_DB_TYPE(ctype) _GetSQLLIBType(ctype)
  #else
    #error DB-Library option not defined!! (_USING_ODBC or _USING_OLEDB or _USING_SQLLIB)
  #endif 

#endif //!defined(_LIB_IGNORE)

//==================================================================================================

#define _DEFINE_ODBC_TYPE_FUNCTION(ctype, odbctype) \
    inline SQLSMALLINT _GetODBCType(ctype) { \
        return odbctype; \
    }

    _DEFINE_ODBC_TYPE_FUNCTION(BYTE*,            SQL_C_BINARY); // binary or varbinary
    _DEFINE_ODBC_TYPE_FUNCTION(const CHAR*,      SQL_C_CHAR)
    _DEFINE_ODBC_TYPE_FUNCTION(signed char,      SQL_C_CHAR)

    _DEFINE_ODBC_TYPE_FUNCTION(__int64,          SQL_C_SBIGINT)
    _DEFINE_ODBC_TYPE_FUNCTION(unsigned __int64, SQL_C_UBIGINT)

    _DEFINE_ODBC_TYPE_FUNCTION(SHORT,            SQL_C_SHORT)  
    _DEFINE_ODBC_TYPE_FUNCTION(int,              SQL_C_LONG)
    _DEFINE_ODBC_TYPE_FUNCTION(LONG,             SQL_C_LONG)

    _DEFINE_ODBC_TYPE_FUNCTION(BYTE,             SQL_C_UTINYINT)
    _DEFINE_ODBC_TYPE_FUNCTION(unsigned short,   SQL_C_LONG)
    _DEFINE_ODBC_TYPE_FUNCTION(unsigned int,     SQL_C_LONG)
    _DEFINE_ODBC_TYPE_FUNCTION(unsigned long,    SQL_C_LONG)
    _DEFINE_ODBC_TYPE_FUNCTION(float,            SQL_C_DOUBLE)
    _DEFINE_ODBC_TYPE_FUNCTION(double,           SQL_C_DOUBLE)

    // ms-help://MS.VSCC.2003/MS.MSDNQTR.2003FEB.1042/acdata/ac_8_qd_12_8lt3.htm
#define _DEFINE_OLEDB_TYPE_FUNCTION(ctype, oledbtype) \
    inline DBTYPE _GetOLEDBType(ctype) { \
        return oledbtype; \
    }

    _DEFINE_OLEDB_TYPE_FUNCTION(BYTE*, DBTYPE_BYTES) // binary or varbinary
    _DEFINE_OLEDB_TYPE_FUNCTION(CHAR*, DBTYPE_STR)
    _DEFINE_OLEDB_TYPE_FUNCTION(const CHAR*, DBTYPE_STR | DBTYPE_BYREF)

    _DEFINE_OLEDB_TYPE_FUNCTION(__int64,          DBTYPE_I8)
    _DEFINE_OLEDB_TYPE_FUNCTION(unsigned __int64, DBTYPE_UI8)

    _DEFINE_OLEDB_TYPE_FUNCTION(signed char,    DBTYPE_I1)
    _DEFINE_OLEDB_TYPE_FUNCTION(SHORT,          DBTYPE_I2)     // DBTYPE_BOOL
    _DEFINE_OLEDB_TYPE_FUNCTION(int,            DBTYPE_I4)
    _DEFINE_OLEDB_TYPE_FUNCTION(LONG ,          DBTYPE_I4)     // DBTYPE_ERROR (SCODE)
    _DEFINE_OLEDB_TYPE_FUNCTION(LARGE_INTEGER,  DBTYPE_I8)     // DBTYPE_CY
    _DEFINE_OLEDB_TYPE_FUNCTION(BYTE ,          DBTYPE_UI1)
    _DEFINE_OLEDB_TYPE_FUNCTION(char ,          DBTYPE_UI1)
    _DEFINE_OLEDB_TYPE_FUNCTION(unsigned short, DBTYPE_UI2)
    _DEFINE_OLEDB_TYPE_FUNCTION(unsigned int,   DBTYPE_UI4)
    _DEFINE_OLEDB_TYPE_FUNCTION(unsigned long,  DBTYPE_UI4)
    _DEFINE_OLEDB_TYPE_FUNCTION(ULARGE_INTEGER, DBTYPE_UI8)
    _DEFINE_OLEDB_TYPE_FUNCTION(float,          DBTYPE_R4)
    _DEFINE_OLEDB_TYPE_FUNCTION(double ,        DBTYPE_R8)     // DBTYPE_DATE
    _DEFINE_OLEDB_TYPE_FUNCTION(DECIMAL,        DBTYPE_DECIMAL)


#define _DEFINE_SQLLIB_TYPE_FUNCTION(ctype, sqllibtype) \
    inline INT _GetSQLLIBType(ctype) { \
        return sqllibtype; \
    }

    _DEFINE_SQLLIB_TYPE_FUNCTION(const CHAR*,      _NTBSTRINGBIND)
    _DEFINE_SQLLIB_TYPE_FUNCTION(signed char,      _CHARBIND)

    _DEFINE_SQLLIB_TYPE_FUNCTION(__int64,          _INTBIND)
    _DEFINE_SQLLIB_TYPE_FUNCTION(unsigned __int64, _INTBIND)

    _DEFINE_SQLLIB_TYPE_FUNCTION(SHORT,            _SMALLBIND)
    _DEFINE_SQLLIB_TYPE_FUNCTION(int,              _INTBIND)
    _DEFINE_SQLLIB_TYPE_FUNCTION(LONG,             _INTBIND)

    _DEFINE_SQLLIB_TYPE_FUNCTION(BYTE,             _TINYBIND)
    _DEFINE_SQLLIB_TYPE_FUNCTION(unsigned short,   _SMALLBIND)
    _DEFINE_SQLLIB_TYPE_FUNCTION(unsigned int,     _INTBIND)
    _DEFINE_SQLLIB_TYPE_FUNCTION(unsigned long,    _INTBIND)
    _DEFINE_SQLLIB_TYPE_FUNCTION(float,            _FLT4BIND)
    _DEFINE_SQLLIB_TYPE_FUNCTION(double,           _FLT8BIND)


#define _DEFINE_ODBC_C_TYPE_FUNCTION(ctype, odbc_c_type) \
    inline int _GetODBC_C_Type(ctype) { \
        return odbc_c_type; \
    }

    _DEFINE_ODBC_C_TYPE_FUNCTION(BYTE*,             SQL_VARBINARY) // binary or varbinary
    _DEFINE_ODBC_C_TYPE_FUNCTION(const CHAR*,       SQL_VARCHAR)
    _DEFINE_ODBC_C_TYPE_FUNCTION(signed char,       SQL_CHAR)

    _DEFINE_ODBC_C_TYPE_FUNCTION(__int64,           SQL_BIGINT)
    _DEFINE_ODBC_C_TYPE_FUNCTION(unsigned __int64,  SQL_BIGINT)

    _DEFINE_ODBC_C_TYPE_FUNCTION(SHORT,             SQL_SMALLINT)
    _DEFINE_ODBC_C_TYPE_FUNCTION(int,               SQL_INTEGER)
    _DEFINE_ODBC_C_TYPE_FUNCTION(LONG,              SQL_INTEGER)

    _DEFINE_ODBC_C_TYPE_FUNCTION(BYTE,              SQL_CHAR)
    _DEFINE_ODBC_C_TYPE_FUNCTION(char,              SQL_CHAR)
    _DEFINE_ODBC_C_TYPE_FUNCTION(unsigned short,    SQL_SMALLINT)
    _DEFINE_ODBC_C_TYPE_FUNCTION(unsigned int,      SQL_INTEGER)
    _DEFINE_ODBC_C_TYPE_FUNCTION(unsigned long,     SQL_INTEGER)
    _DEFINE_ODBC_C_TYPE_FUNCTION(float,             SQL_FLOAT)
    _DEFINE_ODBC_C_TYPE_FUNCTION(double,            SQL_DOUBLE)


#endif //__SOLAR_DATABAES_DEFINE_H__
