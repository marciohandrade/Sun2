#include "SunClientPrecompiledHeader.h"
#include ".\treeitemdata.h"
#include "Mouse.h"

void TreebaseItem::push_Item( CCtrlListWZ* list_control )
{
    if(Perent_tree_ && Perent_tree_->tree_data.isFolder == false)
    {
        return;
    }

    if(tree_data.tree_deep_ != -1)
    {
        int index = list_control->InsertItemBack();

        list_control->SetItemWZA(index, 
            tree_data.tree_item_str_, 
            tree_data.font_ID, 
            DT_VCENTER | DT_LEFT, 
            tree_data.back_Color_, 
            tree_data.text_Color_, 
            tree_data.sell_Color_);

        list_control->SetItemUserData(index, 0, (void*)this);
        list_control->SetItemCallbackDraw(index, 0, OnStaticRender);
    }

    if(!tree_data.isFolder)
    {
        return;
    }

    for (int i = 0 ; i < (int)child_list_.size() ; ++i)
    {
        child_list_[i]->push_Item(list_control);
    }
}

void TreebaseItem::addChildTree( TreebaseItem* Tree,int& origin_ )
{
    Tree->Perent_tree_ = this;
    //해당 트리의 깊이는 부모의 깊이 +1
    Tree->tree_data.tree_deep_ = tree_data.tree_deep_ + 1;
    Tree->origin_value_ = origin_;

    //오리진 값을 1 증가 시킴
    origin_ += 1;

    child_list_.push_back(Tree);
}

void TreebaseItem::render( int SelectCell,int Over_sell,RECT& Render_Pos )
{
    RECT render_icon = Render_Pos;
    RECT render_area = Render_Pos;

    render_icon.left += (tree_data.tree_deep_) * tree_data.tree_left_off_;

    int iconOffset = 0;
    if(!is_leaf())
    {
        BASE_TextureListInfo* tex_info = NULL;
        HANDLE tex_handle = TextureListInfoHandleManager::Instance()\
            ->GetTexture(is_open() ? 10003 : 10002, tex_info);;

        if ((tex_handle != INVALID_HANDLE_VALUE) && (tex_info != NULL))
        {
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

        iconOffset = tex_info->tex_size.right + 3;
        offset_data_ = iconOffset;
    }
    else
    {
        offset_data_ = Perent_tree_->offset_data_ - 3;
    }

    render_area.left += ((tree_data.tree_deep_) * tree_data.tree_left_off_) + offset_data_;

    g_pSunRenderer->x_pManagerTextOut->StartToUseFont(tree_data.font_ID);
    {
        g_pSunRenderer->x_pManagerTextOut->DrawText(tree_data.tree_item_str_,
            &render_area,
            isSelect_item_ ? tree_data.select_Color_: tree_data.text_Color_,
            WzColor_RGBA(0, 0, 0, 0),
            TP_HLEFT | TP_VCENTER);
    }
    g_pSunRenderer->x_pManagerTextOut->EndUseFont();

}

void TreebaseItem::OnStaticRender( CCtrlListWZ* list_control, CCtrlListWZ::CListSellData* sell_data, int sell_index, RECT& render_area )
{
    if (list_control == NULL || sell_data == NULL || sell_data->user_data == NULL) 
    {
        return;
    }

    int select_sell = list_control->GetCurSel();
    int over_sell = list_control->GetOverSel();

    TreebaseItem* tree_object = static_cast<TreebaseItem*>(sell_data->user_data);
    tree_object->render(select_sell,over_sell,render_area);
}

void GaugeTreeItem::render( int SelectCell,int Over_sell,RECT& Render_Pos )
{
    
    RECT render_icon = Render_Pos;
    RECT render_area = Render_Pos;

    render_icon.left += (tree_data.tree_deep_) * tree_data.tree_left_off_ - 10;

    int iconOffset = 0;
    if(!is_leaf())
    {
        BASE_TextureListInfo* tex_info = NULL;
        HANDLE tex_handle = TextureListInfoHandleManager::Instance()\
            ->GetTexture(is_open() ? 10003 : 10002, tex_info);;

        if ((tex_handle != INVALID_HANDLE_VALUE) && (tex_info != NULL))
        {
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

        iconOffset = tex_info->tex_size.right + 3;
        offset_data_ = iconOffset;
    }
    else
    {
        offset_data_ = Perent_tree_->offset_data_;
    }


    RECT render_area2 = Render_Pos;
    RECT text_area = Render_Pos;

    
    int x	= Mouse::Instance()->GetMouseX();
    int y	= Mouse::Instance()->GetMouseY();
    DWORD Texture_number = 10030;

    if(isSelect_item_)
    {
        Texture_number = 10032;
    }
    else if(Render_Pos.left < x && Render_Pos.right > x &&
            Render_Pos.top < y && Render_Pos.bottom > y)
    {
        Texture_number = 10031;
    }


    BASE_TextureListInfo* tex_info = NULL;

    HANDLE tex_handle = TextureListInfoHandleManager::Instance()\
        ->GetTexture(Texture_number, tex_info);;

    render_area2.left += (offset_data_ - 6) - 10; 
    RECT dest_area = render_area2;
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
    
     RECT render_gauge = Render_Pos;
     render_gauge.left += (offset_data_ + 1)-10;
    //여기서 막대기 렌더링만 변궁
     float Item_percent = Current_Gauge == 0 ? 0.0f : (float)Current_Gauge / (float)Max_gauge;
     float Right_item = 0.0f;

    if(is_leaf())
    {
        HANDLE tex_handle2 = TextureListInfoHandleManager::Instance()\
            ->GetTexture(10034, tex_info);;

        RECT dest_area = render_gauge;
        dest_area.top += 4;
        dest_area.bottom = offset_col_ - 8;

        Right_item = (float)tex_info->tex_size.right * Item_percent;

        g_pSunRenderer->RenderTexture(tex_handle2, 
            (float)dest_area.left, 
            (float)dest_area.top, 
            Right_item,
            (float)dest_area.bottom,
            (float)tex_info->tex_size.left,
            (float)tex_info->tex_size.top,
            (float)tex_info->tex_size.right,
            (float)tex_info->tex_size.bottom);
    }
    else
    {
        HANDLE tex_handle2 = TextureListInfoHandleManager::Instance()\
            ->GetTexture(10033, tex_info);;

        RECT dest_area = render_gauge;
        dest_area.top += 4;
        dest_area.bottom = offset_col_ - 8;

        Right_item = (float)tex_info->tex_size.right * Item_percent;

        g_pSunRenderer->RenderTexture(tex_handle2, 
            (float)dest_area.left, 
            (float)dest_area.top, 
            Right_item,
            (float)dest_area.bottom,
            (float)tex_info->tex_size.left,
            (float)tex_info->tex_size.top,
            (float)tex_info->tex_size.right,
            (float)tex_info->tex_size.bottom);
    }

    render_area.left += offset_data_;

    g_pSunRenderer->x_pManagerTextOut->StartToUseFont(tree_data.font_ID);
    {
        g_pSunRenderer->x_pManagerTextOut->DrawText(tree_data.tree_item_str_,
            &render_area,
            isSelect_item_ ? tree_data.select_Color_: tree_data.text_Color_,
            WzColor_RGBA(0, 0, 0, 0),
            TP_HLEFT | TP_VCENTER);
    }
    g_pSunRenderer->x_pManagerTextOut->EndUseFont();


    //렌더링되는 라이트 위치
    int Right = Render_Pos.right;

    g_pSunRenderer->x_pManagerTextOut->StartToUseFont(tree_data.font_ID);
    {
        //현재의 게이지 표시
        TCHAR item_gauge[256];
        if(is_perscent)
        {
            if (is_leaf())
            {
                strcpy(item_gauge, percent_string);
            }
            else
            {
                // 혹시 모를 예외처리
                if (Current_Gauge > Max_gauge)
                {
                    Current_Gauge = Max_gauge;
                }

               sprintf(item_gauge,"%.1f%%",Current_Gauge == 0 ? 0.0f :\
                    ((float)Current_Gauge / (float)Max_gauge * 100));
            }
        }
        else
        {
            sprintf(item_gauge,"%d/%d",Current_Gauge,Max_gauge);        
        }

        SIZE siText;
        g_pSunRenderer->x_pManagerTextOut->GetTextExtentPointA( item_gauge, strlen(item_gauge)
            , &siText);

        text_area.left = (Right - siText.cx) - 5;

        g_pSunRenderer->x_pManagerTextOut->DrawText(item_gauge,
            &text_area,
            WzColor_RGBA(247, 251, 190, 255),
            WzColor_RGBA(0, 0, 0, 0),
            TP_HLEFT | TP_VCENTER);
    }
    g_pSunRenderer->x_pManagerTextOut->EndUseFont();

    

    if(Check_item_Count_ > 0)
    {
        HANDLE tex_handle3 = TextureListInfoHandleManager::Instance()\
            ->GetTexture(10035, tex_info);

        RECT dest_area = text_area;
        dest_area.left -= 7;
        dest_area.bottom = offset_col_ - 8;

        //현재 색상에서 알파 색상으로 
        WzColor color[4];
        DWORD tick_count = clock_function::GetTickCount();

        if (m_LastTickCount == 0)
        {
            m_LastTickCount = tick_count;
        }

        DWORD current_tick_count = (tick_count - m_LastTickCount);
        if (current_tick_count > 30)
        {
            m_LastTickCount = tick_count;
        }
        //선형보간 공식 g(t) = y0 * (1.0f - t) + y1*t;
        //1초의 단위로 나눈다.
        GetNowtime += (float)(current_tick_count/1000.0f);

        int postAlpha = all_hide_alpha;
        all_hide_alpha = GetNowtime >= 1.0f ? (int)((GetNowtime=0.0f) +\
            ( turnChange==true) ? -1 : 0):\
            ( turnChange==false ? -1 : 0 );

        turnChange = (postAlpha != all_hide_alpha) ? !turnChange : turnChange;

        int ResultLerpColor = (all_hide_alpha == -1) ? (int)(0 *  (1.0f - GetNowtime) + 255 * GetNowtime) : \
            (int)(255 *  (1.0f - GetNowtime) + 0 * GetNowtime);

        color[0] = WzColor_RGBA(255,255,255,ResultLerpColor);
        color[1] = WzColor_RGBA(255,255,255,ResultLerpColor);
        color[2] = WzColor_RGBA(255,255,255,ResultLerpColor);
        color[3] = WzColor_RGBA(255,255,255,ResultLerpColor);

        g_pSunRenderer->RenderTexture(tex_handle3, 
            (float)dest_area.left, 
            (float)dest_area.top, 
            (float)tex_info->tex_size.right,
            (float)tex_info->tex_size.bottom,
            (float)tex_info->tex_size.left,
            (float)tex_info->tex_size.top,
            (float)tex_info->tex_size.right,
            (float)tex_info->tex_size.bottom,
            color);
    }

}