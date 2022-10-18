// wzfastmath.h

#ifndef _COMMON_WZFASTMATH_H_
#define _COMMON_WZFASTMATH_H_

#include "wzmath.h"

namespace FastMath 
{
    struct CPUCaps
    {
        // MMX, SSE, and SSE2 Intrinsics for Intel processors
        // 3DNow Intrinsics for AMD processors
        bool bMMX;  
        bool bSSE;
        bool bSSE2;
        bool b3DNow;
        bool b3DNowExt;
    };

    enum SlIntrinsic
    {
        _MMX = 0,
        _SSE,
        _SSE2,
        _3DNOW,
        _3DNOWEXT,
    };

    void GetCPUCaps( CPUCaps* caps );
    void Startup();
};

#endif // _COMMON_WZFASTMATH_H_