#pragma once

#include "Singleton.h"

struct BASE_TextureListInfo;

//------------------------------------------------------------------------------ 
class TextureListInfoHandleManager : public Singleton<TextureListInfoHandleManager>
{
public:
    //! key DWORD -> BASE_TextureListInfo::texture_index
    typedef std::map< DWORD, std::pair<HANDLE, BASE_TextureListInfo*> > TextureHandleMap;

public:
    TextureListInfoHandleManager();
    ~TextureListInfoHandleManager();

    void RemoveAll();
    void Remove(DWORD texture_list_info_index);
    bool LoadIcon(DWORD texture_list_info_index);

    HANDLE GetTexture(DWORD texture_list_info_index, OUT BASE_TextureListInfo*& info);

    TextureHandleMap& tex_handle_map() { return tex_handle_map_; }

private:
     TextureHandleMap tex_handle_map_;
};
