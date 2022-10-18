#include "SunClientPrecompiledHeader.h"
#include "ColorInfoParser.h"

const DWORD default_color[] = {
                //red   green   blue    alpha   //index
    WzColor_RGBA(255,   255,    255,    255 ),  //0 - 기본값
    WzColor_RGBA(255,   255,    255,    255 ),  //1
    WzColor_RGBA(255,   0,      0,      255 ),  //2
    WzColor_RGBA(255,   0,      0,      255 ),  //3
    WzColor_RGBA(255,   0,      0,      255 ),  //4
    WzColor_RGBA(255,   0,      0,      255 ),  //5
    WzColor_RGBA(255,   0,      0,      255 )   //6
};

//----------------------------------------------------------------------------                   	
/**
*/
ColorInfoParser::ColorInfoParser() : data_table_(NULL), is_loaded_(false)
{
    max_color_index_ = (sizeof(default_color)/sizeof(default_color[0])) - 1;
}

//----------------------------------------------------------------------------                   	
/**
*/
ColorInfoParser::~ColorInfoParser()
{
    Release();
    assert(data_table_ == NULL);
}

//----------------------------------------------------------------------------                   	
/**
*/
VOID ColorInfoParser::Init(DWORD pool_size)
{	
    assert(data_table_ == NULL);
    data_table_ = new util::SolarHashTable<ColorInfo*>;
    data_table_->Initialize(pool_size);
}
//----------------------------------------------------------------------------                   	
/**
*/
VOID ColorInfoParser::Init(DWORD pool_size , char *pack_file_name)
{
    assert(data_table_ == NULL);
    data_table_ = new util::SolarHashTable<ColorInfo*>;
    data_table_->Initialize(pool_size);
    SetPackFileName(pack_file_name);
}

//----------------------------------------------------------------------------
/**
*/
VOID ColorInfoParser::Release()
{
    Unload();
    delete data_table_;
    data_table_ = NULL;
}

//----------------------------------------------------------------------------                   	
/**
*/
VOID ColorInfoParser::Unload()
{
    ColorInfo* color_Info = NULL;
    if (data_table_ == NULL)
    {
        return;
    }
    data_table_->SetFirst();
    while (color_Info = data_table_->GetNext())
    {
        delete color_Info;
    }
    data_table_->RemoveAll();
}

//----------------------------------------------------------------------------
/**
*/
BOOL ColorInfoParser::LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload)
{
    assert(ScriptCode == SCRIPT_COLOR_INFO);

    int rowSize = GetRowSize();

    for (int row = 0; row < rowSize; ++row)
    {
        ColorInfo* info = new ColorInfo;
        assert(info);

        info->color_index   = GetDataInt("index", row);
        info->red           = GetDataBYTE("Red", row);
        info->green         = GetDataBYTE("Green", row);
        info->blue          = GetDataBYTE("Blue", row);
        info->alpha         = GetDataBYTE("Alpha", row);

        data_table_->Add(info, info->color_index);
    }

    is_loaded_ = true;

    return TRUE;
}


//----------------------------------------------------------------------------                   	
/**
*/
DWORD ColorInfoParser::GetColor(int color_index)
{
    if (is_loaded_ == true)
    {
        ColorInfo* color_info = data_table_->GetData(color_index);
        if (color_info != NULL)
        {
            return color_info->color;
        }
    }
    else
    {        
        return GetDefaultColor(color_index); //스트립트가 없을 때를 위해 하드코딩된 초기 설정 값
    }

    return default_color[0];
}

//----------------------------------------------------------------------------                   	
/**
*/
DWORD ColorInfoParser::GetDefaultColor(int color_index)
{
    if (color_index > 0 && color_index <= max_color_index_)
    {
        return default_color[color_index];
    }

    return default_color[0];
}
