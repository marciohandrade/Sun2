#pragma once

namespace Ingamba { namespace PaidServicesIntegration { namespace Wcf { namespace NativeClient { namespace Dto {

struct IGetMoneyBalanceResponse : public virtual IReleaseableObject
{
	virtual ~IGetMoneyBalanceResponse() {}

	int Result;
	wchar_t * ErrorMessage;
	double Balance;
};



}}}}}
