// odbcAPI.h
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ODBCAPI_H__11EAA72A_EFF5_4944_8B1F_44973D30EF73__INCLUDED_)
#define AFX_ODBCAPI_H__11EAA72A_EFF5_4944_8B1F_44973D30EF73__INCLUDED_

#include "SQLAPI.h"

// API header(s)
#ifdef LINUX
	#include "./odbc_linux/sql.h"
	#include "./odbc_linux/sqlext.h"
#endif
#ifdef WIN32
	#include "./odbc_win32/sql.h"
	#include "./odbc_win32/sqlext.h"
#endif

extern long g_nODBCDLLVersionLoaded;

extern void AddODBCSupport(const SAConnection *pCon);
extern void ReleaseODBCSupport();

typedef SQLRETURN  (SQL_API *SQLAllocConnect_t)(SQLHENV EnvironmentHandle,
           SQLHDBC *ConnectionHandle);
typedef SQLRETURN  (SQL_API *SQLAllocEnv_t)(SQLHENV *EnvironmentHandle);
typedef SQLRETURN  (SQL_API *SQLAllocHandle_t)(SQLSMALLINT HandleType,
           SQLHANDLE InputHandle, SQLHANDLE *OutputHandle);
typedef SQLRETURN  (SQL_API *SQLAllocStmt_t)(SQLHDBC ConnectionHandle,
           SQLHSTMT *StatementHandle);
typedef SQLRETURN  (SQL_API *SQLBindCol_t)(SQLHSTMT StatementHandle, 
		   SQLUSMALLINT ColumnNumber, SQLSMALLINT TargetType, 
		   SQLPOINTER TargetValue, SQLINTEGER BufferLength, 
	   	   SQLINTEGER *StrLen_or_Ind);
typedef SQLRETURN (SQL_API *SQLBindParameter_t)(
    SQLHSTMT           hstmt,
    SQLUSMALLINT       ipar,
    SQLSMALLINT        fParamType,
    SQLSMALLINT        fCType,
    SQLSMALLINT        fSqlType,
    SQLUINTEGER        cbColDef,
    SQLSMALLINT        ibScale,
    SQLPOINTER         rgbValue,
    SQLINTEGER         cbValueMax,
    SQLINTEGER 		  *pcbValue);
typedef SQLRETURN (SQL_API *SQLBrowseConnect_t)(
    SQLHDBC            hdbc,
    SQLCHAR 		  *szConnStrIn,
    SQLSMALLINT        cbConnStrIn,
    SQLCHAR 		  *szConnStrOut,
    SQLSMALLINT        cbConnStrOutMax,
    SQLSMALLINT       *pcbConnStrOut);
typedef SQLRETURN	(SQL_API	*SQLBulkOperations_t)(
	SQLHSTMT			StatementHandle,
	SQLSMALLINT			Operation);
typedef SQLRETURN  (SQL_API *SQLCancel_t)(SQLHSTMT StatementHandle);
typedef SQLRETURN  (SQL_API *SQLCloseCursor_t)(SQLHSTMT StatementHandle);
typedef SQLRETURN  (SQL_API *SQLColAttribute_t)(SQLHSTMT StatementHandle,
           SQLUSMALLINT ColumnNumber, SQLUSMALLINT FieldIdentifier,
           SQLPOINTER CharacterAttribute, SQLSMALLINT BufferLength,
           SQLSMALLINT *StringLength, SQLPOINTER NumericAttribute);
typedef SQLRETURN (SQL_API *SQLColAttributes_t)(
    SQLHSTMT           hstmt,
    SQLUSMALLINT       icol,
    SQLUSMALLINT       fDescType,
    SQLPOINTER         rgbDesc,
    SQLSMALLINT        cbDescMax,
    SQLSMALLINT 	  *pcbDesc,
    SQLINTEGER 		  *pfDesc);
typedef SQLRETURN (SQL_API *SQLColumnPrivileges_t)(
    SQLHSTMT           hstmt,
    SQLCHAR 		  *szCatalogName,
    SQLSMALLINT        cbCatalogName,
    SQLCHAR 		  *szSchemaName,
    SQLSMALLINT        cbSchemaName,
    SQLCHAR 		  *szTableName,
    SQLSMALLINT        cbTableName,
    SQLCHAR 		  *szColumnName,
    SQLSMALLINT        cbColumnName);
typedef SQLRETURN  (SQL_API *SQLColumns_t)(SQLHSTMT StatementHandle,
           SQLCHAR *CatalogName, SQLSMALLINT NameLength1,
           SQLCHAR *SchemaName, SQLSMALLINT NameLength2,
           SQLCHAR *TableName, SQLSMALLINT NameLength3,
           SQLCHAR *ColumnName, SQLSMALLINT NameLength4);
typedef SQLRETURN  (SQL_API *SQLConnect_t)(SQLHDBC ConnectionHandle,
           SQLCHAR *ServerName, SQLSMALLINT NameLength1,
           SQLCHAR *UserName, SQLSMALLINT NameLength2,
           SQLCHAR *Authentication, SQLSMALLINT NameLength3);
typedef SQLRETURN  (SQL_API *SQLCopyDesc_t)(SQLHDESC SourceDescHandle,
           SQLHDESC TargetDescHandle);
typedef SQLRETURN  (SQL_API *SQLDataSources_t)(SQLHENV EnvironmentHandle,
           SQLUSMALLINT Direction, SQLCHAR *ServerName,
           SQLSMALLINT BufferLength1, SQLSMALLINT *NameLength1,
           SQLCHAR *Description, SQLSMALLINT BufferLength2,
           SQLSMALLINT *NameLength2);
typedef SQLRETURN  (SQL_API *SQLDescribeCol_t)(SQLHSTMT StatementHandle,
           SQLUSMALLINT ColumnNumber, SQLCHAR *ColumnName,
           SQLSMALLINT BufferLength, SQLSMALLINT *NameLength,
           SQLSMALLINT *DataType, SQLUINTEGER *ColumnSize,
           SQLSMALLINT *DecimalDigits, SQLSMALLINT *Nullable);
typedef SQLRETURN (SQL_API *SQLDescribeParam_t)(
    SQLHSTMT           hstmt,
    SQLUSMALLINT       ipar,
    SQLSMALLINT 	  *pfSqlType,
    SQLUINTEGER 	  *pcbParamDef,
    SQLSMALLINT 	  *pibScale,
    SQLSMALLINT 	  *pfNullable);
typedef SQLRETURN  (SQL_API *SQLDisconnect_t)(SQLHDBC ConnectionHandle);
typedef SQLRETURN (SQL_API *SQLDriverConnect_t)(
    SQLHDBC            hdbc,
    SQLHWND            hwnd,
    SQLCHAR 		  *szConnStrIn,
    SQLSMALLINT        cbConnStrIn,
    SQLCHAR           *szConnStrOut,
    SQLSMALLINT        cbConnStrOutMax,
    SQLSMALLINT 	  *pcbConnStrOut,
    SQLUSMALLINT       fDriverCompletion);
typedef SQLRETURN (SQL_API *SQLDrivers_t)(
    SQLHENV            henv,
    SQLUSMALLINT       fDirection,
    SQLCHAR 		  *szDriverDesc,
    SQLSMALLINT        cbDriverDescMax,
    SQLSMALLINT 	  *pcbDriverDesc,
    SQLCHAR 		  *szDriverAttributes,
    SQLSMALLINT        cbDrvrAttrMax,
    SQLSMALLINT 	  *pcbDrvrAttr);
typedef SQLRETURN  (SQL_API *SQLEndTran_t)(SQLSMALLINT HandleType, SQLHANDLE Handle,
           SQLSMALLINT CompletionType);
typedef SQLRETURN  (SQL_API *SQLError_t)(SQLHENV EnvironmentHandle,
           SQLHDBC ConnectionHandle, SQLHSTMT StatementHandle,
           SQLCHAR *Sqlstate, SQLINTEGER *NativeError,
           SQLCHAR *MessageText, SQLSMALLINT BufferLength,
           SQLSMALLINT *TextLength);
typedef SQLRETURN  (SQL_API *SQLExecDirect_t)(SQLHSTMT StatementHandle,
           SQLCHAR *StatementText, SQLINTEGER TextLength);
typedef SQLRETURN  (SQL_API *SQLExecute_t)(SQLHSTMT StatementHandle);
typedef SQLRETURN (SQL_API *SQLExtendedFetch_t)(
    SQLHSTMT           hstmt,
    SQLUSMALLINT       fFetchType,
    SQLINTEGER         irow,
    SQLUINTEGER 	  *pcrow,
    SQLUSMALLINT 	  *rgfRowStatus);
typedef SQLRETURN  (SQL_API *SQLFetch_t)(SQLHSTMT StatementHandle);
typedef SQLRETURN  (SQL_API *SQLFetchScroll_t)(SQLHSTMT StatementHandle,
           SQLSMALLINT FetchOrientation, SQLINTEGER FetchOffset);
typedef SQLRETURN (SQL_API *SQLForeignKeys_t)(
    SQLHSTMT           hstmt,
    SQLCHAR 		  *szPkCatalogName,
    SQLSMALLINT        cbPkCatalogName,
    SQLCHAR 		  *szPkSchemaName,
    SQLSMALLINT        cbPkSchemaName,
    SQLCHAR 		  *szPkTableName,
    SQLSMALLINT        cbPkTableName,
    SQLCHAR 		  *szFkCatalogName,
    SQLSMALLINT        cbFkCatalogName,
    SQLCHAR 		  *szFkSchemaName,
    SQLSMALLINT        cbFkSchemaName,
    SQLCHAR 		  *szFkTableName,
    SQLSMALLINT        cbFkTableName);
typedef SQLRETURN  (SQL_API *SQLFreeConnect_t)(SQLHDBC ConnectionHandle);
typedef SQLRETURN  (SQL_API *SQLFreeEnv_t)(SQLHENV EnvironmentHandle);
typedef SQLRETURN  (SQL_API *SQLFreeHandle_t)(SQLSMALLINT HandleType, SQLHANDLE Handle);
typedef SQLRETURN  (SQL_API *SQLFreeStmt_t)(SQLHSTMT StatementHandle,
           SQLUSMALLINT Option);
typedef SQLRETURN  (SQL_API *SQLGetConnectAttr_t)(SQLHDBC ConnectionHandle,
           SQLINTEGER Attribute, SQLPOINTER Value,
           SQLINTEGER BufferLength, SQLINTEGER *StringLength);
typedef SQLRETURN  (SQL_API *SQLGetConnectOption_t)(SQLHDBC ConnectionHandle,
           SQLUSMALLINT Option, SQLPOINTER Value);
typedef SQLRETURN  (SQL_API *SQLGetCursorName_t)(SQLHSTMT StatementHandle,
           SQLCHAR *CursorName, SQLSMALLINT BufferLength,
           SQLSMALLINT *NameLength);
typedef SQLRETURN  (SQL_API *SQLGetData_t)(SQLHSTMT StatementHandle,
           SQLUSMALLINT ColumnNumber, SQLSMALLINT TargetType,
           SQLPOINTER TargetValue, SQLINTEGER BufferLength,
           SQLINTEGER *StrLen_or_Ind);
typedef SQLRETURN  (SQL_API *SQLGetDescField_t)(SQLHDESC DescriptorHandle,
           SQLSMALLINT RecNumber, SQLSMALLINT FieldIdentifier,
           SQLPOINTER Value, SQLINTEGER BufferLength,
           SQLINTEGER *StringLength);
typedef SQLRETURN  (SQL_API *SQLGetDescRec_t)(SQLHDESC DescriptorHandle,
           SQLSMALLINT RecNumber, SQLCHAR *Name,
           SQLSMALLINT BufferLength, SQLSMALLINT *StringLength,
           SQLSMALLINT *Type, SQLSMALLINT *SubType, 
           SQLINTEGER *Length, SQLSMALLINT *Precision, 
           SQLSMALLINT *Scale, SQLSMALLINT *Nullable);
typedef SQLRETURN  (SQL_API *SQLGetDiagRec_t)(SQLSMALLINT HandleType, SQLHANDLE Handle,
           SQLSMALLINT RecNumber, SQLCHAR *Sqlstate,
           SQLINTEGER *NativeError, SQLCHAR *MessageText,
           SQLSMALLINT BufferLength, SQLSMALLINT *TextLength);
typedef SQLRETURN  (SQL_API *SQLGetDiagField_t)(SQLSMALLINT HandleType, SQLHANDLE Handle,
           SQLSMALLINT RecNumber, SQLSMALLINT DiagIdentifier,
           SQLPOINTER DiagInfo, SQLSMALLINT BufferLength,
           SQLSMALLINT *StringLength);
typedef SQLRETURN  (SQL_API *SQLGetEnvAttr_t)(SQLHENV EnvironmentHandle,
           SQLINTEGER Attribute, SQLPOINTER Value,
           SQLINTEGER BufferLength, SQLINTEGER *StringLength);
typedef SQLRETURN  (SQL_API *SQLGetFunctions_t)(SQLHDBC ConnectionHandle,
           SQLUSMALLINT FunctionId, SQLUSMALLINT *Supported);
typedef SQLRETURN  (SQL_API *SQLGetInfo_t)(SQLHDBC ConnectionHandle,
           SQLUSMALLINT InfoType, SQLPOINTER InfoValue,
           SQLSMALLINT BufferLength, SQLSMALLINT *StringLength);
typedef SQLRETURN  (SQL_API *SQLGetStmtAttr_t)(SQLHSTMT StatementHandle,
           SQLINTEGER Attribute, SQLPOINTER Value,
           SQLINTEGER BufferLength, SQLINTEGER *StringLength);
typedef SQLRETURN  (SQL_API *SQLGetStmtOption_t)(SQLHSTMT StatementHandle,
           SQLUSMALLINT Option, SQLPOINTER Value);
typedef SQLRETURN  (SQL_API *SQLGetTypeInfo_t)(SQLHSTMT StatementHandle,
           SQLSMALLINT DataType);
typedef SQLRETURN (SQL_API *SQLMoreResults_t)(
    SQLHSTMT           hstmt);
typedef SQLRETURN (SQL_API *SQLNativeSql_t)(
    SQLHDBC            hdbc,
    SQLCHAR 		  *szSqlStrIn,
    SQLINTEGER         cbSqlStrIn,
    SQLCHAR 		  *szSqlStr,
    SQLINTEGER         cbSqlStrMax,
    SQLINTEGER 		  *pcbSqlStr);
typedef SQLRETURN (SQL_API *SQLNumParams_t)(
    SQLHSTMT           hstmt,
    SQLSMALLINT 	  *pcpar);
typedef SQLRETURN  (SQL_API *SQLNumResultCols_t)(SQLHSTMT StatementHandle,
           SQLSMALLINT *ColumnCount);
typedef SQLRETURN  (SQL_API *SQLParamData_t)(SQLHSTMT StatementHandle,
           SQLPOINTER *Value);
typedef SQLRETURN (SQL_API *SQLParamOptions_t)(
    SQLHSTMT           hstmt,
    SQLUINTEGER        crow,
    SQLUINTEGER 	  *pirow);
typedef SQLRETURN  (SQL_API *SQLPrepare_t)(SQLHSTMT StatementHandle,
           SQLCHAR *StatementText, SQLINTEGER TextLength);
typedef SQLRETURN (SQL_API *SQLPrimaryKeys_t)(
    SQLHSTMT           hstmt,
    SQLCHAR 		  *szCatalogName,
    SQLSMALLINT        cbCatalogName,
    SQLCHAR 		  *szSchemaName,
    SQLSMALLINT        cbSchemaName,
    SQLCHAR 		  *szTableName,
    SQLSMALLINT        cbTableName);
typedef SQLRETURN (SQL_API *SQLProcedureColumns_t)(
    SQLHSTMT           hstmt,
    SQLCHAR 		  *szCatalogName,
    SQLSMALLINT        cbCatalogName,
    SQLCHAR 		  *szSchemaName,
    SQLSMALLINT        cbSchemaName,
    SQLCHAR 		  *szProcName,
    SQLSMALLINT        cbProcName,
    SQLCHAR 		  *szColumnName,
    SQLSMALLINT        cbColumnName);
typedef SQLRETURN (SQL_API *SQLProcedures_t)(
    SQLHSTMT           hstmt,
    SQLCHAR 		  *szCatalogName,
    SQLSMALLINT        cbCatalogName,
    SQLCHAR 		  *szSchemaName,
    SQLSMALLINT        cbSchemaName,
    SQLCHAR 		  *szProcName,
    SQLSMALLINT        cbProcName);
typedef SQLRETURN  (SQL_API *SQLPutData_t)(SQLHSTMT StatementHandle,
           SQLPOINTER Data, SQLINTEGER StrLen_or_Ind);
typedef SQLRETURN  (SQL_API *SQLRowCount_t)(SQLHSTMT StatementHandle, 
	   SQLINTEGER *RowCount);
typedef SQLRETURN  (SQL_API *SQLSetConnectAttr_t)(SQLHDBC ConnectionHandle,
           SQLINTEGER Attribute, SQLPOINTER Value,
           SQLINTEGER StringLength);
typedef SQLRETURN  (SQL_API *SQLSetConnectOption_t)(SQLHDBC ConnectionHandle,
           SQLUSMALLINT Option, SQLUINTEGER Value);
typedef SQLRETURN  (SQL_API *SQLSetCursorName_t)(SQLHSTMT StatementHandle,
           SQLCHAR *CursorName, SQLSMALLINT NameLength);
typedef SQLRETURN  (SQL_API *SQLSetDescField_t)(SQLHDESC DescriptorHandle,
           SQLSMALLINT RecNumber, SQLSMALLINT FieldIdentifier,
           SQLPOINTER Value, SQLINTEGER BufferLength);
typedef SQLRETURN  (SQL_API *SQLSetDescRec_t)(SQLHDESC DescriptorHandle,
           SQLSMALLINT RecNumber, SQLSMALLINT Type,
           SQLSMALLINT SubType, SQLINTEGER Length,
           SQLSMALLINT Precision, SQLSMALLINT Scale,
           SQLPOINTER Data, SQLINTEGER *StringLength,
           SQLINTEGER *Indicator);
typedef SQLRETURN  (SQL_API *SQLSetEnvAttr_t)(SQLHENV EnvironmentHandle,
           SQLINTEGER Attribute, SQLPOINTER Value,
           SQLINTEGER StringLength);
typedef SQLRETURN  (SQL_API *SQLSetParam_t)(SQLHSTMT StatementHandle,
           SQLUSMALLINT ParameterNumber, SQLSMALLINT ValueType,
           SQLSMALLINT ParameterType, SQLUINTEGER LengthPrecision,
           SQLSMALLINT ParameterScale, SQLPOINTER ParameterValue,
           SQLINTEGER *StrLen_or_Ind);
typedef SQLRETURN (SQL_API *SQLSetPos_t)(
    SQLHSTMT           hstmt,
    SQLUSMALLINT       irow,
    SQLUSMALLINT       fOption,
    SQLUSMALLINT       fLock);
typedef SQLRETURN (SQL_API *SQLSetScrollOptions_t)(    /*      Use SQLSetStmtOptions */
    SQLHSTMT           hstmt,
    SQLUSMALLINT       fConcurrency,
    SQLINTEGER         crowKeyset,
    SQLUSMALLINT       crowRowset);
typedef SQLRETURN  (SQL_API *SQLSetStmtAttr_t)(SQLHSTMT StatementHandle,
           SQLINTEGER Attribute, SQLPOINTER Value,
           SQLINTEGER StringLength);
typedef SQLRETURN  (SQL_API *SQLSetStmtOption_t)(SQLHSTMT StatementHandle,
           SQLUSMALLINT Option, SQLUINTEGER Value);
typedef SQLRETURN  (SQL_API *SQLSpecialColumns_t)(SQLHSTMT StatementHandle,
           SQLUSMALLINT IdentifierType, SQLCHAR *CatalogName,
           SQLSMALLINT NameLength1, SQLCHAR *SchemaName,
           SQLSMALLINT NameLength2, SQLCHAR *TableName,
           SQLSMALLINT NameLength3, SQLUSMALLINT Scope,
           SQLUSMALLINT Nullable);
typedef SQLRETURN  (SQL_API *SQLStatistics_t)(SQLHSTMT StatementHandle,
           SQLCHAR *CatalogName, SQLSMALLINT NameLength1,
           SQLCHAR *SchemaName, SQLSMALLINT NameLength2,
           SQLCHAR *TableName, SQLSMALLINT NameLength3,
           SQLUSMALLINT Unique, SQLUSMALLINT Reserved);
typedef SQLRETURN (SQL_API *SQLTablePrivileges_t)(
    SQLHSTMT           hstmt,
    SQLCHAR 		  *szCatalogName,
    SQLSMALLINT        cbCatalogName,
    SQLCHAR 		  *szSchemaName,
    SQLSMALLINT        cbSchemaName,
    SQLCHAR 		  *szTableName,
    SQLSMALLINT        cbTableName);
typedef SQLRETURN  (SQL_API *SQLTables_t)(SQLHSTMT StatementHandle,
           SQLCHAR *CatalogName, SQLSMALLINT NameLength1,
           SQLCHAR *SchemaName, SQLSMALLINT NameLength2,
           SQLCHAR *TableName, SQLSMALLINT NameLength3,
           SQLCHAR *TableType, SQLSMALLINT NameLength4);
typedef SQLRETURN  (SQL_API *SQLTransact_t)(SQLHENV EnvironmentHandle,
           SQLHDBC ConnectionHandle, SQLUSMALLINT CompletionType);


class SQLAPI_API odbcAPI : public saAPI
{
public:
	odbcAPI();

	SQLAllocConnect_t		SQLAllocConnect;	// 1.0
	SQLAllocEnv_t			SQLAllocEnv;		// 1.0
	SQLAllocHandle_t		SQLAllocHandle;		// 3.0
	SQLAllocStmt_t			SQLAllocStmt;		// 1.0
	SQLBindCol_t			SQLBindCol;			// 1.0
	SQLBindParameter_t		SQLBindParameter;	// 2.0
	SQLBrowseConnect_t		SQLBrowseConnect;	// 1.0
	SQLBulkOperations_t		SQLBulkOperations;	// 3.0
	SQLCancel_t				SQLCancel;			// 1.0
	SQLCloseCursor_t		SQLCloseCursor;		// 3.0
	SQLColAttribute_t		SQLColAttribute;	// 3.0
	SQLColAttributes_t		SQLColAttributes;	// 1.0
	SQLColumnPrivileges_t	SQLColumnPrivileges;// 1.0
	SQLColumns_t			SQLColumns;			// 1.0
	SQLConnect_t			SQLConnect;			// 1.0
	SQLCopyDesc_t			SQLCopyDesc;		// 3.0
	SQLDataSources_t		SQLDataSources;		// 1.0
	SQLDescribeCol_t		SQLDescribeCol;		// 1.0
	SQLDescribeParam_t		SQLDescribeParam;	// 1.0
	SQLDisconnect_t			SQLDisconnect;		// 1.0
	SQLDriverConnect_t		SQLDriverConnect;	// 1.0
	SQLDrivers_t			SQLDrivers;			// 2.0
	SQLEndTran_t			SQLEndTran;			// 3.0
	SQLError_t				SQLError;			// 1.0
	SQLExecDirect_t			SQLExecDirect;		// 1.0
	SQLExecute_t			SQLExecute;			// 1.0
	SQLExtendedFetch_t		SQLExtendedFetch;	// 1.0
	SQLFetch_t				SQLFetch;			// 1.0
	SQLFetchScroll_t		SQLFetchScroll;		// 1.0
	SQLForeignKeys_t		SQLForeignKeys;		// 1.0
	SQLFreeConnect_t		SQLFreeConnect;		// 1.0
	SQLFreeEnv_t			SQLFreeEnv;			// 1.0
	SQLFreeHandle_t			SQLFreeHandle;		// 3.0
	SQLFreeStmt_t			SQLFreeStmt;		// 1.0
	SQLGetConnectAttr_t		SQLGetConnectAttr;	// 3.0
	SQLGetConnectOption_t	SQLGetConnectOption;// 1.0
	SQLGetCursorName_t		SQLGetCursorName;	// 1.0
	SQLGetData_t			SQLGetData;			// 1.0
	SQLGetDescField_t		SQLGetDescField;	// 3.0
	SQLGetDescRec_t			SQLGetDescRec;		// 3.0
	SQLGetDiagField_t		SQLGetDiagField;	// 3.0
	SQLGetDiagRec_t			SQLGetDiagRec;		// 3.0
	SQLGetEnvAttr_t			SQLGetEnvAttr;		// 3.0
	SQLGetFunctions_t		SQLGetFunctions;	// 1.0
	SQLGetInfo_t			SQLGetInfo;			// 1.0
	SQLGetStmtAttr_t		SQLGetStmtAttr;		// 3.0
	SQLGetStmtOption_t		SQLGetStmtOption;	// 1.0
	SQLGetTypeInfo_t		SQLGetTypeInfo;		// 1.0
	SQLMoreResults_t		SQLMoreResults;		// 1.0
	SQLNativeSql_t			SQLNativeSql;		// 1.0
	SQLNumParams_t			SQLNumParams;		// 1.0
	SQLNumResultCols_t		SQLNumResultCols;	// 1.0
	SQLParamData_t			SQLParamData;		// 1.0
	SQLParamOptions_t		SQLParamOptions;	// 1.0
	SQLPrepare_t			SQLPrepare;			// 1.0
	SQLPrimaryKeys_t		SQLPrimaryKeys;		// 1.0
	SQLProcedureColumns_t	SQLProcedureColumns;// 1.0
	SQLProcedures_t			SQLProcedures;		// 1.0
	SQLPutData_t			SQLPutData;			// 1.0
	SQLRowCount_t			SQLRowCount;		// 1.0
	SQLSetConnectAttr_t		SQLSetConnectAttr;	// 3.0
	SQLSetConnectOption_t	SQLSetConnectOption;// 1.0
	SQLSetCursorName_t		SQLSetCursorName;	// 1.0
	SQLSetDescField_t		SQLSetDescField;	// 3.0
	SQLSetDescRec_t			SQLSetDescRec;		// 3.0
	SQLSetEnvAttr_t			SQLSetEnvAttr;		// 3.0
	SQLSetParam_t			SQLSetParam;		// 1.0
	SQLSetPos_t				SQLSetPos;			// 1.0
	SQLSetScrollOptions_t	SQLSetScrollOptions;// 1.0
	SQLSetStmtAttr_t		SQLSetStmtAttr;		// 3.0
	SQLSetStmtOption_t		SQLSetStmtOption;	// 1.0
	SQLSpecialColumns_t		SQLSpecialColumns;	// 1.0
	SQLStatistics_t			SQLStatistics;		// 1.0
	SQLTablePrivileges_t	SQLTablePrivileges;	// 1.0
	SQLTables_t				SQLTables;			// 1.0
	SQLTransact_t			SQLTransact;		// 1.0
};

class SQLAPI_API odbcConnectionHandles : public saConnectionHandles
{
public:
	odbcConnectionHandles();

	SQLHENV	m_hevn;
	SQLHDBC	m_hdbc;
};

class SQLAPI_API odbcCommandHandles : public saCommandHandles
{
public:
	odbcCommandHandles();

	SQLHSTMT	m_hstmt;
};

extern odbcAPI g_odbcAPI;

#endif // !defined(AFX_ODBCAPI_H__11EAA72A_EFF5_4944_8B1F_44973D30EF73__INCLUDED_)
