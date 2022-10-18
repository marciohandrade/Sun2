#pragma once

namespace Ingamba { namespace PaidServicesIntegration { namespace Wcf { namespace NativeClient { namespace Dto {

struct AuthenticateUserRequest
{
	wchar_t * UserLogin;
	wchar_t * UserPassword;
	wchar_t * UserIp;
}; 


}}}}}
