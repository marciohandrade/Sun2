// applast.h

#ifndef _3DFRAMEWORK_APPLAST_H_
#define _3DFRAMEWORK_APPLAST_H_

#include "appdraw.h"
#include <programcommon/wzsingleton.h>

//------------------------------------------------------------------------------
/**
    @class CAppLast
*/
class CAppLast : public CAppDraw, 
                 public CWzSingleton<CAppLast>  
{
public:
    CAppLast();
    virtual ~CAppLast();
};



class iLogHandler
{
    iLogHandler();
};



#endif // _3DFRAMEWORK_APPLAST_H_
