#pragma once

#include <SolarHashTable.h>
#include <Singleton.h>
#include <SolarFileReader.h>

struct BASE_NPCImageInfo 
{
	WORD		wIndex;
	BYTE		bClassCode;
    DWORD       questAcceptAni;
    DWORD       questCompleteAni;
    DWORD       talkAni;
    BYTE        faceImageID;
    BYTE        hairImageID;
    SLOTCODE    AccCode1;
    SLOTCODE    AccCode2;
    SLOTCODE    AccCode3;
    int         Enchant;
    BYTE        showhelmet;
	SLOTCODE  	weaponCode;	
	SLOTCODE  	armorCode;	
	SLOTCODE  	protectCode;	
	SLOTCODE  	helmetCode;	
	SLOTCODE  	pantsCode;	
	SLOTCODE  	bootsCode;	
	SLOTCODE  	gloveCode;	
	SLOTCODE  	beltCode;	
	SLOTCODE  	shirtsCode;	
#ifdef _NA_008185_20150402_COSTUME_ITEM_FOR_NPC
    SLOTCODE    npc_costume_item;
#endif //_NA_008185_20150402_COSTUME_ITEM_FOR_NPC
};

class NPCImageInfoParser	: public Singleton<NPCImageInfoParser>, public IParser
{
public:
	NPCImageInfoParser();
	~NPCImageInfoParser();


	VOID					Init(DWORD dwPoolSize);
	VOID					Init(DWORD dwPoolSize, char * pszPackFileName);
	VOID					Release();

	virtual	BOOL            LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);

	BASE_NPCImageInfo*		GetNpcImageInfo(DWORD dwKey);

private:
	VOID					Unload();
	util::SolarHashTable<BASE_NPCImageInfo*>*   m_pDataTable;
};
