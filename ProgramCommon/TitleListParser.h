#pragma once

#ifndef PROGRAMCOMMON_HONORSYSTEM_TITLELISTPARSER_H_
#define PROGRAMCOMMON_HONORSYSTEM_TITLELISTPARSER_H_

#ifdef _NA_003027_20111013_HONOR_SYSTEM

#include <Singleton.h>
#include <SolarHashTable.h>
#include <SolarFileReader.h>
#include <ScriptCode.IParser.h>

class TitleListParser : public util::Singleton<TitleListParser>, public IParser
{
public:
    enum DisplayType
    {
        kNone = 0,
        kPrefixTitle = 1,
        kPostfixTitle = 2,
    };

    struct TitleInfo
    {
        WORD title_code; // Īȣ�ڵ�
#ifndef _SERVER //if'N'def
        DWORD title_name_code; // Īȣ ����
        BYTE title_display_type; // Īȣ ǥ�� Ÿ��
#endif //_SERVER
    };

    typedef util::SolarHashTable<TitleInfo*> TitleInfoTable;

public:
    TitleListParser(void);
    virtual	~TitleListParser(void);

    void Init();
    void Init(char* pack_filename);
    void Release();

    virtual	BOOL LoadScript(eSCRIPT_CODE script_code, BOOL is_reload);

    TitleInfo* GetTitleInfo(WORD title_code);

    TitleInfoTable& GetTitleInfoTable() { return title_info_table_; }

private:
    BOOL _Load(BOOL is_reload);

    TitleInfoTable title_info_table_;
};

#endif //_NA_003027_20111013_HONOR_SYSTEM

#endif //PROGRAMCOMMON_HONORSYSTEM_TITLELISTPARSER_H_