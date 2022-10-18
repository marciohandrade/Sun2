#pragma once

//트리 데이터 아이템 처리 


#include "TextureListInfoParser.h"
#include "TextureListInfoHandleManager.h"

struct _stTreedata
{
    TCHAR tree_item_str_[256];
    int   tree_deep_;
    int   tree_left_off_;
    int   type_item;
    int   icon_index_;
    DWORD  font_ID;
    DWORD  text_Color_;
    DWORD  back_Color_;
    DWORD  sell_Color_;
    DWORD  select_Color_;

    bool  isFolder; // false - 접힌상태, true - 열린상태
    void*   user_data_Ptr_;

    _stTreedata():
        tree_deep_(-1),
        type_item(-1),
        icon_index_(-1),
        tree_left_off_(0)
    {
        font_ID = StrToWzID("st10");
        memset(tree_item_str_,0,sizeof(TCHAR)*256);

        text_Color_ = RGBA (255, 255, 255, 255);
        sell_Color_ = RGBA (0,0,0,0);
        select_Color_ = RGBA (255,255,255,255);
        back_Color_ = RGBA(0,220,0,255);

        user_data_Ptr_ = NULL;

        isFolder = false;
    }
};
class TreebaseItem
{
public:
    int offset_data_;
    int offset_col_;
    int max_insert_data_;
    bool isSelect_item_;

    //총 펼처진 리스트의 몇번째 순위인지?
    int origin_value_;
    int IDStatic;
public:
    _stTreedata tree_data;

    typedef std::vector<TreebaseItem*> child_tree;
    child_tree    child_list_;
    TreebaseItem*  Perent_tree_;
    
public:
    TreebaseItem() : tree_data()
        ,offset_data_(0)
        ,Perent_tree_(NULL)
        ,isSelect_item_(false)
        ,max_insert_data_(0)
    {
    }

    virtual ~TreebaseItem()
    {
        deleteAllItem();
    }

    bool is_open() { return tree_data.isFolder; }
    void set_open(bool set) { tree_data.isFolder = set; }
    bool is_leaf() { return child_list_.empty(); }
    
    bool is_select() { return isSelect_item_; }
    void set_select(bool set) { isSelect_item_ = set; }

    int  next_origin_value() 
    { 
        int next = is_open() ? 1 : child_list_.size() ;
        return origin_value_ + next;
    }

    child_tree& getchilditem() { return child_list_; }

    void deleteAllItem()
    {
        child_tree::iterator iter_begin = child_list_.begin();
        child_tree::iterator iter_End = child_list_.end();

        for ( ; iter_begin != iter_End ; ++iter_begin)
        {
            delete (*iter_begin);
            *iter_begin = NULL;
        }

        child_list_.clear();
    }

    TreebaseItem* OriginItem(int origin_value)
    {
        if(origin_value == IDStatic)
            return this;

        for (int i = 0 ; i < (int)child_list_.size() ; ++i)
        {
            TreebaseItem* OriginItemValue =child_list_[i]->OriginItem(origin_value);
            if( OriginItemValue != NULL)
            {
                return OriginItemValue;
            }
        }

        return NULL;
    }


    bool deleteTreeItem(TreebaseItem* baseItem)
    {
        child_tree::iterator iter_begin = child_list_.begin();
        child_tree::iterator iter_End = child_list_.end();

        for ( ; iter_begin != iter_End ; )
        {
            if((*iter_begin) == baseItem)
            { 
                iter_begin = child_list_.erase(iter_begin);
                delete baseItem;
                baseItem= NULL;

                return true;
            }
            else
            {
                if((*iter_begin)->deleteTreeItem(baseItem))
                {
                    return true;
                }

                 ++iter_begin;
            }
        }
        return false;
    }
    void addChildTree(TreebaseItem* Tree,int& origin_);
    void push_Item( CCtrlListWZ* list_control);
    virtual void render(int SelectCell,int Over_sell,RECT& Render_Pos);
    //렌더링을 제정의 하여 쓸수 있음.
    static void OnStaticRender(CCtrlListWZ* list_control, 
        CCtrlListWZ::CListSellData* sell_data, 
        int sell_index, 
        RECT& render_area);
};

class TREEITEM : public TreebaseItem
{
public:
    TREEITEM() : TreebaseItem()
    {

    }

    virtual ~TREEITEM()
    {
        
    }
    
    virtual void render(int SelectCell,int Over_sell,RECT& Render_Pos)
    {
        /* todo */

        TreebaseItem::render(SelectCell,Over_sell,Render_Pos);
    }
};

class GaugeTreeItem : public TreebaseItem
{
private:
    TCHAR percent_string[256];
    int   Min_gauge;
    int   Max_gauge;
    int   Current_Gauge;
    int   Check_item_Count_;

    bool  is_perscent;

    float GetNowtime ;
    bool turnChange ;
    int			all_hide_alpha;
    DWORD m_LastTickCount;
public:
    GaugeTreeItem() : TreebaseItem()
        ,Max_gauge(0)
        ,Current_Gauge(0)
        ,is_perscent(false)
        ,Check_item_Count_(0)
        ,GetNowtime(0)
        ,turnChange(false)
        ,all_hide_alpha(-1)
        ,m_LastTickCount(0)
    {

    }

    virtual ~GaugeTreeItem()
    {

    }

    virtual void render(int SelectCell,int Over_sell,RECT& Render_Pos);
    

    void SetPercent(bool percent)
    {
        is_perscent = percent;
    }

    void SetPos(int pos)
    {
        if(pos > Max_gauge)
        {
            return;
        }

        Current_Gauge = pos;
    }

    void SetRange(int minpos, int maxpos)
    {
        Min_gauge = minpos;
        Max_gauge = maxpos;
    }

    void SetPercentString(const float& percent_ratio)
    {
        Sprintf(percent_string, "%.1f%%", percent_ratio);
    }

    void AddCheckItem()
    {
        Check_item_Count_++;
    }

    void MinusCheckItme()
    {
        Check_item_Count_--;
    }

    void SetCheckItem(int Count)
    {
        Check_item_Count_ = Count;
    }
};

class CheckBoxTree : public TreebaseItem
{
public:
    CheckBoxTree() : TreebaseItem()
    {

    }

    virtual ~CheckBoxTree()
    {

    }

    virtual void render(int SelectCell,int Over_sell,RECT& Render_Pos)
    {
        /* todo */

        TreebaseItem::render(SelectCell,Over_sell,Render_Pos);

        RECT render_icon = Render_Pos;

        if(is_leaf())
        {
            BASE_TextureListInfo* tex_info = NULL;
            HANDLE tex_handle = TextureListInfoHandleManager::Instance()\
                ->GetTexture(isSelect_item_ ? 10006 : 10005, tex_info);;

            if ((tex_handle != INVALID_HANDLE_VALUE) && (tex_info != NULL))
            {

                render_icon.left += (((tree_data.tree_deep_) * tree_data.tree_left_off_) + offset_data_) - (tex_info->tex_size.right + 5);
                RECT dest_area = render_icon;
                dest_area.right = tex_info->tex_size.right;
                dest_area.bottom = offset_col_;

                g_pSunRenderer->RenderTexture(tex_handle, 
                    (float)dest_area.left, 
                    (float)dest_area.top, 
                    (float)dest_area.right,
                    (float)dest_area.bottom,
                    (float)tex_info->tex_size.left,
                    (float)tex_info->tex_size.top,
                    (float)tex_info->tex_size.right,
                    (float)tex_info->tex_size.bottom);
            }
        }
    }
};

