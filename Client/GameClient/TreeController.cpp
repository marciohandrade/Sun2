#include "SunClientPrecompiledHeader.h"
#include "Interface\uiBase.h"
#include "treecontroller.h"

TreeController::TreeController(void)
{
    select_item_ = NULL;

    //상위 트리 자체는 열린트리.
    root_tree_.tree_data.isFolder = true;
    origin_value_ = 0;

    scroll_tick_move_ = 1;
}

TreeController::~TreeController(void)
{
}

HTREEITEM TreeController::AddTreeItem( const TCHAR* ItemName , 
                                      int Icon_index, 
                                      int Type_item,
                                      bool isfolder,
                                      DWORD font_ID,
                                      void* user_date_ptr,
                                      HTREEITEM perent )
{
    TreebaseItem* TreeItem = new TreebaseItem();
    strcpy(TreeItem->tree_data.tree_item_str_,ItemName);
    TreeItem->tree_data.icon_index_ = Icon_index;
    TreeItem->tree_data.type_item = Type_item;
    TreeItem->tree_data.font_ID = font_ID;
    TreeItem->tree_data.tree_left_off_ = Left_Gap;
    TreeItem->offset_col_ = Sell_range_;
    TreeItem->tree_data.user_data_Ptr_ = user_date_ptr;
    TreeItem->set_open(isfolder);

    if(perent)
    {
        perent->addChildTree(TreeItem,origin_value_);
    }
    else
    {
        root_tree_.addChildTree(TreeItem,origin_value_);
    }

    Reflush(TreeItem);
    
    return TreeItem;
}



HTREEITEM TreeController::AddTreeItemGauge( const TCHAR* ItemName, 
                                           int Icon_index, 
                                           int Type_item,
                                           bool isfolder ,
                                           DWORD font_ID, 
                                           int maxGauge, 
                                           void* user_date_ptr /*= NULL */,
                                           HTREEITEM perent /*= NULL*/ )
{
    GaugeTreeItem* TreeItem = new GaugeTreeItem();
    strcpy(TreeItem->tree_data.tree_item_str_,ItemName);
    TreeItem->tree_data.icon_index_ = Icon_index;
    TreeItem->tree_data.type_item = Type_item;
    TreeItem->tree_data.font_ID = font_ID;
    TreeItem->tree_data.tree_left_off_ = Left_Gap;
    TreeItem->offset_col_ = Sell_range_;
    TreeItem->tree_data.user_data_Ptr_ = user_date_ptr;
    TreeItem->set_open(isfolder);

    TreeItem->SetRange(0,maxGauge);
    TreeItem->SetPos(0);

    if(perent)
    {
        perent->addChildTree(TreeItem,origin_value_);
    }
    else
    {
        root_tree_.addChildTree(TreeItem,origin_value_);
    }

    Reflush(TreeItem);

    return TreeItem;
}

HTREEITEM TreeController::AddTreeItemCheckBox( const TCHAR* ItemName, 
                                              int Icon_index, 
                                              int Type_item, 
                                              bool isfolder ,
                                              DWORD font_ID,
                                              void* user_date_ptr /*= NULL */
                                              ,HTREEITEM perent /*= NULL*/ )
{
    CheckBoxTree* TreeItem = new CheckBoxTree();
    strcpy(TreeItem->tree_data.tree_item_str_,ItemName);
    TreeItem->tree_data.icon_index_ = Icon_index;
    TreeItem->tree_data.type_item = Type_item;
    TreeItem->tree_data.font_ID = font_ID;
    TreeItem->tree_data.tree_left_off_ = Left_Gap;
    TreeItem->offset_col_ = Sell_range_;
    TreeItem->tree_data.user_data_Ptr_ = user_date_ptr;
    TreeItem->set_open(isfolder);

    if(perent)
    {
        perent->addChildTree(TreeItem,origin_value_);
    }
    else
    {
        root_tree_.addChildTree(TreeItem,origin_value_);
    }

    Reflush(TreeItem);

    return TreeItem;
}

TreeController* TreeController::CreateTree( int Sell,int Leff_offset, CCtrlVScrollWZ* VScroller, CCtrlListWZ* listcontrol )
{
    TreeController* Controller = new TreeController();
    Controller->Sell_range_ = Sell;
    Controller->vscroll_control_ = VScroller;
    Controller->list_control_ = listcontrol;
    Controller->Left_Gap = Leff_offset;

    if(Controller->list_control_)
    {
        Controller->list_control_->SetSellHeight(Controller->Sell_range_);
    }
    
    if(Controller->list_control_)
    {
        Controller->render_max_object_counter_ = listcontrol->GetSize().bottom / Sell;
    }

    return Controller;
}

void TreeController::Reflush()
{
    if (list_control_ == NULL)
    {
        return;
    }

    if (root_tree_.getchilditem().empty())
    {
        return;
    }

    list_control_->DeleteItemAll();

    Reflush(&root_tree_);

    resetScroll();
}

void TreeController::Reflush( HTREEITEM item )
{
    if (list_control_ == NULL)
    {
        return;
    }

    item->push_Item(list_control_);
}

void TreeController::MessageProc( SI_MESSAGE* msg )
{
    switch (msg->eResultMsg)
    {
    case RT_MSG_LBUTTONCLICK:
            OnLbuttomDown();
        break;
    case RT_MSG_SB_LINETOP:
        {
            if(vscroll_control_&& msg->dwCtrlID == vscroll_control_->GetCtrlID())
            {
                UpScroll();
            }
        }
        break;

    case RT_MSG_SB_LINEBOTTOM:
        {
            if(vscroll_control_&& msg->dwCtrlID == vscroll_control_->GetCtrlID())
            {
                DownScroll();
            }
        }
        break;

    case RT_MSG_SB_THUMBTRACK:      //!< 썸을 드래그 하고 있다.
    case RT_MSG_SB_THUMBPOSITION:   //!< 썸을 드래그한후 마우스를 놓았다.
    case RT_MSG_SB_ENDSCROLL:       //!< 스크롤이 종료 되었다.
        {
            if(vscroll_control_ && msg->dwCtrlID == vscroll_control_->GetCtrlID())
            {
                ThumbScroll();
            }
        }
        break;
    }
}

void TreeController::UpdateTree( HTREEITEM item )
{
    if(item == NULL)
    {
        return;
    }

    item->set_select(true);

    //리프 아이템의 경우 갱신할 필요 없음
    if(item->is_leaf())
    {
        return;
    }

    bool isOpen = item->is_open();
    item->set_open(!isOpen);
    

    //폴더 갱신
    Reflush();
}

void TreeController::OnLbuttomDown()
{
    //이전에 선택된 목록은 선택해지
    if(select_item_)
    {
        select_item_->set_select(false);
    }
    
    HTREEITEM Contexted_item = (HTREEITEM)(list_control_->GetItemUserData(list_control_->GetCurSel(), 0));
    
    if(Contexted_item)
    {
        select_item_ = Contexted_item;
    }
  

    UpdateTree(select_item_);
}

void TreeController::UpScroll()
{
    if(vscroll_control_ == NULL)
    {
        return;
    }

    int pos = vscroll_control_->GetScrollPosWZ();
    int mmin = 0, mmax = 0;
    vscroll_control_->GetScrollRangeWZ(&mmin, &mmax);
    if(pos > mmin)
    {        
        pos -= scroll_tick_move_;
        vscroll_control_->SetScrollPosWZ(pos);
    }

    list_control_->SetBeginDrawIndex(pos);
}

void TreeController::DownScroll()
{
    if(vscroll_control_ == NULL)
    {
        return;
    }

    int pos = vscroll_control_->GetScrollPosWZ();
    int mmin = 0, mmax = 0;
    vscroll_control_->GetScrollRangeWZ(&mmin, &mmax);

    if (pos < mmax)
    {        
        pos += scroll_tick_move_;
        vscroll_control_->SetScrollPosWZ(pos);
    }

    list_control_->SetBeginDrawIndex(pos);
}


void TreeController::ThumbScroll()
{
    if(vscroll_control_ == NULL)
    {
        return;
    }

    int pos2 = vscroll_control_->GetScrollPosWZ();
    int pos = pos2;
    int mmin = 0, mmax = 0;
    vscroll_control_->GetScrollRangeWZ(&mmin, &mmax);

    pos = max(mmin, pos);
    pos = min(mmax, pos);

    if(pos != pos2)
    {
        vscroll_control_->SetScrollPosWZ(pos);
    }

    list_control_->SetBeginDrawIndex(pos);
}

void TreeController::resetScroll()
{
    if (vscroll_control_ == NULL || list_control_ == NULL)
    {
        return;
    }

    int scroll_max_num = max(0, list_control_->GetListRowCnt() - list_control_->GetDrawRowCount());

    int thumb_size = 100 / (scroll_max_num + 1);
    thumb_size = max(10, thumb_size);
    vscroll_control_->SetChangeSizeThumb(thumb_size);
    vscroll_control_->SetScrollRangeWZ(0, scroll_max_num);

    int begin_index = list_control_->GetBeginDrawIndex();

    int new_draw_index = begin_index <= scroll_max_num ? begin_index : scroll_max_num;

    list_control_->SetBeginDrawIndex(new_draw_index);
    vscroll_control_->SetScrollPosWZ(new_draw_index);

}

void TreeController::update()
{
    if (list_control_ != NULL && vscroll_control_ != NULL)
    {
        RECT area = list_control_->GetSizeRect();
        BYTE wheel_msg = uiBase::GetMouseWheelStatus(&area);

        switch (wheel_msg)
        {
        case MOUSE_WHEEL_UP:    return UpScroll();
        case MOUSE_WHEEL_DOWN:  return DownScroll();
        }
    }

}

void TreeController::DeleteAllItem()
{
    root_tree_.deleteAllItem();
    list_control_->DeleteItemAll();
    select_item_ = NULL;

    if(vscroll_control_)
    {
        vscroll_control_->SetScrollRangeWZ(0, 0);
        vscroll_control_->SetScrollPosWZ(0);
    }
    
    Reflush();
}

void TreeController::Release()
{
    root_tree_.deleteAllItem();
}

void TreeController::set_scroll_tick_move_value( const int& scroll_tick_move )
{
    scroll_tick_move_ = scroll_tick_move;
}

HTREEITEM TreeController::getOrigin( int origin_value )
{
    for (int i = 0 ; i < (int)root_tree_.child_list_.size() ; ++i)
    {
        HTREEITEM item = root_tree_.child_list_[i]->OriginItem(origin_value);
        if(item)
        {
            return item;
        }
    }

    return NULL;
}

void TreeController::SetSellItem( HTREEITEM item )
{
    if(select_item_)
        select_item_->set_select(false);

    select_item_ = item;
    select_item_->set_select(true);

    UpdateTree(select_item_);
}