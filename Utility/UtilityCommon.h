#pragma once
// WARNING, this file has top order in all includes of all projects
#ifndef UTILITY_COMMON_PCH_H
#define UTILITY_COMMON_PCH_H

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500 
#endif
#define __INTELLISENSE__
// ��� ������Ʈ �� ���� ���� ��ġ�� �����̶�� �����Ͽ� �����Ǵ� ��� ��ġ�� ���� Include
#include "../include/Global.Define.h"

#include <windows.h>
#include <memory>
#include <assert.h>
#include <crtdbg.h>
#include <tchar.h>

#ifndef INOUT
#	define INOUT
#endif //

#include "CommonDef.h"
#include "chunk_stl.h"
#include "SunSTL.Utiltiy.h"
#include "ErrorControl.h"

#include "CommonStruct.h"
#include "Timer.h"
#include "SolarDateTime.h"
#include "StreamHashKey.h"
#include "RandomNumberGenerator.h"
#include "MemoryPoolFactory.h"
#include "SafeMemoryPoolFactory.h"
#include "SolarHashTable.h"

#include "tstring.h"
//#include "Event.h"
//#include "CriticalSection.h"
//#ifdef _NA_0_20080612_WORLDSERVER_REFACTORING
//	#include "MultiIndexDataTable.h"
//	#include "ExtractPacketData.h"
//#endif 

#endif //UTILITY_COMMON_PCH_H