#pragma once

namespace Ingamba { namespace PaidServicesIntegration { namespace Wcf { namespace NativeClient { namespace Dto {

struct IBuyStatusResponse : public virtual IReleaseableObject
{
	virtual ~IBuyStatusResponse() {}

	wchar_t * ErrorMessage;
	ResultEnum Result;
	__int64 TransactionId;
};


}}}}}
