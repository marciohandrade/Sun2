#pragma once

#include <SolarHashTable.h>
#include <Singleton.h>

struct GUIDE_MESSAGE 
{
    DWORD		guide_messge_code;
    TCHAR*		guide_messge_text;
};

class GuideMessageParser    : public Singleton<GuideMessageParser> ,public IParser
{
public:
    GuideMessageParser();
    ~GuideMessageParser();

    VOID Init(DWORD pool_size);
    VOID Init(DWORD pool_size , char* pack_file_name);
    VOID Release();
    virtual BOOL LoadScript(eSCRIPT_CODE script_code, BOOL reload);
    TCHAR* GetGuideMessage(DWORD key);

private:
    VOID Unload();
    util::SolarHashTable<GUIDE_MESSAGE*>* data_table_;
};

