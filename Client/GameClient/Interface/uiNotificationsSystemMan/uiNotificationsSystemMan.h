#pragma once

#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM

#include "uiBaseMan.h"

struct IM_NOTIFICATIONS_SYSTEM_MANAGER
{
	static const DWORD NOTIFICATIONS_SYSTEM_DIALOG;
	static const DWORD NOTIFICATIONS_SYSTEM_LIST;
};

struct NotificationInfo_ForClient
{
	TCHAR notification_string[INTERFACE_STRING_LENGTH];
	NotificationInfo info;

	NotificationInfo_ForClient() 
	{
		ZeroMemory(notification_string, INTERFACE_STRING_LENGTH);
	}
};

class uiNotificationsSystemDialog;
class uiNotificationsSystemListDialog;
class uiNotificationsSystemMan : public uiBaseMan
{
public:
	uiNotificationsSystemMan(InterfaceManager* ui_manager);
	~uiNotificationsSystemMan(void);

public:
	virtual void OnInitialize();
	virtual void NetworkProc( MSG_BASE* message );

public:
	void OnRecv_CW_NOTIFICATION_PREV_ACK( MSG_BASE* message );
	void OnRecv_CW_NOTIFICATION_BRD( MSG_BASE* message );

public:
	void ShowNotificationsMessage(const NotificationInfo_ForClient& notification_info, const NotificationType& notification_type);
	void ToggleNotificationsList();
	void CheckNextNotificationsShow();

public:
	void PushBackNotificationsMessage(std::string message, const NotificationInfo& info);
	void PushBackNotificationsListMessage(std::string message, const NotificationInfo& info);
	void ClearTextTextureVector();
	void DeleteNotificationsListBegin();
	WzColor GetNotificationsTextColor(const NotificationInfo& info, const NotificationType& notification_type);

private:
	void ParseNotificationsString(const NotificationInfo& notification_info, OUT TCHAR* out_string);

public:
	std::vector<HANDLE>&        get_text_texture_vector() { return text_texture_vector_; }

private:
	uiNotificationsSystemDialog*          ui_notifications_system_dialog_;
	uiNotificationsSystemListDialog*      ui_notifications_system_list_dialog_;

	std::vector<HANDLE>                   text_texture_vector_;

	std::list<NotificationInfo_ForClient> notifications_list_;
};

#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM
