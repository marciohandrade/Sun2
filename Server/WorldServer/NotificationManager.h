#pragma once
#ifndef SERVER_WORLDSERVER_NOTIFICATIONMANAGER_H
#define SERVER_WORLDSERVER_NOTIFICATIONMANAGER_H

#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM 

#include <Singleton.h>
#include <list>

class NotificationManager : public util::Singleton<NotificationManager>
{
public:
    NotificationManager();
    ~NotificationManager();

    void ProcessNotification(NotificationInfo& notification);
    void GetPrevNotifications(MSG_CW_NOTIFICATION_PREV_ACK& msg);
    void ClearNotification();

private:
    void AddNotification(NotificationInfo& notification);
    void SendNotification(NotificationInfo& notification);

    std::list<NotificationInfo> m_notifications;
};

#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM

#endif // SERVER_WORLDSERVER_NOTIFICATIONMANAGER_H