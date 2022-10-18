#include "StdAfx.h"
#include ".\AchievementSearch.h"

AchievementParser::AchievementParser(void) : script_data_table_(NULL)
{
}

AchievementParser::~AchievementParser(void)
{
}

VOID AchievementParser::Release()
{
    if (script_data_table_ == NULL) {
        return;
    };
    script_data_table_->SetFirst();
    while (Achievement_ParserInfo * row_info = script_data_table_->GetNext())
    {
        delete row_info;
    }
    script_data_table_->RemoveAll();

    delete script_data_table_;
    script_data_table_ = NULL;

    AchievementSearch::Instance()->DestroyInstance();
}

VOID AchievementParser::Init(DWORD dwPoolSize)
{ 
    ASSERT( script_data_table_ == NULL );
    script_data_table_ = new util::SolarHashTable<Achievement_ParserInfo*>;
    script_data_table_->Initialize( dwPoolSize );
}

VOID AchievementParser::Init(DWORD dwPoolSize , char *pszPackFileName)
{
    Init(dwPoolSize);
    SetPackFileName( pszPackFileName );
}

BOOL AchievementParser::LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload )
{
#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
    if( ScriptCode == SCRIPT_ACHIEVEMENT_INFO)
        return _Load( bReload );
#endif
    
    return FALSE;
}

Achievement_ParserInfo * AchievementParser::GetRowData(WORD index, BOOL reload)
{
    __UNUSED(reload);
    Achievement_ParserInfo* row_info = script_data_table_->GetData(index);

    if (row_info == NULL)
    {            
        row_info = new Achievement_ParserInfo;
        // NOTE: f110520.3L, prevent garbage data in a storage
        ZeroMemory(row_info, sizeof(*row_info));
        row_info->index_ = index;
        script_data_table_->Add(row_info, row_info->index_);
    }
#if !defined(_SERVER)
    else 
    {
        TCHAR string[255] = {0,};
        sprintf(string, "AcScript.txt index : %d 중복!! 확인바랍니다.", index);
        ::MessageBox(NULL, string, "error", MB_OK);
    }
#endif //_SERVER

    return row_info;
}

BOOL AchievementParser::_Load(BOOL reload)
{
    __UNUSED(reload);
    const int max_rows = GetRowSize();

#if !defined(_NA_001990_ACSYSTEM_ADD_INCREASE_INDEX)
    if (max_rows > MAX_ACHIEVEMENT_COUNT)
    {
        ASSERT("업적 최대 제한 개수를 초과하였습니다.");
        __asm { int 3 };
    };
#endif //_NA_001990_ACSYSTEM_ADD_INCREASE_INDEX
    if (max_rows > MAX_ACHIEVEMENT_COUNT)
    {
        ASSERT("업적 최대 제한 개수를 초과하였습니다.");
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] AC MaxCount Error!/ 캐릭터의 수행가능 최대업적량을 초과했습니다.");
    };

    for (int row = 0; row < max_rows; ++row)
    {
        WORD index = GetDataWORD( "Index", row );

#if !defined(_NA_001990_ACSYSTEM_ADD_INCREASE_INDEX)
        if (index > MAX_ACHIEVEMENT_COUNT)
        {
            ASSERT( "업적 최대 제한 index 를 초과하였습니다.");
            __asm { int 3 };
        };
#endif //_NA_001990_ACSYSTEM_ADD_INCREASE_INDEX
        
        Achievement_ParserInfo* row_info = GetRowData(index, reload);
        // side-effects of calling GetRowData are to make cleared data and to align the index.
        //row_info->index_ = index;
        row_info->location_code_ = GetDataWORD("LocationString", row);
        row_info->object_string_code_  = GetDataDWORD("ObejctString", row);
        row_info->tooltip_string_code_ = GetDataDWORD("ToolTipString", row);
        row_info->icon_code_   = GetDataDWORD("Icon", row);
        row_info->object_type_ = GetDataBYTE("ObjectType", row);

#ifdef _NA_006541_RENEWER_ACDIALOG
        row_info->lower_group_index_ = GetDataInt("LocationOrder",row);
        row_info->lower_group_string_= row_info->location_code_;

        row_info->upper_group_index_ = GetDataInt("CroupOrder",row);
        row_info->upper_group_string_= GetDataDWORD("CroupString",row);
#endif

#ifdef _NA_001990_ACSYSTEM_ADD
        row_info->class_type_  = GetDataDWORD("Class", row);
        if (row_info->class_type_ >= eCHAR_TYPE_MAX)
        {
            ASSERT( "ACScript.txt의 Class가 최대 종류를 초과하였습니다.");
            __debugbreak();
        }
        //
        static const char* column_parent_names[MAX_ACHIEVEMENT_PARENT_CODE] = {
            "ParentACCode1", "ParentACCode2", "ParentACCode3", "ParentACCode4",
        };
        for (int i = 0; i < _countof(column_parent_names); ++i)
        {
            row_info->parent_ac_code_[i] = GetDataWORD(column_parent_names[i], row);
        };
#endif //_NA_001990_ACSYSTEM_ADD
        //

        static const char* column_object_code_names[MAX_ACHIEVEMENT_OBJECT] = {
            "ObjectCode1", "ObjectCode2", "ObjectCode3", "ObjectCode4", "ObjectCode5",
            "ObjectCode6", "ObjectCode7", "ObjectCode8",
#ifdef _NA_001990_ACSYSTEM_ADD
                "ObjectCode9", "ObjectCode10",
#endif //_NA_001990_ACSYSTEM_ADD
        };
        static const char* column_object_num_names[MAX_ACHIEVEMENT_OBJECT] = {
            "ObjectNum1", "ObjectNum2", "ObjectNum3", "ObjectNum4", "ObjectNum5",
            "ObjectNum6", "ObjectNum7", "ObjectNum8",
#ifdef _NA_001990_ACSYSTEM_ADD
                "ObjectNum9", "ObjectNum10",
#endif //_NA_001990_ACSYSTEM_ADD
        };
       
        BOOST_STATIC_ASSERT(_countof(column_object_code_names) ==
                            _countof(column_object_num_names));
        for (int i = 0; i < _countof(column_object_code_names); ++i)
        {
            row_info->obejcts_[i].object_code_ = GetDataWORD(column_object_code_names[i], row);
            row_info->obejcts_[i].object_number_ = GetDataWORD(column_object_num_names[i], row);
        };
        //
        static const char* column_reward_type_names[MAX_ACHIEVEMENT_REWARD] = {
            "RewardType1", "RewardType2",
        };
        static const char* column_reward_code_names[MAX_ACHIEVEMENT_REWARD] = {
            "RewardCode1", "RewardCode2",
        };
        BOOST_STATIC_ASSERT(_countof(column_reward_type_names) ==
                            _countof(column_reward_code_names));
        for (int i = 0; i < _countof(column_reward_type_names); ++i)
        {
            Achievement_ParserInfo_Reward* const reward = &row_info->rewards_[i];
            reward->reward_type_ = GetDataWORD(column_reward_type_names[i], row);
            if (reward->reward_type_ == ACHIEVEMENT_REWARD_NONE) {
                continue;
            };
            ++row_info->rewards_count_;
            reward->reward_code_ = GetDataWORD(column_reward_code_names[i], row);
        };


        //
        AchievementSearch::Instance()->AddAchievementParserInfo(row_info);
        //
    }

    return TRUE;
}

Achievement_ParserInfo * AchievementParser::GetAchievement_ParserInfo( WORD key )
{
    return script_data_table_->GetData( key );
}

