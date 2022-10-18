#pragma once

#include <Windows.h>

#define USING_WZASSERT

#ifndef WzAssert

    #ifdef USING_WZASSERT

        #ifdef _DEBUG
            #define WzAssert(expr) \
                ((void)((expr) ? 0 : (FireAssertionFailureA(__FILE__,__LINE__,__FUNCTION__,#expr),0)));
        #else
            #define WzAssert(expr)
        #endif

    #else

        #define WzAssert assert

    #endif

#endif

void FireAssertionFailureA(LPCSTR fileName, DWORD line, LPCSTR func, LPCSTR expr);

