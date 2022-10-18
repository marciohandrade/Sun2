#ifndef __SOLAR_CONSOLE_FACTORY_H__
#define __SOLAR_CONSOLE_FACTORY_H__


#pragma once
#include <unknwn.h>

class SolarConsoleFactory : public IClassFactory
{
public:
	SolarConsoleFactory():m_cRef(1){}
	~SolarConsoleFactory(){}

	virtual HRESULT __stdcall		QueryInterface(const IID& iid, void ** ppv);
	virtual ULONG __stdcall			AddRef();
	virtual ULONG __stdcall			Release();

	virtual HRESULT __stdcall		CreateInstance(IUnknown * pUnknownOuter, const IID & iid, void ** ppv);
	virtual HRESULT __stdcall		LockServer(BOOL bLock);

private:
	LONG							m_cRef;
};


#endif // __SOLAR_CONSOLE_FACTORY_H__