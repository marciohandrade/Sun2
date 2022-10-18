#pragma once

#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM

#include "uiBase.h"
#include "VScrollDialog.h"

#define MAX_SHOW_NOTIFICATIONS_LINE 13

class uiNotificationsSystemMan;
class uiNotificationsSystemListDialog : public uiBase, public VScrollDialog
{
	enum eDIALOG_POS
	{
		DLGPOS_NOTIFICATIONS_LIST, 
		DLGPOS_NOTIFICATIONS_VSCROLL, 

		DIALOG_MAX, 
	};

public:
	uiNotificationsSystemListDialog(InterfaceManager* ui_manager);
	~uiNotificationsSystemListDialog(void);

public:
	virtual void Init(CDrawBase* draw_base);
	virtual void Release();

	virtual void OnShowWindow(BOOL is_show);
	virtual void MessageProc(SI_MESSAGE* message);

	virtual void MoveDefaultWindowPos();

	virtual void OnUpdateSolarDialog();
	virtual void OnRenderSolarDialog();

protected:
	CCtrlVScrollWZ* vscroll_control();
	bool GetScrollArea(RECT& area);
	bool OnVScrollThumb(SI_MESSAGE* message);
	bool OnVScrollUp(SI_MESSAGE* message);
	bool OnVScrollDown(SI_MESSAGE* message);

private:
	void         UpdateScrollPosition();
	void         UpdateScrollBar();

private:
	void         MoveDefaultPosition();

protected:
	CControlWZ*  getControl(POSTYPE at_pos);
	POSTYPE      getCtrlIDToPos(WzID wzId);

public:
	void         PushBackNotificationsMsg(std::string notifications_message, const NotificationType& notification_type, const NotificationInfo& info);
	void         AddString(TCHAR* message, const NotificationType& notification_type, const NotificationInfo& info);
	void         ClearList();

private:
	BOOL                      is_show_;
	static WzID                wz_id_[DIALOG_MAX];
	CCtrlListWZ*               notifications_list_;
	CCtrlVScrollWZ*            notifications_vscroll_;
	std::deque<std::string>    notifications_deque_;
	int                        current_scroll_value_;
	int                        max_scroll_value_;
	uiNotificationsSystemMan*  notifications_system_manager_;
};

inline
CControlWZ *
uiNotificationsSystemListDialog::getControl( POSTYPE at_pos )
{
	ASSERT( at_pos < DIALOG_MAX );

	return GetControlWZ( wz_id_[at_pos] );
}

#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM
