#include "SunClientPrecompiledHeader.h"
#include "CharacterCreationInfoParser.h"
#include "AppearanceResourceCode.h"

CharacterCreationInfoParser::CharacterCreationInfoParser()
{
}

CharacterCreationInfoParser::~CharacterCreationInfoParser()
{
}

void CharacterCreationInfoParser::Release()
{
    Unload();
}

VOID CharacterCreationInfoParser::Init(DWORD info_pool_size)
{
    creation_info_list_.clear();
}


VOID CharacterCreationInfoParser::Init(DWORD info_pool_size, char* pack_filename)
{
    Init(info_pool_size);
    SetPackFileName(pack_filename);
}

VOID CharacterCreationInfoParser::Unload()
{
    CREATION_INFO_ITR found_itr = creation_info_list_.begin();
    CREATION_INFO_ITR list_end_itr = creation_info_list_.end();
    for ( ; found_itr != list_end_itr; ++found_itr)
    {
        CREATION_INFO* creation_info = (*found_itr);
        delete creation_info;
    }
    creation_info_list_.clear();
}

BOOL CharacterCreationInfoParser::LoadScript(eSCRIPT_CODE script_code, BOOL is_reload)
{
    if (script_code != SCRIPT_CHARACTER_CREATION_INFO)
    {
        return FALSE;
    }

    LoadScript();

    return TRUE;
}

#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
void CharacterCreationInfoParser::LoadScript()
{
    int row_size = GetRowSize();

    for (int row = 0; row < row_size; ++row)
    {
        CREATION_INFO* creation_info = new CREATION_INFO;

        // 기본 클래스 타입
        creation_info->class_type =	GetDataBYTE("Index", row);
        // 기본 클래스 이름
        strncpy(&(creation_info->class_name[0]), GetDataString("Class", row), MAX_CHARNAME_LENGTH);
        creation_info->class_name[MAX_CHARNAME_LENGTH] = '\0';
        // 기본 클래스 설명 스트링코드
        creation_info->class_description_code = GetDataDWORD("ClassStrCode", row);
        // 종족 이름
        strncpy(&(creation_info->class_name[0]), GetDataString("Race", row), MAX_CHARNAME_LENGTH);
        creation_info->race_name[MAX_CHARNAME_LENGTH] = '\0';
        // 종족 설명 스트링코드
        creation_info->race_description_code = GetDataDWORD("RaceStrCode", row);
        // 클래스의 기본 성별
        GenderType::Value gender_type = 
            ResourceCode::GetBaseGenderOfClass(static_cast<eCHAR_TYPE>(creation_info->class_type));
        creation_info->base_gender_type = static_cast<BYTE>(gender_type);
            

        // 선택가능한 클래스여부
        // 버서커
        creation_info->enable_select_class[eCHAR_BERSERKER - 1] = GetDataDWORD("Berserker", row);
        // 드래곤나이트
        creation_info->enable_select_class[eCHAR_DRAGON - 1] = GetDataDWORD("Dragonknight", row);
        // 섀도우
        creation_info->enable_select_class[eCHAR_SHADOW - 1] = GetDataDWORD("Shadow", row);
        // 발키리
        creation_info->enable_select_class[eCHAR_VALKYRIE - 1] = GetDataDWORD("Valkyrie", row);
        // 엘리멘탈리스트
        creation_info->enable_select_class[eCHAR_ELEMENTALIST - 1] = GetDataDWORD("Elementalist", row);
        // 미스틱
        creation_info->enable_select_class[eCHAR_MYSTIC - 1] = GetDataDWORD("Mystic", row);

        creation_info_list_.push_back(creation_info);
    }
}
#else
void CharacterCreationInfoParser::LoadScript()
{
    int row_size = GetRowSize();

    for (int row = 0; row < row_size; ++row)
    {
        CREATION_INFO* creation_info = new CREATION_INFO;

        // 기본 클래스 타입
        creation_info->class_type =	GetDataBYTE("ChaIndex", row);
        // 기본 클래스 이름
        strncpy(&(creation_info->class_name[0]), GetDataString("ChaName", row), MAX_CHARNAME_LENGTH);
        creation_info->class_name[MAX_CHARNAME_LENGTH] = '\0';
        // 기본 클래스 설명 스트링코드
        creation_info->class_description_code = GetDataDWORD("ChaStrCode", row);
        // 클래스의 기본 성별
        creation_info->base_gender_type =	GetDataBYTE("GenderType", row);

        // 선택가능한 클래스여부
        // 버서커
        creation_info->enable_select_class[eCHAR_BERSERKER - 1] = GetDataDWORD("Berserker", row);
        // 드래곤나이트
        creation_info->enable_select_class[eCHAR_DRAGON - 1] = GetDataDWORD("Dragonknight", row);
        // 섀도우
        creation_info->enable_select_class[eCHAR_SHADOW - 1] = GetDataDWORD("Shadow", row);
        // 발키리
        creation_info->enable_select_class[eCHAR_VALKYRIE - 1] = GetDataDWORD("Valkyrie", row);
        // 엘리멘탈리스트
        creation_info->enable_select_class[eCHAR_ELEMENTALIST - 1] = GetDataDWORD("Elementalist", row);

        creation_info_list_.push_back(creation_info);
    }
}
#endif //_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

CREATION_INFO* CharacterCreationInfoParser::GetCreationInfo(BYTE class_type)
{
    if ((class_type == eCHAR_NONE) || 
        (class_type >= eCHAR_TYPE_MAX) || 
        ((class_type - 1) >= static_cast<BYTE>(creation_info_list_.size())))
    {
        return NULL;
    }

    return creation_info_list_[class_type - 1];
}
//------------------------------------------------------------------------------ 
int CharacterCreationInfoParser::GetSkinClass(eCHAR_TYPE char_class, OUT bool skin_class[/*eCHAR_TYPE_MAX*/])
{
    if (char_class < eCHAR_BERSERKER || char_class >= eCHAR_TYPE_MAX)
    {
        return 0;
    }

#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    const DWORD kInvalidParamater = 0;
#else
    const DWORD kInvalidParamater = 999999;
#endif //_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT    

    int class_count = 0;
    CREATION_INFO_LIST::iterator begin = creation_info_list_.begin();
    CREATION_INFO_LIST::iterator end = creation_info_list_.end();
    for (; begin != end; ++begin)
    {
        skin_class[(*begin)->class_type] = false;

        DWORD class_parameter = (*begin)->enable_select_class[char_class - 1];
        if (class_parameter != kInvalidParamater)
        {
            skin_class[(*begin)->class_type] = true;
            ++class_count;
        }
    }

    return class_count;
}
