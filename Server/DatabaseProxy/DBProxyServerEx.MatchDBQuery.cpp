#include "stdafx.h"

#include <Protocol_DG.h>

#ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
// 캐릭터 생성시, 플레이어가 접속 종료시
void DBProxyServerEx::ExecuteQuery_MatchChar_Update( USERGUID UserGuid, const TCHAR* szCharName, LEVELTYPE Level, BYTE Class )
{
#if defined (_GLOBAL) || defined (_RUSSIA)
    return;
#endif
	STRING_SAFER_CHAR_NAME ssCharName( szCharName );

	STRING_SAFER_QUERY64 szQueryBuff;
	szQueryBuff.MakeString( "S_MatchChar_Update %d,'%s',%d,%d,%d", UserGuid, (TCHAR*)ssCharName, GetGuid(), Level, Class );

	Query_MatchChar_Update* pQuery = Query_MatchChar_Update::ALLOC();
	pQuery->SetIndex( QUERY_MATCHCHAR_UPDATE );
	pQuery->SetVoidObject( this );
	pQuery->SetQuery( szQueryBuff );

	match_database_.DBQuery( pQuery );
	//	DISPMSG( szQueryBuff );
	//	DISPMSG( "\n" );
}

// 캐릭터 삭제시
void DBProxyServerEx::ExecuteQuery_MatchChar_Delete( USERGUID UserGuid, const TCHAR * szCharName )
{
#if defined (_GLOBAL) || defined (_RUSSIA)
    return;
#endif
	STRING_SAFER_CHAR_NAME ssCharName( szCharName );

	STRING_SAFER_QUERY128 szQueryBuff;
	szQueryBuff.MakeString( "S_MatchChar_Delete %d,'%s',%d", UserGuid, (TCHAR*)ssCharName, GetGuid() );

	Query_MatchChar_Delete * pQuery = Query_MatchChar_Delete::ALLOC();
	pQuery->SetIndex( QUERY_MATCHCHAR_DELETE );
	pQuery->SetVoidObject( this );
	pQuery->SetQuery( szQueryBuff );

	match_database_.DBQuery( pQuery );
	//	DISPMSG( szQueryBuff );
	//	DISPMSG( "\n" );
}
#endif

//피로도 관련 쿼리 함수
#ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
void	DBProxyServerEx::ExecuteQuery_MatchFatigue_Update( USERGUID UserGuid, DWORD dwUserKey, INT nPlayTime )
{
	DBUser* pUser = g_DBUserManager.FindUser( UserGuid );
	if( !pUser )
	{
		SUNLOG( eCRITICAL_LOG, "[DBProxyServerEx::ExecuteQuery_MatchFatigue_Update] User Not Found, Guid = %u", UserGuid );
		return;
	}

	if( !pUser->IsFatigueUser() )		return;

	struct tm* tmUpdate;
	time_t curtime;
	util::TimeSync::time( &curtime );
	tmUpdate = gmtime( &curtime );
	tmUpdate = localtime( &curtime );

	// 업데이트 시는 현재의 시간을 업데이트 한다.
	STRING_SAFER_QUERY128 szQueryBuff;
	szQueryBuff.MakeString(
		"{?=call USP_Update_UserFatigued (%d, '%d-%d-%d %d:%d:%d', %d)}", 
		UserGuid, 
		1900+tmUpdate->tm_year, tmUpdate->tm_mon+1, tmUpdate->tm_mday,
		tmUpdate->tm_hour, tmUpdate->tm_min, tmUpdate->tm_sec, 
		nPlayTime
		);

	SUNLOG( eDEV_LOG, "[DBProxyServerEx::ExecuteQuery_MatchFatigue_Update] %s", szQueryBuff );

	Query_MatchFatigue_Update* pQuery = Query_MatchFatigue_Update::ALLOC();
	pQuery->SetIndex( QUERY_MATCHFATIGUE_UPDATE );
	pQuery->SetUserKey( dwUserKey );
	pQuery->SetVoidObject( this );
	pQuery->SetQuery( szQueryBuff );

	match_database_.DBQuery( pQuery );
}

void	DBProxyServerEx::ExecuteQuery_MatchFatigue_Select( USERGUID UserGuid, DWORD dwUserKey )
{
	DBUser* pUser = g_DBUserManager.FindUser( UserGuid );
	if( !pUser )
	{
		SUNLOG( eCRITICAL_LOG, _T("[DBProxyServerEx::ExecuteQuery_MatchFatigue_Select] User Not Found, Guid = %u"), UserGuid );
		return;
	}

	if( !pUser->IsFatigueUser() )		//피로도를 적용시킬 필요가 없으면..
	{
		pUser->SetPlayTime( 0 );
		pUser->SetOfflineTime( 0 );
		return;
	}

	struct tm* tmLogin;
	time_t	LoginTime = pUser->GetLoginTime();
	tmLogin = gmtime( &LoginTime );
	tmLogin = localtime( &LoginTime );

	STRING_SAFER_QUERY128 szQueryBuff;
	szQueryBuff.MakeString(
		"{?=call USP_Select_UserFatigued (%u, '%d-%d-%d %d:%d:%d')}",
		UserGuid,
		1900+tmLogin->tm_year, tmLogin->tm_mon+1, tmLogin->tm_mday,
		tmLogin->tm_hour, tmLogin->tm_min, tmLogin->tm_sec
		);

	Query_MatchFatigue_Select* pQuery = Query_MatchFatigue_Select::ALLOC();
	pQuery->SetIndex( QUERY_MATCHFATIGUE_SELECT );
	pQuery->SetUserKey( dwUserKey );
	pQuery->SetVoidObject( this );
	pQuery->SetQuery( szQueryBuff );

	match_database_.DBQuery( pQuery );
}
#endif //__CN_OO0510__FATIGUE_MATCHING_UPDATE
