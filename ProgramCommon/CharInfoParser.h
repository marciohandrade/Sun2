#pragma once
#ifndef PROGRAMCOMMON_CHARINFOPARSER_H
#define PROGRAMCOMMON_CHARINFOPARSER_H

//==================================================================================================

typedef const INIT_CHARINFO_BASE INIT_CHARINFO;

//==================================================================================================
// @history
//  ~11.04.25,
//  11.04.25, waverix, CHANGES: f110425.2L, logic improvements

class CharInfoParser : public IParser
{
public:
    //static const size_t kMaxNumberOfCharacterClasses = 5;
    //
    static CharInfoParser* Instance();

    // IParser interfaces
    virtual BOOL LoadScript(eSCRIPT_CODE script_code, BOOL reloaded);
    void Release();

    void Init(DWORD unused);
    void Init(DWORD unused, char* pack_file_name);

    INIT_CHARINFO* GetInitCharInfo(CODETYPE class_code) const;

private:
    struct CharInfoNode {
        const INIT_CHARINFO_BASE* info;
    };
    CharInfoParser();
    virtual ~CharInfoParser();
    static CharInfoParser* CreateInstance();

    //void Reload(); // unused
    //void Unload();
    bool _Load(BOOL bReload);
    // fields...
    CharInfoNode char_info_[eCHAR_TYPE_MAX];
    INIT_CHARINFO_BASE char_info_buffer_[eCHAR_TYPE_MAX-1];
    char file_path_for_debug_[MAX_PATH];
    static CharInfoParser* instance_static_;
};

//==================================================================================================

inline CharInfoParser* CharInfoParser::Instance()
{
    if (instance_static_ != NULL) {
        return instance_static_;
    }
    return CreateInstance();
}

inline INIT_CHARINFO* CharInfoParser::GetInitCharInfo(CODETYPE class_code) const
{
    INIT_CHARINFO* init_char_info = (class_code < eCHAR_TYPE_MAX)
        ?   char_info_[class_code].info
        :   NULL;
    if (init_char_info) {
        return init_char_info;
    };
    assert(!"index out of ranges");
    return NULL;
}

//==================================================================================================

#endif //PROGRAMCOMMON_CHARINFOPARSER_H