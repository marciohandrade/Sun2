#pragma once

#include <SolarHashTable.h>
#include <Singleton.h>

struct _MessageText 
{
     DWORD		messge_text_code;
     TCHAR*		messge_text;
};

class MessageText	: public Singleton<MessageText> ,public IParser
{
public:
	MessageText();
	~MessageText();

	VOID Init(DWORD pool_size);
	VOID Init(DWORD pool_size , char* pack_file_name);
	VOID Release();
	virtual BOOL LoadScript(eSCRIPT_CODE script_code, BOOL reload);
	TCHAR* GetMessageText(DWORD key);

private:
	VOID		   Unload();
	util::SolarHashTable<_MessageText*>* data_table_;
};
