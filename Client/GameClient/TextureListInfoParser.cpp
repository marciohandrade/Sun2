#include "SunClientPrecompiledHeader.h"
#include "TextureListInfoParser.h"

//------------------------------------------------------------------------------ 
TextureListInfoParser::TextureListInfoParser(void)
{
    texureinfo_map().clear();
}
//------------------------------------------------------------------------------ 
TextureListInfoParser::~TextureListInfoParser(void)
{
}
//------------------------------------------------------------------------------ 
void TextureListInfoParser::Release()
{
    texureinfo_map().clear();
}
//------------------------------------------------------------------------------ 
BOOL TextureListInfoParser::LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload)
{
    if (bReload == TRUE)
    {
        texureinfo_map().clear();
    }

    int nRowSize = GetRowSize();
    for (int nRow = 0; nRow < nRowSize; ++nRow)
    {
        BASE_TextureListInfo info;

        info.texture_index = GetDataDWORD("texture_index", nRow);
        info.file_name = GetDataString("file_name", nRow);
        info.tex_size.left = GetDataInt("x", nRow);
        info.tex_size.top = GetDataInt("y", nRow);
        info.tex_size.right = GetDataInt("width", nRow);
        info.tex_size.bottom = GetDataInt("height", nRow);

        texureinfo_map().insert(std::make_pair(info.texture_index, info));
    }

    return TRUE;
}
//------------------------------------------------------------------------------ 
BASE_TextureListInfo* TextureListInfoParser::GetTextureInfo(DWORD index)
{
    TexureInfo_map::iterator iter = texureinfo_map().find(index);
    if (iter != texureinfo_map().end())
    {
        return &(iter->second);
    }
    return NULL;
}