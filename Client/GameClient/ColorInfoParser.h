#pragma once

#include <SolarHashTable.h>
#include <Singleton.h>


struct ColorInfo
{
    int color_index;

    union {
        struct 
        {
            BYTE blue; 
            BYTE green;
            BYTE red;
            BYTE alpha;
        };
        DWORD color;
    };
};



class ColorInfoParser : public Singleton<ColorInfoParser> ,public IParser
{
public:
    ColorInfoParser();
    ~ColorInfoParser();

    VOID            Init(DWORD pool_size);
    VOID            Init(DWORD pool_size , char *pack_file_name);
    VOID            Release();
    virtual	BOOL    LoadScript(eSCRIPT_CODE script_code, BOOL reload);
    DWORD           GetColor(int color_index);

private:
    VOID		    Unload();
    DWORD           GetDefaultColor(int color_index);

    util::SolarHashTable<ColorInfo*>*	data_table_;
    bool    is_loaded_;
    int     max_color_index_;

    
};
