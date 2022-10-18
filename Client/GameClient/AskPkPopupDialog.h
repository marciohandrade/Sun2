#pragma once

#include "askdialog.h"
#include "uiLoginMan\uiLoginMan_def.h"


class AskPkPopupDialog: public AskDialog
{
public:
	AskPkPopupDialog(InterfaceManager* ui_man);
	virtual ~AskPkPopupDialog(void);

	POSTYPE ctrl_id_pos() const { return ctrl_id_pos_; }
	void	set_ctrl_id_pos(POSTYPE pos) { ctrl_id_pos_ = pos; }

	int		server_index() const { return server_index_; }
	void	set_server_index(int index) { server_index_ = index; }

    AuthSetverData* auth_server_data() const { return auth_server_data_; }
    void set_auth_server_data(AuthSetverData* val) { auth_server_data_ = val; }

protected:
	virtual void OnUpdateSolarDialog();
	virtual void MessageProc(SI_MESSAGE* pMessage);
	virtual VOID OnShowWindow(BOOL val);

	void SetTexts();
	void DoIt();
	void Cancel();

private:
	POSTYPE ctrl_id_pos_;
	int		server_index_;

    AuthSetverData* auth_server_data_;
};
