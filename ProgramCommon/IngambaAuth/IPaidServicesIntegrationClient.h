#pragma once

namespace Ingamba {namespace PaidServicesIntegration { namespace Wcf { namespace NativeClient {

using namespace Dto;


// Main Service interface (ADT). Contains pure virtual functions and virtual destructor.
// Handles client requests and service responses.
struct IPaidServicesIntegrationService
{

	virtual void AuthenticateUser(const AuthenticateUserRequest &nativeRequest, IAuthenticateUserResponse ** ppNativeResponse) = 0;
	virtual void StartUsingStatus(const StartUsingStatusRequest &nativeRequest, IStartUsingStatusResponse ** ppNativeResponse) = 0;
	virtual void StopUsingStatus(const StopUsingStatusRequest &nativeRequest, IStopUsingStatusResponse ** ppNativeResponse) = 0;
	virtual void KeepUsingStatus(const KeepUsingStatusRequest &nativeRequest, IKeepUsingStatusResponse ** ppNativeResponse) = 0;
	virtual void GetUserInfo(const GetUserInfoRequest &nativeRequest, IGetUserInfoResponse ** ppNativeResponse) = 0;
	virtual void GetMoneyBalance(const GetMoneyBalanceRequest &nativeRequest, IGetMoneyBalanceResponse ** ppNativeResponse) = 0;
	virtual void BuyStatus(const BuyStatusRequest &nativeRequest, IBuyStatusResponse ** ppNativeResponse) = 0;
	virtual void BuyGameItem(const BuyGameItemRequest &nativeRequest, IBuyGameItemResponse ** ppNativeResponse) = 0;
	 
	/* Virtual destructor makes ADT memory-leak safe, because it is overriden in base class. Both managed and unmanaged resources are released.*/
	virtual ~IPaidServicesIntegrationService() {  }
};

// Sub Service interface (ADT).
// Handles client -> service communication.
struct IClientChannel
{
	// Creates channel between client and service.
	// IMarshaledManagedException is thrown, if service is not started.
	virtual void CreateChannel() = 0;

	// Opens existing channel.
	// IMarshaledManagedException is thrown, if channel has not beed created.
	virtual void Open() = 0;

	// Closes existing channel.
	// IMarshaledManagedException is thrown, if channel has not beed created.
	virtual void Close() = 0;

	virtual ~IClientChannel() {  }
};

struct IPaidServicesIntegrationClient:
	public IPaidServicesIntegrationService,
	public IClientChannel,
	public virtual IReleaseableObject // ATTENTION! Check comments for this class.
{
	virtual ~IPaidServicesIntegrationClient() {}
};


// The only type of exception thrown by application.
class IMarshaledManagedException : public virtual IReleaseableObject
{
public:
	virtual const wchar_t * GetManagedTypeName() const = 0;
	virtual const wchar_t * GetMessage() const = 0;
	virtual const wchar_t * GetStackTrace() const = 0;

	virtual ~IMarshaledManagedException() {}
};

typedef IPaidServicesIntegrationClient * IPaidServicesIntegrationClientPtr;

// Interface creator function protype. It is exported from the library with a "_GetInterface@4" name. "C" style.
// Calling convention - stdcall.
typedef void (__stdcall * CreateInterfaceFunction) (IPaidServicesIntegrationClientPtr *);

}}}}