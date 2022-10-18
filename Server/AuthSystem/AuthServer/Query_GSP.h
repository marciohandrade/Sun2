#ifndef _AUTH_SERVER_QUERY_GSP_H
#define _AUTH_SERVER_QUERY_GSP_H

#include "Query_Base.h"

struct AccountInfo;
class LoginFrontServerSession;
//==================================================================================================
// (WAVERIX) (090827) (ADD-IN) by __NA000000_GLOBAL_AUTH_PROCESS__

namespace nsGSP {
;

enum eGameCode
{
    eGameCode_MU        = 1,
    eGameCode_SUN       = 2,
    eGameCode_ArchLoad  = 8,
    eGameCode_R2        = 9,
};

//--------------------------------------------------------------------------------------------------
// no null-terminated string length
// Global DB is 1byte char based.
const int Length_of_GameID          = 10;
const int Length_of_AccountID       = 20;
const int Length_of_ChararacterName = 20;
const int Length_of_IpAddress       = 15;
const int Length_of_MacAddress      = 17;
const int Length_of_AuthKey         = 36;

typedef char Buffer_of_GameID[Length_of_GameID + 1];
typedef char Buffer_of_AccountID[Length_of_AccountID + 1];
typedef char Buffer_of_CharName[Length_of_ChararacterName + 1];
typedef char Buffer_of_IpAddress[Length_of_IpAddress + 1];
typedef char Buffer_of_MacAddress[Length_of_MacAddress + 1];
typedef char Buffer_of_AuthKey[Length_of_AuthKey + 1];

typedef wchar_t WBuffer_of_GameID[Length_of_GameID + 1];
typedef wchar_t WBuffer_of_AccountID[Length_of_AccountID + 1];
typedef wchar_t WBuffer_of_CharName[Length_of_ChararacterName + 1];
typedef wchar_t WBuffer_of_IpAddress[Length_of_IpAddress + 1];
typedef wchar_t WBuffer_of_MacAddress[Length_of_MacAddress + 1];
typedef wchar_t WBuffer_of_AuthKey[Length_of_AuthKey + 1];

//==================================================================================================

class QueryGspCheckAuthKey;
class QueryGspChangeUserState;
class QueryGspLoggingUserState;
class QueryGspGetAccount;

//==================================================================================================
//
class QueryGspCheckAuthKey : public QueryObject
{
public:
    QueryGspCheckAuthKey();
    virtual ~QueryGspCheckAuthKey();

    // Input Parameters
    //  @GameID,
    //  @GameCode,
    //  @AuthKey,
    //  @IP,
    //  @MacAddress
    // Output Parameters
    //  @AccountID : int,
    //  @AccountType : tinyint, (eAccountType)
    //  @BlockState : tinyint,  (eBlockState)
    //  @Result : tinyint       (eResult)

    // @AccountType - 계정 종류
    enum eAccountType {
        eAccountType_Normal = 1,
        eAccountType_GM     = 2,
        eAccountType_Guest  = 3,
    };
    //  @BlockState - 블럭 상태
    enum eBlockState {
        eBlockState_Normal  = 0,
        eBlockState_Block   = 1,
    };
    //  @Result - 인증키 체크 성공 여부
    enum eResult {
        eResult_Fail    = 0,
        eResult_Success = 1,
    };

    void onResult();
    virtual DB_TYPE GetDBType() { return DB_TYPE_AUTH; }

    void Clear(eAuthToken token_type);
    // for support 'Server Select'
    void ReauthByServerSelect(nsPacket::ReAuthInfoBase::ReAuthKeyType reauth_key);
    // (NOTE) 사실 상의 땜빵 코드... ㅠㅠ 일단 이 로직으로 서버 선택 문제를 해결하고 나중에 개선할 것.
    BOOLEAN onResult_ByServerSelect(LoginFrontServerSession* reply_session);

//private:
    //struct PassingBlock {
    //    DWORD front_key_;
    //    DWORD auth_user_key_;
    //    eAuthToken token_type_;
    //    nsPacket::ReAuthInfoBase::ReAuthKeyType reauth_key_; //__NA001334_090420_SERVER_SELECT_FACILITY__
    //    Buffer_of_IpAddress local_ip_;
    //} passing_data_;
    eAuthToken token_type_;
    AccountInfo* account_; // local alloc/free container
    nsPacket::ReAuthInfoBase::ReAuthKeyType reauth_key_;

    struct InputArgs {
        Buffer_of_GameID        game_id;
        char                    game_code;
        Buffer_of_AuthKey       auth_key;
        Buffer_of_IpAddress     client_ip;
        Buffer_of_MacAddress    mac_address;
    } input_args_;

    struct ResultData {
        int     account_guid;
        short   account_type;
        short   block_state;
        eResult result;
#ifdef _NA_20100307_BETAKEY_SYSTEM
        wchar_t betakey[MAX_BETA_KEY+1];
#endif
    } result_data_;
};

//==================================================================================================
//
class QueryGspChangeUserState : public QueryObject
{
public:
    QueryGspChangeUserState();
    virtual ~QueryGspChangeUserState();

    // Input Parameters
    //  @AccountGuid,
    //  @GameCode,
    //  @ServerGuid,
    //  @ConnectIP
    //  @ConnectState       (eConnectState)
    // Output Parameters
    //  @Result : tinyint

    //  @ConnectState
    enum eConnectState {
        eConnectState_Login     = 0,
        eConnectState_Logout    = 1,
    };

    //  @Result
    enum eResult {
        eResult_Fail    = 0,
        eResult_Success = 1,
    };

    void onResult();
    virtual DB_TYPE GetDBType() { return DB_TYPE_AUTH; }
    void Clear();

//private:

    struct {
        int                 account_guid;
        char                game_code;
        AGENTID             server_guid;
        Buffer_of_IpAddress connect_ip;
        eConnectState       connect_state;
    } input_args_;

    struct {
        eResult result;
    } result_data_;
};

//==================================================================================================
//
class QueryGspLoggingUserState : public QueryObject
{
public:
    QueryGspLoggingUserState();
    virtual ~QueryGspLoggingUserState();

    // Input Parameters
    //  @AccountGuid,
    //  @GameID,
    //  @GameCode,
    //  @ServerGuid,
    //  @PlayTime,  (초 단위, 입력 불가시 0)
    //  @LogType,   (eLogType)
    //  @CharacterName,
    //  @Class,
    //  @Lv,
    //  @LogIP
    // Output Parameters
    //  @Result : tinyint (eResult)

    //  @LogType,
    enum eLogType {
        eLogType_Login  = 0,
        eLogType_Logout = 1,
    };

    //  @Result
    enum eResult {
        eResult_Fail    = 0,
        eResult_Success = 1,
    };

    void onResult();
    virtual DB_TYPE GetDBType() { return DB_TYPE_LOG; }
    void Clear();

//private:
    struct {
        int                 account_guid;
        Buffer_of_GameID    game_id;
        char                game_code;
        AGENTID             server_guid;
        int                 play_time;
        eLogType            log_type;
        Buffer_of_CharName  char_name;
        char                char_class;
        short               char_level;
        Buffer_of_IpAddress ip_address; // log ip?
    } input_args_;

    struct {
        eResult result;
    } result_data_;
};

//==================================================================================================
//
class QueryGspGetAccount : public QueryObject
{
public:
    QueryGspGetAccount();
    virtual ~QueryGspGetAccount();

    // Input Parameters
    //  @GameID,
    //  @GameCode,
    // Output Parameters
    //  @AccountID

    void onResult();
    virtual DB_TYPE GetDBType() { return DB_TYPE_AUTH; }
    void PrepareForAuthStep(AccountInfo* account);
    void PrepareQueryUserIdInfo(AGENTID agent_id, const QueryUserIdInfo* query_info);
    const QueryUserIdInfo* GetQueryUserIdInfo() const { return passing_data_.query_info_; };

private:
    // (NOTE) 성공시 account_의 정보를 new_account에 할당한다.
    AccountInfo* CheckOverlapLogin(BYTE& byResult);

    BOOLEAN auth_process_;  //인증을 위해 쿼리할 경우 true, 이 경우라면 account리소스 해제한다.
    AccountInfo* account_;
public:
    struct {
        QueryUserIdInfo* query_info_;
        AGENTID agent_id_;
    } passing_data_;
    struct {
        Buffer_of_GameID    game_id;
        char                game_code;
    } input_args_;

#pragma pack(push, 1) // for db result buffer overrun detection
    struct {
        WBuffer_of_AccountID account_id;
        DWORD overrun_detector;
    } result_data_;
#pragma pack(pop)
};

//==================================================================================================
//
class QueryGspGetAccountByGuid : public QueryObject
{
public:
    QueryGspGetAccountByGuid();
    virtual ~QueryGspGetAccountByGuid();

    // Input Parameters
    //  @GameID,
    //  @GameCode,
    // Output Parameters
    //  @AccountID

    void onResult();
    virtual DB_TYPE GetDBType() { return DB_TYPE_AUTH; }
    void PrepareQueryUserIdInfo(AGENTID agent_id, const QueryUserIdInfo* query_info);
    const QueryUserIdInfo* GetQueryUserIdInfo() const { return passing_data_.query_info_; };

private:
    // (NOTE) 성공시 account_의 정보를 new_account에 할당한다.
    AccountInfo* CheckOverlapLogin(BYTE& byResult);

    BOOLEAN auth_process_;  //인증을 위해 쿼리할 경우 true, 이 경우라면 account리소스 해제한다.
    AccountInfo* account_;
public:
    struct {
        QueryUserIdInfo* query_info_;
        AGENTID agent_id_;
    } passing_data_;
    struct {
        Buffer_of_GameID    game_id;
        char                game_code;
    } input_args_;

#pragma pack(push, 1) // for db result buffer overrun detection
    struct {
        WBuffer_of_AccountID account_id;
        DWORD overrun_detector;
    } result_data_;
#pragma pack(pop)
};

//==================================================================================================

}; //end of namespace 'nsGSP'

#endif //_AUTH_SERVER_QUERY_GSP_H

