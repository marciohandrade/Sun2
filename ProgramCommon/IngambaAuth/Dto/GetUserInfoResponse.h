#pragma once

namespace Ingamba { namespace PaidServicesIntegration { namespace Wcf { namespace NativeClient { namespace Dto {

struct IGetUserInfoResponse : public virtual IReleaseableObject
{	
	virtual ~IGetUserInfoResponse() {}
	ResultEnum Result ;
	wchar_t * ErrorMessage;
	int StatusId ;
	bool StatusInUse ;
	int StatusRemainingTime ;
	wchar_t * Login;
	wchar_t * Nickname;
	wchar_t * Email;

};



}}}}}
