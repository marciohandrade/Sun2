#ifndef _AUTH_PROCESS_TEST_MANAGER_H
#define _AUTH_PROCESS_TEST_MANAGER_H

#if AUTH_TEST_CODE

#include "ClientSession.h"
#include <Singleton.h>

//==================================================================================================
/// 러시아 Ingamba BillingIntegrationServer와의 통신
/**
@author gamelulu
@since  2011/03/19
@remark
        - 인증 로직 테스트용 Class, 클라이언트 없이도 콘솔에서 특정 키를 입력함으로 원하는 패킷을
        인증 로직으로 전달하여 테스트를할 수 있게 도와준다. 러시아 Ingamba를 대상으로 최초 작성

@history 
        - 2011/03/19 : + 클래스 생성(gamelulu)
                       + AUTH_TEST_CODE define을 활성화 시켜야 테스트가 가능하다.
                       + 유저 정보에 관한 부분은 버전별로 알맞은 데이터를 SendAuthServer() 에서 
                         직접 입력 하여 인증 서버로 보낸다.
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