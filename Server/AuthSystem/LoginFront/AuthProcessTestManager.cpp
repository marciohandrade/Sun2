#include "stdafx.h"
#include "AuthProcessTestManager.h"
#include <SunAuth/AuthProtocol_CF.h>
#include <AuthStepAuthToken.h>
#include <utility>

#if AUTH_TEST_CODE

AuthProcessTestManager::AuthProcessTestManager()
{
    client_session_.Init();
}

AuthProcessTestManager::~AuthProcessTestManager()
{

}

void 
AuthProcessTestManager::Start(TestType test_type)
{
    SendAuthServer(test_type);
}

void 
AuthProcessTestManager::SendAuthServer(TestType test_type)
{
    switch(test_type)
    {
#ifdef _RU_INGAMBA_AUTH_PROCESS
            case russia_ingamba:
                {
                    MSG_CF_AUTH_PASS_RU_INGAMBA_SYN test_msg;
                    {
                        IngambaAuthInfo& info = test_msg.ingamba_auth_info_;

                        info.type_code_ = AuthStepAuthToken::eProtocol_AuthIngamba;
                        ::_tcsncpy(info.user_id_, "user2", _countof(info.user_id_));
                        info.user_id_[_countof(info.user_id_) - 1] = L'\0';
                        ::_tcsncpy(info.user_password_, "pass2", _countof(info.user_password_));
                        info.user_id_[_countof(info.user_password_) - 1] = L'\0';
                    }

                    client_session_.OnCF_AUTH_AUTHENTICATION_SYN(&test_msg, sizeof(test_msg));
                    SUNLOG(eCRITICAL_LOG, "Send ID:%ls PW:%ls IP:%ls", test_msg.ingamba_auth_info_.user_id_,
                        test_msg.ingamba_auth_info_.user_password_);

                }
                break;
#endif
    default:
        break;

    }
}
#endif