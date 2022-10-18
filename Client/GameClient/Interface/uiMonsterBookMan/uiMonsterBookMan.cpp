#include "SunClientPrecompiledHeader.h"

#ifdef _NA_007086_20140318_MONSTERBOOK
#include "uiMonsterBookMan/uiMonsterBookMan.h"
#include "uiMonsterBookMan/uiMonsterBookView.h"
#include "uiMonsterBookMan/uiMonsterBookDropList.h"
#include "uiMonsterBookMan/MonsterBookDropItemParser.h"
//#include "uiMonsterBookMan/uiMonsterBookExtract.h"
#include "MonsterBookParser.h"
#include "GlobalFunc.h"

const DWORD IM_MONSTERBOOK_MANAGER::MONSTERBOOK_VIEW = StrToWzID("3101");
const DWORD IM_MONSTERBOOK_MANAGER::MONSTERBOOK_DROPLIST = StrToWzID("3102");
//const DWORD IM_MONSTERBOOK_MANAGER::MONSTERBOOK_EXTRACT = StrToWzID("3102");



//-----------------------------------------------------------------------------
uiMonsterBookMan::uiMonsterBookMan(InterfaceManager *pUIMan) :
uiBaseMan(pUIMan),
current_dialog_key_(IM_MONSTERBOOK_MANAGER::MONSTERBOOK_VIEW),
identified_monster_count_(0),
total_monster_count_(0)
{
    ZeroMemory(tap_dialog_keys, sizeof(tap_dialog_keys));
}
//-----------------------------------------------------------------------------
uiMonsterBookMan::~uiMonsterBookMan()
{
    MonsterBook_Tree::const_iterator monsterbook_tree_iter = monsterbook_tree().begin();
    while (monsterbook_tree_iter != monsterbook_tree().end())
    {
        MonsterBook_Group* monsterbook_group_tree = monsterbook_tree_iter->second;
        monsterbook_group_tree->clear();
        SAFE_DELETE(monsterbook_group_tree);
        ++monsterbook_tree_iter;
    }
    monsterbook_tree().clear();
    searched_group().clear();
}
//-----------------------------------------------------------------------------
void uiMonsterBookMan::OnInitialize()
{
    // 몬스터도감 트리 생성
    InitMonsterBookTree();

    ui_monsterbook_view_    = CreateUIDialog<uiMonsterBookView>(IM_MONSTERBOOK_MANAGER::MONSTERBOOK_VIEW, "Data\\Interface\\31_1_mob_book.iwz", this, TRUE);
    ui_monsterbook_droplist_= CreateUIDialog<uiMonsterBookDropList>(IM_MONSTERBOOK_MANAGER::MONSTERBOOK_DROPLIST, "Data\\Interface\\31_2_mob_book.iwz", this, TRUE);
    //ui_monsterbook_extract_ = CreateUIDialog<uiMonsterBookExtract>(IM_MONSTERBOOK_MANAGER::MONSTERBOOK_EXTRACT, "Data\\Interface\\31_2_mobbook.iwz", this);

    set_current_dialog_key(IM_MONSTERBOOK_MANAGER::MONSTERBOOK_VIEW);

    tap_dialog_keys[MonsterBookTap_View] = IM_MONSTERBOOK_MANAGER::MONSTERBOOK_VIEW;
    tap_dialog_keys[MonsterBookTap_DropList] = IM_MONSTERBOOK_MANAGER::MONSTERBOOK_DROPLIST;
    //tap_dialog_keys[MonsterBookTap_Extract] = IM_MONSTERBOOK_MANAGER::MONSTERBOOK_EXTRACT;

}
//-----------------------------------------------------------------------------
void uiMonsterBookMan::toggle_monster_book(BOOL close_window)
{
    uiBase* ui_base = GET_CAST_DIALOG(uiBase, current_dialog_key());
    if (ui_base == NULL)
    {
        set_current_dialog_key(IM_MONSTERBOOK_MANAGER::MONSTERBOOK_VIEW);
        uiBase* ui_base = GET_CAST_DIALOG(uiBase, current_dialog_key());
        if (ui_base == NULL)
        {
            return;
        }
    }

    if (ui_base->IsVisibleWindow() == TRUE || (close_window == TRUE))
    {
        ui_base->ShowInterface(FALSE);
    }
    else
    {
        ui_base->ShowInterface(TRUE);
    }
}
//-----------------------------------------------------------------------------
void uiMonsterBookMan::click_tap_button(BYTE index)
{
    if (index >= MonsterBookTap_Max)
    {
        return;
    }

    
    if (current_dialog_key() == tap_dialog_keys[index])
    {
        return;
    }

    uiMonsterBookBase* ui_book_base_old = GET_CAST_DIALOG(uiMonsterBookBase, current_dialog_key());
    if (ui_book_base_old != NULL)
    {
        ui_book_base_old->ShowInterface(FALSE);
    }

    set_current_dialog_key(tap_dialog_keys[index]);

    uiMonsterBookBase* ui_book_base = GET_CAST_DIALOG(uiMonsterBookBase, current_dialog_key());
    if (ui_book_base != NULL)
    {
        ui_book_base->SetWindowState(ui_book_base_old->GetWindowState());
        ui_book_base->ShowInterface(TRUE);
    }
}
//-----------------------------------------------------------------------------
void uiMonsterBookMan::InitMonsterBookTree()
{
    // 몬스터북 스크립트의 데이터를 트리형태로 정렬
    monsterbook_tree().clear();
    
    total_monster_count_ = MonsterBookParser::Instance()->GetMonsterTotalCount();

    MonsterBookParser::MonsterBookContainer_Iterator iter =
        MonsterBookParser::Instance()->MonsterBookInfo_Begin();
    MonsterBookParser::MonsterBookContainer_Iterator iter_end =
        MonsterBookParser::Instance()->MonsterBookInfo_End();

    for ( ; iter != iter_end; ++iter)
    {
        //iter->first; 안씀
        MonsterBookInfo* monsterbook_info = iter->second;
        WORD main_category = monsterbook_info->main_category_;
        WORD sub_category  = monsterbook_info->sub_category_;

        MonsterBook_Tree::const_iterator monsterbook_tree_iter = monsterbook_tree().find(main_category);
        if (monsterbook_tree_iter == monsterbook_tree().end())
        {
            // 메인카테고리부터 삽입
            MonsterBook_Group* monsterbook_group = new MonsterBook_Group;
            monsterbook_group->insert(MonsterBook_Group::value_type(
                sub_category,
                monsterbook_info));

            monsterbook_tree().insert(MonsterBook_Tree::value_type(
                main_category,
                monsterbook_group));

            continue;
        }

        // 서브카테고리부터 삽입
        MonsterBook_Group* monsterbook_group_tree = monsterbook_tree_iter->second;
        monsterbook_group_tree->insert(MonsterBook_Group::value_type(
            sub_category,
            monsterbook_info));
    }
}
//-----------------------------------------------------------------------------
void uiMonsterBookMan::ActiveMonsterIcon(DWORD monster_code, BOOL is_new)
{
    // 몬스터아이콘 활성화
    MonsterBook_Tree::const_iterator tree_itr = monsterbook_tree().begin();
    MonsterBook_Tree::const_iterator tree_itr_end = monsterbook_tree().end();

    for ( ; tree_itr != tree_itr_end; ++tree_itr)
    {
        MonsterBook_Group* monsterbook_group = tree_itr->second;
        MonsterBook_Group::iterator group_itr = monsterbook_group->begin();
        MonsterBook_Group::iterator group_itr_end = monsterbook_group->end();
     
        for ( ; group_itr != group_itr_end; ++group_itr)
        {
            MonsterBookInfo* monster_info = group_itr->second;
            if (monster_info && monster_info->monster_code_ == monster_code)
            {
                monster_info->active_ = TRUE;
                monster_info->is_new_ = is_new;
                
                ++identified_monster_count_;

                uiMonsterBookBase* ui_book_base = GET_CAST_DIALOG(uiMonsterBookBase, current_dialog_key());
                if (ui_book_base != NULL)
                {
                    ui_book_base->RefreshMonsterCount();
                }
                return;
            }
        }
    }
}
//-----------------------------------------------------------------------------
void uiMonsterBookMan::ActiveMonsterIcon(DWORD monster_code[], int size, BOOL is_new /* = FALSE */)
{
    // 몬스터아이콘 활성화
    MonsterBook_Tree::const_iterator tree_itr = monsterbook_tree().begin();
    MonsterBook_Tree::const_iterator tree_itr_end = monsterbook_tree().end();

    for ( ; tree_itr != tree_itr_end; ++tree_itr)
    {
        MonsterBook_Group* monsterbook_group = tree_itr->second;
        MonsterBook_Group::iterator group_itr = monsterbook_group->begin();
        MonsterBook_Group::iterator group_itr_end = monsterbook_group->end();

        for ( ; group_itr != group_itr_end; ++group_itr)
        {
             MonsterBookInfo* monster_info = group_itr->second;
            if (monster_info == NULL)
            {
                continue;
            }
            
            // 이 몬스터코드가 있는지 배열에서 찾음
            for (int i = 0; i < size; ++i)
            {
                if (monster_info->monster_code_ == monster_code[i])
                {
                    monster_info->active_ = TRUE;
                    monster_info->is_new_ = is_new;
                    ++identified_monster_count_;
                }
            }
        }
    }

    uiMonsterBookBase* ui_book_base = GET_CAST_DIALOG(uiMonsterBookBase, current_dialog_key());
    if (ui_book_base != NULL)
    {
        ui_book_base->RefreshMonsterCount();
    }
}
#ifdef _DEV_VER
void uiMonsterBookMan::ActiveMonsterIconAll(BOOL is_bool)
{
    // 임시(클라) 몬스터아이콘 모두 활성화
    identified_monster_count_ = 0;

    MonsterBook_Tree::const_iterator tree_itr = monsterbook_tree().begin();
    MonsterBook_Tree::const_iterator tree_itr_end = monsterbook_tree().end();

    for ( ; tree_itr != tree_itr_end; ++tree_itr)
    {
        MonsterBook_Group* monsterbook_group = tree_itr->second;
        MonsterBook_Group::iterator group_itr = monsterbook_group->begin();
        MonsterBook_Group::iterator group_itr_end = monsterbook_group->end();

        for ( ; group_itr != group_itr_end; ++group_itr)
        {
            MonsterBookInfo* monster_info = group_itr->second;
            if (monster_info)
            {
                monster_info->active_ = is_bool;
                monster_info->is_new_ = is_bool;

                ++identified_monster_count_;
            }
        }
    }
    uiMonsterBookBase* ui_book_base = GET_CAST_DIALOG(uiMonsterBookBase, current_dialog_key());
    if (ui_book_base != NULL)
    {
        ui_book_base->RefreshMonsterCount();
    }
}
#endif //_DEV_VER
//-----------------------------------------------------------------------------
void uiMonsterBookMan::ResetMonsterBookData()
{
    identified_monster_count_ = 0;
    
    // 활성화 정보 초기화
    MonsterBook_Tree::const_iterator tree_itr = monsterbook_tree().begin();
    MonsterBook_Tree::const_iterator tree_itr_end = monsterbook_tree().end();

    for ( ; tree_itr != tree_itr_end; ++tree_itr)
    {
        MonsterBook_Group* monsterbook_group = tree_itr->second;
        MonsterBook_Group::iterator group_itr = monsterbook_group->begin();
        MonsterBook_Group::iterator group_itr_end = monsterbook_group->end();

        for ( ; group_itr != group_itr_end; ++group_itr)
        {
            MonsterBookInfo* monster_info = group_itr->second;
            if (monster_info == NULL)
            {
                continue;
            }

            monster_info->active_ = FALSE;
            monster_info->is_new_ = FALSE;
        }
    }

    uiMonsterBookBase* ui_book_base = GET_CAST_DIALOG(uiMonsterBookBase, current_dialog_key());
    if (ui_book_base != NULL)
    {
        ui_book_base->RefreshMonsterCount();
    }
}
//-----------------------------------------------------------------------------
void uiMonsterBookMan::OnCharacterConnect()
{
    uiMonsterBookBase* ui_book_base = GET_CAST_DIALOG(uiMonsterBookBase, current_dialog_key());
    if (ui_book_base != NULL)
    {
        ui_book_base->set_character_first_show(TRUE);
    }
}
//-----------------------------------------------------------------------------
void uiMonsterBookMan::Search(TCHAR* search_name)
{
    if (strlen(search_name) < 2)
    {
        //return;
    }
    g_InterfaceManager.Trim(search_name);

    searched_group_.clear();
    int group_index = 0;
    int last_main_category = 0;
    int last_sub_category = 0;

    for (int i= 1; i < 999; ++i)
    {
        MonsterBook_Tree::iterator tree_iter = monsterbook_tree().find(i);
        if (tree_iter == monsterbook_tree().end())
        {
            break;
        }

        MonsterBook_Group* group_tree = tree_iter->second;
        for (int j= 1; j < 999; ++j)
        {
            std::pair<uiMonsterBookMan::MonsterBook_Group::iterator,
                uiMonsterBookMan::MonsterBook_Group::iterator> range;
            range = group_tree->equal_range(j);

            if (range.first == range.second)
            {
                break;
            }

            uiMonsterBookMan::MonsterBook_Group::iterator group_itr = range.first;
            for ( ; group_itr != range.second; ++group_itr)
            {
                MonsterBookInfo* monsterbook_info = group_itr->second;
                if (monsterbook_info == NULL)
                {
                    continue;
                }

                BOOL is_drop = MonsterBookDropItemParser::Instance()->SearchMonsterDrop(
                    monsterbook_info->monster_code_, search_name);

                if (is_drop == TRUE)
                {
                    // 인서트 트리
                    if ((monsterbook_info->main_category_ != last_main_category) ||
                        (monsterbook_info->sub_category_ != last_sub_category))
                    {
                        last_main_category = monsterbook_info->main_category_;
                        last_sub_category = monsterbook_info->sub_category_;
                        ++group_index;

                    }
                    searched_group_.insert(MonsterBook_Group::value_type(group_index, monsterbook_info));
                }
            }
        }
    }

    //if (group_index != 0)        // 1개이상 포함됬으면 트리 갱신
    {
        uiMonsterBookBase* ui_book_base = GET_CAST_DIALOG(uiMonsterBookBase, current_dialog_key());
        if (ui_book_base != NULL)
        {
            ui_book_base->set_is_search_mode(TRUE);
            ui_book_base->SetMonsterList(&searched_group_);
        }
    }

    if (group_index == 0)
    {
        TCHAR string[INTERFACE_STRING_LENGTH] = {0,};
        //3235 몬스터 도감에 등록된 몬스터 중 해당 아이템을 드랍하는 몬스터가 없습니다. 
        g_InterfaceManager.GetInterfaceString(3235, string);
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, string);
    }
}
//-----------------------------------------------------------------------------
void uiMonsterBookMan::HotKeyToggleMonsterBookDialog()
{
    uiMonsterBookMan* monsterbook_man = GET_DIALOG_MANAGER(uiMonsterBookMan, UIMAN_MONSTERBOOK);
    if (monsterbook_man)
    {
        monsterbook_man->toggle_monster_book();
    }
}
//-----------------------------------------------------------------------------
//BOOL uiMonsterBookMan::
//-----------------------------------------------------------------------------
#endif //_NA_007086_20140318_MONSTERBOOK