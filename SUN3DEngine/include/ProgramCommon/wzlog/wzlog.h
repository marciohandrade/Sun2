// wzlog.h

#ifndef _PROGRAMCOMMON_WZLOG_H_
#define _PROGRAMCOMMON_WZLOG_H_

#include "wzloghandler.h"

#ifdef LAST_RELEASE
#define WZLOG   // no operation
#else
#define WZLOG   if( WzLogHandler::IsValid() ) WzLogHandler::Instance()->Write
#endif

#endif // _PROGRAMCOMMON_WZLOG_H_