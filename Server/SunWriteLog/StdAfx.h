#pragma once

#define WIN32_LEAN_AND_MEAN

// ��� ������Ʈ �� ���� ���� ��ġ�� �����̶�� �����Ͽ� �����Ǵ� ��� ��ġ�� ���� Include
#include "../../include/Global.Define.h"
#include <UtilityCommon.h>

#include <windows.h>
#include <stdio.h>
#include <io.h>
#include <string>
#include <tchar.h>
//#include <afxcom_.h>
#include <math.h>

#include <Define.h>
#include <Const.h>

#ifndef _DEBUG
    #pragma comment(lib, "../lib/"VC_DIR_REDIRECT"/SolarLog.lib")
#else
    #pragma comment(lib, "../lib/"VC_DIR_REDIRECT"/SolarLog_d.lib")
#endif