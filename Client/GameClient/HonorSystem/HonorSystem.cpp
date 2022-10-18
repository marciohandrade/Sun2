#include "SunClientPrecompiledHeader.h"

#ifdef _NA_003027_20111013_HONOR_SYSTEM
//
#include "HonorSystem.h"
#include "HonorSystemData.h"

namespace HonorSystem
{
    HonorSystemData honor_system_data;

    HonorSystemData& GetHonorSystemData()
    {
        return HonorSystem::honor_system_data;
    }
};
#endif //_NA_003027_20111013_HONOR_SYSTEM