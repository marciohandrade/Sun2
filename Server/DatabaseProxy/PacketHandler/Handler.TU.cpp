#include "stdafx.h"
#include <PacketStruct_AD.h>
#include "Sessions/ChildServerSession.h"
#include "DBUser.h"
#include "DBUserManager.h"
#include "DBProxyServerEx.h"
#include "QueryObjects/AgentServerQuery.h"
#include <ResultCode.h>
#include <TestHarness.h>

/*#ifdef _DEBUG
TEST( QueryTest, S_User_Select )
{  
	//	CHECK_DOUBLES_EQUAL (someValue, 2.0f);

	DISPMSG( "-------------------------------------start\n" );

	USERGUID UserGuid = 2077;
	DWORD UserKey = UserGuid;
	TCHAR szQueryBuff[MAX_QUERY_LENGTH_64+1];
	_sntprintf( szQueryBuff, MAX_QUERY_LENGTH_64, "S_User_Select %d", UserGuid );
	szQueryBuff[MAX_QUERY_LENGTH_64] = '\0';

	Query_User_Select * pQuery = Query_User_Select::ALLOC();
	memset(pQuery->pResult, 0, sizeof(sQUERY_CHARACTER)*MAX_CHARACTER_SLOT_NUM );
	pQuery->SetQuery( szQueryBuff );
	pQuery->SetUserKey( UserKey );
	AgentServerSession * pServerSession = (AgentServerSession *)g_DBProxyServer.FindSession( 30 );
	if( pServerSession )
		pServerSession->DBQuery( AD_CHARINFO, AD_CHARINFO_CHARLISTREQ_DBR, pQuery );


	DISPMSG( "-------------------------------------ok\n" );
}
#endif*/