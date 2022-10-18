//------------------------------------------------------------------------------				    
//                   	
//	(C) 2005년 9월 24일 임대진
//
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include <SolarFileReader.h>
#include "HelpMessageParser.h"

HelpMessageParser::HelpMessageParser()
{
}

HelpMessageParser::~HelpMessageParser()
{
    if (help_message_data_.empty() == false)
    {
        Unload();
    }
}

VOID HelpMessageParser::Release()
{
	Unload();
}


VOID HelpMessageParser::Init(char * pack_filename)
{
    SetPackFileName(pack_filename);
}


BOOL HelpMessageParser::LoadScript(eSCRIPT_CODE script_code, BOOL is_reload)
{
    int row_size = GetRowSize();
    for (int row = 0; row < row_size; ++row)
    {
        HelpMessage* new_help_message = new HelpMessage;

        new_help_message->index = GetDataInt("Index", row);
		StrnCopy(new_help_message->help_message, GetDataString("HelpString", row), MAX_MESSAGE_LENGTH);
        if (GetHelpMessageByIndex(new_help_message->index) == NULL)
        {
		    help_message_data().push_back(new_help_message);
        }
	}

    ShuffleHelpMessage();

	return TRUE;
}


TCHAR *HelpMessageParser::GetHelpMessageByIndex(int Index)
{
	if (help_message_data().empty() == true)
	{
		return NULL;
	}

    HelpMessageVectorItr help_message_itr = help_message_data().begin();
    HelpMessageVectorItr help_message_itr_end = help_message_data().end();
    for ( ;help_message_itr != help_message_itr_end; ++help_message_itr)
    {
        if ((*help_message_itr)->index == Index)
        {
            return (*help_message_itr)->help_message;
        }
    }

	return NULL;
}

TCHAR* HelpMessageParser::GetHelpMessage()
{
    if (help_message_data().empty() == true)
    {
        return NULL;
    }

    HelpMessageVectorItr help_message_itr = current_help_message_itr();
    TCHAR* help_message = (*help_message_itr)->help_message;
    ++help_message_itr;
    set_current_help_message_itr(help_message_itr);

    if (current_help_message_itr() == help_message_data_.end())
    {
        ShuffleHelpMessage();
    }

    return help_message;
}

VOID HelpMessageParser::Unload()
{
    HelpMessageVectorItr help_message_itr = help_message_data().begin();
    HelpMessageVectorItr help_message_itr_end = help_message_data().end();
	for( ;help_message_itr != help_message_itr_end; ++help_message_itr)
	{
		delete (*help_message_itr);
	}

	help_message_data_.clear();
}

