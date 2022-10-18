#include "StdAfx.h"

#ifdef _NA_003027_20111013_HONOR_SYSTEM

#include ".\FameListParser.h"

FameListParser::FameListParser(void)
{
}

FameListParser::~FameListParser(void)
{
}

VOID FameListParser::Init()
{
}

VOID FameListParser::Init(char* pack_filename)
{
	SetPackFileName(pack_filename);
}

VOID FameListParser::Release()
{
    FameInfoMap::iterator fame_info_itr = fame_info_container_.begin();
    FameInfoMap::iterator fame_info_end_itr = fame_info_container_.end();
    while (fame_info_itr != fame_info_end_itr)
    {
        FameInfo* fame_info = (fame_info_itr->second);
        if (fame_info != NULL)
        {
            delete fame_info;
        }
        ++fame_info_itr;
    }
    fame_info_container_.clear();
}

BOOL FameListParser::_Load(BOOL is_reload)
{
    __UNUSED(is_reload);//

    char temp_field_name[MAX_FIELD_NAME_SIZE];

    FameInfoMap::iterator fame_info_itr;
    FameInfoMap::iterator fame_info_end_itr = fame_info_container_.end();

    int row_size = GetRowSize();

    for (int row_index = 0; row_index< row_size; ++row_index)
    {
        // 명성코드
        WORD fame_code = GetDataWORD("FameCode", row_index);
        fame_info_itr = fame_info_container_.find(fame_code);
        if (fame_info_itr != fame_info_end_itr)
        {
            // 중복 코드 체크
            SUNLOG(eCRITICAL_LOG, "Duplicate FameList FameCode(%d)", fame_code);
            continue;
        }
        
        //
        FameInfo* new_fame_info = new FameInfo;
        fame_info_container_.insert(FameInfoMap::value_type(fame_code, new_fame_info));

        // 명성코드
        new_fame_info->fame_code = fame_code; 
        
        // 명성 단계별 필요점수 및 누적점수
        DWORD accumulate_point = 0;
        for (int rating_index = 0; rating_index < kMaxFameRating; ++rating_index)
        {
            // 단계별 필요점수
            _snprintf(temp_field_name, _countof(temp_field_name), "FameRating%d", rating_index + 1);
            new_fame_info->fame_rating_point[rating_index] = GetDataWORD(temp_field_name, row_index);
            
            // 단계별 누적점수
            accumulate_point += new_fame_info->fame_rating_point[rating_index];
            new_fame_info->fame_rating_accumulate_point[rating_index] = accumulate_point;

#ifndef _SERVER //if'N'def
            // 단계별 달성 이펙트
            _snprintf(temp_field_name, _countof(temp_field_name), "effectCode%d", rating_index + 1);
            new_fame_info->fame_effectcode[rating_index] = StrToWzID(GetDataString(temp_field_name, row_index));
#endif //_SERVER
        }

#ifndef _SERVER //if'N'def
        // 명성 제목
        new_fame_info->fame_name_code = GetDataDWORD("StringCode1", row_index);
        // 명성 설명
        new_fame_info->fame_description_code = GetDataDWORD("StringCode2", row_index);
#endif //_SERVER
    }

    return TRUE;
}

BOOL FameListParser::LoadScript(eSCRIPT_CODE script_code, BOOL is_reload)
{
	switch (script_code)
	{
	case SCRIPT_FAME_LIST:
		return _Load(is_reload);
	}

    return FALSE;	
}

FameListParser::FameInfo* FameListParser::GetFameInfo(WORD fame_code)
{
    FameInfo* found_fame_info = NULL;
    FameInfoMap::iterator fame_info_itr = fame_info_container_.find(fame_code);
    FameInfoMap::iterator fame_info_end_itr = fame_info_container_.end();
    if (fame_info_itr != fame_info_end_itr)
    {
        found_fame_info = (fame_info_itr->second);
    }
    
    return found_fame_info;
}

int FameListParser::GetFameRating(FameInfo* fame_info, DWORD current_fame_point)
{
    if (fame_info == NULL)
    {
        return 0;
    }

    int rating_index = 0;
    for ( ; rating_index < kMaxFameRating; ++rating_index)
    {
        if (current_fame_point < fame_info->fame_rating_accumulate_point[rating_index])
        {
            break;
        }
    }

    return rating_index;
}
#endif //_NA_003027_20111013_HONOR_SYSTEM