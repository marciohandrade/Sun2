#include "StdAfx.h"
#include ".\solarconsolefactory.h"
#include ".\solarconsole.h"

extern LONG g_cLocks;
extern LONG g_cComponents;

HRESULT __stdcall SolarConsoleFactory::QueryInterface(const IID& iid, void ** ppv)
{
	if(iid == IID_IClassFactory || iid == IID_IUnknown)
	{
		*ppv = static_cast<IClassFactory*>(this);
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}
	reinterpret_cast<IUnknown*>(*ppv)->AddRef();
	return S_OK;
}

ULONG __stdcall SolarConsoleFactory::AddRef()
{
	return InterlockedIncrement(&m_cRef);
}

ULONG __stdcall SolarConsoleFactory::Release()
{
	if(InterlockedDecrement(&m_cRef) == 0)
	{
		delete this;
		return 0;
	}
	return m_cRef;
}

HRESULT __stdcall SolarConsoleFactory::CreateInstance(IUnknown * pUnknownOuter, const IID & iid, void ** ppv)
{
	if(pUnknownOuter != NULL)
	{
		return CLASS_E_NOAGGREGATION;
	}

	g_pConsole = new SolarConsole;
	if( !g_pConsole ) return E_OUTOFMEMORY;

	HRESULT hr = g_pConsole->QueryInterface(iid, ppv);
	g_pConsole->Release();

	++g_cComponents;

	return hr;
}

HRESULT __stdcall SolarConsoleFactory::LockServer(BOOL bLock)
{
	if(bLock)
	{
		InterlockedIncrement(&g_cLocks);
	}
	else
	{
		InterlockedDecrement(&g_cLocks);
	}
	return S_OK;
}