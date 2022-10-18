#pragma once

#if USING_GAMEGUARD
#include <Windows.h>

class GameGuard : public util::Singleton<GameGuard>
{
public:
    GameGuard() {}
    ~GameGuard() {}

    UINT32 init();
    void release();
private:
    HMODULE csauth_lib;
};

#endif  //USING_GAMEGUARD