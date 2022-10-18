#pragma once
#ifndef WORLDSERVER_PMS_MANAGER_IN_WORLD_H
#define WORLDSERVER_PMS_MANAGER_IN_WORLD_H

//
class PMSManager; // include/pms/PMSManager.h
class IAnnounceRequest;
//

namespace ns_world { namespace services {
;

// PMSManager (for japan) gateway
class PMSManagerInWorld
{
public:
    PMSManagerInWorld();
    ~PMSManagerInWorld();
    //
    void Run();
    void Stop();

    void SetCategory(DWORD category);
    void SetUserCount(DWORD user_count);
    void SetSessionCount(DWORD session_count);
    void SetChannelCount(DWORD channel_count);
    void SetRoomCount(DWORD room_count);

    DWORD GetCategory() const;
    DWORD GetUserCount() const;
    DWORD GetSessioCount() const;
    DWORD GetChannelCount() const;
    DWORD GetRoomCount() const;

    DWORD GetStatus() const;

    void SetAnnounceRequest(IAnnounceRequest* announce_request);

    void DisplayPMSInfo();
private:
    PMSManager* pms_manager_;
    //
    __DISABLE_COPY(PMSManagerInWorld);
};

}}; //end of namespace

#endif //WORLDSERVER_PMS_MANAGER_IN_WORLD_H