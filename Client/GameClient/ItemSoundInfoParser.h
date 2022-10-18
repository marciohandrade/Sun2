#pragma once

#include <SolarHashTable.h>
#include <Singleton.h>

struct BASE_ItemSoundInfo 
{
     WORD     m_w_Code;
     char     m_sz_desc[256];
     DWORD    m_dw_PickDefault;
     DWORD    m_dw_PutDefault;
     DWORD    m_dw_PutWear;
     DWORD    m_dw_RemoveWear;
     DWORD    m_dw_LinkQslot;
     DWORD    m_dw_UnlinkQslot;
     DWORD    m_dw_Dump;

};

class ItemSoundInfoParser	: public Singleton<ItemSoundInfoParser> ,public IParser
{
public:
   ItemSoundInfoParser();
   ~ItemSoundInfoParser();


   VOID			        Init(DWORD dwPoolSize , char *szPackfileName);
   VOID			        Init(DWORD dwPoolSize);

   VOID			        Release();
   virtual	BOOL        LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);

   BASE_ItemSoundInfo*  GetItemSoundInfo(DWORD dwKey);

private:
    VOID		        Unload();
    util::SolarHashTable<BASE_ItemSoundInfo*>*  m_pDataTable;
};
