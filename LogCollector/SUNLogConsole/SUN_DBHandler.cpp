/*------------------------------------------------------------------------------
 * SUN_DBHandler.cpp
 *------------------------------------------------------------------------------
 * DBHandler Ŭ������ ��ӹ޾� ���� SUN ���� DBHandler Ŭ�����̴�.
 *
 * �ĸ��� �α��� data type�� ���� DB�� �����ϴ� �Լ��� �ۼ��ؾ� �Ѵ�.
 * (�ۼ��� �Լ����� �ĸ��� ���� �����Ϳ����� ����ؾ� �Ѵ�.)
 *------------------------------------------------------------------------------
 * All rights reserved by Jin Hye Jin (jiny7749@hotmail.com)
**----------------------------------------------------------------------------*/

#include "stdafx.h"
#include "SUN_DBHandler.h"
#include "LogCollectorLog.h"

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
SUN_DBHandler::SUN_DBHandler(int max, TCHAR * dsn, TCHAR * id, TCHAR * passwd)
{
	if (NULL == dsn ||
		NULL == id ||
		NULL == passwd)
	{
		_ASSERTE(!"SUN_DBHandler");
	}

	m_pDBPool = new DatabasePool(max, dsn, id, passwd);
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
SUN_DBHandler::~SUN_DBHandler()
{
	if (NULL != m_pDBPool)
	{
		delete m_pDBPool, m_pDBPool = NULL;
	}
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
BOOL SUN_DBHandler::WriteInvenSnapShotBlob(TCHAR* szSQL, InventoryDataEx& stData)
{
	const int INVEN_SIZE = stData.GetInventorySize();
	const int TMP_INVEN_SIZE = stData.GetTempInventorySize();

	SQLINTEGER             iData1 = INVEN_SIZE;//MAX_INVENTORYITEM_SIZE;
	SQLINTEGER             iData2 = TMP_INVEN_SIZE;//MAX_TEMPINVENTORYITEM_SIZE;

	// lParam�� �α� ���� ��κ� �����Ͱ� ������ ���·� ���;� �Ѵ�.
	// �׸��� SQLBindParameter()�� �Ŀ� �޸𸮿� ī������, SQLExecute(SQLStateHandle) �ϸ� �����Ͱ� ���Եȴ�.
	/////////////////////////////////////////////////
	UCHAR sqlQuery[512] = {0,};
	SQLHSTMT	hstmt = SQL_NULL_HSTMT;
	SQLHDBC		hdbc = SQL_NULL_HDBC;
	SQLRETURN	retcode;
	DBInfo*	pdbConnection = NULL;
	LogCollectorLog cSQLLog(_T("SQL_FAIL_LOG"), _T("SQL_FAIL_LOG"));
	///////////////////////


	pdbConnection = m_pDBPool->GetDBHandle();
	if ( pdbConnection == NULL ) 
	{
		cSQLLog.AddLog(_T("Error DBConnection is null\n"));
		return FALSE;
	}

	hdbc  = pdbConnection->hSQLConnection;		
	hstmt = AllocStatement(hdbc);

	if (hstmt == SQL_NULL_HSTMT)
	{
		cSQLLog.AddLog(_T("Error SQL_NULL_HSTMT\n"));
		m_pDBPool->ReleaseDBHandle(pdbConnection);
		return FALSE;
	}
	/////////////////////////////////////////////////

	// ������ ������
	retcode = SQLPrepare(hstmt, (SQLTCHAR*)szSQL, SQL_NTS);
	if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO && retcode != SQL_NO_DATA && retcode != SQL_STILL_EXECUTING && retcode != SQL_NEED_DATA)
	{
		SQLTCHAR       SqlState[6], Msg[SQL_MAX_MESSAGE_LENGTH];
		SQLINTEGER    NativeError;
		SQLSMALLINT   i = 1, MsgLen;
		SQLGetDiagRec(SQL_HANDLE_STMT, hstmt, i, SqlState, &NativeError,
			Msg, sizeof(Msg), &MsgLen);

		cSQLLog.AddLog(_T("WriteBlob InventoryData Error, SQLSTATE : %d, Msg : %s"), SqlState , Msg);
		m_pDBPool->ReleaseDBHandle(pdbConnection);
		FreeStatement(hstmt);
		TRACE(_T("SQL Prepare failed : sql : %s"), szSQL);
		return FALSE;
	}

	// ������ ���ε�
	retcode = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_BINARY, iData1, 0, stData.GetInventoryPtr(), 0, &iData1);
	retcode = SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_BINARY, iData2, 0, stData.GetTempInventoryPtr(), 0, &iData2);

	// ���� 
	retcode = SQLExecute(hstmt); //(retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO) && (retcode != SQL_NO_DATA) && (retcode != SQL_STILL_EXECUTING) && (retcode != SQL_NEED_DATA)

	if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO && retcode != SQL_NO_DATA && retcode != SQL_STILL_EXECUTING && retcode != SQL_NEED_DATA)
	{
		TRACE(_T("not SQL_SUCCESS, SQL_SUCCESS_WITH_INFO, szSQL : %s"), szSQL);

		SQLTCHAR       SqlState[6], Msg[SQL_MAX_MESSAGE_LENGTH];
		SQLINTEGER    NativeError;
		SQLSMALLINT   i = 1, MsgLen;
		SQLGetDiagRec(SQL_HANDLE_STMT, hstmt, i, SqlState, &NativeError,
			Msg, sizeof(Msg), &MsgLen);

		cSQLLog.AddLog(_T("WriteBlob Inventory Data Error, SQLSTATE : %d, Msg : %s"), SqlState , Msg);

		FreeStatement(hstmt);
		m_pDBPool->ReleaseDBHandle(pdbConnection);
		return FALSE;
	}

	FreeStatement(hstmt);
	m_pDBPool->ReleaseDBHandle(pdbConnection);

	return TRUE;
}

BOOL SUN_DBHandler::WriteItemBlob(TCHAR* szSQL, ItemDataEx& stData)
{
	SQLINTEGER             iData;
	SQLHSTMT    hstmt = SQL_NULL_HSTMT;
	SQLHDBC     hdbc = SQL_NULL_HDBC;
	SQLRETURN   retcode;
	pDBInfo     pdbConnection = NULL;	
	LogCollectorLog cSQLLog(_T("SQL_FAIL_LOG"), _T("SQL_FAIL_LOG"));

	pdbConnection = m_pDBPool->GetDBHandle();
	if ( pdbConnection == NULL ) 
	{
		cSQLLog.AddLog(_T("Error DBConnection is null\n"));
		return FALSE;
	}

	hdbc  = pdbConnection->hSQLConnection;		
	hstmt = AllocStatement(hdbc);

	if (hstmt == SQL_NULL_HSTMT)
	{
		cSQLLog.AddLog(_T("Error SQL_NULL_HSTMT\n"));
		m_pDBPool->ReleaseDBHandle(pdbConnection);
		return FALSE;
	}

	retcode = SQLPrepare(hstmt, (SQLTCHAR*)szSQL, SQL_NTS);
	if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO && retcode != SQL_NO_DATA && retcode != SQL_STILL_EXECUTING && retcode != SQL_NEED_DATA)
	{
		SQLTCHAR       SqlState[6], Msg[SQL_MAX_MESSAGE_LENGTH];
		SQLINTEGER    NativeError;
		SQLSMALLINT   i = 1, MsgLen;
		SQLGetDiagRec(SQL_HANDLE_STMT, hstmt, i, SqlState, &NativeError,
			Msg, sizeof(Msg), &MsgLen);

		cSQLLog.AddLog(_T("WriteBlob ItemData Error, SQLSTATE : %d, Msg : %s"), SqlState , Msg);
		m_pDBPool->ReleaseDBHandle(pdbConnection);
		FreeStatement(hstmt);
		TRACE(_T("SQL Prepare failed : sql : %s"), szSQL);
		return FALSE;
	}

	// ������ ���ε�
	//////////////////////////////////////////////////////////////////////////
    const int MAX_ITEM_SIZE1 = stData.GetItemStreamEx1Size();
    const int MAX_ITEM_SIZE2 = stData.GetItemStreamEx2Size();

#ifdef _TEST_SERVER
	TCHAR szOutput1[100];
	TCHAR szOutput2[100];

	ZeroMemory(szOutput1, 100);
	ZeroMemory(szOutput1, 100);

	StringCchPrintf(szOutput1, sizeof(szOutput1), _T("ItemStreamEX1Size : %d"), MAX_ITEM_SIZE1);
	StringCchPrintf(szOutput2, sizeof(szOutput2), _T("ItemStreamEX2Size : %d"), MAX_ITEM_SIZE2);

	OutputDebugString(szOutput1);
	OutputDebugString(szOutput2);
#endif //_TEST_SERVER

	//////////////////////////////////////////////////////////////////////////

	iData = MAX_ITEM_SIZE1;
	retcode = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_BINARY, SQL_BINARY, iData, 0, stData.GetItemStreamEx1(), 0, &iData);

	iData = MAX_ITEM_SIZE2;
	retcode = SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_BINARY, SQL_BINARY, iData, 0, stData.GetItemStreamEx2(), 0, &iData);

	// ���� 
	retcode = SQLExecute(hstmt); 

	if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO && retcode != SQL_NO_DATA && retcode != SQL_STILL_EXECUTING && retcode != SQL_NEED_DATA)
	{
		TRACE(_T("not SQL_SUCCESS, SQL_SUCCESS_WITH_INFO, szSQL : %s"), szSQL);

		SQLTCHAR       SqlState[6], Msg[SQL_MAX_MESSAGE_LENGTH];
		SQLINTEGER    NativeError;
		SQLSMALLINT   i = 1, MsgLen;
		SQLGetDiagRec(SQL_HANDLE_STMT, hstmt, i, SqlState, &NativeError,
			Msg, sizeof(Msg), &MsgLen);

		cSQLLog.AddLog(_T("WriteBlob ItemData Data Error, SQLSTATE : %d, Msg : %s"), SqlState , Msg);

		FreeStatement(hstmt);
		m_pDBPool->ReleaseDBHandle(pdbConnection);
		return FALSE;
	}

	FreeStatement(hstmt);
	m_pDBPool->ReleaseDBHandle(pdbConnection);

	return TRUE;

}

BOOL SUN_DBHandler::WriteSnapShotBlob(TCHAR* szSQL, SnapShotDataEx& stData)
{
	SQLHSTMT    hstmt = SQL_NULL_HSTMT;
	SQLHDBC     hdbc = SQL_NULL_HDBC;
	SQLRETURN   retcode;
	pDBInfo     pdbConnection = NULL;	
	LogCollectorLog cSQLLog(_T("SQL_FAIL_LOG"), _T("SQL_FAIL_LOG"));

	pdbConnection = m_pDBPool->GetDBHandle();
	if ( pdbConnection == NULL ) 
	{
		cSQLLog.AddLog(_T("Error DBConnection is null\n"));
		return FALSE;
	}

	hdbc  = pdbConnection->hSQLConnection;		
	hstmt = AllocStatement(hdbc);

	if (hstmt == SQL_NULL_HSTMT)
	{
		cSQLLog.AddLog(_T("Error SQL_NULL_HSTMT\n"));
		m_pDBPool->ReleaseDBHandle(pdbConnection);
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	// ����� DLL�� ���������� �������� ��.
	// 2007-11-20 pavonine66
	const int iCharStatSize = stData.GetCharStatSize();
	const int iCharInvenSize = stData.GetInventorySize();
	const int iCharTmpInvenSize = stData.GetTempInventorySize();
	const int iCharEquipItemSize = stData.GetEquipItemSize();
	const int iCharSkillSize = stData.GetSkillSize();
	const int iCharMissionSize = stData.GetMissionSize();
	const int iCharProgQuestSize = stData.GetProgrQuestSize();
	const int iCharCompQuestSize = stData.GetComplQuestSize();
	const int iCharAcSize = stData.GetACSize();
	//////////////////////////////////////////////////////////////////////////

	SQLINTEGER             iData1 = iCharStatSize;
	SQLINTEGER             iData2 = iCharInvenSize;
	SQLINTEGER             iData3 = iCharTmpInvenSize;
	SQLINTEGER             iData4 = iCharEquipItemSize;
	SQLINTEGER             iData5 = iCharSkillSize;
	SQLINTEGER             iData6 = iCharMissionSize;
	SQLINTEGER             iData7 = iCharProgQuestSize;
	SQLINTEGER             iData8 = iCharCompQuestSize;
	SQLINTEGER             iData9 = iCharAcSize;

	retcode = SQLPrepare(hstmt, (SQLTCHAR*)szSQL, SQL_NTS);
	if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO && retcode != SQL_NO_DATA && retcode != SQL_STILL_EXECUTING && retcode != SQL_NEED_DATA)
	{
		SQLTCHAR       SqlState[6], Msg[SQL_MAX_MESSAGE_LENGTH];
		SQLINTEGER    NativeError;
		SQLSMALLINT   i = 1, MsgLen;
		SQLGetDiagRec(SQL_HANDLE_STMT, hstmt, i, SqlState, &NativeError,
			Msg, sizeof(Msg), &MsgLen);

		cSQLLog.AddLog(_T("WriteBlob SnapShotData Error, SQLSTATE : %d, Msg : %s"), SqlState , Msg);
		m_pDBPool->ReleaseDBHandle(pdbConnection);
		FreeStatement(hstmt);
		TRACE(_T("SQL Prepare failed : sql : %s"), szSQL);
		return FALSE;
	}

	// ������ ���ε�
	retcode = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_BINARY, iCharStatSize, 0, stData.GetCharStatPtr(), 0, &iData1);
	retcode = SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_BINARY, iCharInvenSize, 0, stData.GetInventoryPtr(), 0, &iData2);
	retcode = SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_BINARY, iCharTmpInvenSize, 0, stData.GetTempInventoryPtr(), 0, &iData3);
	retcode = SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_BINARY, iCharEquipItemSize, 0, stData.GetEquipItemPtr(), 0, &iData4);
	retcode = SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_BINARY, iCharSkillSize, 0, stData.GetSkillPtr(), 0, &iData5);
	retcode = SQLBindParameter(hstmt, 6, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_BINARY, iCharMissionSize, 0, stData.GetMissionPtr(), 0, &iData6);
	retcode = SQLBindParameter(hstmt, 7, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_BINARY, iCharProgQuestSize, 0, stData.GetProgrQuestPtr(), 0, &iData7);
	retcode = SQLBindParameter(hstmt, 8, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_BINARY, iCharCompQuestSize, 0, stData.GetComplQuestPtr(), 0, &iData8);
	retcode = SQLBindParameter(hstmt, 9, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_BINARY, iCharAcSize, 0, stData.GetACPtr(), 0, &iData9);

	retcode = SQLExecute(hstmt);

	if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO && retcode != SQL_NO_DATA && retcode != SQL_STILL_EXECUTING && retcode != SQL_NEED_DATA)
	{
		TRACE(_T("not SQL_SUCCESS, SQL_SUCCESS_WITH_INFO, szSQL : %s"), szSQL);

		SQLTCHAR       SqlState[6], Msg[SQL_MAX_MESSAGE_LENGTH];
		SQLINTEGER    NativeError;
		SQLSMALLINT   i = 1, MsgLen;
		SQLGetDiagRec(SQL_HANDLE_STMT, hstmt, i, SqlState, &NativeError,
			Msg, sizeof(Msg), &MsgLen);

		cSQLLog.AddLog(_T("WriteBlob SnapshotData Data Error, SQLSTATE : %d, Msg : %s"), SqlState , Msg);

		FreeStatement(hstmt);
		m_pDBPool->ReleaseDBHandle(pdbConnection);
		return FALSE;
	}

	FreeStatement(hstmt);
	m_pDBPool->ReleaseDBHandle(pdbConnection);

	return TRUE;
}

BOOL SUN_DBHandler::WriteWarehouseBlob(TCHAR* szSQL, WareHouseDataEx& stData)
{
	SQLINTEGER             iData;

	SQLHSTMT    hstmt = SQL_NULL_HSTMT;
	SQLHDBC     hdbc = SQL_NULL_HDBC;
	SQLRETURN   retcode;
	pDBInfo     pdbConnection = NULL;	
	LogCollectorLog cSQLLog(_T("SQL_FAIL_LOG"), _T("SQL_FAIL_LOG"));

	pdbConnection = m_pDBPool->GetDBHandle();
	if ( pdbConnection == NULL ) 
	{
		cSQLLog.AddLog(_T("Error DBConnection is null\n"));
		return FALSE;
	}

	hdbc  = pdbConnection->hSQLConnection;		
	hstmt = AllocStatement(hdbc);

	if (hstmt == SQL_NULL_HSTMT)
	{
		cSQLLog.AddLog(_T("Error SQL_NULL_HSTMT\n"));
		m_pDBPool->ReleaseDBHandle(pdbConnection);
		return FALSE;
	}

	retcode = SQLPrepare(hstmt, (SQLTCHAR*)szSQL, SQL_NTS);
	if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO && retcode != SQL_NO_DATA && retcode != SQL_STILL_EXECUTING && retcode != SQL_NEED_DATA)
	{
		SQLTCHAR       SqlState[6], Msg[SQL_MAX_MESSAGE_LENGTH];
		SQLINTEGER    NativeError;
		SQLSMALLINT   i = 1, MsgLen;
		SQLGetDiagRec(SQL_HANDLE_STMT, hstmt, i, SqlState, &NativeError,
			Msg, sizeof(Msg), &MsgLen);

		cSQLLog.AddLog(_T("WriteBlob SnapShotData Error, SQLSTATE : %d, Msg : %s"), SqlState , Msg);
		m_pDBPool->ReleaseDBHandle(pdbConnection);
		FreeStatement(hstmt);
		TRACE(_T("SQL Prepare failed : sql : %s"), szSQL);
		return FALSE;
	}

	const int IWAREHOUSE_SIZE = stData.GetWareHouseSize();
	iData = IWAREHOUSE_SIZE;

	// ������ ���ε�
	retcode = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_BINARY, SQL_BINARY, iData, 0, stData.GetWareHousePtr(), 0, &iData);

	// ���� 
	retcode = SQLExecute(hstmt); 

	if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO && retcode != SQL_NO_DATA && retcode != SQL_STILL_EXECUTING && retcode != SQL_NEED_DATA)
	{
		TRACE(_T("not SQL_SUCCESS, SQL_SUCCESS_WITH_INFO, szSQL : %s"), szSQL);

		SQLTCHAR       SqlState[6], Msg[SQL_MAX_MESSAGE_LENGTH];
		SQLINTEGER    NativeError;
		SQLSMALLINT   i = 1, MsgLen;
		SQLGetDiagRec(SQL_HANDLE_STMT, hstmt, i, SqlState, &NativeError,
			Msg, sizeof(Msg), &MsgLen);

		cSQLLog.AddLog(_T("WriteBlob WareHouseData Data Error, SQLSTATE : %d, Msg : %s"), SqlState , Msg);

		FreeStatement(hstmt);
		m_pDBPool->ReleaseDBHandle(pdbConnection);
		return FALSE;
	}

	FreeStatement(hstmt);
	m_pDBPool->ReleaseDBHandle(pdbConnection);

	return TRUE;
}