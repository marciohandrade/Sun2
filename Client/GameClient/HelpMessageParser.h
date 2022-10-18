#ifndef		HELPMESSAGEPARSER_H
#define		HELPMESSAGEPARSER_H

#include <SolarHashTable.h>
#include <Singleton.h>
#include <SolarFileReader.h>

class HelpMessageParser : public Singleton<HelpMessageParser>, public IParser
{
    struct HelpMessage
    {
        int index;
        TCHAR help_message[MAX_MESSAGE_LENGTH];

        HelpMessage()
        {
            index = 0;
            memset(help_message, '\0', sizeof(help_message));
        }
    };

    typedef std::vector<HelpMessage*> HelpMessageVector;
    typedef HelpMessageVector::iterator HelpMessageVectorItr;

public:
    HelpMessageParser();
    ~HelpMessageParser();

    void Init(char* pack_filename);
    void Release();
    virtual	BOOL LoadScript(eSCRIPT_CODE script_code, BOOL is_reload);
    TCHAR* GetHelpMessage();

private:
    void Unload();

    TCHAR* GetHelpMessageByIndex(int Index);
    void ShuffleHelpMessage();

    HelpMessageVector& help_message_data() { return help_message_data_; }
    HelpMessageVectorItr current_help_message_itr() { return current_help_message_itr_; }
    void set_current_help_message_itr(HelpMessageVectorItr replace_itr)
    {   
        current_help_message_itr_ = replace_itr;
    }

private:
	HelpMessageVector help_message_data_;
    HelpMessageVectorItr current_help_message_itr_;
};


inline void HelpMessageParser::ShuffleHelpMessage()
{
    std::random_shuffle(help_message_data().begin(), help_message_data().end());
    set_current_help_message_itr(help_message_data().begin());
}

#endif	//	HELPMESSAGEPARSER_H