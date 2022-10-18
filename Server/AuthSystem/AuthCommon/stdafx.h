#pragma once

// 모든 프로젝트 및 공유 폴더 위치가 고정이라는 전제하에 설정되는 상대 위치를 통한 Include
#include "../../../include/Global.Define.h"

#define WIN32_LEAN_AND_MEAN		// 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.

//#include <SunAuth/AuthDefine.Build.h>

#include <windows.h>
#include <string>
#include <stdio.h>
#include <stdarg.h>
#include <tchar.h>


// lib
#include <Define.h>
#include <UtilityCommon.h>
#include <ServerConst.h>
#include <ServerGlobal.h>

// local
#include <SunAuth/AuthDefine.h>
#include <SunAuth/AuthStruct.h>
#include "AuthStructInternal.h"
#include <SunAuth/AuthProtocol.h>
#include "AuthInternal.hxx"

#include "AuthLinkLibrayList.h"

