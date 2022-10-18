#pragma once

namespace Ingamba { namespace PaidServicesIntegration { namespace Wcf { namespace NativeClient { namespace Dto {

struct IAuthenticateUserResponse : public virtual IReleaseableObject
{
	virtual ~IAuthenticateUserResponse() {}

	ResultEnum Result;
	wchar_t * ErrorMessage;
	int UserId;
};


}}}}}
