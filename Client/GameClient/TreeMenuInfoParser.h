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
        //! �� Ʈ���޴��� �������� ����
        kGroupType_CShop_Default_Root = 1,      //!< ĳ�ü� �⺻��:����ī�װ�
        kGroupType_CShop_Default_Subject = 2,      //!< ĳ�ü� �⺻��:����ī�װ�
        kGroupType_CShop_Cash = 5,      //!< ĳ�ü� ������
        kGroupType_CShop_PPCard = 6,   //!< ĳ�ü� PPCard ��ǰ
        kGroupType_CShop_Mileage = 7,   //!< ĳ�ü� ���ϸ��� ��ǰ
        kGroupType_CShop_MyPage = 8,   //!< ĳ�ü� ������

        kGroupType_Infomation = 21, //!<����������
		kGroupType_BattlezoneTree = 22, //!<��Ʋ�� Ʈ��(������)
        kGroupType_BattlezoneTreeReservation = 23, //!<��Ʋ�� Ʈ�� ����(������)
        kGroupType_Chaoszone_BattleTree = 24, //!<ī������ ��Ʋ
        kGroupType_Chaoszone_HistoryTree = 25, //!<ī������ �����丮
#ifdef _NA_006448_ITEM_TOTAL_COMPOSITE_REVIEWER
        kGroupType_Item_Compiosite_Tree = 26, //!<������ ����â Ʈ��
#endif
        kGroupType_Title_List_Template = 41, //!<Īȣ����Ʈ-���ø�
        kGroupType_Honor_Info_Template = 42, //!<��/����-���ø�
        kGroupType_Title_List = 43, //!<Īȣ����Ʈ
        kGroupType_Honor_Info = 44, //!<��/����
        kGroupType_Currency_Info_Template = 51, //!<ȭ������Ʈ-���ø�
        kGroupType_Currency_Info = 52, //!<ȭ������Ʈ
        kGroupType_GuildDomination_Info_Template = 61, //!<�������������-���ø�
        kGroupType_GuildDomination_Info = 62, //!<�������������
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        kGroupType_Chaoszone_GoldRushTree = 63, //!<ī������ ��巯��,
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#ifdef _NA_007086_20140318_MONSTERBOOK
        kGroupType_MonsterBook_Template = 64, //!<���͵���-���ø�,
        kGroupType_MonsterBook_Menu = 65, //!<���͵���,
        kGroupType_MonsterBook_MonsterList = 66, //!<���͵���,
        kGroupType_MonsterBook_DropList = 67, //!<���͵���,
#endif //_NA_007086_20140318_MONSTERBOOK
#ifdef _NA_008334_20150608_SONNENSCHEIN
        kGroupType_SonnenSchein_MissionList = 68, //!<���̼�-�ҳٻ���
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
        kIconType_Cursor, //!< Ŀ�� �̹���
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

    //! tree group  ���� ��ȣ�� ���� Ʈ�� �ȿ� ��� ��.
    Group_Type tree_group; 

    //! �޴��� ������ 255������ ���̴� 4������ ��ȿ
    union
    {
        BYTE menu_depth[4];
        DWORD menu_index;
    };

    //! �޴� ����
    Menu_Type menu_type;

    //! �޴� �鿩���� 
    int menu_depth_width;

    //! �޴� ���ڿ�
    DWORD string_code;

    //! ��Ʈ ����
    DWORD font_id[kFontType_Max];
    DWORD font_color[kFontType_Max];

    // TextureListInfoTreeMenu.txt �� �ִ� �̹��� �ڵ�
    DWORD image_code[kIconType_Max];

    //! ��ũ��Ʈ���� �������� �ʴº���
    TCHAR string_name[kNameLength]; //!< string_code == 0 �϶� �ش� ��Ʈ�����
    DWORD user_data;        //!< ����� ����
    DWORD user_parameter;   //!< ����� ����
#ifdef _NA_006448_ITEM_TOTAL_COMPOSITE_REVIEWER
    void*  user_data_ptr;   //!< ����� ����
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

