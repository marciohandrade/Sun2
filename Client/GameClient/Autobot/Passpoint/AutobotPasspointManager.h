#ifndef _AutobotPasspointManager_H_
#define _AutobotPasspointManager_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "Singleton.h"

#include "AutobotPasspoint.h"

//------------------------------------------------------------------------------
class AutobotPasspointManager : public Singleton<AutobotPasspointManager>
{
public:
    AutobotPasspointManager();
    ~AutobotPasspointManager();

    AutobotPasspoint* Current();
    bool Next();
    void Clear();
    void Pop();
    void Push(AutobotPasspoint& checkpoint);
    void Push(AutobotPasspoint::PasspointType passpoint_type, 
                DWORD objectkey, 
                WzVector& position, 
                eActionID action_id);

    std::list<AutobotPasspoint>& passpoint_list() { return passpoint_list_; }

private:
    std::list<AutobotPasspoint> passpoint_list_;
};



#endif
