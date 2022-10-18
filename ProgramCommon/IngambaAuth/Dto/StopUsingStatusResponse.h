#pragma once

namespace Ingamba { namespace PaidServicesIntegration { namespace Wcf { namespace NativeClient { namespace Dto {

struct IStopUsingStatusResponse : public virtual IReleaseableObject
{
	virtual ~IStopUsingStatusResponse() {}

	ResultEnum Result;
	wchar_t * ErrorMessage;
	int StatusRemainingTime;
};



}}}}}
