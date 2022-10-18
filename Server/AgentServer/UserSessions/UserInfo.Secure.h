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

    // ���� ���� ���¿� ���� ó�� ���п�
    enum eSTATUS
    {
        STAT_NOCONTROL      = 0x00000000,
        STAT_USERINIT       = 0x00000002, // �� Ű��ȯ�� �Ϸ�� ��Ȳ
        STAT_CRYPTED        = 0x00000004, // ���� ���
        STAT_CRYPTING       = 0x00000008, // ������ ���� ���� �����
        STAT_MASK_STATUS    = 0x0000007F, // STAT_TRANSACTION�� ������ ����ũ
        STAT_TRANSACTION    = 0x00000080, // �� �� �̵��� ���� Ŭ���̾�Ʈ�� ������ �����ϰ� �̷����� ���� ��츦 ������ ��
    };

    // ���� �������� ���� ���� ����
    // ���� �����ϴ� �� ��ü�� Ʈ�����
    enum eQUERY_TRANS
    {
        TRANS_NOTRANS           = (0x00000000),
        TRANS_DEFAULT_TRANS     = (0x00008000), // [1]�ֻ�� ��Ʈ�� ����� ����

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
        // ���� Ʈ������� �� Ʈ������� �ɸ��� ��� �۾��� �������� �ʴ´�.
        // CryptingList�� �����ؾ� �Ѵ�
        TRANS_FORCEDCLOSING     = SHL_VALUE(0x60),
        TRANS_MASK_QUERY        = (0x0000FF00),
#undef SHL_VALUE

        // ���� ����, �� BUILTIN_QUERY�� Ư�� ������ �ݵ�� �ߵ��ϵ���, �׸��� ������ ���
        // �ݵ�� �ް� �Ǿ� �ִ� ������ ��Ī�Ѵ�. �Ϲ����� ���� Ʈ����ǰ��� ������ �����Ѵ�.
        TRANS_BUILTIN_QUERY     = (0x01000000), // ASSERT! �̰��� �۵��ϴ� ������ �ٸ� �������� �̼�����
    };

    // ���� �������� ���� ������ �������� ��� ó���� ����Ÿ��
    // ���� ���, ���ǿ� �������� �ʾҰų� Ÿ�Ӿƿ��� �ɷ��� ���
    // ���� ���� Ʈ������� TRANS_FORCEDCLOSING ���·� ��ȯ�Ǵ��� �� ��������Ǵ� ������ ���߿� �α��ϱ� ����
    enum eQUERY_ERRCTRL
    {
        ERR_NOERR           = (0x00000000),
        ERR_DEFAULT_TRANS   = (0x00800000), // [1]�ֻ�� ��Ʈ�� ����� ����

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
        // ���� Ʈ����� ������ ���� Ʈ������� �߻����� �� �⺻���� �����ϴ� ����
        // ���� ���� ���¶� Ư�� ������ �����ϰ� �Ǹ� �ϴ��� Ʈ����� ������ ���� �б�� �� �ִ�.
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
    // ���Ǹ� �� �� �ִ°�?
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
        // �� ���¿� ���� ���� ���� { CHAR_SELECT | (���°� �� �� �̵������� ���) }
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
        // �������ΰ�?
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
        // �������ΰ�?
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

// ���Ǹ� �� �� �ִ°�?
inline bool sSECURE_FLAG::IsEnableQuerying() const
{
    bool querying = ((STAT_CRYPTED & control_info_.flag) && (!STATUS.IsTransaction()));
    return querying;
}
//// ���� CS���� ���� �߻����� ��� �� ���� ����
//static bool IsFault(DWORD& eREF) { return (bool)!!(ERR_FAULT&eREF); }

inline void sSECURE_FLAG::ClearFields()
{
    control_info_.flag = 0;
    control_info_.trans_tick = 0;
    // ITimeout �⺻ ����
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

// �� ���¿� ���� ���� ���� { CHAR_SELECT | (���°� �� �� �̵������� ���) }
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

// �������ΰ�?
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

// �������ΰ�?
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

    // <���� ���� ��� �ð� ����>
    // LIVE��忡�� { ���� Timeout 180sec } 
    // USE : GetRandomTimeDelay_ByMinMaxBoundary
    // RANGE: { 180sec +/- 30sec }
    static DWORD SECURE_KEYEXCHANGE_TIMEOUT_POLE ;
    static DWORD SECURE_KEYEXCHANGE_TIMEOUT_MINMAX_BOUNDARY ;
    // RANGE: { dwFlagTick-dwMin ~ dwFlagTick+dwMax }
    static DWORD GetPostQueryTimeout(DWORD dwFlagTick, DWORD dwMinMax /* {+/-} dword */);
    // RANGE: { dwFlagTick ~ dwFlagTick+dwMax }
    static DWORD GetPostQueryTimeoutOver(DWORD dwFlagTick, DWORD dwMax /* {+} dword */);

    // <���� ���� ����� �Ǵ� �ð� ����>
    // USE : GetRandomTimeDelay
    // RANGE: { MIN 5 ~ MAX 7 } Ȯ�������� 5~7�� ���̰� ����.
    static DWORD SECURE_NEXT_CHECK_INTERVAL_POLE ;
    static DWORD SECURE_NEXT_CHECK_INTERVAL_MINMAX_BOUNDARY ;
    //static DWORD GetNextQueryTargetTick(DWORD dwFlagTick, DWORD dwMinMax /* {+/-} dword */)
    //{ GetPostQueryTimeout(dwFlagTick, dwMinMax); }

    // <���� ��� ���� ������>
    // USE : GetRandomTimeDelay
    // RANGE: { MIN 13 ~ MAX 26 } Ȯ�������� 13~20�� ���̰� ����.
    static DWORD SECURE_FAIL_CLOSING_INTERVAL_POLE ;
    static DWORD SECURE_FAIL_CLOSING_INTERVAL_MINMAX_BOUNDARY ;
    //static DWORD GetCloseWaitTick(DWORD dwFlagTick, DWORD dwMinMax /* {+/-} dword */)
    //{ GetPostQueryTimeout(dwFlagTick, dwMinMax); }

    // <���� ���> - DWORD { 0 ~ DWORD_MAX }
    static DWORD GetRandomNumber();
    // <���� ���> - �ּҰ��� �ִ밪 ���̿��� ���� ��´�.
    static DWORD GetRandomNumber(DWORD dwMin, DWORD dwUpperBound);

    //
    //
    //
    static DWORD SECURE_MANAGER_UPDATE_INTERVAL() { return cSECURE_MANAGER_UPDATE_INTERVAL; }
    static DWORD USER_KEYCHANGE_TIMEOUT() { return cUSER_KEYCHANGE_TIMEOUT; }

    static void Initialize(DWORD dwSERVER_MODE);
    //////////////////////////////////////////////////////////////////////////
    // <�⺻��> SecureManager Update �ֱ�
    static DWORD cSECURE_MANAGER_UPDATE_INTERVAL ;
    // <�⺻��> ���� ���� ���� ���� ��� �ð�
    static DWORD cUSER_KEYCHANGE_TIMEOUT ;
};



//==================================================================================================
// ��Ŷ ��ȣȭ ���� ��ü ��Ű¡
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
// MOLA5th UPDATE ����

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
    // ���� �۾����� ����� �ʿ��ϰ� �� ���̴�.
    // �ƴ� ���ݻ��װ��� �͵鵵 ����ϰ� �� ���̴�.
    DWORD mola_work_key_;
    DWORD mola_builtin_work_key_;
};

//==================================================================================================
// �� User��ü���� �����ϴ� ��Ŷ ��ȣȭ �� ���Ȱ��� ��Ű¡
class SecurePackNode4User : public util::__LList<SecurePackNode4User>
{
public:
    //
    void Init();
    void Release();
    void EnableHeartbeats(BOOL flag);

    //----------------------------------------------------------------------------------------------
    // <CRYPT PACKET CONTROL>
    // ��Ŷ ��ȣȭ ������ΰ�?
    bool IsUsePacketCrypt() const { return use_cryptography_; }
    // ��Ŷ ��ȣȭ ������
    void SetEnablePacketCrypt();
    void InitCryptInfo(DWORD dwUserKey, BYTE byProtoMajorVer, BYTE byProtoMinorVer);
    // ��Ŷ ��ȣȭ Ű���� ����
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
    // ��Ŷ ��ȣȭ
    bool EncryptPacket(BYTE* IN OUT pEncryptingMsg, const INT iSize, BYTE& OUT byCRC);
    // ��Ŷ ��ȣȭ
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
        bool use_cryptography_; // false�� ������ ��
        ITimeout secure_user_timeout_; // ��ü Ÿ�Ӿƿ� üĿ
    };

    // <MAIN PACKET CRYPTOGRAPHY INFORMATION BLOCK> ACK �������� ����
    CryptPack4Packet crypt_packet_info_for_active_;
    // Secure Info Structure
    sSECURE_FLAG secure_control_flag_;

    sSECURE_FLAG::eQUERY_ERRCTRL error_flags_; // Ÿ�Ӿƿ� ��� ������ ��, ��Ȯ�� ���� ���� ã��
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
    CryptPack4Packet crypt_packet_info_for_backup_; // CMD ���� �� ����
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

// ��Ŷ ��ȣȭ
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
// ������ �ֱ⸶�� ���� ����
class SecureManager : public IWorkNode
{
public:
    static const DWORD MAX_PROCESSING_USERS_IN_ONE_CYCLE = 200; // �ֱ�� ���������� ó���� �� �ִ� �ִ� �� ����
    //
    SecureManager();
    ~SecureManager() {}

    // ���� User� ������ ������ �������� �̵���Ű��.
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
    static util::__LList<SecurePackNode4User> sCryptedUserListEntry; // ��ȣȭ ������ ���� ��� ������
    static util::__LList<SecurePackNode4User> sCryptingUserListEntry; // ��ȣȭ �������� ���� ��� ������(no TempUser)
    static util::__LList<SecurePackNode4User> sReleasingUserListEntry;// ��ȣȭ ACK timeout �ɸ� ���� ���

    Util UTIL;
    Fields FIELD;
    //
    friend class User;
    __DISABLE_COPY(SecureManager);
};

#endif //__USERINFO_SECURE_H__
