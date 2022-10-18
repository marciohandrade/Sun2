#pragma once

#include "askdialog.h"
class AskCharAddSlotDialog:
	public AskDialog
{
public:
	enum eENCHANT_PACKET_STATUS 
	{
		NONE		= 0,
        CHAR_ADD_SLOT = 1,
	};

	AskCharAddSlotDialog(InterfaceManager* ui_manager_ptr);
	virtual ~AskCharAddSlotDialog(void);
    void set_item_position(POSTYPE pos){ item_position_ = pos; }
protected:
    virtual void	OnUpdateSolarDialog();
    virtual void   MessageProc(SI_MESSAGE* message);
    virtual void	OnShowWindow(BOOL show_value);
	void			SetTexts();
	void			DoIt();
	void			Cancel();

private:
	CODETYPE			item_code_;
	POSTYPE				item_position_;
	DWORD				packet_status_;
};
