#pragma once


#include "WzLogSystem.h"



#if defined(EXCLUDE_WZLOG) || defined(LAST_RELEASE)
    #define WZLOG   // no operation
#else

    #define WZLOG   OutputWzLog
#endif

//WZERR = 0,
//WZWAR,
//WZINF,
//WZDBG,
//WZTRA,
//WZPKT1,
//WZPKT2,
//WZPKT3,
//WZNUM_LOGLVL

//
//#define WZLOG_ERR(...) WZLOG( WZERR, ... )