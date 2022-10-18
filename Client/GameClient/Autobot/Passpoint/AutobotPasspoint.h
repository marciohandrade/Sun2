#ifndef _AutobotPasspoint_H_
#define _AutobotPasspoint_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <ProgramCommon/wzvector.h>

enum eActionID;

//------------------------------------------------------------------------------
class AutobotPasspoint
{
public:
    enum PasspointType
    {
        kPasspoint_Temporarily = 0,
        kPasspoint_Runaway,
        kPasspoint_Return,  //!< 특정 범위 사냥 일경우
        kPasspoint_FindFollowPlayer,    //!< 파티원 따라기기에서 파티원이 멀리 있어서 따라가기 안될경우 해당 위치까지 이동
        kPasspoint_Max
    };
   
public:
    AutobotPasspoint();
    AutobotPasspoint(PasspointType passpoint_type, DWORD objectkey, 
                    WzVector& position, eActionID action_id);
    ~AutobotPasspoint(){}

    bool IsArrival(WzVector& pos);

    PasspointType passpoint_type() const { return passpoint_type_; }
    DWORD objectkey() const { return objectkey_; }
    WzVector position() { return position_; }
    eActionID action_id() const { return action_id_; }

private:
    PasspointType passpoint_type_;
    DWORD objectkey_;
    WzVector position_;
    eActionID action_id_;
};


#endif
