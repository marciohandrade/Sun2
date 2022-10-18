#pragma once
#include "uibaseman.h"

#define LAST_SELECT_VERSION 1

struct IM_CHARACTERSELECT_MANAGER
{
    static const DWORD CHARACTER_SELECT_DIALOG;
    static const DWORD CHARACTER_SELECT_FONT;
    static const DWORD CHARACTER_SELECT_RENAME;
    static const DWORD CHARACTER_SELECT_RENAME_INPUT;
    static const DWORD CHARACTER_SELECT_LIST;
};

#ifdef _DH_LAST_SELECTED_CHARACTER
typedef WORD SERVER_NUMBER;
typedef WORD CHARACTER_INDEX;
typedef std::map<SERVER_NUMBER, CHARACTER_INDEX> LAST_SELECTED_MAP;

typedef struct _LAST_SELECTED_HEADER
{
    DWORD last_select_version;
    WORD last_select_server;
    WORD last_select_channel;
}LAST_SELECTED_HEADER;

typedef struct _LAST_SELECTED_DATA
{
    _LAST_SELECTED_DATA();
    ~_LAST_SELECTED_DATA();
    WORD server_number;
    WORD character_index;
}LAST_SELECTED_DATA;
#endif//_DH_LAST_SELECTED_CHARACTER

class CharSelectDialog ;
class CharSelectFontDialog;
class uiCharacterSelectMan :
	public uiBaseMan
{
public:
	uiCharacterSelectMan(InterfaceManager *pUIMan);
    
	//--------------------------------------------------------------------------
    // init/release
    //
public:
    virtual void            OnInitialize();
#ifdef _DH_LAST_SELECTED_CHARACTER
    bool MakeToLastSelectInfo(GameOption& game_option, LAST_SELECTED_MAP& apply_info);
    void RecvLastSelect(GameOption& game_option);
    bool MakeToStream(GameOption& game_option);
    void SEND_SELECT_CHARACTER_DATA_GAMEOPTION_LOAD_SYN();
    void SetLastSelectServer(WORD server_number, WORD channel);
    void SetLastSelectCharacter(WORD server_number, WORD select_character);
    LAST_SELECTED_HEADER last_selected_header(){ return last_selected_header_; }
    LAST_SELECTED_MAP last_selected_data(){ return last_selected_data_; }
    WORD FindLastSelectedSlotIndex(WORD server_number);
    void SaveLastSelectedData();
#endif//_DH_LAST_SELECTED_CHARACTER

private:
	CharSelectDialog *		m_pCharSelectDlg;
	CharSelectFontDialog *	m_pCharSelectFontDlg;

#ifdef _DH_LAST_SELECTED_CHARACTER
    LAST_SELECTED_HEADER last_selected_header_;
    LAST_SELECTED_MAP last_selected_data_;
    LAST_SELECTED_DATA data_;
#endif//_DH_LAST_SELECTED_CHARACTER
};
