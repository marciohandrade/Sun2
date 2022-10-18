#include "SunClientPrecompiledHeader.h"

#include "TextureListInfoHandleManager.h"
#include "TextureListInfoParser.h"

//------------------------------------------------------------------------------ 
TextureListInfoHandleManager::TextureListInfoHandleManager()
{
    tex_handle_map().clear();
}
//------------------------------------------------------------------------------ 
TextureListInfoHandleManager::~TextureListInfoHandleManager()
{
    RemoveAll();
}
//------------------------------------------------------------------------------ 
void TextureListInfoHandleManager::RemoveAll()
{
    if (g_pSunRenderer != NULL)
    {
        TextureHandleMap::iterator begin = tex_handle_map().begin();
        TextureHandleMap::iterator end = tex_handle_map().end();
        for (; begin != end; ++begin)
        {
            if (begin->second.first != INVALID_HANDLE_VALUE)
            {
                g_pSunRenderer->x_pManagerTexture->DestroyTexture(begin->second.first);
            }
        }
    }
    tex_handle_map().clear();
}
//------------------------------------------------------------------------------ 
void TextureListInfoHandleManager::Remove(DWORD texture_list_info_index)
{
    if (texture_list_info_index <= 0)
    {
        return;
    }
    TextureHandleMap::iterator it = tex_handle_map().find(texture_list_info_index);
    if (it != tex_handle_map().end())
    {
        g_pSunRenderer->x_pManagerTexture->DestroyTexture(it->second.first);

        tex_handle_map().erase(it);
    }
}
//------------------------------------------------------------------------------ 
bool TextureListInfoHandleManager::LoadIcon(DWORD texture_list_info_index)
{
    if (texture_list_info_index <= 0)
    {
        return false;
    }
    TextureHandleMap::iterator it = tex_handle_map().find(texture_list_info_index);
    if (it != tex_handle_map().end())
    {
        return true;
    }

    BASE_TextureListInfo* tex_info = TextureListInfoParser::Instance()->GetTextureInfo(texture_list_info_index);
    if (tex_info != NULL)
    {
        HANDLE tex_handle = g_pSunRenderer->x_pManagerTexture->LoadTexture(tex_info->file_name.c_str(), 
                                     TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);

        if (tex_handle != INVALID_HANDLE_VALUE)
        {
            tex_handle_map().insert(std::make_pair(texture_list_info_index, std::make_pair(tex_handle, tex_info)));
        }
    }

    return true;
}
//------------------------------------------------------------------------------ 
HANDLE TextureListInfoHandleManager::GetTexture(DWORD texture_list_info_index, OUT BASE_TextureListInfo*& info)
{
    if (texture_list_info_index > 0)
    {
        TextureHandleMap::iterator it = tex_handle_map().find(texture_list_info_index);
        if (it != tex_handle_map().end())
        {
            info = it->second.second;
            return it->second.first;
        }
    }
    return INVALID_HANDLE_VALUE;
}