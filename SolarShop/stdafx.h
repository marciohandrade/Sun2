#pragma once

// 모든 프로젝트 및 공유 폴더 위치가 고정이라는 전제하에 설정되는 상대 위치를 통한 Include
#include "../include/Global.Define.h"

#define WIN32_LEAN_AND_MEAN // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#include <tchar.h>
#include <windows.h>
#include <iostream>
#include <math.h>


#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL

#include <Define.h>
#include <UtilityCommon.h>
#include <Const.h>
#include <Global.h>

#include <MemoryPoolFactory.h>

#include "Define.ShopServer.h"
#include "Protocol.ShopServer.h"

#include "ShopDefine.h"

#endif