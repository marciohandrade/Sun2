#ifndef __SUN_CONSOLE_H__
#define __SUN_CONSOLE_H__

#pragma once

#include <SolarSimpleConsole.h>

#include <ISolarConsole_Define.h>
#include <ISolarConsole_Guid.h>
#include <ISolarConsole.h>

enum eCONSOLE_TYPE { eSIMPLE_CONSOLE, eCOMPLEX_CONSOLE };

class SUNConsole
{
public:
	SUNConsole(){}
	~SUNConsole(){}

	BOOL					Init( eCONSOLE_TYPE eType );
	VOID					Release();

	BOOL					Update();

private:
	SolarSimpleConsole *	m_pSimpleConsole;
	ISolarConsole *			m_pIConsole;
	eCONSOLE_TYPE			m_eType;
};

#endif // __SUN_CONSOLE_H__