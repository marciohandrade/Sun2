#ifndef _INGAMBA_AUTH_CONNECTOR_H
#define _INGAMBA_AUTH_CONNECTOR_H

// 러시아 Ingamba BillingIntegrationServer와의 통신
#ifdef _RU_INGAMBA_AUTH_PROCESS

#include "IngambaAuth\ResultEnum.h"
#include "IngambaAuth\IReleaseableObject.h"
#include "IngambaAuth\Dto\Dto.h"
#include "IngambaAuth\IPaidServicesIntegrationClient.h"

using namespace Ingamba;
using namespace PaidServicesIntegration;
using namespace Wcf;
using namespace NativeClient;

class IngambaAuthConnector
{
public:
    enum {
        WeeklyPrimiumService = 1,
        MonthlyPrimiumService = 1,
    };
    IngambaAuthConnector();
    ~IngambaAuthConnector();

    //인감바 빌링 서버에 user_guid에 해당하는 유저의 인증 요청
    ResultEnum AuthenticateUser(
        const IngambaAuthInfo& auth_info, 
        const char* client_ip, OUT int* user_guid
        );

    //인감바 빌링 서버에 user_guid에 해당하는 정보 요청
    bool GetAccountInfo(
        USERGUID user_guid,
        IngambaUserInfo* OUT user_info
        );

    //프리미엄 서비스 시작
    RC::PrimiumServiceResult StartPrimiumService(
        const AccountInfo& account_info
        );

    //프리미엄 서비스를 유지 할 수 있는가?
    RC::PrimiumServiceResult IsKeepingPrimiumService(
        const AccountInfo& account_info,
        int& OUT check_time,
        int& OUT remaining_time
        );

    //프리미엄 서비스 정지(일시정지)
    void StopPrimiumService(
        const AccountInfo& account_info
        );

private:
    //인감바 라이브러리 로드및 연결 설청
    void _Initialize();

    //릴리즈
    void _Realease();

private:
    IPaidServicesIntegrationClientPtr service_client_;
    HMODULE inatance_module_;
    CreateInterfaceFunction interface_function_;
};
#endif //_RU_INGAMBA_AUTH_PROCESS
#endif //_INGAMBA_AUTH_CONNECTOR_H


