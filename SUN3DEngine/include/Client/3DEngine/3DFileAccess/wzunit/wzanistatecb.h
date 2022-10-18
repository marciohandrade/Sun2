// wzanistatecb.h

#ifndef _3DFILEACCESS_WZANISTATE_CB_H_
#define _3DFILEACCESS_WZANISTATE_CB_H_

#include "wzanistatecbcode.h"

//------------------------------------------------------------------------------
/**
    @class CWzAniStateCB
*/
class CWzAniStateCB
{
public:
    CWzAniStateCB() {}
    virtual ~CWzAniStateCB() = 0 {}

    virtual void OnAniStateNotice( int iCode, DWORD dwParam ) {}
};

#endif
