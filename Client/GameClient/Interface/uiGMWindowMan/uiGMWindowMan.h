#pragma once

#ifdef _DEV_VER
#include "uiBaseMan.h"

const DWORD gm_win_font = StrToWzID("mn12");

class uiGMItemWindow;
class uiGMItemOptionWindow;
class uiGMItemResetWindow;
class uiGMItemMenuWindow;
#ifdef _DH_GM_NPC_WINDOW
class uiGMNpcWindow;
#endif//_DH_GM_NPC_WINDOW

struct IM_GM_WINDOW_MANAGER
{
    static const DWORD GM_ITEM_WINDOW;
    static const DWORD GM_ITEM_OPTION_WINDOW;
    static const DWORD GM_ITEM_RESET_WINDOW;
    static const DWORD GM_ITEM_MENU_WINDOW;
#ifdef _DH_GM_NPC_WINDOW
    static const DWORD GM_ITEM_NPC_WINDOW;
#endif//_DH_GM_NPC_WINDOW
};

enum
{
    UI_GM_ITEM_WINDOW,
    UI_GM_ITEM_OPTION_WINDOW,
    UI_GM_ITEM_RESET_WINDOW,
    UI_GM_ITEM_MENU_WINDOW,
#ifdef _DH_GM_NPC_WINDOW
    UI_GM_ITEM_NPC_WINDOW,
#endif//_DH_GM_NPC_WINDOW
    UI_GM_WINDOW_MAX
};

enum //create_type
{
    CREATE_TYPE_NORMAL,
    CREATE_TYPE_LIMITED,
    CREATE_TYPE_LUCKY,
};

typedef enum parameta_names
{
    PARAM_ITEM_CODE,
    PARAM_ITEM_COUNT,
    PARAM_ITEM_OVERLAPPING,
    PARAM_ITEM_TYPE,
    PARAM_ITEM_ENCHANT,
    PARAM_ITEM_SOCKET_COUNT,
	PARAM_ITEM_AWAKENING,
    PARAM_ITEM_MAIN_FILTER,// ���� ����
    PARAM_ITEM_SUB_FILTER,// ���� ����
    PARAM_ITEM_CLASS,
    PARAM_ITEM_MAX,
}PARAM_NAME;

typedef enum item_type
{
    ITEM_TYPE_NORMAL,
    ITEM_TYPE_DIVINE,
    ITEM_TYPE_ETHERIA,
    ITEM_TYPE_DIVINE_ETHERIA,
    ITEM_TYPE_SKIN,
    ITEM_TYPE_MAX,
}ITEM_TYPE;

enum
{
    ENCHANT_LEVEL_MAX = 15,//��þƮ���� �ִ밪
};

typedef enum filters
{
    FILTER_ALL = 0, // ��ü (���� ����)
    FILTER_EQUIP, // ��� ����
    FILTER_WASTE, // �Ҹ�ǰ ����
    FILTER_GRADE, // ��� ����
    FILTER_GENDER, // ���� ����
    FILTER_ETC, // ���� ����
    FILTER_MAX,
}FILTERS;

enum // ��� ����
{
    EQUIP_ALL = 0,
    EQUIP_WEAPON,
    EQUIP_ARMOR,
    EQUIP_ACCESSORY,
    EQUIP_SPECIALACCESSORY,
    EQUIP_FITERL_MAX,
};

enum // �Ҹ�ǰ ����
{
    WASTE_ALL = 0,
    WASTE_CAN_USE,
    WASTE_ONLY_WASTE,
    WASTE_POTION,
    WASTE_FILTER_MAX,
};

enum // ��� ����
{
    GRADE_NORMAL = 0,// 1~10���
    GRADE_ULTIMATE, // ��Ƽ����Ʈ(�Ϻ��ְ�)
    GRADE_ELITE1, // ����Ʈ 1��
    GRADE_CORRUPT1, // ���ֹ��� ����Ʈ 1��
    GRADE_ELITE1_5, // ����Ʈ 1.5��
    GRADE_ELITE2, // ����Ʈ 2��
	GRADE_ELITE2_5,//����Ʈ 2.5��
    GRADE_UNIQUE1, // ����ũ 1��
    GRADE_UNIQUE1_5, // ����ũ 1.5��
    GRADE_UNIQUE2, // ����ũ 2��
    GRADE_UNIQUE2_5, // ����ũ 2.5��
    GRADE_LUCKY, // ��Ű ������
	GRADE_FIGTER_ELITE2,//������ ����Ʈ 2��
    GRADE_ELITE3,//����Ʈ3��
    GRADE_ELITE3_5,//����Ʈ3.5��
	GRADE_ELITE4,//����Ʈ4��
    GRADE_FILTER_MAX,
};

enum // ���� ����
{
    GENDER_FREE = 0,// ���� ������� ������
    GENDER_MALE,
    GENDER_FEMALE,
    GENDER_MALE_ONLY,
    GENDER_FEMAIL_ONLY,
    GENDER_FILTER_MAX,
};

enum // Ŭ���� ����
{
    CLASS_BERSERKER = 0,
    CLASS_DRAGON,
    CLASS_VALKRIE,
    CLASS_SHADOW,
    CLASS_ELEMENTALLIST,
    CLASS_MYSTIC,
    CLASS_FILTER_MAX,
};

enum // ���� ����
{
    ETC_NORMAL = 0, // �Ϲ� ������
    ETC_QUEST, // ����Ʈ
    ETC_AC, // AC ������
    ETC_ENCHANT_COMPOSITE,// ��þƮ���� ������
    ETC_ZARD, // ��� ������
    ETC_CHARGE, // ĳ�� ������
    ETC_PET, // �� ������
    ETC_RIDER, // Ż ��
    ETC_CHARACTER, // ĳ���� ����
    ETC_RANDOM, // ������ ����ȭ
    ETC_LOTTO, // ���� ������
    ETC_COSTUM, // �ڽ�Ƭ
    ETC_FILTER_MAX,
};

typedef struct parametas
{
    CODETYPE item_code; // ������ �ڵ�
    WORD item_count; // ������ ����
    bool overlapping; // ��ġ��
    ITEM_TYPE item_type; // ������ Ÿ��(0�Ϲ�, 1�����, 2���׸���, 3���׸���+�����, 4����)
    BYTE enchant_level; // ��þƮ ����(0~12)
    BYTE socket_count; // ���ϼ�(0~6)// 0 ��Ȯ��, 1~5�� 1~5��, 6 ����
	BYTE awakening_level; // ���� ����(0~9)
    //filter
    FILTERS filter_main;// ���� ���� �з�
    BYTE filter_subs[FILTER_MAX];// ���� �з��� ��(���� ����)
    BYTE class_type; // Ŭ���� Ÿ��(eCHAR_TYPE)
}ITEM_PARAMETAS;

enum
{
    kState_History,
    kState_BookMark,
    kState_Search,
};

class uiGMWindowMan : public uiBaseMan
{
    enum
    {
        kDefault_History_Max = 50,
        kFind_Word_Max = 50,
        kBookmark_Max = 30,
    };

public:
    enum
    {
        kParser_NoParser,
        kParser_Option,
        kParser_HistoryMax,
        kParser_BookMarkMax,
        kParser_Sate,
        kParser_History,
        kParser_BookMark,
        kParser_FindWord,
        kParser_Unknown,
    };

    uiGMWindowMan(InterfaceManager *pUIMan);
    virtual ~uiGMWindowMan(void);

    //--------------------------------------------------------------------------
    // init/release
    //
public:
    virtual void OnInitialize();

    bool IsShowGMWindow(DWORD gm_window_id);
    void ShowGMWindow(DWORD gm_window_id, bool value);
    void ToggleGMWindow(DWORD gm_window_id);
    void ResetItemParametas();
    void LoadItemParametas();
    ITEM_PARAMETAS* item_parametas(){ return &item_parametas_; }
    ITEM_PARAMETAS* GetItemOption();
    void SetItemParametas(ITEM_PARAMETAS* item_parametas);
    void SetItemOptionParametas(ITEM_PARAMETAS* item_parameta);

    RECT GetResetWindowPosition(RECT *control_rect);
    void SetCaptionFilter();
    bool IsSearching();
    void SetSearchEnd();
    void SetPopupUpdateMenus(BASE_ITEMINFO *item_info, bool is_bookmark_state);
    void CreateItemFromParameta(BASE_ITEMINFO *item_info, ITEM_PARAMETAS* item_parameta);
    void CreateItemLucky(BASE_ITEMINFO *item_info, ITEM_PARAMETAS* item_parameta, int lucky_expiry);
    void CreateLimited(BASE_ITEMINFO *item_info, ITEM_PARAMETAS* item_parameta);
    void CreateItemFromList();

    void AddHistory(DWORD item_code);
    void DeleteBookMark(DWORD item_code);
    void AddBookMark(DWORD item_code);
    void DeleteAllHistroys();
    void DeleteAllBookMarks();
    void AddFindWord(TCHAR* find_string);
    void DeleteAllFindWord();
    int PreSearchFindWord(TCHAR* find_string);
    const char* GetActivateFindData(DWORD index);
    DWORD GetLastMakeItemCode();
    void SetItemOptionMakeCaptions(TCHAR* clipboard_string);
    uiBase* GetGMWindow(DWORD gm_window_id);

    DWORD history_max(){ return history_max_; }
    DWORD find_word_max(){ return find_word_max_; }
    DWORD bookmark_max(){ return bookmark_max_; }
    BYTE item_window_state(){ return item_window_state_; }
    void ItemSearch(TCHAR* search_word);

    std::vector<DWORD>* searched_results(){ return &searched_results_; }
    std::vector<DWORD>* created_histroys(){ return &created_histroys_; }
    std::vector<DWORD>* bookmark_lists(){ return &bookmark_lists_; }
    std::vector<std::string>* activate_find_list(){ return &active_find_words_; }

private:
    uiGMItemWindow* ui_gm_item_window_;
    uiGMItemOptionWindow* ui_gm_item_option_window_;
    uiGMItemResetWindow* ui_gm_item_reset_window_;
    uiGMItemMenuWindow* ui_gm_item_menu_window_;
#ifdef _DH_GM_NPC_WINDOW
    uiGMNpcWindow* ui_gm_npc_window_;
#endif//_DH_GM_NPC_WINDOW

    std::vector<DWORD> searched_results_;
    std::vector<DWORD> created_histroys_;
    std::vector<DWORD> bookmark_lists_;
    std::vector<std::string> find_words_;
    std::vector<std::string> active_find_words_;

    ITEM_PARAMETAS item_parametas_;
    DWORD history_max_;
    DWORD find_word_max_;
    DWORD bookmark_max_;
    BYTE item_window_state_;
private:
    BYTE GMItemOptionParserType(TCHAR *string_buffer);
    void PushItemOptionData(BYTE parser_type, TCHAR* string_buffer);
    void LoadItemData();
    void SaveData(FILE* file_ptr);
};
#endif//_DEV_VER 