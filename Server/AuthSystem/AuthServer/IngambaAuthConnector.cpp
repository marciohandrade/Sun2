#include "stdafx.h"
#ifdef _RU_INGAMBA_AUTH_PROCESS
#include "IngambaAuthConnector.h"

using namespace Ingamba;
using namespace PaidServicesIntegration;
using namespace Wcf;
using namespace NativeClient::Dto;

IngambaAuthConnector::IngambaAuthConnector():inatance_module_(NULL)
                                            ,interface_function_(NULL)
                                            ,service_client_(NULL)
{
    _Initialize();
}

IngambaAuthConnector::~IngambaAuthConnector()
{
    _Realease();
}

RC::PrimiumServiceResult
IngambaAuthConnector::StartPrimiumService(const AccountInfo& account_info)
{
    if (service_client_ == NULL)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            __FUNCTION__" : Not connected ingamba billing server"
            );
        return RC::RC_PRIMIUM_NOT_CONNECTED_BILLINGSERVER;
    }

    GetUserInfoRequest get_user_info_request;
    IGetUserInfoResponse* get_user_info_response = NULL;
    get_user_info_request.UserId = account_info.m_userGuid;

    service_client_->GetUserInfo(get_user_info_request, &get_user_info_response);
    if (get_user_info_response == NULL)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            __FUNCTION__" : Fail get user info : account : %s, user guid : %d",
            account_info.m_account,
            account_info.m_userGuid
            );
        get_user_info_response->Free();
        get_user_info_response = NULL;
        return RC::RC_PRIMIUM_FAIL_GET_USER_INFO;
    }

    if (get_user_info_response->StatusInUse != true)
    {
        SUNLOG(
            eCRITICAL_LOG,
            __FUNCTION__" : Status in use is false : account : %s, user guid : %d",
            account_info.m_account,
            account_info.m_userGuid
            );
        return RC::RC_PRIMIUM_NOT_SUPPORT_USER;
    }

    if (get_user_info_response->StatusId != WeeklyPrimiumService &&
        get_user_info_response->StatusId != MonthlyPrimiumService)
    {
        SUNLOG(
            eCRITICAL_LOG,
            __FUNCTION__" : Incorrect status id : id : %d, account : %s, user guid : %d",
            get_user_info_response->StatusId,
            account_info.m_account,
            account_info.m_userGuid
            );
        return RC::RC_PRIMIUM_INVALID_STATUS_USER;
    }

    StartUsingStatusRequest request_info;
    IStartUsingStatusResponse* response_info = NULL;

    wchar_t client_ip[IP_MAX_LEN] = {0,};
    size_t recv_client_ip_len = strlen(account_info.m_clientIp);
    MultiByteToWideChar(CP_ACP, 
                        0, 
                        account_info.m_clientIp, 
                        recv_client_ip_len, 
                        client_ip, 
                        _countof(client_ip));
    client_ip[_countof(client_ip) - 1] = L'\0';
    
    request_info.UserId = account_info.m_userGuid;
    request_info.NotificationTimeout = -1;
    request_info.UserIp = client_ip;

    service_client_->StartUsingStatus(request_info, &response_info);
    if (response_info == NULL)
    {
        SUNLOG(
            eCRITICAL_LOG,
            __FUNCTION__" : start using status response data is null : account : %s, user guid : %d",
            account_info.m_account,
            account_info.m_userGuid
            );
        return RC::RC_PRIMIUM_RESPONSE_DATA_IS_NULL;
    }

    if (response_info->Result != ::Ok)
    {
        SUNLOG(
            eCRITICAL_LOG,
            __FUNCTION__" : Fail start using status : message : %ls, code : %d, account : %s, user guid : %d",
            response_info->ErrorMessage,
            response_info->Result,
            account_info.m_account,
            account_info.m_userGuid
            );
        return RC::RC_PRIMIUM_FAIL_START_USING_STATUS;
    }

    if (response_info->StatusRemainingTime <= 0)
    {
        SUNLOG(
            eCRITICAL_LOG,
            __FUNCTION__" : Not remain time for primium service  : account : %s, user guid : %d",
            account_info.m_account,
            account_info.m_userGuid
            );
        return RC::RC_PRIMIUM_NOT_REMAIN_TIME;
    }

    return RC::RC_PRIMIUM_SUCCESS;
}

RC::PrimiumServiceResult
IngambaAuthConnector::IsKeepingPrimiumService(const AccountInfo& account_info,
                                              int& OUT check_time,
                                              int& OUT remaining_time)
{
    SUNLOG(eCRITICAL_LOG,
        "Excute keep using status : account = %s, user guid = %d",
        account_info.m_account,
        account_info.m_userGuid
        );
    if (service_client_ == NULL)
    {
        SUNLOG(
            eCRITICAL_LOG,
            __FUNCTION__" : Not connected ingamba billing server"
            );
        return RC::RC_PRIMIUM_NOT_CONNECTED_BILLINGSERVER;
    }

    KeepUsingStatusRequest request_info;
    IKeepUsingStatusResponse* response_info;

    wchar_t client_ip[IP_MAX_LEN] = {0,};
    size_t recv_client_ip_len = strlen(account_info.m_clientIp);
    MultiByteToWideChar(
        CP_ACP,
        0,
        account_info.m_clientIp,
        recv_client_ip_len,
        client_ip,
        _countof(client_ip)
        );
    client_ip[_countof(client_ip) - 1] = L'\0';

    request_info.UserId = account_info.m_userGuid;
    request_info.UserIp = client_ip;

    service_client_->KeepUsingStatus(request_info, &response_info);
    if (response_info == NULL)
    {
        SUNLOG(
            eCRITICAL_LOG,
            __FUNCTION__" : Keep using status response data is null : \
                        account = %s, user guid = %d",
            account_info.m_account,
            account_info.m_userGuid
            );
        return RC::RC_PRIMIUM_RESPONSE_DATA_IS_NULL;
    }

    if (response_info->Result != ::Ok)
    {
        SUNLOG(
            eCRITICAL_LOG,
            __FUNCTION__" : Fail keep using status : message = %ls, \
                        code = %d, account = %s, user guid = %d",
            response_info->ErrorMessage,
            response_info->Result,
            account_info.m_account,
            account_info.m_userGuid
            );
        return RC::RC_PRIMIUM_FAIL_KEEP_USING_STATUS;
    }

    SUNLOG(
        eCRITICAL_LOG,
        __FUNCTION__" : Keep using status result : account = %s, user guid = %d, \
                    result code = %d, notify time out = %d, remaining_time = %d",
        account_info.m_account,
        account_info.m_userGuid,
        response_info->Result,
        response_info->NotificationTimeout,
        response_info->StatusRemainingTime
        );

    //out value
    remaining_time = response_info->StatusRemainingTime;
    check_time = response_info->NotificationTimeout;

    return RC::RC_PRIMIUM_SUCCESS;
}

void
IngambaAuthConnector::StopPrimiumService(const AccountInfo& account_info)
{
    if (service_client_ == NULL)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            __FUNCTION__" : Not connected ingamba billing server"
            );
        return;
    }

    StopUsingStatusRequest request_info;
    IStopUsingStatusResponse* response_info;

    wchar_t client_ip[IP_MAX_LEN] = {0,};
    size_t recv_client_ip_len = strlen(account_info.m_clientIp);
    MultiByteToWideChar(CP_ACP, 
                        0, 
                        account_info.m_clientIp, 
                        recv_client_ip_len, 
                        client_ip, 
                        _countof(client_ip));
    client_ip[_countof(client_ip) - 1] = L'\0';

    request_info.UserId = account_info.m_userGuid;
    request_info.UserIp = client_ip;
    
    service_client_->StopUsingStatus(request_info, &response_info);

    if (response_info->Result != ::Ok)
    {
        SUNLOG(
            eCRITICAL_LOG,
            __FUNCTION__" : Fail stop using status : message : %ls, code : %d, account : %s, user guid : %d",
            response_info->ErrorMessage,
            response_info->Result,
            account_info.m_account,
            account_info.m_userGuid
            );
    }
}

bool
IngambaAuthConnector::GetAccountInfo(USERGUID user_guid, IngambaUserInfo* OUT user_info)
{
    if (service_client_ == NULL)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            __FUNCTION__" : Not connected ingamba billing server"
            );
        return false;
    }

    GetUserInfoRequest get_user_info_request;
    IGetUserInfoResponse* get_user_info_response = NULL;

    get_user_info_request.UserId = user_guid;
    service_client_->GetUserInfo(get_user_info_request, &get_user_info_response);
    if (get_user_info_response == NULL)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            __FUNCTION__" : Fail get user info : user guid : %d", 
            user_guid
            );
        get_user_info_response->Free();
        get_user_info_response = NULL;
        return false;
    }

    if (get_user_info_response->Result != Ok)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            __FUNCTION__" : Fail Response auth : message : %ls, code : %d, user guid : %d", 
            get_user_info_response->ErrorMessage,
            get_user_info_response->Result,
            user_guid
            );
        get_user_info_response->Free();
        get_user_info_response = NULL;
        return false;
    }

    user_info->status_ID = get_user_info_response->StatusId;
    user_info->status_in_use = get_user_info_response->StatusInUse;
    user_info->status_remaining_time = get_user_info_response->StatusRemainingTime;
    //w_char -> char
    WideCharToMultiByte(CP_ACP, 
                        0, 
                        get_user_info_response->Login, 
                        -1, 
                        user_info->user_account, 
                        _countof(user_info->user_account), 
                        NULL, 
                        NULL);

    get_user_info_response->Free();
    get_user_info_response = NULL;

    return true;
}

ResultEnum
IngambaAuthConnector::AuthenticateUser(const IngambaAuthInfo& auth_info, 
                                       const char* recv_client_ip, 
                                       OUT int* user_guid)
{
    if (service_client_ == NULL)
    {
        SUNLOG (eCRITICAL_LOG, __FUNCTION__" : Not connect ingamba billing server");
        return ::InternalServerError;
    }

    wchar_t user_id[auth_info.MAX_ID_LEN + 1] = {0,};
    size_t user_id_len = strlen(auth_info.user_id_);
    user_id_len = min(_countof(auth_info.user_id_), user_id_len);
    MultiByteToWideChar(CP_ACP, 
                        0, 
                        auth_info.user_id_, 
                        user_id_len, 
                        user_id, 
                        _countof(user_id));
    user_id[_countof(user_id) - 1] = L'\0';

    wchar_t user_password[auth_info.MAX_PWD_LEN + 1] = {0,};
    size_t user_password_len = strlen(auth_info.user_password_);
    user_password_len = min(_countof(auth_info.user_password_), user_password_len);
    MultiByteToWideChar(CP_ACP, 
                        0, 
                        auth_info.user_password_, 
                        user_password_len, 
                        user_password, 
                        _countof(user_password));
    user_password[_countof(user_password) - 1] = L'\0';

    wchar_t client_ip[IP_MAX_LEN] = {0,};
    size_t recv_client_ip_len = strlen(recv_client_ip);
    MultiByteToWideChar(CP_ACP, 
                        0, 
                        recv_client_ip, 
                        recv_client_ip_len, 
                        client_ip, 
                        _countof(client_ip));
    client_ip[_countof(client_ip) - 1] = L'\0';

    AuthenticateUserRequest auth_request;
    auth_request.UserLogin = user_id;
    auth_request.UserPassword = user_password;
    auth_request.UserIp = client_ip;

    IAuthenticateUserResponse* auth_response = NULL;
    service_client_->AuthenticateUser(auth_request, &auth_response);
    if (auth_response == NULL)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            __FUNCTION__" : Ingamba response fail"
            );
        return BadPaidServiceId;
    }

    *user_guid = auth_response->UserId;
    ResultEnum auth_result = auth_response->Result;

    SUNLOG(
        eCRITICAL_LOG, 
        __FUNCTION__" : Auth Result : ID : %d, Result : %d", 
        auth_response->UserId, auth_result
        );

    if (auth_result != ::Ok)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            "Auth Error : Message : %ls, Guid : %d", 
            auth_response->ErrorMessage, 
            user_guid
            );
    }

    auth_response->Free();
    auth_response = NULL;

    return auth_result;
}

void
IngambaAuthConnector::_Initialize()
{
    inatance_module_ = ::LoadLibrary("Ingamba.PaidServicesIntegration.Wcf.NativeClient.dll");
    if (inatance_module_ == NULL)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            __FUNCTION__" : Fail load Ingamba dll"
            );
        return;
    }

    interface_function_ = 
        (CreateInterfaceFunction)::GetProcAddress(inatance_module_, "_CreateInterface@4");
    if (interface_function_ == NULL)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            __FUNCTION__" : Fail createInterface"
            );
        return;
    }

    interface_function_(&service_client_);
    if (service_client_ == NULL)
    {
        SUNLOG(
            eCRITICAL_LOG,
            __FUNCTION__" : Fail get service client info"
            );
        return;
    }

    try
    {
        service_client_->CreateChannel();
        service_client_->Open();
        SUNLOG(
            eCRITICAL_LOG, 
            __FUNCTION__" : Success start Ingamba billing service"
            );
    }
    catch (IMarshaledManagedException* e)
    {
        SUNLOG(
            eCRITICAL_LOG,
            __FUNCTION__" : %ls, %ls", e->GetManagedTypeName(), e->GetMessage());
        e->Free();
        service_client_ = NULL;
    }
}

void 
IngambaAuthConnector::_Realease()
{
    if (service_client_ != NULL)
    {
        service_client_->Close();
        service_client_->Free();
        service_client_ = NULL;
    }

    if (inatance_module_ != NULL)
    {
        FreeLibrary(inatance_module_);
        inatance_module_ = NULL;
    }

    interface_function_ = NULL;
}



#endif//_RU_INGAMBA_AUTH_PROCESS