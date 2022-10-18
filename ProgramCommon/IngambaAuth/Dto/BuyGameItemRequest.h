#pragma once

namespace Ingamba { namespace PaidServicesIntegration { namespace Wcf { namespace NativeClient { namespace Dto {

struct BuyGameItemRequest
{
	int UserId;
	wchar_t * UserIp;
	int MoneyTypeId;
	double Amount;
	__int64 PaidServiceTransactionId;
	wchar_t * GameServerName;
	wchar_t * GameShopId;
	wchar_t * LocationId;
	int PlayerId;
	wchar_t * CharacterName;
	int GameItemId;
	wchar_t * GameItemName;
	int GameItemCount;
	wchar_t * ExtraInfo;
} ;



}}}}}
