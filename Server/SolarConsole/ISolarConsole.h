#ifndef __ISOLAR_CONSOLE_H__
#define __ISOLAR_CONSOLE_H__


#pragma once

#include <unknwn.h>
//=======================================================================================================================
/// Window console �� ������ Interface ISolarConsole Class
/**
	@author
			Kim Min Wook < taiyo@webzen.co.kr >	
	@since
			2004. 11. 10
	@remarks
			- �پ��� ����� �ֿܼ� �����ϱ� ���� ����
			- 
	@note
			- 
	@history 
			- 2005.1.19 : �������ڸ� ���� �� �ִ� �Լ� OutputDisplayArg() �߰�
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