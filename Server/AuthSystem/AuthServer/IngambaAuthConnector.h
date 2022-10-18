#ifndef _INGAMBA_AUTH_CONNECTOR_H
#define _INGAMBA_AUTH_CONNECTOR_H

// ���þ� Ingamba BillingIntegrationServer���� ���
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

    //�ΰ��� ���� ������ user_guid�� �ش��ϴ� ������ ���� ��û
    ResultEnum AuthenticateUser(
        const IngambaAuthInfo& auth_info, 
        const char* client_ip, OUT int* user_guid
        );

    //�ΰ��� ���� ������ user_guid�� �ش��ϴ� ���� ��û
    bool GetAccountInfo(
        USERGUID user_guid,
        IngambaUserInfo* OUT user_info
        );

    //�����̾� ���� ����
    RC::PrimiumServiceResult StartPrimiumService(
        const AccountInfo& account_info
        );

    //�����̾� ���񽺸� ���� �� �� �ִ°�?
    RC::PrimiumServiceResult IsKeepingPrimiumService(
        const AccountInfo& account_info,
        int& OUT check_time,
        int& OUT remaining_time
        );

    //�����̾� ���� ����(�Ͻ�����)
    void StopPrimiumService(
        const AccountInfo& account_info
        );

private:
    //�ΰ��� ���̺귯�� �ε�� ���� ��û
    void _Initialize();

    //������
    void _Realease();

private:
    IPaidServicesIntegrationClientPtr service_client_;
    HMODULE inatance_module_;
    CreateInterfaceFunction interface_function_;
};
#endif //_RU_INGAMBA_AUTH_PROCESS
#endif //_INGAMBA_AUTH_CONNECTOR_H


