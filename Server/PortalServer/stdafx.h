#ifndef __STDAFX_H__
#define __STDAFX_H__


#pragma once

#define WIN32_LEAN_AND_MEAN



#include <windows.h>
#include <tchar.h>

#include <Global.Define.h>
#include <Define.h>
#include <ServerCommonDef.h>

#include <UtilityCommon.h>
#include <Const.h>
#include <Global.h>
#include <ServerConst.h>

#include <chunk_stl.h>
#include <ServerStructInPacket.h>


#include <MemoryPoolFactory.h>


#include <ScriptCode.IParser.h>
#include <ScriptDataManager.h>
#include <WorkLoad/WorkLoadPacketStream.h>
#include <WorkLoad/WorkLoadContainer.h>
#include <WorkLoad/WorkLoadManager.h>

#include <SunLog.h>


//////////////////////////////////////////////////////////////////////////
// LIBRARY LINK LIST
#include <LinkLibraryList.h>

//////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG

// <LIB><GUILD SERVER>

#else //

// <LIB><GUILD SERVER>

#endif //
//////////////////////////////////////////////////////////////////////////


#define _TRACE			DebugOut
#define _PRINT			printf
#define SUNLOG			MessageOut

#endif // __STDAFX_H__