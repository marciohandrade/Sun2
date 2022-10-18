#include "stdafx.h"

#include "Services/DBSerialManager.h"

#include <PacketStruct_DG.h>

VOID DBProxyServerEx::DBResult( DWORD dwIndex, Query* pData )
{
	DWORD dwQueryTime = pData->GetQueryTime();
	if( dwQueryTime > 1000 )
	{
		SUNLOG( eCRITICAL_LOG, _T("QueryDelay T0: [%u=%usec] %s"), dwQueryTime, dwQueryTime/1000, pData->GetQuery() );
	}

	switch( dwIndex )
	{
	case QUERY_SELECTDBSERIAL:
		{
			CreateItemSerialQuery* pQData = (CreateItemSerialQuery*)pData;
			if( !pQData->IsError() )
			{
				g_pDBSerialManager->DBResult( true, pQData->GetCurDBSerial(), 
					pQData->GetCurDBSerial() + g_pDBSerialManager->GetInterval() );
				//m_pDBSerialGenerator->SetDBSerial( pQData->GetCurDBSerial(), m_pDBSerialGenerator->GetDBSerialInterval() );
				TCHAR* const pOutFormat = "[NOTIFY]: Issued Serial Res. (DBCur: %I64u, DBMax: %I64u)";
			
				SUNLOG(eCRITICAL_LOG, pOutFormat,
					pQData->GetCurDBSerial(), pQData->GetCurDBSerial() + g_pDBSerialManager->GetInterval() );
			}
			else
			{
				g_pDBSerialManager->DBResult( true, 0, 0);

				//m_pDBSerialGenerator->SetQueryCompleted();
				SUNLOG(eCRITICAL_LOG, _T("[NOTIFY][FAIL]: Can't Issue Serial Res.") );
			}
			CreateItemSerialQuery::FREE( pQData );
			pQData = NULL;
		}
		break;
		//case QUERY_UPDATEWAREHOUSE:
		//	{
		//		Query_Warehouse_Update * pQData = (Query_Warehouse_Update *)pData;
		//		if( Query_Warehouse_Update::QUERY_RESULT_SUCCEEDED != pQData->pParam[0].m_Result )
		//		{
		//			SUNLOG(eFULL_LOG, "[DBProxyServerEx::DBResult] [U:%u][오류%d]:창고정보저장에러", pQData->GetUserKey(), pQData->pParam[0].m_Result );
		//			SUNLOG(eCRITICAL_LOG, "[DBProxyServerEx::DBResult] %s", pQData->GetQuery() );
		//		}
		//		else
		//		{
		//			//SUNLOG( eFULL_LOG, "[%s]창고정보저장성공", pQData->GetCharName() );
		//		}
		//		Query_Warehouse_Update::FREE( pQData ); pQData = NULL;
		//	}
		//	break;
		//case QUERY_UPDATECHARACTER:
		//	{
		//		Query_Char_Update * pQData = (Query_Char_Update *)pData;
		//		if( !pQData->ResultSuccess() )
		//		{
		//			SUNLOG(eFULL_LOG,  "[DBProxyServerEx::DBResult] [U:%u][오류%d]:캐릭터정보저장에러", pQData->GetUserKey(), pQData->pParam[0].m_Result );
		//			SUNLOG(eCRITICAL_LOG,  "[DBProxyServerEx::DBResult] %s", pQData->GetQuery() );
		//		}
		//		else
		//		{
		//			//SUNLOG(eFULL_LOG,  "[U:%d][성공]:캐릭터정보저장성공", pQData->GetUserKey() );
		//		}

		//		Query_Char_Update::FREE( pQData ); pQData = NULL;
		//	}
		//	break;
#ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
	case QUERY_MATCHCHAR_UPDATE:
		{
			Query_MatchChar_Update* pQData = (Query_MatchChar_Update*)pData;
			Query_MatchChar_Update::FREE( pQData );
			pQData = NULL;
		}
		break;
	case QUERY_MATCHCHAR_DELETE:
		{
			Query_MatchChar_Delete* pQData = (Query_MatchChar_Delete*)pData;
			Query_MatchChar_Delete::FREE( pQData );
			pQData = NULL;
		}
		break;
	case QUERY_MATCHFATIGUE_UPDATE:
		{
			Query_MatchFatigue_Update* pQResult = (Query_MatchFatigue_Update*)pData;
			DWORD dwUserKey = pQResult->GetUserKey();

			DBUser* pUser = g_DBUserManager.FindUser(dwUserKey);

			if( !pUser )
			{
				SUNLOG( eCRITICAL_LOG, _T("[DBResult][QUERY_MATCHFATIGUE_UPDATE] DBUserIsNotFound (UserKey = %u)"), dwUserKey );
			}
			else if( !pQResult->ResultSuccess() )
			{
				SUNLOG( eDEV_LOG, _T("[DBResult][QUERY_MATCHFATIGUE_UPDATE] Query Fail, Query = %s "), pQResult->GetQuery() );
			}
			else
			{
				SUNLOG( eDEV_LOG, _T("[DBResult][QUERY_MATCHFATIGUE_UPDATE] Update Success ") );
			}

			Query_MatchFatigue_Update::FREE( pQResult );
			pQResult = NULL;
		}
		break;
	case QUERY_MATCHFATIGUE_SELECT:
		{
			Query_MatchFatigue_Select* pQResult	= (Query_MatchFatigue_Select*)pData;
			DWORD dwUserKey = pQResult->GetUserKey();

			DBUser* pUser = g_DBUserManager.FindUser(dwUserKey);

			if( !pUser )
			{
				SUNLOG( eCRITICAL_LOG, _T("[DG_CHARINFO_CONNECT_CHAR_CMD] DBUserIsNotFound (UserKey = %u)"), dwUserKey );
			}
			else if( !pQResult->ResultSuccess() )
			{
				pUser->SetPlayTime( 0 );
				pUser->SetOfflineTime( 0 );
				SUNLOG( eDEV_LOG, _T("[DBResult][QUERY_MATCHFATIGUE_SELECT] Query Fail, [UGuid:%d] Query = %s "), dwUserKey, pQResult->GetQuery() );
			}
			else
			{
				pUser->SetPlayTime( pQResult->pSelect[0].m_nPlayTime );
				pUser->SetOfflineTime( pQResult->pSelect[0].m_nOfflineTime );
				SUNLOG( eDEV_LOG, _T("[DBResult][QUERY_MATCHFATIGUE_SELECT] UGuid:%d, PlayTime = %u, OfflineTime = %u"),
					dwUserKey, pQResult->pSelect[0].m_nPlayTime, pQResult->pSelect[0].m_nOfflineTime );
			}
			Query_MatchFatigue_Select::FREE( pQResult );
			pQResult = NULL;
		}
		break;
#endif //__CN_OO0510__FATIGUE_MATCHING_UPDATE
	}
}

//------------------------------------------------------------------------------------------------
// 디비쪽 콜백
// using ODBC Thread
VOID	CallBackMessage1( CHAR* szMessage )
{
	SUNCONNLOG( eCRITICAL_LOG, _T("[Callback DBMsg] %s"), szMessage );
}

// using ODBC Thread
VOID	CallBackMessageEx( WORD wDBIndex, QueryResult* pData )
{
	DBProxyServerEx* pDBProxy = (DBProxyServerEx*)pData->GetVoidObject();

	if( strncmp( pData->GetSqlState(), "08S01", 5 ) == 0 )
	{
		if(!pDBProxy->IsDBDisconnected())
		{
			DWORD dwStartQueryTime = pData->GetStartQueryTime();
			if( pDBProxy->SetDBDisconnected(true, dwStartQueryTime))
				SUNCONNLOG( eCRITICAL_LOG, _T("Disconnect DBProxyDB") );
		}
	}

	SUNCONNLOG( eCRITICAL_LOG, _T("[DBIndex:%u]: %s, SQLSTATE:%s, NativeError:%u, %s"),
		wDBIndex, pData->GetErrorMsg(), pData->GetSqlState(), pData->GetNativeError(), pData->GetQuery() );
}

// using ODBC Thread
VOID	CallBackMessage1_Match( CHAR* szMessage )
{
	SUNCONNLOG( eCRITICAL_LOG, _T("[Callback MatchDBMsg]%s"), szMessage );
}

// using ODBC Thread
VOID	CallBackMessageEx_Match( WORD wDBIndex, QueryResult* pData )
{
	DBProxyServerEx* pDBProxy = (DBProxyServerEx*)pData->GetVoidObject();

	if( strncmp( pData->GetSqlState(), "08S01", 5 ) == 0 )
	{
		if(!pDBProxy->IsMatchDBDisconnected())
		{
			DWORD dwStartQueryTime = pData->GetStartQueryTime();
			if( pDBProxy->SetMatchDBDisconnected(true, dwStartQueryTime))
				SUNCONNLOG( eCRITICAL_LOG, _T("Disconnect MatchDB") );
		}
	}

	SUNCONNLOG( eCRITICAL_LOG, _T("[MatchDBIndex:%u]: %s, SQLSTATE:%s, NativeError:%u, %s"),
		wDBIndex, pData->GetErrorMsg(), pData->GetSqlState(), pData->GetNativeError(), pData->GetQuery() );
}

// using LOGIC Thread
VOID	CallBackQueryResult1( DWORD dwIndex, QueryResult* pData )
{
	//DWORD dwQueryTime = pData->GetQueryTime();
	//if( dwQueryTime > 1000 )
	//{
	//	SUNCONNLOG( eCRITICAL_LOG, _T("QueryHighDelay T1:[%u=%u초] %s"), dwQueryTime, dwQueryTime/1000, pData->GetQuery() );
	//}
	((DBProxyServerEx *)pData->GetVoidObject())->DBResult( dwIndex, pData );
}


