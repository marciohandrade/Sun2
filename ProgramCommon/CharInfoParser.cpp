#include "stdafx.h"
#include "./CharInfoParser.h"

#include <Struct.h>

//==================================================================================================

CharInfoParser* CharInfoParser::instance_static_ = NULL;

//==================================================================================================

CharInfoParser* CharInfoParser::CreateInstance()
{
    if (instance_static_ != NULL) {
        return instance_static_;
    }
    static struct AutoClosure {
        AutoClosure(CharInfoParser** static_table) : static_table_(static_table) {
        }
        ~AutoClosure() {
            SAFE_DELETE(*static_table_);
        }
        CharInfoParser** static_table_;
    } auto_closure(&instance_static_);
    //
    CharInfoParser* provider = new CharInfoParser;
    // initialization...
    instance_static_ = provider;
    //
    return instance_static_;
}

//==================================================================================================

CharInfoParser::CharInfoParser()
{
    ZeroMemory(char_info_, sizeof(char_info_));
    file_path_for_debug_[0] = '\0';
}

CharInfoParser::~CharInfoParser()
{
}

void CharInfoParser::Init(DWORD unused)
{
    __UNUSED(unused);
    ZeroMemory(char_info_, sizeof(char_info_));
}

void CharInfoParser::Init(DWORD unused, char* pack_file_name)
{
    Init(unused);
    SetPackFileName(pack_file_name);
}

void CharInfoParser::Release()
{
    Init(0);
}

BOOL CharInfoParser::LoadScript(eSCRIPT_CODE script_code, BOOL reloaded)
{
    switch (script_code)
    {
    case SCRIPT_INITCHARINFO:
    case SCRIPT_INITCHARINFOLIST:
        return _Load(reloaded);
    }

    return false;
}

//==================================================================================================

bool CharInfoParser::_Load(BOOL reloaded)
{
    __UNUSED(reloaded);
    const int max_rows = GetRowSize();
    ZeroMemory(char_info_buffer_, sizeof(char_info_buffer_));
    //
    int processed = 0;
    for (int row = 0; row < max_rows; ++row)
    {
        eCHAR_TYPE class_code = static_cast<eCHAR_TYPE>(GetDataBYTE("ClassCode", row));
        const bool is_valid_class = (class_code == eCHAR_BERSERKER ||
            class_code == eCHAR_DRAGON ||
            class_code == eCHAR_SHADOW ||
            class_code == eCHAR_VALKYRIE ||
            class_code == eCHAR_ELEMENTALIST || 
            class_code == eCHAR_MYSTIC 
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
            || class_code == eCHAR_HELLROID
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
            || class_code == eCHAR_WITCHBLADE
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
            );

        if (is_valid_class == false)
        {
            continue;
        }
        CharInfoNode* const char_info_node = &char_info_[class_code];
        if (char_info_node->info != NULL) {
            continue;
        };

        INIT_CHARINFO_BASE* const char_data = &char_info_buffer_[class_code - 1];
        char_data->m_ClassCode = static_cast<uint8_t>(class_code);

        strncpy(char_data->m_pszClassName, GetDataString("ClassName", row),
                _countof(char_data->m_pszClassName));
        char_data->m_pszClassName[_countof(char_data->m_pszClassName) - 1] = '\0';
        //
        char_data->m_LV = GetDataWORD("Level", row);
        char_data->m_dwMaxHP = GetDataDWORD("MaxHP", row);
        char_data->m_dwMaxMP = GetDataDWORD("MaxMP", row);
        char_data->m_i64Exp  = GetDataInt64("Exp", row);
        char_data->m_dwRemainStat = GetDataWORD("RemainStat", row);
        char_data->m_dwRemainSkill = GetDataWORD("RemainSkill", row);
        //
        char_data->m_dwRegion = GetDataWORD("Region", row);
        char_data->m_sLocationX = static_cast<short>(GetDataInt("LocationX", row));
        char_data->m_sLocationY = static_cast<short>(GetDataInt("LocationY", row));
        char_data->m_sLocationZ = static_cast<short>(GetDataInt("LocationZ", row));
        //
        char_data->m_sStrength    = static_cast<short>(GetDataInt("Strength", row));
        char_data->m_sDexterity   = static_cast<short>(GetDataInt("Dexterity", row));
        char_data->m_sVitality    = static_cast<short>(GetDataInt("Vitality", row));
        char_data->m_sInteligence = static_cast<short>(GetDataInt("Intelligence", row));
        char_data->m_sSpirit      = static_cast<short>(GetDataInt("Spirit", row));
        char_data->m_sSkillStat1  = static_cast<short>(GetDataInt("SkillStat1", row));
        char_data->m_sSkillStat2  = static_cast<short>(GetDataInt("SkillStat2", row));
        //
        char_data->m_ClassNameCode = GetDataDWORD("ClassNameCode", row);
        //
        char wz_id_strbuf[4];
        strncpy(wz_id_strbuf, GetDataString("AnimationCode", row), _countof(wz_id_strbuf));
        char_data->m_RiderSummonCode = StrToWzID(wz_id_strbuf);

    #ifdef _NA_20100307_BETAKEY_SYSTEM
        char_data->m_MenBeytaKey    = GetDataInt("Man_betaKey", row);
        char_data->m_WomanBeytaKey  = GetDataInt("Woman_betaKey", row);
    #endif
        //
        char_info_node->info = char_data;
        //
        ++processed;
    }; //end 'foreach record in records'
    //
    //if (FlowControl::FCAssert(max_rows == kMaxNumberOfCharacterClasses) == false) {
    //    return false;
    //};

    return true;
}

//==================================================================================================
