#include "stdafx.h"

#if USING_GAMEGUARD
#include "GameGuard/GameGuard.h"
#include "GameGuard/ggsrv30.h"  // USING_GAMEGUARD

#pragma comment(lib, "../lib/"VC_DIR_REDIRECT"/ggsrv30lib_MT.lib")

UINT32 GameGuard::init()
{
    return InitCSAuth3("");
}

void GameGuard::release()
{
    CloseCSAuth3();
}
#endif // USING_GAMEGUARD