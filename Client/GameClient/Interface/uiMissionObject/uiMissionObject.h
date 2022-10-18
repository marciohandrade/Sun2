#pragma once

#include "uibase.h"
#include "FTextParser.h"
#include "FTextSentence.h"
#include "FTextParam.h"
#include "FTextRender.h"
#include "MissionInfoParser.h"

#define MISSION_OBJECT_DELAY 5000

class uiMissionObject : public uiBase
{
	enum eDIALOG_POS
	{
		eDIALOG_MISSION_OBJECT_LIST,
		eDIALOG_EXIT,
		eDIALOG_VSCROLL,
        eDIALOG_TITTLE,
        eDIALOG_ALL_BUTTON,
        eDIALOG_PLAY_BUTTON,
        eDIALOG_COMPLETE_BUTTON,
		DLGPOS_MAX,
	};

    enum eDisplayType
    {
        TYPE_ALL,
        TYPE_PLAYING,
        TYPE_COMPLETE,
        TYPE_BUTTON_MAX,
    };

public:
	uiMissionObject(InterfaceManager* ui_manager);
	~uiMissionObject();
	virtual void		Init(CDrawBase* draw_base);
	virtual void		Release();
	virtual void		OnUpdateSolarDialog();
	virtual void		OnRenderSolarDialog();
	virtual void		Process(DWORD tick_count);	
	virtual void		OnShowWindow(BOOL open_value);
	virtual void		MessageProc(SI_MESSAGE* message);
	virtual void		NetworkProc(MSG_BASE* message);
    virtual void	    MouseOver_MsgProc(SI_MESSAGE* message);
    virtual void      UpdatePosition();
    bool MissionObjectComplete(DWORD trigger_id);
    void CheckMissionObjectShow(DWORD trigger_id);
    void SetMissionObjectMapCode(DWORD map_code, DWORD field_code);
protected:
	CControlWZ*		get_control(POSTYPE AtPos);
	POSTYPE			get_control_id_to_pos(WzID wzID);
private:
    void MissionDataReflash();
    void InitMissionList();
    bool AddFormatText(const TCHAR* message);
    void updateMouseWheel();
    void InitLineText();
    void ResetButton();
    void UpdateToolTip();
    void MissionNotice();

    static WzID		wz_control_id_[DLGPOS_MAX];

    MISSION_OBJECT_LIST mission_object_all_;

	CCtrlListWZ*  mission_object_list_;
    CCtrlStaticWZ* mission_title_;
    CCtrlVScrollWZ* vertical_scroll_;
    CCtrlButtonWZ* type_button_[TYPE_BUTTON_MAX];

    FTextRender	ftext_render_;
    FTextSentence ftext_sentence_;
    FTextParser ftext_parser_;
    FTextParam ftext_param_;
    int display_type_;
    DWORD map_code_;
    DWORD field_code_;
    DWORD name_code_;

    TCHAR line_string_[INTERFACE_STRING_LENGTH];
    TCHAR complete_string_[INTERFACE_STRING_LENGTH];
    DWORD tooltip_string_code;
    bool line_update_;

    DWORD mission_delay_tick_count_;
    bool reflash_line_convert_;
};

inline CControlWZ* uiMissionObject::get_control(POSTYPE AtPos)
{
	assert(AtPos < DLGPOS_MAX);
	return GetControlWZ(wz_control_id_[AtPos]);
}

inline POSTYPE uiMissionObject::get_control_id_to_pos(WzID wzID)
{
	for ( int i = 0; i < DLGPOS_MAX ; ++i)
	{
		if ( wz_control_id_[i] == wzID)
			return i;
	}
	return DLGPOS_MAX;
}
