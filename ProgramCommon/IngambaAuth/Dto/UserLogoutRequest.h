#pragma once

namespace Ingamba { namespace PaidServicesIntegration { namespace Wcf { namespace NativeClient { namespace Dto {

struct UserLogoutRequest
{
	int PaidServiceId;
	int UserId;
	wchar_t * UserIp;
};



}}}}}
