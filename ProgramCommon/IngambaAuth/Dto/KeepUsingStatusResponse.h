#pragma once

namespace Ingamba { namespace PaidServicesIntegration { namespace Wcf { namespace NativeClient { namespace Dto {

struct IKeepUsingStatusResponse : public virtual IReleaseableObject
{
	virtual ~IKeepUsingStatusResponse() {}
	ResultEnum Result;
	wchar_t * ErrorMessage;
	int NotificationTimeout;
	int StatusRemainingTime;
};


}}}}}
