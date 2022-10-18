#include "WorldServerPch.hxx"
#include "./PMSManagerInWorld.h"

#ifdef _JP_0_20101123_PMS //
#include <pms/PMSManager.h>
#endif


//==================================================================================================
namespace ns_world { namespace services {
;

}}; //end of namespace

//==================================================================================================

ns_world::services::PMSManagerInWorld::PMSManagerInWorld()
    : pms_manager_(NULL)
{
#ifdef _JP_0_20101123_PMS
    pms_manager_ = new ::PMSManager;
#endif
}

ns_world::services::PMSManagerInWorld::~PMSManagerInWorld()
{
#ifdef _JP_0_20101123_PMS
    SAFE_DELETE(pms_manager_);
#endif
}

//
void ns_world::services::PMSManagerInWorld::Run()
{
#ifdef _JP_0_20101123_PMS
    if (pms_manager_) {
        pms_manager_->Run();
    };
#endif
}

void ns_world::services::PMSManagerInWorld::Stop()
{
#ifdef _JP_0_20101123_PMS
    if (pms_manager_) {
        pms_manager_->Stop();
    };
#endif
}


void ns_world::services::PMSManagerInWorld::SetCategory(DWORD category)
{
    __TOUCHED(category);
#ifdef _JP_0_20101123_PMS
    if (pms_manager_) {
        pms_manager_->SetCategory(category);
    };
#endif
}

void ns_world::services::PMSManagerInWorld::SetUserCount(DWORD user_count)
{
    __TOUCHED(user_count);
#ifdef _JP_0_20101123_PMS
    if (pms_manager_) {
        pms_manager_->SetUserCount(user_count);
    };
#endif
}

void ns_world::services::PMSManagerInWorld::SetSessionCount(DWORD session_count)
{
    __TOUCHED(session_count);
#ifdef _JP_0_20101123_PMS
    if (pms_manager_) {
        pms_manager_->SetSessioCount(session_count);
    };
#endif
}

void ns_world::services::PMSManagerInWorld::SetChannelCount(DWORD channel_count)
{
    __TOUCHED(channel_count);
#ifdef _JP_0_20101123_PMS
    if (pms_manager_) {
        pms_manager_->SetChannelCount(channel_count);
    };
#endif
}

void ns_world::services::PMSManagerInWorld::SetRoomCount(DWORD room_count)
{
    __TOUCHED(room_count);
#ifdef _JP_0_20101123_PMS
    if (pms_manager_) {
        pms_manager_->SetRoomCount(room_count);
    };
#endif
}


DWORD ns_world::services::PMSManagerInWorld::GetCategory() const
{
#ifdef _JP_0_20101123_PMS
    if (pms_manager_) {
        return pms_manager_->GetCategory();
    };
#endif
    return 0;
}

DWORD ns_world::services::PMSManagerInWorld::GetUserCount() const
{
#ifdef _JP_0_20101123_PMS
    if (pms_manager_) {
        return pms_manager_->GetUserCount();
    };
#endif
    return 0;
}

DWORD ns_world::services::PMSManagerInWorld::GetSessioCount() const
{
#ifdef _JP_0_20101123_PMS
    if (pms_manager_) {
        return pms_manager_->GetSessioCount();
    };
#endif
    return 0;
}

DWORD ns_world::services::PMSManagerInWorld::GetChannelCount() const
{
#ifdef _JP_0_20101123_PMS
    if (pms_manager_) {
        return pms_manager_->GetChannelCount();
    };
#endif
    return 0;
}

DWORD ns_world::services::PMSManagerInWorld::GetRoomCount() const
{
#ifdef _JP_0_20101123_PMS
    if (pms_manager_) {
        return pms_manager_->GetRoomCount();
    };
#endif
    return 0;
}


DWORD ns_world::services::PMSManagerInWorld::GetStatus() const
{
#ifdef _JP_0_20101123_PMS
    if (pms_manager_) {
        return pms_manager_->GetStatus();
    };
#endif
    return 0;
}


void ns_world::services::PMSManagerInWorld::SetAnnounceRequest(IAnnounceRequest* announce_request)
{
    __TOUCHED(announce_request);
#ifdef _JP_0_20101123_PMS
    if (pms_manager_) {
        pms_manager_->SetAnnounceRequest(announce_request);
    };
#endif
}


void ns_world::services::PMSManagerInWorld::DisplayPMSInfo()
{
#ifdef _JP_0_20101123_PMS
    if (pms_manager_) {
        pms_manager_->DisplayPMSInfo();
    };
#endif
}