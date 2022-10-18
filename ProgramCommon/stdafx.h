// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이 
// 들어 있는 포함 파일입니다.
//

#pragma once

#define WIN32_LEAN_AND_MEAN


// 모든 프로젝트 및 공유 폴더 위치가 고정이라는 전제하에 설정되는 상대 위치를 통한 Include
#include "../include/Global.Define.h"

#include <tchar.h>
#include <windows.h>

#include <iostream>
#include <algorithm>

#include <Define.h>
#include <Const.h>
#include <UtilityCommon.h>
#include <SolarAssert.h>
#include <Struct.h>
#include <functional_custom.h>

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.

#include <ScriptCode.IParser.h>

#define random(min, max)                    ((rand()%(int)(((max) + 1)-(min)))+ (min)) 

#if !defined(_SERVER)
    #include <GeneralFunction.h>
    #undef SUNLOG
  #ifdef _SUNGAME_VER
    #define SUNLOG __noop
  #else
    #define SUNLOG WriteParserErrLog
  #endif
#endif
