#pragma once

namespace Ingamba { namespace PaidServicesIntegration { namespace Wcf { namespace NativeClient { namespace Dto {

struct IBuyGameItemResponse : public virtual IReleaseableObject
{
	virtual ~IBuyGameItemResponse() { }

	wchar_t * ErrorMessage;
	ResultEnum Result;
	__int64 TransactionId;
};



}}}}}
