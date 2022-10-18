#include "StdAfx.h"

#ifdef _NA_003027_20111013_HONOR_SYSTEM

#include ".\ReputeListParser.h"

ReputeListParser::ReputeListParser(void)
{
}

ReputeListParser::~ReputeListParser(void)
{
}

VOID ReputeListParser::Init()
{
    repute_info_table_.Initialize(50);
}

VOID ReputeListParser::Init(char* pack_filename)
{
    SetPackFileName(pack_filename);
    Init();
}

VOID ReputeListParser::Release()
{
    repute_info_table_.SetFirst();
    ReputeInfo* repute_info = repute_info_table_.GetNext();
    while (repute_info != NULL)
    {
        delete repute_info;
        repute_info = repute_info_table_.GetNext();
    }
    repute_info_table_.clear();

    ReputeGroupInfoMap::iterator repute_group_info_itr = GetReputeGroupInfoMap().begin();
    ReputeGroupInfoMap::iterator repute_group_info_end_itr = GetReputeGroupInfoMap().end();
    for ( ; repute_group_info_itr != repute_group_info_end_itr; ++repute_group_info_itr)
    {
        ReputeGroupInfo* repute_group_info = repute_group_info_itr->second;
        if (repute_group_info != NULL)
        {
            repute_group_info->repute_code_set.clear();
            delete repute_group_info;
        }
    }
    GetReputeGroupInfoMap().clear();    
}

BOOL ReputeListParser::_Load(BOOL is_reload)
{
    __UNUSED(is_reload);//

    char temp_field_name[MAX_FIELD_NAME_SIZE];

    int row_size = GetRowSize();

    for (int row_index = 0; row_index< row_size; ++row_index)
    {
        // 명성코드
        WORD repute_code = GetDataWORD("ReputeCode", row_index);

        ReputeInfo* repute_info = repute_info_table_.GetData(repute_code);
        if (repute_info != NULL)
        {
            // 중복 코드 체크
            SUNLOG(eCRITICAL_LOG, "Duplicate ReputeList ReputeCode(%d)", repute_code);
            continue;
        }

        // 
        repute_info = new ReputeInfo;
        repute_info_table_.Add(repute_info, repute_code);

        // 평판코드
        repute_info->repute_code = repute_code; 

        // 평판그룹코드
        repute_info->repute_group_code = GetDataWORD("ReputeGroup", row_index);

        // 연관 명성 코드
        repute_info->fame_area_code = GetDataWORD("FameArea", row_index);

        DWORD accumulate_point = 0;
        for (int rating_index = 0; rating_index < kMaxReputeRating; ++rating_index)
        {
            // 단계별 필요점수
            _snprintf(temp_field_name, _countof(temp_field_name), "ReputeRating%d", rating_index + 1);
            repute_info->repute_rating_point[rating_index] = GetDataWORD(temp_field_name, row_index);

            // 단계별 명성 보상 점수
            _snprintf(temp_field_name, _countof(temp_field_name), "FamePoint%d", rating_index + 1);
            repute_info->reward_fame_point[rating_index] = GetDataWORD(temp_field_name, row_index);

            // 단계별 누적점수
            accumulate_point += repute_info->repute_rating_point[rating_index];
            repute_info->repute_rating_accumulate_point[rating_index] = accumulate_point;

#ifndef _SERVER //if'N'def
            // 단계별 달성 이펙트
            _snprintf(temp_field_name, _countof(temp_field_name), "effectCode%d", rating_index + 1);
            repute_info->repute_effectcode[rating_index] = StrToWzID(GetDataString(temp_field_name, row_index));
#endif //_SERVER
        }

#ifndef _SERVER //if'N'def
        // 평판 제목
        repute_info->repute_name_code = GetDataDWORD("StringCode1", row_index);
        // 평판 그룹 제목
        repute_info->repute_group_name_code = GetDataDWORD("GroupString", row_index);
        // 평판 설명
        repute_info->repute_description_code = GetDataDWORD("StringCode2", row_index);
#endif //_SERVER

        ReputeGroupInfoMap::iterator found_itr = GetReputeGroupInfoMap().find(repute_info->repute_group_code);
        if (found_itr == GetReputeGroupInfoMap().end())
        {
            // ReputeGroupInfo 추가
            ReputeGroupInfo* repute_group_info = new ReputeGroupInfo;
            repute_group_info->repute_group_code = repute_info->repute_group_code;
#ifndef _SERVER
            repute_group_info->repute_group_name_code = repute_info->repute_group_name_code;
#endif
            repute_group_info->repute_code_set.insert(repute_info->repute_code);

            GetReputeGroupInfoMap().insert(ReputeGroupInfoMap::value_type(repute_info->repute_group_code, repute_group_info));
        }
        else
        {
            // ReputeGroupInfo 에 repute_code 추가
            ReputeGroupInfo* repute_group_info = found_itr->second;
            repute_group_info->repute_code_set.insert(repute_info->repute_code);
        }
    }

    return TRUE;
}

BOOL ReputeListParser::LoadScript(eSCRIPT_CODE script_code, BOOL is_reload)
{
    switch (script_code)
    {
    case SCRIPT_REPUTE_LIST:
        return _Load(is_reload);
    }

    return FALSE;	
}

ReputeListParser::ReputeInfo* ReputeListParser::GetReputeInfo(WORD repute_code)
{
    ReputeInfo* repute_info = repute_info_table_.GetData(repute_code);
    return repute_info;
}

int ReputeListParser::GetReputeRating(ReputeInfo* repute_info, DWORD current_repute_point)
{
    if (repute_info == NULL)
    {
        return 0;
    }

    int rating_index = 0;
    for ( ; rating_index < kMaxReputeRating; ++rating_index)
    {
        if (current_repute_point < repute_info->repute_rating_accumulate_point[rating_index])
        {
            break;
        }
    }

    return rating_index;
}

#endif //_NA_003027_20111013_HONOR_SYSTEM