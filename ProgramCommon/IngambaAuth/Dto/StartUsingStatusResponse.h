#pragma once

namespace Ingamba { namespace PaidServicesIntegration { namespace Wcf { namespace NativeClient { namespace Dto {

struct IStartUsingStatusResponse : public virtual IReleaseableObject
{
	virtual ~IStartUsingStatusResponse() {}
	ResultEnum Result;
	wchar_t * ErrorMessage;
	int NotificationTimeout;
	int StatusRemainingTime;
};



}}}}}
