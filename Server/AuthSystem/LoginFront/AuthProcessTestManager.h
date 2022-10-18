#ifndef _AUTH_PROCESS_TEST_MANAGER_H
#define _AUTH_PROCESS_TEST_MANAGER_H

#if AUTH_TEST_CODE

#include "ClientSession.h"
#include <Singleton.h>

//==================================================================================================
/// ���þ� Ingamba BillingIntegrationServer���� ���
/**
@author gamelulu
@since  2011/03/19
@remark
        - ���� ���� �׽�Ʈ�� Class, Ŭ���̾�Ʈ ���̵� �ֿܼ��� Ư�� Ű�� �Է������� ���ϴ� ��Ŷ��
        ���� �������� �����Ͽ� �׽�Ʈ���� �� �ְ� �����ش�. ���þ� Ingamba�� ������� ���� �ۼ�

@history 
        - 2011/03/19 : + Ŭ���� ����(gamelulu)
                       + AUTH_TEST_CODE define�� Ȱ��ȭ ���Ѿ� �׽�Ʈ�� �����ϴ�.
                       + ���� ������ ���� �κ��� �������� �˸��� �����͸� SendAuthServer() ���� 
                         ���� �Է� �Ͽ� ���� ������ ������.
        - 2011/04/26 : code reviewer (waverix) think that the below code block is good
                       even if waverix don't read it.
*/
//==================================================================================================

struct MSG_CF_AUTH_PASS_RU_INGAMBA_SYN;

class AuthProcessTestManager : public util::Singleton<AuthProcessTestManager>
{
public:

    enum TestType
    { 
        russia_ingamba = 0 
    };

    AuthProcessTestManager();
    ~AuthProcessTestManager();

    void Start(TestType test_type);

private:
    void SendAuthServer(TestType test_type);
    
private:
    ClientSession client_session_;    
};

#endif
#endif//_AUTH_PROCESS_TEST_MANAGER_H