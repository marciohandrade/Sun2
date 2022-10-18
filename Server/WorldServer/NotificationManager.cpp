#include "stdafx.h"
#include "NotificationManager.h"
#include "PacketStruct_CW.h"
#include "UserManager.h"
#include "NotificationInfoParser.h"

#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM 

NotificationManager::NotificationManager()
{

}

NotificationManager::~NotificationManager()
{

}

void NotificationManager::ProcessNotification( NotificationInfo& notification )
{
    if (NotificationInfoParser::Instance()->IsSaveNotification(notification.m_item_code) == true)
    {
        AddNotification(notification);
    }

    SendNotification(notification);

    GAMELOG->LogNotification(notification);
}

void NotificationManager::AddNotification( NotificationInfo& notification )
{
    // 지정된 개수보다 크면 첫번째 데이터를 삭제한다
    if (m_notifications.size() >= MAX_NOTIFICATION_COUNT)
    {
        std::list<NotificationInfo>::iterator iter = m_notifications.begin();
        if (iter != m_notifications.end())
        {
            m_notifications.erase(iter);
        }
    }

    m_notifications.push_back(notification);
}

void NotificationManager::GetPrevNotifications( MSG_CW_NOTIFICATION_PREV_ACK& msg )
{
    std::list<NotificationInfo>::iterator cur_iter = m_notifications.begin();
    std::list<NotificationInfo>::iterator end_iter = m_notifications.end();
    int i = 0;
    for (; cur_iter != end_iter && i < MAX_NOTIFICATION_COUNT; ++cur_iter, ++i)
    {
        msg.infos[i] = *cur_iter;
    }
    msg.count = i;
}

void NotificationManager::SendNotification( NotificationInfo& notification )
{
    MSG_CW_NOTIFICATION_BRD msg;
    msg.info = notification;
    UserManager::Instance()->SendToAll( (BYTE*)&msg, (WORD)sizeof(MSG_CW_NOTIFICATION_BRD) );
}

void NotificationManager::ClearNotification()
{
    m_notifications.clear();
}

#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM