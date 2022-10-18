//------------------------------------------------------------------------------				    
#include "SunClientPrecompiledHeader.h"

#include "GuideMessageParser.h"
#include <SolarFileReader.h>
#include "GlobalFunc.h"


//----------------------------------------------------------------------------                   	
/**                    
*/
GuideMessageParser::GuideMessageParser():	data_table_( NULL )
{
}

//----------------------------------------------------------------------------                   	
/**                    
*/
GuideMessageParser::~GuideMessageParser()
{
    assert(data_table_ == NULL);
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID GuideMessageParser::Release()
{
    Unload();
    delete data_table_;
    data_table_ = NULL;
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID GuideMessageParser::Init(DWORD pool_size , char *pack_file_name)
{
    assert(data_table_ == NULL);
    data_table_ = new util::SolarHashTable<GUIDE_MESSAGE*>;
    data_table_->Initialize(pool_size);
    SetPackFileName(pack_file_name);
}
//----------------------------------------------------------------------------                   	
/**                    
*/
VOID GuideMessageParser::Init(DWORD pool_size)
{	
    assert(data_table_ == NULL);
    data_table_ = new util::SolarHashTable<GUIDE_MESSAGE*>;
    data_table_->Initialize(pool_size);
}
//----------------------------------------------------------------------------                   	
/**                    
*/
VOID GuideMessageParser::Unload()
{
    GUIDE_MESSAGE* guide_message_Info = NULL;
    if (data_table_ == NULL)
    {
        return;
    }
    data_table_->SetFirst();
    while (guide_message_Info = data_table_->GetNext())
    {
        delete [] guide_message_Info->guide_messge_text;
        delete guide_message_Info;
    }
    data_table_->RemoveAll();
}

//----------------------------------------------------------------------------                   	
/**                    
*/
TCHAR* GuideMessageParser::GetGuideMessage(DWORD key)
{
    GUIDE_MESSAGE* guide_message_Info = data_table_->GetData(key);
    return (guide_message_Info) ? data_table_->GetData(key)->guide_messge_text: "ID not Found";
}

#pragma warning (push)
#pragma warning (disable : 4244)
//----------------------------------------------------------------------------                   	
/**                    
*/
BOOL GuideMessageParser::LoadScript(eSCRIPT_CODE script_code, BOOL reload)
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
            WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_FILE,_T("[LibraryText.txt] StrCode[%d] ม฿บน!\n"), 
                string_code);
            assert(NULL == data_table_->GetData(string_code));
#endif//_DH_OS_LANGUAGETYPE
        }
        else
        {
            GUIDE_MESSAGE* guide_message_Info = new GUIDE_MESSAGE;
            guide_message_Info->guide_messge_code = string_code;
            StrCopy(buffer_string, GetDataString("Str", row));
            guide_message_Info->guide_messge_text = new TCHAR[_tcslen(buffer_string)+1];
            StrCopy(guide_message_Info->guide_messge_text, buffer_string);

            data_table_->Add(guide_message_Info, guide_message_Info->guide_messge_code);
        }
    }

    return TRUE;
}
#pragma warning ( pop )

