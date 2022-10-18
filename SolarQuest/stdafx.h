// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이 
// 들어 있는 포함 파일입니다.
//

#pragma once


#define WIN32_LEAN_AND_MEAN		// 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.

// 모든 프로젝트 및 공유 폴더 위치가 고정이라는 전제하에 설정되는 상대 위치를 통한 Include
#include "../include/Global.Define.h"

// Windows 헤더 파일입니다.
#include <windows.h>

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include <assert.h>
#include <hash_map>
#include <SolarHashTable.h>
#include <SolarFileReader.h>
#include <Timer.h>
#include <MemoryPoolFactory.h>
#include <Define.h>
#include <Const.h>
#include "QuestDefine.h"
#include "QuestStruct.h"
#include <ScriptCode.IParser.h>
