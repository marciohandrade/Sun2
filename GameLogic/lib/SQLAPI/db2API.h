// db2API.h
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DB2API_H__11EAA72A_EFF5_4944_8B1F_44973D30EF73__INCLUDED_)
#define AFX_DB2API_H__11EAA72A_EFF5_4944_8B1F_44973D30EF73__INCLUDED_

#include "SQLAPI.h"

#ifdef LINUX
	#include "./db2_linux/sqlcli.h"
	#include "./db2_linux/sqlcli1.h"
#endif
#ifdef WIN32
	#include "./db2_win32/sqlcli.h"
	#include "./db2_win32/sqlcli1.h"
#endif

extern long g_nDB2DLLVersionLoaded;

extern void AddDB2Support();
extern void ReleaseDB2Support();

typedef SQLRETURN (SQL_API_FN  *SQLAllocConnect_t)(SQLHENV           henv,
                                        SQLHDBC     FAR   *phdbc);
typedef SQLRETURN (SQL_API_FN  *SQLAllocEnv_t)(SQLHENV     FAR   *phenv);
typedef SQLRETURN (SQL_API_FN *SQLAllocHandle_t)(    SQLSMALLINT fHandleType,
                                        SQLHANDLE hInput,
                                        SQLHANDLE * phOutput );
typedef SQLRETURN (SQL_API_FN  *SQLAllocStmt_t)(SQLHDBC           hdbc,
                                        SQLHSTMT    FAR   *phstmt);
typedef SQLRETURN (SQL_API_FN  *SQLBindCol_t)(SQLHSTMT          hstmt,
                                        SQLUSMALLINT      icol,
                                        SQLSMALLINT       fCType,
                                        SQLPOINTER        rgbValue,
                                        SQLINTEGER        cbValueMax,
                                        SQLINTEGER  FAR   *pcbValue);
typedef SQLRETURN (SQL_API_FN *SQLBindFileToCol_t)(SQLHSTMT          hstmt,
                                        SQLUSMALLINT      icol,
                                        SQLCHAR     FAR   *FileName,
                                        SQLSMALLINT FAR   *FileNameLength,
                                        SQLUINTEGER FAR   *FileOptions,
                                        SQLSMALLINT       MaxFileNameLength,
                                        SQLINTEGER  FAR   *StringLength,
                                        SQLINTEGER  FAR   *IndicatorValue);
typedef SQLRETURN (SQL_API_FN *SQLBindFileToParam_t)(SQLHSTMT          hstmt,
                                        SQLUSMALLINT      ipar,
                                        SQLSMALLINT       fSqlType,
                                        SQLCHAR     FAR   *FileName,
                                        SQLSMALLINT FAR   *FileNameLength,
                                        SQLUINTEGER FAR   *FileOptions,
                                        SQLSMALLINT       MaxFileNameLength,
                                        SQLINTEGER  FAR   *IndicatorValue);
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
    SQLINTEGER            *pcbValue);


typedef SQLRETURN (SQL_API *SQLBrowseConnect_t)(
    SQLHDBC            hdbc,
    SQLCHAR               *szConnStrIn,
    SQLSMALLINT        cbConnStrIn,
    SQLCHAR               *szConnStrOut,
    SQLSMALLINT        cbConnStrOutMax,
    SQLSMALLINT       *pcbConnStrOut);
typedef SQLRETURN (SQL_API_FN *SQLBuildDataLink_t)(  SQLHSTMT hStmt,
                                        SQLCHAR FAR * pszLinkType,
                                        SQLINTEGER cbLinkType,
                                        SQLCHAR FAR * pszDataLocation,
                                        SQLINTEGER cbDataLocation,
                                        SQLCHAR FAR * pszComment,
                                        SQLINTEGER cbComment,
                                        SQLCHAR FAR * pDataLink,
                                        SQLINTEGER cbDataLinkMax,
                                        SQLINTEGER FAR * pcbDataLink );
typedef SQLRETURN       (SQL_API *SQLBulkOperations_t)(
        SQLHSTMT                        StatementHandle,
        SQLSMALLINT                     Operation);
typedef SQLRETURN (SQL_API_FN  *SQLCancel_t)(SQLHSTMT          hstmt);
typedef SQLRETURN (SQL_API_FN *SQLCloseCursor_t)(    SQLHSTMT hStmt );
typedef SQLRETURN (SQL_API_FN  *SQLColAttribute_t)(SQLHSTMT          hstmt,
                                        SQLUSMALLINT      icol,
                                        SQLUSMALLINT      fDescType,
                                        SQLPOINTER        rgbDesc,
                                        SQLSMALLINT       cbDescMax,
                                        SQLSMALLINT FAR   *pcbDesc,
                                        SQLPOINTER         pfDesc);
typedef SQLRETURN (SQL_API *SQLColAttributes_t)(
    SQLHSTMT           hstmt,
    SQLUSMALLINT       icol,
    SQLUSMALLINT       fDescType,
    SQLPOINTER         rgbDesc,
    SQLSMALLINT        cbDescMax,
    SQLSMALLINT           *pcbDesc,
    SQLINTEGER            *pfDesc);
typedef SQLRETURN (SQL_API *SQLColumnPrivileges_t)(
    SQLHSTMT           hstmt,
    SQLCHAR               *szCatalogName,
    SQLSMALLINT        cbCatalogName,
    SQLCHAR               *szSchemaName,
    SQLSMALLINT        cbSchemaName,
    SQLCHAR               *szTableName,
    SQLSMALLINT        cbTableName,
    SQLCHAR               *szColumnName,
    SQLSMALLINT        cbColumnName);
typedef SQLRETURN (SQL_API_FN  *SQLColumns_t)(SQLHSTMT          hstmt,
                                        SQLCHAR     FAR   *szCatalogName,
                                        SQLSMALLINT       cbCatalogName,
                                        SQLCHAR     FAR   *szSchemaName,
                                        SQLSMALLINT       cbSchemaName,
                                        SQLCHAR     FAR   *szTableName,
                                        SQLSMALLINT       cbTableName,
                                        SQLCHAR     FAR   *szColumnName,
                                        SQLSMALLINT       cbColumnName);
typedef SQLRETURN (SQL_API_FN  *SQLConnect_t)(SQLHDBC           hdbc,
                                        SQLCHAR     FAR   *szDSN,
                                        SQLSMALLINT       cbDSN,
                                        SQLCHAR     FAR   *szUID,
                                        SQLSMALLINT       cbUID,
                                        SQLCHAR     FAR   *szAuthStr,
                                        SQLSMALLINT       cbAuthStr);
typedef SQLRETURN  (SQL_API_FN *SQLCopyDesc_t)(      SQLHDESC hDescSource,
                                        SQLHDESC hDescTarget );
typedef SQLRETURN (SQL_API_FN  *SQLDataSources_t)(SQLHENV           henv,
                                        SQLUSMALLINT      fDirection,
                                        SQLCHAR     FAR   *szDSN,
                                        SQLSMALLINT       cbDSNMax,
                                        SQLSMALLINT FAR   *pcbDSN,
                                        SQLCHAR     FAR   *szDescription,
                                        SQLSMALLINT       cbDescriptionMax,
                                        SQLSMALLINT FAR   *pcbDescription);
typedef SQLRETURN  (SQL_API *SQLDescribeCol_t)(SQLHSTMT StatementHandle,
           SQLUSMALLINT ColumnNumber, SQLCHAR *ColumnName,
           SQLSMALLINT BufferLength, SQLSMALLINT *NameLength,
           SQLSMALLINT *DataType, SQLUINTEGER *ColumnSize,
           SQLSMALLINT *DecimalDigits, SQLSMALLINT *Nullable);
typedef SQLRETURN (SQL_API *SQLDescribeParam_t)(
    SQLHSTMT           hstmt,
    SQLUSMALLINT       ipar,
    SQLSMALLINT           *pfSqlType,
    SQLUINTEGER           *pcbParamDef,
    SQLSMALLINT           *pibScale,
    SQLSMALLINT           *pfNullable);
typedef SQLRETURN  (SQL_API *SQLDisconnect_t)(SQLHDBC ConnectionHandle);
typedef SQLRETURN (SQL_API *SQLDriverConnect_t)(
    SQLHDBC            hdbc,
    SQLHWND            hwnd,
    SQLCHAR               *szConnStrIn,
    SQLSMALLINT        cbConnStrIn,
    SQLCHAR           *szConnStrOut,
    SQLSMALLINT        cbConnStrOutMax,
    SQLSMALLINT           *pcbConnStrOut,
    SQLUSMALLINT       fDriverCompletion);
typedef SQLRETURN  (SQL_API *SQLEndTran_t)(SQLSMALLINT HandleType, SQLHANDLE Handle,
           SQLSMALLINT CompletionType);
typedef SQLRETURN (SQL_API_FN  *SQLError_t)(SQLHENV           henv,
                                        SQLHDBC           hdbc,
                                        SQLHSTMT          hstmt,
                                        SQLCHAR     FAR   *szSqlState,
                                        SQLINTEGER  FAR   *pfNativeError,
                                        SQLCHAR     FAR   *szErrorMsg,
                                        SQLSMALLINT       cbErrorMsgMax,
                                        SQLSMALLINT FAR   *pcbErrorMsg);

typedef SQLRETURN (SQL_API_FN  *SQLExecDirect_t)(SQLHSTMT          hstmt,
                                        SQLCHAR     FAR   *szSqlStr,
                                        SQLINTEGER        cbSqlStr);
typedef SQLRETURN (SQL_API_FN  *SQLExecute_t)(SQLHSTMT          hstmt);
typedef SQLRETURN (SQL_API_FN  *SQLExtendedBind_t)(SQLHSTMT          hstmt,
                                          SQLSMALLINT       fBindCol,
                                          SQLSMALLINT       cRecords,
                                          SQLSMALLINT *     pfCType,
                                          SQLPOINTER  *     rgbValue,
                                          SQLINTEGER  *     cbValueMax,
                                          SQLUINTEGER *     puiPrecisionCType,
                                          SQLSMALLINT *     psScaleCType,
                                          SQLINTEGER  **    pcbValue,
                                          SQLINTEGER  **    piIndicatorPtr,
                                          SQLSMALLINT *     pfParamType,
                                          SQLSMALLINT *     pfSQLType,
                                          SQLUINTEGER *     pcbColDef,
                                          SQLSMALLINT *     pibScale );
typedef SQLRETURN (SQL_API *SQLExtendedFetch_t)(
    SQLHSTMT           hstmt,
    SQLUSMALLINT       fFetchType,
    SQLINTEGER         irow,
    SQLUINTEGER           *pcrow,
    SQLUSMALLINT          *rgfRowStatus);
typedef SQLRETURN (SQL_API_FN *SQLExtendedPrepare_t)( SQLHSTMT      hstmt,
                                         SQLCHAR *     pszSqlStmt,
                                         SQLINTEGER    cbSqlStmt,
                                         SQLINTEGER    cPars,
                                         SQLSMALLINT   sStmtType,
                                         SQLINTEGER    cStmtAttrs,
                                         SQLINTEGER *  piStmtAttr,
                                         SQLINTEGER *  pvParams );
typedef SQLRETURN (SQL_API_FN  *SQLFetch_t)(SQLHSTMT          hstmt);
typedef SQLRETURN  (SQL_API *SQLFetchScroll_t)(      SQLHSTMT StatementHandle,
                                        SQLSMALLINT FetchOrientation,
                                        SQLINTEGER FetchOffset);
typedef SQLRETURN (SQL_API *SQLForeignKeys_t)(
    SQLHSTMT           hstmt,
    SQLCHAR               *szPkCatalogName,
    SQLSMALLINT        cbPkCatalogName,
    SQLCHAR               *szPkSchemaName,
    SQLSMALLINT        cbPkSchemaName,
    SQLCHAR               *szPkTableName,
    SQLSMALLINT        cbPkTableName,
    SQLCHAR               *szFkCatalogName,
    SQLSMALLINT        cbFkCatalogName,
    SQLCHAR               *szFkSchemaName,
    SQLSMALLINT        cbFkSchemaName,
    SQLCHAR               *szFkTableName,
    SQLSMALLINT        cbFkTableName);
typedef SQLRETURN (SQL_API_FN  *SQLFreeConnect_t)(SQLHDBC           hdbc);
typedef SQLRETURN (SQL_API_FN  *SQLFreeEnv_t)(SQLHENV           henv);
typedef SQLRETURN (SQL_API_FN *SQLFreeHandle_t)(     SQLSMALLINT fHandleType,
                                        SQLHANDLE hHandle );
typedef SQLRETURN (SQL_API_FN  *SQLFreeStmt_t)(SQLHSTMT          hstmt,
                                        SQLUSMALLINT      fOption);
typedef SQLRETURN  (SQL_API *SQLGetConnectAttr_t)(   SQLHDBC ConnectionHandle,
                                        SQLINTEGER Attribute,
                                        SQLPOINTER Value,
                                        SQLINTEGER BufferLength,
                                        SQLINTEGER *StringLength);
typedef SQLRETURN (SQL_API_FN  *SQLGetConnectOption_t)(
                                        SQLHDBC           hdbc,
                                        SQLUSMALLINT      fOption,
                                        SQLPOINTER        pvParam);
typedef SQLRETURN (SQL_API_FN  *SQLGetCursorName_t)(SQLHSTMT          hstmt,
                                        SQLCHAR     FAR   *szCursor,
                                        SQLSMALLINT       cbCursorMax,
                                        SQLSMALLINT FAR   *pcbCursor);
typedef SQLRETURN (SQL_API_FN  *SQLGetData_t)(SQLHSTMT          hstmt,
                                        SQLUSMALLINT      icol,
                                        SQLSMALLINT       fCType,
                                        SQLPOINTER        rgbValue,
                                        SQLINTEGER        cbValueMax,
                                        SQLINTEGER  FAR   *pcbValue);
typedef SQLRETURN (SQL_API_FN *SQLGetDataLinkAttr_t)( SQLHSTMT hStmt,
                                         SQLSMALLINT fAttrType,
                                         SQLCHAR FAR * pDataLink,
                                         SQLINTEGER cbDataLink,
                                         SQLPOINTER pAttribute,
                                         SQLINTEGER cbAttributeMax,
                                         SQLINTEGER * pcbAttribute );
typedef SQLRETURN  (SQL_API_FN *SQLGetDescField_t)(SQLHDESC DescriptorHandle,
           SQLSMALLINT RecNumber, SQLSMALLINT FieldIdentifier,
           SQLPOINTER Value, SQLINTEGER BufferLength,
           SQLINTEGER *StringLength);
typedef SQLRETURN  (SQL_API_FN *SQLGetDescRec_t)(SQLHDESC DescriptorHandle,
           SQLSMALLINT RecNumber, SQLCHAR *Name,
           SQLSMALLINT BufferLength, SQLSMALLINT *StringLength,
           SQLSMALLINT *Type, SQLSMALLINT *SubType,
           SQLINTEGER *Length, SQLSMALLINT *Precision,
           SQLSMALLINT *Scale, SQLSMALLINT *Nullable);
typedef SQLRETURN (SQL_API_FN *SQLGetDiagField_t)(   SQLSMALLINT fHandleType,
                                        SQLHANDLE hHandle,
                                        SQLSMALLINT iRecNumber,
                                        SQLSMALLINT fDiagIdentifier,
                                        SQLPOINTER pDiagInfo,
                                        SQLSMALLINT cbDiagInfoMax,
                                        SQLSMALLINT * pcbDiagInfo );
typedef SQLRETURN (SQL_API_FN *SQLGetDiagRec_t)(     SQLSMALLINT fHandleType,
                                        SQLHANDLE hHandle,
                                        SQLSMALLINT iRecNumber,
                                        SQLCHAR * pszSqlState,
                                        SQLINTEGER * pfNativeError,
                                        SQLCHAR * pszErrorMsg,
                                        SQLSMALLINT cbErrorMsgMax,
                                        SQLSMALLINT * pcbErrorMsg );

typedef SQLRETURN (SQL_API_FN *SQLGetEnvAttr_t)(SQLHENV           henv,
                                        SQLINTEGER        Attribute,
                                        SQLPOINTER        Value,
                                        SQLINTEGER        BufferLength,
                                        SQLINTEGER  FAR   *StringLength);
typedef SQLRETURN (SQL_API_FN  *SQLGetFunctions_t)(SQLHDBC           hdbc,
                                        SQLUSMALLINT      fFunction,
                                        SQLUSMALLINT FAR  *pfExists);
typedef SQLRETURN (SQL_API_FN  *SQLGetInfo_t)(SQLHDBC           hdbc,
                                        SQLUSMALLINT      fInfoType,
                                        SQLPOINTER        rgbInfoValue,
                                        SQLSMALLINT       cbInfoValueMax,
                                        SQLSMALLINT FAR   *pcbInfoValue);
typedef SQLRETURN (SQL_API_FN *SQLGetLength_t)(SQLHSTMT          hstmt,
                                        SQLSMALLINT       LocatorCType,
                                        SQLINTEGER        Locator,
                                        SQLINTEGER  FAR   *StringLength,
                                        SQLINTEGER  FAR   *IndicatorValue);
typedef SQLRETURN (SQL_API_FN *SQLGetPosition_t)(SQLHSTMT          hstmt,
                                        SQLSMALLINT       LocatorCType,
                                        SQLINTEGER        SourceLocator,
                                        SQLINTEGER        SearchLocator,
                                        SQLCHAR     FAR   *SearchLiteral,
                                        SQLINTEGER        SearchLiteralLength,
                                        SQLUINTEGER       FromPosition,
                                        SQLUINTEGER FAR   *LocatedAt,
                                        SQLINTEGER  FAR   *IndicatorValue);
typedef SQLRETURN (SQL_API_FN *SQLGetSQLCA_t)(SQLHENV           henv,
                                        SQLHDBC           hdbc,
                                        SQLHSTMT          hstmt,
                                        struct sqlca FAR  *pSqlca );
typedef SQLRETURN  (SQL_API *SQLGetStmtAttr_t)(      SQLHSTMT StatementHandle,
                                        SQLINTEGER Attribute,
                                        SQLPOINTER Value,
                                        SQLINTEGER BufferLength,
                                        SQLINTEGER *StringLength);
typedef SQLRETURN (SQL_API_FN  *SQLGetStmtOption_t)(SQLHSTMT          hstmt,
                                        SQLUSMALLINT      fOption,
                                        SQLPOINTER        pvParam);
typedef SQLRETURN (SQL_API_FN *SQLGetSubString_t)(SQLHSTMT          hstmt,
                                        SQLSMALLINT       LocatorCType,
                                        SQLINTEGER        SourceLocator,
                                        SQLUINTEGER       FromPosition,
                                        SQLUINTEGER       ForLength,
                                        SQLSMALLINT       TargetCType,
                                        SQLPOINTER        rgbValue,
                                        SQLINTEGER        cbValueMax,
                                        SQLINTEGER  FAR   *StringLength,
                                        SQLINTEGER  FAR   *IndicatorValue);
typedef SQLRETURN (SQL_API_FN  *SQLGetTypeInfo_t)(SQLHSTMT          hstmt,
                                        SQLSMALLINT       fSqlType);
typedef SQLRETURN (SQL_API *SQLMoreResults_t)(
    SQLHSTMT           hstmt);
typedef SQLRETURN (SQL_API *SQLNativeSql_t)(
    SQLHDBC            hdbc,
    SQLCHAR               *szSqlStrIn,
    SQLINTEGER         cbSqlStrIn,
    SQLCHAR               *szSqlStr,
    SQLINTEGER         cbSqlStrMax,
    SQLINTEGER            *pcbSqlStr);
typedef SQLRETURN (SQL_API *SQLNumParams_t)(
    SQLHSTMT           hstmt,
    SQLSMALLINT           *pcpar);
typedef SQLRETURN (SQL_API_FN  *SQLNumResultCols_t)(SQLHSTMT          hstmt,
                                        SQLSMALLINT FAR   *pccol);
typedef SQLRETURN (SQL_API_FN  *SQLParamData_t)(SQLHSTMT          hstmt,
                                        SQLPOINTER  FAR   *prgbValue);
typedef SQLRETURN (SQL_API *SQLParamOptions_t)(
    SQLHSTMT           hstmt,
    SQLUINTEGER        crow,
    SQLUINTEGER           *pirow);
typedef SQLRETURN (SQL_API_FN  *SQLPrepare_t)(SQLHSTMT          hstmt,
                                        SQLCHAR     FAR   *szSqlStr,
                                        SQLINTEGER        cbSqlStr);

typedef SQLRETURN (SQL_API *SQLPrimaryKeys_t)(
    SQLHSTMT           hstmt,
    SQLCHAR               *szCatalogName,
    SQLSMALLINT        cbCatalogName,
    SQLCHAR               *szSchemaName,
    SQLSMALLINT        cbSchemaName,
    SQLCHAR               *szTableName,
    SQLSMALLINT        cbTableName);
typedef SQLRETURN (SQL_API *SQLProcedureColumns_t)(
    SQLHSTMT           hstmt,
    SQLCHAR               *szCatalogName,
    SQLSMALLINT        cbCatalogName,
    SQLCHAR               *szSchemaName,
    SQLSMALLINT        cbSchemaName,
    SQLCHAR               *szProcName,
    SQLSMALLINT        cbProcName,
    SQLCHAR               *szColumnName,
    SQLSMALLINT        cbColumnName);
typedef SQLRETURN (SQL_API *SQLProcedures_t)(
    SQLHSTMT           hstmt,
    SQLCHAR               *szCatalogName,
    SQLSMALLINT        cbCatalogName,
    SQLCHAR               *szSchemaName,
    SQLSMALLINT        cbSchemaName,
    SQLCHAR               *szProcName,
    SQLSMALLINT        cbProcName);
typedef SQLRETURN (SQL_API_FN  *SQLPutData_t)(SQLHSTMT          hstmt,
                                        SQLPOINTER        rgbValue,
                                        SQLINTEGER        cbValue);
typedef SQLRETURN (SQL_API_FN  *SQLRowCount_t)(SQLHSTMT          hstmt,
                                        SQLINTEGER  FAR   *pcrow);
typedef SQLRETURN (SQL_API_FN *SQLSetColAttributes_t)(SQLHSTMT        hstmt,
                                        SQLUSMALLINT      icol,
                                        SQLCHAR     FAR   *pszColName,
                                        SQLSMALLINT       cbColName,
                                        SQLSMALLINT       fSQLType,
                                        SQLUINTEGER       cbColDef,
                                        SQLSMALLINT       ibScale,
                                        SQLSMALLINT       fNullable);
typedef SQLRETURN (SQL_API_FN  *SQLSetConnectAttr_t)(
                                        SQLHDBC           hdbc,
                                        SQLINTEGER        fOption,
                                        SQLPOINTER        pvParam,
                                        SQLINTEGER        fStrLen);
typedef SQLRETURN (SQL_API_FN *SQLSetConnection_t)(SQLHDBC           hdbc);
typedef SQLRETURN (SQL_API_FN  *SQLSetConnectOption_t)(
                                        SQLHDBC           hdbc,
                                        SQLUSMALLINT      fOption,
                                        SQLUINTEGER       vParam);
typedef SQLRETURN (SQL_API_FN  *SQLSetCursorName_t)(SQLHSTMT          hstmt,
                                        SQLCHAR     FAR   *szCursor,
                                        SQLSMALLINT       cbCursor);
typedef SQLRETURN  (SQL_API_FN *SQLSetDescField_t)(SQLHDESC DescriptorHandle,
           SQLSMALLINT RecNumber, SQLSMALLINT FieldIdentifier,
           SQLPOINTER Value, SQLINTEGER BufferLength);
typedef SQLRETURN  (SQL_API_FN *SQLSetDescRec_t)(SQLHDESC DescriptorHandle,
           SQLSMALLINT RecNumber, SQLSMALLINT Type,
           SQLSMALLINT SubType, SQLINTEGER Length,
           SQLSMALLINT Precision, SQLSMALLINT Scale,
           SQLPOINTER Data, SQLINTEGER *StringLength,
           SQLINTEGER *Indicator);
typedef SQLRETURN (SQL_API_FN *SQLSetEnvAttr_t)(SQLHENV           henv,
                                        SQLINTEGER        Attribute,
                                        SQLPOINTER        Value,
                                        SQLINTEGER        StringLength);
typedef SQLRETURN (SQL_API_FN  *SQLSetParam_t)(SQLHSTMT          hstmt,
                                        SQLUSMALLINT      ipar,
                                        SQLSMALLINT       fCType,
                                        SQLSMALLINT       fSqlType,
                                        SQLUINTEGER       cbParamDef,
                                        SQLSMALLINT       ibScale,
                                        SQLPOINTER        rgbValue,
                                        SQLINTEGER  FAR   *pcbValue);
typedef SQLRETURN (SQL_API *SQLSetPos_t)(
    SQLHSTMT           hstmt,
    SQLUSMALLINT       irow,
    SQLUSMALLINT       fOption,
    SQLUSMALLINT       fLock);
typedef SQLRETURN (SQL_API_FN  *SQLSetStmtAttr_t)(SQLHSTMT          hstmt,
                                        SQLINTEGER        fOption,
                                        SQLPOINTER        pvParam,
                                        SQLINTEGER       fStrLen);
typedef SQLRETURN (SQL_API_FN  *SQLSetStmtOption_t)(SQLHSTMT          hstmt,
                                        SQLUSMALLINT      fOption,
                                        SQLUINTEGER       vParam);
typedef SQLRETURN (SQL_API_FN  *SQLSpecialColumns_t)(SQLHSTMT          hstmt,
                                        SQLUSMALLINT      fColType,
                                        SQLCHAR     FAR   *szCatalogName,
                                        SQLSMALLINT       cbCatalogName,
                                        SQLCHAR     FAR   *szSchemaName,
                                        SQLSMALLINT       cbSchemaName,
                                        SQLCHAR     FAR   *szTableName,
                                        SQLSMALLINT       cbTableName,
                                        SQLUSMALLINT      fScope,
                                        SQLUSMALLINT      fNullable);
typedef SQLRETURN (SQL_API_FN  *SQLStatistics_t)(SQLHSTMT          hstmt,
                                        SQLCHAR     FAR   *szCatalogName,
                                        SQLSMALLINT       cbCatalogName,
                                        SQLCHAR     FAR   *szSchemaName,
                                        SQLSMALLINT       cbSchemaName,
                                        SQLCHAR     FAR   *szTableName,
                                        SQLSMALLINT       cbTableName,
                                        SQLUSMALLINT      fUnique,
                                        SQLUSMALLINT      fAccuracy);
typedef SQLRETURN (SQL_API *SQLTablePrivileges_t)(
    SQLHSTMT           hstmt,
    SQLCHAR               *szCatalogName,
    SQLSMALLINT        cbCatalogName,
    SQLCHAR               *szSchemaName,
    SQLSMALLINT        cbSchemaName,
    SQLCHAR               *szTableName,
    SQLSMALLINT        cbTableName);
typedef SQLRETURN (SQL_API_FN  *SQLTables_t)(SQLHSTMT          hstmt,
                                        SQLCHAR     FAR   *szCatalogName,
                                        SQLSMALLINT       cbCatalogName,
                                        SQLCHAR     FAR   *szSchemaName,
                                        SQLSMALLINT       cbSchemaName,
                                        SQLCHAR     FAR   *szTableName,
                                        SQLSMALLINT       cbTableName,
                                        SQLCHAR     FAR   *szTableType,
                                        SQLSMALLINT       cbTableType);
typedef SQLRETURN (SQL_API_FN  *SQLTransact_t)(SQLHENV           henv,
                                        SQLHDBC           hdbc,
                                        SQLUSMALLINT      fType);


class SQLAPI_API db2API : public saAPI
{
public:
	db2API();

	SQLAllocConnect_t	SQLAllocConnect;
	SQLAllocEnv_t	SQLAllocEnv;
	SQLAllocHandle_t	SQLAllocHandle;
	SQLAllocStmt_t	SQLAllocStmt;
	SQLBindCol_t	SQLBindCol;
	SQLBindFileToCol_t	SQLBindFileToCol;
	SQLBindFileToParam_t	SQLBindFileToParam;
	SQLBindParameter_t	SQLBindParameter;
	SQLBrowseConnect_t	SQLBrowseConnect;
	SQLBuildDataLink_t	SQLBuildDataLink;
	SQLBulkOperations_t	SQLBulkOperations;
	SQLCancel_t	SQLCancel;
	SQLCloseCursor_t	SQLCloseCursor;
	SQLColAttribute_t	SQLColAttribute;
	SQLColAttributes_t	SQLColAttributes;
	SQLColumnPrivileges_t	SQLColumnPrivileges;
	SQLColumns_t	SQLColumns;
	SQLConnect_t	SQLConnect;
	SQLCopyDesc_t	SQLCopyDesc;
	SQLDataSources_t	SQLDataSources;
	SQLDescribeCol_t	SQLDescribeCol;
	SQLDescribeParam_t	SQLDescribeParam;
	SQLDisconnect_t	SQLDisconnect;
	SQLDriverConnect_t	SQLDriverConnect;
	SQLEndTran_t	SQLEndTran;
	SQLError_t	SQLError;
	SQLExecDirect_t	SQLExecDirect;
	SQLExecute_t	SQLExecute;
	SQLExtendedBind_t	SQLExtendedBind;
	SQLExtendedFetch_t	SQLExtendedFetch;
	SQLExtendedPrepare_t	SQLExtendedPrepare;
	SQLFetch_t	SQLFetch;
	SQLFetchScroll_t	SQLFetchScroll;
	SQLForeignKeys_t	SQLForeignKeys;
	SQLFreeConnect_t	SQLFreeConnect;
	SQLFreeEnv_t	SQLFreeEnv;
	SQLFreeHandle_t	SQLFreeHandle;
	SQLFreeStmt_t	SQLFreeStmt;
	SQLGetConnectAttr_t	SQLGetConnectAttr;
	SQLGetConnectOption_t	SQLGetConnectOption;
	SQLGetCursorName_t	SQLGetCursorName;
	SQLGetData_t	SQLGetData;
	SQLGetDataLinkAttr_t	SQLGetDataLinkAttr;
	SQLGetDescField_t	SQLGetDescField;
	SQLGetDescRec_t	SQLGetDescRec;
	SQLGetDiagField_t	SQLGetDiagField;
	SQLGetDiagRec_t	SQLGetDiagRec;
	SQLGetEnvAttr_t	SQLGetEnvAttr;
	SQLGetFunctions_t	SQLGetFunctions;
	SQLGetInfo_t	SQLGetInfo;
	SQLGetLength_t	SQLGetLength;
	SQLGetPosition_t	SQLGetPosition;
	SQLGetSQLCA_t	SQLGetSQLCA;
	SQLGetStmtAttr_t	SQLGetStmtAttr;
	SQLGetStmtOption_t	SQLGetStmtOption;
	SQLGetSubString_t	SQLGetSubString;
	SQLGetTypeInfo_t	SQLGetTypeInfo;
	SQLMoreResults_t	SQLMoreResults;
	SQLNativeSql_t	SQLNativeSql;
	SQLNumParams_t	SQLNumParams;
	SQLNumResultCols_t	SQLNumResultCols;
	SQLParamData_t	SQLParamData;
	SQLParamOptions_t	SQLParamOptions;
	SQLPrepare_t	SQLPrepare;
	SQLPrimaryKeys_t	SQLPrimaryKeys;
	SQLProcedureColumns_t	SQLProcedureColumns;
	SQLProcedures_t	SQLProcedures;
	SQLPutData_t	SQLPutData;
	SQLRowCount_t	SQLRowCount;
	SQLSetColAttributes_t	SQLSetColAttributes;
	SQLSetConnectAttr_t	SQLSetConnectAttr;
	SQLSetConnection_t	SQLSetConnection;
	SQLSetConnectOption_t	SQLSetConnectOption;
	SQLSetCursorName_t	SQLSetCursorName;
	SQLSetDescField_t	SQLSetDescField;
	SQLSetDescRec_t	SQLSetDescRec;
	SQLSetEnvAttr_t	SQLSetEnvAttr;
	SQLSetParam_t	SQLSetParam;
	SQLSetPos_t	SQLSetPos;
	SQLSetStmtAttr_t	SQLSetStmtAttr;
	SQLSetStmtOption_t	SQLSetStmtOption;
	SQLSpecialColumns_t	SQLSpecialColumns;
	SQLStatistics_t	SQLStatistics;
	SQLTablePrivileges_t	SQLTablePrivileges;
	SQLTables_t	SQLTables;
	SQLTransact_t	SQLTransact;
};

class SQLAPI_API db2ConnectionHandles : public saConnectionHandles
{
public:
	db2ConnectionHandles();

	SQLHENV	m_hevn;
	SQLHDBC	m_hdbc;
};

class SQLAPI_API db2CommandHandles : public saCommandHandles
{
public:
	db2CommandHandles();

	SQLHSTMT	m_hstmt;
};

extern db2API g_db2API;

#endif // !defined(AFX_DB2API_H__11EAA72A_EFF5_4944_8B1F_44973D30EF73__INCLUDED_)
