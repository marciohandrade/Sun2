#pragma once
#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
#include "uiBase.h"
#include "FTextParser.h"
#include "FTextSentence.h"
#include "FTextParam.h"
#include "FTextRender.h"

class HeimTradingvolumeSubject;
class uiSystemHeimTradingvolume : public uiBase
{
private:
	enum UIControlPos
	{
		kControl_Title_Text = 0,    // 제목
		kControl_Message_Text,      // 내용
		kControl_Heim_Text,         // 하임량
		kControl_Verify_Button,     // 확인버튼
		kControl_Cancel_Button,     // 취소

		kControl_Size, 
	};

public:
	uiSystemHeimTradingvolume(InterfaceManager* interface_manager);
	virtual ~uiSystemHeimTradingvolume(void);

public:
	virtual void Init(CDrawBase* draw_base);
	virtual void Release();
	virtual void OnShowWindow(BOOL is_show);
	virtual void MessageProc(SI_MESSAGE* messsage);
	virtual void OnUpdateSolarDialog();
	virtual void OnRenderSolarDialog();

	void Clear();
	void SetModal(const bool& is_modal);
	void SetTimer(const DWORD life_time);
	DWORD expired_time() const { return expired_time_; }
	void set_expired_time(const DWORD val) { expired_time_ = val; }
	void set_modal(const bool& is_modal) { is_modal_ = is_modal; }
	bool is_modal() const { return is_modal_; }
	void set_parent_dialog_key_(const DWORD& parent_dialog_key) { parent_dialog_key_ = parent_dialog_key; }
	DWORD get_parent_dialog_key_() { return parent_dialog_key_; }

	static void DoSystemHeimTradingvolume(DWORD l_param, DWORD r_param);

public:
	void SetData(HeimTradingvolumeSubject* heim_tradingvolume_subject);

private:
	bool OnLButtonClick(SI_MESSAGE* message);

	static DWORD& CurrentDialogKey() { return current_dialog_key_; }

	void SettingInfo();

private:
	DWORD expired_time_;
	bool  is_modal_;

	static DWORD current_dialog_key_;

	DWORD parent_dialog_key_;

	FTextParser   ftext_parser_;
	FTextSentence ftext_sentence_;
	FTextParam    ftext_param_;
	FTextRender   ftext_render_;

private:
	HeimTradingvolumeSubject* heim_tradingvolume_subject_;
};

#endif // _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
