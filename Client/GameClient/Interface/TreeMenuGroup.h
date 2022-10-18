#pragma once

#ifdef _YMS_TREECONTROL

#if WZENGINEVER < 400
    #error "Impossible Build!! - Reason: WZENGINEVER < 400!!"
#endif //#if WZENGINEVER >= 400

#include "TreeMenuInfoParser.h"
#include "TextureListInfoParser.h"
#include "ListboxScrollControlProduce.h"

//------------------------------------------------------------------------------ 
class TreeMenuGroup : public ListboxScrollControlProduce
{
public:
    class TreeObject;

#ifdef _DEBUG
    typedef std::list<TreeObject> TreeObjectList;
#else
    typedef STLX_LIST<TreeObject> TreeObjectList;
#endif //_DEBUG

    //! call back �Լ����� �׸��� üũ�ؼ� ���ϴ� ���·� �׸�
    enum DataFlag
    {
        kDataFlag           = (1 << 1),
        kDataFlag_Notice    = (kDataFlag | 1 << 2),
        kDataFlag_IconBlink = (kDataFlag | 1 << 3),
        kDataFlag_None      = (1<<31),
    };

    class TreeObject
    {
    public:
        TreeObject();
        TreeObject(TreeObject* _parent, BASE_TreeMenuInfo* _info, bool _visible, bool _enable, DWORD _data_flag = kDataFlag_None);
        ~TreeObject();

        void add_data_flag(DataFlag e) { data_flag_ |= e; }
        void remove_data_flag(DataFlag e) { data_flag_ &= (~e); }
        bool is_notice() { return ((data_flag_ & kDataFlag_Notice) == kDataFlag_Notice); }
        bool is_icon_blink() { return ((data_flag_ & kDataFlag_IconBlink) == kDataFlag_IconBlink); }

        //------------------------------------------------------------------------------ 
        TreeObject* parent;

        bool is_visible;
        bool is_enable;
        union
        {
            bool is_unfold; //!< BASE_TreeMenuInfo::menu_type == kMenuType_Group �϶�
            bool is_check;  //!< BASE_TreeMenuInfo::menu_type == kMenuType_CheckButton �϶�
        };

        BASE_TreeMenuInfo* info;

        //! ���� �޴�
        TreeObjectList menu_list;

    private:
        //DataFlag
        DWORD data_flag_;
    };

    //------------------------------------------------------------------------------ 
    //! Ʈ���޴��� �׸� �⺻ �ݹ� �Լ�
    //! Init���� �ݹ��� ���� ������ �Ʒ� �Լ� ���
    static void Callback_TreeRender(CCtrlListWZ* list_control, 
                                    CCtrlListWZ::CListSellData* sell_data, 
                                    int sell_index, 
                                    RECT& render_area);
public:
    TreeMenuGroup();
    virtual ~TreeMenuGroup();

    //------------------------------------------------------------------------------
    //! Returns:   bool
    //! Qualifier: ���޴� �ʱ�ȭ
    //! Parameter: BASE_TreeMenuInfo::Group_Type tree_group : TreeMenuInfoParser.txt �� Group ��ȣ
    //! Parameter: bool is_simple_menu : ���� �޴�(����)�� �ϳ��� ������
    //! Parameter: CCtrlListWZ::fn_CallbackListBoxSellDraw fn : �޴��� �׸��� ����� �ݹ� �Լ�
    //!             NULL : �⺻ �ݹ���(TreeMenuGroup::Callback_TreeRender)
    //------------------------------------------------------------------------------
    bool Init(BASE_TreeMenuInfo::Group_Type tree_group, 
                CCtrlListWZ* _list_control, 
                CCtrlVScrollWZ* _vscroll_control,
                bool is_simple_menu,
                CCtrlListWZ::fn_CallbackListBoxSellDraw fn);

    bool ApplyTreeGroup(BASE_TreeMenuInfo::Group_Type tree_group);

    void Release();

    //! Ʈ�����븸 ����
    void Cleanup();

    //------------------------------------------------------------------------------
    //! Returns:   bool : true : Ʈ���޴����� �̺�Ʈ�� ó���Ǿ������
    //! Qualifier: �޼��� ó��
    //! Parameter: SI_MESSAGE * msg
    //! Parameter: OUT TreeObject * & tree_object : �޴� Ŭ���� �Ͼ� ������ ���õ� �޴�
    //------------------------------------------------------------------------------
    bool MessageProc(SI_MESSAGE* msg, OUT TreeObject*& tree_object);

    void Fill();
    TreeObject* CurrectTreeObject();
    TreeObject* OverTreeObject();
    TreeObject* Find(DWORD menu_index);
    TreeObject* Find(BYTE menuidx_1, BYTE menuidx_2, BYTE menuidx_3, BYTE menuidx_4);

    void set_menu_height_gab(int val);
    bool is_simple_menu() const { return simple_menu_; }
    void set_simple_menu(bool val) { simple_menu_ = val; }
    TreeMenuGroup::TreeObject& root_menu() { return root_menu_; }

    void set_extend_always( bool val) { extend_always_ = val; }

    //------------------------------------------------------------------------------
    //! Returns:   bool
    //! Qualifier: �ش� �����Ͱ� �ִ� ����Ʈ ���� �����Ѵ�.
    //! Parameter: TreeObject * tree_object
    //------------------------------------------------------------------------------
    bool Select(TreeObject* tree_object);
    bool Select();

private:
    void Cleanup(TreeObject& tree_object);
    bool Insert(TreeObject& tree_object, BASE_TreeMenuInfo* info, int menu_depth);
    void Fill(TreeObject& tree_object);
    TreeObject* Find(TreeObject& tree_object, DWORD menu_index);

    //------------------------------------------------------------------------------
    //! Returns:   TreeObject* : ��ư Ŭ���� �Ͼ� ������ ���õ� �޴� ����
    //! Qualifier: ����Ʈ �ڽ� Ŭ���� Ʈ���޴� �籸��
    //------------------------------------------------------------------------------
    bool OnLButtonClick(SI_MESSAGE* msg, OUT TreeObject*& tree_object);
    bool OnLButtonDClick(SI_MESSAGE* msg, OUT TreeObject*& tree_object);

    //------------------------------------------------------------------------------
    //! Qualifier: �ڽ��� ������ �������� ��� �ݴ´�.
    //! Parameter: TreeObject * tree_object
    //------------------------------------------------------------------------------
    void Collapse(TreeObject* tree_object);

    //------------------------------------------------------------------------------
    //! Qualifier: �ڽ��� ������ �θ����� ��� ����.
    //! Parameter: TreeObject * tree_object
    //------------------------------------------------------------------------------
    void Unfold(TreeObject* tree_object);

    //! ��Ʈ �޴�
    TreeObject root_menu_;

    //! �ϳ��� ���� �޴��� ������
    bool simple_menu_;
    bool extend_always_;    // �̹� ������ �ִ°�� �� Ŭ���� ������ �ʵ���

    //! ����Ʈ �ڽ� �� �׸� �ݹ�
    CCtrlListWZ::fn_CallbackListBoxSellDraw callback_listbox_sell_draw_;
};

#else 
    // ���� �ȳ��� �������̽��� ������.
    #include "TreeMenuInfoParser.h"
    #include "ListboxScrollControlProduce.h"
    class TreeMenuGroup : public ListboxScrollControlProduce
    { 
    public : 
        enum DataFlag
        {
            kDataFlag           = (1 << 1),
            kDataFlag_Notice    = (kDataFlag | 1 << 2),
            kDataFlag_IconBlink = (kDataFlag | 1 << 3),
            kDataFlag_None      = (1<<31),
        };
        class TreeObject 
        {
        public:
            TreeObject() : info(0) {}
            void add_data_flag(DataFlag e) { }
            void remove_data_flag(DataFlag e) { }
            bool is_notice() { return false; }
            bool is_icon_blink() { return false; }
            BASE_TreeMenuInfo* info;
        }; 
    public:
        void Release(){}
        void Cleanup(){}
        bool MessageProc(SI_MESSAGE* msg, OUT TreeObject*& tree_object){return false;}
        void Fill(){}
        TreeObject* CurrectTreeObject(){return NULL;}
        bool ApplyTreeGroup(BASE_TreeMenuInfo::Group_Type tree_group){ return false; }
        bool Select(TreeObject* tree_object) { return false; }
        bool Select() { return false; }
        TreeObject* Find(DWORD menu_index) { return NULL; }
        TreeObject* Find(BYTE menuidx_1, BYTE menuidx_2, BYTE menuidx_3, BYTE menuidx_4) { return NULL; }
    };
#endif //#ifdef _YMS_TREECONTROL
