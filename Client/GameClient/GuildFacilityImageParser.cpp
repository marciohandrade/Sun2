#include "SunClientPrecompiledHeader.h"
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#include ".\guildfacilityimageparser.h"

GuildFacilityImageParser::GuildFacilityImageParser(void) 
: data_table_(NULL)
{
}

GuildFacilityImageParser::~GuildFacilityImageParser(void)
{
    assert( data_table_ == NULL );
}

VOID GuildFacilityImageParser::Release()
{
    Unload();
    if (data_table_)
        delete data_table_;
    data_table_ = NULL;
}

VOID GuildFacilityImageParser::Init( DWORD pool_size )
{
    assert( data_table_ == NULL );
    data_table_ = new util::SolarHashTable<BASE_GuildFacilityImageInfo*>;
    data_table_->Initialize( pool_size );
}

VOID GuildFacilityImageParser::Init( DWORD pool_size, char* pack_file_name )
{
    Init(pool_size);
    SetPackFileName( pack_file_name );
}

VOID GuildFacilityImageParser::Unload()
{
    BASE_GuildFacilityImageInfo* info_ptr = NULL;
    if (data_table_ == NULL)
    {
        return;
    }
    data_table_->SetFirst();
    while (info_ptr = data_table_->GetNext() )
    {
        delete info_ptr;
    }
    data_table_->RemoveAll();
}

BASE_GuildFacilityImageInfo* GuildFacilityImageParser::GetGuildFacilityImageInfo( DWORD _key_order )
{
    return data_table_->GetData( _key_order );
}

#pragma warning ( push )
#pragma warning ( disable : 4244 )

BOOL GuildFacilityImageParser::LoadScript( eSCRIPT_CODE script_code, BOOL bReload )
{
    int row_size = GetRowSize();
    for (int row_count = 0; row_count < row_size; ++row_count)
    {
        BASE_GuildFacilityImageInfo* info_ptr = new BASE_GuildFacilityImageInfo;

        info_ptr->index_ =                     GetDataNumber("Index", row_count);
        info_ptr->facility_order_ =            GetDataNumber("facility_order", row_count);
        info_ptr->facility_name_code_ =        GetDataNumber("facility_name", row_count);
        info_ptr->facility_explanation_code_ = GetDataNumber("facility_name2", row_count);
        info_ptr->x_ =                         GetDataNumber("X", row_count);
        info_ptr->y_ =                         GetDataNumber("Y", row_count);
        info_ptr->width_ =                     GetDataNumber("Width", row_count);
        info_ptr->height_ =                    GetDataNumber("Height", row_count);

        assert(NULL == data_table_->GetData(info_ptr->index_));        
        data_table_->Add(info_ptr, info_ptr->facility_order_);
    }

    return TRUE;
}

#pragma warning ( pop )
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION