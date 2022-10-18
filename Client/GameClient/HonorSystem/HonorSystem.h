#pragma once
#ifdef _NA_003027_20111013_HONOR_SYSTEM

#include "HonorSystemPacketParser.h"
#include "HonorSystemFunction.h"
#include "HonorSystemData.h"

namespace HonorSystem
{
    enum
    {
        kNoTitleCode = 0,
        kNoTitleMenuIndex = 0xffff,
        kNoTitleMenuStringCode = 6764,
    };

    HonorSystemData& GetHonorSystemData();
};//namespace HonorSystem

#endif //_NA_003027_20111013_HONOR_SYSTEM