#ifndef _TextureListInfoParser_h_
#define _TextureListInfoParser_h_

#pragma once

//------------------------------------------------------------------------------ 
struct BASE_TextureListInfo
{
    DWORD       texture_index;
    std::string file_name;
    RECT        tex_size;
};

//------------------------------------------------------------------------------ 
class TextureListInfoParser	: public Singleton<TextureListInfoParser>, public IParser
{
    typedef std::map<DWORD, BASE_TextureListInfo> TexureInfo_map;

public:
    TextureListInfoParser(void);
    ~TextureListInfoParser(void);

    virtual void Release();
    virtual	BOOL LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);

    BASE_TextureListInfo* GetTextureInfo(DWORD index);

    TexureInfo_map& texureinfo_map() { return texureinfo_map_; }

private:
    TexureInfo_map texureinfo_map_;
};



#endif // _TextureListInfoParser_h_
