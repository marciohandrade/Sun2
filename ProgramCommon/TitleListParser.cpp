#include "StdAfx.h"

#ifdef _NA_003027_20111013_HONOR_SYSTEM

#include ".\TitleListParser.h"

TitleListParser::TitleListParser(void)
{
}

TitleListParser::~TitleListParser(void)
{
}

VOID TitleListParser::Init()
{
    title_info_table_.Initialize(50);
}

VOID TitleListParser::Init(char* pack_filename)
{
    SetPackFileName(pack_filename);
    Init();
}

VOID TitleListParser::Release()
{
    title_info_table_.SetFirst();
    TitleInfo* title_info = title_info_table_.GetNext();
    while (title_info != NULL)
    {
        delete title_info;
        title_info = title_info_table_.GetNext();
    }
    title_info_table_.clear();
}

BOOL TitleListParser::_Load(BOOL is_reload)
{
    __UNUSED(is_reload);//

    int row_size = GetRowSize();

    for (int row_index = 0; row_index< row_size; ++row_index)
    {
        // 명성코드
        WORD title_code = GetDataWORD("TitleCode", row_index);

        TitleInfo* title_info = title_info_table_.GetData(title_code);
        if (title_info != NULL)
        {
            // 중복 코드 체크
            SUNLOG(eCRITICAL_LOG, "Duplicate TitleList TitleCode(%d)", title_code);
            continue;
        }

        // 
        title_info = new TitleInfo;
        title_info_table_.Add(title_info, title_code);

        // 칭호코드
        title_info->title_code = title_code; 

#ifndef _SERVER //if'N'def
        // 칭호 제목
        title_info->title_name_code = GetDataDWORD("TitleNameCode", row_index);
        // 칭호 표시 타입
        title_info->title_display_type = GetDataBYTE("TitlePos", row_index);
#endif //_SERVER
    }

    return TRUE;
}

BOOL TitleListParser::LoadScript(eSCRIPT_CODE script_code, BOOL is_reload)
{
    switch (script_code)
    {
    case SCRIPT_TITLE_LIST:
        return _Load(is_reload);
    }

    return FALSE;	
}

TitleListParser::TitleInfo* TitleListParser::GetTitleInfo(WORD title_code)
{
    TitleInfo* title_info = title_info_table_.GetData(title_code);
    return title_info;
}

#endif //_NA_003027_20111013_HONOR_SYSTEM