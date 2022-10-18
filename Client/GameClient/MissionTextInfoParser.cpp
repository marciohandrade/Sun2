//------------------------------------------------------------------------------				    
//                   	
//	(C) 2005 y2jinc					
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "MissionTextInfoParser.h"
#include <SolarFileReader.h>
#include "GlobalFunc.h"

//----------------------------------------------------------------------------                   	
/**                    
*/
MissionTextInfoParser::MissionTextInfoParser():	data_table_( NULL )
{
}

//----------------------------------------------------------------------------                   	
/**                    
*/
MissionTextInfoParser::~MissionTextInfoParser()
{
	assert(data_table_ == NULL);
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID MissionTextInfoParser::Release()
{
	Unload();
	delete data_table_;
	data_table_ = NULL;
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID MissionTextInfoParser::Init(DWORD pool_size , char *pack_file_name)
{
	assert(data_table_ == NULL);
	data_table_ = new util::SolarHashTable<MissionTextInfo*>;
	data_table_->Initialize(pool_size);
	SetPackFileName(pack_file_name);
}
//----------------------------------------------------------------------------                   	
/**                    
*/
VOID MissionTextInfoParser::Init(DWORD pool_size)
{	assert(data_table_ == NULL);
	data_table_ = new util::SolarHashTable<MissionTextInfo*>;
	data_table_->Initialize(pool_size);
}
//----------------------------------------------------------------------------                   	
/**                    
*/
VOID MissionTextInfoParser::Unload()
{
	MissionTextInfo* mission_text_Info = NULL;
	if (data_table_ == NULL)
	{
		return;
	}
	data_table_->SetFirst();
	while (mission_text_Info = data_table_->GetNext())
	{
		delete [] mission_text_Info->mission_text;
		delete mission_text_Info;
	}
	data_table_->RemoveAll();
}

//----------------------------------------------------------------------------                   	
/**                    
*/
MissionTextInfo* MissionTextInfoParser::GetMissionTextInfo(DWORD key)
{
	return data_table_->GetData(key);
}

#pragma warning ( push )
#pragma warning ( disable : 4244)
//----------------------------------------------------------------------------                   	
/**                    
*/
BOOL MissionTextInfoParser::LoadScript(eSCRIPT_CODE script_code, BOOL reload)
{
	TCHAR buffer_string[10240];
	int row_size = GetRowSize();
    DWORD string_code = 0;
	for (int row = 0; row < row_size; ++row)
	{
        string_code = GetDataNumber("StrCode", row);

		if (NULL != data_table_->GetData(string_code))
		{
#ifdef _DH_OS_LANGUAGETYPE
#else
			WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_FILE,_T("[MissionText.txt] StrCode[%d] ม฿บน!\n"), 
                string_code);
			assert(NULL == data_table_->GetData(string_code));
#endif//_DH_OS_LANGUAGETYPE
		}
        else
        {
            MissionTextInfo* mission_text_info = new MissionTextInfo;
            mission_text_info->mission_text_code = string_code;
            StrCopy(buffer_string, GetDataString("Str", row));
            mission_text_info->mission_text = new TCHAR[_tcslen(buffer_string)+1];
            StrCopy(mission_text_info->mission_text, buffer_string);

            data_table_->Add(mission_text_info, mission_text_info->mission_text_code);
        }
	}

	return TRUE;
}
#pragma warning ( pop )
