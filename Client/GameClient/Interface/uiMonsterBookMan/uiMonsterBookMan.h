#pragma once

#ifdef _NA_007086_20140318_MONSTERBOOK

#include "uiBaseMan.h"

class uiMonsterBookView;
class uiMonsterBookDropList;
class uiMonsterBookExtract;

class MonsterBookInfo;

struct IM_MONSTERBOOK_MANAGER
{
    static const DWORD MONSTERBOOK_VIEW;
    static const DWORD MONSTERBOOK_DROPLIST;
    //static const DWORD MONSTERBOOK_EXTRACT;
};

enum MonsterBookTap
{
    MonsterBookTap_View = 0,
    MonsterBookTap_DropList,
    //MonsterBookTap_Extract,
    MonsterBookTap_Max
};

class uiMonsterBookMan : public uiBaseMan
{
public:
    typedef STLX_MULTIMAP<WORD, MonsterBookInfo*>            MonsterBook_Group; // 서브카테고리 리스트
    typedef STLX_MAP<WORD, MonsterBook_Group*>          MonsterBook_Tree;// 메인카테고리 리스트


    uiMonsterBookMan(InterfaceManager *pUIMan);
    ~uiMonsterBookMan();

    virtual void OnInitialize();
    void toggle_monster_book(BOOL close_window = FALSE);
    void click_tap_button(BYTE index);

    MonsterBook_Tree& monsterbook_tree() { return monsterbook_tree_; }
    MonsterBook_Group& searched_group() { return searched_group_; }

    int identified_monster_count() { return identified_monster_count_; }
    int total_monster_count() { return total_monster_count_; }

    void ActiveMonsterIcon(DWORD monster_code, BOOL is_new);    // 몬스터아이콘 활성화
    void ActiveMonsterIcon(DWORD monster_code[], int size, BOOL is_new = FALSE);    // 몬스터아이콘 활성화
#ifdef _DEV_VER
    void ActiveMonsterIconAll(BOOL is_bool);    // 임시 몬스터아이콘 모두 활성화
#endif //_DEV_VER

    void ResetMonsterBookData();
    void OnCharacterConnect();

    void Search(TCHAR* search_name);

    static void HotKeyToggleMonsterBookDialog();
private:
    void set_current_dialog_key(DWORD dialog_key) { current_dialog_key_ = dialog_key; }
    DWORD current_dialog_key() { return current_dialog_key_; }

    void InitMonsterBookTree();

    DWORD current_dialog_key_;   // 현재 창 아이디
    uiMonsterBookView*      ui_monsterbook_view_;
    uiMonsterBookDropList*  ui_monsterbook_droplist_;
    //uiMonsterBookExtract*   ui_monsterbook_extract_;
    DWORD tap_dialog_keys[MonsterBookTap_Max];

    MonsterBook_Tree monsterbook_tree_;
    MonsterBook_Group  searched_group_; // 검색된 그룹
    BOOL is_searched_;
    
    int identified_monster_count_;  // 확인된 몬스터 수
    int total_monster_count_;       // 총 몬스터 수
};


#endif //_NA_007086_20140318_MONSTERBOOK