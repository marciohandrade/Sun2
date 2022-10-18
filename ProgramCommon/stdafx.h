// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������ 
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#define WIN32_LEAN_AND_MEAN


// ��� ������Ʈ �� ���� ���� ��ġ�� �����̶�� �����Ͽ� �����Ǵ� ��� ��ġ�� ���� Include
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

// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.

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
