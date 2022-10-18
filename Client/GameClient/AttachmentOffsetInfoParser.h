#pragma once

#include <SolarHashTable.h>
#include <Singleton.h>


struct BASE_AttachmentOffsetInfo 
{
     DWORD     m_dwItemCode;

     float pos_x_;
     float pos_y_;
     float pos_z_;
     float rot_x_;
     float rot_y_;
     float rot_z_;
};


class AttachmentOffsetInfoParser	: public Singleton<AttachmentOffsetInfoParser> ,public IParser
{
public:
   AttachmentOffsetInfoParser();
   ~AttachmentOffsetInfoParser();


   VOID				                Init(DWORD dwPoolSize , char *szPackfileName);
   VOID				                Init(DWORD dwPoolSize);

   VOID				                Release();
   

private:
	VOID			                Unload();
    BOOL			                _Load(BOOL bReload);

	util::SolarHashTable<BASE_AttachmentOffsetInfo *>*      m_pDataTable;



public:
	virtual	BOOL	                LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);
	BASE_AttachmentOffsetInfo*		GetAttachmentOffsetInfo(DWORD dwKey);
	
};
