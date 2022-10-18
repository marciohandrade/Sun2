#include "SunClientPrecompiledHeader.h"

#include "TreeMenuInfoParser.h"

//------------------------------------------------------------------------------ 
TreeMenuInfoParser::TreeMenuInfoParser(void)
{
    treecontrol_multimap().clear();
}
//------------------------------------------------------------------------------ 
TreeMenuInfoParser::~TreeMenuInfoParser(void)
{
}
//------------------------------------------------------------------------------ 
void TreeMenuInfoParser::Release()
{
    treecontrol_multimap().clear();
}
//------------------------------------------------------------------------------ 
BOOL TreeMenuInfoParser::LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload)
{
    if (bReload == TRUE)
    {
        treecontrol_multimap().clear();
    }

    int nRowSize = GetRowSize();
    for (int nRow = 0; nRow < nRowSize; ++nRow)
    {
        BASE_TreeMenuInfo info;

        info.tree_group = static_cast<BASE_TreeMenuInfo::Group_Type>(GetDataInt("Group", nRow));
        info.menu_depth[3] =GetDataBYTE("Menu_Index1", nRow);
        info.menu_depth[2] =GetDataBYTE("Menu_Index2", nRow);
        info.menu_depth[1] =GetDataBYTE("Menu_Index3", nRow);
        info.menu_depth[0] =GetDataBYTE("Menu_Index4", nRow);
        info.menu_type = static_cast<BASE_TreeMenuInfo::Menu_Type>(GetDataInt("Menu_type", nRow));
        info.menu_depth_width = GetDataInt("Menu_Depth", nRow);
        info.string_code = GetDataDWORD("String_Code", nRow);

        info.font_id[BASE_TreeMenuInfo::kFontType_Enable] = StrToWzID(GetDataString("FontID_Enable", nRow));
        info.font_id[BASE_TreeMenuInfo::kFontType_Disable] = StrToWzID(GetDataString("FontID_Disable", nRow));
        info.font_id[BASE_TreeMenuInfo::kFontType_Over] = StrToWzID(GetDataString("FontID_Over", nRow));
        info.font_id[BASE_TreeMenuInfo::kFontType_Select] = StrToWzID(GetDataString("FontID_Select", nRow));

        sscanf(GetDataString("FontColor_Enable", nRow), "%x", &(info.font_color[BASE_TreeMenuInfo::kFontType_Enable]));
        sscanf(GetDataString("FontColor_Disable", nRow), "%x", &(info.font_color[BASE_TreeMenuInfo::kFontType_Disable]));
        sscanf(GetDataString("FontColor_Over", nRow), "%x", &(info.font_color[BASE_TreeMenuInfo::kFontType_Over]));
        sscanf(GetDataString("FontColor_Select", nRow), "%x", &(info.font_color[BASE_TreeMenuInfo::kFontType_Select]));

        info.image_code[BASE_TreeMenuInfo::kIconType_Enable] = GetDataDWORD("ImageIndex_Enable", nRow);
        info.image_code[BASE_TreeMenuInfo::kIconType_Disable] = GetDataDWORD("ImageIndex_Disable", nRow);
        info.image_code[BASE_TreeMenuInfo::kIconType_Collpase] = GetDataDWORD("ImageIndex_Callpase", nRow);
        info.image_code[BASE_TreeMenuInfo::kIconType_Unfold] = GetDataDWORD("ImageIndex_Unfold", nRow);
        info.image_code[BASE_TreeMenuInfo::kIconType_CheckOn] = GetDataDWORD("ImageIndex_CheckOn", nRow);
        info.image_code[BASE_TreeMenuInfo::kIconType_CheckOff] = GetDataDWORD("ImageIndex_CheckOff", nRow);
        info.image_code[BASE_TreeMenuInfo::kIconType_Cursor] = GetDataDWORD("ImageIndex_Cursor", nRow);

        treecontrol_multimap().insert(std::make_pair(info.tree_group, info));
    }

    return TRUE;
}
//------------------------------------------------------------------------------ 
int TreeMenuInfoParser::GetTreeMenu(OUT std::vector<BASE_TreeMenuInfo*>& value_vector, int group)
{
    std::pair<TreeMenu_multimap::iterator, TreeMenu_multimap::iterator> range = treecontrol_multimap().equal_range(group);

    TreeMenu_multimap::iterator begin = range.first;
    TreeMenu_multimap::iterator end = range.second;

    for (; begin != end; ++begin)
    {
        BASE_TreeMenuInfo* info = &(begin->second);

        value_vector.push_back(info);
    }

    return static_cast<int>(value_vector.size());
}
//------------------------------------------------------------------------------ 
void TreeMenuInfoParser::Remove(BASE_TreeMenuInfo::Group_Type group_type)
{
    std::pair<TreeMenu_multimap::iterator, TreeMenu_multimap::iterator> range = treecontrol_multimap().equal_range(group_type);
    if (range.first !=range.second)
    {
        treecontrol_multimap().erase(range.first, range.second);
    }
}
//------------------------------------------------------------------------------ 
bool TreeMenuInfoParser::Insert(BASE_TreeMenuInfo& info)
{
    treecontrol_multimap().insert(std::make_pair(info.tree_group, info));
    return true;
}
//------------------------------------------------------------------------------ 
bool TreeMenuInfoParser::GetFirstTreeMenu(OUT BASE_TreeMenuInfo& info, BASE_TreeMenuInfo::Group_Type group_type)
{
    std::vector<BASE_TreeMenuInfo*> template_info_container;
    if (TreeMenuInfoParser::Instance()->GetTreeMenu(template_info_container, group_type) <= 0)
    {
        return false;
    }

    BASE_TreeMenuInfo* template_info = template_info_container.front();
    if (template_info == NULL)
    {
        return false;
    }

    memcpy(&info, template_info, sizeof(info));

    //info.font_id[BASE_TreeMenuInfo::kFontType_Enable] = StrToWzID("st10");
    //info.font_id[BASE_TreeMenuInfo::kFontType_Disable] = StrToWzID("st10");
    //info.font_id[BASE_TreeMenuInfo::kFontType_Over] = StrToWzID("st10");
    //info.font_id[BASE_TreeMenuInfo::kFontType_Select] = StrToWzID("st10");

    //info.font_color[BASE_TreeMenuInfo::kFontType_Enable] = 0xffffffff;
    //info.font_color[BASE_TreeMenuInfo::kFontType_Disable] = 0xffffffff;
    //info.font_color[BASE_TreeMenuInfo::kFontType_Over] = 0xffffffff;
    //info.font_color[BASE_TreeMenuInfo::kFontType_Select] = 0xffffffff;

    //info.image_code[BASE_TreeMenuInfo::kIconType_Enable] = 0;
    //info.image_code[BASE_TreeMenuInfo::kIconType_Disable] = 10007;
    //info.image_code[BASE_TreeMenuInfo::kIconType_Collpase] = 10002;
    //info.image_code[BASE_TreeMenuInfo::kIconType_Unfold] = 10003;
    //info.image_code[BASE_TreeMenuInfo::kIconType_CheckOn] = 0;
    //info.image_code[BASE_TreeMenuInfo::kIconType_CheckOff] = 0;
    //info.image_code[BASE_TreeMenuInfo::kIconType_Cursor] = 10008;

    return true;
}
