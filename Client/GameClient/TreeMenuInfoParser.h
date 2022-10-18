#ifndef _TreeMenuParser_h_
#define _TreeMenuParser_h_

#if _MSC_VER > 1000
#pragma once
#endif 

#include <Singleton.h>
#include <ScriptCode.IParser.h>

//------------------------------------------------------------------------------ 
struct BASE_TreeMenuInfo 
{
    enum Group_Type
    {
        //! 샵 트리메뉴는 동적으로 생성
        kGroupType_CShop_Default_Root = 1,      //!< 캐시샵 기본값:상위카테고리
        kGroupType_CShop_Default_Subject = 2,      //!< 캐시샵 기본값:하위카테고리
        kGroupType_CShop_Cash = 5,      //!< 캐시샵 아이템
        kGroupType_CShop_PPCard = 6,   //!< 캐시샵 PPCard 상품
        kGroupType_CShop_Mileage = 7,   //!< 캐시샵 마일리지 상품
        kGroupType_CShop_MyPage = 8,   //!< 캐시샵 내정보

        kGroupType_Infomation = 21, //!<정보열람실
		kGroupType_BattlezoneTree = 22, //!<배틀존 트리(리뉴얼)
        kGroupType_BattlezoneTreeReservation = 23, //!<배틀존 트리 예약(리뉴얼)
        kGroupType_Chaoszone_BattleTree = 24, //!<카오스존 배틀
        kGroupType_Chaoszone_HistoryTree = 25, //!<카오스존 히스토리
#ifdef _NA_006448_ITEM_TOTAL_COMPOSITE_REVIEWER
        kGroupType_Item_Compiosite_Tree = 26, //!<아이탬 생성창 트리
#endif
        kGroupType_Title_List_Template = 41, //!<칭호리스트-템플릿
        kGroupType_Honor_Info_Template = 42, //!<명성/평판-템플릿
        kGroupType_Title_List = 43, //!<칭호리스트
        kGroupType_Honor_Info = 44, //!<명성/평판
        kGroupType_Currency_Info_Template = 51, //!<화폐포인트-템플릿
        kGroupType_Currency_Info = 52, //!<화폐포인트
        kGroupType_GuildDomination_Info_Template = 61, //!<길드점령전정보-템플릿
        kGroupType_GuildDomination_Info = 62, //!<길드점령전정보
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        kGroupType_Chaoszone_GoldRushTree = 63, //!<카오스존 골드러쉬,
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#ifdef _NA_007086_20140318_MONSTERBOOK
        kGroupType_MonsterBook_Template = 64, //!<몬스터도감-템플릿,
        kGroupType_MonsterBook_Menu = 65, //!<몬스터도감,
        kGroupType_MonsterBook_MonsterList = 66, //!<몬스터도감,
        kGroupType_MonsterBook_DropList = 67, //!<몬스터도감,
#endif //_NA_007086_20140318_MONSTERBOOK
#ifdef _NA_008334_20150608_SONNENSCHEIN
        kGroupType_SonnenSchein_MissionList = 68, //!<길드미션-소넨샤인
#endif //_NA_008334_20150608_SONNENSCHEIN
        kGroupType_Max,
    };

    enum Menu_Type
    {
        kMenuType_None = 0, 
        kMenuType_Group, 
        kMenuType_CheckButton,
        kMenuType_Max,
    };

    enum Icon_Type
    {
        kIconType_Enable = 0, 
        kIconType_Disable, 
        kIconType_Collpase, 
        kIconType_Unfold, 
        kIconType_CheckOn, 
        kIconType_CheckOff, 
        kIconType_Cursor, //!< 커서 이미지
        kIconType_Max,
    };

    enum Font_type
    {
        kFontType_Enable = 0,
        kFontType_Disable,
        kFontType_Over,
        kFontType_Select,
        kFontType_Max,
    };

    enum Sub_Menu_Index
    {
        kFirstSubMenuIndex = 3,
        kSecondSubMenuIndex = 2,
        kThirdSubMenuIndex = 1,
        kFourthSubMenuIndex = 0,
    };

    static const int kNameLength = 64;

    //! tree group  같은 번호면 같은 트리 안에 들어 감.
    Group_Type tree_group; 

    //! 메뉴는 갯수는 255개까지 깊이는 4까지만 유효
    union
    {
        BYTE menu_depth[4];
        DWORD menu_index;
    };

    //! 메뉴 종류
    Menu_Type menu_type;

    //! 메뉴 들여쓰기 
    int menu_depth_width;

    //! 메뉴 문자열
    DWORD string_code;

    //! 폰트 종류
    DWORD font_id[kFontType_Max];
    DWORD font_color[kFontType_Max];

    // TextureListInfoTreeMenu.txt 에 있는 이미지 코드
    DWORD image_code[kIconType_Max];

    //! 스크립트에는 셋팅하지 않는변수
    TCHAR string_name[kNameLength]; //!< string_code == 0 일때 해당 스트링출력
    DWORD user_data;        //!< 사용자 변수
    DWORD user_parameter;   //!< 사용자 변수
#ifdef _NA_006448_ITEM_TOTAL_COMPOSITE_REVIEWER
    void*  user_data_ptr;   //!< 사용자 변수
#endif


    //------------------------------------------------------------------------------ 
    BASE_TreeMenuInfo()
    {
        tree_group = kGroupType_Max;
        menu_index = 0;
        string_code = 0;
        menu_type = kMenuType_Max;
        menu_depth_width = 0;
        ZeroMemory(font_id, sizeof(font_id));
        ZeroMemory(font_color, sizeof(font_color));
        ZeroMemory(image_code, sizeof(image_code));

        user_data = 0;
        user_parameter = 0;
        ZeroMemory(string_name, sizeof(string_name));
#ifdef _NA_006448_ITEM_TOTAL_COMPOSITE_REVIEWER
        user_data_ptr = NULL;
#endif
    }
    bool is_folder() { return (menu_type == kMenuType_Group); }
    bool is_check() { return (menu_type == kMenuType_CheckButton); }
};

//------------------------------------------------------------------------------ 
class TreeMenuInfoParser : public util::Singleton<TreeMenuInfoParser>, public IParser
{
public:
    typedef std::multimap<int, BASE_TreeMenuInfo> TreeMenu_multimap;

public:

public:
    TreeMenuInfoParser(void);
	~TreeMenuInfoParser(void);

	virtual void Release();
	virtual	BOOL LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);

    void Remove(BASE_TreeMenuInfo::Group_Type group_type);
    bool Insert(BASE_TreeMenuInfo& info);

    int  GetTreeMenu(OUT std::vector<BASE_TreeMenuInfo*>& value_vector, int group);

    bool GetFirstTreeMenu(OUT BASE_TreeMenuInfo& info, BASE_TreeMenuInfo::Group_Type group_type);

    TreeMenu_multimap& treecontrol_multimap() { return treecontrol_multimap_; }

private:
    TreeMenu_multimap treecontrol_multimap_;
};

#endif //_TreeMenuParser_h_

