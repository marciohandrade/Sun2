// stdafx.cpp : 표준 포함 파일만 들어 있는 소스 파일입니다.
#include "stdafx.h"

#if defined(_AUTH_KOREA) &&     defined(_KOREA)
    #define _AUTH_NATION_CHECK      (1)
    #define _IsKOREA_               (1)
#elif defined(_AUTH_CHINA) &&   defined(_CHINA)
    #define _AUTH_NATION_CHECK      (1)
    #define _IsKOREA_               (0)
#elif defined(_AUTH_JAPAN) &&   defined(_JAPAN)
    #define _AUTH_NATION_CHECK      (1)
    #define _IsKOREA_               (0)
#elif defined(_AUTH_TAIWAN) &&  defined(_TAIWAN)
    #define _AUTH_NATION_CHECK      (1)
    #define _IsKOREA_               (0)
#elif defined(_AUTH_USA) &&     defined(_USA)
    #define _AUTH_NATION_CHECK      (1)
    #define _IsKOREA_               (0)
#elif defined(_AUTH_GLOBAL) &&  defined(_GLOBAL)
    #define _AUTH_NATION_CHECK      (1)
    #define _IsKOREA_               (0)
#elif defined(_AUTH_RUSSIA) &&  defined(_RUSSIA)
    #define _AUTH_NATION_CHECK      (1)
    #define _IsKOREA_               (0)
#else
    #define _AUTH_NATION_CHECK      (0)
    #define _IsKOREA_               (0)
#endif

#if defined(__KR000000_090128_CHANNELING__)
    #define _IsCHANNELING_      (1)
#else
    #define _IsCHANNELING_      (0)
#endif

#if defined(_CHANNELING_AUTH_FLOW)
    #define _IsCHANNELING_AUTH_   (1)
#else
    #define _IsCHANNELING_AUTH_   (0)
#endif

BOOST_STATIC_ASSERT(_AUTH_NATION_CHECK);

#if (_IsKOREA_ && (_IsCHANNELING_ != _IsCHANNELING_AUTH_))
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if (!_IsKOREA_ && ((_IsCHANNELING_ != _IsCHANNELING_AUTH_) || _IsCHANNELING_))
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

BOOST_STATIC_ASSERT(AU_AUTH_UPPERBOUND_CHECK_PROTOCOL < CF_AUTH_REQ_PATCH_VER_SYN);

