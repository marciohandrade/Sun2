#include "SunClientPrecompiledHeader.h"
#include "DlgPosInfoParser.h"
#include "TextureAnimationInfoParser.h"
#include "Application.h"

//------------------------------------------------------------------------------ 
TextureAnimationInfoParser::TextureAnimationInfoParser(void)
{
    animationinfo_map().clear();
}
//------------------------------------------------------------------------------ 
TextureAnimationInfoParser::~TextureAnimationInfoParser(void)
{
}
//------------------------------------------------------------------------------ 
void TextureAnimationInfoParser::Release()
{
    animationinfo_map().clear();
}
//------------------------------------------------------------------------------ 
BOOL TextureAnimationInfoParser::LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload)
{
    if (bReload == TRUE)
    {
        animationinfo_map().clear();
    }

    int nRowSize = GetRowSize();
    for (int nRow = 0; nRow < nRowSize; ++nRow)
    {
        BASE_TextureAnimationInfo info;

        info.group_index = GetDataDWORD("group_index", nRow);
        info.tick_millisecond = GetDataDWORD("millisecond", nRow);
        info.alpha = GetDataFloat("alpha", nRow);
        info.blend_mode = static_cast<ENUM_ALPHABLEND>(GetDataDWORD("blend_mode", nRow));

        info.align_x = static_cast<HORIZONTAL_ARRANGE>(GetDataInt("align_x", nRow));
        info.align_y = static_cast<VERTICAL_ARRANGE>(GetDataInt("align_y", nRow));
        info.dest_x = GetDataInt("dest_x", nRow);
        info.dest_y = GetDataInt("dest_y", nRow);
        info.dest_wide_x = GetDataInt("dest_wide_x", nRow);
        info.dest_wide_y = GetDataInt("dest_wide_y", nRow);
        info.dest_width = GetDataInt("dest_width", nRow);
        info.dest_height = GetDataInt("dest_height", nRow);
        info.dest_half_wide_x = GetDataInt("dest_half_wide_x", nRow);
        info.dest_half_wide_y = GetDataInt("dest_half_wide_y", nRow);

        info.texture_index = GetDataDWORD("texture_index", nRow);

        AnimationInfo_map::iterator iter = animationinfo_map().find(info.group_index);
        if (iter != animationinfo_map().end())
        {
            (iter->second).push_back(info);
        }
        else
        {
            std::vector<BASE_TextureAnimationInfo> vec;
            vec.push_back(info);
            animationinfo_map().insert(std::make_pair(info.group_index, vec));
        }
    }

    return TRUE;
}
//------------------------------------------------------------------------------ 
std::vector<BASE_TextureAnimationInfo>* TextureAnimationInfoParser::animation_key(DWORD group_index)
{
    AnimationInfo_map::iterator iter = animationinfo_map().find(group_index);
    if (iter != animationinfo_map().end())
    {
        return &(iter->second);
    }
    return NULL;
}
//------------------------------------------------------------------------------ 
bool TextureAnimationInfoParser::GetDestRect(OUT RECT& out_rect, BASE_TextureAnimationInfo* info)
{
    POINT pt = g_pApplication->Get2DModeMapping();

    RECT rect;
    rect.left = rect.top = 0;
    rect.right = pt.x;
    rect.bottom = pt.y;

    out_rect.left = GetX(info, rect);
    out_rect.top = GetY(info, rect);
    out_rect.right = info->dest_width;
    out_rect.bottom = info->dest_height;

    return true;
}

//------------------------------------------------------------------------------ 
int TextureAnimationInfoParser::GetX(BASE_TextureAnimationInfo* info, RECT& window_rect)
{
    if (info == NULL)
    {
        return 0;
    }

    bool is_wide = (g_pApplication->IsWideModeFlag() == TRUE);
    int offset = (is_wide) ? info->dest_wide_x : info->dest_x;

    if (g_pApplication->screen_ratio() == SR_4_3)   offset = info->dest_x;
    if (g_pApplication->screen_ratio() == SR_16_9)  offset = info->dest_wide_x;
    if (g_pApplication->screen_ratio() == SR_16_10) offset = info->dest_half_wide_x;

    int total_width = window_rect.right - info->dest_width;

    if (info->align_x == HORIZONTAL_LEFT)
    {
        return offset;
    }
    else if (info->align_x == HORIZONTAL_MIDDLE)
    {
        return (total_width / 2) + offset;
    }
    else if (info->align_x == HORIZONTAL_RIGHT)
    {
        return (total_width - abs(offset));
    }

    return 0;
}
//------------------------------------------------------------------------------ 
int TextureAnimationInfoParser::GetY(BASE_TextureAnimationInfo* info, RECT& window_rect)
{
    if (info == NULL)
    {
        return 0;
    }

    bool is_wide = (g_pApplication->IsWideModeFlag() == TRUE);
    int offset = (is_wide) ? info->dest_wide_y : info->dest_y;

    if (g_pApplication->screen_ratio() == SR_4_3)   offset = info->dest_y;
    if (g_pApplication->screen_ratio() == SR_16_9)  offset = info->dest_wide_y;
    if (g_pApplication->screen_ratio() == SR_16_10) offset = info->dest_half_wide_y;

    int total_height = window_rect.bottom - info->dest_height;

    if (info->align_y == VERTICAL_TOP)
    {
        return offset;
    }
    else if (info->align_y == VERTICAL_MIDDLE)
    {
        return (total_height / 2) + offset;
    }
    else if (info->align_y == VERTICAL_BOTTOM)
    {
        return (total_height - abs(offset));
    }

    return 0;
}
