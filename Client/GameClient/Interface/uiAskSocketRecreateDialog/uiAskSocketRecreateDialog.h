#pragma once

#include "askdialog.h"
class AskSocketRecreateDialog:
	public AskDialog
{
public:
	enum eENCHANT_PACKET_STATUS 
	{
		NONE		    = 0,
		RECREATE_SOCKET	= 1,
	};

	AskSocketRecreateDialog(InterfaceManager* ui_manager_ptr);
	virtual ~AskSocketRecreateDialog(void);
    virtual void   Process(DWORD tick_count);
	void			Initialize();
    void set_is_socket_recreate(bool val){ is_socket_recreate_ = val; }
    bool is_socket_recreate(){ return is_socket_recreate_; }
protected:
    virtual void	OnUpdateSolarDialog();
    virtual void   MessageProc(SI_MESSAGE* message);
    virtual void	OnShowWindow(BOOL show_value);
	void			SetTexts();
	void			DoIt();
	void			Cancel();

public:
	void			SetInfo(CODETYPE item_code, POSTYPE recreate_item_positeion_, POSTYPE target_position_);
    CODETYPE        item_code(){ return item_code_; }

private:
	CODETYPE			item_code_;
	POSTYPE				target_position_;
	POSTYPE				recreate_item_positeion_;
	DWORD				packet_status_;
    bool is_socket_recreate_;
};
