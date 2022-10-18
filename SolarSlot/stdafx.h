#ifndef _SOLAR_SLOT_STDAFX_H
#define _SOLAR_SLOT_STDAFX_H
#pragma once

#ifndef WAVERIX_CODE_BACKUP
  #define WAVERIX_CODE_BACKUP (0)
#endif
#define WIN32_LEAN_AND_MEAN

// 모든 프로젝트 및 공유 폴더 위치가 고정이라는 전제하에 설정되는 상대 위치를 통한 Include
#include "../include/Global.Define.h"

#include <windows.h>
#include <stdio.h>

#include <Define.h>
#include <Const.h>
#include <UtilityCommon.h>

#include <ResultCode.h>
#include <ScriptCode.IParser.h>

#if !defined(_SERVER)
    #include <GeneralFunction.h>
    #undef SUNLOG
    #define SUNLOG WriteParserErrLog
#endif

#endif //_SOLAR_SLOT_STDAFX_H

