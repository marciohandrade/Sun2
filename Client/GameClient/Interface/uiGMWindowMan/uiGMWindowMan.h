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
    PARAM_ITEM_MAIN_FILTER,// 상위 필터
    PARAM_ITEM_SUB_FILTER,// 하위 필터
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
    ENCHANT_LEVEL_MAX = 15,//인첸트레벨 최대값
};

typedef enum filters
{
    FILTER_ALL = 0, // 전체 (필터 없음)
    FILTER_EQUIP, // 장비 필터
    FILTER_WASTE, // 소모품 필터
    FILTER_GRADE, // 등급 필터
    FILTER_GENDER, // 성별 필터
    FILTER_ETC, // 종류 필터
    FILTER_MAX,
}FILTERS;

enum // 장비 필터
{
    EQUIP_ALL = 0,
    EQUIP_WEAPON,
    EQUIP_ARMOR,
    EQUIP_ACCESSORY,
    EQUIP_SPECIALACCESSORY,
    EQUIP_FITERL_MAX,
};

enum // 소모품 필터
{
    WASTE_ALL = 0,
    WASTE_CAN_USE,
    WASTE_ONLY_WASTE,
    WASTE_POTION,
    WASTE_FILTER_MAX,
};

enum // 등급 필터
{
    GRADE_NORMAL = 0,// 1~10등급
    GRADE_ULTIMATE, // 얼티메이트(일본최강)
    GRADE_ELITE1, // 엘리트 1차
    GRADE_CORRUPT1, // 저주받은 엘리트 1차
    GRADE_ELITE1_5, // 엘리트 1.5차
    GRADE_ELITE2, // 엘리트 2차
	GRADE_ELITE2_5,//엘리트 2.5차
    GRADE_UNIQUE1, // 유니크 1차
    GRADE_UNIQUE1_5, // 유니크 1.5차
    GRADE_UNIQUE2, // 유니크 2차
    GRADE_UNIQUE2_5, // 유니크 2.5차
    GRADE_LUCKY, // 럭키 아이템
	GRADE_FIGTER_ELITE2,//투사의 엘리트 2차
    GRADE_ELITE3,//엘리트3차
    GRADE_ELITE3_5,//엘리트3.5차
	GRADE_ELITE4,//엘리트4차
    GRADE_FILTER_MAX,
};

enum // 성별 필터
{
    GENDER_FREE = 0,// 성별 관계없는 아이템
    GENDER_MALE,
    GENDER_FEMALE,
    GENDER_MALE_ONLY,
    GENDER_FEMAIL_ONLY,
    GENDER_FILTER_MAX,
};

enum // 클래스 필터
{
    CLASS_BERSERKER = 0,
    CLASS_DRAGON,
    CLASS_VALKRIE,
    CLASS_SHADOW,
    CLASS_ELEMENTALLIST,
    CLASS_MYSTIC,
    CLASS_FILTER_MAX,
};

enum // 종류 필터
{
    ETC_NORMAL = 0, // 일반 아이템
    ETC_QUEST, // 퀘스트
    ETC_AC, // AC 아이템
    ETC_ENCHANT_COMPOSITE,// 인첸트조합 아이템
    ETC_ZARD, // 쟈드 아이템
    ETC_CHARGE, // 캐쉬 아이템
    ETC_PET, // 펫 아이템
    ETC_RIDER, // 탈 것
    ETC_CHARACTER, // 캐릭터 관련
    ETC_RANDOM, // 아이템 랜덤화
    ETC_LOTTO, // 복권 아이템
    ETC_COSTUM, // 코스튬
    ETC_FILTER_MAX,
};

typedef struct parametas
{
    CODETYPE item_code; // 아이템 코드
    WORD item_count; // 아이템 수량
    bool overlapping; // 겹치기
    ITEM_TYPE item_type; // 아이템 타입(0일반, 1디바인, 2에테리아, 3에테리아+디바인, 4외형)
    BYTE enchant_level; // 인첸트 레벨(0~12)
    BYTE socket_count; // 소켓수(0~6)// 0 미확인, 1~5는 1~5개, 6 없음
	BYTE awakening_level; // 각성 레벨(0~9)
    //filter
    FILTERS filter_main;// 필터 상위 분류
    BYTE filter_subs[FILTER_MAX];// 필터 분류의 값(하위 필터)
    BYTE class_type; // 클래스 타입(eCHAR_TYPE)
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