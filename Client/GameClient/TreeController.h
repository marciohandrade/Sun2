#pragma once

#include "TreeItemData.h"

typedef class TreebaseItem *HTREEITEM;

class TreeController
{
public:
    TreeController(void);
    ~TreeController(void);

    //�ϳ��� Ʈ���� �� col���� ������
    HTREEITEM AddTreeItem(const TCHAR* ItemName, 
        int Icon_index, 
        int Type_item,
        bool isfolder
        ,DWORD font_ID,
        void* user_date_ptr = NULL
        ,HTREEITEM perent = NULL);

    HTREEITEM AddTreeItemGauge(const TCHAR* ItemName, 
        int Icon_index, 
        int Type_item,
        bool isfolder
        ,DWORD font_ID,
        int maxGauge,
        void* user_date_ptr = NULL
        ,HTREEITEM perent = NULL);

    HTREEITEM AddTreeItemCheckBox(const TCHAR* ItemName, 
        int Icon_index, 
        int Type_item,
        bool isfolder
        ,DWORD font_ID,
        void* user_date_ptr = NULL
        ,HTREEITEM perent = NULL);


    void      Release();
    void      MessageProc(SI_MESSAGE* msg);
    HTREEITEM GetSelItem() { return select_item_; }

    void      SetSellItem(HTREEITEM item);

    void      update();

    void      DeleteAllItem();
    TreebaseItem* GetTreeItem() { return &root_tree_; }
    HTREEITEM   getOrigin(int origin_value);
protected:
    void      UpdateTree(HTREEITEM item);
    void      OnLbuttomDown();
    void      resetScroll();

    void      UpScroll();
    void      DownScroll();
    void      ThumbScroll();

    
    void      Reflush(HTREEITEM item);
    void      Render_settting(HTREEITEM item);
    

public:
    void      set_scroll_tick_move_value(const int& scroll_tick_move);
    void      Reflush();
private:
    //�������� Ʈ������ ������� �ʴ�.
    TreebaseItem   root_tree_; 
    HTREEITEM     select_item_;

    int           Sell_range_;
    int           Left_Gap;
    int           render_max_object_counter_;
    int           origin_value_;
    int           scroll_tick_move_;

    CCtrlVScrollWZ* vscroll_control_;
    CCtrlListWZ*    list_control_;


public:
    //���� ������ ����
    static TreeController* CreateTree(int Sell,int Leff_offset,
        CCtrlVScrollWZ* VScroller, CCtrlListWZ* listcontrol);
};
