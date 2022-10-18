// wzloghandlerfactory.h

#ifndef _PROGRAMCOMMON_WZLOGHANDLERFACTORY_H_
#define _PROGRAMCOMMON_WZLOGHANDLERFACTORY_H_

#include "wzlogtype.h"

class WzLogHandler;

//------------------------------------------------------------------------------
/**
    @class WzLogHandlerFactory
*/
class WzLogHandlerFactory
{
public:
    WzLogHandlerFactory( void );
    virtual ~WzLogHandlerFactory( void );

    virtual WzLogHandler* NewHandler( WZLOG_TYPE type );
};

#endif // _PROGRAMCOMMON_WZLOGHANDLERFACTORY_H_