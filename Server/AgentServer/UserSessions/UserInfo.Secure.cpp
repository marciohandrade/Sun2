#include "stdafx.h"
#include "UserInfo.Secure.h"

#include "User.h"
#include "TempUser.h"

#include <ServerOptionParserEx.h>

//==================================================================================================

// ��ȣȭ ������ ���� ��� ������
util::__LList<SecurePackNode4User>
SecureManager::sCryptedUserListEntry;
// ��ȣȭ �������� ���� ��� ������(no TempUser)
util::__LList<SecurePackNode4User>
SecureManager::sCryptingUserListEntry;
// ��ȣȭ ACK timeout �ɸ� ���� ���
util::__LList<SecurePackNode4User>
SecureManager::sReleasingUserListEntry;

//==================================================================================================
SecureManager::SecureManager()
    : UTIL(FIELD)
{
    util::LList::Init(&sCryptedUserListEntry);
    util::LList::Init(&sCryptingUserListEntry);
    util::LList::Init(&sReleasingUserListEntry);
}

// NOTE: AddCryptedUser �� ȣ���� �ÿ�, ���� ���� �ƴϾ�� �Ѵ�.
void SecureManager::AddCryptedUser(SecurePackNode4User* node)
{
    SecurePackNode4User& secure = *node; // problem point
    secure.SECURE_FLAG().STATUS.SetStatus(sSECURE_FLAG::STAT_CRYPTED);
    util::LList::AddPrev(node, &sCryptedUserListEntry);
}

void SecureManager::AddCryptedUser(User* user)
{
    AddCryptedUser(&user->GetSecureNode());
}

//
SecurePackNode4User* SecureManager::RemoveCryptedUser(SecurePackNode4User* node)
{
    return util::LList::Delete(node);
}

SecurePackNode4User* SecureManager::RemoveCryptedUser(User* user)
{
    return RemoveCryptedUser(&user->GetSecureNode());
}

//
//      BuiltinQuery�� ���� ����Ʈ���� ����(CRYPTED LIST), �߰�(CRYPTING LIST)���� �ʴ´�.
void SecureManager::AddCryptingUser(SecurePackNode4User* node,
                                    sSECURE_FLAG::eQUERY_TRANS trans_status)
{
    SecurePackNode4User& secure = *node;
    ASSERT(sSECURE_FLAG::TRANS_BUILTIN_QUERY != trans_status);
    secure.SECURE_FLAG().STATUS.SetStatus(sSECURE_FLAG::STAT_CRYPTING);
    secure.SECURE_FLAG().QUERY.TransEnter(trans_status);
    util::LList::AddPrev(node, &sCryptingUserListEntry);
}

void SecureManager::AddCryptingUser(User* user, sSECURE_FLAG::eQUERY_TRANS trans_status)
{
    SecurePackNode4User& secure = user->GetSecureNode();
    AddCryptingUser(&secure, trans_status);
}
//
SecurePackNode4User* SecureManager::RemoveCryptingUser(SecurePackNode4User* node)
{
    return util::LList::Delete(node);
}

SecurePackNode4User* SecureManager::RemoveCryptingUser(User* user)
{
    return RemoveCryptingUser(&user->GetSecureNode());
}

//==================================================================================================
// ��Ŷ ��ȣȭ ���� ��ü ��Ű¡
//class CryptPack4Packet
CryptPack4Packet::CryptPack4Packet(const CryptPack4Packet& backup)
{
    memcpy(&seq_number_, &(backup.seq_number_), sizeof(seq_number_));
    memcpy(&crypt_manager_, &(backup.crypt_manager_), sizeof(crypt_manager_));
};

BYTE CryptPack4Packet::GetCryptAlgorithmInfo(BYTE bySNKey, BYTE* pbyCryptKeyBlock, DWORD _dwUserKey)
{
    return crypt_manager_.GetCryptAlgorithmInfo(bySNKey, pbyCryptKeyBlock, _dwUserKey);
}

void CryptPack4Packet::InitCryptInfo(DWORD dwUserKey, BYTE byProtoMajorVer, BYTE byProtoMinorVer)
{
    /*C2S_HIGH_VERSION_NO, C2S_MIDDLE_VERSION_NO*/
    seq_number_.Initialize(dwUserKey, byProtoMajorVer, byProtoMinorVer);
    crypt_manager_.Initialize(CryptManager::bServerSession);
}

//==================================================================================================
// MOLA5th UPDATE ����

SecureTransaction::SecureTransaction(User* user)
{
    AgentServer::GetGResource()->SetActiveTransactionUser(user);
};

SecureTransaction::~SecureTransaction()
{
    AgentServer::GetGResource()->SetActiveTransactionUser(NULL);
};

//==================================================================================================

void SecurePackNode4User::Init()
{
    util::LList::Init(this);
    use_cryptography_ = false;

    //m_dwFlags = 0;
    error_flags_ = sSECURE_FLAG::ERR_NOERR;
    secure_user_timeout_.Instance();
    //m_iArmorKitSeed = 0;
    SECURE_FLAG().ClearFields();
#if USING_HACKSHIELD
    handle_for_hackshield_ = ANTICPX_INVALID_HANDLE_VALUE;
#elif USING_XIGNCODE
    this->XignCode_Init();
#elif USING_FROST
#elif USING_GAMEGUARD
    gameguard_csauth_.Init(true);
#endif
}

void SecurePackNode4User::Release()
{
    util::LList::Delete(this);
    use_cryptography_ = false;
    error_flags_ = sSECURE_FLAG::ERR_NOERR;
    secure_user_timeout_.Instance();
    SECURE_FLAG().ClearFields();
#if USING_HACKSHIELD
    if (handle_for_hackshield_ != ANTICPX_INVALID_HANDLE_VALUE) {
        _AhnHS_CloseClientHandle(handle_for_hackshield_);
    }
    handle_for_hackshield_ = ANTICPX_INVALID_HANDLE_VALUE;
#elif USING_XIGNCODE
    this->XignCode_Release();
#elif USING_FROST
#elif USING_GAMEGUARD
    gameguard_csauth_.Close();
#endif
}

void SecurePackNode4User::EnableHeartbeats(BOOL flag)
{
    heartbeat_checker_.SetEnable(flag ? true : false);
#ifdef _NA_20100802_SPEED_HACK_DETECTION
    tick_speed_checker_.SetEnabled(flag ? true : false);
#endif
}

void SecurePackNode4User::SECURE_ROUTINE_CHANGE_OBJECT()
{
    crypt_packet_info_for_active_.GetCryptManager().Release();
    if (CryptManager* crypt_manager = &crypt_packet_info_for_active_.GetCryptManager())
    {
        CopyMemory(crypt_manager, &(crypt_packet_info_for_backup_.GetCryptManager()),
                   sizeof(*crypt_manager));
    }
    if (ServerSNKey* sequence_key = &crypt_packet_info_for_active_.GetServerSNKey())
    {
        CopyMemory(sequence_key, &(crypt_packet_info_for_backup_.GetServerSNKey()),
                   sizeof(*sequence_key));
    }
}

void SecurePackNode4User::SECURE_ROUTINE_CHANGE_SNKEY_PATTERN_DUPLEX()
{
    if (ServerSNKey* sequence_key = &crypt_packet_info_for_active_.GetServerSNKey())
    {
        CopyMemory(sequence_key, &(crypt_packet_info_for_backup_.GetServerSNKey()),
                   sizeof(*sequence_key));
    }
}

// ��Ŷ ��ȣȭ ������
void SecurePackNode4User::SetEnablePacketCrypt()
{
    use_cryptography_ = true;
    crypt_packet_info_for_active_.GetCryptManager().StartLogic(use_cryptography_);
}

void SecurePackNode4User::InitCryptInfo(DWORD dwUserKey, BYTE byProtoMajorVer, BYTE byProtoMinorVer)
{
    crypt_packet_info_for_active_.InitCryptInfo(dwUserKey, byProtoMajorVer, byProtoMinorVer);
}

// ��Ŷ ��ȣȭ Ű���� ����
BYTE SecurePackNode4User::GetCryptAlgorithmInfo(BYTE bySNKey, BYTE* pbyCryptInfo, DWORD _dwUserKey)
{
    CryptManager* crypt_manager = &crypt_packet_info_for_active_.GetCryptManager();
    return crypt_manager->GetCryptAlgorithmInfo(bySNKey, pbyCryptInfo, _dwUserKey);
}


//==================================================================================================

BOOL SecurePackNode4User::SendPacket_KEYINFO_KEY(
    UserSession* pCurrSession, UserSession* pPostSession)
{
	TempUser* pTempUser = (TempUser*)pCurrSession;
	User* pUser = (User*)pPostSession;
    __TOUCHED(pUser);

	const	DWORD rndUserKeyBase		= 0x1319A267;
	const	DWORD rndUserKeyCrypt	= 0x316EF35F;
	static	DWORD rndUserKey			= (GetTickCount()>>8)%0x0000FFFF;

	// <Cryptography>
	//   �̰��� SN������ ��ȣȭ ���������� �����ϱ�� �Ѵ�.
	rndUserKey = (rndUserKeyBase * (rndUserKey)) + (rndUserKeyCrypt + ((rndUserKey) >> 16));
    CryptPack4Packet& rCryptBackup = BACKUP_CRYPT_PACKET_INFO();
	rCryptBackup.InitCryptInfo(rndUserKey, C2S_HIGH_VERSION_NO, C2S_MIDDLE_VERSION_NO);

	BYTE keySTREAM[280];
	MSG_CG_CRYPTOGRAPHY_KEY* pMsgKEY = new (keySTREAM) MSG_CG_CRYPTOGRAPHY_KEY();

	BYTE byKey = rCryptBackup.GetSendSNKey();
	pMsgKEY->SetSNKey(byKey);
	byKey = rCryptBackup.GetCryptAlgorithmInfo(rCryptBackup.GetSendPairSNKey(),
                                               pMsgKEY->GetCryptStream(), rndUserKey);
	pMsgKEY->SetCryptInfo(byKey);
	pTempUser->SendPacket(pMsgKEY, sizeof(keySTREAM));

	rCryptBackup.NextRoundSNKey4Send();

	return true;
}

#if defined(_KOREA) || defined(_CHINA) //__NA000000_081110_LIVE_SERVICE_TEST_CLINET_SUPPORT__

// (WAVERIX) (090618) (CHANGES) ���̺� �ٽ��� ���� ��� ���� (for Test) �߰�
struct sLiveSecuritySkipUserID
{
    static const DWORD  MAX_USERS	= 20;
    static const SIZE_T BUFFER_SIZE = (((ID_MAX_LEN) + ((16) - 1)) & (~((16) - 1)));
    union {
        TCHAR   userID[BUFFER_SIZE];
        DWORD   tag;
    };
};

static const sLiveSecuritySkipUserID
LiveSecuritySkipUserID_s[sLiveSecuritySkipUserID::MAX_USERS] =
{
    /*
#if defined(_KOREA)
    "ANGSOO",       // ��â��
    "CHANL2432840", // �����
    "CHANL2478029", // ������
    "CHANL2523927", // ������
    "CRAZYTOTO",    // ������
    "ELF2LEE",      // �̵���
    "IIZUMI",       // �����
    "MYKIKI",       // ������
    "S98807112",    // �ۿ���
    "SUNQA001",
    "SUNQA002",
    "SUNQA003",
    "SUNQA004",
    "SUNQA005",
    "SUNTEST001",
    "SUNTEST002",
    "TOPLESS",      // ������
#elif defined(_CHINA)
    "SUNB-001",
    "SUNB-002",
    "SUNB-003",
    "SUNB-004",
    "SUNB-005",
#endif
    */
};

#endif

BOOL SecurePackNode4User::SendPacket_KEYINFO_CMD(UserSession* pCurrSession,
                                                 UserSession* pPostSession)
{
    TempUser* pTempUser = (TempUser*)pCurrSession;
    User* pUser = (User*)pPostSession;

    MSG_CG_CRYPTOGRAPHY_CMD msgCMD;
    pTempUser->SendPacket(&msgCMD, sizeof(msgCMD));

    SECURE_ROUTINE_CHANGE_OBJECT();
    SetEnablePacketCrypt();

    sSECURE_FLAG& rFlag = pUser->GetSecureNode().SECURE_FLAG();
    SecureManager::RemoveCryptingUser(pUser);
    rFlag.STATUS.SetStatus(sSECURE_FLAG::STAT_CRYPTED);
    SecureManager::AddCryptedUser(pUser);
    rFlag.STATUS.SetStatus(sSECURE_FLAG::STAT_USERINIT);

#if defined(_KOREA)
    if (AgentServer::GetInstance()->GetMode() == eSERVER_DEV)
    {
        SecureManager::RemoveCryptedUser(pUser);
    }
    else
#endif
#if defined(_KOREA) || defined(_CHINA) //__NA000000_081110_LIVE_SERVICE_TEST_CLINET_SUPPORT__
    {   // <- else block
        const TCHAR* user_id = pUser->GetUserID();
        const sLiveSecuritySkipUserID* it = &LiveSecuritySkipUserID_s[0];
        const sLiveSecuritySkipUserID* end = &LiveSecuritySkipUserID_s[_countof(LiveSecuritySkipUserID_s)];
        for ( ; it != end; ++it)
        {
            BOOLEAN check_cond_ok = it->tag && it->tag == *reinterpret_cast<const DWORD*>(user_id);
            if (check_cond_ok == false) {
                continue;
            }
            BOOLEAN is_matched = (0 == strncmp(user_id, it->userID, ID_MAX_LEN));
            if (is_matched)
            {
                SecureManager::RemoveCryptedUser(pUser);
                break;
            }
        }
    };
#endif
#if USING_HACKSHIELD
    // NOTE: f110201.4L, moved from Init method. instance handle if it needs.
    if (handle_for_hackshield_ == ANTICPX_INVALID_HANDLE_VALUE)
    {
        AHNHS_SERVER_HANDLE server_handle_for_hackshield = AgentServer::GetHANDLEofHS();
        handle_for_hackshield_ = _AhnHS_CreateClientObject(server_handle_for_hackshield);
        if (handle_for_hackshield_ == ANTICPX_INVALID_HANDLE_VALUE)
        {
            MessageOut(eCRITICAL_LOG, _T("My God! Failed to Create Client HS"));
        }
    };
#elif USING_XIGNCODE
#elif USING_FROST
    ;{
        SecureManager::RemoveCryptedUser(pUser);
    };
#endif
    return true;
}

BOOL SecurePackNode4User::OnRecvCG_CRYPTOGRAPHY_ACK(User* pUser)
{
    // Ŭ���̾�Ʈ���� �������� �� Ű ����϶�� �˷��ְ� ������ �����Ѵ�.
    // �� ������ ���� ���� ������ �޴� ���� ������ �ٲ� Ű�� ����Ѵ�.
    BOOL bRet = GetErrStatus() == sSECURE_FLAG::ERR_DUPLEX_SNK;

    if(bRet)
    {
        MSG_CG_CRYPTOGRAPHY_SNK_CMD msgCMD;
        pUser->SendPacket(&msgCMD, sizeof(msgCMD));
        //pUser->CopyCrypt_Temp2Active();
        SECURE_ROUTINE_CHANGE_SNKEY_PATTERN_DUPLEX();
        SetEnablePacketCrypt();

        SecureManager::RemoveCryptingUser(pUser);
        SecureManager::AddCryptedUser(pUser);
        SetErrStatus(sSECURE_FLAG::ERR_NOERR);

        DWORD dwNextTickIntevel =
            RandomTimeUtil::GetPostQueryTimeout(RandomTimeUtil::SECURE_NEXT_CHECK_INTERVAL_POLE,
            RandomTimeUtil::SECURE_NEXT_CHECK_INTERVAL_MINMAX_BOUNDARY);

        secure_user_timeout_.SetNextTimeoutByCurTickBased(dwNextTickIntevel);

        return true;
    }
    return false;
}

BOOL SecurePackNode4User::OnRecvCG_CRYPTOGRAPHY_NAK(User* pUser)
{
    MessageOutEx(g_pSunLog, CSunLog::cCRITICAL_LOG, CSunLog::cOUTPUT_BOTH, CSunLog::cCATE_CONNECTION,
        _T("[GUID:%u] ��ȣŰ ��ȯ ����"), pUser->GetUserKey());

    SecureManager::RemoveCryptingUser(pUser);

    pUser->SetDisconnectCode(RC::RC_DISCONNECT_CRYPTOGRAPHY_KEYEXCHANGE_NAK);
    pUser->DisconnectUser(true);

    return true;
}

BOOL SecurePackNode4User::OnRecvCG_CONNECTION_HEARTBEAT_SYN(User* pUser)
{
    __UNUSED(pUser);
    return true;
}

//////////////////////////////////////////////////////////////////////////

BOOL SecurePackNode4User::Query_CRYPTOGRAPHY_SNK_KEY(User* pUser, sSECURE_FLAG::eQUERY_TRANS& eSTAT)
{
    DWORD dwRndUserKey = RandomTimeUtil::GetRandomNumber();

    // <SNKey>
    MSG_CG_CRYPTOGRAPHY_SNK_KEY msgKEY; 
    msgKEY.m_dwSNKey = dwRndUserKey;
    pUser->SendPacket(&msgKEY, sizeof(MSG_CG_CRYPTOGRAPHY_SNK_KEY));

    BACKUP_CRYPT_PACKET_INFO().GetServerSNKey().Initialize(dwRndUserKey, C2S_HIGH_VERSION_NO, C2S_MIDDLE_VERSION_NO);

    eSTAT = sSECURE_FLAG::TRANS_DUPLEX_SNK;
    SetErrStatus(sSECURE_FLAG::ERR_DUPLEX_SNK);

    SUNLOG(eFULL_LOG, _T("KEY EXCHANGE CMD"));

    //// (WAVERIX)
    //MSG_CG_CRYPTOGRAPHY_SNK_CMD   msg;
    //pUser->SendPacket(&msg, sizeof(MSG_CG_CRYPTOGRAPHY_SNK_CMD));
    //bProcess = false;

    return true;
}


BOOL SecurePackNode4User::ForcedDisconnectProcess(User* pUser)
{
    // �ٸ� ��� ���� ���� �����ϰ� ����ó�� ��ƾ���� ����.
    SetErrStatus(sSECURE_FLAG::ERR_FORCEDCLOSING);
    SecureManager::RemoveCryptingUser(pUser);
    SecureManager::AddCryptingUser(pUser, sSECURE_FLAG::TRANS_FORCEDCLOSING);

    //  ���� ��� �ð� ����
    DWORD curTick = GetTickCount();
    DWORD flagTick = secure_user_timeout_.GetTimeoutTicks();
    flagTick = (curTick < flagTick) ? flagTick - curTick : curTick - flagTick;

    if(RandomTimeUtil::MINUTE < flagTick)
    {
        DWORD nextTick =
            RandomTimeUtil::GetPostQueryTimeoutOver(RandomTimeUtil::SECURE_FAIL_CLOSING_INTERVAL_POLE,
            RandomTimeUtil::SECURE_FAIL_CLOSING_INTERVAL_MINMAX_BOUNDARY);
        secure_user_timeout_.SetNextTimeoutByCurTickBased(nextTick);
    }

    return true;
}

#if USING_GAMEGUARD
BOOL SecurePackNode4User::Query_GAMEGUARD_SERVER_QUERY_CMD( User* user, sSECURE_FLAG::eQUERY_TRANS& eSTAT )
{
    const DWORD cur_tick = GetTickCount();
    DWORD timeout = ITimeout::INSTANCE;

    UINT32 return_packet_size = 0;
    UINT32 err_code = gameguard_csauth_.Get(&return_packet_size);
    bool is_already_failed = (this->GetErrStatus() == sSECURE_FLAG::ERR_VIOLATION_FLOW_SEQ);

    if (is_already_failed) {
        // ERR_VIOLATION_FLOW_SEQ����, CMD�� ACK�� ���е� ���¶�� NAK�� ���������� �ʴ´�
        return FALSE;
    }

    if (return_packet_size > 0 && err_code < 3000) {
        MSG_CG_GAMEGUARD_SERVER_QUERY_CMD packet;
        packet.clear();

        memcpy(packet.buffer, gameguard_csauth_.packet, sizeof(packet.buffer));
        packet.query_size = return_packet_size;
        user->SendPacket(&packet, sizeof(packet));

        eSTAT = sSECURE_FLAG::TRANS_GAMEGUARD;
        SetErrStatus(sSECURE_FLAG::ERR_GAMEGUARD_QUERY);

        //SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] Send GameGuard Auth Query! query_size: %d", return_packet_size);
        return TRUE;
    }
    else // if (return_packet_size <= 0 || err_code < 3000)
    {
        // ������ ���
        this->SetErrStatus(sSECURE_FLAG::ERR_VIOLATION_FLOW_SEQ);

        timeout = RandomTimeUtil::GetPostQueryTimeoutOver(
            RandomTimeUtil::SECURE_FAIL_CLOSING_INTERVAL_POLE,
            RandomTimeUtil::SECURE_FAIL_CLOSING_INTERVAL_MINMAX_BOUNDARY);
        secure_user_timeout_.SetNextTimeoutByCurTickBased(timeout);

        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] GameGuard Auth Failed!(query_size:%d/err_code:%d) [Name:%s, CharGuid:%d]", 
               return_packet_size, err_code, user->GetSelectedCharName(), user->GetSelectedCharGuid());

        MSG_CG_GAMEGUARD_SERVER_QUERY_NAK nak_packet;
        nak_packet.result_code = err_code;
        user->SendPacket(&nak_packet, sizeof(nak_packet));

        return TRUE;
    }
    return FALSE;
}

BOOL SecurePackNode4User::OnRecv_GAMEGUARD_SERVER_QUERY_ACK( User* user, MSG_BASE* packet, WORD wSize )
{
    MSG_CG_GAMEGUARD_SERVER_QUERY_ACK* recved_packet = static_cast<MSG_CG_GAMEGUARD_SERVER_QUERY_ACK*>(packet);
    
    ZeroMemory(gameguard_csauth_.packet, sizeof(gameguard_csauth_.packet));
    memcpy(gameguard_csauth_.packet , recved_packet->buffer, sizeof(gameguard_csauth_.packet));

    const DWORD cur_tick = GetTickCount();
    DWORD timeout = ITimeout::INSTANCE;
    // ErrStatus Check
    bool success = (this->GetErrStatus() == sSECURE_FLAG::ERR_GAMEGUARD_QUERY);
    // Auth Query Size Check
    UINT32 result_code = gameguard_csauth_.Check(recved_packet->query_size);

    if (success == false || result_code >= 3000)
    {
        // ������ ���
        this->SetErrStatus(sSECURE_FLAG::ERR_VIOLATION_FLOW_SEQ);
        
        if ((cur_tick + 30 * RandomTimeUtil::SECOND) < secure_user_timeout_.GetTimeoutTicks())
        {
            timeout = RandomTimeUtil::GetPostQueryTimeoutOver(
                RandomTimeUtil::SECURE_FAIL_CLOSING_INTERVAL_POLE,
                RandomTimeUtil::SECURE_FAIL_CLOSING_INTERVAL_MINMAX_BOUNDARY);
            secure_user_timeout_.SetNextTimeoutByCurTickBased(timeout);
        }
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] GameGuard Auth Failed! [Name:%s, CharGuid:%d]", 
               user->GetSelectedCharName(), user->GetSelectedCharGuid());

        MSG_CG_GAMEGUARD_SERVER_QUERY_NAK nak_packet;
        nak_packet.result_code = result_code;
        user->SendPacket(&nak_packet, sizeof(nak_packet));
        return FALSE;
    }
    else // if (success)
    {
        SecureManager::RemoveCryptingUser(this);
        SecureManager::AddCryptedUser(this);
        this->SetErrStatus(sSECURE_FLAG::ERR_NOERR);
        timeout = RandomTimeUtil::GetPostQueryTimeout(
            RandomTimeUtil::SECURE_NEXT_CHECK_INTERVAL_POLE,
            RandomTimeUtil::SECURE_NEXT_CHECK_INTERVAL_MINMAX_BOUNDARY);
        secure_user_timeout_.SetNextTimeoutByCurTickBased(timeout);
        //SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] ErrStatus is Success! timout_setting: %d", timeout);
    }

    //SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] Received GameGuard Auth Query From Client!");
    return TRUE;
}

BOOL SecurePackNode4User::OnRecv_GAMEGUARD_HACKDATA_CMD( User* user, MSG_BASE* packet, WORD size )
{
    MSG_CG_GAMEGUARD_HACKDATA_CMD* recved_packet = static_cast<MSG_CG_GAMEGUARD_HACKDATA_CMD*>(packet);

    UINT32 ret = DecryptHackData("SUN3CN", recved_packet, GAMEGUARD_HACKDATA_PACKET_SIZE);
    if (ret == -1) {
        return FALSE;
    }
    return TRUE;
}
#endif // USING_GAMEGUARD
//==================================================================================================
//  MOLA5th UPDATE ����
BOOL MOLAControl::SendQueryRelease(User* pUser)
{
    SecurePackNode4User& rSecure = pUser->GetSecureNode();
    sSECURE_FLAG& rSecureFlag = rSecure.SECURE_FLAG();
    __TOUCHED(rSecureFlag);
    // SecureTransaction�� �ݵ�� �ʿ��ϴ�
    SecureTransaction SECURE_TRANSACTION(pUser);
    ISecureWorkNode* pSecureNode = CryptManager::GetISecureWorkNode();
    MOLA_QUERY_MSG msgMOLA(pUser->GetUserGUID(), eSECURE_QUERY::DEL_USER, 0, 0, NULL, NULL);
    return (!!pSecureNode->QueryProcess(&msgMOLA));
}

BOOL MOLAControl::SendQueryBuiltinCMD(User* pUser)
{
    //  1) STATUS::CRYPTED (������������ ������ Ȯ��ġ �ʴ�.)
    //  2) STATUS::TRANSACTION (Ȯ���ؾ� �Ѵ�.)
    SecurePackNode4User& rSecure = pUser->GetSecureNode();
    sSECURE_FLAG& rSecureFlag = rSecure.SECURE_FLAG();
    BOOL bStatusTrans = rSecureFlag.STATUS.IsTransaction();
    BOOL bBuiltinTrans = rSecureFlag.QUERYBUILTIN.IsTransaction();

    //  3) ePRS_BEFORE_ENTER_LOBBY
    eZONE_STATE zsSTATE = pUser->GetZoneState();
    if((ePRS_AT_LOBBY == zsSTATE) || (ePRS_BEFORE_ENTER_LOBBY == zsSTATE))
        return true;

    if(bStatusTrans)
    {
        if(!bBuiltinTrans)
        {
            // SecureTransaction�� �ݵ�� �ʿ��ϴ�
            SecureTransaction SECURE_TRANSACTION(pUser);
            ISecureWorkNode* pSecureNode = CryptManager::GetISecureWorkNode();
            MOLA_QUERY_MSG msgMOLA(pUser->GetUserGUID(), eSECURE_QUERY::BUILTIN_QUERY, 0, 0, NULL, NULL);
            WorkKeyBuiltin() = pSecureNode->QueryProcess(&msgMOLA);
            rSecureFlag.QUERYBUILTIN.TransEnter();

            //rSecureFlag.STATUS.TransLeave();

            return true;
        }

        SECURELOG(SECURELOG_USERINFO(pUser, eABUSE_LOGIC_VIOLATION, eABUSE_SUB_LOGIC_VIOLATION_SECURITY_QUERY_DISCONNECT),
            _T("[MOLAControl::SendQueryBuiltinCMD] Assertion Flow Violation"));

        MessageOut(eCRITICAL_LOG, _T("MonitorLog [%u][%s]"), pUser->GetUserGUID(), pUser->GetUserID());

        return false;   // BuiltinQuery �Ҹ���     
    }

    // Disconnect Control
    //ASSERT(bStatusTrans);
    return false;
}

BOOL MOLAControl::SendQuery(User* pUser, eSECURE_QUERY::IDX_T queryT, MSG_BASE* pMsg, WORD wSize)
{
    SecureTransaction SECURE_TRANSACTION(pUser);

    if(!((eSECURE_QUERY::MAX_SECURE_PACKET > queryT) && (eSECURE_QUERY::BUILTIN_QUERY != queryT)))
        return false;

    ISecureWorkNode* pSecureNode = CryptManager::GetISecureWorkNode();
    MOLA_QUERY_MSG rMSG(pUser->GetUserGUID(), queryT, 0, 0, (BYTE*)pMsg, wSize);

    // �ٸ� ��ɾ ������ �Ǹ� ���δ� ������ ��.
    WorkKey() = pSecureNode->QueryProcess(&rMSG);
    if(WorkKey())
    {
        SecurePackNode4User& rSecure = pUser->GetSecureNode();
        rSecure.SetErrStatus(sSECURE_FLAG::ERR_MOLA_CONFIG);
        if(eSECURE_QUERY::BUILTIN_QUERY == queryT)
            rSecure.SECURE_FLAG().QUERYBUILTIN.TransEnter();
        //else
        //  rSecure.SECURE_FLAG().QUERY.TransEnter();
    }

    return true;
}

BOOL MOLAControl::RecvQuery(User* pUser, MSG_BASE* pMsg, WORD wSize)
{
    SecureTransaction SECURE_TRANSACTION(pUser);

    ISecureWorkNode* pSecureNode = CryptManager::GetISecureWorkNode();

    SecurePackNode4User& rSecure = pUser->GetSecureNode();
    sSECURE_FLAG& rSecureFlag = rSecure.SECURE_FLAG();

    MOLA_QUERY_MSG rMSG(pUser->GetUserGUID(), eSECURE_QUERY::NON_OPERATION, WorkKey(), WorkKeyBuiltin(), (BYTE*)pMsg, wSize);
    DWORD dwRET = pSecureNode->OnRecvPacket(&rMSG);
    switch(dwRET)
    {
    case 1: //SUCCESS (NORMAL WORK)
        WorkKey() = 0;
        rSecureFlag.QUERY.TransLeave();
        rSecure.SetErrStatus(sSECURE_FLAG::ERR_NOERR);
        SecureManager::RemoveCryptingUser(&rSecure);
        SecureManager::AddCryptedUser(&rSecure);
        return true;
    case 2: //SUCCESS (BUILTIN WORK)
        WorkKeyBuiltin() = 0;
        rSecureFlag.QUERYBUILTIN.TransLeave();
        // �̰��� ���� ������ �˸� �޸��Ѵ�.
        //rSecure.SetErrStatus(sSECURE_FLAG::ERR_NOERR);
        return true;
    }

    return false;
}

//==================================================================================================
#if USING_HACKSHIELD

BOOL SecurePackNode4User::Query_HACKSHIELD_SERVER_EX_QUERY_CMD(
    User* pUser, sSECURE_FLAG::eQUERY_TRANS& eSTAT)
{
    // HACK SHIELD
    MSG_CG_HACKSHIELD_SERVER_EX_QUERY_CMD msgCMD;

    SecurePackNode4User& rSecure = pUser->GetSecureNode();

    DWORD dwRET = _AhnHS_MakeRequest(rSecure.HANDLEofClientHS(), &msgCMD.m_HShieldReqBlock);
    if(ERROR_SUCCESS == dwRET)
    {
        pUser->SendPacket(&msgCMD, sizeof(msgCMD));
    }
    else
    {
        MessageOut(eCRITICAL_LOG, _T("Can't Make SEED in HackShield [%u]"), dwRET);
        // ���� ���� ó���� �ؾ� �ϰ�����... ��ǻ� �����Ű�� ������ ����
    }
    //MessageOutEx(g_pSunLog, CSunLog::cFULL_LOG, CSunLog::cOUTPUT_FILE, CSunLog::cCATE_CONNECTION, _T("HackShield Query"));

    eSTAT = sSECURE_FLAG::TRANS_HACKSHIELD;
    SetErrStatus(sSECURE_FLAG::ERR_HACKSHIELD);
    return true;
}

BOOL SecurePackNode4User::OnRecvCG_HACKSHIELD_SERVER_EX_QUERY_ACK(
    User* pUser, MSG_CG_HACKSHIELD_SERVER_EX_QUERY_ACK* pMsgACK, WORD wSize)
{
    SecurePackNode4User& rSecure = pUser->GetSecureNode();
    BOOL bProcess = (BOOL)(rSecure.GetErrStatus() == sSECURE_FLAG::ERR_HACKSHIELD);
         bProcess = bProcess && (sizeof(*pMsgACK) == wSize);

    DWORD dwCurTick = GetTickCount();
    DWORD dwTimeout = ITimeout::INSTANCE;

    if(bProcess)
    {
        AHNHS_TRANS_BUFFER& rBuffer = pMsgACK->m_HShieldAnsBlock;
    #if HACKSHIELD_VERSION >= 647
        DWORD resultCode = 0;
        DWORD ret = _AhnHS_VerifyResponseEx(rSecure.HANDLEofClientHS(),
                                            rBuffer.byBuffer, rBuffer.nLength,
                                            &resultCode);
        if(ret == ANTICPX_RECOMMAND_KEEP_SESSION)
    #else
        const DWORD resultCode = _AhnHS_VerifyResponse(rSecure.HANDLEofClientHS(),
                                                       rBuffer.byBuffer, rBuffer.nLength);
        if(ERROR_SUCCESS == resultCode)
    #endif
        {
            SecureManager::RemoveCryptingUser(&rSecure);
            SecureManager::AddCryptedUser(&rSecure);

            rSecure.SetErrStatus(sSECURE_FLAG::ERR_NOERR);

            dwTimeout =
                RandomTimeUtil::GetPostQueryTimeout(RandomTimeUtil::SECURE_NEXT_CHECK_INTERVAL_POLE,
                                                    RandomTimeUtil::SECURE_NEXT_CHECK_INTERVAL_MINMAX_BOUNDARY);
            rSecure.secure_user_timeout_.SetNextTimeoutByCurTickBased(dwTimeout);

            //MessageOutEx(g_pSunLog, CSunLog::cFULL_LOG, CSunLog::cOUTPUT_FILE, CSunLog::cCATE_CONNECTION,
            //  _T("[GUID:%s] Success HS~~ Enabled, TimeInfo Cur[%u], Diff[%u], Next[%u]"),
            //  pUser->GetUserID(), dwCurTick, RandomTimeUtil::SECURE_NEXT_CHECK_INTERVAL_POLE, rSecure.TIMEOUT().TimeoutTick());

            return true;
        }

        // HackShield ���� ���� ���� ����
        switch(resultCode)
        {
        //	Client�� HackShield ��ġ�� ���� �ʰ� �����Ͽ��� ��쿡 �߻��� �� �ִ�.
        //	������ �Ϲ������� �������� ũ���� ���� ���ɼ��� ����� �ִ�. �ϴ� Abusing Point�� ���߰� ����� ����� ���ϱ�� �Ѵ�.
        case ERROR_ANTICPXSVR_UNKNOWN_CLIENT:
            rSecure.SetErrStatus(sSECURE_FLAG::ERR_HACKSHIELD_VERSION);
            break;
        default:
            rSecure.SetErrStatus(sSECURE_FLAG::ERR_HACKSHIELD_DETECT);
            break;
        }
        dwTimeout =
            RandomTimeUtil::GetPostQueryTimeoutOver(RandomTimeUtil::SECURE_FAIL_CLOSING_INTERVAL_POLE,
                                                    RandomTimeUtil::SECURE_FAIL_CLOSING_INTERVAL_MINMAX_BOUNDARY);
        rSecure.secure_user_timeout_.SetNextTimeoutByCurTickBased(dwTimeout);

        MessageOutEx(g_pSunLog,
                     CSunLog::cCRITICAL_LOG, CSunLog::cOUTPUT_FILE, CSunLog::cCATE_CONNECTION,
                     _T("Disconnect Process by Hackshield Query Failed - [ID:%s][GUID:%u] ErrCode [%08X]\n"),
                     pUser->GetUserID(), pUser->GetUserGUID(), resultCode);

        return false;
    }

    rSecure.SetErrStatus(sSECURE_FLAG::ERR_VIOLATION_FLOW_SEQ);

    // ���е� ���
    if((dwCurTick + 30 * RandomTimeUtil::SECOND) < rSecure.secure_user_timeout_.GetTimeoutTicks())
    {
        dwTimeout =
            RandomTimeUtil::GetPostQueryTimeoutOver(RandomTimeUtil::SECURE_FAIL_CLOSING_INTERVAL_POLE,
                                                    RandomTimeUtil::SECURE_FAIL_CLOSING_INTERVAL_MINMAX_BOUNDARY);
        rSecure.secure_user_timeout_.SetNextTimeoutByCurTickBased(dwTimeout);
    }

    return false;
}

#endif //USING_HACKSHIELD

//==================================================================================================

// ���� User� ������ ������ �������� �̵���Ű��.
VOID SecureManager::SetInformation(DWORD dwUpdateInterval, DWORD dwUserTimeout4ACK)
{
    //LList::Init(&sCryptedUserListEntry);
    //LList::Init(&sCryptingUserListEntry);
    //LList::Init(&sReleasingUserListEntry);

    FIELD.m_dwStatus = 0;
    FIELD.update_timer_.Stop();
    FIELD.m_dwUpdateInterval = dwUpdateInterval;
    FIELD.m_dwUserTimeout4ACK = dwUserTimeout4ACK;
}

// ������ �ֱ⸶��
// <INTERFACE><IMPLEMENTATION>
BOOL SecureManager::DoInit()
{
    FIELD.update_timer_.SetTimer(FIELD.m_dwUpdateInterval);

    return true;
}

//#include "UserManager.h"

BOOL SecureManager::DoProcess()
{
    if (FIELD.update_timer_.IsExpired() == false) {
        return true;
    };
    //
    // LOGIC...
    // <> ó���ؾ� �� ���� () ���õ� ó��
    //
    //   <1> ��ȣȭ�� ��Ŷ�� �ְ�޴� ���� ��Ͽ���, �����ϰ� ��Ŷ �ۼ����� �� �� �ִ� �������� �����Ѵ�.
    //      �ش� �������� Ű��ȯ ����̳� �Ƹ�Ŷ �۵����θ� �Ǻ��ϴ� CMD�� ������.
    //       ������ �ٸ���: �Ѹ��� ������ �����ؼ� Ű��ȯ�� ����������, ������ �������� ������� �ΰ��� Ÿ��
    //                   �� �����ؼ� CMD�� ������.
    //   (2) Ű��ȯ�̳� �Ƹ�Ŷ �۵����� �Ǻ� ��ɿ� ���� ACK�� ���Ź޾�, �̿� ���� Packet Handler�� ó����
    //      �����Ѵ�. �ش� ��ƾ�� DoProcess�� ���⿡ �������� �ʴ´�.
    //   <3> �ش� ����� �ް� ACK�� �޾ƾ� �ϴ� �������� ������� ���ѽð�(timeout)�� �˻��ϰ�, ���ѽð���
    //      ����� �ȴٸ�, ���� �����Ų��.
    //

    ProcessCryptedUsers();	// ���� m_dwUserTimeout4ACK
    ProcessCryptingUsers();

    return true;
}

BOOL SecureManager::IsComplete()
{
	return false;
}

BOOL SecureManager::DoRelease()
{
	return true;
}

VOID SecureManager::OnMsg(sSIG_MSG* pSIG_MSG)
{
    __UNUSED(pSIG_MSG);
}

//==================================================================================================
//


#define PROCESS_TEST_CODE	(0)



//==================================================================================================
//	���� ���� ó�� ��� ���� �� ����
//	���: ������ ��Ŷ��ȣȭ�� ���� �� ���� ���� ó�� ������ ���� ��� & !(ĳ���� ���ù�)
VOID SecureManager::ProcessCryptedUsers()
{
    SecurePackNode4User* pWorkNode = GetCryptedUserListEntry();
    if(util::LList::IsEmpty(pWorkNode))
        return;

    //////////////////////////////////////////////////////////////////////////
    // Used Auto Variable
    DWORD dwCurTick     = GetTickCount();       // <TIME INFO>
    __TOUCHED(dwCurTick);
    DWORD dwUserCount   = 0;                    // <USER COUNT>
    BOOL bProcess       = true;                 // ������ �ش� ���� ���� ���� ���� ����

    do {
        sSECURE_FLAG::eQUERY_TRANS eStat = sSECURE_FLAG::TRANS_NOTRANS; // ���� ��

        // ���� ���� ����
        pWorkNode = UTIL.GetNextNode(pWorkNode);
        SecurePackNode4User& rSecure = *pWorkNode;
        sSECURE_FLAG& rSecureFlag = rSecure.SECURE_FLAG();

        // ���� ������ ����Ѵٴ� ���� ���� ���� ó�� ������� �ǹ��Ѵ�
        if(!rSecureFlag.STATUS.IsTransaction() // �� �̵� ���� �ƴϰ�,
            && (sSECURE_FLAG::STAT_CRYPTED == rSecureFlag.STATUS.GetStatus()) // ���� ����̸�,
            && rSecure.secure_user_timeout_.IsExpired()) // ������ �ð��� �Ǿ��� ��,
        {
            // �� ����� User Object ���
            User* pUser = (User*)CONTAINING_RECORD(pWorkNode, User, SECUREINFO);

#if PROCESS_TEST_CODE
            MessageOutEx(g_pSunLog, CSunLog::cCRITICAL_LOG, CSunLog::cOUTPUT_BOTH, CSunLog::cCATE_CONNECTION,
                _T("��ȣŰ ��ȯ ����� [GUID:%u], CurrentTick : Timeout = %u : %u"), pUser->GetUserKey(), dwCurTick, rSecure.secure_user_timeout_.GetTimeoutTicks());

            // ó�� sSECURE_FLAG::STAT_CRYPTED �� ��ȯ�Ǿ��ٸ�, TimeoutTick�� INSTANCE������ ���̴�.
            if(ITimeout::INSTANCE == rSecure.secure_user_timeout_.GetTimeoutTicks())
            {
                bProcess = rSecure.Query_HACKSHIELD_SERVER_EX_QUERY_CMD(pUser, eStat);
                // ���� ó�� ������ ��      (������ ��ġ�� ���ؾ� �� ���̴�.)
                //if(!bProcess)
            }
            else
            {
                bProcess = ProcessQuery(pUser, rSecure, eStat);
            }
#endif
            bProcess = ProcessQuery(pUser, rSecure, eStat);

            if(bProcess)
            {
                // LIVE���, 180�� ���� +/- 30��
                // HACKSHIELD�� ��� 10�� �����̴�. ��� �������� 10�� �������� ����ΰ�, �̿� ������ �� �ֵ��� ����.
                //  (WAVERIX)(080707)(LOGIC-CHANGE)
                //  - [120, 140] return timeout (unit:second)
                if (rSecure.GetErrStatus() != sSECURE_FLAG::ERR_VIOLATION_FLOW_SEQ)
                {
                    DWORD dwTimeout = RandomTimeUtil::GetPostQueryTimeoutOver(
                        RandomTimeUtil::SECURE_KEYEXCHANGE_TIMEOUT_POLE,
                        RandomTimeUtil::SECURE_KEYEXCHANGE_TIMEOUT_MINMAX_BOUNDARY);
                    rSecure.secure_user_timeout_.SetNextTimeoutByCurTickBased(dwTimeout);
                }

                // ���� ��� ���·� ��ȯ ��, ���� ó���� ���� ����
                SecurePackNode4User* pTmpListNode = RemoveCryptedUser(pWorkNode);
                AddCryptingUser(pWorkNode, eStat);
                pWorkNode = pTmpListNode;   //��� ����ڸ� ó���Ϸ��� break������ ��.
            }

            //
            if(!(MAX_PROCESSING_USERS_IN_ONE_CYCLE > ++dwUserCount))
                break;
        }
    } while(GetCryptedUserListEntry() != pWorkNode->next);
}

BOOL SecureManager::ProcessQuery(User* pUser, SecurePackNode4User& rSecure,
                                 sSECURE_FLAG::eQUERY_TRANS& eSTAT)
{
    const DWORD cdwRandomNoUB   = 10;               // �����ѹ� �����ٿ��
    DWORD       dwRandomNo      = cdwRandomNoUB;    // �����ѹ�
    BOOL        bProcess        = false;

    // 0~9 ������ �����ѹ� ���ϱ�
    dwRandomNo = 4; // HackShield

    switch(dwRandomNo)
    {
    case 0:
        bProcess = rSecure.MOLAController().SendQuery(pUser, eSECURE_QUERY::BUILTIN_QUERY, NULL, 0);
        break;
    case 1:
    case 5:
    case 6:
    case 7:
        bProcess = rSecure.Query_CRYPTOGRAPHY_SNK_KEY(pUser, eSTAT);
        break;
    case 2:
        // Send Dummy Packet
        bProcess = false;
#   if PROCESS_TEST_CODE
        SUNLOG(eFULL_LOG, _T("SKIP"));
#   endif
        break;
    case 3:
        //bProcess = rSecure.Query_SIMPLEX_SNK_CMD(pUser, eSTAT);
        bProcess = false;
        break;
    case 4:
#if USING_HACKSHIELD
        bProcess = rSecure.Query_HACKSHIELD_SERVER_EX_QUERY_CMD(pUser, eSTAT);
#elif USING_XIGNCODE
        bProcess = rSecure.Query_XIGNCODE_SERVER_QUERY_CMD(pUser, eSTAT);
#elif USING_GAMEGUARD
        bProcess = rSecure.Query_GAMEGUARD_SERVER_QUERY_CMD(pUser, eSTAT);
#elif USING_FROST
#endif
        break;
    case 8:
        bProcess = false;
        break;
    case 9:
        bProcess = false;
        break;
    default:
        // Irregular
        SUNLOG(eCRITICAL_LOG, _T("Incorrect Number %u"), dwRandomNo);
        break;
    }

    return bProcess;
}

// �������� �������� ������� �������� �ʰ� �ִ� �������� ó���ϱ� ���� ����
//
VOID SecureManager::ProcessCryptingUsers()
{
    SecurePackNode4User* pWorkNode = GetCryptingUserListEntry();
    if(util::LList::IsEmpty(pWorkNode))
        return;

    DWORD dwMaxInterval = RandomTimeUtil::SECURE_KEYEXCHANGE_TIMEOUT_POLE+RandomTimeUtil::SECURE_KEYEXCHANGE_TIMEOUT_MINMAX_BOUNDARY;
    DWORD dwCurTick = GetTickCount();

    do {
        // ���� ���� ����
        pWorkNode = UTIL.GetNextNode(pWorkNode);
        SecurePackNode4User& rSecure = *pWorkNode;
        sSECURE_FLAG& rSecureFlag = rSecure.SECURE_FLAG();

        DWORD rUserTimeout = rSecure.secure_user_timeout_.GetTimeoutTicks();

        // ���Ľɿ���... ������ ����� �ڵ�
        if(dwCurTick < rUserTimeout)
        {
            DWORD dwDiff = (rUserTimeout - dwCurTick);
            if(dwDiff > dwMaxInterval)
            {
                MessageOut(eCRITICAL_LOG, _T("CProcessing Time Value %u"), (dwDiff));
                rSecure.secure_user_timeout_.SetNextTimeoutByCurTickBased(dwMaxInterval);
            }
        }

        // ���� ó�� ���, �� Ÿ�Ӿƿ��� �ɸ� ����.
        if(!(rSecureFlag.STATUS.IsTransaction() || (dwCurTick<rUserTimeout)))
        {
            User* pUser = (User*)CONTAINING_RECORD(pWorkNode, User, SECUREINFO);
            const sSECURE_FLAG::eQUERY_ERRCTRL eErrStat = rSecure.GetErrStatus();

            //
            RC::eCONNECTION_RESULT eResult;
            TCHAR* pErrMsg;

            // ���� ���� �߰��� ��
            switch(eErrStat)
            {
            case sSECURE_FLAG::ERR_SIMPLEX_SNK:     //sSECURE_CONTROL_STATUS::CRYPTFAIL_EX1:
                eResult = RC::RC_DISCONNECT_CRYPTOGRAPHY_INCORRECT_SNKEY;
                pErrMsg = _T("Disconnect User : Cracked Client #1");
                break;
            case sSECURE_FLAG::ERR_DEFAULT_TRANS:   //sSECURE_CONTROL_STATUS::CRYPTFAIL_EX2:
                eResult = RC::RC_DISCONNECT_CRYPTOGRAPHY_INCORRECT_SNKEY;
                pErrMsg = _T("Disconnect User : Cracked Client #2");
                break;
            case sSECURE_FLAG::ERR_CORRUPTED_PACKET:// sSECURE_CONTROL_STATUS::RECEIVED_CRACKED_PACKET:
                eResult = RC::RC_DISCONNECT_CRYPTOGRAPHY_INCORRECT_SNKEY;
                pErrMsg = _T("Disconnect User : Cracked Client #3");
                break;
            case sSECURE_FLAG::ERR_DUPLEX_SNK:      //sSECURE_CONTROL_STATUS::CRYPTFAIL
                eResult = RC::RC_DISCONNECT_CRYPTOGRAPHY_KEYEXCHANGE_NAK;
                pErrMsg = _T("Disconnect User : Cracked Client #4 - CryptSeq Violation");
                break;
            case sSECURE_FLAG::ERR_HACKSHIELD:
                eResult = RC::RC_DISCONNECT_ARMORKIT_NOT_SERVICED;
                pErrMsg = _T("Disconnect User : Cracked Client #5 - [HackShield] Disabled");
                break;
            case sSECURE_FLAG::ERR_HACKSHIELD_DETECT:
                eResult = RC::RC_DISCONNECT_ARMORKIT_NOT_SERVICED;
                pErrMsg = _T("Disconnect User : Cracked Client #6 - [HackShield] Corrupted Data");
                break;
            case sSECURE_FLAG::ERR_ARMORKIT:
                eResult = RC::RC_DISCONNECT_ARMORKIT_NOT_SERVICED;
                pErrMsg = _T("Disconnect User by ArmorKit Disabled Status");
                break;
            case sSECURE_FLAG::ERR_VIOLATION_FLOW_SEQ:
                eResult = RC::RC_DISCONNECT_ARMORKIT_NOT_SERVICED;
                pErrMsg = _T("Disconnect User by Message Logic Flow Violation");
                break;
            case sSECURE_FLAG::ERR_HACKSHIELD_VERSION:
                eResult = RC::RC_DISCONNECT_ARMORKIT_NOT_SERVICED;
                pErrMsg = _T("Disconnect User : Cracked Client #6 - [HackShield] (Non-Client Bot) or (Not Patched Client Version)");
                break;
            case sSECURE_FLAG::ERR_FORCEDCLOSING:
                eResult = RC::RC_DISCONNECT_ARMORKIT_NOT_SERVICED;
                pErrMsg = _T("Disconnect User [Forced or Block]");
                break;
            case sSECURE_FLAG::ERR_GAMEGUARD_QUERY:
                if (rSecureFlag.STATUS.GetStatus() == sSECURE_FLAG::STAT_CRYPTING)
                {
                    //Ŭ���̾�Ʈ�κ��� �������� ������ ���� ������ ���
                    //�������� ���û�� ���� ó��
                    RemoveCryptingUser(&rSecure);
                    AddCryptedUser(&rSecure);
                }
                return;
            default:
                eResult = RC::RC_DISCONNECT_CRYPTOGRAPHY_KEYEXCHANGE_TIMEOUT;
                pErrMsg = _T("Disconnect User by Crypting Sequence - Timeout Violation %d");
                break;
            };

            // �ش� ������ Disconnect
            pUser->SetDisconnectCode(eResult);
            pUser->DisconnectUser(true);

            DWORD dwTimeoutTick = dwCurTick - rUserTimeout;

            switch(eErrStat)
            {
            case sSECURE_FLAG::ERR_HACKSHIELD:
            case sSECURE_FLAG::ERR_HACKSHIELD_DETECT:
                SECURELOG(SECURELOG_USERINFO(pUser, eABUSE_LOGIC_VIOLATION, eABUSE_SUB_LOGIC_VIOLATION_HACKSHIELD_DISCONNECT), pErrMsg, dwTimeoutTick);
                break;
            case sSECURE_FLAG::ERR_HACKSHIELD_VERSION:
                SECURELOG(SECURELOG_USERINFO(pUser, eABUSE_LOGIC_VIOLATION, eABUSE_SUB_LOGIC_VIOLATION_HACKSHIELD_INVALID_STATUS_DISCONNECT), pErrMsg, dwTimeoutTick);
                break;
            case sSECURE_FLAG::ERR_FORCEDCLOSING:
                break;
            default:
                SECURELOG(SECURELOG_USERINFO(pUser, eABUSE_LOGIC_VIOLATION, eABUSE_SUB_LOGIC_VIOLATION_SECURITY_QUERY_DISCONNECT), pErrMsg, dwTimeoutTick);
                break;
            }
            ////////////////////////////////////////////////////////////////////////////////////
            // List ����
            pWorkNode = RemoveCryptingUser(pWorkNode);
            pWorkNode = UTIL.GetPrevNode(pWorkNode);
            ////////////////////////////////////////////////////////////////////////////////////
        }
    } while(GetCryptingUserListEntry() != pWorkNode->next);
}


//==================================================================================================
//==================================================================================================
//==================================================================================================


//==================================================================================================
//  <RandomTimeUtil>
//
//  <SHARED MACROs>
#define _RNDTMFN_F0(x,y,z)  ((x & y) | (z & (x | y)))
#define _RNDTMFN_F1(x,y,z)  (z ^ (x & (y ^ z)))
#define _RNDTMFN_F2(x,y,z)  ((x) ^ (y) ^ (z))

//  <<<32BIT OPERATORs>>>
#define _RNDTMFN32_SHR(x,n)     ((x & 0xFFFFFFFF) >> n)
#define _RNDTMFN32_SHL(x,n)     (x << (32 - n))
#define _RNDTMFN32_SWAP(x,n)    (_RNDTMFN32_SHR(x,n) | _RNDTMFN32_SHL(x,n))

#define RNDTM_SHA32_T1(x) (_RNDTMFN32_SWAP(x, 7) ^ _RNDTMFN32_SWAP(x,18) ^ _RNDTMFN32_SHR(x, 3))
#define RNDTM_SHA32_T2(x) (_RNDTMFN32_SWAP(x,17) ^ _RNDTMFN32_SWAP(x,19) ^ _RNDTMFN32_SHR(x,10))

#define RNDTM_COMPLEX_CALC(a,b,c,d) \
    (((a) + RNDTM_SHA32_T1(b) + RNDTM_SHA32_T2(c)) + (_RNDTMFN32_SWAP(d, 2) + b + c))

//  <���� ���� ��� �ð� ����>
//      LIVE��忡�� { ���� Timeout 180sec } 
//      USE : GetRandomTimeDelay_ByMinMaxBoundary
//      RANGE: { 180sec +/- 30sec }
DWORD   RandomTimeUtil::SECURE_KEYEXCHANGE_TIMEOUT_POLE                 = 80*RandomTimeUtil::SECOND;
DWORD   RandomTimeUtil::SECURE_KEYEXCHANGE_TIMEOUT_MINMAX_BOUNDARY      = 30*RandomTimeUtil::SECOND;

//  <���� ���� ����� �Ǵ� �ð� ����>
//      USE : GetRandomTimeDelay
//      RANGE: { MIN 5 ~ MAX 10 } Ȯ�������� 5~7�� ���̰� ����.
DWORD   RandomTimeUtil::SECURE_NEXT_CHECK_INTERVAL_POLE                 = 6*RandomTimeUtil::MINUTE;
DWORD   RandomTimeUtil::SECURE_NEXT_CHECK_INTERVAL_MINMAX_BOUNDARY      = 1*RandomTimeUtil::MINUTE;

//  <���� ��� ���� ������>
//      USE : GetRandomTimeDelay
//      RANGE: { MIN 13 ~ MAX 26 } Ȯ�������� 13~20�� ���̰� ����.
DWORD   RandomTimeUtil::SECURE_FAIL_CLOSING_INTERVAL_POLE               = 20*RandomTimeUtil::SECOND;
DWORD   RandomTimeUtil::SECURE_FAIL_CLOSING_INTERVAL_MINMAX_BOUNDARY    = 13*RandomTimeUtil::SECOND;

//
const DWORD     cdwRndValue = 0x10337F32;
static DWORD    s_dwRndValueBase = RNDTM_SHA32_T1(cdwRndValue);
static DWORD    s_dwRndValueOut = RNDTM_SHA32_T2(s_dwRndValueBase);
static DWORD    sdwRNDARRAY[4] = { GetTickCount(), 0x90442381, 0x01913384, 0x1F004097 };

DWORD RandomTimeUtil::GetPostQueryTimeout(DWORD dwFlagTick, DWORD dwMinMax /* {+/-} dword */)
{
    //ITimeout sFlagTick;

    if(!(dwFlagTick < dwMinMax))
    {
        s_dwRndValueBase = RNDTM_SHA32_T1(s_dwRndValueBase) + s_dwRndValueOut;
        s_dwRndValueOut = RNDTM_SHA32_T2(s_dwRndValueOut) + s_dwRndValueBase;

        INT iDiff = (INT)(s_dwRndValueOut%(dwMinMax<<1));
        if(dwMinMax > (DWORD)iDiff)
            dwFlagTick -= (DWORD)(dwMinMax-iDiff);
        else
            dwFlagTick += (DWORD)(iDiff-dwMinMax);
    }
    //sFlagTick.SetNextTimeoutInterval(dwFlagTick);

    //return sFlagTick.GetTimeoutTicks();
    return dwFlagTick;
}

//      RANGE: { dwFlagTick ~ dwFlagTick+dwMax }
DWORD RandomTimeUtil::GetPostQueryTimeoutOver(DWORD dwFlagTick, DWORD dwMax /* {+} dword */)
{
    //ITimeout sFlagTick;

    s_dwRndValueBase = RNDTM_SHA32_T1(s_dwRndValueBase) + s_dwRndValueOut;
    s_dwRndValueOut = RNDTM_SHA32_T2(s_dwRndValueOut) + s_dwRndValueBase;

    DWORD dwRandom = s_dwRndValueOut%dwMax;
    dwFlagTick += dwRandom;

    //sFlagTick.SetNextTimeoutInterval(dwFlagTick);

    //return sFlagTick.GetTimeoutTicks();
    return dwFlagTick;
}

DWORD RandomTimeUtil::GetRandomNumber()
{
    DWORD dwRndValueTick = RNDTM_SHA32_T1(GetTickCount());

    sdwRNDARRAY[0] = RNDTM_COMPLEX_CALC(dwRndValueTick, sdwRNDARRAY[0], sdwRNDARRAY[1], sdwRNDARRAY[2]);
    DWORD dwRndTmp = sdwRNDARRAY[0];
    sdwRNDARRAY[0] = sdwRNDARRAY[1];
    sdwRNDARRAY[1] = sdwRNDARRAY[2];
    sdwRNDARRAY[2] = sdwRNDARRAY[3];
    sdwRNDARRAY[3] = dwRndTmp;
    //sdwRNDARRAY[1] = RNDTM_COMPLEX_CALC(sdwRNDARRAY[1], sdwRNDARRAY[2], dwRndValue2, sdwRNDARRAY[3]);
    //sdwRNDARRAY[2] = RNDTM_COMPLEX_CALC(s_dwRndValueBase, sdwRNDARRAY[1], sdwRNDARRAY[3], sdwRNDARRAY[2]);
    //sdwRNDARRAY[3] = RNDTM_COMPLEX_CALC(sdwRNDARRAY[3], sdwRNDARRAY[2], sdwRNDARRAY[1], sdwRNDARRAY[0]);
    s_dwRndValueOut = RNDTM_COMPLEX_CALC(sdwRNDARRAY[2], sdwRNDARRAY[3], s_dwRndValueBase, dwRndValueTick);

    return s_dwRndValueOut;
}

DWORD RandomTimeUtil::GetRandomNumber(DWORD dwMin, DWORD dwUpperBound)
{
    //DWORD dwRandomValue = GetRandomNumber();

    if(dwMin > dwUpperBound)
        (dwMin) ^= (dwUpperBound) ^= (dwMin) ^= (dwUpperBound);

    DWORD dwDiff = dwUpperBound - dwMin;
    if(dwDiff)
        dwDiff = s_dwRndValueBase % dwDiff;

    return dwMin + dwDiff;
}



//==================================================================================================
//  <Timeout Control for ServerMode>
//  Ű ��ȯ ���� : �ݵ�� �������� �Ǿ� �����Ƿ�...
//
enum eCRYPTOGRAPHY_KEY_CHANGE_TIMEOUT
{
    //////////////////////////////////////////////////////////////////////////
    //  ���� ��庰 cUSER_KEYCHANGE_TIMEOUT �缳����
    CRYPTO_KEYCHANGE_TIMEOUT_MODE_DEV               = 999*RandomTimeUtil::MINUTE,
    CRYPTO_KEYCHANGE_TIMEOUT_MODE_INTRAOFFICE       = 999*RandomTimeUtil::MINUTE,
    CRYPTO_KEYCHANGE_TIMEOUT_MODE_LIVE              = 120*RandomTimeUtil::SECOND,
    CRYPTO_KEYCHANGE_TIMEOUT_MODE_TESTLIVE          = 120*RandomTimeUtil::SECOND,

    SECURE_MANAGER_UPDATE_INTERVAL_MODE_DEV         = 999*RandomTimeUtil::MINUTE,
    SECURE_MANAGER_UPDATE_INTERVAL_MODE_INTRAOFFICE = 999*RandomTimeUtil::MINUTE,
    SECURE_MANAGER_UPDATE_INTERVAL_MODE_LIVE        =  11*RandomTimeUtil::SECOND,
    SECURE_MANAGER_UPDATE_INTERVAL_MODE_TESTLIVE    =  11*RandomTimeUtil::SECOND,
};


//  <�⺻��>   SecureManager Update �ֱ�
DWORD RandomTimeUtil::cSECURE_MANAGER_UPDATE_INTERVAL = SECURE_MANAGER_UPDATE_INTERVAL_MODE_LIVE;
//  <�⺻��>   ���� ���� ���� ���� ��� �ð�
DWORD RandomTimeUtil::cUSER_KEYCHANGE_TIMEOUT         = CRYPTO_KEYCHANGE_TIMEOUT_MODE_LIVE;

VOID RandomTimeUtil::Initialize(DWORD dwSERVER_MODE)
{
    cUSER_KEYCHANGE_TIMEOUT
        = (eSERVER_DEV == dwSERVER_MODE) ?          CRYPTO_KEYCHANGE_TIMEOUT_MODE_DEV
        : (eSERVER_INTRAOFFICE == dwSERVER_MODE) ?  CRYPTO_KEYCHANGE_TIMEOUT_MODE_INTRAOFFICE
        : (eSERVER_LIVE == dwSERVER_MODE) ?         CRYPTO_KEYCHANGE_TIMEOUT_MODE_LIVE
        : (eSERVER_TESTLIVE == dwSERVER_MODE) ?     CRYPTO_KEYCHANGE_TIMEOUT_MODE_TESTLIVE
        :                                           CRYPTO_KEYCHANGE_TIMEOUT_MODE_LIVE
        ;

    SECURE_KEYEXCHANGE_TIMEOUT_POLE = cUSER_KEYCHANGE_TIMEOUT;
    SECURE_KEYEXCHANGE_TIMEOUT_MINMAX_BOUNDARY
        = ((1*RandomTimeUtil::MINUTE) > SECURE_KEYEXCHANGE_TIMEOUT_POLE)
        ?    5*RandomTimeUtil::SECOND
        :   20*RandomTimeUtil::SECOND
        ;

    cSECURE_MANAGER_UPDATE_INTERVAL
        = (eSERVER_DEV == dwSERVER_MODE) ?          SECURE_MANAGER_UPDATE_INTERVAL_MODE_DEV
        : (eSERVER_INTRAOFFICE == dwSERVER_MODE) ?  SECURE_MANAGER_UPDATE_INTERVAL_MODE_INTRAOFFICE
        : (eSERVER_LIVE == dwSERVER_MODE) ?         SECURE_MANAGER_UPDATE_INTERVAL_MODE_LIVE
        : (eSERVER_TESTLIVE == dwSERVER_MODE) ?     SECURE_MANAGER_UPDATE_INTERVAL_MODE_TESTLIVE
        :                                           SECURE_MANAGER_UPDATE_INTERVAL_MODE_LIVE
        ;

#if USING_GAMEGUARD
    SECURE_NEXT_CHECK_INTERVAL_POLE                 =  4  * RandomTimeUtil::MINUTE;
    SECURE_NEXT_CHECK_INTERVAL_MINMAX_BOUNDARY      =  30 * RandomTimeUtil::SECOND;

    SECURE_FAIL_CLOSING_INTERVAL_POLE               =  10 * RandomTimeUtil::SECOND;
    SECURE_FAIL_CLOSING_INTERVAL_MINMAX_BOUNDARY    =  1  * RandomTimeUtil::SECOND;
#else
    SECURE_NEXT_CHECK_INTERVAL_POLE                 =  70*RandomTimeUtil::SECOND;
    SECURE_NEXT_CHECK_INTERVAL_MINMAX_BOUNDARY      =  20*RandomTimeUtil::SECOND;

    SECURE_FAIL_CLOSING_INTERVAL_POLE               =  20*RandomTimeUtil::SECOND;
    SECURE_FAIL_CLOSING_INTERVAL_MINMAX_BOUNDARY    =  15*RandomTimeUtil::SECOND;
#endif

    
}

