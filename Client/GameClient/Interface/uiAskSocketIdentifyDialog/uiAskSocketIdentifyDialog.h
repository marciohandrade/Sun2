#pragma once

#include "askdialog.h"
class AskSocketIdentifyDialog:
	public AskDialog
{
public:
	enum eENCHANT_PACKET_STATUS 
	{
		NONE		= 0,
		IDENTIFY_SOCKET	= 1,
	};

	AskSocketIdentifyDialog(InterfaceManager* ui_manager_ptr);
	virtual ~AskSocketIdentifyDialog(void);
    virtual void   Process(DWORD tick_count);
	void			Initialize();
    void set_is_socket_identify(bool val){ is_socket_identify_ = val; }
    bool is_socket_identify(){ return is_socket_identify_; }
protected:
    virtual void	OnUpdateSolarDialog();
    virtual void   MessageProc(SI_MESSAGE* message);
    virtual void	OnShowWindow(BOOL show_value);
	void			SetTexts();
	void			DoIt();
	void			Cancel();

public:
	void			SetInfo(CODETYPE item_code, POSTYPE Identify_item_positeion, POSTYPE target_position_);

private:
	CODETYPE			item_code_;
	POSTYPE				target_position_;
	POSTYPE				Identify_item_positeion_;
	DWORD				packet_status_;
    bool is_socket_identify_;
};
