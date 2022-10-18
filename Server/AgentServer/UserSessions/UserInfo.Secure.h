#pragma once
#ifndef __USERINFO_SECURE_H__
#define __USERINFO_SECURE_H__


#include <PacketStruct_CG.h>
#include <CryptManager.h>
#include <ITimeout.h>
//

//#include "AgentServer.h"
#include "TimeCheck/HeartbeatChecker.h"
#include "TimeCheck/heartbeatManager.h"
#include "TimeCheck/TickSpeedChecker.h" //_NA_20100802_SPEED_HACK_DETECTION
#include "./UserInfo.SecureXignCode.h" // USING_XIGNCODE
#include "GameGuard/ggsrv30.h"  // USING_GAMEGUARD

class User;
class UserManager;

//==================================================================================================
// <TYPE DEFINITIONs><sSECURE_CONTROL_STATUS REVISION>
// <sSECURE_FLAG>
//==================================================================================================
// <TYPE DEFINITIONs><sSECURE_CONTROL_STATUS REVISION>
// <sSECURE_FLAG>
class sSECURE_FLAG
{
public:
    struct ControlInfo
    {
        DWORD flag; // Control Field
        DWORD trans_tick; // Transaction Start Tick
    };

    // 현재 유저 상태에 따른 처리 구분용
    enum eSTATUS
    {
        STAT_NOCONTROL      = 0x00000000,
        STAT_USERINIT       = 0x00000002, // 막 키교환이 완료된 상황
        STAT_CRYPTED        = 0x00000004, // 쿼리 대상
        STAT_CRYPTING       = 0x00000008, // 쿼리에 대한 응답 대기중
        STAT_MASK_STATUS    = 0x0000007F, // STAT_TRANSACTION는 제외한 마스크
        STAT_TRANSACTION    = 0x00000080, // 존 간 이동과 같은 클라이언트의 응답이 적절하게 이뤄지지 못할 경우를 감안한 것
    };

    // 현재 수행중인 보안 쿼리 형태
    // 값이 존재하는 것 자체가 트랜잭션
    enum eQUERY_TRANS
    {
        TRANS_NOTRANS           = (0x00000000),
        TRANS_DEFAULT_TRANS     = (0x00008000), // [1]최상단 비트는 예약된 것임

#define SHL_VALUE(val) (((val) << 8) | TRANS_DEFAULT_TRANS)
        TRANS_DUPLEX_SNK        = SHL_VALUE(0x10), // In MOLA, Duplex Query
        TRANS_DUPLEX_SNK_T1     = SHL_VALUE(0x11), // In Client, Cooperating
        TRANS_SIMPLEX_SNK       = SHL_VALUE(0x20), // In MOLA, Simplex Query
        TRANS_SIMPLEX_SNK_T1    = SHL_VALUE(0x21), // In MOLA, Simplex Query
        TRANS_SIMPLEX_SNK_T2    = SHL_VALUE(0x22), // In MOLA, Simplex Query
        TRANS_SIMPLEX_SNK_T3    = SHL_VALUE(0x23), // In MOLA, Simplex Query
        TRANS_SIMPLEX_SNK_T4    = SHL_VALUE(0x24), // In MOLA, Simplex Query
        TRANS_MOLA_QUERY        = SHL_VALUE(0x31), // In MOLA, 
        TRANS_MOLA_CONFIG       = SHL_VALUE(0x32), // In MOLA, 
        TRANS_MOLA_HEARTBEAT    = SHL_VALUE(0x33), // In MOLA, 
        TRANS_GRES_CHECK        = SHL_VALUE(0x40), // In MOLA,
        TRANS_ARMORKIT          = SHL_VALUE(0x51), // In MOLA,
        //__NA000000_070913_HACKSHIELD_INSTALLATION__
        TRANS_HACKSHIELD        = SHL_VALUE(0x52), 
        TRANS_GAMEGUARD         = SHL_VALUE(0x53),
        // 쿼리 트랜잭션중 이 트랜잭션이 걸리면 어떠한 작업도 수행하지 않는다.
        // CryptingList에 존재해야 한다
        TRANS_FORCEDCLOSING     = SHL_VALUE(0x60),
        TRANS_MASK_QUERY        = (0x0000FF00),
#undef SHL_VALUE

        // 예외 사항, 이 BUILTIN_QUERY는 특정 시점에 반드시 발동하도록, 그리고 정상일 경우
        // 반드시 받게 되어 있는 쿼리를 지칭한다. 일반적인 쿼리 트랜잭션과는 별도로 동작한다.
        TRANS_BUILTIN_QUERY     = (0x01000000), // ASSERT! 이것을 작동하는 시점엔 다른 쿼리들은 미수행중
    };

    // 현재 수행중인 보안 쿼리를 실패했을 경우 처리할 에러타입
    // 예를 들어, 조건에 만족하지 않았거나 타임아웃이 걸렸을 경우
    // 보안 쿼리 트랜잭션은 TRANS_FORCEDCLOSING 상태로 전환되더라도 그 강제종료되는 사유를 나중에 로깅하기 위함
    enum eQUERY_ERRCTRL
    {
        ERR_NOERR           = (0x00000000),
        ERR_DEFAULT_TRANS   = (0x00800000), // [1]최상단 비트는 예약된 것임

#define SHL_VALUE(val) (((val) << 16) | ERR_DEFAULT_TRANS)
        ERR_DUPLEX_SNK          = SHL_VALUE(0x10), // In MOLA, Duplex Query
        ERR_DUPLEX_SNK_T1       = SHL_VALUE(0x11), // In Client, Cooperating
        ERR_SIMPLEX_SNK         = SHL_VALUE(0x20), // In MOLA, Simplex Query
        ERR_SIMPLEX_SNK_T1      = SHL_VALUE(0x21), // In MOLA, Simplex Query
        ERR_SIMPLEX_SNK_T2      = SHL_VALUE(0x22), // In MOLA, Simplex Query
        ERR_SIMPLEX_SNK_T3      = SHL_VALUE(0x23), // In MOLA, Simplex Query
        ERR_SIMPLEX_SNK_T4      = SHL_VALUE(0x24), // In MOLA, Simplex Query
        ERR_MOLA_QUERY          = SHL_VALUE(0x31), // In MOLA, 
        ERR_MOLA_CONFIG         = SHL_VALUE(0x32), // In MOLA, 
        ERR_MOLA_HEARTBEAT      = SHL_VALUE(0x33), // In MOLA, 
        ERR_GRES_CHECK          = SHL_VALUE(0x40), // In MOLA,
        ERR_ARMORKIT            = SHL_VALUE(0x51), // In MOLA,
        //__NA000000_070913_HACKSHIELD_INSTALLATION__
        ERR_HACKSHIELD          = SHL_VALUE(0x52), 
        ERR_HACKSHIELD_DETECT   = SHL_VALUE(0x53),
        ERR_GAMEGUARD_QUERY     = SHL_VALUE(0x54),
        // 상위 트랜잭션 에러는 쿼리 트랜잭션이 발생했을 때 기본으로 설정하는 값들
        // 만약 위의 상태라도 특정 조건을 만족하게 되면 하단의 트랜잭션 에러와 같이 분기될 수 있다.
        ERR_FORCEDCLOSING       = SHL_VALUE(0x60),
        ERR_CLOSE_BY_MASTER     = SHL_VALUE(0x61),
        ERR_CORRUPTED_PACKET    = SHL_VALUE(0x62), // In MOLA,
        ERR_VIOLATION_FLOW_SEQ  = SHL_VALUE(0x63), // In MOLA,
        ERR_INITIAL_KEY_EXCHG   = SHL_VALUE(0x64),
        ERR_BUILTIN_QUERY       = SHL_VALUE(0x65),
        ERR_HACKSHIELD_VERSION  = SHL_VALUE(0x66), //__NA001083_080609_ABUSING_LOGICAL_DETECTION_ANAL_AND_IMPL_V2__
        ERR_MASK_QUERY          = (0x00FF0000),
#undef SHL_VALUE
    };

    //----------------------------------------------------------------------------------------------
    // <FIELDs>
public:
    sSECURE_FLAG();
    DWORD& FLAG();
    DWORD& TRANS_TICK();
    // 질의를 할 수 있는가?
    bool IsEnableQuerying() const;
    void ClearFields();
    //----------------------------------------------------------------------------------------------
private:
    ControlInfo control_info_;

public:
    struct sSTATUS
    {
        DWORD& FLAG();
        DWORD& TRANS_TICK();

        sSTATUS(ControlInfo& control_info);

        void SetStatus(eSTATUS status_value);
        eSTATUS GetStatus() const;
        void TransEnter();
        void TransLeave();
        // 존 상태에 따른 상태 제어 { CHAR_SELECT | (상태가 존 간 이동상태일 경우) }
        bool IsTransaction() const;
        DWORD GetTransTime() const;
        //
        ControlInfo& control_info_;
    } STATUS;

    struct sQUERY
    {
        DWORD& FLAG();
        sQUERY(ControlInfo& control_info);
        void TransEnter(eQUERY_TRANS trans_value);
        eQUERY_TRANS TransType() const;
        void TransLeave();
        // 질의중인가?
        bool IsTransaction() const;
        //
        ControlInfo& control_info_;
    } QUERY;

    struct sQUERYBUILTIN
    {
        DWORD& FLAG();
        sQUERYBUILTIN(ControlInfo& control_info);
        void TransEnter();
        void TransLeave();
        // 질의중인가?
        bool IsTransaction() const;
        //
        ControlInfo& control_info_;
    } QUERYBUILTIN;
};

//==================================================================================================

inline sSECURE_FLAG::sSECURE_FLAG()
    : STATUS(control_info_)
    , QUERY(control_info_)
    , QUERYBUILTIN(control_info_)
{
    ClearFields();
}

inline DWORD& sSECURE_FLAG::FLAG() {
    return control_info_.flag;
}

inline DWORD& sSECURE_FLAG::TRANS_TICK() {
    return control_info_.trans_tick;
}

// 질의를 할 수 있는가?
inline bool sSECURE_FLAG::IsEnableQuerying() const
{
    bool querying = ((STAT_CRYPTED & control_info_.flag) && (!STATUS.IsTransaction()));
    return querying;
}
//// 현재 CS보안 위반 발생했을 경우 그 에러 제어
//static bool IsFault(DWORD& eREF) { return (bool)!!(ERR_FAULT&eREF); }

inline void sSECURE_FLAG::ClearFields()
{
    control_info_.flag = 0;
    control_info_.trans_tick = 0;
    // ITimeout 기본 무한
}

//==================================================================================================

inline DWORD& sSECURE_FLAG::sSTATUS::FLAG() {
    return control_info_.flag;
}

inline DWORD& sSECURE_FLAG::sSTATUS::TRANS_TICK() {
    return control_info_.trans_tick;
}

inline sSECURE_FLAG::sSTATUS::sSTATUS(ControlInfo& control_info)
    : control_info_(control_info)
{
}

inline void sSECURE_FLAG::sSTATUS::SetStatus(eSTATUS status_value)
{
    control_info_.flag = (eSTATUS)((control_info_.flag & (~STAT_MASK_STATUS)) |
                                   (STAT_MASK_STATUS & status_value));
}

inline sSECURE_FLAG::eSTATUS sSECURE_FLAG::sSTATUS::GetStatus() const
{
    return (eSTATUS)(STAT_MASK_STATUS & control_info_.flag);
}

inline void sSECURE_FLAG::sSTATUS::TransEnter()
{
    control_info_.flag |= STAT_TRANSACTION;
    control_info_.trans_tick = GetTickCount();
}

inline void sSECURE_FLAG::sSTATUS::TransLeave()
{
    control_info_.flag = control_info_.flag & (~STAT_TRANSACTION);
    control_info_.trans_tick = 0;
}

// 존 상태에 따른 상태 제어 { CHAR_SELECT | (상태가 존 간 이동상태일 경우) }
inline bool sSECURE_FLAG::sSTATUS::IsTransaction() const {
    return (control_info_.flag & STAT_TRANSACTION) == 0 ? false : true;
}

inline DWORD sSECURE_FLAG::sSTATUS::GetTransTime() const
{
    if (IsTransaction()) {
        return GetTickCount() - control_info_.trans_tick;
    }
    else {
        return 0;
    }
}

//==================================================================================================

inline DWORD& sSECURE_FLAG::sQUERY::FLAG() {
    return control_info_.flag;
}

inline sSECURE_FLAG::sQUERY::sQUERY(ControlInfo& control_info)
    : control_info_(control_info)
{
}

inline void sSECURE_FLAG::sQUERY::TransEnter(eQUERY_TRANS trans_value)
{
    control_info_.flag = (eQUERY_TRANS)((control_info_.flag & (~TRANS_MASK_QUERY)) |
                                        (TRANS_MASK_QUERY & trans_value));
}

inline sSECURE_FLAG::eQUERY_TRANS sSECURE_FLAG::sQUERY::TransType() const
{
    return (eQUERY_TRANS)(TRANS_MASK_QUERY & control_info_.flag);
}

inline void sSECURE_FLAG::sQUERY::TransLeave()
{
    control_info_.flag = (control_info_.flag & (~TRANS_MASK_QUERY));
}

// 질의중인가?
inline bool sSECURE_FLAG::sQUERY::IsTransaction() const {
    return (control_info_.flag & TRANS_DEFAULT_TRANS) == 0 ? false : true;
}

//==================================================================================================

inline DWORD& sSECURE_FLAG::sQUERYBUILTIN::FLAG() {
    return control_info_.flag;
}

inline sSECURE_FLAG::sQUERYBUILTIN::sQUERYBUILTIN(ControlInfo& control_info)
    : control_info_(control_info)
{
}

inline void sSECURE_FLAG::sQUERYBUILTIN::TransEnter()
{
    control_info_.flag = (eQUERY_TRANS)((control_info_.flag & (~TRANS_BUILTIN_QUERY)) |
                                        (TRANS_BUILTIN_QUERY));
}

inline void sSECURE_FLAG::sQUERYBUILTIN::TransLeave()
{
    control_info_.flag = (control_info_.flag & (~TRANS_BUILTIN_QUERY));
}

// 질의중인가?
inline bool sSECURE_FLAG::sQUERYBUILTIN::IsTransaction() const {
    return (control_info_.flag & TRANS_BUILTIN_QUERY) == 0 ? false : true;
}

//==================================================================================================
// <RANDOM TIME UTILITY>
class RandomTimeUtil
{
public:
    static const DWORD SECOND = 1000;
    static const DWORD MINUTE = 60 * SECOND;

    // <쿼리 이후 대기 시간 설정>
    // LIVE모드에서 { 기준 Timeout 180sec } 
    // USE : GetRandomTimeDelay_ByMinMaxBoundary
    // RANGE: { 180sec +/- 30sec }
    static DWORD SECURE_KEYEXCHANGE_TIMEOUT_POLE ;
    static DWORD SECURE_KEYEXCHANGE_TIMEOUT_MINMAX_BOUNDARY ;
    // RANGE: { dwFlagTick-dwMin ~ dwFlagTick+dwMax }
    static DWORD GetPostQueryTimeout(DWORD dwFlagTick, DWORD dwMinMax /* {+/-} dword */);
    // RANGE: { dwFlagTick ~ dwFlagTick+dwMax }
    static DWORD GetPostQueryTimeoutOver(DWORD dwFlagTick, DWORD dwMax /* {+} dword */);

    // <다음 쿼리 대상이 되는 시간 설정>
    // USE : GetRandomTimeDelay
    // RANGE: { MIN 5 ~ MAX 7 } 확률적으로 5~7분 사이가 많다.
    static DWORD SECURE_NEXT_CHECK_INTERVAL_POLE ;
    static DWORD SECURE_NEXT_CHECK_INTERVAL_MINMAX_BOUNDARY ;
    //static DWORD GetNextQueryTargetTick(DWORD dwFlagTick, DWORD dwMinMax /* {+/-} dword */)
    //{ GetPostQueryTimeout(dwFlagTick, dwMinMax); }

    // <종료 대기 상태 딜레이>
    // USE : GetRandomTimeDelay
    // RANGE: { MIN 13 ~ MAX 26 } 확률적으로 13~20초 사이가 많다.
    static DWORD SECURE_FAIL_CLOSING_INTERVAL_POLE ;
    static DWORD SECURE_FAIL_CLOSING_INTERVAL_MINMAX_BOUNDARY ;
    //static DWORD GetCloseWaitTick(DWORD dwFlagTick, DWORD dwMinMax /* {+/-} dword */)
    //{ GetPostQueryTimeout(dwFlagTick, dwMinMax); }

    // <난수 얻기> - DWORD { 0 ~ DWORD_MAX }
    static DWORD GetRandomNumber();
    // <난수 얻기> - 최소값과 최대값 사이에서 값을 얻는다.
    static DWORD GetRandomNumber(DWORD dwMin, DWORD dwUpperBound);

    //
    //
    //
    static DWORD SECURE_MANAGER_UPDATE_INTERVAL() { return cSECURE_MANAGER_UPDATE_INTERVAL; }
    static DWORD USER_KEYCHANGE_TIMEOUT() { return cUSER_KEYCHANGE_TIMEOUT; }

    static void Initialize(DWORD dwSERVER_MODE);
    //////////////////////////////////////////////////////////////////////////
    // <기본값> SecureManager Update 주기
    static DWORD cSECURE_MANAGER_UPDATE_INTERVAL ;
    // <기본값> 보안 질의 이후 응답 대기 시간
    static DWORD cUSER_KEYCHANGE_TIMEOUT ;
};



//==================================================================================================
// 패킷 암호화 관련 객체 패키징
class CryptPack4Packet
{
    //////////////////////////////////////////////////////////////////////////
    // <FIELDs>
private:
    ServerSNKey seq_number_;
    CryptManager crypt_manager_;

    //////////////////////////////////////////////////////////////////////////
    // <METHODs>
public:
    CryptPack4Packet() { }
    CryptPack4Packet(const CryptPack4Packet& backup);
    ~CryptPack4Packet() { }

    ServerSNKey& GetServerSNKey() { return seq_number_; }
    CryptManager& GetCryptManager() { return crypt_manager_; }
    // About SNKey
    void NextRoundSNKey4Send() { seq_number_.NextRoundSNKey4Send(); };
    void NextRoundSNKey4Recv() { seq_number_.NextRoundSNKey4Recv(); };

    BYTE GetSendSNKey() { return seq_number_.GetSendSNKey(RandomSNKey::SNKEY_BASE); };
    BYTE GetSendPairSNKey() { return seq_number_.GetSendSNKey(RandomSNKey::SNKEY_PAIR); };
    BYTE GetRecvSNKey() { return seq_number_.GetRecvSNKey(RandomSNKey::SNKEY_BASE); };
    BYTE GetRecvPairSNKey() { return seq_number_.GetRecvSNKey(RandomSNKey::SNKEY_PAIR); };

    // Cryptography
    BYTE GetCryptAlgorithmInfo(BYTE bySNKey, BYTE* pbyCryptKeyBlock, DWORD _dwUserKey);
    void InitCryptInfo(DWORD dwUserKey, BYTE byProtoMajorVer, BYTE byProtoMinorVer);
};


//==================================================================================================
// MOLA5th UPDATE 관련

class SecurePackNode4User;

struct SecureTransaction
{
    SecureTransaction(User* user);
    ~SecureTransaction();
};

//==================================================================================================

class MOLAControl
{
public:
    MOLAControl() : mola_work_key_(0), mola_builtin_work_key_(0) {}
    // 
    BOOL SendQueryRelease(User* user);
    // Builtin Query
    BOOL SendQueryBuiltinCMD(User* user);
    // Normal Query
    BOOL SendQuery(User* user, eSECURE_QUERY::IDX_T queryT, MSG_BASE* pMsg = NULL, WORD wSize = 0);
    BOOL RecvQuery(User* user, MSG_BASE* pMsg, WORD wSize);

    DWORD& WorkKey() { return mola_work_key_; }
    DWORD& WorkKeyBuiltin() { return mola_builtin_work_key_; }
private:
    // 현재 작업중인 목록이 필요하게 될 것이다.
    // 아님 위반사항같은 것들도 기록하게 될 것이다.
    DWORD mola_work_key_;
    DWORD mola_builtin_work_key_;
};

//==================================================================================================
// 각 User객체마다 존재하는 패킷 암호화 및 보안관련 패키징
class SecurePackNode4User : public util::__LList<SecurePackNode4User>
{
public:
    //
    void Init();
    void Release();
    void EnableHeartbeats(BOOL flag);

    //----------------------------------------------------------------------------------------------
    // <CRYPT PACKET CONTROL>
    // 패킷 암호화 사용중인가?
    bool IsUsePacketCrypt() const { return use_cryptography_; }
    // 패킷 암호화 설정용
    void SetEnablePacketCrypt();
    void InitCryptInfo(DWORD dwUserKey, BYTE byProtoMajorVer, BYTE byProtoMinorVer);
    // 패킷 암호화 키정보 추출
    BYTE GetCryptAlgorithmInfo(BYTE bySNKey, BYTE* pbyCryptInfo, DWORD _dwUserKey);
    // About SNKey
    void NextRoundSNKey4Send() { crypt_packet_info_for_active_.NextRoundSNKey4Send(); };
    void NextRoundSNKey4Recv() { crypt_packet_info_for_active_.NextRoundSNKey4Recv(); };
    BYTE GetSendSNKey() { return crypt_packet_info_for_active_.GetSendSNKey(); };
    BYTE GetSendPairSNKey() { return crypt_packet_info_for_active_.GetSendPairSNKey(); };
    BYTE GetRecvSNKey() { return crypt_packet_info_for_active_.GetRecvSNKey(); };
    BYTE GetRecvPairSNKey() { return crypt_packet_info_for_active_.GetRecvPairSNKey(); };

public:
    sSECURE_FLAG& SECURE_FLAG() { return secure_control_flag_; }
    //
    CryptPack4Packet& ACTIVE_CRYPT_PACKET_INFO() { return crypt_packet_info_for_active_; }
    CryptPack4Packet& BACKUP_CRYPT_PACKET_INFO() { return crypt_packet_info_for_backup_; }
    //
#if USING_HACKSHIELD
    AHNHS_CLIENT_HANDLE HANDLEofClientHS() { return handle_for_hackshield_; }
#elif USING_XIGNCODE
#elif USING_FROST
#elif USING_GAMEGUARD
#endif
    MOLAControl& MOLAController() { return mola_control_; }
    //
    HeartbeatChecker& HEARTBEAT() { return heartbeat_checker_; }
#ifdef _NA_20100802_SPEED_HACK_DETECTION
    TickSpeedChecker& TICKSPEED() { return tick_speed_checker_; }
#endif
    //----------------------------------------------------------------------------------------------
    // <SECURE LOGIC QUERY&REPLY CONTROL>
    void SECURE_ROUTINE_CHANGE_OBJECT();
    void SECURE_ROUTINE_CHANGE_SNKEY_PATTERN_DUPLEX();

    //----------------------------------------------------------------------------------------------
    // <PACKET ENCRYPTION & DECRYPTION>
    // 패킷 암호화
    bool EncryptPacket(BYTE* IN OUT pEncryptingMsg, const INT iSize, BYTE& OUT byCRC);
    // 패킷 복호화
    bool DecryptPacket(BYTE* IN OUT pEncryptedMsg, const INT iSize, BYTE IN byCRC);
    sSECURE_FLAG::eQUERY_ERRCTRL GetErrStatus() const;
    void SetErrStatus(sSECURE_FLAG::eQUERY_ERRCTRL status);

    //----------------------------------------------------------------------------------------------
    // <CRYPTOGRAPHY CONTROL>
    // <TEMP USER>
    BOOL SendPacket_KEYINFO_KEY(UserSession* pCurrSession, UserSession* pPostSession);
    BOOL SendPacket_KEYINFO_CMD(UserSession* pCurrSession, UserSession* pPostSession);
    // <USER REPLY PROCESSING>
    BOOL OnRecvCG_CRYPTOGRAPHY_ACK(User* user);
    BOOL OnRecvCG_CRYPTOGRAPHY_NAK(User* user);
    BOOL OnRecvCG_SECURE_POLICY_PATTERN02_SIMPLEX_SNK_ACK(User* user,
        MSG_CG_SECURE_POLICY_PATTERN02_SIMPLEX_SNK_ACK* pMsgACK);
    BOOL OnRecvCG_CONNECTION_HEARTBEAT_SYN(User* user);
    // <CRYPTING USER QUERY>
    BOOL Query_CRYPTOGRAPHY_SNK_KEY(User* user, sSECURE_FLAG::eQUERY_TRANS& eSTAT);
    BOOL Query_ARMORKIT_SEED_TEST_CMD(User* user, sSECURE_FLAG::eQUERY_TRANS& eSTAT);
    BOOL Query_SIMPLEX_SNK_CMD(User* user, sSECURE_FLAG::eQUERY_TRANS& eSTAT);
    //
    BOOL ForcedDisconnectProcess(User* user);
#if USING_HACKSHIELD
    BOOL Query_HACKSHIELD_SERVER_EX_QUERY_CMD(User* user, sSECURE_FLAG::eQUERY_TRANS& eSTAT);
    BOOL OnRecvCG_HACKSHIELD_SERVER_EX_QUERY_ACK(User* user,
        MSG_CG_HACKSHIELD_SERVER_EX_QUERY_ACK* pMsgACK, WORD wSize);
#elif USING_XIGNCODE
    bool XignCode_Init();
    bool XignCode_Release();
    BOOL Query_XIGNCODE_SERVER_QUERY_CMD(User* user, sSECURE_FLAG::eQUERY_TRANS& eSTAT);
    BOOL OnRecvXIGNCODE_SERVER_QUERY_ACK(User* user,
        MSG_CG_XIGNCODE_SERVER_QUERY_ACK* pMsgACK, WORD wSize);
#elif USING_FROST
#elif USING_GAMEGUARD
    BOOL Query_GAMEGUARD_SERVER_QUERY_CMD(User* user, sSECURE_FLAG::eQUERY_TRANS& eSTAT);
    BOOL OnRecv_GAMEGUARD_SERVER_QUERY_ACK(User* user, MSG_BASE* packet, WORD wSize);

    BOOL OnRecv_GAMEGUARD_HACKDATA_CMD(User* user, MSG_BASE* packet, WORD size);
#endif
private:
    //----------------------------------------------------------------------------------------------
    // <FIELDs>
    struct
    {
        bool use_cryptography_; // false로 설정할 것
        ITimeout secure_user_timeout_; // 객체 타임아웃 체커
    };

    // <MAIN PACKET CRYPTOGRAPHY INFORMATION BLOCK> ACK 떨어지면 세팅
    CryptPack4Packet crypt_packet_info_for_active_;
    // Secure Info Structure
    sSECURE_FLAG secure_control_flag_;

    sSECURE_FLAG::eQUERY_ERRCTRL error_flags_; // 타임아웃 대기 상태일 때, 정확한 에러 원인 찾기
#if USING_HACKSHIELD
    struct
    {
        AHNHS_CLIENT_HANDLE handle_for_hackshield_;
    };
#endif
#if USING_XIGNCODE
    ns_secure::XignCode xigncode_;
#endif
#if USING_FROST
#endif
#if USING_GAMEGUARD
    CCSAuth3 gameguard_csauth_;
#endif //USING_GAMEGUARD

    MOLAControl mola_control_;
    // <HEARTBEAT CONTROL>
    HeartbeatChecker heartbeat_checker_;
#ifdef _NA_20100802_SPEED_HACK_DETECTION
    TickSpeedChecker tick_speed_checker_;
#endif
    CryptPack4Packet crypt_packet_info_for_backup_; // CMD 보낼 때 세팅
    //----------------------------------------------------------------------------------------------
    friend class SecureManager;
    friend class MOLAControl;
};


inline bool SecurePackNode4User::EncryptPacket(
    BYTE* IN OUT pEncryptingMsg, const INT iSize, BYTE& OUT byCRC)
{
    CryptManager* crypt_manager = &crypt_packet_info_for_active_.GetCryptManager();
    return crypt_manager->Encrypt(pEncryptingMsg, iSize, byCRC) != false;
}

// 패킷 복호화
inline bool SecurePackNode4User::DecryptPacket(
    BYTE* IN OUT pEncryptedMsg, const INT iSize, BYTE IN byCRC)
{
    CryptManager* crypt_manager = &crypt_packet_info_for_active_.GetCryptManager();
    return crypt_manager->Decrypt(pEncryptedMsg, iSize, byCRC) != false;
}

inline sSECURE_FLAG::eQUERY_ERRCTRL SecurePackNode4User::GetErrStatus() const {
    return error_flags_;
}

inline void SecurePackNode4User::SetErrStatus(sSECURE_FLAG::eQUERY_ERRCTRL status)
{
    //ASSERT((sSECURE_FLAG::MASK & status) == status);
    error_flags_ = status;
}


//==================================================================================================
// <SecureManager>
// 일정한 주기마다 동작 관리
class SecureManager : public IWorkNode
{
public:
    static const DWORD MAX_PROCESSING_USERS_IN_ONE_CYCLE = 200; // 주기당 보안쿼리를 처리할 수 있는 최대 수 제한
    //
    SecureManager();
    ~SecureManager() {}

    // 이후 User등에 설정된 정보를 이쪽으로 이동시키자.
    void SetInformation(DWORD dwUpdateInterval, DWORD dwUserTimeout4ACK);
    //----------------------------------------------------------------------------------------------
    // <IWORKNODE INTERFACEs>
    virtual BOOL DoInit();
    virtual BOOL DoProcess();
    virtual BOOL IsComplete();
    virtual BOOL DoRelease();

    virtual void OnMsg(sSIG_MSG* pSIG_MSG);

public:
    //----------------------------------------------------------------------------------------------
    // <LIST MANAGEMENT>
    static SecurePackNode4User* GetCryptedUserListEntry() { return static_cast<SecurePackNode4User*>(&sCryptedUserListEntry); }
    static SecurePackNode4User* GetCryptingUserListEntry() { return static_cast<SecurePackNode4User*>(&sCryptingUserListEntry); }

    //
    static void AddCryptedUser(User* user);
    static void AddCryptedUser(SecurePackNode4User* pNode);
    //
    static SecurePackNode4User* RemoveCryptedUser(User* user);
    static SecurePackNode4User* RemoveCryptedUser(SecurePackNode4User* pNode);
    //
    static void AddCryptingUser(User* user, sSECURE_FLAG::eQUERY_TRANS eStat);
    static void AddCryptingUser(SecurePackNode4User* pNode, sSECURE_FLAG::eQUERY_TRANS eStat);
    //
    static SecurePackNode4User* RemoveCryptingUser(User* user);
    static SecurePackNode4User* RemoveCryptingUser(SecurePackNode4User* pNode);

private:
    void ProcessCryptedUsers();
    void ProcessCryptingUsers();
    BOOL ProcessQuery(User* user, SecurePackNode4User& rSECURE, sSECURE_FLAG::eQUERY_TRANS& eSTAT);

    //----------------------------------------------------------------------------------------------
    // <LIST MANAGEMENT><NODE CONTROL>
    struct Fields
    {
        DWORD m_dwStatus;
        // CHANGES: f101101.1L, change timer
        ITimerBase update_timer_;
        DWORD m_dwUpdateInterval;
        DWORD m_dwUserTimeout4ACK;
    };

    struct Util
    {
        Fields& m_rFields;
        Util(Fields& rFields) : m_rFields(rFields) {}

        SecurePackNode4User* GetNextNode(SecurePackNode4User* pNode) { return pNode->next; }
        SecurePackNode4User* GetPrevNode(SecurePackNode4User* pNode) { return pNode->prev; }
        // WARNING: this method is only used list control in SecureManager.
        //SecurePackNode4User* GetSecurePackNode4User(util::PListNode node) {
        //    return reinterpret_cast<SecurePackNode4User*>(node);
        //}
    };

    //----------------------------------------------------------------------------------------------
    // <FIELDs>
private:
    static util::__LList<SecurePackNode4User> sCryptedUserListEntry; // 암호화 설정된 유저 목록 기준점
    static util::__LList<SecurePackNode4User> sCryptingUserListEntry; // 암호화 설정중인 유저 목록 시작점(no TempUser)
    static util::__LList<SecurePackNode4User> sReleasingUserListEntry;// 암호화 ACK timeout 걸린 유저 목록

    Util UTIL;
    Fields FIELD;
    //
    friend class User;
    __DISABLE_COPY(SecureManager);
};

#endif //__USERINFO_SECURE_H__
