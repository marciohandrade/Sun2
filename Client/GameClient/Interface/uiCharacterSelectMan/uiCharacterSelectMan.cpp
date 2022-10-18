#include "SunClientPrecompiledHeader.h"
#include "uicharacterselectman.h"

#include "interfacemanager.h"
#include "CharSelectDialog.h"
#include "CharSelectFontDialog.h"
#ifdef _JP_20110222_INGAME_CHARNAME_CHANGE
#include "uiCharacterRenameDialog/uiCharacterRenameDialog.h"
#include "uiCharacterRenameInputDialog/uiCharacterRenameInputDialog.h"
#endif //_JP_20110222_INGAME_CHARNAME_CHANGE
#include "uiCharacterSelectList/uiCharacterSelectList.h"

#ifdef _NA_000000_20130114_RENEWER_UI
#include "uiCharacterSelectList/uiCharacterSelectList2.h"
#endif //#ifdef _NA_000000_20130114_RENEWER_UI

#include "GlobalFunc.h"

const DWORD IM_CHARACTERSELECT_MANAGER::CHARACTER_SELECT_DIALOG = StrToWzID("0041");
const DWORD IM_CHARACTERSELECT_MANAGER::CHARACTER_SELECT_FONT = StrToWzID("0040");
const DWORD IM_CHARACTERSELECT_MANAGER::CHARACTER_SELECT_RENAME = StrToWzID("0308");
const DWORD IM_CHARACTERSELECT_MANAGER::CHARACTER_SELECT_RENAME_INPUT = g_IDGenerator.GenerateID();
const DWORD IM_CHARACTERSELECT_MANAGER::CHARACTER_SELECT_LIST = StrToWzID("0042");

uiCharacterSelectMan::uiCharacterSelectMan(InterfaceManager *pUIMan):
uiBaseMan(pUIMan)
{
    m_pCharSelectDlg				= NULL;
    m_pCharSelectFontDlg			= NULL;	
}

//------------------------------------------------------------------------------
/**
*/
void	uiCharacterSelectMan::OnInitialize()
{
    char character_select_file_name[MAX_PATH] = {NULL, };

#ifdef _CHINA
    Snprintf(character_select_file_name, MAX_PATH, "%s", "Data\\Interface\\4_1_Character_Select_ch.iwz");
#else
    Snprintf(character_select_file_name, MAX_PATH, "%s", "Data\\Interface\\4_1_Character_Select.iwz");
#endif //_CHINA

    m_pCharSelectDlg = new CharSelectDialog;
    if( !g_InterfaceManager.LoadInterface(m_pCharSelectDlg, character_select_file_name, StrToWzID("0041"), FALSE) )
    {
        ASSERT(false && "load error : Data\\Interface\\4_1_Character_Select.iwz");
    }

    m_pCharSelectFontDlg = new CharSelectFontDialog;
    if( !g_InterfaceManager.LoadInterface( 
        m_pCharSelectFontDlg, 
        "Data\\Interface\\4_Character_Select.iwz", 
        StrToWzID("0040"), 
        FALSE ) )
    {
    }

#ifdef _JP_20110222_INGAME_CHARNAME_CHANGE
    uiCharacterRenameDialog* rename_dialog = CreateUIDialog<uiCharacterRenameDialog>(
        IM_CHARACTERSELECT_MANAGER::CHARACTER_SELECT_RENAME, 
        "Data\\Interface\\3_8_Character_Name_Edit.iwz", 
        this, TRUE);
    assert(rename_dialog);

    uiCharacterRenameInputDialog* rename_input = CreateUIDialog<uiCharacterRenameInputDialog>(
        IM_CHARACTERSELECT_MANAGER::CHARACTER_SELECT_RENAME_INPUT, 
        "Data\\Interface\\87_Input_Message.iwz", 
        this, TRUE, TRUE);
    assert(rename_input);
#endif //_JP_20110222_INGAME_CHARNAME_CHANGE

#ifdef _NA_000000_20130114_RENEWER_UI 
	uiCharacterSelectList2* select_list = CreateUIDialog<uiCharacterSelectList2>(
		IM_CHARACTERSELECT_MANAGER::CHARACTER_SELECT_LIST, 
		"Data\\Interface\\4_2_Character_Select.iwz", 
		this, TRUE);
	assert(select_list);
#else
    uiCharacterSelectList* select_list = CreateUIDialog<uiCharacterSelectList>(
        IM_CHARACTERSELECT_MANAGER::CHARACTER_SELECT_LIST, 
        "Data\\Interface\\4_2_Character_Select.iwz", 
        this, TRUE);
    assert(select_list);
#endif
}

#ifdef _DH_LAST_SELECTED_CHARACTER
LAST_SELECTED_DATA::_LAST_SELECTED_DATA()
{

}

LAST_SELECTED_DATA::~_LAST_SELECTED_DATA()
{

}

void uiCharacterSelectMan::SetLastSelectServer(WORD server_number, WORD channel)
{
    last_selected_header_.last_select_server = server_number;
    last_selected_header_.last_select_channel = channel;
    //last_selected_header_.last_select_version = LAST_SELECT_VERSION;
}

void uiCharacterSelectMan::SetLastSelectCharacter(WORD server_number, WORD select_character)
{
    LAST_SELECTED_MAP::iterator iter = last_selected_data_.find(server_number);
    if (iter != last_selected_data_.end())
    {
        iter->second = select_character;
    }
    else
    {
        last_selected_data_.insert(std::make_pair<SERVER_NUMBER, CHARACTER_INDEX>(server_number, select_character));
    }
}

bool uiCharacterSelectMan::MakeToLastSelectInfo(GameOption& game_option, LAST_SELECTED_MAP& apply_info)
{
    bool is_success = false;
    if (game_option.binary_data_size_ < sizeof(WORD))
    {
        return is_success;
    }

    WORD* stream = (WORD*)(&game_option.binary_data_[0]);
    uint offset = 0;

    uint header_size = sizeof(LAST_SELECTED_HEADER);
    // 버전 얻기
    if (game_option.binary_data_size_ > 0)
    {
        memcpy(&last_selected_header_, game_option.binary_data_, header_size);
    }
    else
    {
        last_selected_header_.last_select_channel = 0;
        last_selected_header_.last_select_server = 0;
        last_selected_header_.last_select_version = 0;//LAST_SELECT_VERSION;
        return is_success;
    }

    offset += header_size;

    last_selected_data_.clear();

    for (;offset <= game_option.binary_data_size_;)
    {
        switch (last_selected_header_.last_select_version)
        {
        case LAST_SELECT_VERSION:
            {
                memcpy(&data_, (game_option.binary_data_ + offset), sizeof(LAST_SELECTED_DATA));
                last_selected_data_.insert(std::make_pair<SERVER_NUMBER, CHARACTER_INDEX>(data_.server_number, data_.character_index));
                offset += sizeof(LAST_SELECTED_DATA);
            }        
            break;
        default:
            // 무한 루프 방지
            break;
        }

    }

#ifdef _NA_000000_20130114_RENEWER_UI 
    uiCharacterSelectList2* select_list_dialog = GET_CAST_DIALOG(uiCharacterSelectList2, IM_CHARACTERSELECT_MANAGER::CHARACTER_SELECT_LIST);
#else
	uiCharacterSelectList* select_list_dialog = GET_CAST_DIALOG(uiCharacterSelectList, IM_CHARACTERSELECT_MANAGER::CHARACTER_SELECT_LIST);
#endif

    if (select_list_dialog != NULL)
    {
        select_list_dialog->CharacterListUp();
    }            
    
    is_success = true;
    return is_success;
}

WORD uiCharacterSelectMan::FindLastSelectedSlotIndex(WORD server_number)
{
    const WORD const_server_number = server_number;
    LAST_SELECTED_MAP::iterator find_iter = last_selected_data_.find(const_server_number);
    if (find_iter != last_selected_data_.end())
    {
        return find_iter->second;
    }
    return 0;
}

void uiCharacterSelectMan::RecvLastSelect(GameOption& game_option)
{
    if (game_option.option_type_ == GameOption::kLastSelectedChar)
    {
        // 계정 세팅
        // 스트림을 풀어서 current에 저장
        if (MakeToLastSelectInfo(game_option, last_selected_data_))
        {
            //
        }
    }
}

void uiCharacterSelectMan::SaveLastSelectedData()
{
    // 스트림으로 변경
    MSG_CG_ETC_GAMEOPTION_SAVE_SYN send_packet;    
    send_packet.game_option_.option_type_ = GameOption::kLastSelectedChar;
    if (MakeToStream(send_packet.game_option_) == true)
    {
        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
    }
}

bool uiCharacterSelectMan::MakeToStream(GameOption& game_option)
{
    WORD* stream = (WORD*)(&game_option.binary_data_[0]);
    uint offset = 0;
    bool is_success = false;

    // 버전 관리
    last_selected_header_.last_select_version = LAST_SELECT_VERSION;
    memcpy(&game_option.binary_data_[0], &last_selected_header_, sizeof(LAST_SELECTED_HEADER));
    offset += sizeof(LAST_SELECTED_HEADER);

    // 키값저장
    LAST_SELECTED_MAP::iterator found_itr = last_selected_data_.begin();
    LAST_SELECTED_MAP::iterator end_itr = last_selected_data_.end();
    for ( ; found_itr != end_itr; ++found_itr)
    {
        LAST_SELECTED_DATA data;
        data.server_number = found_itr->first;
        data.character_index = found_itr->second;
        memcpy(&game_option.binary_data_[offset], &data, sizeof(LAST_SELECTED_DATA));
        offset += sizeof(LAST_SELECTED_DATA);
        is_success = true;
    }
    game_option.binary_data_size_ = offset;
    return is_success;
}

void uiCharacterSelectMan::SEND_SELECT_CHARACTER_DATA_GAMEOPTION_LOAD_SYN()
{
    MSG_CG_ETC_GAMEOPTION_LOAD_SYN sync;
    sync.option_type_ = GameOption::kLastSelectedChar;
    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sizeof(sync));
}
#endif//_DH_LAST_SELECTED_CHARACTER