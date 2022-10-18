#ifndef __ISOLAR_CONSOLE_H__
#define __ISOLAR_CONSOLE_H__


#pragma once

#include <unknwn.h>
//=======================================================================================================================
/// Window console 을 구현한 Interface ISolarConsole Class
/**
	@author
			Kim Min Wook < taiyo@webzen.co.kr >	
	@since
			2004. 11. 10
	@remarks
			- 다양한 기능을 콘솔에 적용하기 위해 제작
			- 
	@note
			- 
	@history 
			- 2005.1.19 : 가변인자를 받을 수 있는 함수 OutputDisplayArg() 추가
*/
//=======================================================================================================================
interface ISolarConsole : IUnknown
{
public:

	virtual HRESULT __stdcall			QueryInterface( const IID& iid, void ** ppv ) = 0;
	virtual ULONG __stdcall				AddRef() = 0;
	virtual ULONG __stdcall				Release() = 0;

	virtual BOOL __stdcall				CreateConsole( const SOLAR_CONSOLE_DESC & desc, HWND * hWndOut ) = 0;
	virtual VOID __stdcall				DestroyConsole() = 0;
	virtual VOID __stdcall				OutputDisplay( char * szString ) = 0;
	virtual VOID __stdcall				OutputDisplayArg( char * szString, ... ) = 0;
	virtual VOID __stdcall				OutputDisplayEx( char * szString, DWORD color ) = 0;
	virtual VOID __stdcall				OutputFile( char * szString ) = 0;
	virtual VOID __stdcall				MessageLoop() = 0;
	virtual BOOL __stdcall				MessageUpdate() = 0;
};


#endif // __ISOLAR_CONSOLE_H__