// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������ 
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#define WIN32_LEAN_AND_MEAN		// ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
#ifndef _WIN32_WINNT
    #define _WIN32_WINNT    0x0500
#endif
#include <windows.h>

#include <UtilityCommon.h>
#include <SolarAssert.h>
//#include <CommonDef.h>

#include "SolarClientNetwork.h"		//< for shared macro
#include "Connection.h"
#include "SunNetworkApi.h"

#pragma comment( lib, "ws2_32.lib" )

// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
