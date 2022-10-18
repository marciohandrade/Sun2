// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이 
// 들어 있는 포함 파일입니다.
//

#pragma once

// 모든 프로젝트 및 공유 폴더 위치가 고정이라는 전제하에 설정되는 상대 위치를 통한 Include
#include "../include/Global.Define.h"

#define WIN32_LEAN_AND_MEAN		// 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.

#include <Define.Build.h>
#include <UtilityCommon.h>

#include <ProgramCommon/WzDataType.h>
#include <Define.h>
//#include <CommonDef.h>
//#include <UtilityCommon.h>

#if defined(_DH_TRIGGER_LOG_MESSAGE) && !defined(_SERVER)
    #define _DH_TRIGGER_LOG_MESSAGE_IN_TRIGGER
#endif
