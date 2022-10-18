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

    //! call back 함수에서 그릴때 체크해서 원하는 형태로 그림
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
            bool is_unfold; //!< BASE_TreeMenuInfo::menu_type == kMenuType_Group 일때
            bool is_check;  //!< BASE_TreeMenuInfo::menu_type == kMenuType_CheckButton 일때
        };

        BASE_TreeMenuInfo* info;

        //! 하위 메뉴
        TreeObjectList menu_list;

    private:
        //DataFlag
        DWORD data_flag_;
    };

    //------------------------------------------------------------------------------ 
    //! 트리메뉴를 그릴 기본 콜백 함수
    //! Init에서 콜백을 넣지 않으면 아래 함수 사용
    static void Callback_TreeRender(CCtrlListWZ* list_control, 
                                    CCtrlListWZ::CListSellData* sell_data, 
                                    int sell_index, 
                                    RECT& render_area);
public:
    TreeMenuGroup();
    virtual ~TreeMenuGroup();

    //------------------------------------------------------------------------------
    //! Returns:   bool
    //! Qualifier: 리메뉴 초기화
    //! Parameter: BASE_TreeMenuInfo::Group_Type tree_group : TreeMenuInfoParser.txt 의 Group 번호
    //! Parameter: bool is_simple_menu : 하위 메뉴(폴더)를 하나만 열게함
    //! Parameter: CCtrlListWZ::fn_CallbackListBoxSellDraw fn : 메뉴를 그릴때 사용할 콜백 함수
    //!             NULL : 기본 콜백사용(TreeMenuGroup::Callback_TreeRender)
    //------------------------------------------------------------------------------
    bool Init(BASE_TreeMenuInfo::Group_Type tree_group, 
                CCtrlListWZ* _list_control, 
                CCtrlVScrollWZ* _vscroll_control,
                bool is_simple_menu,
                CCtrlListWZ::fn_CallbackListBoxSellDraw fn);

    bool ApplyTreeGroup(BASE_TreeMenuInfo::Group_Type tree_group);

    void Release();

    //! 트리내용만 지움
    void Cleanup();

    //------------------------------------------------------------------------------
    //! Returns:   bool : true : 트리메뉴에서 이벤트가 처리되었을경우
    //! Qualifier: 메세지 처리
    //! Parameter: SI_MESSAGE * msg
    //! Parameter: OUT TreeObject * & tree_object : 메뉴 클릭이 일어 났을때 선택된 메뉴
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
    //! Qualifier: 해당 데이터가 있는 리스트 셀을 선택한다.
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
    //! Returns:   TreeObject* : 버튼 클릭이 일어 났을때 선택된 메뉴 리턴
    //! Qualifier: 리스트 박스 클릭시 트리메뉴 재구성
    //------------------------------------------------------------------------------
    bool OnLButtonClick(SI_MESSAGE* msg, OUT TreeObject*& tree_object);
    bool OnLButtonDClick(SI_MESSAGE* msg, OUT TreeObject*& tree_object);

    //------------------------------------------------------------------------------
    //! Qualifier: 자신을 포함한 하위폴더 모두 닫는다.
    //! Parameter: TreeObject * tree_object
    //------------------------------------------------------------------------------
    void Collapse(TreeObject* tree_object);

    //------------------------------------------------------------------------------
    //! Qualifier: 자신을 포함한 부모폴더 모두 연다.
    //! Parameter: TreeObject * tree_object
    //------------------------------------------------------------------------------
    void Unfold(TreeObject* tree_object);

    //! 루트 메뉴
    TreeObject root_menu_;

    //! 하나의 하위 메뉴만 열리게
    bool simple_menu_;
    bool extend_always_;    // 이미 열려져 있는경우 재 클릭시 접히지 않도록

    //! 리스트 박스 셀 그릴 콜백
    CCtrlListWZ::fn_CallbackListBoxSellDraw callback_listbox_sell_draw_;
};

#else 
    // 에러 안나게 인터페이스만 만들자.
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
