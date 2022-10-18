#pragma once

#define WIN32_LEAN_AND_MEAN

// 모든 프로젝트 및 공유 폴더 위치가 고정이라는 전제하에 설정되는 상대 위치를 통한 Include
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