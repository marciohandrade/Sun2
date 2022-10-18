#include "SunClientPrecompiledHeader.h"

#ifdef _DEV_VER
#include "uiGMWindowMan/uiGMWindowMan.h"
#include "uiGMItemWindow/uiGMItemWindow.h"
#include "uiGMItemOptionWindow/uiGMItemOptionWindow.h"
#include "uiGMItemResetWindow/uiGMItemResetWindow.h"
#include "uiGMItemMenuWindow/uiGMItemMenuWindow.h"
#ifdef _DH_GM_NPC_WINDOW
#include "uiGMNpcWindow/uiGMNpcWindow.h"
#endif//_DH_GM_NPC_WINDOW
#include "interfacemanager.h"
#include "GlobalFunc.h"
#include "ChatDialog.h"
#include "uiChatMan/uiChatMan_def.h"

const DWORD IM_GM_WINDOW_MANAGER::GM_ITEM_WINDOW = g_IDGenerator.GenerateID();
const DWORD IM_GM_WINDOW_MANAGER::GM_ITEM_OPTION_WINDOW = g_IDGenerator.GenerateID();
const DWORD IM_GM_WINDOW_MANAGER::GM_ITEM_RESET_WINDOW = g_IDGenerator.GenerateID();
const DWORD IM_GM_WINDOW_MANAGER::GM_ITEM_MENU_WINDOW = g_IDGenerator.GenerateID();
#ifdef _DH_GM_NPC_WINDOW
const DWORD IM_GM_WINDOW_MANAGER::GM_ITEM_NPC_WINDOW = g_IDGenerator.GenerateID();
#endif//_DH_GM_NPC_WINDOW

#ifdef _NA_002884_AC_DIALOG_RENEWAL
const char c_strDlgFname_GMItemWindow[] = ("Data\\Interface\\92_3_AC.iwz");
#else
const char c_strDlgFname_GMItemWindow[] = ("Data\\Interface\\92_1_AC.iwz");
#endif //_NA_002884_AC_DIALOG_RENEWAL
const char c_strDlgFname_GMItemOptionWindow[] = ("Data\\Interface\\3_6_Character_Create_Left.iwz");
const char c_strDlgFname_GMItemResetWindow[] = ("Data\\Interface\\27_1_Personal_Option.iwz");
const char c_strDlgFname_GMItemMenuWindow[] = ("Data\\Interface\\27_1_Personal_Option.iwz");
const char c_strDlgFname_GMNpcWindow[] = ("Data\\Interface\\61_2_BattleZone_List_new.iwz");

TCHAR* parser_string[uiGMWindowMan::kParser_Unknown] = {"Data", "[OPTION]\n", "[HISTORYMAX]\n", "[BOOKMARKMAX]\n",
    "[STATE]\n", "[HISTORY]\n", "[BOOKMARK]\n", "[FINDWORD]\n" };

uiGMWindowMan::uiGMWindowMan(InterfaceManager *pUIMan):
uiBaseMan(pUIMan)
{
    ui_gm_item_window_ = NULL;
    ui_gm_item_option_window_ = NULL;
    ui_gm_item_reset_window_ = NULL;
    ui_gm_item_menu_window_ = NULL;
#ifdef _DH_GM_NPC_WINDOW
    ui_gm_npc_window_ = NULL;
#endif//_DH_GM_NPC_WINDOW
}

uiGMWindowMan::~uiGMWindowMan(void)
{
    FILE* file_ptr = fopen("C:\\GMItem.dat", "w+");
    SaveData(file_ptr);
    if (file_ptr)
    {
        fclose(file_ptr);
    }

	//Release();
}

//------------------------------------------------------------------------------
/**
*/
void uiGMWindowMan::OnInitialize()
{
    LoadItemData();

    // GM 아이템 시작
    ui_gm_item_window_ = CreateUIDialog<uiGMItemWindow>(IM_GM_WINDOW_MANAGER::GM_ITEM_WINDOW,
         c_strDlgFname_GMItemWindow, this, TRUE, TRUE);
    assert(ui_gm_item_window_);

    ui_gm_item_option_window_ = CreateUIDialog<uiGMItemOptionWindow>(IM_GM_WINDOW_MANAGER::GM_ITEM_OPTION_WINDOW,		
        c_strDlgFname_GMItemOptionWindow, this, TRUE, TRUE);
    assert(ui_gm_item_option_window_);

    ui_gm_item_reset_window_ = CreateUIDialog<uiGMItemResetWindow>(IM_GM_WINDOW_MANAGER::GM_ITEM_RESET_WINDOW,
        c_strDlgFname_GMItemResetWindow, this, TRUE, TRUE);
    assert(ui_gm_item_reset_window_);

    ui_gm_item_menu_window_ = CreateUIDialog<uiGMItemMenuWindow>(IM_GM_WINDOW_MANAGER::GM_ITEM_MENU_WINDOW,
        c_strDlgFname_GMItemMenuWindow, this, TRUE, TRUE);
    assert(ui_gm_item_menu_window_);

#ifdef _DH_GM_NPC_WINDOW
    // GM NPC 시작
    ui_gm_npc_window_ = CreateUIDialog<uiGMNpcWindow>(IM_GM_WINDOW_MANAGER::GM_ITEM_NPC_WINDOW,
        c_strDlgFname_GMNpcWindow, this, TRUE, TRUE);
    assert(ui_gm_npc_window_);
#endif//_DH_GM_NPC_WINDOW
}

void uiGMWindowMan::ResetItemParametas()
{
    item_parametas_.item_code = 0;
    item_parametas_.item_count = 0;
    item_parametas_.overlapping = true;
    item_parametas_.item_type = ITEM_TYPE_NORMAL;
    item_parametas_.enchant_level = 0;
    item_parametas_.socket_count = 0;
    item_parametas_.filter_main = FILTER_ALL;
    item_parametas_.class_type = eCHAR_NONE;
    for (int i = 0; i < FILTER_MAX; ++i)
    {
        item_parametas_.filter_subs[i] = 0;
    }
}

void uiGMWindowMan::LoadItemData()
{
    history_max_ = kDefault_History_Max;
    find_word_max_ = kFind_Word_Max;
    bookmark_max_ = kBookmark_Max;

    ResetItemParametas();

    TCHAR string_buffer[INTERFACE_STRING_LENGTH] = {0,};
    FILE* gm_item_file_ptr = fopen("C:\\GMItem.dat", "r");
    BYTE parser_type = kParser_Unknown;
    BYTE current_parser_type = kParser_Unknown;
    if (gm_item_file_ptr != NULL)
    {        
        while (fgets(string_buffer, INTERFACE_STRING_LENGTH, gm_item_file_ptr) != NULL)
        {
            parser_type = GMItemOptionParserType(string_buffer);
            if (parser_type == kParser_NoParser)
            {
                PushItemOptionData(current_parser_type, string_buffer);
            }
            else
            {
                current_parser_type = parser_type;
            }
        }       
        fclose(gm_item_file_ptr);
    }    
}

void uiGMWindowMan::PushItemOptionData(BYTE parser_type, TCHAR* string_buffer)
{
    DWORD number = atoi(string_buffer);
    switch (parser_type)
    {
    case kParser_Option:
        sscanf(string_buffer, "%d %d %d %d %d", &item_parametas_.item_count, &item_parametas_.overlapping,
            &item_parametas_.item_type, &item_parametas_.enchant_level, &item_parametas_.socket_count);
        break;
    case kParser_HistoryMax:
        history_max_ = number;
        break;
    case kParser_BookMarkMax:
        bookmark_max_ = number;
        break;
    case kParser_Sate:
        item_window_state_ = static_cast<BYTE>(number);
        break;
    case kParser_History:
        if (number != 0)
        {
            created_histroys_.push_back(number);// 중복체크 없이 다이렉트로 넣는다.
        }
        break;
    case kParser_BookMark:
        if (number != 0)
        {
            bookmark_lists_.push_back(number);// 중복체크 없이 다이렉트로 넣는다.
        }
        break;
    case kParser_FindWord:
        find_words_.push_back(string_buffer);
        break;
    default:
        break;
    }
}

void uiGMWindowMan::SaveData(FILE* file_ptr)
{
    if (file_ptr == NULL)
    {
        return;
    }

    DWORD data = 0;

    // 옵션 저장
    fprintf(file_ptr, "%s", parser_string[kParser_Option]);
    fprintf(file_ptr, "%d %d %d %d %d\n", item_parametas_.socket_count, item_parametas_.overlapping,
        item_parametas_.item_type, item_parametas_.enchant_level, item_parametas_.socket_count);

    // 히스토리 최대값 저장
    fprintf(file_ptr, "%s", parser_string[kParser_HistoryMax]);
    fprintf(file_ptr, "%d\n", history_max_);

    // 즐겨찾기 최대값 저장
    fprintf(file_ptr, "%s", parser_string[kParser_BookMarkMax]);
    fprintf(file_ptr, "%d\n", bookmark_max_);

    // 상태 저장
    // ...

    std::vector<DWORD>::iterator iter = created_histroys_.begin();
    std::vector<DWORD>::iterator iter_end = created_histroys_.end();

    // 히스토리 저장
    fprintf(file_ptr, "%s", parser_string[kParser_History]);
    size_t histroy_size = created_histroys_.size();

    DWORD count = 0;
    if (histroy_size > history_max_) //히스토리는 최근 50건(history_max_)만큼만 저장
    {
        count = histroy_size - history_max_;//시작값을 설정
    }
    for (DWORD i = count; i < histroy_size; ++i)
    {
        fprintf(file_ptr, "%d\n", created_histroys_[i]);  // 데이터 저장
    }

    iter = bookmark_lists_.begin();
    iter_end = bookmark_lists_.end();

    // 즐겨찾기 저장
    fprintf(file_ptr, "%s", parser_string[kParser_BookMark]);
    while (iter != iter_end)
    {
        data = (*iter);
        fprintf(file_ptr, "%d\n", data); // 데이터 저장
        ++iter;
    }

    // 검색어 저장: 일단 기능 제거 사용안함(문제가 있음)
    /*std::vector<std::string>::iterator iter_str = find_words_.begin();
    std::vector<std::string>::iterator iter_str_end = find_words_.end();
    std::string string;
    fprintf(file_ptr, "%s", parser_string[kParser_FindWord]);
    while (iter_str != iter_str_end)
    {
        string = (*iter_str);
        fprintf(file_ptr, "%s", string.c_str()); // 데이터 저장
        ++iter_str;
    }*/
}

BYTE uiGMWindowMan::GMItemOptionParserType(TCHAR *string_buffer)
{
    BYTE parser_type = kParser_Unknown;
    // 파싱스테이트에 따라서 다르게 넣기
    if (string_buffer[0] == '[') // 옵션 구문인가?
    {
        for (int i = kParser_Option; i < kParser_Unknown; ++i)
        {
            if (strcmp(string_buffer, parser_string[i]) == 0)
            {
                parser_type =  i;
                break;
            }
        }
    }
    else
    {
        parser_type = kParser_NoParser; // 파서가 아님
    }
    return parser_type;
}

uiBase* uiGMWindowMan::GetGMWindow(DWORD gm_window_id)
{
    uiBase* ui_gm_window_ptr = NULL;
    switch(gm_window_id)
    {
    case UI_GM_ITEM_WINDOW:// GM 아이템 윈도우
        ui_gm_window_ptr = (uiBase*) ui_gm_item_window_;
        break;
    case UI_GM_ITEM_OPTION_WINDOW:// GM 아이템 옵션 윈도우
        ui_gm_window_ptr = (uiBase*) ui_gm_item_option_window_;
        break;
    case UI_GM_ITEM_RESET_WINDOW:// GM 아이템 초기화 윈도우
        ui_gm_window_ptr = (uiBase*) ui_gm_item_reset_window_;
        break;
    case UI_GM_ITEM_MENU_WINDOW:// GM 아이템 메뉴 윈도우
        ui_gm_window_ptr = (uiBase*) ui_gm_item_menu_window_;
        break;
#ifdef _DH_GM_NPC_WINDOW
    case UI_GM_ITEM_NPC_WINDOW:// GM 엔피시 윈도우
        ui_gm_window_ptr = (uiBase*) ui_gm_npc_window_;
#endif//_DH_GM_NPC_WINDOW
    default:
        break;
    }
    return ui_gm_window_ptr;
}
void uiGMWindowMan::ToggleGMWindow(DWORD gm_window_id)
{
    uiBase* ui_gm_window_ptr = GetGMWindow(gm_window_id);
    if (ui_gm_window_ptr)
    {
        BOOL is_show = ui_gm_window_ptr->IsVisible();
        ui_gm_window_ptr->ShowInterface((is_show != TRUE));
    }
}

bool uiGMWindowMan::IsShowGMWindow(DWORD gm_window_id)
{
    uiBase* ui_gm_window_ptr = GetGMWindow(gm_window_id);
    bool is_show = false;
    if (ui_gm_window_ptr)
    {
        is_show = (ui_gm_window_ptr->IsVisible() == TRUE);
    }
    return is_show;
}

void uiGMWindowMan::ShowGMWindow(DWORD gm_window_id, bool value)
{
    uiBase* ui_gm_window_ptr = GetGMWindow(gm_window_id);
    if (ui_gm_window_ptr)
    {
        ui_gm_window_ptr->ShowInterface(value?TRUE:FALSE);
    }
}

void uiGMWindowMan::SetCaptionFilter()
{
    TCHAR caption_string[128] = {0,};
    if (ui_gm_item_option_window_)
    {
        ui_gm_item_option_window_->SetCaptionSubFilter(caption_string);
    }
    if (ui_gm_item_window_)
    {
        if (ui_gm_item_window_)
        {
            ui_gm_item_window_->SetFilterName(caption_string);
        }
    }
}

bool uiGMWindowMan::IsSearching()
{
    if (ui_gm_item_window_)
    {
        return !(ui_gm_item_window_->search_end());
    }
    return false;
}

void uiGMWindowMan::SetSearchEnd()
{
    if (ui_gm_item_option_window_)
    {
        ui_gm_item_option_window_->SetSearch(false);
    }
}

RECT uiGMWindowMan::GetResetWindowPosition(RECT *control_rect)
{
    if (ui_gm_item_option_window_)
    {
        ui_gm_item_option_window_->GetResetControlPosition(control_rect);
    }
    return (*control_rect);
}

ITEM_PARAMETAS* uiGMWindowMan::GetItemOption()
{
    ITEM_PARAMETAS* item_parameta = NULL;
    if (ui_gm_item_option_window_)
    {
        item_parameta = ui_gm_item_option_window_->item_option_parametas();
    }
    return item_parameta;
}

void uiGMWindowMan::SetItemOptionParametas(ITEM_PARAMETAS* item_parameta)
{
    if (ui_gm_item_option_window_)
    {
        ui_gm_item_option_window_->SetItemOptionParametas(item_parameta);
    }
}

void uiGMWindowMan::SetPopupUpdateMenus(BASE_ITEMINFO *item_info, bool is_bookmark_state)
{
    if (ui_gm_item_menu_window_)
    {
        ui_gm_item_menu_window_->UpdateMenus(item_info, is_bookmark_state);
    }
}

void uiGMWindowMan::CreateItemFromParameta(BASE_ITEMINFO *item_info, ITEM_PARAMETAS* item_parameta)
{
    if (ui_gm_item_window_)
    {
        ui_gm_item_window_->CreateItem(item_info, item_parameta);
    }
}

void uiGMWindowMan::CreateItemLucky(BASE_ITEMINFO *item_info, ITEM_PARAMETAS* item_parameta, int lucky_expiry)
{
    if (ui_gm_item_window_)
    {
        ui_gm_item_window_->CreateItem(item_info, item_parameta, CREATE_TYPE_LUCKY, lucky_expiry);
    }
}

void uiGMWindowMan::CreateLimited(BASE_ITEMINFO *item_info, ITEM_PARAMETAS* item_parameta)
{
    if (ui_gm_item_window_)
    {
        ui_gm_item_window_->CreateItem(item_info, item_parameta, CREATE_TYPE_LIMITED);
    }
}

void uiGMWindowMan::CreateItemFromList()
{
    if (ui_gm_item_window_)
    {
        ui_gm_item_window_->CreateItemFromList();
    }
}

void uiGMWindowMan::AddFindWord(TCHAR* find_string)
{
    bool is_finded = false;
    if (find_string == "")
    {
        return; // 스트링이 없는 경우 추가하지 않는다.
    }
    else
    {
        size_t find_list_size = find_words_.size();
        for (DWORD i = 0; i < find_list_size; ++i)
        {
            std::string string = find_words_[i].c_str();
            if (string.find(find_string, 0) != -1)
            {
                is_finded = true;
            }
        }

        if (is_finded == false)
        {
            if (find_list_size > 0 && find_list_size >= kFind_Word_Max)
            {
                find_words_.erase(find_words_.begin());
            }
            find_words_.push_back(find_string);
        }
    }
}

void uiGMWindowMan::DeleteAllFindWord()
{
    find_words_.clear();

    TCHAR message[INTERFACE_STRING_LENGTH] = {0,};
    sprintf(message, "GMItemWin: 검색어 리스트가 삭제되었습니다.");
    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, message, CHAT_TYPE_SYSTEM);
}

int uiGMWindowMan::PreSearchFindWord(TCHAR* find_string)
{
    size_t find_list_size = find_words_.size();
    active_find_words_.clear();
    std::vector<std::string>::iterator iter = find_words_.begin();
    int find_count = 0;

    for (DWORD i = 0; i < find_list_size; ++i)
    {
        std::string string = find_words_[i].c_str();
        if (string.find(find_string, 0) != -1)
        {
            active_find_words_.push_back(string.c_str());
            ++find_count;
            if (find_count >= 9)
            {
                break;
            }
        }
    }
    return find_count;
}

const char* uiGMWindowMan::GetActivateFindData(DWORD index)
{
    DWORD size = active_find_words_.size();

    if (size > 0)
    {
        return active_find_words_[index].c_str();
    }
    return NULL;
}

void uiGMWindowMan::AddBookMark(DWORD item_code)
{
    DWORD size = bookmark_lists_.size();
    bool is_addable = true;

    if (size >= bookmark_max_)
    {
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, "GMItemWin: 즐겨찾기 제한 수 이상 추가 될수 없습니다.(Warring)", CHAT_TYPE_SYSTEM);
        return;
    }
    std::vector<DWORD>::iterator iter = bookmark_lists_.end();
    // 중복체크 모든 경우를 중복체크함
    for (DWORD i = 0; i < size; ++i)
    {
        if (bookmark_lists_[i] == item_code)
        {   // 중복됨 추가 안함
            is_addable = false;
            break;
        }
    }

    if (is_addable == true)
    {
        bookmark_lists_.push_back(item_code);
        BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(item_code);
        TCHAR item_name[INTERFACE_STRING_LENGTH] = {0,};
        TCHAR buffer_string[INTERFACE_STRING_LENGTH] = {0,};
        if (item_info)
        {
            g_InterfaceManager.GetItemName(item_code, item_name);
            sprintf(buffer_string, "GMItemWin: (ID: %d) %s 아이템이 즐겨찾기에 추가되었습니다.", item_code, item_name);
        }
        else
        {
            sprintf(buffer_string, "GMItemWin: (ID: %d) 아이템 정보가 없습니다.", item_code);
        }
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, buffer_string, CHAT_TYPE_SYSTEM);
    }
    else
    {
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, "GMItemWin: 즐겨찾기에 이미 등록된 아이템 입니다.(Warring)", CHAT_TYPE_SYSTEM);
    }
}

void uiGMWindowMan::DeleteBookMark(DWORD item_code)
{
    int size = (int)bookmark_lists_.size();
    bool is_deleted = false;

    std::vector<DWORD>::iterator iter = bookmark_lists_.begin();
    std::vector<DWORD>::iterator iter_end = bookmark_lists_.end();
    // 중복체크 모든 경우를 중복체크함
    for (int i = 0; i < size; ++i)
    {
        if (bookmark_lists_[i] == item_code)
        {   // 찾았음 삭제
            bookmark_lists_.erase(iter);
            is_deleted = true;
            break;
        }
        else
        {
            ++iter;
        }
    }

    if (ui_gm_item_window_)
    {
        DWORD size = bookmark_lists_.size();
        ui_gm_item_window_->RefreshList(kState_BookMark, size, size);
    }
}

void uiGMWindowMan::DeleteAllHistroys()
{
    DWORD size = created_histroys_.size();
    created_histroys_.clear();

    TCHAR message[INTERFACE_STRING_LENGTH] = {0,};
    sprintf(message, "GMItemWin: 히스토리 %d 개가 삭제되었습니다.", size);
    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, message, CHAT_TYPE_SYSTEM);

    if (ui_gm_item_window_)
    {
        ui_gm_item_window_->RefreshList(kState_History, 0, 0);
    }
}

void uiGMWindowMan::DeleteAllBookMarks()
{
    DWORD size = created_histroys_.size();
    bookmark_lists_.clear();

    TCHAR message[INTERFACE_STRING_LENGTH] = {0,};
    sprintf(message, "GMItemWin: 즐겨찾기 %d 개가 삭제되었습니다.", size);
    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, message, CHAT_TYPE_SYSTEM);

    if (ui_gm_item_window_)
    {
        ui_gm_item_window_->RefreshList(kState_BookMark, 0, 0);
    }
}

void uiGMWindowMan::AddHistory(DWORD item_code)
{
    int size = (int)created_histroys_.size();
    std::vector<DWORD>::iterator iter = created_histroys_.end();

    DWORD count = 0;
    // 중복체크 최근 50건에 한해서만 중복체크함
    for (int i = (size - 1); i >= 0; --i)
    {
        --iter;
        if (created_histroys_[i] == item_code)
        {   // 중복됨
            created_histroys_.erase(iter);//삭제 후 추가
            break;
        }
        ++count;
        if (count >= history_max_)
        {   // 중복없음
            break;
        }
    }    
    created_histroys_.push_back(item_code);

    if (ui_gm_item_window_)
    {
        DWORD size = created_histroys_.size();
        ui_gm_item_window_->RefreshList(kState_History, size, size);
    }
}

DWORD uiGMWindowMan::GetLastMakeItemCode()
{
    if (ui_gm_item_window_)
    {
        return ui_gm_item_window_->last_make_item_code();
    }
    return 0;
}

void uiGMWindowMan::SetItemParametas(ITEM_PARAMETAS* item_parametas)
{ 
    item_parametas_ = *item_parametas;

    if (ui_gm_item_window_)
    {
        ui_gm_item_window_->RefreshItemInfo();
    }
}

void uiGMWindowMan::SetItemOptionMakeCaptions(TCHAR *clipboard_string)
{
    if (ui_gm_item_option_window_)
    {
        ui_gm_item_option_window_->SetItemOptionCaptions(PARAM_ITEM_CODE, clipboard_string);
    }
}

void uiGMWindowMan::ItemSearch(TCHAR* search_word)
{
    if (ui_gm_item_window_)
    {
        ui_gm_item_window_->ShowInterface(TRUE);
        ui_gm_item_window_->ItemSearch(search_word);
    }
}
#endif//_DEV_VER