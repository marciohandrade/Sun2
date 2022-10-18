#pragma once

namespace Ingamba { namespace PaidServicesIntegration { namespace Wcf { namespace NativeClient { namespace Dto {

struct StartUsingStatusRequest
{
	int UserId;
	wchar_t * UserIp;
	int NotificationTimeout;
};




}}}}}
