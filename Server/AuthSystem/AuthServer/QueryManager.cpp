#include "stdafx.h"
#include "QueryManager.h"

//==================================================================================================

#include <ISharemem.h>
#include "ServerDataManager.h"
#include "AuthFrame.h"
#include "Query_Auth.h"
#include "Query_Log.h"
#include "Query_RTT.h"
#include "Query_Netcafe.h"
#include "Query_Extra.h"
#include "Query_GSP.h"
#include "DBWorker.h"
#include <AuthStepAuthToken.h>

//==================================================================================================
//
//

QueryManager::QueryManager()
{
    m_dbWorker = NULL;
    m_AllocQueryCount = 0;
}

QueryManager::~QueryManager()
{
}

//==================================================================================================
//  국가별 풀 설정
//_AUTH_KOREA _AUTH_CHINA _AUTH_JAPAN _AUTH_TAIWAN _AUTH_USA _AUTH_GLOBAL
#if 0
#elif defined(_AUTH_KOREA)  || \
      defined(_AUTH_CHINA)  || \
      defined(_AUTH_TAIWAN) || \
      defined(_AUTH_USA)    || \
      defined(_AUTH_RUSSIA)
static const int QueryPoolBaseSize[QUERYTYPE_MAX] =
{
    500,    //QUERYTYPE_LOGINUSER,
    500,    //QUERYTYPE_USERSTS,
     10,    //QUERYTYPE_RECOVERSTS,
    500,    //QUERYTYPE_AUTHLOG,
     10,    //QUERYTYPE_CONTROLIP,
     10,    //QUERYTYPE_GAMEVERSION,
    500,    //QUERYTYPE_INCRESEAUTHFAILCOUNT,
    500,    //QUERYTYPE_AUTHLOG_UPDATE,
      1,    //QUERYTYPE_NETCATFE_LOG,           // Japan _NET_CAFE
      1,    //QUERYTYPE_NETCATFE_LOGIN,         // (WAVERIX) (090708) (CHANGES) x (BUG-FIX)
      1,    //QUERYTYPE_NETCAFE_OTP_CHECK       // (100119) add in
     50,    //QUERYTYPE_USERID,                 //__NA000000_SHOP_USERID_50_PASSING_PROC__
      1,    //QUERYTYPE_GSP_CHECK_AUTH,         // GSP DB   - Check AuthKey
      1,    //QUERYTYPE_GSP_ACCOUNT_STATE,      // GSP DB   - Change Login User State
      1,    //QUERYTYPE_GSP_CONNECT_LOG,        // GSP DB   - Logging User State
      1,    //QUERYTYPE_GSP_ACCOUNT_ID,         // GSP DB   - Get Account ID from GameID
      1,    //QUERYTYPE_GSP_ACCOUNT_ID_BY_GUID, // GSP DB - Get Account ID by AccountGuid
	  50,	//QUERYTYPE_TOKEN_UPDATE
      1,    //QUERYTYPE_PCROOM_IPLIST
      50,   //QUERYTYPE_CHECK_PASSWORD
      1,    //QUERYTYPE_END,
};
#elif defined(_AUTH_JAPAN)
static const int QueryPoolBaseSize[QUERYTYPE_MAX] =
{
    500,    //QUERYTYPE_LOGINUSER,
    500,    //QUERYTYPE_USERSTS,
     10,    //QUERYTYPE_RECOVERSTS,
    500,    //QUERYTYPE_AUTHLOG,
     10,    //QUERYTYPE_CONTROLIP,
     10,    //QUERYTYPE_GAMEVERSION,
    500,    //QUERYTYPE_INCRESEAUTHFAILCOUNT,
    500,    //QUERYTYPE_AUTHLOG_UPDATE,
    500,    //QUERYTYPE_NETCATFE_LOG,           // Japan _NET_CAFE
    500,    //QUERYTYPE_NETCATFE_LOGIN,         // (WAVERIX) (090708) (CHANGES) x (BUG-FIX)
    500,    //QUERYTYPE_NETCAFE_OTP_CHECK       // (100119) add in
     50,    //QUERYTYPE_USERID,                 //__NA000000_SHOP_USERID_50_PASSING_PROC__
      1,    //QUERYTYPE_GSP_CHECK_AUTH,         // GSP DB   - Check AuthKey
      1,    //QUERYTYPE_GSP_ACCOUNT_STATE,      // GSP DB   - Change Login User State
      1,    //QUERYTYPE_GSP_CONNECT_LOG,        // GSP DB   - Logging User State
      1,    //QUERYTYPE_GSP_ACCOUNT_ID,         // GSP DB   - Get Account ID from GameID
      1,    //QUERYTYPE_GSP_ACCOUNT_ID_BY_GUID, // GSP DB - Get Account ID by AccountGuid
	  50,	//QUERYTYPE_TOKEN_UPDATE
      1,    //QUERYTYPE_PCROOM_IPLIST
      1,    //QUERYTYPE_END,
};
#elif defined(_AUTH_GLOBAL)
static const int QueryPoolBaseSize[QUERYTYPE_MAX] =
{
      1,    //QUERYTYPE_LOGINUSER,
      1,    //QUERYTYPE_USERSTS,
      1,    //QUERYTYPE_RECOVERSTS,
      1,    //QUERYTYPE_AUTHLOG,
      1,    //QUERYTYPE_CONTROLIP,
      1,    //QUERYTYPE_GAMEVERSION,
      1,    //QUERYTYPE_INCRESEAUTHFAILCOUNT,
      1,    //QUERYTYPE_AUTHLOG_UPDATE,
      1,    //QUERYTYPE_NETCATFE_LOG,           // Japan _NET_CAFE
      1,    //QUERYTYPE_NETCATFE_LOGIN,         // (WAVERIX) (090708) (CHANGES) x (BUG-FIX)
      1,    //QUERYTYPE_NETCAFE_OTP_CHECK       // (100119) add in
      1,    //QUERYTYPE_USERID,                 //__NA000000_SHOP_USERID_50_PASSING_PROC__
    500,    //QUERYTYPE_GSP_CHECK_AUTH,         // GSP DB   - Check AuthKey
    500,    //QUERYTYPE_GSP_ACCOUNT_STATE,      // GSP DB   - Change Login User State
    500,    //QUERYTYPE_GSP_CONNECT_LOG,        // GSP DB   - Logging User State
    500,    //QUERYTYPE_GSP_ACCOUNT_ID,         // GSP DB   - Get Account ID from GameID
    500,    //QUERYTYPE_GSP_ACCOUNT_ID_BY_GUID, // GSP DB - Get Account ID by AccountGuid
	 50,    //QUERYTYPE_TOKEN_UPDATE
      1,    //QUERYTYPE_PCROOM_IPLIST
      1,    //QUERYTYPE_END,
};
#endif
//==================================================================================================
template<typename _type, int _enum>
struct QueryInfoNode
{
    typedef _type type;
    static const int value = _enum;
};

#define QUERY_POOL_NAME(_enum)  INFONODE_##_enum

#define LIST_QUERY_NODES()  \
    DECL_QUERY_NODE(QueryAuth,                  QUERYTYPE_LOGINUSER)      \
    DECL_QUERY_NODE(QueryUserStsChange,         QUERYTYPE_USERSTS)        \
    DECL_QUERY_NODE(QueryRecoverLocation,       QUERYTYPE_RECOVERSTS)     \
    DECL_QUERY_NODE(QueryAuthLog,               QUERYTYPE_AUTHLOG)        \
    DECL_QUERY_NODE(QueryControlIp,             QUERYTYPE_CONTROLIP)      \
    DECL_QUERY_NODE(QueryGameVersionUpdate,     QUERYTYPE_GAMEVERSION)    \
    DECL_QUERY_NODE(QueryAuthFailCount,         QUERYTYPE_INCRESEAUTHFAILCOUNT) \
    DECL_QUERY_NODE(QueryAuthLogUpdate,         QUERYTYPE_AUTHLOG_UPDATE) \
    DECL_QUERY_NODE(QueryNetCafeLog,            QUERYTYPE_NETCATFE_LOG)   \
    DECL_QUERY_NODE(QueryNetCafeLogIn,          QUERYTYPE_NETCATFE_LOGIN) \
    DECL_QUERY_NODE(QueryNetCafe2ndAuth,        QUERYTYPE_NETCAFE_OTP_CHECK) \
    DECL_QUERY_NODE(QueryUserId,                QUERYTYPE_USERID)         \
    DECL_QUERY_NODE(nsGSP::QueryGspCheckAuthKey,    QUERYTYPE_GSP_CHECK_AUTH)     \
    DECL_QUERY_NODE(nsGSP::QueryGspChangeUserState, QUERYTYPE_GSP_ACCOUNT_STATE)  \
    DECL_QUERY_NODE(nsGSP::QueryGspLoggingUserState,QUERYTYPE_GSP_CONNECT_LOG)    \
    DECL_QUERY_NODE(nsGSP::QueryGspGetAccount,      QUERYTYPE_GSP_ACCOUNT_ID)     \
    DECL_QUERY_NODE(nsGSP::QueryGspGetAccountByGuid,QUERYTYPE_GSP_ACCOUNT_ID_BY_GUID) \
	DECL_QUERY_NODE(QueryUpdateTokenKey			   ,QUERYTYPE_TOKEN_UPDATE) \
    DECL_QUERY_NODE(QuerySelectPCRoomIPList 	   ,QUERYTYPE_PCROOM_IPLIST) \
    DECL_QUERY_NODE(QueryCheckPassword,         QUERYTYPE_CHECK_PASSWORD) \
    DECL_QUERY_NODE(QueryEnd,                   QUERYTYPE_END)

#undef DECL_QUERY_NODE
#define DECL_QUERY_NODE(_type_name, _enum)  \
    typedef QueryInfoNode<_type_name, _enum>    INFONODE_##_enum;

// declaration types
LIST_QUERY_NODES();

//==================================================================================================

void
QueryManager::Create(int DBWorkerNum)
{
#undef DECL_QUERY_NODE
#define DECL_QUERY_NODE(_type_name, _enum)  \
    m_QueryPool.Register<QUERY_POOL_NAME(_enum)::type>(QUERY_POOL_NAME(_enum)::value);

    // register query pool types
    LIST_QUERY_NODES();

#undef DECL_QUERY_NODE
#define DECL_QUERY_NODE(_type_name, _enum)  \
    m_QueryPool.Initialize(QUERY_POOL_NAME(_enum)::value, \
                           QUERY_POOL_NAME(_enum)::value, \
                           QUERY_POOL_NAME(_enum)::value >> 1);

    // register query pool value
    LIST_QUERY_NODES();

    m_dbWorkerNum = DBWorkerNum;
    m_dbWorker = new DBWorker*[m_dbWorkerNum];

    m_QueryQueue.Create(m_dbWorkerNum);

    for(int i = 0; i < m_dbWorkerNum; ++i)
    {
        m_dbWorker[i] = _new_dbg_ DBWorker();
        m_dbWorker[i]->Initialize();
    }
}

void
QueryManager::Destroy()
{
    util::CSyncCriticalSection cs(m_csPool);

    for(int i = 0; i < m_dbWorkerNum; ++i)
    {
        if(m_dbWorker[i] == 0)
            continue;

        m_dbWorker[i]->Destroy();

        delete m_dbWorker[i];
    }
    delete [] m_dbWorker;

    m_QueryPool.Release();
}

//==================================================================================================

void
QueryManager::Update()
{
    GUARD(guard, m_cslist);

    QueryObject* pQuery = NULL;
    LIST_QUERY_RESULT::iterator it = m_listQueryResult.begin(),
                                end = m_listQueryResult.end();

    for( ; it != end; )
    {
        pQuery = *it;

        if(pQuery->m_Success)
            pQuery->onResult();

        it = m_listQueryResult.erase(it);
        FreeQuery(pQuery);
    }   
}

//==================================================================================================

void
QueryManager::AddQueryResult(QueryObject* pQuery)
{
    GUARD(guard, m_cslist);

    m_listQueryResult.push_back(pQuery);
}

QueryObject*
QueryManager::AllocQuery(eQUERYTYPE type)
{
    util::CSyncCriticalSection cs(m_csPool);
    QueryObject* pQuery = (QueryObject*)m_QueryPool.Alloc(type);
    pQuery->m_StartTime = ::GetTickCount();

    ++m_AllocQueryCount;
    return pQuery;
}

void
QueryManager::FreeQuery(QueryObject* pQuery)
{
    util::CSyncCriticalSection cs(m_csPool);

    pQuery->Release();
    m_QueryPool.Free(pQuery->m_queryType, pQuery);

    --m_AllocQueryCount;
}

void
QueryManager::EndQuery()
{
    QueryEnd* queryObj = (QueryEnd*)AllocQuery(QUERYTYPE_END);
    m_QueryQueue.PostCompletion(queryObj, 0);
}

QueryObject*
QueryManager::WaitForQueryObj(int WaitTick)
{
    eXtreme::AsyncResult tmp;
    eXtreme::AsyncResult* asyncResult = &tmp;

    // AsyncResult는 여기에서 사용하지 않지만, 인자를 Null로 처리할 수 없으므로 임시로 사용한다.
    QueryObject* queryObj =
        dynamic_cast<QueryObject*>(m_QueryQueue.GetCompletionEvent(&asyncResult, WaitTick));

    return queryObj;
}

//==================================================================================================

#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
void 
QueryManager::QueryLoginUserByAuthTokeyGameChu(DWORD login_front_id,
                    GAMECHUUSN gamechu_token, DWORD frontUserId,
                    nsPacket::ReAuthInfoBase::ReAuthKeyType reAuthKey,
                    LPTSTR clientIp, LPTSTR localIp)
{
    char query_buffer[256] = { 0, };

    QueryAuth* queryObj = (QueryAuth*)AllocQuery(QUERYTYPE_LOGINUSER);
    queryObj->Configuration(eAuthToken_AuthTokenGameChu);

    queryObj->m_key = login_front_id;
    queryObj->m_front_user_id = frontUserId;
    queryObj->m_ReAuthKey = reAuthKey;
    queryObj->gamechu_usn_ = gamechu_token;    

    queryObj->m_password[0] = '\0';
    ::_tcsncpy(queryObj->m_clientIp, clientIp, IP_MAX_LEN - 1);
    ::_tcsncpy(queryObj->m_localIp, localIp, IP_MAX_LEN - 1);

    AuthFrame* const auth_frame = AuthFrame::Instance();
    char block_limit = char(auth_frame->GetBlockLimit());
    int block_time = int(auth_frame->GetBlockTime());    

    _snprintf(query_buffer, _countof(query_buffer),
        "EXEC S_LoginUserForGameChu %d, '%s', %d, %d",
        queryObj->gamechu_usn_, clientIp, block_limit, block_time);

    eXtreme::RecordSet& recordSet = queryObj->m_recordSet;
    recordSet.RegisterParameter(NULL, "@UserID", adInteger, adParamInput,
        (long)sizeof(DWORD), _bstr_t(queryObj->gamechu_usn_));
    recordSet.RegisterParameter(NULL, "@IpAddr", adVarChar, adParamInput,
        (long)strlen(clientIp), _bstr_t(clientIp));
    recordSet.RegisterParameter(NULL, "@ti_BlockLimit", adTinyInt, adParamInput,
        (long)sizeof(BYTE), _bstr_t(block_limit));
    recordSet.RegisterParameter(NULL, "@i_BlockTime", adInteger, adParamInput,
        (long)sizeof(DWORD), _bstr_t(block_time));

    queryObj->m_spName = "S_LoginUserForGameChu";
    queryObj->m_query = query_buffer;

    m_QueryQueue.PostCompletion(queryObj, 0);
}
#endif

#ifdef _RU_INGAMBA_AUTH_PROCESS
void 
QueryManager::QueryLoginUserByRussia(DWORD login_front_id, DWORD front_user_id, int user_guid,
                                     const char* user_id, nsPacket::ReAuthInfoBase::ReAuthKeyType re_auth_key,
                                     LPTSTR client_ip, LPTSTR local_ip)
{
    char query_string[256] = {0, };

    QueryAuth* query_object = (QueryAuth*)AllocQuery(QUERYTYPE_LOGINUSER);
    query_object->Configuration(eAuthToken_AuthIngamba);

    query_object->m_key = login_front_id;
    query_object->m_front_user_id = front_user_id;
    query_object->m_ReAuthKey = re_auth_key;

    CHECK_ID_LENGTH_BUFFER(query_object->m_account);
    ::_tcsncpy(query_object->m_account, user_id, _countof(query_object->m_account));
    query_object->m_clientIp[_countof(query_object->m_account) - 1] = '\0';
    _strupr(query_object->m_account);
    ::_tcsncpy(query_object->m_clientIp, client_ip, _countof(query_object->m_clientIp));
    query_object->m_clientIp[_countof(query_object->m_clientIp) - 1] = '\0';
    ::_tcsncpy(query_object->m_localIp, local_ip, _countof(query_object->m_localIp));
    query_object->m_localIp[_countof(query_object->m_localIp) - 1] = '\0';

    AuthFrame* auth_frame = AuthFrame::Instance();
    char block_limit = char(auth_frame->GetBlockLimit());
    int block_time = int(auth_frame->GetBlockTime());

    const char* procedure_name = "S_LoginUserForRussia";
    const char* query_format = "EXEC S_LoginUserForRussia %d, '%s'";

    _snprintf(query_string, _countof(query_string), query_format, user_guid, query_object->m_account);
    SUNLOG(eCRITICAL_LOG, "query : %s", query_string);

    eXtreme::RecordSet& record_set = query_object->m_recordSet;
    record_set.RegisterParameter(0, 
                                "@UserGuid",
                                adInteger, 
                                adParamInput,
                                (long)sizeof(int), 
                                _bstr_t(user_guid));
    record_set.RegisterParameter(0,
                                "@UserID",
                                adVarChar, 
                                adParamInput,
                                (long)strlen(query_object->m_account), 
                                _bstr_t(query_object->m_account));
    query_object->m_spName = procedure_name;
    query_object->m_query = query_string;

    m_QueryQueue.PostCompletion(query_object, 0);
}

void 
QueryManager::QueryReLogintUserByRussia(DWORD login_front_id, DWORD front_user_id, const char* user_id, 
                                        nsPacket::ReAuthInfoBase::ReAuthKeyType re_auth_key,
                                        LPTSTR client_ip, LPTSTR local_ip)
{
    char query_string[256] = {0, };

    QueryAuth* query_object = (QueryAuth*)AllocQuery(QUERYTYPE_LOGINUSER);
    query_object->Configuration(eAuthToken_AuthIngamba);

    query_object->m_key = login_front_id;
    query_object->m_front_user_id = front_user_id;
    query_object->m_ReAuthKey = re_auth_key;

    CHECK_ID_LENGTH_BUFFER(query_object->m_account);
    ::_tcsncpy(query_object->m_account, user_id, _countof(query_object->m_account));
    query_object->m_clientIp[_countof(query_object->m_account) - 1] = '\0';
    _strupr(query_object->m_account);
    ::_tcsncpy(query_object->m_clientIp, client_ip, _countof(query_object->m_clientIp));
    query_object->m_clientIp[_countof(query_object->m_clientIp) - 1] = '\0';
    ::_tcsncpy(query_object->m_localIp, local_ip, _countof(query_object->m_localIp));
    query_object->m_localIp[_countof(query_object->m_localIp) - 1] = '\0';

    AuthFrame* auth_frame = AuthFrame::Instance();
    char block_limit = char(auth_frame->GetBlockLimit());
    int block_time = int(auth_frame->GetBlockTime());

    const char* procedure_name = "S_SelectUserForRussia";
    const char* query_format = "EXEX S_SelectUserForRussia '%s'";

    _snprintf(query_string, _countof(query_string), query_format, query_object->m_account);
    SUNLOG(eCRITICAL_LOG, "query : %s", query_string);

    eXtreme::RecordSet& record_set = query_object->m_recordSet;
    record_set.RegisterParameter(0, "@UserID", adVarChar, adParamInput,
                                 (long)strlen(query_object->m_account), 
                                 _bstr_t(query_object->m_account));
    query_object->m_spName = procedure_name;
    query_object->m_query = query_string;

    m_QueryQueue.PostCompletion(query_object, 0);
}
#endif

#ifdef __NA000000_NORTH_AMERICA_HANGAME_CHANNELING__
void
QueryManager::QueryLoginUserByAuthTokenNorthA(DWORD login_front_id, LPTSTR account,
                                              //BYTE* auth_token, WORD auth_token_length,
                                              DWORD frontUserId,
                                              nsPacket::ReAuthInfoBase::ReAuthKeyType reAuthKey,
                                              LPTSTR clientIp, LPTSTR localIp)
{
    NORTHA_DEBUGLOG(__FUNCTION__"%d\n", 0);
    BOOST_STATIC_ASSERT(sizeof(TCHAR) == sizeof(char));
    char query_string[256];

    QueryAuth* query = (QueryAuth*)AllocQuery(QUERYTYPE_LOGINUSER);
    query->Configuration(eAuthToken_AuthTokenNorthA);
    query->m_key = login_front_id;
    query->m_front_user_id = frontUserId;
    query->m_ReAuthKey = reAuthKey;
    CHECK_ID_LENGTH_BUFFER(query->m_account);
    strncpy(query->m_account, account, _countof(query->m_account));
    query->m_account[_countof(query->m_account) - 1] = '\0';
    // (WAVERIX) (090805) (CHANGES) 북미는 소문자 기반이지만, 기존에 모두 대문자로 처리하므로...
    // 하기 로직'_strupr'은 초반에 호출하던 것을 후반에 호출하도록 위치 변경이 이뤄진 것이다.
    _strupr(query->m_account);

    strncpy(query->m_clientIp, clientIp, _countof(query->m_clientIp));
    query->m_clientIp[_countof(query->m_clientIp) - 1] = '\0';
    strncpy(query->m_localIp, localIp, _countof(query->m_localIp));
    query->m_localIp[_countof(query->m_localIp) - 1] = '\0';

    AuthFrame* auth_frame = AuthFrame::Instance();
    char block_limit = char(auth_frame->GetBlockLimit());
    int block_time = int(auth_frame->GetBlockTime());

    BOOST_STATIC_WARNING(0);
    CopyMemory(query->m_AuthToken, AuthStepAuthToken::AuthTokenStreamNull,
               sizeof(query->m_AuthToken));

#if _AUTH_NORTHA_TEST_MODE == _AUTH_NORTHA_TEST_MODE__DEVTEST__
    const char* procedure_name = "S_LoginUserForNorthA";
    const char* query_format = "EXEX S_LoginUserForNorthA '%s', '%s', %d, %d";
#elif _AUTH_NORTHA_TEST_MODE == _AUTH_NORTHA_TEST_MODE__DEVIDPWD__
    const char* procedure_name = "S_LoginUser";
    const char* query_format = "EXEC S_LoginUser '%s', '%s', %d, %d";
#else // alpha version <= current mode
    const char* procedure_name = "S_LoginUserForNorthA";
    const char* query_format = "EXEX S_LoginUserForNorthA '%s', '%s', %d, %d";
#endif
    NORTHA_DEBUGLOG(__FUNCTION__" mode - %d\n", _AUTH_NORTHA_TEST_MODE);

    _snprintf(query_string, _countof(query_string),
              query_format,
              query->m_account, query->m_clientIp, block_limit, block_time);

    NORTHA_DEBUGLOG(__FUNCTION__" query string %s\n", query_string);

    eXtreme::RecordSet& record_set = query->m_recordSet;
    record_set.RegisterParameter(0, "@UserID",
                                 adVarChar, adParamInput,
                                 (long)strlen(query->m_account), _bstr_t(query->m_account));
    record_set.RegisterParameter(0, "@IpAddr",
                                 adVarChar, adParamInput,
                                 (long)strlen(query->m_clientIp), _bstr_t(query->m_clientIp));
    record_set.RegisterParameter(0, "@ti_BlockLimit",
                                 adTinyInt, adParamInput,
                                 (long)sizeof(BYTE), _bstr_t(block_limit));
    record_set.RegisterParameter(0, "@i_BlockTime",
                                 adInteger, adParamInput,
                                 (long)sizeof(int), _bstr_t(block_time));

    query->m_spName = procedure_name;
    query->m_query = query_string;

    m_QueryQueue.PostCompletion(query, 0);
}
#endif

//==================================================================================================

#ifdef _CHANNELING_AUTH_FLOW
void
QueryManager::QueryLoginUserByAuthTokenKorea(DWORD login_front_id, LPTSTR account,
                                             BYTE* auth_token, WORD auth_token_length,
                                             DWORD frontUserId,
                                             nsPacket::ReAuthInfoBase::ReAuthKeyType reAuthKey,
                                             LPTSTR clientIp, LPTSTR localIp)
{
    BOOST_STATIC_ASSERT(sizeof(TCHAR) == sizeof(char));
    char query_string[256];

    QueryAuth* query = (QueryAuth*)AllocQuery(QUERYTYPE_LOGINUSER);
    query->Configuration(eAuthToken_AuthTokenKorea);
    query->m_key = login_front_id;
    query->m_front_user_id = frontUserId;
    query->m_ReAuthKey = reAuthKey;
    CHECK_ID_LENGTH_BUFFER(query->m_account);
    strncpy(query->m_account, account, _countof(query->m_account));
    query->m_account[_countof(query->m_account) - 1] = '\0';
    strncpy(query->m_clientIp, clientIp, _countof(query->m_clientIp));
    query->m_clientIp[_countof(query->m_clientIp) - 1] = '\0';
    strncpy(query->m_localIp, localIp, _countof(query->m_localIp));
    query->m_localIp[_countof(query->m_localIp) - 1] = '\0';

    for(int i = 0; i < _countof(query->m_AuthToken); ++i)
      query->m_AuthToken[i] = auth_token[i];

    AuthFrame* auth_frame = AuthFrame::Instance();
    char block_limit = char(auth_frame->GetBlockLimit());
    int block_time = int(auth_frame->GetBlockTime());

#if !defined(_CHANNELING_AUTH_FLOW_HANGAME_USER_)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif
#ifdef _NA_20100307_BETAKEY_SYSTEM
    const char* procedure_name = "S_LoginUserForChanneling";
    const char* query_format = "EXEC S_LoginUserForChanneling '%s', '%s', %d, %d";
/* <jaykang> 20110526 dummy beta key
    const char* procedure_name = "S_LoginUserForChannelingBetakey";
    const char* query_format = "EXEC S_LoginUserForChannelingBetakey '%s', '%s', %d, %d";
*/
#else
    const char* procedure_name = "S_LoginUserForChanneling";
    const char* query_format = "EXEC S_LoginUserForChanneling '%s', '%s', %d, %d";
#endif

    _snprintf(query_string, sizeof(query_string),
              query_format,
              query->m_account, query->m_clientIp, block_limit, block_time);

    eXtreme::RecordSet& record_set = query->m_recordSet;
    record_set.RegisterParameter(0, "@UserID",
                                 adVarChar, adParamInput,
                                 (long)strlen(query->m_account), _bstr_t(query->m_account));
    record_set.RegisterParameter(0, "@IpAddr",
                                 adVarChar, adParamInput,
                                 (long)strlen(query->m_clientIp), _bstr_t(query->m_clientIp));
    record_set.RegisterParameter(0, "@ti_BlockLimit",
                                 adTinyInt, adParamInput,
                                 (long)sizeof(BYTE), _bstr_t(block_limit));
    record_set.RegisterParameter(0, "@i_BlockTime",
                                 adInteger, adParamInput,
                                 (long)sizeof(int), _bstr_t(block_time));

    query->m_spName = procedure_name;
    query->m_query = query_string;

    m_QueryQueue.PostCompletion(query, 0);
}
#endif //_CHANNELING_AUTH_FLOW

//==================================================================================================
#ifdef __NA000000_GLOBAL_AUTH_PROCESS__
void
QueryManager::QueryLoginUserByAuthTokenGlobal(DWORD login_front_id, LPTSTR game_id,
                                              char* auth_token, WORD auth_token_length,
                                              DWORD frontUserId,
                                              nsPacket::ReAuthInfoBase::ReAuthKeyType reAuthKey,
                                              LPTSTR client_ip, LPTSTR local_ip,
                                              LPTSTR mac_address)
{
    using namespace nsGSP;
    const char game_code = nsGSP::eGameCode_SUN;

    char temp[256];
    QueryGspCheckAuthKey* queryObj =
        static_cast<QueryGspCheckAuthKey*>(AllocQuery(QUERYTYPE_GSP_CHECK_AUTH));

    queryObj->Clear(eAuthToken_AuthTokenGlobal); //side-effect : alloc inited AccountInfo 'account_'
    queryObj->m_key = login_front_id; //
    queryObj->reauth_key_ = reAuthKey; // 만약 서버 선택일 경우라면 이미 키 인증은 끝난 상태다.

    AccountInfo* temp_account = queryObj->account_;
    temp_account->m_LoginFrontId = login_front_id;
    temp_account->m_LoginFrontUserId = frontUserId;
    strncpy(temp_account->m_localIp, local_ip, _countof(temp_account->m_localIp));
    temp_account->m_localIp[_countof(temp_account->m_localIp) - 1] = '\0';

    QueryGspCheckAuthKey::InputArgs& input_args = queryObj->input_args_;
    strncpy(input_args.game_id, game_id, _countof(input_args.game_id));
    input_args.game_id[_countof(input_args.game_id) - 1] = '\0';
    input_args.game_code = game_code;
    strncpy(input_args.auth_key, auth_token, _countof(input_args.auth_key));
    input_args.auth_key[_countof(input_args.auth_key) - 1] = '\0';
    strncpy(input_args.client_ip, client_ip, _countof(input_args.client_ip));
    input_args.client_ip[_countof(input_args.client_ip) - 1] = '\0';
    strncpy(input_args.mac_address, mac_address, _countof(input_args.mac_address));
    input_args.mac_address[_countof(input_args.mac_address) - 1] = '\0';

    _snprintf(temp, _countof(temp),
#ifdef _NA_20100307_BETAKEY_SYSTEM
              "EXEC gb.USP_WS_CheckAuthKey_SUN '%s', %d, '%s', '%s', '%s'",
#else
              "EXEC gb.USP_WS_CheckAuthKey '%s', %d, '%s', '%s', '%s'",
#endif
              input_args.game_id, input_args.game_code,
              input_args.auth_key, input_args.client_ip, input_args.mac_address);

    eXtreme::RecordSet& recordSet = queryObj->m_recordSet;
    recordSet.RegisterParameter(0, "@GameID", adVarChar, adParamInput,
                                (long)strlen(input_args.game_id), _bstr_t(input_args.game_id));
    recordSet.RegisterParameter(0, "@GameCode", adTinyInt, adParamInput,
                                sizeof(input_args.game_code), _bstr_t(input_args.game_code));
    recordSet.RegisterParameter(0, "@AuthKey", adVarChar, adParamInput,
                                (long)strlen(input_args.auth_key), _bstr_t(input_args.auth_key));

    recordSet.RegisterParameter(0, "@IP", adVarChar, adParamInput,
                                (long)strlen(input_args.client_ip), _bstr_t(input_args.client_ip));
    {   // MAC Address : 일단 MAC Address는 사용하지 않는 것으로 처리한다.
        long mac_address_len = (long)strlen(mac_address);
        mac_address = input_args.mac_address;
        if(mac_address_len == 0)
            mac_address_len = _countof(input_args.mac_address) - 1;

        recordSet.RegisterParameter(0, "@MacAddress", adVarChar, adParamInput,
                                    mac_address_len,
                                    _bstr_t(input_args.mac_address));
    };

    QueryGspCheckAuthKey::ResultData& result_block = queryObj->result_data_;

    _bstr_t empty = _bstr_t();
    recordSet.RegisterParameter(&result_block.account_guid, "@AccountGuid",
                                adInteger, adParamOutput,
                                sizeof(result_block.account_guid), empty);
    recordSet.RegisterParameter(&result_block.account_type, "@AccountType",
                                adTinyInt, adParamOutput,
                                sizeof(result_block.account_type), empty);
    recordSet.RegisterParameter(&result_block.block_state, "@BlockState",
                                adTinyInt, adParamOutput,
                                sizeof(result_block.block_state), empty);
    recordSet.RegisterParameter(&result_block.result, "@Result",
                                adTinyInt, adParamOutput,
                                sizeof(result_block.result), empty);
#ifdef _NA_20100307_BETAKEY_SYSTEM
    recordSet.RegisterParameter(&result_block.result, "@BetaValue",
                                adVarChar, adParamOutput,
                                sizeof(result_block.betakey), empty);
#endif

#ifdef _NA_20100307_BETAKEY_SYSTEM
    queryObj->m_spName = "gb.USP_WS_CheckAuthKey_SUN";
#else
    queryObj->m_spName = "gb.USP_WS_CheckAuthKey";
#endif    
    queryObj->m_query = temp;

    m_QueryQueue.PostCompletion(queryObj, 0);
}
#endif //__NA000000_GLOBAL_AUTH_PROCESS__

#ifdef _CN_ZY_AUTH_PROCESS
void QueryManager::QueryCheckPasswordByChinaZY(DWORD login_front_id, DWORD frontUserId, \
                                               LPTSTR account, LPTSTR password, \
                                               nsPacket::ReAuthInfoBase::ReAuthKeyType reAuthKey,
                                               LPTSTR clientIp, LPTSTR localIp)
{
    QueryCheckPassword* queryObj = static_cast<QueryCheckPassword*>(AllocQuery(QUERYTYPE_CHECK_PASSWORD));
    CHECK_ID_LENGTH_BUFFER(queryObj->m_account);
    ::_tcsncpy(queryObj->m_account, account, _countof(queryObj->m_account));
    ::_tcsncpy(queryObj->m_password, password, PASSWD_MAX_LEN - 1);
    queryObj->m_key = login_front_id;
    queryObj->m_front_user_id = frontUserId;
    queryObj->m_ReAuthKey = reAuthKey;
    ::_tcsncpy(queryObj->m_clientIp, clientIp, IP_MAX_LEN - 1);
    ::_tcsncpy(queryObj->m_localIp,  localIp,  IP_MAX_LEN - 1);
    queryObj->m_spName = "S_LoginUserCheckPassword";

    //char query_buffer[256] = { 0, };
    //_snprintf(query_buffer, _countof(query_buffer), \
    //    "EXEC S_LoginUserCheckPassword '%s', '%s'", account, password);
    //queryObj->m_query = query_buffer;

    eXtreme::RecordSet& recordSet = queryObj->m_recordSet;
    recordSet.RegisterParameter(NULL, "@UserID", adVarChar, adParamInput,
        (long)strlen(account), _bstr_t(account));
    recordSet.RegisterParameter(NULL, "@Password", adVarChar, adParamInput,
        (long)strlen(password), _bstr_t(password));

    m_QueryQueue.PostCompletion(queryObj, 0);
}

void QueryManager::QueryLoginUserByChinaZY( DWORD login_front_id, LPTSTR account, LPTSTR password, 
    DWORD frontUserId, nsPacket::ReAuthInfoBase::ReAuthKeyType reAuthKey,
    LPTSTR clientIp, LPTSTR localIp )
{
    QueryAuth* queryObj = (QueryAuth*)AllocQuery(QUERYTYPE_LOGINUSER);
    queryObj->Configuration(eAuthToken_AuthChinaZY);

    // Query를 작성하고, QueryQueue로 삽입한다.
    queryObj->m_key = login_front_id;
    queryObj->m_front_user_id = frontUserId;
    queryObj->m_ReAuthKey = reAuthKey;

    CHECK_ID_LENGTH_BUFFER(queryObj->m_account);
    ::_tcsncpy(queryObj->m_account, account, _countof(queryObj->m_account));
    queryObj->m_account[_countof(queryObj->m_account) - 1] = '\0';
    //::_tcsncpy(queryObj->m_password, password, PASSWD_MAX_LEN - 1);//QueryCheckPasswordByChinaZY()
    ::_tcsncpy(queryObj->m_clientIp, clientIp, IP_MAX_LEN - 1);
    ::_tcsncpy(queryObj->m_localIp,  localIp,  IP_MAX_LEN - 1);

    AuthFrame* const auth_frame = AuthFrame::Instance();
    char block_limit = char(auth_frame->GetBlockLimit());
    int block_time = int(auth_frame->GetBlockTime());

    eXtreme::RecordSet& recordSet = queryObj->m_recordSet;
    recordSet.RegisterParameter(NULL, "@UserID", adVarChar, adParamInput,
                                (long)strlen(account), _bstr_t(account));
    recordSet.RegisterParameter(NULL, "@IpAddr", adVarChar, adParamInput,
                                (long)strlen(clientIp), _bstr_t(clientIp));
    recordSet.RegisterParameter(NULL, "@ti_BlockLimit", adTinyInt, adParamInput,
                                (long)sizeof(BYTE), _bstr_t(block_limit));
    recordSet.RegisterParameter(NULL, "@i_BlockTime", adInteger, adParamInput,
                                (long)sizeof(DWORD), _bstr_t(block_time));
    queryObj->m_spName = "S_LoginUser";

    //char query_buffer[256] = { 0, };
    //_snprintf(query_buffer, _countof(query_buffer), \
    //    "EXEC S_LoginUser '%s', '%s', %d, %d", account, clientIp, block_limit, block_time);
    //queryObj->m_query = query_buffer;

    m_QueryQueue.PostCompletion(queryObj, 0);
}
#endif //_CN_ZY_AUTH_PROCESS

//==================================================================================================

void
QueryManager::QueryLoginUser(DWORD login_front_id,
                             LPTSTR account, LPTSTR password,
                             DWORD frontUserId,
                             nsPacket::ReAuthInfoBase::ReAuthKeyType reAuthKey,
                             LPTSTR clientIp, LPTSTR localIp)
{
    char query_buffer[256] = { 0, };

    QueryAuth* queryObj = (QueryAuth*)AllocQuery(QUERYTYPE_LOGINUSER);
    queryObj->Configuration(eAuthToken_None);

    // Query를 작성하고, QueryQueue로 삽입한다.
    queryObj->m_key = login_front_id;
    queryObj->m_front_user_id = frontUserId;
    queryObj->m_ReAuthKey = reAuthKey;

    CHECK_ID_LENGTH_BUFFER(queryObj->m_account);
    ::_tcsncpy(queryObj->m_account, account, _countof(queryObj->m_account));
    queryObj->m_account[_countof(queryObj->m_account) - 1] = '\0';

    ::_tcsncpy(queryObj->m_password, password, PASSWD_MAX_LEN - 1);
    ::_tcsncpy(queryObj->m_clientIp, clientIp, IP_MAX_LEN - 1);
    ::_tcsncpy(queryObj->m_localIp, localIp, IP_MAX_LEN - 1);

    AuthFrame* const auth_frame = AuthFrame::Instance();
    char block_limit = char(auth_frame->GetBlockLimit());
    int block_time = int(auth_frame->GetBlockTime());

#ifdef _NA_20100307_BETAKEY_SYSTEM
    _snprintf(query_buffer, _countof(query_buffer),
                                        "EXEC S_LoginUser '%s', '%s', %d, %d",
                                        account, clientIp, block_limit, block_time);
/* <jaykang> 20110526 dummy beta key
    _snprintf(query_buffer, _countof(query_buffer),
                                        "EXEC S_LoginUserForBetaKey '%s', '%s', %d, %d",
                                        account, clientIp, block_limit, block_time);
*/
#else
    _snprintf(query_buffer, _countof(query_buffer),
                                        "EXEC S_LoginUser '%s', '%s', %d, %d",
                                        account, clientIp, block_limit, block_time);
#endif
    

    eXtreme::RecordSet& recordSet = queryObj->m_recordSet;
    recordSet.RegisterParameter(NULL, "@UserID", adVarChar, adParamInput,
                                (long)strlen(account), _bstr_t(account));
    recordSet.RegisterParameter(NULL, "@IpAddr", adVarChar, adParamInput,
                                (long)strlen(clientIp), _bstr_t(clientIp));
    recordSet.RegisterParameter(NULL, "@ti_BlockLimit", adTinyInt, adParamInput,
                                (long)sizeof(BYTE), _bstr_t(block_limit));
    recordSet.RegisterParameter(NULL, "@i_BlockTime", adInteger, adParamInput,
                                (long)sizeof(DWORD), _bstr_t(block_time));
#ifdef _NA_20100307_BETAKEY_SYSTEM
    queryObj->m_spName = "S_LoginUser";
/* <jaykang> 20110526 dummy beta key
    queryObj->m_spName = "S_LoginUserForBetaKey";
*/
#else
    queryObj->m_spName = "S_LoginUser";
#endif    
    queryObj->m_query = query_buffer;

    m_QueryQueue.PostCompletion(queryObj, 0);
}

//==================================================================================================
//!_AUTH_GLOBAL
void
QueryManager::QueryChangeUserSts(LPTSTR account, DWORD userGuid, LPTSTR glogKey, AGENTID serverId,
                                 LPTSTR publicIp, LPTSTR localIp,
                                 USERSTATUS_TYPE status, SYSTEMTIME &loginTime, DWORD authKey)
{
    SYSTEMTIME localTime;
    TCHAR tmp[256] = { 0, };
    TCHAR StsTime[32], ConnectTime[32];

    AGENTID location = 0;
    ServerData* pServerData = ServerDataManager::Instance()->FindServerData(serverId);
    if(pServerData)
        location = pServerData->GetGameAgentID();

    QueryUserStsChange* queryObj = (QueryUserStsChange*)AllocQuery(QUERYTYPE_USERSTS);

    ::GetLocalTime(&localTime);

    queryObj->m_key = 0;

    //  인증서버 재시작으로 복구된 계정인 경우, LoginTime이 모두 0이다.
    if(loginTime.wYear == 0)
    {
        loginTime.wYear     = localTime.wYear;
        loginTime.wMonth    = localTime.wMonth;
        loginTime.wDay      = localTime.wDay;
        loginTime.wHour     = 1;
        loginTime.wMinute   = 1;
    }

    ::_stprintf(StsTime, "%d-%d-%d %d:%d:0", localTime.wYear, localTime.wMonth, localTime.wDay, localTime.wHour, localTime.wMinute);
    ::_stprintf(ConnectTime, "%d-%d-%d %d:%d:0", loginTime.wYear, loginTime.wMonth, loginTime.wDay, loginTime.wHour, loginTime.wMinute);

    if(AuthFrame::Instance()->GetMode() == eSERVER_TESTLIVE)
    {
        queryObj->m_spName = "S_ChangeUserStatus_Test";
    }
    else
    {
        queryObj->m_spName = "S_ChangeUserStatus";
    }
    ::_stprintf(tmp, _T("EXEC %s N'%s', %d, '%s', %d, N'%s', N'%s', '%s', %d, %d, '%s'"),
                queryObj->m_spName.c_str(), account, userGuid, glogKey, location, publicIp, localIp,
                StsTime, status, authKey, ConnectTime);

    eXtreme::RecordSet& recordSet = queryObj->m_recordSet;
    recordSet.RegisterParameter(NULL, "@UserID", adVarChar, adParamInput, (long)strlen(account), _bstr_t(account));
    recordSet.RegisterParameter(NULL, "@UserGuid", adInteger, adParamInput, (long)sizeof(DWORD), _bstr_t(userGuid));
    recordSet.RegisterParameter(NULL, "@AuthUserGuid", adVarChar, adParamInput, (long)strlen(glogKey), _bstr_t(glogKey));
    recordSet.RegisterParameter(NULL, "@ServerGuid", adInteger, adParamInput, (long)sizeof(DWORD), _bstr_t(location));
    recordSet.RegisterParameter(NULL, "@IpPublicAddr", adVarChar, adParamInput, (long)strlen(publicIp), _bstr_t(publicIp));
    recordSet.RegisterParameter(NULL, "@IpLocalAddr", adVarChar, adParamInput, (long)strlen(localIp), _bstr_t(localIp));
    recordSet.RegisterParameter(NULL, "@StsTime", adVarChar, adParamInput, (long)strlen(StsTime), _bstr_t(StsTime));
    recordSet.RegisterParameter(NULL, "@Sts", adTinyInt, adParamInput, (long)sizeof(BYTE), _bstr_t(status));
    recordSet.RegisterParameter(NULL, "@AuthKey", adInteger, adParamInput, (long)sizeof(DWORD), _bstr_t(authKey));
    recordSet.RegisterParameter(NULL, "@ConnectTime", adVarChar, adParamInput, (long)strlen(ConnectTime), _bstr_t(ConnectTime));

    queryObj->m_query = tmp;

    m_QueryQueue.PostCompletion(queryObj, 0);
}

//==================================================================================================
//!_AUTH_GLOBAL||!_AUTH_RUSSIA
void
QueryManager::QueryConnectLog(DWORD userGuid, LPTSTR account, DWORD serverGuid,
                              LPTSTR charName, short classType, WORD lv, LPTSTR ip, int pcBangGuid,
                              int usedTime, SYSTEMTIME& timeLogin, BYTE status)
{
    SYSTEMTIME timeLogout;
    TCHAR tmp[256] = { 0, };
    TCHAR loginTime[32], logoutTime[32];

    QueryAuthLog* queryObj = (QueryAuthLog*)AllocQuery(QUERYTYPE_AUTHLOG);

    //  Login/out 시간을 문자열로 변환한다.
    ::GetLocalTime(&timeLogout);

    //  인증서버 재시작으로 복구된 계정인 경우, LoginTime이 모두 0이다.
    if(timeLogin.wYear == 0)
    {
        timeLogin.wYear		= timeLogout.wYear;
        timeLogin.wMonth	= timeLogout.wMonth;
        timeLogin.wDay		= timeLogout.wDay;
        timeLogin.wHour		= 1;
        timeLogin.wMinute	= 1;
        timeLogin.wSecond	= 1;
    }

    ::_stprintf(loginTime, "%d-%d-%d %d:%d:%d", timeLogin.wYear, timeLogin.wMonth, timeLogin.wDay,
        timeLogin.wHour, timeLogin.wMinute, timeLogin.wSecond);

    if(status == 0)
    {
        //  Login에서는 Logout 시간을 설정하지 않는다.
        ::_stprintf(logoutTime, "%d-%d-%d 1:1:1", timeLogout.wYear, timeLogout.wMonth, timeLogout.wDay);
    }
    else
    {
        //  Logout 시간을 설정한다.
        ::_stprintf(logoutTime, "%d-%d-%d %d:%d:%d", timeLogout.wYear, timeLogout.wMonth, timeLogout.wDay,
            timeLogout.wHour, timeLogout.wMinute, timeLogout.wSecond);
    }

    queryObj->m_recordSet.RegisterParameter(&queryObj->m_dwLogSeq, "", adInteger, adParamReturnValue, sizeof(DWORD), "");

    CHECK_ID_LENGTH_BUFFER(queryObj->m_account);
    ::_tcsncpy(queryObj->m_account, account, _countof(queryObj->m_account));
    queryObj->m_account[_countof(queryObj->m_account) - 1] = '\0';

    queryObj->m_btLogout = status;

    if(!charName || !strlen(charName))
    {
        charName = " ";
    }

    eXtreme::RecordSet& recordSet = queryObj->m_recordSet;
    recordSet.RegisterParameter(NULL, "@UserGuid"   , adInteger,  adParamInput, (long)sizeof(DWORD)				, _bstr_t(userGuid));
    recordSet.RegisterParameter(NULL, "@UserID"		, adVarChar,  adParamInput, (long)strlen(account)			, _bstr_t(account));
    recordSet.RegisterParameter(NULL, "@ServerGuid"	, adInteger,  adParamInput, (long)sizeof(DWORD)				, _bstr_t(serverGuid));
    recordSet.RegisterParameter(NULL, "@CharName"	, adVarChar,  adParamInput, (long)strlen(charName)			, _bstr_t(charName));
    recordSet.RegisterParameter(NULL, "@ClassCode"	, adTinyInt,  adParamInput, (long)sizeof(BYTE)				, _bstr_t(static_cast<BYTE>(classType)));
    recordSet.RegisterParameter(NULL, "@LV"			, adSmallInt, adParamInput, (long)sizeof(WORD)				, _bstr_t(lv));
    recordSet.RegisterParameter(NULL, "@IpAddr"		, adVarChar,  adParamInput, (long)strlen(ip)				, _bstr_t(ip));
    recordSet.RegisterParameter(NULL, "@IpGuid"		, adInteger,  adParamInput, (long)sizeof(DWORD)				, _bstr_t(pcBangGuid));
    recordSet.RegisterParameter(NULL, "@UsedTime"   , adInteger,  adParamInput, (long)sizeof(DWORD)				, _bstr_t(usedTime));
    recordSet.RegisterParameter(NULL, "@LoginTime"	, adVarChar,  adParamInput, (long)strlen(loginTime)			, _bstr_t(loginTime));
    recordSet.RegisterParameter(NULL, "@LogoutTime" , adVarChar,  adParamInput, (long)strlen(logoutTime)		, _bstr_t(logoutTime));
    recordSet.RegisterParameter(NULL, "@ConnectSts" , adInteger,  adParamInput, (long)sizeof(DWORD)				, _bstr_t(static_cast<DWORD>(status)));

    ::_stprintf(tmp, _T("EXEC S_LoginInsert %d, '%s', %d, '%s', %d, %d, '%s', %d, %d, '%s', '%s', %d"),
        userGuid, account, serverGuid, charName, classType, lv, ip, pcBangGuid, usedTime,
        loginTime, logoutTime, status);

    queryObj->m_spName = "S_LoginInsert";
    queryObj->m_query = tmp;

    m_QueryQueue.PostCompletion(queryObj, 0);
}

//==================================================================================================
//!_AUTH_GLOBAL
void
QueryManager::QueryConnectLogUpdate(DWORD SeqNum, DWORD userGuid, char* CharName, SYSTEMTIME& timeLogin)
{
    char tmp[128] = { 0, };
    TCHAR loginTime[32] = { 0, };

    QueryAuthLogUpdate* queryObj = (QueryAuthLogUpdate*)AllocQuery(QUERYTYPE_AUTHLOG_UPDATE);

    if(timeLogin.wYear == 0)
    {
        SYSTEMTIME localTime;
        ::GetLocalTime(&localTime);

        timeLogin.wYear		= localTime.wYear;
        timeLogin.wMonth	= localTime.wMonth;
        timeLogin.wDay		= localTime.wDay;
        timeLogin.wHour		= 1;
        timeLogin.wMinute	= 1;
        timeLogin.wSecond	= 1;
    }

    ::_stprintf(loginTime, "%d-%d-%d %d:%d:%d", timeLogin.wYear, timeLogin.wMonth, timeLogin.wDay,	timeLogin.wHour, timeLogin.wMinute, timeLogin.wSecond);

    ::_stprintf(tmp, _T("EXEC S_LoginUpdate %d, %d, '%s', '%s'"), SeqNum, userGuid, CharName, loginTime);

    eXtreme::RecordSet& recordSet = queryObj->m_recordSet;
    recordSet.RegisterParameter(NULL, "@Seq", adInteger, adParamInput, (long)sizeof(DWORD), _bstr_t(SeqNum));
    recordSet.RegisterParameter(NULL, "@UserGuid", adInteger, adParamInput, (long)sizeof(DWORD), _bstr_t(userGuid));
    recordSet.RegisterParameter(NULL, "@CharName", adVarChar, adParamInput, (long)strlen(CharName), _bstr_t(CharName));
    recordSet.RegisterParameter(NULL, "@LoginTime", adVarChar, adParamInput, (long)strlen(loginTime), _bstr_t(loginTime));

    queryObj->m_spName = "S_LoginUpdate";
    queryObj->m_query = tmp;

    m_QueryQueue.PostCompletion(queryObj, 0);
}

//==================================================================================================

void
QueryManager::QueryRecoverSts()
{
    QueryRecoverLocation* queryObj = (QueryRecoverLocation*)AllocQuery(QUERYTYPE_RECOVERSTS);

    if(AuthFrame::Instance()->GetMode() == eSERVER_TESTLIVE)
    {
        queryObj->m_spName = "S_LoginUserSelect_Test";
        queryObj->m_query = _T("EXEC S_LoginUserSelect_Test");
    }
    else
    {
        queryObj->m_spName = "S_LoginUserSelect";
        queryObj->m_query = _T("EXEC S_LoginUserSelect");
    }

    m_QueryQueue.PostCompletion(queryObj, 0);
}

//==================================================================================================

void
QueryManager::QueryControlIpList()
{
    QueryControlIp* queryObj = (QueryControlIp*)AllocQuery(QUERYTYPE_CONTROLIP);

    queryObj->m_spName = "USP_GetIPAddressForAuthServer";
    queryObj->m_query = _T("EXEC USP_GetIPAddressForAuthServer");

    m_QueryQueue.PostCompletion(queryObj, 0);
}

//==================================================================================================

void
QueryManager::QueryGameVersion()
{
    QueryGameVersionUpdate* queryObj = (QueryGameVersionUpdate*)AllocQuery(QUERYTYPE_GAMEVERSION);

    queryObj->m_spName = "USP_GetVersionForAuthServer";
    queryObj->m_query = _T("EXEC USP_GetVersionForAuthServer");

    m_QueryQueue.PostCompletion(queryObj, 0);
}

//==================================================================================================

void
QueryManager::QueryUpdateAuthFailCount(LPTSTR account, LPTSTR clientIP, BYTE btFlag,
                                       DWORD front_user_id, DWORD login_front_id, BYTE btAuthType)
{
	char query_buffer[127] = { 0, };

	QueryAuthFailCount* queryObj = (QueryAuthFailCount*)AllocQuery(QUERYTYPE_INCRESEAUTHFAILCOUNT);

    CHECK_ID_LENGTH_BUFFER(queryObj->m_account);
    ::_tcsncpy(queryObj->m_account, account, _countof(queryObj->m_account));
    queryObj->m_account[_countof(queryObj->m_account) - 1] = '\0';

    ::_tcsncpy(queryObj->m_clientIP, clientIP, IP_MAX_LEN-1);

    queryObj->m_btFlag = btFlag;
    queryObj->m_front_user_id = front_user_id;
    queryObj->m_key = login_front_id;
    queryObj->m_btAuthType = btAuthType;

    eXtreme::RecordSet& recordSet = queryObj->m_recordSet;
    recordSet.RegisterParameter(&queryObj->m_btAuthFailCount, "", adTinyInt, adParamReturnValue, sizeof(DWORD), "");
    recordSet.RegisterParameter(NULL, "@UserID", adVarChar, adParamInput, (long)strlen(account), _bstr_t(account));
    recordSet.RegisterParameter(NULL, "@btFlag", adTinyInt, adParamInput, (long)sizeof(BYTE), _bstr_t(btFlag));

    // _AUTH_FAIL_LOG :  인증 오류시 실패계정정보 로그 생성
    recordSet.RegisterParameter(NULL, "@IpAddress", adVarChar, adParamInput, (long)strlen(clientIP), _bstr_t(clientIP));
    BYTE btSection = 0;		// 0 : 본섭, 1 : 테섭
    if(AuthFrame::Instance()->GetMode() == eSERVER_TESTLIVE)
        btSection = 1;
    recordSet.RegisterParameter(NULL, "@ServerType", adTinyInt, adParamInput, (long)sizeof(BYTE), _bstr_t(btSection));

#ifdef _AUTH_MATRIX_FAIL_COUNT
    recordSet.RegisterParameter(NULL, "@AuthType", adTinyInt, adParamInput, (long)sizeof(BYTE), _bstr_t(btAuthType));
#endif

    // (WAVERIX) (090417) Change _stprintf (no buffer size) -> _snprintf (with size)
#ifdef _AUTH_MATRIX_FAIL_COUNT
    _snprintf(query_buffer, _countof(query_buffer),
              "EXEC S_UpdateAuthFailCount '%s', %d, '%s', %d, %d",
              account, btFlag, clientIP, btSection, btAuthType);
#else
    _snprintf(query_buffer, _countof(query_buffer),
              "EXEC S_UpdateAuthFailCount '%s', %d, '%s', %d",
              account, btFlag, clientIP, btSection);
#endif

    queryObj->m_spName = "S_UpdateAuthFailCount";
    queryObj->m_query = query_buffer;

    m_QueryQueue.PostCompletion(queryObj, 0);
}

//==================================================================================================

#ifdef _NET_CAFE
void
QueryManager::QueryNetCafeLogInsert(DWORD key, DWORD dwFrontUserId, LPTSTR account,
                                    LPTSTR clientIp, 
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
                                    eNetCafeLoginType login_type,
#else
                                    bool bLogin,
#endif
                                    DWORD re_auth_key)
{
    char tmp[256] = {0, };

    // (WAVERIX) (090708) (CHANGES) x (BUG-FIX)
    eQUERYTYPE query_type = QUERYTYPE_NETCATFE_LOG;

#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
    if(login_type != NETCAFE_LOGOUT)
        query_type = QUERYTYPE_NETCATFE_LOGIN;
#else
    if(bLogin)
        query_type = QUERYTYPE_NETCATFE_LOGIN;
#endif

    QueryNetCafeLog* queryObj = (QueryNetCafeLog*)AllocQuery(query_type);
    queryObj->ClearField();
    queryObj->m_key = key;
    queryObj->m_dwFrontUserId = dwFrontUserId;
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
    queryObj->m_bLogin = login_type == NETCAFE_LOGOUT ? false : true;
    queryObj->SetNetCafeLoginType(login_type);
#else
    queryObj->m_bLogin = bLogin;
#endif
    
#ifdef _JP000000_ONETIMEPASSWORD_AUTHFLOW_
    queryObj->m_ReAuthKey = re_auth_key;
    int need_generate_otk = queryObj->m_ReAuthKey == 0;
#endif

    CHECK_ID_LENGTH_BUFFER(queryObj->m_account);
    ::_tcsncpy(queryObj->m_account, account, _countof(queryObj->m_account));
    queryObj->m_account[_countof(queryObj->m_account) - 1] = '\0';
    AUTH_DEBUGLOG("%s", queryObj->m_account);

#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
    if(login_type != NETCAFE_LOGOUT)	// 로그인이면 넷카페 ID를 받아온다
#else
    if(bLogin)	// 로그인이면 넷카페 ID를 받아온다
#endif    
    {
        //// (NOTE) 10/01/20현재 임시 조치이며, 0905버전 인증서버가 변경 적용 된 후 수정반영하기로 한다.
        //eSERVER_MODE server_mode = AuthFrame::Instance()->GetMode();
        //if (server_mode == eSERVER_DEV)
        //{
        //    queryObj->m_spName = "SP_SUN_USER_LOGIN_JAPAN";
        //    _snprintf(tmp, _countof(tmp), "EXEC SP_SUN_USER_LOGIN_JAPAN '%s', '%s'",
        //              clientIp, account);
        //}
        //else
        //{
    #ifdef _JP000000_ONETIMEPASSWORD_AUTHFLOW_
        //SP_SUN_USER_LOGIN_JAPAN
        queryObj->m_spName = "SP_SUN_USER_LOGIN";
        if(login_type == NETCAFE_ONTIME_LOGIN)
        {
            _snprintf(tmp, _countof(tmp), "EXEC SP_SUN_USER_LOGIN '%s', '%s', %d",
                clientIp, queryObj->m_account, need_generate_otk);        
        }
        else
        {
            _snprintf(tmp, _countof(tmp), "EXEC SP_SUN_USER_LOGIN '%s', '%s'",
                clientIp, queryObj->m_account);
        }        
    #else
        queryObj->m_spName = "SP_SUN_USER_LOGIN";
        _snprintf(tmp, _countof(tmp), "EXEC SP_SUN_USER_LOGIN '%s', '%s'",
                  clientIp, queryObj->m_account);
    #endif
        //}
//#endif
    }
    else
    {
        queryObj->m_spName = "SP_SUN_USER_LOGOUT";
        ::sprintf(tmp, "EXEC SP_SUN_USER_LOGOUT '%s', '%s'", clientIp, queryObj->m_account);
    }

    eXtreme::RecordSet& record_set = queryObj->m_recordSet;
    record_set.RegisterParameter(NULL, "@IP",
                                 adVarChar, adParamInput,
                                 (long)strlen(clientIp), _bstr_t(clientIp));
    record_set.RegisterParameter(NULL, "@ID",
                                 adVarChar, adParamInput,
                                 (long)strlen(queryObj->m_account), _bstr_t(queryObj->m_account));
#ifdef _JP000000_ONETIMEPASSWORD_AUTHFLOW_
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
    if(login_type != NETCAFE_LOGOUT)	// 로그인이면 넷카페 ID를 받아온다
#else
    if(bLogin)	// 로그인이면 넷카페 ID를 받아온다
#endif 
    {
        record_set.RegisterParameter(NULL, "@AUTH_REQ",
                                     adInteger, adParamInput,
                                     sizeof(need_generate_otk), _bstr_t(need_generate_otk));
    }
#endif

    queryObj->m_query = tmp;

    m_QueryQueue.PostCompletion(queryObj, 0);
}

//_JP000000_ONETIMEPASSWORD_AUTHFLOW_
void
QueryManager::QueryNetCafe2ndAuthResult(DWORD key, DWORD front_user_id, LPTSTR account,
                                        const nsPacket::OneTimePassword& one_time_password)
{
#ifdef _JP000000_ONETIMEPASSWORD_AUTHFLOW_
    char buffer[128] = { 0, };

    // (WAVERIX) (090708) (CHANGES) x (BUG-FIX)
    const eQUERYTYPE query_type = QUERYTYPE_NETCAFE_OTP_CHECK;

    QueryNetCafe2ndAuth* const query_object = \
        static_cast<QueryNetCafe2ndAuth*>(AllocQuery(query_type));
    query_object->ClearField();
    query_object->m_key = key;
    query_object->front_user_id_ = front_user_id;

    CHECK_ID_LENGTH_BUFFER(query_object->query_info_.account);
    ::_tcsncpy(query_object->query_info_.account, account,
               _countof(query_object->query_info_.account));
    query_object->query_info_.account[_countof(query_object->query_info_.account) - 1] = '\0';

    query_object->m_spName = "SP_GET_2ND_AUTHRESULT";
    _snprintf(buffer, _countof(buffer), 
              "EXEC SP_GET_2ND_AUTHRESULT '%s', '%s'",
              query_object->query_info_.account, one_time_password.value);

    eXtreme::RecordSet& record_set = query_object->m_recordSet;
    record_set.RegisterParameter(NULL, "@ACCOUNT",
                                 adVarChar, adParamInput,
                                 static_cast<long>(strlen(query_object->query_info_.account)),
                                 _bstr_t(query_object->query_info_.account));
    record_set.RegisterParameter(NULL, "@PASSWORD",
                                 adVarChar, adParamInput,
                                 static_cast<long>(strlen(one_time_password.value)),
                                 _bstr_t(one_time_password.value));

    query_object->m_query = buffer;
    m_QueryQueue.PostCompletion(query_object, 0);
#endif
}

#endif //_NET_CAFE


//
//
//==================================================================================================
#ifdef __NA000000_SHOP_USERID_50_PASSING_PROC__
void
QueryManager::Query_UserId(AGENTID game_agent_id, const QueryUserIdInfo& query_info)
{
    char tmp[128] = { 0, };

    QueryUserId* queryObj = (QueryUserId*)AllocQuery(QUERYTYPE_USERID);

    queryObj->AlignQueryUserIdInfo(&query_info);
    queryObj->SetGameAgentID(game_agent_id);
    ::_stprintf(tmp, _T("EXEC USP_SelectUserID %d"), query_info.user_guid_);

    eXtreme::RecordSet& recordSet = queryObj->m_recordSet;
    recordSet.RegisterParameter(NULL, "@UserGuid", adInteger, adParamInput, (long)sizeof(DWORD), _bstr_t(query_info.user_guid_));

    queryObj->m_spName = "USP_SelectUserID";
    queryObj->m_query = tmp;

    m_QueryQueue.PostCompletion(queryObj, 0);
}
#endif

#ifdef __NA000000_GLOBAL_AUTH_PROCESS__
// gb.USP_WS_CheckAuthKey
//void QueryLoginUserByAuthTokenGlobal
// gb.USP_INSERT_CONNECTLOG
void
QueryManager::Query_GspUpdateConnectLog(DWORD user_guid,
                                        const char* game_id, //assert(null-terminated)
                                        //GameCode,
                                        AGENTID server_guid,
                                        int play_time,
                                        BOOLEAN is_login, // true(login), false(logout)
                                        const char* char_name, //assert(null-terminated)
                                        char char_class,
                                        short char_level,
                                        const char* log_ip_address)
{
    using namespace nsGSP;
    char temp[256];
    QueryGspLoggingUserState* queryObj =
        static_cast<QueryGspLoggingUserState*>(AllocQuery(QUERYTYPE_GSP_CONNECT_LOG));

    queryObj->Clear();

    const char game_code = nsGSP::eGameCode_SUN;
    const char login_type = is_login == 0;

    _snprintf(temp, _countof(temp),
              "EXEC gb.USP_INSERT_CONNECTLOG %d, '%s', %d, %d, %d, %d, '%s', %d, %d, '%s'",
              user_guid, game_id, game_code, server_guid, play_time, login_type,
              char_name, char_class, char_level, log_ip_address);

    eXtreme::RecordSet& recordSet = queryObj->m_recordSet;
    recordSet.RegisterParameter(0, "@AccountGuid", adInteger, adParamInput,
                                sizeof(user_guid), _bstr_t(user_guid));
    recordSet.RegisterParameter(0, "@GameID", adVarChar, adParamInput,
                                (long)strlen(game_id), _bstr_t(game_id));
    recordSet.RegisterParameter(0, "@GameCode", adTinyInt, adParamInput,
                                sizeof(game_code), _bstr_t(game_code));
    recordSet.RegisterParameter(0, "@ServerGuid", adInteger, adParamInput,
                                sizeof(server_guid), _bstr_t(server_guid));
    recordSet.RegisterParameter(0, "@PlayTime", adInteger, adParamInput,
                                sizeof(play_time), _bstr_t(play_time));
    recordSet.RegisterParameter(0, "@LogType", adTinyInt, adParamInput,
                                sizeof(login_type), _bstr_t(login_type));
    recordSet.RegisterParameter(0, "@CharacterName", adVarChar, adParamInput,
                                (long)strlen(char_name), _bstr_t(char_name));
    recordSet.RegisterParameter(0, "@Class", adTinyInt, adParamInput,
                                sizeof(char_class), _bstr_t(char_class));
    recordSet.RegisterParameter(0, "@Lv", adSmallInt, adParamInput,
                                sizeof(char_level), _bstr_t(char_level));
    recordSet.RegisterParameter(0, "@LogIP", adVarChar, adParamInput,
                                (long)strlen(log_ip_address), _bstr_t(log_ip_address));

    // output
    recordSet.RegisterParameter(&queryObj->result_data_.result, "@Result",
                                adTinyInt, adParamOutput,
                                sizeof(queryObj->result_data_.result), _bstr_t());

    queryObj->m_spName = "gb.USP_INSERT_CONNECTLOG";
    queryObj->m_query = temp;

    m_QueryQueue.PostCompletion(queryObj, 0);
}

// gb.USP_INSERT_ACCOUNTSTATE
void
QueryManager::Query_GspUpdateUserState(DWORD user_guid,
                                       //GameCode,
                                       AGENTID server_guid,
                                       const char* connect_ip,//assert(null-terminated)
                                       BOOLEAN is_connect)   // true(login), false(logout)
{
    using namespace nsGSP;
    char temp[256];
    QueryGspChangeUserState* queryObj =
        static_cast<QueryGspChangeUserState*>(AllocQuery(QUERYTYPE_GSP_ACCOUNT_STATE));

    queryObj->Clear();

    const char game_code = nsGSP::eGameCode_SUN;
    const char connect_state = is_connect == 0;

    _snprintf(temp, _countof(temp),
              "EXEC gb.USP_INSERT_ACCOUNTSTATE %d, %d, %d, '%s', %d",
              user_guid, game_code, server_guid, connect_ip, connect_state);

    eXtreme::RecordSet& recordSet = queryObj->m_recordSet;
    recordSet.RegisterParameter(0, "@AccountGuid", adInteger, adParamInput,
                                sizeof(user_guid), _bstr_t(user_guid));
    recordSet.RegisterParameter(0, "@GameCode", adTinyInt, adParamInput,
                                sizeof(game_code), _bstr_t(game_code));
    recordSet.RegisterParameter(0, "@ServerGuid", adInteger, adParamInput,
                                sizeof(server_guid), _bstr_t(server_guid));
    recordSet.RegisterParameter(0, "@ConnectIP", adVarChar, adParamInput,
                                (long)strlen(connect_ip), _bstr_t(connect_ip));
    recordSet.RegisterParameter(0, "@ConnectState", adTinyInt, adParamInput,
                                sizeof(connect_state), _bstr_t(connect_state));

    // output
    recordSet.RegisterParameter(&queryObj->result_data_.result, "@Result",
                                adTinyInt, adParamOutput,
                                sizeof(queryObj->result_data_.result), _bstr_t());

    queryObj->m_spName = "gb.USP_INSERT_ACCOUNTSTATE";
    queryObj->m_query = temp;

    m_QueryQueue.PostCompletion(queryObj, 0);
}


// gb.USP_WS_GetAccountID
void
QueryManager::Query_GspGetAccountID(AGENTID game_agent_id, const QueryUserIdInfo& query_info)
{
    const char game_code = nsGSP::eGameCode_SUN;
    using namespace nsGSP;
    char query_string[256];
    QueryGspGetAccountByGuid* queryObj =
        static_cast<QueryGspGetAccountByGuid*>(AllocQuery(QUERYTYPE_GSP_ACCOUNT_ID_BY_GUID));

    queryObj->PrepareQueryUserIdInfo(game_agent_id, &query_info);

    _snprintf(query_string, _countof(query_string),
              "EXEC gb.USP_WS_GetAccountIDbyAccountGuid %d, %d",
              query_info.user_guid_, game_code);

    GLOBAL_DEBUGLOG(__FUNCTION__" query_string - {%s}\n", query_string); // DEBUG_STRING

    eXtreme::RecordSet& recordSet = queryObj->m_recordSet;
    recordSet.RegisterParameter(0, "@AccountGuid", adInteger, adParamInput,
                                sizeof(query_info.user_guid_), _bstr_t(query_info.user_guid_));
    recordSet.RegisterParameter(0, "@GameCode", adTinyInt, adParamInput,
                                sizeof(game_code), _bstr_t(game_code));

    // output
    recordSet.RegisterParameter(queryObj->result_data_.account_id, "@AccountID",
                                adVarChar, adParamOutput,
                                sizeof(queryObj->result_data_.account_id), _bstr_t());

    queryObj->m_spName = "gb.USP_WS_GetAccountIDbyAccountGuid";
    queryObj->m_query = query_string;

    m_QueryQueue.PostCompletion(queryObj, 0);
}

// gb.USP_WS_GetAccountID
void
QueryManager::Query_GspGetAccountID_ForAuth(AccountInfo* account) // passing block
{
#if !defined(__NA000000_SHOP_USERID_50_PASSING_PROC__)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

    const char game_code = nsGSP::eGameCode_SUN;
    using namespace nsGSP;
    char temp[256];
    QueryGspGetAccount* queryObj =
        static_cast<QueryGspGetAccount*>(AllocQuery(QUERYTYPE_GSP_ACCOUNT_ID));

    ASSERT(account);
    queryObj->PrepareForAuthStep(account);

    _snprintf(temp, _countof(temp),
              "EXEC gb.USP_WS_GetAccountID '%s', %d",
              account->m_GameID, game_code);

    eXtreme::RecordSet& recordSet = queryObj->m_recordSet;
    recordSet.RegisterParameter(0, "@GameID", adVarChar, adParamInput,
                                (long)strlen(account->m_GameID), _bstr_t(account->m_GameID));
    recordSet.RegisterParameter(0, "@GameCode", adTinyInt, adParamInput,
                                sizeof(game_code), _bstr_t(game_code));

    // output
    recordSet.RegisterParameter(queryObj->result_data_.account_id, "@AccountID",
                                adVarChar, adParamOutput,
                                sizeof(queryObj->result_data_.account_id), _bstr_t());

    queryObj->m_spName = "gb.USP_WS_GetAccountID";
    queryObj->m_query = temp;

    m_QueryQueue.PostCompletion(queryObj, 0);
}

#endif //__NA000000_GLOBAL_AUTH_PROCESS__

#ifdef __CN_20100909_SECURITY_TOKEN__
void
QueryManager::Query_TokenKeyUpdate(DWORD user_guid, char token_base64_key_[TOKEN_BASE64_KEY_MAX + 1])
{
	QueryUpdateTokenKey* query_obj = 
		static_cast<QueryUpdateTokenKey*>(AllocQuery(QUERYTYPE_TOKEN_UPDATE));	

	char temp[TOKEN_BASE64_KEY_MAX + 256];

	_snprintf(temp, _countof(temp),
		"EXEC S_Set_Token_Key %d, '%s'", user_guid, token_base64_key_);
    temp[_countof(temp) - 1] = '\0';

	eXtreme::RecordSet& recordSet = query_obj->m_recordSet;
	recordSet.RegisterParameter(NULL, "@UserGuid", adInteger, adParamInput, 
								(long)sizeof(DWORD), _bstr_t(user_guid));
	recordSet.RegisterParameter(NULL, "@Token_key", adVarChar, adParamInput,
                                (long)strlen(token_base64_key_), _bstr_t(token_base64_key_));

	query_obj->m_spName = "S_Set_Token_Key";
	query_obj->m_query = temp;

	m_QueryQueue.PostCompletion(query_obj, 0);
}
#endif

void 
QueryManager::QueryPCRoomIPList(const long pc_room_guid)
{   
    QuerySelectPCRoomIPList* query_obj = 
        static_cast<QuerySelectPCRoomIPList*>(AllocQuery(QUERYTYPE_PCROOM_IPLIST));	

    query_obj->SetPCRoomIP(pc_room_guid);

    char query_string[256];

    _snprintf(query_string, _countof(query_string), "EXEC S_Select_PcIp %d", pc_room_guid);

    eXtreme::RecordSet& recordSet = query_obj->m_recordSet;
    recordSet.RegisterParameter(NULL, "@CompGuid", adInteger, adParamInput, 
        (long)sizeof(DWORD), _bstr_t(pc_room_guid));
   
    query_obj->m_spName = "S_Select_PcIp";
    query_obj->m_query = query_string;

    m_QueryQueue.PostCompletion(query_obj, 0);
}

















