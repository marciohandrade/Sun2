//------------------------------------------------------------------------------				    
#include "SunClientPrecompiledHeader.h"
#include "MessageText.h"
#include <SolarFileReader.h>
#include "GlobalFunc.h"

#include "Message.h"

//----------------------------------------------------------------------------                   	
/**                    
*/
MessageText::MessageText():	data_table_( NULL )
{
}

//----------------------------------------------------------------------------                   	
/**                    
*/
MessageText::~MessageText()
{
    assert(data_table_ == NULL);
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID MessageText::Release()
{
    Unload();
    delete data_table_;
    data_table_ = NULL;
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID MessageText::Init(DWORD pool_size , char *pack_file_name)
{
    assert(data_table_ == NULL);
    data_table_ = new util::SolarHashTable<_MessageText*>;
    data_table_->Initialize(pool_size);
    SetPackFileName(pack_file_name);
}
//----------------------------------------------------------------------------                   	
/**                    
*/
VOID MessageText::Init(DWORD pool_size)
{	
    assert(data_table_ == NULL);
    data_table_ = new util::SolarHashTable<_MessageText*>;
    data_table_->Initialize(pool_size);
}
//----------------------------------------------------------------------------                   	
/**                    
*/
VOID MessageText::Unload()
{
    _MessageText* message_text_Info = NULL;
    if (data_table_ == NULL)
    {
        return;
    }
    data_table_->SetFirst();
    while (message_text_Info = data_table_->GetNext())
    {
        delete [] message_text_Info->messge_text;
        delete message_text_Info;
    }
    data_table_->RemoveAll();
}

//----------------------------------------------------------------------------                   	
/**                    
*/
TCHAR* MessageText::GetMessageText(DWORD key)
{
    _MessageText* message_text_info = data_table_->GetData(key);
    return (message_text_info) ? data_table_->GetData(key)->messge_text: "";
}

#pragma warning (push)
#pragma warning (disable : 4244)
//----------------------------------------------------------------------------                   	
/**                    
*/
BOOL MessageText::LoadScript(eSCRIPT_CODE script_code, BOOL reload)
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
            WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_FILE,_T("[MessageText.txt] StrCode[%d] ม฿บน!\n"),
                string_code);
            assert(NULL == data_table_->GetData(string_code));
#endif//_DH_OS_LANGUAGETYPE
        }
        else
        {
            _MessageText* message_text_Info = new _MessageText;
            message_text_Info->messge_text_code = string_code;
            StrCopy(buffer_string, GetDataString("Str", row));
            message_text_Info->messge_text = new TCHAR[_tcslen(buffer_string)+1];
            StrCopy(message_text_Info->messge_text, buffer_string);

            data_table_->Add(message_text_Info, message_text_Info->messge_text_code);
            MessageInfoParser::Instance()->SetMessage(string_code, message_text_Info->messge_text);
        }
    }

    return TRUE;
}
#pragma warning ( pop )
