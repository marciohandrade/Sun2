#pragma once
#ifdef _DEV_VER
#include "uiBase.h"
#include "uiGMWindowMan/uiGMWindowMan.h"

class uiGMItemOptionWindow : public uiBase
{
public:

    enum         
    {
        kClass_Berserker,
        kClass_DragonKnight,
        kClass_Shadow,
        kClass_Valkyrie,
        kClass_Elementalist,
        kClass_All,
    };

	enum CONTROL_ID
	{
		ITEM_NUMBER_EDIT,
		INIT_BTN,
        BERSERKER_BTN,
        DRAGONKNIGHT_BTN,
        SHADOW_BTN,
        VALKYRIE_BTN,
        ELEMENTALIST_BTN,
		OPTION1_FORWARD_BTN,
		OPTION1_BACKWARD_BTN,
		OPTION2_FORWARD_BTN,
		OPTION2_BACKWARD_BTN,
		OPTION3_FORWARD_BTN,
		OPTION3_BACKWARD_BTN,
        OPTION4_FORWARD_BTN,
        OPTION4_BACKWARD_BTN,
		OPTION5_FORWARD_BTN, 
		OPTION5_BACKWARD_BTN, 

		OPTION1_TEXT,
		OPTION2_TEXT,
		OPTION3_TEXT,
        OPTION4_TEXT,
		OPTION5_TEXT,

		FILTER1_FORWARD_BTN,
		FILTER1_BACKWARD_BTN,

		FILTER2_FORWARD_BTN,
		FILTER2_BACKWARD_BTN,

		FILTER1_TEXT,
		FILTER2_TEXT,

		OK_BTN,
		CANCLE_BTN,

        NUMBER_TEXT,  // 생성 갯수를 입력 하세요

        TITLE_NAME, //StrToWzID("ST00"),
        OPTION_NAME, //StrToWzID("T002")
        FILTER_NAME, //StrToWzID("T006")
        FULL_STRING,//StrToWZID("ST09")
		MAX_CONTROL
	};

	uiGMItemOptionWindow(InterfaceManager* ui_manager);
	virtual ~uiGMItemOptionWindow(void);

	virtual void Init(CDrawBase* drawbase);
	virtual void Release();

	virtual void MessageProc(SI_MESSAGE* message);
	virtual void OnUpdateSolarDialog();
//	void    InitText();

	virtual void OnShowWindow(BOOL val);

	CControlWZ*	getControl(POSTYPE AtPos);
	POSTYPE	getCtrlIDToPos(WzID wzId);

    void SetCaptionSubFilter(TCHAR* caption_string);
    void SetSearch(bool value);
    RECT GetResetControlPosition(RECT* control_rect);
    void InitParametas();
    ITEM_PARAMETAS* item_option_parametas(){ return &item_option_parametas_; }
    void SetItemOptionParametas(ITEM_PARAMETAS* item_parameta);
    void RefreshList(BYTE state);
    void SetItemOptionCaptions(PARAM_NAME parameta_name, TCHAR* clipboard_string = NULL);
    void SetClassType(int class_type);

private:
    void ParametaScroll(PARAM_NAME parameta_name, bool is_down_scroll);
    void OnLButtonClick(SI_MESSAGE* message);
    void ScrollSubFilter(bool is_down_scroll);
    void ScrollMainFilter(bool is_down_scroll);
    //void SetCaptionSubFilter(TCHAR* caption_string);
    void SetCaptionMainFilter(TCHAR* caption_string);
    bool IsChange();
    void SetClass(int class_type);

private:
    uiGMWindowMan* ui_gm_window_manager_;
    ITEM_PARAMETAS item_option_parametas_;

	static	WzID m_wzId[MAX_CONTROL];

    CCtrlStaticWZ* static_edit_control_;
    CCtrlDxEditBox* count_edit_box_;
    CCtrlStaticWZ* full_cheat_string_;
    CCtrlButtonWZ* ok_control_;
    CCtrlButtonWZ* init_control_;
    bool is_search_;
};
#endif//_DEV_VER