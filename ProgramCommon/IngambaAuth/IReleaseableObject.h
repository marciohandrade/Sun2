#pragma once

namespace Ingamba {namespace PaidServicesIntegration { namespace Wcf { namespace NativeClient {

// Handles resources release operation.
// Use Free() rather than "delete" operator call on any object derived from this interface.
// "delete" call does not guarantee any kind of heap-allocated resource to be realeased correctly.
struct IReleaseableObject
{
	virtual void Free() = 0;
	virtual ~IReleaseableObject() {}
};

}}}}