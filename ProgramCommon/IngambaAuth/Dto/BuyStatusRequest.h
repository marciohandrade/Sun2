#pragma once

namespace Ingamba { namespace PaidServicesIntegration { namespace Wcf { namespace NativeClient { namespace Dto {

struct BuyStatusRequest
{
	int UserId;
	wchar_t * UserIp;
	int RecipientUserId;
	int StatusId;
	int StatusTariffId;
	int MoneyTypeId;
};


}}}}}

