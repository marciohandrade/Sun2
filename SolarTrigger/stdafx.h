// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������ 
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

// ��� ������Ʈ �� ���� ���� ��ġ�� �����̶�� �����Ͽ� �����Ǵ� ��� ��ġ�� ���� Include
#include "../include/Global.Define.h"

#define WIN32_LEAN_AND_MEAN		// ���� ������ �ʴ� ������ Windows ������� �����մϴ�.

#include <Define.Build.h>
#include <UtilityCommon.h>

#include <ProgramCommon/WzDataType.h>
#include <Define.h>
//#include <CommonDef.h>
//#include <UtilityCommon.h>

#if defined(_DH_TRIGGER_LOG_MESSAGE) && !defined(_SERVER)
    #define _DH_TRIGGER_LOG_MESSAGE_IN_TRIGGER
#endif
