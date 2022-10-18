#pragma once

#include "BaseStatus.h"


class EtcStatus : public BaseStatus
{
public :
    EtcStatus();
    virtual ~EtcStatus();

    virtual void SetDownTime(WORD down_time)	
    { 
        __UNUSED(down_time); 
    }
};