#pragma once

#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM

#include "uiBase.h"
#include "uiNotificationsSystemMan/uiNotificationsSystemMan.h"

class uiNotificationsSystemMan;
class uiNotificationsSystemDialog : public uiBase
{
	enum eDIALOG_POS
	{
		DLGPOS_NOTIFICATIONS_LIST, 
		DLGPOS_NOTIFICATIONS_SPECIAL_LIST, 
		DLGPOS_NOTIFICATIONS_TEXT, 
		DLGPOS_EYE_BTN, 

		DIALOG_MAX, 
	};
public:
	uiNotificationsSystemDialog(InterfaceManager* ui_manager);
	~uiNotificationsSystemDialog(void);

public:
	virtual void Init(CDrawBase* draw_base);
	virtual void Release();

	virtual void OnShowWindow(BOOL is_show);
	virtual void MessageProc(SI_MESSAGE* message);

public:
	void MoveDefaultPosition();
	void SetNotificationsString(TCHAR* notifications_string);
	void ResetNotificationsTextRect();
	void ResetNotificationsTime();
	void ResetUIType();
	void SpecialTextUIShow();

public:
	virtual void	MoveDefaultWindowPos();

private:
	virtual void OnUpdateSolarDialog();
	virtual void OnRenderSolarDialog();

protected:
	POSTYPE getCtrlIDToPos( WzID wzId );

private:
	uiNotificationsSystemMan*    notifications_system_manager_;
	static WzID                  wz_id_[DIALOG_MAX];
	CCtrlListWZ*                 notifications_text_control_;
	CCtrlListWZ*                 notifications_special_text_control_;
	CCtrlButtonWZ*               eye_button_control_;
	RECT                         notifications_text_rect_;
	BOOL                         is_show_;
	float                        move_offset_;
	float                        second_move_offset_;
	bool                         is_second_move_;
};
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM
