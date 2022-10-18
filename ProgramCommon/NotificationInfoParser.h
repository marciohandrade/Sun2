#pragma once

#ifndef PRAGRAMCOMMON_NOTIFICATION_PARSER_H
#define PRAGRAMCOMMON_NOTIFICATION_PARSER_H
#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM

#include <Singleton.h>
#include <ScriptCode.IParser.h>
#include <SCItemSlot.h>
#include "Struct.h"

struct NotificationParserStruct
{
    SLOTCODE item_code;
    BYTE     enchant;
	DWORD    enchant_color;
    BYTE     awakening;
	DWORD    awakening_color;
    bool     drop;
	DWORD    drop_color;
    bool     craft;
	DWORD    craft_color;
    bool     evolution;
	DWORD    evolution_color;
    bool     box;
	DWORD    box_color;
    bool     ac_reward;
	DWORD    ac_reward_color;
    bool     mission_reward;
	DWORD    mission_reward_color;
	bool     bingo_reward;
	DWORD    bingo_reward_color;
    bool     save;
    BYTE     ui_type;
};

class NotificationInfoParser : public util::Singleton<NotificationInfoParser>, public IParser
{
public:
    NotificationInfoParser(void){};
    ~NotificationInfoParser(void){};

public:
    /** Parser **/
    virtual void Release(){};
    virtual	BOOL LoadScript(eSCRIPT_CODE script_code, BOOL is_reload);

    const bool IsNotificationItem(const SCItemSlot& item_slot, NotificationType notification_type);
    const bool IsNotificationItem(SLOTCODE item_code, NotificationType notification_type,
        BYTE enchant, BYTE awakening);
    const bool IsSaveNotification(SLOTCODE item_code);
    const BYTE GetUIType(SLOTCODE item_code);
    const NotificationParserStruct* GetNotification(SLOTCODE item_code);

private:
    const bool IsValidNotification(NotificationParserStruct& notification_info);

    typedef std::map<SLOTCODE, NotificationParserStruct> NOTIFICATION_MAP;
    NOTIFICATION_MAP m_notification_map;
};

#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM
#endif // PRAGRAMCOMMON_NOTIFICATION_PARSER_H