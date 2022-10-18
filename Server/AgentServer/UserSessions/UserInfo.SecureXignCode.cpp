#include "stdafx.h"
#include "UserInfo.Secure.h"

#if USING_XIGNCODE

#include <XignCode/IncludeListForXignCode.h>
#include "./UserInfo.SecureXignCode_impl.hxx"
//==================================================================================================
#define XIGNCODE_FLOWTEST   (0)
#define SUNLOG_XIGNCODE MessageSecureModuleOut

namespace ns_secure {
;

bool ns_secure::XignCode_Initilize()
{
    TCHAR module_path[MAX_PATH] = { 0, };
    DWORD written = GetCurrentDirectory(_countof(module_path), module_path);
    _sntprintf(&module_path[written], _countof(module_path) - written,
               _T("\\XignCode\\%d\\"), XIGNCODE_VERSION);
    module_path[_countof(module_path) - 1] = _T('\0');
    BOOL result = ZSWAVE_Initialize(module_path);
    if (result == false) {
        SUNLOG(eCRITICAL_LOG, _T("[XignCode]|Initalize failed|ErrorCode=%u|"), ::GetLastError());
    }
    return (result != false);
}

bool ns_secure::XignCode_Uninitialize()
{
    BOOL result = ZSWAVE_Uninitialize();
    return (result != false);
}

//==================================================================================================

struct XignCodeQueryUtil
{
    XignCodeQueryUtil(XignCode* xigncode)
        : xigncode_(xigncode)
    {
    };
    //
    bool RegenerateHandle() const
    {
        switch (xigncode_->code_type)
        {
        case Z_SCAN_TYPE_ANTI_CHEAT_ENGINE:
        case Z_SCAN_TYPE_ANTI_NONECLIENTBOT:
            break;
        default:
            xigncode_->code_type = Z_SCAN_TYPE_ANTI_NONECLIENTBOT;
            ASSERT(!"check");
            break;
        };

        xigncode_->handle = ZSWAVE_Open(XIGNCODE_GAME_PACKET_SIZE, xigncode_->code_type);
        return (xigncode_->handle != NULL);
    };

    bool CheckFullPacket() const
    {
        bool reverse_result = (xigncode_->code_type == Z_SCAN_TYPE_ANTI_CHEAT_ENGINE) &&
                              (xigncode_->completed_anti_cheat_code != false);
        return reverse_result == false;
    };

    void ToggleSendCodeType() const
    {
        if (((xigncode_->number_of_send_packets + 1) % ns_secure::kXignCodePacketCycle) == 0)
        {
            xigncode_->code_type = Z_SCAN_TYPE_ANTI_NONECLIENTBOT;
        }
        else
        {
            xigncode_->code_type = Z_SCAN_TYPE_ANTI_CHEAT_ENGINE;
        }
    };
    //
    XignCode* const xigncode_;
    //
    __DISABLE_COPY(XignCodeQueryUtil);
};

struct XignCodeResultClosure
{
    XignCodeResultClosure(XignCode* xigncode)
        : xigncode_(xigncode)
    {
    };

    ~XignCodeResultClosure()
    {
        ++xigncode_->number_of_send_packets;
        ZSWAVE_Close(xigncode_->handle);
        xigncode_->handle = NULL;
    }
    //
    XignCode* const xigncode_;
    //
    __DISABLE_COPY(XignCodeResultClosure);
};

};
//==================================================================================================
//==================================================================================================

bool SecurePackNode4User::XignCode_Init()
{
    ZeroMemory(&xigncode_, sizeof(xigncode_));
    xigncode_.code_type = Z_SCAN_TYPE_ANTI_NONECLIENTBOT;
    xigncode_.handle = NULL; // INVALID_HANDLE_VALUE;
    return true;
}

bool SecurePackNode4User::XignCode_Release()
{
    if (xigncode_.handle != NULL) {
        ZSWAVE_Close(xigncode_.handle);
    }
    ZeroMemory(&xigncode_, sizeof(xigncode_));
    return true;
}

BOOL SecurePackNode4User::Query_XIGNCODE_SERVER_QUERY_CMD(
    User* user, sSECURE_FLAG::eQUERY_TRANS& eSTAT)
{
    ns_secure::XignCodeQueryUtil query_util(&xigncode_);

    //----------------------------------------------------------------------------------------------
    if (query_util.RegenerateHandle() == false)
    {
        SUNLOG(eCRITICAL_LOG, _T("[XignCode Query Failed]|CodeBox=%08X|"), ::GetLastError());
        eSTAT = sSECURE_FLAG::TRANS_HACKSHIELD;
        SetErrStatus(sSECURE_FLAG::ERR_HACKSHIELD);

        return false; // <---
    }
    MSG_CG_XIGNCODE_SERVER_QUERY_CMD msg_cmd;
#if XIGNCODE_FLOWTEST
    const int number_of_tokens = 1; //ZSWAVE_GetDataCount(xigncode_.handle);
#else
    const int number_of_tokens = ZSWAVE_GetDataCount(xigncode_.handle);
#endif
    bool loop_continue = true;
    for (int number = 0; (number < number_of_tokens) && (loop_continue); ++number)
    {
        ZeroMemory(msg_cmd.buffer, sizeof(msg_cmd.buffer));
    #if XIGNCODE_FLOWTEST == 0
        if (ZSWAVE_GetDataAt(xigncode_.handle, number, reinterpret_cast<char*>(msg_cmd.buffer)))
    #endif
        {
            user->SendPacket(&msg_cmd, sizeof(msg_cmd));
            loop_continue = query_util.CheckFullPacket();
        };
    };
    if (xigncode_.code_type == Z_SCAN_TYPE_ANTI_CHEAT_ENGINE) {
        xigncode_.completed_anti_cheat_code = true;
    }
    query_util.ToggleSendCodeType();

    eSTAT = sSECURE_FLAG::TRANS_HACKSHIELD;
    SetErrStatus(sSECURE_FLAG::ERR_HACKSHIELD);

    return true;
}

BOOL SecurePackNode4User::OnRecvXIGNCODE_SERVER_QUERY_ACK(User* user,
    MSG_CG_XIGNCODE_SERVER_QUERY_ACK* msg_ack, WORD msg_size)
{
    ns_secure::XignCodeResultClosure auto_closure(&xigncode_);

    const DWORD cur_tick = GetTickCount();
    DWORD timeout = ITimeout::INSTANCE;

    bool success = (this->GetErrStatus() == sSECURE_FLAG::ERR_HACKSHIELD);
    ;    success = success && (sizeof(*msg_ack) == msg_size);
    //
    if (success == false)
    {
        this->SetErrStatus(sSECURE_FLAG::ERR_VIOLATION_FLOW_SEQ);
        // 실패한 경우
        if ((cur_tick + 30 * RandomTimeUtil::SECOND) < secure_user_timeout_.GetTimeoutTicks())
        {
            timeout = RandomTimeUtil::GetPostQueryTimeoutOver(
                RandomTimeUtil::SECURE_FAIL_CLOSING_INTERVAL_POLE,
                RandomTimeUtil::SECURE_FAIL_CLOSING_INTERVAL_MINMAX_BOUNDARY);
            secure_user_timeout_.SetNextTimeoutByCurTickBased(timeout);
        }
        return false;
    };
#if XIGNCODE_FLOWTEST
    success = true;
#else
    success = ZSWAVE_Verify(xigncode_.handle,
        reinterpret_cast<CPACKETDATA>(msg_ack->buffer), sizeof(msg_ack->buffer)) != 0;
#endif
    if (success)
    {
        SecureManager::RemoveCryptingUser(this);
        SecureManager::AddCryptedUser(this);
        this->SetErrStatus(sSECURE_FLAG::ERR_NOERR);
        timeout = RandomTimeUtil::GetPostQueryTimeout(
            RandomTimeUtil::SECURE_NEXT_CHECK_INTERVAL_POLE,
            RandomTimeUtil::SECURE_NEXT_CHECK_INTERVAL_MINMAX_BOUNDARY);
        secure_user_timeout_.SetNextTimeoutByCurTickBased(timeout);
        return true;
    };
    //
    assert(success == false);
    TCHAR error_msg_buffer[sizeof(msg_ack->buffer)] = { 0, };
    //
    const DWORD user_guid = user->GetUserKey();
    //
    int result_code = ZSWAVE_VerifyEx(xigncode_.handle,
        reinterpret_cast<char*>(msg_ack->buffer), sizeof(msg_ack->buffer),
        error_msg_buffer, _countof(error_msg_buffer)); // sizeof? countof?...
    error_msg_buffer[_countof(error_msg_buffer) - 1] = _T('\0');
    //
    bool need_disconnect = false;
    switch (result_code)
    {
    case Z_RTN_NONCLIENT:
        SUNLOG_XIGNCODE(eCRITICAL_LOG, "|CharGuid=%8u|Type=NonClient|\t%s",
                        user_guid, error_msg_buffer);
        need_disconnect = true;
        break;
	case Z_RTN_BLACK_CODE:
        SUNLOG_XIGNCODE(eCRITICAL_LOG, "|CharGuid=%8u|Type=BlackCode|\t%s",
                        user_guid, error_msg_buffer);
        need_disconnect = true;
        break;
	case Z_RTN_SUSPICIOUS:
        SUNLOG_XIGNCODE(eCRITICAL_LOG, "|CharGuid=%8u|Type=Suspicious|\t%s",
                        user_guid, error_msg_buffer);
        break;
	case Z_RTN_USERDEFINED:
        SUNLOG_XIGNCODE(eCRITICAL_LOG, "|CharGuid=%8u|Type=UserDefined|\t%s",
                        user_guid, error_msg_buffer);
        break;
	case Z_RTN_RESEND:
        SUNLOG_XIGNCODE(eCRITICAL_LOG, "|CharGuid=%8u|Type=Resend|",
                        user_guid, error_msg_buffer);
        xigncode_.completed_anti_cheat_code = false;
        break;
    case Z_RTN_NONE:
        SUNLOG_XIGNCODE(eDEV_LOG, "|CharGuid=%8u|Type=No problem|",
                        user_guid);
        break;
    default:
        SUNLOG_XIGNCODE(eCRITICAL_LOG, "|CharGuid=%8u|Type=logical error(%d)|",
                        user_guid, result_code);
        need_disconnect = true; // ...
        break;
    };

    if (need_disconnect)
    {
        this->SetErrStatus(sSECURE_FLAG::ERR_HACKSHIELD_DETECT);
        timeout = RandomTimeUtil::GetPostQueryTimeoutOver(
            RandomTimeUtil::SECURE_FAIL_CLOSING_INTERVAL_POLE,
            RandomTimeUtil::SECURE_FAIL_CLOSING_INTERVAL_MINMAX_BOUNDARY);
        secure_user_timeout_.SetNextTimeoutByCurTickBased(timeout);
        return false;
    }
    else
    {
        SecureManager::RemoveCryptingUser(this);
        SecureManager::AddCryptedUser(this);
        this->SetErrStatus(sSECURE_FLAG::ERR_NOERR);
        timeout = RandomTimeUtil::GetPostQueryTimeout(
            RandomTimeUtil::SECURE_NEXT_CHECK_INTERVAL_POLE,
            RandomTimeUtil::SECURE_NEXT_CHECK_INTERVAL_MINMAX_BOUNDARY);
        secure_user_timeout_.SetNextTimeoutByCurTickBased(timeout);
        return true;
    }
    //
    //return false;
}

//==================================================================================================

#endif //USING_XIGNCODE

